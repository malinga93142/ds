
#include "malloc.c.h"

struct node{
    int data;
    struct node* next;
};
int main(){
    int *ptr = __malloc(sizeof(int));
    *ptr = 10;
    printf("%p, %d\n", ptr, *ptr);
    __free(ptr);
    printf("%p, %d\n", ptr, *ptr);
    int *p = __malloc(sizeof(int));
    printf("%p, %d\n", p, *p);
    return 0;
}
