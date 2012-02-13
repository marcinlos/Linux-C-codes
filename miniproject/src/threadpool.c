#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include "../include/task.h"

#define POOL_SIZE 3

static pthread_t threads[POOL_SIZE];
static pthread_t sig_handler;
static short flag = 1;
static pthread_mutex_t flag_lock = PTHREAD_MUTEX_INITIALIZER;
static sigset_t mask;

static void* _worker(void* unused)
{
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
    short local = 1;
    for (;;)
    {
        pthread_mutex_lock(&flag_lock);
        local = flag;
        pthread_mutex_unlock(&flag_lock);
        if (local == 0)
        {
            break;
        }
        pop_task();
    }
    return NULL;
}

static void* _sig_handler(void* unused)
{
    int sig;
    for (;;)
    {
        sigwait(&mask, &sig);
        printf("No, you cannot interrupt me now!!!!\n");
    }
}

void setup_pool(void)
{
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
    int i = 0;
    while (i < POOL_SIZE)
    {
        pthread_create(&threads[i ++], NULL, &_worker, NULL);
    }
    pthread_create(&sig_handler, NULL, &_sig_handler, NULL);
}

void stop_threads(void)
{
    pthread_mutex_lock(&flag_lock);
    flag = 0;
    pthread_mutex_unlock(&flag_lock);
    wait_for_threads();
}

void wait_for_threads(void)
{
    int i = 0;
    while (i < POOL_SIZE)
    {
        pthread_join(threads[i ++], NULL);
    }    
    pthread_join(sig_handler, NULL);
}
    
