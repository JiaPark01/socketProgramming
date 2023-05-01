#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

// task func
void* thread_main(void*);

int main()
{
    pthread_t pt_id = 0ul;
    int thread_param = 10;
    void* thread_return = NULL;
    // val when thread_main task finishes
    if (pthread_create((&pt_id), NULL, thread_main, (void*)&thread_param) != 0) {
        fputs("Thread create() error\r\n", stderr);
        exit(1);
    }
    
    if (pthread_join(pt_id, &thread_return) != 0) {
        fputs("pthread_join() error", stderr);
        exit(1);
    }

    fprintf(stdout, "Returned message : %s\r\n", (const char*)thread_return);
    
    free(thread_return);    // free heap mem
    thread_return = NULL;

    return 0;
}

void* thread_main(void* args)
{
    int param = *((int*)args);
    char* message = (char*)malloc(sizeof(char)* 50);    // 50 bytes
    strcpy(message, "Hello~ I'm thread\r\n");
    for (int i = 0; i < param; ++i) {
        sleep(1);
        puts("I am Thread..\r\n");
    }

    return (void*)message;
}
