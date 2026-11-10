# CORE-USER-ENTRY-STACK-BINDING-66 — 固定源码直接证据

固定源码：`time` 分支，短提交 `a039d9803ade`。每条源引文均从单一函数、宏、结构体或对象定义的连续1–5物理行逐字复制，保留TAB及空行。分D00（上一批文档纠正）与A/B/C三专题；本文件不代表机器码、编译、真实TSS/栈或运行结果。

## D 分组

### D01 — 原A01数组独立定义

[VERIFIED mykernel/arch/x86_64/kernel/idt.c::idt_table]
```c
static gate_desc idt_table[IDT_ENTRIES] __page_aligned_bss;
```

### D02 — 原A01描述符独立定义

[VERIFIED mykernel/arch/x86_64/kernel/idt.c::idt_descr]
```c
struct desc_ptr idt_descr __ro_after_init = {
	.size		= IDT_TABLE_SIZE - 1,
	.address	= (unsigned long)&idt_table,
};
```

### D03 — 原A16去除定义外注释

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::load_idt]
```c
		void
		load_idt(const struct desc_ptr *dtr) {
			asm volatile(	"lidt	%0		\t\n"
						:
						:	"m" (*dtr)
```

### D04 — 原A17首个单宏

[VERIFIED mykernel/arch/x86_64/include/asm/idtentry.h::HAS_KERNEL_IBT]
```c
	#define HAS_KERNEL_IBT	0
```

### D05 — 原A17第二个单宏

[VERIFIED mykernel/arch/x86_64/include/asm/idtentry.h::IDT_ALIGN]
```c
	#define IDT_ALIGN	(8 * (2 + HAS_KERNEL_IBT))
```

### D06 — 原A18第一单宏

[VERIFIED mykernel/arch/x86_64/processor/desc_const_arch.h::IDT_ENTRIES]
```c
	#define IDT_ENTRIES				256
```

### D07 — 原A18第二单宏

[VERIFIED mykernel/arch/x86_64/processor/desc_const_arch.h::NUM_EXCEPTION_VECTORS]
```c
	#define NUM_EXCEPTION_VECTORS	32
```

### D08 — 原A20第一单宏

[VERIFIED mykernel/arch/x86_64/include/asm/trapnr.h::X86_TRAP_GP]
```c
	#define X86_TRAP_GP			13	/* General Protection Fault */
```

### D09 — 原A20第二单宏

[VERIFIED mykernel/arch/x86_64/include/asm/trapnr.h::X86_TRAP_PF]
```c
	#define X86_TRAP_PF			14	/* Page Fault */
```

### D10 — 原A21第一单宏

[VERIFIED mykernel/arch/x86_64/include/obsolete/myos_irq_vectors.h::GEN_PROT_VEC]
```c
	#define GEN_PROT_VEC				13		/* general protection */
```

### D11 — 原A21第二单宏

[VERIFIED mykernel/arch/x86_64/include/obsolete/myos_irq_vectors.h::PAGE_FAULT_VEC]
```c
	#define PAGE_FAULT_VEC				14
```

### D12 — 原A22只留单宏

[VERIFIED mykernel/arch/x86_64/include/asm/irq_vectors.h::FIRST_EXTERNAL_VECTOR]
```c
	#define FIRST_EXTERNAL_VECTOR			0x20
```

### D13 — 原A23从真正标签开始

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::idt_handler_array]
```asm
SYM_CODE_START(idt_handler_array)
	vector = 0
	.rept		NUM_EXCEPTION_VECTORS
```

### D14 — 原B11只保留typedef

[VERIFIED mykernel/include/linux/kernel/types.h::reg_t]
```c
		typedef __UINT64_TYPE__ reg_t;
```

### D15 — 原B12宏一

[VERIFIED mykernel/include/generated/asm-offsets.h::IRQ_NR]
```c
	#define IRQ_NR 120
```

### D16 — 原B12宏二

[VERIFIED mykernel/include/generated/asm-offsets.h::ORIG_RAX]
```c
	#define ORIG_RAX 128
```

### D17 — 原B12宏三

[VERIFIED mykernel/include/generated/asm-offsets.h::RIP]
```c
	#define RIP 136
```

### D18 — 原B12宏四

[VERIFIED mykernel/include/generated/asm-offsets.h::CS]
```c
	#define CS 144
```

### D19 — 原B12宏五

[VERIFIED mykernel/include/generated/asm-offsets.h::EFLAGS]
```c
	#define EFLAGS 152
```

### D20 — 原B13宏一

[VERIFIED mykernel/include/generated/asm-offsets.h::RSP]
```c
	#define RSP 160
```

### D21 — 原B13宏二

[VERIFIED mykernel/include/generated/asm-offsets.h::SS]
```c
	#define SS 168
```

### D22 — 原B14属于main内OFFSET调用

[VERIFIED mykernel/arch/x86_64/kbuild/asm-offsets_64.c::main]
```c
	OFFSET(IRQ_NR, pt_regs, irq_nr);
	OFFSET(ORIG_RAX, pt_regs, orig_ax);
	OFFSET(RIP, pt_regs, ip);
	OFFSET(CS, pt_regs, cs);
	OFFSET(EFLAGS, pt_regs, flags);
```

### D23 — 原C04宏真实定义

[VERIFIED mykernel/sched/scheduler/preempt_macro.h::in_atomic]
```c
	#define in_atomic()	(				\
				preempt_count() != 0	\
			)
```

### D24 — 补足门present与vector字段

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::init_idt_data]
```c
			data->addr			= addr;
			data->segment		= __KERNEL_CS;
			data->bits.type		= GATE_INTERRUPT;
			data->bits.p		= 1;
		}
```

### D25 — 补足gate.offset_low赋值

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::idt_init_desc]
```c
		idt_init_desc(gate_desc *gate, const struct idt_data *d) {
			ulong addr	= (ulong) d->addr;

			gate->offset_low	= (u16) addr;
			gate->segment		= (u16) d->segment;
```

### D26 — 正确table_write命题

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::write_idt_entry]
```c
		write_idt_entry(gate_desc *idt, int entry, const gate_desc *gate) {
			memcpy(&idt[entry], gate, sizeof(*gate));
		}
```

### D27 — 修正stub条件补码

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::idt_handler_array]
```asm
	.rept		NUM_EXCEPTION_VECTORS
	/* Push 0 for Error Number placeholder */
	.if ((EXCEPTION_ERRCODE_MASK >> vector) & 1) == 0
		endbr64
		pushq	$0		# Dummy error code, to make stack frame uniform
```

### D28 — 修正stub压vector及fill

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::idt_handler_array]
```asm
	pushq		$vector		# 72(%rsp) Vector number
	jmp			sa_entintr_retp
	vector = vector + 1
	.fill		idt_handler_array + vector*IDT_ALIGN - ., 1, 0xcc
	.endr
```

## A 分组

### A01 — init栈上界计算

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_macro_arch.h::TOP_OF_INIT_STACK]
```c
	#define TOP_OF_INIT_STACK (										\
				(unsigned long)&init_stack + sizeof(init_stack) -	\
					TOP_OF_KERNEL_STACK_PADDING						\
			)
```

### A02 — 任务pt_regs地址计算及指针-1

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_macro_arch.h::task_pt_regs]
```c
	#define task_pt_regs(task)	({									\
				loff_t __ptr = (loff_t)task_stack_page(task);		\
				__ptr += THREAD_SIZE - TOP_OF_KERNEL_STACK_PADDING;	\
				((pt_regs_s *)__ptr) - 1;							\
			})
```

### A03 — task top计算

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_macro_arch.h::task_top_of_stack]
```c
	#define task_top_of_stack(task) ((unsigned long)(task_pt_regs(task) + 1))
```

### A04 — task stack直接返回

[VERIFIED mykernel/sched/misc/sched_misc.h::task_stack_page]
```c
		PREFIX_STATIC_INLINE
		void
		*task_stack_page(const task_s *task) {
			return task->stack;
		}
```

### A05 — 页面位数定义

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT]
```c
	#define PAGE_SHIFT				12
```

### A06 — 页面大小定义

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```

### A07 — 栈页阶宏

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::THREAD_SIZE_ORDER]
```c
	#define THREAD_SIZE_ORDER		3
```

### A08 — THREAD_SIZE宏

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::THREAD_SIZE]
```c
	#define THREAD_SIZE				(PAGE_SIZE << THREAD_SIZE_ORDER)
```

### A09 — x86-64栈顶保留padding

[VERIFIED mykernel/arch/x86_64/sched/context/thread_info_const_arch.h::TOP_OF_KERNEL_STACK_PADDING]
```c
	#define TOP_OF_KERNEL_STACK_PADDING	0
```

### A10 — task结构栈指针成员

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c

```

### A11 — 初始化任务栈指向init_stack

[VERIFIED mykernel/init/init_task.c::init_task]
```c
task_s init_task __aligned(L1_CACHE_BYTES) = {
	.thread_info		= INIT_THREAD_INFO(init_task),
	// .stack_refcount		= REFCOUNT_INIT(1),
	.__state			= TASK_RUNNING,
	.stack				= init_stack,
```

### A12 — kmalloc申请线程栈

[VERIFIED mykernel/sched/forkexec/fork.c::alloc_thread_stack_node]
```c
	stack = kmalloc(THREAD_SIZE, GFP_KERNEL);
	if (!stack)
		return -ENOMEM;
```

### A13 — 将申请结果存入task.stack

[VERIFIED mykernel/sched/forkexec/fork.c::alloc_thread_stack_node]
```c
	// stack = kasan_reset_tag(stack);
	tsk->stack = stack;
	return 0;
```

### A14 — 调用栈申请函数

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	err = alloc_thread_stack_node(tsk, node);
	if (err)
		goto free_tsk;
```

### A15 — task_pt_regs中loff_t定义

[VERIFIED mykernel/include/linux/kernel/types.h::loff_t]
```c
			typedef __kernel_loff_t		loff_t;
```

### A16 — loff_t下层有符号整数定义

[VERIFIED mykernel/include/uapi/asm-generic/posix_types.h::__kernel_loff_t]
```c
	typedef long long			__kernel_loff_t;
```

### A17 — 已有生成头中的结构大小

[VERIFIED mykernel/include/generated/asm-offsets.h::PTREGS_SIZE]
```c
	#define PTREGS_SIZE 176
```

### A18 — 帧尾成员的真实结构定义

[VERIFIED mykernel/arch/x86_64/include/asm/ptrace.h::pt_regs_s]
```c
			reg_t	ip;
			reg_t	cs;
			reg_t	flags;
			reg_t	sp;
			reg_t	ss;
```

## B 分组

### B01 — 独立perCPU current_task定义

[VERIFIED mykernel/arch/x86_64/smp/percpu_area.c::current_task]
```c
DEFINE_PER_CPU_CACHE_HOT(task_s *, current_task) = &init_task;
```

### B02 — pcpu_hot成员初始化

[VERIFIED mykernel/arch/x86_64/smp/percpu_area.c::pcpu_hot]
```c
DEFINE_PER_CPU_CACHE_ALIGNED(pcpu_hot_s, pcpu_hot) = {
	.current_task	= &init_task,
	.preempt_count	= INIT_PREEMPT_COUNT,
	.top_of_stack	= TOP_OF_INIT_STACK,
};
```

### B03 — 每CPU TSS初始化器入口

[VERIFIED mykernel/arch/x86_64/smp/percpu_area.c::cpu_tss_rw]
```c
DEFINE_PER_CPU_PAGE_ALIGNED(struct tss_struct, cpu_tss_rw)  = {
	.x86_tss = {
```

### B04 — TSS.sp0初值与io_bitmap_base

[VERIFIED mykernel/arch/x86_64/smp/percpu_area.c::cpu_tss_rw]
```c
		.sp0				= (1UL << (BITS_PER_LONG-1)) + 1,
		.io_bitmap_base		= IO_BITMAP_OFFSET_INVALID,
	 },
```

### B05 — TSS硬件结构sp0和sp1

[VERIFIED mykernel/arch/x86_64/processor/processor_types_arch.h::x86_hw_tss]
```c
	struct x86_hw_tss {
		u32		reserved1;
		u64		sp0;
		u64		sp1;

```

### B06 — TSS.sp2独立成员

[VERIFIED mykernel/arch/x86_64/processor/processor_types_arch.h::x86_hw_tss]
```c
		 * the user RSP value.
		 */
		u64		sp2;
```

### B07 — struct tss_struct定义开始

[VERIFIED mykernel/arch/x86_64/processor/processor_types_arch.h::tss_struct]
```c
	struct tss_struct {
		/*
		 * The fixed hardware portion.  This must not cross a page boundary
		 * at risk of violating the SDM's advice and potentially triggering
```

### B08 — TSS嵌入x86_tss字段

[VERIFIED mykernel/arch/x86_64/processor/processor_types_arch.h::tss_struct]
```c
		x86_hw_tss_s	x86_tss;

		x86_io_bitmap_s	io_bitmap;
	} __aligned(PAGE_SIZE);
```

### B09 — 异常初始化函数开头

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::cpu_init_exception_handling]
```c
void cpu_init_exception_handling(void)
{
	// struct tss_struct *tss = this_cpu_ptr(&cpu_tss_rw);
	int cpu = smp_processor_id();

```

### B10 — cpu TSS地址与load_TR请求

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::cpu_init_exception_handling]
```c
	set_tss_desc(cpu, &(per_cpu(cpu_tss_rw, cpu).x86_tss));

	load_TR_desc();
```

### B11 — 随后load_current_idt请求

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::cpu_init_exception_handling]
```c
	// /* Finally load the IDT */
	load_current_idt();
}
```

### B12 — 独立cpu_init与curr

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::cpu_init]
```c
void cpu_init(void)
{
	task_s *curr = current;
	int cpu = smp_processor_id();

```

### B13 — cpu_init调用syscall_init

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::cpu_init]
```c
	loadsegment(fs, 0);
	// memset(cur->thread.tls_array, 0, GDT_ENTRY_TLS_ENTRIES * 8);
	syscall_init();
```

### B14 — 旧trampoline注释不可当活动代码

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::cpu_init]
```c
	// /*
	//  * sp0 points to the entry trampoline stack regardless of what task
	//  * is running.
	//  */
	// load_sp0((unsigned long)(cpu_entry_stack(cpu) + 1));
```

### B15 — cpu_init末尾GDT调用

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::cpu_init]
```c
	load_direct_gdt(cpu);
}
```

### B16 — 取指定CPU的GDT

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::get_cpu_gdt_rw]
```c
		PREFIX_STATIC_INLINE
		desc_s
		*get_cpu_gdt_rw(uint cpu) {
			return per_cpu(gdt_page, cpu).gdt;
		}
```

### B17 — TSS descriptor填充入口

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::set_tssldt_descriptor]
```c
		set_tssldt_descriptor(void *d, ulong addr,
				uint type, uint size) {

			struct ldttss_desc *desc = d;
			memset(desc, 0, sizeof(*desc));
```

### B18 — TSS descriptor低base和present

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::set_tssldt_descriptor]
```c
			desc->limit0	= (u16) size;
			desc->base0		= (u16) addr;
			desc->base1		= (addr >> 16) & 0xFF;
			desc->type		= type;
			desc->p			= 1;
```

### B19 — TSS descriptor高base

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::set_tssldt_descriptor]
```c
			desc->limit1	= (size >> 16) & 0xF;
			desc->base2		= (addr >> 24) & 0xFF;
			desc->base3		= (u32) (addr >> 32);
		}
```

### B20 — 选GDT与TSS descriptor局部

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::set_tss_desc]
```c
		void
		set_tss_desc(uint cpu, x86_hw_tss_s *addr) {
			desc_s *d = get_cpu_gdt_rw(cpu);
			tss_desc tss;
```

### B21 — TSS目标地址写入GDT

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::set_tss_desc]
```c
			set_tssldt_descriptor(&tss, (ulong)addr,
					DESC_TSS, __KERNEL_TSS_LIMIT);
			write_gdt_entry(d, GDT_ENTRY_TSS, &tss, DESC_TSS);
		}
```

### B22 — 根据描述符类型选择大小

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::write_gdt_entry]
```c
		write_gdt_entry(desc_s *gdt, int entry,
				const void *desc, int type) {
			uint size;

			switch (type) {
```

### B23 — DESC_TSS决定复制大小

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::write_gdt_entry]
```c
				case DESC_TSS:	size = sizeof(tss_desc);	break;
				case DESC_LDT:	size = sizeof(ldt_desc);	break;
				default:		size = sizeof(*gdt);		break;
```

### B24 — 按类型memcpy写GDT

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::write_gdt_entry]
```c
			memcpy(&gdt[entry], desc, size);
		}
```

### B25 — TR加载函数和先装载GDT

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::load_TR_desc]
```c
		void
		load_TR_desc(void) {
			int cpu = smp_processor_id();
			load_direct_gdt(cpu);
			asm volatile(	"ltr	%w0		\t\n"
```

### B26 — inline ltr请求

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::load_TR_desc]
```c
			asm volatile(	"ltr	%w0		\t\n"
						:
						:	"q" (GDT_ENTRY_TSS*8)
						);
		}
```

### B27 — 直接更新当前CPU TSS.sp0

[VERIFIED mykernel/arch/x86_64/processor/processor_arch.h::load_sp0]
```c
		load_sp0(ulong sp0) {
			// native_load_sp0(unsigned long sp0) {
				this_cpu_ptr(&cpu_tss_rw)->x86_tss.sp0 = sp0;
			// }
		}
```

### B28 — task栈更新函数开头

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_arch.h::update_task_stack]
```c
		PREFIX_STATIC_INLINE
		void
		update_task_stack(task_s *task) {
```

### B29 — 活动load_sp0调用

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_arch.h::update_task_stack]
```c
			// if (cpu_feature_enabled(X86_FEATURE_XENPV))
				load_sp0(task_top_of_stack(task));
		}
```

### B30 — next_p与CPU对象

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::__switch_to]
```c
*__switch_to(task_s *prev_p, task_s *next_p)
{
	thread_s *prev = &prev_p->thread;
	thread_s *next = &next_p->thread;
	int cpu = smp_processor_id();
```

### B31 — 先发布current_task和top

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::__switch_to]
```c
	*this_cpu_ptr(&pcpu_hot.current_task) = next_p;
	*this_cpu_ptr(&pcpu_hot.top_of_stack) = task_top_of_stack(next_p);

```

### B32 — 后发布TSS.sp0

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::__switch_to]
```c
	/* Reload sp0. */
	update_task_stack(next_p);
```

### B33 — 当前任务与栈顶分属pcpu热点成员

[VERIFIED mykernel/arch/x86_64/include/asm/current.h::pcpu_hot_s]
```c
				task_s		*current_task;
				int			preempt_count;
				int			cpu_number;
				u64			call_depth;
				ulong		top_of_stack;
```

## C 分组

### C01 — 调用__switch_to_asm的switch_to宏

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_macro_arch.h::switch_to]
```c
	#define switch_to(prev, next, last)						\
			do {											\
				((last) = __switch_to_asm((prev), (next)));	\
			} while (0)
```

### C02 — inactive frame前部成员

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_types_arch.h::inactive_task_frame]
```c
	struct inactive_task_frame {
		reg_t	r15;
		reg_t	r14;
		reg_t	r13;
		reg_t	r12;
```

### C03 — inactive frame尾部bp和返回地址

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_types_arch.h::inactive_task_frame]
```c
		reg_t	bp;
		reg_t	ret_addr;
	};
```

### C04 — 汇编上下文切换唯一标签

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::__switch_to_asm]
```asm
SYM_FUNC_START(__switch_to_asm)
	/*
```

### C05 — 保存6个callee寄存器的前五个

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::__switch_to_asm]
```asm
	pushq		%rbp
	pushq		%rbx
	pushq		%r12
	pushq		%r13
	pushq		%r14
```

### C06 — 保存r15第六个寄存器

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::__switch_to_asm]
```asm
	pushq		%r15
```

### C07 — 保存旧thread.sp并从next取新rsp

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::__switch_to_asm]
```asm
	movq		%rsp,		TASK_threadsp(%rdi)
	movq		TASK_threadsp(%rsi),	%rsp
```

### C08 — 恢复新栈前五个callee寄存器

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::__switch_to_asm]
```asm
	/* restore callee-saved registers */
	popq		%r15
	popq		%r14
	popq		%r13
	popq		%r12
```

### C09 — 恢复rbp并跳转__switch_to

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::__switch_to_asm]
```asm
	popq		%rbx
	popq		%rbp

	jmp			__switch_to
SYM_FUNC_END(__switch_to_asm)
```

### C10 — 已生成thread.sp结构偏移

[VERIFIED mykernel/include/generated/asm-offsets.h::TASK_threadsp]
```c
	#define TASK_threadsp 552
```

### C11 — TASK_threadsp的生成表达

[VERIFIED mykernel/arch/x86_64/kbuild/asm-offsets.c::common]
```c
static void __used common(void)
{
	BLANK();
	OFFSET(TASK_threadsp, task_struct, thread.sp);
// #ifdef CONFIG_STACKPROTECTOR
```

### C12 — thread_struct保存栈指针字段

[VERIFIED mykernel/arch/x86_64/processor/processor_types_arch.h::thread_struct]
```c
	struct thread_struct {
		/* Cached TLS descriptors: */
		// desc_s tls_array[GDT_ENTRY_TLS_ENTRIES];
		reg_t			sp;
		ushort			es;
```

### C13 — SYSCALL入口与swapgs

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::entry_SYSCALL_64]
```asm
SYM_CODE_START(entry_SYSCALL_64)
	cli
	endbr64

	swapgs
```

### C14 — 用户rsp暂存TSS.sp2并选择hot.top_of_stack

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::entry_SYSCALL_64]
```asm
	swapgs
	/* tss.sp2 is scratch space. */
	movq		%rsp,		PER_CPU_VAR(cpu_tss_rw + TSS_sp2)
	// SWITCH_TO_KERNEL_CR3 scratch_reg=%rsp
	movq		PER_CPU_VAR(pcpu_hot + X86_top_of_stack),	%rsp
```

### C15 — 构造软件系统调用返回帧

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::entry_SYSCALL_64]
```asm
	pushq		$__USER_DS				/* pt_regs->ss */
	pushq		PER_CPU_VAR(cpu_tss_rw + TSS_sp2)	/* pt_regs->sp */
	pushq		%r11					/* pt_regs->flags */
	pushq		$__USER_CS				/* pt_regs->cs */
	pushq		%rcx					/* pt_regs->ip */
```

### C16 — 保存系统调用号与软件frame

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::entry_SYSCALL_64]
```asm
	pushq		%rax					/* pt_regs->orig_ax */
	pushq		$0x80					/* pt_regs->irq_nr */
	PUSH_REGS
	sti
```

### C17 — 异常入口保存寄存器不显式切栈

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::sa_entintr_retp]
```asm
SYM_CODE_START(sa_entintr_retp)
	cli
	PUSH_REGS
	testb		$3,			CS(%rsp)
```

### C18 — 异常入口将rsp传递给C并开中断

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::sa_entintr_retp]
```asm
	cld
	movq		%rsp,		%rdi
	sti

	callq 		excep_hwint_context
```

### C19 — TSS.sp2偏移

[VERIFIED mykernel/include/generated/asm-offsets.h::TSS_sp2]
```c
	#define TSS_sp2 20
```

### C20 — 每CPU hotspot栈顶偏移

[VERIFIED mykernel/include/generated/asm-offsets.h::X86_top_of_stack]
```c
	#define X86_top_of_stack 24
```

### C21 — current_task的percpu读取定义

[VERIFIED mykernel/arch/x86_64/include/asm/current.h::get_current]
```c
	static __always_inline task_s *get_current(void) {
		return *this_cpu_ptr(&pcpu_hot.current_task);
	}
```

### C22 — __switch_to源码正常返回条件

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::__switch_to]
```c
	return prev_p;
}
```

