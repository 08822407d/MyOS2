---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-EXEC-STACK-PLACEMENT-142"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-22"
conversation_display_name: "MYOS2-A-C10a 代码分析执行"
produced_by: "GPT-6.2（系统模型族标识；界面实际型号不可独立核实）"
model_per_owner: "次一档GPT-6.2 Sol；Extra/高档偏好"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
date: "2026-11-24"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
read_channel: "connector"
status: "AUTHOR_CONTENT_PREPARED_PENDING_PUBLICATION_AND_G04"
mainline_acceptance: "PENDING"
full_local_P9: "NOT_RUN"
open_questions: ["实际配置、C指针/对象合法性、vma前置、映射和真实运行仍未证"]
inputs_read: [{"path":"mykernel/scripts/options_flags.cmake","ref":"a039d9803ade","blob":"6019436417ad"},{"path":"mykernel/fs/vfs/binfmt_elf.c","ref":"a039d9803ade","blob":"579fb4e9fadd"},{"path":"mykernel/arch/x86_64/include/asm/elf.h","ref":"a039d9803ade","blob":"421649d6f3bc"},{"path":"mykernel/include/linux/kernel/elf.h","ref":"a039d9803ade","blob":"dd427f17f005"},{"path":"mykernel/include/uapi/linux/elf.h","ref":"a039d9803ade","blob":"eff5e1ff56b8"},{"path":"mykernel/include/uapi/linux/auxvec.h","ref":"a039d9803ade","blob":"166fa1066af1"},{"path":"mykernel/include/linux/fs/binfmts.h","ref":"a039d9803ade","blob":"002c2aec4aa8"},{"path":"mykernel/include/uapi/linux/binfmts.h","ref":"a039d9803ade","blob":"a60f4e7c53b0"},{"path":"mykernel/mm/misc/mm_misc_const.h","ref":"a039d9803ade","blob":"ec983b79166f"},{"path":"mykernel/include/linux/kernel/asm-generic/param.h","ref":"a039d9803ade","blob":"16bba89f98f5"},{"path":"mykernel/mm/misc/mm_misc_types.h","ref":"a039d9803ade","blob":"b9d93f504756"},{"path":"mykernel/arch/x86_64/sched/context/process_arch.c","ref":"a039d9803ade","blob":"4431545af5fb"},{"path":"mykernel/sched/forkexec/exec.c","ref":"a039d9803ade","blob":"0861ebae6fc2"},{"path":"mykernel/include/linux/kernel/types.h","ref":"a039d9803ade","blob":"ef531baa8317"}]
record_type: "phase2_source_evidence"
---

# 第142批新正式源码锚点

新窗1–5连续物理行，单定义内逐字原文，使用/定义、声明、配置、注释性质分开。D00旧129窗不复制为新标记，见facts.D00.R141-01.reconfirmation。给定模型推论见正文与机器件。

### E142-001 · stack_add
mykernel/fs/vfs/binfmt_elf.c:110–111；macro_definition；将sp转换elf_addr_t用户指针后减items；只取整数投影
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::STACK_ADD]
```c
#define STACK_ADD(sp, items)	\
			((elf_addr_t __user *)(sp) - (items))
```

### E142-002 · stack_round
mykernel/fs/vfs/binfmt_elf.c:112–113；macro_definition；指针差投影再转unsigned long清低4位
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::STACK_ROUND]
```c
#define STACK_ROUND(sp, items)	\
			(((unsigned long) (sp - items)) &~ 15UL)
```

### E142-003 · stack_alloc
mykernel/fs/vfs/binfmt_elf.c:114–114；macro_definition；局部sp参数减len
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::STACK_ALLOC]
```c
#define STACK_ALLOC(sp, len) (sp -= len)
```

### E142-004 · base_platform_default
mykernel/fs/vfs/binfmt_elf.c:122–122；macro_definition；ifndef默认NULL；按题设选
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::ELF_BASE_PLATFORM]
```c
#  define ELF_BASE_PLATFORM	NULL
```

### E142-005 · entry
mykernel/fs/vfs/binfmt_elf.c:129–133；active_function_fragment；mm、p、局部argc/envc及sp从bprm取值
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	mm_s *mm = current->mm;
	ulong p = bprm->p;
	int argc = bprm->argc;
	int envc = bprm->envc;
	elf_addr_t __user *sp = (void *)p;
```

### E142-006 · platform_decl
mykernel/fs/vfs/binfmt_elf.c:134–138；local_declaration；局部平台指针声明及内核平台串来源
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	elf_addr_t __user *u_platform;
	elf_addr_t __user *u_base_platform;
	elf_addr_t __user *u_rand_bytes;
	const char *k_platform = ELF_PLATFORM;
	const char *k_base_platform = ELF_BASE_PLATFORM;
```

### E142-007 · vma_decl
mykernel/fs/vfs/binfmt_elf.c:145–145；local_declaration；vma局部声明无初始化
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	vma_s *vma;
```

### E142-008 · align_call
mykernel/fs/vfs/binfmt_elf.c:152–152；active_function_fragment；仅local p赋对齐返回值
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	p = arch_align_stack(p);
```

### E142-009 · platform_alloc
mykernel/fs/vfs/binfmt_elf.c:160–164；active_function_fragment；u_platform置NULL、平台门、strlen+1、分配并转换
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	u_platform = NULL;
	if (k_platform) {
		size_t len = strlen(k_platform) + 1;

		u_platform = (elf_addr_t __user *)STACK_ALLOC(p, len);
```

### E142-010 · platform_copy
mykernel/fs/vfs/binfmt_elf.c:165–167；active_function_fragment；非零copy返回-EFAULT；所选成功前提
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
		if (copy_to_user(u_platform, k_platform, len))
			return -EFAULT;
	}
```

### E142-011 · base_platform_gate
mykernel/fs/vfs/binfmt_elf.c:173–177；conditional；基平台用户地址置NULL及条件分配；模型不进入
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	u_base_platform = NULL;
	if (k_base_platform) {
		size_t len = strlen(k_base_platform) + 1;

		u_base_platform = (elf_addr_t __user *)STACK_ALLOC(p, len);
```

### E142-012 · saved_start
mykernel/fs/vfs/binfmt_elf.c:192–192；active_function_fragment；elf_info从内核saved_auxv起
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	elf_info = (elf_addr_t *)mm->saved_auxv;
```

### E142-013 · aux_macro
mykernel/fs/vfs/binfmt_elf.c:194–198；macro_definition；连续写tag/value两字
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::NEW_AUX_ENT]
```c
#define NEW_AUX_ENT(id, val)	\
		do {					\
			*elf_info++ = id;	\
			*elf_info++ = val;	\
		} while (0)
```

### E142-014 · execfn_platform
mykernel/fs/vfs/binfmt_elf.c:229–233；active_function_fragment；保存文件名及平台用户地址的两个加入点
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_EXECFN, bprm->exec);
	if (k_platform) {
		NEW_AUX_ENT(AT_PLATFORM,
			    (elf_addr_t)(ulong)u_platform);
	}
```

### E142-015 · optional_fd
mykernel/fs/vfs/binfmt_elf.c:238–240；active_function_fragment；have_execfd条件下加入fd
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (bprm->have_execfd) {
		NEW_AUX_ENT(AT_EXECFD, bprm->execfd);
	}
```

### E142-016 · clear
mykernel/fs/vfs/binfmt_elf.c:246–248；active_function_fragment；memset只清内核缓冲从当前elf_info至末尾
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	/* AT_NULL is zero; clear the rest too */
	memset(elf_info, 0, (char *)mm->saved_auxv +
			sizeof(mm->saved_auxv) - (char *)elf_info);
```

### E142-017 · advance
mykernel/fs/vfs/binfmt_elf.c:250–251；active_function_fragment；跨过终止对两字
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	/* And advance past the AT_NULL entry.  */
	elf_info += 2;
```

### E142-018 · ei
mykernel/fs/vfs/binfmt_elf.c:253–254；active_function_fragment；ei_index为指针差的字数；sp取STACK_ADD
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	ei_index = elf_info - (elf_addr_t *)mm->saved_auxv;
	sp = STACK_ADD(p, ei_index);
```

### E142-019 · items_round
mykernel/fs/vfs/binfmt_elf.c:256–257；active_function_fragment；items整数式及bprm.p赋STACK_ROUND
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	items = (argc + 1) + (envc + 1) + 1;
	bprm->p = STACK_ROUND(sp, items);
```

### E142-020 · sp_reset
mykernel/fs/vfs/binfmt_elf.c:260–260；active_function_fragment；sp重置到bprm.p
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	sp = (elf_addr_t __user *)bprm->p;
```

### E142-021 · vma_lock
mykernel/fs/vfs/binfmt_elf.c:266–267；active_function_fragment；锁失败门
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (mmap_write_lock_killable(mm))
		return -EINTR;
```

### E142-022 · vma_gate
mykernel/fs/vfs/binfmt_elf.c:268–271；mixed_comment_and_active_gate；vma赋值是注释；解锁后测试未初始化vma
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	// vma = find_extend_vma_locked(mm, bprm->p);
	mmap_write_unlock(mm);
	if (!vma)
		return -EFAULT;
```

### E142-023 · argc_write
mykernel/fs/vfs/binfmt_elf.c:274–275；active_function_fragment；put_user argc至旧sp且sp++，非零失败
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (put_user(argc, sp++))
		return -EFAULT;
```

### E142-024 · argv_reset_loop
mykernel/fs/vfs/binfmt_elf.c:278–282；active_function_fragment；p与arg_end复位；后缀计数测试；写参数指针
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	p = mm->arg_end = mm->arg_start;
	while (argc-- > 0) {
		size_t len;
		if (put_user((elf_addr_t)p, sp++))
			return -EFAULT;
```

### E142-025 · argv_len
mykernel/fs/vfs/binfmt_elf.c:283–287；active_function_fragment；读取字符串长度并测试，再推进local p
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
		len = strnlen_user((void __user *)p, MAX_ARG_STRLEN);
		if (!len || len > MAX_ARG_STRLEN)
			return -EINVAL;
		p += len;
	}
```

### E142-026 · argv_term
mykernel/fs/vfs/binfmt_elf.c:288–290；active_function_fragment；写零终止槽；arg_end更新
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (put_user(0, sp++))
		return -EFAULT;
	mm->arg_end = p;
```

### E142-027 · env_start_loop
mykernel/fs/vfs/binfmt_elf.c:293–297；active_function_fragment；env_end=env_start=p；后缀计数与写环境指针
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	mm->env_end = mm->env_start = p;
	while (envc-- > 0) {
		size_t len;
		if (put_user((elf_addr_t)p, sp++))
			return -EFAULT;
```

### E142-028 · env_len
mykernel/fs/vfs/binfmt_elf.c:298–302；active_function_fragment；环境长度和p推进
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
		len = strnlen_user((void __user *)p, MAX_ARG_STRLEN);
		if (!len || len > MAX_ARG_STRLEN)
			return -EINVAL;
		p += len;
	}
```

### E142-029 · env_term
mykernel/fs/vfs/binfmt_elf.c:303–305；active_function_fragment；env终止槽写及env_end最终更新
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (put_user(0, sp++))
		return -EFAULT;
	mm->env_end = p;
```

### E142-030 · aux_copy
mykernel/fs/vfs/binfmt_elf.c:307–310；active_function_fragment；sp直接传copy，ei_index乘sizeof；非零失败，局部return0
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	/* Put the elf_info on the stack in the right place.  */
	if (copy_to_user(sp, mm->saved_auxv, ei_index * sizeof(elf_addr_t)))
		return -EFAULT;
	return 0;
```

### E142-031 · caller_table
mykernel/fs/vfs/binfmt_elf.c:1075–1078；active_function_fragment；表调用与负返回退出
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	retval = create_elf_tables(bprm, elf_ex,
				interp_load_addr, e_entry, phdr_addr);
	if (retval < 0)
		goto out;
```

### E142-032 · caller_mm
mykernel/fs/vfs/binfmt_elf.c:1080–1080；active_function_fragment；caller再取current->mm
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	mm = current->mm;
```

### E142-033 · caller_stack
mykernel/fs/vfs/binfmt_elf.c:1085–1085；active_function_fragment；mm.start_stack=bprm.p
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	mm->start_stack = bprm->p;
```

### E142-034 · caller_regs
mykernel/fs/vfs/binfmt_elf.c:1115–1115；active_function_fragment；regs取current_pt_regs
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	regs = current_pt_regs();
```

### E142-035 · caller_start
mykernel/fs/vfs/binfmt_elf.c:1130–1134；active_function_fragment；finalize_exec、START_THREAD栈实参bprm.p、返回
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	finalize_exec(bprm);
	START_THREAD(elf_ex, regs, elf_entry, bprm->p);
	retval = 0;
out:
	return retval;
```

### E142-036 · align_body
mykernel/arch/x86_64/sched/context/process_arch.c:168–169；definition_head；ulong形参与函数体
[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::arch_align_stack]
```c
ulong arch_align_stack(ulong sp)
{
```

### E142-037 · align_comment
mykernel/arch/x86_64/sched/context/process_arch.c:170–172；comment_only；随机化条件及减随机数均注释
[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::arch_align_stack]
```c
	// if (!(current->personality & ADDR_NO_RANDOMIZE) &&
	// 		randomize_va_space)
	// 	sp -= get_random_u32_below(8192);
```

### E142-038 · align_return
mykernel/arch/x86_64/sched/context/process_arch.c:173–174；active_function_fragment；活动返回sp & ~0xf
[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::arch_align_stack]
```c
	return sp & ~0xf;
}
```

### E142-039 · upstream_shift
mykernel/sched/forkexec/exec.c:478–481；active_function_fragment；stack_shift、bprm.p下移、mm.arg_start赋值
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	stack_shift = vma->vm_end - stack_top;

	bprm->p -= stack_shift;
	mm->arg_start = bprm->p;
```

### E142-040 · upstream_exec
mykernel/sched/forkexec/exec.c:483–485；active_function_fragment；loader条件平移与exec无条件减shift
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	if (bprm->loader)
		bprm->loader -= stack_shift;
	bprm->exec -= stack_shift;
```

### E142-041 · upstream_lock
mykernel/sched/forkexec/exec.c:487–488；active_function_fragment；平移后还有锁失败早退
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	if (mmap_write_lock_killable(mm))
		return -EINTR;
```

### E142-042 · platform
mykernel/arch/x86_64/include/asm/elf.h:151–151；macro_definition；x86_64平台串
[VERIFIED mykernel/arch/x86_64/include/asm/elf.h::ELF_PLATFORM]
```c
	#define ELF_PLATFORM ("x86_64")
```

### E142-043 · pagesz
mykernel/arch/x86_64/include/asm/elf.h:156–156；macro_definition；ELF页载荷4096
[VERIFIED mykernel/arch/x86_64/include/asm/elf.h::ELF_EXEC_PAGESIZE]
```c
	#define ELF_EXEC_PAGESIZE	4096
```

### E142-044 · hwcap
mykernel/arch/x86_64/include/asm/elf.h:169–169；macro_definition；能力表达式；H仍题设
[VERIFIED mykernel/arch/x86_64/include/asm/elf.h::ELF_HWCAP]
```c
	#define ELF_HWCAP			(boot_cpu_data.x86_capability[CPUID_1_EDX])
```

### E142-045 · elf_addr
mykernel/include/linux/kernel/elf.h:47–47；macro_definition；Elf64_Off别名，非Elf64_Addr
[VERIFIED mykernel/include/linux/kernel/elf.h::elf_addr_t]
```c
	#define elf_addr_t	Elf64_Off
```

### E142-046 · phdr_alias
mykernel/include/linux/kernel/elf.h:44–44；macro_definition；elf64_phdr标签宏
[VERIFIED mykernel/include/linux/kernel/elf.h::elf_phdr]
```c
	#define elf_phdr	elf64_phdr
```

### E142-047 · phdr_s
mykernel/include/linux/kernel/elf.h:53–53；type_alias；结构类型别名
[VERIFIED mykernel/include/linux/kernel/elf.h::elf_phdr_s]
```c
	typedef struct elf_phdr		elf_phdr_s;
```

### E142-048 · start_macro
mykernel/include/linux/kernel/elf.h:29–30；macro_definition；默认条件宏，未认证实际预处理绑定
[VERIFIED mykernel/include/linux/kernel/elf.h::START_THREAD]
```c
	#	define START_THREAD(elf_ex, regs, elf_entry, start_stack) \
					start_thread(regs, elf_entry, start_stack)
```

### E142-049 · Elf64_Off
mykernel/include/uapi/linux/elf.h:19–19；type_definition；ELF底层类型别名
[VERIFIED mykernel/include/uapi/linux/elf.h::Elf64_Off]
```c
	typedef __u64		Elf64_Off;
```

### E142-050 · Elf64_Addr
mykernel/include/uapi/linux/elf.h:16–16；type_definition；ELF底层类型别名
[VERIFIED mykernel/include/uapi/linux/elf.h::Elf64_Addr]
```c
	typedef __u64		Elf64_Addr;
```

### E142-051 · Elf64_Word
mykernel/include/uapi/linux/elf.h:21–21；type_definition；ELF底层类型别名
[VERIFIED mykernel/include/uapi/linux/elf.h::Elf64_Word]
```c
	typedef __u32		Elf64_Word;
```

### E142-052 · Elf64_Xword
mykernel/include/uapi/linux/elf.h:22–22；type_definition；ELF底层类型别名
[VERIFIED mykernel/include/uapi/linux/elf.h::Elf64_Xword]
```c
	typedef __u64		Elf64_Xword;
```

### E142-053 · phdr_fields1
mykernel/include/uapi/linux/elf.h:270–274；struct_member；两个Word，offset/vaddr/paddr
[VERIFIED mykernel/include/uapi/linux/elf.h::elf64_phdr]
```c
		Elf64_Word	p_type;
		Elf64_Word	p_flags;
		Elf64_Off	p_offset;	/* Segment file offset */
		Elf64_Addr	p_vaddr;	/* Segment virtual address */
		Elf64_Addr	p_paddr;	/* Segment physical address */
```

### E142-054 · phdr_fields2
mykernel/include/uapi/linux/elf.h:275–278；struct_member；filesz/memsz/align
[VERIFIED mykernel/include/uapi/linux/elf.h::elf64_phdr]
```c
		Elf64_Xword	p_filesz;	/* Segment size in file */
		Elf64_Xword	p_memsz;	/* Segment size in memory */
		Elf64_Xword	p_align;	/* Segment alignment, file & memory */
	} Elf64_Phdr;
```

### E142-055 · ulong_type
mykernel/include/linux/kernel/types.h:105–105；type_definition；unsigned long别名；8字节仍题设
[VERIFIED mykernel/include/linux/kernel/types.h::ulong]
```c
		typedef unsigned long		ulong;
```

### E142-056 · bprm_p
mykernel/include/linux/fs/binfmts.h:21–21；struct_member；bprm.p ulong成员
[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
		ulong			p; /* current top of mem */
```

### E142-057 · bprm_counts
mykernel/include/linux/fs/binfmts.h:49–49；struct_member；argc/envc int成员
[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
		int				argc, envc;
```

### E142-058 · bprm_exec
mykernel/include/linux/fs/binfmts.h:55–57；struct_member；interp_flags、execfd及exec ulong成员
[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
		uint			interp_flags;
		int				execfd;		/* File descriptor of the executable */
		ulong			loader, exec;
```

### E142-059 · mm_stack
mykernel/mm/misc/mm_misc_types.h:100–100；struct_member；start_stack字段
[VERIFIED mykernel/mm/misc/mm_misc_types.h::mm_struct]
```c
			ulong			start_brk, brk, start_stack;
```

### E142-060 · mm_bounds
mykernel/mm/misc/mm_misc_types.h:101–101；struct_member；arg/env四字段
[VERIFIED mykernel/mm/misc/mm_misc_types.h::mm_struct]
```c
			ulong			arg_start, arg_end, env_start, env_end;
```

### E142-061 · mm_aux
mykernel/mm/misc/mm_misc_types.h:103–103；struct_member；saved_auxv数组
[VERIFIED mykernel/mm/misc/mm_misc_types.h::mm_struct]
```c
			ulong			saved_auxv[AT_VECTOR_SIZE]; /* for /proc/PID/auxv */
```

### E142-062 · size_arch
mykernel/mm/misc/mm_misc_const.h:6–6；macro_definition；所选默认0
[VERIFIED mykernel/mm/misc/mm_misc_const.h::AT_VECTOR_SIZE_ARCH]
```c
	#  define AT_VECTOR_SIZE_ARCH	0
```

### E142-063 · size_base
mykernel/mm/misc/mm_misc_const.h:9–9；macro_definition；容量基础22
[VERIFIED mykernel/mm/misc/mm_misc_const.h::AT_VECTOR_SIZE_BASE]
```c
	#define AT_VECTOR_SIZE_BASE		22 /* NEW_AUX_ENT entries in auxiliary table */
```

### E142-064 · size_vec
mykernel/mm/misc/mm_misc_const.h:12–12；macro_definition；2*(arch+base+1)
[VERIFIED mykernel/mm/misc/mm_misc_const.h::AT_VECTOR_SIZE]
```c
	#define AT_VECTOR_SIZE			(2*(AT_VECTOR_SIZE_ARCH + AT_VECTOR_SIZE_BASE + 1))
```

### E142-065 · AT_NULL
mykernel/include/uapi/linux/auxvec.h:9–9；macro_definition；tag定义
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_NULL]
```c
	#define AT_NULL		0		/* end of vector */
```

### E142-066 · AT_PLATFORM
mykernel/include/uapi/linux/auxvec.h:24–24；macro_definition；tag定义
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_PLATFORM]
```c
	#define AT_PLATFORM	15		/* string identifying CPU for optimizations */
```

### E142-067 · AT_EXECFN
mykernel/include/uapi/linux/auxvec.h:38–38；macro_definition；tag定义
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_EXECFN]
```c
	#define AT_EXECFN		31		/* filename of program */
```

### E142-068 · CLK
mykernel/include/linux/kernel/asm-generic/param.h:11–11；macro_definition；USER_HZ
[VERIFIED mykernel/include/linux/kernel/asm-generic/param.h::CLOCKS_PER_SEC]
```c
	#define CLOCKS_PER_SEC	(USER_HZ)	/* in "ticks" like times() */
```

### E142-069 · USER_HZ
mykernel/include/linux/kernel/asm-generic/param.h:10–10；macro_definition；250
[VERIFIED mykernel/include/linux/kernel/asm-generic/param.h::USER_HZ]
```c
	#define USER_HZ			250			/* some user interfaces are */
```

### E142-070 · HZ
mykernel/include/linux/kernel/asm-generic/param.h:9–9；macro_definition；1000非auxv时钟载荷
[VERIFIED mykernel/include/linux/kernel/asm-generic/param.h::HZ]
```c
	#define HZ				1000		/* Internal kernel timer frequency */
```

### E142-071 · build_arch
mykernel/scripts/options_flags.cmake:7–9；build_declaration；默认架构声明
[VERIFIED mykernel/scripts/options_flags.cmake::ARCH]
```c
if (NOT DEFINED ${ARCH})
	set(ARCH x86_64)
endif()
```

### E142-072 · build_64
mykernel/scripts/options_flags.cmake:39–43；build_declaration；-m64等明确C选项
[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```c
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

### E142-073 · build_config
mykernel/scripts/options_flags.cmake:58–62；build_declaration；显式CONFIG_64BIT；非全部实际配置
[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```c
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

非活动架构候选与条件的历史来源：固定141的evidence及本轮D00核对；未使用未选宏推断全配置。当前source_index均为新窗，不混计旧129。
