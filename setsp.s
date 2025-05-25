; setsp for x86-64

global setsp
; void setsp(void *ptr)
setsp:
        lea rsp, [rdi] ; set stack to custom address
        ret

global getsp
; void *getsp(void)
getsp:
        lea rax, [rsp] ; return sp
        ret

global set_spret
; void set_spret(void *ptr)
set_spret:
        mov rax, [rsp] ; ret dir into rax
        mov [rdi - 8], rax ; set new stack ret dir to prev ret dir
        lea rax, [rdi - 8] ; return new sp
        ret
