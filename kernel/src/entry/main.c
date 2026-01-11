#include <stdint.h>
#include <console.h>
#include <x86_64/system.h>

void main()
{
	clearScreen();

	x86_outb(0xE9, 'h');
	puts("Hello world from kernel!\n");

	while (1);
}

