.section .note.GNU-stack
.section .text
    .globl mem_map
    .globl mem_unmap

mem_map:

    mov $0x9, %rax

    mov %edx, %edx
    mov %ecx, %ecx
    mov %rcx, %r10
    mov %r8d, %r8d
    mov %r9d, %r9d
    syscall

    ret

mem_unmap:
    mov $0x0b, %rax
    syscall

    cmp $0, %rax
    je success

    mov $-1, %rax
    ret

success:
    ret

