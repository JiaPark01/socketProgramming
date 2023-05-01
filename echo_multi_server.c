#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>    // allows true/false
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>     // signal()
#include <sys/wait.h>   // wait()
#include <string.h>

void error_handling(const char*);
void read_childproc(int);

int main(int argc, const char* argv[])
{
    pid_t pid;  // var to allocate child process id
    int serv_sock = 0;
    int clnt_sock = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    struct sigaction act;       // use struct to control signal
    socklen_t addr_size;
    int str_length = 0;
    bool state = false;
    char buf[BUFSIZ] = {'\0', };

    if (argc != 2) {
        error_handling("ECHO_MULTI_SERVER 9999");
    }

    act.sa_handler = read_childproc;    // set func
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);    // calls when child process terminates
    if (state == -1) {
        error_handling("sigaction() func error");
    }
    // 1. socket()
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket() func error");
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&clnt_addr, 0, sizeof(clnt_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    // 2. bind()
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) error_handling("bind() func error");

    // 3. listen()
    if (listen(serv_sock, 5) == -1) error_handling("listen() func error");
    
    // 4. accept()
    while (true) {
        addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*)&addr_size);
        if (clnt_sock == -1) continue;  // despite client error, continue and wait for the next client
        else puts("New client connected ...\r\n");
        // create clones
        pid = fork();
        if (pid == -1) {
            close(clnt_sock);
            continue;

        }
        // child proc codes
        if (pid == 0) {
            close(serv_sock);       // close one because two serv_sock are created due to fork();
            while((str_length = read(clnt_sock, buf, BUFSIZ)) != 0) {
                write(clnt_sock, buf, str_length);  // send to client
            }
            close(clnt_sock);
            puts("Client disconnected ...\r\n");
            return 0;
        } else close(clnt_sock);    // from parent proc
    }


    return 0;
}

void error_handling(const char* _msg)
{
    fputs(_msg, stdout);
    fputs("\r\n", stdout);
    
    exit(1);

    return;
}

// checks whether child proc terminated successfully
void read_childproc(int _signal)
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("Removed process id : %d\r\n\n", pid);

    return;
}
