#!/bin/bash

set -e

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
cd "$SCRIPTPATH"

echo "Creating disk image"
dd if=/dev/zero of=../build/disk.img bs=1M count=64

echo "Formatting"
mkfs.fat -F 32 ../build/disk.img

echo "Copying files"
dd if=../build/stage1/boot.bin of=../build/disk.img bs=1 seek=0 count=3 conv=notrunc
dd if=../build/stage1/boot.bin of=../build/disk.img bs=1 seek=138 skip=138 conv=notrunc

dd if=../build/stage2/stage2.bin of=../build/disk.img bs=512 seek=1

echo "Finished"

