	.align	8
header_tag_entry_address_start:
	.short	MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
	.short	MULTIBOOT_HEADER_TAG_OPTIONAL
	.long	header_tag_entry_address_end - header_tag_entry_address_start
	.long	multiboot_entry
header_tag_entry_address_end:
//===========================================
// 	.align	8
// SYM_CODE_START(multiboot_tag_framebuffer_start)
// 	.long	MULTIBOOT_TAG_TYPE_FRAMEBUFFER
// 	.long	multiboot_tag_framebuffer_end - multiboot_tag_framebuffer_start
// 	.quad	0
// 	.long	0
// 	.long	0
// 	.long	0
// 	.byte	0
// 	.byte	0
// 	.short	0
// multiboot_tag_framebuffer_common_end:
// 	.byte	0
// 	.byte	0
// 	.byte	0
// 	.byte	0
// 	.byte	0
// 	.byte	0
// multiboot_tag_framebuffer_end:
//===========================================
// use grub2 derectly boot into long mode failed
// can't find reason why step into fisrt instruction of
// my code entry will cause cpu reset
//===========================================
// 	.align	8
// efi_boot_service_tag_start:
// 	.short	MULTIBOOT_HEADER_TAG_EFI_BS
// 	.short	MULTIBOOT_HEADER_TAG_OPTIONAL
// 	.long	efi_boot_service_tag_end - efi_boot_service_tag_start
// efi_boot_service_tag_end:
//===========================================
// 	.align	8
// efi_amd64_entry_address_tag_start:
// 	.short	MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI64
// 	.short	MULTIBOOT_HEADER_TAG_OPTIONAL
// 	.long	efi_amd64_entry_address_tag_end - efi_amd64_entry_address_tag_start
// 	.long	code_64_start
// efi_amd64_entry_address_tag_end:

