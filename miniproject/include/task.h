typedef void (*task_handler)(const char*);

typedef struct task_definition
{
    void* module_handle;
    char* name;
    task_handler handler;        
    struct task_definition* next;
} 
task_definition;

void pop_task(void);
void push_task(task_definition* task_def, const char* arguments);
void clean_tasks(void);

void setup_pool(void);
void stop_threads(void);
void wait_for_threads(void);

task_definition* get_task_definition(const char* name);
int load_module(const char* name);
void clean_modules();


