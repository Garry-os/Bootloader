#include "bootInfo.h"

// Global boot info for other files to access
BootInfo g_BootInfo;

// Copy the boot info into a global struct
// This prevents the kernel from keeping accessing the
// memory address 0x5000, which is a bit safer
void CopyBootInfo(BootInfo* bootInfo)
{
	g_BootInfo.kernel_phys_address = bootInfo->kernel_phys_address;
	g_BootInfo.bootDrive = bootInfo->bootDrive;
}

