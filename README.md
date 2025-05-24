# User Space Threads

Declarations:
```c
/* Set stack pointer to ptr */
extern void setsp(void *ptr);
/* Store current dir as return value in sp
and return new sp (sp - 8) */
extern void *set_spret(void *);
/* lock and unlock mutexes */
extern void mutex_lock(int *mutex);
extern void mutex_unlock(int *mutex);


typedef Thr;
typedef __subrutine_call;

#define UST_THR_INIT
/* Add a thread */
void ust_init(Thr *thr, __subrutine_call func, void *args);
/* Execute added threads */
void ust_loop();
```


Usage:
```c

void* foo (void* args){ /* function body */ }

Thr t1 = UST_THR_INIT;
Thr t2 = UST_THR_INIT;

int main(){
    ust_init(&t1, foo, NULL);
    ust_init(&t2, foo, NULL);
    ust_loop();
    return 0;
}

```

