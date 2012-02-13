#include "../include/task.h"
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

const char* DIR = "tasks";

static task_definition* head = NULL;

static void _add_module(void* handle, const char* name, task_handler handler)
{
    task_definition* task = (task_definition*) malloc(sizeof(task_definition));
    char* buffer = (char*) malloc(strlen(name) + 1);
    strcpy(buffer, name);
    task->module_handle = handle;
    task->name = buffer;
    task->handler = handler;
    task->next = head;
    head = task;
}

int load_module(const char* name)
{
    char* path = (char*) malloc(strlen(DIR) + strlen(name) + 8);
    sprintf(path, "../%s/%s.so", DIR, name);
    void* handle = dlopen(path, RTLD_NOW);
    free(path);
    if (handle == NULL)
    {
        printf("Cannot load module: %s\n", dlerror());
        return 1;
    }
    task_handler handler = (task_handler) dlsym(handle, "task_handler_function");
    if (handler == NULL)
    {
        printf("Cannot load function: %s\n", dlerror());
        return 1;
    }
    _add_module(handle, name, handler);
    return 0;
}

static task_definition* _find_task(const char* name)
{
    while (head != NULL)
    {
        if (strcmp(head->name, name) == 0)
        {
            return head;
        }
    }
    return NULL;
}

task_definition* get_task_definition(const char* name)
{
    task_definition* task = _find_task(name);
    if (task == NULL)
    {
        load_module(name);
        // Caution: crappy code
        return _find_task(name);
    }
    return task;
}

void clean_modules()
{
    task_definition* tmp = NULL;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        dlclose(tmp->handler);
        free(tmp->name);
        free(tmp);
    }
}
