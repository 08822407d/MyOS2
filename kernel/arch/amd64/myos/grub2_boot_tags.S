	.align	8
entry_address_tag_start:
	.short	MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
	.short	MULTIBOOT_HEADER_TAG_OPTIONAL
	.long	entry_address_tag_end - entry_address_tag_start
	.long	multiboot_entry
entry_address_tag_end:
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

