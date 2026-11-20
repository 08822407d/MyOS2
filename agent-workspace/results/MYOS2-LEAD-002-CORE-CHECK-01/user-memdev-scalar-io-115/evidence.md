# 第115批：固定time源码短引文

每个 MD115 新P2标签为固定源单一定义体、单宏、聚合初始化或CMake设置内部的连续1–5行。映射调用实例、条件包含、完整函数负面观察、签名和类型别名仅列POSITION_ONLY；旧批证据与评论冻结。

### MD115-E001 [D_CFG_WRAPPER]

[VERIFIED mykernel/scripts/options_flags.cmake::CMake_C_FLAGS]
```c
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
```

固定time@a039d9803ade；mykernel/scripts/options_flags.cmake#L65–65；active CONFIG_ARCH_HAS_SYSCALL_WRAPPER；仅静态，不代表已编译运行。

### MD115-E002 [D_SWITCH_DEF]

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
```

固定time@a039d9803ade；mykernel/arch/x86_64/entry/syscall_64.c#L26–26；case expansion definition；仅静态，不代表已编译运行。

### MD115-E003 [D_SWITCH_USE]

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```c
	switch (nr) {
	#include <asm/syscalls_64.h>
	default: return __x64_sys_ni_syscall(regs);
```

固定time@a039d9803ade；mykernel/arch/x86_64/entry/syscall_64.c#L29–31；actual switch including map；仅静态，不代表已编译运行。

### MD115-E004 [D_DEFINE3]

[VERIFIED mykernel/include/linux/kernel/syscalls.h::MYOS_SYSCALL_DEFINE3]
```c
	#define MYOS_SYSCALL_DEFINE3(name, ...)	__MYOS_SYSCALL_DEFINEx(3, _##name, __VA_ARGS__)
```

固定time@a039d9803ade；mykernel/include/linux/kernel/syscalls.h#L1386–1386；macro for 3 arguments；仅静态，不代表已编译运行。

### MD115-E005 [D_STUB_USE]

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
	#define __MYOS_SYSCALL_DEFINEx(x, name, ...)						\
				long __se_sys##name(const pt_regs_s *regs);				\
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));	\
				__SYS_STUBx(x64, sys##name)								\
```

固定time@a039d9803ade；mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L241–244；generator invokes __SYS_STUBx；仅静态，不代表已编译运行。

### MD115-E006 [D_STUB_RETURN]

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__SYS_STUBx]
```c
				long __##abi##_##name(const pt_regs_s *regs);	\
				long __##abi##_##name(const pt_regs_s *regs)	\
				{												\
					return __se_##name(regs);					\
				}	
```

固定time@a039d9803ade；mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L105–109；x64 long stub returns __se_##name(regs)；仅静态，不代表已编译运行。

### MD115-E007 [D_SE_TO_DO]

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
				long __se_sys##name(const pt_regs_s *regs)				\
				{														\
					long ret = __do_sys##name(							\
								__ARCH_MAP(x,__SC_CAST,__VA_ARGS__));	\
					return ret;											\
```

固定time@a039d9803ade；mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L245–249；__se_sys long calls __do_sys；仅静态，不代表已编译运行。

### MD115-E008 [D_CHAR_AGGREGATE]

[VERIFIED mykernel/drivers/char/char_dev.c::def_chr_fops]
```c
const file_ops_s def_chr_fops = {
	.open = chrdev_open,
	.llseek = noop_llseek,
};
```

固定time@a039d9803ade；mykernel/drivers/char/char_dev.c#L95–98；old E044 is one aggregate definition；仅静态，不代表已编译运行。

### MD115-E009 [A_READ_SYSCALL]

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(read)]
```c
	return ksys_read(fd, buf, count);
}
```

固定time@a039d9803ade；mykernel/fs/syscall.c#L13–14；actual read entry calls ksys_read；仅静态，不代表已编译运行。

### MD115-E010 [A_WRITE_SYSCALL]

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(write)]
```c
	return ksys_write(fd, buf, count);
}
```

固定time@a039d9803ade；mykernel/fs/syscall.c#L34–35；actual write entry calls ksys_write；仅静态，不代表已编译运行。

### MD115-E011 [A_READ_FDGET]

[VERIFIED mykernel/fs/vfs/file.c::myos_fdget_pos]
```c
	task_s *curr = current;
	file_s *fp = curr->files->fd_array[fd];
	
	return (fd_s){.file = fp, .flags = 0};
```

固定time@a039d9803ade；mykernel/fs/vfs/file.c#L199–202；direct fd_array read and flags zero；仅静态，不代表已编译运行。

### MD115-E012 [A_WRITE_FDGET]

[VERIFIED mykernel/include/linux/fs/file.h::fdget_pos]
```c
		return __to_fd(fd);
```

固定time@a039d9803ade；mykernel/include/linux/fs/file.h#L74–74；fdget_pos returns __to_fd(fd)；仅静态，不代表已编译运行。

### MD115-E013 [A_TO_FD]

[VERIFIED mykernel/include/linux/fs/file.h::__to_fd]
```c
		files_struct_s *files = current->files;
		file_s *file = files->fd_array[v];
		// return (fd_s){(file_s *)(v & ~3),v & 3};
		return (fd_s){file, 3};
```

固定time@a039d9803ade；mykernel/include/linux/fs/file.h#L53–56；current fd_array read and flags3；仅静态，不代表已编译运行。

### MD115-E014 [A_PPOS]

[VERIFIED mykernel/fs/vfs/read_write.c::file_ppos]
```c
	return file->f_mode & FMODE_STREAM ? NULL : &file->f_pos;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L177–177；stream returns NULL otherwise address f_pos；仅静态，不代表已编译运行。

### MD115-E015 [A_KREAD_FD]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_read]
```c
	fd_s f = myos_fdget_pos(fd);
	ssize_t ret = -EBADF;

	if (f.file) {
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L182–185；read fd acquisition and initial EBADF；仅静态，不代表已编译运行。

### MD115-E016 [A_KREAD_POS]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_read]
```c
	if (f.file) {
		loff_t pos, *ppos = file_ppos(f.file);
		if (ppos) {
			pos = *ppos;
			ppos = &pos;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L185–189；read local position copy；仅静态，不代表已编译运行。

### MD115-E017 [A_KREAD_CALL]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_read]
```c
		ret = vfs_read(f.file, buf, count, ppos);
		if (ret >= 0 && ppos)
			f.file->f_pos = pos;
		fdput_pos(f);
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L191–194；read VFS and conditional nonnegative writeback；仅静态，不代表已编译运行。

### MD115-E018 [A_KREAD_RETURN]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_read]
```c
	return ret;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L196–196；read ret；仅静态，不代表已编译运行。

### MD115-E019 [A_KWRITE_FD]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]
```c
	fd_s f = fdget_pos(fd);
	ssize_t ret = -EBADF;
	file_s *filp = f.file;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L321–323；write fd acquisition and initial EBADF；仅静态，不代表已编译运行。

### MD115-E020 [A_KWRITE_POS]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]
```c
	if (filp != NULL) {
		loff_t pos, *ppos = file_ppos(filp);
		if (ppos) {
			pos = *ppos;
			ppos = &pos;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L325–329；write local pos copy and stream gate；仅静态，不代表已编译运行。

### MD115-E021 [A_KWRITE_CALL]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]
```c
		ret = vfs_write(filp, buf, count, ppos);
		if (ret >= 0 && ppos)
			filp->f_pos = pos;
		fdput_pos(f);
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L331–334；write VFS and conditional nonnegative writeback；仅静态，不代表已编译运行。

### MD115-E022 [A_KWRITE_RETURN]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]
```c
	return ret;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L337–337；write ret；仅静态，不代表已编译运行。

### MD115-E023 [A_VREAD_MODE]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_read]
```c
	if (!(file->f_mode & FMODE_READ))
		return -EBADF;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L147–148；FMODE_READ guard；仅静态，不代表已编译运行。

### MD115-E024 [A_VREAD_ACCESS]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_read]
```c
	if (unlikely(!access_ok(buf, count)))
		return -EFAULT;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L151–152；access_ok guard read；仅静态，不代表已编译运行。

### MD115-E025 [A_VREAD_VERIFY]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_read]
```c
	ret = rw_verify_area(READ, file, pos, count);
	if (ret)
		return ret;
	if (count > MAX_RW_COUNT)
		count =  MAX_RW_COUNT;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L154–158；verify and MAX_RW_COUNT cap；仅静态，不代表已编译运行。

### MD115-E026 [A_VREAD_DISPATCH]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_read]
```c
	if (file->f_op->read_iter)
		ret = new_sync_read(file, buf, count, pos);
	else if (file->f_op->read)
		ret = file->f_op->read(file, buf, count, pos);
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L160–163；read_iter then read priority；仅静态，不代表已编译运行。

### MD115-E027 [A_VREAD_MISSING]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_read]
```c
	else
		ret = -EINVAL;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L164–165；missing read methods EINVAL；仅静态，不代表已编译运行。

### MD115-E028 [A_VWRITE_MODE]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	if (!(file->f_mode & FMODE_WRITE))
		return -EBADF;
	if (!(file->f_mode & FMODE_CAN_WRITE))
		return -EINVAL;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L291–294；FMODE_WRITE then FMODE_CAN_WRITE；仅静态，不代表已编译运行。

### MD115-E029 [A_VWRITE_ACCESS]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	if (unlikely(!access_ok(buf, count)))
		return -EFAULT;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L295–296；access_ok guard write；仅静态，不代表已编译运行。

### MD115-E030 [A_VWRITE_VERIFY]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	ret = rw_verify_area(WRITE, file, pos, count);
	if (ret)
		return ret;
	if (count > MAX_RW_COUNT)
		count =  MAX_RW_COUNT;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L298–302；verify and MAX_RW_COUNT cap；仅静态，不代表已编译运行。

### MD115-E031 [A_VWRITE_DISPATCH]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	if (file->f_op->write)
		ret = file->f_op->write(file, buf, count, pos);
	else if (file->f_op->write_iter)
		ret = new_sync_write(file, buf, count, pos);
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L304–307；write before write_iter；仅静态，不代表已编译运行。

### MD115-E032 [A_VWRITE_MISSING]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	else
		ret = -EINVAL;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L308–309；missing write methods EINVAL；仅静态，不代表已编译运行。

### MD115-E033 [A_VERIFY_EARLY]

[VERIFIED mykernel/fs/vfs/read_write.c::rw_verify_area]
```c
	return 0;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L51–51；verify function immediate return zero；仅静态，不代表已编译运行。

### MD115-E034 [A_SYNC_READ]

[VERIFIED mykernel/fs/vfs/read_write.c::new_sync_read]
```c
	ret = filp->f_op->read_iter(&kiocb, &iter);
	BUG_ON(ret == -EIOCBQUEUED);
	if (ppos)
		*ppos = kiocb.ki_pos;
	return ret;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L88–92；read_iter and ppos update；仅静态，不代表已编译运行。

### MD115-E035 [A_SYNC_WRITE]

[VERIFIED mykernel/fs/vfs/read_write.c::new_sync_write]
```c
	ret = filp->f_op->write_iter(&kiocb, &iter);
	BUG_ON(ret == -EIOCBQUEUED);
	if (ret > 0 && ppos)
		*ppos = kiocb.ki_pos;
	return ret;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L212–216；write_iter and positive pos update；仅静态，不代表已编译运行。

### MD115-E036 [A_READ_FLAG]

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_READ]
```c
	#define FMODE_READ			((__force fmode_t)0x1)
```

固定time@a039d9803ade；mykernel/include/linux/fs/fs.h#L61–61；read mode flag；仅静态，不代表已编译运行。

### MD115-E037 [A_WRITE_FLAG]

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_WRITE]
```c
	#define FMODE_WRITE			((__force fmode_t)0x2)
```

固定time@a039d9803ade；mykernel/include/linux/fs/fs.h#L63–63；write mode flag；仅静态，不代表已编译运行。

### MD115-E038 [A_CAN_WRITE_FLAG]

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_CAN_WRITE]
```c
	#define FMODE_CAN_WRITE			((__force fmode_t)0x40000)
```

固定time@a039d9803ade；mykernel/include/linux/fs/fs.h#L108–108；CAN_WRITE mode flag；仅静态，不代表已编译运行。

### MD115-E039 [A_STREAM_FLAG]

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_STREAM]
```c
	#define FMODE_STREAM			((__force fmode_t)0x200000)
```

固定time@a039d9803ade；mykernel/include/linux/fs/fs.h#L114–114；stream mode flag；仅静态，不代表已编译运行。

### MD115-E040 [A_OPEN_FMODE]

[VERIFIED mykernel/include/linux/fs/fs.h::OPEN_FMODE]
```c
	#define OPEN_FMODE(flag) (									\
				(__force fmode_t)(((flag + 1) & O_ACCMODE) |	\
					(flag & __FMODE_NONOTIFY))					\
				)
```

固定time@a039d9803ade；mykernel/include/linux/fs/fs.h#L3425–3428；access bits formula；仅静态，不代表已编译运行。

### MD115-E041 [A_ALLOC_MODE]

[VERIFIED mykernel/fs/vfs/file_table.c::__alloc_file]
```c
	f->f_flags = flags;
	f->f_mode = OPEN_FMODE(flags);
	/* f->f_version: 0 */

	return f;
```

固定time@a039d9803ade；mykernel/fs/vfs/file_table.c#L59–63；file.f_mode OPEN_FMODE and return；仅静态，不代表已编译运行。

### MD115-E042 [A_OPEN_ASSIGN]

[VERIFIED mykernel/fs/vfs/open.c::myos_do_dentry_open]
```c
	f->f_op = inode->i_fop;
	if(f->f_op && f->f_op->open)
		error = f->f_op->open(f->f_path.dentry->d_inode, f);
```

固定time@a039d9803ade；mykernel/fs/vfs/open.c#L53–55；file.f_op from inode and open callback；仅静态，不代表已编译运行。

### MD115-E043 [A_RW_CAP]

[VERIFIED mykernel/include/linux/fs/fs.h::MAX_RW_COUNT]
```c
	#define MAX_RW_COUNT (INT_MAX & PAGE_MASK)
```

固定time@a039d9803ade；mykernel/include/linux/fs/fs.h#L2423–2423；INT_MAX & PAGE_MASK；仅静态，不代表已编译运行。

### MD115-E044 [A_O_ACCMODE]

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_ACCMODE]
```c
	#define O_ACCMODE			00000003
```

固定time@a039d9803ade；mykernel/include/uapi/asm-generic/fcntl.h#L19–19；access mode mask；仅静态，不代表已编译运行。

### MD115-E045 [A_O_RDONLY]

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_RDONLY]
```c
	#define O_RDONLY			00000000
```

固定time@a039d9803ade；mykernel/include/uapi/asm-generic/fcntl.h#L20–20；zero；仅静态，不代表已编译运行。

### MD115-E046 [A_O_WRONLY]

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_WRONLY]
```c
	#define O_WRONLY			00000001
```

固定time@a039d9803ade；mykernel/include/uapi/asm-generic/fcntl.h#L21–21；one；仅静态，不代表已编译运行。

### MD115-E047 [A_O_RDWR]

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_RDWR]
```c
	#define O_RDWR				00000002
```

固定time@a039d9803ade；mykernel/include/uapi/asm-generic/fcntl.h#L22–22；two；仅静态，不代表已编译运行。

### MD115-E048 [B_PAGE_SHIFT]

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT]
```c
	#define PAGE_SHIFT				12
```

固定time@a039d9803ade；mykernel/arch/x86_64/mm/page/page_const_arch.h#L9–9；PAGE_SHIFT 12；仅静态，不代表已编译运行。

### MD115-E049 [B_PAGE_SIZE]

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```

固定time@a039d9803ade；mykernel/arch/x86_64/mm/page/page_const_arch.h#L10–10；PAGE_SIZE 1UL shifted；仅静态，不代表已编译运行。

### MD115-E050 [B_MIN_T]

[VERIFIED mykernel/include/linux/kernel/minmax.h::min_t]
```c
	#define min_t(type, x, y)	__cmp((type)(x), (type)(y), <)
```

固定time@a039d9803ade；mykernel/include/linux/kernel/minmax.h#L114–114；typed minimum macro；仅静态，不代表已编译运行。

### MD115-E051 [B_ZERO_INIT]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
	size_t cleared = 0;
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L85–85；cleared initialized zero；仅静态，不代表已编译运行。

### MD115-E052 [B_ZERO_CHUNK]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
	while (count) {
		size_t chunk = min_t(size_t, count, PAGE_SIZE);
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L87–88；while count and page size chunk；仅静态，不代表已编译运行。

### MD115-E053 [B_ZERO_MEMSET]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
		memset(buf + cleared, 0, chunk);
		count -= chunk;
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L90–91；memset then count decrement；仅静态，不代表已编译运行。

### MD115-E054 [B_ZERO_BREAK]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
		if (count == 0)
			break;
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L92–93；break before cleared update；仅静态，不代表已编译运行。

### MD115-E055 [B_ZERO_ACCUM]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
		cleared += chunk;
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L95–95；accumulate only nonfinal chunk；仅静态，不代表已编译运行。

### MD115-E056 [B_ZERO_RETURN]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
	return cleared;
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L108–108；return cleared excludes last chunk；仅静态，不代表已编译运行。

### MD115-E057 [C_READ_NULL]

[VERIFIED mykernel/drivers/char/mem.c::read_null]
```c
	return 0;
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L61–61；returns 0 no buffer write；仅静态，不代表已编译运行。

### MD115-E058 [C_WRITE_NULL]

[VERIFIED mykernel/drivers/char/mem.c::write_null]
```c
	return count;
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L67–67；returns count no pos write；仅静态，不代表已编译运行。

### MD115-E059 [C_WRITE_ZERO_ALIAS1]

[VERIFIED mykernel/drivers/char/mem.c::write_zero]
```c
#define write_zero write_null
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L44–44；first write_zero alias write_null；仅静态，不代表已编译运行。

### MD115-E060 [C_WRITE_ZERO_ALIAS2]

[VERIFIED mykernel/drivers/char/mem.c::write_zero]
```c
#define write_zero	write_null
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L113–113；second write_zero alias write_null；仅静态，不代表已编译运行。

### MD115-E061 [C_NULL_OPS]

[VERIFIED mykernel/drivers/char/mem.c::null_fops]
```c
static const file_ops_s null_fops = {
	.llseek		= null_lseek,
	.read		= read_null,
	.write		= write_null,
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L125–128；null active scalar read and write；仅静态，不代表已编译运行。

### MD115-E062 [C_ZERO_OPS]

[VERIFIED mykernel/drivers/char/mem.c::zero_fops]
```c
		.llseek		= zero_lseek,
		.write		= write_zero,
	// 	.read_iter	= read_iter_zero,
		.read		= read_zero,
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L142–145；zero active scalar read write and commented iter；仅静态，不代表已编译运行。

### MD115-E063 [C_DEV_NULL]

[VERIFIED mykernel/drivers/char/mem.c::devlist]
```c
	[3] = {"null", 0666, &null_fops, FMODE_NOWAIT},
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L170–170；minor 3 mode0666 fmode NOWAIT；仅静态，不代表已编译运行。

### MD115-E064 [C_DEV_ZERO]

[VERIFIED mykernel/drivers/char/mem.c::devlist]
```c
	[5] = {"zero", 0666, &zero_fops, FMODE_NOWAIT},
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L174–174；minor 5 mode0666 fmode NOWAIT；仅静态，不代表已编译运行。

### MD115-E065 [C_DEV_FULL]

[VERIFIED mykernel/drivers/char/mem.c::devlist]
```c
	[7] = {"full", 0666, &full_fops, 0},
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L175–175；minor 7 mode0666 fmode0；仅静态，不代表已编译运行。

### MD115-E066 [C_INIT_CDEV]

[VERIFIED mykernel/drivers/char/mem.c::chr_dev_init]
```c
		memdev.cdev.kobj.name = memdev.name;
		dev_t devt = MKDEV(MEM_MAJOR, minor);
		cdev_init(&memdev.cdev, memdev.fops);
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L197–199；cdev_init with memdev fops；仅静态，不代表已编译运行。

### MD115-E067 [C_ADD_CDEV]

[VERIFIED mykernel/drivers/char/mem.c::chr_dev_init]
```c
		if (cdev_add(&memdev.cdev, devt, 1))
			color_printk(RED, BLACK, "Couldn't register %s driver\n", memdev.name);
		else
			myos_device_create(mem_class, devt, memdev.name);
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L200–203；cdev_add and conditional device creation；仅静态，不代表已编译运行。

### MD115-E068 [C_FOPS_FROM_DEV]

[VERIFIED mykernel/drivers/char/char_dev.c::chrdev_open]
```c
	// fops = fops_get(p->ops);
	fops = p->ops;
	if (!fops)
		goto out_cdev_put;
```

固定time@a039d9803ade；mykernel/drivers/char/char_dev.c#L70–73；ops from p->ops and null gate；仅静态，不代表已编译运行。

### MD115-E069 [C_FOPS_SWAP]

[VERIFIED mykernel/drivers/char/char_dev.c::chrdev_open]
```c
	filp->f_op = fops;
	if (filp->f_op->open) {
		ret = filp->f_op->open(inode, filp);
```

固定time@a039d9803ade；mykernel/drivers/char/char_dev.c#L76–78；install p->ops into file；仅静态，不代表已编译运行。

### MD115-E070 [C_NOWAIT_FLAG]

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_NOWAIT]
```c
	#define FMODE_NOWAIT			((__force fmode_t)0x8000000)
```

固定time@a039d9803ade；mykernel/include/linux/fs/fs.h#L120–120；NOWAIT not WRITE/CAN_WRITE；仅静态，不代表已编译运行。

## POSITION_ONLY，独立保存历史、类型、签名和负向全体观察

- **MD115-P01** `mykernel/arch/x86_64/include/asm/syscalls_64.h#L11–11` — 旧FS114-E003是映射调用实例，应归普通位置。
- **MD115-P02** `mykernel/include/linux/kernel/syscalls.h#L93–102` — 条件包含宏/头文件路径及适用范围。
- **MD115-P03** `mykernel/scripts/options_flags.cmake#L64–66` — CMake的CONFIG_ARCH_HAS_SYSCALL_WRAPPER在65，CONFIG_BUG在66。
- **MD115-P04** `mykernel/arch/x86_64/entry/syscall_64.c#L26–33` — 活动switch含映射调用，宏定义另证。
- **MD115-P05** `mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L104–109` — x64桩整个宏，返回__se。
- **MD115-P06** `mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L241–251` — __MYOS_SYSCALL_DEFINEx完整生成链。
- **MD115-P07** `mykernel/fs/syscall.c#L75–91` — lseek完整体：fd_count读取后才拒绝fd_array。
- **MD115-P08** `mykernel/fs/fat/myos_fat32.c#L278–309` — FAT32完整switch、界限、写位和返回。
- **MD115-P09** `mykernel/drivers/char/char_dev.c#L95–98` — 旧E044完整聚合初始化，仍合格。
- **MD115-P10** `mykernel/fs/vfs/read_write.c#L142–171` — vfs_read完整体，CAN_READ门是注释。
- **MD115-P11** `mykernel/fs/vfs/read_write.c#L287–317` — vfs_write完整体，CAN_WRITE门启用。
- **MD115-P12** `mykernel/fs/vfs/read_write.c#L49–72` — rw_verify_area全体，第一句return0。
- **MD115-P13** `mykernel/fs/vfs/read_write.c#L175–196` — file_ppos与ksys_read的局部pos和提交。
- **MD115-P14** `mykernel/fs/vfs/read_write.c#L319–338` — ksys_write获取/局部pos/提交。
- **MD115-P15** `mykernel/fs/vfs/read_write.c#L75–93` — new_sync_read对kiocb及ppos的写回。
- **MD115-P16** `mykernel/fs/vfs/read_write.c#L202–217` — new_sync_write正返回时才写本地pos。
- **MD115-P17** `mykernel/include/linux/fs/file.h#L51–82` — __to_fd和fdget_pos，fdput_pos。
- **MD115-P18** `mykernel/fs/vfs/file.c#L197–203` — myos_fdget_pos直接取fd_array。
- **MD115-P19** `mykernel/fs/vfs/file_table.c#L38–64` — __alloc_file完成f_mode设值。
- **MD115-P20** `mykernel/include/linux/fs/fs.h#L3421–3428` — OPEN_FMODE表达式和前置别名。
- **MD115-P21** `mykernel/include/uapi/asm-generic/fcntl.h#L19–22` — O_ACCMODE/READ/WRITE/RDWR各定义。
- **MD115-P22** `mykernel/include/linux/fs/fs.h#L2418–2424` — MAX_RW_COUNT宏。
- **MD115-P23** `mykernel/drivers/char/mem.c#L82–109` — read_zero全部循环和返回。
- **MD115-P24** `mykernel/drivers/char/mem.c#L58–68` — read_null/write_null完整体且未写ppos。
- **MD115-P25** `mykernel/drivers/char/mem.c#L111–155` — 宏重复和三设备操作表完整范围，full仅注释。
- **MD115-P26** `mykernel/drivers/char/mem.c#L166–181` — devlist权限和fmode字段内容，非运行状态。
- **MD115-P27** `mykernel/drivers/char/mem.c#L183–208` — chr_dev_init注册尝试非运行证明。
- **MD115-P28** `mykernel/drivers/char/char_dev.c#L65–98` — fops替换与默认表关系。
- **MD115-P29** `mykernel/fs/vfs/open.c#L46–65` — myos_do_dentry_open继承inode操作表。
- **MD115-P30** `mykernel/arch/x86_64/mm/page/page_const_arch.h#L8–11` — PAGE_SIZE与PAGE_SHIFT定义。
- **MD115-P31** `mykernel/include/linux/kernel/minmax.h#L25–34` — __cmp实际宏体及min_t依赖。
- **MD115-P32** `mykernel/include/linux/fs/fs.h#L58–120` — FMODE_READ/WRITE/CAN_WRITE/STREAM/NOWAIT分别定义。
- **MD115-P33** `mykernel/include/linux/fs/fs.h#L869–880` — file_f_mode/file_f_pos/file_f_op字段类型。
- **MD115-P34** `mykernel/drivers/char/mem.c#L151–155` — full_fops注释掉所有实际标量read/write成员。
- **MD115-P35** `mykernel/include/linux/fs/fs.h#L1858–1866` — file_operations read/read_iter/write/write_iter签名。

冻结114引文：作者逐字58/58、C07验收单一定义57/58，E003为syscalls_64.h:11映射调用不合格；原列28个普通定位保留。FS114-E004为活动宏定义，FS114-E044是def_chr_fops完整聚合体。本批MD115标签完全另计。

本批新标签数：70；逐字候选：70；预分类单一定义：70；普通位置：35。
