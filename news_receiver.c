#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>     // uint8_t
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define TTL 64

void error_handling (const char*);

int main(int argc, const char* argv[])
{
    int recv_sock = 0;
    int str_length = 0;
    char buffer[BUFSIZ] = {'\0', };
    struct sockaddr_in serv_addr;
    struct ip_mreq join_addr;       // struct for Grouping

    if (argc != 3) error_handling("NEWS_RECEIVER <GroupIP> 9999");
    
    // 1. socket()
    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (recv_sock == -1) error_handling("socket() error");
    
    // setting addr
    memset(&serv_addr, 0, sizeof serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // receiver -> receive from any IP
    serv_addr.sin_port = htons(atoi(argv[2]));
    
    // 2. bind()
    if (bind(recv_sock, (const struct sockaddr*)&serv_addr, sizeof serv_addr) == -1) error_handling("bind() error");
    
    // grouping
    join_addr.imr_multiaddr.s_addr = inet_addr(argv[1]);    // GROUP IP
    join_addr.imr_interface.s_addr = htonl(INADDR_ANY);      // PORT

    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_addr, sizeof join_addr);

    // print
    while (true) {
        str_length = recvfrom(recv_sock, buffer, BUFSIZ-1, 0, NULL, 0);
        if (str_length < 0) break;
        buffer[str_length] = '\0';
        fputs(buffer, stdout);
    }
    close(recv_sock);

    return 0;
}

void error_handling(const char* _msg)
{
    fputs(_msg, stderr);
    fputs("\r\n", stderr);

    exit(1);

    return;
}
