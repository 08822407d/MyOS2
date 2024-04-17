#ifndef _ASM_X86_THREAD_INFO_MACRO_H_
#define _ASM_X86_THREAD_INFO_MACRO_H_

	/*
	 * sizeof(unsigned long) coming from an extra "long" at the end of the
	 * iobitmap. The limit is inclusive, i.e. the last valid byte.
	 */
	#define __KERNEL_TSS_LIMIT							\
				(offsetof(struct tss_struct, x86_tss) +	\
					sizeof(x86_hw_tss_s) - 1)

	#define INIT_THREAD_INFO(tsk)	{	\
				.flags		= 0,		\
			}

#endif /* _ASM_X86_THREAD_INFO_H_ */