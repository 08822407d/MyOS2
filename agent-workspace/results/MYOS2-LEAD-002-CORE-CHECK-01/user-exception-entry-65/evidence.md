# CORE-USER-EXCEPTION-ENTRY-65 / fixed-time evidence

Each quoted snippet is copied as an unmodified contiguous 1–5 physical-line interval from the named definition of `time@a039d9803ade`; blank lines and tabs count. Only source text is verified: preprocessing, compilation, link, CPU and runtime are NOT_RUN.

### D01 — correct definition start (L643–645)

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::arch_cpu_finalize_init]
```c
void __init arch_cpu_finalize_init(void)
{
	// struct cpuinfo_x86 *c = this_cpu_ptr(&cpu_info);
```

### D02 — old C23 contiguous quote with blank lines (L689–693)

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::arch_cpu_finalize_init]
```c
	// *c = boot_cpu_data;
	// c->initialized = true;

	// alternative_instructions();

```

### D03 — cpu_init actual end (L637–639)

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::cpu_init]
```c

	load_direct_gdt(cpu);
}
```

### D04 — other alternative invocation commented (L60–63)

[VERIFIED mykernel/arch/x86_64/kernel/cpu/bugs.c::check_bugs]
```c
	// mds_print_mitigation();
	// arch_smt_update();
	// alternative_instructions();
	// /*
```

### A01 — IDT table and descriptor (L27–31)

[VERIFIED mykernel/arch/x86_64/kernel/idt.c::idt_table/idt_descr]
```c
static gate_desc idt_table[IDT_ENTRIES] __page_aligned_bss;
struct desc_ptr idt_descr __ro_after_init = {
	.size		= IDT_TABLE_SIZE - 1,
	.address	= (unsigned long)&idt_table,
};
```

### A02 — constructor call site (L39–43)

[VERIFIED mykernel/arch/x86_64/kernel/idt.c::set_intr_gate]
```c
static __init void set_intr_gate(unsigned int n, const void *addr)
{
	struct idt_data data;
	gate_desc desc;

```

### A03 — idt_data source (L44–45)

[VERIFIED mykernel/arch/x86_64/kernel/idt.c::set_intr_gate]
```c
	init_idt_data(&data, n, addr);

```

### A04 — descriptor write (L48–49)

[VERIFIED mykernel/arch/x86_64/kernel/idt.c::set_intr_gate]
```c
		idt_init_desc(&desc, &data);
		write_idt_entry(idt_table, data.vector, &desc);
```

### A05 — exception stub addresses (L57–61)

[VERIFIED mykernel/arch/x86_64/kernel/idt.c::idt_setup_early_handler]
```c
void __init idt_setup_early_handler(void)
{
	for (int i = 0; i < NUM_EXCEPTION_VECTORS; i++) {
		void *entry = idt_handler_array + IDT_ALIGN * i;
		set_intr_gate(i, entry);
```

### A06 — early load request (L61–64)

[VERIFIED mykernel/arch/x86_64/kernel/idt.c::idt_setup_early_handler]
```c
		set_intr_gate(i, entry);
	}
	load_idt(&idt_descr);
}
```

### A07 — later setup definition (L70–74)

[VERIFIED mykernel/arch/x86_64/kernel/idt.c::idt_setup_apic_and_irq_gates]
```c
void __init idt_setup_apic_and_irq_gates(void)
{
	int i;
	void *entry;

```

### A08 — later exception entries (L78–82)

[VERIFIED mykernel/arch/x86_64/kernel/idt.c::idt_setup_apic_and_irq_gates]
```c
	for (i = 0; i < NUM_EXCEPTION_VECTORS; i++)
	{
		entry = idt_handler_array + IDT_ALIGN * i;
		set_intr_gate(i, entry);
	}
```

### A09 — external interrupt entries (L85–89)

[VERIFIED mykernel/arch/x86_64/kernel/idt.c::idt_setup_apic_and_irq_gates]
```c
	for (i = FIRST_EXTERNAL_VECTOR; i < NR_VECTORS; i++)
	{
		entry = irq_entries_start + IDT_ALIGN * (i - FIRST_EXTERNAL_VECTOR);
		set_intr_gate(i, entry);
	}
```

### A10 — later load request (L92–95)

[VERIFIED mykernel/arch/x86_64/kernel/idt.c::idt_setup_apic_and_irq_gates]
```c
	// /* Map IDT into CPU entry area and reload it. */
	// idt_map_in_cea();
	load_idt(&idt_descr);

```

### A11 — real data definition (L248–252)

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::init_idt_data]
```c
		PREFIX_STATIC_INLINE
		void
		init_idt_data(struct idt_data *data,
				uint n, const void *addr) {

```

### A12 — vector address and type (L255–259)

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::init_idt_data]
```c
			memset(data, 0, sizeof(*data));
			data->vector		= n;
			data->addr			= addr;
			data->segment		= __KERNEL_CS;
			data->bits.type		= GATE_INTERRUPT;
```

### A13 — real packing definition (L263–267)

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::idt_init_desc]
```c
		PREFIX_STATIC_INLINE
		void
		idt_init_desc(gate_desc *gate, const struct idt_data *d) {
			ulong addr	= (ulong) d->addr;

```

### A14 — gate fields (L269–273)

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::idt_init_desc]
```c
			gate->segment		= (u16) d->segment;
			gate->bits			= d->bits;
			gate->offset_middle	= (u16) (addr >> 16);
			gate->offset_high	= (u32) (addr >> 32);
			gate->reserved		= 0;
```

### A15 — memory write (L108–112)

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::write_idt_entry]
```c
		PREFIX_STATIC_INLINE
		void
		write_idt_entry(gate_desc *idt, int entry, const gate_desc *gate) {
			memcpy(&idt[entry], gate, sizeof(*gate));
		}
```

### A16 — lidt instruction source (L185–189)

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::load_idt]
```c
		// static __always_inline void native_load_idt(const struct desc_ptr *dtr)
		PREFIX_STATIC_AWLWAYS_INLINE
		void
		load_idt(const struct desc_ptr *dtr) {
			asm volatile(	"lidt	%0		\t\n"
```

### A17 — source 16-byte spacing (L11–12)

[VERIFIED mykernel/arch/x86_64/include/asm/idtentry.h::IDT_ALIGN]
```c
	#define HAS_KERNEL_IBT	0
	#define IDT_ALIGN	(8 * (2 + HAS_KERNEL_IBT))
```

### A18 — IDT sizes (L167–168)

[VERIFIED mykernel/arch/x86_64/processor/desc_const_arch.h::IDT_ENTRIES/NUM_EXCEPTION_VECTORS]
```c
	#define IDT_ENTRIES				256
	#define NUM_EXCEPTION_VECTORS	32
```

### A19 — configured error-code mask (L185–185)

[VERIFIED mykernel/arch/x86_64/processor/desc_const_arch.h::EXCEPTION_ERRCODE_MASK]
```c
	#define EXCEPTION_ERRCODE_MASK	0x20027d00
```

### A20 — trap vector numbers (L20–21)

[VERIFIED mykernel/arch/x86_64/include/asm/trapnr.h::X86_TRAP_GP/X86_TRAP_PF]
```c
	#define X86_TRAP_GP			13	/* General Protection Fault */
	#define X86_TRAP_PF			14	/* Page Fault */
```

### A21 — C vector numbers (L22–23)

[VERIFIED mykernel/arch/x86_64/include/obsolete/myos_irq_vectors.h::GEN_PROT_VEC/PAGE_FAULT_VEC]
```c
	#define GEN_PROT_VEC				13		/* general protection */
	#define PAGE_FAULT_VEC				14
```

### A22 — external boundary (L35–38)

[VERIFIED mykernel/arch/x86_64/include/asm/irq_vectors.h::FIRST_EXTERNAL_VECTOR]
```c
	 * IDT vectors usable for external interrupt sources start at 0x20.
	 * (0x80 is the syscall vector, 0x30-0x3f are for ISA)
	 */
	#define FIRST_EXTERNAL_VECTOR			0x20
```

### A23 — stub array start (L330–334)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::idt_handler_array]
```asm
/* hardware exceptions entries */
	.align IDT_ALIGN
SYM_CODE_START(idt_handler_array)
	vector = 0
	.rept		NUM_EXCEPTION_VECTORS
```

### A24 — conditional zero error slot (L335–339)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::idt_handler_array]
```asm
	/* Push 0 for Error Number placeholder */
	.if ((EXCEPTION_ERRCODE_MASK >> vector) & 1) == 0
		endbr64
		pushq	$0		# Dummy error code, to make stack frame uniform
	.else
```

### A25 — push vector and branch (L340–344)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::idt_handler_array]
```asm
		endbr64
	.endif
	pushq		$vector		# 72(%rsp) Vector number
	jmp			sa_entintr_retp
	vector = vector + 1
```

### A26 — slot fill (L345–347)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::idt_handler_array]
```asm
	.fill		idt_handler_array + vector*IDT_ALIGN - ., 1, 0xcc
	.endr
SYM_CODE_END(idt_handler_array)
```

### B01 — PUSH first registers (L47–51)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::PUSH_REGS]
```asm
.macro PUSH_REGS
	pushq		%rdi
	pushq		%rsi
	pushq		%rdx
	pushq		%rcx
```

### B02 — PUSH middle registers (L53–57)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::PUSH_REGS]
```asm
	pushq		%r8
	pushq		%r9
	pushq		%r10
	pushq		%r11
	pushq		%rbx
```

### B03 — PUSH last registers (L58–62)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::PUSH_REGS]
```asm
	pushq		%rbp
	pushq		%r12
	pushq		%r13
	pushq		%r14
	pushq		%r15	
```

### B04 — POP first registers (L65–69)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::POP_REGS]
```asm
.macro POP_REGS
	popq		%r15
	popq		%r14
	popq		%r13
	popq		%r12
```

### B05 — POP last registers (L77–81)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::POP_REGS]
```asm
	popq		%rcx
	popq		%rdx
	popq		%rsi
	popq		%rdi
.endm
```

### B06 — frame start (L9–13)

[VERIFIED mykernel/arch/x86_64/include/asm/ptrace.h::pt_regs_s]
```c
		typedef struct pt_regs {
		/*
		 * C ABI says these regs are callee-preserved. They aren't saved on kernel entry
		 * unless syscall needs a complete, fully filled "pt_regs_s".
		 */
```

### B07 — vector slot (L29–33)

[VERIFIED mykernel/arch/x86_64/include/asm/ptrace.h::pt_regs_s]
```c
			reg_t	di;

		// myos push irq vec number separately
			reg_t	irq_nr;
		/*
```

### B08 — error slot (L34–38)

[VERIFIED mykernel/arch/x86_64/include/asm/ptrace.h::pt_regs_s]
```c
		 * On syscall entry, this is syscall#. On CPU exception, this is error code.
		 * On hw interrupt, it's IRQ number:
		 */
			reg_t	orig_ax;
		/* Return frame for iretq */
```

### B09 — return frame (L39–43)

[VERIFIED mykernel/arch/x86_64/include/asm/ptrace.h::pt_regs_s]
```c
			reg_t	ip;
			reg_t	cs;
			reg_t	flags;
			reg_t	sp;
			reg_t	ss;
```

### B10 — frame close (L44–45)

[VERIFIED mykernel/arch/x86_64/include/asm/ptrace.h::pt_regs_s]
```c
		/* top of stack page */
		} pt_regs_s;
```

### B11 — 64bit reg_t (L13–17)

[VERIFIED mykernel/include/linux/kernel/types.h::reg_t]
```c
	// #ifdef DEBUG
	// 	typedef void			*reg_t;
	// #else
		typedef __UINT64_TYPE__ reg_t;
	// #endif
```

### B12 — generated offsets (L43–47)

[VERIFIED mykernel/include/generated/asm-offsets.h::IRQ_NR/ORIG_RAX/RIP/CS/EFLAGS]
```c
	#define IRQ_NR 120
	#define ORIG_RAX 128
	#define RIP 136
	#define CS 144
	#define EFLAGS 152
```

### B13 — return tail offsets (L48–49)

[VERIFIED mykernel/include/generated/asm-offsets.h::RSP/SS]
```c
	#define RSP 160
	#define SS 168
```

### B14 — generating source only (L64–68)

[VERIFIED mykernel/arch/x86_64/kbuild/asm-offsets_64.c::OFFSET macro expansions]
```c
	OFFSET(IRQ_NR, pt_regs, irq_nr);
	OFFSET(ORIG_RAX, pt_regs, orig_ax);
	OFFSET(RIP, pt_regs, ip);
	OFFSET(CS, pt_regs, cs);
	OFFSET(EFLAGS, pt_regs, flags);
```

### B15 — common entry (L306–310)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::sa_entintr_retp]
```asm
SYM_CODE_START(sa_entintr_retp)
	cli
	PUSH_REGS
	testb		$3,			CS(%rsp)
	jz	1f
```

### B16 — argument register and sti (L313–317)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::sa_entintr_retp]
```asm
	cld
	movq		%rsp,		%rdi
	sti

	callq 		excep_hwint_context
```

### B17 — vector read (L119–123)

[VERIFIED mykernel/arch/x86_64/myos/interrupt.c::excep_hwint_context]
```c
void excep_hwint_context(pt_regs_s *regs)
{
	unsigned long vec = (unsigned long)regs->irq_nr;

	if (vec < FIRST_EXTERNAL_VECTOR)
```

### B18 — exception vs IRQ (L124–128)

[VERIFIED mykernel/arch/x86_64/myos/interrupt.c::excep_hwint_context]
```c
		exception_handler(regs);
	else
		hwint_irq_handler(regs);

	if (!in_atomic())
```

### B19 — switch vector (L132–136)

[VERIFIED mykernel/arch/x86_64/myos/interrupt.c::exception_handler]
```c
void exception_handler(pt_regs_s *regs)
{
	unsigned long vec = (unsigned long)regs->irq_nr;

	switch (vec)
```

### B20 — GP PF cases (L148–152)

[VERIFIED mykernel/arch/x86_64/myos/interrupt.c::exception_handler]
```c
		excep_gen_prot(regs);
		break;
	case PAGE_FAULT_VEC:
		exc_page_fault(regs, regs->orig_ax);
		break;
```

### B21 — PF CR2 read (L415–419)

[VERIFIED mykernel/arch/x86_64/mm/fault.c::exc_page_fault]
```c
__visible noinstr void
exc_page_fault(pt_regs_s *regs, ulong error_code)
{
	ulong address = read_cr2();
	// irqentry_state_t state;
```

### B22 — PF page handler (L459–463)

[VERIFIED mykernel/arch/x86_64/mm/fault.c::exc_page_fault]
```c
	// instrumentation_begin();
	handle_page_fault(regs, error_code, address);
	// instrumentation_end();

	// irqentry_exit(regs, state);
```

### B23 — high-low address dispatch (L398–402)

[VERIFIED mykernel/arch/x86_64/mm/fault.c::handle_page_fault]
```c
	/* Was the fault on kernel-controlled part of the address space? */
	if (unlikely(fault_in_kernel_space(address))) {
		do_kern_addr_fault(regs, err_code, address);
	} else {
		do_user_addr_fault(regs, err_code, address);
```

### C01 — post-C swapgs test (L317–321)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::sa_entintr_retp]
```asm
	callq 		excep_hwint_context

	testb		$3,			CS(%rsp)
	jz	2f
	swapgs
```

### C02 — normal return sequence (L323–327)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::sa_entintr_retp]
```asm
	cli
	POP_REGS
	addq		$0x10,		%rsp	// skip err-code and vec-nr
	sti
	iretq
```

### C03 — conditional schedule (L126–130)

[VERIFIED mykernel/arch/x86_64/myos/interrupt.c::excep_hwint_context]
```c
		hwint_irq_handler(regs);

	if (!in_atomic())
		schedule();
}
```

### C04 — in_atomic macro (L60–64)

[VERIFIED mykernel/sched/scheduler/preempt_macro.h::in_atomic]
```c
	 * Do not use in_atomic() in driver code.
	 */
	#define in_atomic()	(				\
				preempt_count() != 0	\
			)
```

### C05 — GP handler diagnostic (L91–95)

[VERIFIED mykernel/arch/x86_64/myos/interrupt.c::excep_gen_prot]
```c
void excep_gen_prot(pt_regs_s *regs)
{
	unsigned long error_code = (unsigned long)regs->orig_ax;
	color_printk(RED,BLACK,"do_general_protection(13),ERROR_CODE:%#018lx , code address:%#018lx\n",error_code, regs->ip);

```

### C06 — GP while (L109–113)

[VERIFIED mykernel/arch/x86_64/myos/interrupt.c::excep_gen_prot]
```c

	color_printk(RED,BLACK,"Segment Selector Index:%#010x\n",error_code & 0xfff8);

	while (1);
}
```

### C07 — PF high-address while (L150–154)

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_kern_addr_fault]
```c
	//  */
	// bad_area_nosemaphore(regs, hw_err_code, address);

	while (1);
}
```

### C08 — PF result while (L322–325)

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	// fault = handle_mm_fault(vma, address, flags, regs);
	fault = myos_handle_mm_fault(vma, address, flags);
	while (fault != ENOERR);

```

### C09 — user IRQ gate (L235–239)

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	if (user_mode(regs)) {
		local_irq_enable();
		flags |= FAULT_FLAG_USER;
	} else {
		if (regs->flags & X86_EFLAGS_IF)
```

### C10 — user CS test (L81–83)

[VERIFIED mykernel/arch/x86_64/include/asm/ptrace.h::user_mode]
```c
		static __always_inline int user_mode(pt_regs_s *regs) {
			return !!(regs->cs & 3);
		}
```

