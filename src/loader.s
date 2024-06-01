.cpu cortex-m0
.thumb

.section .boot2, "ax"
_entry:
    ldr r0, =0x20041000 ; Load the stack pointer into sp
    mov sp, r0          

    ldr r0, =VTOR_CTRL_REG ; Load the vtor control register address
    ldr r1, =VTOR_TABLE ; Load the vtor table memory address
    str r1, [r0] ; Store the vtor mem addr in the vtor control register

    b main ; Exit bootloader
    b .
 

.set VTOR_TABLE, 0x20000100
.set VTOR_CTRL_REG, 0xe000ed08
