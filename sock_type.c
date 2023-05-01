#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

int main(int argc, const char* argv[])
{
    int tcp_socket = 0;
    int udp_socket = 0;
    socklen_t option_len;   // leave it to gcc compiler. read-only
    int state = 0;

    // 1. socket()
    tcp_socket = socket(PF_INET, SOCK_STREAM, 0);
    udp_socket = socket(PF_INET, SOCK_DGRAM, 0);

    printf("SOCK_STREAM : %d\r\n", SOCK_STREAM);    // 1
    printf("SOCK_DGRAM  : %d\r\n", SOCK_DGRAM);     // 2
    
    int socket_type;        // leave it to gcc
    state = getsockopt(tcp_socket, SOL_SOCKET, SO_TYPE, (void*)&socket_type, (socklen_t*)&option_len);
    if (state) printf("Error\r\n");

    printf("TCP socket_type : %d \t option_len : %d\r\n", socket_type, option_len);

    getsockopt(udp_socket, SOL_SOCKET, SO_TYPE, (void*)&socket_type, (socklen_t*)&option_len);
  
    printf("UDP socket_type : %d \t option_len : %d\r\n", socket_type, option_len);


    return 0;
}
