#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define TTL 64

void error_handling(const char*);

/*
 * Server -> sender
 * Receiver -> client
*/

int main(int argc, const char* argv[])
{
    int send_sock = 0;  // var for socket()
    int time_live = TTL;
    /*
     * send HDD File to clients
     * FILE struct: address of HDD
     * fp
     */
    FILE* fp = NULL;
    char buffer[BUFSIZ] = {'\0', };
    struct sockaddr_in multi_addr;

    memset(&multi_addr, 0, sizeof multi_addr);

    if (argc != 3) error_handling("NEWS_SENDER 192.168.56.1 9999"); // 3 arg unlike other server

    // 1. socket()
    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (send_sock == -1) error_handling("socket() error");

    multi_addr.sin_family = AF_INET;
    multi_addr.sin_addr.s_addr = inet_addr(argv[1]);    // 192. ... <- 32 bit
    multi_addr.sin_port = htons(atoi(argv[2]));  // 16 bit

    // 2. setsockopt()  -- set TTL
    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live/*TTL*/, sizeof time_live);

    if ((fp = fopen("news.txt", "r")) == NULL) error_handling("open() error");
    while (!feof(fp)) {
        fgets(buffer, BUFSIZ, fp);  //stdin -- keyboard, fp -- file("news.txt")
        // 3. sendto()
        sendto(send_sock, buffer, strlen(buffer), 0, (struct sockaddr*)&multi_addr, sizeof multi_addr);
        sleep(2);
    }

    fclose(fp);
    close(send_sock);

    return 0;
}

void error_handling(const char* _msg)
{
    // stderr -> 2
    fputs(_msg, stderr);
    fputs("\r\n", stderr);

    exit(1);

    return;
}
