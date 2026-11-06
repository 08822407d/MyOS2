---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FS-PATH-ACCESS-46
record_type: fixed_time_direct_definition_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
date: 2026-11-06
source_pin_short12: a039d9803ade
read_channel: connector
runtime_evidence: NOT_RUN
---

# 第46批：getter/setter及直接path/dentry helper源码

下列每个标签均取自固定time@a039d9803ade的单函数/结构体/单宏真实定义体1–5行，使用实际读取行号。两轮GitHub连接器对九个所选文件全文读取，并对每段按同位行和符号身份逐字比对。相邻和重叠片段仅用于分辨活动顺序，不是相同数量的独立缺陷。函数内注释作非活动上下文；函数外注释、extern、#if选择和EXPORT不直接算作函数行为。未运行预处理认证链接绑定。

### FP46-A001 — path_mnt_dentry（7–10行）

[VERIFIED mykernel/include/linux/fs/path.h::path_s]
```c
	typedef struct path {
		vfsmount_s	*mnt;
		dentry_s	*dentry;
	} path_s;
```

### FP46-A002 — real_struct（66–69行）

[VERIFIED mykernel/include/linux/fs/dcache.h::dentry_s]
```c
	typedef struct dentry
	{
		/* RCU lookup touched fields */
		unsigned int 		d_flags;		/* protected by d_lock */
```

### FP46-A003 — parent_name_inode（72–75行）

[VERIFIED mykernel/include/linux/fs/dcache.h::dentry_s]
```c
		dentry_s			*d_parent;		/* parent directory */
		qstr_s				d_name;
		inode_s				*d_inode;		/* Where the name belongs to - NULL is
											* negative */
```

### FP46-A004 — commented_lockref（78–81行）

[VERIFIED mykernel/include/linux/fs/dcache.h::dentry_s]
```c
		/* Ref lookup also touches following */
		// lockref_s		d_lockref;			/* per-dentry lock and refcount */
		const dentry_ops_s	*d_op;
		super_block_s		*d_sb;			/* The root of the dentry tree */
```

### FP46-A005 — signature_and_noop（281–285行）

[VERIFIED mykernel/include/linux/fs/dcache.h::dget]
```c
	static inline dentry_s *
	dget(dentry_s *dentry) {
		// if (dentry)
		// 	lockref_get(&dentry->d_lockref);
		return dentry;
```

### FP46-A006 — commented_ref_and_return（282–285行）

[VERIFIED mykernel/include/linux/fs/dcache.h::dget]
```c
	dget(dentry_s *dentry) {
		// if (dentry)
		// 	lockref_get(&dentry->d_lockref);
		return dentry;
```

### FP46-A007 — return_pointer（285–286行）

[VERIFIED mykernel/include/linux/fs/dcache.h::dget]
```c
		return dentry;
	}
```

### FP46-A008 — void_function_begin（108–110行）

[VERIFIED mykernel/fs/vfs/dcache.c::dput]
```c
void dput(dentry_s *dentry)
{
	// while (dentry) {
```

### FP46-A009 — commented_fast_release（113–117行）

[VERIFIED mykernel/fs/vfs/dcache.c::dput]
```c
	// 	rcu_read_lock();
	// 	if (likely(fast_dput(dentry))) {
	// 		rcu_read_unlock();
	// 		return;
	// 	}
```

### FP46-A010 — commented_retention（122–126行）

[VERIFIED mykernel/fs/vfs/dcache.c::dput]
```c
	// 	if (likely(retain_dentry(dentry))) {
	// 		spin_unlock(&dentry->d_lock);
	// 		return;
	// 	}

```

### FP46-A011 — commented_kill_and_close（127–129行）

[VERIFIED mykernel/fs/vfs/dcache.c::dput]
```c
	// 	dentry = dentry_kill(dentry);
	// }
}
```

### FP46-A012 — path_get_wrapper（266–270行）

[VERIFIED mykernel/fs/vfs/namei.c::path_get]
```c
void path_get(const path_s *path)
{
	// mntget(path->mnt);
	dget(path->dentry);
}
```

### FP46-A013 — mount_get_commented（268–270行）

[VERIFIED mykernel/fs/vfs/namei.c::path_get]
```c
	// mntget(path->mnt);
	dget(path->dentry);
}
```

### FP46-A014 — ignored_dget_result（269–269行）

[VERIFIED mykernel/fs/vfs/namei.c::path_get]
```c
	dget(path->dentry);
```

### FP46-A015 — path_put_wrapper（278–282行）

[VERIFIED mykernel/fs/vfs/namei.c::path_put]
```c
void path_put(const path_s *path)
{
	dput(path->dentry);
	// mntput(path->mnt);
}
```

### FP46-A016 — mount_put_commented（280–282行）

[VERIFIED mykernel/fs/vfs/namei.c::path_put]
```c
	dput(path->dentry);
	// mntput(path->mnt);
}
```

### FP46-A017 — dput_call（280–280行）

[VERIFIED mykernel/fs/vfs/namei.c::path_put]
```c
	dput(path->dentry);
```

### FP46-A018 — getter_start_and_lock（24–27行）

[VERIFIED mykernel/sched/misc/fs_struct.h::get_fs_root]
```c
		PREFIX_STATIC_INLINE
		void
		get_fs_root(taskfs_s *fs, path_s *root) {
			spin_lock(&fs->lock);
```

### FP46-A019 — get_root_order（26–30行）

[VERIFIED mykernel/sched/misc/fs_struct.h::get_fs_root]
```c
		get_fs_root(taskfs_s *fs, path_s *root) {
			spin_lock(&fs->lock);
			*root = fs->root;
			path_get(root);
			spin_unlock(&fs->lock);
```

### FP46-A020 — get_root_effect（27–30行）

[VERIFIED mykernel/sched/misc/fs_struct.h::get_fs_root]
```c
			spin_lock(&fs->lock);
			*root = fs->root;
			path_get(root);
			spin_unlock(&fs->lock);
```

### FP46-A021 — get_root_copy_then_get（28–29行）

[VERIFIED mykernel/sched/misc/fs_struct.h::get_fs_root]
```c
			*root = fs->root;
			path_get(root);
```

### FP46-A022 — getter_pwd_start（33–36行）

[VERIFIED mykernel/sched/misc/fs_struct.h::get_fs_pwd]
```c
		PREFIX_STATIC_INLINE
		void
		get_fs_pwd(taskfs_s *fs, path_s *pwd) {
			spin_lock(&fs->lock);
```

### FP46-A023 — get_pwd_order（35–39行）

[VERIFIED mykernel/sched/misc/fs_struct.h::get_fs_pwd]
```c
		get_fs_pwd(taskfs_s *fs, path_s *pwd) {
			spin_lock(&fs->lock);
			*pwd = fs->pwd;
			path_get(pwd);
			spin_unlock(&fs->lock);
```

### FP46-A024 — get_pwd_effect（36–39行）

[VERIFIED mykernel/sched/misc/fs_struct.h::get_fs_pwd]
```c
			spin_lock(&fs->lock);
			*pwd = fs->pwd;
			path_get(pwd);
			spin_unlock(&fs->lock);
```

### FP46-A025 — get_pwd_copy_then_get（37–38行）

[VERIFIED mykernel/sched/misc/fs_struct.h::get_fs_pwd]
```c
			*pwd = fs->pwd;
			path_get(pwd);
```

### FP46-A026 — set_root_start（9–13行）

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_root]
```c
void set_fs_root(taskfs_s *fs, const path_s *path)
{
	path_s old_root;

	path_get(path);
```

### FP46-A027 — input_get_before_lock（13–14行）

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_root]
```c
	path_get(path);
	spin_lock(&fs->lock);
```

### FP46-A028 — root_lock_and_copy（14–18行）

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_root]
```c
	spin_lock(&fs->lock);
	// write_seqcount_begin(&fs->seq);

	old_root = fs->root;
	fs->root = *path;
```

### FP46-A029 — old_root_then_publish（17–18行）

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_root]
```c
	old_root = fs->root;
	fs->root = *path;
```

### FP46-A030 — root_unlock_then_conditional_put（20–24行）

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_root]
```c
	// write_seqcount_end(&fs->seq);
	spin_unlock(&fs->lock);
	if (old_root.dentry)
		path_put(&old_root);
}
```

### FP46-A031 — root_dentry_gate（21–24行）

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_root]
```c
	spin_unlock(&fs->lock);
	if (old_root.dentry)
		path_put(&old_root);
}
```

### FP46-A032 — root_gate_only_dentry（22–23行）

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_root]
```c
	if (old_root.dentry)
		path_put(&old_root);
```

### FP46-A033 — set_pwd_start（30–34行）

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_pwd]
```c
void set_fs_pwd(taskfs_s *fs, const path_s *path)
{
	path_s old_pwd;

	path_get(path);
```

### FP46-A034 — pwd_get_before_lock（34–35行）

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_pwd]
```c
	path_get(path);
	spin_lock(&fs->lock);
```

### FP46-A035 — pwd_lock_and_copy（35–39行）

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_pwd]
```c
	spin_lock(&fs->lock);
	// write_seqcount_begin(&fs->seq);

	old_pwd = fs->pwd;
	fs->pwd = *path;
```

### FP46-A036 — old_pwd_then_publish（38–39行）

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_pwd]
```c
	old_pwd = fs->pwd;
	fs->pwd = *path;
```

### FP46-A037 — pwd_unlock_then_conditional_put（41–45行）

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_pwd]
```c
	// write_seqcount_end(&fs->seq);
	spin_unlock(&fs->lock);
	if (old_pwd.dentry)
		path_put(&old_pwd);
}
```

### FP46-A038 — pwd_dentry_gate（42–45行）

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_pwd]
```c
	spin_unlock(&fs->lock);
	if (old_pwd.dentry)
		path_put(&old_pwd);
}
```

### FP46-A039 — pwd_gate_only_dentry（43–44行）

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_pwd]
```c
	if (old_pwd.dentry)
		path_put(&old_pwd);
```

### FP46-A040 — taskfs_users_lock（9–11行）

[VERIFIED mykernel/sched/misc/fs_struct_types.h::fs_struct]
```c
	struct fs_struct {
		int			users;
		spinlock_t	lock;
```

### FP46-A041 — root_pwd_embedded_fields（13–16行）

[VERIFIED mykernel/sched/misc/fs_struct_types.h::fs_struct]
```c
		int			umask;
		int			in_exec;
		path_s		root, pwd;
	};
```

### FP46-A042 — previous_copy_requests（123–127行）

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
		spin_lock(&old->lock);
		fs->root = old->root;
		path_get(&fs->root);
		fs->pwd = old->pwd;
		path_get(&fs->pwd);
```

### FP46-A043 — previous_free_requests（85–89行）

[VERIFIED mykernel/sched/misc/fs_struct.c::free_fs_struct]
```c
void free_fs_struct(taskfs_s *fs)
{
	path_put(&fs->root);
	path_put(&fs->pwd);
	kmem_cache_free(fs_cachep, fs);
```

### FP46-A044 — real_callsite_start（647–651行）

[VERIFIED mykernel/fs/vfs/namespace.c::init_mount_tree]
```c
static void init_mount_tree(void)
{
	vfsmount_s *mnt;
	mount_s *m;
	path_s root;
```

### FP46-A045 — current_and_mount_origin（652–656行）

[VERIFIED mykernel/fs/vfs/namespace.c::init_mount_tree]
```c
	task_s *curr= current;

	mnt = vfs_kern_mount(&rootfs_fs_type, 0, "rootfs", NULL);
	if (IS_ERR(mnt))
	{
```

### FP46-A046 — mount_error_guard（654–657行）

[VERIFIED mykernel/fs/vfs/namespace.c::init_mount_tree]
```c
	mnt = vfs_kern_mount(&rootfs_fs_type, 0, "rootfs", NULL);
	if (IS_ERR(mnt))
	{
		color_printk(RED, BLACK, "Can't create rootfs");
```

### FP46-A047 — root_pointer_origin（661–663行）

[VERIFIED mykernel/fs/vfs/namespace.c::init_mount_tree]
```c
	root.mnt = mnt;
	root.dentry = mnt->mnt_root;
	mnt->mnt_flags |= MNT_LOCKED;
```

### FP46-A048 — two_setters_one_root（665–667行）

[VERIFIED mykernel/fs/vfs/namespace.c::init_mount_tree]
```c
	set_fs_pwd(curr->fs, &root);
	set_fs_root(curr->fs, &root);
}
```

### FP46-A049 — mnt_init_invokes_tree（669–673行）

[VERIFIED mykernel/fs/vfs/namespace.c::mnt_init]
```c
void mnt_init(void)
{
	shmem_init();
	init_rootfs();
	init_mount_tree();
```

### FP46-A050 — macro_nondebug_context（6–9行）

[VERIFIED mykernel/include/linux/compiler/myos_debug_option.h::PREFIX_STATIC_INLINE]
```c
	#ifndef DEBUG

	#	define PREFIX_STATIC_INLINE static inline
	#	define PREFIX_STATIC_AWLWAYS_INLINE static __always_inline
```

### FP46-A051 — macro_debug_context（11–14行）

[VERIFIED mykernel/include/linux/compiler/myos_debug_option.h::PREFIX_STATIC_INLINE]
```c
	#else /* DEBUG */

	#	define PREFIX_STATIC_INLINE
	#	define PREFIX_STATIC_AWLWAYS_INLINE
```

### FP46-A052 — get_root_void（26–26行）

[VERIFIED mykernel/sched/misc/fs_struct.h::get_fs_root]
```c
		get_fs_root(taskfs_s *fs, path_s *root) {
```

### FP46-A053 — get_pwd_void（35–35行）

[VERIFIED mykernel/sched/misc/fs_struct.h::get_fs_pwd]
```c
		get_fs_pwd(taskfs_s *fs, path_s *pwd) {
```

### FP46-A054 — set_root_void（9–9行）

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_root]
```c
void set_fs_root(taskfs_s *fs, const path_s *path)
```

### FP46-A055 — set_pwd_void（30–30行）

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_pwd]
```c
void set_fs_pwd(taskfs_s *fs, const path_s *path)
```

### FP46-A056 — path_get_void（266–266行）

[VERIFIED mykernel/fs/vfs/namei.c::path_get]
```c
void path_get(const path_s *path)
```

### FP46-A057 — path_put_void（278–278行）

[VERIFIED mykernel/fs/vfs/namei.c::path_put]
```c
void path_put(const path_s *path)
```

### FP46-A058 — dput_void（108–109行）

[VERIFIED mykernel/fs/vfs/dcache.c::dput]
```c
void dput(dentry_s *dentry)
{
```

### FP46-A059 — dget_pointer_return_type（281–282行）

[VERIFIED mykernel/include/linux/fs/dcache.h::dget]
```c
	static inline dentry_s *
	dget(dentry_s *dentry) {
```

## 直接定义体之外的负面证明边界

- dcache.h::dget完整活动体281–286无计数/字段写入，仅return dentry，返回dentry_s*。所选path_get不保存该返回值。dcache.c::dput完整体108–129为void且所有循环、refcount/retain/dentry_kill调用都在注释中，**没有活动语句**。此结论只在这些当前固定体内成立，不证所有其它dentry生存期机制。
- dentry_s结构内d_lockref是注释成员，当前该结构未见活动的等价计数字段；这不构成全仓无其它生命周期记录的证明。dget/dput被传入的指针值可以是NULL，在这些体内没有对它解引用；但求值未初始化或陷阱指针表示本身未证有效，不能笼统说任意非法参数安全。
- namei.c::path_get/path_put分别只发起dget(path->dentry)和dput(path->dentry)调用，mntget/mntput两侧均注释；path本体必须有效可读。path_s的mnt/dentry按值复制不是对象深复制；整个调用链当前没有由该包装层得到可证dentry计数增减或mount引用改变。
- fs_struct.h中两个getter位于DEBUG/FS_STRUCT_DEFINATION条件编译选择里。实际源码定义均为void：锁fs->lock，*out=fs相应path_s，path_get(out)，解锁；没有输出有效性检查、返回errno或先把输出旧值path_put的行为。某项操作不正常返回则不得补出下一项。
- fs_struct.c两个setter活动体：先path_get(input)，再锁fs，保存旧path_s值，发布input的值，解锁，if(old.dentry)才请求path_put(old)。old.mnt不是这道门。当前path_put→dput为空体，因此“请求归还”和“已实际引用减少/目录项销毁”必须分开。
- 在本轮GitHub默认分支代码搜索中，get_fs_root/get_fs_pwd只返回定义自身，未获得独立调用位置；它不是固定time全仓检索认证，**不得宣布无调用者**。固定time的namespace.c::init_mount_tree确实生成root.mnt/root.dentry，先set_fs_pwd、后set_fs_root，使用同一个root path值；不会因此自动给目录项/挂载建立两份真实计数引用。更深挂载成功、全局初始化完成和实际指针寿命均不在本轮。
- 原FS45-F02/R45-S01已包含两层引用helper未生效的核心事实，本批增加get/set访问接口和dentry结构取证，不重复登记历史同义根因。历史UV-10中“path_get已增加路径引用”的附带表述按R45-S01收窄，close绕过f_count的独立问题仍保留。
- 每条VERIFIED仅证固定源片段存在、与所标符号对应且在有定义的输入条件下支持局部语句；不会将相关注释或逻辑结构推演等同实际机器指令。

## P9分项与未执行

标签数59，两轮同片段复读59，降级至推断0。没有Graphviz、独立YAML解析、完整本地P9、原始源码字节物化、C预处理/编译、仓库C/ASM/脚本、QEMU/硬件、真实路径/挂载/锁/目录项销毁或额外付费API运行。文本复核和静态模型不等于实际内核正确性证明。
