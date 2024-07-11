#include <stdio.h>
#include <unistd.h> // for brk

extern int brk(void *);

void *current_brk(void) {
    void *ret;
    asm("mov $0x0c, %%rax\n\t"
        "syscall\n\t"
        : "=a"(ret));
    return ret;
}

int main() {
    void *start_of_heap = sbrk(0); // Get initial end of data segment (start of heap)
    printf("Start of heap: %p\n", start_of_heap);

    // Allocate 4 KB (for demonstration)
    void *new_brk = (char *)start_of_heap + 4096;
    int result = brk(new_brk);
    if (result != 0) {
        perror("brk");
        return 1;
    }
    printf("Allocated memory from %p to %p\n", start_of_heap, new_brk);

    // Freeing memory (conceptual example, not actual deallocation)
    // In reality, you'd manage pointers or metadata to track allocated blocks.
    // This example just sets the end of the data segment back to the original start.
    result = brk(start_of_heap);
    if (result != 0) {
        perror("brk");
        return 1;
    }
    printf("Freed memory, returning to start of heap: %p\n", start_of_heap);

    return 0;
}
