#include <stdint.h>
#include <io.h>
#include <console.h>
#include <printf.h>
#include <disk.h>

void main(uint8_t bootDrive)
{
	clearScreen();

	printf("Hello World!\n");
	printf("Test printf 0x%x %d %s\n", 0x123, 1234, "printf");

	printf("Boot drive: 0x%x\n", bootDrive);

	DISK disk;
	DiskInit(&disk, bootDrive);

	uint8_t buffer[1024];

	// Read disk test
	if (!DiskRead(&disk, 0, 2, buffer))
	{
		printf("Failed to read disk!");
	}

	// Verify
	for (int i = 0; i < 3; i++)
	{
		printf("0x%x ", buffer[i]);
	}

	while (1);
}
