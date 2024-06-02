.cpu cortex-m0
.thumb

.section .vectors, "ax"
_vtor_table:
  .word 0 ;@ 0 stack pointer
  .word 0 ;@ 1 reset routine
  .word 0 ;@ 2 NMI
  .word 0 ;@ 3 hardFault
  .word 0 ;@ 4 reserved
  .word 0 ;@ 5 reserved
  .word 0 ;@ 6 reserved
  .word 0 ;@ 7 reserved
  .word 0 ;@ 8 reserved
  .word 0 ;@ 9 reserved
  .word 0 ;@ 10 reserved
  .word 0 ;@ 11 SVCall
  .word 0 ;@ 12 reserved
  .word 0 ;@ 13 reserved
  .word 0 ;@ 14 pendSV
  .word 0 ;@ 15 sysTick
  .word 0 ;@ ext interrupt 0 
  .word 0 ;@ ext interrupt 1
  .word 0 ;@ ext interrupt 2
  .word 0 ;@ ext interrupt 3
  .word 0 ;@ ext interrupt 4
  .word usb_isr ;@ ext interrupt 5

.section .boot2, "ax"
_entry:
    ldr r0, =0x20041000 ;@ Load the stack pointer into sp
    mov sp, r0          

    ldr r0, =VTOR_CTRL_REG ;@ Load the vtor control register address
    ldr r1, =VTOR_TABLE ;@ Load the vtor table memory address
    str r1, [r0] ;@ Store the vtor mem addr in the vtor control register

    b main ;@ Exit bootloader
    b .



.set VTOR_TABLE, 0x20042000
.set VTOR_CTRL_REG, 0xe000ed08

