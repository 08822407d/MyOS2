#include <linux/kernel/types.h>
#include <linux/mm/mm.h>
#include <linux/lib/string.h>

#include <obsolete/glo.h>
#include <obsolete/proto.h>
#include <obsolete/ktypes.h>
#include <obsolete/printk.h>
#include <obsolete/arch_proto.h>

extern position_t Pos;

void myos_init_video()
{	
	Pos.FB_addr = (unsigned int *)framebuffer.FB_virbase;
	Pos.FB_length = framebuffer.FB_size;
	Pos.XResolution = framebuffer.X_Resolution;
	Pos.YResolution = framebuffer.Y_Resolution;

	Pos.XPosition = 0;
	Pos.YPosition = 0;
	Pos.XCharSize = 8;
	Pos.YCharSize = 16;

	spin_lock_init(&Pos.lock);
	// clean screen
	memset((void *)framebuffer.FB_virbase, 0, framebuffer.FB_size);

	char linebuf[4096] = {0};
	int i;
	for (i = 0; i < Pos.XResolution / Pos.XCharSize ; i++)
		linebuf[i] = ' ';
	color_printk(BLACK, GREEN, "%s", linebuf);
	color_printk(BLACK, GREEN, "\n");
}

// map VBE frame_buffer, this part should not be
// add into any memory manage unit
void myos_init_VBE_mapping()
{
	u64 start, end;
	start = PFN_PHYS(PFN_DOWN(framebuffer.FB_phybase));
	end = start + PFN_PHYS(PFN_UP(framebuffer.FB_size));
	myos_kernel_physical_mapping_init(start, end);
}