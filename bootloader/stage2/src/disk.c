#include "disk.h"
#include <x86.h>
#include <mbr.h>
#include <utils/memory.h>

#define DISK_BUFFER ((void*)0x1000)

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

bool DiskReadRaw(DISK* disk, uint32_t lba, uint8_t sectorCount, void* buffer)
{
	// Read with 3 times retry
	for (int i = 0; i < 3; i++)
	{
		if (x86_DiskRead(disk->driveID, lba + disk->partitionOffset, sectorCount, buffer))
		{
			return true;
		}

		// Reset disk
		x86_DiskReset(disk->driveID);
	}

	return false;
}

// To deal with 16 bit mode boundaries,
// we will read it into a temporary buffer,
// then memcpy() it into the actual buffer
bool DiskRead(DISK* disk, uint32_t lba, uint32_t sectorCount, void* buffer)
{
	void* diskBufferPtr = DISK_BUFFER;

	for (uint32_t i = 0; i < sectorCount; i++)
	{
		if (!DiskReadRaw(disk, lba + i, 1, diskBufferPtr))
		{
			return false;
		}

		// Copy data into buffer
		memcpy(buffer, diskBufferPtr, 512);
		buffer += 512;
	}

	return true;
}

