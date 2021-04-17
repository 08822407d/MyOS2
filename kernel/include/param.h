
#ifndef _OS_PARAM_H
#define _OS_PARAM_H 1

#include "ktypes.h"
#include "../arch/amd64/include/multiboot2.h"

/* Number of processes contained in the system image. */
#define NR_BOOT_PROCS   (NR_TASKS + LAST_SPECIAL_PROC_NR + 1)

/* This is used to obtain system information through SYS_GETINFO. */

typedef struct kinfo {
	phy_addr	kernel_phy_base;
	vir_addr	kernel_vir_base;
	vir_addr	kernel_vir_end;

	int		nr_procs;           /* number of user processes */
	int		nr_tasks;           /* number of kernel tasks */
	// char	release[6];        /* kernel release number */
	// char	version[6];        /* kernel version number */
	int		vm_allocated_bytes; /* allocated by kernel to load vm */
	int		kernel_allocated_bytes;		/* used by kernel */
	int		kernel_allocated_bytes_dynamic;	/* used by kernel (runtime) */
} kinfo_s;

typedef struct framebuffer
{
	phy_addr	FB_phybase;
	vir_addr	FB_virbase;
	uint64_t	FB_size;

	uint32_t	PixperScanline;
	uint32_t	X_Resolution;
	uint32_t	Y_Resolution;
} framebuffer_s;


#endif