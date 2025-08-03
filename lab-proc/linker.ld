ENTRY(start)                               /* rótulo do vetor */

MEMORY { RAM (rwx) : ORIGIN = 0x0, LENGTH = 16M }

SECTIONS
{
    .boot 0x8000 : { *(.boot) } > RAM

    . = ALIGN(8);
    .text : { *(.text*) } > RAM

    /* ------------------- SALTO 1 MiB ------------------- */
    . = 0x00100000;

    /* definir .data ANTES de qualquer referência implícita */
    .data : { *(.data*) } > RAM
    .rodata : { *(.rodata*) } > RAM

    .bss (NOLOAD) : ALIGN(8)
    {
        bss_begin = .;
        *(.bss*)
        *(COMMON)
        bss_end = .;
    } > RAM

    .secure_bss (NOLOAD) : ALIGN(8) { *(.secure_bss) } > RAM

    /* ----------- TTB + estruturas do escalonador -------- */
    . = 0x00500000;                       /* 16 KiB‑aligned */
    .sched (NOLOAD) : { *(.sched) } > RAM
    . += 16K;                             /* espaço p/ TTB  */
    stack_sys = .;

    /* ----------- heap + pilhas de exceção / usuário ----- */
    .heapstack (NOLOAD) : ALIGN(8)
    {
        heap_addr   = .;  . += 32K;
        stack_irq   = .;  . += 4K;
        stack_user1 = .;  . += 4K;
        stack_user2 = .;  . += 4K;
        stack_user3 = .;  . += 4K;
    } > RAM
}
