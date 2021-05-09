#ifndef _K_TYPES_H_
#define _K_TYPES_H_

#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/utils.h>

#include "task.h"

#include "../arch/amd64/include/archconst.h"
#include "../arch/amd64/include/keyboard.h"
#include "../arch/amd64/include/multiboot2.h"

	#define MAXMEMZONE		64

	struct  slab;
	typedef struct slab slab_s;

	struct	slab_cache;
	typedef struct slab_cache slab_cache_s;
	/* for page management */
	typedef struct Page
	{
		struct MemZone *zone_belonged;
		phys_addr		page_start_addr;
		
		unsigned long	attr;
		unsigned long	ref_count;
		unsigned long	age;

		slab_s *		slab_ptr;
	} Page_s;

	typedef struct MemZone
	{
		struct Page	*	page_zone;
		unsigned long	page_nr;

		phys_addr		zone_start_addr;
		phys_addr		zone_end_addr;
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

	typedef struct slab
	{
		slab_s *		prev;
		slab_s *		next;

		slab_cache_s *	slabcache_ptr;

		unsigned long	total;
		unsigned long	free;

		virt_addr		virt_addr;
		Page_s *		page;
		bitmap_t *		colormap;
	} slab_s;

	typedef struct slab_cache
	{
		slab_cache_s *	prev;
		slab_cache_s *	next;

		unsigned long	obj_size;

		unsigned long	nslab_count;
		slab_s *		normal_slab;
		unsigned long	nsobj_free_count;
		unsigned long	nsobj_used_count;

		// unsigned long	dslab_count;
		// slab_s *		dma_slab;
		// unsigned long	dsobj_free_count;
		// unsigned long	dsobj_used_count;
	} slab_cache_s;
	
	typedef struct keyboard_inputbuffer
	{
		unsigned char * p_head;
		unsigned char * p_tail;
		int count;
		unsigned char buf[KB_BUF_SIZE];
	} kbd_inbuf_s;


#endif /* _K_TYPES_H_ */