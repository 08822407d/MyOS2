#include <sys/param.h>

#include <sys/types.h>
#include <string.h>

#include <include/glo.h>
#include <include/memory.h>
#include <include/proto.h>
#include <include/ktypes.h>
#include <include/printk.h>

#include <arch/amd64/include/archconst.h>
#include <arch/amd64/include/archtypes.h>
#include <arch/amd64/include/arch_glo.h>
#include <arch/amd64/include/arch_proto.h>

extern position_t Pos;

void init_video()
{	

	// make sure have get framebuffer infomation
	while (!kparam.arch_init_flags.frame_buffer_info);

	Pos.FB_addr = framebuffer.FB_virbase;
	Pos.FB_length = framebuffer.FB_size;
	Pos.XResolution = framebuffer.X_Resolution;
	Pos.YResolution = framebuffer.Y_Resolution;

	Pos.XPosition = 0;
	Pos.YPosition = 0;
	Pos.XCharSize = 8;
	Pos.YCharSize = 16;

	init_spin_lock(&Pos.printk_lock);
	// clean screen
	memset(framebuffer.FB_virbase, 0, framebuffer.FB_size);
}