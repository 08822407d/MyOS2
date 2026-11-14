---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-PATH-INIT-31
record_type: fixed_time_verbatim_source_evidence
executor: MYOS2-LEAD-002-CHAT-EXEC-03
date: 2026-11-03
source_ref: time
source_pin_short12: a039d9803ade
read_channel: connector
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---
# 第31批固定源码逐字引文

全部引文来自同一个 `time@a039d9803ade` 的GitHub连接器返回；每段1–5行（空行计数），对应真实函数体、结构体、宏或内联定义。标题仅作导航，不能替代关键语句本身；`path_s`指针值复制不自动意味着持有文件系统引用，`return 0`也不意味着实际路径可用。

### PI31-A001 · nameidata结构的path/last/root三字段
[VERIFIED mykernel/fs/vfs/namei.c::nameidata_s]
```c
typedef struct nameidata
{
	path_s path;
	qstr_s last;
	path_s root;
```
固定源码行36–40。

### PI31-A002 · nameidata的root/inode/last_type/flags字段
[VERIFIED mykernel/fs/vfs/namei.c::nameidata_s]
```c
	path_s root;
	inode_s *inode; /* path.dentry.d_inode */

	int last_type;
	unsigned flags, state;
```
固定源码行40–44。

### PI31-A003 · nameidata的state和借入filename成员
[VERIFIED mykernel/fs/vfs/namei.c::nameidata_s]
```c
	int last_type;
	unsigned flags, state;
	unsigned depth;
	int total_link_count;
	filename_s *name;
```
固定源码行43–47。

### PI31-A004 · nameidata的dfd与结构尾
[VERIFIED mykernel/fs/vfs/namei.c::nameidata_s]
```c
	// struct filename	*name;
	// struct nameidata *saved;
	unsigned root_seq;
	int dfd;
} nameidata_s;
```
固定源码行49–53。

### PI31-A005 · path_s只保存mnt与dentry指针
[VERIFIED mykernel/include/linux/fs/path.h::path_s]
```c
	typedef struct path {
		vfsmount_s	*mnt;
		dentry_s	*dentry;
	} path_s;
```
固定源码行7–10。

### PI31-A006 · fs_struct对象内含users/lock/umask等
[VERIFIED mykernel/sched/misc/fs_struct_types.h::fs_struct]
```c
	struct fs_struct {
		int			users;
		spinlock_t	lock;
		// seqcount_spinlock_t seq;
		int			umask;
```
固定源码行9–13。

### PI31-A007 · fs_struct同时保存root和pwd两个path_s
[VERIFIED mykernel/sched/misc/fs_struct_types.h::fs_struct]
```c
		int			in_exec;
		path_s		root, pwd;
	};
```
固定源码行14–16。

### PI31-A008 · filename_s的name、uptr和len
[VERIFIED mykernel/include/linux/fs/fs.h::filename_s]
```c
	typedef struct filename {
		const char	*name;	/* pointer to actual string */
		const char	*uptr;	/* original userland pointer */
		int			len;
```
固定源码行2517–2520。

### PI31-A009 · set_nameidata的p/dfd/name/root签名
[VERIFIED mykernel/fs/vfs/namei.c::set_nameidata]
```c
static void set_nameidata(OUT nameidata_s *p, int dfd,
		IN filename_s *name, IN path_s *root)
{
```
固定源码行142–144。

### PI31-A010 · set_nameidata先memset再借入name和dfd
[VERIFIED mykernel/fs/vfs/namei.c::set_nameidata]
```c
		memset(p, 0, sizeof(nameidata_s));

		p->name = name;
		p->dfd = dfd;
```
固定源码行149–152。

### PI31-A011 · set_nameidata赋name、dfd并清空path两指针
[VERIFIED mykernel/fs/vfs/namei.c::set_nameidata]
```c
		p->name = name;
		p->dfd = dfd;
		p->path.mnt = NULL;
		p->path.dentry = NULL;

```
固定源码行151–155。

### PI31-A012 · set_nameidata对非NULL root按值复制
[VERIFIED mykernel/fs/vfs/namei.c::set_nameidata]
```c
		if (root != NULL)
		{
			p->root = *root;
		}
```
固定源码行156–159。

### PI31-A013 · path_init在设置任何字段前从nd.name解引用字符串
[VERIFIED mykernel/fs/vfs/namei.c::path_init]
```c
static const char *path_init(OUT nameidata_s *nd, unsigned flags)
{
	const char *s = nd->name->name;
```
固定源码行547–549。

### PI31-A014 · path_init赋nd.flags并从current.fs取root
[VERIFIED mykernel/fs/vfs/namei.c::path_init]
```c
	nd->flags = flags;

	nd->root = current->fs->root;
```
固定源码行551–553。

### PI31-A015 · path_init按首字节选择root或pwd起点
[VERIFIED mykernel/fs/vfs/namei.c::path_init]
```c
	nd->root = current->fs->root;
	if (s[0] == '/')
		nd->path = nd->root;
	else
		nd->path = current->fs->pwd;
```
固定源码行553–557。

### PI31-A016 · path_init没有改变字符串指针而返回原s
[VERIFIED mykernel/fs/vfs/namei.c::path_init]
```c
	if (s[0] == '/')
		nd->path = nd->root;
	else
		nd->path = current->fs->pwd;

```
固定源码行554–558。

### PI31-A017 · path_init相对路径cwd选择与返回
[VERIFIED mykernel/fs/vfs/namei.c::path_init]
```c
		nd->path = current->fs->pwd;

	return s;
}
```
固定源码行557–560。

### PI31-A018 · LOOKUP_FOLLOW位定义
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_FOLLOW]
```c
	#define LOOKUP_FOLLOW			0x0001	/* follow links at the end */
```
固定源码行39–39。

### PI31-A019 · LOOKUP_DIRECTORY位定义
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_DIRECTORY]
```c
	#define LOOKUP_DIRECTORY		0x0002	/* require a directory */
```
固定源码行40–40。

### PI31-A020 · LOOKUP_EMPTY位定义
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_EMPTY]
```c
	#define LOOKUP_EMPTY			0x4000	/* accept empty path [user_... only] */
```
固定源码行42–42。

### PI31-A021 · LOOKUP_BENEATH位定义
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_BENEATH]
```c
	#define LOOKUP_BENEATH			0x080000 /* No escaping from starting point. */
```
固定源码行62–62。

### PI31-A022 · LOOKUP_IN_ROOT位定义
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_IN_ROOT]
```c
	#define LOOKUP_IN_ROOT			0x100000 /* Treat dirfd as fs root. */
```
固定源码行63–63。

### PI31-A023 · LOOKUP_CACHED位定义
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_CACHED]
```c
	#define LOOKUP_CACHED			0x200000 /* Only do cached lookup */
```
固定源码行64–64。

### PI31-A024 · AT_FDCWD定义为-100（带行内续注释）
[VERIFIED mykernel/include/uapi/linux/fcntl.h::AT_FDCWD]
```c
	#define AT_FDCWD				-100    /* Special value used to indicate
											   openat should use the current
											   working directory. */
```
固定源码行95–97。

### PI31-A025 · do_filp_open局部nameidata及从op读取lookup
[VERIFIED mykernel/fs/vfs/namei.c::do_filp_open]
```c
file_s *do_filp_open(int dfd, IN filename_s * name, const open_flags_s *op)
{
	nameidata_s nd;
	int flags = op->lookup_flags;
	file_s *filp;
```
固定源码行753–757。

### PI31-A026 · do_filp_open用NULL root调用set_nameidata及path_openat
[VERIFIED mykernel/fs/vfs/namei.c::do_filp_open]
```c
	set_nameidata(&nd, dfd, name, NULL);
	filp = path_openat(&nd, op, flags);
	// if (unlikely(filp == ERR_PTR(-ECHILD)))
	// 	filp = path_openat(&nd, op, flags);
	restore_nameidata();
```
固定源码行759–763。

### PI31-A027 · do_filp_open两个实际函数调用实参
[VERIFIED mykernel/fs/vfs/namei.c::do_filp_open]
```c
	set_nameidata(&nd, dfd, name, NULL);
	filp = path_openat(&nd, op, flags);
```
固定源码行759–760。

### PI31-A028 · path_openat声明op/flags与局部file
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
static file_s *path_openat(IN nameidata_s *nd,
		const open_flags_s *op, unsigned flags)
{
	file_s *file;
	int error;
```
固定源码行713–717。

### PI31-A029 · path_openat在path_init之前调用alloc_empty_file及错误门
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	file = alloc_empty_file(op->open_flag);
	if (IS_ERR(file))
		return file;

	const char *s = path_init(nd, flags);
```
固定源码行719–723。

### PI31-A030 · path_openat调用path_init再交给link_path_walk/open_last_lookups
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	const char *s = path_init(nd, flags);
	while (!(error = link_path_walk(s, nd)) &&
		(s = open_last_lookups(nd, flags)) != NULL)
		;
	if (error == 0)
```
固定源码行723–727。

### PI31-A031 · path_openat有条件调用do_open和terminate_walk
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	if (error == 0)
		error = do_open(nd, file, flags);
	terminate_walk(nd);
	if (error == 0)
```
固定源码行727–730。

### PI31-A032 · path_openat error零时返回file
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	terminate_walk(nd);
	if (error == 0)
	{
		// if (file->f_mode & FMODE_OPENED)
			return file;
```
固定源码行729–733。

### PI31-A033 · path_openat错误口调用fput并返回ERR_PTR
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	}
	fput(file);
	return ERR_PTR(error);
}
```
固定源码行735–738。

### PI31-A034 · filename_lookup具有root实参与局部nd
[VERIFIED mykernel/fs/vfs/namei.c::filename_lookup]
```c
int filename_lookup(int dfd, IN filename_s *name, unsigned flags,
				OUT path_s *path, IN path_s *root)
{
	int retval;
	nameidata_s nd;
```
固定源码行597–601。

### PI31-A035 · filename_lookup只检查IS_ERR(name)且实际调用set_nameidata传NULL
[VERIFIED mykernel/fs/vfs/namei.c::filename_lookup]
```c
	nameidata_s nd;
	if (IS_ERR(name))
		return PTR_ERR(name);

	set_nameidata(&nd, dfd, name, NULL);
```
固定源码行601–605。

### PI31-A036 · filename_lookup调用path_lookupat再putname
[VERIFIED mykernel/fs/vfs/namei.c::filename_lookup]
```c
	set_nameidata(&nd, dfd, name, NULL);
	retval = path_lookupat(&nd, flags, path);
	// if (unlikely(retval == -ECHILD))
	//	 retval = path_lookupat(&nd, flags, path);

```
固定源码行605–609。

### PI31-A037 · filename_lookup末尾putname并返回retval
[VERIFIED mykernel/fs/vfs/namei.c::filename_lookup]
```c
	putname(name);
	return retval;
}
```
固定源码行610–612。

### PI31-A038 · path_lookupat先调用path_init
[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
static int path_lookupat(IN nameidata_s *nd, unsigned flags, OUT path_s *path)
{
	const char *s = path_init(nd, flags);
	int err;
```
固定源码行572–575。

### PI31-A039 · path_lookupat链接walk与complete_walk条件
[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	while (!(err = link_path_walk(s, nd)) &&
		   (s = lookup_last(nd)) != NULL)
		;

	if (err == 0)
```
固定源码行577–581。

### PI31-A040 · path_lookupat正常结果按值复制nd.path并清局部
[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	if (err == 0)
	{
		*path = nd->path;
		nd->path.mnt = NULL;
		nd->path.dentry = NULL;
```
固定源码行584–588。

### PI31-A041 · path_lookupat清局部path并terminate_walk后返回
[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
		*path = nd->path;
		nd->path.mnt = NULL;
		nd->path.dentry = NULL;
	}
	terminate_walk(nd);
```
固定源码行586–590。

### PI31-A042 · lookup_last只是调用walk_component
[VERIFIED mykernel/fs/vfs/namei.c::lookup_last]
```c
static inline const char *lookup_last(IN nameidata_s *nd)
{
	return walk_component(nd);
```
固定源码行564–566。

### PI31-A043 · link_path_walk先设LAST_ROOT再IS_ERR(name)
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
static int link_path_walk(IN const char *name, IN OUT nameidata_s *nd)
{
	int err;

	nd->last_type = LAST_ROOT;
```
固定源码行478–482。

### PI31-A044 · link_path_walk错误门后才跳过前导斜线
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
	nd->last_type = LAST_ROOT;
	if (IS_ERR(name))
		return PTR_ERR(name);
	while (*name == '/')
		name++;
```
固定源码行482–486。

### PI31-A045 · link_path_walk跳过前导斜线且在NUL时return0
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
	while (*name == '/')
		name++;
	if (*name == '\0')
	{
		return 0;
```
固定源码行485–489。

### PI31-A046 · link_path_walk原始空串/全斜线的前缀分支
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
	if (*name == '\0')
	{
		return 0;
	}
```
固定源码行487–490。

### PI31-A047 · link_path_walk进入逐分量分析后的边界
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
	/* At this point we know we have a real path component. */
	for (;;)
	{
		int type;
		const char *link;
```
固定源码行492–496。

### PI31-A048 · restore_nameidata实际正文是注释
[VERIFIED mykernel/fs/vfs/namei.c::restore_nameidata]
```c
static void restore_nameidata(void)
{
	// nameidata_s *now = current->nameidata, *old = now->saved;

	// current->nameidata = old;
```
固定源码行163–167。

### PI31-A049 · restore_nameidata剩余正文均为注释
[VERIFIED mykernel/fs/vfs/namei.c::restore_nameidata]
```c
	// if (old)
	// 	old->total_link_count = now->total_link_count;
	// if (now->stack != now->internal)
	// 	kfree(now->stack);
}
```
固定源码行168–172。

### PI31-A050 · terminate_walk实际函数体为空
[VERIFIED mykernel/fs/vfs/namei.c::terminate_walk]
```c
static void terminate_walk(nameidata_s *nd)
{

}
```
固定源码行174–177。

### PI31-A051 · complete_walk当前恒返回0
[VERIFIED mykernel/fs/vfs/namei.c::complete_walk]
```c
static int complete_walk(nameidata_s *nd)
{
	return 0;
}
```
固定源码行189–192。

### PI31-A052 · complete_walk只有在err=0时被调用
[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	if (err == 0)
		err = complete_walk(nd);

```
固定源码行581–583。

### PI31-A053 · 有效name门之后实传root=NULL
[VERIFIED mykernel/fs/vfs/namei.c::filename_lookup]
```c
	if (IS_ERR(name))
		return PTR_ERR(name);

	set_nameidata(&nd, dfd, name, NULL);
	retval = path_lookupat(&nd, flags, path);
```
固定源码行602–606。

### PI31-A054 · 打开链实际restore_nameidata然后return filp
[VERIFIED mykernel/fs/vfs/namei.c::do_filp_open]
```c
	filp = path_openat(&nd, op, flags);
	// if (unlikely(filp == ERR_PTR(-ECHILD)))
	// 	filp = path_openat(&nd, op, flags);
	restore_nameidata();
	return filp;
```
固定源码行760–764。

### PI31-A055 · alloc_empty_file失败直接在path_init之前返回
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	file = alloc_empty_file(op->open_flag);
	if (IS_ERR(file))
		return file;

```
固定源码行719–722。

### PI31-A056 · path_init直接读已借入nd.name与flags
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	const char *s = path_init(nd, flags);
	while (!(error = link_path_walk(s, nd)) &&
		(s = open_last_lookups(nd, flags)) != NULL)
		;
	if (error == 0)
```
固定源码行723–727。

### PI31-A057 · user_path_at_empty先getname并把root=NULL传filename_lookup
[VERIFIED mykernel/fs/vfs/namei.c::user_path_at_empty]
```c
int user_path_at_empty(int dfd, const char *name,
		unsigned flags, OUT path_s *path)
{
	filename_s *fn = getname(name);
	return filename_lookup(dfd, fn, flags, path, NULL);
```
固定源码行743–747。

### PI31-A058 · kern_path使用AT_FDCWD和getname_kernel
[VERIFIED mykernel/fs/vfs/namei.c::kern_path]
```c
int kern_path(const char *name, unsigned int flags, OUT path_s *path)
{
	return filename_lookup(AT_FDCWD, getname_kernel(name),
					flags, path, NULL);
}
```
固定源码行650–654。

### PI31-A059 · do_sys_open向do_sys_openat2传原dfd
[VERIFIED mykernel/fs/vfs/open.c::do_sys_open]
```c
long do_sys_open(int dfd, const char * filename, int flags, umode_t mode)
{
	open_how_s how = build_open_how(flags, mode);
	return do_sys_openat2(dfd, filename, &how);
}
```
固定源码行266–270。

### PI31-A060 · sys_open的dfd实参是0
[VERIFIED mykernel/fs/syscall.c::sys_open]
```c
MYOS_SYSCALL_DEFINE3(open, const char *, filename,
		int, flags, umode_t, mode)
{
	return do_sys_open(0, filename, flags, mode);
}
```
固定源码行38–42。

### PI31-A061 · do_sys_openat2把dfd原值传do_filp_open
[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	fd = get_unused_fd_flags(how->flags);
	if (fd >=0)
	{
		file_s * f = do_filp_open(dfd, tmp, &op);
```
固定源码行251–254。

### PI31-A062 · 执行用途open_flags初始化lookup_flags
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	open_flags_s open_exec_flags = {
		.open_flag = O_LARGEFILE | O_RDONLY | __FMODE_EXEC,
		.acc_mode = MAY_EXEC,
		.intent = LOOKUP_OPEN,
		.lookup_flags = LOOKUP_FOLLOW,
```
固定源码行558–562。

### PI31-A063 · AT_*标志条件可影响lookup_flags
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	if ((flags & ~(AT_SYMLINK_NOFOLLOW | AT_EMPTY_PATH)) != 0)
		return ERR_PTR(-EINVAL);
	if (flags & AT_SYMLINK_NOFOLLOW)
		open_exec_flags.lookup_flags &= ~LOOKUP_FOLLOW;
	if (flags & AT_EMPTY_PATH)
```
固定源码行565–569。

### PI31-A064 · LOOKUP_EMPTY由AT_EMPTY_PATH条件写入
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	if (flags & AT_EMPTY_PATH)
		open_exec_flags.lookup_flags |= LOOKUP_EMPTY;
```
固定源码行569–570。

### PI31-A065 · do_open_execat直接传fd/name/op给do_filp_open
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	file = do_filp_open(fd, name, &open_exec_flags);
	if (IS_ERR(file))
		goto out;
```
固定源码行572–574。

### PI31-A066 · open_exec取内核filename后以AT_FDCWD打开
[VERIFIED mykernel/sched/forkexec/exec.c::open_exec]
```c
file_s *open_exec(const char *name)
{
	filename_s *filename = getname_kernel(name);
	file_s *f = ERR_CAST(filename);

```
固定源码行604–608。

### PI31-A067 · open_exec传0打开并putname
[VERIFIED mykernel/sched/forkexec/exec.c::open_exec]
```c
	if (!IS_ERR(filename)) {
		f = do_open_execat(AT_FDCWD, filename, 0);
		putname(filename);
	}
	return f;
```
固定源码行609–613。

### PI31-A068 · open_last_lookups根据last_type处理特殊末分量
[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
static const char *open_last_lookups(IN nameidata_s *nd, int open_flag)
{
	dentry_s *dentry, *dir = nd->path.dentry;
	const char *res;

```
固定源码行662–666。

### PI31-A069 · open_last_lookups检查O_CREAT
[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
	}

	if (!(open_flag & O_CREAT))
	{
		dentry = __d_lookup(dir, &nd->last);
```
固定源码行670–674。

### PI31-A070 · path_lookupat结果交接后局部清理调用
[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	if (err == 0)
	{
		*path = nd->path;
		nd->path.mnt = NULL;
		nd->path.dentry = NULL;
```
固定源码行584–588。

### PI31-A071 · link_path_walk中续路径斜线/NUL时有另一返回
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
		while (*name == '/')
			name++;

		if (*name == 0)
			return -ENOERR;
```
固定源码行527–531。

### PI31-A072 · link_path_walk对末组件返回-ENOERR或继续walk
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
		if (*name == 0)
			return -ENOERR;
		else
		{
			link = walk_component(nd);
```
固定源码行530–534。

### PI31-A073 · open路径按值传flags给path_init
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	const char *s = path_init(nd, flags);
	while (!(error = link_path_walk(s, nd)) &&
		(s = open_last_lookups(nd, flags)) != NULL)
```
固定源码行723–725。

### PI31-A074 · lookup路径按值传flags给path_init
[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
static int path_lookupat(IN nameidata_s *nd, unsigned flags, OUT path_s *path)
{
	const char *s = path_init(nd, flags);
	int err;

```
固定源码行572–576。

### PI31-A075 · IS_ERR错误编码判断（定义层）
[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR]
```c
		static inline bool
		IS_ERR(const void *ptr) {
			return IS_ERR_VALUE((size_t)ptr);
		}
```
固定源码行36–39。

### PI31-A076 · PTR_ERR将错误指针转换long（定义层）
[VERIFIED mykernel/include/linux/kernel/err.h::PTR_ERR]
```c
		static inline long
		PTR_ERR(const void *ptr) {
			return (long) ptr;
		}
```
固定源码行31–34。

### PI31-A077 · set_nameidata对name/dfd/path的独立赋值
[VERIFIED mykernel/fs/vfs/namei.c::set_nameidata]
```c
		p->name = name;
		p->dfd = dfd;
		p->path.mnt = NULL;
		p->path.dentry = NULL;

```
固定源码行151–155。

### PI31-A078 · path_init的固定起点选择不读取nd.dfd
[VERIFIED mykernel/fs/vfs/namei.c::path_init]
```c
	const char *s = nd->name->name;

	nd->flags = flags;

	nd->root = current->fs->root;
```
固定源码行549–553。

### PI31-A079 · complete_walk名义完成但活动仅return0
[VERIFIED mykernel/fs/vfs/namei.c::complete_walk]
```c
static int complete_walk(nameidata_s *nd)
{
	return 0;
}
```
固定源码行189–192。

### PI31-A080 · path_init入口与读取name字符串的有效对象前提
[VERIFIED mykernel/fs/vfs/namei.c::path_init]
```c
static const char *path_init(OUT nameidata_s *nd, unsigned flags)
{
	const char *s = nd->name->name;

	nd->flags = flags;
```
固定源码行547–551。

## 实际范围、重复回读与未执行

本次源码直接证据来自10个冻结time文件；证据重点限定在nameidata、set_nameidata/path_init、两个直接调用链、link_path_walk入口和三处结束帮助函数。相同符号中多段引文均严格在该函数/结构体定义范围；没有把定义外Linux原型注释、头文件include或其它宏冒充函数证据。所有输入均须满足有效对象与正常上游返回的前提。

不运行仓库脚本、C/ASM、预处理/编译、QEMU/内核/硬件、真实路径或块设备、外部libc。除直接范围以外的完整VFS、权限、mount/symlink/dotdot路径、fs_struct引用/锁、CPU异常恢复等均未做。运行证据NOT_RUN。
