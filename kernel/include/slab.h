#ifndef _SLAB_H_
#define _SLAB_H_

#include <sys/types.h>
#include <lib/utils.h>

#include "const.h"
#include "proto.h"

#include "../arch/amd64/include/archconst.h"

typedef struct
{
	List_s 			list;
	unsigned long	total;
	unsigned long	free;
	vir_addr		vir_addr;
	page_s *		page;
	bitmap_t *		colormap;
} Slab_s;

typedef struct
{
	List_s			list;
	unsigned long	obj_size;
	Slab_s *		normal_slab;
	Slab_s *		dma_slab;
} Slab_Cache_s;

#endif /* _SLAB_H_ */