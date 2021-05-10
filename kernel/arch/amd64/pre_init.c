#include <sys/types.h>
#include <lib/string.h>

#include "include/bootinfo.h"
#include "include/arch_glo.h"
#include "include/arch_proto.h"
#include "include/archconst.h"
#include "include/multiboot2.h"

#include "../../include/param.h"
#include "../../include/glo.h"
#include "../../include/ktypes.h"

extern char _k_phys_start;
extern char _k_virt_start;

extern char _bss;
extern char _ebss;
extern char _end;

extern uint64_t	boot_from_grub2;

extern memory_info_s	mem_info;

kinfo_s			kparam;
cpu_info_s		cpuinfo;
framebuffer_s	framebuffer;

uint64_t	apic_id[CONFIG_MAX_CPUS];
struct cputopo	smp_topos[CONFIG_MAX_CPUS];

void cpuid_info(void);

void pre_init(void)
{
	// enalble syscall/sysret machanism
	uint64_t ia32_efer = rdmsr(IA32_EFER);
	ia32_efer |= MSR_IA32_EFER_SCE;	// bit0: SCE , enable syscall/sysret
	wrmsr(IA32_EFER, ia32_efer);

	memset((virt_addr)&_bss, 0, &_ebss - &_bss);

	kparam.kernel_phy_base	= &_k_phys_start;
	kparam.kernel_vir_base	= &_k_virt_start;
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
	kparam.arch_init_flags.memory_layout = 1;

	framebuffer.FB_phybase = (phys_addr)bootinfo->Graphics_Info.FrameBufferBase;
	framebuffer.FB_virbase = phys2virt(framebuffer.FB_phybase);
	framebuffer.FB_size = bootinfo->Graphics_Info.FrameBufferSize;
	framebuffer.X_Resolution = bootinfo->Graphics_Info.HorizontalResolution;
	framebuffer.Y_Resolution = bootinfo->Graphics_Info.VerticalResolution;
	framebuffer.PixperScanline = bootinfo->Graphics_Info.PixelsPerScanLine;
	kparam.arch_init_flags.frame_buffer_info = 1;

	kparam.nr_lcpu = bootinfo->efi_smp_info.core_available;
	uint64_t lcpu_count = 0;
	for (i = 0; i < bootinfo->efi_smp_info.core_num; i++)
	{
		efi_cpudesc_s * curr_cpu = &bootinfo->efi_smp_info.cpus[i];
		if ((curr_cpu->status & 0x4) > 0)
		{
			apic_id[lcpu_count] = curr_cpu->proccessor_id & 0xFF;
			smp_topos[lcpu_count].not_use = 0;
			smp_topos[lcpu_count].pack_id = curr_cpu->pack_id;
			smp_topos[lcpu_count].core_id = curr_cpu->core_id;
			smp_topos[lcpu_count].thd_id  = curr_cpu->thd_id;
			lcpu_count++;
		}
	}
	kparam.arch_init_flags.smp_info = 1;

	cpuid_info();
}

void cpuid_info(void)
{
	unsigned int CpuFacName[4] = {0,0,0,0};

	//vendor_string
	cpuid(0,0,&CpuFacName[0],&CpuFacName[1],&CpuFacName[2],&CpuFacName[3]);
	*(unsigned int*)&cpuinfo.id_string[0] = CpuFacName[1];
	*(unsigned int*)&cpuinfo.id_string[4] = CpuFacName[3];
	*(unsigned int*)&cpuinfo.id_string[8] = CpuFacName[2];	
	cpuinfo.id_string[12] = '\0';
	
	//brand_string
	int i = 0, j = -4;
	for(i = 0x80000002;i < 0x80000005;i++)
	{
		cpuid(i,0,&CpuFacName[0],&CpuFacName[1],&CpuFacName[2],&CpuFacName[3]);
		*(unsigned int*)&cpuinfo.cpu_model[j += 4] = CpuFacName[0];
		*(unsigned int*)&cpuinfo.cpu_model[j += 4] = CpuFacName[1];
		*(unsigned int*)&cpuinfo.cpu_model[j += 4] = CpuFacName[2];
		*(unsigned int*)&cpuinfo.cpu_model[j += 4] = CpuFacName[3];
	}
	cpuinfo.id_string[j += 4] = '\0';

	//Version Informatin Type,Family,Model,and Stepping ID
	cpuid(1,0,&CpuFacName[0],&CpuFacName[1],&CpuFacName[2],&CpuFacName[3]);
	cpuinfo.family_code = CpuFacName[0] >> 8 & 0xf;
	cpuinfo.extend_family = CpuFacName[0] >> 20 & 0xff;
	cpuinfo.model_number = CpuFacName[0] >> 4 & 0xf;
	cpuinfo.extend_model = CpuFacName[0] >> 16 & 0xf;
	cpuinfo.processor_type = CpuFacName[0] >> 12 & 0x3;
	cpuinfo.stepping_id = CpuFacName[0] & 0xf;

	//get Linear/Physical Address size
	cpuid(0x80000008,0,&CpuFacName[0],&CpuFacName[1],&CpuFacName[2],&CpuFacName[3]);
	cpuinfo.physical_address_width = CpuFacName[0] & 0xff;
	cpuinfo.linear_address_width = CpuFacName[0] >> 8 & 0xff;

	//max cpuid operation code
	cpuid(0,0,&CpuFacName[0],&CpuFacName[1],&CpuFacName[2],&CpuFacName[3]);
	cpuinfo.max_basic_opcode = CpuFacName[0];

	cpuid(0x80000000,0,&CpuFacName[0],&CpuFacName[1],&CpuFacName[2],&CpuFacName[3]);
	cpuinfo.max_extend_opcode = CpuFacName[0];

	kparam.arch_init_flags.get_cpuid = 1;
}

void set_bsp_env()
{
	pre_init();

	init_bsp_arch_data();
	kparam.arch_init_flags.init_bsp_arch_data = 1;
	reload_bsp_arch_data();
	kparam.arch_init_flags.reload_bsp_arch_env = 1;

	reload_arch_page();
}

// void get_multiboot2_info(size_t multiboot2_info_base)
// {
// 	int mb2_tagsize = 0;
// 	virt_addr mb2info_start = phys2virt((phys_addr)multiboot2_info_base);
// 	virt_addr mb2info_end = mb2info_start + *((uint32_t *)mb2info_start);
// 	virt_addr mb2info_curr = mb2info_start;
// 	virt_addr mb2info_next = mb2info_start + 8;	// the head tag is 8byte long
// 	int mb2_tagtype = 0;
// 	while (mb2info_next < mb2info_end)
// 	{
// 		mb2info_curr = mb2info_next;
// 		mb2_tagsize  = *(uint32_t *)(mb2info_curr + 4);
// 		mb2info_next = mb2info_curr + (size_t)mb2_tagsize;
// 		mb2info_next = (virt_addr)(((size_t)mb2info_next + 7) & 0xFFFFFFF8); // tag must start at 8byte addr
// 		mb2_tagtype = *(uint32_t *)mb2info_curr;
// 		// find e820 tag
// 		if (mb2_tagtype == MULTIBOOT_TAG_TYPE_MMAP)
// 		{
// 			struct multiboot_tag_mmap * mb2_mmap_p = (struct multiboot_tag_mmap *)mb2info_curr;
// 			size_t e820_entnum = (mb2_mmap_p->size -16) / mb2_mmap_p->entry_size;
// 			struct multiboot_mmap_entry * e820_entry_p = &mb2_mmap_p->entries[0];

// 			mem_info.mb_memmap_nr = e820_entnum;
// 			for (int i = 0; (virt_addr)e820_entry_p < mb2info_next; e820_entry_p++, i++)
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
// 			struct multiboot_tag_framebuffer_common * mb2_fb_p = (struct multiboot_tag_framebuffer_common *)(mb2info_curr + 16);
// 			framebuffer.FB_phybase = (phys_addr)mb2_fb_p->framebuffer_addr;
// 			framebuffer.FB_size	   = mb2_fb_p->size;
// 			framebuffer.X_Resolution = mb2_fb_p->framebuffer_width;
// 			framebuffer.Y_Resolution = mb2_fb_p->framebuffer_height;
// 		}
// 	}
// }

