#ifndef _K_TYPES_H_
#define _K_TYPES_H_

#include <sys/types.h>

#include <sys/cdefs.h>
#include <lib/utils.h>

#include <include/task.h>
#include <include/page.h>

#include <arch/amd64/include/archconst.h>
#include <arch/amd64/include/keyboard.h>
#include <arch/amd64/include/multiboot2.h>

	#define MAXMEMZONE		64

	typedef struct MemZone
	{
		struct Page	*	page_zone;
		unsigned long	page_nr;

		phys_addr_t		zone_start_addr;
		phys_addr_t		zone_end_addr;
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