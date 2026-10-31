---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-MM-CONTEXT-15
record_type: bounded_mm_reference_fork_exec_schedule_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: "2026-10-31"
read_channel: connector
source_pin: "time @ a039d9803ade"
results_base: "dd8e81ba6fc7"
contract_ref: "33-user-mm-context-contract.md @ 9a423f5b45b1"
prior_review: "CORE-USER-VMA-LIFETIME-14-REVIEW-001 @ 18ee22673d83"
prior_verdict: PASS_PENDING_LOCAL_WITH_R14_S01_S02
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第15批：mm的所有权、任务地址空间替换与CR3切换之间的边界

## 一、关键结论与限制

本树实际有两个计数：`mm_users`与`mm_refcount`。 `mm_init`把二者都初始化为1；`mmget/mmput`增减的是**mm_users**，而`mmdrop`在实际函数体中**没有检查或递减mm_refcount**，直接经过`pgd_free`并归还mm缓存对象（唯对`init_mm`有明确禁止条件）。`mmput`在`mm_users`降为0时调用`__mmput`，但该函数的`exit_mmap`、`mmdrop`全部被注释。**“用户计数归零”“结构对象最终引用已归零”“页表根/内存已释放”三者不等价。** 不能把Linux的mm_count/mmgrab语义直接套到这个名为mm_refcount的字段。

在fork中`CLONE_VM`与非共享复制路径确实区分：前者对父mm做一次`mmget`并将新task的`mm`和`active_mm`都指向它，**两个指针字段不是两份引用**；后者分配新的mm、先对旧mm结构体memcpy，后经`mm_init`重建计数/pgd并复制VMAs。复制VMA/页表后报错会落到仅`mmput`的清理入口，不能因此保证已经发生的VMA/file/page副作用完全撤销。

调度切换的`context_switch`**真正在`scheduler.h`内联定义**，`scheduler_core.c`定义`SCHEDULER_DEFINATION`后引用它，而不是仅有名字出现在`scheduler_core.c`调用点。用户→无mm内核任务时，借用`prev->active_mm`并对其**mm_users做mmget**，不是mm_refcount上的mmgrab；内核→用户时把旧借用保存在`rq->prev_mm`，但`finish_task_switch`虽清空此字段，其相关mmdrop_lazy_tlb清理是注释，借用的计数配对未获证明。不同mm路径经`switch_mm_irqs_off→load_new_mm_cr3→write_cr3`实际具备C/内联汇编写CR3的操作，**但不说明真实运行成功或TLB跨CPU同步正确**。

本轮只验证固定`time @ a039d9803ade`的相关源码，不预处理/编译/执行C/ASM/QEMU。14的R14-S01准确消费为“非零munmap若正常回到sys_brk out会恢复origbrk；VMA事务未必回滚”，R14-S02补真实调用锚点，不重做14/13。以下所有计数和控制流结论均是静态与前提性推断。

## 二、A01：对象、计数、分配与释放端点

`task_struct`同时拥有`mm_s *mm`及`mm_s *active_mm`字段，`mm_s`定义中实际字段为`atomic_t mm_users`和`atomic_t mm_refcount`；代码注释中`mm_count`是历史文案而非此结构实际字段名。`mmget`只执行`atomic_inc(&mm->mm_users)`，`get_task_mm`先以task_lock读取task.mm、若非NULL但有`PF_KTHREAD`则返回NULL，否则才`mmget`。它不因task.active_mm借用了用户mm就一定对调用者返回该mm。

**[VERIFIED mykernel/sched/task/task_types.h::task_struct · MC15-A001，136–138行]**

```c

		mm_s			*mm;
		mm_s			*active_mm;
```

**[VERIFIED mykernel/sched/task/task_types.h::task_struct · MC15-A002，137–139行]**

```c
		mm_s			*mm;
		mm_s			*active_mm;

```

**[VERIFIED mykernel/mm/misc/mm_misc_types.h::mm_s · MC15-A003，44–46行]**

```c
			 */
			atomic_t		mm_users;

```

**[VERIFIED mykernel/mm/misc/mm_misc_types.h::mm_s · MC15-A004，53–55行]**

```c
			 */
			atomic_t		mm_refcount;
			atomic_long_t	pgtables_bytes;		/* PTE page table pages */
```

**[VERIFIED mykernel/mm/misc/mm_misc.h::mmget · MC15-A005，38–40行]**

```c
		mmget(mm_s *mm) {
			atomic_inc(&mm->mm_users);
		}
```

**[VERIFIED mykernel/sched/forkexec/fork.c::get_task_mm · MC15-A024，525–527行]**

```c
		else
			mmget(mm);
	}
```


`allocate_mm`宏为`kmem_cache_alloc(mm_cachep,GFP_KERNEL)`，`mm_alloc`成功分配后调用`mm_init`。其中初始化`mm_mt`、`map_count=0`、两个计数为1，并请求`pgd_alloc`作为页表根。pgd_alloc通过`get_zeroed_page`获取root，给`mm->pgd`赋值、复制所选内核PGD条目并登记到pgd链；失败时返回NULL，`mm_init`相应直接free_mm并返回NULL。**“分配了mm对象”“已初始化页表根”“发布到task.mm”是不同时间点。** 未对页表下级、allocator并发及所有初始化字段完备性作全局保证。

**[VERIFIED mykernel/sched/forkexec/fork.c::allocate_mm macro · MC15-A006，248–250行]**

```c

#define allocate_mm()	(kmem_cache_alloc(mm_cachep, GFP_KERNEL))
#define free_mm(mm)		(kmem_cache_free(mm_cachep, (mm)))
```

**[VERIFIED mykernel/sched/forkexec/fork.c::mm_init · MC15-A013，367–369行]**

```c
	// mm->vmacache_seqnum = 0;
	atomic_set(&mm->mm_users, 1);
	atomic_set(&mm->mm_refcount, 1);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::mm_init · MC15-A014，368–370行]**

```c
	atomic_set(&mm->mm_users, 1);
	atomic_set(&mm->mm_refcount, 1);
	// seqcount_init(&mm->write_protect_seq);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::mm_init · MC15-A015，401–403行]**

```c

	mm->pgd = pgd_alloc(mm);
	if (!mm->pgd)
```

**[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.c::pgd_alloc · MC15-A050，29–31行]**

```c

	pgd = (pgd_t *)get_zeroed_page(GFP_PGTABLE_USER);
	if (pgd == NULL)
```

**[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.c::pgd_alloc · MC15-A052，47–49行]**

```c
		   references from swapper_pg_dir. */
		memcpy(pgd + KERNEL_PGD_BOUNDARY,
			swapper_pg_dir + KERNEL_PGD_BOUNDARY,
```

**[VERIFIED mykernel/sched/forkexec/fork.c::mm_init · MC15-A016，414–416行]**

```c
fail_nopgd:
	free_mm(mm);
	return NULL;
```


`mmput`在mm_users减至0时调用`__mmput`，但后者的`exit_mmap`和`mmdrop`均是注释。单独调用的`mmdrop`却直接经`mm_free_pgd→pgd_free→_pgd_free`释放PGD根，再`free_mm`，没有`atomic_dec_and_test(mm_refcount)`之类的主动门。它对`&init_mm`有`BUG_ON`和`while`禁止，对当前`mm/active_mm`有WARN门；所以不能从名字推断“只有最后一个结构引用归零才释放”。

**[VERIFIED mykernel/sched/forkexec/fork.c::mmput · MC15-A019，459–461行]**

```c

	if (atomic_dec_and_test(&mm->mm_users))
		__mmput(mm);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::__mmput · MC15-A021，439–441行]**

```c
	// khugepaged_exit(mm); /* must run before exit_mmap */
	// exit_mmap(mm);
	// mm_put_huge_zero_page(mm);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::__mmput · MC15-A022，449–451行]**

```c
	// 	module_put(mm->binfmt->module);
	// mmdrop(mm);
}
```

**[VERIFIED mykernel/sched/forkexec/fork.c::mmdrop · MC15-A009，259–261行]**

```c
{
	BUG_ON(mm == &init_mm);
	while (mm == &init_mm);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::mmdrop · MC15-A010，263–265行]**

```c
	WARN_ON_ONCE(mm == current->active_mm);
	mm_free_pgd(mm);
	// destroy_context(mm);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::mmdrop · MC15-A011，268–270行]**

```c
	// put_user_ns(mm->user_ns);
	free_mm(mm);
}
```

**[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.c::pgd_free · MC15-A057，77–79行]**

```c
	// }
	_pgd_free(pgd);
}
```


特别注意`init_mm`是静态对象，初始`mm_users=2`、`mm_refcount=1`，PGD为`swapper_pg_dir`；CPU初始化的特定路径将`curr->active_mm=&init_mm`且要求curr->mm为空。把init_mm当成普通mm传给mmdrop或从普通构造路径计算余量均不成立。

**[VERIFIED mykernel/mm/misc/init-mm.c::init_mm · MC15-A058，31–33行]**

```c
	.pgd				= swapper_pg_dir,
	.mm_users			= ATOMIC_INIT(2),
	.mm_refcount		= ATOMIC_INIT(1),
```

**[VERIFIED mykernel/mm/misc/init-mm.c::init_mm · MC15-A059，32–34行]**

```c
	.mm_users			= ATOMIC_INIT(2),
	.mm_refcount		= ATOMIC_INIT(1),
	// .write_protect_seq	= SEQCNT_ZERO(init_mm.write_protect_seq),
```

**[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::cpu_init · MC15-A061，616–618行]**

```c
	// mmgrab(&init_mm);
	curr->active_mm = &init_mm;
	BUG_ON(curr->mm);
```


## 三、A02：fork共享与复制、发布和错误路径

`copy_process`在复制文件表、fs、signal等步骤之后调用`copy_mm(clone_flags,p)`；随后还有namespace、thread、PID及任务挂接等步骤，失败标签可回到`bad_fork_cleanup_mm`。这里仅追与mm有关的清理，不重做PID或sched普通任务发布。

**[VERIFIED mykernel/sched/forkexec/fork.c::copy_process · MC15-A039，1133–1135行]**

```c
		goto bad_fork_cleanup_sighand;
	retval = copy_mm(clone_flags, p);
	if (retval)
```

**[VERIFIED mykernel/sched/forkexec/fork.c::copy_process · MC15-A040，1138–1140行]**

```c
	if (retval)
		goto bad_fork_cleanup_mm;
	// retval = copy_io(clone_flags, p);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::copy_process · MC15-A041，1397–1399行]**

```c
		// mm_clear_owner(p->mm, p);
		mmput(p->mm);
	}
```


`copy_mm`最先把新task的`mm`和`active_mm`设置NULL。如果`current->mm`为NULL，它返回0且不建立任何用户mm引用。若current.mm有效且`CLONE_VM`为真，`mmget(oldmm)`使mm_users增1，再使两个task字段同指oldmm。若非共享，`dup_mm`失败则返回-ENOMEM，成功时才把两个字段指向新mm。

**[VERIFIED mykernel/sched/forkexec/fork.c::copy_mm · MC15-A031，678–680行]**

```c
	mm_s *mm, *oldmm;
	tsk->mm = NULL;
	tsk->active_mm = NULL;
```

**[VERIFIED mykernel/sched/forkexec/fork.c::copy_mm · MC15-A033，686–688行]**

```c
	 */
	if ((oldmm = current->mm) == NULL)
		return 0;
```

**[VERIFIED mykernel/sched/forkexec/fork.c::copy_mm · MC15-A034，689–691行]**

```c

	if (clone_flags & CLONE_VM) {
		mmget(oldmm);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::copy_mm · MC15-A035，690–692行]**

```c
	if (clone_flags & CLONE_VM) {
		mmget(oldmm);
		mm = oldmm;
```

**[VERIFIED mykernel/sched/forkexec/fork.c::copy_mm · MC15-A036，692–694行]**

```c
		mm = oldmm;
	} else if ((mm = dup_mm(tsk, oldmm)) == NULL) {
			return -ENOMEM;
```

**[VERIFIED mykernel/sched/forkexec/fork.c::copy_mm · MC15-A037，696–698行]**

```c

	tsk->mm = mm;
	tsk->active_mm = mm;
```

**[VERIFIED mykernel/sched/forkexec/fork.c::copy_mm · MC15-A038，697–699行]**

```c
	tsk->mm = mm;
	tsk->active_mm = mm;
	return 0;
```


`dup_mm`先分配mm，**直接`memcpy(mm,oldmm,sizeof(*mm))`**，随后`mm_init`重设mm_users/mm_refcount=1、清新链/分配新pgd，再`dup_mmap`尝试复制各VMA。后者给`tmp->vm_mm=mm`、文件VMA有条件get_file、链入新mm并尝试`copy_page_range`；源文件及COW/页表引用的实际复制正确性未完整审计。结构memcpy并不意味着所有内嵌引用获得新持有。

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mm · MC15-A027，661–663行]**

```c

	memcpy(mm, oldmm, sizeof(*mm));
	if (!mm_init(mm, tsk))
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mm · MC15-A028，662–664行]**

```c
	memcpy(mm, oldmm, sizeof(*mm));
	if (!mm_init(mm, tsk))
		goto fail_nomem;
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mm · MC15-A029，665–667行]**

```c

	if ((err = dup_mmap(mm, oldmm)) != 0)
		goto free_pt;
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · MC15-A043，187–189行]**

```c
		}
		if ((tmp = vm_area_creat_dup(mpnt)) == NULL)
			goto fail_nomem;
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · MC15-A044，189–191行]**

```c
			goto fail_nomem;
		tmp->vm_mm = mm;
		// retval = dup_userfaultfd(tmp, &uf);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · MC15-A045，198–200行]**

```c

			get_file(file);
			i_mmap_lock_write(mapping);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · MC15-A047，217–219行]**

```c
		if (!(tmp->vm_flags & VM_WIPEONFORK))
			retval = copy_page_range(tmp, mpnt);

```


**失败边界：** 新mm的pgd_alloc失败时，mm_init走free_mm并返回NULL，这与dup_mmap进行过部分VMA/file/page操作后失败不同。后一种情况下dup_mm跳到free_pt调用`mmput(mm)`，只是用户计数从1降至0并进入不释放VMA/PGD的`__mmput`；此前副作用不能视为自动撤销。又如copy_process在copy_mm完成后的其它阶段失败，`bad_fork_cleanup_mm`会条件性`mmput(p->mm)`；共享mm可扣回那份用户引用，新mm则仍受__mmput的清理断点限制。错误返回不能推出整个fork事务恢复。

**[VERIFIED mykernel/sched/forkexec/fork.c::mm_init · MC15-A016，414–416行]**

```c
fail_nopgd:
	free_mm(mm);
	return NULL;
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mm · MC15-A030，671–673行]**

```c
free_pt:
	mmput(mm);
fail_nomem:
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · MC15-A048，222–224行]**

```c

		if (retval)
			goto out;
```

**[VERIFIED mykernel/sched/forkexec/fork.c::copy_process · MC15-A041，1397–1399行]**

```c
		// mm_clear_owner(p->mm, p);
		mmput(p->mm);
	}
```


## 四、A03：exec对象替换和调度借用、真实CR3接口

### 4.1 exec新mm与旧mm释放

`bprm_mm_init`先用mm_alloc取得新mm，再经`__bprm_mm_init`建立临时栈VMA；后者出错时`bprm_mm_init`执行`mmdrop(mm)`，与后来的`free_bprm`发现bprm.mm非NULL时执行`mmput`是两条不同的收尾路径。必要调用者`begin_new_exec`调用`exec_mmap(bprm->mm)`，成功则把`bprm->mm=NULL`，表示该临时持有字段不再继续被free_bprm归还。尚未执行ELF加载或完整用户argv/stack路径。

**[VERIFIED mykernel/sched/forkexec/exec.c::bprm_mm_init · MC15-A062，160–162行]**

```c

	bprm->mm = mm = mm_alloc();
	err = -ENOMEM;
```

**[VERIFIED mykernel/sched/forkexec/exec.c::bprm_mm_init · MC15-A063，170–172行]**

```c

	err = __bprm_mm_init(bprm);
	if (err)
```

**[VERIFIED mykernel/sched/forkexec/exec.c::bprm_mm_init · MC15-A064，179–181行]**

```c
		bprm->mm = NULL;
		mmdrop(mm);
	}
```

**[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec · MC15-A078，908–910行]**

```c
	// acct_arg_size(bprm, 0);
	retval = exec_mmap(bprm->mm);
	if (retval)
```

**[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec · MC15-A079，912–914行]**

```c

	bprm->mm = NULL;

```

**[VERIFIED mykernel/sched/forkexec/exec.c::free_bprm · MC15-A067，1075–1077行]**

```c
		// acct_arg_size(bprm, 0);
		mmput(bprm->mm);
	}
```


`exec_mmap(mm)`先保存`old_mm=current->mm`并无条件调用`exec_mm_release(tsk,old_mm)`，该帮助函数进入`mm_release`，而后者的`deactivate_mm`宏只包含本树所见的FS/GS寄存器恢复逻辑，不等于归还mm引用。当`old_mm`非NULL时，进入条件的mmap_read_lock，然后在task_lock/IRQ范围内将`tsk->active_mm=mm; tsk->mm=mm`，调用`activate_mm(active_mm,mm)`；随后`BUG_ON(active_mm != old_mm)`，再`mmput(old_mm)`。源文中部分锁失败检查被注释，不能保证所有输入都按成功分支达到归还。

**[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap · MC15-A068，628–630行]**

```c
	tsk = current;
	old_mm = current->mm;
	exec_mm_release(tsk, old_mm);
```

**[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap · MC15-A069，629–631行]**

```c
	old_mm = current->mm;
	exec_mm_release(tsk, old_mm);

```

**[VERIFIED mykernel/sched/forkexec/fork.c::mm_release · MC15-A080，603–605行]**

```c
	/* Get rid of any cached register state */
	deactivate_mm(tsk, mm);

```

**[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap · MC15-A071，652–654行]**

```c
	local_irq_disable();
	active_mm = tsk->active_mm;
	tsk->active_mm = mm;
```

**[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap · MC15-A072，653–655行]**

```c
	active_mm = tsk->active_mm;
	tsk->active_mm = mm;
	tsk->mm = mm;
```

**[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap · MC15-A073，654–656行]**

```c
	tsk->active_mm = mm;
	tsk->mm = mm;
	// mm_init_cid(mm);
```

**[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap · MC15-A074，665–667行]**

```c
	// 	local_irq_enable();
	activate_mm(active_mm, mm);
	// if (IS_ENABLED(CONFIG_ARCH_WANT_IRQS_OFF_ACTIVATE_MM))
```

**[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap · MC15-A075，673–675行]**

```c
		mmap_read_unlock(old_mm);
		BUG_ON(active_mm != old_mm);
		// setmax_mm_hiwater_rss(&tsk->signal->maxrss, old_mm);
```

**[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap · MC15-A076，676–678行]**

```c
		// mm_update_next_owner(old_mm);
		mmput(old_mm);
		return 0;
```


**old_mm为NULL时与上条分开。** 此时`exec_mm_release(tsk,NULL)`仍调用；`mm_release`若`clear_child_tid`非NULL，还会读`mm->mm_users`，所以NULL指针存在前提风险。若顺利完成替换/activate，源码末尾`mmdrop(active_mm)`。但是CPU初始化可能使这个先前的active_mm指向静态init_mm，而`mmdrop(&init_mm)`会BUG/while；不能称旧mm为空时的exec一定成功释放。反过来，当active_mm为其它有效非init对象时，mmdrop可直接free PGD/mm，缺乏结构refcount门。以上皆为**条件性静态路径**，不是已复现崩溃。

**[VERIFIED mykernel/sched/forkexec/fork.c::mm_release · MC15-A081，611–613行]**

```c
	if (tsk->clear_child_tid) {
		if (atomic_read(&mm->mm_users) > 1) {
			/*
```

**[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap · MC15-A077，679–681行]**

```c
	}
	mmdrop(active_mm);
	return 0;
```

**[VERIFIED mykernel/sched/forkexec/fork.c::mmdrop · MC15-A009，259–261行]**

```c
{
	BUG_ON(mm == &init_mm);
	while (mm == &init_mm);
```

**[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::cpu_init · MC15-A061，616–618行]**

```c
	// mmgrab(&init_mm);
	curr->active_mm = &init_mm;
	BUG_ON(curr->mm);
```


### 4.2 调度接口：任务指针、mm借用、CR3写入分开

固定`scheduler_core.c`在首行定义`SCHEDULER_DEFINATION`并包含`scheduler.h`，`context_switch`的实际定义位于该头文件中。 `__schedule`仅当`prev != next`时调用它；同一task被继续选中不会因此进入下列mm切换。实际寄存器上下文切换经`switch_to`宏展开为`__switch_to_asm`；未执行该ASM。

**[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule · MC15-A084，1123–1125行]**

```c

	if (likely(prev != next)) {
		rq->nr_switches++;
```

**[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule · MC15-A085，1161–1163行]**

```c
		/* Also unlocks the rq: */
		rq = context_switch(rq, prev, next, &rf);
	} else {
```

**[VERIFIED mykernel/sched/scheduler/scheduler.h::context_switch · MC15-A094，210–212行]**

```c
			/* Here we just switch the register state and the stack. */
			switch_to(prev, next, prev);
			barrier();
```

**[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_macro_arch.h::switch_to macro · MC15-A100，9–11行]**

```c
			do {											\
				((last) = __switch_to_asm((prev), (next)));	\
			} while (0)
```


`context_switch`将四种前后任务情形分开：

| 转换 | 源码中可见的task指针/计数变化 | 与硬件和释放的区别 |
|---|---|---|
| user→user，mm不同 | `switch_mm_irqs_off(prev.active_mm,next.mm,next)`，当prev!=next调用load_new_mm_cr3 | 真正CR3值还受next.pgd有效、virt_to_phys前提；不证TLB/跨CPU |
| user→user，mm相同 | arch的`prev==next`直接return | 相同指针不等于已读取CPU当前CR3验证 |
| user→kernel (next.mm=NULL) | `next.active_mm=prev.active_mm`，若prev.mm非NULL，调用`mmget(prev.active_mm)` | **增的是mm_users而非mm_refcount**；`enter_lazy_tlb`是注释 |
| kernel→kernel | 将prev.active_mm复制给next.active_mm，prev.active_mm清NULL | 没有新mmget，要求此前借用值与生命期有效 |
| kernel→user | 切换mm后`rq.prev_mm=prev.active_mm; prev.active_mm=NULL` | `finish_task_switch`读取并清空rq.prev_mm，但mmdrop_lazy_tlb操作注释，未得到该借用引用的配对归还 |

**[VERIFIED mykernel/sched/scheduler/scheduler.h::context_switch · MC15-A086，172–174行]**

```c
			 */
			if (!next->mm) {								// to kernel
				// enter_lazy_tlb(prev->active_mm, next);
```

**[VERIFIED mykernel/sched/scheduler/scheduler.h::context_switch · MC15-A087，175–177行]**

```c

				next->active_mm = prev->active_mm;
				if (prev->mm)								// from user
```

**[VERIFIED mykernel/sched/scheduler/scheduler.h::context_switch · MC15-A089，178–180行]**

```c
					// mmgrab_lazy_tlb(prev->active_mm);
					mmget(prev->active_mm);
				else
```

**[VERIFIED mykernel/sched/scheduler/scheduler.h::context_switch · MC15-A090，180–182行]**

```c
				else
					prev->active_mm = NULL;
			} else {										// to user
```

**[VERIFIED mykernel/sched/scheduler/scheduler.h::context_switch · MC15-A091，191–193行]**

```c
				 */
				switch_mm_irqs_off(prev->active_mm, next->mm, next);
				// lru_gen_use_mm(next->mm);
```

**[VERIFIED mykernel/sched/scheduler/scheduler.h::context_switch · MC15-A092，196–198行]**

```c
					/* will mmdrop_lazy_tlb() in finish_task_switch(). */
					rq->prev_mm = prev->active_mm;
					prev->active_mm = NULL;
```

**[VERIFIED mykernel/sched/scheduler/scheduler.h::context_switch · MC15-A093，197–199行]**

```c
					rq->prev_mm = prev->active_mm;
					prev->active_mm = NULL;
				}
```

**[VERIFIED mykernel/sched/scheduler/scheduler_core.c::finish_task_switch · MC15-A096，788–790行]**

```c
	rq_s *rq = this_rq();
	mm_s *mm = rq->prev_mm;
	uint prev_state;
```

**[VERIFIED mykernel/sched/scheduler/scheduler_core.c::finish_task_switch · MC15-A097，807–809行]**

```c

	rq->prev_mm = NULL;

```

**[VERIFIED mykernel/sched/scheduler/scheduler_core.c::finish_task_switch · MC15-A098，852–854行]**

```c
	// 	membarrier_mm_sync_core_before_usermode(mm);
	// 	mmdrop_lazy_tlb_sched(mm);
	// }
```


`activate_mm`宏真实调用`switch_mm`，后者保存IRQ并调用`switch_mm_irqs_off`。该arch函数先检查`prev == next`并可能早退；不同mm时以`next->pgd`调用`load_new_mm_cr3`，经`virt_to_phys`构造CR3值，最后`write_cr3`的C内联ASM为`mov ...,%cr3`。这证明**源码存在这条写寄存器路径**，不证明动态运行时CPU实际执行、屏障/PCID/TLB shootdown、跨CPU一致性或VMA/PTE页回收。

**[VERIFIED mykernel/arch/x86_64/mm/mmu/mmu_macro_arch.h::activate_mm macro · MC15-A101，10–12行]**

```c
	#define activate_mm(prev, next) do {			\
				switch_mm((prev), (next), NULL);	\
			} while (0);
```

**[VERIFIED mykernel/arch/x86_64/mm/mmu/mmu_arch.c::switch_mm · MC15-A103，47–49行]**

```c
	local_irq_save(flags);
	switch_mm_irqs_off(prev, next, tsk);
	local_irq_restore(flags);
```

**[VERIFIED mykernel/arch/x86_64/mm/mmu/mmu_arch.c::switch_mm_irqs_off · MC15-A104，54–56行]**

```c
{
	if (prev == next)
		return;
```

**[VERIFIED mykernel/arch/x86_64/mm/mmu/mmu_arch.c::switch_mm_irqs_off · MC15-A106，212–214行]**

```c

	load_new_mm_cr3(next->pgd, new_asid, need_flush);

```

**[VERIFIED mykernel/arch/x86_64/mm/mmu/mmu_arch.c::load_new_mm_cr3 · MC15-A107，24–26行]**

```c
	ulong new_mm_cr3;
	new_mm_cr3 = (ulong)virt_to_phys(pgdir);

```

**[VERIFIED mykernel/arch/x86_64/mm/mmu/mmu_arch.c::load_new_mm_cr3 · MC15-A108，38–40行]**

```c
	 */
	write_cr3(new_mm_cr3);
}
```

**[VERIFIED mykernel/arch/x86_64/insns/special_insns_arch.h::write_cr3 · MC15-A109，125–127行]**

```c
		write_cr3(ulong val) {
			asm volatile(	"mov	%0,		%%cr3	\n\t"
						:
```


## 五、状态/所有权、反证及释放边界

所有权表按facts的`MC15-O01～O10`保存：`task.mm`是任务用户地址空间指针，`task.active_mm`可能为借用值；`mm_users`因mmget/mmput而变化，`mm_refcount`在这些函数中没有同样变化；`mm.pgd`是页表根而非全部VMA/PTE；exec的bprm.mm是安装前的临时拥有字段。不能凭字段别名判断引用数。

输入矩阵`MC15-M01～M19`覆盖无current.mm、CLONE_VM共享、独立dup、mm/PGD/VMA失败、copy_process后续失败、exec有/无old_mm、同mm/异mm切换、内核任务借用/转移与归还、mmput用户计数非零/零、直接mmdrop、init_mm以及get_task_mm的PF_KTHREAD门。**所有表内计数推导都是固定源码条件下的算术，不是实际并发观测。**

出口方面，`do_exit`的`exit_mm`被注释，`__mmput`的`exit_mmap`和`mmdrop`也被注释；`exec_mmap`的old_mm非NULL分支即便mmput到0也不会通过该__mmput清空旧VMA/页表。另一条old_mm NULL分支却可能直接mmdrop，因此不能断言“全部mm对象都永远不释放”。此外`mmdrop`虽能直接调用pgd_free/free_mm，但无refcount最后持有判定，不能把API名字当作安全的最后引用回收器。旧14关于VMA列表摘链不清PTE/TLB的静态断点继续有效。

**[VERIFIED mykernel/sched/forkexec/exit.c::do_exit · MC15-A110，119–121行]**

```c

	// exit_mm();

```

**[VERIFIED mykernel/sched/forkexec/fork.c::__mmput · MC15-A021，439–441行]**

```c
	// khugepaged_exit(mm); /* must run before exit_mmap */
	// exit_mmap(mm);
	// mm_put_huge_zero_page(mm);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::__mmput · MC15-A022，449–451行]**

```c
	// 	module_put(mm->binfmt->module);
	// mmdrop(mm);
}
```

**[VERIFIED mykernel/sched/forkexec/fork.c::mmdrop · MC15-A011，268–270行]**

```c
	// put_user_ns(mm->user_ns);
	free_mm(mm);
}
```


**反证清单：** users归零≠真正释放；task.mm与active_mm相同≠两次ref；memcpy(mm)≠独立页；写task.mm≠切换CR3；write_cr3源码存在≠运行及跨CPU TLB正确；失败返回≠此前VMA/file副作用被回滚；有mmdrop函数≠所有出口调用它。普通问题的有界条件/反证/未知与既有`VL14-F03/F04`、`FC13-F05`、退出MM相关项的追加证据只写入`deferred-findings.yaml`。

**本轮禁止与未知：** 不修内核、不复制/生成补丁、不预处理/编译/运行C/ASM/QEMU、不验证硬件TLB或多CPU任务、无外部libc/镜像读取。不做第13批缓存、第14批分割或完整04页表分析。本包只申请`READY_FOR_MAINLINE_REVIEW`，`mainline_acceptance=PENDING`，静态验收上限`PASS_PENDING_LOCAL`。
