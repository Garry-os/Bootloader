#include <stdint.h>
#include <io.h>
#include <console.h>
#include <printf.h>
#include <disk.h>
#include <fat32.h>

void hang()
{
	while (1);
}

void main(uint8_t bootDrive)
{
	clearScreen();

	printf("Hello World!\n");
	printf("Test printf 0x%x %d %s\n", 0x123, 1234, "printf");

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
	FAT32_Traverse(&disk, "hellof.txt", &entry);

	hang();
}
