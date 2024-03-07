// source: linux-6.4.9

#include <linux/kernel/ioport.h>
#include <linux/kernel/mm.h>

#include <asm/page.h>
#include <asm/page_types.h>
#include <asm/sections.h>
#include <asm/setup.h>
#include <asm/tlbflush.h>


#include <obsolete/ktypes.h>


unsigned long
myos_kernel_mapping_normal(size_t paddr_start, size_t paddr_end);

int after_bootmem;


// this value is also loaded by APboot assembly code
phys_addr_t kernel_cr3 = 0;

pud_t	*init_pud_buf;
pmd_t	*init_pmd_buf;
pte_t	*init_pte_buf;

// 与linux不同，MYOS这里一次性为内核映射所有物理内存申请了全部所需映射表内存空间
// 并填充映射表,但目前并没有完成映射的最后一步（即填充pte）
void __init early_alloc_pgt_buf(void)
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
		init_top_pgt[pgt_idx + PGD_KERNEL_START] =
				arch_make_pgd(_PAGE_TABLE | __pa(init_pud_buf + pgt_idx * PTRS_PER_PUD));
	// fill pdpt entries
	for (int pud_idx = 0; pud_idx < pmdbuf_pgcount; pud_idx++)
		init_pud_buf[pud_idx] = arch_make_pud(_PAGE_TABLE | __pa(init_pmd_buf + pud_idx * PTRS_PER_PMD));
	// fill pdt entries
	for (int pmd_idx = 0; pmd_idx < ptebuf_pgcount; pmd_idx++)
		init_pmd_buf[pmd_idx] = arch_make_pmd(_PAGE_TABLE | __pa(init_pte_buf + pmd_idx * PTRS_PER_PTE));

#if defined(MAP_LOWHALF)
	memcpy(init_top_pgt, init_top_pgt + PGD_KERNEL_START, PAGE_SIZE / 2);
#endif
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
	u64 start, end;
	int i;

	for_each_mem_pfn_range(i, &start_pfn, &end_pfn) {
		start = PFN_PHYS(start_pfn);
		end = PFN_PHYS(end_pfn);
		if (start >= end)
			continue;

		myos_kernel_mapping_normal(start, end);
		mapped_ram_size += end - start;
	}

	kernel_cr3 = virt_to_phys((virt_addr_t)init_top_pgt);

extern void myos_init_VBE_mapping(void);
	myos_init_VBE_mapping();
// 默认支持x2APIC，所以无需映射MMIO空间
	// init_apic_mappings();
extern void __init init_io_apic_mappings(void);
	init_io_apic_mappings();
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
	__flush_tlb_all();
	// early_memtest(0, max_pfn_mapped << PAGE_SHIFT);
}


/*==============================================================================================*
 *										init_64.c codes											*
 *==============================================================================================*/
/*
 * NOTE: pagetable_init alloc all the fixmap pagetables contiguous on the
 * physical space so we can cache the place of the first one and move
 * around without checking the pgd every time.
 */
/* Bits supported by the hardware: */
pteval_t __supported_pte_mask __read_mostly = ~0;
/* Bits allowed in normal kernel mappings: */
pteval_t __default_kernel_pte_mask __read_mostly = ~0;

/*
 * Create page table mapping for the physical memory for specific physical
 * addresses. Note that it can only be used to populate non-present entries.
 * The virtual and physical addresses have to be aligned on PMD level
 * down. It returns the last physical address mapped.
 */
// unsigned long __meminit
// kernel_physical_mapping_init(unsigned long paddr_start,
// 			     unsigned long paddr_end,
// 			     unsigned long page_size_mask, pgprot_t prot)
static unsigned long __meminit
__myos_kernel_physical_mapping_init(
	size_t paddr_start, size_t paddr_end, unsigned long prot)
{
	unsigned long pg_addr = PFN_ALIGN(paddr_start);
	do
	{
		// map physical memory only to high-half of kernel mapping
		unsigned long kv_addr =
			(unsigned long)phys_to_virt(pg_addr);

		pgd_t *pgdp = pgd_ent_offset(&init_mm, kv_addr);
		while (pgdp == 0);
		p4d_t *p4dp = p4d_ent_offset(pgdp, kv_addr);
		while (p4dp == 0);
		pud_t *pudp = pud_ent_offset(p4dp, kv_addr);
		while (pudp == 0);
		pmd_t *pmdp = pmd_ent_offset(pudp, kv_addr);
		while (pmdp == 0);
		pte_t *ptep = pte_ent_offset(pmdp, kv_addr);
		*ptep = arch_make_pte(prot | _PAGE_PAT | pg_addr);

		pg_addr += PAGE_SIZE;
	} while (pg_addr < paddr_end);

	return phys_to_virt(paddr_start);
}
unsigned long __meminit
myos_kernel_mapping_normal(size_t paddr_start, size_t paddr_end)
{
	return __myos_kernel_physical_mapping_init(
				paddr_start, paddr_end, __PAGE_KERNEL_EXEC);
}
void __iomem
*myos_ioremap(size_t paddr_start, unsigned long size)
{
	return (void *)__myos_kernel_physical_mapping_init(paddr_start,
				PFN_PHYS(PFN_UP(paddr_start + size)), __PAGE_KERNEL_NOCACHE);
}


void __init zone_sizes_init(void)
{
	pg_data_t *nodes = myos_memblock_alloc_normal(
			MAX_NUMNODES *sizeof(pg_data_t), SMP_CACHE_BYTES);
	for (int i = 0; i < MAX_NUMNODES; i++)
		NODE_DATA(i) = nodes + i;

	unsigned long max_zone_pfns[MAX_NR_ZONES];
	memset(max_zone_pfns, 0, sizeof(max_zone_pfns));

	max_zone_pfns[ZONE_DMA]		= min((unsigned long)MAX_DMA_PFN, max_low_pfn);
	max_zone_pfns[ZONE_DMA32]	= min(MAX_DMA32_PFN, max_low_pfn);
	max_zone_pfns[ZONE_NORMAL]	= max_low_pfn;

	free_area_init(max_zone_pfns);
}


void __init mem_init(void)
{
	// pci_iommu_alloc();

	/* clear_bss() already clear the empty_zero_page */

	/* this will put all memory onto the freelists */
	memblock_free_all();
	after_bootmem = 1;
	// x86_init.hyper.init_after_bootmem();

	// /*
	//  * Must be done after boot memory is put on freelist, because here we
	//  * might set fields in deferred struct pages that have not yet been
	//  * initialized, and memblock_free_all() initializes all the reserved
	//  * deferred pages for us.
	//  */
	// register_page_bootmem_info();

	// /* Register memory areas for /proc/kcore */
	// if (get_gate_vma(&init_mm))
	// 	kclist_add(&kcore_vsyscall, (void *)VSYSCALL_ADDR, PAGE_SIZE, KCORE_USER);

	// preallocate_vmalloc_pages();
}

void myos_unmap_kernel_lowhalf()
{
	memset(init_top_pgt, 0, PAGE_SIZE / 2);
}