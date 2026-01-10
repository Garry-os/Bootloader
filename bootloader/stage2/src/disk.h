#pragma once
#include <stdint.h>

typedef struct
{
	uint8_t driveID;
	uint32_t partitionOffset;
	uint32_t partitionSize;
} DISK;

bool DiskInit(DISK* disk, uint8_t drive);
bool DiskRead(DISK* disk, uint32_t lba, uint32_t sectorcount, void* buffer);


