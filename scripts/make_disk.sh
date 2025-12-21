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
dd if=../build/boot.bin of=../build/disk.img conv=notrunc

echo "Finished"

