#pragma once

//
// Small embedded printf() implementation
//

// The user should implement putc()
void printf_putc(char c);

void printf(const char* fmt, ...);


