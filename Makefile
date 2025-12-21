ASM = nasm
ASMFLAGS = -f bin

.PHONY: all disk clean run stage1 always

disk: build/disk.img
build/disk.img: stage1 stage2
	@ chmod +x ./scripts/make_disk.sh
	@ ./scripts/make_disk.sh

stage1: always
	@ mkdir -p build/stage1
	@ $(MAKE) -C src/stage1

stage2: always
	@ mkdir -p build/stage2
	@ $(MAKE) -C src/stage2

always:
	@ mkdir -p build

run:
	qemu-system-i386 -hda build/disk.img

clean:
	rm -rf build


