
ARM_TOOLCHAIN = arm-none-eabi
CARGS =  -mcpu=cortex-m0plus -ffreestanding -g -O0 -fpic -mthumb -c
AARGS = -mcpu=cortex-m0plus --warn --fatal-warnings -g
LDARGS = -nostdlib
BUILD = ./build

$(BUILD)/firmware.elf: $(BUILD)/loader.o $(BUILD)/main.o linker.ld 
	$(ARM_TOOLCHAIN)-ld $(LDARGS) -T linker.ld $(BUILD)/loader.o $(BUILD)/main.o -o $@
	$(ARM_TOOLCHAIN)-objcopy -O binary $@ $(BUILD)/firmware.bin
	${PICO_SDK}/src/rp2_common/boot_stage2/pad_checksum -p 256 -s 0xFFFFFFFF $(BUILD)/firmware.bin $(BUILD)/padded_checked_bootloader.s
	$(ARM_TOOLCHAIN)-as $(AARGS) $(BUILD)/padded_checked_bootloader.s -o $(BUILD)/padded_checked_bootloader.elf

$(BUILD)/main.o: main.c
	$(ARM_TOOLCHAIN)-gcc $(CARGS) $< -o $@

$(BUILD)/loader.o: loader.s
	$(ARM_TOOLCHAIN)-as $(AARGS) $< -o $@ 
