#pragma once
#include <stdint.h>

typedef struct
{
	uint8_t driveID;
} DISK;

bool DiskInit(DISK* disk, uint8_t drive);
bool DiskRead(DISK* disk, uint32_t lba, uint8_t sectorcount, void* output);


