#include "idt.h"
#include "../types.h"
#include "../text/fonts.h"
#include "../io.h"
#include "../console/console.h"

struct IDTEntry idt[256];
struct IDTPointer idt_ptr;

void idt_set_gate(uint8 num, uint32 base, uint16 sel, uint8 flags) {
    idt[num].offset_low  = base & 0xFFFF;
    idt[num].selector    = sel;
    idt[num].zero        = 0;
    idt[num].type_attr   = flags;
    idt[num].offset_high = (base >> 16) & 0xFFFF;
}

void idt_load() {
    asm volatile("lidt %0" : : "m"(idt_ptr));
}

// ---------------- ISR C handlers ----------------
void isr0_handler() {
    draw_text(100, 100, "EXCECAO DE DIVISAO POR 0", 1);
}


void irq1_handler() {
    uint8 scancode;
    asm volatile("inb $0x60, %0" : "=a"(scancode));
    if (scancode & 0x80){
        outb(PIC1_COMMAND, 0x20);
        return;
    }
    console_put_char(scancode);
    outb(PIC1_COMMAND, 0x20);
}
 
// ---------------- Inicialização IDT ----------------
extern void isr0();
extern void irq1();

void idt_init() {
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base  = (uint32)&idt;

    for (int i = 0; i < 256; i++)
        idt_set_gate(i, 0, 0, 0);

    idt_set_gate(0,  (uint32)isr0, 0x08, 0x8E);
    idt_set_gate(33, (uint32)irq1, 0x08, 0x8E); // IRQ1 = 33

    idt_load();
}


void pic_remap() {
    outb(PIC1_COMMAND, ICW1_INIT);
    outb(PIC2_COMMAND, ICW1_INIT);
    outb(PIC1_DATA, 0x20); // offset mestre
    outb(PIC2_DATA, 0x28); // offset escravo
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    // libera apenas teclado (IRQ1), mascara resto
    outb(PIC1_DATA, 0xFD); // 11111101
    outb(PIC2_DATA, 0xFF); // todos do escravo mascarados
}