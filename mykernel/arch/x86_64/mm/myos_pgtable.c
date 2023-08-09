#include <linux/kernel/slab.h>
#include <linux/kernel/types.h>
#include <linux/kernel/stddef.h>
#include <linux/kernel/math.h>
#include <linux/mm/mm.h>
#include <linux/mm/memblock.h>
#include <linux/lib/string.h>
#include <asm/setup.h>
#include <asm/processor.h>
#include <asm/pgtable_64.h>

#include <obsolete/glo.h>
#include <obsolete/ktypes.h>
#include <obsolete/printk.h>
#include <obsolete/dbg_utils.h>

#include <obsolete/archconst.h>
#include <obsolete/archtypes.h>
#include <obsolete/arch_glo.h>
#include <obsolete/arch_proto.h>

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
void myos_unmap_kernel_lowhalf(atomic_t *um_flag)
{
	memset(init_top_pgt, 0, PAGE_SIZE / 2);
	atomic_inc(um_flag);
}


void myos_update_mmu_tlb(void)
{
	uint64_t tempreg;
	asm volatile(	"movq	%%cr3,	%0	\n\t"
					"nop				\n\t"
					"movq	%0,	%%cr3	\n\t"
					"nop				\n\t"
				:	"=r"(tempreg)
				:
				:
				);
}