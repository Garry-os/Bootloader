//
// Bootloader's boot protocol
// Used by both the bootloader & the kernel
//
#pragma once
#include <stdint.h>

#define BOOTINFO_ADDRESS ((void*)0x5000)


// The structure that the bootloader
// will pass the the kernel
typedef struct
{
	uint64_t kernel_phys_address; // The kernel's physical address in memory
	uint8_t bootDrive;
} BootInfo;

