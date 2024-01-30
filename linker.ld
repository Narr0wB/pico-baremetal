MEMORY {
    // We are declaring a region of memory in the memory map that is the SRAM
    // This memory region has read (r) and execute (x) permission

    SRAM(rx) : ORIGIN = 0x20000000, LENGTH = 0x1000
}

SECTIONS {
    . = ORIGIN(SRAM)

    // Here we are declaring the different sections that will make up the ELF file, which will be then converted to UF2
    // The .text section will be contained in the SRAM
    .text {
        
    } > SRAM // This signifies that this section will be stored in SRAM
}