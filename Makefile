ASM = nasm
ASMFLAGS = -f bin

build/floppy.img: build/boot.o
	cp $< $@
	truncate -s 1440k $@

build/boot.o: src/boot.asm
	@ mkdir -p build
	$(ASM) $(ASMFLAGS) -o $@ $<

run:
	qemu-system-i386 -fda build/floppy.img

