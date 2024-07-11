#include "brk.h"
#include <stdio.h>

union header{
    struct{
        size_t size;
        unsigned is_free;
        union header* next;
    }s;
    ALIGN stubs;
};



header_t *head, *tail;

void *get_free_block(size_t size){
    header_t *curr = head;
    while(curr){
        if(curr->s.is_free && curr->s.size >= size){
            return curr;
        }
        curr = curr->s.next;
    }
    return NULL;
}


void *__malloc(size_t size){
    size_t total_size;
    void *block;
    header_t *header;

    if(!size){
        return NULL;
    }
    pthread_mutex_lock(&global_malloc_lock);

    header = get_free_block(size);

    if(header){
        header->s.is_free = 0;
        pthread_mutex_unlock(&global_malloc_lock);
        return (void*)(header + 1);
    }

    total_size = sizeof(header_t) + size;

    block = sbrk(total_size);

    if(block == (void*)-1){
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }

    header = block;
    header->s.is_free = 0;
    header->s.size    = size;
    header->s.next    = NULL;

    if(!head){
        head = header;
    }
    if(tail){
        tail->s.next = header;
    }
    tail = header;

    pthread_mutex_unlock(&global_malloc_lock);

    return (void*)(header + 1);

}

void __free(void* block){
    header_t *header, *tmp;

    void *pbreak;

    if(!block){
        return;
    }
    pthread_mutex_lock(&global_malloc_lock);
    header = (header_t*)block - 1;

    pbreak = sbrk(0);

    if((char*)block + header->s.size == pbreak){
        if(head == tail){
            head = tail = NULL;
        }
        else{
            tmp = head;
            while(tmp){
                if(tmp->s.next == tail){
                    tmp->s.next = NULL;
                    tail = tmp;
                }
                tmp = tmp->s.next;
            }
        }
        sbrk(0 - sizeof(header_t) - header->s.size);
    }
    else
        header->s.is_free = 1;
    pthread_mutex_unlock(&global_malloc_lock);
}


void *__calloc(size_t num, size_t numblock){
    size_t size;
    void *block;

    if(!num || !numblock){
        return NULL;
    }
    size = num * numblock;

    if(numblock != size/num){
        return NULL;
    }
    block = __malloc(size);
    if(!block){
        return NULL;
    }
    memset(block, 0, size);
    return block;
}

void *__realloc(void *block, size_t size){
    header_t *header;
    void *ret;

    if(!block || !size){
        return __malloc(size);
    }

    header = (header_t*)block - 1;

    if(header->s.size >= size){
        return block;
    }

    if(ret){
        memcpy(ret, block, header->s.size);
        __free(block);
    }
    return ret;
}

void mem_leaks(){
    header_t *curr = head;
    while(curr){
        if(!curr->s.is_free){
            printf("Memory leak detected : %zu by allocated not freed\n", curr->s.size);
        }
        // printf("status : %p -> %zu - %d\n", curr->s, curr->s.size, curr->s.is_free);
        curr = curr->s.next;
    }
}

int main(){
    int *ptr = __malloc(4);
    int *ptr2 = __malloc(1000);
    int *ptr3 = __malloc(10000);

    printf("%p\n%p\n%p\n", ptr, ptr2, ptr3);
    return 0;
}