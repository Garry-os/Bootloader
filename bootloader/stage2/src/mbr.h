#pragma once
#include <disk.h>
#include <stdint.h>

typedef struct
{
	uint32_t partitionOffset;
	uint32_t partitionSize;
} PartitionInfo;

void MBR_DetectPartition(PartitionInfo* partOut, void* partitionEntry);

