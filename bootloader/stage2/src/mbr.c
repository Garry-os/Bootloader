#include "mbr.h"

typedef struct
{
	uint8_t Attributes;
	uint8_t CHS_Start[3];
	uint8_t PartitionType;
	uint8_t CHS_End[3];
	uint32_t LBAStart;
	uint32_t Size;
} __attribute__((packed)) MBREntry;

void MBR_DetectPartition(PartitionInfo* partOut, void* partitionEntry)
{
	MBREntry* entry = (MBREntry*)partitionEntry;
	partOut->partitionOffset = entry->LBAStart;
	partOut->partitionSize = entry->Size;
}

