MEMORY {
    FLASH(rx) : ORIGIN = 0x1000000, LENGTH = 2048K
    RAM(rx) : ORIGIN = 0x2000000, LENGTH = 256K

    /* The first 256 bytes of the uf2 (252 payload + 4 checksum) are copied into SRAM BANK 5
    (which starts at address 0x20041000) and then executed */
}

SECTIONS {
    . = ORIGIN(RAM);

    .boot2 : {
      KEEP(*(.boot2))   
    } > RAM

    .text : {
      *(.text)
    } > RAM
}
