#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* thread_main(void* args)
{
    int param = *((int*)args);      // go to the addr of args
    for (int i = 0; i < param; ++i) {
        sleep(1);
        puts("I'm thread.\r\n");
    }

    return NULL;
}

int main()
{
    pthread_t pt_id = 0u;
    int thread_param = 10;
    
    int state = pthread_create(&pt_id/*created by the os*/, NULL, thread_main/*func ptr*/, (void*)&thread_param);

    if (state != 0) {
        fputs("Thread error", stderr);
        exit(1);
    }
    fputs("Main goes to sleep\r\n", stdout);
    sleep(20);

    return 0;
}
