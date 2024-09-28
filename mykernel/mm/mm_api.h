#ifndef _MM_API_H_
#define _MM_API_H_

	#include "mm_type_declaration.h"
	#include <linux/kernel/mmap_lock.h>
	#include <asm/mm_api.h>


	#include "early/earlymem_api.h"
	#include "fault/memfault_api.h"
	#include "highmem/highmem_api.h"
	#include "kmalloc/kmalloc_api.h"
	#include "misc/mm_misc_api.h"
	#include "page_alloc/page_alloc_api.h"
	#include "vm_map/vm_map_api.h"


	#include <asm-generic/getorder.h>
	#include <linux/kernel/err.h>
	#include <linux/kernel/overflow.h>
	#include <linux/kernel/sched_api.h>
	#include <linux/kernel/irqflags.h>

#endif /* _MM_API_H_ */