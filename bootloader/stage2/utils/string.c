#include "string.h"

const char* strchr(const char* str, char c)
{
	if (str == NULL)
	{
		return NULL;
	}

	while (*str)
	{
		if (*str == c)
		{
			return str;
		}

		str++;
	}

	return NULL;
}

char* strcpy(char* dst, const char* src)
{
	char* originalDst = dst;

	if (dst == NULL)
	{
		return NULL;
	}

	if (src == NULL)
	{
		*dst = '\0';
		return dst;
	}

	while (*src)
	{
		*dst = *src;
		src++;
		dst++;
	}

	*dst = '\0';
	return originalDst;
}

unsigned strlen(const char* str)
{
	unsigned len = 0;

	while (*str)
	{
		len++;
		str++;
	}

	return len;
}

int strcmp(const char* a, const char* b)
{
	if (a == NULL && b == NULL)
	{
		return 0;
	}

	if (a == NULL || b == NULL)
	{
		return -1;
	}

	while (*a && *b && *a == *b)
	{
		a++;
		b++;
	}

	return (*a) - (*b);
}


