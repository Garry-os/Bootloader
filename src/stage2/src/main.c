#include <stdint.h>
#include <io.h>

void main()
{
	uint8_t* addr = (uint8_t*)0xB8000;
	*addr = 'c';

	x86_outb(0xE9, 'c');

	while (1);
}
