#ifndef _LINUX_PGALLOC_MACRO_H_
#define _LINUX_PGALLOC_MACRO_H_


	#define pte_alloc				pgtble_alloc
	#define pte_alloc_map			pgtble_alloc_map
	#define pte_alloc_map_lock		pgtble_alloc_map_lock

	#define pte_alloc_one(mmp)		(pte_t *)__myos_pgtable_alloc_one(mmp)
	#define pmd_alloc_one(mmp)		(pmd_t *)__myos_pgtable_alloc_one(mmp)
	#define pud_alloc_one(mmp)		(pud_t *)__myos_pgtable_alloc_one(mmp)

#endif /* _LINUX_PGALLOC_MACRO_H_ */