; mutex funtions for x86-64

; switch thread in execution
extern ust_yield

global mutex_lock
; void mutex_lock(void *lock)
mutex_lock:
        mov rax, 1      ; value to put into lock
        xchg rax, [rdi] ; set rdi to rax and rax to rdi
        cmp rax, 0      ; if lock == 0, lock is closed
        je .return      ; if lock is open, return
        call ust_yield  ; call scheduler
        jmp mutex_lock  ; try to adquire lock again
.return:
        ret

global mutex_unlock
; void mutex_unlock(void *lock)
mutex_unlock:
        mov rax, 0      ; lock value after unlock
        mov [rdi], rax  ; set lock to 0
        ret


