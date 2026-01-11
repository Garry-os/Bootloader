#include <stdint.h>

void main()
{
	uint8_t* buffer = (uint8_t*)0xB8000;
	*buffer = 'A';

	while (1);
}

