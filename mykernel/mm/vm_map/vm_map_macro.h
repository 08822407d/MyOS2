#ifndef _LINUX_VM_MAP_MACRO_H_
#define _LINUX_VM_MAP_MACRO_H_

	#ifndef arch_calc_vm_prot_bits
	#  define arch_calc_vm_prot_bits(prot, pkey)	0
	#endif

	#ifndef arch_calc_vm_flag_bits
	#  define arch_calc_vm_flag_bits(flags)			0
	#endif

	/*
	 * Optimisation macro.  It is equivalent to:
	 *      (x & bit1) ? bit2 : 0
	 * but this version is faster.
	 * ("bit1" and "bit2" must be single bits)
	 */
	#define _calc_vm_trans(x, bit1, bit2) (						\
				(!(bit1) || !(bit2)) ? 0 : 						\
					((bit1) <= (bit2) ?							\
						((x) & (bit1)) * ((bit2) / (bit1))		\
						: ((x) & (bit1)) / ((bit1) / (bit2)))	\
			)

	#define LIST_TO_VMA(list_p)	\
				container_of(list_p, vma_s, list)

	#define for_each_vma(mm, __vma)								\
			while (												\
				((__vma) = mm_next_vma((mm), (__vma))) != NULL	\
			)

	#define for_each_vma_topdown(mm, __vma)						\
			while (												\
				((__vma) = mm_prev_vma((mm), (__vma))) != NULL	\
			)

	// /* Bottom UP raverse vmas in mm->mm_mt while __vma's end lower than __end */
	// #define for_each_vma_range(mm, __vma, __end)				\
	// 		while (												\
	// 			((__vma) = mm_next_vma((mm), (__vma))) != NULL	&&	\
	// 				__vma->vm_end <= __end						\
	// 		)

	// /* Top Down raverse vmas in mm->mm_mt while __vma's start higher than __start */
	// #define for_each_vma_range_topdown(mm, __vma, __start)		\
	// 		while (												\
	// 			((__vma) = mm_prev_vma((mm), (__vma))) != NULL	&&	\
	// 				__vma->vm_start >= __start					\
	// 		)

#endif /* _LINUX_VM_MAP_MACRO_H_ */