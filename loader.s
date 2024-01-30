.cpu cortex-m0
.thumb

_entry:
    ldr r0, =0x20001000
    mov sp, r0
    b main
    b .
    
