ASM = nasm
ASMFLAGS = -f bin

BUILD_DIR=../../build

$(BUILD_DIR)/stage1/boot.bin: boot.asm
	@ echo "AS" $<
	@ $(ASM) $(ASMFLAGS) -o $@ $<



