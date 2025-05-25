#include <assert.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ust.h"

static Thr *alive_threads[UST_MAX_THREADS] = { 0 };
static int current_thread_index = 0;

void *
alloc_stack()
{
        uintptr_t s = (uintptr_t) malloc(UST_STACK_SIZE);
        assert(s);
        s += UST_STACK_SIZE; // upper bound
        s &= ~0xF; // align
        return (void *) s;
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
                        // debug_printf("New thread with func %p\n", thr->func);
                        return;
                }
        }

        fprintf(stderr, "Can not allocate more threads (max=%d)\n", UST_MAX_THREADS);
}

void
ust_run(Thr *thr)
{
        thr->sp = set_spret(thr->sp);
        setsp(thr->sp);

        if (thr->init) longjmp(thr->runenv, 1);

        thr->init = 1;
        thr->retval = thr->func(thr->args);
        alive_threads[thr->id] = NULL;
}


// change
int
ust_switch()
{
        Thr *t = alive_threads[current_thread_index];
        static int init = 0;
        if (t && setjmp(t->runenv)) return 0; // save current thread execution state
        if (t && (init++)) t->sp = getsp(); // do not change sp if it is not set and used yet

        for (int offset = 0; offset < UST_MAX_THREADS; offset++) {
                current_thread_index = (current_thread_index + 1) % UST_MAX_THREADS;
                if ((t = alive_threads[current_thread_index])) {
                        ust_run(t);
                        // printf("\n");
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
