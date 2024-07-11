
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#undef MAP_ANONYMOUS
#define MAP_ANONYMOUS (0x20)

extern void* mem_map(void*addr, size_t size, int prot, int flag, int fd, long off);
extern int mem_unmap(void* addr, size_t);

int main(){
    int fd = open("/dev/zero", O_RDWR);
    void *p = mem_map(NULL, 1024, PROT_WRITE | PROT_READ, MAP_PRIVATE | 0x20, fd, 0);
    
    void *p2=mem_map(p, 1024, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, fd, 0);
    if(mem_unmap(p, 1024) == -1){
        return 1;
    }
    if(mem_unmap(p2, 1024) == -1){
        return 1;
    }
    return 0;
}