

#include <errno.h>
#include <unistd.h>
// #include <sys/syscall.h>
// #include "ext.h"
#include <stdint.h>

#ifndef __NR_nbrk
#define __NR_nbrk 12
#endif

#define INLINE_SYSCALL(name, nr, ...) ({            \
    long ret;                                       \
    __asm__ volatile(                               \
        "movq %1, %%rax\n"                          \
        "movq %2, %%rdi\n"                          \
        "syscall\n"                                 \
        "movq %%rax, %0\n"                          \
        :"=r"(ret)                                  \
        :"i"(__NR_##name), "r"((long)(__VA_ARGS__)) \
        :"%rax", "%rdi", "%rcx", "%r11", "memory"   \
        );                                          \
    ret;                                            \
})

void* __curr_brk = 0;
int __brk(void* addr)
{
    void* newbrk;
    __curr_brk = newbrk = (void*)INLINE_SYSCALL(nbrk, 1, addr);

    if(newbrk < addr)
    {
        errno = ENOMEM;
        return -1;
    }
    return 0;
}
// lib_hidden_def(__brk);
// _weak_alias(__brk, _brk);

