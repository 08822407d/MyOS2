---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-LAST-COMPONENT-32
record_type: fixed_time_verbatim_definition_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
date: 2026-11-03
source_ref: time
source_pin_short12: a039d9803ade
read_channel: connector
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---
# 第32批：末分量真实源码证据

下列每个固定path::symbol直接引文标签只引用固定`time@a039d9803ade`中一个真实函数、宏或结构体定义，原文1–5行，含缩进和内部空行。标题限本片段实际出现的语句；“不存在某个赋值/返回”等负面结论必须结合已读完整函数，而不能由单个短引文推断。C语言类型、有效对象前提、是否运行和物理结果不由源码存在自动证明。

### LC32-A001 · nameidata包含path/last/root
[VERIFIED mykernel/fs/vfs/namei.c::nameidata_s]
```c
typedef struct nameidata
{
	path_s path;
	qstr_s last;
	path_s root;
```
固定源码行36–40。

### LC32-A002 · nameidata的last_type/flags/name字段
[VERIFIED mykernel/fs/vfs/namei.c::nameidata_s]
```c
	int last_type;
	unsigned flags, state;
	unsigned depth;
	int total_link_count;
	filename_s *name;
```
固定源码行43–47。

### LC32-A003 · qstr_s中len/name字段
[VERIFIED mykernel/include/linux/fs/dcache.h::qstr_s]
```c
	typedef struct qstr {
		size_t				len;
		const unsigned char	*name;
	} qstr_s;
```
固定源码行47–50。

### LC32-A004 · LAST_NORM枚举常量
[VERIFIED mykernel/include/linux/fs/namei.h::LAST_NORM]
```c
		LAST_NORM,
```
固定源码行32–32。
### LC32-A005 · ENOERR值为0
[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOERR]
```c
	#define	ENOERR		0	/*  */
```
固定源码行5–5。

### LC32-A006 · ENOENT值为2
[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOENT]
```c
	#define	ENOENT		2	/* No such file or directory */
```
固定源码行8–8。

### LC32-A007 · MAX_ERRNO值为4095
[VERIFIED mykernel/include/linux/kernel/err.h::MAX_ERRNO]
```c
	#define MAX_ERRNO	4095
```
固定源码行15–15。

### LC32-A008 · IS_ERR_VALUE数值范围定义
[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR_VALUE]
```c
	#	define IS_ERR_VALUE(x) (								\
					(size_t)(void *)(x) >= (size_t)-MAX_ERRNO	\
				)
```
固定源码行18–20。

### LC32-A009 · ERR_PTR把long编码成指针
[VERIFIED mykernel/include/linux/kernel/err.h::ERR_PTR]
```c
		static inline void *
		ERR_PTR(long error) {
			return (void *) error;
		}
```
固定源码行26–29。

### LC32-A010 · PTR_ERR从指针取long
[VERIFIED mykernel/include/linux/kernel/err.h::PTR_ERR]
```c
		static inline long
		PTR_ERR(const void *ptr) {
			return (long) ptr;
		}
```
固定源码行31–34。

### LC32-A011 · IS_ERR调用错误数值判定
[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR]
```c
		static inline bool
		IS_ERR(const void *ptr) {
			return IS_ERR_VALUE((size_t)ptr);
		}
```
固定源码行36–39。

### LC32-A012 · ERR_CAST只做类型转换
[VERIFIED mykernel/include/linux/kernel/err.h::ERR_CAST]
```c
		static inline void *
		ERR_CAST(const void *ptr) {
			/* cast away the const */
			return (void *) ptr;
		}
```
固定源码行59–63。

### LC32-A013 · O_CREAT八进制0100
[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_CREAT]
```c
	#	define O_CREAT			00000100	/* not fcntl */
```
固定源码行24–24。

### LC32-A014 · LOOKUP_RCU为0x0040
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_RCU]
```c
	#define LOOKUP_RCU				0x0040	/* RCU pathwalk mode; semi-internal */
```
固定源码行47–47。

### LC32-A015 · LOOKUP_CREATE为0x0200
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_CREATE]
```c
	#define LOOKUP_CREATE			0x0200	/* ... in object creation */
```
固定源码行51–51。

### LC32-A016 · LOOKUP_OPEN为0x0100
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_OPEN]
```c
	#define LOOKUP_OPEN				0x0100	/* ... in open */
```
固定源码行50–50。

### LC32-A017 · LOOKUP_EXCL为0x0400
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_EXCL]
```c
	#define LOOKUP_EXCL				0x0400	/* ... in exclusive creation */
```
固定源码行52–52。

### LC32-A018 · LOOKUP_FOLLOW为0x0001
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_FOLLOW]
```c
	#define LOOKUP_FOLLOW			0x0001	/* follow links at the end */
```
固定源码行39–39。

### LC32-A019 · LOOKUP_EMPTY为0x4000
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_EMPTY]
```c
	#define LOOKUP_EMPTY			0x4000	/* accept empty path [user_... only] */
```
固定源码行42–42。

### LC32-A020 · link_path_walk整数返回并先设LAST_ROOT
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
static int link_path_walk(IN const char *name, IN OUT nameidata_s *nd)
{
	int err;

	nd->last_type = LAST_ROOT;
```
固定源码行478–482。

### LC32-A021 · 先判断IS_ERR再跳过斜线
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
	nd->last_type = LAST_ROOT;
	if (IS_ERR(name))
		return PTR_ERR(name);
	while (*name == '/')
		name++;
```
固定源码行482–486。

### LC32-A022 · 跳过前导斜线后见NUL返回0
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
	while (*name == '/')
		name++;
	if (*name == '\0')
	{
		return 0;
```
固定源码行485–489。

### LC32-A023 · 按非斜线字节循环计算长度
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
		// compute length of current dirname
		int i = 0;
		while (name[i] != 0 && name[i] != '/')
			i++;

```
固定源码行498–502。

### LC32-A024 · LAST_NORM初值及点开头分派
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
		type = LAST_NORM;
		// detect . and .. dir
		if (name[0] == '.')
			switch (i)
			{
```
固定源码行503–507。

### LC32-A025 · 长度二双点设LAST_DOTDOT
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
			case 2:
				if (name[1] == '.')
				{
					type = LAST_DOTDOT;
				}
```
固定源码行508–512。

### LC32-A026 · 长度一单点设LAST_DOT
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
				break;
			case 1:
				type = LAST_DOT;
			}
```
固定源码行513–516。

### LC32-A027 · 连续写nd.last.len/name/type
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
		nd->last.len = i;
		nd->last.name = name;
		nd->last_type = type;
```
固定源码行518–520。

### LC32-A028 · 跳过分量之后的连续斜线
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
		 * If it wasn't NUL, we know it was '/'. Skip that
		 * slash, and continue until no more slashes.
		 */
		while (*name == '/')
			name++;
```
固定源码行524–528。

### LC32-A029 · 末组件遇NUL时返回-ENOERR
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
		if (*name == 0)
			return -ENOERR;
		else
		{
			link = walk_component(nd);
```
固定源码行530–534。

### LC32-A030 · 中间组件调用walk_component并判断错误
[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
		{
			link = walk_component(nd);
			if (IS_ERR(link))
				return PTR_ERR(link);
		}
```
固定源码行533–537。

### LC32-A031 · lookup_last直接返回walk_component
[VERIFIED mykernel/fs/vfs/namei.c::lookup_last]
```c
static inline const char *lookup_last(IN nameidata_s *nd)
{
	return walk_component(nd);
}
```
固定源码行564–567。

### LC32-A032 · walk_component返回const char*并读取parent
[VERIFIED mykernel/fs/vfs/namei.c::walk_component]
```c
static const char *walk_component(IN nameidata_s *nd)
{
	dentry_s *dentry, *parent = nd->path.dentry;
	inode_s *inode;
```
固定源码行429–432。

### LC32-A033 · 非普通LAST类型交handle_dots
[VERIFIED mykernel/fs/vfs/namei.c::walk_component]
```c
	if (nd->last_type != LAST_NORM)
	{
		return handle_dots(nd);
	}
```
固定源码行439–442。

### LC32-A034 · 普通分量快查dentry并判IS_ERR
[VERIFIED mykernel/fs/vfs/namei.c::walk_component]
```c
		dentry = __d_lookup(parent, &nd->last);
	// }
	if (IS_ERR(dentry))
		return ERR_CAST(dentry);
```
固定源码行448–451。

### LC32-A035 · 快查NULL时转入慢查分支
[VERIFIED mykernel/fs/vfs/namei.c::walk_component]
```c
	if (dentry == NULL)
	{
		// linux call stack :
		//	static struct dentry *lookup_slow(const struct qstr *name,
		//			  struct dentry *dir, unsigned int flags)
```
固定源码行453–457。

### LC32-A036 · 慢查错误指针通过ERR_CAST返回
[VERIFIED mykernel/fs/vfs/namei.c::walk_component]
```c
			dentry = __lookup_slow(&nd->last, parent, nd->flags);
		// }
		if (IS_ERR(dentry))
			return ERR_CAST(dentry);
```
固定源码行459–462。

### LC32-A037 · 普通分量返回step_into
[VERIFIED mykernel/fs/vfs/namei.c::walk_component]
```c
	return step_into(nd, dentry);
}
```
固定源码行465–466。

### LC32-A038 · 仅LAST_DOTDOT进入点点处理
[VERIFIED mykernel/fs/vfs/namei.c::handle_dots]
```c
static const char *handle_dots(IN nameidata_s *nd)
{
	if (nd->last_type == LAST_DOTDOT)
	{
		const char *error = NULL;
```
固定源码行407–411。

### LC32-A039 · 跟随dotdot父目录后检查ERR/NULL
[VERIFIED mykernel/fs/vfs/namei.c::handle_dots]
```c
		parent = follow_dotdot(nd);
		if (IS_ERR(parent))
			return ERR_CAST(parent);
		if (parent == NULL)
			error = step_into(nd, nd->path.dentry);
```
固定源码行414–418。

### LC32-A040 · 按parent选step_into并检查error
[VERIFIED mykernel/fs/vfs/namei.c::handle_dots]
```c
			error = step_into(nd, nd->path.dentry);
		else
			error = step_into(nd, parent);
		if (error)
			return error;
```
固定源码行418–422。

### LC32-A041 · 条件返回error否则返回NULL
[VERIFIED mykernel/fs/vfs/namei.c::handle_dots]
```c
		if (error)
			return error;
	}
	return NULL;
}
```
固定源码行421–425。

### LC32-A042 · follow_dotdot有parent和NULL出口
[VERIFIED mykernel/fs/vfs/namei.c::follow_dotdot]
```c
	parent = old->d_parent;

	return parent;
in_root:
	return NULL;
```
固定源码行398–402。

### LC32-A043 · step_into调用handle_mounts并把负err编码
[VERIFIED mykernel/fs/vfs/namei.c::step_into]
```c
{
	path_s path;
	int err = handle_mounts(nd, dentry, &path);
	if (err < 0)
		return ERR_PTR(err);
```
固定源码行294–298。

### LC32-A044 · step_into负返回短路或写nd.path
[VERIFIED mykernel/fs/vfs/namei.c::step_into]
```c
	int err = handle_mounts(nd, dentry, &path);
	if (err < 0)
		return ERR_PTR(err);

	nd->path = path;
```
固定源码行296–300。

### LC32-A045 · step_into成功分支ERR_PTR(-ENOERR)
[VERIFIED mykernel/fs/vfs/namei.c::step_into]
```c
	nd->path = path;

	return ERR_PTR(-ENOERR);
}
```
固定源码行300–303。

### LC32-A046 · handle_mounts写path并转交traverse_mounts
[VERIFIED mykernel/fs/vfs/namei.c::handle_mounts]
```c
{
	path->mnt = nd->path.mnt;
	path->dentry = dentry;

	return traverse_mounts(path);
```
固定源码行250–254。

### LC32-A047 · __lookup_slow仅作有类型不透明边界
[VERIFIED mykernel/fs/vfs/namei.c::__lookup_slow]
```c
static dentry_s *__lookup_slow(IN qstr_s *name,
		IN dentry_s *dir, unsigned flags)
{
```
固定源码行308–310。

### LC32-A048 · open_last_lookups中dentry仅声明未赋
[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
static const char *open_last_lookups(IN nameidata_s *nd, int open_flag)
{
	dentry_s *dentry, *dir = nd->path.dentry;
	const char *res;
```
固定源码行662–665。

### LC32-A049 · 非LAST_NORM直接handle_dots
[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
	if (nd->last_type != LAST_NORM)
	{
		return handle_dots(nd);
	}
```
固定源码行667–670。

### LC32-A050 · 只有未命中O_CREAT才给dentry赋值
[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
	if (!(open_flag & O_CREAT))
	{
		dentry = __d_lookup(dir, &nd->last);
		if (dentry == NULL)
```
固定源码行672–675。

### LC32-A051 · 快查NULL后慢查dentry赋值
[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
		dentry = __d_lookup(dir, &nd->last);
		if (dentry == NULL)
		{
			dentry = __lookup_slow(&nd->last, dir, nd->flags);
		}
```
固定源码行674–678。

### LC32-A052 · dentry错误门和goto finish_lookup
[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
		if (IS_ERR(dentry))
			return ERR_CAST(dentry);
		if (dentry)
			goto finish_lookup;
	}
```
固定源码行680–684。

### LC32-A053 · O_CREAT的else/lookup_open路径已注释
[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
	// else
	// {
	// 	dentry = lookup_open(nd, file, op);
	// }
```
固定源码行685–688。

### LC32-A054 · finish_lookup将dentry交step_into
[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
finish_lookup:
	res = step_into(nd, dentry);
	return res;
```
固定源码行690–692。

### LC32-A055 · path_lookupat先path_init再声明err
[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
static int path_lookupat(IN nameidata_s *nd, unsigned flags, OUT path_s *path)
{
	const char *s = path_init(nd, flags);
	int err;
```
固定源码行572–575。

### LC32-A056 · lookup外层while写整数err和指针s
[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	while (!(err = link_path_walk(s, nd)) &&
		   (s = lookup_last(nd)) != NULL)
		;
```
固定源码行577–579。

### LC32-A057 · 只有err0调用complete_walk
[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	if (err == 0)
		err = complete_walk(nd);
```
固定源码行581–582。

### LC32-A058 · err0条件输出path并清局部字段
[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	if (err == 0)
	{
		*path = nd->path;
		nd->path.mnt = NULL;
		nd->path.dentry = NULL;
```
固定源码行584–588。

### LC32-A059 · lookup尾部terminate_walk返回err
[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	terminate_walk(nd);
	return err;
}
```
固定源码行590–592。

### LC32-A060 · path_openat返回file_s*并声明error
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
static file_s *path_openat(IN nameidata_s *nd,
		const open_flags_s *op, unsigned flags)
{
	file_s *file;
	int error;
```
固定源码行713–717。

### LC32-A061 · 先alloc_empty_file再path_init
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	file = alloc_empty_file(op->open_flag);
	if (IS_ERR(file))
		return file;

	const char *s = path_init(nd, flags);
```
固定源码行719–723。

### LC32-A062 · open外层while写整数error及指针s
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	const char *s = path_init(nd, flags);
	while (!(error = link_path_walk(s, nd)) &&
		(s = open_last_lookups(nd, flags)) != NULL)
		;
```
固定源码行723–726。

### LC32-A063 · error0调用do_open并terminate_walk
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	if (error == 0)
		error = do_open(nd, file, flags);
	terminate_walk(nd);
```
固定源码行727–729。

### LC32-A064 · error0有返回file指针分支
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	if (error == 0)
	{
		// if (file->f_mode & FMODE_OPENED)
			return file;
```
固定源码行730–733。

### LC32-A065 · 失败时fput及ERR_PTR(error)
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	}
	fput(file);
	return ERR_PTR(error);
```
固定源码行735–737。

### LC32-A066 · do_filp_open读取op.lookup_flags
[VERIFIED mykernel/fs/vfs/namei.c::do_filp_open]
```c
file_s *do_filp_open(int dfd, IN filename_s * name, const open_flags_s *op)
{
	nameidata_s nd;
	int flags = op->lookup_flags;
	file_s *filp;
```
固定源码行753–757。

### LC32-A067 · do_filp_open实传flags给path_openat
[VERIFIED mykernel/fs/vfs/namei.c::do_filp_open]
```c
	set_nameidata(&nd, dfd, name, NULL);
	filp = path_openat(&nd, op, flags);
	// if (unlikely(filp == ERR_PTR(-ECHILD)))
	// 	filp = path_openat(&nd, op, flags);
	restore_nameidata();
```
固定源码行759–763。

### LC32-A068 · open_flags_s的open_flag/mode/acc_mode
[VERIFIED mykernel/include/linux/fs/internal.h::open_flags_s]
```c
typedef struct open_flags {
	int open_flag;
	umode_t mode;
	int acc_mode;
	int intent;
```
固定源码行91–95。

### LC32-A069 · open_flags_s的intent/lookup_flags
[VERIFIED mykernel/include/linux/fs/internal.h::open_flags_s]
```c
	int intent;
	int lookup_flags;
} open_flags_s;
```
固定源码行95–97。

### LC32-A070 · O_CREAT仅添加op.intent中的CREATE/EXCL
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (flags & O_CREAT) {
		op->intent |= LOOKUP_CREATE;
		if (flags & O_EXCL) {
			op->intent |= LOOKUP_EXCL;
			flags |= O_NOFOLLOW;
```
固定源码行149–153。

### LC32-A071 · flags到lookup_flags的DIRECTORY/FOLLOW映射
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (flags & O_DIRECTORY)
		lookup_flags |= LOOKUP_DIRECTORY;
	if (!(flags & O_NOFOLLOW))
		lookup_flags |= LOOKUP_FOLLOW;
```
固定源码行157–160。

### LC32-A072 · RESOLVE_BENEATH/IN_ROOT到lookup_flags
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (how->resolve & RESOLVE_BENEATH)
		lookup_flags |= LOOKUP_BENEATH;
	if (how->resolve & RESOLVE_IN_ROOT)
		lookup_flags |= LOOKUP_IN_ROOT;
```
固定源码行168–171。

### LC32-A073 · CACHED分支对lookup_flags的条件设置
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (how->resolve & RESOLVE_CACHED) {
		/* Don't bother even trying for create/truncate/tmpfile open */
		if (flags & (O_TRUNC | O_CREAT | O_TMPFILE))
			return -EAGAIN;
		lookup_flags |= LOOKUP_CACHED;
```
固定源码行172–176。

### LC32-A074 · 最后写op.lookup_flags并return0
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	op->lookup_flags = lookup_flags;
	return 0;
```
固定源码行179–180。

### LC32-A075 · user-open先建立op及build_open_flags返回
[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	open_flags_s op;
	int fd = build_open_flags(how, &op);
	filename_s *tmp;
```
固定源码行243–245。

### LC32-A076 · user-open实传op给do_filp_open
[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	fd = get_unused_fd_flags(how->flags);
	if (fd >=0)
	{
		file_s * f = do_filp_open(dfd, tmp, &op);
```
固定源码行251–254。

### LC32-A077 · exec用open_flags四字段赋值
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	open_flags_s open_exec_flags = {
		.open_flag = O_LARGEFILE | O_RDONLY | __FMODE_EXEC,
		.acc_mode = MAY_EXEC,
		.intent = LOOKUP_OPEN,
		.lookup_flags = LOOKUP_FOLLOW,
```
固定源码行558–562。

### LC32-A078 · AT标志可能修改lookup_flags
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	if (flags & AT_SYMLINK_NOFOLLOW)
		open_exec_flags.lookup_flags &= ~LOOKUP_FOLLOW;
	if (flags & AT_EMPTY_PATH)
		open_exec_flags.lookup_flags |= LOOKUP_EMPTY;
```
固定源码行567–570。

### LC32-A079 · exec路径向do_filp_open传op
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	file = do_filp_open(fd, name, &open_exec_flags);
	if (IS_ERR(file))
		goto out;
```
固定源码行572–574。

### LC32-A080 · restore_nameidata前段只有注释
[VERIFIED mykernel/fs/vfs/namei.c::restore_nameidata]
```c
static void restore_nameidata(void)
{
	// nameidata_s *now = current->nameidata, *old = now->saved;

	// current->nameidata = old;
```
固定源码行163–167。

### LC32-A081 · restore_nameidata后段只有注释
[VERIFIED mykernel/fs/vfs/namei.c::restore_nameidata]
```c
	// if (old)
	// 	old->total_link_count = now->total_link_count;
	// if (now->stack != now->internal)
	// 	kfree(now->stack);
}
```
固定源码行168–172。

### LC32-A082 · terminate_walk活动函数体为空
[VERIFIED mykernel/fs/vfs/namei.c::terminate_walk]
```c
static void terminate_walk(nameidata_s *nd)
{

}
```
固定源码行174–177。

### LC32-A083 · complete_walk活动return0
[VERIFIED mykernel/fs/vfs/namei.c::complete_walk]
```c
static int complete_walk(nameidata_s *nd)
{
	return 0;
}
```
固定源码行189–192。

### LC32-A084 · path_init取得路径字符串再保存flags
[VERIFIED mykernel/fs/vfs/namei.c::path_init]
```c
static const char *path_init(OUT nameidata_s *nd, unsigned flags)
{
	const char *s = nd->name->name;

	nd->flags = flags;
```
固定源码行547–551。

### LC32-A085 · path_init按首字节选择root/pwd
[VERIFIED mykernel/fs/vfs/namei.c::path_init]
```c
	nd->root = current->fs->root;
	if (s[0] == '/')
		nd->path = nd->root;
	else
		nd->path = current->fs->pwd;
```
固定源码行553–557。

### LC32-A086 · do_open调用vfs_open后到函数末尾无return
[VERIFIED mykernel/fs/vfs/namei.c::do_open]
```c
	// if ((nd->flags & LOOKUP_DIRECTORY) && !d_can_lookup(nd->path.dentry))
	// 	return -ENOTDIR;

	vfs_open(&nd->path, file);
}
```
固定源码行703–707。


### LC32-A087 · LAST_ROOT枚举常量
[VERIFIED mykernel/include/linux/fs/namei.h::LAST_ROOT]
```c
		LAST_ROOT,
```
固定源码行33–33。

### LC32-A088 · LAST_DOT枚举常量
[VERIFIED mykernel/include/linux/fs/namei.h::LAST_DOT]
```c
		LAST_DOT,
```
固定源码行34–34。

### LC32-A089 · LAST_DOTDOT枚举常量
[VERIFIED mykernel/include/linux/fs/namei.h::LAST_DOTDOT]
```c
		LAST_DOTDOT
```
固定源码行35–35。
## 取得方式和边界

本批9份固定time源码参与直接引文，包括`qstr_s`成员、LAST枚举、O_CREAT和LOOKUP位、`ENOERR`直接定义、errno指针辅助及调用/返回生产者。另核`linux/lib/errno.h → uapi/linux/errno.h → uapi/asm-generic/errno.h → errno-base.h`包含链，include只能作来源导航，不能作为ENOERR宏定义的VERIFIED替代。`step_into`只到handle_mounts返回及ERR_PTR处理；`__d_lookup`、`__lookup_slow`、follow_dotdot/mount全链保持不透明。

没有运行C/ASM、仓库脚本、预处理/编译、QEMU/硬件、真实路径或VFS/权限/文件引用测试；runtime=NOT_RUN。
