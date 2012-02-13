/*
    Illustration of very simple form of interprocess communication between
    parent and forked child. 
    
    Parent and child are filling an array alternately. The order is enforced
    by waiting for signal from the other process.
*/
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

/* 
    Flag used to inform the process about signal. It's set by signal handler,
    hence the type and volatile qualifier.
*/
volatile sig_atomic_t flag;

/* Sets up masks and handlers */
void prepare(void);

/* Wait for signal */
void wait();

/* Send signal */
void notify(pid_t pid, int sig);

/* Sets the flag */
void sig_handler(int sig);

sigset_t newmask, oldmask, zeromask;

int main()
{
    int table[20];
    pid_t pid;
    prepare();
    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "fork error\n");
    }
    else if (pid == 0) /* Child process */
    {
        int i;
        printf("Child starts\n");
        for (i = 10; i < 20; ++ i)
        {
            wait(); /* Parent goes first */
            printf("Child, i = %d\n", i);
            table[i] = i * i;
            notify(getppid(), SIGUSR1);
        }
        _exit(0);
    }
    else /* Parent process */
    {
        int i;
        printf("Parent starts\n");
        for (i = 0; i < 10; ++ i)
        {
            printf("Parent, i = %d\n", i);
            table[i] = i * i;
            sleep(2);
            notify(pid, SIGUSR1);
            wait();
        }
    }
    return 0;
}

void prepare(void)
{
    signal(SIGUSR1, sig_handler);
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
}

void wait()
{
    /* 
        Blocking the signal ensures it can be delivered only while we're inside
        sigsuspend(). Otherwise flag could change between check and call to
        sigsuspend(), which could potentially cause a deadlock.
    */        
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);
    
    while (flag == 0)
    {
        sigsuspend(&zeromask);
    }
    flag = 0;
    
    sigprocmask(SIG_SETMASK, &oldmask, NULL);
}

void notify(pid_t pid, int sig)
{
    kill(pid, sig);
}

void sig_handler(int sig)
{
    flag = 1;
}
