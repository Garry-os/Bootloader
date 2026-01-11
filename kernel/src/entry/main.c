#include <console.h>
#include <x86_64/system.h>
#include <bootInfo.h>

void main()
{
	clearScreen();

	// Copy the boot info
	BootInfo* bootInfo = (BootInfo*)BOOTINFO_ADDRESS;
	CopyBootInfo(bootInfo);

	// Prints out the boot infos
	printf("Kernel physical address: 0x%llx\n", g_BootInfo.kernel_phys_address);
	printf("Boot drive: 0x%llx\n", g_BootInfo.bootDrive);

	while (1);
}

