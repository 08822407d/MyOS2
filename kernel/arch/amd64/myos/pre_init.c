#include <linux/kernel/types.h>
#include <linux/kernel/math.h>
#include <linux/mm/memblock.h>
#include <linux/lib/string.h>

#include <asm/e820-types.h>

#include <obsolete/arch_proto.h>
#include <obsolete/archconst.h>
#include <obsolete/arch_glo.h>

#include <obsolete/proto.h>

#include <uefi/multiboot2.h>
#include <uefi/bootloader.h>

extern uint64_t	boot_from_grub2;

kinfo_s			kparam;
framebuffer_s	framebuffer;

uint64_t		apic_id[CONFIG_NR_CPUS];
struct cputopo	smp_topos[CONFIG_NR_CPUS];

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

	memblock_add(framebuffer.FB_phybase, framebuffer.FB_size);
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

void myos_early_init_sytem(void)
{
	struct KERNEL_BOOT_PARAMETER_INFORMATION * machine_info =
			(struct KERNEL_BOOT_PARAMETER_INFORMATION *)MACHINE_CONF_ADDR;
	extern e820_entry_s *e820_table;
	e820_table = (e820_entry_s *)machine_info->mb_mmap;
	get_VBE_info(&machine_info->mb_fb_common);
	get_SMP_info(&machine_info->efi_smp_info);
}

