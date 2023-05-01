#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int _signal)
{
    if (_signal == SIGALRM) {
        puts("Time out \r\n");
    }
    alarm(2);
}

int main()
{
    struct sigaction act;
    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, 0);
    alarm(2);

    for (int i = 0; i < 3; ++i) {
        puts("wait... \r\n");
        sleep(100);
    }


    return 0;
}
