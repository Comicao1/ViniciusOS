#ifndef KHEAP_H
#define KHEAP_H

#include "../types.h"

#define KHEAP_MAGIC 0x12345678
#define KHEAP_MIN_SIZE 0x2000000 

typedef struct header {
    struct header *prev;   
    struct header *next;   
    uint32 size;           
    uint8  is_free;        
    uint32 magic;          
} header_t;

void kheap_init();
void* kmalloc(uint32 size);
void kfree(void* ptr);

#endif