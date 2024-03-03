/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_IO_H_
#define _ASM_X86_IO_H_

	#include <linux/compiler/myos_optimize_option.h>
	#include <linux/kernel/types.h>
	#include <asm/page.h>

	#ifdef DEBUG
	
		extern phys_addr_t
		virt_to_phys(volatile virt_addr_t address);
		
		extern virt_addr_t
		phys_to_virt(volatile phys_addr_t address);

		extern void
		slow_down_io(void);

	#endif

	#if defined(ARCH_INSTRUCTION_DEFINATION) || !(DEBUG)

		/**
		 *	virt_to_phys	-	map virtual addresses to physical
		 *	@address: address to remap
		 *
		 *	The returned physical address is the physical (CPU) mapping for
		 *	the memory address given. It is only valid to use this function on
		 *	addresses directly mapped or allocated via kmalloc.
		 *
		 *	This function does not give bus mappings for DMA transfers. In
		 *	almost all conceivable cases a device driver should not be using
		 *	this function
		 */
		PREFIX_STATIC_INLINE
		phys_addr_t
		virt_to_phys(volatile virt_addr_t address) {
			return __pa(address);
		}
		// #define virt_to_phys	virt_to_phys

		/**
		 *	phys_to_virt	-	map physical address to virtual
		 *	@address: address to remap
		 *
		 *	The returned virtual address is a current CPU mapping for
		 *	the memory address given. It is only valid to use this function on
		 *	addresses that have a kernel mapping
		 *
		 *	This function does not handle bus mappings for DMA transfers. In
		 *	almost all conceivable cases a device driver should not be using
		 *	this function
		 */
		PREFIX_STATIC_INLINE
		virt_addr_t
		phys_to_virt(volatile phys_addr_t address) {
			return __va(address);
		}
		// #define phys_to_virt	phys_to_virt

		PREFIX_STATIC_INLINE
		void
		slow_down_io(void) {
		// 	native_io_delay();
		// #ifdef REALLY_SLOW_IO
		// 	native_io_delay();
		// 	native_io_delay();
		// 	native_io_delay();
		// #endif
			asm volatile(	"mfence		\n\t"
							"nop		\n\t"
							"nop		\n\t"
							"nop		\n\t"
							"nop		\n\t"
						);
		}

	#endif

	/*
	 * This file contains the definitions for the x86 IO instructions
	 * inb/inw/inl/outb/outw/outl and the "string versions" of the same
	 * (insb/insw/insl/outsb/outsw/outsl). You can also use "pausing"
	 * versions of the single-IO instructions (inb_p/inw_p/..).
	 *
	 * This file is not meant to be obfuscating: it's just complicated
	 * to (a) handle it all in a way that makes gcc able to optimize it
	 * as well as possible and (b) trying to avoid writing the same thing
	 * over and over again with slight variations and possibly making a
	 * mistake somewhere.
	 */

	/*
	 * Thanks to James van Artsdalen for a better timing-fix than
	 * the two short jumps: using outb's to a nonexistent port seems
	 * to guarantee better timings even on fast machines.
	 *
	 * On the other hand, I'd like to be sure of a non-existent port:
	 * I feel a bit unsafe about using 0x80 (should be safe, though)
	 *
	 *		Linus
	 */

	/*
	 *  Bit simplified and optimized by Jan Hubicka
	 *  Support of BIGMEM added by Gerhard Wichert, Siemens AG, July 1999.
	 *
	 *  isa_memset_io, isa_memcpy_fromio, isa_memcpy_toio added,
	 *  isa_read[wl] and isa_write[wl] fixed
	 *  - Arnaldo Carvalho de Melo <acme@conectiva.com.br>
	 */
	#define build_mmio_read(name, size, type, reg, barrier)					\
				static inline type											\
				name(const volatile void *addr) {							\
					type ret;												\
					asm volatile(	"mov" size " %1,	%0"					\
								:	reg (ret)								\
								:	"m" (*(volatile type __force *)addr)	\
									barrier);								\
					return ret;												\
				}

	#define build_mmio_write(name, size, type, reg, barrier)				\
				static inline void											\
				name(type val, volatile void *addr) {						\
					asm volatile(	"mov" size " %0,	%1"					\
								:											\
								:	reg (val),								\
									"m" (*(volatile type __force *)addr)	\
									barrier);								\
				}

	build_mmio_read(readb, "b", unsigned char, "=q", :"memory")
	build_mmio_read(readw, "w", unsigned short, "=r", :"memory")
	build_mmio_read(readl, "l", unsigned int, "=r", :"memory")

	build_mmio_read(__readb, "b", unsigned char, "=q", )
	build_mmio_read(__readw, "w", unsigned short, "=r", )
	build_mmio_read(__readl, "l", unsigned int, "=r", )

	build_mmio_write(writeb, "b", unsigned char, "q", :"memory")
	build_mmio_write(writew, "w", unsigned short, "r", :"memory")
	build_mmio_write(writel, "l", unsigned int, "r", :"memory")

	build_mmio_write(__writeb, "b", unsigned char, "q", )
	build_mmio_write(__writew, "w", unsigned short, "r", )
	build_mmio_write(__writel, "l", unsigned int, "r", )

	#define readb					readb
	#define readw					readw
	#define readl					readl
	#define readb_relaxed(a)		__readb(a)
	#define readw_relaxed(a)		__readw(a)
	#define readl_relaxed(a)		__readl(a)
	#define __raw_readb				__readb
	#define __raw_readw				__readw
	#define __raw_readl				__readl

	#define writeb					writeb
	#define writew					writew
	#define writel					writel
	#define writeb_relaxed(v, a)	__writeb(v, a)
	#define writew_relaxed(v, a)	__writew(v, a)
	#define writel_relaxed(v, a)	__writel(v, a)
	#define __raw_writeb			__writeb
	#define __raw_writew			__writew
	#define __raw_writel			__writel

	build_mmio_read(readq, "q", u64, "=r", :"memory")
	build_mmio_read(__readq, "q", u64, "=r", )
	build_mmio_write(writeq, "q", u64, "r", :"memory")
	build_mmio_write(__writeq, "q", u64, "r", )

	#define readq_relaxed(a)		__readq(a)
	#define writeq_relaxed(v, a)	__writeq(v, a)

	#define __raw_readq				__readq
	#define __raw_writeq			__writeq

	/* Let people know that we have them */
	#define readq					readq
	#define writeq					writeq



	#define BUILDIO(bwl, bw, type)									\
				static inline void									\
				out##bwl(unsigned type value, int port) {			\
					asm volatile(	"out" #bwl " %" #bw "0, %w1"	\
								:									\
								:	"a"(value),						\
									"Nd"(port));					\
				}													\
																	\
				static inline unsigned type							\
				in##bwl(int port) {									\
					unsigned type value;							\
					asm volatile(	"in" #bwl " %w1, %" #bw "0"		\
								:	"=a"(value)						\
								:	"Nd"(port));					\
					return value;									\
				}													\
																	\
				static inline void									\
				out##bwl##_p(unsigned type value, int port) {		\
					out##bwl(value, port);							\
					slow_down_io();									\
				}													\
																	\
				static inline unsigned type							\
				in##bwl##_p(int port) {								\
					unsigned type value = in##bwl(port);			\
					slow_down_io();									\
					return value;									\
				}													\
																	\
				static inline void									\
				outs##bwl(int port, const void *addr,				\
						unsigned long count) {						\
					asm volatile(	"rep; outs" #bwl				\
								:	"+S"(addr),						\
									"+c"(count)						\
								:	"d"(port)						\
								:	"memory");						\
				}													\
																	\
				static inline void									\
				ins##bwl(int port, void *addr,						\
						unsigned long count)	{					\
					asm volatile(	"rep; ins" #bwl					\
								:	"+D"(addr),						\
									"+c"(count)						\
								:	"d"(port)						\
								:	"memory");						\
				}

	BUILDIO(b, b, char)
	BUILDIO(w, w, short)
	BUILDIO(l, , int)

	#define inb		inb
	#define inw		inw
	#define inl		inl
	#define inb_p	inb_p
	#define inw_p	inw_p
	#define inl_p	inl_p
	#define insb	insb
	#define insw	insw
	#define insl	insl

	#define outb	outb
	#define outw	outw
	#define outl	outl
	#define outb_p	outb_p
	#define outw_p	outw_p
	#define outl_p	outl_p
	#define outsb	outsb
	#define outsw	outsw
	#define outsl	outsl

	#include <asm-generic/io.h>

#endif /* _ASM_X86_IO_H_ */
