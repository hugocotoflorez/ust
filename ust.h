#include <setjmp.h>
#include <stdlib.h>
#define UST_STACK_SIZE 4096
#define UST_MAX_THREADS 16

extern void setsp(void *);
extern void *getsp(void);
extern void *set_spret(void *);
extern void mutex_lock(void *mutex);
extern void mutex_unlock(void *mutex);

typedef void *(*__subrutine_call)(void *);

typedef struct {
        jmp_buf runenv;
        volatile __subrutine_call func;
        volatile void *args;
        volatile void *retval;
        volatile int init;
        volatile int id;
        volatile void *sp;
} Thr;

#define UST_THR_INIT            \
        (Thr)                   \
        {                       \
                .func = NULL,   \
                .args = NULL,   \
                .sp = NULL,     \
                .retval = NULL, \
                .init = 0,      \
        }


void ust_init(Thr *thr, __subrutine_call func, void *args);
void ust_yield();
void ust_loop();
