#ifndef _AMD64_TYPES_H
#define _AMD64_TYPES_H

#include <sys/types.h>
#include <sys/cdefs.h>

/*===========================================================================*
 *								segment structs								 *
 *===========================================================================*/
	/* type field of segdesc, for codeseg and dataseg */
	typedef enum
	{
		// R=read, W=write, E=executable, A=accessed
		R_DATA		= 0,
		RA_DATA		= 1,
		RW_DATA		= 2,
		RWA_DATA	= 3,

		E_CODE		= 8,
		EA_CODE		= 9,
		ER_CODE		= 10,
		ERA_CODE	= 11,
	} CommSegType;
	/* type field of segdesc, for system segment */
	typedef enum
	{	
		LDTdesc		= 2,
		TSS_AVAIL	= 9,
		TSS_BUSY	= 11,
		CALLGATE	= 12,
		INTRGATE	= 14,
		TRAPGATE	= 15
	} SysSegType;


	/* 64bit version segdesc for common segment */

	typedef struct __attribute__((packed)) {
		uint64_t			: 16, // unused
							: 24, // unused
					Type	: 4,
					Sflag	: 1, // 0=system seg, 1=common seg
					DPL		: 2,
					Pflag	: 1, // segment present
							: 5, // unused
					Lflag	: 1, // 0=dataseg, 1=codeseg
							: 10; // unused
	} segdesc64_s;

	/* 64bit version segdesc for TSS segment */
	typedef struct  __attribute__((packed)) {
		uint64_t	Limit1	: 16,
					Base1	: 24,
					Type	: 4,
							: 1, // Sflag must be 0
					DPL		: 2,
					Pflag	: 1,
					Limit2	: 4,
					AVL		: 1,
							: 3; // unused
		uint64_t	Base2	: 40;
		uint32_t			: 32; // unused
	} TSSsegdesc_s;

	/* 64bit version segdesc for gate */
	typedef struct __attribute__((packed)) {
		uint64_t	offs1	: 16,
					segslct	: 16,
					IST		: 3,
							: 5, // unused
					Type	: 4,
							: 1, // Sflag must be 0
					DPL		: 2,
					Present	: 1;
		uint64_t	offs2	: 48;
		uint32_t			: 32; // unused
	} gatedesc64_s;


	/* input for 64bit version instruction lgdt and lidt */
	typedef struct __attribute__((packed)) {
		uint32_t	limit	: 16;
		uint64_t	base	: 64;
		uint32_t			: 16;
	} desctblptr64_s;

	typedef struct {
	} segframe_s;

	/* 64bit version TSS */
	typedef struct __attribute__((packed)) {
		uint32_t			: 32; // unused
		reg_t		rsp0	: 64;
		reg_t		rsp1	: 64;
		reg_t		rsp2	: 64;
		reg_t				: 64; // unused
		reg_t		ist1	: 64;
		reg_t		ist2	: 64;
		reg_t		ist3	: 64;
		reg_t		ist4	: 64;
		reg_t		ist5	: 64;
		reg_t		ist6	: 64;
		reg_t		ist7	: 64;
		reg_t				: 64; // unused
		uint64_t			: 32, // unused
					iobm	: 32;
	} tss64_s;

/*===========================================================================*
 *								page structs								 *
 *===========================================================================*/
	// typedef struct __attribute__((packed)) {
	// 	uint64_t	Pflag	: 1,
	// 				RWflag	: 1,
	// 				USflag	: 1,
	// 				PWTflag	: 1,
	// 				PCDflag	: 1,
	// 				Aflag	: 1,
	// 						: 6,
	// 				PHYADDR	: 36,
	// 						: 15,
	// 				XDflag	: 1;
	// } PML4E;
	typedef struct __attribute__((packed)) {
		uint64_t	Pflag	: 1,
					RWflag	: 1,
					USflag	: 1,
					PWTflag	: 1,
					PCDflag	: 1,
					Aflag	: 1,
							: 6,
					PHYADDR	: 36,
							: 15,
					XDflag	: 1;
	} PML4E_defs_s;
	typedef struct __attribute__((packed)) {
		uint64_t	FLAGS	: 6,
							: 6,
					PHYADDR	: 36,
							: 15,
					XDflag	: 1;
	} PML4E_s;
	typedef union
	{
		PML4E_s			PML4E;
		PML4E_defs_s	PML4E_defs;
	} PML4E_u;
	
	// typedef struct __attribute__((packed)) {
	// 	uint16_t	Pflag	: 1,
	// 				RWflag	: 1,
	// 				USflag	: 1,
	// 				PWTflag	: 1,
	// 				PCDflag	: 1,
	// 			 	Aflag	: 1,
	// 				Dflag	: 1,
	// 				PATflag	: 1,
	// 				Gflag	: 1,
	// 						: 3;
	// 	uint64_t	PHYADDR	: 36,
	// 						: 15,
	// 				XDflag	: 1;
	// } PDPTE;
	// typedef PDPTE PDE;
	// typedef PDPTE PTE;
	typedef struct __attribute__((packed)) {
		uint16_t	Pflag	: 1,
					RWflag	: 1,
					USflag	: 1,
					PWTflag	: 1,
					PCDflag	: 1,
				 	Aflag	: 1,
					Dflag	: 1,
					PATflag	: 1,
					Gflag	: 1,
							: 3;
		uint64_t	PHYADDR	: 36,
							: 15,
					XDflag	: 1;
	} PDPTE_defs_s;
	typedef PDPTE_defs_s PDE_defs_s;
	typedef PDPTE_defs_s PTE_defs_s;
	typedef struct __attribute__((packed)) {
		uint64_t	FLAGS	: 9,
							: 3,
					PHYADDR	: 36,
							: 15,
					XDflag	: 1;
	} PDPTE_s;
	typedef PDPTE_s PDE_s;
	typedef PDPTE_s PTE_s;
	typedef union
	{
		PDPTE_s			PDPTE;
		PDPTE_defs_s	PDPTE_defs;
	} PDPTE_u;
	typedef union
	{
		PDE_s			PDE;
		PDE_defs_s		PDE_defs;
	} PDE_u;
	typedef union
	{
		PTE_s			PTE;
		PTE_defs_s		PTE_defs;
	} PTE_u;
	


	typedef struct {
	} cpu_info_s ;

	// typedef u32_t atomic_t;	/* access to an aligned 32bit value is atomic on i386 */

#endif /* #ifndef _AMD64_TYPES_H */