#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, const char* argv[])
{
    int sock = 0;
    char message[BUFSIZ] = {'\0', };
    int str_length = 0;
    int receive_length = 0;
    int receive_count = 0;
    struct sockaddr_in serv_addr;
    
    if (argc != 3) {
        puts("ECHO_CLIENT2 127.0.0.1 9999\r\n");
        exit(1);
    }
    
    // 1. socket()
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        puts("socket() func error\r\n");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;     // IPv4
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // 127.0.0.1    --> 32bit 정수값
    serv_addr.sin_port = htons(atoi(argv[2]));      // 9999         --> 16bit 정수값

    // 2. connect()
    if (connect(sock, (const struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        puts("connect() func error\r\n");
        exit(1);
    } else {
        puts("Connected ...\r\n");
    }

    while(1) {
        fputs("Input your message (Q to quit) : ", stdout);  // stdout --> 2 (file descriptor)
        fgets(message, BUFSIZ, stdin);  // save input string as message
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            puts("Good bye\r\n");
            break;
        }
        str_length = write(sock, message, strlen(message)); // returns str len
        receive_length = 0;

        while(str_length > receive_length) {
            receive_count = read(sock, &(message[receive_length]), BUFSIZ - 1); // receive a letter by letter
            if (receive_count == -1) puts("Error in reading\r\n");
            receive_length += receive_count;
        }
        message[receive_length] = '\0';
        fprintf(stdout, "Message from Server : %s\r\n", message);
    }
    close(sock);

    return 0;
}
