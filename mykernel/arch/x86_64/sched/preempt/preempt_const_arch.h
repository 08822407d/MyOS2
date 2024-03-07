// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
/* Written 2000 by Andi Kleen */
#ifndef _ASM_X86_PREEMPT_CONST_H_
#define _ASM_X86_PREEMPT_CONST_H_

	/* We use the MSB mostly because its available */
	#define PREEMPT_NEED_RESCHED	0x80000000

	/*
	 * We use the PREEMPT_NEED_RESCHED bit as an inverted NEED_RESCHED such
	 * that a decrement hitting 0 means we can and should reschedule.
	 */
	#define PREEMPT_ENABLED			(0 + PREEMPT_NEED_RESCHED)

#endif /* _ASM_X86_PREEMPT_CONST_H_ */
