#include <linux/kernel/types.h>

#include <string.h>

int memcmp(const void *x, const void *y, size_t n)
{
	const uint8_t* x8 = (const uint8_t*)x;
	const uint8_t* y8 = (const uint8_t*)y;
	size_t i;
	for(i=0; i<n; i++) {
		if(x8[i] < y8[i])
			return -1;
		else if(x8[i] > y8[i])
			return 1;
	}
	return 0;
}
