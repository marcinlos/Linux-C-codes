#include "../include/commons.h"
#include "../include/task.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024

void execute(char* line);

int main()
{
    task_definition printer;
    printer.module_handle = NULL;
    printer.name = "printer";
    printer.handler = &sync_print;
    
    setup_pool();
    
    char buffer[BUFFER_SIZE];
    
    while (1)
    {
        fgets(buffer, BUFFER_SIZE, stdin);
        if (strcmp(buffer, "quit\n") == 0)
        {
            break;
        }
        execute(buffer);
    }
    
    wait_for_threads();
    clean_tasks();
    return 0;
}

void execute(char* line)
{
    int len = strlen(line);
    if (line[len - 1] == '\n')
    {
        line[len - 1] = '\0';
    }   
    char* p = strchr(line, ' ');
    if (p != NULL)
    {
        *p ++ = '\0';
    }
    task_definition* task = get_task_definition(line);
    if (task != NULL)
    {
        push_task(task, p);
    }
}


