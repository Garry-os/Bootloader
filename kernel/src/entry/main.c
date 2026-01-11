#include <stdint.h>
#include <console.h>
#include <x86_64/system.h>

void main()
{
	clearScreen();

	printf("Hello World 0x%x\n", 0x123);

	while (1);
}

