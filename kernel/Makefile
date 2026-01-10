ASM = nasm
ASMFLAGS = -f elf32

LD = i686-elf-ld
LDFLAGS = -nostdlib -T linker.ld

BUILD_DIR = ../build

ASM_SOURCES := $(shell find src -name "*.asm")
ASM_OBJECTS := $(patsubst src/%.asm, $(BUILD_DIR)/kernel/%_asm_o, $(ASM_SOURCES))

$(BUILD_DIR)/kernel/kernel.bin: $(ASM_OBJECTS)
	@ echo "LD" $@
	@ $(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/kernel/%_asm_o: src/%.asm
	@ mkdir -p $(dir $@)
	@ echo "AS" $<
	@ $(ASM) $(ASMFLAGS) -o $@ $<

