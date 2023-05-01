#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(const char*);

int main(int argc, const char* argv[])
{
    int sock = 0;
    char message[BUFSIZ] = {'\0', };
    ssize_t str_length = 0u;
    socklen_t addr_size = 0u;
    struct sockaddr_in serv_addr;
    struct sockaddr_in from_addr;   // dummy 충돌 방지용

    if (argc != 3) {
        error_handling("UDP_ECHO_CLIENT 127.0.0.1 9999");
    }

    // 1. socket()
    sock = socket(PF_INET, SOCK_DGRAM/*UDP*/, 0);
    if (sock == -1) {
        error_handling("socket() func error");
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&from_addr, 0, sizeof(from_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // "127.0.0.1"
    serv_addr.sin_port = htons(atoi(argv[2]));

    while(1) {
        fputs("Input your message (Q to quit) : ", stdout);
        fgets(message, BUFSIZ, stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            puts("Good Bye\r\n");
            break;
        }
        // connect() X in UDP
        sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        addr_size = sizeof(from_addr);  // dummy
        str_length = recvfrom(sock, message, BUFSIZ, 0, (struct sockaddr*)&from_addr, (socklen_t*)&addr_size);
        message[str_length] = '\0';     // doesn't matter where it receives from, so use an empty IP to prevent crashes. dummy remains as 0 from memset

        fprintf(stdout, "Message from Server : %s\r\n", message);
    }
    
    close(sock);

    return 0;
}

void error_handling(const char* _msg)
{
    fputs(_msg, stdout);
    fputs("\r\n", stdout);

    exit(1);
    
    return;
}
