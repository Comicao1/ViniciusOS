global isr0
global irq1

extern isr0_handler
extern irq1_handler

[BITS 32]

global gdt_flush

gdt_flush:
    mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter
    lgdt [eax]        ; Load the new GDT pointer

    mov ax, 0x10      ; 0x10 is the offset in the GDT to our DATA segment
    mov ds, ax        ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    jmp 0x08:.flush   ; 0x08 is the offset to our CODE segment. Far jump fixes CS.
.flush:
    ret

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
