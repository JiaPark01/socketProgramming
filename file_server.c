#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

static void error_handling(const char*);        // private to this .c

int main(int argc, const char* argv[])
{
    int serv_sd = 0;
    int clnt_sd = 0;
    FILE* fp = NULL;
    char buf[BUFSIZ] = {'\0', };
    int read_count = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&clnt_addr, 0, sizeof(clnt_addr));

    socklen_t clnt_size = 0;

    if (argc != 2) {
        error_handling("FILE_SERVER 9999");
    }
    
    fp = fopen("file_server.c", "rb");  // open file from HDD

    // 1. socket()
    serv_sd = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sd == -1) {
        error_handling("socket() func error");
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    // 2. bind()
    if (bind(serv_sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() func error");
    }
    // 3. listen()
    if (listen(serv_sd, 5) == -1) error_handling("listen() func error");
    clnt_size = sizeof(clnt_addr);
    clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_size);
    
    while(1) {
        read_count = fread((void*)buf, 1, BUFSIZ, fp);  // calc byte using fp
        if (read_count < BUFSIZ) {  // if larger than BUFSIZ keep sending it
            write(clnt_sd, buf, read_count);
            break;
        }
        write(clnt_sd, buf, BUFSIZ);
    }
    shutdown(clnt_sd, SHUT_WR);     // shuts down after writing all
    
    read(clnt_sd, buf, BUFSIZ);
    fprintf(stdout, "Message from client : %s\r\n", buf);

    fclose(fp);
    close(clnt_sd);
    close(serv_sd);

    return 0;
}

void error_handling(const char* _msg)
{
    fputs(_msg, stdout);
    fputs("\r\n", stdout);
    
    exit(1);

    return;
}
