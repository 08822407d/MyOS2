#include <linux/kernel/types.h>
#include <linux/kernel/math.h>
#include <linux/mm/memblock.h>
#include <linux/lib/string.h>

#include <asm/e820-types.h>

#include "include/arch_proto.h"
#include "include/archconst.h"
#include "include/arch_glo.h"

#include <obsolete/proto.h>

#include <uefi/multiboot2.h>
#include <uefi/bootloader.h>

extern char _k_phys_start;
extern char _k_virt_start;

extern char _bss;
extern char _ebss;
extern char _end;

extern uint64_t	boot_from_grub2;

kinfo_s			kparam;
cpu_info_s		cpuinfo;
framebuffer_s	framebuffer;

uint64_t		apic_id[CONFIG_NR_CPUS];
struct cputopo	smp_topos[CONFIG_NR_CPUS];


static void enable_AMD_syscall()
{
	// enalble syscall/sysret machanism
	uint64_t ia32_efer = rdmsr(IA32_EFER);
	ia32_efer |= MSR_IA32_EFER_SCE;	// bit0: SCE , enable syscall/sysret
	wrmsr(IA32_EFER, ia32_efer);
}

static void get_VBE_info(mb_fb_common_s * vbe_info)
{
	framebuffer.FB_phybase = vbe_info->framebuffer_addr;
	framebuffer.FB_virbase = myos_phys2virt(framebuffer.FB_phybase);
	framebuffer.FB_size = vbe_info->size;
	framebuffer.X_Resolution = vbe_info->framebuffer_pitch;
	framebuffer.X_Resolution = vbe_info->framebuffer_width;
	framebuffer.Y_Resolution = vbe_info->framebuffer_height;
	framebuffer.PixperScanline = vbe_info->framebuffer_pitch;
	// set init flag
	kparam.arch_init_flags.framebuffer = 1;

	memblock_reserve(framebuffer.FB_phybase, framebuffer.FB_size);
}

static void get_SMP_info(efi_smpinfo_s * smp_info)
{
	kparam.nr_lcpu = smp_info->core_available;
	uint64_t lcpu_count = 0;
	for (int i = 0; i < smp_info->core_num; i++)
	{
		efi_cpudesc_s * this_cpu = &smp_info->cpus[i];
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

void myos_early_init_sytem(void)
{
	memset((void *)&_bss, 0, &_ebss - &_bss);
	enable_AMD_syscall();

	struct KERNEL_BOOT_PARAMETER_INFORMATION * machine_info =
			(struct KERNEL_BOOT_PARAMETER_INFORMATION *)MACHINE_CONF_ADDR;
	extern e820_entry_s *e820_table;
	e820_table = (e820_entry_s *)machine_info->mb_mmap;
	get_VBE_info(&machine_info->mb_fb_common);
	get_SMP_info(&machine_info->efi_smp_info);

	cpuid_info();
}

