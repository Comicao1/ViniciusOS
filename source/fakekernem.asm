[BITS 32]
[ORG 0x9000]
start:

    mov al, 'X'
    mov ah, 0x0f
    mov [0xb8000], ax

    jmp $

times 512-($ - $$) db 0