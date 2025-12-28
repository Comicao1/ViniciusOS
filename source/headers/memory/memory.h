#ifndef MEMORY_H
#define MEMORY_H

#include "../types.h"   /* define uint32, uint8 etc */
#include <stdint.h>     /* só se precisar de tipos padrão; opcional */

#define PAGE_SIZE 4096
#define PT_ENTRIES 1024
#define PD_ENTRIES 1024

#define MAX_FRAMES (BITMAP_SIZE * 8)
#define BITMAP_SIZE 131072 // Tamanho fixo para cobrir 4GB

extern uint8 pmm_bitmap[BITMAP_SIZE];   /* correção: declarar tipo */
//extern int pmm_initialized = 0;

typedef struct {
    uint32 size;
    uint32 addr;
    uint32 len;
    uint32 type;
} multiboot_mmap_entry_t;

typedef struct {
    uint32 base;
    uint32 length;
    uint32 type;
} e820_entry_t; /* se seu E820 tem low/high, ajuste aqui */

extern uint32 page_directory[PD_ENTRIES];
extern int pmm_initialized;
extern uint32 next_free_phys_addr;
void init_paging(void);
void map_identity(uint32 phys_addr, uint32 size);
void map_all_physical_memory(uint32 mmap_addr, uint32 mmap_length);
void enable_paging(uint32 page_directory_address);
void map_range(uint32 virt, uint32 phys, uint32 size);
void map_page(uint32 virt, uint32 phys);
uint32 allocate_pt_phys(void);

void bitmap_set(uint32 frame);
void bitmap_clear(uint32 frame);
uint32 bitmap_test(uint32 frame);

uint32 pmm_alloc_frame(void);
void pmm_free_frame(uint32 phys_addr);
void pmm_init(e820_entry_t* map, uint32 count);
void *memset(void *s, int c, uint32 n);
#endif /* MEMORY_H */
