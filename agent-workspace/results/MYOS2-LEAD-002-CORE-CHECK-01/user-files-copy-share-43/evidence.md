---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-FILES-COPY-SHARE-43
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
source_pin_short12: a039d9803ade
read_channel: connector
runtime_evidence: NOT_RUN
---
# 第43批固定源码引文（定义体49段）

下列49段均直接从固定time@a039d9803ade回源，逐项保留实际行号、文件和定义所属符号；每段最多5行。两轮连接器全文读取逐段文本一致。相邻或重叠片段不是独立事实计数。函数外注释、extern、EXPORT、#if/#endif不冒充定义体；对条件编译绑定未做预处理。引用真实性和语义解释分别审查。

### FT43-A001 — 初始任务全局对象（73–77行）

[VERIFIED mykernel/init/init_task.c::init_task]
```c
task_s init_task __aligned(L1_CACHE_BYTES) = {
	.thread_info		= INIT_THREAD_INFO(init_task),
	// .stack_refcount		= REFCOUNT_INIT(1),
	.__state			= TASK_RUNNING,
	.stack				= init_stack,
```

### FT43-A002 — 初始任务绑定文件表（116–120行）

[VERIFIED mykernel/init/init_task.c::init_task]
```c
	.comm				= INIT_TASK_COMM,
	.thread				= INIT_THREAD,
	.fs					= &init_fs,
	.files				= &init_files,
#ifdef CONFIG_IO_URING
```

### FT43-A003 — 静态文件表与引用初值（110–111行）

[VERIFIED mykernel/fs/vfs/file.c::init_files]
```c
files_struct_s init_files = {
	.refcount		= ATOMIC_INIT(1),
```

### FT43-A004 — 初始表逻辑fd数量（121–123行）

[VERIFIED mykernel/fs/vfs/file.c::init_files]
```c
	.file_lock		= __SPIN_LOCK_UNLOCKED(init_files.file_lock),
	.fd_count		= 0,
};
```

### FT43-A005 — 任务结构复制函数入口（273–277行）

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
static task_s *dup_task_struct(task_s *orig, int node)
{
	task_s *tsk;
	int err;

```

### FT43-A006 — 任务对象分配失败门（280–282行）

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	tsk = alloc_task_struct_node(node);
	if (!tsk)
		return NULL;
```

### FT43-A007 — 整结构按值复制（286–290行）

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	// int __weak arch_dup_task_struct(task_s *dst, task_s *src)
	// {
		*tsk = *orig;
		err = 0;
	// }
```

### FT43-A008 — copy_process生产任务对象（958–961行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = -ENOMEM;
	p = dup_task_struct(current, node);
	if (!p)
		goto fork_out;
```

### FT43-A009 — copy_process调用copy_files（1122–1124行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_files(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_semundo;
```

### FT43-A010 — copy_files签名及错误初值（722–725行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_files]
```c
static int copy_files(unsigned long clone_flags, task_s *tsk)
{
	files_struct_s *oldf, *newf;
	int error = 0;
```

### FT43-A011 — 旧表来源及NULL分支（730–732行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_files]
```c
	oldf = current->files;
	if (!oldf)
		goto out;
```

### FT43-A012 — 共享引用增量而不写tsk（734–737行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_files]
```c
	if (clone_flags & CLONE_FILES) {
		atomic_inc(&oldf->refcount);
		goto out;
	}
```

### FT43-A013 — 独立新表调用与失败分支（739–741行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_files]
```c
	newf = dup_fd(oldf, NR_OPEN_MAX, &error);
	if (!newf)
		goto out;
```

### FT43-A014 — 发布新表和返回（743–746行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_files]
```c
	tsk->files = newf;
	error = 0;
out:
	return error;
```

### FT43-A015 — 共享表标记宏（13–13行）

[VERIFIED mykernel/include/uapi/linux/sched.h::CLONE_FILES]
```c
	#define CLONE_FILES				0x00000400		/* set if open files shared between processes */
```

### FT43-A016 — 固定数组容量（22–22行）

[VERIFIED mykernel/include/linux/kernel/fdtable.h::NR_OPEN_DEFAULT]
```c
	#define NR_OPEN_DEFAULT	256
```

### FT43-A017 — 已读调用者容量参数（24–24行）

[VERIFIED mykernel/include/linux/kernel/fdtable.h::NR_OPEN_MAX]
```c
	#define NR_OPEN_MAX		NR_OPEN_DEFAULT
```

### FT43-A018 — 文件表与atomic_t计数（48–52行）

[VERIFIED mykernel/include/linux/kernel/fdtable.h::files_struct_s]
```c
	typedef struct files_struct {
		/*
		 * read mostly part
		 */
		atomic_t		refcount;
```

### FT43-A019 — 逻辑数量与物理内嵌数组（67–69行）

[VERIFIED mykernel/include/linux/kernel/fdtable.h::files_struct_s]
```c
		unsigned long	fd_count;
		file_s __rcu	*fd_array[NR_OPEN_DEFAULT];
	} files_struct_s;
```

### FT43-A020 — dup_fd形参与局部变量（29–33行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
files_struct_s *dup_fd(files_struct_s *oldf, unsigned int max_fds, int *errorp)
{
	files_struct_s *newf;
	file_s **old_fds, **new_fds;
	unsigned int open_files, i;
```

### FT43-A021 — 错误指针预设和外层分配失败（35–38行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	*errorp = -ENOMEM;
	newf = kmem_cache_alloc(files_cachep, GFP_KERNEL);
	if (!newf)
		goto out;
```

### FT43-A022 — 新表引用和逻辑数量初值（40–41行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	atomic_set(&newf->refcount, 1);
	newf->fd_count = NR_OPEN_DEFAULT;
```

### FT43-A023 — 新表锁初始化（43–43行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	spin_lock_init(&newf->file_lock);
```

### FT43-A024 — 新旧指针数组来源（48–49行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	old_fds = oldf->fd_array;
	new_fds = newf->fd_array;
```

### FT43-A025 — max_fds循环和旧槽读取（51–53行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	for (i = 0; i < max_fds; i++)
	{
		file_s *old_fp = old_fds[i];
```

### FT43-A026 — 非空槽逐个分配复制并发布（54–58行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
		if (old_fp != NULL)
		{
			file_s *new_fp = kzalloc(sizeof(file_s), GFP_KERNEL);
			*new_fp = *old_fp;
			new_fds[i] = new_fp;
```

### FT43-A027 — 内部kzalloc失败未设NULL门（56–58行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
			file_s *new_fp = kzalloc(sizeof(file_s), GFP_KERNEL);
			*new_fp = *old_fp;
			new_fds[i] = new_fp;
```

### FT43-A028 — 成功时返回新表（64–64行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	return newf;
```

### FT43-A029 — 释放标号与NULL返回（66–69行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
out_release:
	kmem_cache_free(files_cachep, newf);
out:
	return NULL;
```

### FT43-A030 — 表对象缓存全局变量定义（56–56行）

[VERIFIED mykernel/sched/forkexec/fork.c::files_cachep]
```c
kmem_cache_s *files_cachep;
```

### FT43-A031 — files_cachep创建且无构造器（1660–1664行）

[VERIFIED mykernel/sched/forkexec/fork.c::proc_caches_init]
```c
	files_cachep =
			kmem_cache_create("files_cache",
				sizeof(files_struct_s), 0,
				SLAB_HWCACHE_ALIGN|SLAB_PANIC|SLAB_ACCOUNT,
				NULL);
```

### FT43-A032 — 文件计数与位置（870–874行）

[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
		fmode_t				f_mode;
		atomic_long_t		f_count;
		// mutex_s				f_pos_lock;
		loff_t				f_pos;
		uint 				f_flags;
```

### FT43-A033 — 路径与操作表字段（878–880行）

[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
		path_s				f_path;
		inode_s				*f_inode;	/* cached value */
		const file_ops_s	*f_op;
```

### FT43-A034 — 映射指针和file_s定义（893–896行）

[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
	addr_spc_s				*f_mapping;
	// 	errseq_t		f_wb_err;
	// 	errseq_t		f_sb_err; /* for syncfs */
	}__attribute__((aligned(4))) file_s;	/* lest something weird decides that 2 is OK */
```

### FT43-A035 — 增加现有文件计数的接口（905–908行）

[VERIFIED mykernel/include/linux/fs/fs.h::get_file]
```c
	static inline file_s *get_file(file_s *f) {
		atomic_long_inc(&f->f_count);
		return f;
	}
```

### FT43-A036 — 文件表最后引用门（72–74行）

[VERIFIED mykernel/fs/vfs/file.c::put_files_struct]
```c
void put_files_struct(files_struct_s *files)
{
	if (atomic_dec_and_test(&files->refcount)) {
```

### FT43-A037 — 遍历和调用filp_close（83–87行）

[VERIFIED mykernel/fs/vfs/file.c::put_files_struct]
```c
		unsigned int i = 0;
		for(i = 0; i < files->fd_count; i++) {
			file_s *file = files->fd_array[i];
			if(file != NULL)
				filp_close(file);
```

### FT43-A038 — 文件表缓存free请求（94–96行）

[VERIFIED mykernel/fs/vfs/file.c::put_files_struct]
```c
		kmem_cache_free(files_cachep, files);
	}
}
```

### FT43-A039 — unshare_fd输出指针形参（1580–1584行）

[VERIFIED mykernel/sched/forkexec/fork.c::unshare_fd]
```c
int unshare_fd(unsigned long unshare_flags,
		unsigned int max_fds, files_struct_s **new_fdp)
{
	files_struct_s *fd = current->files;
	int error = 0;
```

### FT43-A040 — 实际复制门和失败出口（1586–1590行）

[VERIFIED mykernel/sched/forkexec/fork.c::unshare_fd]
```c
	if ((unshare_flags & CLONE_FILES) &&
		(fd && atomic_read(&fd->refcount) > 1)) {
		*new_fdp = dup_fd(fd, max_fds, &error);
		if (!*new_fdp)
			return error;
```

### FT43-A041 — 无需复制时返回0（1591–1594行）

[VERIFIED mykernel/sched/forkexec/fork.c::unshare_fd]
```c
	}

	return 0;
}
```

### FT43-A042 — 调用者初始化copy为NULL（1603–1607行）

[VERIFIED mykernel/sched/forkexec/fork.c::unshare_files]
```c
int unshare_files(void)
{
	task_s *task = current;
	files_struct_s *old, *copy = NULL;
	int error;
```

### FT43-A043 — helper返回和早退条件（1609–1612行）

[VERIFIED mykernel/sched/forkexec/fork.c::unshare_files]
```c
	error = unshare_fd(CLONE_FILES, NR_OPEN_MAX, &copy);
	if (error || !copy)
		return error;

```

### FT43-A044 — 保存旧表并发布新表（1613–1616行）

[VERIFIED mykernel/sched/forkexec/fork.c::unshare_files]
```c
	old = task->files;
	task_lock(task);
	task->files = copy;
	task_unlock(task);
```

### FT43-A045 — 归还旧表与返回0（1617–1619行）

[VERIFIED mykernel/sched/forkexec/fork.c::unshare_files]
```c
	put_files_struct(old);
	return 0;
}
```

### FT43-A046 — exec入口实际调用周边（886–889行）

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	/* Ensure the files table is not shared. */
	retval = unshare_files();
	if (retval)
		goto out;
```

### FT43-A047 — kzalloc选定包装体（168–172行）

[VERIFIED mykernel/mm/kmalloc/slub.h::kzalloc]
```c
		PREFIX_STATIC_INLINE
		__alloc_size(1) void
		*kzalloc(size_t size, gfp_t flags) {
			return kmalloc(size, flags | __GFP_ZERO);
		}
```

### FT43-A048 — uint独立别名定义（104–104行）

[VERIFIED mykernel/include/linux/kernel/types.h::uint]
```c
		typedef unsigned int		uint;
```

### FT43-A049 — ulong独立别名定义（105–105行）

[VERIFIED mykernel/include/linux/kernel/types.h::ulong]
```c
		typedef unsigned long		ulong;
```

## 摘录身份与支持强度

- FT43-A002/A003/A004证实静态init_task.files与init_files的绑定和初始计数；实际退出时最后一份引用是否发生仍为CL42-F01的未证部分。
- FT43-A007/A008/A009将dup_task_struct按值复制*tsk=*orig与copy_process实际传p给copy_files相连；共享分支没有本地tsk->files赋值，须先有有效orig=current对象及先行指针拷贝、无中途写者。atomic_inc本身不证明child已持有表。
- FT43-A012/A013/A014分别是共享表refcount增加与复制新表后发布，不得混同。复制branch的error=0是调用者显式赋值；旧表NULL时靠继承的tsk->files初值，不由copy_files自己写NULL。
- FT43-A025–A029的dup_fd循环用max_fds而非oldf.fd_count；宏NR_OPEN_MAX=256只是本批确认的两个调用点输入上界，不是dup_fd内部检查。NULL旧槽本轮不写新槽，不等于实际返回脏数据或全NULL。outer kmem_cache_alloc无本地清零标记，不能假定物理结果。
- FT43-A026/A027有重叠：kzalloc(sizeof(file_s))的返回没有NULL门就被解引用；这是新增条件性失败路径，不证明实际分配失败或硬件结果。out_release只有标号，没有当前活动跳转到该标号；不能将标号当作已执行清理。
- 按值复制*new_fp=*old_fp会得到独立file_s对象和独立标量f_count/f_pos值，指针成员f_path/f_op/f_mapping等按值拷贝、不会自动深复制指向的对象或为原file调用get_file。UV-09已有同义浅复制/缓存来源问题；本批只扩充来源与别名矩阵。
- FT43-A039–A045明确未复制时unshare_fd不写调用者输出槽；本次unshare_files将copy预设NULL，所以!copy门有效；不能推广到任意未初始化输出指针。发布新表与put旧表是不同事件，且要保留被检测oldfd与后来put的old身份一致、计数/对象仍有效和正常返回前提。
- FT43-A046是begin_new_exec中的实际单点调用，前序de_thread以及后继exec深层状态均保持opaque。
- FT43-A047包含kzalloc函数定义和__GFP_ZERO的包装调用，但外部条件预处理未执行，不能据此认证所有目标构建配置、深层分配器初值与实际成功。files_cachep创建使用NULL构造器不等于运行对象必然包含旧数据。
- R42-S01/S02及R41-S01/S02关于非NULL与有效对象、计数到零、源码副作用与物理回收、单符号引用、图示条件等限定继续适用。所有旧UV/US/CL42问题保持原状态，需完整调用链及新授权才能处理。

## 实际校验与未做事项

本轮49个证据标签在固定time源两轮连接器全文读取中逐字和符号身份确认49/49；重叠片段不另计新根因。没有执行仓库C/ASM/脚本、预处理/编译、QEMU/硬件、实际fork/exec/exit、锁/分配器或其他付费模型。Graphviz、完整本地P9与真实运行均NOT_RUN。
