//
// 80x25 text mode console implementation
//

#include "console.h"
#include <stdint.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define DEFAULT_COLOR 0x7 // White-Gray

uint8_t* g_ScreenBuffer = (uint8_t*)0xB8000;
int g_PosX = 0;
int g_PosY = 0;

void putChar(int x, int y, char c)
{
	g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x)] = c;
}

char getChar(int x, int y)
{
	return g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x)];
}

void putColor(int x, int y, uint8_t color)
{
	g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x) + 1] = color;
}

uint8_t getColor(int x, int y)
{
	return g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x) + 1];
}

void clearScreen()
{
	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			putChar(x, y, ' ');
			putColor(x, y, DEFAULT_COLOR);
		}
	}
	
	// Reset positions
	g_PosX = 0;
	g_PosY = 0;
}

void putc(char c)
{
	switch (c)
	{
		case '\n':
			g_PosX = 0;
			g_PosY++;
			break;
		case '\r':
			g_PosX = 0;
			break;
		case '\t':
			for (int i = 0; i < 4; i++)
			{
				putc(' ');
			}
			break;
		default:
			putChar(g_PosX, g_PosY, c);
			g_PosX++;
			break;
	}

	if (g_PosX >= SCREEN_WIDTH)
	{
		g_PosX = 0;
		g_PosY++;
	}

	if (g_PosY >= SCREEN_HEIGHT)
	{
		clearScreen();
	}
}

void puts(const char* str)
{
	while (*str)
	{
		putc(*str);
		str++;
	}
}

