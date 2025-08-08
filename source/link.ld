ENTRY(kernel_main)

SECTIONS {
    . = 0x1000;

    .text : { *(.text .text.*) }
    .rodata : { *(.rodata .rodata.*) }
    .data : { *(.data .data.*) }

    .bss : {
        _bss_start = .;
        *(.bss .bss.* COMMON)
        _bss_end = .;
    }

    /DISCARD/ : { *(.eh_frame) *(.note.GNU-stack) }
}
