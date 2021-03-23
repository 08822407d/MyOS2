#ifndef _K_TYPES_H_
#define _K_TYPES_H_

#include <sys/types.h>
#include "../arch/amd64/include/archconst.h"
#include "../arch/amd64/include/multiboot2.h"

	#define MAXMEMZONE		64

	typedef uint64_t phy_addr;
	typedef uint64_t vir_addr;

	/* for page management */
	typedef struct Page
	{
		struct MemZone *zone_belonged;
		phy_addr		page_start_addr;
		
		unsigned long	attr;
		unsigned long	ref_count;
		unsigned long	age;
	} page_s;

	typedef struct MemZone
	{
		struct Page	*	page_zone;
		unsigned long	page_nr;

		phy_addr		zone_start_addr;
		phy_addr		zone_end_addr;
		size_t			zone_size;

		unsigned long	attr;
		unsigned long 	page_busy_nr;
		unsigned long	page_free_nr;
		unsigned long	page_total_ref;
	} memzone_s;

	typedef struct Memory_Info
	{
		multiboot_memory_map_s	mb_memmap[MAXMEMZONE];
		unsigned long	mb_memmap_nr;

		struct Page		pages[PAGE_NR];
		bitmap_t		page_bitmap[PAGE_NR / BITMAP_UNITSIZE];
		unsigned long	page_total_nr;

		struct MemZone	memzones[MAXMEMZONE];
		unsigned long	memzone_total_nr;	
	} memory_info_s;
	
#endif /* _K_TYPES_H_ */