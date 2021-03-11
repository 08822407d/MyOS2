#include <lib/string.h>

// _Static_assert (szieof(__UINT64_TYPE__) - 8, "assert1");
extern char _bss;
extern char _ebss;

void pre_init()
{
	memset((void *)&_bss, 0, (void *)&_ebss - (void *)&_bss);
}