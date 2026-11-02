---
task_id: CORE-USER-EXEC-STACK-23
track_id: MYOS2-LEAD-002
record_type: frozen_time_source_anchor_ledger
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-02
conversation_display_name: MYOS2-A-C03b 代码分析执行续建
produced_by: GPT-6
model_per_owner: GPT-6
effort_per_owner: Extra (Owner selected, backend not attestable)
date: 2026-11-02
source_branch: time
source_pin_short12: a039d9803ade
task_contract_short12: c9be5ac03550
result_base_short12: b32f3ebc9095
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
self_check:
  quote_segments: 116
  quotes_reconfirmed: 116
  downgraded_to_inferred: 0
---

# ES23 固定源码逐字证据

以下逐条为冻结time源码的path::symbol与1–5行源码文本。VERIFIED仅证明文本与局部活动语句，不能自动认证调用者时序、对象有效性、编译生成、CPU PTE及用户栈真实可用。空语句或被注释代码不视为有效副作用。

### ES23-A001 — 参数页接口使用bprm的临时mm及vma
[VERIFIED mykernel/sched/forkexec/exec.c::get_arg_page]
```c
*get_arg_page(linux_bprm_s *bprm, ulong pos, int write) {
	page_s *page;
	vma_s *vma = bprm->vma;
	mm_s *mm = bprm->mm;
	int ret;
```

### ES23-A002 — mmap_read_lock与GUP调用
[VERIFIED mykernel/sched/forkexec/exec.c::get_arg_page]
```c
	/*
	 * We are doing an exec().  'current' is the process
	 * doing the exec and 'mm' is the new process's mm.
	 */
	ret = myos_get_user_pages(mm, pos, 1, &page);
```

### ES23-A003 — GUP局部ret决定参数页返回
[VERIFIED mykernel/sched/forkexec/exec.c::get_arg_page]
```c
	ret = myos_get_user_pages(mm, pos, 1, &page);
	mmap_read_unlock(mm);
	if (ret <= 0)
		return NULL;
```

### ES23-A004 — 成功才归还page指针
[VERIFIED mykernel/sched/forkexec/exec.c::get_arg_page]
```c
	// if (write)
	// 	acct_arg_size(bprm, vma_pages(vma));

	return page;
```

### ES23-A005 — mm_alloc为bprm创建临时mm
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_mm_init]
```c

	bprm->mm = mm = mm_alloc();
	err = -ENOMEM;
	if (!mm)
		goto err;
```

### ES23-A006 — rlim_stack保存代码被注释
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_mm_init]
```c
	// /* Save current stack limit for all calculations made during exec. */
	// task_lock(current->group_leader);
	// bprm->rlim_stack = current->signal->rlim[RLIMIT_STACK];
	// task_unlock(current->group_leader);
```

### ES23-A007 — 临时栈初始化成功门
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_mm_init]
```c
	err = __bprm_mm_init(bprm);
	if (err)
		goto err;

	return 0;
```

### ES23-A008 — 失败释放bprm临时mm
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_mm_init]
```c
		bprm->mm = NULL;
		mmdrop(mm);
	}

	return err;
```

### ES23-A009 — vma由bprm->mm生成并检查分配
[VERIFIED mykernel/sched/forkexec/exec.c::__bprm_mm_init]
```c
	mm_s *mm = bprm->mm;

	bprm->vma = vma = vm_area_alloc(mm);
	if (!vma)
		return -ENOMEM;
```

### ES23-A010 — vma设置匿名并尝试锁包装
[VERIFIED mykernel/sched/forkexec/exec.c::__bprm_mm_init]
```c

	if (mmap_write_lock_killable(mm)) {
		err = -EINTR;
		goto err_free;
	}
```

### ES23-A011 — 初始一个PAGE_SIZE VMA与保护模板
[VERIFIED mykernel/sched/forkexec/exec.c::__bprm_mm_init]
```c
	// BUILD_BUG_ON(VM_STACK_FLAGS & VM_STACK_INCOMPLETE_SETUP);
	vma->vm_end = STACK_TOP_MAX;
	vma->vm_start = vma->vm_end - PAGE_SIZE;
	vma->vm_flags = VM_SOFTDIRTY | VM_STACK_FLAGS | VM_STACK_INCOMPLETE_SETUP;
	vma->vm_page_prot = vm_get_page_prot(vma->vm_flags);
```

### ES23-A012 — insert_vm_struct及失败门
[VERIFIED mykernel/sched/forkexec/exec.c::__bprm_mm_init]
```c
	err = insert_vm_struct(mm, vma);
	if (err)
		goto err;
```

### ES23-A013 — insert成功后写栈计数与bprm p
[VERIFIED mykernel/sched/forkexec/exec.c::__bprm_mm_init]
```c

	mm->stack_vm = mm->total_vm = 1;
	mmap_write_unlock(mm);
	bprm->p = vma->vm_end - sizeof(void *);
	return 0;
```

### ES23-A014 — 插入失败或锁失败的各自清理
[VERIFIED mykernel/sched/forkexec/exec.c::__bprm_mm_init]
```c
	mmap_write_unlock(mm);
err_free:
	bprm->vma = NULL;
	vm_area_free(vma);
	return err;
```

### ES23-A015 — argmin从bprm p和limit推导
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_set_stack_limit]
```c
	/* Avoid a pathological bprm->p. */
	if (bprm->p < limit)
		return -E2BIG;
	bprm->argmin = bprm->p - limit;
	return 0;
```

### ES23-A016 — argv限定不读取注释rlim_stack
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_stack_limits]
```c
	/*
	 * We've historically supported up to 32 pages (ARG_MAX)
	 * of argument strings even with small stacks
	 */
	limit = max_t(ulong, limit, ARG_MAX);
```

### ES23-A017 — 实际copy_strings先减bprm p
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
		pos = bprm->p;
		str += len;
		bprm->p -= len;
		if (bprm->p < bprm->argmin)
			goto out;
```

### ES23-A018 — copy_strings经get_arg_page写页
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
					goto out;
				}

				memcpy_to_page(page, offset_in_page(pos), str, bytes_to_copy);
				put_arg_page(page);
```

### ES23-A019 — setup_arg_pages使用current mm及bprm vma
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
{
	ulong ret;
	ulong stack_shift;
	mm_s *mm = current->mm;
	vma_s *vma = bprm->vma;
```

### ES23-A020 — 局部flag与栈大小及限额变量
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	ulong vm_flags;
	ulong stack_base;
	ulong stack_size;
	ulong stack_expand;
	ulong rlim_stack;
```

### ES23-A021 — stack_top对齐与最小地址检查为注释
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c

	// if (unlikely(stack_top < mmap_min_addr) ||
	// 		unlikely(vma->vm_end - vma->vm_start >=
	// 			stack_top - mmap_min_addr))
	// 	return -ENOMEM;
```

### ES23-A022 — stack_shift先改变p和arg_start
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	stack_shift = vma->vm_end - stack_top;

	bprm->p -= stack_shift;
	mm->arg_start = bprm->p;
```

### ES23-A023 — loader exec再变更后才锁检查
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
		bprm->loader -= stack_shift;
	bprm->exec -= stack_shift;

	if (mmap_write_lock_killable(mm))
		return -EINTR;
```

### ES23-A024 — 局部vm_flags初值不是VMA字段
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	vm_flags = VM_STACK_FLAGS;
```

### ES23-A025 — 按EXSTACK政策更改局部flags
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	if (unlikely(executable_stack == EXSTACK_ENABLE_X))
		vm_flags |= VM_EXEC;
	else if (executable_stack == EXSTACK_DISABLE_X)
		vm_flags &= ~VM_EXEC;
```

### ES23-A026 — OR mm def_flags及临时标志到局部
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
		vm_flags &= ~VM_EXEC;
	vm_flags |= mm->def_flags;
	vm_flags |= VM_STACK_INCOMPLETE_SETUP;
```

### ES23-A027 — mprotect_fixup和TLB处理被注释
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	// tlb_gather_mmu(&tlb, mm);
	// ret = mprotect_fixup(vma, &prev, vma->vm_start, vma->vm_end,
	// 		vm_flags);
	// tlb_finish_mmu(&tlb);
```

### ES23-A028 — 局部VM_EXEC只触发可执行栈警告
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	// BUG_ON(prev != vma);

	if (unlikely(vm_flags & VM_EXEC)) {
		pr_warn_once("process '%pD4' started with"
			" executable stack\n", bprm->file);
```

### ES23-A029 — shift_arg_pages搬迁被注释
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	// if (stack_shift) {
	// 	ret = shift_arg_pages(vma, stack_shift);
	// 	if (ret)
	// 		goto out_unlock;
	// }
```

### ES23-A030 — 清VMA临时标志而非整体赋vm_flags
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	/* mprotect_fixup is overkill to remove the temporary stack flags */
	vm_flags_clear(vma, VM_STACK_INCOMPLETE_SETUP);

	stack_expand = 32 * PAGE_SIZE; /* randomly 32*4k (or 2*64k) pages */
	stack_size = vma->vm_end - vma->vm_start;
```

### ES23-A031 — rlim_stack限额计算被注释
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	//  * Align this down to a page boundary as expand_stack
	//  * will align it up.
	//  */
	// rlim_stack = bprm->rlim_stack.rlim_cur & PAGE_MASK;
	// stack_expand = min(rlim_stack, stack_size + stack_expand);
```

### ES23-A032 — 先写start_stack再调用expand_stack
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	stack_base = vma->vm_end - stack_expand;
	current->mm->start_stack = bprm->p;
	ret = expand_stack(vma, stack_base);
	if (ret)
		ret = -EFAULT;
```

### ES23-A033 — unlock与return ret
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
out_unlock:
	mmap_write_unlock(mm);
	return ret;
```

### ES23-A034 — exec_mmap入参与旧mm来源
[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap]
```c

	/* Notify parent that we're no longer interested in the old VM */
	tsk = current;
	old_mm = current->mm;
	exec_mm_release(tsk, old_mm);
```

### ES23-A035 — 替换active_mm和current mm身份
[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap]
```c

	local_irq_disable();
	active_mm = tsk->active_mm;
	tsk->active_mm = mm;
	tsk->mm = mm;
```

### ES23-A036 — activate_mm和任务锁操作
[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap]
```c
	// if (IS_ENABLED(CONFIG_ARCH_WANT_IRQS_OFF_ACTIVATE_MM))
		local_irq_enable();
	// tsk->mm->vmacache_seqnum = 0;
	// vmacache_flush(tsk);
	task_unlock(tsk);
```

### ES23-A037 — 标记不可返回阶段
[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	/*
	 * Ensure all future errors are fatal.
	 */
	bprm->point_of_no_return = true;
```

### ES23-A038 — 替换mm前的exe_file
[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	retval = set_mm_exe_file(bprm->mm, bprm->file);
	if (retval)
		goto out;
```

### ES23-A039 — 执行mm替换然后将bprm mm置NULL
[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	retval = exec_mmap(bprm->mm);
	if (retval)
		goto out;

	bprm->mm = NULL;
```

### ES23-A040 — mm布局绑定使用current mm
[VERIFIED mykernel/sched/forkexec/exec.c::setup_new_exec]
```c
	/* Setup things that can depend upon the personality */
	task_s *me = current;

	arch_pick_mmap_layout(me->mm, &bprm->rlim_stack);

```

### ES23-A041 — 最终stack limit保存处于注释
[VERIFIED mykernel/sched/forkexec/exec.c::finalize_exec]
```c
	// /* Store any stack rlimit changes before starting thread. */
	// task_lock(current->group_leader);
	// current->signal->rlim[RLIMIT_STACK] = bprm->rlim_stack;
	// task_unlock(current->group_leader);
}
```

### ES23-A042 — free_bprm仅在bprm.mm非NULL时mmput
[VERIFIED mykernel/sched/forkexec/exec.c::free_bprm]
```c
	if (bprm->mm) {
		// acct_arg_size(bprm, 0);
		mmput(bprm->mm);
	}
	free_arg_pages(bprm);
```

### ES23-A043 — alloc_bprm调用bprm_mm_init的上游
[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
		return bprm;

out_free:
	free_bprm(bprm);
	return ERR_PTR(retval);
```

### ES23-A044 — 参数空间限制与bprm exec指针
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c

	retval = copy_string_kernel(bprm->filename, bprm);
	if (retval < 0)
		goto out_free;
	bprm->exec = bprm->p;
```

### ES23-A045 — 实际env和argv copy顺序
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	if (retval < 0)
		goto out_free;

	retval = copy_strings(bprm->argc, argv, bprm);
	if (retval < 0)
```

### ES23-A046 — VMA分配时调用vma_init
[VERIFIED mykernel/mm/vm_map/mmap.c::vm_area_alloc]
```c
{
	vma_s *vma = kmem_cache_alloc(vm_area_cachep, GFP_KERNEL);
	if (vma)
		vma_init(vma, mm);
	return vma;
```

### ES23-A047 — free_vma包装与栈失败清理相关
[VERIFIED mykernel/mm/vm_map/mmap.c::vm_area_free]
```c
vm_area_free(vma_s *vma)
{
	// free_anon_vma_name(vma);
	kmem_cache_free(vm_area_cachep, vma);
}
```

### ES23-A048 — 入链由__vma_link_to_list执行
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_link]
```c
	__vma_link_to_list(mm, vma, prev);
	vma_link_file(vma);

	// validate_mm(mm);
	return 0;
```

### ES23-A049 — 插入vma的实际初始prev查询
[VERIFIED mykernel/mm/vm_map/mmap.c::insert_vm_struct]
```c
int
insert_vm_struct(mm_s *mm, vma_s *vma)
{
	vma_s *prev;
	find_vma_prev(mm, vma->vm_start, &prev);
```

### ES23-A050 — 匿名判定后设置vm_pgoff为页号
[VERIFIED mykernel/mm/vm_map/mmap.c::insert_vm_struct]
```c
	 */
	if (vma_is_anonymous(vma)) {
		BUG_ON(vma->anon_vma);
		vma->vm_pgoff = vma->vm_start >> PAGE_SHIFT;
	}
```

### ES23-A051 — simple_vma_link结果门与返回
[VERIFIED mykernel/mm/vm_map/mmap.c::insert_vm_struct]
```c
		// if (vma->vm_flags & VM_ACCOUNT)
		// 	vm_unacct_memory(charged);
		return -ENOMEM;
	}
	return 0;
```

### ES23-A052 — mmap_min_addr源码初值PAGE_SIZE
[VERIFIED mykernel/mm/vm_map/mmap.c::mmap_min_addr]
```c
static bool ignore_rlimit_data = false;
ulong mmap_min_addr = PAGE_SIZE;
```

### ES23-A053 — 栈guard常量独立存在
[VERIFIED mykernel/mm/vm_map/mmap.c::stack_guard_gap]
```c
/* enforced gap between the expanding stack and other mappings. */
ulong stack_guard_gap = 256UL<<PAGE_SHIFT;
```

### ES23-A054 — 输入地址按PAGE_MASK清低位并拒越界
[VERIFIED mykernel/mm/vm_map/mmap.c::expand_stack]
```c
	vma_s *prev;
	int error = -ENOERR;

	address &= PAGE_MASK;
	if (address < mmap_min_addr)
```

### ES23-A055 — stack_guard_gap相邻映射检查注释
[VERIFIED mykernel/mm/vm_map/mmap.c::expand_stack]
```c
	// if (prev && !(prev->vm_flags & VM_GROWSDOWN) &&
	// 		vma_is_accessible(prev)) {
	// 	if (address - prev->vm_end < stack_guard_gap)
	// 		return -ENOMEM;
	// }
```

### ES23-A056 — anon_vma_prepare被注释
[VERIFIED mykernel/mm/vm_map/mmap.c::expand_stack]
```c
	// /* We must make sure the anon_vma is allocated. */
	// if (unlikely(anon_vma_prepare(vma)))
	// 	return -ENOMEM;
```

### ES23-A057 — 扩栈匿名锁声明与活动代码区别
[VERIFIED mykernel/mm/vm_map/mmap.c::expand_stack]
```c
	// * vma->vm_start/vm_end cannot change under us because the caller
	// * is required to hold the mmap_lock in read mode.  We need the
	// * anon_vma lock to serialize against concurrent expand_stacks.
	// */
	// anon_vma_lock_write(vma->anon_vma);
```

### ES23-A058 — 低于起点才计算size和grow
[VERIFIED mykernel/mm/vm_map/mmap.c::expand_stack]
```c
	if (address < vma->vm_start) {
		ulong size, grow;

		size = vma->vm_end - address;
		grow = (vma->vm_start - address) >> PAGE_SHIFT;
```

### ES23-A059 — 只有grow<=vm_pgoff才进入更新
[VERIFIED mykernel/mm/vm_map/mmap.c::expand_stack]
```c
		// error = -ENOMEM;
		if (grow <= vma->vm_pgoff) {
			// error = acct_stack_growth(vma, size, grow);
```

### ES23-A060 — page_table_lock及目标start/pgoff写入
[VERIFIED mykernel/mm/vm_map/mmap.c::expand_stack]
```c
			// 		mm->locked_vm += grow;
			// 	vm_stat_account(mm, vma->vm_flags, grow);
			// 	anon_vma_interval_tree_pre_update_vma(vma);
				vma->vm_start = address;
				vma->vm_pgoff -= grow;
```

### ES23-A061 — 更新vm_start和vm_pgoff后unlock
[VERIFIED mykernel/mm/vm_map/mmap.c::expand_stack]
```c
				vma->vm_start = address;
				vma->vm_pgoff -= grow;
			// 	anon_vma_interval_tree_post_update_vma(vma);
			// 	vma_gap_update(vma);
				spin_unlock(&mm->page_table_lock);
```

### ES23-A062 — 失败以外保留error为零返回
[VERIFIED mykernel/mm/vm_map/mmap.c::expand_stack]
```c
	}
	// anon_vma_unlock_write(vma->anon_vma);
	// validate_mm(mm);
	return error;
}
```

### ES23-A063 — 用户缺页mm来自current
[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	uint flags = FAULT_FLAG_DEFAULT;

	tsk = current;
	mm = tsk->mm;

```

### ES23-A064 — 缺页WRITE/INSTR旗标赋值
[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
		flags |= FAULT_FLAG_WRITE;
	if (err_code & X86_PF_WRITE)
		flags |= FAULT_FLAG_WRITE;
	if (err_code & X86_PF_INSTR)
		flags |= FAULT_FLAG_INSTRUCTION;
```

### ES23-A065 — 点包含查询为空则bad_area
[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	vma = simple_find_vma(mm, address);
	if (unlikely(!vma)) {
		myos_bad_area(regs, err_code, address);
		return;
	}
```

### ES23-A066 — 已包含时绕过grow分支
[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	if (unlikely(!(vma->vm_flags & VM_GROWSDOWN))) {
		myos_bad_area(regs, err_code, address);
		return;
	}
	if (unlikely(expand_stack(vma, address))) {
```

### ES23-A067 — 可选grow和lock_mmap标签
[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	if (unlikely(expand_stack(vma, address))) {
		myos_bad_area(regs, err_code, address);
		return;
	}
lock_mmap:
```

### ES23-A068 — access_error权限校验被注释
[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	//  */
	// if (unlikely(access_error(error_code, vma))) {
	// 	bad_area_access_error(regs, error_code, address, vma);
	// 	return;
	// }
```

### ES23-A069 — 单次myos_handle_mm_fault和非零循环
[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	// fault = handle_mm_fault(vma, address, flags, regs);
	fault = myos_handle_mm_fault(vma, address, flags);
	while (fault != ENOERR);
```

### ES23-A070 — executable_stack局部默认与bprm
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	int			executable_stack = EXSTACK_DEFAULT;
	elfhdr_s	*elf_ex = (elfhdr_s *)bprm->buf;
	elfhdr_s	*interp_elf_ex = NULL;
	mm_s		*mm;
	pt_regs_s	*regs;
```

### ES23-A071 — PT_GNU_STACK读取PF_X生成策略
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
			if (elf_ppnt->p_flags & PF_X)
				executable_stack = EXSTACK_ENABLE_X;
			else
				executable_stack = EXSTACK_DISABLE_X;
			break;
```

### ES23-A072 — begin_new_exec先于栈页面设置
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	/* Flush all traces of the currently running executable */
	retval = begin_new_exec(bprm);
	if (retval)
		goto out_free_dentry;
```

### ES23-A073 — setup_new_exec再setup_arg_pages的真实调用
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	   change some of these later */
	retval = setup_arg_pages(bprm,
				randomize_stack_top(STACK_TOP), executable_stack);
	if (retval < 0)
		goto out_free_dentry;
```

### ES23-A074 — finalize_exec及START_THREAD抽象入口
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c

	finalize_exec(bprm);
	START_THREAD(elf_ex, regs, elf_entry, bprm->p);
	retval = 0;
```

### ES23-A075 — arch_align_stack按16字节向下对齐
[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::arch_align_stack]
```c
{
	// if (!(current->personality & ADDR_NO_RANDOMIZE) &&
	// 		randomize_va_space)
	// 	sp -= get_random_u32_below(8192);
	return sp & ~0xf;
```

### ES23-A076 — randomize_stack_top无活动随机化
[VERIFIED mykernel/mm/vm_map/util.c::randomize_stack_top]
```c

	// return PAGE_ALIGN(stack_top) - random_variable;

	return stack_top;
}
```

### ES23-A077 — GUP检查点包含并遍历页
[VERIFIED mykernel/mm/misc/gup.c::myos_get_user_pages]
```c
{
	for (int i = 0; i < nr_pages; i++) {
		virt_addr_t addr = start + i * PAGE_SIZE;
		vma_s *vma = simple_find_vma(mm, addr);
		if (vma == NULL)
```

### ES23-A078 — 缺VMA则continue，页不存在才alloc
[VERIFIED mykernel/mm/misc/gup.c::myos_get_user_pages]
```c
			continue;
		
		page_s *pg = myos_get_one_page_from_mapping(mm, addr);
		if (pg == NULL) {
			pg = alloc_page(GFP_USER);
```

### ES23-A079 — GUP的分配映射与引用代码
[VERIFIED mykernel/mm/misc/gup.c::myos_get_user_pages]
```c
			if (pg != NULL) {
				myos_creat_one_page_mapping(mm, addr, pg);
				get_page(pg);
			}
		}
```

### ES23-A080 — GUP尾部写页指针但无return
[VERIFIED mykernel/mm/misc/gup.c::myos_get_user_pages]
```c
		}

		pages[i] = pg;
	}
}
```

### ES23-A081 — mm_init初始化链表和map_count
[VERIFIED mykernel/sched/forkexec/fork.c::mm_init]
```c
	// seqcount_init(&mm->write_protect_seq);
	// mmap_init_lock(mm);
	// INIT_LIST_HEAD(&mm->mmlist);
	// mm_pgtables_bytes_init(mm);
	mm->map_count = 0;
```

### ES23-A082 — mm def_flags继承/初始规则
[VERIFIED mykernel/sched/forkexec/fork.c::mm_init]
```c
		mm->def_flags = current->mm->def_flags & VM_INIT_DEF_MASK;
	} else {
		// mm->flags = default_dump_filter;
		mm->def_flags = 0;
	}
```

### ES23-A083 — mm_alloc间接调用mm_init
[VERIFIED mykernel/sched/forkexec/fork.c::mm_alloc]
```c
	mm = allocate_mm();
	if (!mm)
		return NULL;

	return mm_init(mm, current);
```

### ES23-A084 — VMA初始清零并设置dummy vm_ops
[VERIFIED mykernel/mm/vm_map/vm_map.h::vma_init]
```c
			INIT_LIST_S(&vma->list);
			// INIT_LIST_HEAD(&vma->anon_vma_chain);
			vma->vm_mm = mm;
			vma->vm_ops = &dummy_vm_ops;
		}
```

### ES23-A085 — 匿名标志实际上vm_ops=NULL
[VERIFIED mykernel/mm/vm_map/vm_map.h::vma_set_anonymous]
```c
		PREFIX_STATIC_INLINE
		void
		vma_set_anonymous(vma_s *vma) {
			vma->vm_ops = NULL;
		}
```

### ES23-A086 — 匿名判定依据vm_ops
[VERIFIED mykernel/mm/vm_map/vm_map.h::vma_is_anonymous]
```c
		bool
		vma_is_anonymous(vma_s *vma) {
			return !vma->vm_ops;
```

### ES23-A087 — 清VMA标志的真实作用者
[VERIFIED mykernel/mm/vm_map/vm_map.h::vm_flags_clear]
```c
		void
		vm_flags_clear(vma_s *vma, vm_flags_t flags) {
			// vma_start_write(vma);
			// ACCESS_PRIVATE(vma, __vm_flags) &= ~flags;
			ACCESS_PRIVATE(vma, vm_flags) &= ~flags;
```

### ES23-A088 — 点包含条件及NULL返回
[VERIFIED mykernel/mm/vm_map/vm_map.h::simple_find_vma]
```c
			for_each_vma(mm, vma) {
				if (addr >= vma->vm_start && addr < vma->vm_end)
					return vma;
			}
			return NULL;
```

### ES23-A089 — 临时栈标志宏
[VERIFIED mykernel/mm/vm_map/vm_map_const.h::VM_STACK_INCOMPLETE_SETUP]
```c
	// /* Bits set in the VMA until the stack is in its final location */
	#define VM_STACK_INCOMPLETE_SETUP	(VM_RAND_READ | VM_SEQ_READ)
```

### ES23-A090 — TASK_EXEC及默认栈flags宏链
[VERIFIED mykernel/mm/vm_map/vm_map_const.h::VM_STACK_FLAGS]
```c
	#endif

	#define VM_STACK		VM_GROWSDOWN

	#define VM_STACK_FLAGS		(VM_STACK | VM_STACK_DEFAULT_FLAGS | VM_ACCOUNT)
```

### ES23-A091 — SOFTDIRTY随配置为位或零
[VERIFIED mykernel/mm/vm_map/vm_map_const.h::VM_SOFTDIRTY]
```c
	#	ifdef CONFIG_MEM_SOFT_DIRTY
	#		define VM_SOFTDIRTY		0x08000000	/* Not soft dirty clean area */
	#	else
	#		define VM_SOFTDIRTY		0
	#	endif
```

### ES23-A092 — x86用户地址上界宏
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::TASK_SIZE_MAX]
```c
	#define __VIRTUAL_MASK_SHIFT	47
	#define task_size_max()			((_AC(1, UL) << __VIRTUAL_MASK_SHIFT) - PAGE_SIZE)

	#define TASK_SIZE_MAX			task_size_max()
	#define DEFAULT_MAP_WINDOW		((1UL << 47) - PAGE_SIZE)
```

### ES23-A093 — STACK_TOP与STACK_TOP_MAX在此相同
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::STACK_TOP_MAX]
```c
	#define TASK_SIZE				TASK_SIZE_MAX
	// #define TASK_SIZE_OF(child)		((test_tsk_thread_flag(child, TIF_ADDR32)) ? IA32_PAGE_OFFSET : TASK_SIZE_MAX)

	#define STACK_TOP				TASK_SIZE_LOW
	#define STACK_TOP_MAX			TASK_SIZE_MAX
```

### ES23-A094 — PAGE_SHIFT12及PAGE_SIZE/PAGE_MASK
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	/* PAGE_SHIFT determines the page size */
	#define PAGE_SHIFT				12
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
	#define PAGE_MASK				(~(PAGE_SIZE-1))
```

### ES23-A095 — 写锁帮助体实际return0
[VERIFIED mykernel/include/linux/kernel/mmap_lock.h::mmap_write_lock_killable]
```c
		// __mmap_lock_trace_start_locking(mm, true);
		// ret = down_write_killable(&mm->mmap_lock);
		// __mmap_lock_trace_acquire_returned(mm, true, ret == 0);
		// return ret;
		return 0;
```

### ES23-A096 — 写锁unlock无活动up_write
[VERIFIED mykernel/include/linux/kernel/mmap_lock.h::mmap_write_unlock]
```c
	{
		// __mmap_lock_trace_released(mm, true);
		// vma_end_write_all(mm);
		// up_write(&mm->mmap_lock);
	}
```

### ES23-A097 — 参数GUP readlock未实际down_read
[VERIFIED mykernel/include/linux/kernel/mmap_lock.h::mmap_read_lock]
```c
	{
		// __mmap_lock_trace_start_locking(mm, false);
		// down_read(&mm->mmap_lock);
		// __mmap_lock_trace_acquire_returned(mm, false, true);
	}
```

### ES23-A098 — EXSTACK默认启用禁用常量
[VERIFIED mykernel/include/linux/fs/binfmts.h::EXSTACK_ENABLE_X]
```c
	#define EXSTACK_DEFAULT		0	/* Whatever the arch defaults to */
	#define EXSTACK_DISABLE_X	1	/* Disable executable stacks */
	#define EXSTACK_ENABLE_X	2	/* Enable executable stacks */
```

### ES23-A099 — copy_strings实际调用get_arg_page并检查NULL
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c

				page = get_arg_page(bprm, pos, 1);
				if (!page) {
					ret = -E2BIG;
					goto out;
```

### ES23-A100 — copy_strings取得页后调用memcpy_to_page
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
				}

				memcpy_to_page(page, offset_in_page(pos), str, bytes_to_copy);
				put_arg_page(page);

```

### ES23-A101 — align调用先于栈偏移计算
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c

	stack_top = arch_align_stack(stack_top);

	// if (unlikely(stack_top < mmap_min_addr) ||
```

### ES23-A102 — expand_stack失败改EFAULT再解锁返回
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	ret = expand_stack(vma, stack_base);
	if (ret)
		ret = -EFAULT;
```

### ES23-A103 — 临时新mm的VMA构建并匿名化
[VERIFIED mykernel/sched/forkexec/exec.c::__bprm_mm_init]
```c

	bprm->vma = vma = vm_area_alloc(mm);
	if (!vma)
		return -ENOMEM;
	vma_set_anonymous(vma);
```

### ES23-A104 — 可执行栈警告只消费局部vm_flags
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	if (unlikely(vm_flags & VM_EXEC)) {
		pr_warn_once("process '%pD4' started with"
			" executable stack\n", bprm->file);
	}
```

### ES23-A105 — GUP调用的映射帮助函数形参
[VERIFIED mykernel/mm/fault/fault.c::myos_creat_one_page_mapping]
```c
pte_t *myos_creat_one_page_mapping(mm_s *mm,
		virt_addr_t addr, page_s *page)
{
```

### ES23-A106 — 映射PTE使用固定PAGE_SHARED_EXEC值
[VERIFIED mykernel/mm/fault/fault.c::myos_creat_one_page_mapping]
```c
	if (page != NULL)
	{
		atomic_inc(&(page->_mapcount));
		ulong page_addr = page_to_phys(page);
		*pte = arch_make_pte(pgprot_val(PAGE_SHARED_EXEC) | _PAGE_PAT | page_addr);
```

### ES23-A107 — 帮助函数返回pte或ERR_PTR
[VERIFIED mykernel/mm/fault/fault.c::myos_creat_one_page_mapping]
```c
	return pte;

fail:
	return ERR_PTR(-ENOMEM);
}
```

### ES23-A108 — 已有映射获取页函数接口
[VERIFIED mykernel/mm/fault/fault.c::myos_get_one_page_from_mapping]
```c
page_s *myos_get_one_page_from_mapping(mm_s *mm, virt_addr_t addr)
{
	page_s *retval = NULL;
	pgd_t *pgd;
	p4d_t *p4d;
```

### ES23-A109 — VMA权限表的private可执行写组合
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::protection_map]
```c
	[VM_WRITE | VM_READ]						= PAGE_COPY,
	[VM_EXEC]									= PAGE_READONLY_EXEC,
	[VM_EXEC | VM_READ]							= PAGE_READONLY_EXEC,
	[VM_EXEC | VM_WRITE]						= PAGE_COPY_EXEC,
	[VM_EXEC | VM_WRITE | VM_READ]				= PAGE_COPY_EXEC,
```

### ES23-A110 — vm_get_page_prot按VM位消费模板
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::vm_get_page_prot]
```c
pgprot_t vm_get_page_prot(ulong vm_flags)
{
	ulong val = pgprot_val(protection_map[vm_flags &
					(VM_READ|VM_WRITE|VM_EXEC|VM_SHARED)]);
```

### ES23-A111 — 模板进入PTE前的supported mask筛选
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::vm_get_page_prot]
```c
	// val = __sme_set(val);
	if (val & _PAGE_PRESENT)
		val &= __supported_pte_mask;
	return __pgprot(val);
```

### ES23-A112 — PAGE_SHARED_EXEC具有RW和无NX
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PAGE_SHARED_EXEC]
```c
	#define PAGE_SHARED					__pg(__PP|__RW|_USR|___A|__NX|   0|   0|   0)
	#define PAGE_SHARED_EXEC			__pg(__PP|__RW|_USR|___A|   0|   0|   0|   0)
	#define PAGE_COPY_NOEXEC			__pg(__PP|   0|_USR|___A|__NX|   0|   0|   0)
	#define PAGE_COPY_EXEC				__pg(__PP|   0|_USR|___A|   0|   0|   0|   0)
	#define PAGE_COPY					__pg(__PP|   0|_USR|___A|__NX|   0|   0|   0)
```

### ES23-A113 — ENOERR是0的定义
[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOERR]
```c
	#define	ENOERR		0	/*  */
```

### ES23-A114 — 默认stack_top不执行随机改变
[VERIFIED mykernel/mm/vm_map/util.c::randomize_stack_top]
```c

	// return PAGE_ALIGN(stack_top) - random_variable;

	return stack_top;
```

### ES23-A115 — mm_init创建VMA链表与计数引用
[VERIFIED mykernel/sched/forkexec/fork.c::mm_init]
```c
	// mm->mmap = NULL;
	INIT_LIST_HEADER_S(&mm->mm_mt);
	// mm->vmacache_seqnum = 0;
	atomic_set(&mm->mm_users, 1);
	atomic_set(&mm->mm_refcount, 1);
```

### ES23-A116 — grow与vma_pgoff门的实际顺序
[VERIFIED mykernel/mm/vm_map/mmap.c::expand_stack]
```c
		size = vma->vm_end - address;
		grow = (vma->vm_start - address) >> PAGE_SHIFT;

		// error = -ENOMEM;
		if (grow <= vma->vm_pgoff) {
```

## 证据及未做边界

- 完整读取42号固定任务及22主线回执R22-S01/S02/S03；定点读取12回执及MF12-F01/F02/F03延期、21初始NULL遍历的消费限定。复用15的mm替换身份、19匿名页入口、20 VMA权限与22分阶段副作用口径。
- 主要固定对象为exec.c、binfmt_elf.c、mmap.c、vm_map.h、vm_map_const.h、vm_map_types.h、fork.c、gup.c、fault.c、arch/x86_64/mm/fault.c、arch_align_stack、x86 PAGE/PTE保护模板、fs/binfmts.h及mmap_lock.h。
- 编译条件/源文本可以显示初始VMA与局部策略分离；活动栈参数复制依赖尚未认证的GUP long返回值与对应page指针的合法性。运行CPU初始RSP、页面内容、文件上下文与页表缓存不在本批证明范围。
- 实行的文本/JSON/图ID/有限无符号算术自查不等同执行仓库脚本。没有预处理、编译、C/ASM运行、QEMU或实机实验；没有修改旧源码或以前任一批件。
