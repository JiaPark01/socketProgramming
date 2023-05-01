#include <stdio.h>
#include <unistd.h> // fork()

int main()
{
    pid_t pid = fork();
    
    if (pid == 0) {
        // child proc
        puts("Hello, this is a child process\r\n");
    } else {
        // parent proc
        printf("The child process PID : %d\r\n", pid);
        sleep(30);
    }

    if (pid == 0) {
        puts("End child proc\r\n");
    } else {
        puts("End parent proc\r\n");
    }


    return 0;
}
