#include <linux/mm/gfp.h>
// #include <linux/initrd.h>
// #include <linux/ioport.h>
// #include <linux/swap.h>
#include <linux/mm/memblock.h>
// #include <linux/swapfile.h>
// #include <linux/swapops.h>
// #include <linux/kmemleak.h>
// #include <linux/sched/task.h>

// #include <asm/set_memory.h>
#include <asm/e820-api.h>
// #include <asm/init.h>
#include <asm/page.h>
#include <asm/page_types.h>
// #include <asm/sections.h>
#include <asm/setup.h>
// #include <asm/tlbflush.h>
// #include <asm/tlb.h>
// #include <asm/proto.h>
// #include <asm/dma.h>		/* for MAX_DMA_PFN */
// #include <asm/microcode.h>
// #include <asm/kaslr.h>
// #include <asm/hypervisor.h>
// #include <asm/cpufeature.h>
// #include <asm/pti.h>
// #include <asm/text-patching.h>
// #include <asm/memtype.h>

// #include "mm_internal.h"


#include <obsolete/arch_proto.h>

/**
 * memory_map_bottom_up - Map [map_start, map_end) bottom up
 * @map_start: start address of the target memory range
 * @map_end: end address of the target memory range
 *
 * This function will setup direct mapping for memory range
 * [map_start, map_end) in bottom-up. Since we have limited the
 * bottom-up allocation above the kernel, the page tables will
 * be allocated just above the kernel and we map the memory
 * in [map_start, map_end) in bottom-up.
 */
static void __init memory_map_bottom_up(
		unsigned long map_start, unsigned long map_end)
{
	// unsigned long next, start;
	// unsigned long mapped_ram_size = 0;
	// /* step_size need to be small so pgt_buf from BRK could cover it */
	// unsigned long step_size = PMD_SIZE;

	// start = map_start;
	// min_pfn_mapped = start >> PAGE_SHIFT;

	// /*
	//  * We start from the bottom (@map_start) and go to the top (@map_end).
	//  * The memblock_find_in_range() gets us a block of RAM from the
	//  * end of RAM in [min_pfn_mapped, max_pfn_mapped) used as new pages
	//  * for page table.
	//  */
	// while (start < map_end) {
	// 	if (step_size && map_end - start > step_size) {
	// 		next = round_up(start + 1, step_size);
	// 		if (next > map_end)
	// 			next = map_end;
	// 	} else {
	// 		next = map_end;
	// 	}

	// 	mapped_ram_size += init_range_memory_mapping(start, next);
	// 	start = next;

	// 	if (mapped_ram_size >= step_size)
	// 		step_size = get_new_step_size(step_size);
	// }

	extern void myos_init_arch_page();
	myos_init_arch_page();
}

void __init init_mem_mapping(void)
{
	unsigned long end;

	// pti_check_boottime_disable();
	// probe_page_size_mask();
	// setup_pcid();

	end = max_pfn << PAGE_SHIFT;

	/* the ISA range is always mapped regardless of memory holes */
	// init_memory_mapping(0, ISA_END_ADDRESS, PAGE_KERNEL);

	// /* Init the trampoline, possibly with KASLR memory offset */
	// init_trampoline();

	memory_map_bottom_up(ISA_END_ADDRESS, end);

	if (max_pfn > max_low_pfn) {
		/* can we preserve max_low_pfn ?*/
		max_low_pfn = max_pfn;
	}

	// load_cr3(swapper_pg_dir);
	// __flush_tlb_all();

	// x86_init.hyper.init_mem_mapping();

	// early_memtest(0, max_pfn_mapped << PAGE_SHIFT);
}