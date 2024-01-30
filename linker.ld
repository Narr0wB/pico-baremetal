MEMORY {
    /* We are declaring a region of memory in the memory map that is the SRAM
       This memory region has read (r) and execute (x) permission */

    SRAM5(rx) : ORIGIN = 0x20041000, LENGTH = 0x1000 
    /* The first 256 bytes of the uf2 (252 payload + 4 checksum) are copied into SRAM BANK 5
    (which starts at address 0x20041000) and then executed */
}

SECTIONS {
    . = ORIGIN(SRAM5);
    .boot2 : {
        *(.boot2)   
    } > SRAM5

    /* Here we are declaring the different sections that will make up the ELF file, which will be then converted to UF2
        The .text section will be contained in the SRAM */
    .text : {
        *(.text)
    } > SRAM5 /* This signifies that this section will be stored in SRAM */
}