	.file	"asm-offsets.c"
	.text
.Ltext0:
	.file 0 "/home/cheyh/projs/MyOS2/build/mykernel" "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c"
	.section	.text.common,"ax",@progbits
	.type	common, @function
common:
.LFB35:
	.file 1 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c"
	.loc 1 6 1
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
.L2:
	leaq	.L2(%rip), %rax
	movabsq	$_GLOBAL_OFFSET_TABLE_-.L2, %r11
	addq	%r11, %rax
	.loc 1 7 2
#APP
# 7 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->"
# 0 "" 2
	.loc 1 8 2
# 8 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->TASK_threadsp $352 offsetof(struct task_struct, thread.sp)"
# 0 "" 2
	.loc 1 13 2
# 13 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->"
# 0 "" 2
	.loc 1 73 2
# 73 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->"
# 0 "" 2
	.loc 1 74 2
# 74 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->PTREGS_SIZE $176 sizeof(struct pt_regs)"
# 0 "" 2
	.loc 1 85 2
# 85 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->TSS_sp0 $4 offsetof(struct tss_struct, x86_tss.sp0)"
# 0 "" 2
	.loc 1 86 2
# 86 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->TSS_sp1 $12 offsetof(struct tss_struct, x86_tss.sp1)"
# 0 "" 2
	.loc 1 87 2
# 87 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->TSS_sp2 $20 offsetof(struct tss_struct, x86_tss.sp2)"
# 0 "" 2
	.loc 1 88 2
# 88 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->X86_top_of_stack $24 offsetof(struct pcpu_hot, top_of_stack)"
# 0 "" 2
	.loc 1 89 2
# 89 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/kbuild/asm-offsets.c" 1
	
.ascii "->X86_current_task $0 offsetof(struct pcpu_hot, current_task)"
# 0 "" 2
	.loc 1 100 1
#NO_APP
	nop
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE35:
	.size	common, .-common
	.text
.Letext0:
	.file 2 "/home/cheyh/projs/MyOS2/mykernel/sched/task/../sched_type_declaration.h"
	.file 3 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/sched/sched_type_declaration_arch.h"
	.file 4 "/home/cheyh/projs/MyOS2/mykernel/sched/task/task_types.h"
	.file 5 "/home/cheyh/projs/MyOS2/mykernel/include/uapi/asm-generic/int-ll64.h"
	.file 6 "/home/cheyh/projs/MyOS2/mykernel/include/linux/kernel/asm-generic/int-ll64.h"
	.file 7 "/home/cheyh/projs/MyOS2/mykernel/include/uapi/asm-generic/posix_types.h"
	.file 8 "/home/cheyh/projs/MyOS2/mykernel/include/linux/kernel/types.h"
	.file 9 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/sched/context/thread_info_types_arch.h"
	.file 10 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/lock_IPC/atomic/../lock_ipc_type_declaration_arch.h"
	.file 11 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/lock_IPC/atomic/atomic_types_arch.h"
	.file 12 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_types_arch.h"
	.file 13 "/home/cheyh/projs/MyOS2/mykernel/lock_IPC/lock_ipc_type_declaration.h"
	.file 14 "/home/cheyh/projs/MyOS2/mykernel/lib/lib_type_declaration.h"
	.file 15 "/home/cheyh/projs/MyOS2/mykernel/lib/list/double_list_types.h"
	.file 16 "/home/cheyh/projs/MyOS2/mykernel/mm/mm_type_declaration.h"
	.file 17 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/mm/mm_type_declaration_arch.h"
	.file 18 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/mm/pgtable/pgtable_types_arch.h"
	.file 19 "/home/cheyh/projs/MyOS2/mykernel/mm/fault/fault_types.h"
	.file 20 "/home/cheyh/projs/MyOS2/mykernel/mm/misc/mm_misc_types.h"
	.file 21 "/home/cheyh/projs/MyOS2/mykernel/mm/page_alloc/buddy_types.h"
	.file 22 "/home/cheyh/projs/MyOS2/mykernel/mm/page_alloc/page-flags_types.h"
	.file 23 "/home/cheyh/projs/MyOS2/mykernel/mm/vm_map/vm_map_types.h"
	.file 24 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/include/asm/current.h"
	.file 25 "/home/cheyh/projs/MyOS2/mykernel/arch/x86_64/processor/processor_types_arch.h"
	.file 26 "/home/cheyh/projs/MyOS2/mykernel/include/linux/kernel/swait.h"
	.file 27 "/home/cheyh/projs/MyOS2/mykernel/include/linux/kernel/completion.h"
	.file 28 "/home/cheyh/projs/MyOS2/mykernel/sched/task/../scheduler/scheduler_types.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x1321
	.value	0x5
	.byte	0x1
	.byte	0x8
	.long	.Ldebug_abbrev0
	.uleb128 0x1d
	.long	.LASF278
	.byte	0x1d
	.long	.LASF0
	.long	.LASF1
	.long	.LLRL0
	.quad	0
	.long	.Ldebug_line0
	.uleb128 0x2
	.long	.LASF2
	.byte	0x2
	.byte	0x6
	.byte	0x1f
	.long	0x36
	.uleb128 0x12
	.long	.LASF4
	.uleb128 0x2
	.long	.LASF3
	.byte	0x2
	.byte	0xa
	.byte	0x1e
	.long	0x47
	.uleb128 0x12
	.long	.LASF5
	.uleb128 0x2
	.long	.LASF6
	.byte	0x3
	.byte	0x5
	.byte	0x1d
	.long	0x58
	.uleb128 0x19
	.long	.LASF69
	.value	0x198
	.byte	0x4
	.byte	0x14
	.long	0x273
	.uleb128 0x1
	.long	.LASF7
	.byte	0x4
	.byte	0x19
	.byte	0x11
	.long	0x3b3
	.byte	0
	.uleb128 0x1
	.long	.LASF8
	.byte	0x4
	.byte	0x1a
	.byte	0x10
	.long	0x2ba
	.byte	0x18
	.uleb128 0x1
	.long	.LASF9
	.byte	0x4
	.byte	0x27
	.byte	0xb
	.long	0x3b1
	.byte	0x20
	.uleb128 0x1
	.long	.LASF10
	.byte	0x4
	.byte	0x28
	.byte	0xd
	.long	0x401
	.byte	0x28
	.uleb128 0x1
	.long	.LASF11
	.byte	0x4
	.byte	0x2a
	.byte	0x10
	.long	0x2ba
	.byte	0x2c
	.uleb128 0x1
	.long	.LASF12
	.byte	0x4
	.byte	0x2d
	.byte	0xa
	.long	0x2a7
	.byte	0x30
	.uleb128 0x1
	.long	.LASF13
	.byte	0x4
	.byte	0x3a
	.byte	0xa
	.long	0x2a7
	.byte	0x34
	.uleb128 0x1
	.long	.LASF14
	.byte	0x4
	.byte	0x3b
	.byte	0xa
	.long	0x2a7
	.byte	0x38
	.uleb128 0x1
	.long	.LASF15
	.byte	0x4
	.byte	0x3c
	.byte	0xa
	.long	0x2a7
	.byte	0x3c
	.uleb128 0x7
	.string	"se"
	.byte	0x4
	.byte	0x43
	.byte	0x13
	.long	0x10d0
	.byte	0x40
	.uleb128 0x7
	.string	"rt"
	.byte	0x4
	.byte	0x44
	.byte	0x15
	.long	0x10f7
	.byte	0x48
	.uleb128 0x7
	.string	"mm"
	.byte	0x4
	.byte	0x90
	.byte	0xb
	.long	0xef8
	.byte	0x60
	.uleb128 0x1
	.long	.LASF16
	.byte	0x4
	.byte	0x91
	.byte	0xb
	.long	0xef8
	.byte	0x68
	.uleb128 0x1
	.long	.LASF17
	.byte	0x4
	.byte	0x99
	.byte	0xa
	.long	0x2a7
	.byte	0x70
	.uleb128 0x1
	.long	.LASF18
	.byte	0x4
	.byte	0x9a
	.byte	0xa
	.long	0x2a7
	.byte	0x74
	.uleb128 0x1
	.long	.LASF19
	.byte	0x4
	.byte	0x9b
	.byte	0xa
	.long	0x2a7
	.byte	0x78
	.uleb128 0x1a
	.long	.LASF20
	.byte	0xc1
	.long	0x2ba
	.value	0x3e0
	.uleb128 0x1a
	.long	.LASF21
	.byte	0xc2
	.long	0x2ba
	.value	0x3e1
	.uleb128 0x7
	.string	"pid"
	.byte	0x4
	.byte	0xe0
	.byte	0xb
	.long	0x362
	.byte	0x80
	.uleb128 0x1
	.long	.LASF22
	.byte	0x4
	.byte	0xee
	.byte	0x11
	.long	0x1027
	.byte	0x88
	.uleb128 0x1
	.long	.LASF23
	.byte	0x4
	.byte	0xf0
	.byte	0x11
	.long	0x1027
	.byte	0x90
	.uleb128 0x1
	.long	.LASF24
	.byte	0x4
	.byte	0xf5
	.byte	0xf
	.long	0x527
	.byte	0x98
	.uleb128 0x1
	.long	.LASF25
	.byte	0x4
	.byte	0xf6
	.byte	0xc
	.long	0x4c6
	.byte	0xb0
	.uleb128 0xf
	.long	.LASF26
	.value	0x103
	.byte	0xc
	.long	0x12e1
	.byte	0xc0
	.uleb128 0xf
	.long	.LASF27
	.value	0x105
	.byte	0xc
	.long	0x4c6
	.byte	0xc8
	.uleb128 0xf
	.long	.LASF28
	.value	0x109
	.byte	0x11
	.long	0x12e6
	.byte	0xd8
	.uleb128 0xf
	.long	.LASF29
	.value	0x112
	.byte	0xb
	.long	0x3b1
	.byte	0xe0
	.uleb128 0xf
	.long	.LASF30
	.value	0x114
	.byte	0xa
	.long	0x312
	.byte	0xe8
	.uleb128 0xf
	.long	.LASF31
	.value	0x115
	.byte	0xa
	.long	0x312
	.byte	0xf0
	.uleb128 0xf
	.long	.LASF32
	.value	0x11a
	.byte	0xa
	.long	0x312
	.byte	0xf8
	.uleb128 0xa
	.long	.LASF33
	.value	0x124
	.byte	0x11
	.long	0x31d
	.value	0x100
	.uleb128 0xa
	.long	.LASF34
	.value	0x125
	.byte	0x11
	.long	0x31d
	.value	0x108
	.uleb128 0xa
	.long	.LASF35
	.value	0x128
	.byte	0xa
	.long	0x312
	.value	0x110
	.uleb128 0xa
	.long	.LASF36
	.value	0x12b
	.byte	0xa
	.long	0x312
	.value	0x118
	.uleb128 0xa
	.long	.LASF37
	.value	0x14f
	.byte	0xa
	.long	0x12eb
	.value	0x120
	.uleb128 0x1e
	.string	"fs"
	.byte	0x4
	.value	0x15c
	.byte	0xe
	.long	0x12fb
	.value	0x140
	.uleb128 0xa
	.long	.LASF38
	.value	0x15f
	.byte	0x13
	.long	0x1300
	.value	0x148
	.uleb128 0xa
	.long	.LASF39
	.value	0x169
	.byte	0xd
	.long	0x1305
	.value	0x150
	.uleb128 0xa
	.long	.LASF40
	.value	0x185
	.byte	0xf
	.long	0x4ba
	.value	0x158
	.uleb128 0xa
	.long	.LASF41
	.value	0x2f7
	.byte	0xd
	.long	0x12d5
	.value	0x160
	.byte	0
	.uleb128 0x8
	.byte	0x1
	.byte	0x6
	.long	.LASF43
	.uleb128 0x2
	.long	.LASF42
	.byte	0x5
	.byte	0x15
	.byte	0x19
	.long	0x286
	.uleb128 0x8
	.byte	0x1
	.byte	0x8
	.long	.LASF44
	.uleb128 0x8
	.byte	0x2
	.byte	0x5
	.long	.LASF45
	.uleb128 0x2
	.long	.LASF46
	.byte	0x5
	.byte	0x18
	.byte	0x1a
	.long	0x2a0
	.uleb128 0x8
	.byte	0x2
	.byte	0x7
	.long	.LASF47
	.uleb128 0x1f
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.long	.LASF48
	.byte	0x5
	.byte	0x1b
	.byte	0x18
	.long	0x2ba
	.uleb128 0x8
	.byte	0x4
	.byte	0x7
	.long	.LASF49
	.uleb128 0x2
	.long	.LASF50
	.byte	0x5
	.byte	0x1e
	.byte	0x1d
	.long	0x2cd
	.uleb128 0x8
	.byte	0x8
	.byte	0x5
	.long	.LASF51
	.uleb128 0x2
	.long	.LASF52
	.byte	0x5
	.byte	0x1f
	.byte	0x1f
	.long	0x2e0
	.uleb128 0x8
	.byte	0x8
	.byte	0x7
	.long	.LASF53
	.uleb128 0x13
	.string	"u8"
	.byte	0xf
	.byte	0x10
	.long	0x27a
	.uleb128 0x13
	.string	"u16"
	.byte	0x11
	.byte	0x11
	.long	0x294
	.uleb128 0x13
	.string	"u32"
	.byte	0x13
	.byte	0x11
	.long	0x2ae
	.uleb128 0x13
	.string	"s64"
	.byte	0x14
	.byte	0x11
	.long	0x2c1
	.uleb128 0x13
	.string	"u64"
	.byte	0x15
	.byte	0x11
	.long	0x2d4
	.uleb128 0x8
	.byte	0x8
	.byte	0x7
	.long	.LASF54
	.uleb128 0x8
	.byte	0x1
	.byte	0x6
	.long	.LASF55
	.uleb128 0x8
	.byte	0x8
	.byte	0x5
	.long	.LASF56
	.uleb128 0x2
	.long	.LASF57
	.byte	0x7
	.byte	0x11
	.byte	0x19
	.long	0x31d
	.uleb128 0x2
	.long	.LASF58
	.byte	0x7
	.byte	0x1d
	.byte	0x12
	.long	0x2a7
	.uleb128 0x2
	.long	.LASF59
	.byte	0x7
	.byte	0x49
	.byte	0x1d
	.long	0x332
	.uleb128 0x2
	.long	.LASF60
	.byte	0x8
	.byte	0x10
	.byte	0x1b
	.long	0x31d
	.uleb128 0x2
	.long	.LASF61
	.byte	0x8
	.byte	0x26
	.byte	0x1b
	.long	0x33e
	.uleb128 0x2
	.long	.LASF62
	.byte	0x8
	.byte	0x2e
	.byte	0x14
	.long	0x37a
	.uleb128 0x8
	.byte	0x1
	.byte	0x2
	.long	.LASF63
	.uleb128 0x2
	.long	.LASF64
	.byte	0x8
	.byte	0x47
	.byte	0x1d
	.long	0x34a
	.uleb128 0x2
	.long	.LASF65
	.byte	0x8
	.byte	0x68
	.byte	0x19
	.long	0x2ba
	.uleb128 0x2
	.long	.LASF66
	.byte	0x8
	.byte	0x69
	.byte	0x1a
	.long	0x31d
	.uleb128 0x2
	.long	.LASF67
	.byte	0x8
	.byte	0xa6
	.byte	0x22
	.long	0x2ba
	.uleb128 0x20
	.byte	0x8
	.uleb128 0x2
	.long	.LASF68
	.byte	0x3
	.byte	0x13
	.byte	0x1d
	.long	0x3bf
	.uleb128 0x6
	.long	.LASF7
	.byte	0x18
	.byte	0x9
	.byte	0x13
	.byte	0x11
	.long	0x401
	.uleb128 0x1
	.long	.LASF11
	.byte	0x9
	.byte	0x14
	.byte	0x9
	.long	0x399
	.byte	0
	.uleb128 0x1
	.long	.LASF70
	.byte	0x9
	.byte	0x15
	.byte	0x9
	.long	0x399
	.byte	0x8
	.uleb128 0x1
	.long	.LASF71
	.byte	0x9
	.byte	0x16
	.byte	0x8
	.long	0x2fc
	.byte	0x10
	.uleb128 0x7
	.string	"cpu"
	.byte	0x9
	.byte	0x17
	.byte	0x8
	.long	0x2fc
	.byte	0x14
	.byte	0
	.uleb128 0x2
	.long	.LASF72
	.byte	0xa
	.byte	0x9
	.byte	0x18
	.long	0x40d
	.uleb128 0x6
	.long	.LASF73
	.byte	0x4
	.byte	0xb
	.byte	0x7
	.byte	0x11
	.long	0x428
	.uleb128 0x1
	.long	.LASF74
	.byte	0xb
	.byte	0x8
	.byte	0x7
	.long	0x2a7
	.byte	0
	.byte	0
	.uleb128 0x2
	.long	.LASF75
	.byte	0xa
	.byte	0xb
	.byte	0x1a
	.long	0x434
	.uleb128 0x6
	.long	.LASF76
	.byte	0x8
	.byte	0xb
	.byte	0xb
	.byte	0x11
	.long	0x44f
	.uleb128 0x1
	.long	.LASF74
	.byte	0xb
	.byte	0xc
	.byte	0x7
	.long	0x307
	.byte	0
	.byte	0
	.uleb128 0x2
	.long	.LASF77
	.byte	0xa
	.byte	0xf
	.byte	0x1b
	.long	0x45b
	.uleb128 0x6
	.long	.LASF78
	.byte	0x8
	.byte	0xc
	.byte	0x8
	.byte	0x11
	.long	0x46f
	.uleb128 0xb
	.long	0x493
	.byte	0
	.byte	0
	.uleb128 0x10
	.byte	0x8
	.byte	0xc
	.byte	0xc
	.byte	0x4
	.long	0x493
	.uleb128 0x1
	.long	.LASF79
	.byte	0xc
	.byte	0xd
	.byte	0xa
	.long	0x2fc
	.byte	0
	.uleb128 0x1
	.long	.LASF80
	.byte	0xc
	.byte	0xe
	.byte	0xa
	.long	0x2fc
	.byte	0x4
	.byte	0
	.uleb128 0x11
	.byte	0x8
	.byte	0xc
	.byte	0x9
	.byte	0x3
	.long	0x4ae
	.uleb128 0xc
	.string	"val"
	.byte	0xc
	.byte	0xa
	.byte	0xf
	.long	0x428
	.uleb128 0x14
	.long	0x46f
	.byte	0
	.uleb128 0x2
	.long	.LASF81
	.byte	0xd
	.byte	0x9
	.byte	0x15
	.long	0x428
	.uleb128 0x2
	.long	.LASF82
	.byte	0xd
	.byte	0xd
	.byte	0x1a
	.long	0x44f
	.uleb128 0x2
	.long	.LASF83
	.byte	0xe
	.byte	0xb
	.byte	0x1b
	.long	0x4d2
	.uleb128 0x6
	.long	.LASF84
	.byte	0x10
	.byte	0xf
	.byte	0x8
	.byte	0x11
	.long	0x4fa
	.uleb128 0x1
	.long	.LASF85
	.byte	0xf
	.byte	0x9
	.byte	0xc
	.long	0x4fa
	.byte	0
	.uleb128 0x1
	.long	.LASF86
	.byte	0xf
	.byte	0xa
	.byte	0xc
	.long	0x4fa
	.byte	0x8
	.byte	0
	.uleb128 0x5
	.long	0x4c6
	.uleb128 0x6
	.long	.LASF87
	.byte	0x18
	.byte	0xf
	.byte	0xd
	.byte	0x11
	.long	0x527
	.uleb128 0x1
	.long	.LASF88
	.byte	0xf
	.byte	0xe
	.byte	0xb
	.long	0x4c6
	.byte	0
	.uleb128 0x1
	.long	.LASF89
	.byte	0xf
	.byte	0xf
	.byte	0xa
	.long	0x399
	.byte	0x10
	.byte	0
	.uleb128 0x2
	.long	.LASF90
	.byte	0xf
	.byte	0x10
	.byte	0x4
	.long	0x4ff
	.uleb128 0x2
	.long	.LASF91
	.byte	0x10
	.byte	0x5
	.byte	0x1f
	.long	0x53f
	.uleb128 0x12
	.long	.LASF92
	.uleb128 0x2
	.long	.LASF93
	.byte	0x10
	.byte	0x7
	.byte	0x16
	.long	0x550
	.uleb128 0x12
	.long	.LASF94
	.uleb128 0x2
	.long	.LASF95
	.byte	0x11
	.byte	0x15
	.byte	0x10
	.long	0x399
	.uleb128 0x2
	.long	.LASF96
	.byte	0x11
	.byte	0x16
	.byte	0x10
	.long	0x399
	.uleb128 0x2
	.long	.LASF97
	.byte	0x11
	.byte	0x17
	.byte	0x10
	.long	0x399
	.uleb128 0x2
	.long	.LASF98
	.byte	0x11
	.byte	0x19
	.byte	0x10
	.long	0x399
	.uleb128 0x2
	.long	.LASF99
	.byte	0x11
	.byte	0x1c
	.byte	0x1a
	.long	0x591
	.uleb128 0x6
	.long	.LASF100
	.byte	0x8
	.byte	0x12
	.byte	0x8
	.byte	0x11
	.long	0x5f2
	.uleb128 0x3
	.string	"P"
	.byte	0xa
	.long	0x31d
	.byte	0
	.uleb128 0x3
	.string	"RW"
	.byte	0xb
	.long	0x31d
	.byte	0x1
	.uleb128 0x3
	.string	"US"
	.byte	0xc
	.long	0x31d
	.byte	0x2
	.uleb128 0x3
	.string	"PWT"
	.byte	0xd
	.long	0x31d
	.byte	0x3
	.uleb128 0x3
	.string	"PCD"
	.byte	0xe
	.long	0x31d
	.byte	0x4
	.uleb128 0x3
	.string	"A"
	.byte	0xf
	.long	0x31d
	.byte	0x5
	.uleb128 0x4
	.long	.LASF101
	.byte	0x12
	.byte	0x11
	.long	0x31d
	.byte	0x24
	.byte	0xc
	.uleb128 0x3
	.string	"XD"
	.byte	0x13
	.long	0x31d
	.byte	0x3f
	.byte	0
	.uleb128 0x2
	.long	.LASF102
	.byte	0x11
	.byte	0x1e
	.byte	0x1a
	.long	0x5fe
	.uleb128 0x6
	.long	.LASF103
	.byte	0x8
	.byte	0x12
	.byte	0x16
	.byte	0x11
	.long	0x669
	.uleb128 0x3
	.string	"P"
	.byte	0x18
	.long	0x31d
	.byte	0
	.uleb128 0x3
	.string	"RW"
	.byte	0x19
	.long	0x31d
	.byte	0x1
	.uleb128 0x3
	.string	"US"
	.byte	0x1a
	.long	0x31d
	.byte	0x2
	.uleb128 0x3
	.string	"PWT"
	.byte	0x1b
	.long	0x31d
	.byte	0x3
	.uleb128 0x3
	.string	"PCD"
	.byte	0x1c
	.long	0x31d
	.byte	0x4
	.uleb128 0x3
	.string	"A"
	.byte	0x1d
	.long	0x31d
	.byte	0x5
	.uleb128 0x3
	.string	"PS"
	.byte	0x1f
	.long	0x31d
	.byte	0x7
	.uleb128 0x4
	.long	.LASF101
	.byte	0x12
	.byte	0x22
	.long	0x31d
	.byte	0x24
	.byte	0xc
	.uleb128 0x3
	.string	"XD"
	.byte	0x24
	.long	0x31d
	.byte	0x3f
	.byte	0
	.uleb128 0x2
	.long	.LASF104
	.byte	0x11
	.byte	0x20
	.byte	0x1a
	.long	0x675
	.uleb128 0x6
	.long	.LASF105
	.byte	0x8
	.byte	0x12
	.byte	0x27
	.byte	0x11
	.long	0x6e0
	.uleb128 0x3
	.string	"P"
	.byte	0x29
	.long	0x31d
	.byte	0
	.uleb128 0x3
	.string	"RW"
	.byte	0x2a
	.long	0x31d
	.byte	0x1
	.uleb128 0x3
	.string	"US"
	.byte	0x2b
	.long	0x31d
	.byte	0x2
	.uleb128 0x3
	.string	"PWT"
	.byte	0x2c
	.long	0x31d
	.byte	0x3
	.uleb128 0x3
	.string	"PCD"
	.byte	0x2d
	.long	0x31d
	.byte	0x4
	.uleb128 0x3
	.string	"A"
	.byte	0x2e
	.long	0x31d
	.byte	0x5
	.uleb128 0x3
	.string	"PS"
	.byte	0x30
	.long	0x31d
	.byte	0x7
	.uleb128 0x4
	.long	.LASF101
	.byte	0x12
	.byte	0x33
	.long	0x31d
	.byte	0x24
	.byte	0xc
	.uleb128 0x3
	.string	"XD"
	.byte	0x35
	.long	0x31d
	.byte	0x3f
	.byte	0
	.uleb128 0x2
	.long	.LASF106
	.byte	0x11
	.byte	0x22
	.byte	0x1a
	.long	0x6ec
	.uleb128 0x6
	.long	.LASF107
	.byte	0x8
	.byte	0x12
	.byte	0x38
	.byte	0x11
	.long	0x76a
	.uleb128 0x3
	.string	"P"
	.byte	0x3a
	.long	0x31d
	.byte	0
	.uleb128 0x3
	.string	"RW"
	.byte	0x3b
	.long	0x31d
	.byte	0x1
	.uleb128 0x3
	.string	"US"
	.byte	0x3c
	.long	0x31d
	.byte	0x2
	.uleb128 0x3
	.string	"PWT"
	.byte	0x3d
	.long	0x31d
	.byte	0x3
	.uleb128 0x3
	.string	"PCD"
	.byte	0x3e
	.long	0x31d
	.byte	0x4
	.uleb128 0x3
	.string	"A"
	.byte	0x3f
	.long	0x31d
	.byte	0x5
	.uleb128 0x3
	.string	"D"
	.byte	0x40
	.long	0x31d
	.byte	0x6
	.uleb128 0x3
	.string	"PAT"
	.byte	0x41
	.long	0x31d
	.byte	0x7
	.uleb128 0x3
	.string	"G"
	.byte	0x42
	.long	0x31d
	.byte	0x8
	.uleb128 0x4
	.long	.LASF101
	.byte	0x12
	.byte	0x44
	.long	0x31d
	.byte	0x24
	.byte	0xc
	.uleb128 0x3
	.string	"XD"
	.byte	0x46
	.long	0x31d
	.byte	0x3f
	.byte	0
	.uleb128 0x2
	.long	.LASF108
	.byte	0x11
	.byte	0x24
	.byte	0x1a
	.long	0x776
	.uleb128 0x15
	.long	.LASF111
	.byte	0x49
	.long	0x799
	.uleb128 0xc
	.string	"val"
	.byte	0x12
	.byte	0x4a
	.byte	0xc
	.long	0x579
	.uleb128 0x9
	.long	.LASF109
	.byte	0x12
	.byte	0x4b
	.byte	0xe
	.long	0x585
	.byte	0
	.uleb128 0x2
	.long	.LASF110
	.byte	0x11
	.byte	0x26
	.byte	0x1a
	.long	0x7a5
	.uleb128 0x15
	.long	.LASF112
	.byte	0x4e
	.long	0x7c8
	.uleb128 0xc
	.string	"pgd"
	.byte	0x12
	.byte	0x4f
	.byte	0xa
	.long	0x76a
	.uleb128 0x9
	.long	.LASF109
	.byte	0x12
	.byte	0x50
	.byte	0xe
	.long	0x585
	.byte	0
	.uleb128 0x2
	.long	.LASF113
	.byte	0x11
	.byte	0x28
	.byte	0x1a
	.long	0x7d4
	.uleb128 0x15
	.long	.LASF114
	.byte	0x53
	.long	0x7f7
	.uleb128 0xc
	.string	"val"
	.byte	0x12
	.byte	0x54
	.byte	0xc
	.long	0x56d
	.uleb128 0x9
	.long	.LASF109
	.byte	0x12
	.byte	0x55
	.byte	0xe
	.long	0x5f2
	.byte	0
	.uleb128 0x2
	.long	.LASF115
	.byte	0x11
	.byte	0x2a
	.byte	0x1a
	.long	0x803
	.uleb128 0x15
	.long	.LASF116
	.byte	0x58
	.long	0x826
	.uleb128 0xc
	.string	"val"
	.byte	0x12
	.byte	0x59
	.byte	0xc
	.long	0x561
	.uleb128 0x9
	.long	.LASF109
	.byte	0x12
	.byte	0x5a
	.byte	0xe
	.long	0x669
	.byte	0
	.uleb128 0x2
	.long	.LASF117
	.byte	0x11
	.byte	0x2c
	.byte	0x1a
	.long	0x832
	.uleb128 0x15
	.long	.LASF118
	.byte	0x5d
	.long	0x855
	.uleb128 0xc
	.string	"val"
	.byte	0x12
	.byte	0x5e
	.byte	0xc
	.long	0x555
	.uleb128 0x9
	.long	.LASF109
	.byte	0x12
	.byte	0x5f
	.byte	0xe
	.long	0x6e0
	.byte	0
	.uleb128 0x2
	.long	.LASF119
	.byte	0x10
	.byte	0x22
	.byte	0x19
	.long	0x38d
	.uleb128 0x2
	.long	.LASF120
	.byte	0x10
	.byte	0x24
	.byte	0x1a
	.long	0x86d
	.uleb128 0x6
	.long	.LASF121
	.byte	0x60
	.byte	0x13
	.byte	0x68
	.byte	0x11
	.long	0x8e9
	.uleb128 0xb
	.long	0xc39
	.byte	0
	.uleb128 0x1
	.long	.LASF11
	.byte	0x13
	.byte	0x6f
	.byte	0x13
	.long	0xbaa
	.byte	0x20
	.uleb128 0x7
	.string	"p4d"
	.byte	0x13
	.byte	0x72
	.byte	0xc
	.long	0xc43
	.byte	0x28
	.uleb128 0x7
	.string	"pud"
	.byte	0x13
	.byte	0x73
	.byte	0xc
	.long	0xc48
	.byte	0x30
	.uleb128 0x7
	.string	"pmd"
	.byte	0x13
	.byte	0x76
	.byte	0xc
	.long	0xc4d
	.byte	0x38
	.uleb128 0x7
	.string	"pte"
	.byte	0x13
	.byte	0x78
	.byte	0xc
	.long	0xc52
	.byte	0x40
	.uleb128 0x1
	.long	.LASF122
	.byte	0x13
	.byte	0x7d
	.byte	0xb
	.long	0x826
	.byte	0x48
	.uleb128 0x1
	.long	.LASF123
	.byte	0x13
	.byte	0x83
	.byte	0xd
	.long	0xc57
	.byte	0x50
	.uleb128 0x1
	.long	.LASF124
	.byte	0x13
	.byte	0x84
	.byte	0xd
	.long	0xc57
	.byte	0x58
	.byte	0
	.uleb128 0x2
	.long	.LASF125
	.byte	0x10
	.byte	0x32
	.byte	0x1b
	.long	0x8f5
	.uleb128 0x6
	.long	.LASF126
	.byte	0xf0
	.byte	0x14
	.byte	0x7
	.byte	0x11
	.long	0x916
	.uleb128 0x1
	.long	.LASF127
	.byte	0x14
	.byte	0x8
	.byte	0xd
	.long	0x381
	.byte	0
	.uleb128 0xb
	.long	0xc5c
	.byte	0x8
	.byte	0
	.uleb128 0x2
	.long	.LASF128
	.byte	0x10
	.byte	0x3a
	.byte	0x16
	.long	0x922
	.uleb128 0x6
	.long	.LASF124
	.byte	0x38
	.byte	0x15
	.byte	0x2d
	.byte	0x11
	.long	0x94f
	.uleb128 0xb
	.long	0xdf6
	.byte	0
	.uleb128 0xb
	.long	0xec2
	.byte	0x8
	.uleb128 0xb
	.long	0xed6
	.byte	0x30
	.uleb128 0x1
	.long	.LASF129
	.byte	0x15
	.byte	0x97
	.byte	0xd
	.long	0x401
	.byte	0x34
	.byte	0
	.uleb128 0x2
	.long	.LASF130
	.byte	0x10
	.byte	0x3e
	.byte	0x1d
	.long	0x95b
	.uleb128 0x6
	.long	.LASF131
	.byte	0x3
	.byte	0x16
	.byte	0xb4
	.byte	0x11
	.long	0xaa1
	.uleb128 0x4
	.long	.LASF132
	.byte	0x16
	.byte	0xb6
	.long	0x31d
	.byte	0x1
	.byte	0
	.uleb128 0x4
	.long	.LASF133
	.byte	0x16
	.byte	0xb7
	.long	0x31d
	.byte	0x1
	.byte	0x1
	.uleb128 0x4
	.long	.LASF134
	.byte	0x16
	.byte	0xb8
	.long	0x31d
	.byte	0x1
	.byte	0x2
	.uleb128 0x4
	.long	.LASF135
	.byte	0x16
	.byte	0xb9
	.long	0x31d
	.byte	0x1
	.byte	0x3
	.uleb128 0x4
	.long	.LASF136
	.byte	0x16
	.byte	0xba
	.long	0x31d
	.byte	0x1
	.byte	0x4
	.uleb128 0x4
	.long	.LASF137
	.byte	0x16
	.byte	0xbb
	.long	0x31d
	.byte	0x1
	.byte	0x5
	.uleb128 0x4
	.long	.LASF138
	.byte	0x16
	.byte	0xbc
	.long	0x31d
	.byte	0x1
	.byte	0x6
	.uleb128 0x4
	.long	.LASF139
	.byte	0x16
	.byte	0xbd
	.long	0x31d
	.byte	0x1
	.byte	0x7
	.uleb128 0x4
	.long	.LASF140
	.byte	0x16
	.byte	0xbe
	.long	0x31d
	.byte	0x1
	.byte	0x8
	.uleb128 0x4
	.long	.LASF141
	.byte	0x16
	.byte	0xbf
	.long	0x31d
	.byte	0x1
	.byte	0x9
	.uleb128 0x4
	.long	.LASF142
	.byte	0x16
	.byte	0xc0
	.long	0x31d
	.byte	0x1
	.byte	0xa
	.uleb128 0x4
	.long	.LASF143
	.byte	0x16
	.byte	0xc1
	.long	0x31d
	.byte	0x1
	.byte	0xb
	.uleb128 0x4
	.long	.LASF144
	.byte	0x16
	.byte	0xc2
	.long	0x31d
	.byte	0x1
	.byte	0xc
	.uleb128 0x4
	.long	.LASF145
	.byte	0x16
	.byte	0xc3
	.long	0x31d
	.byte	0x1
	.byte	0xd
	.uleb128 0x4
	.long	.LASF146
	.byte	0x16
	.byte	0xc4
	.long	0x31d
	.byte	0x1
	.byte	0xe
	.uleb128 0x4
	.long	.LASF147
	.byte	0x16
	.byte	0xc5
	.long	0x31d
	.byte	0x1
	.byte	0xf
	.uleb128 0x4
	.long	.LASF148
	.byte	0x16
	.byte	0xc6
	.long	0x31d
	.byte	0x1
	.byte	0x10
	.uleb128 0x4
	.long	.LASF149
	.byte	0x16
	.byte	0xc7
	.long	0x31d
	.byte	0x1
	.byte	0x11
	.uleb128 0x4
	.long	.LASF150
	.byte	0x16
	.byte	0xc8
	.long	0x31d
	.byte	0x1
	.byte	0x12
	.uleb128 0x4
	.long	.LASF151
	.byte	0x16
	.byte	0xc9
	.long	0x31d
	.byte	0x1
	.byte	0x13
	.uleb128 0x4
	.long	.LASF152
	.byte	0x16
	.byte	0xca
	.long	0x31d
	.byte	0x1
	.byte	0x14
	.uleb128 0x4
	.long	.LASF153
	.byte	0x16
	.byte	0xcb
	.long	0x31d
	.byte	0x1
	.byte	0x15
	.uleb128 0x4
	.long	.LASF154
	.byte	0x16
	.byte	0xd6
	.long	0x31d
	.byte	0x1
	.byte	0x16
	.uleb128 0x4
	.long	.LASF155
	.byte	0x16
	.byte	0xd7
	.long	0x31d
	.byte	0x1
	.byte	0x17
	.byte	0
	.uleb128 0x2
	.long	.LASF156
	.byte	0x10
	.byte	0x42
	.byte	0x20
	.long	0xaad
	.uleb128 0x6
	.long	.LASF157
	.byte	0x48
	.byte	0x17
	.byte	0xd
	.byte	0x11
	.long	0xb30
	.uleb128 0x1
	.long	.LASF158
	.byte	0x17
	.byte	0x10
	.byte	0xb
	.long	0x399
	.byte	0
	.uleb128 0x1
	.long	.LASF159
	.byte	0x17
	.byte	0x11
	.byte	0xb
	.long	0x399
	.byte	0x8
	.uleb128 0x1
	.long	.LASF160
	.byte	0x17
	.byte	0x15
	.byte	0xc
	.long	0xc3e
	.byte	0x10
	.uleb128 0x1
	.long	.LASF161
	.byte	0x17
	.byte	0x15
	.byte	0x16
	.long	0xc3e
	.byte	0x18
	.uleb128 0x1
	.long	.LASF162
	.byte	0x17
	.byte	0x23
	.byte	0xb
	.long	0xef8
	.byte	0x20
	.uleb128 0x1
	.long	.LASF163
	.byte	0x17
	.byte	0x2a
	.byte	0xb
	.long	0x399
	.byte	0x28
	.uleb128 0x1
	.long	.LASF164
	.byte	0x17
	.byte	0x4d
	.byte	0x13
	.long	0xefd
	.byte	0x30
	.uleb128 0x1
	.long	.LASF165
	.byte	0x17
	.byte	0x50
	.byte	0xb
	.long	0x399
	.byte	0x38
	.uleb128 0x1
	.long	.LASF166
	.byte	0x17
	.byte	0x51
	.byte	0xd
	.long	0xdf1
	.byte	0x40
	.byte	0
	.uleb128 0x2
	.long	.LASF167
	.byte	0x10
	.byte	0x44
	.byte	0x26
	.long	0xb41
	.uleb128 0x1b
	.long	0xb30
	.uleb128 0x6
	.long	.LASF168
	.byte	0x38
	.byte	0x17
	.byte	0x62
	.byte	0x11
	.long	0xbaa
	.uleb128 0x1
	.long	.LASF169
	.byte	0x17
	.byte	0x63
	.byte	0xb
	.long	0xf0d
	.byte	0
	.uleb128 0x1
	.long	.LASF170
	.byte	0x17
	.byte	0x68
	.byte	0xb
	.long	0xf0d
	.byte	0x8
	.uleb128 0x1
	.long	.LASF171
	.byte	0x17
	.byte	0x6a
	.byte	0xb
	.long	0xf26
	.byte	0x10
	.uleb128 0x1
	.long	.LASF172
	.byte	0x17
	.byte	0x6b
	.byte	0xb
	.long	0xf3a
	.byte	0x18
	.uleb128 0x1
	.long	.LASF173
	.byte	0x17
	.byte	0x73
	.byte	0x10
	.long	0xf53
	.byte	0x20
	.uleb128 0x1
	.long	.LASF174
	.byte	0x17
	.byte	0x76
	.byte	0x10
	.long	0xf71
	.byte	0x28
	.uleb128 0x1
	.long	.LASF175
	.byte	0x17
	.byte	0x7c
	.byte	0x10
	.long	0xf53
	.byte	0x30
	.byte	0
	.uleb128 0x21
	.long	.LASF279
	.byte	0x7
	.byte	0x4
	.long	0x2ba
	.byte	0x13
	.byte	0x51
	.byte	0x7
	.long	0xbfb
	.uleb128 0xd
	.long	.LASF176
	.byte	0x1
	.uleb128 0xd
	.long	.LASF177
	.byte	0x2
	.uleb128 0xd
	.long	.LASF178
	.byte	0x4
	.uleb128 0xd
	.long	.LASF179
	.byte	0x8
	.uleb128 0xd
	.long	.LASF180
	.byte	0x10
	.uleb128 0xd
	.long	.LASF181
	.byte	0x20
	.uleb128 0xd
	.long	.LASF182
	.byte	0x40
	.uleb128 0xd
	.long	.LASF183
	.byte	0x80
	.uleb128 0x1c
	.long	.LASF184
	.value	0x100
	.uleb128 0x1c
	.long	.LASF185
	.value	0x200
	.byte	0
	.uleb128 0x10
	.byte	0x20
	.byte	0x13
	.byte	0x69
	.byte	0x9
	.long	0xc39
	.uleb128 0x7
	.string	"vma"
	.byte	0x13
	.byte	0x6a
	.byte	0xc
	.long	0xc3e
	.byte	0
	.uleb128 0x1
	.long	.LASF186
	.byte	0x13
	.byte	0x6b
	.byte	0xb
	.long	0x3a5
	.byte	0x8
	.uleb128 0x1
	.long	.LASF187
	.byte	0x13
	.byte	0x6c
	.byte	0xd
	.long	0x31d
	.byte	0x10
	.uleb128 0x1
	.long	.LASF188
	.byte	0x13
	.byte	0x6d
	.byte	0xb
	.long	0x399
	.byte	0x18
	.byte	0
	.uleb128 0x1b
	.long	0xbfb
	.uleb128 0x5
	.long	0xaa1
	.uleb128 0x5
	.long	0x799
	.uleb128 0x5
	.long	0x7c8
	.uleb128 0x5
	.long	0x7f7
	.uleb128 0x5
	.long	0x826
	.uleb128 0x5
	.long	0x916
	.uleb128 0x10
	.byte	0xe8
	.byte	0x14
	.byte	0x9
	.byte	0x3
	.long	0xdec
	.uleb128 0x1
	.long	.LASF189
	.byte	0x14
	.byte	0xa
	.byte	0xd
	.long	0xc3e
	.byte	0
	.uleb128 0x1
	.long	.LASF190
	.byte	0x14
	.byte	0x10
	.byte	0xc
	.long	0x399
	.byte	0x8
	.uleb128 0x1
	.long	.LASF191
	.byte	0x14
	.byte	0x17
	.byte	0xc
	.long	0x399
	.byte	0x10
	.uleb128 0x1
	.long	.LASF192
	.byte	0x14
	.byte	0x18
	.byte	0xc
	.long	0x399
	.byte	0x18
	.uleb128 0x7
	.string	"pgd"
	.byte	0x14
	.byte	0x19
	.byte	0xd
	.long	0xdec
	.byte	0x20
	.uleb128 0x1
	.long	.LASF193
	.byte	0x14
	.byte	0x2e
	.byte	0xe
	.long	0x401
	.byte	0x28
	.uleb128 0x1
	.long	.LASF194
	.byte	0x14
	.byte	0x37
	.byte	0xe
	.long	0x401
	.byte	0x2c
	.uleb128 0x1
	.long	.LASF195
	.byte	0x14
	.byte	0x38
	.byte	0x12
	.long	0x4ae
	.byte	0x30
	.uleb128 0x1
	.long	.LASF196
	.byte	0x14
	.byte	0x39
	.byte	0xb
	.long	0x2a7
	.byte	0x38
	.uleb128 0x1
	.long	.LASF197
	.byte	0x14
	.byte	0x3a
	.byte	0x10
	.long	0x4ba
	.byte	0x40
	.uleb128 0x1
	.long	.LASF198
	.byte	0x14
	.byte	0x53
	.byte	0xc
	.long	0x399
	.byte	0x48
	.uleb128 0x1
	.long	.LASF199
	.byte	0x14
	.byte	0x54
	.byte	0xc
	.long	0x399
	.byte	0x50
	.uleb128 0x1
	.long	.LASF200
	.byte	0x14
	.byte	0x55
	.byte	0x10
	.long	0x428
	.byte	0x58
	.uleb128 0x1
	.long	.LASF201
	.byte	0x14
	.byte	0x56
	.byte	0xc
	.long	0x399
	.byte	0x60
	.uleb128 0x1
	.long	.LASF202
	.byte	0x14
	.byte	0x57
	.byte	0xc
	.long	0x399
	.byte	0x68
	.uleb128 0x1
	.long	.LASF203
	.byte	0x14
	.byte	0x58
	.byte	0xc
	.long	0x399
	.byte	0x70
	.uleb128 0x1
	.long	.LASF204
	.byte	0x14
	.byte	0x59
	.byte	0xc
	.long	0x399
	.byte	0x78
	.uleb128 0x1
	.long	.LASF205
	.byte	0x14
	.byte	0x64
	.byte	0xc
	.long	0x399
	.byte	0x80
	.uleb128 0x1
	.long	.LASF206
	.byte	0x14
	.byte	0x64
	.byte	0x18
	.long	0x399
	.byte	0x88
	.uleb128 0x1
	.long	.LASF207
	.byte	0x14
	.byte	0x64
	.byte	0x22
	.long	0x399
	.byte	0x90
	.uleb128 0x1
	.long	.LASF208
	.byte	0x14
	.byte	0x64
	.byte	0x2e
	.long	0x399
	.byte	0x98
	.uleb128 0x1
	.long	.LASF209
	.byte	0x14
	.byte	0x65
	.byte	0xc
	.long	0x399
	.byte	0xa0
	.uleb128 0x7
	.string	"brk"
	.byte	0x14
	.byte	0x65
	.byte	0x17
	.long	0x399
	.byte	0xa8
	.uleb128 0x1
	.long	.LASF210
	.byte	0x14
	.byte	0x65
	.byte	0x1c
	.long	0x399
	.byte	0xb0
	.uleb128 0x1
	.long	.LASF211
	.byte	0x14
	.byte	0x66
	.byte	0xc
	.long	0x399
	.byte	0xb8
	.uleb128 0x1
	.long	.LASF212
	.byte	0x14
	.byte	0x66
	.byte	0x17
	.long	0x399
	.byte	0xc0
	.uleb128 0x1
	.long	.LASF213
	.byte	0x14
	.byte	0x66
	.byte	0x20
	.long	0x399
	.byte	0xc8
	.uleb128 0x1
	.long	.LASF214
	.byte	0x14
	.byte	0x66
	.byte	0x2b
	.long	0x399
	.byte	0xd0
	.uleb128 0x1
	.long	.LASF11
	.byte	0x14
	.byte	0x75
	.byte	0xc
	.long	0x399
	.byte	0xd8
	.uleb128 0x1
	.long	.LASF215
	.byte	0x14
	.byte	0x8b
	.byte	0x12
	.long	0xdf1
	.byte	0xe0
	.byte	0
	.uleb128 0x5
	.long	0x76a
	.uleb128 0x5
	.long	0x544
	.uleb128 0x11
	.byte	0x8
	.byte	0x15
	.byte	0x2e
	.byte	0x3
	.long	0xe18
	.uleb128 0x9
	.long	.LASF11
	.byte	0x15
	.byte	0x2f
	.byte	0xc
	.long	0x399
	.uleb128 0x9
	.long	.LASF216
	.byte	0x15
	.byte	0x30
	.byte	0x12
	.long	0x94f
	.byte	0
	.uleb128 0x10
	.byte	0x10
	.byte	0x15
	.byte	0x42
	.byte	0x6
	.long	0xe3c
	.uleb128 0x1
	.long	.LASF217
	.byte	0x15
	.byte	0x44
	.byte	0xd
	.long	0x3b1
	.byte	0
	.uleb128 0x1
	.long	.LASF218
	.byte	0x15
	.byte	0x46
	.byte	0xc
	.long	0x38d
	.byte	0x8
	.byte	0
	.uleb128 0x11
	.byte	0x10
	.byte	0x15
	.byte	0x3f
	.byte	0x5
	.long	0xe6f
	.uleb128 0xc
	.string	"lru"
	.byte	0x15
	.byte	0x40
	.byte	0xe
	.long	0x4c6
	.uleb128 0x14
	.long	0xe18
	.uleb128 0x9
	.long	.LASF219
	.byte	0x15
	.byte	0x49
	.byte	0xe
	.long	0x4c6
	.uleb128 0x9
	.long	.LASF220
	.byte	0x15
	.byte	0x4a
	.byte	0xe
	.long	0x4c6
	.byte	0
	.uleb128 0x10
	.byte	0x28
	.byte	0x15
	.byte	0x39
	.byte	0x4
	.long	0xea6
	.uleb128 0xb
	.long	0xe3c
	.byte	0
	.uleb128 0x1
	.long	.LASF221
	.byte	0x15
	.byte	0x4d
	.byte	0x12
	.long	0xea6
	.byte	0x10
	.uleb128 0x1
	.long	.LASF222
	.byte	0x15
	.byte	0x4e
	.byte	0xf
	.long	0x31d
	.byte	0x18
	.uleb128 0x1
	.long	.LASF223
	.byte	0x15
	.byte	0x55
	.byte	0xd
	.long	0x399
	.byte	0x20
	.byte	0
	.uleb128 0x5
	.long	0x533
	.uleb128 0x10
	.byte	0x8
	.byte	0x15
	.byte	0x6f
	.byte	0x4
	.long	0xec2
	.uleb128 0x1
	.long	.LASF224
	.byte	0x15
	.byte	0x70
	.byte	0xd
	.long	0x399
	.byte	0
	.byte	0
	.uleb128 0x11
	.byte	0x28
	.byte	0x15
	.byte	0x38
	.byte	0x3
	.long	0xed6
	.uleb128 0x14
	.long	0xe6f
	.uleb128 0x14
	.long	0xeab
	.byte	0
	.uleb128 0x11
	.byte	0x4
	.byte	0x15
	.byte	0x87
	.byte	0x3
	.long	0xef8
	.uleb128 0x9
	.long	.LASF225
	.byte	0x15
	.byte	0x8c
	.byte	0xd
	.long	0x401
	.uleb128 0x9
	.long	.LASF226
	.byte	0x15
	.byte	0x93
	.byte	0xa
	.long	0x38d
	.byte	0
	.uleb128 0x5
	.long	0x8e9
	.uleb128 0x5
	.long	0xb3c
	.uleb128 0x22
	.long	0xf0d
	.uleb128 0xe
	.long	0xc3e
	.byte	0
	.uleb128 0x5
	.long	0xf02
	.uleb128 0x16
	.long	0x2a7
	.long	0xf26
	.uleb128 0xe
	.long	0xc3e
	.uleb128 0xe
	.long	0x399
	.byte	0
	.uleb128 0x5
	.long	0xf12
	.uleb128 0x16
	.long	0x2a7
	.long	0xf3a
	.uleb128 0xe
	.long	0xc3e
	.byte	0
	.uleb128 0x5
	.long	0xf2b
	.uleb128 0x16
	.long	0x855
	.long	0xf4e
	.uleb128 0xe
	.long	0xf4e
	.byte	0
	.uleb128 0x5
	.long	0x861
	.uleb128 0x5
	.long	0xf3f
	.uleb128 0x16
	.long	0x855
	.long	0xf71
	.uleb128 0xe
	.long	0xf4e
	.uleb128 0xe
	.long	0x31d
	.uleb128 0xe
	.long	0x31d
	.byte	0
	.uleb128 0x5
	.long	0xf58
	.uleb128 0x6
	.long	.LASF227
	.byte	0x40
	.byte	0x18
	.byte	0x12
	.byte	0x12
	.long	0xf8a
	.uleb128 0xb
	.long	0x102c
	.byte	0
	.byte	0
	.uleb128 0x23
	.long	.LASF280
	.value	0x5000
	.value	0x1000
	.byte	0x19
	.byte	0x9d
	.byte	0x9
	.long	0xfb5
	.uleb128 0x1
	.long	.LASF228
	.byte	0x19
	.byte	0xa4
	.byte	0x10
	.long	0x11dc
	.byte	0
	.uleb128 0x1
	.long	.LASF229
	.byte	0x19
	.byte	0xa6
	.byte	0x13
	.long	0x123c
	.byte	0x68
	.byte	0
	.uleb128 0x10
	.byte	0x30
	.byte	0x18
	.byte	0x14
	.byte	0x5
	.long	0x1027
	.uleb128 0x1
	.long	.LASF230
	.byte	0x18
	.byte	0x15
	.byte	0x10
	.long	0x1027
	.byte	0
	.uleb128 0x1
	.long	.LASF231
	.byte	0x18
	.byte	0x16
	.byte	0xd
	.long	0x2a7
	.byte	0x8
	.uleb128 0x1
	.long	.LASF232
	.byte	0x18
	.byte	0x17
	.byte	0xd
	.long	0x2a7
	.byte	0xc
	.uleb128 0x1
	.long	.LASF233
	.byte	0x18
	.byte	0x18
	.byte	0xd
	.long	0x312
	.byte	0x10
	.uleb128 0x1
	.long	.LASF234
	.byte	0x18
	.byte	0x19
	.byte	0x14
	.long	0x31d
	.byte	0x18
	.uleb128 0x1
	.long	.LASF235
	.byte	0x18
	.byte	0x1a
	.byte	0xe
	.long	0x3b1
	.byte	0x20
	.uleb128 0x1
	.long	.LASF236
	.byte	0x18
	.byte	0x1b
	.byte	0xd
	.long	0x2f1
	.byte	0x28
	.uleb128 0x1
	.long	.LASF237
	.byte	0x18
	.byte	0x1c
	.byte	0xd
	.long	0x36e
	.byte	0x2a
	.byte	0
	.uleb128 0x5
	.long	0x4c
	.uleb128 0x11
	.byte	0x40
	.byte	0x18
	.byte	0x13
	.byte	0x4
	.long	0x1047
	.uleb128 0x14
	.long	0xfb5
	.uleb128 0xc
	.string	"pad"
	.byte	0x18
	.byte	0x1e
	.byte	0x8
	.long	0x1047
	.byte	0
	.uleb128 0x17
	.long	0x2e7
	.long	0x1057
	.uleb128 0x18
	.long	0x31d
	.byte	0x3f
	.byte	0
	.uleb128 0x6
	.long	.LASF238
	.byte	0x20
	.byte	0x1a
	.byte	0x29
	.byte	0x11
	.long	0x107f
	.uleb128 0x1
	.long	.LASF239
	.byte	0x1a
	.byte	0x2a
	.byte	0x13
	.long	0x44f
	.byte	0
	.uleb128 0x1
	.long	.LASF240
	.byte	0x1a
	.byte	0x2b
	.byte	0xf
	.long	0x527
	.byte	0x8
	.byte	0
	.uleb128 0x2
	.long	.LASF241
	.byte	0x1a
	.byte	0x2c
	.byte	0x4
	.long	0x1057
	.uleb128 0x6
	.long	.LASF242
	.byte	0x28
	.byte	0x1b
	.byte	0x1a
	.byte	0x11
	.long	0x10b3
	.uleb128 0x1
	.long	.LASF243
	.byte	0x1b
	.byte	0x1b
	.byte	0x10
	.long	0x2ba
	.byte	0
	.uleb128 0x1
	.long	.LASF244
	.byte	0x1b
	.byte	0x1c
	.byte	0x11
	.long	0x107f
	.byte	0x8
	.byte	0
	.uleb128 0x2
	.long	.LASF245
	.byte	0x1b
	.byte	0x1d
	.byte	0x4
	.long	0x108b
	.uleb128 0x2
	.long	.LASF246
	.byte	0x2
	.byte	0x1a
	.byte	0x1b
	.long	0x10cb
	.uleb128 0x12
	.long	.LASF247
	.uleb128 0x2
	.long	.LASF248
	.byte	0x2
	.byte	0x2e
	.byte	0x1e
	.long	0x10dc
	.uleb128 0x6
	.long	.LASF249
	.byte	0x8
	.byte	0x1c
	.byte	0x8
	.byte	0x11
	.long	0x10f7
	.uleb128 0x1
	.long	.LASF250
	.byte	0x1c
	.byte	0x11
	.byte	0xa
	.long	0x312
	.byte	0
	.byte	0
	.uleb128 0x2
	.long	.LASF251
	.byte	0x2
	.byte	0x30
	.byte	0x21
	.long	0x1103
	.uleb128 0x6
	.long	.LASF252
	.byte	0x18
	.byte	0x1c
	.byte	0x2a
	.byte	0x11
	.long	0x112b
	.uleb128 0x1
	.long	.LASF253
	.byte	0x1c
	.byte	0x2b
	.byte	0xc
	.long	0x4c6
	.byte	0
	.uleb128 0x1
	.long	.LASF254
	.byte	0x1c
	.byte	0x2e
	.byte	0x10
	.long	0x2ba
	.byte	0x10
	.byte	0
	.uleb128 0x2
	.long	.LASF255
	.byte	0x2
	.byte	0x38
	.byte	0x15
	.long	0x1137
	.uleb128 0x24
	.string	"pid"
	.uleb128 0x6
	.long	.LASF256
	.byte	0x68
	.byte	0x19
	.byte	0x68
	.byte	0x11
	.long	0x11cc
	.uleb128 0x1
	.long	.LASF257
	.byte	0x19
	.byte	0x69
	.byte	0x7
	.long	0x2fc
	.byte	0
	.uleb128 0x7
	.string	"sp0"
	.byte	0x19
	.byte	0x6a
	.byte	0x7
	.long	0x312
	.byte	0x4
	.uleb128 0x7
	.string	"sp1"
	.byte	0x19
	.byte	0x6b
	.byte	0x7
	.long	0x312
	.byte	0xc
	.uleb128 0x7
	.string	"sp2"
	.byte	0x19
	.byte	0x72
	.byte	0x7
	.long	0x312
	.byte	0x14
	.uleb128 0x1
	.long	.LASF258
	.byte	0x19
	.byte	0x74
	.byte	0x7
	.long	0x312
	.byte	0x1c
	.uleb128 0x7
	.string	"ist"
	.byte	0x19
	.byte	0x75
	.byte	0x7
	.long	0x11cc
	.byte	0x24
	.uleb128 0x1
	.long	.LASF259
	.byte	0x19
	.byte	0x76
	.byte	0x7
	.long	0x2fc
	.byte	0x5c
	.uleb128 0x1
	.long	.LASF260
	.byte	0x19
	.byte	0x77
	.byte	0x7
	.long	0x2fc
	.byte	0x60
	.uleb128 0x1
	.long	.LASF261
	.byte	0x19
	.byte	0x78
	.byte	0x7
	.long	0x2f1
	.byte	0x64
	.uleb128 0x1
	.long	.LASF262
	.byte	0x19
	.byte	0x79
	.byte	0x7
	.long	0x2f1
	.byte	0x66
	.byte	0
	.uleb128 0x17
	.long	0x312
	.long	0x11dc
	.uleb128 0x18
	.long	0x31d
	.byte	0x6
	.byte	0
	.uleb128 0x2
	.long	.LASF263
	.byte	0x19
	.byte	0x7b
	.byte	0x1c
	.long	0x113c
	.uleb128 0x19
	.long	.LASF264
	.value	0x4020
	.byte	0x19
	.byte	0x80
	.long	0x122b
	.uleb128 0x1
	.long	.LASF265
	.byte	0x19
	.byte	0x83
	.byte	0x7
	.long	0x312
	.byte	0
	.uleb128 0x1
	.long	.LASF266
	.byte	0x19
	.byte	0x8c
	.byte	0x10
	.long	0x2ba
	.byte	0x8
	.uleb128 0x1
	.long	.LASF267
	.byte	0x19
	.byte	0x94
	.byte	0x11
	.long	0x122b
	.byte	0x10
	.uleb128 0x25
	.long	.LASF268
	.byte	0x19
	.byte	0x9a
	.byte	0x11
	.long	0x122b
	.value	0x2018
	.byte	0
	.uleb128 0x17
	.long	0x31d
	.long	0x123c
	.uleb128 0x26
	.long	0x31d
	.value	0x400
	.byte	0
	.uleb128 0x2
	.long	.LASF269
	.byte	0x19
	.byte	0x9b
	.byte	0x4
	.long	0x11e8
	.uleb128 0x6
	.long	.LASF270
	.byte	0x38
	.byte	0x19
	.byte	0xa9
	.byte	0x11
	.long	0x12d5
	.uleb128 0x7
	.string	"sp"
	.byte	0x19
	.byte	0xac
	.byte	0xb
	.long	0x356
	.byte	0
	.uleb128 0x7
	.string	"es"
	.byte	0x19
	.byte	0xad
	.byte	0x12
	.long	0x2a0
	.byte	0x8
	.uleb128 0x7
	.string	"ds"
	.byte	0x19
	.byte	0xae
	.byte	0x12
	.long	0x2a0
	.byte	0xa
	.uleb128 0x1
	.long	.LASF271
	.byte	0x19
	.byte	0xaf
	.byte	0x12
	.long	0x2a0
	.byte	0xc
	.uleb128 0x1
	.long	.LASF272
	.byte	0x19
	.byte	0xb0
	.byte	0x12
	.long	0x2a0
	.byte	0xe
	.uleb128 0x1
	.long	.LASF273
	.byte	0x19
	.byte	0xb2
	.byte	0xb
	.long	0x356
	.byte	0x10
	.uleb128 0x1
	.long	.LASF274
	.byte	0x19
	.byte	0xb3
	.byte	0xb
	.long	0x356
	.byte	0x18
	.uleb128 0x7
	.string	"cr2"
	.byte	0x19
	.byte	0xbc
	.byte	0xb
	.long	0x356
	.byte	0x20
	.uleb128 0x1
	.long	.LASF275
	.byte	0x19
	.byte	0xbd
	.byte	0x11
	.long	0x31d
	.byte	0x28
	.uleb128 0x1
	.long	.LASF276
	.byte	0x19
	.byte	0xbe
	.byte	0x11
	.long	0x31d
	.byte	0x30
	.byte	0
	.uleb128 0x2
	.long	.LASF277
	.byte	0x19
	.byte	0xdc
	.byte	0x4
	.long	0x1248
	.uleb128 0x5
	.long	0x112b
	.uleb128 0x5
	.long	0x10b3
	.uleb128 0x17
	.long	0x324
	.long	0x12fb
	.uleb128 0x18
	.long	0x31d
	.byte	0x1f
	.byte	0
	.uleb128 0x5
	.long	0x10bf
	.uleb128 0x5
	.long	0x3b
	.uleb128 0x5
	.long	0x2a
	.uleb128 0x27
	.long	.LASF281
	.byte	0x1
	.byte	0x5
	.byte	0x14
	.quad	.LFB35
	.quad	.LFE35-.LFB35
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
	.uleb128 0x21
	.sleb128 18
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
	.uleb128 0x4
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
	.uleb128 0x7
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
	.sleb128 4
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
	.sleb128 4
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
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 6
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0xd
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x17
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 18
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 16
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x16
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
	.uleb128 0x17
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
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
	.sleb128 4
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 13
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
	.uleb128 0x1f
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
	.uleb128 0x20
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
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
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3c
	.uleb128 0x19
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
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.LFB35
	.quad	.LFE35-.LFB35
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
	.quad	.LFB35
	.uleb128 .LFE35-.LFB35
	.byte	0
.Ldebug_ranges3:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF180:
	.string	"FAULT_FLAG_KILLABLE"
.LASF64:
	.string	"size_t"
.LASF187:
	.string	"pgoff"
.LASF275:
	.string	"trap_nr"
.LASF194:
	.string	"mm_refcount"
.LASF80:
	.string	"tail"
.LASF184:
	.string	"FAULT_FLAG_INSTRUCTION"
.LASF164:
	.string	"vm_ops"
.LASF237:
	.string	"hardirq_stack_inuse"
.LASF132:
	.string	"PG_locked"
.LASF84:
	.string	"list_head"
.LASF106:
	.string	"arch_pte_T"
.LASF46:
	.string	"__u16"
.LASF22:
	.string	"real_parent"
.LASF21:
	.string	"in_iowait"
.LASF42:
	.string	"__u8"
.LASF78:
	.string	"tspinlock"
.LASF262:
	.string	"io_bitmap_base"
.LASF270:
	.string	"thread_struct"
.LASF37:
	.string	"comm"
.LASF169:
	.string	"open"
.LASF191:
	.string	"task_size"
.LASF271:
	.string	"fsindex"
.LASF177:
	.string	"FAULT_FLAG_MKWRITE"
.LASF264:
	.string	"x86_io_bitmap"
.LASF131:
	.string	"pgflag_defs"
.LASF196:
	.string	"map_count"
.LASF230:
	.string	"current_task"
.LASF96:
	.string	"pmdval_t"
.LASF241:
	.string	"swqueue_hdr_s"
.LASF36:
	.string	"start_boottime"
.LASF130:
	.string	"pgflag_defs_s"
.LASF186:
	.string	"gfp_mask"
.LASF24:
	.string	"children"
.LASF48:
	.string	"__u32"
.LASF58:
	.string	"__kernel_pid_t"
.LASF142:
	.string	"PG_error"
.LASF220:
	.string	"pcp_list"
.LASF272:
	.string	"gsindex"
.LASF13:
	.string	"recent_used_cpu"
.LASF30:
	.string	"utime"
.LASF56:
	.string	"long int"
.LASF206:
	.string	"end_code"
.LASF228:
	.string	"x86_tss"
.LASF215:
	.string	"exe_file"
.LASF182:
	.string	"FAULT_FLAG_USER"
.LASF26:
	.string	"thread_pid"
.LASF167:
	.string	"vm_ops_s"
.LASF269:
	.string	"x86_io_bitmap_s"
.LASF134:
	.string	"PG_referenced"
.LASF60:
	.string	"reg_t"
.LASF107:
	.string	"arch_pte"
.LASF157:
	.string	"vm_area_struct"
.LASF79:
	.string	"head"
.LASF223:
	.string	"private"
.LASF110:
	.string	"p4d_t"
.LASF246:
	.string	"taskfs_s"
.LASF43:
	.string	"signed char"
.LASF218:
	.string	"mlock_count"
.LASF2:
	.string	"signal_s"
.LASF103:
	.string	"arch_pud"
.LASF52:
	.string	"__u64"
.LASF121:
	.string	"vm_fault"
.LASF44:
	.string	"unsigned char"
.LASF160:
	.string	"vm_next"
.LASF122:
	.string	"orig_pte"
.LASF133:
	.string	"PG_writeback"
.LASF161:
	.string	"vm_prev"
.LASF146:
	.string	"PG_reserved"
.LASF247:
	.string	"fs_struct"
.LASF245:
	.string	"completion_s"
.LASF181:
	.string	"FAULT_FLAG_TRIED"
.LASF250:
	.string	"vruntime"
.LASF63:
	.string	"_Bool"
.LASF205:
	.string	"start_code"
.LASF216:
	.string	"flag_defs"
.LASF125:
	.string	"mm_s"
.LASF235:
	.string	"hardirq_stack_ptr"
.LASF211:
	.string	"arg_start"
.LASF55:
	.string	"char"
.LASF192:
	.string	"highest_vm_end"
.LASF14:
	.string	"wake_cpu"
.LASF176:
	.string	"FAULT_FLAG_WRITE"
.LASF135:
	.string	"PG_uptodate"
.LASF99:
	.string	"arch_pgd_T"
.LASF111:
	.string	"pgd_union"
.LASF225:
	.string	"_mapcount"
.LASF20:
	.string	"in_execve"
.LASF239:
	.string	"lock"
.LASF25:
	.string	"sibling"
.LASF39:
	.string	"signal"
.LASF165:
	.string	"vm_pgoff"
.LASF229:
	.string	"io_bitmap"
.LASF209:
	.string	"start_brk"
.LASF3:
	.string	"files_struct_s"
.LASF185:
	.string	"FAULT_FLAG_INTERRUPTIBLE"
.LASF27:
	.string	"pid_links"
.LASF11:
	.string	"flags"
.LASF104:
	.string	"arch_pmd_T"
.LASF65:
	.string	"uint"
.LASF120:
	.string	"vm_fault_s"
.LASF57:
	.string	"__kernel_ulong_t"
.LASF197:
	.string	"page_table_lock"
.LASF189:
	.string	"mmap"
.LASF5:
	.string	"files_struct"
.LASF109:
	.string	"defs"
.LASF94:
	.string	"file"
.LASF117:
	.string	"pte_t"
.LASF172:
	.string	"mremap"
.LASF81:
	.string	"atomic_long_t"
.LASF66:
	.string	"ulong"
.LASF102:
	.string	"arch_pud_T"
.LASF208:
	.string	"end_data"
.LASF202:
	.string	"exec_vm"
.LASF231:
	.string	"preempt_count"
.LASF195:
	.string	"pgtables_bytes"
.LASF95:
	.string	"pteval_t"
.LASF233:
	.string	"call_depth"
.LASF281:
	.string	"common"
.LASF253:
	.string	"run_list"
.LASF156:
	.string	"vma_s"
.LASF222:
	.string	"index"
.LASF147:
	.string	"PG_private"
.LASF123:
	.string	"cow_page"
.LASF54:
	.string	"long unsigned int"
.LASF116:
	.string	"pmd_union"
.LASF71:
	.string	"status"
.LASF188:
	.string	"address"
.LASF212:
	.string	"arg_end"
.LASF162:
	.string	"vm_mm"
.LASF119:
	.string	"vm_fault_t"
.LASF18:
	.string	"exit_code"
.LASF31:
	.string	"stime"
.LASF278:
	.ascii	"GNU C"
	.string	"17 11.4.0 -m64 -mcmodel=large -mabi=sysv -mno-red-zone -mtune=generic -march=x86-64 -ggdb -fPIC -ffreestanding -fno-stack-protector -fno-unwind-tables -fdata-sections -ffunction-sections -fasynchronous-unwind-tables -fstack-clash-protection -fcf-protection"
.LASF144:
	.string	"PG_owner_priv_1"
.LASF207:
	.string	"start_data"
.LASF68:
	.string	"thread_info_s"
.LASF198:
	.string	"total_vm"
.LASF249:
	.string	"sched_entity"
.LASF268:
	.string	"mapall"
.LASF4:
	.string	"signal_struct"
.LASF128:
	.string	"page_s"
.LASF210:
	.string	"start_stack"
.LASF69:
	.string	"task_struct"
.LASF28:
	.string	"vfork_done"
.LASF51:
	.string	"long long int"
.LASF274:
	.string	"gsbase"
.LASF74:
	.string	"counter"
.LASF59:
	.string	"__kernel_size_t"
.LASF7:
	.string	"thread_info"
.LASF214:
	.string	"env_end"
.LASF9:
	.string	"stack"
.LASF23:
	.string	"parent"
.LASF141:
	.string	"PG_workingset"
.LASF266:
	.string	"prev_max"
.LASF91:
	.string	"addr_spc_s"
.LASF70:
	.string	"syscall_work"
.LASF129:
	.string	"_refcount"
.LASF6:
	.string	"task_s"
.LASF244:
	.string	"wait"
.LASF151:
	.string	"PG_swapbacked"
.LASF251:
	.string	"sched_rt_entity_s"
.LASF89:
	.string	"count"
.LASF200:
	.string	"pinned_vm"
.LASF224:
	.string	"compound_head"
.LASF76:
	.string	"atomic64"
.LASF190:
	.string	"mmap_base"
.LASF49:
	.string	"unsigned int"
.LASF234:
	.string	"top_of_stack"
.LASF137:
	.string	"PG_lru"
.LASF82:
	.string	"spinlock_t"
.LASF267:
	.string	"bitmap"
.LASF108:
	.string	"pgd_t"
.LASF280:
	.string	"tss_struct"
.LASF140:
	.string	"PG_active"
.LASF171:
	.string	"may_split"
.LASF255:
	.string	"pid_s"
.LASF61:
	.string	"pid_t"
.LASF163:
	.string	"vm_flags"
.LASF226:
	.string	"page_type"
.LASF166:
	.string	"vm_file"
.LASF148:
	.string	"PG_private_2"
.LASF83:
	.string	"List_s"
.LASF115:
	.string	"pmd_t"
.LASF243:
	.string	"done"
.LASF232:
	.string	"cpu_number"
.LASF279:
	.string	"fault_flag"
.LASF138:
	.string	"PG_head"
.LASF124:
	.string	"page"
.LASF149:
	.string	"PG_mappedtodisk"
.LASF203:
	.string	"stack_vm"
.LASF265:
	.string	"prev_sequence"
.LASF62:
	.string	"bool"
.LASF219:
	.string	"buddy_list"
.LASF93:
	.string	"file_s"
.LASF277:
	.string	"thread_s"
.LASF113:
	.string	"pud_t"
.LASF73:
	.string	"atomic"
.LASF213:
	.string	"env_start"
.LASF145:
	.string	"PG_arch_1"
.LASF154:
	.string	"PG_arch_2"
.LASF155:
	.string	"PG_arch_3"
.LASF179:
	.string	"FAULT_FLAG_RETRY_NOWAIT"
.LASF53:
	.string	"long long unsigned int"
.LASF92:
	.string	"address_space"
.LASF90:
	.string	"List_hdr_s"
.LASF238:
	.string	"swait_queue_head"
.LASF88:
	.string	"anchor"
.LASF201:
	.string	"data_vm"
.LASF150:
	.string	"PG_reclaim"
.LASF139:
	.string	"PG_waiters"
.LASF276:
	.string	"error_code"
.LASF100:
	.string	"arch_pgd"
.LASF77:
	.string	"arch_spinlock_t"
.LASF199:
	.string	"locked_vm"
.LASF252:
	.string	"sched_rt_entity"
.LASF242:
	.string	"completion"
.LASF98:
	.string	"pgdval_t"
.LASF143:
	.string	"PG_slab"
.LASF41:
	.string	"thread"
.LASF97:
	.string	"pudval_t"
.LASF50:
	.string	"__s64"
.LASF227:
	.string	"pcpu_hot"
.LASF45:
	.string	"short int"
.LASF17:
	.string	"exit_state"
.LASF8:
	.string	"__state"
.LASF10:
	.string	"usage"
.LASF152:
	.string	"PG_unevictable"
.LASF114:
	.string	"pud_union"
.LASF159:
	.string	"vm_end"
.LASF175:
	.string	"page_mkwrite"
.LASF34:
	.string	"nivcsw"
.LASF257:
	.string	"reserved1"
.LASF258:
	.string	"reserved2"
.LASF259:
	.string	"reserved3"
.LASF260:
	.string	"reserved4"
.LASF261:
	.string	"reserved5"
.LASF29:
	.string	"worker_private"
.LASF240:
	.string	"task_list_hdr"
.LASF87:
	.string	"List_hdr"
.LASF174:
	.string	"map_pages"
.LASF15:
	.string	"on_rq"
.LASF67:
	.string	"gfp_t"
.LASF273:
	.string	"fsbase"
.LASF16:
	.string	"active_mm"
.LASF178:
	.string	"FAULT_FLAG_ALLOW_RETRY"
.LASF112:
	.string	"p4d_union"
.LASF263:
	.string	"x86_hw_tss_s"
.LASF19:
	.string	"exit_signal"
.LASF204:
	.string	"def_flags"
.LASF136:
	.string	"PG_dirty"
.LASF183:
	.string	"FAULT_FLAG_REMOTE"
.LASF153:
	.string	"PG_mlocked"
.LASF193:
	.string	"mm_users"
.LASF256:
	.string	"x86_hw_tss"
.LASF33:
	.string	"nvcsw"
.LASF236:
	.string	"softirq_pending"
.LASF217:
	.string	"__filler"
.LASF127:
	.string	"entry_point"
.LASF118:
	.string	"pte_union"
.LASF35:
	.string	"start_time"
.LASF47:
	.string	"short unsigned int"
.LASF72:
	.string	"atomic_t"
.LASF38:
	.string	"files"
.LASF221:
	.string	"mapping"
.LASF173:
	.string	"fault"
.LASF101:
	.string	"PHYADDR"
.LASF105:
	.string	"arch_pmd"
.LASF254:
	.string	"time_slice"
.LASF12:
	.string	"on_cpu"
.LASF126:
	.string	"mm_struct"
.LASF158:
	.string	"vm_start"
.LASF85:
	.string	"next"
.LASF168:
	.string	"vm_operations_struct"
.LASF170:
	.string	"close"
.LASF40:
	.string	"alloc_lock"
.LASF86:
	.string	"prev"
.LASF75:
	.string	"atomic64_t"
.LASF248:
	.string	"sched_entity_s"
.LASF32:
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
