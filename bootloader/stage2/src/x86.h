#pragma once

#define ASMCALL __attribute__((cdecl))

void ASMCALL x86_Test();

bool x86_DiskReset(uint8_t drive);

