#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

void* reading(void*);
void* accumulate(void*);

int number = 0;
sem_t sem_one;
sem_t sem_two;

int main()
{
    pthread_t pt_id1 = 0ul;
    pthread_t pt_id2 = 0ul;

    // semaphore init
    sem_init(&sem_one, 0, 0);   // key val 0
    sem_init(&sem_two, 0, 1);   // key val 1
    
    // thread init
    pthread_create(&pt_id1, NULL, reading, NULL/*no param*/);
    pthread_create(&pt_id2, NULL, accumulate, NULL);
    
    // await thread
    pthread_join(pt_id1, NULL/*no return*/);
    pthread_join(pt_id2, NULL);


    sem_destroy(&sem_one);
    sem_destroy(&sem_two);

    return 0;
}

void* reading(void* args)
{
    for (int i = 0; i < 5; ++i) {
        sem_wait(&sem_two);     // 1. sem2 becomes 0, continue
        printf("Input num: \r\n");  // 3. sem2 becomes -1, pauses
        scanf("%d", &number);           // 5. sem2 became 0, cont
        sem_post(&sem_one);     // 2. sem1 becomes 0.
    }
    
    return NULL;
}

void* accumulate(void* args)
{
    int summation = 0;
    for (int i = 0; i < 5; ++i) {
        sem_wait(&sem_one);     // 1. becomes -1, pauses immediately
        summation += number;        // 3. sem1 became 0, continue
        sem_post(&sem_two);         // 4. sem2 becomes 1
    }
    
    fprintf(stdout, "Result : %d\r\n", summation);

    return NULL;
}
