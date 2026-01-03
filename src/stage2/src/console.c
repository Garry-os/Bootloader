#include "console.h"
#include <stdint.h>
#include <io.h>

// 80x25 text mode
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define TEXT_COLOR 0x7 // White-Gray

uint8_t* g_ScreenBuffer = (uint8_t*)0xB8000;
int g_PosX = 0;
int g_PosY = 0;

// Helpers functions
void putChar(int x, int y, char c)
{
	g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x)] = c;
}

void putColor(int x, int y, uint8_t color)
{
	g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x) + 1] = color;
}

char getChar(int x, int y)
{
	return g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x)];
}

uint8_t getColor(int x, int y)
{
	return g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x) + 1];
}

void setCursor(int x, int y)
{
	int pos = y * SCREEN_WIDTH + x;

	x86_outb(0x3D4, 0x0F);
    x86_outb(0x3D5, (uint8_t)(pos & 0xFF)); // Send lower byte
    x86_outb(0x3D4, 0x0E);
    x86_outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF)); // Send higher byte
}

//
// Core functions
//
void clearScreen()
{
	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			putChar(x, y, ' ');
		}
	}

	// Reset positions
	g_PosX = 0;
	g_PosY = 0;
}

void scroll(int lines)
{
	for (int y = lines; y < SCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			putChar(x, y - lines, getChar(x, y));
			putColor(x, y - lines, getColor(x, y));
		}
	}

	for (int y = SCREEN_HEIGHT - lines; y < SCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			putChar(x, y, ' ');
			putColor(x, y, TEXT_COLOR);
		}
	}

	g_PosY -= lines;
}

// Put a character to the screen
// Also handling characters '\n', '\r', '\t'
void putc(char c)
{
	switch (c)
	{
		case '\t':
			for (int i = 0; i < 4; i++)
			{
				putc(' ');
			}
			break;
		case '\n':
			g_PosY++;
			g_PosX = 0;
			break;
		case '\r':
			g_PosX = 0;
			break;
		default:
			putChar(g_PosX, g_PosY, c);
			g_PosX++;
			break;
	}

	// Check for overflowing the screen
	if (g_PosX >= SCREEN_WIDTH)
	{
		g_PosY++;
		g_PosX = 0;
	}

	if (g_PosY >= SCREEN_HEIGHT)
	{
		scroll(1);
	}

	setCursor(g_PosX, g_PosY);
}

void puts(char* str)
{
	while (*str)
	{
		putc(*str);
		str++;
	}
}



