	.file	"asm-offsets.c"
	.text
.Ltext0:
	.file 0 "/home/cheyh/projs/MyOS2/build/mykernel" "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c"
	.section	.text.main,"ax",@progbits
	.globl	main
	.type	main, @function
main:
.LFB131:
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
.LFE131:
	.size	main, .-main
	.section	.text.common,"ax",@progbits
	.type	common, @function
common:
.LFB132:
	.file 2 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c"
	.loc 2 9 1
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
	.loc 2 10 2
#APP
# 10 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->"
# 0 "" 2
	.loc 2 11 2
# 11 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->TASK_threadsp $352 offsetof(struct task_struct, thread.sp)"
# 0 "" 2
	.loc 2 16 2
# 16 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->"
# 0 "" 2
	.loc 2 76 2
# 76 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->"
# 0 "" 2
	.loc 2 77 2
# 77 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->PTREGS_SIZE $176 sizeof(struct pt_regs)"
# 0 "" 2
	.loc 2 88 2
# 88 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->TSS_sp0 $4 offsetof(struct tss_struct, x86_tss.sp0)"
# 0 "" 2
	.loc 2 89 2
# 89 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->TSS_sp1 $12 offsetof(struct tss_struct, x86_tss.sp1)"
# 0 "" 2
	.loc 2 90 2
# 90 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->TSS_sp2 $20 offsetof(struct tss_struct, x86_tss.sp2)"
# 0 "" 2
	.loc 2 91 2
# 91 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->X86_top_of_stack $24 offsetof(struct pcpu_hot, top_of_stack)"
# 0 "" 2
	.loc 2 92 2
# 92 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->X86_current_task $0 offsetof(struct pcpu_hot, current_task)"
# 0 "" 2
	.loc 2 103 1
#NO_APP
	nop
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE132:
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
	.file 23 "/home/cheyh/projs/MyOS2/mykernel/mm/fault/fault_types.h"
	.file 24 "/home/cheyh/projs/MyOS2/mykernel/mm/misc/mm_misc_types.h"
	.file 25 "/home/cheyh/projs/MyOS2/mykernel/mm/page_alloc/buddy_types.h"
	.file 26 "/home/cheyh/projs/MyOS2/mykernel/mm/page_alloc/page-flags_types.h"
	.file 27 "/home/cheyh/projs/MyOS2/mykernel/mm/vm_map/vm_map_types.h"
	.file 28 "/home/cheyh/projs/MyOS2/mykernel/include/linux/fs/vfs_s_defs.h"
	.file 29 "/home/cheyh/projs/MyOS2/mykernel/include/linux/fs/path.h"
	.file 30 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/include/asm/current.h"
	.file 31 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/processor/processor_types_arch.h"
	.file 32 "/home/cheyh/projs/MyOS2/mykernel/include/linux/kernel/swait.h"
	.file 33 "/home/cheyh/projs/MyOS2/mykernel/include/linux/kernel/completion.h"
	.file 34 "/home/cheyh/projs/MyOS2/mykernel/sched/misc/fs_struct_types.h"
	.file 35 "/home/cheyh/projs/MyOS2/mykernel/sched/scheduler/scheduler_types.h"
	.file 36 "/home/cheyh/projs/MyOS2/mykernel/sched/task/pid_types.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x15e2
	.value	0x5
	.byte	0x1
	.byte	0x8
	.long	.Ldebug_abbrev0
	.uleb128 0x1d
	.long	.LASF300
	.byte	0x1d
	.long	.LASF0
	.long	.LASF1
	.long	.LLRL0
	.quad	0
	.long	.Ldebug_line0
	.uleb128 0x8
	.byte	0x8
	.byte	0x7
	.long	.LASF2
	.uleb128 0x8
	.byte	0x1
	.byte	0x6
	.long	.LASF3
	.uleb128 0x8
	.byte	0x4
	.byte	0x7
	.long	.LASF4
	.uleb128 0x8
	.byte	0x1
	.byte	0x6
	.long	.LASF5
	.uleb128 0x2
	.long	.LASF8
	.byte	0x3
	.byte	0x15
	.byte	0x19
	.long	0x52
	.uleb128 0x8
	.byte	0x1
	.byte	0x8
	.long	.LASF6
	.uleb128 0x8
	.byte	0x2
	.byte	0x5
	.long	.LASF7
	.uleb128 0x2
	.long	.LASF9
	.byte	0x3
	.byte	0x18
	.byte	0x1a
	.long	0x6c
	.uleb128 0x8
	.byte	0x2
	.byte	0x7
	.long	.LASF10
	.uleb128 0x1e
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
	.uleb128 0x8
	.byte	0x8
	.byte	0x5
	.long	.LASF13
	.uleb128 0x2
	.long	.LASF14
	.byte	0x3
	.byte	0x1f
	.byte	0x1f
	.long	0xa5
	.uleb128 0x8
	.byte	0x8
	.byte	0x7
	.long	.LASF15
	.uleb128 0x13
	.string	"u8"
	.byte	0xf
	.byte	0x10
	.long	0x46
	.uleb128 0x13
	.string	"u16"
	.byte	0x11
	.byte	0x11
	.long	0x60
	.uleb128 0x13
	.string	"u32"
	.byte	0x13
	.byte	0x11
	.long	0x7a
	.uleb128 0x13
	.string	"s64"
	.byte	0x14
	.byte	0x11
	.long	0x86
	.uleb128 0x13
	.string	"u64"
	.byte	0x15
	.byte	0x11
	.long	0x99
	.uleb128 0x8
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
	.uleb128 0x8
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
	.uleb128 0x2
	.long	.LASF27
	.byte	0x6
	.byte	0xa6
	.byte	0x18
	.long	0x38
	.uleb128 0x1f
	.byte	0x8
	.uleb128 0x2
	.long	.LASF28
	.byte	0x7
	.byte	0x6
	.byte	0x1f
	.long	0x176
	.uleb128 0x19
	.long	.LASF30
	.value	0x100
	.byte	0x8
	.byte	0x57
	.long	0x191
	.uleb128 0x1
	.long	.LASF32
	.byte	0x8
	.byte	0xce
	.byte	0xc
	.long	0x159d
	.byte	0
	.byte	0
	.uleb128 0x2
	.long	.LASF29
	.byte	0x7
	.byte	0x8
	.byte	0x18
	.long	0x19d
	.uleb128 0x5
	.long	.LASF31
	.byte	0x10
	.byte	0x9
	.byte	0x2b
	.byte	0x11
	.long	0x1c5
	.uleb128 0x1
	.long	.LASF33
	.byte	0x9
	.byte	0x2c
	.byte	0x14
	.long	0xe9
	.byte	0
	.uleb128 0x1
	.long	.LASF34
	.byte	0x9
	.byte	0x2d
	.byte	0x14
	.long	0xe9
	.byte	0x8
	.byte	0
	.uleb128 0x2
	.long	.LASF35
	.byte	0x7
	.byte	0xa
	.byte	0x1e
	.long	0x1d1
	.uleb128 0x14
	.long	.LASF99
	.uleb128 0x2
	.long	.LASF36
	.byte	0xa
	.byte	0x5
	.byte	0x1d
	.long	0x1e2
	.uleb128 0x19
	.long	.LASF37
	.value	0x198
	.byte	0xb
	.byte	0x14
	.long	0x3fd
	.uleb128 0x1
	.long	.LASF38
	.byte	0xb
	.byte	0x19
	.byte	0x11
	.long	0x51c
	.byte	0
	.uleb128 0x1
	.long	.LASF39
	.byte	0xb
	.byte	0x1a
	.byte	0x10
	.long	0x38
	.byte	0x18
	.uleb128 0x1
	.long	.LASF40
	.byte	0xb
	.byte	0x27
	.byte	0x9
	.long	0x168
	.byte	0x20
	.uleb128 0x1
	.long	.LASF41
	.byte	0xb
	.byte	0x28
	.byte	0xc
	.long	0x56a
	.byte	0x28
	.uleb128 0x1
	.long	.LASF42
	.byte	0xb
	.byte	0x2a
	.byte	0x10
	.long	0x38
	.byte	0x2c
	.uleb128 0x1
	.long	.LASF43
	.byte	0xb
	.byte	0x2d
	.byte	0x7
	.long	0x73
	.byte	0x30
	.uleb128 0x1
	.long	.LASF44
	.byte	0xb
	.byte	0x3a
	.byte	0x7
	.long	0x73
	.byte	0x34
	.uleb128 0x1
	.long	.LASF45
	.byte	0xb
	.byte	0x3b
	.byte	0x7
	.long	0x73
	.byte	0x38
	.uleb128 0x1
	.long	.LASF46
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
	.long	0x12e6
	.byte	0x40
	.uleb128 0x3
	.string	"rt"
	.byte	0xb
	.byte	0x44
	.byte	0x15
	.long	0x130d
	.byte	0x48
	.uleb128 0x3
	.string	"mm"
	.byte	0xb
	.byte	0x90
	.byte	0x9
	.long	0x1061
	.byte	0x60
	.uleb128 0x1
	.long	.LASF47
	.byte	0xb
	.byte	0x91
	.byte	0x9
	.long	0x1061
	.byte	0x68
	.uleb128 0x1
	.long	.LASF48
	.byte	0xb
	.byte	0x99
	.byte	0x7
	.long	0x73
	.byte	0x70
	.uleb128 0x1
	.long	.LASF49
	.byte	0xb
	.byte	0x9a
	.byte	0x7
	.long	0x73
	.byte	0x74
	.uleb128 0x1
	.long	.LASF50
	.byte	0xb
	.byte	0x9b
	.byte	0x7
	.long	0x73
	.byte	0x78
	.uleb128 0x1a
	.long	.LASF51
	.byte	0xc1
	.long	0x38
	.value	0x3e0
	.uleb128 0x1a
	.long	.LASF52
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
	.long	.LASF53
	.byte	0xb
	.byte	0xee
	.byte	0xb
	.long	0x11e6
	.byte	0x88
	.uleb128 0x1
	.long	.LASF54
	.byte	0xb
	.byte	0xf0
	.byte	0xb
	.long	0x11e6
	.byte	0x90
	.uleb128 0x1
	.long	.LASF55
	.byte	0xb
	.byte	0xf5
	.byte	0xe
	.long	0x690
	.byte	0x98
	.uleb128 0x1
	.long	.LASF56
	.byte	0xb
	.byte	0xf6
	.byte	0xa
	.long	0x62f
	.byte	0xb0
	.uleb128 0xf
	.long	.LASF57
	.value	0x103
	.byte	0xa
	.long	0x1574
	.byte	0xc0
	.uleb128 0xf
	.long	.LASF58
	.value	0x105
	.byte	0xa
	.long	0x62f
	.byte	0xc8
	.uleb128 0xf
	.long	.LASF59
	.value	0x109
	.byte	0x11
	.long	0x1579
	.byte	0xd8
	.uleb128 0xf
	.long	.LASF60
	.value	0x112
	.byte	0x9
	.long	0x168
	.byte	0xe0
	.uleb128 0xf
	.long	.LASF61
	.value	0x114
	.byte	0x7
	.long	0xd7
	.byte	0xe8
	.uleb128 0xf
	.long	.LASF62
	.value	0x115
	.byte	0x7
	.long	0xd7
	.byte	0xf0
	.uleb128 0xf
	.long	.LASF63
	.value	0x11a
	.byte	0x7
	.long	0xd7
	.byte	0xf8
	.uleb128 0xa
	.long	.LASF64
	.value	0x124
	.byte	0x11
	.long	0x2a
	.value	0x100
	.uleb128 0xa
	.long	.LASF65
	.value	0x125
	.byte	0x11
	.long	0x2a
	.value	0x108
	.uleb128 0xa
	.long	.LASF66
	.value	0x128
	.byte	0x7
	.long	0xd7
	.value	0x110
	.uleb128 0xa
	.long	.LASF67
	.value	0x12b
	.byte	0x7
	.long	0xd7
	.value	0x118
	.uleb128 0xa
	.long	.LASF68
	.value	0x14f
	.byte	0x8
	.long	0x157e
	.value	0x120
	.uleb128 0x20
	.string	"fs"
	.byte	0xb
	.value	0x15c
	.byte	0xd
	.long	0x158e
	.value	0x140
	.uleb128 0xa
	.long	.LASF69
	.value	0x15f
	.byte	0x13
	.long	0x1593
	.value	0x148
	.uleb128 0xa
	.long	.LASF70
	.value	0x169
	.byte	0xd
	.long	0x1598
	.value	0x150
	.uleb128 0xa
	.long	.LASF71
	.value	0x185
	.byte	0xe
	.long	0x623
	.value	0x158
	.uleb128 0xa
	.long	.LASF72
	.value	0x2f7
	.byte	0xd
	.long	0x1568
	.value	0x160
	.byte	0
	.uleb128 0x5
	.long	.LASF73
	.byte	0xb0
	.byte	0xc
	.byte	0x9
	.byte	0x12
	.long	0x51c
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
	.long	.LASF74
	.byte	0xc
	.byte	0x20
	.byte	0xa
	.long	0x10d
	.byte	0x78
	.uleb128 0x1
	.long	.LASF75
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
	.long	.LASF42
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
	.long	.LASF76
	.byte	0xa
	.byte	0x13
	.byte	0x1d
	.long	0x528
	.uleb128 0x5
	.long	.LASF38
	.byte	0x18
	.byte	0xd
	.byte	0x13
	.byte	0x11
	.long	0x56a
	.uleb128 0x1
	.long	.LASF42
	.byte	0xd
	.byte	0x14
	.byte	0x9
	.long	0x150
	.byte	0
	.uleb128 0x1
	.long	.LASF77
	.byte	0xd
	.byte	0x15
	.byte	0x9
	.long	0x150
	.byte	0x8
	.uleb128 0x1
	.long	.LASF78
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
	.long	.LASF79
	.byte	0xe
	.byte	0x9
	.byte	0x18
	.long	0x576
	.uleb128 0x5
	.long	.LASF80
	.byte	0x4
	.byte	0xf
	.byte	0x7
	.byte	0x11
	.long	0x591
	.uleb128 0x1
	.long	.LASF81
	.byte	0xf
	.byte	0x8
	.byte	0x7
	.long	0x73
	.byte	0
	.byte	0
	.uleb128 0x2
	.long	.LASF82
	.byte	0xe
	.byte	0xb
	.byte	0x1a
	.long	0x59d
	.uleb128 0x5
	.long	.LASF83
	.byte	0x8
	.byte	0xf
	.byte	0xb
	.byte	0x11
	.long	0x5b8
	.uleb128 0x1
	.long	.LASF81
	.byte	0xf
	.byte	0xc
	.byte	0x7
	.long	0xcc
	.byte	0
	.byte	0
	.uleb128 0x2
	.long	.LASF84
	.byte	0xe
	.byte	0xf
	.byte	0x1b
	.long	0x5c4
	.uleb128 0x5
	.long	.LASF85
	.byte	0x8
	.byte	0x10
	.byte	0x8
	.byte	0x11
	.long	0x5d8
	.uleb128 0xb
	.long	0x5fc
	.byte	0
	.byte	0
	.uleb128 0x10
	.byte	0x8
	.byte	0x10
	.byte	0xc
	.byte	0x4
	.long	0x5fc
	.uleb128 0x1
	.long	.LASF86
	.byte	0x10
	.byte	0xd
	.byte	0x9
	.long	0xc1
	.byte	0
	.uleb128 0x1
	.long	.LASF87
	.byte	0x10
	.byte	0xe
	.byte	0x9
	.long	0xc1
	.byte	0x4
	.byte	0
	.uleb128 0x11
	.byte	0x8
	.byte	0x10
	.byte	0x9
	.byte	0x3
	.long	0x617
	.uleb128 0xc
	.string	"val"
	.byte	0x10
	.byte	0xa
	.byte	0xf
	.long	0x591
	.uleb128 0x15
	.long	0x5d8
	.byte	0
	.uleb128 0x2
	.long	.LASF88
	.byte	0x11
	.byte	0x9
	.byte	0x15
	.long	0x591
	.uleb128 0x2
	.long	.LASF89
	.byte	0x11
	.byte	0xd
	.byte	0x1a
	.long	0x5b8
	.uleb128 0x2
	.long	.LASF90
	.byte	0x12
	.byte	0xb
	.byte	0x1b
	.long	0x63b
	.uleb128 0x5
	.long	.LASF91
	.byte	0x10
	.byte	0x13
	.byte	0x8
	.byte	0x11
	.long	0x663
	.uleb128 0x1
	.long	.LASF92
	.byte	0x13
	.byte	0x9
	.byte	0xc
	.long	0x663
	.byte	0
	.uleb128 0x1
	.long	.LASF93
	.byte	0x13
	.byte	0xa
	.byte	0xc
	.long	0x663
	.byte	0x8
	.byte	0
	.uleb128 0x7
	.long	0x62f
	.uleb128 0x5
	.long	.LASF94
	.byte	0x18
	.byte	0x13
	.byte	0xd
	.byte	0x11
	.long	0x690
	.uleb128 0x1
	.long	.LASF95
	.byte	0x13
	.byte	0xe
	.byte	0xa
	.long	0x62f
	.byte	0
	.uleb128 0x1
	.long	.LASF96
	.byte	0x13
	.byte	0xf
	.byte	0x9
	.long	0x150
	.byte	0x10
	.byte	0
	.uleb128 0x2
	.long	.LASF97
	.byte	0x13
	.byte	0x10
	.byte	0x4
	.long	0x668
	.uleb128 0x2
	.long	.LASF98
	.byte	0x14
	.byte	0x5
	.byte	0x1f
	.long	0x6a8
	.uleb128 0x14
	.long	.LASF100
	.uleb128 0x2
	.long	.LASF101
	.byte	0x14
	.byte	0x7
	.byte	0x16
	.long	0x6b9
	.uleb128 0x14
	.long	.LASF102
	.uleb128 0x2
	.long	.LASF103
	.byte	0x15
	.byte	0x15
	.byte	0x10
	.long	0x150
	.uleb128 0x2
	.long	.LASF104
	.byte	0x15
	.byte	0x16
	.byte	0x10
	.long	0x150
	.uleb128 0x2
	.long	.LASF105
	.byte	0x15
	.byte	0x17
	.byte	0x10
	.long	0x150
	.uleb128 0x2
	.long	.LASF106
	.byte	0x15
	.byte	0x19
	.byte	0x10
	.long	0x150
	.uleb128 0x2
	.long	.LASF107
	.byte	0x15
	.byte	0x1c
	.byte	0x1a
	.long	0x6fa
	.uleb128 0x5
	.long	.LASF108
	.byte	0x8
	.byte	0x16
	.byte	0x8
	.byte	0x11
	.long	0x75b
	.uleb128 0x4
	.string	"P"
	.byte	0xa
	.long	0x2a
	.byte	0
	.uleb128 0x4
	.string	"RW"
	.byte	0xb
	.long	0x2a
	.byte	0x1
	.uleb128 0x4
	.string	"US"
	.byte	0xc
	.long	0x2a
	.byte	0x2
	.uleb128 0x4
	.string	"PWT"
	.byte	0xd
	.long	0x2a
	.byte	0x3
	.uleb128 0x4
	.string	"PCD"
	.byte	0xe
	.long	0x2a
	.byte	0x4
	.uleb128 0x4
	.string	"A"
	.byte	0xf
	.long	0x2a
	.byte	0x5
	.uleb128 0x6
	.long	.LASF109
	.byte	0x16
	.byte	0x11
	.long	0x2a
	.byte	0x24
	.byte	0xc
	.uleb128 0x4
	.string	"XD"
	.byte	0x13
	.long	0x2a
	.byte	0x3f
	.byte	0
	.uleb128 0x2
	.long	.LASF110
	.byte	0x15
	.byte	0x1e
	.byte	0x1a
	.long	0x767
	.uleb128 0x5
	.long	.LASF111
	.byte	0x8
	.byte	0x16
	.byte	0x16
	.byte	0x11
	.long	0x7d2
	.uleb128 0x4
	.string	"P"
	.byte	0x18
	.long	0x2a
	.byte	0
	.uleb128 0x4
	.string	"RW"
	.byte	0x19
	.long	0x2a
	.byte	0x1
	.uleb128 0x4
	.string	"US"
	.byte	0x1a
	.long	0x2a
	.byte	0x2
	.uleb128 0x4
	.string	"PWT"
	.byte	0x1b
	.long	0x2a
	.byte	0x3
	.uleb128 0x4
	.string	"PCD"
	.byte	0x1c
	.long	0x2a
	.byte	0x4
	.uleb128 0x4
	.string	"A"
	.byte	0x1d
	.long	0x2a
	.byte	0x5
	.uleb128 0x4
	.string	"PS"
	.byte	0x1f
	.long	0x2a
	.byte	0x7
	.uleb128 0x6
	.long	.LASF109
	.byte	0x16
	.byte	0x22
	.long	0x2a
	.byte	0x24
	.byte	0xc
	.uleb128 0x4
	.string	"XD"
	.byte	0x24
	.long	0x2a
	.byte	0x3f
	.byte	0
	.uleb128 0x2
	.long	.LASF112
	.byte	0x15
	.byte	0x20
	.byte	0x1a
	.long	0x7de
	.uleb128 0x5
	.long	.LASF113
	.byte	0x8
	.byte	0x16
	.byte	0x27
	.byte	0x11
	.long	0x849
	.uleb128 0x4
	.string	"P"
	.byte	0x29
	.long	0x2a
	.byte	0
	.uleb128 0x4
	.string	"RW"
	.byte	0x2a
	.long	0x2a
	.byte	0x1
	.uleb128 0x4
	.string	"US"
	.byte	0x2b
	.long	0x2a
	.byte	0x2
	.uleb128 0x4
	.string	"PWT"
	.byte	0x2c
	.long	0x2a
	.byte	0x3
	.uleb128 0x4
	.string	"PCD"
	.byte	0x2d
	.long	0x2a
	.byte	0x4
	.uleb128 0x4
	.string	"A"
	.byte	0x2e
	.long	0x2a
	.byte	0x5
	.uleb128 0x4
	.string	"PS"
	.byte	0x30
	.long	0x2a
	.byte	0x7
	.uleb128 0x6
	.long	.LASF109
	.byte	0x16
	.byte	0x33
	.long	0x2a
	.byte	0x24
	.byte	0xc
	.uleb128 0x4
	.string	"XD"
	.byte	0x35
	.long	0x2a
	.byte	0x3f
	.byte	0
	.uleb128 0x2
	.long	.LASF114
	.byte	0x15
	.byte	0x22
	.byte	0x1a
	.long	0x855
	.uleb128 0x5
	.long	.LASF115
	.byte	0x8
	.byte	0x16
	.byte	0x38
	.byte	0x11
	.long	0x8d3
	.uleb128 0x4
	.string	"P"
	.byte	0x3a
	.long	0x2a
	.byte	0
	.uleb128 0x4
	.string	"RW"
	.byte	0x3b
	.long	0x2a
	.byte	0x1
	.uleb128 0x4
	.string	"US"
	.byte	0x3c
	.long	0x2a
	.byte	0x2
	.uleb128 0x4
	.string	"PWT"
	.byte	0x3d
	.long	0x2a
	.byte	0x3
	.uleb128 0x4
	.string	"PCD"
	.byte	0x3e
	.long	0x2a
	.byte	0x4
	.uleb128 0x4
	.string	"A"
	.byte	0x3f
	.long	0x2a
	.byte	0x5
	.uleb128 0x4
	.string	"D"
	.byte	0x40
	.long	0x2a
	.byte	0x6
	.uleb128 0x4
	.string	"PAT"
	.byte	0x41
	.long	0x2a
	.byte	0x7
	.uleb128 0x4
	.string	"G"
	.byte	0x42
	.long	0x2a
	.byte	0x8
	.uleb128 0x6
	.long	.LASF109
	.byte	0x16
	.byte	0x44
	.long	0x2a
	.byte	0x24
	.byte	0xc
	.uleb128 0x4
	.string	"XD"
	.byte	0x46
	.long	0x2a
	.byte	0x3f
	.byte	0
	.uleb128 0x2
	.long	.LASF116
	.byte	0x15
	.byte	0x24
	.byte	0x1a
	.long	0x8df
	.uleb128 0x16
	.long	.LASF119
	.byte	0x49
	.long	0x902
	.uleb128 0xc
	.string	"val"
	.byte	0x16
	.byte	0x4a
	.byte	0xc
	.long	0x6e2
	.uleb128 0x9
	.long	.LASF117
	.byte	0x16
	.byte	0x4b
	.byte	0xe
	.long	0x6ee
	.byte	0
	.uleb128 0x2
	.long	.LASF118
	.byte	0x15
	.byte	0x26
	.byte	0x1a
	.long	0x90e
	.uleb128 0x16
	.long	.LASF120
	.byte	0x4e
	.long	0x931
	.uleb128 0xc
	.string	"pgd"
	.byte	0x16
	.byte	0x4f
	.byte	0x9
	.long	0x8d3
	.uleb128 0x9
	.long	.LASF117
	.byte	0x16
	.byte	0x50
	.byte	0xe
	.long	0x6ee
	.byte	0
	.uleb128 0x2
	.long	.LASF121
	.byte	0x15
	.byte	0x28
	.byte	0x1a
	.long	0x93d
	.uleb128 0x16
	.long	.LASF122
	.byte	0x53
	.long	0x960
	.uleb128 0xc
	.string	"val"
	.byte	0x16
	.byte	0x54
	.byte	0xc
	.long	0x6d6
	.uleb128 0x9
	.long	.LASF117
	.byte	0x16
	.byte	0x55
	.byte	0xe
	.long	0x75b
	.byte	0
	.uleb128 0x2
	.long	.LASF123
	.byte	0x15
	.byte	0x2a
	.byte	0x1a
	.long	0x96c
	.uleb128 0x16
	.long	.LASF124
	.byte	0x58
	.long	0x98f
	.uleb128 0xc
	.string	"val"
	.byte	0x16
	.byte	0x59
	.byte	0xc
	.long	0x6ca
	.uleb128 0x9
	.long	.LASF117
	.byte	0x16
	.byte	0x5a
	.byte	0xe
	.long	0x7d2
	.byte	0
	.uleb128 0x2
	.long	.LASF125
	.byte	0x15
	.byte	0x2c
	.byte	0x1a
	.long	0x99b
	.uleb128 0x16
	.long	.LASF126
	.byte	0x5d
	.long	0x9be
	.uleb128 0xc
	.string	"val"
	.byte	0x16
	.byte	0x5e
	.byte	0xc
	.long	0x6be
	.uleb128 0x9
	.long	.LASF117
	.byte	0x16
	.byte	0x5f
	.byte	0xe
	.long	0x849
	.byte	0
	.uleb128 0x2
	.long	.LASF127
	.byte	0x14
	.byte	0x22
	.byte	0xf
	.long	0x144
	.uleb128 0x2
	.long	.LASF128
	.byte	0x14
	.byte	0x24
	.byte	0x1a
	.long	0x9d6
	.uleb128 0x5
	.long	.LASF129
	.byte	0x60
	.byte	0x17
	.byte	0x68
	.byte	0x11
	.long	0xa52
	.uleb128 0xb
	.long	0xda2
	.byte	0
	.uleb128 0x1
	.long	.LASF42
	.byte	0x17
	.byte	0x6f
	.byte	0x13
	.long	0xd13
	.byte	0x20
	.uleb128 0x3
	.string	"p4d"
	.byte	0x17
	.byte	0x72
	.byte	0xa
	.long	0xdac
	.byte	0x28
	.uleb128 0x3
	.string	"pud"
	.byte	0x17
	.byte	0x73
	.byte	0xa
	.long	0xdb1
	.byte	0x30
	.uleb128 0x3
	.string	"pmd"
	.byte	0x17
	.byte	0x76
	.byte	0xa
	.long	0xdb6
	.byte	0x38
	.uleb128 0x3
	.string	"pte"
	.byte	0x17
	.byte	0x78
	.byte	0xa
	.long	0xdbb
	.byte	0x40
	.uleb128 0x1
	.long	.LASF130
	.byte	0x17
	.byte	0x7d
	.byte	0xa
	.long	0x98f
	.byte	0x48
	.uleb128 0x1
	.long	.LASF131
	.byte	0x17
	.byte	0x83
	.byte	0xb
	.long	0xdc0
	.byte	0x50
	.uleb128 0x1
	.long	.LASF132
	.byte	0x17
	.byte	0x84
	.byte	0xb
	.long	0xdc0
	.byte	0x58
	.byte	0
	.uleb128 0x2
	.long	.LASF133
	.byte	0x14
	.byte	0x32
	.byte	0x1b
	.long	0xa5e
	.uleb128 0x5
	.long	.LASF134
	.byte	0xf0
	.byte	0x18
	.byte	0x7
	.byte	0x11
	.long	0xa7f
	.uleb128 0x1
	.long	.LASF135
	.byte	0x18
	.byte	0x8
	.byte	0xa
	.long	0x138
	.byte	0
	.uleb128 0xb
	.long	0xdc5
	.byte	0x8
	.byte	0
	.uleb128 0x2
	.long	.LASF136
	.byte	0x14
	.byte	0x3a
	.byte	0x16
	.long	0xa8b
	.uleb128 0x5
	.long	.LASF132
	.byte	0x38
	.byte	0x19
	.byte	0x2d
	.byte	0x11
	.long	0xab8
	.uleb128 0xb
	.long	0xf5f
	.byte	0
	.uleb128 0xb
	.long	0x102b
	.byte	0x8
	.uleb128 0xb
	.long	0x103f
	.byte	0x30
	.uleb128 0x1
	.long	.LASF137
	.byte	0x19
	.byte	0x97
	.byte	0xc
	.long	0x56a
	.byte	0x34
	.byte	0
	.uleb128 0x2
	.long	.LASF138
	.byte	0x14
	.byte	0x3e
	.byte	0x1d
	.long	0xac4
	.uleb128 0x5
	.long	.LASF139
	.byte	0x3
	.byte	0x1a
	.byte	0xb4
	.byte	0x11
	.long	0xc0a
	.uleb128 0x6
	.long	.LASF140
	.byte	0x1a
	.byte	0xb6
	.long	0x2a
	.byte	0x1
	.byte	0
	.uleb128 0x6
	.long	.LASF141
	.byte	0x1a
	.byte	0xb7
	.long	0x2a
	.byte	0x1
	.byte	0x1
	.uleb128 0x6
	.long	.LASF142
	.byte	0x1a
	.byte	0xb8
	.long	0x2a
	.byte	0x1
	.byte	0x2
	.uleb128 0x6
	.long	.LASF143
	.byte	0x1a
	.byte	0xb9
	.long	0x2a
	.byte	0x1
	.byte	0x3
	.uleb128 0x6
	.long	.LASF144
	.byte	0x1a
	.byte	0xba
	.long	0x2a
	.byte	0x1
	.byte	0x4
	.uleb128 0x6
	.long	.LASF145
	.byte	0x1a
	.byte	0xbb
	.long	0x2a
	.byte	0x1
	.byte	0x5
	.uleb128 0x6
	.long	.LASF146
	.byte	0x1a
	.byte	0xbc
	.long	0x2a
	.byte	0x1
	.byte	0x6
	.uleb128 0x6
	.long	.LASF147
	.byte	0x1a
	.byte	0xbd
	.long	0x2a
	.byte	0x1
	.byte	0x7
	.uleb128 0x6
	.long	.LASF148
	.byte	0x1a
	.byte	0xbe
	.long	0x2a
	.byte	0x1
	.byte	0x8
	.uleb128 0x6
	.long	.LASF149
	.byte	0x1a
	.byte	0xbf
	.long	0x2a
	.byte	0x1
	.byte	0x9
	.uleb128 0x6
	.long	.LASF150
	.byte	0x1a
	.byte	0xc0
	.long	0x2a
	.byte	0x1
	.byte	0xa
	.uleb128 0x6
	.long	.LASF151
	.byte	0x1a
	.byte	0xc1
	.long	0x2a
	.byte	0x1
	.byte	0xb
	.uleb128 0x6
	.long	.LASF152
	.byte	0x1a
	.byte	0xc2
	.long	0x2a
	.byte	0x1
	.byte	0xc
	.uleb128 0x6
	.long	.LASF153
	.byte	0x1a
	.byte	0xc3
	.long	0x2a
	.byte	0x1
	.byte	0xd
	.uleb128 0x6
	.long	.LASF154
	.byte	0x1a
	.byte	0xc4
	.long	0x2a
	.byte	0x1
	.byte	0xe
	.uleb128 0x6
	.long	.LASF155
	.byte	0x1a
	.byte	0xc5
	.long	0x2a
	.byte	0x1
	.byte	0xf
	.uleb128 0x6
	.long	.LASF156
	.byte	0x1a
	.byte	0xc6
	.long	0x2a
	.byte	0x1
	.byte	0x10
	.uleb128 0x6
	.long	.LASF157
	.byte	0x1a
	.byte	0xc7
	.long	0x2a
	.byte	0x1
	.byte	0x11
	.uleb128 0x6
	.long	.LASF158
	.byte	0x1a
	.byte	0xc8
	.long	0x2a
	.byte	0x1
	.byte	0x12
	.uleb128 0x6
	.long	.LASF159
	.byte	0x1a
	.byte	0xc9
	.long	0x2a
	.byte	0x1
	.byte	0x13
	.uleb128 0x6
	.long	.LASF160
	.byte	0x1a
	.byte	0xca
	.long	0x2a
	.byte	0x1
	.byte	0x14
	.uleb128 0x6
	.long	.LASF161
	.byte	0x1a
	.byte	0xcb
	.long	0x2a
	.byte	0x1
	.byte	0x15
	.uleb128 0x6
	.long	.LASF162
	.byte	0x1a
	.byte	0xd6
	.long	0x2a
	.byte	0x1
	.byte	0x16
	.uleb128 0x6
	.long	.LASF163
	.byte	0x1a
	.byte	0xd7
	.long	0x2a
	.byte	0x1
	.byte	0x17
	.byte	0
	.uleb128 0x2
	.long	.LASF164
	.byte	0x14
	.byte	0x42
	.byte	0x20
	.long	0xc16
	.uleb128 0x5
	.long	.LASF165
	.byte	0x48
	.byte	0x1b
	.byte	0xd
	.byte	0x11
	.long	0xc99
	.uleb128 0x1
	.long	.LASF166
	.byte	0x1b
	.byte	0x10
	.byte	0x9
	.long	0x150
	.byte	0
	.uleb128 0x1
	.long	.LASF167
	.byte	0x1b
	.byte	0x11
	.byte	0x9
	.long	0x150
	.byte	0x8
	.uleb128 0x1
	.long	.LASF168
	.byte	0x1b
	.byte	0x15
	.byte	0xa
	.long	0xda7
	.byte	0x10
	.uleb128 0x1
	.long	.LASF169
	.byte	0x1b
	.byte	0x15
	.byte	0x14
	.long	0xda7
	.byte	0x18
	.uleb128 0x1
	.long	.LASF170
	.byte	0x1b
	.byte	0x23
	.byte	0x9
	.long	0x1061
	.byte	0x20
	.uleb128 0x1
	.long	.LASF171
	.byte	0x1b
	.byte	0x2a
	.byte	0x9
	.long	0x150
	.byte	0x28
	.uleb128 0x1
	.long	.LASF172
	.byte	0x1b
	.byte	0x4d
	.byte	0x13
	.long	0x1066
	.byte	0x30
	.uleb128 0x1
	.long	.LASF173
	.byte	0x1b
	.byte	0x50
	.byte	0x9
	.long	0x150
	.byte	0x38
	.uleb128 0x1
	.long	.LASF174
	.byte	0x1b
	.byte	0x51
	.byte	0xb
	.long	0xf5a
	.byte	0x40
	.byte	0
	.uleb128 0x2
	.long	.LASF175
	.byte	0x14
	.byte	0x44
	.byte	0x26
	.long	0xcaa
	.uleb128 0x1b
	.long	0xc99
	.uleb128 0x5
	.long	.LASF176
	.byte	0x38
	.byte	0x1b
	.byte	0x62
	.byte	0x11
	.long	0xd13
	.uleb128 0x1
	.long	.LASF177
	.byte	0x1b
	.byte	0x63
	.byte	0xa
	.long	0x1076
	.byte	0
	.uleb128 0x1
	.long	.LASF178
	.byte	0x1b
	.byte	0x68
	.byte	0xa
	.long	0x1076
	.byte	0x8
	.uleb128 0x1
	.long	.LASF179
	.byte	0x1b
	.byte	0x6a
	.byte	0x9
	.long	0x108f
	.byte	0x10
	.uleb128 0x1
	.long	.LASF180
	.byte	0x1b
	.byte	0x6b
	.byte	0x9
	.long	0x10a3
	.byte	0x18
	.uleb128 0x1
	.long	.LASF181
	.byte	0x1b
	.byte	0x73
	.byte	0x10
	.long	0x10bc
	.byte	0x20
	.uleb128 0x1
	.long	.LASF182
	.byte	0x1b
	.byte	0x76
	.byte	0x10
	.long	0x10da
	.byte	0x28
	.uleb128 0x1
	.long	.LASF183
	.byte	0x1b
	.byte	0x7c
	.byte	0x10
	.long	0x10bc
	.byte	0x30
	.byte	0
	.uleb128 0x21
	.long	.LASF301
	.byte	0x7
	.byte	0x4
	.long	0x38
	.byte	0x17
	.byte	0x51
	.byte	0x7
	.long	0xd64
	.uleb128 0xd
	.long	.LASF184
	.byte	0x1
	.uleb128 0xd
	.long	.LASF185
	.byte	0x2
	.uleb128 0xd
	.long	.LASF186
	.byte	0x4
	.uleb128 0xd
	.long	.LASF187
	.byte	0x8
	.uleb128 0xd
	.long	.LASF188
	.byte	0x10
	.uleb128 0xd
	.long	.LASF189
	.byte	0x20
	.uleb128 0xd
	.long	.LASF190
	.byte	0x40
	.uleb128 0xd
	.long	.LASF191
	.byte	0x80
	.uleb128 0x1c
	.long	.LASF192
	.value	0x100
	.uleb128 0x1c
	.long	.LASF193
	.value	0x200
	.byte	0
	.uleb128 0x10
	.byte	0x20
	.byte	0x17
	.byte	0x69
	.byte	0x9
	.long	0xda2
	.uleb128 0x3
	.string	"vma"
	.byte	0x17
	.byte	0x6a
	.byte	0xb
	.long	0xda7
	.byte	0
	.uleb128 0x1
	.long	.LASF194
	.byte	0x17
	.byte	0x6b
	.byte	0xa
	.long	0x15c
	.byte	0x8
	.uleb128 0x1
	.long	.LASF195
	.byte	0x17
	.byte	0x6c
	.byte	0x12
	.long	0x2a
	.byte	0x10
	.uleb128 0x1
	.long	.LASF196
	.byte	0x17
	.byte	0x6d
	.byte	0xa
	.long	0x150
	.byte	0x18
	.byte	0
	.uleb128 0x1b
	.long	0xd64
	.uleb128 0x7
	.long	0xc0a
	.uleb128 0x7
	.long	0x902
	.uleb128 0x7
	.long	0x931
	.uleb128 0x7
	.long	0x960
	.uleb128 0x7
	.long	0x98f
	.uleb128 0x7
	.long	0xa7f
	.uleb128 0x10
	.byte	0xe8
	.byte	0x18
	.byte	0x9
	.byte	0x3
	.long	0xf55
	.uleb128 0x1
	.long	.LASF197
	.byte	0x18
	.byte	0xa
	.byte	0xb
	.long	0xda7
	.byte	0
	.uleb128 0x1
	.long	.LASF198
	.byte	0x18
	.byte	0x10
	.byte	0xa
	.long	0x150
	.byte	0x8
	.uleb128 0x1
	.long	.LASF199
	.byte	0x18
	.byte	0x17
	.byte	0xa
	.long	0x150
	.byte	0x10
	.uleb128 0x1
	.long	.LASF200
	.byte	0x18
	.byte	0x18
	.byte	0xa
	.long	0x150
	.byte	0x18
	.uleb128 0x3
	.string	"pgd"
	.byte	0x18
	.byte	0x19
	.byte	0xb
	.long	0xf55
	.byte	0x20
	.uleb128 0x1
	.long	.LASF201
	.byte	0x18
	.byte	0x2e
	.byte	0xd
	.long	0x56a
	.byte	0x28
	.uleb128 0x1
	.long	.LASF202
	.byte	0x18
	.byte	0x37
	.byte	0xd
	.long	0x56a
	.byte	0x2c
	.uleb128 0x1
	.long	.LASF203
	.byte	0x18
	.byte	0x38
	.byte	0x12
	.long	0x617
	.byte	0x30
	.uleb128 0x1
	.long	.LASF204
	.byte	0x18
	.byte	0x39
	.byte	0x8
	.long	0x73
	.byte	0x38
	.uleb128 0x1
	.long	.LASF205
	.byte	0x18
	.byte	0x3a
	.byte	0xf
	.long	0x623
	.byte	0x40
	.uleb128 0x1
	.long	.LASF206
	.byte	0x18
	.byte	0x53
	.byte	0xa
	.long	0x150
	.byte	0x48
	.uleb128 0x1
	.long	.LASF207
	.byte	0x18
	.byte	0x54
	.byte	0xa
	.long	0x150
	.byte	0x50
	.uleb128 0x1
	.long	.LASF208
	.byte	0x18
	.byte	0x55
	.byte	0xf
	.long	0x591
	.byte	0x58
	.uleb128 0x1
	.long	.LASF209
	.byte	0x18
	.byte	0x56
	.byte	0xa
	.long	0x150
	.byte	0x60
	.uleb128 0x1
	.long	.LASF210
	.byte	0x18
	.byte	0x57
	.byte	0xa
	.long	0x150
	.byte	0x68
	.uleb128 0x1
	.long	.LASF211
	.byte	0x18
	.byte	0x58
	.byte	0xa
	.long	0x150
	.byte	0x70
	.uleb128 0x1
	.long	.LASF212
	.byte	0x18
	.byte	0x59
	.byte	0xa
	.long	0x150
	.byte	0x78
	.uleb128 0x1
	.long	.LASF213
	.byte	0x18
	.byte	0x64
	.byte	0xa
	.long	0x150
	.byte	0x80
	.uleb128 0x1
	.long	.LASF214
	.byte	0x18
	.byte	0x64
	.byte	0x16
	.long	0x150
	.byte	0x88
	.uleb128 0x1
	.long	.LASF215
	.byte	0x18
	.byte	0x64
	.byte	0x20
	.long	0x150
	.byte	0x90
	.uleb128 0x1
	.long	.LASF216
	.byte	0x18
	.byte	0x64
	.byte	0x2c
	.long	0x150
	.byte	0x98
	.uleb128 0x1
	.long	.LASF217
	.byte	0x18
	.byte	0x65
	.byte	0xa
	.long	0x150
	.byte	0xa0
	.uleb128 0x3
	.string	"brk"
	.byte	0x18
	.byte	0x65
	.byte	0x15
	.long	0x150
	.byte	0xa8
	.uleb128 0x1
	.long	.LASF218
	.byte	0x18
	.byte	0x65
	.byte	0x1a
	.long	0x150
	.byte	0xb0
	.uleb128 0x1
	.long	.LASF219
	.byte	0x18
	.byte	0x66
	.byte	0xa
	.long	0x150
	.byte	0xb8
	.uleb128 0x1
	.long	.LASF220
	.byte	0x18
	.byte	0x66
	.byte	0x15
	.long	0x150
	.byte	0xc0
	.uleb128 0x1
	.long	.LASF221
	.byte	0x18
	.byte	0x66
	.byte	0x1e
	.long	0x150
	.byte	0xc8
	.uleb128 0x1
	.long	.LASF222
	.byte	0x18
	.byte	0x66
	.byte	0x29
	.long	0x150
	.byte	0xd0
	.uleb128 0x1
	.long	.LASF42
	.byte	0x18
	.byte	0x75
	.byte	0xa
	.long	0x150
	.byte	0xd8
	.uleb128 0x1
	.long	.LASF223
	.byte	0x18
	.byte	0x8b
	.byte	0xc
	.long	0xf5a
	.byte	0xe0
	.byte	0
	.uleb128 0x7
	.long	0x8d3
	.uleb128 0x7
	.long	0x6ad
	.uleb128 0x11
	.byte	0x8
	.byte	0x19
	.byte	0x2e
	.byte	0x3
	.long	0xf81
	.uleb128 0x9
	.long	.LASF42
	.byte	0x19
	.byte	0x2f
	.byte	0xa
	.long	0x150
	.uleb128 0x9
	.long	.LASF224
	.byte	0x19
	.byte	0x30
	.byte	0x12
	.long	0xab8
	.byte	0
	.uleb128 0x10
	.byte	0x10
	.byte	0x19
	.byte	0x42
	.byte	0x6
	.long	0xfa5
	.uleb128 0x1
	.long	.LASF225
	.byte	0x19
	.byte	0x44
	.byte	0xd
	.long	0x168
	.byte	0
	.uleb128 0x1
	.long	.LASF226
	.byte	0x19
	.byte	0x46
	.byte	0xc
	.long	0x144
	.byte	0x8
	.byte	0
	.uleb128 0x11
	.byte	0x10
	.byte	0x19
	.byte	0x3f
	.byte	0x5
	.long	0xfd8
	.uleb128 0xc
	.string	"lru"
	.byte	0x19
	.byte	0x40
	.byte	0xd
	.long	0x62f
	.uleb128 0x15
	.long	0xf81
	.uleb128 0x9
	.long	.LASF227
	.byte	0x19
	.byte	0x49
	.byte	0xd
	.long	0x62f
	.uleb128 0x9
	.long	.LASF228
	.byte	0x19
	.byte	0x4a
	.byte	0xd
	.long	0x62f
	.byte	0
	.uleb128 0x10
	.byte	0x28
	.byte	0x19
	.byte	0x39
	.byte	0x4
	.long	0x100f
	.uleb128 0xb
	.long	0xfa5
	.byte	0
	.uleb128 0x1
	.long	.LASF229
	.byte	0x19
	.byte	0x4d
	.byte	0x11
	.long	0x100f
	.byte	0x10
	.uleb128 0x1
	.long	.LASF230
	.byte	0x19
	.byte	0x4e
	.byte	0x13
	.long	0x2a
	.byte	0x18
	.uleb128 0x1
	.long	.LASF231
	.byte	0x19
	.byte	0x55
	.byte	0xb
	.long	0x150
	.byte	0x20
	.byte	0
	.uleb128 0x7
	.long	0x69c
	.uleb128 0x10
	.byte	0x8
	.byte	0x19
	.byte	0x6f
	.byte	0x4
	.long	0x102b
	.uleb128 0x1
	.long	.LASF232
	.byte	0x19
	.byte	0x70
	.byte	0xb
	.long	0x150
	.byte	0
	.byte	0
	.uleb128 0x11
	.byte	0x28
	.byte	0x19
	.byte	0x38
	.byte	0x3
	.long	0x103f
	.uleb128 0x15
	.long	0xfd8
	.uleb128 0x15
	.long	0x1014
	.byte	0
	.uleb128 0x11
	.byte	0x4
	.byte	0x19
	.byte	0x87
	.byte	0x3
	.long	0x1061
	.uleb128 0x9
	.long	.LASF233
	.byte	0x19
	.byte	0x8c
	.byte	0xd
	.long	0x56a
	.uleb128 0x9
	.long	.LASF234
	.byte	0x19
	.byte	0x93
	.byte	0x9
	.long	0x144
	.byte	0
	.uleb128 0x7
	.long	0xa52
	.uleb128 0x7
	.long	0xca5
	.uleb128 0x22
	.long	0x1076
	.uleb128 0xe
	.long	0xda7
	.byte	0
	.uleb128 0x7
	.long	0x106b
	.uleb128 0x18
	.long	0x73
	.long	0x108f
	.uleb128 0xe
	.long	0xda7
	.uleb128 0xe
	.long	0x150
	.byte	0
	.uleb128 0x7
	.long	0x107b
	.uleb128 0x18
	.long	0x73
	.long	0x10a3
	.uleb128 0xe
	.long	0xda7
	.byte	0
	.uleb128 0x7
	.long	0x1094
	.uleb128 0x18
	.long	0x9be
	.long	0x10b7
	.uleb128 0xe
	.long	0x10b7
	.byte	0
	.uleb128 0x7
	.long	0x9ca
	.uleb128 0x7
	.long	0x10a8
	.uleb128 0x18
	.long	0x9be
	.long	0x10da
	.uleb128 0xe
	.long	0x10b7
	.uleb128 0xe
	.long	0x2a
	.uleb128 0xe
	.long	0x2a
	.byte	0
	.uleb128 0x7
	.long	0x10c1
	.uleb128 0x2
	.long	.LASF235
	.byte	0x1c
	.byte	0x4
	.byte	0x18
	.long	0x10eb
	.uleb128 0x14
	.long	.LASF236
	.uleb128 0x2
	.long	.LASF237
	.byte	0x1c
	.byte	0x11
	.byte	0x1a
	.long	0x10fc
	.uleb128 0x14
	.long	.LASF238
	.uleb128 0x2
	.long	.LASF239
	.byte	0x1c
	.byte	0x21
	.byte	0x16
	.long	0x110d
	.uleb128 0x5
	.long	.LASF240
	.byte	0x10
	.byte	0x1d
	.byte	0x7
	.byte	0x11
	.long	0x1135
	.uleb128 0x3
	.string	"mnt"
	.byte	0x1d
	.byte	0x8
	.byte	0xf
	.long	0x13b5
	.byte	0
	.uleb128 0x1
	.long	.LASF236
	.byte	0x1d
	.byte	0x9
	.byte	0xd
	.long	0x13ba
	.byte	0x8
	.byte	0
	.uleb128 0x5
	.long	.LASF241
	.byte	0x40
	.byte	0x1e
	.byte	0x10
	.byte	0x11
	.long	0x1149
	.uleb128 0xb
	.long	0x11eb
	.byte	0
	.byte	0
	.uleb128 0x23
	.long	.LASF302
	.value	0x5000
	.value	0x1000
	.byte	0x1f
	.byte	0x9d
	.byte	0x9
	.long	0x1174
	.uleb128 0x1
	.long	.LASF242
	.byte	0x1f
	.byte	0xa3
	.byte	0x10
	.long	0x146f
	.byte	0
	.uleb128 0x1
	.long	.LASF243
	.byte	0x1f
	.byte	0xa5
	.byte	0x13
	.long	0x14cf
	.byte	0x68
	.byte	0
	.uleb128 0x10
	.byte	0x30
	.byte	0x1e
	.byte	0x12
	.byte	0x4
	.long	0x11e6
	.uleb128 0x1
	.long	.LASF244
	.byte	0x1e
	.byte	0x13
	.byte	0xd
	.long	0x11e6
	.byte	0
	.uleb128 0x1
	.long	.LASF245
	.byte	0x1e
	.byte	0x14
	.byte	0x9
	.long	0x73
	.byte	0x8
	.uleb128 0x1
	.long	.LASF246
	.byte	0x1e
	.byte	0x15
	.byte	0x9
	.long	0x73
	.byte	0xc
	.uleb128 0x1
	.long	.LASF247
	.byte	0x1e
	.byte	0x16
	.byte	0x9
	.long	0xd7
	.byte	0x10
	.uleb128 0x1
	.long	.LASF248
	.byte	0x1e
	.byte	0x17
	.byte	0x13
	.long	0x2a
	.byte	0x18
	.uleb128 0x1
	.long	.LASF249
	.byte	0x1e
	.byte	0x18
	.byte	0xb
	.long	0x168
	.byte	0x20
	.uleb128 0x1
	.long	.LASF250
	.byte	0x1e
	.byte	0x19
	.byte	0x9
	.long	0xb6
	.byte	0x28
	.uleb128 0x1
	.long	.LASF251
	.byte	0x1e
	.byte	0x1a
	.byte	0xa
	.long	0x125
	.byte	0x2a
	.byte	0
	.uleb128 0x7
	.long	0x1d6
	.uleb128 0x11
	.byte	0x40
	.byte	0x1e
	.byte	0x11
	.byte	0x3
	.long	0x1206
	.uleb128 0x15
	.long	0x1174
	.uleb128 0xc
	.string	"pad"
	.byte	0x1e
	.byte	0x1c
	.byte	0x7
	.long	0x1206
	.byte	0
	.uleb128 0x12
	.long	0xac
	.long	0x1216
	.uleb128 0x17
	.long	0x2a
	.byte	0x3f
	.byte	0
	.uleb128 0x5
	.long	.LASF252
	.byte	0x20
	.byte	0x20
	.byte	0x29
	.byte	0x11
	.long	0x123e
	.uleb128 0x1
	.long	.LASF253
	.byte	0x20
	.byte	0x2a
	.byte	0x13
	.long	0x5b8
	.byte	0
	.uleb128 0x1
	.long	.LASF254
	.byte	0x20
	.byte	0x2b
	.byte	0xe
	.long	0x690
	.byte	0x8
	.byte	0
	.uleb128 0x2
	.long	.LASF255
	.byte	0x20
	.byte	0x2c
	.byte	0x4
	.long	0x1216
	.uleb128 0x5
	.long	.LASF256
	.byte	0x28
	.byte	0x21
	.byte	0x1a
	.byte	0x11
	.long	0x1272
	.uleb128 0x1
	.long	.LASF257
	.byte	0x21
	.byte	0x1b
	.byte	0x10
	.long	0x38
	.byte	0
	.uleb128 0x1
	.long	.LASF258
	.byte	0x21
	.byte	0x1c
	.byte	0x11
	.long	0x123e
	.byte	0x8
	.byte	0
	.uleb128 0x2
	.long	.LASF259
	.byte	0x21
	.byte	0x1d
	.byte	0x4
	.long	0x124a
	.uleb128 0x2
	.long	.LASF260
	.byte	0x7
	.byte	0x1a
	.byte	0x1b
	.long	0x128a
	.uleb128 0x5
	.long	.LASF261
	.byte	0x38
	.byte	0x22
	.byte	0x9
	.byte	0x11
	.long	0x12e6
	.uleb128 0x1
	.long	.LASF262
	.byte	0x22
	.byte	0xa
	.byte	0x7
	.long	0x73
	.byte	0
	.uleb128 0x1
	.long	.LASF253
	.byte	0x22
	.byte	0xb
	.byte	0xe
	.long	0x623
	.byte	0x8
	.uleb128 0x1
	.long	.LASF263
	.byte	0x22
	.byte	0xd
	.byte	0x7
	.long	0x73
	.byte	0x10
	.uleb128 0x1
	.long	.LASF264
	.byte	0x22
	.byte	0xe
	.byte	0x7
	.long	0x73
	.byte	0x14
	.uleb128 0x1
	.long	.LASF265
	.byte	0x22
	.byte	0xf
	.byte	0xa
	.long	0x1101
	.byte	0x18
	.uleb128 0x3
	.string	"pwd"
	.byte	0x22
	.byte	0xf
	.byte	0x10
	.long	0x1101
	.byte	0x28
	.byte	0
	.uleb128 0x2
	.long	.LASF266
	.byte	0x7
	.byte	0x2e
	.byte	0x1e
	.long	0x12f2
	.uleb128 0x5
	.long	.LASF267
	.byte	0x8
	.byte	0x23
	.byte	0x8
	.byte	0x11
	.long	0x130d
	.uleb128 0x1
	.long	.LASF268
	.byte	0x23
	.byte	0x11
	.byte	0x7
	.long	0xd7
	.byte	0
	.byte	0
	.uleb128 0x2
	.long	.LASF269
	.byte	0x7
	.byte	0x30
	.byte	0x21
	.long	0x1319
	.uleb128 0x5
	.long	.LASF270
	.byte	0x18
	.byte	0x23
	.byte	0x2a
	.byte	0x11
	.long	0x1341
	.uleb128 0x1
	.long	.LASF271
	.byte	0x23
	.byte	0x2b
	.byte	0xa
	.long	0x62f
	.byte	0
	.uleb128 0x1
	.long	.LASF272
	.byte	0x23
	.byte	0x2e
	.byte	0x10
	.long	0x38
	.byte	0x10
	.byte	0
	.uleb128 0x2
	.long	.LASF273
	.byte	0x7
	.byte	0x36
	.byte	0x16
	.long	0x134d
	.uleb128 0x5
	.long	.LASF274
	.byte	0x4
	.byte	0x24
	.byte	0x33
	.byte	0x11
	.long	0x1367
	.uleb128 0x3
	.string	"nr"
	.byte	0x24
	.byte	0x34
	.byte	0x7
	.long	0x73
	.byte	0
	.byte	0
	.uleb128 0x2
	.long	.LASF275
	.byte	0x7
	.byte	0x38
	.byte	0x15
	.long	0x1373
	.uleb128 0x24
	.string	"pid"
	.byte	0x18
	.byte	0x24
	.byte	0x38
	.byte	0x11
	.long	0x13b5
	.uleb128 0x1
	.long	.LASF96
	.byte	0x24
	.byte	0x39
	.byte	0xc
	.long	0x56a
	.byte	0
	.uleb128 0x1
	.long	.LASF276
	.byte	0x24
	.byte	0x3a
	.byte	0x10
	.long	0x38
	.byte	0x4
	.uleb128 0x1
	.long	.LASF253
	.byte	0x24
	.byte	0x3b
	.byte	0xe
	.long	0x623
	.byte	0x8
	.uleb128 0x1
	.long	.LASF277
	.byte	0x24
	.byte	0x42
	.byte	0xa
	.long	0x13bf
	.byte	0x10
	.byte	0
	.uleb128 0x7
	.long	0x10f0
	.uleb128 0x7
	.long	0x10df
	.uleb128 0x12
	.long	0x1341
	.long	0x13cf
	.uleb128 0x17
	.long	0x2a
	.byte	0
	.byte	0
	.uleb128 0x5
	.long	.LASF278
	.byte	0x68
	.byte	0x1f
	.byte	0x68
	.byte	0x11
	.long	0x145f
	.uleb128 0x1
	.long	.LASF279
	.byte	0x1f
	.byte	0x69
	.byte	0x7
	.long	0xc1
	.byte	0
	.uleb128 0x3
	.string	"sp0"
	.byte	0x1f
	.byte	0x6a
	.byte	0x7
	.long	0xd7
	.byte	0x4
	.uleb128 0x3
	.string	"sp1"
	.byte	0x1f
	.byte	0x6b
	.byte	0x7
	.long	0xd7
	.byte	0xc
	.uleb128 0x3
	.string	"sp2"
	.byte	0x1f
	.byte	0x72
	.byte	0x7
	.long	0xd7
	.byte	0x14
	.uleb128 0x1
	.long	.LASF280
	.byte	0x1f
	.byte	0x74
	.byte	0x7
	.long	0xd7
	.byte	0x1c
	.uleb128 0x3
	.string	"ist"
	.byte	0x1f
	.byte	0x75
	.byte	0x7
	.long	0x145f
	.byte	0x24
	.uleb128 0x1
	.long	.LASF281
	.byte	0x1f
	.byte	0x76
	.byte	0x7
	.long	0xc1
	.byte	0x5c
	.uleb128 0x1
	.long	.LASF282
	.byte	0x1f
	.byte	0x77
	.byte	0x7
	.long	0xc1
	.byte	0x60
	.uleb128 0x1
	.long	.LASF283
	.byte	0x1f
	.byte	0x78
	.byte	0x7
	.long	0xb6
	.byte	0x64
	.uleb128 0x1
	.long	.LASF284
	.byte	0x1f
	.byte	0x79
	.byte	0x7
	.long	0xb6
	.byte	0x66
	.byte	0
	.uleb128 0x12
	.long	0xd7
	.long	0x146f
	.uleb128 0x17
	.long	0x2a
	.byte	0x6
	.byte	0
	.uleb128 0x2
	.long	.LASF285
	.byte	0x1f
	.byte	0x7b
	.byte	0x1c
	.long	0x13cf
	.uleb128 0x19
	.long	.LASF286
	.value	0x4020
	.byte	0x1f
	.byte	0x80
	.long	0x14be
	.uleb128 0x1
	.long	.LASF287
	.byte	0x1f
	.byte	0x83
	.byte	0x7
	.long	0xd7
	.byte	0
	.uleb128 0x1
	.long	.LASF288
	.byte	0x1f
	.byte	0x8c
	.byte	0x10
	.long	0x38
	.byte	0x8
	.uleb128 0x1
	.long	.LASF289
	.byte	0x1f
	.byte	0x94
	.byte	0x11
	.long	0x14be
	.byte	0x10
	.uleb128 0x25
	.long	.LASF290
	.byte	0x1f
	.byte	0x9a
	.byte	0x11
	.long	0x14be
	.value	0x2018
	.byte	0
	.uleb128 0x12
	.long	0x2a
	.long	0x14cf
	.uleb128 0x26
	.long	0x2a
	.value	0x400
	.byte	0
	.uleb128 0x2
	.long	.LASF291
	.byte	0x1f
	.byte	0x9b
	.byte	0x4
	.long	0x147b
	.uleb128 0x5
	.long	.LASF292
	.byte	0x38
	.byte	0x1f
	.byte	0xb5
	.byte	0x11
	.long	0x1568
	.uleb128 0x3
	.string	"sp"
	.byte	0x1f
	.byte	0xb8
	.byte	0x9
	.long	0x10d
	.byte	0
	.uleb128 0x3
	.string	"es"
	.byte	0x1f
	.byte	0xb9
	.byte	0x12
	.long	0x6c
	.byte	0x8
	.uleb128 0x3
	.string	"ds"
	.byte	0x1f
	.byte	0xba
	.byte	0x12
	.long	0x6c
	.byte	0xa
	.uleb128 0x1
	.long	.LASF293
	.byte	0x1f
	.byte	0xbb
	.byte	0x12
	.long	0x6c
	.byte	0xc
	.uleb128 0x1
	.long	.LASF294
	.byte	0x1f
	.byte	0xbc
	.byte	0x12
	.long	0x6c
	.byte	0xe
	.uleb128 0x1
	.long	.LASF295
	.byte	0x1f
	.byte	0xbe
	.byte	0x9
	.long	0x10d
	.byte	0x10
	.uleb128 0x1
	.long	.LASF296
	.byte	0x1f
	.byte	0xbf
	.byte	0x9
	.long	0x10d
	.byte	0x18
	.uleb128 0x3
	.string	"cr2"
	.byte	0x1f
	.byte	0xc8
	.byte	0x9
	.long	0x10d
	.byte	0x20
	.uleb128 0x1
	.long	.LASF297
	.byte	0x1f
	.byte	0xc9
	.byte	0x11
	.long	0x2a
	.byte	0x28
	.uleb128 0x1
	.long	.LASF298
	.byte	0x1f
	.byte	0xca
	.byte	0x11
	.long	0x2a
	.byte	0x30
	.byte	0
	.uleb128 0x2
	.long	.LASF299
	.byte	0x1f
	.byte	0xe8
	.byte	0x4
	.long	0x14db
	.uleb128 0x7
	.long	0x1367
	.uleb128 0x7
	.long	0x1272
	.uleb128 0x12
	.long	0x31
	.long	0x158e
	.uleb128 0x17
	.long	0x2a
	.byte	0x1f
	.byte	0
	.uleb128 0x7
	.long	0x127e
	.uleb128 0x7
	.long	0x1c5
	.uleb128 0x7
	.long	0x16a
	.uleb128 0x12
	.long	0x191
	.long	0x15ad
	.uleb128 0x17
	.long	0x2a
	.byte	0xf
	.byte	0
	.uleb128 0x27
	.long	.LASF303
	.byte	0x2
	.byte	0x8
	.byte	0x27
	.quad	.LFB132
	.quad	.LFE132-.LFB132
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x28
	.long	.LASF304
	.byte	0x1
	.byte	0xe
	.byte	0x5
	.long	0x73
	.quad	.LFB131
	.quad	.LFE131-.LFB131
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
	.uleb128 0x5
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
	.uleb128 0x6
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0xd
	.uleb128 0xb
	.uleb128 0x6b
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 8
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
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
	.uleb128 0x9
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
	.uleb128 0xa
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
	.uleb128 0xb
	.uleb128 0xd
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xc
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
	.uleb128 0xd
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
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
	.uleb128 0x10
	.uleb128 0x13
	.byte	0x1
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
	.uleb128 0x11
	.uleb128 0x17
	.byte	0x1
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
	.uleb128 0x12
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
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
	.uleb128 0x14
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0x19
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
	.uleb128 0x17
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 22
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 16
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x18
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
	.uleb128 0x19
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
	.uleb128 0x1a
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
	.uleb128 0x1b
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x1d
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
	.uleb128 0x1e
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
	.uleb128 0x1f
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x20
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
	.uleb128 0x21
	.uleb128 0x4
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
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
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x23
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
	.uleb128 0x24
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
	.uleb128 0x25
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
	.uleb128 0x26
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x27
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
	.uleb128 0x28
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
	.quad	.LFB131
	.quad	.LFE131-.LFB131
	.quad	.LFB132
	.quad	.LFE132-.LFB132
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
	.quad	.LFB131
	.uleb128 .LFE131-.LFB131
	.byte	0x7
	.quad	.LFB132
	.uleb128 .LFE132-.LFB132
	.byte	0
.Ldebug_ranges3:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF276:
	.string	"level"
.LASF24:
	.string	"size_t"
.LASF195:
	.string	"pgoff"
.LASF297:
	.string	"trap_nr"
.LASF202:
	.string	"mm_refcount"
.LASF87:
	.string	"tail"
.LASF192:
	.string	"FAULT_FLAG_INSTRUCTION"
.LASF172:
	.string	"vm_ops"
.LASF251:
	.string	"hardirq_stack_inuse"
.LASF265:
	.string	"root"
.LASF140:
	.string	"PG_locked"
.LASF80:
	.string	"atomic"
.LASF240:
	.string	"path"
.LASF91:
	.string	"list_head"
.LASF114:
	.string	"arch_pte_T"
.LASF9:
	.string	"__u16"
.LASF235:
	.string	"dentry_s"
.LASF53:
	.string	"real_parent"
.LASF52:
	.string	"in_iowait"
.LASF8:
	.string	"__u8"
.LASF85:
	.string	"tspinlock"
.LASF68:
	.string	"comm"
.LASF177:
	.string	"open"
.LASF199:
	.string	"task_size"
.LASF293:
	.string	"fsindex"
.LASF185:
	.string	"FAULT_FLAG_MKWRITE"
.LASF286:
	.string	"x86_io_bitmap"
.LASF139:
	.string	"pgflag_defs"
.LASF204:
	.string	"map_count"
.LASF29:
	.string	"rlimit_s"
.LASF244:
	.string	"current_task"
.LASF104:
	.string	"pmdval_t"
.LASF262:
	.string	"users"
.LASF255:
	.string	"swqueue_hdr_s"
.LASF67:
	.string	"start_boottime"
.LASF138:
	.string	"pgflag_defs_s"
.LASF194:
	.string	"gfp_mask"
.LASF55:
	.string	"children"
.LASF11:
	.string	"__u32"
.LASF18:
	.string	"__kernel_pid_t"
.LASF150:
	.string	"PG_error"
.LASF228:
	.string	"pcp_list"
.LASF294:
	.string	"gsindex"
.LASF44:
	.string	"recent_used_cpu"
.LASF61:
	.string	"utime"
.LASF16:
	.string	"long int"
.LASF214:
	.string	"end_code"
.LASF242:
	.string	"x86_tss"
.LASF223:
	.string	"exe_file"
.LASF190:
	.string	"FAULT_FLAG_USER"
.LASF57:
	.string	"thread_pid"
.LASF175:
	.string	"vm_ops_s"
.LASF291:
	.string	"x86_io_bitmap_s"
.LASF304:
	.string	"main"
.LASF142:
	.string	"PG_referenced"
.LASF20:
	.string	"reg_t"
.LASF115:
	.string	"arch_pte"
.LASF165:
	.string	"vm_area_struct"
.LASF86:
	.string	"head"
.LASF238:
	.string	"vfsmount"
.LASF263:
	.string	"umask"
.LASF231:
	.string	"private"
.LASF118:
	.string	"p4d_t"
.LASF260:
	.string	"taskfs_s"
.LASF5:
	.string	"signed char"
.LASF226:
	.string	"mlock_count"
.LASF28:
	.string	"signal_s"
.LASF111:
	.string	"arch_pud"
.LASF14:
	.string	"__u64"
.LASF74:
	.string	"irq_nr"
.LASF129:
	.string	"vm_fault"
.LASF6:
	.string	"unsigned char"
.LASF168:
	.string	"vm_next"
.LASF130:
	.string	"orig_pte"
.LASF141:
	.string	"PG_writeback"
.LASF32:
	.string	"rlim"
.LASF169:
	.string	"vm_prev"
.LASF154:
	.string	"PG_reserved"
.LASF261:
	.string	"fs_struct"
.LASF259:
	.string	"completion_s"
.LASF189:
	.string	"FAULT_FLAG_TRIED"
.LASF268:
	.string	"vruntime"
.LASF23:
	.string	"_Bool"
.LASF213:
	.string	"start_code"
.LASF224:
	.string	"flag_defs"
.LASF133:
	.string	"mm_s"
.LASF249:
	.string	"hardirq_stack_ptr"
.LASF219:
	.string	"arg_start"
.LASF3:
	.string	"char"
.LASF200:
	.string	"highest_vm_end"
.LASF45:
	.string	"wake_cpu"
.LASF184:
	.string	"FAULT_FLAG_WRITE"
.LASF143:
	.string	"PG_uptodate"
.LASF107:
	.string	"arch_pgd_T"
.LASF119:
	.string	"pgd_union"
.LASF233:
	.string	"_mapcount"
.LASF31:
	.string	"rlimit"
.LASF51:
	.string	"in_execve"
.LASF253:
	.string	"lock"
.LASF56:
	.string	"sibling"
.LASF70:
	.string	"signal"
.LASF173:
	.string	"vm_pgoff"
.LASF243:
	.string	"io_bitmap"
.LASF217:
	.string	"start_brk"
.LASF35:
	.string	"files_struct_s"
.LASF193:
	.string	"FAULT_FLAG_INTERRUPTIBLE"
.LASF58:
	.string	"pid_links"
.LASF42:
	.string	"flags"
.LASF112:
	.string	"arch_pmd_T"
.LASF25:
	.string	"uint"
.LASF128:
	.string	"vm_fault_s"
.LASF17:
	.string	"__kernel_ulong_t"
.LASF205:
	.string	"page_table_lock"
.LASF197:
	.string	"mmap"
.LASF33:
	.string	"rlim_cur"
.LASF99:
	.string	"files_struct"
.LASF117:
	.string	"defs"
.LASF102:
	.string	"file"
.LASF125:
	.string	"pte_t"
.LASF180:
	.string	"mremap"
.LASF88:
	.string	"atomic_long_t"
.LASF26:
	.string	"ulong"
.LASF110:
	.string	"arch_pud_T"
.LASF216:
	.string	"end_data"
.LASF210:
	.string	"exec_vm"
.LASF236:
	.string	"dentry"
.LASF73:
	.string	"pt_regs"
.LASF245:
	.string	"preempt_count"
.LASF203:
	.string	"pgtables_bytes"
.LASF103:
	.string	"pteval_t"
.LASF247:
	.string	"call_depth"
.LASF303:
	.string	"common"
.LASF271:
	.string	"run_list"
.LASF164:
	.string	"vma_s"
.LASF230:
	.string	"index"
.LASF155:
	.string	"PG_private"
.LASF131:
	.string	"cow_page"
.LASF2:
	.string	"long unsigned int"
.LASF124:
	.string	"pmd_union"
.LASF78:
	.string	"status"
.LASF196:
	.string	"address"
.LASF220:
	.string	"arg_end"
.LASF170:
	.string	"vm_mm"
.LASF127:
	.string	"vm_fault_t"
.LASF49:
	.string	"exit_code"
.LASF62:
	.string	"stime"
.LASF152:
	.string	"PG_owner_priv_1"
.LASF215:
	.string	"start_data"
.LASF76:
	.string	"thread_info_s"
.LASF206:
	.string	"total_vm"
.LASF267:
	.string	"sched_entity"
.LASF290:
	.string	"mapall"
.LASF30:
	.string	"signal_struct"
.LASF136:
	.string	"page_s"
.LASF218:
	.string	"start_stack"
.LASF37:
	.string	"task_struct"
.LASF59:
	.string	"vfork_done"
.LASF13:
	.string	"long long int"
.LASF296:
	.string	"gsbase"
.LASF81:
	.string	"counter"
.LASF19:
	.string	"__kernel_size_t"
.LASF38:
	.string	"thread_info"
.LASF222:
	.string	"env_end"
.LASF40:
	.string	"stack"
.LASF54:
	.string	"parent"
.LASF149:
	.string	"PG_workingset"
.LASF288:
	.string	"prev_max"
.LASF98:
	.string	"addr_spc_s"
.LASF77:
	.string	"syscall_work"
.LASF137:
	.string	"_refcount"
.LASF36:
	.string	"task_s"
.LASF258:
	.string	"wait"
.LASF159:
	.string	"PG_swapbacked"
.LASF269:
	.string	"sched_rt_entity_s"
.LASF96:
	.string	"count"
.LASF208:
	.string	"pinned_vm"
.LASF232:
	.string	"compound_head"
.LASF292:
	.string	"thread_struct"
.LASF75:
	.string	"orig_ax"
.LASF83:
	.string	"atomic64"
.LASF198:
	.string	"mmap_base"
.LASF4:
	.string	"unsigned int"
.LASF248:
	.string	"top_of_stack"
.LASF145:
	.string	"PG_lru"
.LASF89:
	.string	"spinlock_t"
.LASF289:
	.string	"bitmap"
.LASF116:
	.string	"pgd_t"
.LASF302:
	.string	"tss_struct"
.LASF148:
	.string	"PG_active"
.LASF179:
	.string	"may_split"
.LASF275:
	.string	"pid_s"
.LASF21:
	.string	"pid_t"
.LASF171:
	.string	"vm_flags"
.LASF234:
	.string	"page_type"
.LASF174:
	.string	"vm_file"
.LASF156:
	.string	"PG_private_2"
.LASF284:
	.string	"io_bitmap_base"
.LASF90:
	.string	"List_s"
.LASF123:
	.string	"pmd_t"
.LASF257:
	.string	"done"
.LASF246:
	.string	"cpu_number"
.LASF301:
	.string	"fault_flag"
.LASF146:
	.string	"PG_head"
.LASF277:
	.string	"numbers"
.LASF132:
	.string	"page"
.LASF157:
	.string	"PG_mappedtodisk"
.LASF211:
	.string	"stack_vm"
.LASF287:
	.string	"prev_sequence"
.LASF22:
	.string	"bool"
.LASF300:
	.ascii	"GNU C17 "
	.string	"11.4.0 -m64 -mcmodel=large -mabi=sysv -mno-red-zone -mtune=generic -march=x86-64 -ggdb -g -fPIC -ffreestanding -fno-stack-protector -fno-unwind-tables -fdata-sections -ffunction-sections -fasynchronous-unwind-tables -fstack-clash-protection -fcf-protection"
.LASF227:
	.string	"buddy_list"
.LASF101:
	.string	"file_s"
.LASF299:
	.string	"thread_s"
.LASF121:
	.string	"pud_t"
.LASF239:
	.string	"path_s"
.LASF221:
	.string	"env_start"
.LASF153:
	.string	"PG_arch_1"
.LASF162:
	.string	"PG_arch_2"
.LASF163:
	.string	"PG_arch_3"
.LASF188:
	.string	"FAULT_FLAG_KILLABLE"
.LASF187:
	.string	"FAULT_FLAG_RETRY_NOWAIT"
.LASF34:
	.string	"rlim_max"
.LASF15:
	.string	"long long unsigned int"
.LASF100:
	.string	"address_space"
.LASF97:
	.string	"List_hdr_s"
.LASF252:
	.string	"swait_queue_head"
.LASF95:
	.string	"anchor"
.LASF209:
	.string	"data_vm"
.LASF158:
	.string	"PG_reclaim"
.LASF147:
	.string	"PG_waiters"
.LASF237:
	.string	"vfsmount_s"
.LASF298:
	.string	"error_code"
.LASF108:
	.string	"arch_pgd"
.LASF84:
	.string	"arch_spinlock_t"
.LASF207:
	.string	"locked_vm"
.LASF270:
	.string	"sched_rt_entity"
.LASF256:
	.string	"completion"
.LASF106:
	.string	"pgdval_t"
.LASF151:
	.string	"PG_slab"
.LASF72:
	.string	"thread"
.LASF274:
	.string	"upid"
.LASF105:
	.string	"pudval_t"
.LASF12:
	.string	"__s64"
.LASF241:
	.string	"pcpu_hot"
.LASF7:
	.string	"short int"
.LASF48:
	.string	"exit_state"
.LASF39:
	.string	"__state"
.LASF41:
	.string	"usage"
.LASF160:
	.string	"PG_unevictable"
.LASF122:
	.string	"pud_union"
.LASF167:
	.string	"vm_end"
.LASF183:
	.string	"page_mkwrite"
.LASF65:
	.string	"nivcsw"
.LASF264:
	.string	"in_exec"
.LASF279:
	.string	"reserved1"
.LASF280:
	.string	"reserved2"
.LASF281:
	.string	"reserved3"
.LASF282:
	.string	"reserved4"
.LASF283:
	.string	"reserved5"
.LASF60:
	.string	"worker_private"
.LASF254:
	.string	"task_list_hdr"
.LASF94:
	.string	"List_hdr"
.LASF182:
	.string	"map_pages"
.LASF46:
	.string	"on_rq"
.LASF27:
	.string	"gfp_t"
.LASF295:
	.string	"fsbase"
.LASF47:
	.string	"active_mm"
.LASF186:
	.string	"FAULT_FLAG_ALLOW_RETRY"
.LASF120:
	.string	"p4d_union"
.LASF285:
	.string	"x86_hw_tss_s"
.LASF50:
	.string	"exit_signal"
.LASF212:
	.string	"def_flags"
.LASF144:
	.string	"PG_dirty"
.LASF191:
	.string	"FAULT_FLAG_REMOTE"
.LASF161:
	.string	"PG_mlocked"
.LASF201:
	.string	"mm_users"
.LASF278:
	.string	"x86_hw_tss"
.LASF64:
	.string	"nvcsw"
.LASF250:
	.string	"softirq_pending"
.LASF225:
	.string	"__filler"
.LASF135:
	.string	"entry_point"
.LASF126:
	.string	"pte_union"
.LASF66:
	.string	"start_time"
.LASF10:
	.string	"short unsigned int"
.LASF79:
	.string	"atomic_t"
.LASF69:
	.string	"files"
.LASF229:
	.string	"mapping"
.LASF181:
	.string	"fault"
.LASF109:
	.string	"PHYADDR"
.LASF113:
	.string	"arch_pmd"
.LASF272:
	.string	"time_slice"
.LASF43:
	.string	"on_cpu"
.LASF134:
	.string	"mm_struct"
.LASF166:
	.string	"vm_start"
.LASF92:
	.string	"next"
.LASF176:
	.string	"vm_operations_struct"
.LASF178:
	.string	"close"
.LASF273:
	.string	"upid_s"
.LASF71:
	.string	"alloc_lock"
.LASF93:
	.string	"prev"
.LASF82:
	.string	"atomic64_t"
.LASF266:
	.string	"sched_entity_s"
.LASF63:
	.string	"gtime"
	.section	.debug_line_str,"MS",@progbits,1
.LASF0:
	.string	"/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c"
.LASF1:
	.string	"/home/cheyh/projs/MyOS2/build/mykernel"
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
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
