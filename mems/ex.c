#include <brk.h>
#include <stdio.h>
#include <stdint.h>

void *align_address(size_t size, size_t alignment){
    void* malloc_ptr = __malloc(size + alignment - 1*sizeof(void*));
    if(malloc_ptr == NULL)
        return NULL;
    uintptr_t addr = (uintptr_t)malloc_ptr + sizeof(void*);
    uintptr_t aligned_address = (addr + alignment - 1) & ~(alignment - 1);

    ((void**)align_address)[-1] = malloc_ptr;
    return (void*)align_address;
}
int main(){
    void* ptr = align_address(1,100);
    void* ptr1 = align_address(1,1);
    printf("%p\n", ptr);
    printf("%p\n", ptr1);
}