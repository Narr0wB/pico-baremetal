MEMORY {
    FLASH(rx) : ORIGIN = 0x1000000, LENGTH = 2048K
    RAM(rx) : ORIGIN = 0x2000000, LENGTH = 256K

    /* The first 256 bytes of the uf2 (252 payload + 4 checksum) are copied into SRAM BANK 5
    (which starts at address 0x20041000) and then executed */
}

SECTIONS {

    .boot2 : {
      . = ORIGIN(RAM);
      KEEP(*(.boot2))   
    } > RAM

    .vectors ALIGN(256) : {
      . = ORIGIN(RAM) + 0x100;
      KEEP(*(.vectors))
    } > RAM
    
    .text : {
      . = ORIGIN(RAM) + 0x200;
      KEEP(*(.text))
    } > RAM
}
