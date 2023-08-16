// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_FRAME_H
#define _ASM_X86_FRAME_H

	#include <asm/asm.h>

	// /*
	// * These are stack frame creation macros.  They should be used by every
	// * callable non-leaf asm function to make kernel stack traces more reliable.
	// */

	// #ifdef CONFIG_FRAME_POINTER

	// #ifdef __ASSEMBLY__

	// .macro FRAME_BEGIN
	// 	push %_ASM_BP
	// 	_ASM_MOV %_ASM_SP, %_ASM_BP
	// .endm

	// .macro FRAME_END
	// 	pop %_ASM_BP
	// .endm

	// /*
	// * This is a sneaky trick to help the unwinder find pt_regs on the stack.  The
	// * frame pointer is replaced with an encoded pointer to pt_regs.  The encoding
	// * is just setting the LSB, which makes it an invalid stack address and is also
	// * a signal to the unwinder that it's a pt_regs pointer in disguise.
	// *
	// * NOTE: This macro must be used *after* PUSH_AND_CLEAR_REGS because it corrupts
	// * the original rbp.
	// */
	// .macro ENCODE_FRAME_POINTER ptregs_offset=0
	// 	leaq 1+\ptregs_offset(%rsp), %rbp
	// .endm

	// #else /* !__ASSEMBLY__ */

	// #define FRAME_BEGIN				\
	// 	"push %" _ASM_BP "\n"			\
	// 	_ASM_MOV "%" _ASM_SP ", %" _ASM_BP "\n"

	// #define FRAME_END "pop %" _ASM_BP "\n"

	// #define ENCODE_FRAME_POINTER			\
	// 	"lea 1(%rsp), %rbp\n\t"

	// static inline unsigned long encode_frame_pointer(struct pt_regs *regs)
	// {
	// 	return (unsigned long)regs + 1;
	// }

	// #endif /* __ASSEMBLY__ */

	// #define FRAME_OFFSET __ASM_SEL(4, 8)

	// #else /* !CONFIG_FRAME_POINTER */

	// #ifdef __ASSEMBLY__

	// .macro ENCODE_FRAME_POINTER ptregs_offset=0
	// .endm

	// #else /* !__ASSEMBLY */

	// #define ENCODE_FRAME_POINTER

	// static inline unsigned long encode_frame_pointer(struct pt_regs *regs)
	// {
	// 	return 0;
	// }

	// #endif

	// #define FRAME_BEGIN
	// #define FRAME_END
	// #define FRAME_OFFSET 0

	// #endif /* CONFIG_FRAME_POINTER */

#endif /* _ASM_X86_FRAME_H */
