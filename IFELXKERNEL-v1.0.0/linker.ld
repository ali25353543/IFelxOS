ENTRY(start)

SECTIONS
{
    . = 1M;

    /* 🔥 Multiboot MUST be first in file */
    .multiboot ALIGN(8) : {
        KEEP(*(.multiboot))
    }

    /* code */
    .text ALIGN(16) : {
        *(.text*)
    }

    /* read-only data */
    .rodata ALIGN(16) : {
        *(.rodata*)
    }

    .data ALIGN(16) : {
        *(.data*)
    }

    .bss ALIGN(16) : {
        *(.bss*)
    }
}
