#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int status = 0;
    pid_t pid = fork(); // copy & make child proc
    if (pid == 0) {
        return 3;       // send error code 3 despite successful termination
    } else {
        printf("Child PID : %d\r\n", pid);
        pid = fork();   // another child proc
        if (pid == 0) {
            exit(7);    // send error code 7 along with abnormal termination
        } else {
            printf("Child PID : %d\r\n", pid);
            wait(&status);
            if (WIFEXITED(status)) {    // WIFEXITED() macro -> check for successful termination of child process
                printf("Child send one : %d \r\n", WEXITSTATUS(status));
                // if successfully terminated, printf the error code of child process termination
            }
            wait(&status);
            if (WIFEXITED(status)) {
                printf("Child send two : %d \r\n", WEXITSTATUS(status));
            }
            sleep(30);  // wait to give time to read results
        }
    }

    return 0;
}
