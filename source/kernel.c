#include "headers/idt/idt.h"
#include "headers/memory/memory.h"
#include "headers/text/fonts.h"
#include "headers/console/console.h"
#include "headers/gdt/gdt.h"
#include "headers/memory/kheap.h"
void kernel_main(uint32 e820_map_addr, uint32 e820_entry_count) {
    e820_entry_t *map = (e820_entry_t*)e820_map_addr;
    for (int i = 0; i < 1024; ++i) page_directory[i] = 0;
    map_identity(0x0, 0x400000); 
    for (uint32 i = 0; i < e820_entry_count; ++i) {
        if (map[i].type != 1) continue; 
        uint64_t base = map[i].base;
        uint64_t len  = map[i].length;
        if (base >= 0xFFFFFFFF) continue;
        if (base + len > 0xFFFFFFFF) len = 0xFFFFFFFF - base;
        map_identity((uint32)base, (uint32)len);
    }
    enable_paging((uint32)page_directory);
    pmm_init(map, e820_entry_count);
    uint32 used_by_tables = next_free_phys_addr;    
    for (uint32 addr = 0x400000; addr < used_by_tables; addr += PAGE_SIZE) {
        uint32 frame = addr / PAGE_SIZE;
        bitmap_set(frame); 
    }
    pmm_initialized = 1; 
    gdt_init();
    pic_remap();
    idt_init();
    kheap_init();

    void* a = kmalloc(10);
    void* b = kmalloc(20);
    console_put_string("Malloc success!");
    kfree(a); // Should merge if b is freed later
    asm volatile("sti"); 
    for(;;) {
        asm volatile("hlt");
    }
}