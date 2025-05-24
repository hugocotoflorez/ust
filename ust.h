#include <setjmp.h>
#include <stdlib.h>
#define UST_STACK_SIZE 4096
#define UST_MAX_THREADS 16

extern void setsp(void *);
extern void mutex_lock(void *mutex);
extern void mutex_unlock(void *mutex);

typedef void *(*__subrutine_call)(void *);

typedef struct {
        struct __jmp_buf_tag runenv;
        __subrutine_call func;
        void *args;
        void *retval;
        int init;
        void *sp;
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
void ust_loop();
