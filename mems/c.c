#include <stdio.h>

extern void* _brk(void*);

int main(){
    register void* rax asm("%rax");
    void* ptr = _brk(NULL);
    printf("ptr  -> %p\n", ptr);
    void* new_brk = (char*)ptr + 1024;
    _brk(new_brk);
    printf("%%rax -> %p\n", rax);
    void* new_ptr = _brk(NULL);

    printf("%%rax -> %p\n", new_brk);
}