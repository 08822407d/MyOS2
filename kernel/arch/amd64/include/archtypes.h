#ifndef _AMD64_TYPES_H
#define _AMD64_TYPES_H

#include <sys/types.h>
#include <sys/cdefs.h>

enum SegType {	LDTdesc = 2,
				TSS_AVAIL = 9,
				TSS_BUSY = 11,
				CALLGATE = 12,
				INTGATE = 14,
				TRAPGATE = 15 };

typedef struct __attribute__((packed)) {/* segment descriptor for protected mode */
	uint16_t			: 16;
	uint32_t			: 24;
	uint8_t		Type	: 4;
	uint8_t		Sflag	: 1;
	uint8_t		Privil	: 2;
	uint8_t		Present	: 1;
	uint8_t				: 6;
	uint8_t 	Lflag	: 1; // 0=dataseg, 1=codeseg
	uint16_t			: 9;
} segdesc64_s;

typedef struct __attribute__((packed)) {
	uint16_t	leng	: 16;
	uint16_t	base1	: 16;
	uint32_t	attr	: 32;
	uint32_t	base2	: 32;
	uint32_t			: 32;
} gatedesc64_s;

typedef struct __attribute__((packed)) {
	uint32_t	limit;
	segdesc64_s	*base;
} desctblptr64_s;

typedef struct {
} segframe_s;

/* protect.c */
typedef struct __attribute__((packed)) {
	uint32_t		: 32;
	uint64_t rsp0	: 64;
	uint64_t rsp1	: 64;
	uint64_t rsp2	: 64;
	uint64_t		: 64;
	uint64_t ist1	: 64;
	uint64_t ist2	: 64;
	uint64_t ist3	: 64;
	uint64_t ist4	: 64;
	uint64_t ist5	: 64;
	uint64_t ist6	: 64;
	uint64_t ist7	: 64;
	uint64_t		: 64;
	uint32_t		: 32;
	uint32_t iobm	: 32;
} tss64_s;

typedef struct {
} cpu_info_s ;

// typedef u32_t atomic_t;	/* access to an aligned 32bit value is atomic on i386 */

#endif /* #ifndef _AMD64_TYPES_H */