#include <stdint.h>

void main()
{
	uint8_t* addr = (uint8_t*)0xB8000;
	*addr = 'c';

	while (1);
}
