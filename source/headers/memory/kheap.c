#include "kheap.h"
#include "../console/console.h" 



extern uint32 end; 

header_t *head = 0;
header_t *tail = 0;
uint32 heap_start = 0;

void kheap_init() {
    
    
    heap_start = (uint32)&end + 0x1000;
    
    
    
    head = (header_t*)heap_start;
    
    head->size = KHEAP_MIN_SIZE - sizeof(header_t);
    head->is_free = 1;
    head->prev = 0;
    head->next = 0;
    head->magic = KHEAP_MAGIC;

    tail = head;
}

void* kmalloc(uint32 size) {
    if (size == 0) return 0;

    
    header_t *curr = head;
    while (curr != 0) {
        
        if (curr->is_free && curr->size >= size) {
            
            
            
            
            if (curr->size >= size + sizeof(header_t) + 4) {
                
                
                header_t *new_block = (header_t*)((uint32)curr + sizeof(header_t) + size);
                
                
                new_block->is_free = 1;
                new_block->size = curr->size - size - sizeof(header_t);
                new_block->magic = KHEAP_MAGIC;
                
                
                new_block->next = curr->next;
                new_block->prev = curr;
                
                if (curr->next != 0) {
                    curr->next->prev = new_block;
                }
                
                curr->next = new_block;
                
                
                curr->size = size;
                
                
                if (curr == tail) {
                    tail = new_block;
                }
            }
            
            
            curr->is_free = 0;
            curr->magic = KHEAP_MAGIC; 
            
            
            return (void*)((uint32)curr + sizeof(header_t));
        }
        
        curr = curr->next;
    }
    
    
    return 0;
}

void kfree(void* ptr) {
    if (ptr == 0) return;

    
    header_t *curr = (header_t*)((uint32)ptr - sizeof(header_t));

    
    if (curr->magic != KHEAP_MAGIC) {
        
        
        return;
    }

    
    curr->is_free = 1;

    
    
    if (curr->next != 0 && curr->next->is_free) {
        curr->size += curr->next->size + sizeof(header_t);
        curr->next = curr->next->next;
        
        if (curr->next != 0) {
            curr->next->prev = curr;
        } else {
            tail = curr; 
        }
    }

    
    
    if (curr->prev != 0 && curr->prev->is_free) {
        curr->prev->size += curr->size + sizeof(header_t);
        curr->prev->next = curr->next;
        
        if (curr->next != 0) {
            curr->next->prev = curr->prev;
        } else {
            tail = curr->prev;
        }
        
        
        
    }
}