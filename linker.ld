MEMORY {
    /* We are declaring a region of memory in the memory map that is the SRAM
       This memory region has read (r) and execute (x) permission */

    FLASH(rx) : ORIGIN = 0x1000000, LENGTH = 2048K

    /* The first 256 bytes of the uf2 (252 payload + 4 checksum) are copied into SRAM BANK 5
    (which starts at address 0x20041000) and then executed */
}

SECTIONS {
    . = ORIGIN(FLASH);
    
    .boot2 : {
        *(.boot2)   
    } > FLASH

    .text : {
        *(.text)
    } > FLASH
}