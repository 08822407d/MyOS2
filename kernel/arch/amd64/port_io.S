#include <arch/amd64/asm.h>

/*	inb() - Input one byte		Author: Kees J. Bot */
/*								18 Mar 1996 */
/*	unsigned inb(U16_t port); */
ENTRY(inb)
	push	%rbp
	movq	%rsp, %rbp
	movq	16(%rbp), %rdx	/* port */
	xorq	%rax, %rax
	inb		%dx				/* read 1 byte */
	pop		%rbp
	ret

/*	inl() - Input one dword		Author: Kees J. Bot */
/*								18 Mar 1996 */
/*	unsigned inl(U16_t port); */
ENTRY(inl)
	push	%rbp
	movq	%rsp, %rbp
	movq	16(%rbp), %rdx	/* port */
	inl		%dx				/* read 1 dword */
	pop		%rbp
	ret

/*	intr_disable(), intr_enable - Disable/Enable hardware interrupts. */
/*							Author: Kees J. Bot */
/*								18 Mar 1996 */
/*	void intr_disable(void); */
/*	void intr_enable(void); */
ENTRY(intr_disable)
	cli
	ret

ENTRY(intr_enable)
	sti
	ret

/*	inw() - Input one word		Author: Kees J. Bot */
/*								18 Mar 1996 */
/*	unsigned inw(U16_t port); */
ENTRY(inw)
	push	%rbp
	movq	%rsp, %rbp
	movq	16(%rbp), %rdx	/* port */
	xorq	%rax, %rax
	inw		%dx				/* read 1 word */
	pop		%rbp
	ret

/*	outb() - Output one byte	Author: Kees J. Bot */
/*								18 Mar 1996 */
/*	void outb(U16_t port, U8_t value); */
ENTRY(outb)
	push	%rbp
	movq	%rsp, %rbp
	movq	16(%rbp), %rdx	/* port */
	movq	16+8(%rbp), %rax/* value */
	outb	%dx				/* output 1 byte */
	pop		%rbp
	ret

/*	outl() - Output one dword	Author: Kees J. Bot */
/*								18 Mar 1996 */
/*	void outl(U16_t port, u32_t value); */
ENTRY(outl)
	push	%rbp
	movq	%rsp, %rbp
	movq	16(%rbp), %rdx	/* port */
	movq	16+8(%rbp), %rax/* value */
	outl	%dx				/* output 1 dword */
	pop		%rbp
	ret

/*	outw() - Output one word	Author: Kees J. Bot */
/*								18 Mar 1996 */
/*	void outw(U16_t port, U16_t value); */
ENTRY(outw)
	push	%rbp
	movq	%rsp, %rbp
	movq	16(%rbp), %rdx	/* port */
	movq	16+8(%rbp), %rax/* value */
	outw	%dx				/* output 1 word */
	pop		%rbp
	ret