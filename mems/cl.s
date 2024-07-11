.section .note.GNU-stack

.section .text

.globl ptr
.globl ptr2
ptr:
    push %rbp
    mov %rsp, %rbp
    movl %edi, -0x14(%rbp)
    movl -0x14(%rbp), %eax
    cltq
    movq %rax, -0x8(%rbp)
    movq -0x8(%rbp), %rax
    popq %rbp
    ret
ptr2:
    movslq %edi, %rax
    ret