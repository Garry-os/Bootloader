#!/bin/bash

#
## Disk creation script
#

set -e

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
cd "$SCRIPTPATH"

TARGET_DIR=../target
MOUNT_DIR=/mnt/fs
IMAGE_PATH=../build/disk.img

if test -f /dev/loop101; then
    echo "/dev/loop101 is assigned for somethings"
    exit 1
fi

echo "Creating disk image"
dd if=/dev/zero of=../build/disk.img bs=1M count=64

echo "Partitioning"
## Create one FAT32 partition
parted "${IMAGE_PATH}" mklabel msdos
parted "${IMAGE_PATH}" mkpart primary fat32 2048s 100%
parted "${IMAGE_PATH}" set 1 boot on

echo "Formatting"

## Setup loopback device
sudo losetup -P /dev/loop101 "${IMAGE_PATH}"

sudo mkfs.fat -F 32 /dev/loop101p1
sudo fatlabel /dev/loop101p1 OS

echo "Installing bootloader..."
sudo dd if=../build/stage1/boot.bin of=/dev/loop101p1 bs=1 seek=0 count=3 conv=notrunc
sudo dd if=../build/stage1/boot.bin of=/dev/loop101p1 bs=1 seek=90 skip=90 conv=notrunc

sudo dd if=../build/stage2/stage2.bin of=/dev/loop101p1 bs=512 seek=1 conv=notrunc

echo "Mounting image"
sudo mkdir -p "${MOUNT_DIR}"
sudo mount /dev/loop101p1 "${MOUNT_DIR}"

echo "Copying files"
sudo cp -r $TARGET_DIR/* $MOUNT_DIR/

echo "Cleaning up"
sudo umount "${MOUNT_DIR}"
sudo losetup -d /dev/loop101

echo "Finished"

