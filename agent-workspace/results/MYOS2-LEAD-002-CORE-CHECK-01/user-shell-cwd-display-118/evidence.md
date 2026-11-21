# 第118批固定time新增源码短引文

仅将单个固定time函数体、宏定义或聚合初始化内部连续1–5个物理原行记为本批P2 `[VERIFIED]`。调用实例、声明、完整体否定观察及历史证据在POSITION_ONLY单列；本批未编译或运行。

### CW118-E001 [D_SWITCH_DEF]

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
```

固定time@a039d9803ade · mykernel/arch/x86_64/entry/syscall_64.c#L26–L26 · D_SWITCH_DEF；源码条件性短窗，运行NOT_RUN。

### CW118-E002 [D_SWITCH_USE]

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```c
long x64_sys_call(const pt_regs_s *regs, unsigned int nr)
{
	switch (nr) {
	#include <asm/syscalls_64.h>
	default: return __x64_sys_ni_syscall(regs);
```

固定time@a039d9803ade · mykernel/arch/x86_64/entry/syscall_64.c#L27–L31 · D_SWITCH_USE；源码条件性短窗，运行NOT_RUN。

### CW118-E003 [D_DEFINE3]

[VERIFIED mykernel/include/linux/kernel/syscalls.h::MYOS_SYSCALL_DEFINE3]
```c
	#define MYOS_SYSCALL_DEFINE3(name, ...)	__MYOS_SYSCALL_DEFINEx(3, _##name, __VA_ARGS__)
```

固定time@a039d9803ade · mykernel/include/linux/kernel/syscalls.h#L1386–L1386 · D_DEFINE3；源码条件性短窗，运行NOT_RUN。

### CW118-E004 [D_WRAPPER_STUB]

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__SYS_STUBx]
```c
	#define __SYS_STUBx(abi, name, ...)							\
				long __##abi##_##name(const pt_regs_s *regs);	\
				long __##abi##_##name(const pt_regs_s *regs)	\
				{												\
					return __se_##name(regs);					\
```

固定time@a039d9803ade · mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L104–L108 · D_WRAPPER_STUB；源码条件性短窗，运行NOT_RUN。

### CW118-E005 [D_WRAPPER_MAP]

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
	#define __MYOS_SYSCALL_DEFINEx(x, name, ...)						\
				long __se_sys##name(const pt_regs_s *regs);				\
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));	\
				__SYS_STUBx(x64, sys##name)								\
```

固定time@a039d9803ade · mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L241–L244 · D_WRAPPER_MAP；源码条件性短窗，运行NOT_RUN。

### CW118-E006 [D_WRAPPER_SE]

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
				long __se_sys##name(const pt_regs_s *regs)				\
				{														\
					long ret = __do_sys##name(							\
								__ARCH_MAP(x,__SC_CAST,__VA_ARGS__));	\
					return ret;											\
```

固定time@a039d9803ade · mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L245–L249 · D_WRAPPER_SE；源码条件性短窗，运行NOT_RUN。

### CW118-E007 [D_SYS_READ]

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(read)]
```c
MYOS_SYSCALL_DEFINE3(read, unsigned int, fd,
		char *,buf, size_t, count) {
	return ksys_read(fd, buf, count);
}
```

固定time@a039d9803ade · mykernel/fs/syscall.c#L11–L14 · D_SYS_READ；源码条件性短窗，运行NOT_RUN。

### CW118-E008 [D_SYS_WRITE]

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(write)]
```c

	return ksys_write(fd, buf, count);
}
```

固定time@a039d9803ade · mykernel/fs/syscall.c#L33–L35 · D_SYS_WRITE；源码条件性短窗，运行NOT_RUN。

### CW118-E009 [D_FILE_PPOS]

[VERIFIED mykernel/fs/vfs/read_write.c::file_ppos]
```c
/* file_ppos returns &file->f_pos or NULL if file is stream */
static inline loff_t *file_ppos(file_s *file) {
	return file->f_mode & FMODE_STREAM ? NULL : &file->f_pos;
}
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L175–L178 · D_FILE_PPOS；源码条件性短窗，运行NOT_RUN。

### CW118-E010 [D_KREAD_FD]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_read]
```c
ssize_t ksys_read(uint fd, char __user *buf, size_t count)
{
	fd_s f = myos_fdget_pos(fd);
	ssize_t ret = -EBADF;

```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L180–L184 · D_KREAD_FD；源码条件性短窗，运行NOT_RUN。

### CW118-E011 [D_KREAD_LOCAL]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_read]
```c
	if (f.file) {
		loff_t pos, *ppos = file_ppos(f.file);
		if (ppos) {
			pos = *ppos;
			ppos = &pos;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L185–L189 · D_KREAD_LOCAL；源码条件性短窗，运行NOT_RUN。

### CW118-E012 [D_KREAD_CALL]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_read]
```c
		ret = vfs_read(f.file, buf, count, ppos);
		if (ret >= 0 && ppos)
			f.file->f_pos = pos;
		fdput_pos(f);
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L191–L194 · D_KREAD_CALL；源码条件性短窗，运行NOT_RUN。

### CW118-E013 [D_KWRITE_FD]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]
```c
ssize_t ksys_write(unsigned int fd, const char __user *buf, size_t count)
{
	fd_s f = fdget_pos(fd);
	ssize_t ret = -EBADF;
	file_s *filp = f.file;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L319–L323 · D_KWRITE_FD；源码条件性短窗，运行NOT_RUN。

### CW118-E014 [D_KWRITE_LOCAL]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]
```c
	if (filp != NULL) {
		loff_t pos, *ppos = file_ppos(filp);
		if (ppos) {
			pos = *ppos;
			ppos = &pos;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L325–L329 · D_KWRITE_LOCAL；源码条件性短窗，运行NOT_RUN。

### CW118-E015 [D_KWRITE_CALL]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]
```c
		ret = vfs_write(filp, buf, count, ppos);
		if (ret >= 0 && ppos)
			filp->f_pos = pos;
		fdput_pos(f);
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L331–L334 · D_KWRITE_CALL；源码条件性短窗，运行NOT_RUN。

### CW118-E016 [D_VERIFY]

[VERIFIED mykernel/fs/vfs/read_write.c::rw_verify_area]
```c
int rw_verify_area(int read_write, file_s *file, const loff_t *ppos, size_t count)
{
	return 0;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L49–L51 · D_VERIFY；源码条件性短窗，运行NOT_RUN。

### CW118-E017 [D_VFS_R_GATE]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_read]
```c
	if (!(file->f_mode & FMODE_READ))
		return -EBADF;
	// if (!(file->f_mode & FMODE_CAN_READ))
	// 	return -EINVAL;
	if (unlikely(!access_ok(buf, count)))
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L147–L151 · D_VFS_R_GATE；源码条件性短窗，运行NOT_RUN。

### CW118-E018 [D_VFS_R_CLIP]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_read]
```c
	ret = rw_verify_area(READ, file, pos, count);
	if (ret)
		return ret;
	if (count > MAX_RW_COUNT)
		count =  MAX_RW_COUNT;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L154–L158 · D_VFS_R_CLIP；源码条件性短窗，运行NOT_RUN。

### CW118-E019 [D_VFS_R_METHOD]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_read]
```c
	if (file->f_op->read_iter)
		ret = new_sync_read(file, buf, count, pos);
	else if (file->f_op->read)
		ret = file->f_op->read(file, buf, count, pos);
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L160–L163 · D_VFS_R_METHOD；源码条件性短窗，运行NOT_RUN。

### CW118-E020 [D_VFS_W_GATE]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	if (!(file->f_mode & FMODE_WRITE))
		return -EBADF;
	if (!(file->f_mode & FMODE_CAN_WRITE))
		return -EINVAL;
	if (unlikely(!access_ok(buf, count)))
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L291–L295 · D_VFS_W_GATE；源码条件性短窗，运行NOT_RUN。

### CW118-E021 [D_VFS_W_CLIP]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	ret = rw_verify_area(WRITE, file, pos, count);
	if (ret)
		return ret;
	if (count > MAX_RW_COUNT)
		count =  MAX_RW_COUNT;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L298–L302 · D_VFS_W_CLIP；源码条件性短窗，运行NOT_RUN。

### CW118-E022 [D_VFS_W_METHOD]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	if (file->f_op->write)
		ret = file->f_op->write(file, buf, count, pos);
	else if (file->f_op->write_iter)
		ret = new_sync_write(file, buf, count, pos);
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L304–L307 · D_VFS_W_METHOD；源码条件性短窗，运行NOT_RUN。

### CW118-E023 [D_SYNC_R_ITER]

[VERIFIED mykernel/fs/vfs/read_write.c::new_sync_read]
```c
	ret = filp->f_op->read_iter(&kiocb, &iter);
	BUG_ON(ret == -EIOCBQUEUED);
	if (ppos)
		*ppos = kiocb.ki_pos;
	return ret;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L88–L92 · D_SYNC_R_ITER；源码条件性短窗，运行NOT_RUN。

### CW118-E024 [D_SYNC_W_ITER]

[VERIFIED mykernel/fs/vfs/read_write.c::new_sync_write]
```c
	ret = filp->f_op->write_iter(&kiocb, &iter);
	BUG_ON(ret == -EIOCBQUEUED);
	if (ret > 0 && ppos)
		*ppos = kiocb.ki_pos;
	return ret;
```

固定time@a039d9803ade · mykernel/fs/vfs/read_write.c#L212–L216 · D_SYNC_W_ITER；源码条件性短窗，运行NOT_RUN。

### CW118-E025 [D_BUG_ON]

[VERIFIED mykernel/include/linux/kernel/asm-generic/bug.h::BUG_ON]
```c
	#			define BUG_ON(condition) do { if (unlikely(condition)) BUG(); } while (0)
```

固定time@a039d9803ade · mykernel/include/linux/kernel/asm-generic/bug.h#L66–L66 · D_BUG_ON；源码条件性短窗，运行NOT_RUN。

### CW118-E026 [D_ZERO_FOPS]

[VERIFIED mykernel/drivers/char/mem.c::zero_fops]
```c
static const file_ops_s zero_fops = {
		.llseek		= zero_lseek,
		.write		= write_zero,
	// 	.read_iter	= read_iter_zero,
		.read		= read_zero,
```

固定time@a039d9803ade · mykernel/drivers/char/mem.c#L141–L145 · D_ZERO_FOPS；源码条件性短窗，运行NOT_RUN。

### CW118-E027 [D_ZERO_INIT]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
	size_t cleared = 0;

	while (count) {
		size_t chunk = min_t(size_t, count, PAGE_SIZE);
```

固定time@a039d9803ade · mykernel/drivers/char/mem.c#L85–L88 · D_ZERO_INIT；源码条件性短窗，运行NOT_RUN。

### CW118-E028 [D_ZERO_WRITE]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
		memset(buf + cleared, 0, chunk);
		count -= chunk;
		if (count == 0)
			break;
```

固定time@a039d9803ade · mykernel/drivers/char/mem.c#L90–L93 · D_ZERO_WRITE；源码条件性短窗，运行NOT_RUN。

### CW118-E029 [D_ZERO_ACCUM]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
		cleared += chunk;
```

固定time@a039d9803ade · mykernel/drivers/char/mem.c#L95–L95 · D_ZERO_ACCUM；源码条件性短窗，运行NOT_RUN。

### CW118-E030 [D_ZERO_RETURN]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
	return cleared;
```

固定time@a039d9803ade · mykernel/drivers/char/mem.c#L108–L108 · D_ZERO_RETURN；源码条件性短窗，运行NOT_RUN。

### CW118-E031 [D_INVALID_TEST]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::invalid_name]
```c
invalid_name(const char *name)
{
	if (name[0] == '\0')
	{
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L40–L43 · D_INVALID_TEST；源码条件性短窗，运行NOT_RUN。

### CW118-E032 [D_INVALID_TRUE]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::invalid_name]
```c
		// __set_errno(ENOENT);
		return true;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L46–L47 · D_INVALID_TRUE；源码条件性短窗，运行NOT_RUN。

### CW118-E033 [D_INVALID_FALSE]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::invalid_name]
```c
	return false;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L49–L49 · D_INVALID_FALSE；源码条件性短窗，运行NOT_RUN。

### CW118-E034 [D_GEN1]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::SYSFUNC_DEF]
```c
#define SYSFUNC_DEF(name)						\
			_SYSFUNC_DEF_(name, __NR_##name)
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/syscall/syscall.c#L13–L14 · D_GEN1；源码条件性短窗，运行NOT_RUN。

### CW118-E035 [D_GEN2]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::_SYSFUNC_DEF_]
```c
#define _SYSFUNC_DEF_(name, nr)					\
			__SYSFUNC_DEF__(name, nr)
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/syscall/syscall.c#L16–L17 · D_GEN2；源码条件性短窗，运行NOT_RUN。

### CW118-E036 [D_GEN3]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::__SYSFUNC_DEF__]
```c
#define __SYSFUNC_DEF__(name, nr)							\
			__asm__	(										\
				".global	"#name"					\n\t"	\
				".type		"#name",	@function	\n\t"	\
				#name":								\n\t"	\
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/syscall/syscall.c#L19–L23 · D_GEN3；源码条件性短窗，运行NOT_RUN。

### CW118-E037 [D_GEN_NUM]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::__SYSFUNC_DEF__]
```c
				"pushq	%rbp						\n\t"	\
				"movq	%rsp,	%rbp				\n\t"	\
				"movq	$"#nr",	%rax				\n\t"	\
				"jmp	LABEL_SYSCALL				\n\t"	\
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/syscall/syscall.c#L24–L27 · D_GEN_NUM；源码条件性短窗，运行NOT_RUN。

### CW118-E038 [D_ASM_SYSCALL]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::LABEL_SYSCALL]
```c
	"LABEL_SYSCALL:							\n\t"
	"leaq	sysexit_ret_addr(%rip),	%rcx	\n\t"
	"syscall								\n\t"
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/syscall/syscall.c#L71–L73 · D_ASM_SYSCALL；源码条件性短窗，运行NOT_RUN。

### CW118-E039 [D_ASM_SYSEXIT]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::sysexit_ret_addr]
```c
	"sysexit_ret_addr:						\n\t"
	"cmpq	$-0x1000,	%rax				\n\t"
	"jb	LABEL_SYSCALL_RET					\n\t"
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/syscall/syscall.c#L74–L76 · D_ASM_SYSEXIT；源码条件性短窗，运行NOT_RUN。

### CW118-E040 [D_ASM_RET]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::LABEL_SYSCALL_RET]
```c
	"LABEL_SYSCALL_RET:						\n\t"
	"leaveq									\n\t"
	"retq									\n\t"
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/syscall/syscall.c#L79–L81 · D_ASM_RET；源码条件性短窗，运行NOT_RUN。

### CW118-E041 [D_DIR_INIT]

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
	fd_s f;
	getdents_cbk64_s buf = {
		.ctx.actor = filldir64,
		.count = count,
		.current_dir = dirent
```

固定time@a039d9803ade · mykernel/fs/vfs/readdir.c#L135–L139 · D_DIR_INIT；源码条件性短窗，运行NOT_RUN。

### CW118-E042 [D_DIR_PREV]

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	prev_reclen = buf->prev_reclen;
	dirent = buf->current_dir;
	prev = (void *)dirent - prev_reclen;

	prev->d_off = offset;
```

固定time@a039d9803ade · mykernel/fs/vfs/readdir.c#L109–L113 · D_DIR_PREV；源码条件性短窗，运行NOT_RUN。

### CW118-E043 [D_DIR_WRITE]

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	dirent->d_ino = ino;
	dirent->d_reclen = reclen;
	dirent->d_type = d_type;
	cplen = copy_to_user(dirent->d_name, (char *)name, namelen);
	dirent->d_name[namelen] = '\0';
```

固定time@a039d9803ade · mykernel/fs/vfs/readdir.c#L114–L118 · D_DIR_WRITE；源码条件性短窗，运行NOT_RUN。

### CW118-E044 [D_DIR_ACCOUNT]

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	buf->prev_reclen = reclen;
	buf->current_dir = (void *)dirent + reclen;
	buf->count -= reclen;
	return 0;
```

固定time@a039d9803ade · mykernel/fs/vfs/readdir.c#L120–L123 · D_DIR_ACCOUNT；源码条件性短窗，运行NOT_RUN。

### CW118-E045 [D_DIR_TAIL]

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
	if (buf.prev_reclen) {
	// 	linux_dirent64_s *lastdirent;
	// 	typeof(lastdirent->d_off) d_off = buf.ctx.pos;

	// 	lastdirent = (void *) buf.current_dir - buf.prev_reclen;
```

固定time@a039d9803ade · mykernel/fs/vfs/readdir.c#L157–L161 · D_DIR_TAIL；源码条件性短窗，运行NOT_RUN。

### CW118-E046 [D_FAT_ACTOR]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_getdents64]
```c
		error = ctx->actor(ctx, name, namelen, 0, 0, 0);

		kfree(name);
		if (error !=0)
			break;
```

固定time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L340–L344 · D_FAT_ACTOR；源码条件性短窗，运行NOT_RUN。

### CW118-E047 [D_FAT_FOPS]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_file_ops]
```c
	.open			= FAT32_open,
	.read			= FAT32_read,
	.write			= FAT32_write,
	.llseek			= FAT32_lseek,
	.read_iter		= generic_file_read_iter,
```

固定time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L356–L360 · D_FAT_FOPS；源码条件性短窗，运行NOT_RUN。

### CW118-E048 [D_GENERIC_ZERO]

[VERIFIED mykernel/mm/vm_map/filemap.c::generic_file_read_iter]
```c
	if (!count)
		return 0; /* skip atime */
```

固定time@a039d9803ade · mykernel/mm/vm_map/filemap.c#L269–L270 · D_GENERIC_ZERO；源码条件性短窗，运行NOT_RUN。

### CW118-E049 [D_GENERIC_DELEGATE]

[VERIFIED mykernel/mm/vm_map/filemap.c::generic_file_read_iter]
```c
	return simple_filemap_read(iocb, iter);
```

固定time@a039d9803ade · mykernel/mm/vm_map/filemap.c#L305–L305 · D_GENERIC_DELEGATE；源码条件性短窗，运行NOT_RUN。

### CW118-E050 [A_PATH_MAX]

[VERIFIED myinitramfs/myshell/myshell.c::PATH_MAX]
```c
	#define PATH_MAX        4096	/* # chars in a path name including nul */
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L15–L15 · A_PATH_MAX；源码条件性短窗，运行NOT_RUN。

### CW118-E051 [A_HOMEDIR]

[VERIFIED myinitramfs/myshell/myshell.c::homedir]
```c
char *homedir = "/";
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L59–L59 · A_HOMEDIR；源码条件性短窗，运行NOT_RUN。

### CW118-E052 [A_CURRENT_DIR]

[VERIFIED myinitramfs/myshell/myshell.c::current_dir]
```c
char current_dir[PATH_MAX];
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L60–L60 · A_CURRENT_DIR；源码条件性短窗，运行NOT_RUN。

### CW118-E053 [A_SHELL_CD_PWD_BIND]

[VERIFIED myinitramfs/myshell/myshell.c::shell_internal_cmd]
```c
builtincmd_s shell_internal_cmd[] = 
{
	{"cd",		cd_command},
	{"ls",		ls_command},
	{"pwd",		pwd_command},
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L44–L48 · A_SHELL_CD_PWD_BIND；源码条件性短窗，运行NOT_RUN。

### CW118-E054 [A_MAIN_ENTRY]

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
int main(int argc, const char *argv[])
{
	test_PrintDateTime();

	printf("This is MyShell v0.01.\n");
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L66–L70 · A_MAIN_ENTRY；源码条件性短窗，运行NOT_RUN。

### CW118-E055 [A_MAIN_CONSOLE]

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
	console_fd = open("/dev/console", O_RDWR);
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L73–L73 · A_MAIN_CONSOLE；源码条件性短窗，运行NOT_RUN。

### CW118-E056 [A_MAIN_ZERO_GETCWD]

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
	memset(current_dir, 0, PATH_MAX);
	getcwd(current_dir, PATH_MAX);
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L75–L76 · A_MAIN_ZERO_GETCWD；源码条件性短窗，运行NOT_RUN。

### CW118-E057 [A_MAIN_PROMPT]

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
	while (1)
	{
		printf("\n");
		printf("MyOS2:%s # _", current_dir);
		fflush(stdout);
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L83–L87 · A_MAIN_PROMPT；源码条件性短窗，运行NOT_RUN。

### CW118-E058 [A_MAIN_DISPATCH]

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
		index = parse_command(buf, &__argc, &__argv);
		if(index < 0)
			printf("Input Error, No Command Found!\n");
		else
			run_cmd(index, __argc, __argv);	//argc,argv
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L98–L102 · A_MAIN_DISPATCH；源码条件性短窗，运行NOT_RUN。

### CW118-E059 [A_RUN_CMD]

[VERIFIED myinitramfs/myshell/myshell.c::run_cmd]
```c
void run_cmd(int index, int argc, char **argv)
{
	shell_internal_cmd[index].function(argc, argv);
}
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L139–L142 · A_RUN_CMD；源码条件性短窗，运行NOT_RUN。

### CW118-E060 [A_USER_NR_GETCWD]

[VERIFIED mykernel/arch/x86_64/include/asm/unistd_64.h::__NR_getcwd]
```c
	#define __NR_getcwd			79
```

固定time@a039d9803ade · mykernel/arch/x86_64/include/asm/unistd_64.h#L98–L98 · A_USER_NR_GETCWD；源码条件性短窗，运行NOT_RUN。

### CW118-E061 [A_USER_NR_CHDIR]

[VERIFIED mykernel/arch/x86_64/include/asm/unistd_64.h::__NR_chdir]
```c
	#define __NR_chdir			80
```

固定time@a039d9803ade · mykernel/arch/x86_64/include/asm/unistd_64.h#L100–L100 · A_USER_NR_CHDIR；源码条件性短窗，运行NOT_RUN。

### CW118-E062 [B_CD_LOCALS]

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
int cd_command(int argc, char **argv)
{
	char *path = NULL;
	int len = 0;
	int i = 0;
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L188–L192 · B_CD_LOCALS；源码条件性短窗，运行NOT_RUN。

### CW118-E063 [B_CD_SELECT]

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	if (argc <= 1)
		path = homedir;
	else
		path = argv[1];
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L194–L197 · B_CD_SELECT；源码条件性短窗，运行NOT_RUN。

### CW118-E064 [B_CD_DOT]

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	/////.
	if(!strcmp(".", path))
		return 1;
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L199–L201 · B_CD_DOT；源码条件性短窗，运行NOT_RUN。

### CW118-E065 [B_CD_CALL]

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	i = chdir(path);
	if(!i)
	{
		memset(current_dir, 0, PATH_MAX);
		getcwd(current_dir, PATH_MAX);
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L203–L207 · B_CD_CALL；源码条件性短窗，运行NOT_RUN。

### CW118-E066 [B_CD_FAIL]

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	else
		printf("cd: %s: %s\n", strerror(errno), path);
	return 1;
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L209–L211 · B_CD_FAIL；源码条件性短窗，运行NOT_RUN。

### CW118-E067 [B_STRCMP_INIT]

[VERIFIED myinitramfs/mylib/myglibc/string/strcmp.c::STRCMP]
```c
int STRCMP(const char *p1, const char *p2)
{
	const unsigned char *s1 = (const unsigned char *)p1;
	const unsigned char *s2 = (const unsigned char *)p2;
	unsigned char c1, c2;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/string/strcmp.c#L29–L33 · B_STRCMP_INIT；源码条件性短窗，运行NOT_RUN。

### CW118-E068 [B_STRCMP_LOOP]

[VERIFIED myinitramfs/mylib/myglibc/string/strcmp.c::STRCMP]
```c
	{
		c1 = (unsigned char)*s1++;
		c2 = (unsigned char)*s2++;
		if (c1 == '\0')
			return c1 - c2;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/string/strcmp.c#L36–L40 · B_STRCMP_LOOP；源码条件性短窗，运行NOT_RUN。

### CW118-E069 [B_STRCMP_RET]

[VERIFIED myinitramfs/mylib/myglibc/string/strcmp.c::STRCMP]
```c
	} while (c1 == c2);

	return c1 - c2;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/string/strcmp.c#L41–L43 · B_STRCMP_RET；源码条件性短窗，运行NOT_RUN。

### CW118-E070 [B_KERNEL_CHDIR_INIT]

[VERIFIED mykernel/fs/vfs/open.c::MYOS_SYSCALL_DEFINE1(chdir)]
```c
MYOS_SYSCALL_DEFINE1(chdir, const char *, filename)
{
	path_s path;
	task_s *curr = current;
	int error;
```

固定time@a039d9803ade · mykernel/fs/vfs/open.c#L272–L276 · B_KERNEL_CHDIR_INIT；源码条件性短窗，运行NOT_RUN。

### CW118-E071 [B_KERNEL_CHDIR_LOOKUP]

[VERIFIED mykernel/fs/vfs/open.c::MYOS_SYSCALL_DEFINE1(chdir)]
```c
retry:
	error = user_path_at(AT_FDCWD, filename, lookup_flags, &path);
	if (error)
		goto out;
```

固定time@a039d9803ade · mykernel/fs/vfs/open.c#L278–L281 · B_KERNEL_CHDIR_LOOKUP；源码条件性短窗，运行NOT_RUN。

### CW118-E072 [B_KERNEL_CHDIR_WRITE]

[VERIFIED mykernel/fs/vfs/open.c::MYOS_SYSCALL_DEFINE1(chdir)]
```c
	curr->fs->pwd = path;
```

固定time@a039d9803ade · mykernel/fs/vfs/open.c#L283–L283 · B_KERNEL_CHDIR_WRITE；源码条件性短窗，运行NOT_RUN。

### CW118-E073 [B_KERNEL_CHDIR_RETURN]

[VERIFIED mykernel/fs/vfs/open.c::MYOS_SYSCALL_DEFINE1(chdir)]
```c
out:
	return error;
```

固定time@a039d9803ade · mykernel/fs/vfs/open.c#L291–L292 · B_KERNEL_CHDIR_RETURN；源码条件性短窗，运行NOT_RUN。

### CW118-E074 [C_PWD_PRINT]

[VERIFIED myinitramfs/myshell/myshell.c::pwd_command]
```c
int pwd_command(int argc, char **argv)
{
	if(current_dir)
		printf("%s\n", current_dir);
	return 1;
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L245–L249 · C_PWD_PRINT；源码条件性短窗，运行NOT_RUN。

### CW118-E075 [C_MAIN_EXTRA_LF]

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
	while (1)
	{
		printf("\n");
		printf("MyOS2:%s # _", current_dir);
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L83–L86 · C_MAIN_EXTRA_LF；源码条件性短窗，运行NOT_RUN。

### CW118-E076 [C_MAIN_FFLUSH]

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
		printf("\n");
		printf("MyOS2:%s # _", current_dir);
		fflush(stdout);
		memset(buf, 0, SZ_4K);
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L85–L88 · C_MAIN_FFLUSH；源码条件性短窗，运行NOT_RUN。

### CW118-E077 [C_CAT_DIR_READ]

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	len = strlen(current_dir);
	i = len + strlen(argv[1]);
	filename = malloc(i + 2);
	memset(filename, 0, i + 2);
	strcpy(filename, current_dir);
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L260–L264 · C_CAT_DIR_READ；源码条件性短窗，运行NOT_RUN。

### CW118-E078 [C_CAT_SLASH]

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	if(len > 1)
		filename[len] = '/';
	strcat(filename, argv[1]);
```

固定time@a039d9803ade · myinitramfs/myshell/myshell.c#L265–L267 · C_CAT_SLASH；源码条件性短窗，运行NOT_RUN。

### CW118-E079 [C_PRINTF_VS]

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	char buf[SZ_4K];
	int rv;

	va_start(ap, fmt);
	rv = vsnprintf(buf, SZ_4K, fmt, ap);
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/stdio/printf.c#L14–L18 · C_PRINTF_VS；源码条件性短窗，运行NOT_RUN。

### CW118-E080 [C_PRINTF_WRITE]

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	// buffered IO has unsolved bug, so derectly use the write() function
	write(stdout->_fileno, buf, rv);
	// rv = (fputs(buf, stdout) < 0) ? 0 : rv;
	// fflush(stdout);
	return rv;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/stdio/printf.c#L21–L25 · C_PRINTF_WRITE；源码条件性短窗，运行NOT_RUN。

### CW118-E081 [C_VSN_LITERAL]

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			else
			{
				EMIT(ch);
			}
			break;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L570–L574 · C_VSN_LITERAL；源码条件性短窗，运行NOT_RUN。

### CW118-E082 [C_VSN_STRLEN]

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				case 's':		/* String */
					sarg = va_arg(ap, const char *);
					sarg = sarg ? sarg : "(null)";
					slen = strlen(sarg);
					goto is_string;
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L783–L787 · C_VSN_STRLEN；源码条件性短窗，运行NOT_RUN。

### CW118-E083 [C_VSN_COPY]

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
						for (i = slen; i; i--)
						{
							sch = *sarg++;
							EMIT(sch);
						}
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L805–L809 · C_VSN_COPY；源码条件性短窗，运行NOT_RUN。

### CW118-E084 [C_VSN_RETURN]

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	if (o < n)
		*q = '\0';				/* No overflow */
	else if (n > 0)
		buf[n - 1] = '\0';		/* Overflow - terminate at end of buffer */
```

固定time@a039d9803ade · myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L871–L874 · C_VSN_RETURN；源码条件性短窗，运行NOT_RUN。

### CW118-E085 [C_GETCWD_PWD]

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
MYOS_SYSCALL_DEFINE2(getcwd, char *, buf, unsigned long, size)
{
	int		error;
	task_s *curr = current;
	path_s	pwd = curr->fs->pwd,
```

固定time@a039d9803ade · mykernel/fs/vfs/d_path.c#L196–L200 · C_GETCWD_PWD；源码条件性短窗，运行NOT_RUN。

### CW118-E086 [C_GETCWD_ROOT]

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
	path_s	pwd = curr->fs->pwd,
			root = curr->fs->root;
	prpndbuf_s b;
	char *bufhead;
```

固定time@a039d9803ade · mykernel/fs/vfs/d_path.c#L200–L203 · C_GETCWD_ROOT；源码条件性短窗，运行NOT_RUN。

### CW118-E087 [C_GETCWD_ALLOC]

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		unsigned len;
		b.len = PATH_MAX;
		bufhead = kzalloc(b.len, GFP_KERNEL);
		b.buf = bufhead + PATH_MAX;
```

固定time@a039d9803ade · mykernel/fs/vfs/d_path.c#L209–L212 · C_GETCWD_ALLOC；源码条件性短窗，运行NOT_RUN。

### CW118-E088 [C_GETCWD_PREPEND]

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		myos_prepend(&b, "", 1);
		if (myos_prepend_path(&pwd, &root, &b) > 0)
			myos_prepend(&b, "(unreachable)", 13);
```

固定time@a039d9803ade · mykernel/fs/vfs/d_path.c#L214–L216 · C_GETCWD_PREPEND；源码条件性短窗，运行NOT_RUN。

### CW118-E089 [C_GETCWD_LENGTH]

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		len = PATH_MAX - b.len;
		if (len > PATH_MAX)
			error = -ENAMETOOLONG;
		else if (len > size)
			error = -ERANGE;
```

固定time@a039d9803ade · mykernel/fs/vfs/d_path.c#L218–L222 · C_GETCWD_LENGTH；源码条件性短窗，运行NOT_RUN。

### CW118-E090 [C_GETCWD_COPY]

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		else if (copy_to_user(buf, b.buf, len))
			error = -EFAULT;
		else
			error = (long)buf;
```

固定time@a039d9803ade · mykernel/fs/vfs/d_path.c#L223–L226 · C_GETCWD_COPY；源码条件性短窗，运行NOT_RUN。

### CW118-E091 [C_GETCWD_RETURN]

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
	kfree(bufhead);
	return error;
```

固定time@a039d9803ade · mykernel/fs/vfs/d_path.c#L229–L230 · C_GETCWD_RETURN；源码条件性短窗，运行NOT_RUN。

## POSITION_ONLY：宏调用/声明/完整函数/历史条件位置

- CW118-P01 — `mykernel/arch/x86_64/include/asm/syscalls_64.h#L4–7`；read/write/open/close均为映射宏调用实例，非定义。
- CW118-P02 — `mykernel/arch/x86_64/include/asm/syscalls_64.h#L11`；lseek映射宏调用，非read/write证据。
- CW118-P03 — `mykernel/arch/x86_64/include/asm/syscalls_64.h#L41–42`；getcwd/chdir映射宏调用实例，非定义。
- CW118-P04 — `myinitramfs/mylib/myglibc/syscall/syscall.c#L30–37`；open/close/read/write/lseek用户SYSFUNC_DEF实例。
- CW118-P05 — `myinitramfs/mylib/myglibc/syscall/syscall.c#L49–51`；getdents/getcwd/chdir用户SYSFUNC_DEF实例。
- CW118-P06 — `myinitramfs/myshell/myshell.c#L28–41`；shell本地char* getcwd、int chdir声明与头注释区分。
- CW118-P07 — `myinitramfs/myshell/myshell.c#L44–60`；命令表、全局homedir/current_dir和文件级存储。
- CW118-P08 — `myinitramfs/myshell/myshell.c#L66–104`；main完整循环，printf/fflush/read_line调度。
- CW118-P09 — `myinitramfs/myshell/myshell.c#L188–212`；cd_command完整函数，早退、成功刷新、失败输出。
- CW118-P10 — `myinitramfs/myshell/myshell.c#L245–250`；pwd_command完整体，数组条件和return。
- CW118-P11 — `myinitramfs/myshell/myshell.c#L252–286`；cat_command完整体复用current_dir。
- CW118-P12 — `myinitramfs/mylib/myglibc/posix/unistd.h#L510–526`；chdir/getcwd的声明与注释均非本地活动函数。
- CW118-P13 — `myinitramfs/mylib/myglibc/string/strcmp.c#L29–44`；strcmp完整体，点名判等来源。
- CW118-P14 — `myinitramfs/mylib/myglibc/stdio/printf.c#L11–26`；printf正常格式化与请求write完整体。
- CW118-P15 — `myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L533–877`；vsnprintf字符串与字面字符所有活动分支。
- CW118-P16 — `mykernel/fs/vfs/open.c#L272–293`；chdir完整体，user_path_at错误不做赋值。
- CW118-P17 — `mykernel/fs/vfs/d_path.c#L196–230`；getcwd整个体、bufhead未见NULL检查及后置copy_to_user。
- CW118-P18 — `mykernel/fs/vfs/readdir.c#L109–123`；filldir64 prev/current实际写入尾项。
- CW118-P19 — `mykernel/fs/vfs/readdir.c#L132–168`；getdents64初始化、返回、尾部补写注释。
- CW118-P20 — `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L39–50`；invalid_name两个出口及注释。
- CW118-P21 — `myinitramfs/mylib/myglibc/dirent/dirent.h#L141–147`；closedir(DIR*)声明非close(int)。
- CW118-P22 — `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/closedir.c#L26–51`；closedir完整体全部close/free/return被注释。
- CW118-P23 — `myinitramfs/mylib/myglibc/syscall/syscall.c#L70–82`；用户汇编LABEL_SYSCALL、sysexit_ret_addr、LABEL_SYSCALL_RET三标签完整体与注释。
- CW118-P24 — `mykernel/arch/x86_64/entry/syscall_64.c#L26–33`；内核__SYSCALL宏定义及活动switch，映射实例另列。
- CW118-P25 — `mykernel/fs/vfs/read_write.c#L175–197`；file_ppos和ksys_read实际局部位置及提交。
- CW118-P26 — `mykernel/fs/vfs/read_write.c#L319–338`；ksys_write实际局部位置及提交。
- CW118-P27 — `mykernel/drivers/char/mem.c#L82–109`；read_zero整个循环及0/P/P1三个有限模型。
- CW118-P28 — `mykernel/mm/vm_map/filemap.c#L263–307`；generic_file_read_iter零返回与委派。
- CW118-P29 — `mykernel/fs/fat/myos_fat32.c#L354–364`；FAT32_file_ops操作表绑定。
- CW118-P30 — `mykernel/include/linux/lib/errno.h#L31`；特殊EIOCBQUEUED常量。
- CW118-P31 — `mykernel/arch/x86_64/include/asm/unistd_64.h#L98–100`；getcwd号79、chdir号80两独立宏定义。
- CW118-P32 — `mykernel/fs/vfs/d_path.c#L95–176`；getcwd上游prepend_path的限定目录名字拼接历史R60。

历史MD115、DS116、CT117旧锚仅按已发布固定原件引用，不混入本批新引文及计数。

新P2=91；普通位置=32；直接取证源文件=19。编译、运行、Graphviz和QEMU均NOT_RUN。
