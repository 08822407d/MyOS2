#include <linux/mm/gfp.h>
// #include <linux/initrd.h>
// #include <linux/ioport.h>
// #include <linux/swap.h>
#include <linux/mm/memblock.h>
// #include <linux/swapfile.h>
// #include <linux/swapops.h>
// #include <linux/kmemleak.h>
#include <linux/sched/task.h>

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


// this value is also loaded by APboot assembly code
phys_addr_t kernel_cr3 = 0;

pud_t	*init_pud_buf;
pmd_t	*init_pmd_buf;
pte_t	*init_pte_buf;

void __init myos_early_alloc_pgt_buf(void)
{
	int ent_size = sizeof(pte_t);
	long ptebuf_pgcount = PFN_UP(max_pfn * ent_size);
	long pmdbuf_pgcount = PFN_UP(ptebuf_pgcount * ent_size);
	long pudbuf_pgcount = PFN_UP(pmdbuf_pgcount * ent_size);

	init_pud_buf = extend_brk(pudbuf_pgcount * PAGE_SIZE, PAGE_SIZE);
	init_pmd_buf = extend_brk(pmdbuf_pgcount * PAGE_SIZE, PAGE_SIZE);
	init_pte_buf = extend_brk(ptebuf_pgcount * PAGE_SIZE, PAGE_SIZE);

	// fill pml4 entries
	for (int pgt_idx = 0; pgt_idx < pudbuf_pgcount; pgt_idx++)
		init_top_pgt[pgt_idx] = arch_make_pgd(_PAGE_TABLE | __pa(init_pud_buf + pgt_idx * PTRS_PER_PUD));
	// fill pdpt entries
	for (int pud_idx = 0; pud_idx < pmdbuf_pgcount; pud_idx++)
		init_pud_buf[pud_idx] = arch_make_pud(_PAGE_TABLE | __pa(init_pmd_buf + pud_idx * PTRS_PER_PMD));
	// fill pdt entries
	for (int pmd_idx = 0; pmd_idx < ptebuf_pgcount; pmd_idx++)
		init_pmd_buf[pmd_idx] = arch_make_pmd(_PAGE_TABLE | __pa(init_pte_buf + pmd_idx * PTRS_PER_PTE));

	memcpy(init_top_pgt + PGD_KERNEL_START, init_top_pgt, PAGE_SIZE / 2);
}


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
static void __init myos_memory_map()
{
	unsigned long start_pfn, end_pfn;
	unsigned long mapped_ram_size = 0;
	int i;

	for_each_mem_pfn_range(i, &start_pfn, &end_pfn) {
		u64 start = PFN_PHYS(start_pfn);
		u64 end = PFN_PHYS(end_pfn);
		if (start >= end)
			continue;

		myos_kernel_physical_mapping_init(start, end);
		mapped_ram_size += end - start;
	}

	kernel_cr3 = myos_virt2phys((virt_addr_t)init_top_pgt);
}

void __init init_mem_mapping(void)
{
	unsigned long end;

	// pti_check_boottime_disable();
	// probe_page_size_mask();
	// setup_pcid();

	end = max_pfn << PAGE_SHIFT;

	/* the ISA range is always mapped regardless of memory holes */
	// init_memory_mapping(0, ISA_END_ADDRESS);

	// /* Init the trampoline, possibly with KASLR memory offset */
	// init_trampoline();

	myos_memory_map();

	if (max_pfn > max_low_pfn) {
		/* can we preserve max_low_pfn ?*/
		max_low_pfn = max_pfn;
	}

	load_cr3(swapper_pg_dir);
	// __flush_tlb_all();
	// early_memtest(0, max_pfn_mapped << PAGE_SHIFT);
}