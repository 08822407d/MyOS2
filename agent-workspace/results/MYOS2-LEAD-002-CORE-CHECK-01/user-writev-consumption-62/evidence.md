---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-WRITEV-CONSUMPTION-62
record_type: bounded_fixed_time_direct_source_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
base_snapshot: time
source_pin_short12: a039d9803ade
execution_base_short12: 7f37c7b1e445
read_channel: GitHub_connector
status: READY_FOR_MAINLINE_REVIEW
runtime_evidence: NOT_RUN
---
# 第62批：写向量、逐段累计及FAT32位置/副作用直接源码锚

每个直接证据标签取固定time下真实函数、类型、单独宏或唯一系统调用宏体的连续1–5个源码物理行；源码空行计入、原tab与可见CRLF保留。函数内被注释的逻辑只用于说明当前不活动，不当成实际执行。不同引文须合读相应完整函数和正文前提。第61批R61-S01/S02/S03及第08批原有UB/US问题仅作为历史消费，不混入本批新证据计数。系统调用宏只核未预处理的源码入口，并非已编译ABI验证。

## A · 系统调用、向量来源、模式与标量写对照

### A01 · MYOS_SYSCALL_DEFINE3(writev)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(writev)]
```c
MYOS_SYSCALL_DEFINE3(writev, unsigned long, fd,
		const iov_s __user *, vec, unsigned long, vlen)
{
```

### A02 · MYOS_SYSCALL_DEFINE3(writev)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(writev)]
```c
MYOS_SYSCALL_DEFINE3(writev, unsigned long, fd,
		const iov_s __user *, vec, unsigned long, vlen)
{
	return do_writev(fd, vec, vlen, 0);
}
```

### A03 · do_writev

[VERIFIED mykernel/fs/vfs/read_write.c::do_writev]
```c
ssize_t do_writev(ulong fd, const iov_s __user *vec, ulong vlen, rwf_t flags)
{
	fd_s f = fdget_pos(fd);
	ssize_t ret = -EBADF;

```

### A04 · do_writev

[VERIFIED mykernel/fs/vfs/read_write.c::do_writev]
```c
	fd_s f = fdget_pos(fd);
	ssize_t ret = -EBADF;

	if (f.file) {
		loff_t pos, *ppos = file_ppos(f.file);
```

### A05 · do_writev

[VERIFIED mykernel/fs/vfs/read_write.c::do_writev]
```c
	if (f.file) {
		loff_t pos, *ppos = file_ppos(f.file);
		if (ppos) {
			pos = *ppos;
			ppos = &pos;
```

### A06 · do_writev

[VERIFIED mykernel/fs/vfs/read_write.c::do_writev]
```c
			ppos = &pos;
		}
		ret = vfs_writev(f.file, vec, vlen, ppos, flags);
		if (ret >= 0 && ppos)
			f.file->f_pos = pos;
```

### A07 · do_writev

[VERIFIED mykernel/fs/vfs/read_write.c::do_writev]
```c
		if (ret >= 0 && ppos)
			f.file->f_pos = pos;
		fdput_pos(f);
	}

```

### A08 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
static ssize_t
vfs_writev(file_s *file, const iov_s *vec,
		ulong vlen, loff_t *pos, rwf_t flags) {

	iov_s iovstack[UIO_FASTIOV];
```

### A09 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
	iov_s iovstack[UIO_FASTIOV];
	iov_s *iov = iovstack;
	iov_iter_s iter;
	size_t tot_len;
	ssize_t ret = 0;
```

### A10 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c

	if (!(file->f_mode & FMODE_WRITE))
		return -EBADF;
	// if (!(file->f_mode & FMODE_CAN_WRITE))
	// 	return -EINVAL;
```

### A11 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
	// ret = import_iovec(ITER_SOURCE, vec, vlen,
	// 		ARRAY_SIZE(iovstack), &iov, &iter);
	// if (ret < 0)
	// 	return ret;

```

### A12 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
	// tot_len = iov_iter_count(&iter);
	// if (!tot_len)
	// 	goto out;

	// ret = rw_verify_area(WRITE, file, pos, tot_len);
```

### A13 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
	// if (file->f_op->write_iter)
	// 	ret = do_iter_readv_writev(file, &iter, pos, WRITE, flags);
	// else
	// 	ret = do_loop_readv_writev(file, &iter, pos, WRITE, flags);
	// if (ret > 0)
```

### A14 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
	for (int i = 0; i < vlen; i++)
		ret += file->f_op->write(file, vec[i].iov_base, vec[i].iov_len, pos);

	return ret;
```

### A15 · iov_s

[VERIFIED mykernel/include/uapi/linux/uio.h::iov_s]
```c
	typedef struct iovec {
		void __user     *iov_base;	/* BSD uses caddr_t (1003.1g requires void *) */
		__kernel_size_t iov_len; /* Must be size_t (1003.1g) */
	} iov_s;
```

### A16 · UIO_FASTIOV

[VERIFIED mykernel/include/uapi/linux/uio.h::UIO_FASTIOV]
```c
	#define UIO_FASTIOV	8
```

### A17 · UIO_MAXIOV

[VERIFIED mykernel/include/uapi/linux/uio.h::UIO_MAXIOV]
```c
	#define UIO_MAXIOV	1024
```

### A18 · fdget_pos

[VERIFIED mykernel/include/linux/fs/file.h::fdget_pos]
```c
	static inline fd_s
	fdget_pos(int fd) {
		// return __to_fd(__fdget_pos(fd));
		return __to_fd(fd);
	}
```

### A19 · __to_fd

[VERIFIED mykernel/include/linux/fs/file.h::__to_fd]
```c
	static inline fd_s
	__to_fd(ulong v) {
		files_struct_s *files = current->files;
		file_s *file = files->fd_array[v];
		// return (fd_s){(file_s *)(v & ~3),v & 3};
```

### A20 · __to_fd

[VERIFIED mykernel/include/linux/fs/file.h::__to_fd]
```c
		file_s *file = files->fd_array[v];
		// return (fd_s){(file_s *)(v & ~3),v & 3};
		return (fd_s){file, 3};
	}
```

### A21 · fdput_pos

[VERIFIED mykernel/include/linux/fs/file.h::fdput_pos]
```c
	static inline void
	fdput_pos(fd_s f) {
		// if (f.flags & FDPUT_POS_UNLOCK)
		// 	__f_unlock_pos(f.file);
		fdput(f);
```

### A22 · fdput

[VERIFIED mykernel/include/linux/fs/file.h::fdput]
```c
	static inline void fdput(fd_s fd)
	{
		if (fd.flags & FDPUT_FPUT)
			fput(fd.file);
	}
```

### A23 · FDPUT_FPUT

[VERIFIED mykernel/include/linux/fs/file.h::FDPUT_FPUT]
```c
	#define FDPUT_FPUT			1
```

### A24 · files_struct_s

[VERIFIED mykernel/include/linux/kernel/fdtable.h::files_struct_s]
```c
		// unsigned long	open_fds_init[1];
		// unsigned long	full_fds_bits_init[1];
		unsigned long	fd_count;
		file_s __rcu	*fd_array[NR_OPEN_DEFAULT];
	} files_struct_s;
```

### A25 · NR_OPEN_DEFAULT

[VERIFIED mykernel/include/linux/kernel/fdtable.h::NR_OPEN_DEFAULT]
```c
	#define NR_OPEN_DEFAULT	256
```

### A26 · vfs_write

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	if (!(file->f_mode & FMODE_WRITE))
		return -EBADF;
	if (!(file->f_mode & FMODE_CAN_WRITE))
		return -EINVAL;
	if (unlikely(!access_ok(buf, count)))
```

### A27 · vfs_write

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	ret = rw_verify_area(WRITE, file, pos, count);
	if (ret)
		return ret;
	if (count > MAX_RW_COUNT)
		count =  MAX_RW_COUNT;
```

### A28 · vfs_write

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	if (file->f_op->write)
		ret = file->f_op->write(file, buf, count, pos);
	else if (file->f_op->write_iter)
		ret = new_sync_write(file, buf, count, pos);
	else
```

### A29 · ksys_write

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]
```c
ssize_t ksys_write(unsigned int fd, const char __user *buf, size_t count)
{
	fd_s f = fdget_pos(fd);
	ssize_t ret = -EBADF;
	file_s *filp = f.file;
```

### A30 · ksys_write

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]
```c
	if (filp != NULL) {
		loff_t pos, *ppos = file_ppos(filp);
		if (ppos) {
			pos = *ppos;
			ppos = &pos;
```

### A31 · FMODE_WRITE

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_WRITE]
```c
	#define FMODE_WRITE			((__force fmode_t)0x2)
```

### A32 · FMODE_CAN_WRITE

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_CAN_WRITE]
```c
	#define FMODE_CAN_WRITE			((__force fmode_t)0x40000)
```

### A33 · file_s

[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
		fmode_t				f_mode;
		atomic_long_t		f_count;
		// mutex_s				f_pos_lock;
		loff_t				f_pos;
		uint 				f_flags;
```

### A34 · file_ops_s

[VERIFIED mykernel/include/linux/fs/fs.h::file_ops_s]
```c
		loff_t		(*llseek) (file_s *file, loff_t pos, int);
		ssize_t		(*read) (file_s *file, char *buf, size_t size, loff_t *pos);
		ssize_t		(*write) (file_s *file, const char *buf, size_t size, loff_t *pos);
```

### A35 · ssize_t

[VERIFIED mykernel/include/linux/kernel/types.h::ssize_t]
```c
			typedef __kernel_ssize_t	ssize_t;
```

### A36 · rw_verify_area

[VERIFIED mykernel/fs/vfs/read_write.c::rw_verify_area]
```c
int rw_verify_area(int read_write, file_s *file, const loff_t *ppos, size_t count)
{
	return 0;

	if (likely((ssize_t) count < 0))
```


## B · 逐项write回调、符号累计与循环继续条件

### B01 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
vfs_writev(file_s *file, const iov_s *vec,
		ulong vlen, loff_t *pos, rwf_t flags) {

	iov_s iovstack[UIO_FASTIOV];
	iov_s *iov = iovstack;
```

### B02 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
	iov_iter_s iter;
	size_t tot_len;
	ssize_t ret = 0;

	if (!(file->f_mode & FMODE_WRITE))
```

### B03 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
	if (!(file->f_mode & FMODE_WRITE))
		return -EBADF;
	// if (!(file->f_mode & FMODE_CAN_WRITE))
	// 	return -EINVAL;
```

### B04 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
	// ret = import_iovec(ITER_SOURCE, vec, vlen,
	// 		ARRAY_SIZE(iovstack), &iov, &iter);
	// if (ret < 0)
	// 	return ret;

```

### B05 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
	// tot_len = iov_iter_count(&iter);
	// if (!tot_len)
	// 	goto out;

	// ret = rw_verify_area(WRITE, file, pos, tot_len);
```

### B06 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
	// ret = rw_verify_area(WRITE, file, pos, tot_len);
	// if (ret < 0)
	// 	goto out;

```

### B07 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
	// if (file->f_op->write_iter)
	// 	ret = do_iter_readv_writev(file, &iter, pos, WRITE, flags);
	// else
	// 	ret = do_loop_readv_writev(file, &iter, pos, WRITE, flags);
	// if (ret > 0)
```

### B08 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
	// if (ret > 0)
	// 	fsnotify_modify(file);
	// file_end_write(file);
// out:
	// kfree(iov);
```

### B09 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
// out:
	// kfree(iov);
	// return ret;

```

### B10 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
	for (int i = 0; i < vlen; i++)
		ret += file->f_op->write(file, vec[i].iov_base, vec[i].iov_len, pos);

	return ret;
```

### B11 · vfs_writev

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
		ret += file->f_op->write(file, vec[i].iov_base, vec[i].iov_len, pos);

	return ret;
```

### B12 · iov_s

[VERIFIED mykernel/include/uapi/linux/uio.h::iov_s]
```c
	typedef struct iovec {
		void __user     *iov_base;	/* BSD uses caddr_t (1003.1g requires void *) */
		__kernel_size_t iov_len; /* Must be size_t (1003.1g) */
	} iov_s;
```

### B13 · UIO_FASTIOV

[VERIFIED mykernel/include/uapi/linux/uio.h::UIO_FASTIOV]
```c
	#define UIO_FASTIOV	8
```

### B14 · UIO_MAXIOV

[VERIFIED mykernel/include/uapi/linux/uio.h::UIO_MAXIOV]
```c
	#define UIO_MAXIOV	1024
```

### B15 · ssize_t

[VERIFIED mykernel/include/linux/kernel/types.h::ssize_t]
```c
			typedef __kernel_ssize_t	ssize_t;
```

### B16 · file_ops_s

[VERIFIED mykernel/include/linux/fs/fs.h::file_ops_s]
```c
		loff_t		(*llseek) (file_s *file, loff_t pos, int);
		ssize_t		(*read) (file_s *file, char *buf, size_t size, loff_t *pos);
		ssize_t		(*write) (file_s *file, const char *buf, size_t size, loff_t *pos);
```

### B17 · do_writev

[VERIFIED mykernel/fs/vfs/read_write.c::do_writev]
```c
ssize_t do_writev(ulong fd, const iov_s __user *vec, ulong vlen, rwf_t flags)
{
	fd_s f = fdget_pos(fd);
	ssize_t ret = -EBADF;

```

### B18 · do_writev

[VERIFIED mykernel/fs/vfs/read_write.c::do_writev]
```c
		}
		ret = vfs_writev(f.file, vec, vlen, ppos, flags);
		if (ret >= 0 && ppos)
			f.file->f_pos = pos;
		fdput_pos(f);
```

### B19 · do_writev

[VERIFIED mykernel/fs/vfs/read_write.c::do_writev]
```c
			f.file->f_pos = pos;
		fdput_pos(f);
	}

```

### B20 · vfs_write

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	if (!(file->f_mode & FMODE_WRITE))
		return -EBADF;
	if (!(file->f_mode & FMODE_CAN_WRITE))
		return -EINVAL;
	if (unlikely(!access_ok(buf, count)))
```


## C · 局部f_pos提交、fd归还与FAT32_write消费者

### C01 · file_ppos

[VERIFIED mykernel/fs/vfs/read_write.c::file_ppos]
```c
static inline loff_t *file_ppos(file_s *file) {
	return file->f_mode & FMODE_STREAM ? NULL : &file->f_pos;
}
```

### C02 · do_writev

[VERIFIED mykernel/fs/vfs/read_write.c::do_writev]
```c
ssize_t do_writev(ulong fd, const iov_s __user *vec, ulong vlen, rwf_t flags)
{
	fd_s f = fdget_pos(fd);
	ssize_t ret = -EBADF;

```

### C03 · do_writev

[VERIFIED mykernel/fs/vfs/read_write.c::do_writev]
```c
	if (f.file) {
		loff_t pos, *ppos = file_ppos(f.file);
		if (ppos) {
			pos = *ppos;
			ppos = &pos;
```

### C04 · do_writev

[VERIFIED mykernel/fs/vfs/read_write.c::do_writev]
```c
		if (ppos) {
			pos = *ppos;
			ppos = &pos;
		}
		ret = vfs_writev(f.file, vec, vlen, ppos, flags);
```

### C05 · do_writev

[VERIFIED mykernel/fs/vfs/read_write.c::do_writev]
```c
		ret = vfs_writev(f.file, vec, vlen, ppos, flags);
		if (ret >= 0 && ppos)
			f.file->f_pos = pos;
		fdput_pos(f);
	}
```

### C06 · do_writev

[VERIFIED mykernel/fs/vfs/read_write.c::do_writev]
```c
			f.file->f_pos = pos;
		fdput_pos(f);
	}

	// if (ret > 0)
```

### C07 · fdput_pos

[VERIFIED mykernel/include/linux/fs/file.h::fdput_pos]
```c
	static inline void
	fdput_pos(fd_s f) {
		// if (f.flags & FDPUT_POS_UNLOCK)
		// 	__f_unlock_pos(f.file);
		fdput(f);
```

### C08 · fdput

[VERIFIED mykernel/include/linux/fs/file.h::fdput]
```c
	static inline void fdput(fd_s fd)
	{
		if (fd.flags & FDPUT_FPUT)
			fput(fd.file);
	}
```

### C09 · fput

[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
void fput(file_s *file)
{
	if (atomic_long_dec_and_test(&file->f_count)) {
		task_s *task = current;

```

### C10 · fput

[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
		// 	schedule_delayed_work(&delayed_fput_work, 1);

		kmem_cache_free(filp_cachep, file);
	}
}
```

### C11 · FMODE_STREAM

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_STREAM]
```c
	#define FMODE_STREAM			((__force fmode_t)0x200000)
```

### C12 · file_s

[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
		fmode_t				f_mode;
		atomic_long_t		f_count;
		// mutex_s				f_pos_lock;
		loff_t				f_pos;
		uint 				f_flags;
```

### C13 · file_ops_s

[VERIFIED mykernel/include/linux/fs/fs.h::file_ops_s]
```c
		loff_t		(*llseek) (file_s *file, loff_t pos, int);
		ssize_t		(*read) (file_s *file, char *buf, size_t size, loff_t *pos);
		ssize_t		(*write) (file_s *file, const char *buf, size_t size, loff_t *pos);
```

### C14 · FAT32_file_ops

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_file_ops]
```c
{
	.open			= FAT32_open,
	.read			= FAT32_read,
	.write			= FAT32_write,
	.llseek			= FAT32_lseek,
```

### C15 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
ssize_t FAT32_write(file_s *filp, const char *buf, size_t count, loff_t *position)
{
	FAT32_inode_info_s * finode = filp->f_path.dentry->d_inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = filp->f_path.dentry->d_inode->i_sb->private_sb_info;

```

### C16 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	u32 cluster = finode->first_cluster;
	u32 next_cluster = 0;
	unsigned long sector = 0;
	int i,length = 0;
	long ret_val = 0;
```

### C17 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	long ret_val = 0;
	long flags = 0;
	ulong cplen;
	int index = *position / fsbi->bytes_per_cluster;
	long offset = *position % fsbi->bytes_per_cluster;
```

### C18 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	ulong cplen;
	int index = *position / fsbi->bytes_per_cluster;
	long offset = *position % fsbi->bytes_per_cluster;
	char * buffer = (char *)kmalloc(fsbi->bytes_per_cluster, GFP_KERNEL);

```

### C19 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	if(!cluster)
	{
		cluster = FAT32_find_available_cluster(fsbi);
		flags = 1;
	}
```

### C20 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	else
		for(i = 0; i < index; i++)
			cluster = FAT32_read_FAT_Entry(fsbi, cluster);

	if(!cluster)
```

### C21 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	if(!cluster)
	{
		kfree(buffer);
		return -ENOSPC;
	}
```

### C22 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	if(flags)
	{
		finode->first_cluster = cluster;
		filp->f_path.dentry->d_inode->i_sb->s_op->write_inode(filp->f_path.dentry->d_inode);
		FAT32_write_FAT_Entry(fsbi, cluster, 0x0ffffff8);
```

### C23 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		finode->first_cluster = cluster;
		filp->f_path.dentry->d_inode->i_sb->s_op->write_inode(filp->f_path.dentry->d_inode);
		FAT32_write_FAT_Entry(fsbi, cluster, 0x0ffffff8);
	}
```

### C24 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	index = count;

	do
	{
		if(!flags)
```

### C25 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		if(!flags)
		{
			memset(buffer, 0, fsbi->bytes_per_cluster);
			sector = fsbi->Data_firstsector + (cluster - 2) * fsbi->sector_per_cluster;
			if(ROOTBLK_TRANSFER(CMD_READ, sector,
```

### C26 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
			if(ROOTBLK_TRANSFER(CMD_READ, sector,
					fsbi->sector_per_cluster, (unchar *)buffer))
			{
				color_printk(RED, BLACK, "FAT32 FS(write) read disk ERROR!!!!!!!!!!\n");
				ret_val = -EIO;
```

### C27 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
				ret_val = -EIO;
				break;
			}
		}

```

### C28 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		length = index <= fsbi->bytes_per_cluster - offset ? index : fsbi->bytes_per_cluster - offset;

		if((unsigned long)buf < TASK_SIZE_MAX)
			cplen = copy_from_user(buffer + offset, (void *)buf, length);
		else
```

### C29 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		if((unsigned long)buf < TASK_SIZE_MAX)
			cplen = copy_from_user(buffer + offset, (void *)buf, length);
		else
			memcpy(buffer + offset, buf, length);

```

### C30 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		if(ROOTBLK_TRANSFER(CMD_WRITE, sector,
				fsbi->sector_per_cluster, (unchar *)buffer))
		{
			color_printk(RED, BLACK, "FAT32 FS(write) write disk ERROR!!!!!!!!!!\n");
			ret_val = -EIO;
```

### C31 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
			ret_val = -EIO;
			break;
		}

		index -= length;
```

### C32 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		index -= length;
		buf += length;
		offset -= offset;
		*position += length;

```

### C33 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		if(index)
			next_cluster = FAT32_read_FAT_Entry(fsbi, cluster);
		else
			break;

```

### C34 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		if(next_cluster >= 0x0ffffff8)
		{
			next_cluster = FAT32_find_available_cluster(fsbi);
			if(!next_cluster)
			{
```

### C35 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
			next_cluster = FAT32_find_available_cluster(fsbi);
			if(!next_cluster)
			{
				kfree(buffer);
				return -ENOSPC;
```

### C36 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
				
			FAT32_write_FAT_Entry(fsbi, cluster, next_cluster);
			FAT32_write_FAT_Entry(fsbi, next_cluster, 0x0ffffff8);
			cluster = next_cluster;
			flags = 1;
```

### C37 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	}while(index);

	if(*position > filp->f_path.dentry->d_inode->i_size)
	{
		filp->f_path.dentry->d_inode->i_size = *position;
```

### C38 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	if(*position > filp->f_path.dentry->d_inode->i_size)
	{
		filp->f_path.dentry->d_inode->i_size = *position;
		filp->f_path.dentry->d_inode->i_sb->s_op->write_inode(filp->f_path.dentry->d_inode);
	}
```

### C39 · FAT32_write

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	kfree(buffer);
	if(!index)
		ret_val = count;
	return ret_val;
```

### C40 · copy_from_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::copy_from_user]
```c
	static __always_inline ulong __must_check
	copy_from_user(void *to, const void __user *from, ulong n) {
		// if (!check_copy_size(to, n, false))
		// 	return n;
	// #ifdef INLINE_COPY_FROM_USER
```

### C41 · _inline_copy_from_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_from_user]
```c
	_inline_copy_from_user(void *to, const void __user *from, ulong n) {
		ulong res = n;
		// might_fault();
		// if (!should_fail_usercopy() && likely(access_ok(from, n))) {
		// 	/*
```

### C42 · _inline_copy_from_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_from_user]
```c
		// 	barrier_nospec();
		// 	instrument_copy_from_user_before(to, from, n);
			res = raw_copy_from_user(to, from, n);
		// 	instrument_copy_from_user_after(to, from, n, res);
		// }
```

