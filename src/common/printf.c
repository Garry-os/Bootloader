#include "printf.h"
#include <stdarg.h>
#include <stdbool.h>

typedef enum
{
	PRINTF_STATE_NORMAL,
	PRINTF_STATE_LEN,
	PRINTF_STATE_LEN_SHORT,
	PRINTF_STATE_LEN_LONG,
	PRINTF_STATE_SPEC
} PRINTF_STATE;

typedef enum
{
	PRINTF_LEN_DEFAULT,
	PRINTF_LEN_SHORT,
	PRINTF_LEN_SHORT_SHORT,
	PRINTF_LEN_LONG,
	PRINTF_LEN_LONG_LONG
} PRINTF_LEN;

// Helpers functions
void printf_puts(const char* str)
{
	while (*str)
	{
		printf_putc(*str);
		str++;
	}
}

static const char g_HexBuffer[] = "0123456789abcdef";

void printf_unsigned(unsigned long long number, int radix)
{
	char buffer[32];
	int pos = 0;

	do
	{
		unsigned long long reminder = number % radix;
		number /= radix;
		buffer[pos++] = g_HexBuffer[reminder];
	} while (number > 0);

	// Print backward
	while (--pos >= 0)
	{
		printf_putc(buffer[pos]);
	}
}

void printf_signed(long long number, int radix)
{
	if (number < 0)
	{
		printf_putc('-'); // Negative sign
		printf_unsigned(-number, radix);
	}
	else
	{
		printf_unsigned(number, radix);
	}
}

void printf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	int state = PRINTF_STATE_NORMAL;
	int len = PRINTF_LEN_DEFAULT;
	int radix = 10;

	bool sign = false;
	bool number = false;

	while (*fmt)
	{
		switch (state)
		{
			case PRINTF_STATE_NORMAL:
				switch (*fmt)
				{
					case '%':
						state = PRINTF_STATE_LEN;
						break;
					default:
						printf_putc(*fmt);
						break;
				}
				break;
			case PRINTF_STATE_LEN:
				switch (*fmt)
				{
					case 'h':
						// Short
						len = PRINTF_LEN_SHORT;
						state = PRINTF_STATE_LEN_SHORT;
						break;
					case 'l':
						// Long
						len = PRINTF_LEN_LONG;
						state = PRINTF_STATE_LEN_LONG;
						break;
					default:
						goto _PRINTF_STATE_SPEC;
						break;
				}
				break;
			case PRINTF_STATE_LEN_SHORT:
				if (*fmt == 'h')
				{
					len = PRINTF_LEN_SHORT_SHORT;
					state = PRINTF_STATE_SPEC;
				}
				else
				{
					goto _PRINTF_STATE_SPEC;
				}
				break;
			case PRINTF_STATE_LEN_LONG:
				if (*fmt == 'l')
				{
					len = PRINTF_LEN_LONG_LONG;
					state = PRINTF_STATE_SPEC;
				}
				else
				{
					goto _PRINTF_STATE_SPEC;
				}
				break;
			case PRINTF_STATE_SPEC:
			_PRINTF_STATE_SPEC:
				switch (*fmt)
				{
					case 'c':
						printf_putc((char)va_arg(args, int));
						break;
					case 's':
						printf_puts(va_arg(args, const char*));
						break;
					case '%':
						printf_putc('%');
						break;
					case 'd':
					case 'i':
						radix = 10;
						number = true;
						sign = true;
						break;
					case 'u':
						radix = 10;
						number = true;
						sign = false;
						break;
					case 'X':
					case 'x':
					case 'p':
						radix = 16;
						number = true;
						sign = false;
						break;
					case 'o':
						radix = 8;
						number = true;
						sign = false;
						break;

					// Ignore invalid
					default:
						break;
				}
				
				if (number)
				{
					if (sign)
					{
						switch (len)
						{
							case PRINTF_LEN_SHORT_SHORT:
							case PRINTF_LEN_SHORT:
							case PRINTF_LEN_DEFAULT:
								printf_signed(va_arg(args, int), radix);
								break;
							case PRINTF_LEN_LONG:
								printf_signed(va_arg(args, long), radix);
								break;
							case PRINTF_LEN_LONG_LONG:
								printf_signed(va_arg(args, long long), radix);
								break;
						}
					}
					else
					{
						switch (len)
						{
							case PRINTF_LEN_SHORT_SHORT:
							case PRINTF_LEN_SHORT:
							case PRINTF_LEN_DEFAULT:
								printf_unsigned(va_arg(args, int), radix);
								break;
							case PRINTF_LEN_LONG:
								printf_unsigned(va_arg(args, long), radix);
								break;
							case PRINTF_LEN_LONG_LONG:
								printf_unsigned(va_arg(args, long long), radix);
								break;
						}
					}
				}

				// Reset
				state = PRINTF_STATE_NORMAL;
				len = PRINTF_LEN_DEFAULT;
				radix = 10;

				sign = false;
				number = false;
				break;
		}

		fmt++;
	}

	va_end(args);
}


