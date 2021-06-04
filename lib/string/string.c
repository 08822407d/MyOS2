#include <lib/string.h>

int strlen(char * s)
{
    char * count = s;
    while(*count++);
    return count - s;
}

int strcmp(char * str1, char * str2)
{
   	unsigned char c1, c2;
	int res = 0;

	do {
		c1 = *str1++;
		c2 = *str2++;
		res = c1 - c2;
		if (res)
			break;
	} while (c1);
	return res;
}