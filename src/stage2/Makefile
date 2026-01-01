ASM = nasm
ASMFLAGS = -f elf 

LD = ld
LDFLAGS = -nostdlib -T linker.ld -melf_i386

BUILD_DIR=../../build

ASM_SOURCES := $(shell find src -name "*.asm")
ASM_OBJECTS := $(patsubst src/%.asm, $(BUILD_DIR)/stage2/%.o, $(ASM_SOURCES))

$(BUILD_DIR)/stage2/stage2.bin: $(ASM_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $<

$(BUILD_DIR)/stage2/%.o: src/%.asm
	$(ASM) $(ASMFLAGS) -o $@ $<


