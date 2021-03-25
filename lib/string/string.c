#include <lib/string.h>

int strlen(char * s)
{
    char * count = s;
    while(*count++);
    return count - s;
}