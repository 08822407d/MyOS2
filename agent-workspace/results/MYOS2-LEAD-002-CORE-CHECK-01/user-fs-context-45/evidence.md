---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FS-CONTEXT-45
record_type: fixed_source_definition_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
date: 2026-11-06
source_pin_short12: a039d9803ade
read_channel: connector
runtime_evidence: NOT_RUN
---

# 第45批taskfs共享、复制、路径请求及归还的逐字证据

下列每段为固定time@a039d9803ade所列文件的1–5行单函数、真实结构体、单独typedef或单宏定义短片段，使用本轮实际读取所见行号。两轮GitHub连接器对同一固定源码全文独立回读，逐段原文、符号身份及支持强度核对。相邻/重叠片段只帮助解释实际顺序，并不形成新的独立发现。函数内注释仍属非活动上下文；extern/EXPORT、函数外注释及#if不作为本批直接定义体证据。未运行预处理。

### FS45-A001 — 静态任务绑定全局fs对象（118–118行）

[VERIFIED mykernel/init/init_task.c::init_task]
```c
	.fs					= &init_fs,
```

### FS45-A002 — 全局fs对象与users初值（135–137行）

[VERIFIED mykernel/sched/misc/fs_struct.c::init_fs]
```c
taskfs_s init_fs = {
	.users		= 1,
	.lock		= __SPIN_LOCK_UNLOCKED(init_fs.lock),
```

### FS45-A003 — 全局fs的umask初值（139–140行）

[VERIFIED mykernel/sched/misc/fs_struct.c::init_fs]
```c
	.umask		= 0022,
};
```

### FS45-A004 — taskfs_s的单独类型别名（22–23行）

[VERIFIED mykernel/sched/sched_type_declaration.h::taskfs_s]
```c
	struct fs_struct;
	typedef struct fs_struct taskfs_s;
```

### FS45-A005 — fs_struct定义中的users和lock（9–11行）

[VERIFIED mykernel/sched/misc/fs_struct_types.h::fs_struct]
```c
	struct fs_struct {
		int			users;
		spinlock_t	lock;
```

### FS45-A006 — fs_struct的umask、in_exec、内嵌root/pwd（13–16行）

[VERIFIED mykernel/sched/misc/fs_struct_types.h::fs_struct]
```c
		int			umask;
		int			in_exec;
		path_s		root, pwd;
	};
```

### FS45-A007 — CLONE_FS单宏（12–12行）

[VERIFIED mykernel/include/uapi/linux/sched.h::CLONE_FS]
```c
	#define CLONE_FS				0x00000200		/* set if fs info shared between processes */
```

### FS45-A008 — EAGAIN单宏（17–17行）

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::EAGAIN]
```c
	#define	EAGAIN		11	/* Try again */
```

### FS45-A009 — ENOMEM单宏（18–18行）

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOMEM]
```c
	#define	ENOMEM		12	/* Out of memory */
```

### FS45-A010 — node门和分配返回检查（278–282行）

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	if (node == NUMA_NO_NODE)
		// node = tsk_fork_get_node(orig);
	tsk = alloc_task_struct_node(node);
	if (!tsk)
		return NULL;
```

### FS45-A011 — 真实整task结构复制（286–290行）

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	// int __weak arch_dup_task_struct(task_s *dst, task_s *src)
	// {
		*tsk = *orig;
		err = 0;
	// }
```

### FS45-A012 — copy_process获取新task的前缀（958–961行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = -ENOMEM;
	p = dup_task_struct(current, node);
	if (!p)
		goto fork_out;
```

### FS45-A013 — copy_files在copy_fs之前（1122–1124行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_files(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_semundo;
```

### FS45-A014 — copy_fs返回错误时跳files标签（1125–1127行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_fs(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_files;
```

### FS45-A015 — copy_sighand错误跳fs标签（1128–1130行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_sighand(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_fs;
```

### FS45-A016 — sighand、fs清理贯穿位置（1403–1406行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cleanup_sighand:
	__cleanup_sighand(p->sighand);
bad_fork_cleanup_fs:
	exit_fs(p); /* blocking */
```

### FS45-A017 — fs之后才是files清理（1407–1409行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cleanup_files:
	exit_files(p); /* blocking */
bad_fork_cleanup_semundo:
```

### FS45-A018 — copy_fs签名、old fs与标志门（702–705行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_fs]
```c
static int copy_fs(unsigned long clone_flags, task_s *tsk)
{
	taskfs_s *fs = current->fs;
	if (clone_flags & CLONE_FS) {
```

### FS45-A019 — 共享路径在users增前检查in_exec（706–710行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_fs]
```c
		/* tsk->fs is already what we want */
		// spin_lock(&fs->lock);
		if (fs->in_exec) {
			// spin_unlock(&fs->lock);
			return -EAGAIN;
```

### FS45-A020 — 共享路径users递增并返回0（711–714行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_fs]
```c
		}
		fs->users++;
		// spin_unlock(&fs->lock);
		return 0;
```

### FS45-A021 — 非共享先发布new指针再判NULL（716–718行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_fs]
```c
	tsk->fs = copy_fs_struct(fs);
	if (!tsk->fs)
		return -ENOMEM;
```

### FS45-A022 — 正常返回0的尾部（719–720行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_fs]
```c
	return 0;
}
```

### FS45-A023 — copy_fs_struct的分配入口（111–113行）

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
taskfs_s *copy_fs_struct(taskfs_s *old)
{
	taskfs_s *fs = kmem_cache_alloc(fs_cachep, GFP_KERNEL);
```

### FS45-A024 — 有返回对象才初始化users和in_exec（115–117行）

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
	if (fs) {
		fs->users = 1;
		fs->in_exec = 0;
```

### FS45-A025 — 新锁初始化、umask在old锁前读取（119–121行）

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
		spin_lock_init(&fs->lock);
		// seqcount_spinlock_init(&fs->seq, &fs->lock);
		fs->umask = old->umask;
```

### FS45-A026 — 旧锁内拷贝root并请求path_get（123–125行）

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
		spin_lock(&old->lock);
		fs->root = old->root;
		path_get(&fs->root);
```

### FS45-A027 — 拷贝pwd、第二次path_get、解锁（126–128行）

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
		fs->pwd = old->pwd;
		path_get(&fs->pwd);
		spin_unlock(&old->lock);
```

### FS45-A028 — 条件块与返回fs（129–131行）

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
	}
	return fs;
}
```

### FS45-A029 — exit_fs入口先取得tsk.fs（92–94行）

[VERIFIED mykernel/sched/misc/fs_struct.c::exit_fs]
```c
void exit_fs(task_s *tsk)
{
	taskfs_s *fs = tsk->fs;
```

### FS45-A030 — 非NULL条件、task锁、fs锁（96–99行）

[VERIFIED mykernel/sched/misc/fs_struct.c::exit_fs]
```c
	if (fs) {
		int kill;
		task_lock(tsk);
		spin_lock(&fs->lock);
```

### FS45-A031 — 清tsk.fs及users前减一（101–102行）

[VERIFIED mykernel/sched/misc/fs_struct.c::exit_fs]
```c
		tsk->fs = NULL;
		kill = !--fs->users;
```

### FS45-A032 — 解锁与kill时free_fs_struct（104–107行）

[VERIFIED mykernel/sched/misc/fs_struct.c::exit_fs]
```c
		spin_unlock(&fs->lock);
		task_unlock(tsk);
		if (kill)
			free_fs_struct(fs);
```

### FS45-A033 — free_fs_struct按root、pwd、缓存次序（85–89行）

[VERIFIED mykernel/sched/misc/fs_struct.c::free_fs_struct]
```c
void free_fs_struct(taskfs_s *fs)
{
	path_put(&fs->root);
	path_put(&fs->pwd);
	kmem_cache_free(fs_cachep, fs);
```

### FS45-A034 — root的path_put请求（87–87行）

[VERIFIED mykernel/sched/misc/fs_struct.c::free_fs_struct]
```c
	path_put(&fs->root);
```

### FS45-A035 — pwd的path_put请求（88–88行）

[VERIFIED mykernel/sched/misc/fs_struct.c::free_fs_struct]
```c
	path_put(&fs->pwd);
```

### FS45-A036 — fs_cachep释放请求（89–89行）

[VERIFIED mykernel/sched/misc/fs_struct.c::free_fs_struct]
```c
	kmem_cache_free(fs_cachep, fs);
```

### FS45-A037 — path_s含两个内嵌指针字段（7–10行）

[VERIFIED mykernel/include/linux/fs/path.h::path_s]
```c
	typedef struct path {
		vfsmount_s	*mnt;
		dentry_s	*dentry;
	} path_s;
```

### FS45-A038 — path_get活动dget与注释mntget（266–270行）

[VERIFIED mykernel/fs/vfs/namei.c::path_get]
```c
void path_get(const path_s *path)
{
	// mntget(path->mnt);
	dget(path->dentry);
}
```

### FS45-A039 — path_put活动dput与注释mntput（278–282行）

[VERIFIED mykernel/fs/vfs/namei.c::path_put]
```c
void path_put(const path_s *path)
{
	dput(path->dentry);
	// mntput(path->mnt);
}
```

### FS45-A040 — path_get活动dget单句（269–269行）

[VERIFIED mykernel/fs/vfs/namei.c::path_get]
```c
	dget(path->dentry);
```

### FS45-A041 — path_put活动dput单句（280–280行）

[VERIFIED mykernel/fs/vfs/namei.c::path_put]
```c
	dput(path->dentry);
```

### FS45-A042 — 动态fs缓存的全局变量（58–59行）

[VERIFIED mykernel/sched/forkexec/fork.c::fs_cachep]
```c
/* SLAB cache for fs_struct structures (tsk->fs) */
kmem_cache_s *fs_cachep;
```

### FS45-A043 — fs_cachep创建参数及NULL构造器（1665–1669行）

[VERIFIED mykernel/sched/forkexec/fork.c::proc_caches_init]
```c
	fs_cachep =
			kmem_cache_create("fs_cache",
				sizeof(taskfs_s), 0,
				SLAB_HWCACHE_ALIGN|SLAB_PANIC|SLAB_ACCOUNT,
				NULL);
```

### FS45-A044 — kernel_clone实际NUMA_NO_NODE入口（1485–1485行）

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	p = copy_process(NULL, trace, NUMA_NO_NODE, args);
```

### FS45-A045 — users被初始化为1（116–116行）

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
		fs->users = 1;
```

### FS45-A046 — in_exec被初始化为0（117–117行）

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
		fs->in_exec = 0;
```

### FS45-A047 — umask复制语句在旧锁之前（121–121行）

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
		fs->umask = old->umask;
```

### FS45-A048 — root先拷贝再get（124–125行）

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
		fs->root = old->root;
		path_get(&fs->root);
```

### FS45-A049 — pwd后拷贝再get（126–127行）

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
		fs->pwd = old->pwd;
		path_get(&fs->pwd);
```

### FS45-A050 — users减后零值判定（102–102行）

[VERIFIED mykernel/sched/misc/fs_struct.c::exit_fs]
```c
		kill = !--fs->users;
```

### FS45-A051 — 共享分支EAGAIN早退（708–710行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_fs]
```c
		if (fs->in_exec) {
			// spin_unlock(&fs->lock);
			return -EAGAIN;
```

### FS45-A052 — 共享分支users直接++并return（712–714行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_fs]
```c
		fs->users++;
		// spin_unlock(&fs->lock);
		return 0;
```

### FS45-A053 — 非共享将返回值写tsk.fs（716–716行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_fs]
```c
	tsk->fs = copy_fs_struct(fs);
```

### FS45-A054 — 复制后NULL门返回ENOMEM（717–718行）

[VERIFIED mykernel/sched/forkexec/fork.c::copy_fs]
```c
	if (!tsk->fs)
		return -ENOMEM;
```

### FS45-A055 — 归还root与pwd后缓存free（87–89行）

[VERIFIED mykernel/sched/misc/fs_struct.c::free_fs_struct]
```c
	path_put(&fs->root);
	path_put(&fs->pwd);
	kmem_cache_free(fs_cachep, fs);
```

## 证据强度和未验证范围

- copy_fs完整活动体为fork.c702–720。共享分支读取fs->in_exec，非零返回-EAGAIN而不执行本地users++；为零则对普通int users直接++并返回0，未在本体写tsk->fs。非共享分支先将copy_fs_struct返回指针写tsk->fs，随后检查NULL返回-ENOMEM。
- copy_fs_struct完整活动体为fs_struct.c111–131：kmem_cache_alloc返回NULL时不进入if(fs)，直接return NULL；成功对象先初始化users/in_exec/lock，再于旧锁之前读取old->umask；取得旧锁后依次root值复制、path_get(root)、pwd值复制、path_get(pwd)、解锁、返回fs。任何前序对象/锁或路径请求失效均在首个失效点停止，不构造正常失败回滚。
- exit_fs/free_fs_struct完整活动体为fs_struct.c85–109：保存tsk.fs在task_lock之前；非NULL门内task_lock、fs.lock、tsk.fs=NULL、kill=!--users、解锁、kill时free_fs_struct。后者按path_put(root)、path_put(pwd)、kmem_cache_free(fs_cachep,fs)次序执行；后一步的正常执行要前一步正常返回及相应对象仍有效。
- path_s包含mnt和dentry两个指针；fs_struct的root和pwd各为内嵌path_s。copy赋值是结构体值/其中指针值复制，不代表dentry或vfsmount深复制。
- namei.c的path_get/path_put所选完整活动体中，dget(path->dentry)/dput(path->dentry)是活动调用，mntget/mntput两行以//注释。包装层请求和底层实际引用增减、物理释放是不同认证层级，本批不审dget/dput具体实现。
- 由proc_caches_init创建的fs_cachep用于动态fs_struct分配/释放；static init_fs的来源与该cache不同。没有由其名字推定全部cache内存自动清零。
- static init_fs初始users=1，umask=0022，init_task.fs=&init_fs。不能从静态初值推断真实运行时users、root/pwd或最终归还。
- 历史files_struct.refcount是atomic_t、file_s.f_count是atomic_long_t，与本批普通int taskfs.users不同；该两类型已在本轮直接定点读源码，但不计入本批二轮证据标签。R44/R31的已有限定不会因为重新引用而关闭。
- EAGAIN=11、ENOMEM=12作为单宏分别证实；不把符号意义等同完整errno/ABI路径正确性。
- 没有对其它set_fs_root/get_fs_root/chdir、path walk、mount命名空间、完整fork/exit或slab进行审计。

## 方法登记

直接固定time证据标签 55、两轮源段逐字复读 55、降级至推断0。原始物理源下载/P9全项、C/ASM/仓库脚本、预处理/编译/QEMU/硬件/真实fork、Graphviz、深层路径引用或缓存释放均NOT_RUN。
