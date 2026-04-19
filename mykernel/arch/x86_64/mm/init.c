// source: linux-6.4.9

#include <linux/kernel/ioport.h>
#include <linux/kernel/mm.h>

#include <asm/sections.h>
#include <asm/setup.h>
#include <asm/dma.h>


#include <obsolete/ktypes.h>



#define NR_RANGE_MR	5

static ulong
__kernel_physical_mapping_init(ulong paddr_start, ulong paddr_end,
		ulong page_size_mask, pgprot_t prot, bool init);

/*
 * Create page table mapping for the physical memory for specific physical
 * addresses. Note that it can only be used to populate non-present entries.
 * The virtual and physical addresses have to be aligned on PMD level
 * down. It returns the last physical address mapped.
 */
#define kernel_physical_mapping_init(paddr_start,			\
				paddr_end, page_size_mask, prot)			\
			__kernel_physical_mapping_init(paddr_start,		\
					paddr_end, page_size_mask, prot, true)

/*
 * This function is similar to kernel_physical_mapping_init() above with the
 * exception that it uses set_{pud,pmd}() instead of the set_{pud,pte}_safe()
 * when updating the mapping. The caller is responsible to flush the TLBs after
 * the function returns.
 */
#define kernel_physical_mapping_change(paddr_start,			\
				paddr_end, page_size_mask)					\
			__kernel_physical_mapping_init(paddr_start,		\
					paddr_end, page_size_mask,				\
						PAGE_KERNEL, false)




static ulong __initdata pgt_buf_start;
static ulong __initdata pgt_buf_end;
static ulong __initdata pgt_buf_top;

static ulong min_pfn_mapped;

/*
 * Pages returned are already directly mapped.
 *
 * Changing that is likely to break Xen, see commit:
 *
 *    279b706 x86,xen: introduce x86_init.mapping.pagetable_reserve
 *
 * for detailed information.
 */
__ref void
*alloc_low_pages(uint num) {
	if (after_bootmem) {
		uint order = get_order((ulong)num << PAGE_SHIFT);
		return (void *)__get_free_pages(GFP_ATOMIC | __GFP_ZERO, order);
	}

	ulong pfn;
	int i;
	if ((pgt_buf_end + num) > pgt_buf_top) {
		ulong ret = 0;

		if (min_pfn_mapped < max_pfn_mapped) {
			ret = memblock_phys_alloc_range(
					PAGE_SIZE * num, PAGE_SIZE,
					min_pfn_mapped << PAGE_SHIFT,
					max_pfn_mapped << PAGE_SHIFT);
		}

		if (!ret) {
			panic("alloc_low_pages: can not alloc memory");
			while (1);
		}

		pfn = ret >> PAGE_SHIFT;
	} else {
		pfn = pgt_buf_end;
		pgt_buf_end += num;
	}

	for (i = 0; i < num; i++) {
		void *adr = __va((pfn + i) << PAGE_SHIFT);
		clear_page(adr);
	}

	return __va(pfn << PAGE_SHIFT);
}


// this value is also loaded by APboot assembly code
phys_addr_t kernel_cr3 = 0;

/*
 * By default need to be able to allocate page tables below PGD firstly for
 * the 0-ISA_END_ADDRESS range and secondly for the initial PMD_SIZE mapping.
 * With KASLR memory randomization, depending on the machine e820 memory and the
 * PUD alignment, twice that many pages may be needed when KASLR memory
 * randomization is enabled.
 */
// #define INIT_PGD_PAGE_TABLES	4
// #define INIT_PGD_PAGE_COUNT		(4 * INIT_PGD_PAGE_TABLES)
// #define INIT_PGT_BUF_SIZE		(INIT_PGD_PAGE_COUNT * PAGE_SIZE)
/*
 * pgt_buf 以 PMD_SIZE(2 MiB) 为粒度管理。
 * 你当前设定最多 4 个 chunk，即 8 MiB。
 */
#define EARLY_PGTBUF_CHUNK_SIZE      PMD_SIZE
#define EARLY_PGTBUF_MAX_CHUNKS      4UL
#define EARLY_PGTBUF_SIZE            (EARLY_PGTBUF_MAX_CHUNKS * EARLY_PGTBUF_CHUNK_SIZE)

RESERVE_BRK(early_pgt_alloc, EARLY_PGTBUF_SIZE+PMD_SIZE);
static pud_t *early_pgtbuf_pud;
static pmd_t *early_pgtbuf_pmd;

void __init early_alloc_pgt_buf(void) {
	ulong tables = EARLY_PGTBUF_SIZE;
	phys_addr_t base = __pa(extend_brk(tables, PMD_SIZE));

	pgt_buf_start = base >> PAGE_SHIFT;
	early_pgtbuf_pud = (void *)base + __START_KERNEL_map;
	early_pgtbuf_pmd = (void *)early_pgtbuf_pud + PAGE_SIZE;
	pgt_buf_end = pgt_buf_start + 2;
	pgt_buf_top = pgt_buf_start + (tables >> PAGE_SHIFT);
}

/*
 * 你如果没有 Linux 那套 pgd_index/pud_index/pmd_index 宏，
 * 就自己替换成现有实现。
 *
 * 这里按 Linux 风格写：
 *   direct map VA = __va(pa)
 *   目标是提前插入 PMD huge leaf:
 *       pmd = phys | PAGE_KERNEL_LARGE
 */
void __init myos_early_map_pgt_buf_into_directmap(void) {
	ulong phys_start, phys_end;
	ulong va, vend;
	ulong pa;
	ulong nr_pmd;
	ulong i;

	pgd_t *early_pgd;
	pgd_t *init_pgd;
	p4d_t *p4d;
	pud_t *pud_base;
	pud_t *pud;
	pmd_t *pmd_base;

	phys_start	= pgt_buf_start << PAGE_SHIFT;
	phys_end	= pgt_buf_top   << PAGE_SHIFT;
	/*
	 * 这里只做地址换算。
	 * 这时 pgt_buf 的 direct-map 还没建好，但 __va(phys) 作为目标 VA 计算是可以的。
	 */
	va			= (ulong)__va(phys_start);
	vend		= (ulong)__va(phys_end - 1) + 1;

	// /*
	//  * 因为只有 1 张 early_pgtbuf_pud[] 和 1 张 early_pgtbuf_pmd[]，
	//  * 所以这版必须保证：
	//  *   1. 不跨 PGD 边界
	//  *   2. 不跨 PUD(1GiB) 边界
	//  */
	// if (pgd_index(va) != pgd_index(vend - 1))
	// 	panic("myos_early_map_pgt_buf_into_directmap: pgt_buf crosses PGD boundary");
	// if (pud_index(va) != pud_index(vend - 1))
	// 	panic("myos_early_map_pgt_buf_into_directmap: pgt_buf crosses PUD boundary");

	/*
	 * ---------- 第 1 步：确保 PGD -> PUD page ----------
	 *
	 * 你的前提是 direct-map 顶层初始全空，
	 * 所以这里不能假定 pgd 一定 present。
	 */
	early_pgd	= &early_top_pgt[pgd_index(va)];
	init_pgd	= &init_top_pgt[pgd_index(va)];
	p4d			= (p4d_t *)early_pgd;
	memset(early_pgtbuf_pud, 0, PAGE_SIZE);
	p4d_populate(&init_mm, p4d, early_pgtbuf_pud);
	pud_base	= early_pgtbuf_pud;


	/*
	 * ---------- 第 2 步：确保 PUD entry -> PMD page ----------
	 */
	pud			= &pud_base[pud_index(va)];
	memset(early_pgtbuf_pmd, 0, PAGE_SIZE);
	pud_populate(&init_mm, pud, early_pgtbuf_pmd);
	pmd_base	= early_pgtbuf_pmd;


	/*
	 * ---------- 第 3 步：在 PMD 中填 2 MiB huge leaf ----------
	 */
	nr_pmd	= (phys_end - phys_start) / PMD_SIZE;
	pa		= phys_start;
	for (i = 0; i < nr_pmd; i++, pa += PMD_SIZE) {
		ulong this_va	= va + i * PMD_SIZE;
		ulong pmd_slot	= pmd_index(this_va);
		pmd_t *pmd		= &pmd_base[pmd_slot];
		pmdval_t val	= pa | pgprot_val(PAGE_KERNEL_LARGE);
		set_pmd(pmd, __pmd(val));
	}

	/*
	 * 让后面的 __va() 访问 pgt_buf 确实生效。
	 * 你自己的内核如果没有 __flush_tlb_all()，这里就 reload CR3。
	 */
	__flush_tlb_all();

	// 将当前在 early_pgd 中设置的映射复制到 init_pgd 中，后者是正式启用的 direct-map 顶层页表。
	*init_pgd = *early_pgd;
}

int after_bootmem;
int direct_gbpages = 1;
struct map_range {
	ulong	start;
	ulong	end;
	uint	page_size_mask;
};

static int page_size_mask;



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
static void __init
myos_memory_map() {
extern void myos_init_VBE_mapping(void);
	myos_init_VBE_mapping();
// 默认支持x2APIC，所以无需映射MMIO空间
	// init_apic_mappings();
extern void __init init_io_apic_mappings(void);
	init_io_apic_mappings();
}



static void __init
simple_probe_page_size_mask(void) {
	/*
	 * For pagealloc debugging, identity mapping will use small pages.
	 * This will simplify cpa(), which otherwise needs to support splitting
	 * large pages into small in interrupt context, etc.
	 */
	if (boot_cpu_data.x86_capa_bits.PageSize_Ext)
		page_size_mask |= 1 << PG_LEVEL_2M;
	else
		direct_gbpages = 0;

	/* Enable 1 GB linear kernel mappings if available: */
	if (direct_gbpages && boot_cpu_data.x86_capa_bits.Page_1GB) {
		printk(KERN_INFO "Using GB pages for direct mapping\n");
		page_size_mask |= 1 << PG_LEVEL_1G;
	} else {
		direct_gbpages = 0;
	}
}
#define probe_page_size_mask simple_probe_page_size_mask


static int __meminit
save_mr(struct map_range *mr,
	int nr_range, ulong start_pfn,
	ulong end_pfn, ulong page_size_mask) {

	if (start_pfn < end_pfn) {
		if (nr_range >= NR_RANGE_MR)
			panic("run out of range for init_memory_mapping\n");
		mr[nr_range].start = start_pfn<<PAGE_SHIFT;
		mr[nr_range].end   = end_pfn<<PAGE_SHIFT;
		mr[nr_range].page_size_mask = page_size_mask;
		nr_range++;
	}
	return nr_range;
}

/*
 * adjust the page_size_mask for small range to go with
 *	big page size instead small one if nearby are ram too.
 */
static void __ref
adjust_range_page_size_mask(
	struct map_range *mr, int nr_range) {

	for (int i = 0; i < nr_range; i++) {
		if ((page_size_mask & (1<<PG_LEVEL_2M)) &&
			!(mr[i].page_size_mask & (1<<PG_LEVEL_2M))) {
			ulong start = round_down(mr[i].start, PMD_SIZE);
			ulong end = round_up(mr[i].end, PMD_SIZE);

			if (memblock_is_region_memory(start, end - start))
				mr[i].page_size_mask |= 1<<PG_LEVEL_2M;
		}
		if ((page_size_mask & (1<<PG_LEVEL_1G)) &&
			!(mr[i].page_size_mask & (1<<PG_LEVEL_1G))) {
			ulong start = round_down(mr[i].start, PUD_SIZE);
			ulong end = round_up(mr[i].end, PUD_SIZE);

			if (memblock_is_region_memory(start, end - start))
				mr[i].page_size_mask |= 1<<PG_LEVEL_1G;
		}
	}
}

static const char
*page_size_string(struct map_range *mr) {
	static const char str_1g[] = "1G";
	static const char str_2m[] = "2M";
	static const char str_4m[] = "4M";
	static const char str_4k[] = "4k";

	if (mr->page_size_mask & (1<<PG_LEVEL_1G))
		return str_1g;

	if (mr->page_size_mask & (1<<PG_LEVEL_2M))
		return str_2m;

	return str_4k;
}

static int __meminit
split_mem_range(struct map_range *mr,
	int nr_range, ulong start, ulong end) {

	ulong start_pfn, end_pfn, limit_pfn, pfn;
	int i;

	limit_pfn = PFN_DOWN(end);

	/* head if not big page alignment ? */
	pfn = start_pfn = PFN_DOWN(start);
	end_pfn = round_up(pfn, PFN_DOWN(PMD_SIZE));
	if (end_pfn > limit_pfn)
		end_pfn = limit_pfn;
	if (start_pfn < end_pfn) {
		nr_range = save_mr(mr, nr_range, start_pfn, end_pfn, 0);
		pfn = end_pfn;
	}

	/* big page (2M) range */
	start_pfn = round_up(pfn, PFN_DOWN(PMD_SIZE));
	end_pfn = round_up(pfn, PFN_DOWN(PUD_SIZE));
	if (end_pfn > round_down(limit_pfn, PFN_DOWN(PMD_SIZE)))
		end_pfn = round_down(limit_pfn, PFN_DOWN(PMD_SIZE));

	if (start_pfn < end_pfn) {
		nr_range =
			save_mr(mr, nr_range, start_pfn, end_pfn,
				page_size_mask & (1<<PG_LEVEL_2M));
		pfn = end_pfn;
	}

	/* big page (1G) range */
	start_pfn = round_up(pfn, PFN_DOWN(PUD_SIZE));
	end_pfn = round_down(limit_pfn, PFN_DOWN(PUD_SIZE));
	if (start_pfn < end_pfn) {
		nr_range =
			save_mr(mr, nr_range, start_pfn, end_pfn,
				page_size_mask & ((1<<PG_LEVEL_2M)|(1<<PG_LEVEL_1G)));
		pfn = end_pfn;
	}

	/* tail is not big page (1G) alignment */
	start_pfn = round_up(pfn, PFN_DOWN(PMD_SIZE));
	end_pfn = round_down(limit_pfn, PFN_DOWN(PMD_SIZE));
	if (start_pfn < end_pfn) {
		nr_range =
			save_mr(mr, nr_range, start_pfn, end_pfn,
				page_size_mask & (1<<PG_LEVEL_2M));
		pfn = end_pfn;
	}

	/* tail is not big page (2M) alignment */
	start_pfn = pfn;
	end_pfn = limit_pfn;
	nr_range = save_mr(mr, nr_range, start_pfn, end_pfn, 0);

	if (!after_bootmem)
		adjust_range_page_size_mask(mr, nr_range);

	/* try to merge same page size and continuous */
	for (i = 0; nr_range > 1 && i < nr_range - 1; i++) {
		ulong old_start;
		if (mr[i].end != mr[i+1].start ||
			mr[i].page_size_mask != mr[i+1].page_size_mask)
			continue;
		/* move it */
		old_start = mr[i].start;
		memmove(&mr[i], &mr[i+1],
			(nr_range - 1 - i) * sizeof(struct map_range));
		mr[i--].start = old_start;
		nr_range--;
	}

	for (i = 0; i < nr_range; i++)
		pr_debug(" [mem %#010lx-%#010lx] page %s\n",
				mr[i].start, mr[i].end - 1,
				page_size_string(&mr[i]));

	return nr_range;
}



/*
 * Setup the direct mapping of the physical memory at PAGE_OFFSET.
 * This runs before bootmem is initialized and gets pages directly from
 * the physical memory. To access them they are temporarily mapped.
 */
ulong __ref
init_memory_mapping(ulong start, ulong end, pgprot_t prot) {
	struct map_range mr[NR_RANGE_MR];
	ulong ret = 0;
	int nr_range, i;

	pr_debug("init_memory_mapping: [mem %#010lx-%#010lx]\n",
			start, end - 1);

	memset(mr, 0, sizeof(mr));
	nr_range = split_mem_range(mr, 0, start, end);

	for (i = 0; i < nr_range; i++)
		ret = kernel_physical_mapping_init(mr[i].start,
				mr[i].end, mr[i].page_size_mask, prot);

	// add_pfn_range_mapped(start >> PAGE_SHIFT, ret >> PAGE_SHIFT);

	return ret >> PAGE_SHIFT;
}

/*
 * We need to iterate through the E820 memory map and create direct mappings
 * for only E820_TYPE_RAM and E820_KERN_RESERVED regions. We cannot simply
 * create direct mappings for all pfns from [0 to max_low_pfn) and
 * [4GB to max_pfn) because of possible memory holes in high addresses
 * that cannot be marked as UC by fixed/variable range MTRRs.
 * Depending on the alignment of E820 ranges, this may possibly result
 * in using smaller size (i.e. 4K instead of 2M or 1G) page tables.
 *
 * init_mem_mapping() calls init_range_memory_mapping() with big range.
 * That range would have hole in the middle or ends, and only ram parts
 * will be mapped in init_range_memory_mapping().
 */
static ulong __init
init_range_memory_mapping(ulong r_start, ulong r_end) {
	ulong start_pfn, end_pfn;
	ulong mapped_ram_size = 0;
	int i;

	for_each_mem_pfn_range(i, &start_pfn, &end_pfn) {
		u64 start = clamp_val(PFN_PHYS(start_pfn), r_start, r_end);
		u64 end = clamp_val(PFN_PHYS(end_pfn), r_start, r_end);
		if (start >= end)
			continue;

		init_memory_mapping(start, end, PAGE_KERNEL);
		mapped_ram_size += end - start;
	}

	return mapped_ram_size;
}
#define memory_map_bottom_up	init_range_memory_mapping

void __init init_mem_mapping(void)
{
	ulong end;

	// pti_check_boottime_disable();
	probe_page_size_mask();
	// setup_pcid();

	end = max_pfn << PAGE_SHIFT;

	/* the ISA range is always mapped regardless of memory holes */
	init_memory_mapping(0, ISA_END_ADDRESS, PAGE_KERNEL);

	/* Init the trampoline, possibly with KASLR memory offset */
	// init_trampoline();

	/*
	 * If the allocation is in bottom-up direction, we setup direct mapping
	 * in bottom-up, otherwise we setup direct mapping in top-down.
	 */
	if (memblock_bottom_up()) {
		ulong kernel_end = __pa_symbol(_end);

		/*
		 * we need two separate calls here. This is because we want to
		 * allocate page tables above the kernel. So we first map
		 * [kernel_end, end) to make memory above the kernel be mapped
		 * as soon as possible. And then use page tables allocated above
		 * the kernel to map [ISA_END_ADDRESS, kernel_end).
		 */
		memory_map_bottom_up(kernel_end, end);
		memory_map_bottom_up(ISA_END_ADDRESS, kernel_end);
	} else {
		// memory_map_top_down(ISA_END_ADDRESS, end);
	}
	myos_memory_map();

	if (max_pfn > max_low_pfn) {
		/* can we preserve max_low_pfn ?*/
		max_low_pfn = max_pfn;
	}

	load_cr3(swapper_pg_dir);
	__flush_tlb_all();
	// early_memtest(0, max_pfn_mapped << PAGE_SHIFT);
}




/*==========================================================================================*
 *									set_memory.c codes										*
 *==========================================================================================*/
static ulong direct_pages_count[PG_LEVEL_NUM];
void update_page_count(int level, ulong pages) {
	/* Protect against CPA */
	spin_lock(&pgd_lock);
	direct_pages_count[level] += pages;
	spin_unlock(&pgd_lock);
}


/*==========================================================================================*
 *										init_64.c codes										*
 *==========================================================================================*/
static inline pgprot_t
prot_sethuge(pgprot_t prot) {
	WARN_ON_ONCE(pgprot_val(prot) & _PAGE_PAT);
	return __pgprot(pgprot_val(prot) | _PAGE_PSE);
}


/*
 * NOTE: This function is marked __ref because it calls __init function
 * (alloc_bootmem_pages). It's safe to do it ONLY when after_bootmem == 0.
 */
static __ref void
*spp_getpage(void) {
	void *ptr;

	if (after_bootmem)
		ptr = (void *) get_zeroed_page(GFP_ATOMIC);
	else
		ptr = memblock_alloc(PAGE_SIZE, PAGE_SIZE);

	if (!ptr || ((ulong)ptr & ~PAGE_MASK)) {
		panic("set_pte_phys: cannot allocate page data %s\n",
				after_bootmem ? "after bootmem" : "");
	}

	pr_debug("spp_getpage %p\n", ptr);

	return ptr;
}

static pud_t
*fill_pud(p4d_t *p4d, ulong vaddr) {
	if (p4d_none(*p4d)) {
		pud_t *pud = (pud_t *)spp_getpage();
		p4d_populate(&init_mm, p4d, pud);
		if (pud != pud_offset(p4d, 0))
			printk(KERN_ERR "PAGETABLE BUG #01! %p <-> %p\n",
					pud, pud_offset(p4d, 0));
	}
	return pud_offset(p4d, vaddr);
}

static pmd_t
*fill_pmd(pud_t *pud, ulong vaddr) {
	if (pud_none(*pud)) {
		pmd_t *pmd = (pmd_t *) spp_getpage();
		pud_populate(&init_mm, pud, pmd);
		if (pmd != pmd_offset(pud, 0))
			printk(KERN_ERR "PAGETABLE BUG #02! %p <-> %p\n",
					pmd, pmd_offset(pud, 0));
	}
	return pmd_offset(pud, vaddr);
}

static pte_t
*fill_pte(pmd_t *pmd, ulong vaddr) {
	if (pmd_none(*pmd)) {
		pte_t *pte = (pte_t *) spp_getpage();
		pmd_populate_kernel(&init_mm, pmd, pte);
		if (pte != pte_offset_kernel(pmd, 0))
			printk(KERN_ERR "PAGETABLE BUG #03!\n");
	}
	return pte_offset_kernel(pmd, vaddr);
}


/*
 * Create PTE level page table mapping for physical addresses.
 * It returns the last physical address mapped.
 */
static ulong __meminit
phys_pte_init(pte_t *pte_page, ulong paddr,
		ulong paddr_end, pgprot_t prot, bool init) {

	ulong pages = 0, paddr_next;
	ulong paddr_last = paddr_end;
	pte_t *pte;
	int i;

	pte = pte_page + pte_index(paddr);
	i = pte_index(paddr);

	for (; i < PTRS_PER_PTE; i++, paddr = paddr_next, pte++) {
		paddr_next = (paddr & PAGE_MASK) + PAGE_SIZE;
		if (paddr >= paddr_end)
			continue;

		/*
		 * We will re-use the existing mapping.
		 * Xen for example has some special requirements, like mapping
		 * pagetable pages as RO. So assume someone who pre-setup
		 * these mappings are more intelligent.
		 */
		if (!pte_none(*pte)) {
			if (!after_bootmem)
				pages++;
			continue;
		}

		if (0)
			pr_info("   pte=%p addr=%lx pte=%016lx\n", pte, paddr,
				pfn_pte(paddr >> PAGE_SHIFT, PAGE_KERNEL).val);
		pages++;
		set_pte_init(pte, pfn_pte(paddr >> PAGE_SHIFT, prot), init);
		paddr_last = (paddr & PAGE_MASK) + PAGE_SIZE;
	}
	update_page_count(PG_LEVEL_4K, pages);
	return paddr_last;
}

/*
 * Create PMD level page table mapping for physical addresses. The virtual
 * and physical address have to be aligned at this level.
 * It returns the last physical address mapped.
 */
static ulong __meminit
phys_pmd_init(pmd_t *pmd_page, ulong paddr, ulong paddr_end,
		ulong page_size_mask, pgprot_t prot, bool init) {

	ulong pages = 0, paddr_next;
	ulong paddr_last = paddr_end;

	int i = pmd_index(paddr);

	for (; i < PTRS_PER_PMD; i++, paddr = paddr_next) {
		pmd_t *pmd = pmd_page + pmd_index(paddr);
		pte_t *pte;
		pgprot_t new_prot = prot;

		paddr_next = (paddr & PMD_MASK) + PMD_SIZE;
		if (paddr >= paddr_end)
			continue;

		if (!pmd_none(*pmd)) {
			if (!pmd_leaf(*pmd)) {
				spin_lock(&init_mm.page_table_lock);
				pte = (pte_t *)pmd_page_vaddr(*pmd);
				paddr_last = phys_pte_init(pte, paddr,
							   paddr_end, prot,
							   init);
				spin_unlock(&init_mm.page_table_lock);
				continue;
			}
			/*
			 * If we are ok with PG_LEVEL_2M mapping, then we will
			 * use the existing mapping,
			 *
			 * Otherwise, we will split the large page mapping but
			 * use the same existing protection bits except for
			 * large page, so that we don't violate Intel's TLB
			 * Application note (317080) which says, while changing
			 * the page sizes, new and old translations should
			 * not differ with respect to page frame and
			 * attributes.
			 */
			if (page_size_mask & (1 << PG_LEVEL_2M)) {
				if (!after_bootmem)
					pages++;
				paddr_last = paddr_next;
				continue;
			}
			new_prot = pte_pgprot(pte_clrhuge(*(pte_t *)pmd));
		}

		if (page_size_mask & (1<<PG_LEVEL_2M)) {
			pages++;
			spin_lock(&init_mm.page_table_lock);
			set_pmd_init(pmd,
				pfn_pmd(paddr >> PAGE_SHIFT, prot_sethuge(prot)),
					init);
			spin_unlock(&init_mm.page_table_lock);
			paddr_last = paddr_next;
			continue;
		}

		pte = alloc_low_page();
		paddr_last = phys_pte_init(pte, paddr, paddr_end, new_prot, init);

		spin_lock(&init_mm.page_table_lock);
		pmd_populate_kernel_init(&init_mm, pmd, pte, init);
		spin_unlock(&init_mm.page_table_lock);
	}
	update_page_count(PG_LEVEL_2M, pages);
	return paddr_last;
}

/*
 * Create PUD level page table mapping for physical addresses. The virtual
 * and physical address do not have to be aligned at this level. KASLR can
 * randomize virtual addresses up to this level.
 * It returns the last physical address mapped.
 */
static ulong __meminit
phys_pud_init(pud_t *pud_page, ulong paddr, ulong paddr_end,
		ulong page_size_mask, pgprot_t _prot, bool init) {

	ulong pages = 0, paddr_next;
	ulong paddr_last = paddr_end;
	ulong vaddr = (ulong)__va(paddr);
	int i = pud_index(vaddr);

	for (; i < PTRS_PER_PUD; i++, paddr = paddr_next) {
		pud_t *pud;
		pmd_t *pmd;
		pgprot_t prot = _prot;

		vaddr = (ulong)__va(paddr);
		pud = pud_page + pud_index(vaddr);
		paddr_next = (paddr & PUD_MASK) + PUD_SIZE;

		// 超出映射范围的终止条件
		if (paddr >= paddr_end)
			continue;

		if (!pud_none(*pud)) {
			if (!pud_leaf(*pud)) {
				pmd = pmd_offset(pud, 0);
				paddr_last =
					phys_pmd_init(pmd, paddr, paddr_end,
						page_size_mask, prot, init);
				continue;
			}
			/*
			 * If we are ok with PG_LEVEL_1G mapping, then we will
			 * use the existing mapping.
			 *
			 * Otherwise, we will split the gbpage mapping but use
			 * the same existing protection  bits except for large
			 * page, so that we don't violate Intel's TLB
			 * Application note (317080) which says, while changing
			 * the page sizes, new and old translations should
			 * not differ with respect to page frame and
			 * attributes.
			 */
			if (page_size_mask & (1 << PG_LEVEL_1G)) {
				if (!after_bootmem)
					pages++;
				paddr_last = paddr_next;
				continue;
			}
			prot = pte_pgprot(pte_clrhuge(*(pte_t *)pud));
		}

		if (page_size_mask & (1<<PG_LEVEL_1G)) {
			pages++;
			spin_lock(&init_mm.page_table_lock);
			set_pud_init(pud,
				pfn_pud(paddr >> PAGE_SHIFT, prot_sethuge(prot)),
					init);
			spin_unlock(&init_mm.page_table_lock);
			paddr_last = paddr_next;
			continue;
		}

		pmd = alloc_low_page();
		paddr_last = phys_pmd_init(pmd, paddr, paddr_end,
					   page_size_mask, prot, init);

		spin_lock(&init_mm.page_table_lock);
		pud_populate_init(&init_mm, pud, pmd, init);
		spin_unlock(&init_mm.page_table_lock);
	}
	update_page_count(PG_LEVEL_1G, pages);
	return paddr_last;
}

#define phys_p4d_init(p4d_page, paddr, paddr_end,					\
				page_size_mask, prot, init)							\
			phys_pud_init((pud_t *)(p4d_page), (paddr),				\
					(paddr_end), (page_size_mask), (prot), (init))

static ulong __meminit
__kernel_physical_mapping_init(ulong paddr_start,
		ulong paddr_end, ulong page_size_mask,
		pgprot_t prot, bool init) {

	bool pgd_changed = false;
	ulong vaddr, vaddr_start, vaddr_end, vaddr_next, paddr_last;

	paddr_last = paddr_end;
	vaddr = (ulong)__va(paddr_start);
	vaddr_end = (ulong)__va(paddr_end);
	vaddr_start = vaddr;

	for (; vaddr < vaddr_end; vaddr = vaddr_next) {
		pgd_t *pgd = pgd_offset_k(vaddr);
		p4d_t *p4d;

		vaddr_next = (vaddr & PGDIR_MASK) + PGDIR_SIZE;

		if (pgd_val(*pgd)) {
			p4d = (p4d_t *)pgd_page_vaddr(*pgd);
			paddr_last = phys_p4d_init(p4d,
					__pa(vaddr), __pa(vaddr_end),
					page_size_mask, prot, init);
			continue;
		}

		p4d = alloc_low_page();
		paddr_last = phys_p4d_init(p4d, __pa(vaddr),
			__pa(vaddr_end), page_size_mask, prot, init);

		spin_lock(&init_mm.page_table_lock);
		p4d_populate_init(&init_mm, p4d_offset(pgd, vaddr),
				(pud_t *) p4d, init);
		spin_unlock(&init_mm.page_table_lock);

		pgd_changed = true;
	}
	// if (pgd_changed)
	// 	sync_global_pgds(vaddr_start, vaddr_end - 1);
	return paddr_last;
}


/*
 * NOTE: pagetable_init alloc all the fixmap pagetables contiguous on the
 * physical space so we can cache the place of the first one and move
 * around without checking the pgd every time.
 */
/* Bits supported by the hardware: */
pteval_t __supported_pte_mask __read_mostly = ~0;
/* Bits allowed in normal kernel mappings: */
pteval_t __default_kernel_pte_mask __read_mostly = ~0;

void __iomem
*myos_ioremap(size_t paddr_start, ulong size)
{
	// return __myos_kernel_physical_mapping_init(paddr_start,
	// 			PFN_PHYS(PFN_UP(paddr_start + size)), __PAGE_KERNEL_NOCACHE);
	return (void *)__kernel_physical_mapping_init(paddr_start,
			PFN_PHYS(PFN_UP(paddr_start + size)),
				1 << PG_LEVEL_4K, PAGE_KERNEL_NOENC, true);
}
void iounmap(volatile void __iomem *addr)
{

}

void __init mem_init(void)
{
	/* clear_bss() already clear the empty_zero_page */

	after_bootmem = 1;
	// x86_init.hyper.init_after_bootmem();

	/* this will put all memory onto the freelists */
	memblock_free_all();

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