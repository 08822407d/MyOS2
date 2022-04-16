#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include <include/ktypes.h>
#include <include/proto.h>
#include <include/task/task.h>

#include "include/arch_proto.h"
#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_task.h"

#define SEG_NR	6

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

	virt_addr_t codepg_p = mmpr[0].startp = (virt_addr_t)PAGE_ROUND_DOWN(mm->start_code);
	long codepg_nr = mmpr[0].pgnr = (PAGE_ROUND_UP(mm->end_code) - (reg_t)codepg_p) / PAGE_SIZE;
	virt_addr_t rodatapg_p = mmpr[1].startp = (virt_addr_t)PAGE_ROUND_DOWN(mm->start_rodata);
	long rodatapg_nr = mmpr[1].pgnr = (PAGE_ROUND_UP(mm->end_rodata) - (reg_t)rodatapg_p) / PAGE_SIZE;
	virt_addr_t datapg_p = mmpr[2].startp = (virt_addr_t)PAGE_ROUND_DOWN(mm->start_data);
	long datapg_nr = mmpr[2].pgnr = (PAGE_ROUND_UP(mm->end_data) - (reg_t)datapg_p) / PAGE_SIZE;
	virt_addr_t bsspg_p = mmpr[3].startp = (virt_addr_t)PAGE_ROUND_DOWN(mm->start_bss);
	long bsspg_nr = mmpr[3].pgnr = (PAGE_ROUND_UP(mm->end_bss) - (reg_t)bsspg_p) / PAGE_SIZE;
	virt_addr_t brkpg_p = mmpr[4].startp = (virt_addr_t)PAGE_ROUND_DOWN(mm->start_brk);
	long brkpg_nr = mmpr[4].pgnr = (PAGE_ROUND_UP(mm->end_brk) - (reg_t)brkpg_p) / PAGE_SIZE;
	virt_addr_t stackpg_p = mmpr[5].startp = (virt_addr_t)PAGE_ROUND_DOWN(user_rsp);
	long stackpg_nr = mmpr[5].pgnr = (PAGE_ROUND_UP(mm->start_stack) - (reg_t)stackpg_p) / PAGE_SIZE;
}

void creat_exec_addrspace(task_s * task)
{
	stack_frame_s * context = get_stackframe(task);
	mm_s * mm = task->mm_struct;
	context->rsp = PAGE_ROUND_DOWN(mm->start_stack) - PAGE_SIZE;
	get_seginfo(task);
	unsigned long attr = ARCH_PG_PRESENT | ARCH_PG_RW | ARCH_PG_USER;

	for (int seg_idx = 0; seg_idx < SEG_NR; seg_idx++)
	{
		for (int pgnr = 0; pgnr < mmpr[seg_idx].pgnr; pgnr++)
		{
			Page_s * pgp = alloc_pages(ZONE_NORMAL, 0);
			pgp->map_count++;
			pgp->attr = PG_PTable_Maped;
			arch_page_domap(mmpr[seg_idx].startp + pgnr * PAGE_SIZE,
							page_to_paddr(pgp), attr, &mm->cr3);
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
								ARCH_PG_RW, &mm->cr3);
			phys_addr_t paddr = 0;
			get_paddr(ARCH_PGS_ADDR(mm->cr3),
						mmpr[seg_idx].startp + pgnr * PAGE_SIZE, &paddr);
			Page_s * pgp = paddr_to_page(paddr);
			pgp->map_count++;
		}
	}

	refresh_arch_page();
}

int do_COW(task_s * task, virt_addr_t virt)
{
	int ret_val = 0;
	mm_s * mm = task->mm_struct;
	get_seginfo(task);

	reg_t orig_cr3 = read_cr3();
	reg_t new_cr3 = 0;
	phys_addr_t orig_paddr = 0;
	get_paddr(orig_cr3, virt, &orig_paddr);
	Page_s * orig_pgp = paddr_to_page(orig_paddr);
	Page_s * new_pgp = alloc_pages(ZONE_NORMAL, 0);

	if (new_pgp != NULL)
	{
		new_pgp->attr = PG_PTable_Maped | PG_Referenced;
		if (!arch_page_duplicate(virt, page_to_paddr(new_pgp), orig_cr3, &new_cr3))
		{
			new_pgp->map_count++;
			arch_page_setattr(virt, ARCH_PG_RW, &new_cr3);

			orig_pgp->map_count--;
			if (orig_pgp->map_count < 2)
				arch_page_setattr(virt, ARCH_PG_RW, &orig_cr3);

			virt_addr_t orig_pg_vaddr = (virt_addr_t)phys2virt(page_to_paddr(orig_pgp));
			virt_addr_t new_pg_vaddr = (virt_addr_t)phys2virt(page_to_paddr(new_pgp));
			memcpy(new_pg_vaddr, orig_pg_vaddr, PAGE_SIZE);

			pg_load_cr3(new_cr3);
			task->mm_struct->cr3 = new_cr3;
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
		(cr2 >= mm->start_rodata && cr2 < mm->end_rodata) ||
		(cr2 >= mm->start_data && cr2 < mm->end_data) ||
		(cr2 >= mm->start_bss && cr2 < mm->end_bss) ||
		(cr2 >= mm->start_brk && cr2 < mm->end_brk) ||
		(cr2 >= rsp && cr2 < mm->start_stack))
		ret_val = true;

	return ret_val;
}

reg_t do_brk(reg_t start, reg_t length)
{
	reg_t new_end = start + length;
	for (reg_t vaddr = start; vaddr < new_end; vaddr += PAGE_SIZE)
	{
		unsigned long attr = ARCH_PG_PRESENT | ARCH_PG_USER | ARCH_PG_RW;
		Page_s * pg_p = alloc_pages(ZONE_NORMAL, 0);
		arch_page_domap((virt_addr_t)vaddr, page_to_paddr(pg_p), attr, &curr_tsk->mm_struct->cr3);
	}
	curr_tsk->mm_struct->end_brk = new_end;
	return new_end;
}