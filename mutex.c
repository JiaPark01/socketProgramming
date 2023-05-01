// copied from thread4.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void* thread_increment(void*);
void* thread_decrement(void*);

long long number = 0LL;
#define MAX_THREAD 100
pthread_mutex_t mutex_key;

int main()
{
    pthread_t pt_id[MAX_THREAD] = {0ul, };
    printf("SIZEOF LONG LONG : %ld\r\n", sizeof (long long));
    
    pthread_mutex_init(&mutex_key, NULL);

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
    
    pthread_mutex_destroy(&mutex_key);

    return 0;
}

void* thread_increment(void* args)
{
    for (int i = 0; i < 5000000; ++i) {
        pthread_mutex_lock(&mutex_key);
        number += 1;
        pthread_mutex_unlock(&mutex_key);
    }

    return NULL;
}

void* thread_decrement(void* args)
{
    for (int i = 0; i < 5000000; ++i) {
        pthread_mutex_lock(&mutex_key);
        number -= 1;
        pthread_mutex_unlock(&mutex_key);
    }

    return NULL;
}
