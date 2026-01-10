ASM = nasm
ASMFLAGS = -f bin

.PHONY: all disk clean run stage1 stage2 kernel always

disk: build/disk.img
build/disk.img: stage1 stage2 kernel
	@ chmod +x ./scripts/make_disk.sh
	@ ./scripts/make_disk.sh

stage1: always
	@ mkdir -p build/stage1
	@ $(MAKE) -C bootloader/stage1

stage2: always
	@ mkdir -p build/stage2
	@ $(MAKE) -C bootloader/stage2

kernel: always
	@ mkdir -p build/kernel
	@ $(MAKE) -C kernel/
	@ cp build/kernel/kernel.bin target/

always:
	@ mkdir -p build

run:
	qemu-system-x86_64 -hda build/disk.img -debugcon stdio

clean:
	rm -rf build


