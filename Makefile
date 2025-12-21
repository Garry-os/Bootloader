ASM = nasm
ASMFLAGS = -f bin

.PHONY: all disk clean run

disk: build/disk.img
build/disk.img: build/boot.bin
	@ chmod +x ./scripts/make_disk.sh
	@ ./scripts/make_disk.sh

build/boot.bin: src/boot.asm
	@ mkdir -p build
	$(ASM) $(ASMFLAGS) -o $@ $<

run:
	qemu-system-i386 -hda build/disk.img

clean:
	rm -rf build


