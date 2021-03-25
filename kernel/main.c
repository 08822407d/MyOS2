#include <lib/string.h>
#include <lib/assert.h>

#include "include/glo.h"
#include "include/proto.h"
#include "include/printk.h"
#include "arch/amd64/include/arch_proto.h"

void kmain(void)
{
    prot_init();

	mem_init();
	
	video_init();
	// clear screen
	memset(framebuffer.FB_virbase, 0, framebuffer.FB_size);
	
	color_printk(RED, BLACK,"Hellow OS\n");

    // char *addr = framebuffer.FB_virbase;
	// int width = framebuffer.X_Resolution * 20;
	// int i;

	// for (i = 0; i < width; i++)
	// {
	// 	*addr++ = 0x00;
	// 	*addr++ = 0x00;
	// 	*addr++ = 0xff;
	// 	*addr++ = 0x00;
	// }
	// for (i = 0; i < width; i++)
	// {
	// 	*addr++ = 0x00;
	// 	*addr++ = 0xff;
	// 	*addr++ = 0x00;
	// 	*addr++ = 0x00;
	// }
	// for (i = 0; i < width; i++)
	// {
	// 	*addr++ = 0xff;
	// 	*addr++ = 0x00;
	// 	*addr++ = 0x00;
	// 	*addr++ = 0x00;
	// }
	// for (i = 0; i < width; i++)
	// {
	// 	*addr++ = 0xff;
	// 	*addr++ = 0xff;
	// 	*addr++ = 0xff;
	// 	*addr++ = 0x00;
	// }

    while(1){};
}