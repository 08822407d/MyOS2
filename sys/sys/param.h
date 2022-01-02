#ifndef _SYS_PARAM_H_
#define _SYS_PARAM_H_

#include <include/ktypes.h>

/* Number of processes contained in the system image. */
#define NR_BOOT_PROCS   (NR_TASKS + LAST_SPECIAL_PROC_NR + 1)

	typedef struct
	{
		int		page_mm;
		int		slab;

		int		idle_task;
		int		vfs;
	} init_flags_s;
	
/* This is used to obtain system information through SYS_GETINFO. */

	typedef struct kinfo {
		phys_addr_t	kernel_phy_base;
		virt_addr_t	kernel_vir_base;
		virt_addr_t	kernel_vir_end;

		unsigned	nr_lcpu;
		unsigned	nr_task;           /* number of kernel tasks */

		reg_t		max_phys_mem;

		int		vm_allocated_bytes; /* allocated by kernel to load vm */
		int		kernel_allocated_bytes;		/* used by kernel */
		int		kernel_allocated_bytes_dynamic;	/* used by kernel (runtime) */
		
		init_flags_s	init_flags;
		arch_init_flags_s	arch_init_flags;
	} kinfo_s;

	typedef struct framebuffer
	{
		phys_addr_t	FB_phybase;
		virt_addr_t	FB_virbase;
		uint64_t	FB_size;

		uint32_t	PixperScanline;
		uint32_t	X_Resolution;
		uint32_t	Y_Resolution;
	} framebuffer_s;

	typedef struct time
	{
		int second;	//00
		int minute;	//02
		int hour;	//04
		int day;	//07
		int month;	//08
		int year;	//09+32
	} time_s;

#endif