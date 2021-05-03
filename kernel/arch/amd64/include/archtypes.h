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
	typedef union
	{
		uint64_t		PML4E;
		PML4E_defs_s	PML4E_defs;
	} PML4E_u;
	
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
	typedef union
	{
		uint64_t		PDPTE;
		PDPTE_defs_s	PDPTE_defs;
	} PDPTE_u;
	typedef union
	{
		uint64_t		PDE;
		PDE_defs_s		PDE_defs;
	} PDE_u;
	typedef union
	{
		uint64_t		PTE;
		PTE_defs_s		PTE_defs;
	} PTE_u;
	
/*===========================================================================*
 *								APIC structs								 *
 *===========================================================================*/
	/* LVT */
	typedef struct __attribute__((packed))
	{
		uint32_t	vector			:8,		//0~7	ALL
					deliver_mode	:3,		//8~10	      CMCI LINT0 LINT1 PerformCounter ThermalSensor
					res_1			:1,		//11
					deliver_status	:1,		//12	ALL
					polarity		:1,		//13	           LINT0 LINT1
					irr				:1,		//14	           LINT0 LINT1
					trigger			:1,		//15	           LINT0 LINT1
					mask			:1,		//16	ALL
					timer_mode		:2,		//17~18	Timer
					res_2			:13;	//19~31
	} apic_lvt_s;

	/* ICR */
	typedef struct __attribute__((packed))
	{
		uint32_t	vector			:8,		//0~7
					deliver_mode	:3,		//8~10
					dest_mode		:1,		//11
					deliver_status	:1,		//12
					res_1			:1,		//13
					level			:1,		//14
					trigger			:1,		//15
					res_2			:2,		//16~17
					dest_shorthand	:2,		//18~19
					res_3			:12;	//20~31
		
		union {
			struct {
				uint32_t	res_4		:24,//32~55
							dest_field	:8;	//56~63		
			} apic_dst;
				
			uint32_t	x2apic_dst;			//32~63
		} dst;
			
	} intcmd_reg_s;

	/* RTE */
	typedef struct __attribute__((packed))
	{
		uint32_t	vector			:8,		//0~7
					deliver_mode	:3,		//8~10
					dest_mode		:1,		//11
					deliver_status	:1,		//12
					polarity		:1,		//13
					irr				:1,		//14
					trigger			:1,		//15
					mask			:1,		//16
					reserved		:15;	//17~31

		union{
			struct {
				uint32_t	reserved1	:24,//32~55
							phy_dest	:4,	//56~59
							reserved2	:4;	//60~63
			} physical;

			struct {
				uint32_t	reserved1		:24,//32~55
							logical_dest	:8;	//56~63
			} logical;
		} dst;
	} ioapic_retentry_s;


/*===========================================================================*
 *								CPUID										 *
 *===========================================================================*/
	typedef struct {
		char		id_string[13];
		char		cpu_model[49];
		uint8_t		family_code;
		uint8_t		extend_family;
		uint8_t		model_number;
		uint8_t		extend_model;
		uint8_t		processor_type;
		uint8_t		stepping_id;
		uint8_t		physical_address_width;
		uint8_t		linear_address_width;
		uint32_t	max_basic_opcode;
		uint32_t	max_extend_opcode;
	} cpu_info_s ;

#endif /* #ifndef _AMD64_TYPES_H */