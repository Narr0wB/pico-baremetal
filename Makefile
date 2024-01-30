
ARM_TOOLCHAIN = arm-none-eabi
CARGS =  -mcpu=cortex-m0plus -ffreestanding -g -O0 -fpic -mthumb -c
AARGS = -mcpu=cortex-m0plus --warn --fatal-warnings -g
LDARGS = -nostdlib
BUILD = ./build
UTILS = ./utils

$(BUILD)/firmware.uf2: $(BUILD)/firmware-padded-checked.bin
	python3 $(UTILS)/uf2conv.py $(BUILD)/firmware-padded-checked.bin --base 0x10000000 --family RP2040 --output $(BUILD)/firmware.uf2

$(BUILD)/firmware-padded-checked.bin: $(BUILD)/loader.o $(BUILD)/main.o linker.ld
	$(ARM_TOOLCHAIN)-ld $(LDARGS) -T linker.ld $(BUILD)/main.o $(BUILD)/loader.o -o $(BUILD)/tmp.elf
	$(ARM_TOOLCHAIN)-objcopy -O binary $(BUILD)/tmp.elf $(BUILD)/tmp.bin
	rm $(BUILD)/tmp.elf
	${PICO_SDK}/src/rp2_common/boot_stage2/pad_checksum -p 256 -s 0xFFFFFFFF $(BUILD)/tmp.bin $(BUILD)/firmware-padded-checked.s
	rm $(BUILD)/tmp.bin
	$(ARM_TOOLCHAIN)-as $(AARGS) $(BUILD)/firmware-padded-checked.s -o $(BUILD)/firmware-padded-checked.elf
	$(ARM_TOOLCHAIN)-objcopy -O binary $(BUILD)/firmware-padded-checked.elf $(BUILD)/firmware-padded-checked.bin

$(BUILD)/main.o: main.c
	$(ARM_TOOLCHAIN)-gcc $(CARGS) $< -o $@

$(BUILD)/loader.o: loader.s
	$(ARM_TOOLCHAIN)-as $(AARGS) $< -o $@ 
