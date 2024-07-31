#if B
#pragma GCC visibility push(hidden)
#endif
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>

extern void* __curr_brk;
extern int __brk(void*);


void* __SBRK(intptr_t increment)
{
    // bool update_brk = __curr_brk == NULL;

    // if(update_brk)
    //     if(__brk(0) < 0)
    //     {
    //         return (void*)-1;
    //     }
    // if(increment == 0)
    // {
    //     return __curr_brk;
    // }
    // void* old_brk = __curr_brk;

    // if(increment > 0
    //     ? ((uintptr_t)old_brk + (uintptr_t)increment < (uintptr_t)old_brk)
    //     : ((uintptr_t)old_brk  < (uintptr_t)-increment))
    // {
    //     errno = ENOMEM;
    //     return (void*)-1;
    // }

    // if(__brk(old_brk + increment) < 0)
    // {
    //     return (void*)-1;
    // }
    // return old_brk;

    void* old_brk;

    if(__curr_brk == NULL /*|| __glibc_multiple_libcs*/)
    
        if(__brk(0) < 0)
            return (void*)-1;

    if(increment == 0)
        return __curr_brk;
    
    old_brk = __curr_brk;

    if(increment > 0
       ? ((uintptr_t)old_brk + (uintptr_t)increment) < 0
       : ((uintptr_t)old_brk < (uintptr_t)-increment))
    {
        errno = ENOMEM;
        return (void*)-1;
    }

    if(__brk(old_brk + increment) < 0)
    {
        return (void*)-1;
    }
    return old_brk;
}

// lib_hidden_def(__sbrk);
// _weak_alias(__sbrk, _sbrk);