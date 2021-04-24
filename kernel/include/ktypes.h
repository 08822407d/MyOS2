#ifndef _K_TYPES_H_
#define _K_TYPES_H_

#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/utils.h>

#include "../arch/amd64/include/archconst.h"
#include "../arch/amd64/include/keyboard.h"
#include "../arch/amd64/include/multiboot2.h"

	#define MAXMEMZONE		64

	typedef void*	phy_addr;
	typedef void*	vir_addr;

	struct  Slab;
	typedef struct Slab Slab_s;
	/* for page management */
	typedef struct Page
	{
		struct MemZone *zone_belonged;
		phy_addr		page_start_addr;
		
		unsigned long	attr;
		unsigned long	ref_count;
		unsigned long	age;

		Slab_s *		slab_ptr;
	} Page_s;

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

	struct Slab_Cache;
	typedef struct Slab_Cache Slab_Cache_s;
	typedef struct Slab
	{
		List_s 			slab_list;
		Slab_Cache_s *	slabcache_ptr;
		unsigned long	total;
		unsigned long	free;
		vir_addr		vir_addr;
		Page_s *		page;
		bitmap_t *		colormap;
	} Slab_s;

	typedef struct Slab_Cache
	{
		List_s			list;
		unsigned long	obj_size;

		unsigned long	nslab_count;
		Slab_s *		normal_slab;
		unsigned long	nsobj_free_count;

		unsigned long	dslab_count;
		Slab_s *		dma_slab;
		unsigned long	dsobj_free_count;
	} Slab_Cache_s;
	
	typedef struct keyboard_inputbuffer
	{
		unsigned char * p_head;
		unsigned char * p_tail;
		int count;
		unsigned char buf[KB_BUF_SIZE];
	} kbd_inbuf_s;

#endif /* _K_TYPES_H_ */