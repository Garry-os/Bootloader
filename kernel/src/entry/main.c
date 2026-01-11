#include <stdint.h>
#include <console.h>
#include <x86_64/system.h>
#include <boot.h>

void main()
{
	clearScreen();

	BootInfo* bootInfo = (BootInfo*)BOOTINFO_ADDRESS;

	printf("Hello World 0x%x\n", 0x123);
	printf("Kernel physical address: 0x%llx\n", bootInfo->kernel_phys_address);
	printf("Boot drive 0x%hhx\n", bootInfo->bootDrive);

	while (1);
}

