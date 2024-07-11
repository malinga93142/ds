#include <stdio.h>
#include <stdlib.h>
#define FILENAME(x) *x


int main(int argc, char** argv){
    char *x = malloc(sizeof(char)*100);
    x = FILENAME(argv[1]);
    printf("%s\n",x);
}