#ifndef _K_TYPES_H_
#define _K_TYPES_H_

#include <stdint.h>

#include <sys/cdefs.h>
#include <lib/utils.h>

#include "task.h"

#include <arch/amd64/include/archconst.h>
#include <arch/amd64/include/keyboard.h>
#include <arch/amd64/include/multiboot2.h>

	#define MAXMEMZONE		64

	struct  slab;
	typedef struct slab slab_s;

	struct	slab_cache;
	typedef struct slab_cache slab_cache_s;
/*==============================================================================================*
 *									physical page management									*
 *==============================================================================================*/
	typedef struct Page
	{
		struct MemZone *zone_belonged;
		phys_addr		page_start_addr;
		
		unsigned long	attr;
		unsigned long	ref_count;
		unsigned long	map_count;
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

		Page_s			pages[PDE_NR];
		bitmap_t		page_bitmap[PDE_NR / BITMAP_UNITSIZE];
		unsigned long	page_total_nr;

		memzone_s		memzones[MAXMEMZONE];
		unsigned long	memzone_total_nr;	
	} memory_info_s;

/*==============================================================================================*
 *									kernel malloc and slab										*
 *==============================================================================================*/
	m_define_list_header(slab);

	typedef struct slab
	{
		slab_s *		prev;
		slab_s *		next;

		slab_list_s *	list_header;

		slab_cache_s *	slabcache_ptr;

		unsigned long	total;
		unsigned long	free;

		virt_addr		virt_addr;
		Page_s *		page;
		bitmap_t *		colormap;
	} slab_s;

	m_define_list_header(slab_cache);

	typedef struct slab_cache
	{
		List_s			slabcache_list;
		slab_cache_list_s *	list_header;

		unsigned long	obj_size;

		unsigned long	normal_slab_total;
		slab_list_s		normal_slab_free;
		slab_list_s		normal_slab_used;
		slab_list_s		normal_slab_full;
		unsigned long	nsobj_free_count;
		unsigned long	nsobj_used_count;

		// the base slab should not be freed
		slab_s *		normal_base_slab;
	} slab_cache_s;
	
/*==============================================================================================*
 *									data structures for devices									*
 *==============================================================================================*/
	typedef struct keyboard_inputbuffer
	{
		unsigned char * p_head;
		unsigned char * p_tail;
		int count;
		unsigned char buf[KB_BUF_SIZE];
	} kbd_inbuf_s;


#endif /* _K_TYPES_H_ */