#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/types.h>

//#define NAME_SIZE 20
void* send_msg(void*);
void* receive_msg(void*);
void error_handling(const char*);

//char name[NAME_SIZE] = {'\0', };
char message[BUFSIZ] = {'\0', };

int main(int argc, const char* argv[])
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    pthread_t send_thread = 0ul;
    pthread_t receive_thread = 0ul;
    void* thread_return = NULL;     // NULL again when threads terminate
    
    if (argc != 3) error_handling("./MULTI_THREAD_CHAT_CLIENT serverIP 9999");
    //sprintf(name, "[%s]", argv[3]); // save nickname in [NAME] format in name var

    // 1. socket()
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) error_handling("socket() error");

    memset(&serv_addr, 0, sizeof serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // 2. connect()
    if (connect(sock, (const struct sockaddr*)&serv_addr, sizeof serv_addr) == -1) error_handling("connect() error");
    
    pthread_create(&send_thread, NULL, send_msg, (void*)&sock);
    pthread_create(&receive_thread, NULL, receive_msg, (void*)&sock);
    
    pthread_join(send_thread, &thread_return);  // save returned NULL to thread_return
    pthread_join(receive_thread, &thread_return);   // or can just write NULL instead

    close(sock);

    return 0;
}

void* send_msg(void* args)
{
    int sock = *((int*)args);
    //char name_msg[NAME_SIZE + BUFSIZ] = {'\0', };
    while(true) {
        fgets(message, BUFSIZ, stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            close(sock);
            fputs("Bye\r\n", stdout);
            exit(1);
        }
        //sprintf(name_msg, "%s\t: %s", name, message);
        write(sock, message, strlen(message));
    }

    return NULL;
}

void* receive_msg(void* args)
{
    int sock = *((int*)args);
    //char name_msg[NAME_SIZE + BUFSIZ] = {'\0', };
    int str_len = 0;
    while (true) {
        if ((str_len = read(sock, message,BUFSIZ - 1)) == -1) return NULL;
        //name_msg[str_len] = '\0';
        fputs(message, stdout);
    }
    
    return NULL;
}

void error_handling(const char* _msg)
{
    fputs(_msg, stderr);
    fputs("\r\n", stderr);

    exit(1);

    return;
}
