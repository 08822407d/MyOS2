/* SPDX-License-Identifier: GPL-2.0 */
/* Written 2000 by Andi Kleen */
#ifndef _ASM_X86_DESC_DEFS_H
#define _ASM_X86_DESC_DEFS_H

	/*
	 * Segment descriptor structure definitions, usable from both x86_64 and i386
	 * archs.
	 */

	#ifndef __ASSEMBLY__

	#	include <linux/kernel/types.h>

		/* 8 byte segment descriptor */
		typedef struct desc_struct {
			u16	limit0;
			u16	base0;
			u16	base1: 8, type: 4, s: 1, dpl: 2, p: 1;
			u16	limit1: 4, avl: 1, l: 1, d: 1, g: 1, base2: 8;
		} desc_s;
		// } desc_s __attribute__((packed));

	#	define GDT_ENTRY_INIT(flags, base, limit)	{		\
					.limit0		= (u16) (limit),			\
					.limit1		= ((limit) >> 16) & 0x0F,	\
					.base0		= (u16) (base),				\
					.base1		= ((base) >> 16) & 0xFF,	\
					.base2		= ((base) >> 24) & 0xFF,	\
					.type		= (flags & 0x0f),			\
					.s			= (flags >> 4) & 0x01,		\
					.dpl		= (flags >> 5) & 0x03,		\
					.p			= (flags >> 7) & 0x01,		\
					.avl		= (flags >> 12) & 0x01,		\
					.l			= (flags >> 13) & 0x01,		\
					.d			= (flags >> 14) & 0x01,		\
					.g			= (flags >> 15) & 0x01,		\
				}

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
		} idt_bits_s;
		// } idt_bits_s __attribute__((packed));

		struct idt_data {
			unsigned int	vector;
			unsigned int	segment;
			idt_bits_s		bits;
			const void		*addr;
		};

		struct gate_struct {
			u16			offset_low;
			u16			segment;
			idt_bits_s	bits;
			u16			offset_middle;
			u32			offset_high;
			u32			reserved;
		} __attribute__((packed));

		typedef struct gate_struct gate_desc;

		static inline unsigned long gate_offset(const gate_desc *g) {
			return g->offset_low | ((unsigned long)g->offset_middle << 16) |
				((unsigned long) g->offset_high << 32);
		}

		static inline unsigned long gate_segment(const gate_desc *g) {
			return g->segment;
		}

		struct desc_ptr {
			unsigned short size;
			unsigned long address;
		} __attribute__((packed)) ;

	#endif /* !__ASSEMBLY__ */

	/* Boot IDT definitions */
	#define	BOOT_IDT_ENTRIES	32

	/* Access rights as returned by LAR */
	#define AR_TYPE_RODATA			(0 * (1 << 9))
	#define AR_TYPE_RWDATA			(1 * (1 << 9))
	#define AR_TYPE_RODATA_EXPDOWN	(2 * (1 << 9))
	#define AR_TYPE_RWDATA_EXPDOWN	(3 * (1 << 9))
	#define AR_TYPE_XOCODE			(4 * (1 << 9))
	#define AR_TYPE_XRCODE			(5 * (1 << 9))
	#define AR_TYPE_XOCODE_CONF		(6 * (1 << 9))
	#define AR_TYPE_XRCODE_CONF		(7 * (1 << 9))
	#define AR_TYPE_MASK			(7 * (1 << 9))

	#define AR_DPL0					(0 * (1 << 13))
	#define AR_DPL3					(3 * (1 << 13))
	#define AR_DPL_MASK				(3 * (1 << 13))

	#define AR_A					(1 << 8)   /* "Accessed" */
	#define AR_S					(1 << 12)  /* If clear, "System" segment */
	#define AR_P					(1 << 15)  /* "Present" */
	#define AR_AVL					(1 << 20)  /* "AVaiLable" (no HW effect) */
	#define AR_L					(1 << 21)  /* "Long mode" for code segments */
	#define AR_DB					(1 << 22)  /* D/B, effect depends on type */
	#define AR_G					(1 << 23)  /* "Granularity" (limit in pages) */

#endif /* _ASM_X86_DESC_DEFS_H */
