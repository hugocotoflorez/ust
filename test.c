#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "ust.h"

Thr t1 = UST_THR_INIT;
Thr t2 = UST_THR_INIT;
Thr t3 = UST_THR_INIT;
Thr t4 = UST_THR_INIT;

int mutex = 0;


void *
func(void *arg)
{
        int count = 0;
        if (count++ == 0) {
                printf("Yield from thread %ld\n", (long) arg);
                ust_yield();
        }
        printf("Ret from thread %ld\n", (long) arg);
        return NULL;
}

void *
func2(void *arg)
{
        printf("Lock from thread %ld\n", (long) arg);
        mutex_lock(&mutex);
        printf("Ret from thread %ld\n", (long) arg);
}

void *
func3(void *arg)
{
        printf("Unlock from thread %ld\n", (long) arg);
        mutex_unlock(&mutex);
        printf("Ret from thread %ld\n", (long) arg);
}

int
main()
{
        // ust_init(&t1, func, (void *) 0);
        // ust_init(&t2, func, (void *) 1);
        ust_init(&t4, func3, (void *) 3);
        ust_init(&t3, func2, (void *) 2);
        ust_loop();
        return 0;
}
