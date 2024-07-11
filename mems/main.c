#include <stdio.h>
#define POWER(x) (x << 31)
extern long ptr(int);
extern long ptr2(int);
int main(){
    long k = ptr(POWER(1));
    long p = ptr2(POWER(1));
    printf("%ld\n", k);
    printf("%ld\n", p);
    return 0;
}