ENTRY(kernel_main)

SECTIONS {
    . = 0x1000;

    .text : {
            *(.text.entry)   /* GARANTE que a kernel_main venha PRIMEIRO */
            *(.text .text.*) /* Coloca o código de oi() e o resto DEPOIS */
        }
    .rodata : { *(.rodata .rodata.*) }
    .data : { *(.data .data.*) }

    .bss : {
        _bss_start = .;
        *(.bss .bss.* COMMON)
        _bss_end = .;
    }

    /DISCARD/ : { *(.eh_frame) *(.note.GNU-stack) }
}
