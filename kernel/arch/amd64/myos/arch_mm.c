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
	mm_s * mm = task->mm_struct;
	reg_t user_rsp = get_stackframe(task)->rsp;

	virt_addr_t codepg_p = mmpr[0].startp = (virt_addr_t)round_down(mm->start_code, SZ_2M);
	long codepg_nr = mmpr[0].pgnr = (round_up(mm->end_code, SZ_2M) - (reg_t)codepg_p) / PAGE_SIZE;
	virt_addr_t datapg_p = mmpr[1].startp = (virt_addr_t)round_down(mm->start_data, SZ_2M);
	long datapg_nr = mmpr[1].pgnr = (round_up(mm->end_data, SZ_2M) - (reg_t)datapg_p) / PAGE_SIZE;
	virt_addr_t brkpg_p = mmpr[2].startp = (virt_addr_t)round_down(mm->start_brk, SZ_2M);
	long brkpg_nr = mmpr[2].pgnr = (round_up(mm->brk, SZ_2M) - (reg_t)brkpg_p) / PAGE_SIZE;
	virt_addr_t stackpg_p = mmpr[3].startp = (virt_addr_t)round_down(user_rsp, SZ_2M);
	long stackpg_nr = mmpr[3].pgnr = (round_up(mm->start_stack, SZ_2M) - (reg_t)stackpg_p) / PAGE_SIZE;
}

void creat_exec_addrspace(task_s * task)
{
	stack_frame_s * context = get_stackframe(task);
	mm_s * mm = task->mm_struct;
	context->rsp = round_down(mm->start_stack, SZ_2M) - SZ_2M;
	get_seginfo(task);
	unsigned long attr = PAGE_SHARED;

	for (int seg_idx = 0; seg_idx < SEG_NR; seg_idx++)
	{
		for (int pgnr = 0; pgnr < mmpr[seg_idx].pgnr; pgnr++)
		{
			page_s *page = alloc_pages(ZONE_NORMAL, 0);
			atomic_inc(&(page->_mapcount));
			arch_page_domap(mmpr[seg_idx].startp + pgnr * PAGE_SIZE,
							page_to_paddr(page), attr, &mm->pgd_ptr);
		}
	}
}

void prepair_COW(task_s * task)
{
	mm_s * mm = task->mm_struct;
	get_seginfo(task);

	for (int seg_idx = 0; seg_idx < SEG_NR; seg_idx++)
	{
		for (int pgnr = 0; pgnr < mmpr[seg_idx].pgnr; pgnr++)
		{
			// set all pages to read-only
			arch_page_clearattr(mmpr[seg_idx].startp + pgnr * PAGE_SIZE,
								_PAGE_RW, &mm->pgd_ptr);
			phys_addr_t paddr = 0;
			get_paddr(ARCH_PGS_ADDR(mm->pgd_ptr),
						mmpr[seg_idx].startp + pgnr * PAGE_SIZE, &paddr);
			page_s *page = paddr_to_page(paddr);
			atomic_inc(&(page->_mapcount));
		}
	}

	myos_refresh_arch_page();
}

int do_COW(task_s * task, virt_addr_t virt)
{
	int ret_val = 0;
	mm_s * mm = task->mm_struct;
	get_seginfo(task);

	reg_t orig_cr3 = myos_read_cr3();
	reg_t new_cr3 = 0;
	phys_addr_t orig_paddr = 0;
	get_paddr(orig_cr3, virt, &orig_paddr);
	page_s * orig_page = paddr_to_page(orig_paddr);
	page_s * new_page = alloc_pages(ZONE_NORMAL, 0);

	if (new_page != NULL)
	{
		SetPageReferenced(new_page);
		if (!arch_page_duplicate(virt, page_to_paddr(new_page), orig_cr3, &new_cr3))
		{
			atomic_inc(&(new_page->_mapcount));
			arch_page_setattr(virt, _PAGE_RW, &new_cr3);

			atomic_dec(&(orig_page->_mapcount));
			if (atomic_read(&(orig_page->_mapcount)) < 2)
				arch_page_setattr(virt, _PAGE_RW, &orig_cr3);

			virt_addr_t orig_pg_vaddr = (virt_addr_t)myos_phys2virt(page_to_paddr(orig_page));
			virt_addr_t new_pg_vaddr = (virt_addr_t)myos_phys2virt(page_to_paddr(new_page));
			memcpy((void *)new_pg_vaddr, (void *)orig_pg_vaddr, PAGE_SIZE);

			load_cr3(new_cr3);
			task->mm_struct->pgd_ptr = new_cr3;
		}
	}

	return ret_val;
}

int check_addr_writable(reg_t cr2, task_s * task)
{
	int ret_val = false;
	mm_s * mm = task->mm_struct;
	reg_t rsp = get_stackframe(task)->rsp;

	if ((cr2 >= mm->start_code && cr2 < mm->end_code) ||
		(cr2 >= mm->start_data && cr2 < mm->end_data) ||
		(cr2 >= mm->start_brk && cr2 < mm->brk) ||
		(cr2 >= rsp && cr2 < mm->start_stack))
		ret_val = true;

	return ret_val;
}

reg_t do_brk(reg_t start, reg_t length)
{
	reg_t new_end = start + length;
	for (reg_t vaddr = start; vaddr < new_end; vaddr += PAGE_SIZE)
	{
		unsigned long attr = PAGE_SHARED;
		page_s * pg_p = alloc_pages(ZONE_NORMAL, 0);
		arch_page_domap((virt_addr_t)vaddr, page_to_paddr(pg_p),
				attr, &curr_tsk->mm_struct->pgd_ptr);
	}
	curr_tsk->mm_struct->brk = new_end;
	return new_end;
}