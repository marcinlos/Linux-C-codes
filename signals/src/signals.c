#include <unistd.h>
#include <signal.h>
#include <stdio.h>

volatile sig_atomic_t flag;

void prepare(void);
void wait();
void notify(pid_t pid, int sig);

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
    else if (pid == 0)
    {
        int i;
        printf("Child starts\n");
        for (i = 10; i < 20; ++ i)
        {
            wait();
            printf("Child, i = %d\n", i);
            table[i] = i * i;
            notify(getppid(), SIGUSR2);
        }
        _exit(0);
    }
    else
    {
        int i;
        printf("Parent starts\n");
        for (i = 0; i < 10; ++ i)
        {
            table[i] = i * i;
            sleep(1);
            notify(pid, SIGUSR1);
            wait();
        }
    }
    return 0;
}

void prepare(void)
{
    signal(SIGUSR1, sig_handler);
    signal(SIGUSR2, sig_handler);
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2); 
}

void wait()
{
    while (flag == 0)
    {
        sigsuspend(&zeromask);
    }
    flag = 0;
    
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
    {
        fprintf(stderr, "sigprocmask error\n");
    } 
}

void notify(pid_t pid, int sig)
{
    kill(pid, sig);
}

void sig_handler(int sig)
{
    flag = 1;
}
