#include <stdio.h>
#include <arpa/inet.h>

int main()
{
    const char* str1 = "1.2.3.4";
    const char* str2 = "1.23.4.256";    // originally IPv4 has 0~255, so this can't be an address

    unsigned long convert_value = inet_addr(str1);
    
    if (convert_value == INADDR_NONE) {   // -1
        puts("Convert Error\r\n");
    } else {
        printf("Convert value : 0x%lx\r\n", convert_value);
    }

    convert_value = inet_addr(str2);

    if (convert_value == INADDR_NONE) {   // -1
        puts("Convert Error\r\n");
    } else {
        printf("Convert value : 0x%lx\r\n", convert_value);
    }

    return 0;
}
