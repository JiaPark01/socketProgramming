#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void* thread_increment(void*);
void* thread_decrement(void*);

long long number = 0LL;
#define MAX_THREAD 100

int main()
{
    pthread_t pt_id[MAX_THREAD] = {0ul, };
    printf("SIZEOF LONG LONG : %ld\r\n", sizeof (long long));
    
    for (int i = 0; i < 100; ++i) {
        if (i % 2) {
            pthread_create(&(pt_id[i]), NULL, thread_increment, NULL);
        } else {
            pthread_create(&(pt_id[i]), NULL, thread_decrement, NULL);

        }
    }

    for (int i = 0; i < 100; ++i) {
        pthread_join(pt_id[i], NULL);
    }

    fprintf(stdout, "Result = %ld\r\n", number);

    return 0;
}

void* thread_increment(void* args)
{
    for (int i = 0; i < 5000000; ++i) {
        number += 1;
    }

    return NULL;
}

void* thread_decrement(void* args)
{
    for (int i = 0; i < 5000000; ++i) {
        number -= 1;
    }

    return NULL;
}
