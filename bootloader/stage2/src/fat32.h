#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <disk.h>

// Memory location
#define MEM_FAT_ADDRESS ((void*)0x20000)
#define MEM_FAT_SIZE 0x10000

typedef struct
{
	uint8_t Name[11];
	uint8_t Attributes;
	uint8_t Reserved;
	uint8_t CreatedTimeTenths;
	uint16_t CreatedTime;
	uint16_t CreatedDate;
	uint16_t LastAccessedDate;
	uint16_t FirstClusterHigh;
	uint16_t LastModificatedTime;
	uint16_t LastModificatedDate;
	uint16_t FirstClusterLow;
	uint32_t FileSize;
} __attribute__((packed)) FAT32_DirectoryEntry;

enum FAT_Attributes
{
    FAT_ATTRIBUTE_READ_ONLY         = 0x01,
    FAT_ATTRIBUTE_HIDDEN            = 0x02,
    FAT_ATTRIBUTE_SYSTEM            = 0x04,
    FAT_ATTRIBUTE_VOLUME_ID         = 0x08,
    FAT_ATTRIBUTE_DIRECTORY         = 0x10,
    FAT_ATTRIBUTE_ARCHIVE           = 0x20,
    FAT_ATTRIBUTE_LFN               = FAT_ATTRIBUTE_READ_ONLY | FAT_ATTRIBUTE_HIDDEN | FAT_ATTRIBUTE_SYSTEM | FAT_ATTRIBUTE_VOLUME_ID
};

bool FAT32_Init(DISK* disk);
bool FAT32_Traverse(DISK* disk, const char* fileName, FAT32_DirectoryEntry* entryOut);

