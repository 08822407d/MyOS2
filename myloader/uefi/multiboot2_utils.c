#include <Uefi.h>

#include "bootloader.h"
#include "multiboot2.h"

mb2_hdr_s * mb2_hdr_p = NULL;
mb2_hdr_tag_kaddr_s * mb2_kaddr_p = NULL;


void parse_mb2_header(EFI_PHYSICAL_ADDRESS start_addr)
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

mbi_tag_s *init_info_tag_addr(PHYSICAL_ADDRESS start_addr)
{
	return (mbi_tag_s *) (start_addr + sizeof(mbi_tags_header_s));
}

mbi_tag_s *next_info_tag_addr(mbi_tag_s *prev_info_tag)
{
	return (mbi_tag_s *) ((UINT8 *) prev_info_tag +
				((prev_info_tag->size + 7) & ~7));
}

mbi_tag_s *fill_info_tag_end(mbi_tag_s *info_tag_ptr)
{
	info_tag_ptr->type = MULTIBOOT_TAG_TYPE_END;
	info_tag_ptr->size = sizeof(mbi_tag_s);
	return next_info_tag_addr(info_tag_ptr);
}

void fill_info_tag_header(mbi_tags_header_s *mbi_tags_header_ptr,
		PHYSICAL_ADDRESS start_addr, mbi_tag_s *info_tag_end)
{
	mbi_tags_header_ptr->total_size = (PHYSICAL_ADDRESS)info_tag_end - start_addr;
	mbi_tags_header_ptr->reserved = 0;
}