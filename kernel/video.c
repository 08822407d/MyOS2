#include <sys/types.h>
#include "include/const.h"
#include "include/ktypes.h"
#include "include/param.h"

#include "arch/amd64/include/archconst.h"
#include "arch/amd64/include/archtypes.h"
#include "arch/amd64/include/arch_proto.h"

extern kinfo_s kparam;
extern framebuffer_s framebuffer;

void video_init()
{	
	phy_addr phy_pg_base = framebuffer.FB_phybase;
	vir_addr vir_pg_base = framebuffer.FB_virbase;
	int fb_pages = CONFIG_PAGE_ALIGH(framebuffer.FB_size) / CONFIG_PAGE_SIZE;
	for (int i = 0; i < fb_pages; i++)
	{
		pg_domap(vir_pg_base, phy_pg_base, 0);
		vir_pg_base += CONFIG_PAGE_SIZE;
		phy_pg_base += CONFIG_PAGE_SIZE;
	}
}