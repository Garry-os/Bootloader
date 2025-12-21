ASM = nasm
ASMFLAGS = -f bin

BUILD_DIR=../../build

$(BUILD_DIR)/stage2/stage2.bin: entry.asm
	$(ASM) $(ASMFLAGS) -o $@ $<



