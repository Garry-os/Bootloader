ASM = nasm
ASMFLAGS = -f elf32

CC = i686-elf-gcc
CFLAGS =  -Wall -Wextra -ffreestanding -fno-exceptions \
		  -fno-stack-protector -fno-stack-check -fno-lto -fno-PIC -mno-red-zone \
		  -mno-80387 -mno-sse2 -mno-sse -I src/ -I ../../common/

LD = i686-elf-gcc
LDFLAGS = -nostdlib -T linker.ld 

BUILD_DIR=../../build

ASM_SOURCES := $(shell find src -name "*.asm")
ASM_OBJECTS := $(patsubst src/%.asm, $(BUILD_DIR)/stage2/%._asm_o, $(ASM_SOURCES))

C_SOURCES := $(shell find src -name "*.c")
C_OBJECTS := $(patsubst src/%.c, $(BUILD_DIR)/stage2/%.o, $(C_SOURCES))

$(BUILD_DIR)/stage2/stage2.bin: $(ASM_OBJECTS) $(C_OBJECTS) $(BUILD_DIR)/stage2/printf.o
	@ echo "LD" $@
	@ $(LD) $(LDFLAGS) -o $@ $^ -lgcc

$(BUILD_DIR)/stage2/%._asm_o: src/%.asm
	@ echo "AS" $<
	@ $(ASM) $(ASMFLAGS) -o $@ $<

$(BUILD_DIR)/stage2/%.o: src/%.c
	@ echo "CC" $<
	@ $(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/stage2/printf.o: ../../common/printf.c
	@ echo "CC" $<
	@ $(CC) $(CFLAGS) -c -o $@ $<

