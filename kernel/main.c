#include <lib/string.h>
#include <lib/assert.h>

#include "include/glo.h"
#include "include/proto.h"
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

	mem_init();
	
	video_init();
	
	memset(framebuffer.FB_virbase, 0, framebuffer.FB_size);

    char *addr = framebuffer.FB_virbase;
	int width = framebuffer.X_Resolution * 20;
	int i;

	for (i = 0; i < width; i++)
	{
		*addr++ = 0x00;
		*addr++ = 0x00;
		*addr++ = 0xff;
		*addr++ = 0x00;
	}
	for (i = 0; i < width; i++)
	{
		*addr++ = 0x00;
		*addr++ = 0xff;
		*addr++ = 0x00;
		*addr++ = 0x00;
	}
	for (i = 0; i < width; i++)
	{
		*addr++ = 0xff;
		*addr++ = 0x00;
		*addr++ = 0x00;
		*addr++ = 0x00;
	}
	for (i = 0; i < width; i++)
	{
		*addr++ = 0xff;
		*addr++ = 0xff;
		*addr++ = 0xff;
		*addr++ = 0x00;
	}

    while(1){};
}