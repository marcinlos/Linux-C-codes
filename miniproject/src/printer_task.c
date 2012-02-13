#include "../include/task.h"
#include "../include/commons.h"

void task_handler_function(const char* arguments)
{
    sync_print("printer: ");
    sync_print(arguments);
    sync_print("\n");
}

