#include <lib/string.h>

#include "include/bootinfo.h"
#include "../../include/param.h"
#include "../../include/glo.h"
#include "../../include/ktypes.h"

extern char _bss;
extern char _ebss;
extern char _end;
extern memory_info_s	mem_info;

kinfo_s kparam;
framebuffer_s framebuffer;

void pre_init()
{
	memset((void *)&_bss, 0, (void *)&_ebss - (void *)&_bss);
	memset((void *)&kparam, 0, sizeof(kparam));
	memset((void *)&mem_info, 0, sizeof(mem_info));

	struct KERNEL_BOOT_PARAMETER_INFORMATION *bootinfo =
		(struct KERNEL_BOOT_PARAMETER_INFORMATION *)BOOTINFO_ADDR;

	int i;
	for (i = 0; bootinfo->E820_Info.E820_Entry[i].length != 0; i++)
	{
		mem_info.mb_memmap[i].addr = bootinfo->E820_Info.E820_Entry[i].address;
		mem_info.mb_memmap[i].len  = bootinfo->E820_Info.E820_Entry[i].length;
		mem_info.mb_memmap[i].type = bootinfo->E820_Info.E820_Entry[i].type;
		mem_info.mb_memmap[i].zero = 0;
	}
	mem_info.mb_memmap_nr = i + 1;

	framebuffer.FB_phybase = (phy_addr)bootinfo->Graphics_Info.FrameBufferBase;
	framebuffer.FB_size = bootinfo->Graphics_Info.FrameBufferSize;
	framebuffer.X_Resolution = bootinfo->Graphics_Info.HorizontalResolution;
	framebuffer.Y_Resolution = bootinfo->Graphics_Info.VerticalResolution;
	framebuffer.PixperScanline = bootinfo->Graphics_Info.PixelsPerScanLine;

	framebuffer.FB_virbase = (vir_addr)CONFIG_PAGE_ALIGH((uint64_t)&_end);
}