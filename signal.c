#include <stdio.h>
#include <unistd.h>
#include <signal.h>
// signal (int, func);
// calls func when O/S event occurs

void timeout(int _signal)
{
    if (_signal == SIGALRM) {  // 14
        puts("Time out!!\r\n");
    }
    alarm(2);   // rings after 2 sec
}

void keycontrol(int _signal) {
    if (_signal == SIGINT) {
        puts("Ctrl + c pressed...\r\n");
    }
}

int main()
{
    signal(SIGALRM/*14*/, timeout);     // wakes up from sleep. releases CPU from the for loop
    signal(SIGINT/*2*/, keycontrol);
    alarm(2);

    for (int i = 0; i < 3; ++i) {
        puts("wait ... \r\n");
        sleep(100);
    }

    return 0;
}
