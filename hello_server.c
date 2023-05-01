#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // linux library
#include <arpa/inet.h>  // arpa lib
#include <sys/socket.h> // socket lib

void error_handling(const char* );  // func prototype

int main(int argc, const char* argv[])
{
    int serv_sock = 0;
    int clnt_sock = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = 0u;
    const char message[] = {"Hello world!\n"};
    
    if (argc != 2) {
        fprintf(stdout, "Usage : %s PORT \r\n", argv[0]);
        exit(1);
    }

    // 1. socket()
    serv_sock = socket(PF_INET /*IPv4*/, SOCK_STREAM/*TCP*/, 0); // bridge

    if (serv_sock == -1) {
        error_handling("socket() error");
        exit(1);
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));   // struct sockaddr_in intialising

    serv_addr.sin_family = AF_INET;             // IPv4 (same as PF_INET but just diff name)
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // loop back? virtual server/IP
    serv_addr.sin_port = htons(atoi(argv[1]));      // ASCII to int

    // 2. bind
    int bind_state = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));    // bridge, address(needs typecasting as sockaddr != sockaddr_in), size

    if (bind_state == -1) {
        error_handling("bind() error");
        exit(1);
    }

    // 3. listen()
    int listen_state = listen(serv_sock, 5);  // bridge, num of times to listen??

    if (listen_state == -1) {
        error_handling("listen() error");
        exit(1);
    }

    // 4. accept()
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&serv_addr, &clnt_addr_size);  // bridge, sockaddr, addrlen
    
    if (clnt_sock == -1) {
        error_handling("accept() error");
        exit(1);
    }

    write(clnt_sock, message, sizeof(message));
    close(clnt_sock);
    close(serv_sock);

    
    return 0;
}

void error_handling(const char* _message)
{
    fputs(_message, stdout);
    fputs("\r\n", stdout);

    return;
}
