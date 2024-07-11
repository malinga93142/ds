#include <stdio.h>
#include <assert.h>
#include <omp.h>
#define FUNCTION __extension__ __attribute__((section(".text"))) __attribute__((noinline))

#define FUNC_NAME __extension__ 
FUNCTION void foo(){
    printf("Hello World\n");
}

int main(){
    foo();
    return 0;
}
