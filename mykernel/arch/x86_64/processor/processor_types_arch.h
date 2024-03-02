/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PROCESSOR_TYPES_H_
#define _ASM_X86_PROCESSOR_TYPES_H_

    #include <linux/compiler/compiler_types.h>
    #include <linux/kernel/types.h>

    #include <asm/cpufeatures.h>
	#include <asm/page_types.h>

	/*
	 *  CPU type and hardware bug flags. Kept separately for each CPU.
	 *  Members of this structure are referenced in head_32.S, so think twice
	 *  before touching them. [mj]
	 */

	typedef struct cpuinfo_x86 {
		__u8	x86_family;		/* CPU family */
		__u8	x86_vendor;		/* CPU vendor */
		__u8	x86_model;
		__u8	x86_stepping;
		/* Number of 4K pages in DTLB/ITLB combined(in pages): */
		int		x86_tlbsize;
	#ifdef CONFIG_X86_VMX_FEATURE_NAMES
		__u32	vmx_capability[NVMXINTS];
	#endif
		__u8	x86_virt_bits;
		__u8	x86_phys_bits;
		/* CPUID returned core id bits: */
		__u8	x86_coreid_bits;
		__u8	cu_id;
		/* Max extended CPUID function supported: */
		/* or to say maximum CPUID extend opcode */
		__u32	extended_cpuid_level;
		/* Maximum supported CPUID level, -1=no CPUID: */
		/* or to say maximum CPUID basic opcode */
		int		cpuid_level;

		/*
		 * Align to size of unsigned long because the x86_capability array
		 * is passed to bitops which require the alignment. Use unnamed
		 * union to enforce the array is aligned to size of unsigned long.
		 */
		union
		{
			struct x86_cap_bits x86_capa_bits;
			__u32 x86_capability[NCAPINTS + NBUGINTS];
			unsigned long x86_capability_alignment;
		};

		char	x86_vendor_id[16];
		char	x86_model_id[64];
		/* in KB - valid for CPUS which support this call: */
		unsigned int	x86_cache_size;
		int		x86_cache_alignment;		/* In bytes */
		/* Cache QoS architectural values, valid only on the BSP: */
		int		x86_cache_max_rmid;			/* max index */
		int		x86_cache_occ_scale;		/* scale to bytes */
		int		x86_cache_mbm_width_offset;
		int		x86_power;
		unsigned long	loops_per_jiffy;
		/* cpuid returned max cores value: */
		u16		x86_max_cores;
		u16		apicid;
		u16		initial_apicid;
		u16		x86_clflush_size;
		/* number of cores as seen by the OS: */
		u16		booted_cores;
		/* Physical processor id: */
		u16		phys_proc_id;
		/* Logical processor id: */
		u16		logical_proc_id;
		/* Core id: */
		u16		cpu_core_id;
		u16		cpu_die_id;
		u16		logical_die_id;
		/* Index into per_cpu list: */
		u16		cpu_index;
		/*  Is SMT active on this core? */
		bool	smt_active;
		u32		microcode;
		/* Address space bits used by the cache internally */
		u8		x86_cache_bits;
		unsigned	initialized : 1;
	} cpuinfo_x86_s;

	typedef struct cpuid_regs {
		u32 eax, ebx, ecx, edx;
	} cpuid_regs_s;

	enum cpuid_regs_idx {
		CPUID_EAX = 0,
		CPUID_EBX,
		CPUID_ECX,
		CPUID_EDX,
	};

	/*
	 * Note that while the legacy 'TSS' name comes from 'Task State Segment',
	 * on modern x86 CPUs the TSS also holds information important to 64-bit mode,
	 * unrelated to the task-switch mechanism:
	 */
	typedef struct x86_hw_tss {
		u32 reserved1;
		u64 sp0;
		u64 sp1;

		/*
		* Since Linux does not use ring 2, the 'sp2' slot is unused by
		* hardware.  entry_SYSCALL_64 uses it as scratch space to stash
		* the user RSP value.
		*/
		u64 sp2;

		u64 reserved2;
		u64 ist[7];
		u32 reserved3;
		u32 reserved4;
		u16 reserved5;
		u16 io_bitmap_base;

	} __attribute__((packed)) x86_hw_tss_s;

	/*
	 * All IO bitmap related data stored in the TSS:
	 */
	typedef struct x86_io_bitmap
	{
		/* The sequence number of the last active bitmap. */
		u64 prev_sequence;

		/*
		 * Store the dirty size of the last io bitmap offender. The next
		 * one will have to do the cleanup as the switch out to a non io
		 * bitmap user will just set x86_tss.io_bitmap_base to a value
		 * outside of the TSS limit. So for sane tasks there is no need to
		 * actually touch the io_bitmap at all.
		 */
		unsigned int prev_max;

		/*
		 * The extra 1 is there because the CPU will access an
		 * additional byte beyond the end of the IO permission
		 * bitmap. The extra byte must be all 1 bits, and must
		 * be within the limit.
		 */
		unsigned long bitmap[IO_BITMAP_LONGS + 1];

		/*
		 * Special I/O bitmap to emulate IOPL(3). All bytes zero,
		 * except the additional byte at the end.
		 */
		unsigned long mapall[IO_BITMAP_LONGS + 1];
	} x86_io_bitmap_s;

	struct tss_struct
	{
		/*
		 * The fixed hardware portion.  This must not cross a page boundary
		 * at risk of violating the SDM's advice and potentially triggering
		 * errata.
		 */
		x86_hw_tss_s x86_tss;

		x86_io_bitmap_s io_bitmap;
	} __aligned(PAGE_SIZE);

	typedef struct thread_struct {
		/* Cached TLS descriptors: */
		// desc_s tls_array[GDT_ENTRY_TLS_ENTRIES];
		reg_t			sp;
		unsigned short	es;
		unsigned short	ds;
		unsigned short	fsindex;
		unsigned short	gsindex;

		reg_t			fsbase;
		reg_t			gsbase;

		// /* Save middle states of ptrace breakpoints */
		// struct perf_event *ptrace_bps[HBP_NUM];
		// /* Debug status used for traps, single steps, etc... */
		// unsigned long virtual_dr6;
		// /* Keep track of the exact dr7 value set by the user */
		// unsigned long ptrace_dr7;
		/* Fault info: */
		reg_t			cr2;
		unsigned long	trap_nr;
		unsigned long	error_code;

		// /* IO permissions: */
		// struct io_bitmap *io_bitmap;

		// /*
		// * IOPL. Privilege level dependent I/O permission which is
		// * emulated via the I/O bitmap to prevent user space from disabling
		// * interrupts.
		// */
		// unsigned long iopl_emul;

		// unsigned int iopl_warn : 1;
		// unsigned int sig_on_uaccess_err : 1;

		// /*
		// * Protection Keys Register for Userspace.  Loaded immediately on
		// * context switch. Store it in thread_struct to avoid a lookup in
		// * the tasks's FPU xstate buffer. This value is only valid when a
		// * task is scheduled out. For 'current' the authoritative source of
		// * PKRU is the hardware itself.
		// */
		// u32 pkru;

		// /* Floating point and extended processor state */
		// struct fpu fpu;
		/*
		 * WARNING: 'fpu' is dynamically-sized.  It *MUST* be at
		 * the end.
		 */
	} thread_s;

#endif /* _ASM_X86_PROCESSOR_CONST_H_ */