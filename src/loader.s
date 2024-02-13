.cpu cortex-m0
.thumb

.section .boot2, "ax"
_entry:
    ldr r0, =0x20001000 ; Load the stack pointer into sp
    mov sp, r0          

    ldr r0, =0xe000ed08 ; Load the vtor control register address
    ldr r1, =0x20041000 ; Load the vtor memory address
    str r1, [r0] ; Store the vtor mem addr in the vtor control register

    b main ; Exit bootloader
    b .
    
set_interrupt:
    