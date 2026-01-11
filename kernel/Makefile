ASM = nasm
ASMFLAGS = -f elf64

LD = x86_64-elf-gcc
LDFLAGS = -nostdlib -T linker.ld

CC = x86_64-elf-gcc
CFLAGS = -Wall -Wextra -ffreestanding -fno-stack-protector \
		 -fno-stack-check -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone

## Includes files
CFLAGS += -I src/ -I src/graphical -I src/arch

BUILD_DIR = ../build

ASM_SOURCES := $(shell find src -name "*.asm")
ASM_OBJECTS := $(patsubst src/%.asm, $(BUILD_DIR)/kernel/%._asm_o, $(ASM_SOURCES))

C_SOURCES := $(shell find src -name "*.c")
C_OBJECTS := $(patsubst src/%.c, $(BUILD_DIR)/kernel/%.o, $(C_SOURCES))

$(BUILD_DIR)/kernel/kernel.bin: $(ASM_OBJECTS) $(C_OBJECTS)
	@ echo "LD" $@
	@ $(LD) $(LDFLAGS) -o $@ $^ -lgcc

$(BUILD_DIR)/kernel/%._asm_o: src/%.asm
	@ mkdir -p $(dir $@)
	@ echo "AS" $<
	@ $(ASM) $(ASMFLAGS) -o $@ $<

$(BUILD_DIR)/kernel/%.o: src/%.c
	@ mkdir -p $(dir $@)
	@ echo "CC" $<
	@ $(CC) $(CFLAGS) -c -o $@ $<

