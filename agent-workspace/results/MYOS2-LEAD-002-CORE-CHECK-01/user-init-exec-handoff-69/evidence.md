# CORE-USER-INIT-EXEC-HANDOFF-69 — 固定源码证据

仅以`time@a039d9803ade`直接读取的源码文字为证据。每条短引文为单一定义内部原样连续1–5物理行，非空且保留空白与制表符；未执行仓库代码、预处理、编译、链接、CPU、QEMU、实机。入口成功、返回数值与用户指令执行必须分层。

## A — 直接引文

### A01 — 声明的hwtss_cpu0起始NULL指针对象 (L6–L6)

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::hwtss_cpu0]
```c
x86_hw_tss_s *hwtss_cpu0 = NULL;
```

### A02 — 进入桥接函数并发出日志 (L9–L12)

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
void kjmp_to_doexecve()
{
	// while (1);
	pr_info("Now jump to initd\n");
```

### A03 — curr取得当前任务、预先定位帧并清PF_KTHREAD (L15–L17)

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
	task_s * curr = current;
	pt_regs_s *curr_ptregs = task_pt_regs(curr);
	curr->flags &= ~PF_KTHREAD;
```

### A04 — 固定首个初始化程序路径字符串 (L19–L19)

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
	const char *initd_name = "/boot/init";
```

### A05 — argv/envp均为本地独立常量指针数组 (L22–L25)

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
	const char *const argv[] =
			{ initd_name , "kernarg_test_1", "kernarg_test_2", NULL };
	const char *const envp[] =
			{ "kernenv_test_1", "kernenv_test_2", "kernenv_test_3", NULL };
```

### A06 — 修改任务mm字段并调用kernel_execve无结果消费 (L30–L33)

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
	// 因为这里是内核线程，要通过execve进用户态
	// 需要通过current->mm和active_mm的检查，所以设置下mm
	curr->mm = curr->active_mm;
	kernel_execve(initd_name, argv, envp);
```

### A07 — hwtss_cpu0保存当前CPU TSS结构指针而非写sp0 (L35–L35)

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
	hwtss_cpu0 = this_cpu_ptr(&(cpu_tss_rw.x86_tss));
```

### A08 — 汇编字符串请求mov内存操作数到rsp及无条件跳转标签 (L37–L39)

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
	asm volatile(	"movq	%0,		%%rsp		\n\t"
					// "jmp	sysexit_entp		\n\t"
					"jmp	syscall_return_via_sysret	\n\t"
```

### A09 — 内联asm空输出约束、m输入与空clobber表 (L40–L43)

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
				:
				:	"m"(curr_ptregs)
				:
				);
```

### A10 — 内核初始化前直接root disk处理 (L297–L299)

[VERIFIED mykernel/init/main.c::kernel_init]
```c
	// color_printk(GREEN, BLACK, "Enter task init.\n");
	myos_switch_to_root_disk();
	// color_printk(GREEN, BLACK, "VFS initiated.\n");
```

### A11 — 桥接调用后存在while真与源文本返回值 (L304–L308)

[VERIFIED mykernel/init/main.c::kernel_init]
```c
	kjmp_to_doexecve();

	while (true);
	
	return 1;
```

### A12 — task的mm与active_mm为两个指针成员 (L137–L138)

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		mm_s			*mm;
		mm_s			*active_mm;
```

### A13 — 取每CPU current_task的直接定义 (L33–L35)

[VERIFIED mykernel/arch/x86_64/include/asm/current.h::get_current]
```c
	static __always_inline task_s *get_current(void) {
		return *this_cpu_ptr(&pcpu_hot.current_task);
	}
```

### A14 — current实际为get_current宏 (L37–L37)

[VERIFIED mykernel/arch/x86_64/include/asm/current.h::current]
```c
	#define current get_current()
```

### A15 — 经task_stack_page及THREAD_SIZE求寄存器帧的宏 (L17–L21)

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_macro_arch.h::task_pt_regs]
```c
	#define task_pt_regs(task)	({									\
				loff_t __ptr = (loff_t)task_stack_page(task);		\
				__ptr += THREAD_SIZE - TOP_OF_KERNEL_STACK_PADDING;	\
				((pt_regs_s *)__ptr) - 1;							\
			})
```

### A16 — 当前任务寄存器帧的别名宏 (L385–L385)

[VERIFIED mykernel/include/linux/kernel/ptrace.h::current_pt_regs]
```c
	#	define current_pt_regs()	task_pt_regs(current)
```

### A17 — 内核任务标记宏的真实取值 (L112–L112)

[VERIFIED mykernel/sched/task/task_const.h::PF_KTHREAD]
```c
	#define PF_KTHREAD				0x00200000	/* I am a kernel thread */
```

### A18 — mm赋值与exec请求相邻且无检查 (L32–L33)

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
	curr->mm = curr->active_mm;
	kernel_execve(initd_name, argv, envp);
```

## B — 直接引文

### B01 — kernel_execve文件名argv和envp直接形参 (L1375–L1378)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
int kernel_execve(const char *kernel_filename,
		const char *const *argv, const char *const *envp)
{
	task_s * curr = current;
```

### B02 — PF_KTHREAD真则返回EINVAL错误值 (L1385–L1387)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	/* It is non-sense for kernel threads to call execve */
	if (WARN_ON_ONCE(current->flags & PF_KTHREAD))
		return -EINVAL;
```

### B03 — getname_kernel失败返回PTR_ERR (L1389–L1391)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	filename = getname_kernel(kernel_filename);
	if (IS_ERR(filename))
		return PTR_ERR(filename);
```

### B04 — alloc_bprm失败转out_ret (L1393–L1397)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	bprm = alloc_bprm(fd, filename, 0);
	if (IS_ERR(bprm)) {
		retval = PTR_ERR(bprm);
		goto out_ret;
	}
```

### B05 — argv计数零或负值失败出口 (L1399–L1403)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = count_strings_kernel(argv);
	if (retval == 0)
		retval = -EINVAL;
	if (retval < 0)
		goto out_free;
```

### B06 — argv通过计数后写argc (L1404–L1404)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	bprm->argc = retval;
```

### B07 — envp计数失败出口及envc记录 (L1406–L1409)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = count_strings_kernel(envp);
	if (retval < 0)
		goto out_free;
	bprm->envc = retval;
```

### B08 — bprm_stack_limits失败出口 (L1411–L1413)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = bprm_stack_limits(bprm);
	if (retval < 0)
		goto out_free;
```

### B09 — 复制文件名并设置bprm.exec (L1415–L1418)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = copy_string_kernel(bprm->filename, bprm);
	if (retval < 0)
		goto out_free;
	bprm->exec = bprm->p;
```

### B10 — 复制envp失败出口 (L1420–L1422)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = copy_strings_kernel(bprm->envc, envp, bprm);
	if (retval < 0)
		goto out_free;
```

### B11 — 复制argv失败出口 (L1424–L1426)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = copy_strings_kernel(bprm->argc, argv, bprm);
	if (retval < 0)
		goto out_free;
```

### B12 — 调用bprm_execve后统一free_bprm (L1428–L1430)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = bprm_execve(bprm, fd, filename, 0);
out_free:
	free_bprm(bprm);
```

### B13 — putname并返回最后retval (L1432–L1434)

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
out_ret:
	putname(filename);
	return retval;
```

### B14 — bprm_execve入口和retval (L1227–L1231)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
static int
bprm_execve(linux_bprm_s *bprm, int fd,
		filename_s *filename, int flags) {

	int retval;
```

### B15 — in_execve置一 (L1245–L1246)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	curr->in_execve = 1;
	// sched_mm_cid_before_execve(current);
```

### B16 — search_binary_handler返回负值则进out (L1258–L1261)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
		retval = search_binary_handler(bprm);
	// }
	if (retval < 0)
		goto out;
```

### B17 — 非负分支清fs.in_exec和in_execve (L1264–L1267)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	/* execve succeeded */
	curr->fs->in_exec = 0;
	curr->in_execve = 0;
	// rseq_execve(current);
```

### B18 — 非负分支返回retval (L1271–L1271)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	return retval;
```

### B19 — out失败出口与point_of_no_return注释 (L1273–L1275)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
out:
	// /*
	//  * If past the point of no return ensure the code never
```

### B20 — 失败出口清字段再返回retval (L1283–L1287)

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	// sched_mm_cid_after_execve(current);
	current->fs->in_exec = 0;
	current->in_execve = 0;

	return retval;
```

### B21 — search_binary_handler入口和current.mm局部取值 (L1176–L1180)

[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
static int
search_binary_handler(linux_bprm_s *bprm) {
	mm_s *currmm = current->mm;

	// bool need_retry = IS_ENABLED(CONFIG_MODULES);
```

### B22 — prepare_binprm负值提前返回 (L1184–L1186)

[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
	retval = prepare_binprm(bprm);
	if (retval < 0)
		return retval;
```

### B23 — retval初值ENOENT与formats遍历 (L1192–L1196)

[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
	retval = -ENOENT;
//  retry:
	// read_lock(&binfmt_lock);
	list_header_for_each_container(fmt, &formats, lh) {
		// if (!try_module_get(fmt->module))
```

### B24 — 每个fmt的load_binary直接调用 (L1200–L1200)

[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
		retval = fmt->load_binary(bprm);
```

### B25 — 提前返回分支为注释 (L1204–L1207)

[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
		// if (bprm->point_of_no_return || (retval != -ENOEXEC)) {
		// 	read_unlock(&binfmt_lock);
		// 	return retval;
		// }
```

### B26 — 循环后模块重试门被注释 (L1209–L1211)

[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
	// read_unlock(&binfmt_lock);

	// if (need_retry) {
```

### B27 — 最终返回retval (L1221–L1222)

[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
	return retval;
}
```

### B28 — 读取文件头且直接返回kernel_read结果 (L1161–L1165)

[VERIFIED mykernel/sched/forkexec/exec.c::prepare_binprm]
```c
prepare_binprm(linux_bprm_s *bprm) {
	loff_t pos = 0;

	memset(bprm->buf, 0, BINPRM_BUF_SIZE);
	return kernel_read(bprm->file, bprm->buf, BINPRM_BUF_SIZE, &pos);
```

### B29 — do_open_execat失败返回ERR_CAST (L1103–L1107)

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	int retval = -ENOMEM;

	file = do_open_execat(fd, filename, flags);
	if (IS_ERR(file))
		return ERR_CAST(file);
```

### B30 — bprm分配失败关闭file返回ENOMEM (L1109–L1113)

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	bprm = kzalloc(sizeof(*bprm), GFP_KERNEL);
	if (bprm == NULL) {
		do_close_execat(file);
		return ERR_PTR(-ENOMEM);
	}
```

### B31 — bprm绑定file和文件名 (L1115–L1119)

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	bprm->file = file;

	if (fd == AT_FDCWD || filename->name[0] == '/') {
		bprm->filename = filename->name;
	} else {
```

### B32 — bprm_mm_init成功返回bprm (L1142–L1146)

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	bprm->interp = bprm->filename;

	retval = bprm_mm_init(bprm);
	if (retval == 0)
		return bprm;
```

### B33 — alloc_bprm错误清理再返回ERR_PTR (L1148–L1150)

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
out_free:
	free_bprm(bprm);
	return ERR_PTR(retval);
```

### B34 — free_bprm对非空mm执行mmput (L1073–L1077)

[VERIFIED mykernel/sched/forkexec/exec.c::free_bprm]
```c
free_bprm(linux_bprm_s *bprm) {
	if (bprm->mm) {
		// acct_arg_size(bprm, 0);
		mmput(bprm->mm);
	}
```

### B35 — file非空时存在释放及do_close_execat调用 (L1083–L1087)

[VERIFIED mykernel/sched/forkexec/exec.c::free_bprm]
```c
	if (bprm->file) {
		// allow_write_access(bprm->file);
		fput(bprm->file);
	}
	do_close_execat(bprm->file);
```

### B36 — 释放不同interp名称与bprm对象 (L1091–L1095)

[VERIFIED mykernel/sched/forkexec/exec.c::free_bprm]
```c
	if (bprm->interp != bprm->filename)
		kfree(bprm->interp);
	kfree(bprm->fdpath);
	kfree(bprm);
}
```

### B37 — copy_strings_kernel按计数循环取字符串 (L436–L440)

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings_kernel]
```c
static int
copy_strings_kernel(int argc, const char *const *argv,
		linux_bprm_s *bprm) {

	while (argc-- > 0) {
```

### B38 — 复制字符串失败立即返回 (L441–L443)

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings_kernel]
```c
		int ret = copy_string_kernel(argv[argc], bprm);
		if (ret < 0)
			return ret;
```

### B39 — register_binfmt把格式节点加入formats (L44–L48)

[VERIFIED mykernel/sched/forkexec/exec.c::register_binfmt]
```c
void register_binfmt(linux_bfmt_s * fmt)
{
	// write_lock(&binfmt_lock);
	list_header_add_to_tail(&formats, &fmt->lh);
	// write_unlock(&binfmt_lock);
```

## C — 直接引文

### C01 — ELF格式处理器入口和解释器指针 (L612–L615)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
static int
load_elf_binary(linux_bprm_s *bprm) {
	file_s		*interpreter = NULL; /* to shut gcc up */
	ulong		load_addr,
```

### C02 — mm和regs局部声明及默认ENOEXEC值 (L638–L642)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	mm_s		*mm;
	pt_regs_s	*regs;
	arch_elf_state_s	arch_state = INIT_ARCH_ELF_STATE;

	retval = -ENOEXEC;
```

### C03 — ELF魔数和文件类型检查，架构门另见相邻定义 (L643–L647)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	/* First of all, some simple consistency checks */
	if (memcmp(elf_ex->e_ident, ELFMAG, SELFMAG) != 0)
		goto out;

	if (elf_ex->e_type != ET_EXEC && elf_ex->e_type != ET_DYN)
```

### C04 — 解释器存在时以load_elf_interp求入口 (L1035–L1038)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	if (interpreter) {
		elf_entry = load_elf_interp(interp_elf_ex, interpreter,
						load_bias, interp_elf_phdata, &arch_state);
		if (!IS_ERR_VALUE(elf_entry)) {
```

### C05 — 解释器入口加e_entry并检查BAD_ADDR (L1043–L1047)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
			interp_load_addr = elf_entry;
			elf_entry += interp_elf_ex->e_entry;
		}
		if (BAD_ADDR(elf_entry)) {
			retval = IS_ERR_VALUE(elf_entry) ?
```

### C06 — 无解释器的e_entry和地址检查前半部 (L1057–L1061)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	} else {
		elf_entry = e_entry;
		if (BAD_ADDR(elf_entry)) {
			retval = -EINVAL;
			goto out_free_dentry;
```

### C07 — 构造ELF初始用户栈错误门 (L1075–L1078)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	retval = create_elf_tables(bprm, elf_ex,
				interp_load_addr, e_entry, phdr_addr);
	if (retval < 0)
		goto out;
```

### C08 — MM中的最终start_stack取bprm.p (L1083–L1085)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	mm->start_data = start_data;
	mm->end_data = end_data;
	mm->start_stack = bprm->p;
```

### C09 — loader重新读取当前任务regs (L1115–L1115)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	regs = current_pt_regs();
```

### C10 — finalize_exec、START_THREAD和retval零返回 (L1130–L1134)

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	finalize_exec(bprm);
	START_THREAD(elf_ex, regs, elf_entry, bprm->p);
	retval = 0;
out:
	return retval;
```

### C11 — START_THREAD备用宏将入口与栈参数传入start_thread (L29–L30)

[VERIFIED mykernel/include/linux/kernel/elf.h::START_THREAD]
```c
	#	define START_THREAD(elf_ex, regs, elf_entry, start_stack) \
					start_thread(regs, elf_entry, start_stack)
```

### C12 — start_thread_common函数与current_pt_regs一致性告警 (L251–L254)

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::start_thread_common]
```c
static void
start_thread_common(pt_regs_s *regs, ulong new_ip,
		ulong new_sp, u16 _cs, u16 _ss, u16 _ds) {
	WARN_ON_ONCE(regs != current_pt_regs());
```

### C13 — FS/ES/DS和GS段装载源码请求 (L264–L267)

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::start_thread_common]
```c
	loadsegment(fs, 0);
	loadsegment(es, _ds);
	loadsegment(ds, _ds);
	load_gs_index(0);
```

### C14 — 初始化regs.ip及regs.sp (L269–L270)

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::start_thread_common]
```c
	regs->ip		= new_ip;
	regs->sp		= new_sp;
```

### C15 — 初始化regs.cs及regs.ss (L271–L272)

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::start_thread_common]
```c
	regs->cs		= _cs;
	regs->ss		= _ss;
```

### C16 — 将flags设置IF或FIXED源组合 (L298–L299)

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::start_thread_common]
```c
	regs->flags		= X86_EFLAGS_IF | X86_EFLAGS_FIXED;
}
```

### C17 — 真实start_thread从用户段常量调用common (L301–L305)

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::start_thread]
```c
void
start_thread(pt_regs_s *regs, ulong new_ip, ulong new_sp)
{
	start_thread_common(regs, new_ip, new_sp, __USER_CS, __USER_DS, 0);
}
```

### C18 — pt_regs含r11和若干保存通用槽 (L20–L24)

[VERIFIED mykernel/arch/x86_64/include/asm/ptrace.h::pt_regs_s]
```c
		/* These regs are callee-clobbered. Always saved on kernel entry. */
			reg_t	r11;
			reg_t	r10;
			reg_t	r9;
			reg_t	r8;
```

### C19 — pt_regs含ax、cx与di等通用槽 (L25–L29)

[VERIFIED mykernel/arch/x86_64/include/asm/ptrace.h::pt_regs_s]
```c
			reg_t	ax;
			reg_t	cx;
			reg_t	dx;
			reg_t	si;
			reg_t	di;
```

### C20 — irq_nr为独立保存槽 (L31–L32)

[VERIFIED mykernel/arch/x86_64/include/asm/ptrace.h::pt_regs_s]
```c
		// myos push irq vec number separately
			reg_t	irq_nr;
```

### C21 — orig_ax的独立来源说明与字段 (L33–L37)

[VERIFIED mykernel/arch/x86_64/include/asm/ptrace.h::pt_regs_s]
```c
		/*
		 * On syscall entry, this is syscall#. On CPU exception, this is error code.
		 * On hw interrupt, it's IRQ number:
		 */
			reg_t	orig_ax;
```

### C22 — 源代码中IP/CS/flags/SP/SS相邻尾帧 (L39–L43)

[VERIFIED mykernel/arch/x86_64/include/asm/ptrace.h::pt_regs_s]
```c
			reg_t	ip;
			reg_t	cs;
			reg_t	flags;
			reg_t	sp;
			reg_t	ss;
```

### C23 — 静态生成头中R11偏移为48 (L34–L34)

[VERIFIED mykernel/include/generated/asm-offsets.h::R11]
```c
	#define R11 48
```

### C24 — 静态生成头中RCX偏移为88 (L39–L39)

[VERIFIED mykernel/include/generated/asm-offsets.h::RCX]
```c
	#define RCX 88
```

### C25 — 独立IRQ_NR偏移宏 (L43–L43)

[VERIFIED mykernel/include/generated/asm-offsets.h::IRQ_NR]
```c
	#define IRQ_NR 120
```

### C26 — 独立RSP偏移宏 (L48–L48)

[VERIFIED mykernel/include/generated/asm-offsets.h::RSP]
```c
	#define RSP 160
```

### C27 — 现存PTREGS_SIZE为176 (L56–L56)

[VERIFIED mykernel/include/generated/asm-offsets.h::PTREGS_SIZE]
```c
	#define PTREGS_SIZE 176
```

### C28 — 汇编宏POP_REGS入口与前四项恢复 (L65–L69)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::POP_REGS]
```asm
.macro POP_REGS
	popq		%r15
	popq		%r14
	popq		%r13
	popq		%r12
```

### C29 — 汇编宏恢复bp、bx、r11、r10、r9 (L70–L74)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::POP_REGS]
```asm
	popq		%rbp
	popq		%rbx
	popq		%r11
	popq		%r10
	popq		%r9
```

### C30 — 汇编宏恢复r8、rax、rcx、rdx、rsi (L75–L79)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::POP_REGS]
```asm
	popq		%r8
	popq		%rax
	popq		%rcx
	popq		%rdx
	popq		%rsi
```

### C31 — 汇编宏最后弹rdi (L80–L81)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::POP_REGS]
```asm
	popq		%rdi
.endm
```

### C32 — 共享SYSRET标签执行cli (L184–L185)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::syscall_return_via_sysret]
```asm
SYM_CODE_START(syscall_return_via_sysret)
	cli
```

### C33 — 先交换live r11/rcx与flags/IP再与保存R11/RCX (L187–L190)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::syscall_return_via_sysret]
```asm
	xchgq		%r11,		EFLAGS(%rsp)
	xchgq		%rcx,		RIP(%rsp)
	xchgq		%r11,		R11(%rsp)
	xchgq		%rcx,		RCX(%rsp)
```

### C34 — 重复交换live r11/rcx与flags/IP (L191–L192)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::syscall_return_via_sysret]
```asm
	xchgq		%r11,		EFLAGS(%rsp)
	xchgq		%rcx,		RIP(%rsp)
```

### C35 — POP_REGS后加0x28并pop到RSP (L193–L195)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::syscall_return_via_sysret]
```asm
	POP_REGS
	addq		$0x28,		%rsp
	popq		%rsp
```

### C36 — swapgs/sti/sysretq原样指令 (L196–L199)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::syscall_return_via_sysret]
```asm
	swapgs

	sti
	sysretq
```

### C37 — 普通分支用户数据段选择子的宏 (L160–L160)

[VERIFIED mykernel/arch/x86_64/processor/desc_const_arch.h::__USER_DS]
```c
	#  define __USER_DS				(GDT_ENTRY_DEFAULT_USER_DS * 8 + 3)
```

### C38 — 普通分支用户代码段选择子的宏 (L162–L162)

[VERIFIED mykernel/arch/x86_64/processor/desc_const_arch.h::__USER_CS]
```c
	#  define __USER_CS				(GDT_ENTRY_DEFAULT_USER_CS * 8 + 3)
```

### C39 — EFLAGS固定bit的真实宏 (L38–L39)

[VERIFIED mykernel/arch/x86_64/processor/processor_const_arch.h::X86_EFLAGS_FIXED]
```c
	#define X86_EFLAGS_FIXED_BIT	1 /* Bit 1 - always on */
	#define X86_EFLAGS_FIXED		_BITUL(X86_EFLAGS_FIXED_BIT)
```

### C40 — EFLAGS中断bit的真实宏 (L50–L51)

[VERIFIED mykernel/arch/x86_64/processor/processor_const_arch.h::X86_EFLAGS_IF]
```c
	#define X86_EFLAGS_IF_BIT		9 /* Interrupt Flag */
	#define X86_EFLAGS_IF			_BITUL(X86_EFLAGS_IF_BIT)
```

### C41 — current_pt_regs转task_pt_regs(current) (L385–L385)

[VERIFIED mykernel/include/linux/kernel/ptrace.h::current_pt_regs]
```c
	#	define current_pt_regs()	task_pt_regs(current)
```

### C42 — syscall_exit_to_user_mode实际调用用户退出工作 (L69–L73)

[VERIFIED mykernel/entry/common.c::syscall_exit_to_user_mode]
```c
__visible noinstr void
syscall_exit_to_user_mode(pt_regs_s *regs) {
	// instrumentation_begin();
	__syscall_exit_to_user_mode_work(regs);
	// instrumentation_end();
```

### C43 — 正常首帧C出口有用户工作调用 (L76–L80)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::ret_from_fork]
```c
		regs->ax = 0;
	}

	syscall_exit_to_user_mode(regs);
}
```

### C44 — 独立ORIG_RAX偏移宏 (L44–L44)

[VERIFIED mykernel/include/generated/asm-offsets.h::ORIG_RAX]
```c
	#define ORIG_RAX 128
```

### C45 — 独立RIP偏移宏 (L45–L45)

[VERIFIED mykernel/include/generated/asm-offsets.h::RIP]
```c
	#define RIP 136
```

### C46 — 独立CS偏移宏 (L46–L46)

[VERIFIED mykernel/include/generated/asm-offsets.h::CS]
```c
	#define CS 144
```

### C47 — 独立EFLAGS偏移宏 (L47–L47)

[VERIFIED mykernel/include/generated/asm-offsets.h::EFLAGS]
```c
	#define EFLAGS 152
```

### C48 — 独立SS偏移宏 (L49–L49)

[VERIFIED mykernel/include/generated/asm-offsets.h::SS]
```c
	#define SS 168
```

