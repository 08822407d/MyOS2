// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
/*
 * linux/include/asm/dma.h: Defines for using and allocating dma channels.
 * Written by Hennus Bergman, 1992.
 * High DMA channel support & info by Hannu Savolainen
 * and John Boyd, Nov. 1992.
 */

#ifndef _ASM_X86_DMA_H
#define _ASM_X86_DMA_H

	#include <linux/kernel/lock_ipc.h>	/* And spinlocks */
	#include <asm/insns.h>		/* need byte IO */

	#ifdef HAVE_REALLY_SLOW_DMA_CONTROLLER
	#	define dma_outb	outb_p
	#else
	#	define dma_outb	outb
	#endif

	#define dma_inb		inb



	// #ifdef CONFIG_ISA_DMA_API
	// extern spinlock_t  dma_spin_lock;

	// static inline unsigned long claim_dma_lock(void)
	// {
	// 	unsigned long flags;
	// 	spin_lock_irqsave(&dma_spin_lock, flags);
	// 	return flags;
	// }

	// static inline void release_dma_lock(unsigned long flags)
	// {
	// 	spin_unlock_irqrestore(&dma_spin_lock, flags);
	// }
	// #endif /* CONFIG_ISA_DMA_API */

	// /* enable/disable a specific DMA channel */
	// static inline void enable_dma(unsigned int dmanr)
	// {
	// 	if (dmanr <= 3)
	// 		dma_outb(dmanr, DMA1_MASK_REG);
	// 	else
	// 		dma_outb(dmanr & 3, DMA2_MASK_REG);
	// }

	// static inline void disable_dma(unsigned int dmanr)
	// {
	// 	if (dmanr <= 3)
	// 		dma_outb(dmanr | 4, DMA1_MASK_REG);
	// 	else
	// 		dma_outb((dmanr & 3) | 4, DMA2_MASK_REG);
	// }

	// /* Clear the 'DMA Pointer Flip Flop'.
	// * Write 0 for LSB/MSB, 1 for MSB/LSB access.
	// * Use this once to initialize the FF to a known state.
	// * After that, keep track of it. :-)
	// * --- In order to do that, the DMA routines below should ---
	// * --- only be used while holding the DMA lock ! ---
	// */
	// static inline void clear_dma_ff(unsigned int dmanr)
	// {
	// 	if (dmanr <= 3)
	// 		dma_outb(0, DMA1_CLEAR_FF_REG);
	// 	else
	// 		dma_outb(0, DMA2_CLEAR_FF_REG);
	// }

	// /* set mode (above) for a specific DMA channel */
	// static inline void set_dma_mode(unsigned int dmanr, char mode)
	// {
	// 	if (dmanr <= 3)
	// 		dma_outb(mode | dmanr, DMA1_MODE_REG);
	// 	else
	// 		dma_outb(mode | (dmanr & 3), DMA2_MODE_REG);
	// }

	// /* Set only the page register bits of the transfer address.
	// * This is used for successive transfers when we know the contents of
	// * the lower 16 bits of the DMA current address register, but a 64k boundary
	// * may have been crossed.
	// */
	// static inline void set_dma_page(unsigned int dmanr, char pagenr)
	// {
	// 	switch (dmanr) {
	// 	case 0:
	// 		dma_outb(pagenr, DMA_PAGE_0);
	// 		break;
	// 	case 1:
	// 		dma_outb(pagenr, DMA_PAGE_1);
	// 		break;
	// 	case 2:
	// 		dma_outb(pagenr, DMA_PAGE_2);
	// 		break;
	// 	case 3:
	// 		dma_outb(pagenr, DMA_PAGE_3);
	// 		break;
	// 	case 5:
	// 		dma_outb(pagenr & 0xfe, DMA_PAGE_5);
	// 		break;
	// 	case 6:
	// 		dma_outb(pagenr & 0xfe, DMA_PAGE_6);
	// 		break;
	// 	case 7:
	// 		dma_outb(pagenr & 0xfe, DMA_PAGE_7);
	// 		break;
	// 	}
	// }


	// /* Set transfer address & page bits for specific DMA channel.
	// * Assumes dma flipflop is clear.
	// */
	// static inline void set_dma_addr(unsigned int dmanr, unsigned int a)
	// {
	// 	set_dma_page(dmanr, a>>16);
	// 	if (dmanr <= 3)  {
	// 		dma_outb(a & 0xff, ((dmanr & 3) << 1) + IO_DMA1_BASE);
	// 		dma_outb((a >> 8) & 0xff, ((dmanr & 3) << 1) + IO_DMA1_BASE);
	// 	}  else  {
	// 		dma_outb((a >> 1) & 0xff, ((dmanr & 3) << 2) + IO_DMA2_BASE);
	// 		dma_outb((a >> 9) & 0xff, ((dmanr & 3) << 2) + IO_DMA2_BASE);
	// 	}
	// }


	// /* Set transfer size (max 64k for DMA0..3, 128k for DMA5..7) for
	// * a specific DMA channel.
	// * You must ensure the parameters are valid.
	// * NOTE: from a manual: "the number of transfers is one more
	// * than the initial word count"! This is taken into account.
	// * Assumes dma flip-flop is clear.
	// * NOTE 2: "count" represents _bytes_ and must be even for channels 5-7.
	// */
	// static inline void set_dma_count(unsigned int dmanr, unsigned int count)
	// {
	// 	count--;
	// 	if (dmanr <= 3)  {
	// 		dma_outb(count & 0xff, ((dmanr & 3) << 1) + 1 + IO_DMA1_BASE);
	// 		dma_outb((count >> 8) & 0xff,
	// 			((dmanr & 3) << 1) + 1 + IO_DMA1_BASE);
	// 	} else {
	// 		dma_outb((count >> 1) & 0xff,
	// 			((dmanr & 3) << 2) + 2 + IO_DMA2_BASE);
	// 		dma_outb((count >> 9) & 0xff,
	// 			((dmanr & 3) << 2) + 2 + IO_DMA2_BASE);
	// 	}
	// }


	// /* Get DMA residue count. After a DMA transfer, this
	// * should return zero. Reading this while a DMA transfer is
	// * still in progress will return unpredictable results.
	// * If called before the channel has been used, it may return 1.
	// * Otherwise, it returns the number of _bytes_ left to transfer.
	// *
	// * Assumes DMA flip-flop is clear.
	// */
	// static inline int get_dma_residue(unsigned int dmanr)
	// {
	// 	unsigned int io_port;
	// 	/* using short to get 16-bit wrap around */
	// 	unsigned short count;

	// 	io_port = (dmanr <= 3) ? ((dmanr & 3) << 1) + 1 + IO_DMA1_BASE
	// 		: ((dmanr & 3) << 2) + 2 + IO_DMA2_BASE;

	// 	count = 1 + dma_inb(io_port);
	// 	count += dma_inb(io_port) << 8;

	// 	return (dmanr <= 3) ? count : (count << 1);
	// }


	// /* These are in kernel/dma.c because x86 uses CONFIG_GENERIC_ISA_DMA */
	// #ifdef CONFIG_ISA_DMA_API
	// extern int request_dma(unsigned int dmanr, const char *device_id);
	// extern void free_dma(unsigned int dmanr);
	// #endif

#endif /* _ASM_X86_DMA_H */
