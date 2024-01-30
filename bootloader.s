.cpu cortex-m0
.thumb

    .section .vector, "ax"
    .align 2
    .global _vector
_vector:
    .word 0x20001000
    .word _reset

    .thumb_func
    .global _reset
_reset:
    ldr sp, =0x20001000 ; Load the stack pointer

    b main
    b .
    
