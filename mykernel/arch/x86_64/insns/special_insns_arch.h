/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SPECIAL_INSNS_H_
#define _ASM_X86_SPECIAL_INSNS_H_

	#include <linux/compiler/myos_debug_option.h>
	#include <asm/insns.h>
	#include <asm/mm.h>

	/*
	 * The compiler should not reorder volatile asm statements with respect to each
	 * other: they should execute in program order. However GCC 4.9.x and 5.x have
	 * a bug (which was fixed in 8.1, 7.3 and 6.5) where they might reorder
	 * volatile asm. The write functions are not affected since they have memory
	 * clobbers preventing reordering. To prevent reads from being reordered with
	 * respect to writes, use a dummy memory operand.
	 */

	#define __FORCE_ORDER "m"(*(unsigned int *)0x1000UL)

	#ifdef DEBUG

		extern ulong
		read_cr0(void);
		void
		write_cr0(ulong val);

		extern ulong
		read_cr2(void);
		extern void
		write_cr2(ulong val);

		extern ulong
		__read_cr3(void);
		extern void
		write_cr3(ulong val);
		extern ulong
		read_cr3_pa(void);
		extern void
		load_cr3(pgd_t *pgdir);
		
		extern ulong
		read_cr4(void);

		extern void
		native_wbinvd(void);

	#endif

	#if defined(ARCH_INSTRUCTION_DEFINATION) || !(DEBUG)
	
		// static inline unsigned long native_read_cr0(void) {
		PREFIX_STATIC_INLINE
		ulong
		read_cr0(void) {
			ulong val;
			asm volatile(	"mov	%%cr0,	%0		\n\t"
						:	"=r"(val)
						:	__FORCE_ORDER
						);
			return val;
		}

		// void native_write_cr0(unsigned long val)
		PREFIX_STATIC_INLINE
		void
		write_cr0(ulong val) {
			ulong bits_missing = 0;

		set_register:
			asm volatile(	"mov	%0,		%%cr0	\n\t"
						:	"+r" (val)
						:
						:	"memory"
						);

			// if (static_branch_likely(&cr_pinning)) {
			// 	if (unlikely((val & X86_CR0_WP) != X86_CR0_WP)) {
			// 		bits_missing = X86_CR0_WP;
			// 		val |= bits_missing;
			// 		goto set_register;
			// 	}
			// 	/* Warn after we've set the missing bits. */
			// 	WARN_ONCE(bits_missing, "CR0 WP bit went missing!?\n");
			// }
		}

		// static __always_inline unsigned long native_read_cr2(void) {
		PREFIX_STATIC_AWLWAYS_INLINE
		ulong
		read_cr2(void) {
			ulong val;
			asm volatile(	"mov	%%cr2,	%0		\n\t"
						:	"=r"(val)
						:	__FORCE_ORDER
						);
			return val;
		}

		// static __always_inline void native_write_cr2(unsigned long val) {
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		write_cr2(ulong val) {
			asm volatile(	"mov	%0,		%%cr2	\n\t"
						:
						:	"r"(val)
						:	"memory"
						);
		}

		// static inline unsigned long __native_read_cr3(void) {
		PREFIX_STATIC_INLINE
		ulong
		__read_cr3(void) {
			ulong val;
			asm volatile(	"mov	%%cr3,	%0		\n\t"
						:	"=r"(val)
						:	__FORCE_ORDER
						);
			return val;
		}

		// static inline void native_write_cr3(unsigned long val) {
		PREFIX_STATIC_INLINE
		void
		write_cr3(ulong val) {
			asm volatile(	"mov	%0,		%%cr3	\n\t"
						:
						:	"r"(val)
						:	"memory"
						);
		}

		/*
		 * Friendlier CR3 helpers.
		 */
		// static inline unsigned long native_read_cr3_pa(void)
		PREFIX_STATIC_INLINE
		ulong
		read_cr3_pa(void) {
			return __read_cr3() & CR3_ADDR_MASK;
		}

		PREFIX_STATIC_INLINE
		void
		load_cr3(pgd_t *pgdir) {
			write_cr3(__pa(pgdir));
		}

		// static inline unsigned long native_read_cr4(void) {
		PREFIX_STATIC_INLINE
		ulong
		read_cr4(void) {
			ulong val;
			/* CR4 always exists on x86_64. */
			asm volatile(	"mov	%%cr4,	%0		\n\t"
						:	"=r"(val)
						:	__FORCE_ORDER
						);
			return val;
		}

		PREFIX_STATIC_INLINE
		void
		native_wbinvd(void) {
			asm volatile(	"wbinvd"
						:
						:
						:	"memory"
						);
		}

	#endif

#endif /* _ASM_X86_SPECIAL_INSNS_H_ */