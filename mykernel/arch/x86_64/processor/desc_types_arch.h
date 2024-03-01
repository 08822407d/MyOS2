// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
/* Written 2000 by Andi Kleen */
#ifndef _ASM_X86_DESC_DEFS_H
#define _ASM_X86_DESC_DEFS_H

	#include "desc_const_arch.h"

	/*
	 * Segment descriptor structure definitions, usable from both x86_64 and i386
	 * archs.
	 */

	#ifndef __ASSEMBLY__

	#  include <linux/kernel/types.h>

		// Intel Manual Volume 3 - June 2023
		// Chapter 5: Protection
		/* 8 byte segment descriptor */
		typedef struct desc_struct {
			u16	limit0;
			u16	base0;
			u16	base1	: 8,
				type	: 4,
				s		: 1,
				dpl		: 2,
				p		: 1;
			u16	limit1	: 4,
				avl		: 1,
				l		: 1,
				d		: 1,
				g		: 1,
				base2	: 8;
		} __attribute__((packed)) desc_s;


		enum {
			GATE_INTERRUPT	= 0xE,
			GATE_TRAP		= 0xF,
			GATE_CALL		= 0xC,
			GATE_TASK		= 0x5,
		};

		enum {
			DESC_TSS	= 0x9,
			DESC_LDT	= 0x2,
			DESCTYPE_S	= 0x10,	/* !system */
		};

		// Intel Manual Volume 3 Chapter 8: Task Management
		/* LDT or TSS descriptor in the GDT. */
		struct ldttss_desc {
			u16	limit0;
			u16	base0;

			u16	base1	: 8,
				type	: 5,
				dpl		: 2,
				p		: 1;
			u16	limit1	: 4,
				zero0	: 3,
				g		: 1,
				base2	: 8;
			u32	base3;
			u32	zero1;
		} __attribute__((packed));
		typedef struct ldttss_desc ldt_desc;
		typedef struct ldttss_desc tss_desc;

		typedef struct idt_bits {
			u16	ist		: 3,
				zero	: 5,
				type	: 5,
				dpl		: 2,
				p		: 1;
		} __attribute__((packed)) idt_bits_s;

		struct idt_data {
			unsigned int	vector;
			unsigned int	segment;
			idt_bits_s		bits;
			const void		*addr;
		};

		// Intel Manual Volume 3 Chapter 6: Interrupt and Exception Handling
		struct gate_struct {
			u16			offset_low;
			u16			segment;
			idt_bits_s	bits;
			u16			offset_middle;
			u32			offset_high;
			u32			reserved;
		} __attribute__((packed));
		typedef struct gate_struct gate_desc;


		struct desc_ptr {
			unsigned short size;
			unsigned long address;
		} __attribute__((packed)) ;

		struct gdt_page {
			desc_s gdt[GDT_ENTRIES];
		} __attribute__((aligned(PAGE_SIZE)));

	#endif /* !__ASSEMBLY__ */

#endif /* _ASM_X86_DESC_DEFS_H */
