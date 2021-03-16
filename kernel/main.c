#include <lib/string.h>
#include <lib/assert.h>

#include "glo.h"
#include "proto.h"
#include "arch/amd64/include/arch_proto.h"

void cstart(void)
{
    prot_init();
}

void kmain(void)
{
    static int bss_test;

    /* bss sanity check */
    // assert(bss_test == 0);
    bss_test = 1;

    cstart();

    void *addr = (void *)(framebuffer.FB_base);
	int i;

	for (i = 0; i < 2048 * 20; i++)
	{
		*((char *)addr + 0) = (char)0x00;
		*((char *)addr + 1) = (char)0x00;
		*((char *)addr + 2) = (char)0xff;
		*((char *)addr + 3) = (char)0x00;
		addr += 1;
	}
	for (i = 0; i < 2048 * 20; i++)
	{
		*((char *)addr + 0) = (char)0x00;
		*((char *)addr + 1) = (char)0xff;
		*((char *)addr + 2) = (char)0x00;
		*((char *)addr + 3) = (char)0x00;
		addr += 1;
	}
	for (i = 0; i < 2048 * 20; i++)
	{
		*((char *)addr + 0) = (char)0xff;
		*((char *)addr + 1) = (char)0x00;
		*((char *)addr + 2) = (char)0x00;
		*((char *)addr + 3) = (char)0x00;
		addr += 1;
	}
	for (i = 0; i < 2048 * 20; i++)
	{
		*((char *)addr + 0) = (char)0xff;
		*((char *)addr + 1) = (char)0xff;
		*((char *)addr + 2) = (char)0xff;
		*((char *)addr + 3) = (char)0x00;
		addr += 1;
	}

    int k = 1 / 0;

    while(1){};
}