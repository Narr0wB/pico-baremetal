
ARM_TOOLCHAIN = arm-none-eabi
CARGS =  -mcpu=cortex-m0plus -ffreestanding -g -O0 -fpic -mthumb -c
AARGS = -mcpu=cortex-m0plus --warn --fatal-warnings -g
LDARGS = -nostdlib --entry 0x2000000
BUILD = ./build
UTILS = ./utils

$(BUILD)/firmware-padded-checked.bin: $(BUILD)/loader.o $(BUILD)/main.o linker.ld
	$(ARM_TOOLCHAIN)-ld $(LDARGS) -T linker.ld $(BUILD)/main.o $(BUILD)/loader.o -o $(BUILD)/tmp.elf

$(BUILD)/main.o: src/main.c
	$(ARM_TOOLCHAIN)-gcc $(CARGS) $< -o $@

$(BUILD)/loader.o: src/loader.s
	$(ARM_TOOLCHAIN)-as $(AARGS) $< -o $@ 
