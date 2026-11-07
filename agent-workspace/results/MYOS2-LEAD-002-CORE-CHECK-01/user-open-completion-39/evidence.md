---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-OPEN-COMPLETION-39
record_type: fixed_time_direct_source_evidence
date: 2026-11-05
base_snapshot: time
source_pin_short12: a039d9803ade
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
runtime_evidence: NOT_RUN
---

# 第39批固定源码直接引文

本轮每段是固定time中**单一函数、结构体、对象表或单宏定义体**的1–5行逐字摘录。相邻重叠引文可能支持同一事实，69个标签不等于69项独立事实或漏洞。含注释行只作所在活动函数的上下文，不用注释当执行。函数缺少return等全函数否定结论须合读已读取完整活动定义；本文件不构成编译或运行证据。

### OC39-A001｜__alloc_file 固定第39–43行

[VERIFIED mykernel/fs/vfs/file_table.c::__alloc_file]
```c
static file_s *__alloc_file(int flags)
{
	file_s *f;
	int error;

```

### OC39-A002｜__alloc_file 固定第44–46行

[VERIFIED mykernel/fs/vfs/file_table.c::__alloc_file]
```c
	f = kmem_cache_zalloc(filp_cachep, GFP_KERNEL);
	if (unlikely(!f))
		return ERR_PTR(-ENOMEM);
```

### OC39-A003｜__alloc_file 固定第55–59行

[VERIFIED mykernel/fs/vfs/file_table.c::__alloc_file]
```c
	atomic_long_set(&f->f_count, 1);
	// rwlock_init(&f->f_owner.lock);
	spin_lock_init(&f->f_lock);
	// mutex_init(&f->f_pos_lock);
	f->f_flags = flags;
```

### OC39-A004｜__alloc_file 固定第59–63行

[VERIFIED mykernel/fs/vfs/file_table.c::__alloc_file]
```c
	f->f_flags = flags;
	f->f_mode = OPEN_FMODE(flags);
	/* f->f_version: 0 */

	return f;
```

### OC39-A005｜alloc_empty_file 固定第76–80行

[VERIFIED mykernel/fs/vfs/file_table.c::alloc_empty_file]
```c
file_s *alloc_empty_file(int flags)
{
	static long old_max;
	file_s *f;

```

### OC39-A006｜alloc_empty_file 固定第93–97行

[VERIFIED mykernel/fs/vfs/file_table.c::alloc_empty_file]
```c
	f = __alloc_file(flags);
	// if (!IS_ERR(f))
	// 	percpu_counter_inc(&nr_files);

	return f;
```

### OC39-A007｜alloc_empty_file 固定第101–105行

[VERIFIED mykernel/fs/vfs/file_table.c::alloc_empty_file]
```c
	// if (get_nr_files() > old_max) {
	// 	pr_info("VFS: file-max limit %lu reached\n", get_max_files());
	// 	old_max = get_nr_files();
	// }
	return ERR_PTR(-ENFILE);
```

### OC39-A008｜fput 固定第108–112行

[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
void fput(file_s *file)
{
	if (atomic_long_dec_and_test(&file->f_count)) {
		task_s *task = current;

```

### OC39-A009｜fput 固定第125–129行

[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
		// 	schedule_delayed_work(&delayed_fput_work, 1);

		kmem_cache_free(filp_cachep, file);
	}
}
```

### OC39-A010｜files_init 固定第132–136行

[VERIFIED mykernel/fs/vfs/file_table.c::files_init]
```c
void __init files_init(void)
{
	filp_cachep = kmem_cache_create("filp", sizeof(file_s), 0,
			SLAB_HWCACHE_ALIGN | SLAB_PANIC | SLAB_ACCOUNT, NULL);
	// percpu_counter_init(&nr_files, 0, GFP_KERNEL);
```

### OC39-A011｜do_open 固定第701–705行

[VERIFIED mykernel/fs/vfs/namei.c::do_open]
```c
static int do_open(IN nameidata_s *nd, OUT file_s *file, int open_flag)
{
	// if ((nd->flags & LOOKUP_DIRECTORY) && !d_can_lookup(nd->path.dentry))
	// 	return -ENOTDIR;

```

### OC39-A012｜do_open 固定第703–707行

[VERIFIED mykernel/fs/vfs/namei.c::do_open]
```c
	// if ((nd->flags & LOOKUP_DIRECTORY) && !d_can_lookup(nd->path.dentry))
	// 	return -ENOTDIR;

	vfs_open(&nd->path, file);
}
```

### OC39-A013｜path_openat 固定第713–717行

[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
static file_s *path_openat(IN nameidata_s *nd,
		const open_flags_s *op, unsigned flags)
{
	file_s *file;
	int error;
```

### OC39-A014｜path_openat 固定第719–723行

[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	file = alloc_empty_file(op->open_flag);
	if (IS_ERR(file))
		return file;

	const char *s = path_init(nd, flags);
```

### OC39-A015｜path_openat 固定第723–726行

[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	const char *s = path_init(nd, flags);
	while (!(error = link_path_walk(s, nd)) &&
		(s = open_last_lookups(nd, flags)) != NULL)
		;
```

### OC39-A016｜path_openat 固定第727–731行

[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	if (error == 0)
		error = do_open(nd, file, flags);
	terminate_walk(nd);
	if (error == 0)
	{
```

### OC39-A017｜path_openat 固定第730–734行

[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	if (error == 0)
	{
		// if (file->f_mode & FMODE_OPENED)
			return file;
		// error = -EINVAL;
```

### OC39-A018｜path_openat 固定第735–738行

[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	}
	fput(file);
	return ERR_PTR(error);
}
```

### OC39-A019｜do_filp_open 固定第753–757行

[VERIFIED mykernel/fs/vfs/namei.c::do_filp_open]
```c
file_s *do_filp_open(int dfd, IN filename_s * name, const open_flags_s *op)
{
	nameidata_s nd;
	int flags = op->lookup_flags;
	file_s *filp;
```

### OC39-A020｜do_filp_open 固定第760–764行

[VERIFIED mykernel/fs/vfs/namei.c::do_filp_open]
```c
	filp = path_openat(&nd, op, flags);
	// if (unlikely(filp == ERR_PTR(-ECHILD)))
	// 	filp = path_openat(&nd, op, flags);
	restore_nameidata();
	return filp;
```

### OC39-A021｜open_last_lookups 固定第662–666行

[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
static const char *open_last_lookups(IN nameidata_s *nd, int open_flag)
{
	dentry_s *dentry, *dir = nd->path.dentry;
	const char *res;

```

### OC39-A022｜open_last_lookups 固定第672–676行

[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
	if (!(open_flag & O_CREAT))
	{
		dentry = __d_lookup(dir, &nd->last);
		if (dentry == NULL)
		{
```

### OC39-A023｜open_last_lookups 固定第679–683行

[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c

		if (IS_ERR(dentry))
			return ERR_CAST(dentry);
		if (dentry)
			goto finish_lookup;
```

### OC39-A024｜open_last_lookups 固定第690–693行

[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
finish_lookup:
	res = step_into(nd, dentry);
	return res;
}
```

### OC39-A025｜terminate_walk 固定第174–177行

[VERIFIED mykernel/fs/vfs/namei.c::terminate_walk]
```c
static void terminate_walk(nameidata_s *nd)
{

}
```

### OC39-A026｜myos_do_dentry_open 固定第47–51行

[VERIFIED mykernel/fs/vfs/open.c::myos_do_dentry_open]
```c
{
	ulong error = -ENOERR;
	path_get(&f->f_path);
	f->f_inode = inode;
	f->f_mapping = inode->i_mapping;
```

### OC39-A027｜myos_do_dentry_open 固定第53–55行

[VERIFIED mykernel/fs/vfs/open.c::myos_do_dentry_open]
```c
	f->f_op = inode->i_fop;
	if(f->f_op && f->f_op->open)
		error = f->f_op->open(f->f_path.dentry->d_inode, f);
```

### OC39-A028｜myos_do_dentry_open 固定第57–60行

[VERIFIED mykernel/fs/vfs/open.c::myos_do_dentry_open]
```c
	if(f->f_mode & O_TRUNC)
	{
		f->f_path.dentry->d_inode->i_size = 0;
	}
```

### OC39-A029｜myos_do_dentry_open 固定第61–65行

[VERIFIED mykernel/fs/vfs/open.c::myos_do_dentry_open]
```c
	if(f->f_mode & O_APPEND)
	{
		f->f_pos = f->f_path.dentry->d_inode->i_size;
	}
}
```

### OC39-A030｜vfs_open 固定第301–305行

[VERIFIED mykernel/fs/vfs/open.c::vfs_open]
```c
int vfs_open(const path_s * path, file_s * file)
{
	file->f_path = *path;
	return myos_do_dentry_open(file, path->dentry->d_inode);
}
```

### OC39-A031｜build_open_how 固定第27–31行

[VERIFIED mykernel/fs/vfs/open.c::build_open_how]
```c
open_how_s build_open_how(int flags, umode_t mode)
{
	struct open_how how = {
		.flags = flags & VALID_OPEN_FLAGS,
		.mode = mode & S_IALLUGO,
```

### OC39-A032｜build_open_flags 固定第67–71行

[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
int build_open_flags(const open_how_s *how, open_flags_s *op)
{
	uint64_t flags = how->flags;
	uint64_t strip = FMODE_NONOTIFY | O_CLOEXEC;
	int lookup_flags = 0;
```

### OC39-A033｜build_open_flags 固定第134–138行

[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	op->open_flag = flags;

	/* O_TRUNC implies we need access checks for write permissions */
	if (flags & O_TRUNC)
		acc_mode |= MAY_WRITE;
```

### OC39-A034｜build_open_flags 固定第145–149行

[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	op->acc_mode = acc_mode;

	op->intent = flags & O_PATH ? 0 : LOOKUP_OPEN;

	if (flags & O_CREAT) {
```

### OC39-A035｜build_open_flags 固定第157–160行

[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (flags & O_DIRECTORY)
		lookup_flags |= LOOKUP_DIRECTORY;
	if (!(flags & O_NOFOLLOW))
		lookup_flags |= LOOKUP_FOLLOW;
```

### OC39-A036｜build_open_flags 固定第172–176行

[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (how->resolve & RESOLVE_CACHED) {
		/* Don't bother even trying for create/truncate/tmpfile open */
		if (flags & (O_TRUNC | O_CREAT | O_TMPFILE))
			return -EAGAIN;
		lookup_flags |= LOOKUP_CACHED;
```

### OC39-A037｜build_open_flags 固定第179–180行

[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	op->lookup_flags = lookup_flags;
	return 0;
```

### OC39-A038｜file_open_name 固定第197–201行

[VERIFIED mykernel/fs/vfs/open.c::file_open_name]
```c
file_s *file_open_name(filename_s *name, int flags, umode_t mode)
{
	open_flags_s op;
	open_how_s how = build_open_how(flags, mode);
	int err = build_open_flags(&how, &op);
```

### OC39-A039｜file_open_name 固定第201–205行

[VERIFIED mykernel/fs/vfs/open.c::file_open_name]
```c
	int err = build_open_flags(&how, &op);
	if (err)
		return ERR_PTR(err);
	return do_filp_open(AT_FDCWD, name, &op);
}
```

### OC39-A040｜do_sys_openat2 固定第241–245行

[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
static long do_sys_openat2(int dfd, const char *filename, open_how_s *how)
{
	open_flags_s op;
	int fd = build_open_flags(how, &op);
	filename_s *tmp;
```

### OC39-A041｜do_sys_openat2 固定第248–252行

[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	if (IS_ERR(tmp))
		return PTR_ERR(tmp);

	fd = get_unused_fd_flags(how->flags);
	if (fd >=0)
```

### OC39-A042｜do_sys_openat2 固定第251–255行

[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	fd = get_unused_fd_flags(how->flags);
	if (fd >=0)
	{
		file_s * f = do_filp_open(dfd, tmp, &op);
		if (IS_ERR(f))
```

### OC39-A043｜do_sys_openat2 固定第254–258行

[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
		file_s * f = do_filp_open(dfd, tmp, &op);
		if (IS_ERR(f))
			fd = PTR_ERR(f);
		else
			fd_install(fd, f);
```

### OC39-A044｜do_sys_openat2 固定第258–262行

[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
			fd_install(fd, f);
	}
	putname(tmp);
	return fd;
}
```

### OC39-A045｜do_sys_open 固定第266–270行

[VERIFIED mykernel/fs/vfs/open.c::do_sys_open]
```c
long do_sys_open(int dfd, const char * filename, int flags, umode_t mode)
{
	open_how_s how = build_open_how(flags, mode);
	return do_sys_openat2(dfd, filename, &how);
}
```

### OC39-A046｜file_s 固定第870–874行

[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
		fmode_t				f_mode;
		atomic_long_t		f_count;
		// mutex_s				f_pos_lock;
		loff_t				f_pos;
		uint 				f_flags;
```

### OC39-A047｜file_s 固定第878–880行

[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
		path_s				f_path;
		inode_s				*f_inode;	/* cached value */
		const file_ops_s	*f_op;
```

### OC39-A048｜file_s 固定第892–896行

[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
	// #endif /* #ifdef CONFIG_EPOLL */
	addr_spc_s				*f_mapping;
	// 	errseq_t		f_wb_err;
	// 	errseq_t		f_sb_err; /* for syncfs */
	}__attribute__((aligned(4))) file_s;	/* lest something weird decides that 2 is OK */
```

### OC39-A049｜FMODE_READ 固定第61–61行

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_READ]
```c
	#define FMODE_READ			((__force fmode_t)0x1)
```

### OC39-A050｜FMODE_WRITE 固定第63–63行

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_WRITE]
```c
	#define FMODE_WRITE			((__force fmode_t)0x2)
```

### OC39-A051｜FMODE_OPENED 固定第110–110行

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_OPENED]
```c
	#define FMODE_OPENED			((__force fmode_t)0x80000)
```

### OC39-A052｜FMODE_NONOTIFY 固定第117–117行

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_NONOTIFY]
```c
	#define FMODE_NONOTIFY			((__force fmode_t)0x4000000)
```

### OC39-A053｜OPEN_FMODE 固定第3425–3428行

[VERIFIED mykernel/include/linux/fs/fs.h::OPEN_FMODE]
```c
	#define OPEN_FMODE(flag) (									\
				(__force fmode_t)(((flag + 1) & O_ACCMODE) |	\
					(flag & __FMODE_NONOTIFY))					\
				)
```

### OC39-A054｜open_flags_s 固定第91–95行

[VERIFIED mykernel/include/linux/fs/internal.h::open_flags_s]
```c
typedef struct open_flags {
	int open_flag;
	umode_t mode;
	int acc_mode;
	int intent;
```

### OC39-A055｜open_flags_s 固定第94–97行

[VERIFIED mykernel/include/linux/fs/internal.h::open_flags_s]
```c
	int acc_mode;
	int intent;
	int lookup_flags;
} open_flags_s;
```

### OC39-A056｜O_ACCMODE 固定第19–19行

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_ACCMODE]
```c
	#define O_ACCMODE			00000003
```

### OC39-A057｜O_RDONLY 固定第20–20行

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_RDONLY]
```c
	#define O_RDONLY			00000000
```

### OC39-A058｜O_WRONLY 固定第21–21行

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_WRONLY]
```c
	#define O_WRONLY			00000001
```

### OC39-A059｜O_RDWR 固定第22–22行

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_RDWR]
```c
	#define O_RDWR				00000002
```

### OC39-A060｜O_CREAT 固定第24–24行

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_CREAT]
```c
	#	define O_CREAT			00000100	/* not fcntl */
```

### OC39-A061｜O_TRUNC 固定第33–33行

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_TRUNC]
```c
	#	define O_TRUNC			00001000	/* not fcntl */
```

### OC39-A062｜O_APPEND 固定第36–36行

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_APPEND]
```c
	#	define O_APPEND			00002000
```

### OC39-A063｜FAT32_open 固定第108–112行

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_open]
```c
int FAT32_open(inode_s *inode, file_s *file_p)
{
	file_p->f_pos = 0;
	return 1;
}
```

### OC39-A064｜FAT32_file_ops 固定第354–358行

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_file_ops]
```c
file_ops_s FAT32_file_ops = 
{
	.open			= FAT32_open,
	.read			= FAT32_read,
	.write			= FAT32_write,
```

### OC39-A065｜ERR_PTR 固定第26–29行

[VERIFIED mykernel/include/linux/kernel/err.h::ERR_PTR]
```c
		static inline void *
		ERR_PTR(long error) {
			return (void *) error;
		}
```

### OC39-A066｜IS_ERR 固定第36–39行

[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR]
```c
		static inline bool
		IS_ERR(const void *ptr) {
			return IS_ERR_VALUE((size_t)ptr);
		}
```

### OC39-A067｜ENOERR 固定第5–5行

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOERR]
```c
	#define	ENOERR		0	/*  */
```

### OC39-A068｜ENOMEM 固定第18–18行

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOMEM]
```c
	#define	ENOMEM		12	/* Out of memory */
```

### OC39-A069｜ENFILE 固定第24–24行

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENFILE]
```c
	#define	EXDEV		18	/* Cross-device link */
```

