#include <stdint.h>
#include <io.h>
#include <console.h>
#include <printf.h>
#include <disk.h>
#include <fat32.h>

#define KERNEL_LOAD_ADDR ((void*)0x00100000)

typedef void (*KernelStart)();

void hang()
{
	while (1);
}

void main(uint8_t bootDrive)
{
	clearScreen();

	printf("Stage2 loaded\n");
	printf("Booting kernel...\n");
	printf("Boot drive: 0x%x\n", bootDrive);

	// Initialize disk
	DISK disk;
	DiskInit(&disk, bootDrive);

	// Initialize FAT32
	if (!FAT32_Init(&disk))
	{
		hang();
	}

	FAT32_DirectoryEntry entry;
	if (!FAT32_Traverse(&disk, "kernel.bin", &entry))
	{
		printf("Boot process halted!\n");
		hang();
	}

	uint8_t* loadBuffer = (uint8_t*)KERNEL_LOAD_ADDR;
	if (!FAT32_ReadFile(&disk, &entry, loadBuffer))
	{
		printf("Boot process halted!\n");
		hang();
	}

	// Execute
	KernelStart KernelEntry = (KernelStart)KERNEL_LOAD_ADDR;
	KernelEntry();

	hang();
}


