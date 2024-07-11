#ifndef _TYPES_H
#define _TYPES_H
#include <string.h>
int CompareInt(void *arg1, void* arg2){
    return *(int*)arg1 - *(int*)arg2;
}
int CompareString(void* arg1, void* arg2){
    return strcmp((char*)arg1, (char*)arg2);
}
#endif