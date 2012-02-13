#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

#define THREADS 2

pthread_t threads[THREADS];
sem_t barrier, barrier2;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int count = 0;

void* worker(void* id)
{
    long thread_id = (long) id;
    struct timespec wait_time;
    wait_time.tv_sec = 0;
    wait_time.tv_nsec = (rand() % 1000) * 1000000;
    nanosleep(&wait_time, NULL);
    wait_time.tv_nsec = (rand() % 500 + 500) * 1000000;
    
    for (;;)
    {         
        pthread_mutex_lock(&mutex);
        ++ count;
        if (count == THREADS)
        {
            sem_wait(&barrier2);
            sem_post(&barrier);
        }
        pthread_mutex_unlock(&mutex);
        sem_wait(&barrier);
        sem_post(&barrier);
        
        printf("%ld: Start\n", thread_id);
        wait_time.tv_nsec = (rand() % 500 + 500) * 1000000;
        nanosleep(&wait_time, NULL);
        
        pthread_mutex_lock(&mutex);
        -- count;
        if (count == 0)
        {
            sem_wait(&barrier);
            sem_post(&barrier2);
        } 
        pthread_mutex_unlock(&mutex);
        
        sem_wait(&barrier2);
        sem_post(&barrier2);
        
        printf("%ld: End\n", thread_id);
    }
    return NULL;
}

void init_threads()
{
    int i = 0;
    while (i < THREADS)
    {
        long ii = i;
        pthread_create(&threads[i ++], NULL, &worker, (void*) ii);
    }
    sem_init(&barrier, 0, 0);
    sem_init(&barrier2, 0, 1);
    srand(time(NULL));
}

void wait_for_threads()
{
    int i = 0;
    while (i < THREADS)
    {
        pthread_join(threads[i ++], NULL);
    }
    sem_destroy(&barrier2);
    sem_destroy(&barrier);
}

int main()
{
    init_threads();
    wait_for_threads();   
    return 0;
}
    
