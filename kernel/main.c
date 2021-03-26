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

	int i = 1 / 0;

    while(1){};
}