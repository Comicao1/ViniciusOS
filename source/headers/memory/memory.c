#include <stdint.h>
#include "memory.h" 

#define PAGE_SIZE    0x1000
#define PT_ENTRIES   1024
#define PD_ENTRIES   1024


uint32 page_directory[PD_ENTRIES] __attribute__((aligned(4096)));
uint8 pmm_bitmap[BITMAP_SIZE];


uint32 next_free_phys_addr = 0x400000;


static inline uint32 align_down(uint32 a, uint32 b) { return a & ~(b - 1); }
static inline uint32 align_up(uint32 a, uint32 b)   { return (a + b - 1) & ~(b - 1); }

int pmm_initialized = 0;

uint32 allocate_pt_phys() {
    
    if (!pmm_initialized) {
        uint32 addr = next_free_phys_addr;
        next_free_phys_addr += PAGE_SIZE;
        
        
        uint32* ptr = (uint32*)addr;
        for (int i=0; i<1024; i++) ptr[i] = 0;
        
        return addr;
    }

    
    uint32 frame_phys = pmm_alloc_frame();
    if (frame_phys == 0xFFFFFFFF) {
        
        return 0; 
    }

    uint32 *ptr = (uint32*)frame_phys;
    for (int i = 0; i < 1024; ++i) ptr[i] = 0;
    return frame_phys;
}


void map_page(uint32 virt, uint32 phys) {
    uint32 pdi = (virt >> 22) & 0x3FF;
    uint32 pti = (virt >> 12) & 0x3FF;

    
    if (!(page_directory[pdi] & 1)) {
        uint32 pt_phys = allocate_pt_phys();
        page_directory[pdi] = pt_phys | 0x3; 
    }

    
    uint32* pt = (uint32*)(page_directory[pdi] & 0xFFFFF000);

    pt[pti] = (phys & 0xFFFFF000) | 0x3; 
}


void map_identity(uint32 phys_addr, uint32 size) {
    if (size == 0) return;

    uint32 start = align_down(phys_addr, PAGE_SIZE);
    uint32 end   = align_up(phys_addr + size, PAGE_SIZE);

    for (uint32 addr = start; addr < end; addr += PAGE_SIZE) {
        uint32 pdi = (addr >> 22) & 0x3FF;
        uint32 pti = (addr >> 12) & 0x3FF;

        
        if (!(page_directory[pdi] & 0x1)) {
            uint32 pt_phys = allocate_pt_phys();
            page_directory[pdi] = pt_phys | 0x3; 
        }

        
        uint32 *pt = (uint32*) (page_directory[pdi] & 0xFFFFF000);

        
        pt[pti] = (addr & 0xFFFFF000) | 0x3; 
    }
}


void enable_paging(uint32 page_directory_physical_addr) {
    asm volatile(
        "movl %0, %%cr3\n\t"
        "movl %%cr0, %%eax\n\t"
        "orl $0x80000001, %%eax\n\t" 
        "movl %%eax, %%cr0\n\t"
        :
        : "r"(page_directory_physical_addr)
        : "eax"
    );
}

void map_range(uint32 virt, uint32 phys, uint32 size) {
    for (uint32 offset = 0; offset < size; offset += 0x1000) {
        map_page(virt + offset, phys + offset);
    }
}


void map_all_physical_memory(uint32 e820_map_addr, uint32 e820_entry_count) {
    e820_entry_t *map = (e820_entry_t*) e820_map_addr;

    
    for (int i = 0; i < PD_ENTRIES; ++i) page_directory[i] = 0;

    
    
    map_identity(0xB8000, 0x1000); 

    
    
    map_identity(0x0, 0x400000); 

    
    for (uint32 i = 0; i < e820_entry_count; ++i) {
        if (map[i].type != 1) continue; 

        uint64_t base = map[i].base;
        uint64_t len  = map[i].length;

        
        if (base >= 0xFFFFFFFF) continue;
        if (base + len > 0xFFFFFFFF) len = 0xFFFFFFFF - base;

        map_identity((uint32)base, (uint32)len);
    }
    
    
    
}


void bitmap_set(uint32 frame) {
    if (frame >= MAX_FRAMES) return; 
    pmm_bitmap[frame / 8] |= (1 << (frame % 8));
}

void bitmap_clear(uint32 frame) {
    if (frame >= MAX_FRAMES) return; 
    pmm_bitmap[frame / 8] &= ~(1 << (frame % 8));
}

uint32 bitmap_test(uint32 frame) {
    if (frame >= MAX_FRAMES) return 0; 
    return pmm_bitmap[frame / 8] & (1 << (frame % 8));
}

uint32 pmm_alloc_frame() {
    for (uint32 i = 0; i < MAX_FRAMES; i++) {
        if (!bitmap_test(i)) {
            bitmap_set(i);
            return i * PAGE_SIZE; 
        }
    }
    return 0xFFFFFFFF; 
}

void pmm_free_frame(uint32 phys_addr) {
    uint32 frame = phys_addr / 4096;
    bitmap_clear(frame);
}

void pmm_init(e820_entry_t* map, uint32 count) {
   
    memset(pmm_bitmap, 0xFF, BITMAP_SIZE);

   
    for (uint32 i = 0; i < count; ++i) {
        if (map[i].type != 1) continue;

        uint64_t base = map[i].base;
        uint64_t len  = map[i].length;
        uint64_t end  = base + len;

       
        if (base >= 0xFFFFFFFF) continue;
        if (end > 0xFFFFFFFF) end = 0xFFFFFFFF;

        for (uint64_t addr = base; addr < end; addr += PAGE_SIZE) {
            uint32 frame = (uint32)(addr / PAGE_SIZE);
            if (frame < (BITMAP_SIZE * 8)) {
                bitmap_clear(frame);
            }
        }
    }

   
   
    for (uint32 addr = 0; addr < 0x400000; addr += PAGE_SIZE) {
        uint32 frame = addr / PAGE_SIZE;
        bitmap_set(frame);
    }

   
    uint32 vga_frame = 0xB8000 / PAGE_SIZE;
    bitmap_set(vga_frame);
}

void *memset(void *s, int c, uint32 n) {
    uint8 *p = (uint8*)s;
    while (n--) *p++ = (uint8)c;
    return s;
}