#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

static pthread_mutex_t lock;

void sync_print(const char* text)
{
    sleep(5);
    //pthread_mutex_lock(&lock);
    while (*text != 0)
    {
        putchar(*text ++);
        fflush(stdout);
        sleep(1);
    }
    //pthread_mutex_unlock(&lock);
}
