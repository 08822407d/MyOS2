//==========================================
	.align	8
tag_info_memmap_start:
	.short	MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST
	.short	0
	.long	tag_info_memmap_end - tag_info_memmap_start
	.long	MULTIBOOT_TAG_TYPE_MMAP
tag_info_memmap_end:
//============================================
	.align	8
tag_info_vbe_start:
	.short	MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST
	.short	0
	.long	tag_info_vbe_end - tag_info_vbe_start
	.long	MULTIBOOT_TAG_TYPE_VBE
tag_info_vbe_end:
//===========================================
	.align	8
tag_info_framebuffer_start:
	.short	MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST
	.short	0
	.long	tag_info_framebuffer_end - tag_info_framebuffer_start
	.long	MULTIBOOT_TAG_TYPE_FRAMEBUFFER
tag_info_framebuffer_end:
//===========================================



