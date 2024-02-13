.cpu cortex-m0
.thumb

.global memset
memset:
    push {r7, lr}
memset_compare:
    cmp r2, #0           ;@ check if size is zero
    bne memset_store    ;@ if r3 is not zero (i. e. there are more bytes to set) then jump to the set routine
    pop {r7, pc}         ;@ return if equal
memset_store:
    strb r1, [r0]        ;@ store one byte of the value stored in r1 at r0 (increase after)
    add r0, #1           ;@ increase the pointer at r0
    sub r2, #1           ;@ decrease counter by 1
    b memset_compare       ;@ jump to the second instruction in
