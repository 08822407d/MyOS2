// source: linux-6.4.9

// SPDX-License-Identifier: GPL-2.0
#include <linux/mm/mm.h>
#include <linux/kernel/lock_ipc.h>
#include <linux/lib/list.h>
#include <linux/kernel/cpumask.h>
#include <linux/mm/mman.h>
#include <linux/mm/pgtable.h>

#ifndef INIT_MM_CONTEXT
#	define INIT_MM_CONTEXT(name)
#endif

pgd_t	init_top_pgt[ENT_PER_TABLE]__aligned(PAGE_SIZE);

/*
 * For dynamically allocated mm_structs, there is a dynamically sized cpumask
 * at the end of the structure, the size of which depends on the maximum CPU
 * number the system can see. That way we allocate only as much memory for
 * mm_cpumask() as needed for the hundreds, or thousands of processes that
 * a system typically runs.
 *
 * Since there is only one init_mm in the entire system, keep it simple
 * and size this cpu_bitmask to NR_CPUS.
 */
mm_s init_mm = {
	// .mm_mt				= MTREE_INIT_EXT(mm_mt, MM_MT_FLAGS, init_mm.mmap_lock),
	.pgd				= swapper_pg_dir,
	.mm_users			= ATOMIC_INIT(2),
	.mm_refcount		= ATOMIC_INIT(1),
	// .write_protect_seq	= SEQCNT_ZERO(init_mm.write_protect_seq),
	// MMAP_LOCK_INITIALIZER(init_mm)
	// .page_table_lock	=  __SPIN_LOCK_UNLOCKED(init_mm.page_table_lock),
	// .arg_lock			=  __SPIN_LOCK_UNLOCKED(init_mm.arg_lock),
	// .mmlist				= LIST_HEAD_INIT(init_mm.mmlist),
// #ifdef CONFIG_PER_VMA_LOCK
// 	.mm_lock_seq		= 0,
// #endif
	// .user_ns			= &init_user_ns,
	// .cpu_bitmap			= CPU_BITS_NONE,
// #ifdef CONFIG_IOMMU_SVA
// 	.pasid				= IOMMU_PASID_INVALID,
// #endif
	INIT_MM_CONTEXT(init_mm)
};
vma_s init_vma;

void setup_initial_init_mm(void *start_code,
		void *end_code, void *end_data, void *brk)
{
	init_mm.start_code	= (unsigned long)start_code;
	init_mm.end_code	= (unsigned long)end_code;
	init_mm.end_data	= (unsigned long)end_data;
	init_mm.brk			= (unsigned long)brk;

	memset(&init_vma, 0, sizeof(vma_s));
	init_mm.mmap		= &init_vma;
	init_mm.map_count	= 1;
	init_vma.vm_mm		= &init_mm;
	init_vma.vm_start	= TASK_SIZE_MAX - SZ_16M;
	init_vma.vm_end		= TASK_SIZE_MAX;
	init_vma.vm_flags	|= VM_WRITE;
}
