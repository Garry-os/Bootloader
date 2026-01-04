#include <stdint.h>
#include <io.h>
#include <console.h>
#include <printf.h>
#include <x86.h>

void main()
{
	clearScreen();

	printf("Hello World!\n");
	printf("Test printf 0x%x %d %s", 0x123, 1234, "printf");

	x86_Test();

	while (1);
}
