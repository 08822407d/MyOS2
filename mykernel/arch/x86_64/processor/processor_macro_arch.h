#ifndef _ASM_X86_PROCESSOR_MACRO_H_
#define _ASM_X86_PROCESSOR_MACRO_H_


	#define cpuid_subleaf(leaf, subleaf, regs) {			\
				static_assert(sizeof(*(regs)) == 16);		\
				__cpuid_read(leaf, subleaf, (u32 *)(regs));	\
			}

	#define cpuid_leaf(leaf, regs) {						\
				static_assert(sizeof(*(regs)) == 16);		\
				__cpuid_read(leaf, 0, (u32 *)(regs));		\
			}

	#define cpuid_subleaf_reg(leaf, subleaf, regidx, reg) {				\
				static_assert(sizeof(*(reg)) == 4);						\
				__cpuid_read_reg(leaf, subleaf, regidx, (u32 *)(reg));	\
			}

	#define cpuid_leaf_reg(leaf, regidx, reg) {							\
				static_assert(sizeof(*(reg)) == 4);						\
				__cpuid_read_reg(leaf, 0, regidx, (u32 *)(reg));		\
			}

#endif /* _ASM_X86_PROCESSOR_MACRO_H_ */	