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

#endif /* _LINUX_VM_MAP_MACRO_H_ */