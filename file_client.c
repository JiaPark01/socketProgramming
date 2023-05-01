#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void error_handling(const char*);

int main(int argc, const char* argv[])
{
    int sock_sd = 0;
    FILE* fp = fopen("server.dat", "wb");   // save the received file as server.dat
    char buf[BUFSIZ] = {'\0', };
    int read_count = 0;
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    if (argc != 3) {
        error_handling("FILE_CLIENT 127.0.0.1 9999");
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    
    // 1. socket()
    sock_sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sock_sd == -1) error_handling("socket() func error");

    // 2. connect()
    if (connect(sock_sd, (const struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("connect() func error");
    }

    while ((read_count = read(sock_sd, buf, BUFSIZ)) != 0) {
        fwrite((void*)buf, 1, read_count, fp);      // 1byte 씩
    }
    puts("A file received..\r\n");
    fclose(fp);
    close(sock_sd);

    return 0;
}

void error_handling(const char* _msg)
{
    fputs(_msg, stdout);
    fputs("\r\n", stdout);

    exit(1);

    return;
}
