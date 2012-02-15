#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <poll.h>

#define MAX_BUFFER 512

/* Descriptors for pipe: 0 - reading, 1 - writing */
int pipedes[2];

void create_pipe(void)
{
    pipe(pipedes);
}

void parent_loop(void)
{
    int n;
    char buffer[MAX_BUFFER] = "Event: ";
    
    /* We wait for either the pipe, or the standard input */
    struct pollfd inputs[2];
    inputs[0].fd = STDIN_FILENO;
    inputs[0].events = POLLIN;
    inputs[1].fd = pipedes[0];
    inputs[1].events = POLLIN;
    for (;"ever";)
    {
        if (poll(inputs, 2, 0) > 0)
        {   
            if (inputs[0].revents & POLLIN)
            {
                n = read(STDIN_FILENO, buffer + 7, MAX_BUFFER - 7);
                write(STDOUT_FILENO, buffer, n + 7);
                printf("\n");
            }
            if (inputs[1].revents & POLLIN)
            {
                n = read(pipedes[0], buffer + 7, MAX_BUFFER - 7);
                write(STDOUT_FILENO, buffer, n + 7);
                printf("\n");
            }
        }
    }
}

void child_loop(void)
{
    int i = 0;
    char buffer[MAX_BUFFER];
    for (;"ever";)
    {
        sprintf(buffer, "Message %d", i ++);
        write(pipedes[1], buffer, strlen(buffer));
        sleep(3);
    }
}

int main()
{
    create_pipe();
    int pid;
    if ((pid = fork()) > 0) /* parent */
    {
        /* Close the write end of the pipe */
        close(pipedes[1]);
        
        parent_loop();
    }
    else if (pid == 0) /* child */
    {
        /* Close the read end of the pipe */
        close(pipedes[0]);
        
        child_loop();
    }
    else
    {
        fprintf(stderr, "Error: %s", strerror(errno));
        exit(1);
    }
    return 0;
}
