#include <sys/param.h>

#include <sys/types.h>
#include <string.h>

#include <include/glo.h>
#include <include/ktypes.h>

#include "include/bootinfo.h"
#include "include/arch_glo.h"
#include "include/arch_proto.h"
#include "include/archconst.h"
#include "include/multiboot2.h"

#include <include/proto.h>
#include <include/memblock.h>
#include <include/math.h>

extern char _k_phys_start;
extern char _k_virt_start;

extern char _bss;
extern char _ebss;
extern char _end;

extern uint64_t	boot_from_grub2;

kinfo_s			kparam;
cpu_info_s		cpuinfo;
framebuffer_s	framebuffer;

uint64_t	apic_id[CONFIG_MAX_CPUS];
struct cputopo	smp_topos[CONFIG_MAX_CPUS];


static void enable_AMD_syscall()
{
	// enalble syscall/sysret machanism
	uint64_t ia32_efer = rdmsr(IA32_EFER);
	ia32_efer |= MSR_IA32_EFER_SCE;	// bit0: SCE , enable syscall/sysret
	wrmsr(IA32_EFER, ia32_efer);
}

static void get_VBE_info(struct KERNEL_BOOT_PARAMETER_INFORMATION *bootinfo)
{
	framebuffer.FB_phybase = (phys_addr_t)bootinfo->efi_graphics_info.FrameBufferBase;
	framebuffer.FB_virbase = phys2virt(framebuffer.FB_phybase);
	framebuffer.FB_size = bootinfo->efi_graphics_info.FrameBufferSize;
	// framebuffer.X_Resolution = bootinfo->efi_graphics_info.HorizontalResolution;
	framebuffer.X_Resolution = bootinfo->efi_graphics_info.PixelsPerScanLine;
	framebuffer.Y_Resolution = bootinfo->efi_graphics_info.VerticalResolution;
	framebuffer.PixperScanline = bootinfo->efi_graphics_info.PixelsPerScanLine;
	// set init flag
	kparam.arch_init_flags.frame_buffer_info = 1;
}

static void init_memblock(struct KERNEL_BOOT_PARAMETER_INFORMATION *bootinfo)
{
	kparam.kernel_phy_base	= &_k_phys_start;
	kparam.kernel_vir_base	= &_k_virt_start;
	kparam.kernel_vir_end	= &_end;

	int i;
	efi_e820entry_s * efi_e820;
	for (i = 0; bootinfo->efi_e820_info.e820_entry[i].length != 0; i++)
	{
		efi_e820 = &bootinfo->efi_e820_info.e820_entry[i];
		if (efi_e820->type == 1 && efi_e820->length != 0)
		{
			memblock_add((phys_addr_t)efi_e820->address, efi_e820->length);
		}
	}
	kparam.max_phys_mem = efi_e820->address + efi_e820->length;
	if (((size_t)framebuffer.FB_phybase + framebuffer.FB_size) > kparam.max_phys_mem)
		kparam.max_phys_mem = (size_t)framebuffer.FB_phybase + framebuffer.FB_size;
	max_low_pfn =
	kparam.phys_page_nr = round_up(kparam.max_phys_mem, PAGE_SIZE) / PAGE_SIZE;

	// some part of memmory space is reserved
	memblock_reserve(0, 16 * CONST_1M);
	memblock_reserve((phys_addr_t)round_down((size_t)kparam.kernel_phy_base, PAGE_SIZE),
					round_up((size_t)kparam.kernel_vir_end, PAGE_SIZE) -
					round_down((size_t)kparam.kernel_vir_base, PAGE_SIZE));
}

static void init_memblock1(struct KERNEL_BOOT_PARAMETER_INFORMATION *bootinfo)
{
	kparam.kernel_phy_base	= &_k_phys_start;
	kparam.kernel_vir_base	= &_k_virt_start;
	kparam.kernel_vir_end	= &_end;

	int i;
	multiboot_memory_map_s * mb_mmap_ent;
	for (i = 0; bootinfo->efi_e820_info.mb_mmap[i].len != 0; i++)
	{
		mb_mmap_ent = &bootinfo->efi_e820_info.mb_mmap[i];
		if (mb_mmap_ent->type == 1 && mb_mmap_ent->len != 0)
		{
			memblock_add((phys_addr_t)mb_mmap_ent->addr, mb_mmap_ent->len);
		}
	}
	kparam.max_phys_mem = mb_mmap_ent->addr + mb_mmap_ent->len;
	if (((size_t)framebuffer.FB_phybase + framebuffer.FB_size) > kparam.max_phys_mem)
		kparam.max_phys_mem = (size_t)framebuffer.FB_phybase + framebuffer.FB_size;
	max_low_pfn =
	kparam.phys_page_nr = round_up(kparam.max_phys_mem, PAGE_SIZE) / PAGE_SIZE;

	// some part of memmory space is reserved
	memblock_reserve(0, 16 * CONST_1M);
	memblock_reserve((phys_addr_t)round_down((size_t)kparam.kernel_phy_base, PAGE_SIZE),
					round_up((size_t)kparam.kernel_vir_end, PAGE_SIZE) -
					round_down((size_t)kparam.kernel_vir_base, PAGE_SIZE));
}

static void get_SMP_info(struct KERNEL_BOOT_PARAMETER_INFORMATION *bootinfo)
{
	kparam.nr_lcpu = bootinfo->efi_smp_info.core_available;
	uint64_t lcpu_count = 0;
	for (int i = 0; i < bootinfo->efi_smp_info.core_num; i++)
	{
		efi_cpudesc_s * this_cpu = &bootinfo->efi_smp_info.cpus[i];
		if ((this_cpu->status & 0x4) > 0)
		{
			apic_id[lcpu_count] = this_cpu->proccessor_id & 0xFF;
			smp_topos[lcpu_count].not_use = 0;
			smp_topos[lcpu_count].pack_id = this_cpu->pack_id;
			smp_topos[lcpu_count].core_id = this_cpu->core_id;
			smp_topos[lcpu_count].thd_id  = this_cpu->thd_id;
			lcpu_count++;
		}
	}
}

static void cpuid_info(void)
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
}

void pre_init(void)
{
	memset((virt_addr_t)&_bss, 0, &_ebss - &_bss);

	enable_AMD_syscall();
	get_VBE_info((struct KERNEL_BOOT_PARAMETER_INFORMATION *)BOOTINFO_ADDR);
	init_memblock((struct KERNEL_BOOT_PARAMETER_INFORMATION *)BOOTINFO_ADDR);
	get_SMP_info((struct KERNEL_BOOT_PARAMETER_INFORMATION *)BOOTINFO_ADDR);

	cpuid_info();
}

