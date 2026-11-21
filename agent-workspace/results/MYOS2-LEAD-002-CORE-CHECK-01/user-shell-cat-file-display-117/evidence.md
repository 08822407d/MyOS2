# 第117批固定time新增P2源码引文

新增P2仅取固定time真实函数、宏定义及聚合初始化中的连续1–5物理行。映射调用实例、原型、整函数否定结论和条件编译前提另外列为POSITION_ONLY；不改写历史115/116。

### CT117-E001 [D_KSYS_READ_FD]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_read]
```c
ssize_t ksys_read(uint fd, char __user *buf, size_t count)
{
	fd_s f = myos_fdget_pos(fd);
	ssize_t ret = -EBADF;

```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L180–L184 · D_KSYS_READ_FD；仅静态源码。

### CT117-E002 [D_KSYS_READ_CALL]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_read]
```c
		ret = vfs_read(f.file, buf, count, ppos);
		if (ret >= 0 && ppos)
			f.file->f_pos = pos;
		fdput_pos(f);
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L191–L194 · D_KSYS_READ_CALL；仅静态源码。

### CT117-E003 [D_KSYS_WRITE_FD]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]
```c
ssize_t ksys_write(unsigned int fd, const char __user *buf, size_t count)
{
	fd_s f = fdget_pos(fd);
	ssize_t ret = -EBADF;
	file_s *filp = f.file;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L319–L323 · D_KSYS_WRITE_FD；仅静态源码。

### CT117-E004 [D_KSYS_WRITE_CALL]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]
```c
		ret = vfs_write(filp, buf, count, ppos);
		if (ret >= 0 && ppos)
			filp->f_pos = pos;
		fdput_pos(f);
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L331–L334 · D_KSYS_WRITE_CALL；仅静态源码。

### CT117-E005 [D_VERIFY_ZERO]

[VERIFIED mykernel/fs/vfs/read_write.c::rw_verify_area]
```c
int rw_verify_area(int read_write, file_s *file, const loff_t *ppos, size_t count)
{
	return 0;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L49–L51 · D_VERIFY_ZERO；仅静态源码。

### CT117-E006 [D_VFS_READ_LIMIT]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_read]
```c
	ret = rw_verify_area(READ, file, pos, count);
	if (ret)
		return ret;
	if (count > MAX_RW_COUNT)
		count =  MAX_RW_COUNT;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L154–L158 · D_VFS_READ_LIMIT；仅静态源码。

### CT117-E007 [D_VFS_WRITE_LIMIT]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	ret = rw_verify_area(WRITE, file, pos, count);
	if (ret)
		return ret;
	if (count > MAX_RW_COUNT)
		count =  MAX_RW_COUNT;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L298–L302 · D_VFS_WRITE_LIMIT；仅静态源码。

### CT117-E008 [D_VFS_READ_METHOD]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_read]
```c
	if (file->f_op->read_iter)
		ret = new_sync_read(file, buf, count, pos);
	else if (file->f_op->read)
		ret = file->f_op->read(file, buf, count, pos);
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L160–L163 · D_VFS_READ_METHOD；仅静态源码。

### CT117-E009 [D_VFS_READ_MISSING]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_read]
```c
	else
		ret = -EINVAL;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L164–L165 · D_VFS_READ_MISSING；仅静态源码。

### CT117-E010 [D_VFS_WRITE_METHOD]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	if (file->f_op->write)
		ret = file->f_op->write(file, buf, count, pos);
	else if (file->f_op->write_iter)
		ret = new_sync_write(file, buf, count, pos);
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L304–L307 · D_VFS_WRITE_METHOD；仅静态源码。

### CT117-E011 [D_VFS_WRITE_MISSING]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	else
		ret = -EINVAL;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L308–L309 · D_VFS_WRITE_MISSING；仅静态源码。

### CT117-E012 [D_ITER_READ_BUG]

[VERIFIED mykernel/fs/vfs/read_write.c::new_sync_read]
```c
	ret = filp->f_op->read_iter(&kiocb, &iter);
	BUG_ON(ret == -EIOCBQUEUED);
	if (ppos)
		*ppos = kiocb.ki_pos;
	return ret;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L88–L92 · D_ITER_READ_BUG；仅静态源码。

### CT117-E013 [D_ITER_WRITE_BUG]

[VERIFIED mykernel/fs/vfs/read_write.c::new_sync_write]
```c
	ret = filp->f_op->write_iter(&kiocb, &iter);
	BUG_ON(ret == -EIOCBQUEUED);
	if (ret > 0 && ppos)
		*ppos = kiocb.ki_pos;
	return ret;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L212–L216 · D_ITER_WRITE_BUG；仅静态源码。

### CT117-E014 [D_BUG_MACRO]

[VERIFIED mykernel/include/linux/kernel/asm-generic/bug.h::BUG_ON]
```c
	#			define BUG_ON(condition) do { if (unlikely(condition)) BUG(); } while (0)
```

固定time@a039d9803ade · mykernel/include/linux/kernel/asm-generic/bug.h#L66–L66 · D_BUG_MACRO；仅静态源码。

### CT117-E015 [D_ZERO_FOPS]

[VERIFIED mykernel/drivers/char/mem.c::zero_fops]
```c
static const file_ops_s zero_fops = {
		.llseek		= zero_lseek,
		.write		= write_zero,
	// 	.read_iter	= read_iter_zero,
		.read		= read_zero,
```

固定time@a039d9803ade · mykernel/drivers/char/mem.c#L141–L145 · D_ZERO_FOPS；仅静态源码。

### CT117-E016 [D_ZERO_LOOP]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
	size_t cleared = 0;

	while (count) {
		size_t chunk = min_t(size_t, count, PAGE_SIZE);
```

固定time@a039d9803ade · mykernel/drivers/char/mem.c#L85–L88 · D_ZERO_LOOP；仅静态源码。

### CT117-E017 [D_ZERO_MEMSET]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
		memset(buf + cleared, 0, chunk);
		count -= chunk;
		if (count == 0)
			break;
```

固定time@a039d9803ade · mykernel/drivers/char/mem.c#L90–L93 · D_ZERO_MEMSET；仅静态源码。

### CT117-E018 [D_ZERO_ACCUM]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
		cleared += chunk;
```

固定time@a039d9803ade · mykernel/drivers/char/mem.c#L95–L95 · D_ZERO_ACCUM；仅静态源码。

### CT117-E019 [D_ZERO_RETURN]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
	return cleared;
```

固定time@a039d9803ade · mykernel/drivers/char/mem.c#L108–L108 · D_ZERO_RETURN；仅静态源码。

### CT117-E020 [D_RUN_CMD]

[VERIFIED myinitramfs/myshell/myshell.c::run_cmd]
```c
void run_cmd(int index, int argc, char **argv)
{
	shell_internal_cmd[index].function(argc, argv);
}
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L139–L142 · D_RUN_CMD；仅静态源码。

### CT117-E021 [D_OPENDIR_GATE]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::opendir]
```c
DIR *opendir(const char *name)
{
	if (invalid_name(name))
		return NULL;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L63–L66 · D_OPENDIR_GATE；仅静态源码。

### CT117-E022 [D_OPENDIR_OPEN]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::opendir]
```c
	int fd = open(name, opendir_oflags, O_RDONLY);

	return __alloc_dir(fd, true, 0);
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L68–L70 · D_OPENDIR_OPEN；仅静态源码。

### CT117-E023 [D_DIR_ALLOC]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::__alloc_dir]
```c
	size_t allocation = allocation_size;

	DIR *dirp = (DIR *)malloc(sizeof(DIR) + allocation);
	if (dirp == NULL)
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L100–L103 · D_DIR_ALLOC；仅静态源码。

### CT117-E024 [D_DIR_FAIL]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::__alloc_dir]
```c
	if (dirp == NULL)
	{
		// if (close_fd)
		// 	__close_nocancel_nostatus(fd);
		return NULL;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L103–L107 · D_DIR_FAIL；仅静态源码。

### CT117-E025 [D_DIR_INIT]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::__alloc_dir]
```c
	dirp->allocation = allocation;
	dirp->size = 0;
	dirp->offset = 0;
	dirp->filepos = 0;
	dirp->errcode = 0;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L114–L118 · D_DIR_INIT；仅静态源码。

### CT117-E026 [D_READDIR_GATE]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c::readdir]
```c
		if (dirp->offset >= dirp->size)
		{
			/* We've emptied out our buffer.  Refill it.  */

			size_t maxread = dirp->allocation;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L39–L43 · D_READDIR_GATE；仅静态源码。

### CT117-E027 [D_READDIR_CALL]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c::readdir]
```c
			if (maxread > INT_MAX)
				maxread = INT_MAX;
			bytes = getdents64(dirp->fd, dirp->data, maxread);
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L47–L49 · D_READDIR_CALL；仅静态源码。

### CT117-E028 [D_READDIR_NONPOS]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c::readdir]
```c
			bytes = getdents64(dirp->fd, dirp->data, maxread);
			if (bytes <= 0)
			{
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L49–L51 · D_READDIR_NONPOS；仅静态源码。

### CT117-E029 [D_READDIR_NULL]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c::readdir]
```c
				// dp = NULL;
				// break;
				return NULL;
			}
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L61–L64 · D_READDIR_NULL；仅静态源码。

### CT117-E030 [D_READDIR_DATA]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c::readdir]
```c
		dp = (struct dirent64 *)&dirp->data[dirp->offset];

		reclen = dp->d_reclen;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L71–L73 · D_READDIR_DATA；仅静态源码。

### CT117-E031 [D_READDIR_ADVANCE]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c::readdir]
```c
		reclen = dp->d_reclen;

		dirp->offset += reclen;

		dirp->filepos = dp->d_off;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L73–L77 · D_READDIR_ADVANCE；仅静态源码。

### CT117-E032 [D_GETDENTS_INIT]

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
	fd_s f;
	getdents_cbk64_s buf = {
		.ctx.actor = filldir64,
		.count = count,
		.current_dir = dirent
```

固定time@a039d9803ade · mykernel/fs/vfs/readdir.c#L135–L139 · D_GETDENTS_INIT；仅静态源码。

### CT117-E033 [D_GETDENTS_POS]

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
		buf.ctx.pos = f.file->f_pos;
		error = f.file->f_op->iterate_shared(f.file, &buf.ctx);
		f.file->f_pos = buf.ctx.pos;
```

固定time@a039d9803ade · mykernel/fs/vfs/readdir.c#L150–L152 · D_GETDENTS_POS；仅静态源码。

### CT117-E034 [D_GETDENTS_RETBRANCH]

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
	if (error >= 0)
		error = buf.error;
	if (buf.prev_reclen) {
```

固定time@a039d9803ade · mykernel/fs/vfs/readdir.c#L155–L157 · D_GETDENTS_RETBRANCH；仅静态源码。

### CT117-E035 [D_GETDENTS_RETURN]

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
			error = count - buf.count;
	}
	fdput_pos(f);
	return error;
```

固定time@a039d9803ade · mykernel/fs/vfs/readdir.c#L165–L168 · D_GETDENTS_RETURN；仅静态源码。

### CT117-E036 [D_FILL_PREV]

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	prev_reclen = buf->prev_reclen;
	dirent = buf->current_dir;
	prev = (void *)dirent - prev_reclen;

	prev->d_off = offset;
```

固定time@a039d9803ade · mykernel/fs/vfs/readdir.c#L109–L113 · D_FILL_PREV；仅静态源码。

### CT117-E037 [D_FILL_NOW]

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	dirent->d_ino = ino;
	dirent->d_reclen = reclen;
	dirent->d_type = d_type;
	cplen = copy_to_user(dirent->d_name, (char *)name, namelen);
	dirent->d_name[namelen] = '\0';
```

固定time@a039d9803ade · mykernel/fs/vfs/readdir.c#L114–L118 · D_FILL_NOW；仅静态源码。

### CT117-E038 [D_FILL_ACCOUNT]

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	buf->prev_reclen = reclen;
	buf->current_dir = (void *)dirent + reclen;
	buf->count -= reclen;
	return 0;
```

固定time@a039d9803ade · mykernel/fs/vfs/readdir.c#L120–L123 · D_FILL_ACCOUNT；仅静态源码。

### CT117-E039 [D_FAT_ACTOR]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_getdents64]
```c
		error = ctx->actor(ctx, name, namelen, 0, 0, 0);

		kfree(name);
		if (error !=0)
			break;
```

固定time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L340–L344 · D_FAT_ACTOR；仅静态源码。

### CT117-E040 [D_INVALID_TRUE]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::invalid_name]
```c
	if (name[0] == '\0')
	{
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L42–L43 · D_INVALID_TRUE；仅静态源码。

### CT117-E041 [D_INVALID_FALSE]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::invalid_name]
```c
	return false;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L49–L49 · D_INVALID_FALSE；仅静态源码。

### CT117-E042 [D_SYSMACRO1]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::SYSFUNC_DEF]
```c
#define SYSFUNC_DEF(name)						\
			_SYSFUNC_DEF_(name, __NR_##name)
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/syscall/syscall.c#L13–L14 · D_SYSMACRO1；仅静态源码。

### CT117-E043 [D_SYSMACRO2]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::_SYSFUNC_DEF_]
```c
#define _SYSFUNC_DEF_(name, nr)					\
			__SYSFUNC_DEF__(name, nr)
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/syscall/syscall.c#L16–L17 · D_SYSMACRO2；仅静态源码。

### CT117-E044 [D_SYSMACRO3]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::__SYSFUNC_DEF__]
```c
#define __SYSFUNC_DEF__(name, nr)							\
			__asm__	(										\
				".global	"#name"					\n\t"	\
				".type		"#name",	@function	\n\t"	\
				#name":								\n\t"	\
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/syscall/syscall.c#L19–L23 · D_SYSMACRO3；仅静态源码。

### CT117-E045 [D_SYSMACRO_NUM]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::__SYSFUNC_DEF__]
```c
				"pushq	%rbp						\n\t"	\
				"movq	%rsp,	%rbp				\n\t"	\
				"movq	$"#nr",	%rax				\n\t"	\
				"jmp	LABEL_SYSCALL				\n\t"	\
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/syscall/syscall.c#L24–L27 · D_SYSMACRO_NUM；仅静态源码。

### CT117-E046 [D_SYSCALL_INSTR]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::LABEL_SYSCALL]
```c
__asm__	(
	"LABEL_SYSCALL:							\n\t"
	"leaq	sysexit_ret_addr(%rip),	%rcx	\n\t"
	"syscall								\n\t"
	"sysexit_ret_addr:						\n\t"
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/syscall/syscall.c#L70–L74 · D_SYSCALL_INSTR；仅静态源码。

### CT117-E047 [D_SYSCALL_TEST]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::sysexit_ret_addr]
```c
	"cmpq	$-0x1000,	%rax				\n\t"
	"jb	LABEL_SYSCALL_RET					\n\t"
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/syscall/syscall.c#L75–L76 · D_SYSCALL_TEST；仅静态源码。

### CT117-E048 [D_SYSCALL_RET]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::sysexit_ret_addr]
```c
	"LABEL_SYSCALL_RET:						\n\t"
	"leaveq									\n\t"
	"retq									\n\t"
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/syscall/syscall.c#L79–L81 · D_SYSCALL_RET；仅静态源码。

### CT117-E049 [D_SWITCH_DEF]

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
```

固定time@a039d9803ade · mykernel/arch/x86_64/entry/syscall_64.c#L26–L26 · D_SWITCH_DEF；仅静态源码。

### CT117-E050 [A_CAT_BIND]

[VERIFIED myinitramfs/myshell/myshell.c::shell_internal_cmd]
```c
	{"ls",		ls_command},
	{"pwd",		pwd_command},
	{"cat",		cat_command},
	{"touch",	touch_command},
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L47–L50 · A_CAT_BIND；仅静态源码。

### CT117-E051 [A_CAT_LOCALS]

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	int len = 0;
	char *filename = NULL;
	int fd = 0;
	char *buf = NULL;
	int i = 0;
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L254–L258 · A_CAT_LOCALS；仅静态源码。

### CT117-E052 [A_CAT_LENGTHS]

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	len = strlen(current_dir);
	i = len + strlen(argv[1]);
	filename = malloc(i + 2);
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L260–L262 · A_CAT_LENGTHS；仅静态源码。

### CT117-E053 [A_CAT_INIT]

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	memset(filename, 0, i + 2);
	strcpy(filename, current_dir);
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L263–L264 · A_CAT_INIT；仅静态源码。

### CT117-E054 [A_CAT_SLASH]

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	if(len > 1)
		filename[len] = '/';
	strcat(filename, argv[1]);
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L265–L267 · A_CAT_SLASH；仅静态源码。

### CT117-E055 [A_CAT_OPEN]

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	fd = open(filename, O_RDONLY, 0);	
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L270–L270 · A_CAT_OPEN；仅静态源码。

### CT117-E056 [A_CAT_FAIL]

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	if (fd < 0)
	{
		printf("ls: %s: %s\n", strerror(errno), filename);
		return -1;
	}
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L271–L275 · A_CAT_FAIL；仅静态源码。

### CT117-E057 [A_STRCAT]

[VERIFIED myinitramfs/mylib/myglibc/string/strcat.c::STRCAT]
```c
STRCAT(char *dest, const char *src)
{
	strcpy(dest + strlen(dest), src);
	return dest;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/string/strcat.c#L28–L31 · A_STRCAT；仅静态源码。

### CT117-E058 [A_STRCPY]

[VERIFIED myinitramfs/mylib/myglibc/string/strcpy.c::STRCPY]
```c
char *
STRCPY(char *dest, const char *src)
{
	return memcpy(dest, src, strlen(src) + 1);
}
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/string/strcpy.c#L28–L32 · A_STRCPY；仅静态源码。

### CT117-E059 [B_SEEK_END]

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	i = lseek(fd, 0, SEEK_END);
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L276–L276 · B_SEEK_END；仅静态源码。

### CT117-E060 [B_SEEK_SET]

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	lseek(fd, 0, SEEK_SET);
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L277–L277 · B_SEEK_SET；仅静态源码。

### CT117-E061 [B_ALLOC_BUF]

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	buf = malloc(i + 1);
	memset(buf, 0, i + 1);
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L278–L279 · B_ALLOC_BUF；仅静态源码。

### CT117-E062 [B_READ_ONCE]

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	len = read(fd, buf, i);
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L280–L280 · B_READ_ONCE；仅静态源码。

### CT117-E063 [B_LSEEK_DEF]

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(lseek)]
```c
MYOS_SYSCALL_DEFINE3(lseek, unsigned int, fd,
		off_t, offset, unsigned int, whence)
{
	task_s * curr = current;
	file_s * fp = NULL;
```

固定time@a039d9803ade · mykernel/fs/syscall.c#L75–L79 · B_LSEEK_DEF；仅静态源码。

### CT117-E064 [B_LSEEK_GATES]

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(lseek)]
```c
	if(fd < 0 || fd >= curr->files->fd_count)
		return -EBADF;
	if(whence < 0 || whence >= SEEK_MAX)
		return -EINVAL;
```

固定time@a039d9803ade · mykernel/fs/syscall.c#L82–L85 · B_LSEEK_GATES；仅静态源码。

### CT117-E065 [B_LSEEK_CALL]

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(lseek)]
```c
	fp = current->files->fd_array[fd];
	if(fp->f_op && fp->f_op->llseek)
		ret = fp->f_op->llseek(fp, offset, whence);
	return ret;
```

固定time@a039d9803ade · mykernel/fs/syscall.c#L87–L90 · B_LSEEK_CALL；仅静态源码。

### CT117-E066 [B_FAT_END]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
		case SEEK_END:
				pos = filp->f_path.dentry->d_inode->i_size + offset;
			break;
```

固定time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L293–L295 · B_FAT_END；仅静态源码。

### CT117-E067 [B_FAT_SET]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
		case SEEK_SET:
				pos = offset;
			break;
```

固定time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L285–L287 · B_FAT_SET；仅静态源码。

### CT117-E068 [B_FAT_RANGE]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
	if(pos < 0 || pos > filp->f_path.dentry->d_inode->i_size)
		return -EOVERFLOW;

	filp->f_pos = pos;
```

固定time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L302–L305 · B_FAT_RANGE；仅静态源码。

### CT117-E069 [B_FAT_RET]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
	return pos;
```

固定time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L308–L308 · B_FAT_RET；仅静态源码。

### CT117-E070 [B_FAT_FOPS]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_file_ops]
```c
	.open			= FAT32_open,
	.read			= FAT32_read,
	.write			= FAT32_write,
	.llseek			= FAT32_lseek,
	.read_iter		= generic_file_read_iter,
```

固定time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L356–L360 · B_FAT_FOPS；仅静态源码。

### CT117-E071 [B_KSYS_LOCAL]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_read]
```c
	if (f.file) {
		loff_t pos, *ppos = file_ppos(f.file);
		if (ppos) {
			pos = *ppos;
			ppos = &pos;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L185–L189 · B_KSYS_LOCAL；仅静态源码。

### CT117-E072 [B_VFS_READ_MODE]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_read]
```c
	if (!(file->f_mode & FMODE_READ))
		return -EBADF;
	// if (!(file->f_mode & FMODE_CAN_READ))
	// 	return -EINVAL;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L147–L150 · B_VFS_READ_MODE；仅静态源码。

### CT117-E073 [B_VFS_READ_ITER]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_read]
```c
	if (file->f_op->read_iter)
		ret = new_sync_read(file, buf, count, pos);
	else if (file->f_op->read)
		ret = file->f_op->read(file, buf, count, pos);
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L160–L163 · B_VFS_READ_ITER；仅静态源码。

### CT117-E074 [B_FILEMAP_ENTER]

[VERIFIED mykernel/mm/vm_map/filemap.c::generic_file_read_iter]
```c
generic_file_read_iter(kiocb_s *iocb, iov_iter_s *iter)
{
	// size_t count = iov_iter_count(iter);
	size_t count = iter->count;
	ssize_t retval = 0;
```

固定time@a039d9803ade · mykernel/mm/vm_map/filemap.c#L263–L267 · B_FILEMAP_ENTER；仅静态源码。

### CT117-E075 [B_FILEMAP_ZERO]

[VERIFIED mykernel/mm/vm_map/filemap.c::generic_file_read_iter]
```c
	if (!count)
		return 0; /* skip atime */
```

固定time@a039d9803ade · mykernel/mm/vm_map/filemap.c#L269–L270 · B_FILEMAP_ZERO；仅静态源码。

### CT117-E076 [B_FILEMAP_TO_SIMPLE]

[VERIFIED mykernel/mm/vm_map/filemap.c::generic_file_read_iter]
```c
	return simple_filemap_read(iocb, iter);
```

固定time@a039d9803ade · mykernel/mm/vm_map/filemap.c#L305–L305 · B_FILEMAP_TO_SIMPLE；仅静态源码。

### CT117-E077 [C_PRINTF_CALL]

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	len = read(fd, buf, i);
	// printf("length:%d\t%s\n", len, buf);
	printf("%s\n", buf);
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L280–L282 · C_PRINTF_CALL；仅静态源码。

### CT117-E078 [C_CAT_RETURN]

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	printf("%s\n", buf);

	close(fd);
	return 1;
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L282–L285 · C_CAT_RETURN；仅静态源码。

### CT117-E079 [C_PRINTF_LOCAL]

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
int printf(const char * fmt, ...)
{
	va_list ap;
	char buf[SZ_4K];
	int rv;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/stdio/printf.c#L11–L15 · C_PRINTF_LOCAL；仅静态源码。

### CT117-E080 [C_PRINTF_VSN]

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	va_start(ap, fmt);
	rv = vsnprintf(buf, SZ_4K, fmt, ap);
	va_end(ap);
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/stdio/printf.c#L17–L19 · C_PRINTF_VSN；仅静态源码。

### CT117-E081 [C_PRINTF_WRITE]

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	// buffered IO has unsolved bug, so derectly use the write() function
	write(stdout->_fileno, buf, rv);
	// rv = (fputs(buf, stdout) < 0) ? 0 : rv;
	// fflush(stdout);
	return rv;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/stdio/printf.c#L21–L25 · C_PRINTF_WRITE；仅静态源码。

### CT117-E082 [C_VS_EMIT]

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::EMIT]
```c
#define EMIT(x)			({ if(o<n) {*q++ = (x);} o++; })
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L39–L39 · C_VS_EMIT；仅静态源码。

### CT117-E083 [C_VS_NORMAL]

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	while ((ch = *p++))
	{
		switch (state)
		{
		case st_normal:
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L557–L561 · C_VS_NORMAL；仅静态源码。

### CT117-E084 [C_VS_LITERAL]

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			else
			{
				EMIT(ch);
			}
			break;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L570–L574 · C_VS_LITERAL；仅静态源码。

### CT117-E085 [C_VS_STRLEN]

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				case 's':		/* String */
					sarg = va_arg(ap, const char *);
					sarg = sarg ? sarg : "(null)";
					slen = strlen(sarg);
					goto is_string;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L783–L787 · C_VS_STRLEN；仅静态源码。

### CT117-E086 [C_VS_COPY]

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
						for (i = slen; i; i--)
						{
							sch = *sarg++;
							EMIT(sch);
						}
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L805–L809 · C_VS_COPY；仅静态源码。

### CT117-E087 [C_VS_NUL]

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	if (o < n)
		*q = '\0';				/* No overflow */
	else if (n > 0)
		buf[n - 1] = '\0';		/* Overflow - terminate at end of buffer */
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L871–L874 · C_VS_NUL；仅静态源码。

### CT117-E088 [C_VS_RET]

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	return o;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L876–L876 · C_VS_RET；仅静态源码。

## POSITION_ONLY 普通源码位置

- CT117-P01: `myinitramfs/mylib/myglibc/posix/unistd.h#L335`，lseek原型 __off_t返回。
- CT117-P02: `myinitramfs/mylib/myglibc/posix/unistd.h#L354`，close声明。
- CT117-P03: `myinitramfs/mylib/myglibc/posix/unistd.h#L369–377`，read/write声明及ssize_t返回。
- CT117-P04: `myinitramfs/mylib/myglibc/syscall/syscall.c#L30–37`，open/close/read/write/lseek宏调用实例。
- CT117-P05: `mykernel/arch/x86_64/include/asm/syscalls_64.h#L8–17`，内核调用映射实例。
- CT117-P06: `mykernel/arch/x86_64/include/asm/unistd_64.h#L1–30`，read/write/open/close/lseek编号宏。
- CT117-P07: `myinitramfs/myshell/myshell.c#L252–286`，cat_command完整体，return与无free。
- CT117-P08: `myinitramfs/mylib/myglibc/string/strcat.c#L26–32`，STRCAT完整体。
- CT117-P09: `myinitramfs/mylib/myglibc/string/strcpy.c#L27–32`，STRCPY完整体。
- CT117-P10: `mykernel/fs/fat/myos_fat32.c#L278–309`，FAT32_lseek三分支和范围。
- CT117-P11: `mykernel/fs/fat/myos_fat32.c#L354–364`，FAT32操作表绑定。
- CT117-P12: `mykernel/fs/vfs/read_write.c#L142–172`，vfs_read完整分派。
- CT117-P13: `mykernel/fs/vfs/read_write.c#L175–197`，ksys_read局部位置。
- CT117-P14: `mykernel/mm/vm_map/filemap.c#L263–307`，generic_file_read_iter仅限入口。
- CT117-P15: `myinitramfs/mylib/myglibc/stdio/printf.c#L11–26`，printf不消费write返回。
- CT117-P16: `myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L533–877`，vsnprintf格式化全范围。
- CT117-P17: `mykernel/fs/vfs/readdir.c#L92–169`，旧116 getdents/filldir尾项范围。
- CT117-P18: `mykernel/fs/fat/myos_fat32.c#L311–351`，FAT actor ctx pos退出。
- CT117-P19: `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/closedir.c#L26–51`，closedir全部动作注释。
- CT117-P20: `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L39–50`，invalid_name含true/false两出口。
- CT117-P21: `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L63–121`，opendir和__alloc_dir两个函数。
- CT117-P22: `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L28–83`，readdir缓存/非正分支。
- CT117-P23: `myinitramfs/mylib/myglibc/syscall/syscall.c#L13–28`，syscall宏三层中转。
- CT117-P24: `myinitramfs/mylib/myglibc/syscall/syscall.c#L70–82`，汇编入口和共享退出段。
- CT117-P25: `mykernel/fs/vfs/read_write.c#L75–93`，new_sync_read队列特殊门。
- CT117-P26: `mykernel/fs/vfs/read_write.c#L202–217`，new_sync_write队列特殊门。
- CT117-P27: `mykernel/drivers/char/mem.c#L82–109`，read_zero零次及跨页全体。
- CT117-P28: `mykernel/fs/vfs/file.c#L197–203`，fd获取完整体。
- CT117-P29: `mykernel/include/linux/kernel/asm-generic/bug.h#L56–67`，BUG_ON真实定义守卫。
- CT117-P30: `myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L37–40`，EMIT计数宏。
- CT117-P31: `mykernel/fs/vfs/readdir.c#L135–168`，getdents零初始化和退出。
- CT117-P32: `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/dirstream.h#L30–51`，DIR的数据/游标成员。
- CT117-P33: `myinitramfs/myshell/myshell.c#L44–56`，shell命令表绑定。
- CT117-P34: `myinitramfs/myshell/myshell.c#L139–142`，run_cmd间接调用且忽略返回。
- CT117-P35: `mykernel/include/linux/lib/errno.h#L31`，EIOCBQUEUED常量。

115与116历史引文保留原固定对象，旧MD115/DS116编号只用于引用，不混入新CT117-P2计数。

新增短引文88项，普通位置35项，固定源码文件16份。编译和运行NOT_RUN。
