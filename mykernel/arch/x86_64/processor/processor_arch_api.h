#ifndef _ASM_X86_PROCESSOR_API_H_
#define _ASM_X86_PROCESSOR_API_H_

    #include "processor_const_arch.h"
    #include "processor_types_arch.h"

	#include "processor_arch.h"
    #include "desc_arch.h"

	/*
	 * capabilities of CPUs
	 */
	extern cpuinfo_x86_s boot_cpu_data;

	extern void early_cpu_init(void);
	extern void identify_boot_cpu(void);
	extern void identify_secondary_cpu(cpuinfo_x86_s *);


	#define TOP_OF_INIT_STACK (										\
				(unsigned long)&init_stack + sizeof(init_stack) -	\
					TOP_OF_KERNEL_STACK_PADDING						\
			)

	#define task_pt_regs(task)	({									\
				loff_t __ptr = (loff_t)task_stack_page(task);		\
				__ptr += THREAD_SIZE - TOP_OF_KERNEL_STACK_PADDING;	\
				((pt_regs_s *)__ptr) - 1;							\
			})

	#define task_top_of_stack(task) ((unsigned long)(task_pt_regs(task) + 1))

	/*
	 * sizeof(unsigned long) coming from an extra "long" at the end of the
	 * iobitmap. The limit is inclusive, i.e. the last valid byte.
	 */
	#define __KERNEL_TSS_LIMIT							\
				(offsetof(struct tss_struct, x86_tss) +	\
					sizeof(x86_hw_tss_s) - 1)

	/* Save actual FS/GS selectors and bases to current->thread */
	void current_save_fsgs(void);

	// extern void fpu_thread_struct_whitelist(unsigned long *offset, unsigned long *size);

	extern void
	start_thread(pt_regs_s *regs, unsigned long new_ip, unsigned long new_sp);

#endif /* _ASM_X86_PROCESSOR_API_H_ */
