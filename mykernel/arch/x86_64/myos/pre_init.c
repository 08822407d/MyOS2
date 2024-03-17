#include <linux/kernel/types.h>
#include <linux/kernel/math.h>
#include <linux/lib/string.h>

#include <asm/apic.h>

#include <obsolete/arch_proto.h>
#include <obsolete/proto.h>

#include <uefi/multiboot2.h>
#include <uefi/bootloader.h>

extern uint64_t	boot_from_grub2;
extern u64 mbi_magic;
extern u64 mbi_base;

efi_machine_conf_s	*machine_info;
unsigned			nr_lcpu;
mbi_mmap_ent_s		*ram_map;
framebuffer_s		framebuffer;
mbi_tags_header_s	*mbi_tag_header;


void parse_tag (unsigned long magic, unsigned long addr);

static void get_framebuffer_info(mbi_framebuffer_s* framebuffer_info)
{
	mbi_framebuffer_common_s *common = &framebuffer_info->common;
	framebuffer.FB_phybase = common->framebuffer_addr;
	framebuffer.FB_virbase = phys_to_virt(framebuffer.FB_phybase);
	framebuffer.FB_size = common->framebuffer_width * common->framebuffer_height * common->framebuffer_bpp / 8;
	framebuffer.X_Resolution = common->framebuffer_width;
	framebuffer.Y_Resolution = common->framebuffer_height;
	framebuffer.PixperScanline = common->framebuffer_pitch / (common->framebuffer_bpp / 8);
}

void myos_early_init_system(void)
{
	u64 *virt_mbi_magic_ptr = (u64 *)phys_to_virt((phys_addr_t)&mbi_magic);
	u64 *virt_mbi_base_ptr = (u64 *)phys_to_virt((phys_addr_t)&mbi_base);

	parse_tag(*(unsigned long *)virt_mbi_magic_ptr, *(unsigned long *)virt_mbi_base_ptr);

	nr_lcpu = 1;
	// while (1);
}


void
parse_tag (unsigned long magic, unsigned long addr)
{
	mbi_tag_s *tag;
	unsigned size;

	/*  Am I booted by a Multiboot-compliant boot loader? */
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
		return;
	}

	if (addr & 7) {
		return;
	}

	mbi_tag_header = (mbi_tags_header_s *) addr;
	size = mbi_tag_header->total_size;
	for (tag = (mbi_tag_s *) (addr + 8); tag->type != MULTIBOOT_TAG_TYPE_END;
		tag = (mbi_tag_s *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7)))
	{
		switch (tag->type)
		{
		case MULTIBOOT_TAG_TYPE_CMDLINE:
			char *mbi_cmdline = ((mbi_string_s *)tag)->string;
			break;
		case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
			char *mbi_loader_name = ((mbi_string_s *)tag)->string;
			break;
		case MULTIBOOT_TAG_TYPE_MODULE:
			mbi_module_s *mbi_module = (mbi_module_s *)tag;
			break;
		case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
			mbi_basic_meminfo_s *mbi_meminfo = (mbi_basic_meminfo_s *)tag;
			break;
		case MULTIBOOT_TAG_TYPE_BOOTDEV:
			mbi_bootdev_s *mbi_bootdev = (mbi_bootdev_s *)tag;
			break;
		case MULTIBOOT_TAG_TYPE_MMAP:
			mbi_mmap_s *mbi_mmap = (mbi_mmap_s *)tag;
			ram_map = (mbi_mmap_ent_s *)phys_to_virt((phys_addr_t)mbi_mmap->entries);
			break;
		case MULTIBOOT_TAG_TYPE_VBE:
			mbi_vbe_s *mbi_mbi_mmap_svbe = (mbi_vbe_s *)tag;
			break;
		case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
			mbi_framebuffer_s *mbi_fb = (mbi_framebuffer_s *)tag;
			get_framebuffer_info(mbi_fb);
			break;
		case MULTIBOOT_TAG_TYPE_EFI64:
			mbi_efi64_s *mbi_efi64 = (mbi_efi64_s *)tag;
			break;
		case MULTIBOOT_TAG_TYPE_ACPI_NEW:
			mbi_acpi_new_s *mbi_acpi_new = (mbi_acpi_new_s *)tag;
			break;
		case MULTIBOOT_TAG_TYPE_EFI64_IH:
			mbi_efi64_ih_s *mbi_efi64_ih = (mbi_efi64_ih_s *)tag;
			break;
		case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
			mbi_load_base_addr_s *mbi_load_base_addr = (mbi_load_base_addr_s *)tag;
			break;
		}
	}
}    