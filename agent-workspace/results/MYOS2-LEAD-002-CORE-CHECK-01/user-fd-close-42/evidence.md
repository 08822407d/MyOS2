---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FD-CLOSE-42
record_type: fixed_time_source_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
date: 2026-11-05
source_pin_short12: a039d9803ade
read_channel: connector
runtime_evidence: NOT_RUN
---

# 第42批：固定定义体证据与引用身份

本批仅以time@a039d9803ade的源码为事实入口。下面每个CL42-S编号是一段1–5行的原样定义体引文。两轮GitHub连接器针对同一固定提交全文读取，按路径/行范围比较文本，同时核对所指符号身份及支持力。多个切片属于同一函数，不计作多项独立发现；正文注释只用于识别非活动分支，不能单独作为活动定义证据。

### CL42-S001 — close的无符号参数与当前任务对象（45–49行）

[VERIFIED mykernel/fs/syscall.c::close]
```c
MYOS_SYSCALL_DEFINE1(close, unsigned int, fd)
{
	task_s * curr = current;

	file_s * fp = NULL;
```

### CL42-S002 — 逻辑界限判定及取槽次序（51–54行）

[VERIFIED mykernel/fs/syscall.c::close]
```c
	if(fd < 0 || fd >= curr->files->fd_count)
		return -EBADF;

	fp = curr->files->fd_array[fd];
```

### CL42-S003 — kfree先于槽清空（58–59行）

[VERIFIED mykernel/fs/syscall.c::close]
```c
	kfree(fp);
	curr->files->fd_array[fd] = NULL;
```

### CL42-S004 — 局部正常返回0（70–71行）

[VERIFIED mykernel/fs/syscall.c::close]
```c
	return 0;
}
```

### CL42-S005 — 固定容量宏256（22–22行）

[VERIFIED mykernel/include/linux/kernel/fdtable.h::NR_OPEN_DEFAULT]
```c
	#define NR_OPEN_DEFAULT	256
```

### CL42-S006 — 文件表引用计数类型（52–52行）

[VERIFIED mykernel/include/linux/kernel/fdtable.h::files_struct_s]
```c
		atomic_t		refcount;
```

### CL42-S007 — unsigned long逻辑计数与固定数组（67–69行）

[VERIFIED mykernel/include/linux/kernel/fdtable.h::files_struct_s]
```c
		unsigned long	fd_count;
		file_s __rcu	*fd_array[NR_OPEN_DEFAULT];
	} files_struct_s;
```

### CL42-S008 — 所选kfree函数定义头（182–186行）

[VERIFIED mykernel/mm/kmalloc/slub.h::kfree]
```c
		void
		kfree(const void *object) {
			folio_s *folio;
			page_s *slab;
			kmem_cache_s *s;
```

### CL42-S009 — NULL早退门（188–189行）

[VERIFIED mykernel/mm/kmalloc/slub.h::kfree]
```c
			if (unlikely(object == NULL))
				return;
```

### CL42-S010 — 非NULL走页/缓存来源的分派（191–195行）

[VERIFIED mykernel/mm/kmalloc/slub.h::kfree]
```c
			folio = virt_to_folio(object);
			if (unlikely(!folio_test_slab(folio)))
				free_large_kmalloc(folio, (void *)object);
			else
				kmem_cache_free(folio_slab(folio)->slab_cache, (void *)object);
```

### CL42-S011 — filp_close返回int与retval初值（314–317行）

[VERIFIED mykernel/fs/vfs/open.c::filp_close]
```c
int filp_close(file_s *filp)
{
	int retval = 0;

```

### CL42-S012 — 调用fput并返回retval（330–332行）

[VERIFIED mykernel/fs/vfs/open.c::filp_close]
```c
	fput(filp);
	return retval;
}
```

### CL42-S013 — file计数减一及零门（108–111行）

[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
void fput(file_s *file)
{
	if (atomic_long_dec_and_test(&file->f_count)) {
		task_s *task = current;
```

### CL42-S014 — file缓存释放请求（127–129行）

[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
		kmem_cache_free(filp_cachep, file);
	}
}
```

### CL42-S015 — file对象从filp_cache分配（40–44行）

[VERIFIED mykernel/fs/vfs/file_table.c::__alloc_file]
```c
{
	file_s *f;
	int error;

	f = kmem_cache_zalloc(filp_cachep, GFP_KERNEL);
```

### CL42-S016 — 分配失败的编码错误返回（44–46行）

[VERIFIED mykernel/fs/vfs/file_table.c::__alloc_file]
```c
	f = kmem_cache_zalloc(filp_cachep, GFP_KERNEL);
	if (unlikely(!f))
		return ERR_PTR(-ENOMEM);
```

### CL42-S017 — 初始化file.f_count=1（55–55行）

[VERIFIED mykernel/fs/vfs/file_table.c::__alloc_file]
```c
	atomic_long_set(&f->f_count, 1);
```

### CL42-S018 — filp_cachep创建入口（132–135行）

[VERIFIED mykernel/fs/vfs/file_table.c::files_init]
```c
void __init files_init(void)
{
	filp_cachep = kmem_cache_create("filp", sizeof(file_s), 0,
			SLAB_HWCACHE_ALIGN | SLAB_PANIC | SLAB_ACCOUNT, NULL);
```

### CL42-S019 — files_struct引用计数零门（72–74行）

[VERIFIED mykernel/fs/vfs/file.c::put_files_struct]
```c
void put_files_struct(files_struct_s *files)
{
	if (atomic_dec_and_test(&files->refcount)) {
```

### CL42-S020 — unsigned int遍历索引（83–85行）

[VERIFIED mykernel/fs/vfs/file.c::put_files_struct]
```c
		unsigned int i = 0;
		for(i = 0; i < files->fd_count; i++) {
			file_s *file = files->fd_array[i];
```

### CL42-S021 — 非NULL调用filp_close（85–88行）

[VERIFIED mykernel/fs/vfs/file.c::put_files_struct]
```c
			file_s *file = files->fd_array[i];
			if(file != NULL)
				filp_close(file);
		}
```

### CL42-S022 — files表缓存释放请求（94–96行）

[VERIFIED mykernel/fs/vfs/file.c::put_files_struct]
```c
		kmem_cache_free(files_cachep, files);
	}
}
```

### CL42-S023 — 退出入口NULL门（98–102行）

[VERIFIED mykernel/fs/vfs/file.c::exit_files]
```c
void exit_files(task_s *tsk)
{
	files_struct_s * files = tsk->files;

	if (files) {
```

### CL42-S024 — 锁内摘除文件表后再put（103–107行）

[VERIFIED mykernel/fs/vfs/file.c::exit_files]
```c
		task_lock(tsk);
		tsk->files = NULL;
		task_unlock(tsk);
		put_files_struct(files);
	}
```

### CL42-S025 — 全局文件表refcount初值1（110–112行）

[VERIFIED mykernel/fs/vfs/file.c::init_files]
```c
files_struct_s init_files = {
	.refcount		= ATOMIC_INIT(1),
	// .resize_wait	= __WAIT_QUEUE_HEAD_INITIALIZER(init_files.resize_wait),
```

### CL42-S026 — 全局文件表fd_count初值0（121–123行）

[VERIFIED mykernel/fs/vfs/file.c::init_files]
```c
	.file_lock		= __SPIN_LOCK_UNLOCKED(init_files.file_lock),
	.fd_count		= 0,
};
```

### CL42-S027 — 新files_struct分配与NULL处理（35–38行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	*errorp = -ENOMEM;
	newf = kmem_cache_alloc(files_cachep, GFP_KERNEL);
	if (!newf)
		goto out;
```

### CL42-S028 — 新文件表refcount1和fd_count256（40–41行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	atomic_set(&newf->refcount, 1);
	newf->fd_count = NR_OPEN_DEFAULT;
```

### CL42-S029 — 复制槽时kzalloc与按值file副本（53–57行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
		file_s *old_fp = old_fds[i];
		if (old_fp != NULL)
		{
			file_s *new_fp = kzalloc(sizeof(file_s), GFP_KERNEL);
			*new_fp = *old_fp;
```

### CL42-S030 — 副本赋值和新槽发布（56–58行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
			file_s *new_fp = kzalloc(sizeof(file_s), GFP_KERNEL);
			*new_fp = *old_fp;
			new_fds[i] = new_fp;
```

### CL42-S031 — 旧别名来源get_file与安装（235–239行）

[VERIFIED mykernel/fs/vfs/file.c::f_dupfd]
```c
	err = alloc_fd(from, nofile, flags);
	if (err >= 0) {
		get_file(file);
		fd_install(err, file);
	}
```

### CL42-S032 — file对象独立计数及字段（870–874行）

[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
		fmode_t				f_mode;
		atomic_long_t		f_count;
		// mutex_s				f_pos_lock;
		loff_t				f_pos;
		uint 				f_flags;
```

### CL42-S033 — 额外file引用的计数增量（905–908行）

[VERIFIED mykernel/include/linux/fs/fs.h::get_file]
```c
	static inline file_s *get_file(file_s *f) {
		atomic_long_inc(&f->f_count);
		return f;
	}
```

### CL42-S034 — files_cachep全局变量定义（55–56行）

[VERIFIED mykernel/sched/forkexec/fork.c::files_cachep]
```c
/* SLAB cache for files_struct structures (tsk->files) */
kmem_cache_s *files_cachep;
```

### CL42-S035 — files_cachep创建入口（1660–1664行）

[VERIFIED mykernel/sched/forkexec/fork.c::proc_caches_init]
```c
	files_cachep =
			kmem_cache_create("files_cache",
				sizeof(files_struct_s), 0,
				SLAB_HWCACHE_ALIGN|SLAB_PANIC|SLAB_ACCOUNT,
				NULL);
```

### CL42-S036 — 历史非空files门（730–732行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_files]
```c
	oldf = current->files;
	if (!oldf)
		goto out;
```

### CL42-S037 — CLONE_FILES时共享表计数增量（734–737行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_files]
```c
	if (clone_flags & CLONE_FILES) {
		atomic_inc(&oldf->refcount);
		goto out;
	}
```

### CL42-S038 — 另造表并赋给任务（739–743行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_files]
```c
	newf = dup_fd(oldf, NR_OPEN_MAX, &error);
	if (!newf)
		goto out;

	tsk->files = newf;
```

### CL42-S039 — 直接exit_files调用点（126–129行）

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	// exit_sem(tsk);
	// exit_shm(tsk);
	exit_files(tsk);
	exit_fs(tsk);
```

### CL42-S040 — dup_fd签名与新旧对象局部（29–33行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
files_struct_s *dup_fd(files_struct_s *oldf, unsigned int max_fds, int *errorp)
{
	files_struct_s *newf;
	file_s **old_fds, **new_fds;
	unsigned int open_files, i;
```

## 定义、上下文、支持力三层区分

- close的完整活动体在mykernel/fs/syscall.c45–71，只有fd>=fd_count的早退、取槽、kfree、清NULL、return0。未调用filp_close/fput/f_count/操作表close回调。源码中对f_op->close和close_fd的语句被注释，非活动调用。
- slub.h的kfree定义位于头文件第41行的#if defined(SLUB_DEFINATION) || !(DEBUG)条件块；该#if是编译选择上下文，不是函数体定义，不计成直接引文，也未运行预处理证明实际目标绑定。本批只按读到的定义体条件解释kfree(NULL)第188–189行的早退。其他未知指针、错误编码、已经无效的对象均不得假称经过保护。
- kfree非NULL分支通过virt_to_folio与folio_test_slab选择后续释放调用；这仅是被读到的局部代码，既没有复核完整分配器也没有实证物理释放。
- filp_close函数活动体只初始化retval=0，调用fput，返回retval；flush、file_count、dnotify/locks等被注释，不能当执行。
- file_table.c fput对file.f_count调用atomic_long_dec_and_test，在零门内发起kmem_cache_free(filp_cachep,file)；从“调fput”到“减计数归零”、再到“调用缓存free”、最后“物理回收”是不同证据层级，后两者不自动认证。
- put_files_struct的filp_close调用在file!=NULL门内，返回值没有被消费；先读取文件槽，遍历本身不将槽清NULL。其最后kmem_cache_free(files_cachep,files)是对文件表对象的释放请求，与各file对象不同。
- fork.c仅定点读copy_files的共享表计数增量及files_cachep定义/初始化；dup_fd仅读file副本与计数初始化。之前UV-09/UV-10对实际fork、缓存错配的历史分析并未重做。
- fs.h中file_s的f_path、f_op、f_mapping是对象字段/指针，不意味着本批两个关闭路径会自动归还路径引用或调用回调。未审inode、VMA、页缓存、挂载或整条退出清理链。
- 每个[VERIFIED]仅对直接定义体的选定文本和被其支持的局部断言负责，不证明调用的实际可达性、运行结果或异常终态。P9全文字节/目标符号检查和运行检验须由主线/本地独立完成。

## 本批方法登记

实际引用标签总数：40（CL42-S001..S040），复读确认40，降级0。每段不超过5行；无extern/EXPORT独立充作定义体、无伪造40位SHA。定点读9个源码文件，额外核双金丝雀与旧结果；既有主线裁定和旧编号未因本读取改变。

不执行：编译宏展开、构建、仓库任何C/ASM/脚本、QEMU/真实硬件、文件系统I/O、实际并发、完整slab或fork/exit路径、完整P9。纯文本及有限模型检查不等于生产内核功能认证。
