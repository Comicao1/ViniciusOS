// idt.h
#ifndef IDT_H
#define IDT_H

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2+1)
#define ICW1_INIT    0x11
#define ICW4_8086    0x01
#define PIC_EOI 0x20

#include "../types.h"

struct IDTEntry {
    uint16 offset_low;
    uint16 selector;
    uint8  zero;
    uint8  type_attr;
    uint16 offset_high;
} __attribute__((packed));

struct IDTPointer {
    uint16 limit;
    uint32 base;
} __attribute__((packed));

void idt_set_gate(uint8 num, uint32 base, uint16 sel, uint8 flags);
void idt_load();
void idt_init();
void pic_remap();

#endif
