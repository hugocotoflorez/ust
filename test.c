#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "ust.h"

Thr t1 = UST_THR_INIT;


void *
func(void *arg)
{
        printf("From thread %d\n", *(int *) arg);
        sleep(10);
}

int
main()
{
        printf("1\n");
        ust_init(&t1, func, (void *) 1);
        return 0;
        printf("2\n");
        ust_loop();
        printf("3\n");
        return 0;
}
