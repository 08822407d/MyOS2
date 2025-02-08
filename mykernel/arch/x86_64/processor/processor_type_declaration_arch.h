#ifndef _ASM_X86_PROCESSOR_TYPE_DECLARATIONS_H_
#define _ASM_X86_PROCESSOR_TYPE_DECLARATIONS_H_


	/* processor */
	struct cpuinfo_x86;
	typedef struct cpuinfo_x86 cpuinfo_x86_s;
	struct cpuid_regs;
	typedef struct cpuid_regs cpuid_regs_s;
	struct x86_hw_tss;
	typedef struct x86_hw_tss x86_hw_tss_s;
	struct x86_io_bitmap;
	typedef struct x86_io_bitmap x86_io_bitmap_s;
	struct fixed_percpu_data;
	typedef struct fixed_percpu_data fixed_pcpudata_s;
	struct thread_struct;
	typedef struct thread_struct thread_s;

#endif /* _ASM_X86_PROCESSOR_TYPE_DECLARATIONS_H_ */