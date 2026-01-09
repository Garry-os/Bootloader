#include "disk.h"
#include <x86.h>
#include <mbr.h>

bool DiskInit(DISK* disk, uint8_t drive)
{
	disk->driveID = drive;
	disk->partitionOffset = 0;
	disk->partitionSize = 0;

	// Detect MBR partition
	uint8_t buffer[512]; // Read the MBR table
	DiskRead(disk, 0, 1, buffer);

	// Partition entry
	PartitionInfo part;
	void* partEntry = &buffer[446];
	MBR_DetectPartition(&part, partEntry);

	disk->partitionOffset = part.partitionOffset;
	disk->partitionSize = part.partitionSize;

	return true;
}

bool DiskRead(DISK* disk, uint32_t lba, uint8_t sectorCount, void* output)
{
	// Read with 3 times retry
	for (int i = 0; i < 3; i++)
	{
		if (x86_DiskRead(disk->driveID, lba + disk->partitionOffset, sectorCount, output))
		{
			return true;
		}

		// Reset disk
		x86_DiskReset(disk->driveID);
	}

	return false;
}

