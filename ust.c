#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define STACK_SIZE 4096

extern void setsp(void *);

typedef void *(*__subrutine_call)(void *);
typedef struct {
        void *sp;
        struct __jmp_buf_tag env;
        void *retval;
} Thr;

void *
alloc_stack()
{
        void *s = malloc(STACK_SIZE);
        assert(s);
        return s + STACK_SIZE;
}

/* Execute a function in their stack and return to parent stack once
 * it returns */
void
ust_init(Thr *thr, __subrutine_call func, void *args)
{
        thr->sp = alloc_stack();

        if (setjmp(&thr->env) == 0) {
                setsp(thr->sp);
                thr->retval = func(args);
                longjmp(&thr->env, 1);
        }
}

void *
function(void *)
{
        volatile int a = 10;
        volatile int b = 20;
        volatile int c;
        c = a + b;
        printf("The sum of %d and %d is %d\n", a, b, c);
        return NULL;
}

int
main()
{
        int a = time(NULL);
        printf("Start report\n");
        Thr t;
        ust_init(&t, function, NULL);
        printf("End report: %ds\n", time(NULL) - a);
}
