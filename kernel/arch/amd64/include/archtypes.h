#ifndef _AMD64_TYPES_H
#define _AMD64_TYPES_H

#include <sys/types.h>
#include <sys/cdefs.h>

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
	INTGATE		= 14,
	TRAPGATE	= 15
} SysSegType;

/* 64bit version segdesc for common segment */
typedef struct __attribute__((packed)) {
	uint16_t			: 16; // unused
	uint32_t			: 24; // unused
	uint8_t		Type	: 4;
	uint8_t		Sflag	: 1; // 0=system seg, 1=common seg
	uint8_t		Privil	: 2;
	uint8_t		Present	: 1;
	uint8_t				: 6; // unused
	uint8_t 	Lflag	: 1; // 0=dataseg, 1=codeseg
	uint16_t			: 9; // unused
} segdesc64_s;

/* 64bit version segdesc for TSS segment */
typedef struct  __attribute__((packed)) {
	uint16_t	Limit1	: 16;
	uint32_t	Base1	: 24;
	uint8_t		Type	: 4;
	uint8_t				: 1; // Sflag must be 0
	uint8_t		DPL		: 2;
	uint8_t		Present	: 1;
	uint8_t		Limit2	: 4;
	uint8_t		AVL		: 1;
	uint8_t				: 3; // unused
	uint64_t	Base2	: 40;
	uint32_t			: 32; // unused
} TSSsegdesc;

/* 64bit version segdesc for gate */
typedef struct __attribute__((packed)) {
	uint16_t	offs1	: 16;
	uint16_t	segslct	: 16;
	uint8_t		IST		: 3;
	uint8_t				: 5; // unused
	uint8_t		Type	: 4;
	uint8_t				: 1; // Sflag must be 0
	uint8_t		DPL		: 2;
	uint8_t		Present	: 1;
	uint64_t	offs	: 48;
	uint32_t			: 32; // unused
} gatedesc64_s;

/* desc selector */
typedef struct __attribute__((packed)) {
	uint8_t		RPL		: 2;
	uint8_t				: 1; // TIflag we always set it to 0
	uint16_t	Index	: 13;
} descselector_s;

/* input for 64bit version instruction lgdt and lidt */
typedef struct __attribute__((packed)) {
	uint32_t	limit;
	segdesc64_s	*base;
} desctblptr64_s;

typedef struct {
} segframe_s;

/* 64bit version TSS */
typedef struct __attribute__((packed)) {
	uint32_t		: 32; // unused
	uint64_t rsp0	: 64;
	uint64_t rsp1	: 64;
	uint64_t rsp2	: 64;
	uint64_t		: 64; // unused
	uint64_t ist1	: 64;
	uint64_t ist2	: 64;
	uint64_t ist3	: 64;
	uint64_t ist4	: 64;
	uint64_t ist5	: 64;
	uint64_t ist6	: 64;
	uint64_t ist7	: 64;
	uint64_t		: 64; // unused
	uint32_t		: 32; // unused
	uint32_t iobm	: 32;
} tss64_s;

typedef struct {
} cpu_info_s ;

// typedef u32_t atomic_t;	/* access to an aligned 32bit value is atomic on i386 */

#endif /* #ifndef _AMD64_TYPES_H */