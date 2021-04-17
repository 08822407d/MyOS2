#ifndef _CONST_H_
#define _CONST_H_

	#define CONST_1K		0x400ULL
	#define SHIFT_1K		10
	#define MASKF_1K(addr)	(addr & ~(CONST_1K - 1))
	#define ALIGN_1K(addr)	(MASKF_1K(addr + CONST_1K - 1))

	#define CONST_4K		0x1000ULL
	#define SHIFT_4K		12
	#define MASKF_4K(addr)	(addr & ~(CONST_4K - 1))
	#define ALIGN_4K(addr)	(MASKF_4K(addr + CONST_4K - 1))

	#define CONST_1M		0x100000ULL
	#define SHIFT_1M		20
	#define MASKF_1M(addr)	(addr & ~(CONST_1M - 1))
	#define ALIGN_1M(addr)	(MASKF_1M(addr + CONST_1M - 1))

	#define CONST_2M		0x200000ULL
	#define SHIFT_2M		21
	#define MASKF_2M(addr)	(addr & ~(CONST_2M - 1))
	#define ALIGN_2M(addr)	(MASKF_2M(addr + CONST_2M - 1))

	#define CONST_1G		0x40000000ULL
	#define SHIFT_1G		30
	#define MASKF_1G(addr)	(addr & ~(CONST_1G - 1))
	#define ALIGN_1G(addr)	(MASKF_1G(addr + CONST_1G - 1))

	#define CONST_2G		0x80000000ULL
	#define SHIFT_2G		31
	#define MASKF_2G(addr)	(addr & ~(CONST_2G - 1))
	#define ALIGN_2G(addr)	(MASKF_2G(addr + CONST_2G - 1))

	#define COSNT_4G		0x100000000ULL
	#define SHIFT_4G		32
	#define MASKF_4G(addr)	(addr & ~(CONST_4G - 1))
	#define ALIGN_4G(addr)	(MASKF_4G(addr + CONST_4G - 1))

	#define CONST_1T		0x10000000000ULL
	#define SHIFT_1T		40
	#define MASKF_1T(addr)	(addr & ~(CONST_1T - 1))
	#define ALIGN_1T(addr)	(MASKF_1T(addr + CONST_1T - 1))


	////alloc_pages zone_select
	//
	#define ZONE_DMA	(1 << 0)
	//
	#define ZONE_NORMAL	(1 << 1)
	//
	#define ZONE_UNMAPED	(1 << 2)

	////struct page attribute
	//	mapped=1 or un-mapped=0 
	#define PG_PTable_Maped	(1 << 0)
	//	init-code=1 or normal-code/data=0
	#define PG_Kernel_Init	(1 << 1)
	//	device=1 or memory=0
	#define PG_Device	(1 << 2)
	//	kernel=1 or user=0
	#define PG_Kernel	(1 << 3)
	//	shared=1 or single-use=0 
	#define PG_Shared	(1 << 4)

	// ////struct page attribute (alloc_pages flags)
	// //
	// #define PG_PTable_Maped	(1 << 0)
	// //
	// #define PG_Kernel_Init	(1 << 1)
	// //
	// #define PG_Referenced	(1 << 2)
	// //
	// #define PG_Dirty	(1 << 3)
	// //
	// #define PG_Active	(1 << 4)
	// //
	// #define PG_Up_To_Date	(1 << 5)
	// //
	// #define PG_Device	(1 << 6)
	// //
	// #define PG_Kernel	(1 << 7)
	// //
	// #define PG_K_Share_To_U	(1 << 8)
	// //
	// #define PG_Slab		(1 << 9)

	// process and task consts
	#define PROC_KSTACK_SIZE	(32 * CONST_1K)

	// Slab consts
	#define SLAB_LEVEL			16
	#define SLAB_SIZE_BASE		32

#endif /* _CONST_H_ */