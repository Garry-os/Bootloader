#include <stdint.h>
#include <io.h>
#include <console.h>

void main()
{
	clearScreen();

	for (int i = 0; i < 20; i++)
	{
		puts("Hello World from C!\n");
		puts("...\n");
	}

	while (1);
}
