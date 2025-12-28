global isr0
global irq1

extern isr0_handler
extern irq1_handler

[BITS 32]

isr0:
    cli
    pusha
    call isr0_handler
    popa
    sti
    iret

irq1:
    cli
    pusha
    call irq1_handler
    popa
    sti
    iret
