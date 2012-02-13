#include "../include/task.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct task_instance
{
    task_definition* task_def;
    char* arguments;
    struct task_instance* next;
}
task_instance;

static task_instance *head = NULL, *tail = NULL;

static pthread_cond_t new_task = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER;

static void _run_task(task_instance* task)
{
    task_handler handler = task->task_def->handler;
    handler(task->arguments);        
}

void pop_task(void)
{
    task_instance* task;
    pthread_mutex_lock(&queue_lock);
    while (head == NULL)
    {
        pthread_cond_wait(&new_task, &queue_lock);
    }
    task = head;
    head = head->next;
    if (head == NULL)
    {
        tail = head;
    }
    pthread_mutex_unlock(&queue_lock);
    _run_task(task);
    free(task->arguments);
    free(task);
}

void push_task(task_definition* task_def, const char* arguments)
{
    char* buffer = (char*) malloc(strlen(arguments) + 1);
    strcpy(buffer, arguments);
    task_instance* task = (task_instance*) malloc(sizeof(task_instance));
    task->task_def = task_def;
    task->arguments = buffer;
    task->next = NULL;
    
    pthread_mutex_lock(&queue_lock);
    if (tail == NULL)
    {
        head = task;
    }
    else
    {
        tail->next = task;
    }
    tail = task;
    pthread_mutex_unlock(&queue_lock);
    pthread_cond_signal(&new_task);
}

void clean_tasks(void)
{
    task_instance* tmp = NULL;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp->arguments);
        free(tmp);
    }
}

