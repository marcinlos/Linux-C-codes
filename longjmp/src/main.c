#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

int main()
{
    if (setjmp(env) == 3)
    {
        printf("Horrible error, with big, sharp teeth!\n");
    }
    else
    {
        printf("Before test\n");
        test();
        printf("After test\n");
    }
    return 0;
}

void test()
{
    printf("Inside test()\n");
    longjmp(env, 3);
    printf("AFTER LONGJMP; shouldn't print :-(");
}

