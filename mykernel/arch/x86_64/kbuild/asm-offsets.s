	.file	"asm-offsets.c"
	.text
.Ltext0:
	.file 0 "/home/cheyh/projs/MyOS2/build/mykernel" "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c"
	.section	.text.main,"ax",@progbits
	.globl	main
	.type	main, @function
main:
.LFB130:
	.file 1 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c"
	.loc 1 15 1
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
.L3:
	leaq	.L3(%rip), %rax
	movabsq	$_GLOBAL_OFFSET_TABLE_-.L3, %r11
	addq	%r11, %rax
	.loc 1 31 2
#APP
# 31 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->pt_regs_bx $40 offsetof(struct pt_regs, bx)"
# 0 "" 2
	.loc 1 32 2
# 32 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->pt_regs_cx $88 offsetof(struct pt_regs, cx)"
# 0 "" 2
	.loc 1 33 2
# 33 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->pt_regs_dx $96 offsetof(struct pt_regs, dx)"
# 0 "" 2
	.loc 1 34 2
# 34 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->pt_regs_sp $160 offsetof(struct pt_regs, sp)"
# 0 "" 2
	.loc 1 35 2
# 35 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->pt_regs_bp $32 offsetof(struct pt_regs, bp)"
# 0 "" 2
	.loc 1 36 2
# 36 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->pt_regs_si $104 offsetof(struct pt_regs, si)"
# 0 "" 2
	.loc 1 37 2
# 37 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->pt_regs_di $112 offsetof(struct pt_regs, di)"
# 0 "" 2
	.loc 1 38 2
# 38 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->pt_regs_r8 $72 offsetof(struct pt_regs, r8)"
# 0 "" 2
	.loc 1 39 2
# 39 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->pt_regs_r9 $64 offsetof(struct pt_regs, r9)"
# 0 "" 2
	.loc 1 40 2
# 40 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->pt_regs_r10 $56 offsetof(struct pt_regs, r10)"
# 0 "" 2
	.loc 1 41 2
# 41 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->pt_regs_r11 $48 offsetof(struct pt_regs, r11)"
# 0 "" 2
	.loc 1 42 2
# 42 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->pt_regs_r12 $24 offsetof(struct pt_regs, r12)"
# 0 "" 2
	.loc 1 43 2
# 43 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->pt_regs_r13 $16 offsetof(struct pt_regs, r13)"
# 0 "" 2
	.loc 1 44 2
# 44 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->pt_regs_r14 $8 offsetof(struct pt_regs, r14)"
# 0 "" 2
	.loc 1 45 2
# 45 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->pt_regs_r15 $0 offsetof(struct pt_regs, r15)"
# 0 "" 2
	.loc 1 46 2
# 46 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->pt_regs_flags $152 offsetof(struct pt_regs, flags)"
# 0 "" 2
	.loc 1 48 2
# 48 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->"
# 0 "" 2
	.loc 1 51 2
# 51 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->R15 $0 offsetof(struct pt_regs, r15)"
# 0 "" 2
	.loc 1 52 2
# 52 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->R14 $8 offsetof(struct pt_regs, r14)"
# 0 "" 2
	.loc 1 53 2
# 53 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->R13 $16 offsetof(struct pt_regs, r13)"
# 0 "" 2
	.loc 1 54 2
# 54 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->R12 $24 offsetof(struct pt_regs, r12)"
# 0 "" 2
	.loc 1 55 2
# 55 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->RBP $32 offsetof(struct pt_regs, bp)"
# 0 "" 2
	.loc 1 56 2
# 56 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->RBX $40 offsetof(struct pt_regs, bx)"
# 0 "" 2
	.loc 1 57 2
# 57 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->R11 $48 offsetof(struct pt_regs, r11)"
# 0 "" 2
	.loc 1 58 2
# 58 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->R10 $56 offsetof(struct pt_regs, r10)"
# 0 "" 2
	.loc 1 59 2
# 59 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->R9 $64 offsetof(struct pt_regs, r9)"
# 0 "" 2
	.loc 1 60 2
# 60 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->R8 $72 offsetof(struct pt_regs, r8)"
# 0 "" 2
	.loc 1 61 2
# 61 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->RAX $80 offsetof(struct pt_regs, ax)"
# 0 "" 2
	.loc 1 62 2
# 62 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->RCX $88 offsetof(struct pt_regs, cx)"
# 0 "" 2
	.loc 1 63 2
# 63 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->RDX $96 offsetof(struct pt_regs, dx)"
# 0 "" 2
	.loc 1 64 2
# 64 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->RSI $104 offsetof(struct pt_regs, si)"
# 0 "" 2
	.loc 1 65 2
# 65 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->RDI $112 offsetof(struct pt_regs, di)"
# 0 "" 2
	.loc 1 66 2
# 66 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->IRQ_NR $120 offsetof(struct pt_regs, irq_nr)"
# 0 "" 2
	.loc 1 67 2
# 67 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->ORIG_RAX $128 offsetof(struct pt_regs, orig_ax)"
# 0 "" 2
	.loc 1 68 2
# 68 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->RIP $136 offsetof(struct pt_regs, ip)"
# 0 "" 2
	.loc 1 69 2
# 69 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->CS $144 offsetof(struct pt_regs, cs)"
# 0 "" 2
	.loc 1 70 2
# 70 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->EFLAGS $152 offsetof(struct pt_regs, flags)"
# 0 "" 2
	.loc 1 71 2
# 71 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->RSP $160 offsetof(struct pt_regs, sp)"
# 0 "" 2
	.loc 1 72 2
# 72 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->SS $168 offsetof(struct pt_regs, ss)"
# 0 "" 2
	.loc 1 74 2
# 74 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->"
# 0 "" 2
	.loc 1 85 2
# 85 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets_64.c" 1
	
.ascii "->"
# 0 "" 2
	.loc 1 91 9
#NO_APP
	movl	$0, %eax
	.loc 1 92 1
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE130:
	.size	main, .-main
	.section	.text.common,"ax",@progbits
	.type	common, @function
common:
.LFB131:
	.file 2 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c"
	.loc 2 11 1
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
.L5:
	leaq	.L5(%rip), %rax
	movabsq	$_GLOBAL_OFFSET_TABLE_-.L5, %r11
	addq	%r11, %rax
	.loc 2 12 2
#APP
# 12 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->"
# 0 "" 2
	.loc 2 13 2
# 13 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->TASK_threadsp $352 offsetof(struct task_struct, thread.sp)"
# 0 "" 2
	.loc 2 18 2
# 18 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->"
# 0 "" 2
	.loc 2 78 2
# 78 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->"
# 0 "" 2
	.loc 2 79 2
# 79 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->PTREGS_SIZE $176 sizeof(struct pt_regs)"
# 0 "" 2
	.loc 2 90 2
# 90 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->TSS_sp0 $4 offsetof(struct tss_struct, x86_tss.sp0)"
# 0 "" 2
	.loc 2 91 2
# 91 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->TSS_sp1 $12 offsetof(struct tss_struct, x86_tss.sp1)"
# 0 "" 2
	.loc 2 92 2
# 92 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->TSS_sp2 $20 offsetof(struct tss_struct, x86_tss.sp2)"
# 0 "" 2
	.loc 2 93 2
# 93 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->X86_top_of_stack $24 offsetof(struct pcpu_hot, top_of_stack)"
# 0 "" 2
	.loc 2 94 2
# 94 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->X86_current_task $0 offsetof(struct pcpu_hot, current_task)"
# 0 "" 2
	.loc 2 105 1
#NO_APP
	nop
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE131:
	.size	common, .-common
	.text
.Letext0:
	.file 3 "/home/cheyh/projs/MyOS2/mykernel/include/uapi/asm-generic/int-ll64.h"
	.file 4 "/home/cheyh/projs/MyOS2/mykernel/include/linux/kernel/asm-generic/int-ll64.h"
	.file 5 "/home/cheyh/projs/MyOS2/mykernel/include/uapi/asm-generic/posix_types.h"
	.file 6 "/home/cheyh/projs/MyOS2/mykernel/include/linux/kernel/types.h"
	.file 7 "/home/cheyh/projs/MyOS2/mykernel/sched/sched_type_declaration.h"
	.file 8 "/home/cheyh/projs/MyOS2/mykernel/include/linux/sched/signal.h"
	.file 9 "/home/cheyh/projs/MyOS2/mykernel/include/uapi/linux/resource.h"
	.file 10 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/sched/sched_type_declaration_arch.h"
	.file 11 "/home/cheyh/projs/MyOS2/mykernel/sched/task/task_types.h"
	.file 12 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/include/asm/ptrace.h"
	.file 13 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/sched/context/thread_info_types_arch.h"
	.file 14 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/lock_IPC/atomic/../lock_ipc_type_declaration_arch.h"
	.file 15 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/lock_IPC/atomic/atomic_types_arch.h"
	.file 16 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_types_arch.h"
	.file 17 "/home/cheyh/projs/MyOS2/mykernel/lock_IPC/lock_ipc_type_declaration.h"
	.file 18 "/home/cheyh/projs/MyOS2/mykernel/lib/lib_type_declaration.h"
	.file 19 "/home/cheyh/projs/MyOS2/mykernel/lib/list/double_list_types.h"
	.file 20 "/home/cheyh/projs/MyOS2/mykernel/mm/mm_type_declaration.h"
	.file 21 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/mm/mm_type_declaration_arch.h"
	.file 22 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/mm/pgtable/pgtable_types_arch.h"
	.file 23 "/home/cheyh/projs/MyOS2/mykernel/mm/misc/mm_misc_types.h"
	.file 24 "/home/cheyh/projs/MyOS2/mykernel/include/linux/fs/vfs_s_defs.h"
	.file 25 "/home/cheyh/projs/MyOS2/mykernel/include/linux/fs/path.h"
	.file 26 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/include/asm/current.h"
	.file 27 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/processor/processor_types_arch.h"
	.file 28 "/home/cheyh/projs/MyOS2/mykernel/include/linux/kernel/swait.h"
	.file 29 "/home/cheyh/projs/MyOS2/mykernel/include/linux/kernel/completion.h"
	.file 30 "/home/cheyh/projs/MyOS2/mykernel/sched/misc/fs_struct_types.h"
	.file 31 "/home/cheyh/projs/MyOS2/mykernel/sched/scheduler/scheduler_types.h"
	.file 32 "/home/cheyh/projs/MyOS2/mykernel/sched/task/pid_types.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0xe5c
	.value	0x5
	.byte	0x1
	.byte	0x8
	.long	.Ldebug_abbrev0
	.uleb128 0x17
	.long	.LASF203
	.byte	0x1d
	.long	.LASF0
	.long	.LASF1
	.long	.LLRL0
	.quad	0
	.long	.Ldebug_line0
	.uleb128 0x6
	.byte	0x8
	.byte	0x7
	.long	.LASF2
	.uleb128 0x6
	.byte	0x1
	.byte	0x6
	.long	.LASF3
	.uleb128 0x6
	.byte	0x4
	.byte	0x7
	.long	.LASF4
	.uleb128 0x6
	.byte	0x1
	.byte	0x6
	.long	.LASF5
	.uleb128 0x2
	.long	.LASF8
	.byte	0x3
	.byte	0x15
	.byte	0x19
	.long	0x52
	.uleb128 0x6
	.byte	0x1
	.byte	0x8
	.long	.LASF6
	.uleb128 0x6
	.byte	0x2
	.byte	0x5
	.long	.LASF7
	.uleb128 0x2
	.long	.LASF9
	.byte	0x3
	.byte	0x18
	.byte	0x1a
	.long	0x6c
	.uleb128 0x6
	.byte	0x2
	.byte	0x7
	.long	.LASF10
	.uleb128 0x18
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.long	.LASF11
	.byte	0x3
	.byte	0x1b
	.byte	0x18
	.long	0x38
	.uleb128 0x2
	.long	.LASF12
	.byte	0x3
	.byte	0x1e
	.byte	0x1d
	.long	0x92
	.uleb128 0x6
	.byte	0x8
	.byte	0x5
	.long	.LASF13
	.uleb128 0x2
	.long	.LASF14
	.byte	0x3
	.byte	0x1f
	.byte	0x1f
	.long	0xa5
	.uleb128 0x6
	.byte	0x8
	.byte	0x7
	.long	.LASF15
	.uleb128 0xb
	.string	"u8"
	.byte	0xf
	.byte	0x10
	.long	0x46
	.uleb128 0xb
	.string	"u16"
	.byte	0x11
	.byte	0x11
	.long	0x60
	.uleb128 0xb
	.string	"u32"
	.byte	0x13
	.byte	0x11
	.long	0x7a
	.uleb128 0xb
	.string	"s64"
	.byte	0x14
	.byte	0x11
	.long	0x86
	.uleb128 0xb
	.string	"u64"
	.byte	0x15
	.byte	0x11
	.long	0x99
	.uleb128 0x6
	.byte	0x8
	.byte	0x5
	.long	.LASF16
	.uleb128 0x2
	.long	.LASF17
	.byte	0x5
	.byte	0x11
	.byte	0x19
	.long	0x2a
	.uleb128 0x2
	.long	.LASF18
	.byte	0x5
	.byte	0x1d
	.byte	0xf
	.long	0x73
	.uleb128 0x2
	.long	.LASF19
	.byte	0x5
	.byte	0x49
	.byte	0x1d
	.long	0xe9
	.uleb128 0x2
	.long	.LASF20
	.byte	0x6
	.byte	0x10
	.byte	0x1d
	.long	0x2a
	.uleb128 0x2
	.long	.LASF21
	.byte	0x6
	.byte	0x26
	.byte	0x1a
	.long	0xf5
	.uleb128 0x2
	.long	.LASF22
	.byte	0x6
	.byte	0x2e
	.byte	0x11
	.long	0x131
	.uleb128 0x6
	.byte	0x1
	.byte	0x2
	.long	.LASF23
	.uleb128 0x2
	.long	.LASF24
	.byte	0x6
	.byte	0x47
	.byte	0x1c
	.long	0x101
	.uleb128 0x2
	.long	.LASF25
	.byte	0x6
	.byte	0x68
	.byte	0x18
	.long	0x38
	.uleb128 0x2
	.long	.LASF26
	.byte	0x6
	.byte	0x69
	.byte	0x19
	.long	0x2a
	.uleb128 0x19
	.byte	0x8
	.uleb128 0x2
	.long	.LASF27
	.byte	0x7
	.byte	0x6
	.byte	0x1f
	.long	0x16a
	.uleb128 0xe
	.long	.LASF29
	.value	0x100
	.byte	0x8
	.byte	0x57
	.long	0x185
	.uleb128 0x1
	.long	.LASF31
	.byte	0x8
	.byte	0xce
	.byte	0xc
	.long	0xe17
	.byte	0
	.byte	0
	.uleb128 0x2
	.long	.LASF28
	.byte	0x7
	.byte	0x8
	.byte	0x18
	.long	0x191
	.uleb128 0x4
	.long	.LASF30
	.byte	0x10
	.byte	0x9
	.byte	0x2b
	.byte	0x11
	.long	0x1b9
	.uleb128 0x1
	.long	.LASF32
	.byte	0x9
	.byte	0x2c
	.byte	0x14
	.long	0xe9
	.byte	0
	.uleb128 0x1
	.long	.LASF33
	.byte	0x9
	.byte	0x2d
	.byte	0x14
	.long	0xe9
	.byte	0x8
	.byte	0
	.uleb128 0x2
	.long	.LASF34
	.byte	0x7
	.byte	0xa
	.byte	0x1e
	.long	0x1c5
	.uleb128 0xf
	.long	.LASF98
	.uleb128 0x2
	.long	.LASF35
	.byte	0xa
	.byte	0x5
	.byte	0x1d
	.long	0x1d6
	.uleb128 0xe
	.long	.LASF36
	.value	0x198
	.byte	0xb
	.byte	0x14
	.long	0x3f1
	.uleb128 0x1
	.long	.LASF37
	.byte	0xb
	.byte	0x19
	.byte	0x11
	.long	0x510
	.byte	0
	.uleb128 0x1
	.long	.LASF38
	.byte	0xb
	.byte	0x1a
	.byte	0x10
	.long	0x38
	.byte	0x18
	.uleb128 0x1
	.long	.LASF39
	.byte	0xb
	.byte	0x27
	.byte	0x9
	.long	0x15c
	.byte	0x20
	.uleb128 0x1
	.long	.LASF40
	.byte	0xb
	.byte	0x28
	.byte	0xc
	.long	0x55e
	.byte	0x28
	.uleb128 0x1
	.long	.LASF41
	.byte	0xb
	.byte	0x2a
	.byte	0x10
	.long	0x38
	.byte	0x2c
	.uleb128 0x1
	.long	.LASF42
	.byte	0xb
	.byte	0x2d
	.byte	0x7
	.long	0x73
	.byte	0x30
	.uleb128 0x1
	.long	.LASF43
	.byte	0xb
	.byte	0x3a
	.byte	0x7
	.long	0x73
	.byte	0x34
	.uleb128 0x1
	.long	.LASF44
	.byte	0xb
	.byte	0x3b
	.byte	0x7
	.long	0x73
	.byte	0x38
	.uleb128 0x1
	.long	.LASF45
	.byte	0xb
	.byte	0x3c
	.byte	0x7
	.long	0x73
	.byte	0x3c
	.uleb128 0x3
	.string	"se"
	.byte	0xb
	.byte	0x43
	.byte	0x12
	.long	0xb60
	.byte	0x40
	.uleb128 0x3
	.string	"rt"
	.byte	0xb
	.byte	0x44
	.byte	0x15
	.long	0xb87
	.byte	0x48
	.uleb128 0x3
	.string	"mm"
	.byte	0xb
	.byte	0x90
	.byte	0x9
	.long	0x95b
	.byte	0x60
	.uleb128 0x1
	.long	.LASF46
	.byte	0xb
	.byte	0x91
	.byte	0x9
	.long	0x95b
	.byte	0x68
	.uleb128 0x1
	.long	.LASF47
	.byte	0xb
	.byte	0x99
	.byte	0x7
	.long	0x73
	.byte	0x70
	.uleb128 0x1
	.long	.LASF48
	.byte	0xb
	.byte	0x9a
	.byte	0x7
	.long	0x73
	.byte	0x74
	.uleb128 0x1
	.long	.LASF49
	.byte	0xb
	.byte	0x9b
	.byte	0x7
	.long	0x73
	.byte	0x78
	.uleb128 0x12
	.long	.LASF50
	.byte	0xc1
	.long	0x38
	.value	0x3e0
	.uleb128 0x12
	.long	.LASF51
	.byte	0xc2
	.long	0x38
	.value	0x3e1
	.uleb128 0x3
	.string	"pid"
	.byte	0xb
	.byte	0xe0
	.byte	0x9
	.long	0x119
	.byte	0x80
	.uleb128 0x1
	.long	.LASF52
	.byte	0xb
	.byte	0xee
	.byte	0xb
	.long	0x778
	.byte	0x88
	.uleb128 0x1
	.long	.LASF53
	.byte	0xb
	.byte	0xf0
	.byte	0xb
	.long	0x778
	.byte	0x90
	.uleb128 0x1
	.long	.LASF54
	.byte	0xb
	.byte	0xf5
	.byte	0xe
	.long	0x682
	.byte	0x98
	.uleb128 0x1
	.long	.LASF55
	.byte	0xb
	.byte	0xf6
	.byte	0xa
	.long	0x621
	.byte	0xb0
	.uleb128 0x8
	.long	.LASF56
	.value	0x103
	.byte	0xa
	.long	0xdee
	.byte	0xc0
	.uleb128 0x8
	.long	.LASF57
	.value	0x105
	.byte	0xa
	.long	0x621
	.byte	0xc8
	.uleb128 0x8
	.long	.LASF58
	.value	0x109
	.byte	0x11
	.long	0xdf3
	.byte	0xd8
	.uleb128 0x8
	.long	.LASF59
	.value	0x112
	.byte	0x9
	.long	0x15c
	.byte	0xe0
	.uleb128 0x8
	.long	.LASF60
	.value	0x114
	.byte	0x7
	.long	0xd7
	.byte	0xe8
	.uleb128 0x8
	.long	.LASF61
	.value	0x115
	.byte	0x7
	.long	0xd7
	.byte	0xf0
	.uleb128 0x8
	.long	.LASF62
	.value	0x11a
	.byte	0x7
	.long	0xd7
	.byte	0xf8
	.uleb128 0x7
	.long	.LASF63
	.value	0x124
	.byte	0x11
	.long	0x2a
	.value	0x100
	.uleb128 0x7
	.long	.LASF64
	.value	0x125
	.byte	0x11
	.long	0x2a
	.value	0x108
	.uleb128 0x7
	.long	.LASF65
	.value	0x128
	.byte	0x7
	.long	0xd7
	.value	0x110
	.uleb128 0x7
	.long	.LASF66
	.value	0x12b
	.byte	0x7
	.long	0xd7
	.value	0x118
	.uleb128 0x7
	.long	.LASF67
	.value	0x14f
	.byte	0x8
	.long	0xdf8
	.value	0x120
	.uleb128 0x1a
	.string	"fs"
	.byte	0xb
	.value	0x15c
	.byte	0xd
	.long	0xe08
	.value	0x140
	.uleb128 0x7
	.long	.LASF68
	.value	0x15f
	.byte	0x13
	.long	0xe0d
	.value	0x148
	.uleb128 0x7
	.long	.LASF69
	.value	0x169
	.byte	0xd
	.long	0xe12
	.value	0x150
	.uleb128 0x7
	.long	.LASF70
	.value	0x185
	.byte	0xe
	.long	0x615
	.value	0x158
	.uleb128 0x7
	.long	.LASF71
	.value	0x2f7
	.byte	0xd
	.long	0xde2
	.value	0x160
	.byte	0
	.uleb128 0x4
	.long	.LASF72
	.byte	0xb0
	.byte	0xc
	.byte	0x9
	.byte	0x12
	.long	0x510
	.uleb128 0x3
	.string	"r15"
	.byte	0xc
	.byte	0xe
	.byte	0xa
	.long	0x10d
	.byte	0
	.uleb128 0x3
	.string	"r14"
	.byte	0xc
	.byte	0xf
	.byte	0xa
	.long	0x10d
	.byte	0x8
	.uleb128 0x3
	.string	"r13"
	.byte	0xc
	.byte	0x10
	.byte	0xa
	.long	0x10d
	.byte	0x10
	.uleb128 0x3
	.string	"r12"
	.byte	0xc
	.byte	0x11
	.byte	0xa
	.long	0x10d
	.byte	0x18
	.uleb128 0x3
	.string	"bp"
	.byte	0xc
	.byte	0x12
	.byte	0xa
	.long	0x10d
	.byte	0x20
	.uleb128 0x3
	.string	"bx"
	.byte	0xc
	.byte	0x13
	.byte	0xa
	.long	0x10d
	.byte	0x28
	.uleb128 0x3
	.string	"r11"
	.byte	0xc
	.byte	0x15
	.byte	0xa
	.long	0x10d
	.byte	0x30
	.uleb128 0x3
	.string	"r10"
	.byte	0xc
	.byte	0x16
	.byte	0xa
	.long	0x10d
	.byte	0x38
	.uleb128 0x3
	.string	"r9"
	.byte	0xc
	.byte	0x17
	.byte	0xa
	.long	0x10d
	.byte	0x40
	.uleb128 0x3
	.string	"r8"
	.byte	0xc
	.byte	0x18
	.byte	0xa
	.long	0x10d
	.byte	0x48
	.uleb128 0x3
	.string	"ax"
	.byte	0xc
	.byte	0x19
	.byte	0xa
	.long	0x10d
	.byte	0x50
	.uleb128 0x3
	.string	"cx"
	.byte	0xc
	.byte	0x1a
	.byte	0xa
	.long	0x10d
	.byte	0x58
	.uleb128 0x3
	.string	"dx"
	.byte	0xc
	.byte	0x1b
	.byte	0xa
	.long	0x10d
	.byte	0x60
	.uleb128 0x3
	.string	"si"
	.byte	0xc
	.byte	0x1c
	.byte	0xa
	.long	0x10d
	.byte	0x68
	.uleb128 0x3
	.string	"di"
	.byte	0xc
	.byte	0x1d
	.byte	0xa
	.long	0x10d
	.byte	0x70
	.uleb128 0x1
	.long	.LASF73
	.byte	0xc
	.byte	0x20
	.byte	0xa
	.long	0x10d
	.byte	0x78
	.uleb128 0x1
	.long	.LASF74
	.byte	0xc
	.byte	0x25
	.byte	0xa
	.long	0x10d
	.byte	0x80
	.uleb128 0x3
	.string	"ip"
	.byte	0xc
	.byte	0x27
	.byte	0xa
	.long	0x10d
	.byte	0x88
	.uleb128 0x3
	.string	"cs"
	.byte	0xc
	.byte	0x28
	.byte	0xa
	.long	0x10d
	.byte	0x90
	.uleb128 0x1
	.long	.LASF41
	.byte	0xc
	.byte	0x29
	.byte	0xa
	.long	0x10d
	.byte	0x98
	.uleb128 0x3
	.string	"sp"
	.byte	0xc
	.byte	0x2a
	.byte	0xa
	.long	0x10d
	.byte	0xa0
	.uleb128 0x3
	.string	"ss"
	.byte	0xc
	.byte	0x2b
	.byte	0xa
	.long	0x10d
	.byte	0xa8
	.byte	0
	.uleb128 0x2
	.long	.LASF75
	.byte	0xa
	.byte	0x13
	.byte	0x1d
	.long	0x51c
	.uleb128 0x4
	.long	.LASF37
	.byte	0x18
	.byte	0xd
	.byte	0x13
	.byte	0x11
	.long	0x55e
	.uleb128 0x1
	.long	.LASF41
	.byte	0xd
	.byte	0x14
	.byte	0x9
	.long	0x150
	.byte	0
	.uleb128 0x1
	.long	.LASF76
	.byte	0xd
	.byte	0x15
	.byte	0x9
	.long	0x150
	.byte	0x8
	.uleb128 0x1
	.long	.LASF77
	.byte	0xd
	.byte	0x16
	.byte	0x7
	.long	0xc1
	.byte	0x10
	.uleb128 0x3
	.string	"cpu"
	.byte	0xd
	.byte	0x17
	.byte	0x7
	.long	0xc1
	.byte	0x14
	.byte	0
	.uleb128 0x2
	.long	.LASF78
	.byte	0xe
	.byte	0x9
	.byte	0x18
	.long	0x56a
	.uleb128 0x4
	.long	.LASF79
	.byte	0x4
	.byte	0xf
	.byte	0x7
	.byte	0x11
	.long	0x585
	.uleb128 0x1
	.long	.LASF80
	.byte	0xf
	.byte	0x8
	.byte	0x7
	.long	0x73
	.byte	0
	.byte	0
	.uleb128 0x2
	.long	.LASF81
	.byte	0xe
	.byte	0xb
	.byte	0x1a
	.long	0x591
	.uleb128 0x4
	.long	.LASF82
	.byte	0x8
	.byte	0xf
	.byte	0xb
	.byte	0x11
	.long	0x5ac
	.uleb128 0x1
	.long	.LASF80
	.byte	0xf
	.byte	0xc
	.byte	0x7
	.long	0xcc
	.byte	0
	.byte	0
	.uleb128 0x2
	.long	.LASF83
	.byte	0xe
	.byte	0xf
	.byte	0x1b
	.long	0x5b8
	.uleb128 0x4
	.long	.LASF84
	.byte	0x8
	.byte	0x10
	.byte	0x8
	.byte	0x11
	.long	0x5cc
	.uleb128 0x10
	.long	0x5ef
	.byte	0
	.byte	0
	.uleb128 0x13
	.byte	0x8
	.byte	0x10
	.byte	0xc
	.long	0x5ef
	.uleb128 0x1
	.long	.LASF85
	.byte	0x10
	.byte	0xd
	.byte	0x9
	.long	0xc1
	.byte	0
	.uleb128 0x1
	.long	.LASF86
	.byte	0x10
	.byte	0xe
	.byte	0x9
	.long	0xc1
	.byte	0x4
	.byte	0
	.uleb128 0x14
	.byte	0x8
	.byte	0x10
	.byte	0x9
	.long	0x609
	.uleb128 0x11
	.string	"val"
	.byte	0x10
	.byte	0xa
	.byte	0xf
	.long	0x585
	.uleb128 0x15
	.long	0x5cc
	.byte	0
	.uleb128 0x2
	.long	.LASF87
	.byte	0x11
	.byte	0x9
	.byte	0x15
	.long	0x585
	.uleb128 0x2
	.long	.LASF88
	.byte	0x11
	.byte	0xd
	.byte	0x1a
	.long	0x5ac
	.uleb128 0x2
	.long	.LASF89
	.byte	0x12
	.byte	0xb
	.byte	0x1b
	.long	0x62d
	.uleb128 0x4
	.long	.LASF90
	.byte	0x10
	.byte	0x13
	.byte	0x8
	.byte	0x11
	.long	0x655
	.uleb128 0x1
	.long	.LASF91
	.byte	0x13
	.byte	0x9
	.byte	0xc
	.long	0x655
	.byte	0
	.uleb128 0x1
	.long	.LASF92
	.byte	0x13
	.byte	0xa
	.byte	0xc
	.long	0x655
	.byte	0x8
	.byte	0
	.uleb128 0x5
	.long	0x621
	.uleb128 0x4
	.long	.LASF93
	.byte	0x18
	.byte	0x13
	.byte	0xd
	.byte	0x11
	.long	0x682
	.uleb128 0x1
	.long	.LASF94
	.byte	0x13
	.byte	0xe
	.byte	0xa
	.long	0x621
	.byte	0
	.uleb128 0x1
	.long	.LASF95
	.byte	0x13
	.byte	0xf
	.byte	0x9
	.long	0x150
	.byte	0x10
	.byte	0
	.uleb128 0x2
	.long	.LASF96
	.byte	0x13
	.byte	0x10
	.byte	0x4
	.long	0x65a
	.uleb128 0x2
	.long	.LASF97
	.byte	0x14
	.byte	0x7
	.byte	0x16
	.long	0x69a
	.uleb128 0xf
	.long	.LASF99
	.uleb128 0x2
	.long	.LASF100
	.byte	0x15
	.byte	0x19
	.byte	0x10
	.long	0x150
	.uleb128 0x2
	.long	.LASF101
	.byte	0x15
	.byte	0x1c
	.byte	0x1a
	.long	0x6b7
	.uleb128 0x4
	.long	.LASF102
	.byte	0x8
	.byte	0x16
	.byte	0x8
	.byte	0x11
	.long	0x719
	.uleb128 0x9
	.string	"P"
	.byte	0xa
	.long	0x2a
	.byte	0
	.uleb128 0x9
	.string	"RW"
	.byte	0xb
	.long	0x2a
	.byte	0x1
	.uleb128 0x9
	.string	"US"
	.byte	0xc
	.long	0x2a
	.byte	0x2
	.uleb128 0x9
	.string	"PWT"
	.byte	0xd
	.long	0x2a
	.byte	0x3
	.uleb128 0x9
	.string	"PCD"
	.byte	0xe
	.long	0x2a
	.byte	0x4
	.uleb128 0x9
	.string	"A"
	.byte	0xf
	.long	0x2a
	.byte	0x5
	.uleb128 0x1b
	.long	.LASF103
	.byte	0x16
	.byte	0x11
	.byte	0x4
	.long	0x2a
	.byte	0x24
	.byte	0xc
	.uleb128 0x9
	.string	"XD"
	.byte	0x13
	.long	0x2a
	.byte	0x3f
	.byte	0
	.uleb128 0x2
	.long	.LASF104
	.byte	0x15
	.byte	0x24
	.byte	0x1a
	.long	0x725
	.uleb128 0x1c
	.long	.LASF204
	.byte	0x8
	.byte	0x16
	.byte	0x49
	.byte	0x10
	.long	0x74b
	.uleb128 0x11
	.string	"val"
	.byte	0x16
	.byte	0x4a
	.byte	0xc
	.long	0x69f
	.uleb128 0x1d
	.long	.LASF105
	.byte	0x16
	.byte	0x4b
	.byte	0xe
	.long	0x6ab
	.byte	0
	.uleb128 0x2
	.long	.LASF106
	.byte	0x14
	.byte	0x32
	.byte	0x1b
	.long	0x757
	.uleb128 0xe
	.long	.LASF107
	.value	0x110
	.byte	0x17
	.byte	0x7
	.long	0x778
	.uleb128 0x1
	.long	.LASF108
	.byte	0x17
	.byte	0x8
	.byte	0xa
	.long	0x138
	.byte	0
	.uleb128 0x10
	.long	0x77d
	.byte	0x8
	.byte	0
	.uleb128 0x5
	.long	0x1ca
	.uleb128 0x1e
	.value	0x108
	.byte	0x17
	.byte	0x9
	.byte	0x3
	.long	0x929
	.uleb128 0x1
	.long	.LASF109
	.byte	0x17
	.byte	0xa
	.byte	0xf
	.long	0x682
	.byte	0
	.uleb128 0x1
	.long	.LASF110
	.byte	0x17
	.byte	0xc
	.byte	0xc
	.long	0x951
	.byte	0x18
	.uleb128 0x1
	.long	.LASF111
	.byte	0x17
	.byte	0xe
	.byte	0xa
	.long	0x150
	.byte	0x20
	.uleb128 0x1
	.long	.LASF112
	.byte	0x17
	.byte	0xf
	.byte	0xa
	.long	0x150
	.byte	0x28
	.uleb128 0x1
	.long	.LASF113
	.byte	0x17
	.byte	0x15
	.byte	0xa
	.long	0x150
	.byte	0x30
	.uleb128 0x1
	.long	.LASF114
	.byte	0x17
	.byte	0x16
	.byte	0xa
	.long	0x150
	.byte	0x38
	.uleb128 0x3
	.string	"pgd"
	.byte	0x17
	.byte	0x17
	.byte	0xb
	.long	0x956
	.byte	0x40
	.uleb128 0x1
	.long	.LASF115
	.byte	0x17
	.byte	0x2c
	.byte	0xd
	.long	0x55e
	.byte	0x48
	.uleb128 0x1
	.long	.LASF116
	.byte	0x17
	.byte	0x35
	.byte	0xd
	.long	0x55e
	.byte	0x4c
	.uleb128 0x1
	.long	.LASF117
	.byte	0x17
	.byte	0x36
	.byte	0x12
	.long	0x609
	.byte	0x50
	.uleb128 0x1
	.long	.LASF118
	.byte	0x17
	.byte	0x37
	.byte	0x8
	.long	0x73
	.byte	0x58
	.uleb128 0x1
	.long	.LASF119
	.byte	0x17
	.byte	0x38
	.byte	0xf
	.long	0x615
	.byte	0x60
	.uleb128 0x1
	.long	.LASF120
	.byte	0x17
	.byte	0x51
	.byte	0xa
	.long	0x150
	.byte	0x68
	.uleb128 0x1
	.long	.LASF121
	.byte	0x17
	.byte	0x52
	.byte	0xa
	.long	0x150
	.byte	0x70
	.uleb128 0x1
	.long	.LASF122
	.byte	0x17
	.byte	0x53
	.byte	0xf
	.long	0x585
	.byte	0x78
	.uleb128 0x1
	.long	.LASF123
	.byte	0x17
	.byte	0x54
	.byte	0xa
	.long	0x150
	.byte	0x80
	.uleb128 0x1
	.long	.LASF124
	.byte	0x17
	.byte	0x55
	.byte	0xa
	.long	0x150
	.byte	0x88
	.uleb128 0x1
	.long	.LASF125
	.byte	0x17
	.byte	0x56
	.byte	0xa
	.long	0x150
	.byte	0x90
	.uleb128 0x1
	.long	.LASF126
	.byte	0x17
	.byte	0x57
	.byte	0xa
	.long	0x150
	.byte	0x98
	.uleb128 0x1
	.long	.LASF127
	.byte	0x17
	.byte	0x62
	.byte	0xa
	.long	0x150
	.byte	0xa0
	.uleb128 0x1
	.long	.LASF128
	.byte	0x17
	.byte	0x62
	.byte	0x16
	.long	0x150
	.byte	0xa8
	.uleb128 0x1
	.long	.LASF129
	.byte	0x17
	.byte	0x62
	.byte	0x20
	.long	0x150
	.byte	0xb0
	.uleb128 0x1
	.long	.LASF130
	.byte	0x17
	.byte	0x62
	.byte	0x2c
	.long	0x150
	.byte	0xb8
	.uleb128 0x1
	.long	.LASF131
	.byte	0x17
	.byte	0x63
	.byte	0xa
	.long	0x150
	.byte	0xc0
	.uleb128 0x3
	.string	"brk"
	.byte	0x17
	.byte	0x63
	.byte	0x15
	.long	0x150
	.byte	0xc8
	.uleb128 0x1
	.long	.LASF132
	.byte	0x17
	.byte	0x63
	.byte	0x1a
	.long	0x150
	.byte	0xd0
	.uleb128 0x1
	.long	.LASF133
	.byte	0x17
	.byte	0x64
	.byte	0xa
	.long	0x150
	.byte	0xd8
	.uleb128 0x1
	.long	.LASF134
	.byte	0x17
	.byte	0x64
	.byte	0x15
	.long	0x150
	.byte	0xe0
	.uleb128 0x1
	.long	.LASF135
	.byte	0x17
	.byte	0x64
	.byte	0x1e
	.long	0x150
	.byte	0xe8
	.uleb128 0x1
	.long	.LASF136
	.byte	0x17
	.byte	0x64
	.byte	0x29
	.long	0x150
	.byte	0xf0
	.uleb128 0x1
	.long	.LASF41
	.byte	0x17
	.byte	0x73
	.byte	0xa
	.long	0x150
	.byte	0xf8
	.uleb128 0x16
	.long	.LASF137
	.byte	0x17
	.byte	0x89
	.byte	0xc
	.long	0x94c
	.value	0x100
	.byte	0
	.uleb128 0x1f
	.long	0x150
	.long	0x94c
	.uleb128 0xc
	.long	0x94c
	.uleb128 0xc
	.long	0x150
	.uleb128 0xc
	.long	0x150
	.uleb128 0xc
	.long	0x150
	.uleb128 0xc
	.long	0x150
	.byte	0
	.uleb128 0x5
	.long	0x68e
	.uleb128 0x5
	.long	0x929
	.uleb128 0x5
	.long	0x719
	.uleb128 0x5
	.long	0x74b
	.uleb128 0x2
	.long	.LASF138
	.byte	0x18
	.byte	0x4
	.byte	0x18
	.long	0x96c
	.uleb128 0xf
	.long	.LASF139
	.uleb128 0x2
	.long	.LASF140
	.byte	0x18
	.byte	0x11
	.byte	0x1a
	.long	0x97d
	.uleb128 0xf
	.long	.LASF141
	.uleb128 0x2
	.long	.LASF142
	.byte	0x18
	.byte	0x21
	.byte	0x16
	.long	0x98e
	.uleb128 0x4
	.long	.LASF143
	.byte	0x10
	.byte	0x19
	.byte	0x7
	.byte	0x11
	.long	0x9b6
	.uleb128 0x3
	.string	"mnt"
	.byte	0x19
	.byte	0x8
	.byte	0xf
	.long	0xc2f
	.byte	0
	.uleb128 0x1
	.long	.LASF139
	.byte	0x19
	.byte	0x9
	.byte	0xd
	.long	0xc34
	.byte	0x8
	.byte	0
	.uleb128 0x4
	.long	.LASF144
	.byte	0x40
	.byte	0x1a
	.byte	0x10
	.byte	0x11
	.long	0x9ca
	.uleb128 0x10
	.long	0xa66
	.byte	0
	.byte	0
	.uleb128 0x20
	.long	.LASF205
	.value	0x5000
	.value	0x1000
	.byte	0x1b
	.byte	0x9d
	.byte	0x9
	.long	0x9f5
	.uleb128 0x1
	.long	.LASF145
	.byte	0x1b
	.byte	0xa3
	.byte	0x10
	.long	0xce9
	.byte	0
	.uleb128 0x1
	.long	.LASF146
	.byte	0x1b
	.byte	0xa5
	.byte	0x13
	.long	0xd49
	.byte	0x68
	.byte	0
	.uleb128 0x13
	.byte	0x30
	.byte	0x1a
	.byte	0x12
	.long	0xa66
	.uleb128 0x1
	.long	.LASF147
	.byte	0x1a
	.byte	0x13
	.byte	0xd
	.long	0x778
	.byte	0
	.uleb128 0x1
	.long	.LASF148
	.byte	0x1a
	.byte	0x14
	.byte	0x9
	.long	0x73
	.byte	0x8
	.uleb128 0x1
	.long	.LASF149
	.byte	0x1a
	.byte	0x15
	.byte	0x9
	.long	0x73
	.byte	0xc
	.uleb128 0x1
	.long	.LASF150
	.byte	0x1a
	.byte	0x16
	.byte	0x9
	.long	0xd7
	.byte	0x10
	.uleb128 0x1
	.long	.LASF151
	.byte	0x1a
	.byte	0x17
	.byte	0x13
	.long	0x2a
	.byte	0x18
	.uleb128 0x1
	.long	.LASF152
	.byte	0x1a
	.byte	0x18
	.byte	0xb
	.long	0x15c
	.byte	0x20
	.uleb128 0x1
	.long	.LASF153
	.byte	0x1a
	.byte	0x19
	.byte	0x9
	.long	0xb6
	.byte	0x28
	.uleb128 0x1
	.long	.LASF154
	.byte	0x1a
	.byte	0x1a
	.byte	0xa
	.long	0x125
	.byte	0x2a
	.byte	0
	.uleb128 0x14
	.byte	0x40
	.byte	0x1a
	.byte	0x11
	.long	0xa80
	.uleb128 0x15
	.long	0x9f5
	.uleb128 0x11
	.string	"pad"
	.byte	0x1a
	.byte	0x1c
	.byte	0x7
	.long	0xa80
	.byte	0
	.uleb128 0xa
	.long	0xac
	.long	0xa90
	.uleb128 0xd
	.long	0x2a
	.byte	0x3f
	.byte	0
	.uleb128 0x4
	.long	.LASF155
	.byte	0x20
	.byte	0x1c
	.byte	0x29
	.byte	0x11
	.long	0xab8
	.uleb128 0x1
	.long	.LASF156
	.byte	0x1c
	.byte	0x2a
	.byte	0x13
	.long	0x5ac
	.byte	0
	.uleb128 0x1
	.long	.LASF157
	.byte	0x1c
	.byte	0x2b
	.byte	0xe
	.long	0x682
	.byte	0x8
	.byte	0
	.uleb128 0x2
	.long	.LASF158
	.byte	0x1c
	.byte	0x2c
	.byte	0x4
	.long	0xa90
	.uleb128 0x4
	.long	.LASF159
	.byte	0x28
	.byte	0x1d
	.byte	0x1a
	.byte	0x11
	.long	0xaec
	.uleb128 0x1
	.long	.LASF160
	.byte	0x1d
	.byte	0x1b
	.byte	0x10
	.long	0x38
	.byte	0
	.uleb128 0x1
	.long	.LASF161
	.byte	0x1d
	.byte	0x1c
	.byte	0x11
	.long	0xab8
	.byte	0x8
	.byte	0
	.uleb128 0x2
	.long	.LASF162
	.byte	0x1d
	.byte	0x1d
	.byte	0x4
	.long	0xac4
	.uleb128 0x2
	.long	.LASF163
	.byte	0x7
	.byte	0x1a
	.byte	0x1b
	.long	0xb04
	.uleb128 0x4
	.long	.LASF164
	.byte	0x38
	.byte	0x1e
	.byte	0x9
	.byte	0x11
	.long	0xb60
	.uleb128 0x1
	.long	.LASF165
	.byte	0x1e
	.byte	0xa
	.byte	0x7
	.long	0x73
	.byte	0
	.uleb128 0x1
	.long	.LASF156
	.byte	0x1e
	.byte	0xb
	.byte	0xe
	.long	0x615
	.byte	0x8
	.uleb128 0x1
	.long	.LASF166
	.byte	0x1e
	.byte	0xd
	.byte	0x7
	.long	0x73
	.byte	0x10
	.uleb128 0x1
	.long	.LASF167
	.byte	0x1e
	.byte	0xe
	.byte	0x7
	.long	0x73
	.byte	0x14
	.uleb128 0x1
	.long	.LASF168
	.byte	0x1e
	.byte	0xf
	.byte	0xa
	.long	0x982
	.byte	0x18
	.uleb128 0x3
	.string	"pwd"
	.byte	0x1e
	.byte	0xf
	.byte	0x10
	.long	0x982
	.byte	0x28
	.byte	0
	.uleb128 0x2
	.long	.LASF169
	.byte	0x7
	.byte	0x2e
	.byte	0x1e
	.long	0xb6c
	.uleb128 0x4
	.long	.LASF170
	.byte	0x8
	.byte	0x1f
	.byte	0x8
	.byte	0x11
	.long	0xb87
	.uleb128 0x1
	.long	.LASF171
	.byte	0x1f
	.byte	0x11
	.byte	0x7
	.long	0xd7
	.byte	0
	.byte	0
	.uleb128 0x2
	.long	.LASF172
	.byte	0x7
	.byte	0x30
	.byte	0x21
	.long	0xb93
	.uleb128 0x4
	.long	.LASF173
	.byte	0x18
	.byte	0x1f
	.byte	0x2a
	.byte	0x11
	.long	0xbbb
	.uleb128 0x1
	.long	.LASF174
	.byte	0x1f
	.byte	0x2b
	.byte	0xa
	.long	0x621
	.byte	0
	.uleb128 0x1
	.long	.LASF175
	.byte	0x1f
	.byte	0x2e
	.byte	0x8
	.long	0x144
	.byte	0x10
	.byte	0
	.uleb128 0x2
	.long	.LASF176
	.byte	0x7
	.byte	0x36
	.byte	0x16
	.long	0xbc7
	.uleb128 0x4
	.long	.LASF177
	.byte	0x4
	.byte	0x20
	.byte	0x33
	.byte	0x11
	.long	0xbe1
	.uleb128 0x3
	.string	"nr"
	.byte	0x20
	.byte	0x34
	.byte	0x7
	.long	0x73
	.byte	0
	.byte	0
	.uleb128 0x2
	.long	.LASF178
	.byte	0x7
	.byte	0x38
	.byte	0x15
	.long	0xbed
	.uleb128 0x21
	.string	"pid"
	.byte	0x18
	.byte	0x20
	.byte	0x38
	.byte	0x11
	.long	0xc2f
	.uleb128 0x1
	.long	.LASF95
	.byte	0x20
	.byte	0x39
	.byte	0xc
	.long	0x55e
	.byte	0
	.uleb128 0x1
	.long	.LASF179
	.byte	0x20
	.byte	0x3a
	.byte	0x10
	.long	0x38
	.byte	0x4
	.uleb128 0x1
	.long	.LASF156
	.byte	0x20
	.byte	0x3b
	.byte	0xe
	.long	0x615
	.byte	0x8
	.uleb128 0x1
	.long	.LASF180
	.byte	0x20
	.byte	0x42
	.byte	0xa
	.long	0xc39
	.byte	0x10
	.byte	0
	.uleb128 0x5
	.long	0x971
	.uleb128 0x5
	.long	0x960
	.uleb128 0xa
	.long	0xbbb
	.long	0xc49
	.uleb128 0xd
	.long	0x2a
	.byte	0
	.byte	0
	.uleb128 0x4
	.long	.LASF181
	.byte	0x68
	.byte	0x1b
	.byte	0x68
	.byte	0x11
	.long	0xcd9
	.uleb128 0x1
	.long	.LASF182
	.byte	0x1b
	.byte	0x69
	.byte	0x7
	.long	0xc1
	.byte	0
	.uleb128 0x3
	.string	"sp0"
	.byte	0x1b
	.byte	0x6a
	.byte	0x7
	.long	0xd7
	.byte	0x4
	.uleb128 0x3
	.string	"sp1"
	.byte	0x1b
	.byte	0x6b
	.byte	0x7
	.long	0xd7
	.byte	0xc
	.uleb128 0x3
	.string	"sp2"
	.byte	0x1b
	.byte	0x72
	.byte	0x7
	.long	0xd7
	.byte	0x14
	.uleb128 0x1
	.long	.LASF183
	.byte	0x1b
	.byte	0x74
	.byte	0x7
	.long	0xd7
	.byte	0x1c
	.uleb128 0x3
	.string	"ist"
	.byte	0x1b
	.byte	0x75
	.byte	0x7
	.long	0xcd9
	.byte	0x24
	.uleb128 0x1
	.long	.LASF184
	.byte	0x1b
	.byte	0x76
	.byte	0x7
	.long	0xc1
	.byte	0x5c
	.uleb128 0x1
	.long	.LASF185
	.byte	0x1b
	.byte	0x77
	.byte	0x7
	.long	0xc1
	.byte	0x60
	.uleb128 0x1
	.long	.LASF186
	.byte	0x1b
	.byte	0x78
	.byte	0x7
	.long	0xb6
	.byte	0x64
	.uleb128 0x1
	.long	.LASF187
	.byte	0x1b
	.byte	0x79
	.byte	0x7
	.long	0xb6
	.byte	0x66
	.byte	0
	.uleb128 0xa
	.long	0xd7
	.long	0xce9
	.uleb128 0xd
	.long	0x2a
	.byte	0x6
	.byte	0
	.uleb128 0x2
	.long	.LASF188
	.byte	0x1b
	.byte	0x7b
	.byte	0x1c
	.long	0xc49
	.uleb128 0xe
	.long	.LASF189
	.value	0x4020
	.byte	0x1b
	.byte	0x80
	.long	0xd38
	.uleb128 0x1
	.long	.LASF190
	.byte	0x1b
	.byte	0x83
	.byte	0x7
	.long	0xd7
	.byte	0
	.uleb128 0x1
	.long	.LASF191
	.byte	0x1b
	.byte	0x8c
	.byte	0x10
	.long	0x38
	.byte	0x8
	.uleb128 0x1
	.long	.LASF192
	.byte	0x1b
	.byte	0x94
	.byte	0x11
	.long	0xd38
	.byte	0x10
	.uleb128 0x16
	.long	.LASF193
	.byte	0x1b
	.byte	0x9a
	.byte	0x11
	.long	0xd38
	.value	0x2018
	.byte	0
	.uleb128 0xa
	.long	0x2a
	.long	0xd49
	.uleb128 0x22
	.long	0x2a
	.value	0x400
	.byte	0
	.uleb128 0x2
	.long	.LASF194
	.byte	0x1b
	.byte	0x9b
	.byte	0x4
	.long	0xcf5
	.uleb128 0x4
	.long	.LASF195
	.byte	0x38
	.byte	0x1b
	.byte	0xb5
	.byte	0x11
	.long	0xde2
	.uleb128 0x3
	.string	"sp"
	.byte	0x1b
	.byte	0xb8
	.byte	0x9
	.long	0x10d
	.byte	0
	.uleb128 0x3
	.string	"es"
	.byte	0x1b
	.byte	0xb9
	.byte	0x12
	.long	0x6c
	.byte	0x8
	.uleb128 0x3
	.string	"ds"
	.byte	0x1b
	.byte	0xba
	.byte	0x12
	.long	0x6c
	.byte	0xa
	.uleb128 0x1
	.long	.LASF196
	.byte	0x1b
	.byte	0xbb
	.byte	0x12
	.long	0x6c
	.byte	0xc
	.uleb128 0x1
	.long	.LASF197
	.byte	0x1b
	.byte	0xbc
	.byte	0x12
	.long	0x6c
	.byte	0xe
	.uleb128 0x1
	.long	.LASF198
	.byte	0x1b
	.byte	0xbe
	.byte	0x9
	.long	0x10d
	.byte	0x10
	.uleb128 0x1
	.long	.LASF199
	.byte	0x1b
	.byte	0xbf
	.byte	0x9
	.long	0x10d
	.byte	0x18
	.uleb128 0x3
	.string	"cr2"
	.byte	0x1b
	.byte	0xc8
	.byte	0x9
	.long	0x10d
	.byte	0x20
	.uleb128 0x1
	.long	.LASF200
	.byte	0x1b
	.byte	0xc9
	.byte	0x11
	.long	0x2a
	.byte	0x28
	.uleb128 0x1
	.long	.LASF201
	.byte	0x1b
	.byte	0xca
	.byte	0x11
	.long	0x2a
	.byte	0x30
	.byte	0
	.uleb128 0x2
	.long	.LASF202
	.byte	0x1b
	.byte	0xe8
	.byte	0x4
	.long	0xd55
	.uleb128 0x5
	.long	0xbe1
	.uleb128 0x5
	.long	0xaec
	.uleb128 0xa
	.long	0x31
	.long	0xe08
	.uleb128 0xd
	.long	0x2a
	.byte	0x1f
	.byte	0
	.uleb128 0x5
	.long	0xaf8
	.uleb128 0x5
	.long	0x1b9
	.uleb128 0x5
	.long	0x15e
	.uleb128 0xa
	.long	0x185
	.long	0xe27
	.uleb128 0xd
	.long	0x2a
	.byte	0xf
	.byte	0
	.uleb128 0x23
	.long	.LASF206
	.byte	0x2
	.byte	0xa
	.byte	0x27
	.quad	.LFB131
	.quad	.LFE131-.LFB131
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x24
	.long	.LASF207
	.byte	0x1
	.byte	0xe
	.byte	0x5
	.long	0x73
	.quad	.LFB130
	.quad	.LFE130-.LFB130
	.uleb128 0x1
	.byte	0x9c
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 8
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 11
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 11
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 22
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0xd
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x6b
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 17
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0xd
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 11
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 12
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0xd
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x6b
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x17
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 3
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0xd
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x1f
	.uleb128 0x1b
	.uleb128 0x1f
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0xd
	.uleb128 0xb
	.uleb128 0x6b
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x17
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x20
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x88
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x21
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x22
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x23
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x24
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x3c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.LFB130
	.quad	.LFE130-.LFB130
	.quad	.LFB131
	.quad	.LFE131-.LFB131
	.quad	0
	.quad	0
	.section	.debug_rnglists,"",@progbits
.Ldebug_ranges0:
	.long	.Ldebug_ranges3-.Ldebug_ranges2
.Ldebug_ranges2:
	.value	0x5
	.byte	0x8
	.byte	0
	.long	0
.LLRL0:
	.byte	0x7
	.quad	.LFB130
	.uleb128 .LFE130-.LFB130
	.byte	0x7
	.quad	.LFB131
	.uleb128 .LFE131-.LFB131
	.byte	0
.Ldebug_ranges3:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF179:
	.string	"level"
.LASF24:
	.string	"size_t"
.LASF200:
	.string	"trap_nr"
.LASF116:
	.string	"mm_refcount"
.LASF86:
	.string	"tail"
.LASF154:
	.string	"hardirq_stack_inuse"
.LASF168:
	.string	"root"
.LASF119:
	.string	"page_table_lock"
.LASF79:
	.string	"atomic"
.LASF143:
	.string	"path"
.LASF90:
	.string	"list_head"
.LASF9:
	.string	"__u16"
.LASF138:
	.string	"dentry_s"
.LASF52:
	.string	"real_parent"
.LASF51:
	.string	"in_iowait"
.LASF8:
	.string	"__u8"
.LASF84:
	.string	"tspinlock"
.LASF67:
	.string	"comm"
.LASF113:
	.string	"task_size"
.LASF196:
	.string	"fsindex"
.LASF189:
	.string	"x86_io_bitmap"
.LASF118:
	.string	"map_count"
.LASF28:
	.string	"rlimit_s"
.LASF147:
	.string	"current_task"
.LASF165:
	.string	"users"
.LASF158:
	.string	"swqueue_hdr_s"
.LASF66:
	.string	"start_boottime"
.LASF54:
	.string	"children"
.LASF11:
	.string	"__u32"
.LASF18:
	.string	"__kernel_pid_t"
.LASF197:
	.string	"gsindex"
.LASF43:
	.string	"recent_used_cpu"
.LASF60:
	.string	"utime"
.LASF16:
	.string	"long int"
.LASF128:
	.string	"end_code"
.LASF145:
	.string	"x86_tss"
.LASF137:
	.string	"exe_file"
.LASF110:
	.string	"get_unmapped_area"
.LASF56:
	.string	"thread_pid"
.LASF194:
	.string	"x86_io_bitmap_s"
.LASF207:
	.string	"main"
.LASF20:
	.string	"reg_t"
.LASF206:
	.string	"common"
.LASF85:
	.string	"head"
.LASF141:
	.string	"vfsmount"
.LASF166:
	.string	"umask"
.LASF5:
	.string	"signed char"
.LASF163:
	.string	"taskfs_s"
.LASF27:
	.string	"signal_s"
.LASF14:
	.string	"__u64"
.LASF73:
	.string	"irq_nr"
.LASF203:
	.ascii	"GNU C17 "
	.string	"12.3.0 -m64 -mcmodel=large -mabi=sysv -mno-red-zone -mtune=generic -march=x86-64 -ggdb -g -fPIE -ffreestanding -fno-stack-protector -fno-unwind-tables -fdata-sections -ffunction-sections -fasynchronous-unwind-tables -fstack-clash-protection -fcf-protection"
.LASF6:
	.string	"unsigned char"
.LASF31:
	.string	"rlim"
.LASF164:
	.string	"fs_struct"
.LASF162:
	.string	"completion_s"
.LASF171:
	.string	"vruntime"
.LASF23:
	.string	"_Bool"
.LASF127:
	.string	"start_code"
.LASF106:
	.string	"mm_s"
.LASF152:
	.string	"hardirq_stack_ptr"
.LASF133:
	.string	"arg_start"
.LASF3:
	.string	"char"
.LASF114:
	.string	"highest_vm_end"
.LASF44:
	.string	"wake_cpu"
.LASF101:
	.string	"arch_pgd_T"
.LASF204:
	.string	"pgd_union"
.LASF30:
	.string	"rlimit"
.LASF50:
	.string	"in_execve"
.LASF156:
	.string	"lock"
.LASF55:
	.string	"sibling"
.LASF69:
	.string	"signal"
.LASF146:
	.string	"io_bitmap"
.LASF131:
	.string	"start_brk"
.LASF34:
	.string	"files_struct_s"
.LASF57:
	.string	"pid_links"
.LASF41:
	.string	"flags"
.LASF25:
	.string	"uint"
.LASF17:
	.string	"__kernel_ulong_t"
.LASF32:
	.string	"rlim_cur"
.LASF98:
	.string	"files_struct"
.LASF105:
	.string	"defs"
.LASF99:
	.string	"file"
.LASF87:
	.string	"atomic_long_t"
.LASF26:
	.string	"ulong"
.LASF130:
	.string	"end_data"
.LASF124:
	.string	"exec_vm"
.LASF139:
	.string	"dentry"
.LASF72:
	.string	"pt_regs"
.LASF117:
	.string	"pgtables_bytes"
.LASF150:
	.string	"call_depth"
.LASF174:
	.string	"run_list"
.LASF2:
	.string	"long unsigned int"
.LASF77:
	.string	"status"
.LASF134:
	.string	"arg_end"
.LASF48:
	.string	"exit_code"
.LASF61:
	.string	"stime"
.LASF129:
	.string	"start_data"
.LASF75:
	.string	"thread_info_s"
.LASF120:
	.string	"total_vm"
.LASF170:
	.string	"sched_entity"
.LASF193:
	.string	"mapall"
.LASF29:
	.string	"signal_struct"
.LASF132:
	.string	"start_stack"
.LASF36:
	.string	"task_struct"
.LASF58:
	.string	"vfork_done"
.LASF13:
	.string	"long long int"
.LASF199:
	.string	"gsbase"
.LASF80:
	.string	"counter"
.LASF19:
	.string	"__kernel_size_t"
.LASF37:
	.string	"thread_info"
.LASF136:
	.string	"env_end"
.LASF39:
	.string	"stack"
.LASF109:
	.string	"mm_mt"
.LASF53:
	.string	"parent"
.LASF191:
	.string	"prev_max"
.LASF76:
	.string	"syscall_work"
.LASF35:
	.string	"task_s"
.LASF161:
	.string	"wait"
.LASF172:
	.string	"sched_rt_entity_s"
.LASF95:
	.string	"count"
.LASF122:
	.string	"pinned_vm"
.LASF195:
	.string	"thread_struct"
.LASF74:
	.string	"orig_ax"
.LASF82:
	.string	"atomic64"
.LASF111:
	.string	"mmap_base"
.LASF4:
	.string	"unsigned int"
.LASF151:
	.string	"top_of_stack"
.LASF88:
	.string	"spinlock_t"
.LASF192:
	.string	"bitmap"
.LASF104:
	.string	"pgd_t"
.LASF205:
	.string	"tss_struct"
.LASF178:
	.string	"pid_s"
.LASF21:
	.string	"pid_t"
.LASF187:
	.string	"io_bitmap_base"
.LASF89:
	.string	"List_s"
.LASF160:
	.string	"done"
.LASF149:
	.string	"cpu_number"
.LASF180:
	.string	"numbers"
.LASF125:
	.string	"stack_vm"
.LASF190:
	.string	"prev_sequence"
.LASF22:
	.string	"bool"
.LASF97:
	.string	"file_s"
.LASF202:
	.string	"thread_s"
.LASF142:
	.string	"path_s"
.LASF135:
	.string	"env_start"
.LASF33:
	.string	"rlim_max"
.LASF15:
	.string	"long long unsigned int"
.LASF96:
	.string	"List_hdr_s"
.LASF155:
	.string	"swait_queue_head"
.LASF94:
	.string	"anchor"
.LASF123:
	.string	"data_vm"
.LASF140:
	.string	"vfsmount_s"
.LASF201:
	.string	"error_code"
.LASF102:
	.string	"arch_pgd"
.LASF83:
	.string	"arch_spinlock_t"
.LASF121:
	.string	"locked_vm"
.LASF159:
	.string	"completion"
.LASF100:
	.string	"pgdval_t"
.LASF71:
	.string	"thread"
.LASF177:
	.string	"upid"
.LASF112:
	.string	"mmap_legacy_base"
.LASF12:
	.string	"__s64"
.LASF144:
	.string	"pcpu_hot"
.LASF7:
	.string	"short int"
.LASF47:
	.string	"exit_state"
.LASF38:
	.string	"__state"
.LASF40:
	.string	"usage"
.LASF173:
	.string	"sched_rt_entity"
.LASF64:
	.string	"nivcsw"
.LASF167:
	.string	"in_exec"
.LASF182:
	.string	"reserved1"
.LASF183:
	.string	"reserved2"
.LASF184:
	.string	"reserved3"
.LASF185:
	.string	"reserved4"
.LASF186:
	.string	"reserved5"
.LASF59:
	.string	"worker_private"
.LASF157:
	.string	"task_list_hdr"
.LASF93:
	.string	"List_hdr"
.LASF45:
	.string	"on_rq"
.LASF198:
	.string	"fsbase"
.LASF46:
	.string	"active_mm"
.LASF188:
	.string	"x86_hw_tss_s"
.LASF49:
	.string	"exit_signal"
.LASF126:
	.string	"def_flags"
.LASF115:
	.string	"mm_users"
.LASF181:
	.string	"x86_hw_tss"
.LASF63:
	.string	"nvcsw"
.LASF153:
	.string	"softirq_pending"
.LASF148:
	.string	"preempt_count"
.LASF108:
	.string	"entry_point"
.LASF65:
	.string	"start_time"
.LASF10:
	.string	"short unsigned int"
.LASF78:
	.string	"atomic_t"
.LASF68:
	.string	"files"
.LASF103:
	.string	"PHYADDR"
.LASF175:
	.string	"time_slice"
.LASF42:
	.string	"on_cpu"
.LASF107:
	.string	"mm_struct"
.LASF91:
	.string	"next"
.LASF176:
	.string	"upid_s"
.LASF70:
	.string	"alloc_lock"
.LASF92:
	.string	"prev"
.LASF81:
	.string	"atomic64_t"
.LASF169:
	.string	"sched_entity_s"
.LASF62:
	.string	"gtime"
	.section	.debug_line_str,"MS",@progbits,1
.LASF0:
	.string	"/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c"
.LASF1:
	.string	"/home/cheyh/projs/MyOS2/build/mykernel"
	.ident	"GCC: (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
