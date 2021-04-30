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

extern uint64_t	boot_from_grub2;

extern memory_info_s	mem_info;

kinfo_s			kparam;
framebuffer_s	framebuffer;

typedef struct
{
	uint64_t	apic_id[CONFIG_MAX_CPUS];
	uint64_t	cpu_idx[CONFIG_MAX_CPUS];
} apic_id_map_s;
apic_id_map_s apic_id_map;

// void get_multiboot2_info(size_t multiboot2_info_base)
// {
// 	int mb2_tagsize = 0;
// 	vir_addr mb2info_start = phy2vir((phy_addr)multiboot2_info_base);
// 	vir_addr mb2info_end = mb2info_start + *((uint32_t *)mb2info_start);
// 	vir_addr mb2info_curr = mb2info_start;
// 	vir_addr mb2info_next = mb2info_start + 8;	// the head tag is 8byte long
// 	int mb2_tagtype = 0;
// 	while (mb2info_next < mb2info_end)
// 	{
// 		mb2info_curr = mb2info_next;
// 		mb2_tagsize  = *(uint32_t *)(mb2info_curr + 4);
// 		mb2info_next = mb2info_curr + (size_t)mb2_tagsize;
// 		mb2info_next = (vir_addr)(((size_t)mb2info_next + 7) & 0xFFFFFFF8); // tag must start at 8byte addr
// 		mb2_tagtype = *(uint32_t *)mb2info_curr;
// 		// find e820 tag
// 		if (mb2_tagtype == MULTIBOOT_TAG_TYPE_MMAP)
// 		{
// 			struct multiboot_tag_mmap * mb2_mmap_p = (struct multiboot_tag_mmap *)mb2info_curr;
// 			size_t e820_entnum = (mb2_mmap_p->size -16) / mb2_mmap_p->entry_size;
// 			struct multiboot_mmap_entry * e820_entry_p = &mb2_mmap_p->entries[0];

// 			mem_info.mb_memmap_nr = e820_entnum;
// 			for (int i = 0; (vir_addr)e820_entry_p < mb2info_next; e820_entry_p++, i++)
// 			{
// 				mem_info.mb_memmap[i].addr = e820_entry_p->addr;
// 				mem_info.mb_memmap[i].len  = e820_entry_p->len;
// 				mem_info.mb_memmap[i].type = e820_entry_p->type;
// 				mem_info.mb_memmap[i].zero = 0;
// 			}
// 		}
// 		// find framebuffer tag
// 		if (mb2_tagtype == MULTIBOOT_TAG_TYPE_FRAMEBUFFER)
// 		{
// 			struct multiboot_tag_framebuffer_common * mb2_fb_p = (struct multiboot_tag_framebuffer_common *)mb2info_curr;
// 			framebuffer.FB_phybase = (phy_addr)mb2_fb_p->framebuffer_addr;
// 			framebuffer.FB_size	   = mb2_fb_p->size;
// 			framebuffer.X_Resolution = mb2_fb_p->framebuffer_width;
// 			framebuffer.Y_Resolution = mb2_fb_p->framebuffer_height;
// 		}
// 	}
// }

void pre_init(size_t mb2info_base)
{
	// enalble syscall/sysret machanism
	uint64_t ia32_efer = rdmsr(IA32_EFER);
	ia32_efer |= MSR_IA32_EFER_SCE;	// bit0: SCE , enable syscall/sysret
	wrmsr(IA32_EFER, ia32_efer);

	memset((void *)&_bss, 0, (void *)&_ebss - (void *)&_bss);

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

	kparam.lcpu_nr = bootinfo->smp_info.core_available;
	uint64_t lcpu_count = 0;
	for (i = 0; i < bootinfo->smp_info.core_num; i++)
	{
		efi_cpudesc_s * curr_cpu = &bootinfo->smp_info.cpus[i];
		if ((curr_cpu->status & 0x2) > 0)
		{
			apic_id_map.apic_id[lcpu_count] = curr_cpu->proccessor_id;
			apic_id_map.cpu_idx[lcpu_count] = lcpu_count;
			lcpu_count++;
		}
	}
}