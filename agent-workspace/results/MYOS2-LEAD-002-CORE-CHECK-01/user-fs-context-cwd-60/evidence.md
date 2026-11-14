---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-FS-CONTEXT-CWD-60
record_type: fixed_time_short_source_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
source_pin_short12: a039d9803ade
execution_base_short12: 2b3aec6f71e1
base_snapshot: time
read_channel: GitHub_connector
status: READY_FOR_MAINLINE_REVIEW
runtime_evidence: NOT_RUN
---
# 第60批源码证据（A/B/C）

每个VERIFIED标签直接锚定固定time快照中单一真实函数、结构体或宏定义体内连续1–5物理行。保留GitHub连接器返回的tab、源码空行及文件行尾。宏调用体MYOS_SYSCALL_DEFINE是未预处理的源码位置，不是编译认证ABI。本批没有D00纠正；R58C-S01确认58 B25原件有效，旧主线误判撤回，无需再次修改历史文件。跨片段事实须按正文的有效对象/整数/返回前提合读；短锚不是运行轨迹。
## A / chdir、root/pwd、路径引用与更新

### A01 MYOS_SYSCALL_DEFINE1(chdir)

[VERIFIED mykernel/fs/vfs/open.c::MYOS_SYSCALL_DEFINE1(chdir)]
```c
MYOS_SYSCALL_DEFINE1(chdir, const char *, filename)
{
	path_s path;
	task_s *curr = current;
	int error;
```

### A02 MYOS_SYSCALL_DEFINE1(chdir)

[VERIFIED mykernel/fs/vfs/open.c::MYOS_SYSCALL_DEFINE1(chdir)]
```c
	unsigned int lookup_flags = LOOKUP_FOLLOW | LOOKUP_DIRECTORY;
retry:
	error = user_path_at(AT_FDCWD, filename, lookup_flags, &path);
	if (error)
		goto out;
```

### A03 MYOS_SYSCALL_DEFINE1(chdir)

[VERIFIED mykernel/fs/vfs/open.c::MYOS_SYSCALL_DEFINE1(chdir)]
```c
	error = user_path_at(AT_FDCWD, filename, lookup_flags, &path);
	if (error)
		goto out;

	curr->fs->pwd = path;
```

### A04 MYOS_SYSCALL_DEFINE1(chdir)

[VERIFIED mykernel/fs/vfs/open.c::MYOS_SYSCALL_DEFINE1(chdir)]
```c
	curr->fs->pwd = path;

// dput_and_out:
// 	path_put(&path);
// 	if (retry_estale(error, lookup_flags)) {
```

### A05 MYOS_SYSCALL_DEFINE1(chdir)

[VERIFIED mykernel/fs/vfs/open.c::MYOS_SYSCALL_DEFINE1(chdir)]
```c
// 	if (retry_estale(error, lookup_flags)) {
// 		lookup_flags |= LOOKUP_REVAL;
// 		goto retry;
// 	}
out:
```

### A06 init_chdir

[VERIFIED mykernel/fs/vfs/init.c::init_chdir]
```c
int init_chdir(const char *filename)
{
	struct path path;
	int error;

```

### A07 init_chdir

[VERIFIED mykernel/fs/vfs/init.c::init_chdir]
```c
	error = kern_path(filename, LOOKUP_FOLLOW | LOOKUP_DIRECTORY, &path);
	if (error)
		return error;
	// error = path_permission(&path, MAY_EXEC | MAY_CHDIR);
	if (!error)
```

### A08 init_chdir

[VERIFIED mykernel/fs/vfs/init.c::init_chdir]
```c
	if (!error)
		set_fs_pwd(current->fs, &path);
	path_put(&path);
	return error;
}
```

### A09 init_chroot

[VERIFIED mykernel/fs/vfs/init.c::init_chroot]
```c
int init_chroot(const char *filename)
{
	struct path path;
	int error;

```

### A10 init_chroot

[VERIFIED mykernel/fs/vfs/init.c::init_chroot]
```c
	error = kern_path(filename, LOOKUP_FOLLOW | LOOKUP_DIRECTORY, &path);
	if (error)
		return error;
	// error = path_permission(&path, MAY_EXEC | MAY_CHDIR);
	// if (error)
```

### A11 init_chroot

[VERIFIED mykernel/fs/vfs/init.c::init_chroot]
```c
	// error = security_path_chroot(&path);
	// if (error)
	// 	goto dput_and_out;
	set_fs_root(current->fs, &path);
dput_and_out:
```

### A12 init_chroot

[VERIFIED mykernel/fs/vfs/init.c::init_chroot]
```c
	set_fs_root(current->fs, &path);
dput_and_out:
	path_put(&path);
	return error;
}
```

### A13 set_fs_root

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_root]
```c
void set_fs_root(taskfs_s *fs, const path_s *path)
{
	path_s old_root;

	path_get(path);
```

### A14 set_fs_root

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_root]
```c
	path_get(path);
	spin_lock(&fs->lock);
	// write_seqcount_begin(&fs->seq);

	old_root = fs->root;
```

### A15 set_fs_root

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_root]
```c
	old_root = fs->root;
	fs->root = *path;

	// write_seqcount_end(&fs->seq);
	spin_unlock(&fs->lock);
```

### A16 set_fs_root

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_root]
```c
	spin_unlock(&fs->lock);
	if (old_root.dentry)
		path_put(&old_root);
}
```

### A17 set_fs_pwd

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_pwd]
```c
void set_fs_pwd(taskfs_s *fs, const path_s *path)
{
	path_s old_pwd;

	path_get(path);
```

### A18 set_fs_pwd

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_pwd]
```c
	path_get(path);
	spin_lock(&fs->lock);
	// write_seqcount_begin(&fs->seq);

	old_pwd = fs->pwd;
```

### A19 set_fs_pwd

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_pwd]
```c
	old_pwd = fs->pwd;
	fs->pwd = *path;

	// write_seqcount_end(&fs->seq);
	spin_unlock(&fs->lock);
```

### A20 set_fs_pwd

[VERIFIED mykernel/sched/misc/fs_struct.c::set_fs_pwd]
```c
	spin_unlock(&fs->lock);
	if (old_pwd.dentry)
		path_put(&old_pwd);
}
```

### A21 path_s

[VERIFIED mykernel/include/linux/fs/path.h::path_s]
```c
	typedef struct path {
		vfsmount_s	*mnt;
		dentry_s	*dentry;
	} path_s;
```

### A22 path_get

[VERIFIED mykernel/fs/vfs/namei.c::path_get]
```c
void path_get(const path_s *path)
{
	// mntget(path->mnt);
	dget(path->dentry);
}
```

### A23 path_put

[VERIFIED mykernel/fs/vfs/namei.c::path_put]
```c
void path_put(const path_s *path)
{
	dput(path->dentry);
	// mntput(path->mnt);
}
```

### A24 dget

[VERIFIED mykernel/include/linux/fs/dcache.h::dget]
```c
	static inline dentry_s *
	dget(dentry_s *dentry) {
		// if (dentry)
		// 	lockref_get(&dentry->d_lockref);
		return dentry;
```

### A25 dput

[VERIFIED mykernel/fs/vfs/dcache.c::dput]
```c
void dput(dentry_s *dentry)
{
	// while (dentry) {
	// 	might_sleep();

```

### A26 dput

[VERIFIED mykernel/fs/vfs/dcache.c::dput]
```c
	// 	}

	// 	dentry = dentry_kill(dentry);
	// }
}
```

### A27 set_init_taskfs

[VERIFIED mykernel/fs/vfs/myos_vfs.c::set_init_taskfs]
```c
void set_init_taskfs()
{
	task_s * curr = current;
	// set cwd and root-dir of task1
	taskfs_s *taskfs_p = curr->fs;
```

### A28 set_init_taskfs

[VERIFIED mykernel/fs/vfs/myos_vfs.c::set_init_taskfs]
```c
	taskfs_s *taskfs_p = curr->fs;
	taskfs_p->pwd.dentry = 
	taskfs_p->root.dentry = myos_root_sb->s_root;
	taskfs_p->pwd.mnt = 
	taskfs_p->root.mnt = &myos_root_mnt.mnt;
```

### A29 set_init_taskfs

[VERIFIED mykernel/fs/vfs/myos_vfs.c::set_init_taskfs]
```c
	taskfs_p->pwd.mnt = 
	taskfs_p->root.mnt = &myos_root_mnt.mnt;

	*curr->fs = *taskfs_p;
}
```

### A30 user_path_at

[VERIFIED mykernel/include/linux/fs/namei.h::user_path_at]
```c
	static inline int
	user_path_at(int dfd, const char *name, unsigned flags, path_s *path) {
		return user_path_at_empty(dfd, name, flags, path);
	}
```

### A31 user_path_at_empty

[VERIFIED mykernel/fs/vfs/namei.c::user_path_at_empty]
```c
int user_path_at_empty(int dfd, const char *name,
		unsigned flags, OUT path_s *path)
{
	filename_s *fn = getname(name);
	return filename_lookup(dfd, fn, flags, path, NULL);
```

### A32 path_init

[VERIFIED mykernel/fs/vfs/namei.c::path_init]
```c
	const char *s = nd->name->name;

	nd->flags = flags;

	nd->root = current->fs->root;
```

### A33 path_init

[VERIFIED mykernel/fs/vfs/namei.c::path_init]
```c
	nd->root = current->fs->root;
	if (s[0] == '/')
		nd->path = nd->root;
	else
		nd->path = current->fs->pwd;
```

### A34 path_lookupat

[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	if (err == 0)
	{
		*path = nd->path;
		nd->path.mnt = NULL;
		nd->path.dentry = NULL;
```

### A35 fs_struct

[VERIFIED mykernel/sched/misc/fs_struct_types.h::fs_struct]
```c
	struct fs_struct {
		int			users;
		spinlock_t	lock;
		// seqcount_spinlock_t seq;
		int			umask;
```

### A36 fs_struct

[VERIFIED mykernel/sched/misc/fs_struct_types.h::fs_struct]
```c
		int			umask;
		int			in_exec;
		path_s		root, pwd;
	};
```


## B / getcwd逆向拼接、容量与返回

### B01 prpndbuf_s

[VERIFIED mykernel/fs/vfs/d_path.c::prpndbuf_s]
```c
typedef struct prepend_buffer
{
	char *buf;
	int len;
} prpndbuf_s;
```

### B02 DECLARE_BUFFER

[VERIFIED mykernel/fs/vfs/d_path.c::DECLARE_BUFFER]
```c
#define DECLARE_BUFFER(__name, __buf, __len)		\
			struct prepend_buffer __name = {		\
				.buf = __buf + __len, .len = __len	\
			}
```

### B03 extract_string

[VERIFIED mykernel/fs/vfs/d_path.c::extract_string]
```c
static char *extract_string(prpndbuf_s *p)
{
	if (likely(p->len >= 0))
		return p->buf;
	return ERR_PTR(-ENAMETOOLONG);
```

### B04 prepend_char

[VERIFIED mykernel/fs/vfs/d_path.c::prepend_char]
```c
{
	if (likely(p->len > 0)) {
		p->len--;
		*--p->buf = c;
		return true;
```

### B05 myos_prepend

[VERIFIED mykernel/fs/vfs/d_path.c::myos_prepend]
```c
static bool myos_prepend(prpndbuf_s *p, const char *str, int namelen)
{
	// Already overflowed?
	if (p->len < 0)
		return false;
```

### B06 myos_prepend

[VERIFIED mykernel/fs/vfs/d_path.c::myos_prepend]
```c
	p->len -= namelen;
	if (p->len >= 0) {
		p->buf -= namelen;
		memcpy(p->buf, str, namelen);
	}
```

### B07 myos_prepend

[VERIFIED mykernel/fs/vfs/d_path.c::myos_prepend]
```c
	}

	return true;
}
```

### B08 prepend_name

[VERIFIED mykernel/fs/vfs/d_path.c::prepend_name]
```c
static bool prepend_name(prpndbuf_s *p, const qstr_s *name)
{
	const char *dname = (const char *)name->name; /* ^^^ */
	uint32_t dlen = name->len;
	char *s;
```

### B09 prepend_name

[VERIFIED mykernel/fs/vfs/d_path.c::prepend_name]
```c
	p->len -= dlen + 1;
	if (p->len < 0)
		return false;
	s = p->buf -= dlen + 1;
	*s++ = '/';
```

### B10 prepend_name

[VERIFIED mykernel/fs/vfs/d_path.c::prepend_name]
```c
	while (dlen--) {
		char c = *dname++;
		if (c == '\0')
			break;
		*s++ = c;
```

### B11 prepend_name

[VERIFIED mykernel/fs/vfs/d_path.c::prepend_name]
```c
		*s++ = c;
	}
	return true;
}
```

### B12 __prepend_path

[VERIFIED mykernel/fs/vfs/d_path.c::__prepend_path]
```c
static int __prepend_path(const dentry_s *dentry,
		const mount_s *mnt, const path_s *root, prpndbuf_s *p)
{
	while (dentry != root->dentry || &mnt->mnt != root->mnt) {
		const dentry_s *parent = READ_ONCE(dentry->d_parent);
```

### B13 __prepend_path

[VERIFIED mykernel/fs/vfs/d_path.c::__prepend_path]
```c
	while (dentry != root->dentry || &mnt->mnt != root->mnt) {
		const dentry_s *parent = READ_ONCE(dentry->d_parent);

		if (dentry == mnt->mnt.mnt_root) {
			mount_s *m = READ_ONCE(mnt->mnt_parent);
```

### B14 __prepend_path

[VERIFIED mykernel/fs/vfs/d_path.c::__prepend_path]
```c
			mount_s *m = READ_ONCE(mnt->mnt_parent);

			if (likely(mnt != m)) {
				dentry = READ_ONCE(mnt->mnt_mountpoint);
				mnt = m;
```

### B15 __prepend_path

[VERIFIED mykernel/fs/vfs/d_path.c::__prepend_path]
```c
				mnt = m;
				continue;
			}

			return 2;	// detached or not attached yet
```

### B16 __prepend_path

[VERIFIED mykernel/fs/vfs/d_path.c::__prepend_path]
```c
		prefetch(parent);
		if (unlikely(dentry == parent))
			/* Escaped? */
			return 3;

```

### B17 __prepend_path

[VERIFIED mykernel/fs/vfs/d_path.c::__prepend_path]
```c
		if (!prepend_name(p, &dentry->d_name))
			break;
		dentry = parent;
	}
	return 0;
```

### B18 myos_prepend_path

[VERIFIED mykernel/fs/vfs/d_path.c::myos_prepend_path]
```c
static int myos_prepend_path(const path_s *path,
		const path_s *root, prpndbuf_s *p)
{
	unsigned seq, m_seq = 0;
	prpndbuf_s b;
```

### B19 myos_prepend_path

[VERIFIED mykernel/fs/vfs/d_path.c::myos_prepend_path]
```c
restart:
	b = *p;
	error = __prepend_path(path->dentry, real_mount(path->mnt), root, &b);

	if (error == 3)
```

### B20 myos_prepend_path

[VERIFIED mykernel/fs/vfs/d_path.c::myos_prepend_path]
```c
	if (error == 3)
		b = *p;

	if (unlikely(error == 3))
		b = *p;
```

### B21 myos_prepend_path

[VERIFIED mykernel/fs/vfs/d_path.c::myos_prepend_path]
```c
	if (b.len == p->len)
		myos_prepend(&b, "/", 1);

	*p = b;
	return error;
```

### B22 MYOS_SYSCALL_DEFINE2(getcwd)

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
MYOS_SYSCALL_DEFINE2(getcwd, char *, buf, unsigned long, size)
{
	int		error;
	task_s *curr = current;
	path_s	pwd = curr->fs->pwd,
```

### B23 MYOS_SYSCALL_DEFINE2(getcwd)

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
	path_s	pwd = curr->fs->pwd,
			root = curr->fs->root;
	prpndbuf_s b;
	char *bufhead;

```

### B24 MYOS_SYSCALL_DEFINE2(getcwd)

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		unsigned len;
		b.len = PATH_MAX;
		bufhead = kzalloc(b.len, GFP_KERNEL);
		b.buf = bufhead + PATH_MAX;

```

### B25 MYOS_SYSCALL_DEFINE2(getcwd)

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		myos_prepend(&b, "", 1);
		if (myos_prepend_path(&pwd, &root, &b) > 0)
			myos_prepend(&b, "(unreachable)", 13);

		len = PATH_MAX - b.len;
```

### B26 MYOS_SYSCALL_DEFINE2(getcwd)

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		len = PATH_MAX - b.len;
		if (len > PATH_MAX)
			error = -ENAMETOOLONG;
		else if (len > size)
			error = -ERANGE;
```

### B27 MYOS_SYSCALL_DEFINE2(getcwd)

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		else if (len > size)
			error = -ERANGE;
		else if (copy_to_user(buf, b.buf, len))
			error = -EFAULT;
		else
```

### B28 MYOS_SYSCALL_DEFINE2(getcwd)

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		else if (copy_to_user(buf, b.buf, len))
			error = -EFAULT;
		else
			error = (long)buf;
	// }
```

### B29 MYOS_SYSCALL_DEFINE2(getcwd)

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
	// }
	// __putname(page);
	kfree(bufhead);
	return error;
}
```

### B30 PATH_MAX

[VERIFIED mykernel/include/uapi/linux/limits.h::PATH_MAX]
```c
	#define PATH_MAX		4096	/* # chars in a path name including nul */
```

### B31 real_mount

[VERIFIED mykernel/include/linux/fs/mount.h::real_mount]
```c
static inline mount_s *
real_mount(vfsmount_s *mnt) {
	return container_of(mnt, struct mount, mnt);
}
```

### B32 mount_s

[VERIFIED mykernel/include/linux/fs/mount.h::mount_s]
```c
typedef struct mount {
	// hlist_node_s mnt_hash;
	mount_s		*mnt_parent;
	dentry_s	*mnt_mountpoint;
	vfsmount_s	mnt;
```

### B33 qstr_s

[VERIFIED mykernel/include/linux/fs/dcache.h::qstr_s]
```c
	typedef struct qstr {
		size_t				len;
		const unsigned char	*name;
	} qstr_s;
```

### B34 _inline_copy_to_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_to_user]
```c
		if (access_ok(to, n)) {
			instrument_copy_to_user(to, from, n);
			n = raw_copy_to_user(to, from, n);
		}
		return n;
```

### B35 copy_to_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::copy_to_user]
```c
	copy_to_user(void __user *to, const void *from, ulong n) {
		// if (!check_copy_size(from, n, true))
		// 	return n;
	// #ifdef INLINE_COPY_TO_USER
		return _inline_copy_to_user(to, from, n);
```


## C / taskfs复制、CLONE_FS共享与退出

### C01 taskfs_s

[VERIFIED mykernel/sched/sched_type_declaration.h::taskfs_s]
```c
	typedef struct fs_struct taskfs_s;
```

### C02 dup_task_struct

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
static task_s *dup_task_struct(task_s *orig, int node)
{
	task_s *tsk;
	int err;

```

### C03 dup_task_struct

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	if (node == NUMA_NO_NODE)
		// node = tsk_fork_get_node(orig);
	tsk = alloc_task_struct_node(node);
	if (!tsk)
		return NULL;
```

### C04 dup_task_struct

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	INIT_LIST_S(&tsk->tasks);
	// int __weak arch_dup_task_struct(task_s *dst, task_s *src)
	// {
		*tsk = *orig;
		err = 0;
```

### C05 copy_fs

[VERIFIED mykernel/sched/forkexec/fork.c::copy_fs]
```c
static int copy_fs(unsigned long clone_flags, task_s *tsk)
{
	taskfs_s *fs = current->fs;
	if (clone_flags & CLONE_FS) {
		/* tsk->fs is already what we want */
```

### C06 copy_fs

[VERIFIED mykernel/sched/forkexec/fork.c::copy_fs]
```c
	if (clone_flags & CLONE_FS) {
		/* tsk->fs is already what we want */
		// spin_lock(&fs->lock);
		if (fs->in_exec) {
			// spin_unlock(&fs->lock);
```

### C07 copy_fs

[VERIFIED mykernel/sched/forkexec/fork.c::copy_fs]
```c
		if (fs->in_exec) {
			// spin_unlock(&fs->lock);
			return -EAGAIN;
		}
		fs->users++;
```

### C08 copy_fs

[VERIFIED mykernel/sched/forkexec/fork.c::copy_fs]
```c
		fs->users++;
		// spin_unlock(&fs->lock);
		return 0;
	}
	tsk->fs = copy_fs_struct(fs);
```

### C09 copy_fs

[VERIFIED mykernel/sched/forkexec/fork.c::copy_fs]
```c
	tsk->fs = copy_fs_struct(fs);
	if (!tsk->fs)
		return -ENOMEM;
	return 0;
}
```

### C10 copy_fs_struct

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
taskfs_s *copy_fs_struct(taskfs_s *old)
{
	taskfs_s *fs = kmem_cache_alloc(fs_cachep, GFP_KERNEL);
	/* We don't need to lock fs - think why ;-) */
	if (fs) {
```

### C11 copy_fs_struct

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
	if (fs) {
		fs->users = 1;
		fs->in_exec = 0;

		spin_lock_init(&fs->lock);
```

### C12 copy_fs_struct

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
		spin_lock_init(&fs->lock);
		// seqcount_spinlock_init(&fs->seq, &fs->lock);
		fs->umask = old->umask;

		spin_lock(&old->lock);
```

### C13 copy_fs_struct

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
		spin_lock(&old->lock);
		fs->root = old->root;
		path_get(&fs->root);
		fs->pwd = old->pwd;
		path_get(&fs->pwd);
```

### C14 copy_fs_struct

[VERIFIED mykernel/sched/misc/fs_struct.c::copy_fs_struct]
```c
		path_get(&fs->root);
		fs->pwd = old->pwd;
		path_get(&fs->pwd);
		spin_unlock(&old->lock);
	}
```

### C15 init_fs

[VERIFIED mykernel/sched/misc/fs_struct.c::init_fs]
```c
taskfs_s init_fs = {
	.users		= 1,
	.lock		= __SPIN_LOCK_UNLOCKED(init_fs.lock),
	// .seq		= SEQCNT_SPINLOCK_ZERO(init_fs.seq, &init_fs.lock),
	.umask		= 0022,
```

### C16 exit_fs

[VERIFIED mykernel/sched/misc/fs_struct.c::exit_fs]
```c
void exit_fs(task_s *tsk)
{
	taskfs_s *fs = tsk->fs;

	if (fs) {
```

### C17 exit_fs

[VERIFIED mykernel/sched/misc/fs_struct.c::exit_fs]
```c
	if (fs) {
		int kill;
		task_lock(tsk);
		spin_lock(&fs->lock);

```

### C18 exit_fs

[VERIFIED mykernel/sched/misc/fs_struct.c::exit_fs]
```c
		tsk->fs = NULL;
		kill = !--fs->users;

		spin_unlock(&fs->lock);
		task_unlock(tsk);
```

### C19 exit_fs

[VERIFIED mykernel/sched/misc/fs_struct.c::exit_fs]
```c
		spin_unlock(&fs->lock);
		task_unlock(tsk);
		if (kill)
			free_fs_struct(fs);
	}
```

### C20 free_fs_struct

[VERIFIED mykernel/sched/misc/fs_struct.c::free_fs_struct]
```c
void free_fs_struct(taskfs_s *fs)
{
	path_put(&fs->root);
	path_put(&fs->pwd);
	kmem_cache_free(fs_cachep, fs);
```

### C21 copy_process

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if (retval)
		goto bad_fork_cleanup_semundo;
	retval = copy_fs(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_files;
```

### C22 copy_process

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_fs(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_files;
	retval = copy_sighand(clone_flags, p);
	if (retval)
```

### C23 copy_process

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cleanup_fs:
	exit_fs(p); /* blocking */
bad_fork_cleanup_files:
	exit_files(p); /* blocking */
bad_fork_cleanup_semundo:
```

### C24 do_exit

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	// exit_sem(tsk);
	// exit_shm(tsk);
	exit_files(tsk);
	exit_fs(tsk);
	// if (group_dead)
```

### C25 proc_caches_init

[VERIFIED mykernel/sched/forkexec/fork.c::proc_caches_init]
```c
	fs_cachep =
			kmem_cache_create("fs_cache",
				sizeof(taskfs_s), 0,
				SLAB_HWCACHE_ALIGN|SLAB_PANIC|SLAB_ACCOUNT,
				NULL);
```

### C26 init_task

[VERIFIED mykernel/init/init_task.c::init_task]
```c
	.comm				= INIT_TASK_COMM,
	.thread				= INIT_THREAD,
	.fs					= &init_fs,
	.files				= &init_files,
```

### C27 task_s

[VERIFIED mykernel/sched/task/task_types.h::task_s]
```c
		/* Filesystem information: */
		taskfs_s		*fs;

		/* Open file information: */
		files_struct_s	*files;
```

### C28 CLONE_FS

[VERIFIED mykernel/include/uapi/linux/sched.h::CLONE_FS]
```c
	#define CLONE_FS				0x00000200		/* set if fs info shared between processes */
```

### C29 CLONE_FILES

[VERIFIED mykernel/include/uapi/linux/sched.h::CLONE_FILES]
```c
	#define CLONE_FILES				0x00000400		/* set if open files shared between processes */
```

### C30 fs_cachep

[VERIFIED mykernel/sched/forkexec/fork.c::fs_cachep]
```c
kmem_cache_s *fs_cachep;
```

### C31 copy_process

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	p = dup_task_struct(current, node);
	if (!p)
		goto fork_out;
	p->flags &= ~PF_KTHREAD;
```

### C32 get_fs_root

[VERIFIED mykernel/sched/misc/fs_struct.h::get_fs_root]
```c
		get_fs_root(taskfs_s *fs, path_s *root) {
			spin_lock(&fs->lock);
			*root = fs->root;
			path_get(root);
			spin_unlock(&fs->lock);
```

### C33 get_fs_pwd

[VERIFIED mykernel/sched/misc/fs_struct.h::get_fs_pwd]
```c
		get_fs_pwd(taskfs_s *fs, path_s *pwd) {
			spin_lock(&fs->lock);
			*pwd = fs->pwd;
			path_get(pwd);
			spin_unlock(&fs->lock);
```

