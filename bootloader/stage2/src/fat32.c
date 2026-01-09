//
// FAT32 drivers
// Very minimal for a bootloader
// Don't have subdirectories support
//

#include "fat32.h"
#include <printf.h>
#include <utils/memory.h>
#include <utils/string.h>
#include <utils/ctype.h>

#define FAT32_NO_MORE_CLUSTER 0x0FFFFFF8

// FAT 12/16 EBR
typedef struct
{
	uint8_t DriveNumber;
	uint8_t Reserved; // Flags in Windows NT
	uint8_t Signature; // Must be 0x28 or 0x29
	uint32_t SerialNumber;
	uint8_t VolumeLabel[11];
	uint8_t SystemIdentifier[8];
} __attribute__((packed)) FAT_EBR;

typedef struct
{
	uint32_t SectorsPerFat;
	uint16_t Flags;
	uint16_t FATVersion;
	uint32_t RootDirectoryClusters;
	uint16_t FSInfoSector;
	uint16_t BackupBootSector;
	uint8_t Reserved[12];
} __attribute__((packed)) FAT32_EBR;

typedef struct
{
	uint8_t jumpInstructions[3]; // JMP SHORT NOP
	uint8_t OEMIdentifier[8];
	uint16_t BytesPerSector;
	uint8_t SectorsPerCluster;
	uint16_t ReservedSectors;
	uint8_t FatCount;
	uint16_t RootDirectoryEntries;
	uint16_t TotalSectors;
	uint8_t MediaDescriptorType;
	uint16_t SectorsPerFat;
	uint16_t SectorsPerTrack;
	uint16_t Heads;
	uint32_t HiddenSectorsCount;
	uint32_t LargeSectorsCount;

	// EBR
	FAT32_EBR EBR32;
	FAT_EBR EBR;

	// Boot code
	
	// 0xAA55 signature
} __attribute__((packed)) FAT32_BPB;

typedef struct
{
	// Boot sector
	union
	{
		uint8_t BootSectorBytes[512];
		FAT32_BPB BPB;
	} BS;

	uint32_t FatStartLBA;
	uint32_t ClusterStartLBA;
} FAT32_Data;

static FAT32_Data* g_FATData;

bool FAT32_ReadBootSector(DISK* disk)
{
	return DiskRead(disk, 0, 1, g_FATData->BS.BootSectorBytes);
}

bool FAT32_Init(DISK* disk)
{
	// The bootloader doesn't have a memory mananger implemented
	// So we need to rely on manually memory management
	g_FATData = (FAT32_Data*)MEM_FAT_ADDRESS;

	// Read the boot sector
	if (!FAT32_ReadBootSector(disk))
	{
		printf("[FAT32] Failed to read boot sector!\n");
		return false;
	}

	// Calculate where the File allocation table & cluster start
	g_FATData->FatStartLBA = g_FATData->BS.BPB.ReservedSectors;
	g_FATData->ClusterStartLBA = g_FATData->BS.BPB.ReservedSectors + (g_FATData->BS.BPB.EBR32.SectorsPerFat * g_FATData->BS.BPB.FatCount);	
	printf("FatStartLBA: 0x%x\n", g_FATData->FatStartLBA);
	printf("ClusterStartLBA: 0x%x\n", g_FATData->ClusterStartLBA);
	printf("Sector size: %d\n", g_FATData->BS.BPB.BytesPerSector);

	printf("Reserved sectors: 0x%x", g_FATData->BS.BPB.ReservedSectors);
	printf("FatCount: 0x%x\n", g_FATData->BS.BPB.FatCount);
	printf("SectorsPerFAT: 0x%x\n", g_FATData->BS.BPB.EBR32.SectorsPerFat);

	return true;
}

uint32_t FAT32_NextCluster(DISK* disk, uint32_t cluster)
{
	uint32_t fatIndex = cluster * 4;
	uint32_t lba = g_FATData->FatStartLBA + (fatIndex / 512);
	uint8_t buffer[512];

	if (!DiskRead(disk, lba, 1, buffer))
	{
		printf("[FAT32] Failed to read next cluster!\n");
		return 0xFFFFFFFF;
	}

	cluster = (*(uint32_t*)&buffer[fatIndex % 512]) & 0x0FFFFFFF;
	return cluster;
}

uint32_t FAT32_ClusterToLba(uint32_t cluster)
{
	return g_FATData->ClusterStartLBA + (cluster - 2) * g_FATData->BS.BPB.SectorsPerCluster;
}

// Convert normal name to FAT short name (8.3 format)
void FAT32_NameToShort(const char* name, char shortName[12])
{
	memset(shortName, ' ', 12);
	shortName[11] = '\0';

	const char* ext = strchr(name, '.');
	if (ext == NULL)
	{
		ext = name + 11;
	}

	for (int i = 0; i < 8 && name[i] && name + i < ext; i++)
	{
		shortName[i] = toupper(name[i]);
	}

	if (ext != name + 11)
	{
		for (int i = 0; i < 3 && ext[i + 1]; i++)
		{
			shortName[i + 8] = toupper(ext[i + 1]);
		}
	}
}

bool FAT32_FindEntry(DISK* disk, uint32_t startCluster, const char* fileName, FAT32_DirectoryEntry* entryOut)
{
	uint8_t buffer[512];
	uint32_t currentCluster = startCluster;
	char shortName[12];

	while (currentCluster < FAT32_NO_MORE_CLUSTER)
	{
		// Loop through the sectors in the cluster
		for (uint8_t i = 0; i < g_FATData->BS.BPB.SectorsPerCluster; i++)
		{
			uint32_t lba = FAT32_ClusterToLba(currentCluster);
			if (!DiskRead(disk, lba + i, 1, buffer))
			{
				printf("[FAT32] Failed to read!\n");
				return false;
			}

			FAT32_DirectoryEntry* entries = (FAT32_DirectoryEntry*)buffer;
			for (uint32_t j = 0; j < 512 / sizeof(FAT32_DirectoryEntry); j++)
			{
				if (entries[j].Name[0] == 0x00)
				{
					// No more files/directories
					break;
				}
				if (entries[j].Name[0] == 0xE5)
				{
					// Entry is unused
					continue;
				}
				if ((entries[j].Attributes & FAT_ATTRIBUTE_LFN) == FAT_ATTRIBUTE_LFN || 
						entries[j].Attributes & FAT_ATTRIBUTE_VOLUME_ID)
				{
					// Skip LFN and volume label
					continue;
				}

				FAT32_DirectoryEntry* dirEntry = (FAT32_DirectoryEntry*)&entries[j];
				
				if (dirEntry->Attributes == FAT_ATTRIBUTE_DIRECTORY ||
						dirEntry->Attributes == FAT_ATTRIBUTE_ARCHIVE)
				{
					FAT32_NameToShort(fileName, shortName);
					for (int i = 0; i < 11; i++)
					{
						printf("Name: %c\n", shortName[i]);
					}

					// Compare
					if (memcmp(shortName, dirEntry->Name, 11) == 0)
					{
						// Found it
						*entryOut = *dirEntry;
						return true;
					}
				}
			}
		}

		// Update current cluster
		currentCluster = FAT32_NextCluster(disk, currentCluster);
	}

	return false;
}

bool FAT32_Traverse(DISK* disk, const char* fileName, FAT32_DirectoryEntry* entryOut)
{
	uint32_t currentCluster = g_FATData->BS.BPB.EBR32.RootDirectoryClusters;
	FAT32_DirectoryEntry entry;

	if (!FAT32_FindEntry(disk, currentCluster, fileName, &entry))
	{
		printf("[FAT32] Failed to find file: %s\n", fileName);
		return false;
	}

	*entryOut = entry;
	return true;
}



