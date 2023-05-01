#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, const char* argv[])
{
    struct hostent* host = NULL;

    if (argc != 2) {
        fputs("GET_HOST_BY_NAME www.daum.net\r\n", stdout);
        exit(1);
    }

    host = gethostbyname(argv[1]);    // "www.daum.net"
    if (host == NULL) {
        puts("Host not found...\r\n");
    }

    printf("Official name: %s\r\n", host->h_name);
    for (int i = 0; host->h_aliases[i]; ++i) {
        printf("Aliases %d :\t %s\r\n", i + 1, host->h_aliases[i]);
    }
    printf("Address type : %d\r\n", host->h_addrtype);
    printf("Length : %ubytes\r\n", host->h_length);
    for (int i = 0; host->h_addr_list[i]; ++i) {
        printf("IP Address %d :\t %s\r\n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));   // nw to ascii
    }


    return 0;
}
