---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-UACCESS-RECOVERY-64
record_type: bounded_pinned_source_quotes
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
produced_by: GPT-6; backend unknown_or_not_attestable
base_snapshot: time
source_pin_short12: a039d9803ade
result_base_short12: 51477ed51ee2
read_channel: GitHub_connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---
# Batch64 D00 A B C source evidence

Each quote is within a single definition, with its original physical blank lines and at most five lines. Comments, macros, and link scripts must not be interpreted as runtime behavior.
## D00 追加证据：第63批C14/C15/C16重新审定

D01→原C14，D02+D03→原C15，D04→原C16；这四段是真实新片段，不将主线回执里虚构的空行插入源。原C14/C15/C16本身均逐字连续合格，详见事实矩阵；R63-01仅由C05裁定。

## A 异常记录源宏与copy调用锚

### D01 filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
		return -EINVAL;
	prev_reclen = buf->prev_reclen;
	dirent = buf->current_dir;
	prev = (void *)dirent - prev_reclen;
```

### D02 filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	prev->d_off = offset;
	dirent->d_ino = ino;
	dirent->d_reclen = reclen;
```

### D03 filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	dirent->d_type = d_type;
	cplen = copy_to_user(dirent->d_name, (char *)name, namelen);
```

### D04 filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	cplen = copy_to_user(dirent->d_name, (char *)name, namelen);
	dirent->d_name[namelen] = '\0';

	buf->prev_reclen = reclen;
	buf->current_dir = (void *)dirent + reclen;
```

### A01 _ASM_EXTABLE_TYPE

[VERIFIED mykernel/arch/x86_64/include/asm/asm.h::_ASM_EXTABLE_TYPE]
```c
	#    define _ASM_EXTABLE_TYPE(from, to, type)		\
					.pushsection "__ex_table","a" ;		\
					.balign 4 ;							\
					.long (from) - . ;					\
					.long (to) - . ;					\
```

### A02 _ASM_EXTABLE_TYPE

[VERIFIED mykernel/arch/x86_64/include/asm/asm.h::_ASM_EXTABLE_TYPE]
```c
	#    define _ASM_EXTABLE_TYPE(from, to, type)				\
					" .pushsection \"__ex_table\",\"a\"\n"		\
					" .balign 4\n"								\
					" .long (" #from ") - .\n"					\
					" .long (" #to ") - .\n"					\
```

### A03 _ASM_EXTABLE_TYPE

[VERIFIED mykernel/arch/x86_64/include/asm/asm.h::_ASM_EXTABLE_TYPE]
```c
					" .long (" #from ") - .\n"					\
					" .long (" #to ") - .\n"					\
					" .long " __stringify(type) " \n"			\
					" .popsection\n"
```

### A04 _ASM_EXTABLE_UA

[VERIFIED mykernel/arch/x86_64/include/asm/asm.h::_ASM_EXTABLE_UA]
```c
	#  define _ASM_EXTABLE_UA(from, to)	\
				_ASM_EXTABLE_TYPE(from, to, EX_TYPE_UACCESS)
```

### A05 _ASM_EXTABLE_TYPE_REG

[VERIFIED mykernel/arch/x86_64/include/asm/asm.h::_ASM_EXTABLE_TYPE_REG]
```c
	#    define _ASM_EXTABLE_TYPE_REG(from, to, type, reg)		\
					" .pushsection \"__ex_table\",\"a\"\n"		\
					" .balign 4\n"								\
					" .long (" #from ") - .\n"					\
					" .long (" #to ") - .\n"					\
```

### A06 _ASM_EXTABLE_TYPE_REG

[VERIFIED mykernel/arch/x86_64/include/asm/asm.h::_ASM_EXTABLE_TYPE_REG]
```c
					DEFINE_EXTABLE_TYPE_REG						\
					"extable_type_reg reg=" __stringify(reg)	\
						", type=" __stringify(type) " \n"		\
					UNDEFINE_EXTABLE_TYPE_REG					\
					" .popsection\n"
```

### A07 copy_user_generic

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::copy_user_generic]
```c
	static __always_inline __must_check ulong
	copy_user_generic(void *to, const void *from, ulong len) {
		stac();
		/*
		 * If CPU has FSRM feature, use 'rep movs'.
```

### A08 copy_user_generic

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::copy_user_generic]
```c
		asm volatile(	"1:\n\t"
						ALTERNATIVE("rep movsb",
							"call rep_movs_alternative",
							ALT_NOT(X86_FEATURE_FSRM)
						)
```

### A09 copy_user_generic

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::copy_user_generic]
```c
						"2:\n"
						_ASM_EXTABLE_UA(1b, 2b)
					:	"+c" (len), "+D" (to),
						"+S" (from), ASM_CALL_CONSTRAINT
					:
```

### A10 copy_user_generic

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::copy_user_generic]
```c
					:	"memory", "rax"
					);
		clac();
		return len;
	}
```

### A11 _ASM_EXTABLE_CPY

[VERIFIED mykernel/arch/x86_64/include/asm/asm.h::_ASM_EXTABLE_CPY]
```c
	#  define _ASM_EXTABLE_CPY(from, to)	\
				_ASM_EXTABLE_TYPE(from, to, EX_TYPE_COPY)
```

### A12 _ASM_EXTABLE

[VERIFIED mykernel/arch/x86_64/include/asm/asm.h::_ASM_EXTABLE]
```c
	#  define _ASM_EXTABLE(from, to)	\
				_ASM_EXTABLE_TYPE(from, to, EX_TYPE_DEFAULT)
```


## B 页故障分派、用户范围/权限级和返回边界

B段选定源级函数均按实际定义消费，不把被注释的extable/fixup/OOM门当已执行；它不包含CPU IDT绑定的认证。

### B01 fault_in_kernel_space

[VERIFIED mykernel/arch/x86_64/mm/fault.c::fault_in_kernel_space]
```c
bool fault_in_kernel_space(ulong address)
{
	/*
	 * On 64-bit systems, the vsyscall page is at an address above
	 * TASK_SIZE_MAX, but is not considered part of the kernel
```

### B02 fault_in_kernel_space

[VERIFIED mykernel/arch/x86_64/mm/fault.c::fault_in_kernel_space]
```c
	// if (IS_ENABLED(CONFIG_X86_64) && is_vsyscall_vaddr(address))
	// 	return false;

	return address >= TASK_SIZE_MAX;
}
```

### B03 fault_in_kernel_space

[VERIFIED mykernel/arch/x86_64/mm/fault.c::fault_in_kernel_space]
```c
	return address >= TASK_SIZE_MAX;
}
```

### B04 do_kern_addr_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_kern_addr_fault]
```c
static void
do_kern_addr_fault(pt_regs_s *regs, ulong hw_err_code, ulong address) {
	// /*
	//  * Protection keys exceptions only happen on user pages.  We
	//  * have no user pages in the kernel portion of the address
```

### B05 do_kern_addr_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_kern_addr_fault]
```c
	//  * fault we could otherwise deadlock:
	//  */
	// bad_area_nosemaphore(regs, hw_err_code, address);

	while (1);
```

### B06 do_kern_addr_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_kern_addr_fault]
```c
	//  */
	// bad_area_nosemaphore(regs, hw_err_code, address);

	while (1);
}
```

### B07 myos_bad_area

[VERIFIED mykernel/arch/x86_64/mm/fault.c::myos_bad_area]
```c
static noinline void
myos_bad_area(pt_regs_s *regs, ulong err_code, ulong address)
{
	char *buf = kmalloc(500, GFP_KERNEL);
	memset(buf, 0, 500);
```

### B08 myos_bad_area

[VERIFIED mykernel/arch/x86_64/mm/fault.c::myos_bad_area]
```c
	parse_PF_errcode(regs, address, buf);
	// if (!(err_code & (ARCH_PF_EC_WR & ~ARCH_PF_EC_P)))
	// {
		color_printk(RED, BLACK, buf);
		while (1);
```

### B09 parse_PF_errcode

[VERIFIED mykernel/arch/x86_64/mm/fault.c::parse_PF_errcode]
```c
static void
parse_PF_errcode(pt_regs_s *regs, ulong cr2, char *buf)
{
	char tempbuf[100];
	ulong error_code = (ulong)regs->orig_ax;
```

### B10 parse_PF_errcode

[VERIFIED mykernel/arch/x86_64/mm/fault.c::parse_PF_errcode]
```c
	memset(tempbuf, 0, 100);
	snprintf(tempbuf, 45, "do_page_fault(14),ERROR_CODE: %#018lx", error_code);
	strcat(buf, tempbuf);

	if(!(error_code & 0x01))
```

### B11 do_user_addr_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
static inline
void do_user_addr_fault(pt_regs_s *regs, ulong err_code, ulong address) {
	vma_s *vma;
	task_s *tsk;
	mm_s *mm;
```

### B12 do_user_addr_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	vm_fault_t fault;
	uint flags = FAULT_FLAG_DEFAULT;

	tsk = current;
	mm = tsk->mm;
```

### B13 do_user_addr_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	if (user_mode(regs)) {
		local_irq_enable();
		flags |= FAULT_FLAG_USER;
	} else {
		if (regs->flags & X86_EFLAGS_IF)
```

### B14 do_user_addr_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	 */
	if (err_code & X86_PF_SHSTK)
		flags |= FAULT_FLAG_WRITE;
	if (err_code & X86_PF_WRITE)
		flags |= FAULT_FLAG_WRITE;
```

### B15 do_user_addr_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c

	// vm_fault_s vmf = myos_dump_pagetable(address);
	vma = simple_find_vma(mm, address);
	if (unlikely(!vma)) {
		myos_bad_area(regs, err_code, address);
```

### B16 do_user_addr_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	vma = simple_find_vma(mm, address);
	if (unlikely(!vma)) {
		myos_bad_area(regs, err_code, address);
		return;
	}
```

### B17 do_user_addr_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	if (likely(vma->vm_start <= address))
		goto lock_mmap;
	if (unlikely(!(vma->vm_flags & VM_GROWSDOWN))) {
		myos_bad_area(regs, err_code, address);
		return;
```

### B18 do_user_addr_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	}
	if (unlikely(expand_stack(vma, address))) {
		myos_bad_area(regs, err_code, address);
		return;
	}
```

### B19 do_user_addr_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	// fault = handle_mm_fault(vma, address, flags, regs);
	fault = myos_handle_mm_fault(vma, address, flags);
	while (fault != ENOERR);

```

### B20 do_user_addr_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	// if (unlikely(access_error(error_code, vma))) {
	// 	bad_area_access_error(regs, error_code, address, vma);
	// 	return;
	// }
```

### B21 handle_page_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::handle_page_fault]
```c
static __always_inline void
handle_page_fault(pt_regs_s *regs, ulong err_code, ulong address) {

	// trace_page_fault_entries(regs, error_code, address);

```

### B22 handle_page_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::handle_page_fault]
```c
	/* Was the fault on kernel-controlled part of the address space? */
	if (unlikely(fault_in_kernel_space(address))) {
		do_kern_addr_fault(regs, err_code, address);
	} else {
		do_user_addr_fault(regs, err_code, address);
```

### B23 handle_page_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::handle_page_fault]
```c
		do_kern_addr_fault(regs, err_code, address);
	} else {
		do_user_addr_fault(regs, err_code, address);
		/*
		 * User address page fault handling might have reenabled
```

### B24 exc_page_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::exc_page_fault]
```c
__visible noinstr void
exc_page_fault(pt_regs_s *regs, ulong error_code)
{
	ulong address = read_cr2();
	// irqentry_state_t state;
```

### B25 exc_page_fault

[VERIFIED mykernel/arch/x86_64/mm/fault.c::exc_page_fault]
```c
	// instrumentation_begin();
	handle_page_fault(regs, error_code, address);
	// instrumentation_end();

	// irqentry_exit(regs, state);
```

### B26 TASK_SIZE_MAX

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::TASK_SIZE_MAX]
```c
	#define TASK_SIZE_MAX			task_size_max()
```

### B27 task_size_max

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::task_size_max]
```c
	#define task_size_max()			((_AC(1, UL) << __VIRTUAL_MASK_SHIFT) - PAGE_SIZE)
```


## C alternative元数据、stac/clac候选与启动调用处

alt_instructions宏是生成元数据/候选指令的source-level机制，启动路径的alternative_instructions调用是注释。不得将表项存在、CPU启用和真实patch动作混成同一个CALL。

### C01 ALT_NOT

[VERIFIED mykernel/arch/x86_64/include/asm/alternative.h::ALT_NOT]
```c
	#define ALT_NOT(feature)			((ALT_FLAG_NOT << ALT_FLAGS_SHIFT) | (feature))
```

### C02 ALT_FLAGS_SHIFT

[VERIFIED mykernel/arch/x86_64/include/asm/alternative.h::ALT_FLAGS_SHIFT]
```c
	#define ALT_FLAGS_SHIFT				16
```

### C03 ALT_FLAG_NOT

[VERIFIED mykernel/arch/x86_64/include/asm/alternative.h::ALT_FLAG_NOT]
```c
	#define ALT_FLAG_NOT				(1 << 0)
```

### C04 alt_slen

[VERIFIED mykernel/arch/x86_64/include/asm/alternative.h::alt_slen]
```c
	#define alt_slen		"772b-771b"
```

### C05 alt_total_slen

[VERIFIED mykernel/arch/x86_64/include/asm/alternative.h::alt_total_slen]
```c
	#define alt_total_slen		"773b-771b"
```

### C06 alt_rlen

[VERIFIED mykernel/arch/x86_64/include/asm/alternative.h::alt_rlen]
```c
	#define alt_rlen		"775f-774f"
```

### C07 OLDINSTR

[VERIFIED mykernel/arch/x86_64/include/asm/alternative.h::OLDINSTR]
```c
	#define OLDINSTR(oldinstr)						\
		"# ALT: oldinstr\n"						\
		"771:\n\t" oldinstr "\n772:\n"					\
		"# ALT: padding\n"						\
		".skip -(((" alt_rlen ")-(" alt_slen ")) > 0) * "		\
```

### C08 OLDINSTR

[VERIFIED mykernel/arch/x86_64/include/asm/alternative.h::OLDINSTR]
```c
		".skip -(((" alt_rlen ")-(" alt_slen ")) > 0) * "		\
			"((" alt_rlen ")-(" alt_slen ")),0x90\n"		\
		"773:\n"
```

### C09 ALTINSTR_ENTRY

[VERIFIED mykernel/arch/x86_64/include/asm/alternative.h::ALTINSTR_ENTRY]
```c
	#define ALTINSTR_ENTRY(ft_flags)					      \
		".pushsection .altinstructions,\"a\"\n"				      \
		" .long 771b - .\n"				/* label           */ \
		" .long 774f - .\n"				/* new instruction */ \
		" .4byte " __stringify(ft_flags) "\n"		/* feature + flags */ \
```

### C10 ALTINSTR_ENTRY

[VERIFIED mykernel/arch/x86_64/include/asm/alternative.h::ALTINSTR_ENTRY]
```c
		" .long 774f - .\n"				/* new instruction */ \
		" .4byte " __stringify(ft_flags) "\n"		/* feature + flags */ \
		" .byte " alt_total_slen "\n"			/* source len      */ \
		" .byte " alt_rlen "\n"				/* replacement len */ \
		".popsection\n"
```

### C11 ALTINSTR_REPLACEMENT

[VERIFIED mykernel/arch/x86_64/include/asm/alternative.h::ALTINSTR_REPLACEMENT]
```c
	#define ALTINSTR_REPLACEMENT(newinstr)		/* replacement */	\
		".pushsection .altinstr_replacement, \"ax\"\n"			\
		"# ALT: replacement\n"						\
		"774:\n\t" newinstr "\n775:\n"					\
		".popsection\n"
```

### C12 ALTERNATIVE

[VERIFIED mykernel/arch/x86_64/include/asm/alternative.h::ALTERNATIVE]
```c
	#define ALTERNATIVE(oldinstr, newinstr, ft_flags)			\
		OLDINSTR(oldinstr)						\
		ALTINSTR_ENTRY(ft_flags)					\
		ALTINSTR_REPLACEMENT(newinstr)
```

### C13 alternative

[VERIFIED mykernel/arch/x86_64/include/asm/alternative.h::alternative]
```c
	#define alternative(oldinstr, newinstr, ft_flags)			\
		asm_inline volatile(ALTERNATIVE(oldinstr, newinstr, ft_flags) : : : "memory")
```

### C14 __ASM_STAC

[VERIFIED mykernel/arch/x86_64/include/asm/smap.h::__ASM_STAC]
```c
	#define __ASM_STAC	".byte 0x0f,0x01,0xcb"
```

### C15 __ASM_CLAC

[VERIFIED mykernel/arch/x86_64/include/asm/smap.h::__ASM_CLAC]
```c
	#define __ASM_CLAC	".byte 0x0f,0x01,0xca"
```

### C16 stac

[VERIFIED mykernel/arch/x86_64/include/asm/smap.h::stac]
```c
	static __always_inline void
	stac(void) {
		/* Note: a barrier is implicit in alternative() */
		alternative("", __ASM_STAC, X86_FEATURE_SMAP);
	}
```

### C17 clac

[VERIFIED mykernel/arch/x86_64/include/asm/smap.h::clac]
```c
	static __always_inline void
	clac(void) {
		/* Note: a barrier is implicit in alternative() */
		alternative("", __ASM_CLAC, X86_FEATURE_SMAP);
	}
```

### C18 copy_user_generic

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::copy_user_generic]
```c
	copy_user_generic(void *to, const void *from, ulong len) {
		stac();
		/*
		 * If CPU has FSRM feature, use 'rep movs'.
		 * Otherwise, use rep_movs_alternative.
```

### C19 copy_user_generic

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::copy_user_generic]
```c
		asm volatile(	"1:\n\t"
						ALTERNATIVE("rep movsb",
							"call rep_movs_alternative",
							ALT_NOT(X86_FEATURE_FSRM)
						)
```

### C20 copy_user_generic

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::copy_user_generic]
```c
						"2:\n"
						_ASM_EXTABLE_UA(1b, 2b)
					:	"+c" (len), "+D" (to),
						"+S" (from), ASM_CALL_CONSTRAINT
					:
```

### C21 copy_user_generic

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::copy_user_generic]
```c
					:	"memory", "rax"
					);
		clac();
		return len;
	}
```

### C22 check_bugs

[VERIFIED mykernel/arch/x86_64/kernel/cpu/bugs.c::check_bugs]
```c
	//  */
	// mds_print_mitigation();
	// arch_smt_update();
	// alternative_instructions();
	// /*
```

### C23 cpu_init

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::cpu_init]
```c
	// *c = boot_cpu_data;
	// c->initialized = true;

	// alternative_instructions();

```

