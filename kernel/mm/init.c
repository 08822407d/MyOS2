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
// #include <asm/page_types.h>
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


#include <linux/kernel/minmax.h>
#include <linux/lib/string.h>

void __init zone_sizes_init(void)
{
	unsigned long max_zone_pfns[MAX_NR_ZONES];
	memset(max_zone_pfns, 0, sizeof(max_zone_pfns));

#ifdef CONFIG_ZONE_DMA
	max_zone_pfns[ZONE_DMA]		= min((unsigned long)MAX_DMA_PFN, max_low_pfn);
#endif
#ifdef CONFIG_ZONE_DMA32
	max_zone_pfns[ZONE_DMA32]	= min(MAX_DMA32_PFN, max_low_pfn);
#endif
	max_zone_pfns[ZONE_NORMAL]	= max_low_pfn;

	free_area_init(max_zone_pfns);
}

