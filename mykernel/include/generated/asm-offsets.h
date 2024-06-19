
#ifndef __ASM_OFFSETS_H__
#define __ASM_OFFSETS_H__

	pt_regs_bx $40
	pt_regs_cx $88
	pt_regs_dx $96
	pt_regs_sp $160
	pt_regs_bp $32
	pt_regs_si $104
	pt_regs_di $112
	pt_regs_r8 $72
	pt_regs_r9 $64
	pt_regs_r10 $56
	pt_regs_r11 $48
	pt_regs_r12 $24
	pt_regs_r13 $16
	pt_regs_r14 $8
	pt_regs_r15 $0
	pt_regs_flags $152
	
	
	
	TASK_threadsp $352
	
	
	PTREGS_SIZE $176
	TSS_sp0 $4
	TSS_sp1 $12
	TSS_sp2 $20
	X86_top_of_stack $24
	X86_current_task $0

#endif
