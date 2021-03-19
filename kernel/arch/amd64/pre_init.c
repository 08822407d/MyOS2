#include <lib/string.h>

#include "include/bootinfo.h"
#include "../../param.h"
#include "../../glo.h"
#include "../../include/ktypes.h"

extern char _bss;
extern char _ebss;
kinfo_s kparam;
framebuffer_s framebuffer;

void pre_init()
{
	memset((void *)&_bss, 0, (void *)&_ebss - (void *)&_bss);
	struct KERNEL_BOOT_PARAMETER_INFORMATION *bootinfo =
		(struct KERNEL_BOOT_PARAMETER_INFORMATION *)BOOTINFO_ADDR;
	memset((void *)&kparam, 0, sizeof(kparam));

	int i;
	for (i = 0; bootinfo->E820_Info.E820_Entry[i].length != 0; i++)
	{
		kparam.memmap[i].addr = bootinfo->E820_Info.E820_Entry[i].address;
		kparam.memmap[i].len  = bootinfo->E820_Info.E820_Entry[i].length;
		kparam.memmap[i].type = bootinfo->E820_Info.E820_Entry[i].type;
		kparam.memmap[i].zero = 0;
	}
	kparam.memmap_size = i + 1;

	framebuffer.FB_base = bootinfo->Graphics_Info.FrameBufferBase;
	framebuffer.FB_size = bootinfo->Graphics_Info.FrameBufferSize;
	framebuffer.X_Resolution = bootinfo->Graphics_Info.HorizontalResolution;
	framebuffer.Y_Resolution = bootinfo->Graphics_Info.VerticalResolution;
	framebuffer.PixperScanline = bootinfo->Graphics_Info.PixelsPerScanLine;
	
	__asm__ __volatile__("nop");
}