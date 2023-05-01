#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(const char* );

int main(int argc, const char* argv[])
{
    int serv_sock = 0;
    int clnt_sock = 0;
    char message[BUFSIZ] = {'\0', };
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = 0u;
    
    if (argc != 2) {
        puts("ECHO_SERVER 9999");
        exit(1);
    }

    // 1. socket()
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    // serv_sock FD
    if (serv_sock == -1) {
        error_handling("socket 생성 오류");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    // set IP & PORT
    serv_addr.sin_family = AF_INET;     // IPv4
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 127.0.0.1 + MY IP
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    // 2. bind()
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() 오류");
    }

    // 3. listen()
    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() 오류");
    }
    
    // 대기 중에 있다가 클라이언트가 접근
    // 4. accept()
    clnt_addr_size = sizeof(clnt_addr); // 재 초기화 0u;
    int str_len = 0;
    
    for (int i = 0; i < 5; ++i) {
        clnt_sock /*client fd*/ = accept(serv_sock, (struct sockaddr*)&serv_addr, &clnt_addr_size);
        if (clnt_sock == -1) {
            error_handling("clnt_sock - accept() 오류");
        } else {
            printf("Connected client %d\r\n", i + 1);
            while((str_len = read(clnt_sock, message, BUFSIZ)) != 0) {
                write(clnt_sock, message, str_len);  // or sizeof(message) or BUFSIZ
            }
            close(clnt_sock);
        }
    }
    close(serv_sock);

    return 0;
}

void error_handling(const char* _message)
{
    fputs(_message, stdout);
    fputs("\r\n", stdout);
    exit(1);
    return;
}
