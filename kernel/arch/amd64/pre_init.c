#include <sys/types.h>
#include <lib/string.h>

#include "include/bootinfo.h"
#include "include/arch_proto.h"
#include "include/archconst.h"
#include "include/multiboot2.h"

#include "../../include/param.h"
#include "../../include/glo.h"
#include "../../include/ktypes.h"

extern char _k_phy_start;
extern char _k_vir_start;

extern char _bss;
extern char _ebss;
extern char _end;

extern	uint64_t	boot_from_grub2;

extern memory_info_s	mem_info;

kinfo_s kparam;
framebuffer_s framebuffer;

void get_multiboot2_info(uint64_t multiboot2_info_base)
{
	int mb2_tagsize = 0;
	vir_addr mb2info_start = phy2vir((phy_addr)multiboot2_info_base);
	vir_addr mb2info_end = (vir_addr)((uint64_t)*((uint32_t *)mb2info_start));
	vir_addr mb2info_next = mb2info_start + 8;
	// find e820 tag
	while (mb2info_next < mb2info_end)
	{

	}
	// find framebuffer tag
}

void pre_init(uint64_t mb2info_base)
{
	uint64_t tmp = mb2info_base;
	get_multiboot2_info(mb2info_base);
	// enalble syscall/sysret machanism
	uint64_t ia32_efer = rdmsr(IA32_EFER);
	ia32_efer |= MSR_IA32_EFER_SCE;	// bit0: SCE , enable syscall/sysret
	wrmsr(IA32_EFER, ia32_efer);


	memset((void *)&_bss, 0, (void *)&_ebss - (void *)&_bss);
	memset((void *)&kparam, 0, sizeof(kparam));
	memset((void *)&mem_info, 0, sizeof(mem_info));

	kparam.kernel_phy_base	= &_k_phy_start;
	kparam.kernel_vir_base	= &_k_vir_start;
	kparam.kernel_vir_end	= &_end;

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
	framebuffer.FB_virbase = phy2vir(framebuffer.FB_phybase);
	framebuffer.FB_size = bootinfo->Graphics_Info.FrameBufferSize;
	framebuffer.X_Resolution = bootinfo->Graphics_Info.HorizontalResolution;
	framebuffer.Y_Resolution = bootinfo->Graphics_Info.VerticalResolution;
	framebuffer.PixperScanline = bootinfo->Graphics_Info.PixelsPerScanLine;
}