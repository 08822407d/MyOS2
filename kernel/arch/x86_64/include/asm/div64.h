/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_DIV64_H
#define _ASM_X86_DIV64_H

	# include <linux/kernel/asm-generic/div64.h>

	/*
	 * Will generate an #DE when the result doesn't fit u64, could fix with an
	 * __ex_table[] entry when it becomes an issue.
	 */
	static inline u64 mul_u64_u64_div_u64(u64 a, u64 mul, u64 div) {
		u64 q;
		asm (	"mulq	%2		\n\t"
				"divq	%3		\n\t"
			:	"=a"(q)
			:	"a"(a), "rm"(mul), "rm"(div)
			:	"rdx");
		return q;
	}
	#define mul_u64_u64_div_u64 mul_u64_u64_div_u64

	static inline u64 mul_u64_u32_div(u64 a, u32 mul, u32 div) {
		return mul_u64_u64_div_u64(a, mul, div);
	}
	#define mul_u64_u32_div	mul_u64_u32_div

#endif /* _ASM_X86_DIV64_H */
