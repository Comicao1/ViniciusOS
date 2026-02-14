#include "gdt.h"

struct GDTEntry gdt[3];
struct GDTPointer gdt_ptr;

extern void gdt_flush(uint32); 

void gdt_set_gate(int32 num, uint32 base, uint32 limit, uint8 access, uint8 gran) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access      = access;
}

void gdt_init() {
    gdt_ptr.limit = (sizeof(struct GDTEntry) * 3) - 1;
    gdt_ptr.base  = (uint32)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    gdt_flush((uint32)&gdt_ptr);
}