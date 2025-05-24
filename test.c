#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "ust.h"

Thr t1 = UST_THR_INIT;
Thr t2 = UST_THR_INIT;
Thr t3 = UST_THR_INIT;

int mutex = 0;


void *
func(void *arg)
{
        if (arg == (void *) 0x3)
                mutex_lock(&mutex);
        if (arg == (void *) 0x1)
                mutex_unlock(&mutex);
        printf("From thread %ld\n", (long) arg);
}

int
main()
{
        ust_init(&t1, func, (void *) 1);
        ust_init(&t2, func, (void *) 2);
        ust_init(&t3, func, (void *) 3);
        ust_loop();
        return 0;
}
