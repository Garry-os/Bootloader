#include <stdint.h>
#include <io.h>
#include <console.h>
#include <printf.h>
#include <x86.h>

void main(uint8_t bootDrive)
{
	clearScreen();

	printf("Hello World!\n");
	printf("Test printf 0x%x %d %s\n", 0x123, 1234, "printf");

	printf("Boot drive: 0x%x\n", bootDrive);

	while (1);
}
