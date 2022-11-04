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
#include <asm/sections.h>
#include <asm/setup.h>
// #include <asm/tlbflush.h>
// #include <asm/tlb.h>
// #include <asm/proto.h>
// #include <asm/dma.h>		/* for MAX_DMA_PFN */
// #include <asm/microcode.h>
// #include <asm/kaslr.h>
// #include <asm/hypervisor.h>
#include <asm/cpufeature.h>
// #include <asm/pti.h>
// #include <asm/text-patching.h>
// #include <asm/memtype.h>

// #include "mm_internal.h"


#include <linux/lib/string.h>
#include <obsolete/arch_proto.h>
#include <obsolete/ktypes.h>


// this value is also loaded by APboot assembly code
phys_addr_t kernel_cr3 = 0;

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
static void __init myos_memory_map_bottom_up(
		unsigned long map_start, unsigned long map_end)
{
	// static unsigned long __init init_range_memory_mapping(
	// 		unsigned long r_start, unsigned long r_end)
	// {
			unsigned long start_pfn, end_pfn;
			unsigned long mapped_ram_size = 0;
			int i;

			for_each_mem_pfn_range(i, &start_pfn, &end_pfn) {
				u64 start = PFN_PHYS(start_pfn);
				u64 end = PFN_PHYS(end_pfn);
				if (start >= end)
					continue;

				myos_init_memory_mapping(start, end - start);
				mapped_ram_size += end - start;
			}

			// return mapped_ram_size;
	// }

	// 因为页大小是2M，后面要用到第一页的物理地址，所以此处临时手动映射一下
	myos_init_memory_mapping(0, PAGE_SIZE);

	kernel_cr3 = myos_virt2phys((virt_addr_t)init_top_pgt);
	init_mm.pgd_ptr = kernel_cr3;
}

void __init init_mem_mapping(void)
{
	unsigned long end;

	// pti_check_boottime_disable();
	// probe_page_size_mask();
	// setup_pcid();

	end = max_pfn << PAGE_SHIFT;

	/* the ISA range is always mapped regardless of memory holes */
	// myos_init_memory_mapping(0, ISA_END_ADDRESS);

	// /* Init the trampoline, possibly with KASLR memory offset */
	// init_trampoline();

	myos_memory_map_bottom_up(ISA_END_ADDRESS, end);

	if (max_pfn > max_low_pfn) {
		/* can we preserve max_low_pfn ?*/
		max_low_pfn = max_pfn;
	}

	// load_cr3(swapper_pg_dir);
	load_cr3((reg_t)kernel_cr3);
	// __flush_tlb_all();
	// early_memtest(0, max_pfn_mapped << PAGE_SHIFT);
}