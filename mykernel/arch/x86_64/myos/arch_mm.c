#include <linux/mm/mm.h>
#include <linux/lib/string.h>
#include <asm/setup.h>
#include <asm/processor.h>

#include <klib/stdbool.h>

#include <obsolete/ktypes.h>
#include <obsolete/proto.h>
#include <linux/kernel/sched.h>

#include <obsolete/arch_proto.h>
#include <obsolete/archconst.h>
#include <obsolete/archtypes.h>


virt_addr_t do_brk(virt_addr_t start, size_t length)
{
	virt_addr_t new_end = start + length;
	for (virt_addr_t vaddr = start; vaddr < new_end; vaddr += PAGE_SIZE)
	{
		unsigned long attr = PAGE_SHARED;
		page_s * pg_p = alloc_page(GFP_USER);
		// arch_page_domap((virt_addr_t)vaddr, page_to_phys(pg_p),
		// 		attr, (reg_t *)&current->mm->pgd);
	}
	current->mm->brk = (unsigned long)new_end;
	return new_end;
}