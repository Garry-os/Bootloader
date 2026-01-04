#include "disk.h"
#include <x86.h>

bool DiskInit(DISK* disk, uint8_t drive)
{
	disk->driveID = drive;
	return true;
}

bool DiskRead(DISK* disk, uint32_t lba, uint8_t sectorCount, void* output)
{
	// Read with 3 times retry
	for (int i = 0; i < 3; i++)
	{
		if (x86_DiskRead(disk->driveID, lba, sectorCount, output))
		{
			return true;
		}

		// Reset disk
		x86_DiskReset(disk->driveID);
	}

	return false;
}

