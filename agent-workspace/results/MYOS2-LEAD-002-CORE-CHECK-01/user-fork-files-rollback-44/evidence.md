---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FORK-FILES-ROLLBACK-44
record_type: bounded_fixed_source_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
read_channel: connector
source_pin_short12: a039d9803ade
date: 2026-11-05
runtime_evidence: NOT_RUN
---

# 第44批固定time源码定义体及活动错误边

本证据仅取固定time@a039d9803ade中copy_process/copy_files、少量直接callee与errno编码定义。每项引文为同一函数、结构或单独宏定义内1至5行，不把extern、EXPORT、外部注释、条件编译块直接当定义。每处的行号在本轮固定源码实际读到；两轮连接器全文读取同一文件并按行比较。相邻/重叠引文用于控制流贯穿，不等于相同数量的独立根因。

### FR44-A001 — 任务复制函数声明（273–276行）

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
static task_s *dup_task_struct(task_s *orig, int node)
{
	task_s *tsk;
	int err;
```

### FR44-A002 — node条件分配和NULL门（278–282行）

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	if (node == NUMA_NO_NODE)
		// node = tsk_fork_get_node(orig);
	tsk = alloc_task_struct_node(node);
	if (!tsk)
		return NULL;
```

### FR44-A003 — 整结构按值预置（288–289行）

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
		*tsk = *orig;
		err = 0;
```

### FR44-A004 — 最终任务释放包装层（855–857行）

[VERIFIED mykernel/sched/forkexec/fork.c::delayed_free_task]
```c
static __always_inline void delayed_free_task(task_s *tsk) {
	free_task(tsk);
}
```

### FR44-A005 — copy_process签名和返回指针类型（867–871行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
static __latent_entropy task_s
*copy_process(pid_s *pid_struct, int trace, int node, kclone_args_s *args)
{
	int pidfd = -1, retval;
	task_s *p, *curr = current;
```

### FR44-A006 — clone标志和namespace来源（873–874行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	u64 clone_flags = args->flags;
	nsproxy_s *nsp = current->nsproxy;
```

### FR44-A007 — 任务分配前错误初值（958–961行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = -ENOMEM;
	p = dup_task_struct(current, node);
	if (!p)
		goto fork_out;
```

### FR44-A008 — copy_files之前的kthread失败门（1060–1062行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if (p->flags & PF_KTHREAD) {
		if (!set_kthread_struct(p))
			goto bad_fork_cleanup_delayacct;
```

### FR44-A009 — sched_fork失败标签（1103–1106行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	/* Perform scheduler related setup. Assign this task to a CPU. */
	retval = sched_fork(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_policy;
```

### FR44-A010 — copy_files结果和错误goto（1122–1124行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_files(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_semundo;
```

### FR44-A011 — copy_fs失败标签（1125–1127行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_fs(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_files;
```

### FR44-A012 — copy_sighand失败标签（1128–1130行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_sighand(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_fs;
```

### FR44-A013 — copy_signal失败标签（1131–1133行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_signal(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_sighand;
```

### FR44-A014 — copy_mm失败标签（1134–1136行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_mm(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_signal;
```

### FR44-A015 — copy_namespaces失败标签（1137–1139行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_namespaces(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_mm;
```

### FR44-A016 — copy_thread失败标签（1143–1145行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_thread(p, args);
	if (retval)
		goto bad_fork_cleanup_io;
```

### FR44-A017 — alloc_pid条件与错误指针检查（1149–1152行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if (pid_struct != &init_struct_pid) {
		pid_struct = alloc_pid(p->nsproxy->pid_ns_for_children,args->set_tid, args->set_tid_size);
		if (IS_ERR(pid_struct)) {
			retval = PTR_ERR(pid_struct);
```

### FR44-A018 — PTR_ERR生产与cleanup_thread goto（1152–1154行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
			retval = PTR_ERR(pid_struct);
			goto bad_fork_cleanup_thread;
		}
```

### FR44-A019 — 成功return p阻断失败标签落入（1372–1372行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	return p;
```

### FR44-A020 — bad_fork_cancel_cgroup结构入口（1375–1377行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cancel_cgroup:
	// sched_core_free(p);
	spin_unlock(&current->sighand->siglock);
```

### FR44-A021 — bad_fork_put_pidfd的活动体（1380–1384行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_put_pidfd:
	if (clone_flags & CLONE_PIDFD) {
		fput(pidfile);
		put_unused_fd(pidfd);
	}
```

### FR44-A022 — bad_fork_free_pid标签与操作（1385–1387行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_free_pid:
	if (pid_struct != &init_struct_pid)
		free_pid(pid_struct);
```

### FR44-A023 — cleanup_thread中的exit_thread（1388–1389行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cleanup_thread:
	exit_thread(p);
```

### FR44-A024 — cleanup_io与namespaces的注释部分（1390–1394行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cleanup_io:
	// if (p->io_context)
	// 	exit_io_context(p);
bad_fork_cleanup_namespaces:
	// exit_task_namespaces(p);
```

### FR44-A025 — cleanup_mm中的条件mmput（1395–1399行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cleanup_mm:
	if (p->mm) {
		// mm_clear_owner(p->mm, p);
		mmput(p->mm);
	}
```

### FR44-A026 — cleanup_signal与sighand清理（1400–1404行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cleanup_signal:
	// if (!(clone_flags & CLONE_THREAD))
	// 	free_signal_struct(p->signal);
bad_fork_cleanup_sighand:
	__cleanup_sighand(p->sighand);
```

### FR44-A027 — cleanup_fs和cleanup_files真实顺序（1405–1408行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cleanup_fs:
	exit_fs(p); /* blocking */
bad_fork_cleanup_files:
	exit_files(p); /* blocking */
```

### FR44-A028 — cleanup_semundo和security/audit（1409–1413行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cleanup_semundo:
	// exit_sem(p);
bad_fork_cleanup_security:
	// security_task_free(p);
bad_fork_cleanup_audit:
```

### FR44-A029 — 后部policy和delayacct标签（1415–1419行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cleanup_perf:
	// perf_event_free_task(p);
bad_fork_cleanup_policy:
	// lockdep_free_task(p);
bad_fork_cleanup_delayacct:
```

### FR44-A030 — 任务自由标签和状态赋值（1421–1425行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cleanup_count:
	// dec_rlimit_ucounts(task_ucounts(p), UCOUNT_RLIMIT_NPROC, 1);
	// exit_creds(p);
bad_fork_free:
	WRITE_ONCE(p->__state, TASK_DEAD);
```

### FR44-A031 — 栈归还与delayed_free_task调用（1426–1427行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	put_task_stack(p);
	delayed_free_task(p);
```

### FR44-A032 — fork_out最终ERR_PTR（1428–1432行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
fork_out:
	spin_lock_irq(&current->sighand->siglock);
	// hlist_del_init(&delayed.node);
	spin_unlock_irq(&current->sighand->siglock);
	return ERR_PTR(retval);
```

### FR44-A033 — kernel_clone的返回类型（1444–1448行）

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
pid_t kernel_clone(kclone_args_s *args)
{
	u64 clone_flags = args->flags;
	completion_s vfork;
	pid_s *pid_struct;
```

### FR44-A034 — 向copy_process传NUMA_NO_NODE（1485–1485行）

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	p = copy_process(NULL, trace, NUMA_NO_NODE, args);
```

### FR44-A035 — 对错误指针的调用者转换（1488–1489行）

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	if (IS_ERR(p))
		return PTR_ERR(p);
```

### FR44-A036 — copy_files参数与error零初值（722–725行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_files]
```c
static int copy_files(unsigned long clone_flags, task_s *tsk)
{
	files_struct_s *oldf, *newf;
	int error = 0;
```

### FR44-A037 — 当前旧表NULL分流（730–732行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_files]
```c
	oldf = current->files;
	if (!oldf)
		goto out;
```

### FR44-A038 — 共享表仅增refcount（734–736行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_files]
```c
	if (clone_flags & CLONE_FILES) {
		atomic_inc(&oldf->refcount);
		goto out;
```

### FR44-A039 — dup_fd失败时无新表（739–741行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_files]
```c
	newf = dup_fd(oldf, NR_OPEN_MAX, &error);
	if (!newf)
		goto out;
```

### FR44-A040 — 独立文件表发布并return error（743–746行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_files]
```c
	tsk->files = newf;
	error = 0;
out:
	return error;
```

### FR44-A041 — 外层分配失败的errorp输出（35–38行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	*errorp = -ENOMEM;
	newf = kmem_cache_alloc(files_cachep, GFP_KERNEL);
	if (!newf)
		goto out;
```

### FR44-A042 — 新表计数和fd_count初值（40–41行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	atomic_set(&newf->refcount, 1);
	newf->fd_count = NR_OPEN_DEFAULT;
```

### FR44-A043 — 循环按max_fds（51–53行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	for (i = 0; i < max_fds; i++)
	{
		file_s *old_fp = old_fds[i];
```

### FR44-A044 — 内层new file复制和缺NULL门（54–58行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
		if (old_fp != NULL)
		{
			file_s *new_fp = kzalloc(sizeof(file_s), GFP_KERNEL);
			*new_fp = *old_fp;
			new_fds[i] = new_fp;
```

### FR44-A045 — 正常dup_fd返回newf（64–64行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	return newf;
```

### FR44-A046 — 文件表计数归零门（72–74行）

[VERIFIED mykernel/fs/vfs/file.c::put_files_struct]
```c
void put_files_struct(files_struct_s *files)
{
	if (atomic_dec_and_test(&files->refcount)) {
```

### FR44-A047 — 归零才遍历file槽和调用filp_close（83–87行）

[VERIFIED mykernel/fs/vfs/file.c::put_files_struct]
```c
		unsigned int i = 0;
		for(i = 0; i < files->fd_count; i++) {
			file_s *file = files->fd_array[i];
			if(file != NULL)
				filp_close(file);
```

### FR44-A048 — 缓存free请求与结束（94–95行）

[VERIFIED mykernel/fs/vfs/file.c::put_files_struct]
```c
		kmem_cache_free(files_cachep, files);
	}
```

### FR44-A049 — 保存表指针和非NULL门（98–102行）

[VERIFIED mykernel/fs/vfs/file.c::exit_files]
```c
void exit_files(task_s *tsk)
{
	files_struct_s * files = tsk->files;

	if (files) {
```

### FR44-A050 — 先清任务指针再put旧表（103–107行）

[VERIFIED mykernel/fs/vfs/file.c::exit_files]
```c
		task_lock(tsk);
		tsk->files = NULL;
		task_unlock(tsk);
		put_files_struct(files);
	}
```

### FR44-A051 — 表容量与逻辑长度类型（67–69行）

[VERIFIED mykernel/include/linux/kernel/fdtable.h::files_struct_s]
```c
		unsigned long	fd_count;
		file_s __rcu	*fd_array[NR_OPEN_DEFAULT];
	} files_struct_s;
```

### FR44-A052 — 错误编码范围常量（15–15行）

[VERIFIED mykernel/include/linux/kernel/err.h::MAX_ERRNO]
```c
	#define MAX_ERRNO	4095
```

### FR44-A053 — 错误指针识别阈值（18–20行）

[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR_VALUE]
```c
	#	define IS_ERR_VALUE(x) (								\
					(size_t)(void *)(x) >= (size_t)-MAX_ERRNO	\
				)
```

### FR44-A054 — int错误码编码成指针（26–29行）

[VERIFIED mykernel/include/linux/kernel/err.h::ERR_PTR]
```c
		static inline void *
		ERR_PTR(long error) {
			return (void *) error;
		}
```

### FR44-A055 — 指针解码为long（31–34行）

[VERIFIED mykernel/include/linux/kernel/err.h::PTR_ERR]
```c
		static inline long
		PTR_ERR(const void *ptr) {
			return (long) ptr;
		}
```

### FR44-A056 — 调用者错误指针判断（36–39行）

[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR]
```c
		static inline bool
		IS_ERR(const void *ptr) {
			return IS_ERR_VALUE((size_t)ptr);
		}
```

### FR44-A057 — 原始init_task文件表静态绑定（119–119行）

[VERIFIED mykernel/init/init_task.c::init_task]
```c
	.files				= &init_files,
```

## 身份与可证支持范围

- copy_process整段已读，包括前序、成功return p和所有后继错误标签；FR44引文只各取1–5行。goto bad_fork_cleanup_semundo与goto bad_fork_cleanup_files的实际目标不同，semundo在files标签之后，因而copy_files正常失败直接跳过这次exit_files。共享/非共享的已取得引用前提须与43号copy_files定义合读。
- copy_fs、copy_sighand、copy_signal、copy_mm、copy_namespaces、copy_thread与alloc_pid只记录返回/门/goto；除所选源码return或PTR_ERR外，这些操作如何失败、实际资源/副作用未知，不推断具体errno或真实运行必达。
- 到达cleanup_sighand、cleanup_fs、cleanup_files之间各有实际直接调用；其能否正常返回和被传对象有效均为分析前提，尤其既有CORE-USER-VFS-BASELINE-05::UV-14可在更早清理处构成停止界限。
- bad_fork_cancel_cgroup、bad_fork_put_pidfd、bad_fork_free_pid有活动文本，但在本次读取的copy_process所选goto中未找到到这些标签的活动跳转；它们排在成功return p之后，不可按源码位置把其作为成功路径自动执行，也不能让alloc_pid错误跳过已明确的cleanup_thread目标。
- `ERR_PTR(retval)`是将long数值转为指针，`IS_ERR`仅按编码范围判断；不能让正数、越范围负数或本来未定义的retval自动成为有效errno/可识别错误指针。正常分配失败-ENOMEM的模型要求固定源的有效错误值和目标ABI/类型。
- `exit_files`只在传入p.files非NULL并能正常进行时置空，再put_files_struct；表refcount正初值n+1经本轮一次put在无其它写者条件下回n。对新表refcount=1的最后表清理只证遍历/filp_close/缓存free调用，未认证槽位实际有效、物理回收或全局文件生命周期。
- 本轮没有校验优化后的具体控制流或函数内真实运行；提及非法对象/NULL、不可达/未证的边界表示静态可证性止点，不代表CPU必然崩溃或返回某一errno。
- 历史UV-09/10/11/14、US-01、FT43-F01、CL42-F01继续保留，不重复建立同义问题。任何无本地活动goto的判断仅限已读函数/宏选择，不扩为全仓所有调用与配置的排除结论。

## 验证等级

所见固定源文件：fork.c、file.c、fdtable.h、err.h、init_task.c；另为金丝雀读options_flags.cmake/panic.c、为历史证据回读旧成果。直接定义引文两轮逐段原文与符号身份相符；未执行源码、仓库C/ASM/脚本、预处理、编译、QEMU、硬件、真实fork/exit、完整分配器或付费模型调用。文本复读≠本地P9、Graphviz或内核运行认证。
