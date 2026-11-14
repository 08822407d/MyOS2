---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-DIRECTORY-ENUMERATION-59
record_type: fixed_time_direct_source_anchors
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
base_snapshot: time
source_pin_short12: a039d9803ade
read_channel: GitHub_connector
status: READY_FOR_MAINLINE_REVIEW
runtime_evidence: NOT_RUN
---
# 第59批源码证据：D00／A／B／C

所有锚点均为固定time源码中实际函数/结构/单宏定义的连续1–5行。getdents64以真实唯一`MYOS_SYSCALL_DEFINE3(getdents64)`宏调用体定位，非预处理后的ABI证明。保持连接器返回的原tab与CRLF。不能仅以标签数量或片段字面存在替代命题证成和最早停止点审查。

## D00 · R58-01替代引文

### D01 · LOOKUP_CREATE

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_CREATE]
```c
	#define LOOKUP_CREATE			0x0200	/* ... in object creation */
```

### D02 · LOOKUP_EXCL

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_EXCL]
```c
	#define LOOKUP_EXCL				0x0400	/* ... in exclusive creation */
```

### D03 · init_mkdir

[VERIFIED mykernel/fs/vfs/init.c::init_mkdir]
```c
	dentry = kern_path_create(AT_FDCWD, pathname, &path, LOOKUP_DIRECTORY);
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);
	error = vfs_mkdir(path.dentry->d_inode, dentry, mode);
	done_path_create(&path, dentry);
```


## A · 第59批枚举业务锚点

### A01 · getdents_cbk64_s

[VERIFIED mykernel/fs/vfs/readdir.c::getdents_cbk64_s]
```c
typedef struct getdents_callback64 {
	dir_ctxt_s ctx;
	linux_dirent64_s *current_dir;
	int prev_reclen;
	int count;
```

### A02 · getdents_cbk64_s

[VERIFIED mykernel/fs/vfs/readdir.c::getdents_cbk64_s]
```c
	linux_dirent64_s *current_dir;
	int prev_reclen;
	int count;
	int error;
} getdents_cbk64_s;
```

### A03 · iterate_dir

[VERIFIED mykernel/fs/vfs/readdir.c::iterate_dir]
```c
int iterate_dir(file_s *file, dir_ctxt_s *ctx)
{
	bool shared = false;
	int res = -ENOTDIR;
	if (file->f_op->iterate_shared)
```

### A04 · iterate_dir

[VERIFIED mykernel/fs/vfs/readdir.c::iterate_dir]
```c
	ctx->pos = file->f_pos;
	if (shared)
		res = file->f_op->iterate_shared(file, ctx);
	file->f_pos = ctx->pos;
```

### A05 · iterate_dir

[VERIFIED mykernel/fs/vfs/readdir.c::iterate_dir]
```c
out:
	return res;
}
```

### A06 · MYOS_SYSCALL_DEFINE3(getdents64)

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
MYOS_SYSCALL_DEFINE3(getdents64, unsigned int, fd,
		linux_dirent64_s *, dirent, unsigned int, count)
{
	fd_s f;
	getdents_cbk64_s buf = {
```

### A07 · MYOS_SYSCALL_DEFINE3(getdents64)

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
	getdents_cbk64_s buf = {
		.ctx.actor = filldir64,
		.count = count,
		.current_dir = dirent
	};
```

### A08 · MYOS_SYSCALL_DEFINE3(getdents64)

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
	f = myos_fdget_pos(fd);
	if (!f.file)
		return -EBADF;
```

### A09 · MYOS_SYSCALL_DEFINE3(getdents64)

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
	// error = iterate_dir(f.file, &buf.ctx);
	// int iterate_dir(file_s *file, dir_ctxt_s *ctx)
	// {
		buf.ctx.pos = f.file->f_pos;
		error = f.file->f_op->iterate_shared(f.file, &buf.ctx);
```

### A10 · MYOS_SYSCALL_DEFINE3(getdents64)

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
		buf.ctx.pos = f.file->f_pos;
		error = f.file->f_op->iterate_shared(f.file, &buf.ctx);
		f.file->f_pos = buf.ctx.pos;
	// }
```

### A11 · MYOS_SYSCALL_DEFINE3(getdents64)

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
	if (error >= 0)
		error = buf.error;
	if (buf.prev_reclen) {
```

### A12 · MYOS_SYSCALL_DEFINE3(getdents64)

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
	// 	lastdirent = (void *) buf.current_dir - buf.prev_reclen;
	// 	if (put_user(d_off, &lastdirent->d_off))
	// 		error = -EFAULT;
	// 	else
			error = count - buf.count;
```

### A13 · MYOS_SYSCALL_DEFINE3(getdents64)

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
			error = count - buf.count;
	}
	fdput_pos(f);
	return error;
}
```

### A14 · myos_fdget_pos

[VERIFIED mykernel/fs/vfs/file.c::myos_fdget_pos]
```c
fd_s myos_fdget_pos(int fd)
{
	task_s *curr = current;
	file_s *fp = curr->files->fd_array[fd];
```

### A15 · myos_fdget_pos

[VERIFIED mykernel/fs/vfs/file.c::myos_fdget_pos]
```c
	file_s *fp = curr->files->fd_array[fd];
	
	return (fd_s){.file = fp, .flags = 0};
}
```

### A16 · fd_s

[VERIFIED mykernel/include/linux/fs/file.h::fd_s]
```c
	typedef struct fd {
		file_s		*file;
		uint		flags;
	} fd_s;
```

### A17 · fdput_pos

[VERIFIED mykernel/include/linux/fs/file.h::fdput_pos]
```c
	static inline void
	fdput_pos(fd_s f) {
		// if (f.flags & FDPUT_POS_UNLOCK)
		// 	__f_unlock_pos(f.file);
		fdput(f);
```

### A18 · fdput

[VERIFIED mykernel/include/linux/fs/file.h::fdput]
```c
	static inline void fdput(fd_s fd)
	{
		if (fd.flags & FDPUT_FPUT)
			fput(fd.file);
	}
```

### A19 · FDPUT_FPUT

[VERIFIED mykernel/include/linux/fs/file.h::FDPUT_FPUT]
```c
	#define FDPUT_FPUT			1
```

### A20 · file_s

[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
		fmode_t				f_mode;
		atomic_long_t		f_count;
		// mutex_s				f_pos_lock;
		loff_t				f_pos;
		uint 				f_flags;
```

### A21 · file_ops_s

[VERIFIED mykernel/include/linux/fs/fs.h::file_ops_s]
```c
		// int			(*iopoll)(kiocb_s *kiocb, struct io_comp_batch *,
		// 					unsigned int flags);
		int			(*iterate_shared) (file_s *, dir_ctxt_s *);
		// __poll_t	(*poll) (file_s *, struct poll_table_struct *);
```


## B · 第59批枚举业务锚点

### B01 · FAT32_getdents64

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_getdents64]
```c
int FAT32_getdents64(file_s *filp, dir_ctxt_s *ctx)
{
	if (ctx->pos == -1)
		return 0;
```

### B02 · FAT32_getdents64

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_getdents64]
```c
	int error = 0;
	const msdos_dirent_s *tmpde = NULL;
	const msdos_dirslot_s *tmplde = NULL;
	inode_s *inode = filp->f_path.dentry->d_inode;
	FAT32_iobuf_s *iobuf = FAT32_iobuf_init(inode);
```

### B03 · FAT32_getdents64

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_getdents64]
```c
	inode_s *inode = filp->f_path.dentry->d_inode;
	FAT32_iobuf_s *iobuf = FAT32_iobuf_init(inode);
	iobuf->iter_init(iobuf);
	// iterate all fat32 entries in this cluster
	while (!IS_ERR(tmpde = FAT32_get_full_ent(iobuf)))
```

### B04 · FAT32_getdents64

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_getdents64]
```c
	while (!IS_ERR(tmpde = FAT32_get_full_ent(iobuf)))
	{
		tmplde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf,
				iobuf->iter_cursor - 2 * sizeof(msdos_dirent_s));

```

### B05 · FAT32_getdents64

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_getdents64]
```c
		char *name = NULL;
		int namelen = 0;

		if (FAT32_ent_empty(tmpde))
			continue;
```

### B06 · FAT32_getdents64

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_getdents64]
```c
		else if(!IS_ERR(tmplde) &&
			tmplde->attr == ATTR_LONG_NAME &&
			!FAT32_ent_empty((msdos_dirent_s *)tmplde))
			name = FAT32_parse_long(&namelen, iobuf);
		else
```

### B07 · FAT32_getdents64

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_getdents64]
```c
			name = FAT32_parse_long(&namelen, iobuf);
		else
			name = FAT32_parse_short(&namelen, tmpde);

		error = ctx->actor(ctx, name, namelen, 0, 0, 0);
```

### B08 · FAT32_getdents64

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_getdents64]
```c
		error = ctx->actor(ctx, name, namelen, 0, 0, 0);

		kfree(name);
		if (error !=0)
			break;
```

### B09 · FAT32_getdents64

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_getdents64]
```c
	if (error == 0)
		ctx->pos = -1;
	
	FAT32_iobuf_release(iobuf);
	return error;
```

### B10 · FAT32_file_ops

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_file_ops]
```c
	.read_iter		= generic_file_read_iter,
	.write_iter		= generic_file_write_iter,
	.mmap			= generic_file_mmap,
	.iterate_shared	= FAT32_getdents64,
};
```

### B11 · FAT32_iobuf_iter_init

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_iter_init]
```c
int FAT32_iobuf_iter_init(FAT32_iobuf_s *iobuf)
{
	iobuf->iter_cursor = 0;
	iobuf->iter_reach_end = false;
	return -ENOERR;
```

### B12 · FAT32_iobuf_iter_next

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_iter_next]
```c
	const msdos_dirent_s *ret_val = FAT32_iobuf_readent(iobuf, iobuf->iter_cursor);

	iobuf->iter_cursor += sizeof(msdos_dirent_s);
	if (iobuf->iter_cursor >= bufend)
		iobuf->iter_reach_end = true;
```

### B13 · FAT32_iobuf_readent

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
{
	loff_t	bufend = iobuf->nr_clus * iobuf->bufsize;
	if (off >= bufend || off < 0)
		return ERR_PTR(-ENOENT);
	int	bufidx = off / iobuf->bufsize;
```

### B14 · FAT32_get_full_ent

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_get_full_ent]
```c
		ret_val = iobuf->next(iobuf);
		tmpval = (msdos_dirslot_s *)ret_val;
	}
	while ((ret_val->attr == ATTR_LONG_NAME ||
			FAT32_ent_empty(ret_val)) &&
```

### B15 · FAT32_get_full_ent

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_get_full_ent]
```c
	if (iobuf->iter_reach_end)
		ret_val = ERR_PTR(-ENOENT);

	return ret_val;
```

### B16 · FAT32_parse_short

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_short]
```c
char *FAT32_parse_short(int *namelen, const msdos_dirent_s *de)
{
	int x;
	*namelen = 0;
	char *ret_val = kzalloc(15, GFP_KERNEL);
```

### B17 · FAT32_parse_short

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_short]
```c
		if(x == 8)
			ret_val[--(*namelen)] = 0;
	}

	return ret_val;
```

### B18 · FAT32_parse_long

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
	int i = 0, x, y;
	char *ret_val;
	int entlen = sizeof(msdos_dirent_s);
	loff_t cursor = iobuf->iter_cursor - entlen * 2;
	msdos_dirslot_s *lde_tmp = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf, cursor);
```

### B19 · FAT32_parse_long

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
	ret_val = kzalloc(i * 13 + 1, GFP_KERNEL);
	cursor = iobuf->iter_cursor - entlen * 2;
	msdos_dirslot_s *lde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf, cursor);
	for(x = 0; x < i; x++, cursor -= entlen, lde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf, cursor))
	{
```

### B20 · FAT32_ent_empty

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_ent_empty]
```c
{
	return (de->name[0] == 0xe5 ||
			de->name[0] == 0x00 ||
			de->name[0] == 0x05);
}
```

### B21 · FAT32_iobuf_release

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_release]
```c
		kfree(iobuf->clusters);
		kfree(iobuf->flags);
	}
	kfree(iobuf);
}
```

### B22 · FAT32_getdents64

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_getdents64]
```c
		error = ctx->actor(ctx, name, namelen, 0, 0, 0);
```


## C · 第59批枚举业务锚点

### C01 · verify_dirent_name

[VERIFIED mykernel/fs/vfs/readdir.c::verify_dirent_name]
```c
static int verify_dirent_name(const char *name, int len)
{
	if (len <= 0 || len >= PATH_MAX)
		return -EIO;
	if (memchr(name, '/', len))
```

### C02 · verify_dirent_name

[VERIFIED mykernel/fs/vfs/readdir.c::verify_dirent_name]
```c
	if (memchr(name, '/', len))
		return -EIO;
	return 0;
}
```

### C03 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
static int filldir64(dir_ctxt_s *ctx, const char *name,
		int namelen, loff_t offset, u64 ino, unsigned int d_type)
{
	ulong cplen;
	linux_dirent64_s *dirent, *prev;
```

### C04 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	getdents_cbk64_s *buf =
			container_of(ctx, getdents_cbk64_s, ctx);
	int reclen = ALIGN(offsetof(linux_dirent64_s, d_name) + namelen + 1,
			sizeof(u64));
	int prev_reclen = 0;
```

### C05 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	buf->error = verify_dirent_name(name, namelen);
	if (unlikely(buf->error))
		return buf->error;
	buf->error = -EINVAL;	/* only used if we fail.. */
	if (reclen > buf->count)
```

### C06 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	buf->error = -EINVAL;	/* only used if we fail.. */
	if (reclen > buf->count)
		return -EINVAL;
	prev_reclen = buf->prev_reclen;
	dirent = buf->current_dir;
```

### C07 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	prev_reclen = buf->prev_reclen;
	dirent = buf->current_dir;
	prev = (void *)dirent - prev_reclen;

	prev->d_off = offset;
```

### C08 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	prev->d_off = offset;
	dirent->d_ino = ino;
	dirent->d_reclen = reclen;
	dirent->d_type = d_type;
	cplen = copy_to_user(dirent->d_name, (char *)name, namelen);
```

### C09 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	cplen = copy_to_user(dirent->d_name, (char *)name, namelen);
	dirent->d_name[namelen] = '\0';

	buf->prev_reclen = reclen;
	buf->current_dir = (void *)dirent + reclen;
```

### C10 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	buf->prev_reclen = reclen;
	buf->current_dir = (void *)dirent + reclen;
	buf->count -= reclen;
	return 0;
```

### C11 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
// efault_end:
// 	user_write_access_end();
efault:
	buf->error = -EFAULT;
	return -EFAULT;
```

### C12 · linux_dirent64_s

[VERIFIED mykernel/include/linux/kernel/dirent.h::linux_dirent64_s]
```c
	typedef struct linux_dirent64 {
		u64		d_ino;
		s64		d_off;
		unsigned short	d_reclen;
		unsigned char	d_type;
```

### C13 · linux_dirent64_s

[VERIFIED mykernel/include/linux/kernel/dirent.h::linux_dirent64_s]
```c
		unsigned short	d_reclen;
		unsigned char	d_type;
		char	d_name[];
	} linux_dirent64_s;
```

### C14 · dir_ctxt_s

[VERIFIED mykernel/include/linux/fs/fs.h::dir_ctxt_s]
```c
	typedef struct dir_context {
		filldir_t actor;
		loff_t pos;
	} dir_ctxt_s;
```

### C15 · filldir_t

[VERIFIED mykernel/include/linux/fs/fs.h::filldir_t]
```c
	typedef int (*filldir_t)(dir_ctxt_s* ctx, const char * name, int namelen,
					loff_t offset, u64 ino, unsigned int d_type);
```

### C16 · offsetof

[VERIFIED mykernel/include/linux/kernel/stddef.h::offsetof]
```c
	#	define offsetof(TYPE, MEMBER)	__compiler_offsetof(TYPE, MEMBER)
```

### C17 · ALIGN

[VERIFIED mykernel/include/linux/kernel/align.h::ALIGN]
```c
	#define ALIGN(x, a)				__ALIGN_KERNEL((x), (a))
```

### C18 · PATH_MAX

[VERIFIED mykernel/include/uapi/linux/limits.h::PATH_MAX]
```c
	#define PATH_MAX		4096	/* # chars in a path name including nul */
```

### C19 · _inline_copy_to_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_to_user]
```c
	static inline __must_check ulong
	_inline_copy_to_user(void __user *to, const void *from, ulong n) {
		// might_fault();
		// if (should_fail_usercopy())
		// 	return n;
```

### C20 · _inline_copy_to_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_to_user]
```c
		if (access_ok(to, n)) {
			instrument_copy_to_user(to, from, n);
			n = raw_copy_to_user(to, from, n);
		}
		return n;
```

### C21 · copy_to_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::copy_to_user]
```c
	copy_to_user(void __user *to, const void *from, ulong n) {
		// if (!check_copy_size(from, n, true))
		// 	return n;
	// #ifdef INLINE_COPY_TO_USER
		return _inline_copy_to_user(to, from, n);
```

### C22 · getdents_cbk64_s

[VERIFIED mykernel/fs/vfs/readdir.c::getdents_cbk64_s]
```c
	dir_ctxt_s ctx;
	linux_dirent64_s *current_dir;
	int prev_reclen;
	int count;
	int error;
```

### C23 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	getdents_cbk64_s *buf =
			container_of(ctx, getdents_cbk64_s, ctx);
	int reclen = ALIGN(offsetof(linux_dirent64_s, d_name) + namelen + 1,
			sizeof(u64));
```

### C24 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
		return buf->error;
	buf->error = -EINVAL;	/* only used if we fail.. */
	if (reclen > buf->count)
		return -EINVAL;
```

### C25 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	dirent->d_type = d_type;
	cplen = copy_to_user(dirent->d_name, (char *)name, namelen);
	dirent->d_name[namelen] = '\0';

	buf->prev_reclen = reclen;
```

### C26 · MYOS_SYSCALL_DEFINE3(getdents64)

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
	if (error >= 0)
		error = buf.error;
```

