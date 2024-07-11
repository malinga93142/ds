#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
void *manual_mem_align(size_t size, size_t align){
    void *original_ptr = malloc(size + align - 1*sizeof(void*));

    if(original_ptr == NULL)
        return NULL;

    uintptr_t addr = (uintptr_t)original_ptr + sizeof(void*);
    uintptr_t align_addr = (addr + align - 1) & ~(align - 1);

    ((void**)align_addr)[-1] = original_ptr;
    return (void*)align_addr;
}

void manual_free(void* ptr){
    free(((void**)ptr)[-1]);
}

int main(){
    void *ptr = manual_mem_align(1, 64);
    void *ptr2= manual_mem_align(1, 64);
    if(ptr == NULL){
        return 1;
    }
    printf("%p\n", ptr);
    printf("%p\n", ptr2);
    
}