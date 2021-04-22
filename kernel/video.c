#include <sys/types.h>
#include <lib/string.h>

#include "include/const.h"
#include "include/param.h"
#include "include/ktypes.h"
#include "include/printk.h"

#include "arch/amd64/include/archconst.h"
#include "arch/amd64/include/archtypes.h"
#include "arch/amd64/include/arch_proto.h"

extern kinfo_s kparam;
extern framebuffer_s framebuffer;
extern position_t Pos;

void video_init()
{	
	phy_addr phy_pg_base = framebuffer.FB_phybase;
	vir_addr vir_pg_base = framebuffer.FB_virbase;

	uint64_t page_attr = ARCH_PG_PRESENT | ARCH_PG_USER | ARCH_PG_RW;
	int fb_pages = CONFIG_PAGE_ALIGH(framebuffer.FB_size) / CONFIG_PAGE_SIZE;
	for (int i = 0; i < fb_pages; i++)
	{
		pg_domap(vir_pg_base, phy_pg_base, page_attr);
		vir_pg_base += CONFIG_PAGE_SIZE;
		phy_pg_base += CONFIG_PAGE_SIZE;
	}

	Pos.FB_addr = framebuffer.FB_virbase;
	Pos.FB_length = framebuffer.FB_size;
	Pos.XResolution = framebuffer.X_Resolution;
	Pos.YResolution = framebuffer.Y_Resolution;

	Pos.XPosition = 0;
	Pos.YPosition = 0;
	Pos.XCharSize = 8;
	Pos.YCharSize = 16;

	memset(framebuffer.FB_virbase, 0, framebuffer.FB_size);
}