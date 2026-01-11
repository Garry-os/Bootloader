#pragma once

#define ASMCALL __attribute__((cdecl))

//
// Disk operations
// 
bool ASMCALL x86_DiskReset(uint8_t drive);
bool ASMCALL x86_DiskRead(uint8_t drive, uint32_t lba, uint8_t sectorCount, void* dataOut);

