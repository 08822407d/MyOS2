---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-FD-ALLOCATION-INSTALL-40
record_type: distinct_prior_document_correction_and_new_fd_source_evidence
date: 2026-11-05
base_snapshot: time
source_pin_short12: a039d9803ade
read_channel: connector
R39_01_verdict: RETURN
R39_01_correction_status: CORRECTION_SUBMITTED_PENDING_MAINLINE
R39_01_closure: PENDING_C04_REVIEW
status: READY_FOR_MAINLINE_REVIEW
runtime_evidence: NOT_RUN
---

# 两条新增直接源码证据流

`R39C-A`共8段，仅证明R39-01替代宏以及R39-S01要求的位域/宏桥接；`FD40-A`共47段，仅证明第40批分配、选择、安装、错误边界。每条均取自固定time的**单一真实函数、宏或结构定义体内**1–5行；保留源缩进/换行，全部为新回源引文。旧39的69个OC39-A历史标签不算本轮新引文，不追认其余68个标签已经再次核证。同函数重复或重叠片段只支持某些局部语句，不计作不同根因。

## F00｜第39批追加文档纠正的新证据

### R39C-A001｜ENFILE（第29–29行）

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENFILE]
```c
	#define	ENFILE		23	/* File table overflow */
```

### R39C-A002｜EXDEV（第24–24行）

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::EXDEV]
```c
	#define	EXDEV		18	/* Cross-device link */
```

### R39C-A003｜__FMODE_NONOTIFY（第3422–3422行）

[VERIFIED mykernel/include/linux/fs/fs.h::__FMODE_NONOTIFY]
```c
	#define __FMODE_NONOTIFY	((__force int) FMODE_NONOTIFY)
```

### R39C-A004｜OPEN_FMODE（第3425–3428行）

[VERIFIED mykernel/include/linux/fs/fs.h::OPEN_FMODE]
```c
	#define OPEN_FMODE(flag) (									\
				(__force fmode_t)(((flag + 1) & O_ACCMODE) |	\
					(flag & __FMODE_NONOTIFY))					\
				)
```

### R39C-A005｜FMODE_32BITHASH（第80–80行）

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_32BITHASH]
```c
	#define FMODE_32BITHASH		((__force fmode_t)0x200)
```

### R39C-A006｜FMODE_64BITHASH（第82–82行）

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_64BITHASH]
```c
	#define FMODE_64BITHASH		((__force fmode_t)0x400)
```

### R39C-A007｜build_open_flags（第69–71行）

[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	uint64_t flags = how->flags;
	uint64_t strip = FMODE_NONOTIFY | O_CLOEXEC;
	int lookup_flags = 0;
```

### R39C-A008｜build_open_flags（第79–79行）

[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	flags &= ~strip;
```

## F01–F03｜第40批独立fd链的直接证据

### FD40-A001｜NR_OPEN_DEFAULT（第22–22行）

[VERIFIED mykernel/include/linux/kernel/fdtable.h::NR_OPEN_DEFAULT]
```c
	#define NR_OPEN_DEFAULT	256
```

### FD40-A002｜NR_OPEN_MAX（第24–24行）

[VERIFIED mykernel/include/linux/kernel/fdtable.h::NR_OPEN_MAX]
```c
	#define NR_OPEN_MAX		NR_OPEN_DEFAULT
```

### FD40-A003｜files_struct_s（第48–52行）

[VERIFIED mykernel/include/linux/kernel/fdtable.h::files_struct_s]
```c
	typedef struct files_struct {
		/*
		 * read mostly part
		 */
		atomic_t		refcount;
```

### FD40-A004｜files_struct_s（第61–64行）

[VERIFIED mykernel/include/linux/kernel/fdtable.h::files_struct_s]
```c
		// spinlock_t file_lock ____cacheline_aligned_in_smp;
		spinlock_t		file_lock;
		// unsigned int	next_fd;
		// unsigned long	close_on_exec_init[1];
```

### FD40-A005｜files_struct_s（第65–69行）

[VERIFIED mykernel/include/linux/kernel/fdtable.h::files_struct_s]
```c
		// unsigned long	open_fds_init[1];
		// unsigned long	full_fds_bits_init[1];
		unsigned long	fd_count;
		file_s __rcu	*fd_array[NR_OPEN_DEFAULT];
	} files_struct_s;
```

### FD40-A006｜init_files（第110–112行）

[VERIFIED mykernel/fs/vfs/file.c::init_files]
```c
files_struct_s init_files = {
	.refcount		= ATOMIC_INIT(1),
	// .resize_wait	= __WAIT_QUEUE_HEAD_INITIALIZER(init_files.resize_wait),
```

### FD40-A007｜init_files（第121–123行）

[VERIFIED mykernel/fs/vfs/file.c::init_files]
```c
	.file_lock		= __SPIN_LOCK_UNLOCKED(init_files.file_lock),
	.fd_count		= 0,
};
```

### FD40-A008｜dup_fd（第35–38行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	*errorp = -ENOMEM;
	newf = kmem_cache_alloc(files_cachep, GFP_KERNEL);
	if (!newf)
		goto out;
```

### FD40-A009｜dup_fd（第40–43行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	atomic_set(&newf->refcount, 1);
	newf->fd_count = NR_OPEN_DEFAULT;

	spin_lock_init(&newf->file_lock);
```

### FD40-A010｜dup_fd（第48–52行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
	old_fds = oldf->fd_array;
	new_fds = newf->fd_array;

	for (i = 0; i < max_fds; i++)
	{
```

### FD40-A011｜dup_fd（第53–57行）

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
		file_s *old_fp = old_fds[i];
		if (old_fp != NULL)
		{
			file_s *new_fp = kzalloc(sizeof(file_s), GFP_KERNEL);
			*new_fp = *old_fp;
```

### FD40-A012｜alloc_fd（第129–133行）

[VERIFIED mykernel/fs/vfs/file.c::alloc_fd]
```c
static int alloc_fd(uint start, uint end, uint flags)
{
	files_struct_s *files = current->files;
	uint fd;
	int error;
```

### FD40-A013｜alloc_fd（第135–139行）

[VERIFIED mykernel/fs/vfs/file.c::alloc_fd]
```c
	spin_lock(&files->file_lock);
	for(int i = start; i < end; i++)
		if(files->fd_array[i] == NULL)
		{
			fd = i;
```

### FD40-A014｜alloc_fd（第137–141行）

[VERIFIED mykernel/fs/vfs/file.c::alloc_fd]
```c
		if(files->fd_array[i] == NULL)
		{
			fd = i;
			break;
		}
```

### FD40-A015｜alloc_fd（第142–145行）

[VERIFIED mykernel/fs/vfs/file.c::alloc_fd]
```c

	spin_unlock(&files->file_lock);
	return fd;
}
```

### FD40-A016｜get_unused_fd_flags（第147–150行）

[VERIFIED mykernel/fs/vfs/file.c::get_unused_fd_flags]
```c
int get_unused_fd_flags(uint flags)
{
	return alloc_fd(0, current->files->fd_count, flags);
}
```

### FD40-A017｜put_unused_fd（第153–156行）

[VERIFIED mykernel/fs/vfs/file.c::put_unused_fd]
```c
void put_unused_fd(uint fd)
{
	files_struct_s *files = current->files;
	spin_lock(&files->file_lock);
```

### FD40-A018｜put_unused_fd（第161–165行）

[VERIFIED mykernel/fs/vfs/file.c::put_unused_fd]
```c
	// if (fd < files->next_fd)
	// 	files->next_fd = fd;
// }
	spin_unlock(&files->file_lock);
}
```

### FD40-A019｜fd_install（第184–188行）

[VERIFIED mykernel/fs/vfs/file.c::fd_install]
```c
void fd_install(uint fd, file_s *file)
{
	files_struct_s *files = current->files;
	file_s ** fps = files->fd_array;

```

### FD40-A020｜fd_install（第189–193行）

[VERIFIED mykernel/fs/vfs/file.c::fd_install]
```c
	/* coupled with smp_wmb() in expand_fdtable() */
	smp_rmb();
	BUG_ON(fps[fd] != NULL);
	fps[fd] = file;
}
```

### FD40-A021｜fd_install（第190–193行）

[VERIFIED mykernel/fs/vfs/file.c::fd_install]
```c
	smp_rmb();
	BUG_ON(fps[fd] != NULL);
	fps[fd] = file;
}
```

### FD40-A022｜f_dupfd（第229–233行）

[VERIFIED mykernel/fs/vfs/file.c::f_dupfd]
```c
int f_dupfd(uint from, file_s *file, uint flags)
{
	ulong nofile = rlimit(RLIMIT_NOFILE);
	int err;
	if (from >= nofile)
```

### FD40-A023｜f_dupfd（第234–238行）

[VERIFIED mykernel/fs/vfs/file.c::f_dupfd]
```c
		return -EINVAL;
	err = alloc_fd(from, nofile, flags);
	if (err >= 0) {
		get_file(file);
		fd_install(err, file);
```

### FD40-A024｜f_dupfd（第236–240行）

[VERIFIED mykernel/fs/vfs/file.c::f_dupfd]
```c
	if (err >= 0) {
		get_file(file);
		fd_install(err, file);
	}
	return err;
```

### FD40-A025｜BUG_ON（第65–67行）

[VERIFIED mykernel/include/linux/kernel/asm-generic/bug.h::BUG_ON]
```c
	#		ifndef HAVE_ARCH_BUG_ON
	#			define BUG_ON(condition) do { if (unlikely(condition)) BUG(); } while (0)
	#		endif
```

### FD40-A026｜BUG（第57–61行）

[VERIFIED mykernel/include/linux/kernel/asm-generic/bug.h::BUG]
```c
	#			define BUG() do {									\
							printk("BUG: failure at %s:%d/%s()!\n",	\
								__FILE__, __LINE__, __func__);		\
							barrier_before_unreachable();			\
							panic("BUG!");							\
```

### FD40-A027｜do_sys_openat2（第241–245行）

[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
static long do_sys_openat2(int dfd, const char *filename, open_how_s *how)
{
	open_flags_s op;
	int fd = build_open_flags(how, &op);
	filename_s *tmp;
```

### FD40-A028｜do_sys_openat2（第247–251行）

[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	tmp = getname(filename);
	if (IS_ERR(tmp))
		return PTR_ERR(tmp);

	fd = get_unused_fd_flags(how->flags);
```

### FD40-A029｜do_sys_openat2（第251–255行）

[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	fd = get_unused_fd_flags(how->flags);
	if (fd >=0)
	{
		file_s * f = do_filp_open(dfd, tmp, &op);
		if (IS_ERR(f))
```

### FD40-A030｜do_sys_openat2（第254–258行）

[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
		file_s * f = do_filp_open(dfd, tmp, &op);
		if (IS_ERR(f))
			fd = PTR_ERR(f);
		else
			fd_install(fd, f);
```

### FD40-A031｜do_sys_openat2（第259–262行）

[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	}
	putname(tmp);
	return fd;
}
```

### FD40-A032｜__alloc_file（第44–46行）

[VERIFIED mykernel/fs/vfs/file_table.c::__alloc_file]
```c
	f = kmem_cache_zalloc(filp_cachep, GFP_KERNEL);
	if (unlikely(!f))
		return ERR_PTR(-ENOMEM);
```

### FD40-A033｜__alloc_file（第55–59行）

[VERIFIED mykernel/fs/vfs/file_table.c::__alloc_file]
```c
	atomic_long_set(&f->f_count, 1);
	// rwlock_init(&f->f_owner.lock);
	spin_lock_init(&f->f_lock);
	// mutex_init(&f->f_pos_lock);
	f->f_flags = flags;
```

### FD40-A034｜alloc_empty_file（第93–97行）

[VERIFIED mykernel/fs/vfs/file_table.c::alloc_empty_file]
```c
	f = __alloc_file(flags);
	// if (!IS_ERR(f))
	// 	percpu_counter_inc(&nr_files);

	return f;
```

### FD40-A035｜fput（第108–112行）

[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
void fput(file_s *file)
{
	if (atomic_long_dec_and_test(&file->f_count)) {
		task_s *task = current;

```

### FD40-A036｜fput（第125–129行）

[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
		// 	schedule_delayed_work(&delayed_fput_work, 1);

		kmem_cache_free(filp_cachep, file);
	}
}
```

### FD40-A037｜file_s（第870–874行）

[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
		fmode_t				f_mode;
		atomic_long_t		f_count;
		// mutex_s				f_pos_lock;
		loff_t				f_pos;
		uint 				f_flags;
```

### FD40-A038｜file_s（第878–880行）

[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
		path_s				f_path;
		inode_s				*f_inode;	/* cached value */
		const file_ops_s	*f_op;
```

### FD40-A039｜O_CLOEXEC（第63–63行）

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_CLOEXEC]
```c
	#	define O_CLOEXEC		02000000	/* set close_on_exec */
```

### FD40-A040｜ERR_PTR（第26–29行）

[VERIFIED mykernel/include/linux/kernel/err.h::ERR_PTR]
```c
		static inline void *
		ERR_PTR(long error) {
			return (void *) error;
		}
```

### FD40-A041｜IS_ERR（第36–39行）

[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR]
```c
		static inline bool
		IS_ERR(const void *ptr) {
			return IS_ERR_VALUE((size_t)ptr);
		}
```

### FD40-A042｜PTR_ERR（第31–34行）

[VERIFIED mykernel/include/linux/kernel/err.h::PTR_ERR]
```c
		static inline long
		PTR_ERR(const void *ptr) {
			return (long) ptr;
		}
```

### FD40-A043｜EMFILE（第30–30行）

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::EMFILE]
```c
	#define	EMFILE		24	/* Too many open files */
```

### FD40-A044｜ENOMEM（第18–18行）

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOMEM]
```c
	#define	ENOMEM		12	/* Out of memory */
```

### FD40-A045｜EINVAL（第28–28行）

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::EINVAL]
```c
	#define	EINVAL		22	/* Invalid argument */
```

### FD40-A046｜myos_fdget_pos（第197–201行）

[VERIFIED mykernel/fs/vfs/file.c::myos_fdget_pos]
```c
fd_s myos_fdget_pos(int fd)
{
	task_s *curr = current;
	file_s *fp = curr->files->fd_array[fd];
	
```

### FD40-A047｜get_unused_fd_flags（第147–149行）

[VERIFIED mykernel/fs/vfs/file.c::get_unused_fd_flags]
```c
int get_unused_fd_flags(uint flags)
{
	return alloc_fd(0, current->files->fd_count, flags);
```

