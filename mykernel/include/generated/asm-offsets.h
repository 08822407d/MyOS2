
#ifndef __ASM_OFFSETS_H__
#define __ASM_OFFSETS_H__
/*
 * DO NOT MODIFY.
 *
 * This file was generated by Kbuild
 */


	#define pt_regs_bx 40
	#define pt_regs_cx 88
	#define pt_regs_dx 96
	#define pt_regs_sp 160
	#define pt_regs_bp 32
	#define pt_regs_si 104
	#define pt_regs_di 112
	#define pt_regs_r8 72
	#define pt_regs_r9 64
	#define pt_regs_r10 56
	#define pt_regs_r11 48
	#define pt_regs_r12 24
	#define pt_regs_r13 16
	#define pt_regs_r14 8
	#define pt_regs_r15 0
	#define pt_regs_flags 152
	
	#define R15 0
	#define R14 8
	#define R13 16
	#define R12 24
	#define RBP 32
	#define RBX 40
	#define R11 48
	#define R10 56
	#define R9 64
	#define R8 72
	#define RAX 80
	#define RCX 88
	#define RDX 96
	#define RSI 104
	#define RDI 112
	#define IRQ_NR 120
	#define ORIG_RAX 128
	#define RIP 136
	#define CS 144
	#define EFLAGS 152
	#define RSP 160
	#define SS 168
	
	
	
	#define TASK_threadsp 352
	
	
	#define PTREGS_SIZE 176
	#define TSS_sp0 4
	#define TSS_sp1 12
	#define TSS_sp2 20
	#define X86_top_of_stack 24
	#define X86_current_task 0

#endif
