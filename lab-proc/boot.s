/************************************************************
 * boot.s – vetor de interrupções + rotina de reset
 ************************************************************/
        .section .boot, "ax"

        /* exportados para o linker / GDB */
        .global start          @ ponto de entrada REAL (ENTRY)
        .global _start         @ alias opcional

/************  vetor (carregado em 0x8000)  ************/
_start:
start:                                  @ ENTRY(start)
        ldr     pc, _reset_ptr
        ldr     pc, _undef_ptr
        ldr     pc, _swi_ptr
        ldr     pc, _iabort_ptr
        ldr     pc, _dabort_ptr
        nop
        ldr     pc, _irq_ptr
        ldr     pc, _fiq_ptr

_reset_ptr:   .word  reset
_undef_ptr:   .word  panic
_swi_ptr:     .word  swi
_iabort_ptr:  .word  panic
_dabort_ptr:  .word  panic
_irq_ptr:     .word  irq
_fiq_ptr:     .word  panic

/******************  código de reset  ******************/
reset:
#if RPICPU == 2
        /* se vier em EL2, desce para EL1/SVC */
        mrs     r0, cpsr
        and     r0, r0, #0x1f
        cmp     r0, #0x1a
        bne     resume_el1
        mrs     r0, cpsr
        bic     r0, r0, #0x1f
        orr     r0, r0, #0x13
        msr     spsr_cxsf, r0
        add     lr, pc, #4
        msr     ELR_hyp, lr
        eret
resume_el1:
        /* roda só no core 0 */
        mrc     p15,0,r0,c0,c0,5
        tst     r0, #0x03
        beq     core0
        b       panic
core0:
#endif

        /* pilhas IRQ e SVC */
        mov     r0, #0xd2
        msr     cpsr, r0
        ldr     sp, =stack_irq

        mov     r0, #0xd3
        msr     cpsr, r0
        ldr     sp, =stack_sys

        /* copia vetor p/ 0x0000 */
        ldr     r0, =start
        mov     r1, #0
        ldmia   r0!, {r2-r9}
        stmia   r1!, {r2-r9}
        ldmia   r0!, {r2-r9}
        stmia   r1!, {r2-r9}

        /* zera .bss */
        ldr     r0, =bss_begin
        ldr     r1, =bss_end
        mov     r2, #0
bss_clr:
        cmp     r0, r1
        bge     bss_done
        strb    r2, [r0], #1
        b       bss_clr
bss_done:

        /* entra no kernel */
        b       system_main
