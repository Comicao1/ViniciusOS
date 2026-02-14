#ifndef GDT_H
#define GDT_H

#include "../types.h"

struct GDTEntry {
    uint16 limit_low;
    uint16 base_low;
    uint8  base_middle;
    uint8  access;
    uint8  granularity;
    uint8  base_high;
} __attribute__((packed));

struct GDTPointer {
    uint16 limit;
    uint32 base;
} __attribute__((packed));

void gdt_init();

#endif