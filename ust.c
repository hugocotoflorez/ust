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
        int i;
        thr->sp = alloc_stack();
        thr->func = func;
        thr->args = args;

        printf("New thread: \n"
               "  sp   = %p\n"
               "  func = %p\n"
               "  args = %p\n",
               thr->sp, thr->func, thr->args);

        for (i = 0; i <= UST_MAX_THREADS; i++) {
                if (alive_threads[i] == NULL) {
                        alive_threads[i] = thr;
                        printf("New thread: \n"
                               "  sp   = %p\n"
                               "  func = %p\n"
                               "  args = %p\n"
                               "  id = %d\n",
                               thr->sp, thr->func, thr->args, i);
                        return;
                }
        }

        fprintf(stderr, "Can not allocate more threads (max=%d)\n", UST_MAX_THREADS);
}

void
ust_run(Thr *thr)
{
        setsp(thr->sp);
        if (!thr->init)
                thr->func(thr->args);
        else
                longjmp(&thr->runenv, 1);
}

// change
int
ust_switch()
{
        Thr *t = alive_threads[current_thread_index];
        if (setjmp(&t->runenv)) // save current thread execution state
                return 0;

        for (int offset = 0; offset <= UST_MAX_THREADS; offset++) {
                current_thread_index = (current_thread_index + 1) % UST_MAX_THREADS;
                t = alive_threads[current_thread_index];
                if (t) {
                        ust_run(t);
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

        int ret = ust_switch();
        printf("Returning %d from ust_switch()\n", ret);

        longjmp(&env, 1);
}
