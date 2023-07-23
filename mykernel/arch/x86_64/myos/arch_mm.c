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

#define SEG_NR	4

typedef struct mm_pair
{
	virt_addr_t	startp;
	long		pgnr;
} mmpr_s;

mmpr_s mmpr[SEG_NR];

mmpr_s * get_seginfo(task_s * task)
{
	mm_s * mm = task->mm;
	reg_t user_rsp = task_pt_regs(task)->sp;

	virt_addr_t codepg_p = mmpr[0].startp = (virt_addr_t)round_down(mm->start_code, PAGE_SIZE);
	long codepg_nr = mmpr[0].pgnr = (round_up(mm->end_code, PAGE_SIZE) - (unsigned long)codepg_p) / PAGE_SIZE;
	virt_addr_t datapg_p = mmpr[1].startp = (virt_addr_t)round_down(mm->start_data, PAGE_SIZE);
	long datapg_nr = mmpr[1].pgnr = (round_up(mm->end_data, PAGE_SIZE) - (unsigned long)datapg_p) / PAGE_SIZE;
	virt_addr_t brkpg_p = mmpr[2].startp = (virt_addr_t)round_down(mm->start_brk, PAGE_SIZE);
	long brkpg_nr = mmpr[2].pgnr = (round_up(mm->brk, PAGE_SIZE) - (unsigned long)brkpg_p) / PAGE_SIZE;
	virt_addr_t stackpg_p = mmpr[3].startp = (virt_addr_t)round_down((unsigned long)user_rsp, PAGE_SIZE);
	long stackpg_nr = mmpr[3].pgnr = (round_up(mm->start_stack, PAGE_SIZE) - (unsigned long)stackpg_p) / PAGE_SIZE;
}

void creat_exec_addrspace(task_s * task)
{
	pt_regs_s *context = task_pt_regs(task);
	mm_s * mm = task->mm;
	context->sp = (reg_t)round_down(mm->start_stack, PAGE_SIZE) - SZ_1M;
	get_seginfo(task);
	unsigned long attr = PAGE_SHARED;

	for (int seg_idx = 0; seg_idx < SEG_NR; seg_idx++)
	{
		// for (int pgnr = -1; pgnr < mmpr[seg_idx].pgnr; pgnr++)
		// {
		// 	page_s *page = alloc_page(GFP_USER);
		// 	atomic_inc(&(page->_mapcount));
		// 	// arch_page_domap(mmpr[seg_idx].startp + pgnr * PAGE_SIZE,
		// 	// 				page_to_phys(page), attr, (reg_t *)&mm->pgd);
		// }
		unsigned long start = mmpr[seg_idx].startp;
		unsigned long end = start + mmpr[seg_idx].pgnr * PAGE_SIZE;
		myos_map_range(mm, start, end);
	}
	load_cr3(mm->pgd);
}

void prepair_COW(task_s * task)
{
	mm_s * mm = task->mm;
	get_seginfo(task);

	for (int seg_idx = 0; seg_idx < SEG_NR; seg_idx++)
	{
		for (int pgnr = 0; pgnr < mmpr[seg_idx].pgnr; pgnr++)
		{
			// set all pages to read-only
			arch_page_clearattr(mmpr[seg_idx].startp + pgnr * PAGE_SIZE,
								_PAGE_RW, (reg_t *)&mm->pgd);
			phys_addr_t paddr = 0;
			get_paddr((reg_t)ARCH_PGS_ADDR((unsigned long)mm->pgd),
						mmpr[seg_idx].startp + pgnr * PAGE_SIZE, &paddr);
			page_s *page = paddr_to_page(paddr);
			atomic_inc(&(page->_mapcount));
		}
	}
}

int do_COW(task_s * task, virt_addr_t virt)
{
	int ret_val = 0;
	mm_s * mm = task->mm;
	get_seginfo(task);

	reg_t orig_cr3 = (reg_t)read_cr3_pa();
	reg_t new_cr3 = 0;
	phys_addr_t orig_paddr = 0;
	get_paddr(orig_cr3, virt, &orig_paddr);
	page_s * orig_page = paddr_to_page(orig_paddr);
	page_s * new_page = alloc_page(GFP_USER);

	if (new_page != NULL)
	{
		SetPageReferenced(new_page);
		if (!arch_page_duplicate(virt, page_to_phys(new_page), orig_cr3, &new_cr3))
		{
			atomic_inc(&(new_page->_mapcount));
			arch_page_setattr(virt, _PAGE_RW, &new_cr3);

			atomic_dec(&(orig_page->_mapcount));
			if (atomic_read(&(orig_page->_mapcount)) < 2)
				arch_page_setattr(virt, _PAGE_RW, &orig_cr3);

			virt_addr_t orig_pg_vaddr = page_to_virt(orig_page);
			virt_addr_t new_pg_vaddr = page_to_virt(new_page);
			memcpy((void *)new_pg_vaddr, (void *)orig_pg_vaddr, PAGE_SIZE);

			load_cr3((pgd_t *)new_cr3);
			task->mm->pgd = (pgd_t *)new_cr3;
		}
	}

	return ret_val;
}

int check_addr_writable(reg_t cr2, task_s * task)
{
	unsigned long cr2_val = (unsigned long)cr2;
	int ret_val = false;
	mm_s * mm = task->mm;
	reg_t rsp = task_pt_regs(task)->sp;

	if ((cr2_val >= mm->start_code && cr2_val < mm->end_code) ||
		(cr2_val >= mm->start_data && cr2_val < mm->end_data) ||
		(cr2_val >= mm->start_brk && cr2_val < mm->brk) ||
		(cr2 >= rsp && cr2_val < mm->start_stack))
		ret_val = true;

	return ret_val;
}

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