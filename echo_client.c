#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

void error_handling(const char*);

int main (int argc, const char* argv[])
{
    int sock = 0;   // serv와 연결을 위한 FD
    char message[BUFSIZ] = {'\0', };
    int str_length = 0;
    struct sockaddr_in serv_addr;

    if (argc != 3) {
        puts("ECHO_CLIENT 127.0.0.1 9999");
        exit(1);
    }
    // 1. socket()
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() 생성 오류");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));   // 0으로 전부 초기화
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    
    // 2. connect()
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("connect() 함수 오류");
    } else {
        puts("Connected ..... \r\n");
    }

    while(1) {
        fputs("Input your message (Q to quit) : ", stdout);
        fgets(message, BUFSIZ, stdin);
        
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }
        write(sock, message, BUFSIZ - 1);
        str_length = read(sock, message, BUFSIZ - 1);
        message[str_length] = 0;    // insert null
        printf("Message from server : %s\r\n", message);
    }

    close(sock);

    return 0;
}

void error_handling(const char* _message)
{
    fputs(_message, stdout);
    fputs("\r\n", stdout);
    exit(1);

    return;
}
