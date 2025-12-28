BITS 16
org 0x8000


start:

    mov [boot_drive], dl
    mov al, 0
    mov bx, 0
    mov word [buffer2], 0

    call get_memory_map
    call start_os

     call clear_screen
     mov si, strings
     call print_string
     call get_ram
     mov si, ram_msg
     call print_string
     call newline



     jmp loop


E820_BUFFER    equ 0x5000
E820_ENTRY_SIZE equ 20

get_memory_map:
    xor ebx, ebx           ; continuation value = 0
    mov si, E820_BUFFER    ; ponteiro (16-bit) para buffer
    mov word [entries_count], 0

.e820_next:
    mov eax, 0xE820
    mov edx, 0x534D4150    ; 'SMAP'
    mov ecx, E820_ENTRY_SIZE

    mov ax, si
    shr ax, 4
    mov es, ax
    mov di, si
    and di, 0x000F

    int 0x15
    jc .done

    cmp eax, 0x534D4150
    jne .done

    add si, E820_ENTRY_SIZE
    inc word [entries_count]
    test ebx, ebx
    jnz .e820_next

.done:
    ret

entries_count: dw 0



get_ram:
    mov di, 0

    mov ah, 0x88
    int 0x15           ; AX = KB of RAM
    call get_ram_loop
    ret

get_ram_loop:
    mov bx, 10
    xor dx, dx ; set dx to 0
    div bx ; divide by bx, so it's ax/bx
    ;ax = quotient
    ;dx = remainder
    add dl, '0'
    mov [text_buffer+di], dl
    inc di
    cmp ax, 0
    je return
    call get_ram_loop

    mov bx, 0
    call reverso

    ret

reverso:
    
    cmp byte [text_buffer + bx], 0
    je reverso2

    inc bx
    mov di, 0

    call reverso
    ret

reverso2:
    mov al, [text_buffer + bx]
    mov [ram_msg + di], al
    inc di
    dec bx
    cmp bx, -1
    je return
    call reverso2
    ret

print_string:
    lodsb
    cmp al, 0
    je check_next

    mov ah, 0x0e
    int 0x10
    call print_string
    ret

check_next:
    cmp byte [si], 0
    je return
    call newline
    call print_string
    ret

return:
    ret


clear_screen:
    ; Clear screen
    mov ah, 0x06
    mov al, 0
    mov bh, 0x07
    mov cx, 0x0000
    mov dx, 0x184F
    int 0x10

    ; Reset cursor to top-left
    mov ah, 0x02
    mov bh, 0x00
    mov dh, 0x00
    mov dl, 0x00
    int 0x10
    ret

newline:
    ; Get current cursor position
    mov ah, 0x03
    mov bh, 0x00       ; page 0
    int 0x10           ; returns: DH=row, DL=col

    inc dh             ; go to next row
    mov dl, 0x00       ; reset column to 0

    ; Set new cursor position
    mov ah, 0x02
    int 0x10
    ret





loop:
    call keyboard_loop
    jmp loop

keyboard_loop:
    mov ah, 1 ; preparar input de teclado nao bloqueador
    int 0x16 ; interrupt de teclado
    cmp ah, 0 ; comparando o que recebeu com 0
    jz return ; Mesma coisa que return

    mov ah, 0 ;preparar input de teclado e receber ascii
    int 0x16 ; interrupt de teclado
   ; mov [stored_char], al ;move pra memoria o valor recebido

    cmp al, 0
    je return       ; tecla especial, ignora

    cmp al, 0x0d  ; enter
    je get_enter_command

    cmp al, 0x08  ; backspace
    je get_backspace_command

    mov bx, [buffer2]
    cmp bx, 24    ; max buffer length check
    jae return    ; ignore if full

    mov [buffer1 + bx], al
    inc bx
    mov [buffer2], bx
    mov byte [buffer1 + bx], 0  ; null terminator

    mov ah, 0x0e
    int 0x10
    ret

str_cmp:
    mov si, buffer1         ; SI = user input
    mov di, start_command   ; DI = target string

    call next_char

    ret

next_char:
    lodsb                   ; AL = [SI], SI++
    cmp al, [di]            ; compare with [DI]
    jne no_match
    cmp al, 0               ; are we at the end?
    je match
    inc di
    jmp next_char
    ret

match:
    call newline
    mov si, start_command2
    call print_string
    ;call delay_1s_lazy
    call clear_screen
    call start_os
    ret

no_match:
    call newline
    mov si, start_command3
    call print_string
    ret

get_enter_command:
    call str_cmp
    call newline

    ; --- Clear buffer2 (input index) ---
    mov word [buffer2], 0

    ; --- Clear buffer1 (input text) ---
    mov cx, 25        ; buffer1 length (24 + null terminator)
    mov di, buffer1
    xor al, al
clear_buffer1_loop:
    stosb
    loop clear_buffer1_loop

    ret

get_backspace_command:
    mov bx, [buffer2]
    cmp bx, 0
    je return           ; buffer is empty, do nothing

    dec bx              ; move buffer index back
    mov [buffer2], bx   ; store updated buffer2 index

    mov byte [buffer1 + bx], 0  ; clear char from buffer

    ; visually erase character
    mov ah, 0x0E
    mov al, 0x08        ; backspace cursor
    int 0x10
    mov al, ' '         ; overwrite with space
    int 0x10
    mov al, 0x08        ; move back again
    int 0x10
    ret


delay_1s_lazy:
    mov cx, 0xFFFF
.outer_loop:
    push cx
    mov cx, 0xFFFF
.inner_loop:
    loop .inner_loop
    pop cx
    loop .outer_loop
    ret

;Fim de BIOS inicio de kernel

CODE_OFFSET equ 0x8
DATA_OFFSET equ 0x10

KERNEL_LOAD_SEG equ 0x1000
KERNEL_START_ADDR equ 0x1000



start_os:

    mov ax, 0x0013
    int 0x10

    cli           ; Clear interrupts, disabling all maskable interrupts
    mov ax, 0x00  ; Load immediate value 0x00 into register AX
    mov ds, ax    ; Set data segment (DS) to 0x00
    mov es, ax    ; Set extra segment (ES) to 0x00
    mov ss, ax    ; Set stack segment (SS) to 0x00
    mov sp, 0x7c00; Set stack pointer (SP) to 0x7c00, top of the bootloader segment
    ;sti           ; Enable interrupts, allowing them to occur again
    

;Load kernel
mov bx, KERNEL_LOAD_SEG
mov dh, 0x00
mov dl, [boot_drive]
mov cl, 0x04
mov ch, 0x00
mov ah, 0x02 ; BIOS: Read sectors
mov al, 32 ; Read 1 sector
int 0x13

jc disk_read_error


load_PM:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp CODE_OFFSET:PModeMain


disk_read_error:
    hlt

;GDT Implemetation

gdt_start:
    dd 0x0
    dd 0x0

    ; Code segment descriptor
    dw 0xFFFF       ; Limte
    dw 0x0000       ; Base
    db 0x00         ; Base
    db 10011010b    ; Access byte
    db 11001111b    ; Flags
    db 0x00         ; Base

    ; Data segment descriptor
    dw 0xFFFF       ; Limte
    dw 0x0000       ; Base
    db 0x00         ; Base
    db 10010010b    ; Access byte
    db 11001111b    ; Flags
    db 0x00         ; Base

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; Size of GDT -1
    dd gdt_start 


[BITS 32]
PModeMain:
    mov ax, DATA_OFFSET
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov gs, ax
    mov ebp, 0x9C00
    mov esp, ebp

    in al, 0x92
    or al, 2
    out 0x92, al

    mov al, 'A'
    mov ah, 0x0f
    mov [0xb8000], ax

    fninit

    mov ecx, [entries_count]       ; Carrega contagem
    mov eax, 0x5000                ; Endereço do mapa

    push ecx                       ; Arg2: count (NÃO multiplique por 24)
    push eax                       ; Arg1: addr
    push dword 0                   ; Fake Return Address (para alinhar a pilha para o compilador C)

    jmp CODE_OFFSET:KERNEL_START_ADDR


strings:
    db "==[ ViniciusOS BIOS ]==", 0
    db "--> Versao 1.0", 0
    db "> Digite 'INICIAR' para iniciar o Kernel", 0
    db 0
ram_msg:
    db "      KBs de memoria disponivel em True Mode (16 BITS)", 0
    db 0
text_buffer:
    times 6 db 0 ; "215"
    db 0
inverse_buffer: 
    times 6 db 0 ; "512"
    db 0

start_command:
    db "INICIAR", 0
    db 0

start_command2:
    db ":: Kernel carregado com sucesso ::", 0
    db 0

start_command3:
    db "Comando irreconhecido pelo sistema.", 0
    db 0

error:
    db "errorr kkkk.", 0
    db 0

buffer1:
    times 24 db 0
    db 0

buffer2:
    dw 0
boot_drive: db 0


last_tick: resw 1

; --------------------------
; Boot Signature
; --------------------------
times 1024 - ($ - $$) db 0