# CORE-USER-EXEC-RESOURCE-OWNERSHIP-70 — 固定源码短引文

以下引文来自固定`time@a039d9803ade`直接源码对象。每条含文件路径和真实单一函数/宏/结构对象，连续1–5物理行、原样制表符和空行；不将源级调用冒充编译、CPU运行或实际释放。

### D01 — 旧69 C39分拆：FIXED_BIT独立定义 (L38–L38)

[VERIFIED mykernel/arch/x86_64/processor/processor_const_arch.h::X86_EFLAGS_FIXED_BIT]
```c
	#define X86_EFLAGS_FIXED_BIT	1 /* Bit 1 - always on */
```

### D02 — 旧69 C39分拆：FIXED独立定义 (L39–L39)

[VERIFIED mykernel/arch/x86_64/processor/processor_const_arch.h::X86_EFLAGS_FIXED]
```c
	#define X86_EFLAGS_FIXED		_BITUL(X86_EFLAGS_FIXED_BIT)
```

### D03 — 旧69 C40分拆：IF_BIT独立定义 (L50–L50)

[VERIFIED mykernel/arch/x86_64/processor/processor_const_arch.h::X86_EFLAGS_IF_BIT]
```c
	#define X86_EFLAGS_IF_BIT		9 /* Interrupt Flag */
```

### D04 — 旧69 C40分拆：IF独立定义 (L51–L51)

[VERIFIED mykernel/arch/x86_64/processor/processor_const_arch.h::X86_EFLAGS_IF]
```c
	#define X86_EFLAGS_IF			_BITUL(X86_EFLAGS_IF_BIT)
```

### A01 — 格式链表的静态声明 (L41–L41)

[VERIFIED mykernel/sched/forkexec/exec.c::formats]
```c
static DECLARE_LIST_HDR_S(formats);
```

### A02 — 注册接口追加fmt节点 (L44–L48)

[VERIFIED mykernel/sched/forkexec/exec.c::register_binfmt]
```c
void register_binfmt(linux_bfmt_s * fmt)
{
	// write_lock(&binfmt_lock);
	list_header_add_to_tail(&formats, &fmt->lh);
	// write_unlock(&binfmt_lock);
```

### A03 — 注销接口删除fmt节点 (L51–L55)

[VERIFIED mykernel/sched/forkexec/exec.c::unregister_binfmt]
```c
void unregister_binfmt(linux_bfmt_s * fmt)
{
	// write_lock(&binfmt_lock);
	// list_del_init(&fmt->lh);
	list_header_delete_node(&formats, &fmt->lh);
```

### A04 — 处理器列表节点与load_binary类型 (L94–L98)

[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binfmt]
```c
	typedef struct linux_binfmt {
		List_s		lh;
		// module_s	*module;
		int			(*load_binary)(linux_bprm_s *);
		int			(*load_shlib)(file_s *);
```

### A05 — elf_format及load_binary绑定 (L83–L86)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_format]
```c
static linux_bfmt_s elf_format = {
	// .module			= THIS_MODULE,
	.load_binary	= load_elf_binary,
	.load_shlib		= load_elf_library,
```

### A06 — elf_format其余字段及闭合 (L87–L89)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_format]
```c
	// .core_dump		= elf_core_dump,
	.min_coredump	= ELF_EXEC_PAGESIZE,
};
```

### A07 — ELF对象初始化并调用注册 (L1215–L1219)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::init_elf_binfmt]
```c
int __init init_elf_binfmt(void)
{
	INIT_LIST_S(&elf_format.lh);
	register_binfmt(&elf_format);
	return 0;
```

### A08 — ELF格式注销函数 (L1223–L1227)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::exit_elf_binfmt]
```c
static void __exit
exit_elf_binfmt(void) {
	/* Remove the COFF and ELF loaders. */
	unregister_binfmt(&elf_format);
}
```

### A09 — do_initcalls直接调init_elf_binfmt (L216–L220)

[VERIFIED mykernel/init/main.c::do_initcalls]
```c
	/* early_initcall (fn, early) */
	init_sigframe_size();

	/* core_initcall (fn, 1) */
	init_elf_binfmt();
```

### A10 — do_basic_setup直接调do_initcalls (L247–L250)

[VERIFIED mykernel/init/main.c::do_basic_setup]
```c
	// init_irq_proc();
	// do_ctors();
	do_initcalls();
}
```

### A11 — 初始化retval并遍历formats (L1192–L1196)

[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
	retval = -ENOENT;
//  retry:
	// read_lock(&binfmt_lock);
	list_header_for_each_container(fmt, &formats, lh) {
		// if (!try_module_get(fmt->module))
```

### A12 — 循环调用fmt的load_binary (L1198–L1201)

[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
		// read_unlock(&binfmt_lock);

		retval = fmt->load_binary(bprm);

```

### A13 — 提前返回门全是注释 (L1204–L1207)

[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
		// if (bprm->point_of_no_return || (retval != -ENOEXEC)) {
		// 	read_unlock(&binfmt_lock);
		// 	return retval;
		// }
```

### A14 — 循环后的最终retval返回 (L1220–L1222)

[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c

	return retval;
}
```

### A15 — 静态格式头自环和count初值 (L9–L12)

[VERIFIED mykernel/lib/list/double_list_macro.h::LIST_HEADER_INIT]
```c
	#define LIST_HEADER_INIT(name) {				\
				.anchor	= LIST_INIT(name.anchor),	\
				.count	= 0,						\
			}
```

### A16 — DECLARE_LIST_HDR_S定义格式头对象 (L16–L17)

[VERIFIED mykernel/lib/list/double_list_macro.h::DECLARE_LIST_HDR_S]
```c
	#define DECLARE_LIST_HDR_S(name) \
				List_hdr_s name = LIST_HEADER_INIT(name)
```

### A17 — 格式遍历转到list_for_each_entry (L346–L347)

[VERIFIED mykernel/lib/list/double_list_macro.h::list_header_for_each_container]
```c
	#define list_header_for_each_container(pos, header, member)	\
				list_for_each_entry(pos, &((header)->anchor), member)
```

### A18 — 先取首节点然后测试是否头 (L173–L176)

[VERIFIED mykernel/lib/list/double_list_macro.h::list_for_each_entry]
```c
	#define list_for_each_entry(pos, head, member)							\
				for (pos = list_first_entry(head, typeof(*pos), member);	\
					!list_entry_is_head(pos, head, member);					\
					pos = list_next_entry(pos, member))
```

### A19 — 遍历以成员地址和头比较 (L164–L165)

[VERIFIED mykernel/lib/list/double_list_macro.h::list_entry_is_head]
```c
	#define list_entry_is_head(pos, head, member)				\
				(&pos->member == (head))
```

### A20 — 追加节点并更新count (L663–L667)

[VERIFIED mykernel/lib/list/double_list.h::list_header_add_to_tail]
```c
		void
		list_header_add_to_tail(List_hdr_s *lhdr_p, List_s *l_p) {
			list_add_to_prev(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
		}
```

### A21 — 删除节点减少count (L683–L687)

[VERIFIED mykernel/lib/list/double_list.h::list_header_delete_node]
```c
		*list_header_delete_node(List_hdr_s *lhdr_p, List_s *l_p) {
			if (list_header_contains(lhdr_p, l_p)) {
				list_del_init(l_p);
				lhdr_p->count--;
				return l_p;
```

### A22 — bprm类型含独立mm字段 (L17–L21)

[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
	typedef struct linux_binprm {
		vma_s			*vma;
		ulong			vma_pages;
		mm_s			*mm;
		ulong			p; /* current top of mem */
```

### A23 — 注册函数中锁路径被注释 (L46–L48)

[VERIFIED mykernel/sched/forkexec/exec.c::register_binfmt]
```c
	// write_lock(&binfmt_lock);
	list_header_add_to_tail(&formats, &fmt->lh);
	// write_unlock(&binfmt_lock);
```

### A24 — ELF成功尾端调用START_THREAD (L1130–L1134)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	finalize_exec(bprm);
	START_THREAD(elf_ex, regs, elf_entry, bprm->p);
	retval = 0;
out:
	return retval;
```

### B01 — do_open_execat的入口和open flags局部 (L554–L558)

[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
static file_s
*do_open_execat(int fd, filename_s *name, int flags) {
	file_s *file;
	int err;
	open_flags_s open_exec_flags = {
```

### B02 — exec路径参数的有限flags门 (L565–L569)

[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	if ((flags & ~(AT_SYMLINK_NOFOLLOW | AT_EMPTY_PATH)) != 0)
		return ERR_PTR(-EINVAL);
	if (flags & AT_SYMLINK_NOFOLLOW)
		open_exec_flags.lookup_flags &= ~LOOKUP_FOLLOW;
	if (flags & AT_EMPTY_PATH)
```

### B03 — do_filp_open返回file并检查错误指针 (L572–L574)

[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	file = do_filp_open(fd, name, &open_exec_flags);
	if (IS_ERR(file))
		goto out;
```

### B04 — do_open_execat正常返回和错误标签 (L586–L590)

[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
out:
	return file;

exit:
	fput(file);
```

### B05 — alloc_bprm入口及file/bprm局部 (L1097–L1101)

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
static linux_bprm_s
*alloc_bprm(int fd, filename_s *filename, int flags) {
	task_s * curr = current;

	linux_bprm_s *bprm;
```

### B06 — 成功获取file之后错误指针门 (L1105–L1107)

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	file = do_open_execat(fd, filename, flags);
	if (IS_ERR(file))
		return ERR_CAST(file);
```

### B07 — bprm分配失败先关闭file (L1109–L1113)

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	bprm = kzalloc(sizeof(*bprm), GFP_KERNEL);
	if (bprm == NULL) {
		do_close_execat(file);
		return ERR_PTR(-ENOMEM);
	}
```

### B08 — bprm.file字段接file和路径分支 (L1115–L1119)

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	bprm->file = file;

	if (fd == AT_FDCWD || filename->name[0] == '/') {
		bprm->filename = filename->name;
	} else {
```

### B09 — fdpath内存分配两种路径 (L1120–L1124)

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
		if (filename->name[0] == '\0')
			bprm->fdpath = kasprintf(GFP_KERNEL, "/dev/fd/%d", fd);
		else
			bprm->fdpath = kasprintf(GFP_KERNEL, "/dev/fd/%d/%s",
						  fd, filename->name);
```

### B10 — fdpath分配为空时跳出 (L1125–L1126)

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
		if (bprm->fdpath == NULL)
			goto out_free;
```

### B11 — bprm.filename与interp初次别名 (L1140–L1142)

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
		bprm->filename = bprm->fdpath;
	}
	bprm->interp = bprm->filename;
```

### B12 — bprm_mm_init成功后交出bprm (L1144–L1146)

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	retval = bprm_mm_init(bprm);
	if (retval == 0)
		return bprm;
```

### B13 — alloc_bprm错误经free_bprm (L1148–L1150)

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
out_free:
	free_bprm(bprm);
	return ERR_PTR(retval);
```

### B14 — bprm_mm_init取得mm并保存到字段 (L156–L160)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_mm_init]
```c
static int
bprm_mm_init(linux_bprm_s *bprm) {
	int err;
	mm_s *mm = NULL;

```

### B15 — __bprm_mm_init成功后ret零 (L171–L175)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_mm_init]
```c
	err = __bprm_mm_init(bprm);
	if (err)
		goto err;

	return 0;
```

### B16 — 失败时bprm.mm置空及mmdrop (L177–L181)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_mm_init]
```c
err:
	if (mm) {
		bprm->mm = NULL;
		mmdrop(mm);
	}
```

### B17 — bprm私有mm和栈位置字段 (L17–L21)

[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
	typedef struct linux_binprm {
		vma_s			*vma;
		ulong			vma_pages;
		mm_s			*mm;
		ulong			p; /* current top of mem */
```

### B18 — have_execfd位字段 (L23–L25)

[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
		uint
			/* Should an execfd be passed to userspace? */
			have_execfd:1,
```

### B19 — point_of_no_return位字段 (L35–L39)

[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
			/*
			 * Set when errors can no longer be returned to the
			 * original userspace.
			 */
			point_of_no_return:1;
```

### B20 — 三个不同的file指针字段 (L43–L45)

[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
		file_s			*executable; /* Executable to pass to the interpreter */
		file_s			*interpreter;
		file_s			*file;
```

### B21 — argc与filename成员 (L49–L50)

[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
		int				argc, envc;
		const char		*filename;	/* Name of binary as seen by procps */
```

### B22 — interp字符串与fdpath (L51–L54)

[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
		const char		*interp;	/* Name of the binary really executed. Most
								   of the time same as filename, but could be
								   different for binfmt_{misc,script} */
		const char		*fdpath;	/* generated filename for execveat */
```

### B23 — begin_new_exec入口任务与retval (L851–L854)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
int begin_new_exec(linux_bprm_s * bprm)
{
	task_s *me = current;
	int retval;
```

### B24 — point_of_no_return提前写true (L869–L872)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	/*
	 * Ensure all future errors are fatal.
	 */
	bprm->point_of_no_return = true;
```

### B25 — de_thread返回门 (L877–L879)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	retval = de_thread(me);
	if (retval != -ENOERR)
		goto out;
```

### B26 — unshare_files返回门 (L887–L889)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	retval = unshare_files();
	if (retval)
		goto out;
```

### B27 — set_mm_exe_file返回门 (L896–L898)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	retval = set_mm_exe_file(bprm->mm, bprm->file);
	if (retval)
		goto out;
```

### B28 — exec_mmap成功才清bprm.mm (L909–L913)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	retval = exec_mmap(bprm->mm);
	if (retval)
		goto out;

	bprm->mm = NULL;
```

### B29 — have_execfd分支取得空闲fd (L1013–L1017)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	/* Pass the opened binary to the interpreter. */
	if (bprm->have_execfd) {
		retval = get_unused_fd_flags(0);
		if (retval < 0)
			goto out_unlock;
```

### B30 — fd_install然后置空executable (L1018–L1022)

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
		fd_install(retval, bprm->executable);
		bprm->executable = NULL;
		bprm->execfd = retval;
	}
	return 0;
```

### B31 — exec_mmap入口的地址空间对象 (L622–L625)

[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap]
```c
exec_mmap(mm_s *mm) {
	task_s *tsk;
	mm_s *old_mm, *active_mm;
	int ret;
```

### B32 — old_mm取得与exec_mm_release (L628–L630)

[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap]
```c
	tsk = current;
	old_mm = current->mm;
	exec_mm_release(tsk, old_mm);
```

### B33 — 更新active_mm与mm (L652–L656)

[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap]
```c
	local_irq_disable();
	active_mm = tsk->active_mm;
	tsk->active_mm = mm;
	tsk->mm = mm;
	// mm_init_cid(mm);
```

### B34 — activate_mm后恢复IRQ (L666–L668)

[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap]
```c
	activate_mm(active_mm, mm);
	// if (IS_ENABLED(CONFIG_ARCH_WANT_IRQS_OFF_ACTIVATE_MM))
		local_irq_enable();
```

### B35 — old_mm存在时经mmput归还旧地址空间 (L675–L678)

[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap]
```c
		// setmax_mm_hiwater_rss(&tsk->signal->maxrss, old_mm);
		// mm_update_next_owner(old_mm);
		mmput(old_mm);
		return 0;
```

### B36 — old_mm空时mmdrop旧active_mm (L679–L681)

[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap]
```c
	}
	mmdrop(active_mm);
	return 0;
```

### B37 — set_mm_exe_file新旧file对象 (L476–L478)

[VERIFIED mykernel/sched/forkexec/fork.c::set_mm_exe_file]
```c
int set_mm_exe_file(mm_s *mm, file_s *new_exe_file)
{
	file_s *old_exe_file;
```

### B38 — 读旧exe_file (L485–L486)

[VERIFIED mykernel/sched/forkexec/fork.c::set_mm_exe_file]
```c
	// old_exe_file = rcu_dereference_raw(mm->exe_file);
	old_exe_file = mm->exe_file;
```

### B39 — 新exe_file非空时get_file (L493–L495)

[VERIFIED mykernel/sched/forkexec/fork.c::set_mm_exe_file]
```c
		// if (deny_write_access(new_exe_file))
		// 	return -EACCES;
		get_file(new_exe_file);
```

### B40 — 写mm.exe_file并对旧file调用fput (L497–L501)

[VERIFIED mykernel/sched/forkexec/fork.c::set_mm_exe_file]
```c
	// rcu_assign_pointer(mm->exe_file, new_exe_file);
	mm->exe_file = new_exe_file;
	if (old_exe_file) {
		// allow_write_access(old_exe_file);
		fput(old_exe_file);
```

### B41 — ELF所选处理器请求begin_new_exec (L798–L801)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	/* Flush all traces of the currently running executable */
	retval = begin_new_exec(bprm);
	if (retval)
		goto out_free_dentry;
```

### B42 — ELF局部interpreter文件对象 (L1035–L1037)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	if (interpreter) {
		elf_entry = load_elf_interp(interp_elf_ex, interpreter,
						load_bias, interp_elf_phdata, &arch_state);
```

### C01 — do_close_execat对非空file调用fput (L1066–L1069)

[VERIFIED mykernel/sched/forkexec/exec.c::do_close_execat]
```c
static void
do_close_execat(file_s *file) {
	if (file)
		fput(file);
```

### C02 — free_bprm中对非空mm的mmput (L1072–L1076)

[VERIFIED mykernel/sched/forkexec/exec.c::free_bprm]
```c
static void
free_bprm(linux_bprm_s *bprm) {
	if (bprm->mm) {
		// acct_arg_size(bprm, 0);
		mmput(bprm->mm);
```

### C03 — free_arg_pages在清理中被调用 (L1078–L1078)

[VERIFIED mykernel/sched/forkexec/exec.c::free_bprm]
```c
	free_arg_pages(bprm);
```

### C04 — 相同bprm.file先fput再调用do_close_execat (L1083–L1087)

[VERIFIED mykernel/sched/forkexec/exec.c::free_bprm]
```c
	if (bprm->file) {
		// allow_write_access(bprm->file);
		fput(bprm->file);
	}
	do_close_execat(bprm->file);
```

### C05 — executable非空时额外fput (L1088–L1089)

[VERIFIED mykernel/sched/forkexec/exec.c::free_bprm]
```c
	if (bprm->executable)
		fput(bprm->executable);
```

### C06 — interp字符串别名比较及fdpath和bprm释放 (L1090–L1094)

[VERIFIED mykernel/sched/forkexec/exec.c::free_bprm]
```c
	/* If a binfmt changed the interp, free it. */
	if (bprm->interp != bprm->filename)
		kfree(bprm->interp);
	kfree(bprm->fdpath);
	kfree(bprm);
```

### C07 — free_arg_pages当前源体无活动释放语句 (L95–L97)

[VERIFIED mykernel/sched/forkexec/exec.c::free_arg_pages]
```c
static void
free_arg_pages(linux_bprm_s *bprm) {
}
```

### C08 — do_open_execat后检查是否为错误指针 (L1105–L1107)

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	file = do_open_execat(fd, filename, flags);
	if (IS_ERR(file))
		return ERR_CAST(file);
```

### C09 — bprm_mm_init调用与out_free标签，清理实际在下一行 (L1144–L1148)

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	retval = bprm_mm_init(bprm);
	if (retval == 0)
		return bprm;

out_free:
```

### C10 — kernel_execve调用bprm_execve随后free_bprm (L1428–L1430)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = bprm_execve(bprm, fd, filename, 0);
out_free:
	free_bprm(bprm);
```

### C11 — handler返回负值时走out (L1258–L1261)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
		retval = search_binary_handler(bprm);
	// }
	if (retval < 0)
		goto out;
```

### C12 — bprm_execve非负路径清理执行标记 (L1264–L1268)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	/* execve succeeded */
	curr->fs->in_exec = 0;
	curr->in_execve = 0;
	// rseq_execve(current);
	// user_events_execve(current);
```

### C13 — 失败out清标志并返回 (L1283–L1287)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	// sched_mm_cid_after_execve(current);
	current->fs->in_exec = 0;
	current->in_execve = 0;

	return retval;
```

### C14 — 格式链表的循环入口 (L1195–L1195)

[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
	list_header_for_each_container(fmt, &formats, lh) {
```

### C15 — 回调调用并显示原提前退出门注释 (L1200–L1204)

[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
		retval = fmt->load_binary(bprm);

		// read_lock(&binfmt_lock);
		// put_binfmt(fmt);
		// if (bprm->point_of_no_return || (retval != -ENOEXEC)) {
```

### C16 — PT_INTERP分支只对相应program header进入 (L669–L670)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		if (elf_ppnt->p_type != PT_INTERP)
			continue;
```

### C17 — interpreter名称分配错误门 (L680–L683)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		retval = -ENOMEM;
		elf_interpreter = kmalloc(elf_ppnt->p_filesz, GFP_KERNEL);
		if (!elf_interpreter)
			goto out_free_ph;
```

### C18 — 读取interpreter路径的失败出口 (L685–L688)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		retval = elf_read(bprm->file, elf_interpreter, elf_ppnt->p_filesz,
					elf_ppnt->p_offset);
		if (retval < 0)
			goto out_free_interp;
```

### C19 — open_exec解释器file及释放名称 (L694–L698)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		interpreter = open_exec(elf_interpreter);
		kfree(elf_interpreter);
		retval = PTR_ERR(interpreter);
		if (IS_ERR(interpreter))
			goto out_free_ph;
```

### C20 — 解释器ELF头分配失败goto out_free_file (L706–L709)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		interp_elf_ex = kmalloc(sizeof(*interp_elf_ex), GFP_KERNEL);
		if (!interp_elf_ex) {
			retval = -ENOMEM;
			goto out_free_file;
```

### C21 — 解释器ELF头读取失败goto out_free_dentry (L713–L716)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		retval = elf_read(interpreter, interp_elf_ex,
				  sizeof(*interp_elf_ex), 0);
		if (retval < 0)
			goto out_free_dentry;
```

### C22 — 解释器存在时加载入口 (L1035–L1037)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	if (interpreter) {
		elf_entry = load_elf_interp(interp_elf_ex, interpreter,
						load_bias, interp_elf_phdata, &arch_state);
```

### C23 — 解释器成功局部路径提前fput (L1053–L1056)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		fput(interpreter);

		kfree(interp_elf_ex);
		kfree(interp_elf_phdata);
```

### C24 — 当前ELF成功后续调用set_binfmt (L1065–L1067)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	kfree(elf_phdata);

	set_binfmt(&elf_format);
```

### C25 — 成功后续栈表失败可goto out (L1075–L1078)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	retval = create_elf_tables(bprm, elf_ex,
				interp_load_addr, e_entry, phdr_addr);
	if (retval < 0)
		goto out;
```

### C26 — ELF返回零之前START_THREAD (L1130–L1134)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	finalize_exec(bprm);
	START_THREAD(elf_ex, regs, elf_entry, bprm->p);
	retval = 0;
out:
	return retval;
```

### C27 — 失败label中解释器fput与elf_phdata释放，取1140-1144 (L1140–L1144)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
out_free_file:
	if (interpreter)
		fput(interpreter);
out_free_ph:
	kfree(elf_phdata);
```

### C28 — fput通过file.f_count减一为零进入归还 (L108–L111)

[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
void fput(file_s *file)
{
	if (atomic_long_dec_and_test(&file->f_count)) {
		task_s *task = current;
```

### C29 — fput归零后提交kmem_cache_free请求 (L124–L128)

[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
		// if (llist_add(&file->f_u.fu_llist, &delayed_fput_list))
		// 	schedule_delayed_work(&delayed_fput_work, 1);

		kmem_cache_free(filp_cachep, file);
	}
```

### C30 — 新file的f_count初始化为一 (L55–L55)

[VERIFIED mykernel/fs/vfs/file_table.c::__alloc_file]
```c
	atomic_long_set(&f->f_count, 1);
```

### C31 — get_file明确递增f_count (L905–L908)

[VERIFIED mykernel/include/linux/fs/fs.h::get_file]
```c
	static inline file_s *get_file(file_s *f) {
		atomic_long_inc(&f->f_count);
		return f;
	}
```

### C32 — 非空new_exe_file的get_file，取493-495 (L493–L495)

[VERIFIED mykernel/sched/forkexec/fork.c::set_mm_exe_file]
```c
		// if (deny_write_access(new_exe_file))
		// 	return -EACCES;
		get_file(new_exe_file);
```

### C33 — exe_file写入并fput旧值，取497-501 (L497–L501)

[VERIFIED mykernel/sched/forkexec/fork.c::set_mm_exe_file]
```c
	// rcu_assign_pointer(mm->exe_file, new_exe_file);
	mm->exe_file = new_exe_file;
	if (old_exe_file) {
		// allow_write_access(old_exe_file);
		fput(old_exe_file);
```

### C34 — mmput的mm_users计数归零门直接调用__mmput (L459–L461)

[VERIFIED mykernel/sched/forkexec/fork.c::mmput]
```c

	if (atomic_dec_and_test(&mm->mm_users))
		__mmput(mm);
```

### C35 — kernel_execve的out_ret实际归还filename并返回retval (L1432–L1434)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
out_ret:
	putname(filename);
	return retval;
```

