#include <Uefi.h>

#include "bootloader.h"
#include "multiboot2.h"

mb2_hdr_s * mb2_hdr_p = NULL;
mb2_hdr_tag_kaddr_s * mb2_kaddr_p = NULL;


void parse_multiboot2(EFI_PHYSICAL_ADDRESS start_addr)
{
	PHYSICAL_ADDRESS hdr_addr = start_addr;

	UINT64 mb2_hdr_size = 0;

	for (int i = 0; i < MULTIBOOT_SEARCH; i += MULTIBOOT_TAG_ALIGN)
	{
		UINT32 header_tag = *(UINT32 *)(start_addr + i);
		if (header_tag == MULTIBOOT2_HEADER_MAGIC)
		{
			hdr_addr = start_addr + i;
			mb2_hdr_p = (mb2_hdr_s *)hdr_addr;
			mb2_hdr_size = mb2_hdr_p->header_length;
			hdr_addr += sizeof(mb2_hdr_s);
		}
	}

	while (hdr_addr < (start_addr + mb2_hdr_size))
	{
		mb2_hdr_tag_s * tag_p = (mb2_hdr_tag_s *)hdr_addr;
		UINT32 tag_size = tag_p->size;
		tag_size = (tag_size + MULTIBOOT_TAG_ALIGN - 1) & ~(MULTIBOOT_TAG_ALIGN - 1);

		PHYSICAL_ADDRESS hdr_next = hdr_addr + tag_size;

		// Print(L"MB2 header tag : 0x%X - 0x%X .\n", hdr_addr, tag_size);
		Print(L"Tag type 0x%X.\n", tag_p->type);
		switch (tag_p->type)
		{
			case MULTIBOOT_HEADER_TAG_ADDRESS:
				mb2_kaddr_p = (mb2_hdr_tag_kaddr_s *)hdr_addr;
				Print(L"kend : 0x%X.\nkbss : 0x%X.\n", mb2_kaddr_p->load_end_addr, mb2_kaddr_p->bss_end_addr);
				break;

			case MULTIBOOT_HEADER_TAG_END:
				/* code */
				break;
			
			default:
				break;
		}

		hdr_addr = hdr_next;
	}
	
}