; mutex funtions for x86-64

; switch thread in execution
extern ust_switch

global mutex_lock
; void mutex_lock(void *lock)
mutex_lock:
        mov rax, 1      ; value to put into lock
        xchg rax, rdi   ; set rdi to rax and rax to rdi
        cmp rax, 0      ; if lock == 0, lock is closed
        je .return      ; if lock is open, return
        call ust_switch ; call scheduler
        jmp mutex_lock  ; try to adquire lock again
.return:
        ret

global mutex_unlock
; void mutex_unlock(void *lock)
mutex_unlock:
        mov rdi, 0      ; lock value after unlock
        ret


