---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-FD-CONTROL-61
record_type: bounded_single_definition_fixed_time_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
base_snapshot: time
source_pin_short12: a039d9803ade
result_base_short12: 1a6ef1c7a5aa
read_channel: GitHub_connector
status: READY_FOR_MAINLINE_REVIEW
runtime_evidence: NOT_RUN
---
# 第61批源码锚点（A/B/C）

每个标签的路径与符号均指向time固定源码中的唯一函数、结构体或单一宏定义体，短摘录为连续1–5物理行，保留实际制表符、空行及可见CRLF。独立宏分别取证。源中MYOS_SYSCALL_DEFINE宏调用体和相关wrapper只是源码层入口，未进行C预处理或编译，不能将注释里的dup算法当成活动代码。跨多个标签的命题必须合读完整活动函数与正文的valid-object及最早停止点限定。

## A：fcntl命令准入、真实switch与fdput消费的固定time源码锚

### A01 MYOS_SYSCALL_DEFINE3(fcntl)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(fcntl)]
```c
MYOS_SYSCALL_DEFINE3(fcntl, unsigned int, fd,
        unsigned int, cmd, unsigned long, arg)
{	
	fd_s f = fdget_raw(fd);
	long err = -EBADF;
```

### A02 MYOS_SYSCALL_DEFINE3(fcntl)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(fcntl)]
```c
	fd_s f = fdget_raw(fd);
	long err = -EBADF;

	if (!f.file)
		goto out;
```

### A03 MYOS_SYSCALL_DEFINE3(fcntl)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(fcntl)]
```c
		goto out;

	if (unlikely(f.file->f_mode & FMODE_PATH)) {
		if (!check_fcntl_cmd(cmd))
			goto out1;
```

### A04 MYOS_SYSCALL_DEFINE3(fcntl)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(fcntl)]
```c
	if (unlikely(f.file->f_mode & FMODE_PATH)) {
		if (!check_fcntl_cmd(cmd))
			goto out1;
	}

```

### A05 MYOS_SYSCALL_DEFINE3(fcntl)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(fcntl)]
```c
	// err = security_file_fcntl(f.file, cmd, arg);
	// if (!err)
		err = do_fcntl(fd, cmd, arg, f.file);

out1:
```

### A06 MYOS_SYSCALL_DEFINE3(fcntl)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(fcntl)]
```c
out1:
 	fdput(f);
out:
	return err;
}
```

### A07 do_fcntl

[VERIFIED mykernel/fs/vfs/fcntl.c::do_fcntl]
```c
long
do_fcntl(int fd, uint cmd, ulong arg, file_s *filp)
{
	void __user *argp = (void __user *)arg;
	int argi = (int)arg;
```

### A08 do_fcntl

[VERIFIED mykernel/fs/vfs/fcntl.c::do_fcntl]
```c
	int argi = (int)arg;
	flock_s flock;
	long err = -EINVAL;

	switch (cmd) {
```

### A09 do_fcntl

[VERIFIED mykernel/fs/vfs/fcntl.c::do_fcntl]
```c
	switch (cmd) {
	case F_DUPFD:
		err = f_dupfd(argi, filp, 0);
		break;
```

### A10 do_fcntl

[VERIFIED mykernel/fs/vfs/fcntl.c::do_fcntl]
```c
	case F_DUPFD_CLOEXEC:
		err = f_dupfd(argi, filp, O_CLOEXEC);
		break;
```

### A11 do_fcntl

[VERIFIED mykernel/fs/vfs/fcntl.c::do_fcntl]
```c
	// case F_GETFD:
	// 	err = get_close_on_exec(fd) ? FD_CLOEXEC : 0;
	// 	break;
	// case F_SETFD:
	// 	err = 0;
```

### A12 do_fcntl

[VERIFIED mykernel/fs/vfs/fcntl.c::do_fcntl]
```c
	// case F_SETFD:
	// 	err = 0;
	// 	set_close_on_exec(fd, argi & FD_CLOEXEC);
	// 	break;
```

### A13 do_fcntl

[VERIFIED mykernel/fs/vfs/fcntl.c::do_fcntl]
```c
	case F_GETFL:
		err = filp->f_flags;
		break;
	// case F_SETFL:
	// 	err = setfl(fd, filp, argi);
```

### A14 do_fcntl

[VERIFIED mykernel/fs/vfs/fcntl.c::do_fcntl]
```c
	default:
		pr_alert("  Syscall \'fcntl\' unsupported flag: %d\n", cmd);
		break;
	}
	return err;
```

### A15 check_fcntl_cmd

[VERIFIED mykernel/fs/vfs/fcntl.c::check_fcntl_cmd]
```c
int
check_fcntl_cmd(uint cmd)
{
	switch (cmd) {
	case F_DUPFD:
```

### A16 check_fcntl_cmd

[VERIFIED mykernel/fs/vfs/fcntl.c::check_fcntl_cmd]
```c
	case F_DUPFD:
	case F_DUPFD_CLOEXEC:
	case F_GETFD:
	case F_SETFD:
	case F_GETFL:
```

### A17 check_fcntl_cmd

[VERIFIED mykernel/fs/vfs/fcntl.c::check_fcntl_cmd]
```c
	case F_GETFL:
		return 1;
	}
	return 0;
}
```

### A18 __to_fd

[VERIFIED mykernel/include/linux/fs/file.h::__to_fd]
```c
	static inline fd_s
	__to_fd(ulong v) {
		files_struct_s *files = current->files;
		file_s *file = files->fd_array[v];
		// return (fd_s){(file_s *)(v & ~3),v & 3};
```

### A19 __to_fd

[VERIFIED mykernel/include/linux/fs/file.h::__to_fd]
```c
		files_struct_s *files = current->files;
		file_s *file = files->fd_array[v];
		// return (fd_s){(file_s *)(v & ~3),v & 3};
		return (fd_s){file, 3};
	}
```

### A20 fdget_raw

[VERIFIED mykernel/include/linux/fs/file.h::fdget_raw]
```c
	static inline fd_s
	fdget_raw(uint fd) {
		// return __to_fd(__fdget_raw(fd));
		return __to_fd(fd);
	}
```

### A21 fdput

[VERIFIED mykernel/include/linux/fs/file.h::fdput]
```c
	static inline void fdput(fd_s fd)
	{
		if (fd.flags & FDPUT_FPUT)
			fput(fd.file);
	}
```

### A22 FDPUT_FPUT

[VERIFIED mykernel/include/linux/fs/file.h::FDPUT_FPUT]
```c
	#define FDPUT_FPUT			1
```

### A23 FMODE_PATH

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_PATH]
```c
	#define FMODE_PATH				((__force fmode_t)0x4000)
```

### A24 file_s

[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
		fmode_t				f_mode;
		atomic_long_t		f_count;
		// mutex_s				f_pos_lock;
		loff_t				f_pos;
		uint 				f_flags;
```

### A25 F_DUPFD

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::F_DUPFD]
```c
	#define F_DUPFD				0	/* dup */
```

### A26 F_GETFD

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::F_GETFD]
```c
	#define F_GETFD				1	/* get close_on_exec */
```

### A27 F_SETFD

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::F_SETFD]
```c
	#define F_SETFD				2	/* set/clear close_on_exec */
```

### A28 F_GETFL

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::F_GETFL]
```c
	#define F_GETFL				3	/* get file->f_flags */
```

### A29 F_SETFL

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::F_SETFL]
```c
	#define F_SETFL				4	/* set file->f_flags */
```

### A30 F_DUPFD_CLOEXEC

[VERIFIED mykernel/include/uapi/linux/fcntl.h::F_DUPFD_CLOEXEC]
```c
	#define F_DUPFD_CLOEXEC	(F_LINUX_SPECIFIC_BASE + 6)
```

### A31 F_LINUX_SPECIFIC_BASE

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::F_LINUX_SPECIFIC_BASE]
```c
	#define F_LINUX_SPECIFIC_BASE	1024
```

### A32 fput

[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
void fput(file_s *file)
{
	if (atomic_long_dec_and_test(&file->f_count)) {
		task_s *task = current;

```

### A33 fput

[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
		// 	schedule_delayed_work(&delayed_fput_work, 1);

		kmem_cache_free(filp_cachep, file);
	}
}
```


## B（前半）：f_dupfd、alloc_fd、fd_install、get_file/fput与rlimit

### B01 f_dupfd

[VERIFIED mykernel/fs/vfs/file.c::f_dupfd]
```c
int f_dupfd(uint from, file_s *file, uint flags)
{
	ulong nofile = rlimit(RLIMIT_NOFILE);
	int err;
	if (from >= nofile)
```

### B02 f_dupfd

[VERIFIED mykernel/fs/vfs/file.c::f_dupfd]
```c
	ulong nofile = rlimit(RLIMIT_NOFILE);
	int err;
	if (from >= nofile)
		return -EINVAL;
	err = alloc_fd(from, nofile, flags);
```

### B03 f_dupfd

[VERIFIED mykernel/fs/vfs/file.c::f_dupfd]
```c
	if (from >= nofile)
		return -EINVAL;
	err = alloc_fd(from, nofile, flags);
	if (err >= 0) {
		get_file(file);
```

### B04 f_dupfd

[VERIFIED mykernel/fs/vfs/file.c::f_dupfd]
```c
	err = alloc_fd(from, nofile, flags);
	if (err >= 0) {
		get_file(file);
		fd_install(err, file);
	}
```

### B05 f_dupfd

[VERIFIED mykernel/fs/vfs/file.c::f_dupfd]
```c
	if (err >= 0) {
		get_file(file);
		fd_install(err, file);
	}
	return err;
```

### B06 alloc_fd

[VERIFIED mykernel/fs/vfs/file.c::alloc_fd]
```c
static int alloc_fd(uint start, uint end, uint flags)
{
	files_struct_s *files = current->files;
	uint fd;
	int error;
```

### B07 alloc_fd

[VERIFIED mykernel/fs/vfs/file.c::alloc_fd]
```c
	spin_lock(&files->file_lock);
	for(int i = start; i < end; i++)
		if(files->fd_array[i] == NULL)
		{
			fd = i;
```

### B08 alloc_fd

[VERIFIED mykernel/fs/vfs/file.c::alloc_fd]
```c
		if(files->fd_array[i] == NULL)
		{
			fd = i;
			break;
		}
```

### B09 alloc_fd

[VERIFIED mykernel/fs/vfs/file.c::alloc_fd]
```c
			break;
		}

	spin_unlock(&files->file_lock);
	return fd;
```

### B10 alloc_fd

[VERIFIED mykernel/fs/vfs/file.c::alloc_fd]
```c
	spin_unlock(&files->file_lock);
	return fd;
}
```

### B11 fd_install

[VERIFIED mykernel/fs/vfs/file.c::fd_install]
```c
void fd_install(uint fd, file_s *file)
{
	files_struct_s *files = current->files;
	file_s ** fps = files->fd_array;

```

### B12 fd_install

[VERIFIED mykernel/fs/vfs/file.c::fd_install]
```c
	/* coupled with smp_wmb() in expand_fdtable() */
	smp_rmb();
	BUG_ON(fps[fd] != NULL);
	fps[fd] = file;
}
```

### B13 get_file

[VERIFIED mykernel/include/linux/fs/fs.h::get_file]
```c
	static inline file_s *get_file(file_s *f) {
		atomic_long_inc(&f->f_count);
		return f;
	}
```

### B14 fput

[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
void fput(file_s *file)
{
	if (atomic_long_dec_and_test(&file->f_count)) {
		task_s *task = current;

```

### B15 fput

[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
		// 	schedule_delayed_work(&delayed_fput_work, 1);

		kmem_cache_free(filp_cachep, file);
	}
}
```

### B16 files_struct_s

[VERIFIED mykernel/include/linux/kernel/fdtable.h::files_struct_s]
```c
		// unsigned long	open_fds_init[1];
		// unsigned long	full_fds_bits_init[1];
		unsigned long	fd_count;
		file_s __rcu	*fd_array[NR_OPEN_DEFAULT];
	} files_struct_s;
```

### B17 NR_OPEN_DEFAULT

[VERIFIED mykernel/include/linux/kernel/fdtable.h::NR_OPEN_DEFAULT]
```c
	#define NR_OPEN_DEFAULT	256
```

### B18 rlimit

[VERIFIED mykernel/sched/task/task.h::rlimit]
```c
		PREFIX_STATIC_INLINE
		ulong
		rlimit(uint limit) {
			return task_rlimit(current, limit);
		}
```

### B19 task_rlimit

[VERIFIED mykernel/sched/task/task.h::task_rlimit]
```c
		PREFIX_STATIC_INLINE
		ulong
		task_rlimit(const task_s *task, uint limit) {
			return READ_ONCE(task->signal->rlim[limit].rlim_cur);
		}
```

### B20 RLIMIT_NOFILE

[VERIFIED mykernel/include/uapi/asm-generic/resource.h::RLIMIT_NOFILE]
```c
	#	define RLIMIT_NOFILE	7	/* max number of open files */
```

### B21 put_unused_fd

[VERIFIED mykernel/fs/vfs/file.c::put_unused_fd]
```c
void put_unused_fd(uint fd)
{
	files_struct_s *files = current->files;
	spin_lock(&files->file_lock);
// static void __put_unused_fd(struct files_struct *files, unsigned int fd)
```

### B22 get_unused_fd_flags

[VERIFIED mykernel/fs/vfs/file.c::get_unused_fd_flags]
```c
int get_unused_fd_flags(uint flags)
{
	return alloc_fd(0, current->files->fd_count, flags);
}
```


## B（后半）：close-on-exec源、dup/dup2宏体与dummy选择

### B23 O_CLOEXEC

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_CLOEXEC]
```c
	#	define O_CLOEXEC		02000000	/* set close_on_exec */
```

### B24 F_DUPFD_CLOEXEC

[VERIFIED mykernel/include/uapi/linux/fcntl.h::F_DUPFD_CLOEXEC]
```c
	#define F_DUPFD_CLOEXEC	(F_LINUX_SPECIFIC_BASE + 6)
```

### B25 MYOS_SYSCALL_DEFINE1(dup)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE1(dup)]
```c
MYOS_SYSCALL_DEFINE1(dup, unsigned int, fildes)
{
	ALERT_DUMMY_SYSCALL(dup, IF_ALERT_DUMMY_SYSCALL);

```

### B26 MYOS_SYSCALL_DEFINE1(dup)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE1(dup)]
```c

	// int ret = -EBADF;
	// file_s *file = fget_raw(fildes);

	// if (file) {
```

### B27 MYOS_SYSCALL_DEFINE1(dup)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE1(dup)]
```c
	// 	else
	// 		fput(file);
	// }
	// return ret;
}
```

### B28 MYOS_SYSCALL_DEFINE2(dup2)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE2(dup2)]
```c
MYOS_SYSCALL_DEFINE2(dup2, unsigned int, oldfd, unsigned int, newfd)
{
	ALERT_DUMMY_SYSCALL(dup2, IF_ALERT_DUMMY_SYSCALL);

	// if (unlikely(newfd == oldfd)) { /* corner case */
```

### B29 MYOS_SYSCALL_DEFINE2(dup2)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE2(dup2)]
```c
	ALERT_DUMMY_SYSCALL(dup2, IF_ALERT_DUMMY_SYSCALL);

	// if (unlikely(newfd == oldfd)) { /* corner case */
	// 	files_struct_s *files = current->files;
	// 	int retval = oldfd;
```

### B30 MYOS_SYSCALL_DEFINE2(dup2)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE2(dup2)]
```c
	// 	rcu_read_unlock();
	// 	return retval;
	// }
	// return ksys_dup3(oldfd, newfd, 0);
}
```

### B31 IF_ALERT_DUMMY_SYSCALL

[VERIFIED mykernel/include/linux/kernel/syscalls.h::IF_ALERT_DUMMY_SYSCALL]
```c
	#define IF_ALERT_DUMMY_SYSCALL 0
```

### B32 ALERT_DUMMY_SYSCALL

[VERIFIED mykernel/include/linux/kernel/syscalls.h::ALERT_DUMMY_SYSCALL]
```c
	#define ALERT_DUMMY_SYSCALL(syscall_name, show_alert)		\
			if (show_alert) {									\
				pr_alert("\t!!! Dummy Syscall --- %s ---\n",	\
					#syscall_name);								\
			}
```

### B33 MYOS_SYSCALL_DEFINE1

[VERIFIED mykernel/include/linux/kernel/syscalls.h::MYOS_SYSCALL_DEFINE1]
```c
	#define MYOS_SYSCALL_DEFINE1(name, ...)	__MYOS_SYSCALL_DEFINEx(1, _##name, __VA_ARGS__)
```

### B34 MYOS_SYSCALL_DEFINE2

[VERIFIED mykernel/include/linux/kernel/syscalls.h::MYOS_SYSCALL_DEFINE2]
```c
	#define MYOS_SYSCALL_DEFINE2(name, ...)	__MYOS_SYSCALL_DEFINEx(2, _##name, __VA_ARGS__)
```

### B35 __MYOS_SYSCALL_DEFINEx

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
	#define __MYOS_SYSCALL_DEFINEx(x, name, ...)						\
				long __se_sys##name(const pt_regs_s *regs);				\
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));	\
				__SYS_STUBx(x64, sys##name)								\
				long __se_sys##name(const pt_regs_s *regs)				\
```

### B36 __MYOS_SYSCALL_DEFINEx

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
					long ret = __do_sys##name(							\
								__ARCH_MAP(x,__SC_CAST,__VA_ARGS__));	\
					return ret;											\
				}														\
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__))
```


## C：lseek入口、FAT回调、SEEK命令与返回类型的固定time源码证据

### C01 MYOS_SYSCALL_DEFINE3(lseek)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(lseek)]
```c
MYOS_SYSCALL_DEFINE3(lseek, unsigned int, fd,
		off_t, offset, unsigned int, whence)
{
	task_s * curr = current;
	file_s * fp = NULL;
```

### C02 MYOS_SYSCALL_DEFINE3(lseek)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(lseek)]
```c
	task_s * curr = current;
	file_s * fp = NULL;
	ulong ret = 0;

	if(fd < 0 || fd >= curr->files->fd_count)
```

### C03 MYOS_SYSCALL_DEFINE3(lseek)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(lseek)]
```c
	if(fd < 0 || fd >= curr->files->fd_count)
		return -EBADF;
	if(whence < 0 || whence >= SEEK_MAX)
		return -EINVAL;
```

### C04 MYOS_SYSCALL_DEFINE3(lseek)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(lseek)]
```c
	if(whence < 0 || whence >= SEEK_MAX)
		return -EINVAL;

	fp = current->files->fd_array[fd];
	if(fp->f_op && fp->f_op->llseek)
```

### C05 MYOS_SYSCALL_DEFINE3(lseek)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(lseek)]
```c
	fp = current->files->fd_array[fd];
	if(fp->f_op && fp->f_op->llseek)
		ret = fp->f_op->llseek(fp, offset, whence);
	return ret;
```

### C06 MYOS_SYSCALL_DEFINE3(lseek)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(lseek)]
```c
	if(fp->f_op && fp->f_op->llseek)
		ret = fp->f_op->llseek(fp, offset, whence);
	return ret;
}
```

### C07 FAT32_lseek

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
loff_t FAT32_lseek(file_s *filp, loff_t offset, int origin)
{
	inode_s *inode = filp->f_path.dentry->d_inode;
	long pos = 0;

```

### C08 FAT32_lseek

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
	switch(origin)
	{
		case SEEK_SET:
				pos = offset;
			break;
```

### C09 FAT32_lseek

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
		case SEEK_CUR:
				pos =  filp->f_pos + offset;
			break;
```

### C10 FAT32_lseek

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
		case SEEK_END:
				pos = filp->f_path.dentry->d_inode->i_size + offset;
			break;
```

### C11 FAT32_lseek

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
		default:
			return -EINVAL;
			break;
	}
```

### C12 FAT32_lseek

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
	if(pos < 0 || pos > filp->f_path.dentry->d_inode->i_size)
		return -EOVERFLOW;

	filp->f_pos = pos;
```

### C13 FAT32_lseek

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
	filp->f_pos = pos;
	// color_printk(GREEN, BLACK, "FAT32 FS(lseek) alert position:%d\n", filp->position);

	return pos;
}
```

### C14 FAT32_file_ops

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_file_ops]
```c
{
	.open			= FAT32_open,
	.read			= FAT32_read,
	.write			= FAT32_write,
	.llseek			= FAT32_lseek,
```

### C15 file_ops_s

[VERIFIED mykernel/include/linux/fs/fs.h::file_ops_s]
```c
	typedef struct file_operations {
		// module_s	*owner;
		fop_flags_t fop_flags;

		loff_t		(*llseek) (file_s *file, loff_t pos, int);
```

### C16 file_s

[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
		fmode_t				f_mode;
		atomic_long_t		f_count;
		// mutex_s				f_pos_lock;
		loff_t				f_pos;
		uint 				f_flags;
```

### C17 file_s

[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
		path_s				f_path;
		inode_s				*f_inode;	/* cached value */
		const file_ops_s	*f_op;
```

### C18 SEEK_SET

[VERIFIED mykernel/include/uapi/linux/fs.h::SEEK_SET]
```c
	#define SEEK_SET			0			/* seek relative to beginning of file */
```

### C19 SEEK_CUR

[VERIFIED mykernel/include/uapi/linux/fs.h::SEEK_CUR]
```c
	#define SEEK_CUR			1			/* seek relative to current file position */
```

### C20 SEEK_END

[VERIFIED mykernel/include/uapi/linux/fs.h::SEEK_END]
```c
	#define SEEK_END			2			/* seek relative to end of file */
```

### C21 SEEK_DATA

[VERIFIED mykernel/include/uapi/linux/fs.h::SEEK_DATA]
```c
	#define SEEK_DATA			3			/* seek to the next data */
```

### C22 SEEK_HOLE

[VERIFIED mykernel/include/uapi/linux/fs.h::SEEK_HOLE]
```c
	#define SEEK_HOLE			4			/* seek to the next hole */
```

### C23 SEEK_MAX

[VERIFIED mykernel/include/uapi/linux/fs.h::SEEK_MAX]
```c
	#define SEEK_MAX			SEEK_HOLE
```

### C24 off_t

[VERIFIED mykernel/include/linux/kernel/types.h::off_t]
```c
		typedef __kernel_off_t		off_t;
```

### C25 loff_t

[VERIFIED mykernel/include/linux/kernel/types.h::loff_t]
```c
			typedef __kernel_loff_t		loff_t;
```

### C26 __kernel_off_t

[VERIFIED mykernel/include/uapi/asm-generic/posix_types.h::__kernel_off_t]
```c
	typedef __kernel_long_t		__kernel_off_t;
```

### C27 __kernel_loff_t

[VERIFIED mykernel/include/uapi/asm-generic/posix_types.h::__kernel_loff_t]
```c
	typedef long long			__kernel_loff_t;
```

### C28 files_struct_s

[VERIFIED mykernel/include/linux/kernel/fdtable.h::files_struct_s]
```c
		unsigned long	fd_count;
		file_s __rcu	*fd_array[NR_OPEN_DEFAULT];
	} files_struct_s;
```

