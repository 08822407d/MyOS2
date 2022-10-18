#include <linux/kernel/types.h>
#include <linux/mm/mm.h>
#include <linux/lib/string.h>

#include <obsolete/glo.h>
#include <obsolete/proto.h>
#include <obsolete/ktypes.h>
#include <obsolete/printk.h>
#include <obsolete/archconst.h>
#include <obsolete/archtypes.h>
#include <obsolete/arch_glo.h>
#include <obsolete/arch_proto.h>

extern position_t Pos;

void myos_init_video()
{	
	// make sure have get framebuffer infomation
	while (!kparam.arch_init_flags.framebuffer);

	Pos.FB_addr = (unsigned int *)framebuffer.FB_virbase;
	Pos.FB_length = framebuffer.FB_size;
	Pos.XResolution = framebuffer.X_Resolution;
	Pos.YResolution = framebuffer.Y_Resolution;

	Pos.XPosition = 0;
	Pos.YPosition = 0;
	Pos.XCharSize = 8;
	Pos.YCharSize = 16;

	init_spin_lock(&Pos.printk_lock);
	// clean screen
	memset((void *)framebuffer.FB_virbase, 0, framebuffer.FB_size);
}