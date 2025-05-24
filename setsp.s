; setsp for x86-64

section .text

global setsp
; void setsp(void *ptr)
setsp:
        mov rax, [rsp] ; ret dir into rax
        mov [rdi - 8], rax ; set new stack ret dir as prev ret dir
        lea rsp, [rdi -8] ; set stack to custom address
        ret


