# CORE-USER-EXEC-COMMIT-GATES-71 — 固定 time 短源码证据

只记录源代码本身：所有片段均为所属单一函数/宏/类型中的连续1–5个非空物理行，且保留原制表符。调用出现和返回路径不证明真实线程独占、有效引用、构建结果、物理异常处理或系统调用返回。

## A — 直接定义证据

### A01 — de_thread定义与signal/lock前置段落注释 (L684–L688)

[VERIFIED mykernel/sched/forkexec/exec.c::de_thread]
```c
static int
de_thread(task_s *tsk) {
	// struct signal_struct *sig = tsk->signal;
	// struct sighand_struct *oldsighand = tsk->sighand;
	// spinlock_t *lock = &oldsighand->siglock;
```

### A02 — thread_group_empty的直接无组门是注释 (L690–L691)

[VERIFIED mykernel/sched/forkexec/exec.c::de_thread]
```c
	// if (thread_group_empty(tsk))
	// 	goto no_thread_group;
```

### A03 — 线程组并发动作开始段是注释 (L696–L700)

[VERIFIED mykernel/sched/forkexec/exec.c::de_thread]
```c
	// spin_lock_irq(lock);
	// if ((sig->flags & SIGNAL_GROUP_EXIT) || sig->group_exec_task) {
	// 	/*
	// 	 * Another group action in progress, just
	// 	 * return so that the signal is processed.
```

### A04 — group_exec_task及zap_other_threads均被注释 (L706–L709)

[VERIFIED mykernel/sched/forkexec/exec.c::de_thread]
```c
	// sig->group_exec_task = tsk;
	// sig->notify_count = zap_other_threads(tsk);
	// if (!thread_group_leader(tsk))
	// 	sig->notify_count--;
```

### A05 — 等待组内其他任务的状态与调度被注释 (L711–L715)

[VERIFIED mykernel/sched/forkexec/exec.c::de_thread]
```c
	// while (sig->notify_count) {
	// 	__set_current_state(TASK_KILLABLE);
	// 	spin_unlock_irq(lock);
	// 	schedule();
	// 	if (__fatal_signal_pending(tsk))
```

### A06 — 领导者等待内的fatal goto是注释 (L742–L744)

[VERIFIED mykernel/sched/forkexec/exec.c::de_thread]
```c
	// 		schedule();
	// 		if (__fatal_signal_pending(tsk))
	// 			goto killed;
```

### A07 — PID/TGID交换和转移为注释 (L771–L774)

[VERIFIED mykernel/sched/forkexec/exec.c::de_thread]
```c
	// 	exchange_tids(tsk, leader);
	// 	transfer_pid(leader, tsk, PIDTYPE_TGID);
	// 	transfer_pid(leader, tsk, PIDTYPE_PGID);
	// 	transfer_pid(leader, tsk, PIDTYPE_SID);
```

### A08 — notify清理被注释后是no_thread_group标签 (L800–L803)

[VERIFIED mykernel/sched/forkexec/exec.c::de_thread]
```c
	// sig->group_exec_task = NULL;
	// sig->notify_count = 0;

no_thread_group:
```

### A09 — no_thread_group处活动返回0 (L804–L808)

[VERIFIED mykernel/sched/forkexec/exec.c::de_thread]
```c
	// /* we have changed execution domain */
	// tsk->exit_signal = SIGCHLD;

	// BUG_ON(!thread_group_leader(tsk));
	return 0;
```

### A10 — killed标签存在但其清理语句注释 (L810–L814)

[VERIFIED mykernel/sched/forkexec/exec.c::de_thread]
```c
killed:
	// /* protects against exit_notify() and __exit_signal() */
	// read_lock(&tasklist_lock);
	// sig->group_exec_task = NULL;
	// sig->notify_count = 0;
```

### A11 — killed标签下活动return -EAGAIN (L815–L816)

[VERIFIED mykernel/sched/forkexec/exec.c::de_thread]
```c
	// read_unlock(&tasklist_lock);
	return -EAGAIN;
```

### A12 — ENOERR真实宏值为0 (L5–L5)

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOERR]
```c
	#define	ENOERR		0	/*  */
```

### A13 — point_of_no_return在准入门前被置为真 (L869–L872)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	/*
	 * Ensure all future errors are fatal.
	 */
	bprm->point_of_no_return = true;
```

### A14 — de_thread返回与retval非负零门 (L875–L879)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	 * Make this the only thread in the thread group.
	 */
	retval = de_thread(me);
	if (retval != -ENOERR)
		goto out;
```

### A15 — unshare_files后非零goto out门 (L886–L889)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	/* Ensure the files table is not shared. */
	retval = unshare_files();
	if (retval)
		goto out;
```

### A16 — set_mm_exe_file返回门 (L896–L898)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	retval = set_mm_exe_file(bprm->mm, bprm->file);
	if (retval)
		goto out;
```

### A17 — exec_mmap返回零才清bprm.mm (L909–L913)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	retval = exec_mmap(bprm->mm);
	if (retval)
		goto out;

	bprm->mm = NULL;
```

### A18 — ELF上游调用begin_new_exec及失败跳转 (L798–L801)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	/* Flush all traces of the currently running executable */
	retval = begin_new_exec(bprm);
	if (retval)
		goto out_free_dentry;
```

### A19 — have_execfd内get_unused_fd_flags失败跳转 (L1013–L1017)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	/* Pass the opened binary to the interpreter. */
	if (bprm->have_execfd) {
		retval = get_unused_fd_flags(0);
		if (retval < 0)
			goto out_unlock;
```

### A20 — fd_install成功消费指针并返回0 (L1018–L1022)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
		fd_install(retval, bprm->executable);
		bprm->executable = NULL;
		bprm->execfd = retval;
	}
	return 0;
```

### A21 — out_unlock含注释的解锁动作 (L1024–L1028)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
out_unlock:
	// up_write(&me->signal->exec_update_lock);
	// if (!bprm->cred)
	// 	mutex_unlock(&me->signal->cred_guard_mutex);

```

### A22 — out仅返回retval未活动恢复状态 (L1029–L1030)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
out:
	return retval;
```

## B — 直接定义证据

### B01 — unshare_fd入口定义及current.files (L1580–L1584)

[VERIFIED mykernel/sched/forkexec/fork.c::unshare_fd]
```c
int unshare_fd(unsigned long unshare_flags,
		unsigned int max_fds, files_struct_s **new_fdp)
{
	files_struct_s *fd = current->files;
	int error = 0;
```

### B02 — 仅共享文件表refcount大于1才dup_fd并检测错误 (L1586–L1590)

[VERIFIED mykernel/sched/forkexec/fork.c::unshare_fd]
```c
	if ((unshare_flags & CLONE_FILES) &&
		(fd && atomic_read(&fd->refcount) > 1)) {
		*new_fdp = dup_fd(fd, max_fds, &error);
		if (!*new_fdp)
			return error;
```

### B03 — 没有复制也可以返回零 (L1591–L1594)

[VERIFIED mykernel/sched/forkexec/fork.c::unshare_fd]
```c
	}

	return 0;
}
```

### B04 — unshare_files将copy输出变量初始化NULL (L1603–L1607)

[VERIFIED mykernel/sched/forkexec/fork.c::unshare_files]
```c
int unshare_files(void)
{
	task_s *task = current;
	files_struct_s *old, *copy = NULL;
	int error;
```

### B05 — 只有得到非空copy才继续 (L1609–L1611)

[VERIFIED mykernel/sched/forkexec/fork.c::unshare_files]
```c
	error = unshare_fd(CLONE_FILES, NR_OPEN_MAX, &copy);
	if (error || !copy)
		return error;
```

### B06 — 先替换task.files再归还旧表 (L1613–L1617)

[VERIFIED mykernel/sched/forkexec/fork.c::unshare_files]
```c
	old = task->files;
	task_lock(task);
	task->files = copy;
	task_unlock(task);
	put_files_struct(old);
```

### B07 — put_files_struct之后返回零 (L1617–L1619)

[VERIFIED mykernel/sched/forkexec/fork.c::unshare_files]
```c
	put_files_struct(old);
	return 0;
}
```

### B08 — dup_fd源与目标files表局部 (L29–L33)

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
files_struct_s *dup_fd(files_struct_s *oldf, unsigned int max_fds, int *errorp)
{
	files_struct_s *newf;
	file_s **old_fds, **new_fds;
	unsigned int open_files, i;
```

### B09 — 分配前设置错误输出ENOMEM并有NULL门 (L35–L38)

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	*errorp = -ENOMEM;
	newf = kmem_cache_alloc(files_cachep, GFP_KERNEL);
	if (!newf)
		goto out;
```

### B10 — 新表refcount初值1与fd_count初始化 (L40–L43)

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	atomic_set(&newf->refcount, 1);
	newf->fd_count = NR_OPEN_DEFAULT;

	spin_lock_init(&newf->file_lock);
```

### B11 — 读取源fd_array与目标fd_array (L48–L49)

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	old_fds = oldf->fd_array;
	new_fds = newf->fd_array;
```

### B12 — 按max_fds遍历且非空旧槽走复制 (L51–L55)

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	for (i = 0; i < max_fds; i++)
	{
		file_s *old_fp = old_fds[i];
		if (old_fp != NULL)
		{
```

### B13 — 为每个非空旧槽分配新file并按值复制 (L56–L59)

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
			file_s *new_fp = kzalloc(sizeof(file_s), GFP_KERNEL);
			*new_fp = *old_fp;
			new_fds[i] = new_fp;
		}	
```

### B14 — 复制正常完成后返回newf指针 (L62–L64)

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	// rcu_assign_pointer(newf->fdt, new_fdt);

	return newf;
```

### B15 — 分配早期失败清理出口 (L66–L69)

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
out_release:
	kmem_cache_free(files_cachep, newf);
out:
	return NULL;
```

### B16 — put_files_struct仅最后表引用才遍历 (L72–L75)

[VERIFIED mykernel/fs/vfs/file.c::put_files_struct]
```c
void put_files_struct(files_struct_s *files)
{
	if (atomic_dec_and_test(&files->refcount)) {
	// static fdtable_s *close_files(files_struct_s * files)
```

### B17 — 遍历旧表各槽并调用filp_close (L83–L87)

[VERIFIED mykernel/fs/vfs/file.c::put_files_struct]
```c
		unsigned int i = 0;
		for(i = 0; i < files->fd_count; i++) {
			file_s *file = files->fd_array[i];
			if(file != NULL)
				filp_close(file);
```

### B18 — 表归还请求在循环后 (L91–L95)

[VERIFIED mykernel/fs/vfs/file.c::put_files_struct]
```c
		/* free the arrays if they are not embedded */
		// if (fdt != &files->fdtab)
		// 	__free_fdtable(fdt);
		kmem_cache_free(files_cachep, files);
	}
```

### B19 — files_struct拥有独立refcount (L48–L52)

[VERIFIED mykernel/include/linux/kernel/fdtable.h::files_struct]
```c
	typedef struct files_struct {
		/*
		 * read mostly part
		 */
		atomic_t		refcount;
```

### B20 — CLONE_FS路径检查fs.in_exec (L705–L709)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_fs]
```c
	if (clone_flags & CLONE_FS) {
		/* tsk->fs is already what we want */
		// spin_lock(&fs->lock);
		if (fs->in_exec) {
			// spin_unlock(&fs->lock);
```

### B21 — in_exec非零拒绝否则递增users (L708–L712)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_fs]
```c
		if (fs->in_exec) {
			// spin_unlock(&fs->lock);
			return -EAGAIN;
		}
		fs->users++;
```

### B22 — copy_fs_struct创建独立fs且in_exec初值0 (L113–L117)

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
	taskfs_s *fs = kmem_cache_alloc(fs_cachep, GFP_KERNEL);
	/* We don't need to lock fs - think why ;-) */
	if (fs) {
		fs->users = 1;
		fs->in_exec = 0;
```

### B23 — fs_struct含users及in_exec，缩到10至14 (L10–L14)

[VERIFIED mykernel/sched/misc/fs_struct_types.h::fs_struct]
```c
		int			users;
		spinlock_t	lock;
		// seqcount_spinlock_t seq;
		int			umask;
		int			in_exec;
```

### B24 — bprm_execve在分派前设置task.in_execve (L1243–L1246)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	//  */
	// check_unsafe_exec(bprm);
	curr->in_execve = 1;
	// sched_mm_cid_before_execve(current);
```

### B25 — handler非负正常出口清fs.in_exec和task.in_execve (L1263–L1266)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	// sched_mm_cid_after_execve(current);
	/* execve succeeded */
	curr->fs->in_exec = 0;
	curr->in_execve = 0;
```

### B26 — 负返回出口也清两个状态 (L1283–L1287)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	// sched_mm_cid_after_execve(current);
	current->fs->in_exec = 0;
	current->in_execve = 0;

	return retval;
```

### B27 — exec提交后清四项task.flags (L933–L936)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c

	me->flags &= ~(PF_RANDOMIZE | PF_FORKNOEXEC |
					PF_NOFREEZE | PF_NO_SETAFFINITY);
	// flush_thread();
```

### B28 — do_close_on_exec语句是注释 (L943–L947)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	//  * dumpable (in setup_new_exec) to avoid a race with a process in userspace
	//  * trying to access the should-be-closed file descriptors of a process
	//  * undergoing exec(2).
	//  */
	// do_close_on_exec(me->files);
```

### B29 — 清sas_ss_sp与sas_ss_size (L964–L964)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	me->sas_ss_sp = me->sas_ss_size = 0;
```

### B30 — 调用__set_task_comm设置任务名称 (L978–L979)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	// perf_event_exec();
	__set_task_comm(me, bprm->filename);
```

### B31 — __set_task_comm在锁内拷贝comm文本 (L836–L840)

[VERIFIED mykernel/sched/forkexec/exec.c::__set_task_comm]
```c
void __set_task_comm(task_s *tsk, const char *buf)
{
	task_lock(tsk);
	// trace_task_rename(tsk, buf);
	strscpy_pad(tsk->comm, buf, sizeof(tsk->comm));
```

### B32 — flush_signal_handlers语句被注释 (L981–L984)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	// /* An exec changes our domain. We are no longer part of the thread
	//    group */
	// WRITE_ONCE(me->self_exec_id, me->self_exec_id + 1);
	// flush_signal_handlers(me, 0);
```

### B33 — setup_new_exec调用架构MM布局函数 (L1035–L1039)

[VERIFIED mykernel/sched/forkexec/exec.c::setup_new_exec]
```c
{
	/* Setup things that can depend upon the personality */
	task_s *me = current;

	arch_pick_mmap_layout(me->mm, &bprm->rlim_stack);
```

### B34 — setup_new_exec修改mm.task_size (L1047–L1049)

[VERIFIED mykernel/sched/forkexec/exec.c::setup_new_exec]
```c
	 */
	me->mm->task_size = TASK_SIZE;
	// up_write(&me->signal->exec_update_lock);
```

### B35 — finalize_exec入口及rlimit重设是注释 (L1055–L1059)

[VERIFIED mykernel/sched/forkexec/exec.c::finalize_exec]
```c
void finalize_exec(linux_bprm_s *bprm)
{
	// /* Store any stack rlimit changes before starting thread. */
	// task_lock(current->group_leader);
	// current->signal->rlim[RLIMIT_STACK] = bprm->rlim_stack;
```

### B36 — task的in_execve是独立标志位 (L184–L186)

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c

		/* Bit to tell LSMs we're in execve(): */
		uint			in_execve					: 1;
```

### B37 — task的fs及files是两个不同指针 (L338–L342)

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		/* Filesystem information: */
		taskfs_s		*fs;

		/* Open file information: */
		files_struct_s	*files;
```

### B38 — task保存sas备用信号栈字段 (L359–L361)

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		ulong			sas_ss_sp;
		size_t			sas_ss_size;
		uint			sas_ss_flags;
```

### B39 — task保存comm字符数组 (L326–L326)

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		char			comm[TASK_COMM_LEN];
```

### B40 — exec_mmap确实更新task.active_mm与task.mm (L652–L656)

[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap]
```c
	local_irq_disable();
	active_mm = tsk->active_mm;
	tsk->active_mm = mm;
	tsk->mm = mm;
	// mm_init_cid(mm);
```

### B41 — exec_mmap要求activate_mm并恢复IRQ (L666–L668)

[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap]
```c
	activate_mm(active_mm, mm);
	// if (IS_ENABLED(CONFIG_ARCH_WANT_IRQS_OFF_ACTIVATE_MM))
		local_irq_enable();
```

### B42 — old_mm非空时BUG_ON门与mmput后返回0 (L674–L678)

[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap]
```c
		BUG_ON(active_mm != old_mm);
		// setmax_mm_hiwater_rss(&tsk->signal->maxrss, old_mm);
		// mm_update_next_owner(old_mm);
		mmput(old_mm);
		return 0;
```

### B43 — old_mm为空时mmdrop再返回0 (L679–L681)

[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap]
```c
	}
	mmdrop(active_mm);
	return 0;
```

### B44 — 非空new_exe_file才get_file (L493–L495)

[VERIFIED mykernel/sched/forkexec/fork.c::set_mm_exe_file]
```c
		// if (deny_write_access(new_exe_file))
		// 	return -EACCES;
		get_file(new_exe_file);
```

### B45 — 旧exe_file若非空则fput，正常出口返回0 (L499–L503)

[VERIFIED mykernel/sched/forkexec/fork.c::set_mm_exe_file]
```c
	if (old_exe_file) {
		// allow_write_access(old_exe_file);
		fput(old_exe_file);
	}
	return 0;
```

## C — 直接定义证据

### C01 — point_of_no_return是bprm布尔位定义 (L35–L39)

[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
			/*
			 * Set when errors can no longer be returned to the
			 * original userspace.
			 */
			point_of_no_return:1;
```

### C02 — 提交标志在de_thread之前直接置true (L872–L872)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	bprm->point_of_no_return = true;
```

### C03 — 提交之后have_execfd错误可goto out_unlock (L1013–L1017)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	/* Pass the opened binary to the interpreter. */
	if (bprm->have_execfd) {
		retval = get_unused_fd_flags(0);
		if (retval < 0)
			goto out_unlock;
```

### C04 — out_unlock内的解锁均为注释 (L1024–L1028)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
out_unlock:
	// up_write(&me->signal->exec_update_lock);
	// if (!bprm->cred)
	// 	mutex_unlock(&me->signal->cred_guard_mutex);

```

### C05 — begin_new_exec out直接返回retval (L1029–L1030)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
out:
	return retval;
```

### C06 — bprm_execve函数形参及retval变量 (L1227–L1231)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
static int
bprm_execve(linux_bprm_s *bprm, int fd,
		filename_s *filename, int flags) {

	int retval;
```

### C07 — 分派前置task.in_execve标志 (L1244–L1246)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	// check_unsafe_exec(bprm);
	curr->in_execve = 1;
	// sched_mm_cid_before_execve(current);
```

### C08 — search_binary_handler返回负值时转out (L1258–L1261)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
		retval = search_binary_handler(bprm);
	// }
	if (retval < 0)
		goto out;
```

### C09 — 处理器非负结果清fs.in_exec和in_execve (L1264–L1266)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	/* execve succeeded */
	curr->fs->in_exec = 0;
	curr->in_execve = 0;
```

### C10 — 非负状态直接返回retval (L1269–L1271)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	// acct_update_integrals(current);
	// task_numa_free(current, false);
	return retval;
```

### C11 — 致命失败处理文字仅为注释 (L1273–L1277)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
out:
	// /*
	//  * If past the point of no return ensure the code never
	//  * returns to the userspace process.  Use an existing fatal
	//  * signal if present otherwise terminate the process with
```

### C12 — force_fatal_sig条件语句为注释 (L1278–L1282)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	//  * SIGSEGV.
	//  */
	// if (bprm->point_of_no_return && !fatal_signal_pending(current))
	// 	force_fatal_sig(SIGSEGV);

```

### C13 — 错误出口清fs及task标志并返回原retval (L1283–L1287)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	// sched_mm_cid_after_execve(current);
	current->fs->in_exec = 0;
	current->in_execve = 0;

	return retval;
```

### C14 — 普通exec调用链入口 (L1290–L1294)

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
static int
do_execveat_common(int fd, filename_s *filename,
	const char *const *argv, const char *const *envp, int flags) {

	linux_bprm_s *bprm;
```

### C15 — filename错误返回门 (L1297–L1299)

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	if (IS_ERR(filename))
		return PTR_ERR(filename);

```

### C16 — 普通exec先清PF_NPROC_EXCEEDED再alloc_bprm (L1313–L1317)

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	 * further execve() calls fail. */
	current->flags &= ~PF_NPROC_EXCEEDED;

	bprm = alloc_bprm(fd, filename, flags);
	if (IS_ERR(bprm)) {
```

### C17 — bprm分配失败时到out_ret (L1316–L1319)

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	bprm = alloc_bprm(fd, filename, flags);
	if (IS_ERR(bprm)) {
		retval = PTR_ERR(bprm);
		goto out_ret;
```

### C18 — 普通执行bprm_execve后free_bprm (L1365–L1367)

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = bprm_execve(bprm, fd, filename, flags);
out_free:
	free_bprm(bprm);
```

### C19 — 普通调用者putname并返回retval (L1369–L1371)

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
out_ret:
	putname(filename);
	return retval;
```

### C20 — 内核exec进入时PF_KTHREAD拒绝门 (L1385–L1387)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	/* It is non-sense for kernel threads to call execve */
	if (WARN_ON_ONCE(current->flags & PF_KTHREAD))
		return -EINVAL;
```

### C21 — 内核exec bprm分配失败门 (L1393–L1397)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	bprm = alloc_bprm(fd, filename, 0);
	if (IS_ERR(bprm)) {
		retval = PTR_ERR(bprm);
		goto out_ret;
	}
```

### C22 — 内核exec分派后也统一free_bprm (L1428–L1430)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = bprm_execve(bprm, fd, filename, 0);
out_free:
	free_bprm(bprm);
```

### C23 — 内核exec putname并返回retval (L1432–L1434)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
out_ret:
	putname(filename);
	return retval;
```

### C24 — do_execve调用do_execveat_common (L1438–L1442)

[VERIFIED mykernel/sched/forkexec/exec.c::do_execve]
```c
do_execve(filename_s *filename,
		const char *const *__argv,
		const char *const *__envp) {
	return do_execveat_common(AT_FDCWD,
			filename, __argv, __envp, 0);
```

### C25 — execve系统调用入口转到do_execve (L1460–L1464)

[VERIFIED mykernel/sched/forkexec/exec.c::MYOS_SYSCALL_DEFINE3(execve)]
```c
MYOS_SYSCALL_DEFINE3(execve, const char *,filename,
		const char *const __user *, argv,
		const char *const __user *, envp) {
	return do_execve(getname(filename), argv, envp);
}
```

### C26 — 初始化桥对kernel_execve返回值未存储 (L30–L33)

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
	// 因为这里是内核线程，要通过execve进用户态
	// 需要通过current->mm和active_mm的检查，所以设置下mm
	curr->mm = curr->active_mm;
	kernel_execve(initd_name, argv, envp);
```

### C27 — 初始化桥下一个控制意图为汇编跳转 (L35–L39)

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
	hwtss_cpu0 = this_cpu_ptr(&(cpu_tss_rw.x86_tss));

	asm volatile(	"movq	%0,		%%rsp		\n\t"
					// "jmp	sysexit_entp		\n\t"
					"jmp	syscall_return_via_sysret	\n\t"
```

### C28 — ELF对begin_new_exec失败门 (L798–L801)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	/* Flush all traces of the currently running executable */
	retval = begin_new_exec(bprm);
	if (retval)
		goto out_free_dentry;
```

### C29 — 提交成功后调用setup_new_exec (L813–L813)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	setup_new_exec(bprm);
```

### C30 — 提交后setup_arg_pages负值错误出口 (L817–L820)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	retval = setup_arg_pages(bprm,
				randomize_stack_top(STACK_TOP), executable_stack);
	if (retval < 0)
		goto out_free_dentry;
```

### C31 — 提交后计算total_size失败跳出 (L890–L892)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
			if (!total_size) {
				retval = -EINVAL;
				goto out_free_dentry;
```

### C32 — 提交后elf_load错误跳转 (L971–L974)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		if (BAD_ADDR(error)) {
			retval = IS_ERR_VALUE((void *)error) ?
				PTR_ERR((void*)error) : -EINVAL;
			goto out_free_dentry;
```

### C33 — ELF成功尾段及retval0 (L1130–L1134)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	finalize_exec(bprm);
	START_THREAD(elf_ex, regs, elf_entry, bprm->p);
	retval = 0;
out:
	return retval;
```

### C34 — ELF错误清理入口及局部资源归还 (L1137–L1141)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
out_free_dentry:
	kfree(interp_elf_ex);
	kfree(interp_elf_phdata);
out_free_file:
	if (interpreter)
```

### C35 — ELF错误尾部归还解释器并返回out (L1141–L1145)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	if (interpreter)
		fput(interpreter);
out_free_ph:
	kfree(elf_phdata);
	goto out;
```

### C36 — point_of_no_return阻止继续的条件代码全被注释 (L1203–L1207)

[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
		// put_binfmt(fmt);
		// if (bprm->point_of_no_return || (retval != -ENOEXEC)) {
		// 	read_unlock(&binfmt_lock);
		// 	return retval;
		// }
```

