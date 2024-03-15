//==========================================
	.align	8
header_tag_request_info_start:
	.short	MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST
	.short	0
	.long	header_tag_request_info_end - header_tag_request_info_start
	.long	MULTIBOOT_TAG_TYPE_MMAP
	.long	MULTIBOOT_TAG_TYPE_VBE
	.long	MULTIBOOT_TAG_TYPE_FRAMEBUFFER
	.long	MULTIBOOT_TAG_TYPE_EFI_MMAP
header_tag_request_info_end:
//============================================
//===========================================
	.align	8
header_tag_framebuffer_start:
	.short	MULTIBOOT_HEADER_TAG_FRAMEBUFFER
	.short	MULTIBOOT_HEADER_TAG_OPTIONAL
	.long	header_tag_framebuffer_end - header_tag_framebuffer_start
	.long	1920
	.long	1080
	.long	32
header_tag_framebuffer_end:
//===========================================
// MyOS2 only boot from EFI amd64 mode
	.align	8
header_tag_address_start:
	.short	MULTIBOOT_HEADER_TAG_ADDRESS
	.short	MULTIBOOT_HEADER_TAG_OPTIONAL
	.long	header_tag_address_end - header_tag_address_start
	/* header_addr */
	.long	multiboot_header
	/* load_addr */
	.long	_k_phys_start
	/* load_end_addr */
	.long	_k_phys_loadend
	/* bss end addr */
	.long	_k_phys_ebss
header_tag_address_end:
//===========================================
	.align	8
entry_address_tag_start:
	.short	MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
	.short	MULTIBOOT_HEADER_TAG_OPTIONAL
	.long	entry_address_tag_end - entry_address_tag_start
	.long	multiboot_entry
entry_address_tag_end:
//===========================================
	.align	8
efi_boot_service_tag_start:
	.short	MULTIBOOT_HEADER_TAG_EFI_BS
	.short	0
	.long	efi_boot_service_tag_end - efi_boot_service_tag_start
efi_boot_service_tag_end:
//===========================================
	.align	8
efi_amd64_entry_address_tag_start:
	.short	MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI64
	.short	0
	.long	efi_amd64_entry_address_tag_end - efi_amd64_entry_address_tag_start
	.long	code_64_start
efi_amd64_entry_address_tag_end:
//===========================================