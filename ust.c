#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#include "ust.h"

static Thr *alive_threads[UST_MAX_THREADS] = { 0 };
static int current_thread_index = 0;

void *
alloc_stack()
{
        void *s = malloc(UST_STACK_SIZE);
        assert(s);
        return s + UST_STACK_SIZE;
}

/* Execute a function in their stack and return to parent stack once
 * it returns */
void
ust_init(Thr *thr, __subrutine_call func, void *args)
{
        int i = 0;
        for (; i < UST_MAX_THREADS; i++) {
                if (alive_threads[i] == NULL) {
                        thr->sp = alloc_stack();
                        thr->func = func;
                        thr->args = args;
                        thr->id = i;
                        alive_threads[i] = thr;
                        debug_printf("New thread with func %p\n", thr->func);
                        return;
                }
        }

        fprintf(stderr, "Can not allocate more threads (max=%d)\n", UST_MAX_THREADS);
}

void
ust_run(Thr *thr)
{
        debug_printf("Before setsp\n");
        thr->sp = set_spret(thr->sp);
        setsp(thr->sp);
        debug_printf("After setsp\n");
        if (!thr->init) {
                debug_printf("Running funcion %p for the first time!\n", thr->func);
                thr->init = 1;
                thr->func(thr->args);
                debug_printf("Returning from function %p\n", thr->func);
                alive_threads[thr->id] = NULL;
        } else {
                debug_printf("Longjumping\n");
                longjmp(&thr->runenv, 1);
        }
}


// change
int
ust_switch()
{
        Thr *t = alive_threads[current_thread_index];
        if (t && setjmp(&t->runenv)) // save current thread execution state
                return 0;

        for (int offset = 0; offset <= UST_MAX_THREADS; offset++) {
                current_thread_index = (current_thread_index + 1) % UST_MAX_THREADS;
                t = alive_threads[current_thread_index];
                if (t) {
                        debug_printf("before ust_run\n");
                        ust_run(t);
                        debug_printf("after ust_run\n");
                        return 1;
                }
        }
        return 0;
}

/* Round robin threads execution until all threads are done */
void
ust_loop()
{
        struct __jmp_buf_tag env;
        if (setjmp(&env)) return;

        while (ust_switch())
                ; // runing until all processes are done

        longjmp(&env, 1);
}
