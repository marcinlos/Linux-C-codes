/*
    Reusable barrier implemented with semaphores.
    ('The Little Book of Semaphores')
*/
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

/* Number of threads running */
#define THREADS 2

#define ITERS 20;

pthread_t threads[THREADS];
/* Semaphores: this solution requires 2 */
sem_t barrier, barrier2;

/* Counter + his mutex */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int count = 0;

void* worker(void* id)
{
    int i;
    
    /* Passed parameter is treated as an id */
    long thread_id = (long) id;
    
    /* How long to wait; will be filled inside the loop */
    struct timespec wait_time;
    wait_time.tv_sec = 0;
    wait_time.tv_nsec = 0;
    
    for (i = 0; i < ITERS; ++ i)
    {         
        /*
            Critical section protects the counter. The last thread to enter
            unlocks the first semaphore (which is initially set to 0),
            and blocks the second. Thus, all the threads are allowed to run now,
            but wait again for the second semaphore. This prevents some 'fast'
            thread from getting through the first semaphore one more time before
            the other threads (as the semaphore is still open after all the
            threads get through).
        */
        pthread_mutex_lock(&mutex);
        ++ count;
        if (count == THREADS)
        {
            sem_wait(&barrier2);
            sem_post(&barrier);
        }
        pthread_mutex_unlock(&mutex);
        
        /* Passing the first semaphore */
        sem_wait(&barrier);
        sem_post(&barrier);
        
        printf("%ld: Start\n", thread_id);
        wait_time.tv_nsec = (rand() % 500 + 500) * 1000000;
        nanosleep(&wait_time, NULL);
        
        /*
            This time the last thread to enter closes the second barrier, and
            opens the first.
        */
        pthread_mutex_lock(&mutex);
        -- count;
        if (count == 0)
        {
            sem_wait(&barrier);
            sem_post(&barrier2);
        } 
        pthread_mutex_unlock(&mutex);
        
        /* Passing the second semaphore */
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
    /* Initialize semaphores: first one is closed */
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
    
