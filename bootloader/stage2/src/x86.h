#pragma once

#define ASMCALL __attribute__((cdecl))

void ASMCALL x86_Test();

//
// Disk operations
// 
bool x86_DiskReset(uint8_t drive);
bool x86_DiskRead(uint8_t drive, uint32_t lba, uint8_t sectorCount, void* dataOut);

