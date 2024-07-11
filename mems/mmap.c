#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#define PSIZE(x, n) (x << n)
int main(){
    register void *rax asm("%rax");
    void* p0=mmap(NULL, 1024, 3, MAP_PRIVATE | 32, -1, 0);
    void* p1=mmap(NULL, 2*PSIZE(1, 12), 3, MAP_PRIVATE | 32, -1, 0);
    printf("%p\n", rax);
    void* p2=mmap(p1, 1024, 3, MAP_PRIVATE | 32, -1, 0);
    printf("%p\n", rax);


    int *arr = (int*)p2;
    
    if(munmap(p1, 2*PSIZE(1, 12)) == -1){
        return 1;
    }
    if(munmap(p2, PSIZE(1, 10)) == -1)
        return 1;

    return 0;
}