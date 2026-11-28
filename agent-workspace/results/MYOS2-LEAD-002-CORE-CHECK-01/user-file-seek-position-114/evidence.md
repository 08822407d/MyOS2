# 第114批固定 time 源码引文

每个新标签只对应单一真实函数体、宏、文件操作表或数组表定义中的1–5连续原行。完整函数、类型、签名、注释和负向查找另列POSITION_ONLY。固定来源 `time` 短值 a039d9803ade；运行/编译 NOT_RUN。

### FS114-E001 [CONFIG_BUG]

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```c
	-DCONFIG_BUG \
```

fixed time@a039d9803ade · mykernel/scripts/options_flags.cmake#L66-L66 · fixed C config。仅静态来源，不证明已编译/执行。

### FS114-E002 [NR_LSEEK]

[VERIFIED mykernel/arch/x86_64/include/asm/unistd_64.h::__NR_lseek]
```c
	#define __NR_lseek			8
```

fixed time@a039d9803ade · mykernel/arch/x86_64/include/asm/unistd_64.h#L13-L13 · number 8。仅静态来源，不证明已编译/执行。

### FS114-E003 [NR_MAP]

[VERIFIED mykernel/arch/x86_64/include/asm/syscalls_64.h::__SYSCALL(__NR_lseek, sys_lseek)]
```c
__SYSCALL(__NR_lseek, sys_lseek)
```

fixed time@a039d9803ade · mykernel/arch/x86_64/include/asm/syscalls_64.h#L11-L11 · syscall number mapping。仅静态来源，不证明已编译/执行。

### FS114-E004 [SWITCH_MAP]

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
```

fixed time@a039d9803ade · mykernel/arch/x86_64/entry/syscall_64.c#L26-L26 · active switch case expansion。仅静态来源，不证明已编译/执行。

### FS114-E005 [SWITCH_INC]

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```c
	switch (nr) {
	#include <asm/syscalls_64.h>
	default: return __x64_sys_ni_syscall(regs);
```

fixed time@a039d9803ade · mykernel/arch/x86_64/entry/syscall_64.c#L29-L31 · switch includes mapping。仅静态来源，不证明已编译/执行。

### FS114-E006 [SYSCALL_DEFINE]

[VERIFIED mykernel/include/linux/kernel/syscalls.h::MYOS_SYSCALL_DEFINE3]
```c
	#define MYOS_SYSCALL_DEFINE3(name, ...)	__MYOS_SYSCALL_DEFINEx(3, _##name, __VA_ARGS__)
```

fixed time@a039d9803ade · mykernel/include/linux/kernel/syscalls.h#L1386-L1386 · three-argument wrapper macro。仅静态来源，不证明已编译/执行。

### FS114-E007 [WRAP_DECLARE]

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
	#define __MYOS_SYSCALL_DEFINEx(x, name, ...)						\
				long __se_sys##name(const pt_regs_s *regs);				\
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));	\
				__SYS_STUBx(x64, sys##name)								\
```

fixed time@a039d9803ade · mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L241-L244 · long declaration and stub。仅静态来源，不证明已编译/执行。

### FS114-E008 [WRAP_RETURN]

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
				long __se_sys##name(const pt_regs_s *regs)				\
				{														\
					long ret = __do_sys##name(							\
								__ARCH_MAP(x,__SC_CAST,__VA_ARGS__));	\
					return ret;											\
```

fixed time@a039d9803ade · mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L245-L249 · long return from __do_sys。仅静态来源，不证明已编译/执行。

### FS114-E009 [A_LOCALS]

[VERIFIED mykernel/fs/syscall.c::lseek]
```c
	task_s * curr = current;
	file_s * fp = NULL;
	ulong ret = 0;
```

fixed time@a039d9803ade · mykernel/fs/syscall.c#L78-L80 · entry locals and ulong ret=0。仅静态来源，不证明已编译/执行。

### FS114-E010 [A_FD_REJECT]

[VERIFIED mykernel/fs/syscall.c::lseek]
```c
	if(fd < 0 || fd >= curr->files->fd_count)
		return -EBADF;
```

fixed time@a039d9803ade · mykernel/fs/syscall.c#L82-L83 · unsigned fd upper gate。仅静态来源，不证明已编译/执行。

### FS114-E011 [A_WHENCE_REJECT]

[VERIFIED mykernel/fs/syscall.c::lseek]
```c
	if(whence < 0 || whence >= SEEK_MAX)
		return -EINVAL;
```

fixed time@a039d9803ade · mykernel/fs/syscall.c#L84-L85 · unsigned whence >= SEEK_MAX。仅静态来源，不证明已编译/执行。

### FS114-E012 [A_FILE_LOOKUP]

[VERIFIED mykernel/fs/syscall.c::lseek]
```c
	fp = current->files->fd_array[fd];
```

fixed time@a039d9803ade · mykernel/fs/syscall.c#L87-L87 · current fd array indexing。仅静态来源，不证明已编译/执行。

### FS114-E013 [A_CALLBACK]

[VERIFIED mykernel/fs/syscall.c::lseek]
```c
	if(fp->f_op && fp->f_op->llseek)
		ret = fp->f_op->llseek(fp, offset, whence);
```

fixed time@a039d9803ade · mykernel/fs/syscall.c#L88-L89 · method conditional and indirect call。仅静态来源，不证明已编译/执行。

### FS114-E014 [A_RETURN]

[VERIFIED mykernel/fs/syscall.c::lseek]
```c
	return ret;
```

fixed time@a039d9803ade · mykernel/fs/syscall.c#L90-L90 · entry returns ret。仅静态来源，不证明已编译/执行。

### FS114-E015 [SEEK_SET]

[VERIFIED mykernel/include/uapi/linux/fs.h::SEEK_SET]
```c
	#define SEEK_SET			0			/* seek relative to beginning of file */
```

fixed time@a039d9803ade · mykernel/include/uapi/linux/fs.h#L43-L43 · constant 0。仅静态来源，不证明已编译/执行。

### FS114-E016 [SEEK_CUR]

[VERIFIED mykernel/include/uapi/linux/fs.h::SEEK_CUR]
```c
	#define SEEK_CUR			1			/* seek relative to current file position */
```

fixed time@a039d9803ade · mykernel/include/uapi/linux/fs.h#L44-L44 · constant 1。仅静态来源，不证明已编译/执行。

### FS114-E017 [SEEK_END]

[VERIFIED mykernel/include/uapi/linux/fs.h::SEEK_END]
```c
	#define SEEK_END			2			/* seek relative to end of file */
```

fixed time@a039d9803ade · mykernel/include/uapi/linux/fs.h#L45-L45 · constant 2。仅静态来源，不证明已编译/执行。

### FS114-E018 [SEEK_DATA]

[VERIFIED mykernel/include/uapi/linux/fs.h::SEEK_DATA]
```c
	#define SEEK_DATA			3			/* seek to the next data */
```

fixed time@a039d9803ade · mykernel/include/uapi/linux/fs.h#L46-L46 · constant 3。仅静态来源，不证明已编译/执行。

### FS114-E019 [SEEK_HOLE]

[VERIFIED mykernel/include/uapi/linux/fs.h::SEEK_HOLE]
```c
	#define SEEK_HOLE			4			/* seek to the next hole */
```

fixed time@a039d9803ade · mykernel/include/uapi/linux/fs.h#L47-L47 · constant 4。仅静态来源，不证明已编译/执行。

### FS114-E020 [SEEK_MAX]

[VERIFIED mykernel/include/uapi/linux/fs.h::SEEK_MAX]
```c
	#define SEEK_MAX			SEEK_HOLE
```

fixed time@a039d9803ade · mykernel/include/uapi/linux/fs.h#L48-L48 · alias SEEK_HOLE。仅静态来源，不证明已编译/执行。

### FS114-E021 [B_INODE_SIZE]

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
		inode->i_size	= de->size;
```

fixed time@a039d9803ade · mykernel/fs/fat/inode.c#L175-L175 · regular file inode size。仅静态来源，不证明已编译/执行。

### FS114-E022 [B_INODE_FOPS]

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
		inode->i_op		= &vfat_dir_inode_operations;
		inode->i_fop	= &FAT32_file_ops;
		inode->i_mapping->a_ops = &fat_aops;
```

fixed time@a039d9803ade · mykernel/fs/fat/inode.c#L178-L180 · regular file inode fops。仅静态来源，不证明已编译/执行。

### FS114-E023 [B_OPEN_FOPS]

[VERIFIED mykernel/fs/vfs/open.c::myos_do_dentry_open]
```c
	f->f_op = inode->i_fop;
	if(f->f_op && f->f_op->open)
		error = f->f_op->open(f->f_path.dentry->d_inode, f);
```

fixed time@a039d9803ade · mykernel/fs/vfs/open.c#L53-L55 · f_op assigned during open。仅静态来源，不证明已编译/执行。

### FS114-E024 [B_FAT_FOPS]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_file_ops]
```c
	.llseek			= FAT32_lseek,
```

fixed time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L359-L359 · llseek binding。仅静态来源，不证明已编译/执行。

### FS114-E025 [B_FAT_LOCAL]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
	inode_s *inode = filp->f_path.dentry->d_inode;
	long pos = 0;
```

fixed time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L280-L281 · local signed long pos。仅静态来源，不证明已编译/执行。

### FS114-E026 [B_FAT_SET]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
		case SEEK_SET:
				pos = offset;
			break;
```

fixed time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L285-L287 · SEEK_SET branch。仅静态来源，不证明已编译/执行。

### FS114-E027 [B_FAT_CUR]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
		case SEEK_CUR:
				pos =  filp->f_pos + offset;
			break;
```

fixed time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L289-L291 · SEEK_CUR branch。仅静态来源，不证明已编译/执行。

### FS114-E028 [B_FAT_END]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
		case SEEK_END:
				pos = filp->f_path.dentry->d_inode->i_size + offset;
			break;
```

fixed time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L293-L295 · SEEK_END branch。仅静态来源，不证明已编译/执行。

### FS114-E029 [B_FAT_DEFAULT]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
		default:
			return -EINVAL;
			break;
```

fixed time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L297-L299 · default -EINVAL。仅静态来源，不证明已编译/执行。

### FS114-E030 [B_FAT_RANGE]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
	if(pos < 0 || pos > filp->f_path.dentry->d_inode->i_size)
		return -EOVERFLOW;
```

fixed time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L302-L303 · out-of-range -EOVERFLOW。仅静态来源，不证明已编译/执行。

### FS114-E031 [B_FAT_WRITE]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
	filp->f_pos = pos;
```

fixed time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L305-L305 · sole f_pos write。仅静态来源，不证明已编译/执行。

### FS114-E032 [B_FAT_RETURN]

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
	return pos;
```

fixed time@a039d9803ade · mykernel/fs/fat/myos_fat32.c#L308-L308 · backend return candidate。仅静态来源，不证明已编译/执行。

### FS114-E033 [C_ZERO_ALIAS_EARLY]

[VERIFIED mykernel/drivers/char/mem.c::zero_lseek]
```c
#define zero_lseek null_lseek
```

fixed time@a039d9803ade · mykernel/drivers/char/mem.c#L42-L42 · first zero alias。仅静态来源，不证明已编译/执行。

### FS114-E034 [C_FULL_ALIAS_EARLY]

[VERIFIED mykernel/drivers/char/mem.c::full_lseek]
```c
#define full_lseek null_lseek
```

fixed time@a039d9803ade · mykernel/drivers/char/mem.c#L43-L43 · first full alias。仅静态来源，不证明已编译/执行。

### FS114-E035 [C_NULL_LSEEK]

[VERIFIED mykernel/drivers/char/mem.c::null_lseek]
```c
	return file->f_pos = 0;
```

fixed time@a039d9803ade · mykernel/drivers/char/mem.c#L55-L55 · set f_pos=0 and return。仅静态来源，不证明已编译/执行。

### FS114-E036 [C_ZERO_ALIAS_LATE]

[VERIFIED mykernel/drivers/char/mem.c::zero_lseek]
```c
#define zero_lseek	null_lseek
```

fixed time@a039d9803ade · mykernel/drivers/char/mem.c#L111-L111 · second zero alias。仅静态来源，不证明已编译/执行。

### FS114-E037 [C_FULL_ALIAS_LATE]

[VERIFIED mykernel/drivers/char/mem.c::full_lseek]
```c
#define full_lseek	null_lseek
```

fixed time@a039d9803ade · mykernel/drivers/char/mem.c#L112-L112 · second full alias。仅静态来源，不证明已编译/执行。

### FS114-E038 [C_NULL_FOPS]

[VERIFIED mykernel/drivers/char/mem.c::null_fops]
```c
	.llseek		= null_lseek,
```

fixed time@a039d9803ade · mykernel/drivers/char/mem.c#L126-L126 · null llseek entry。仅静态来源，不证明已编译/执行。

### FS114-E039 [C_ZERO_FOPS]

[VERIFIED mykernel/drivers/char/mem.c::zero_fops]
```c
		.llseek		= zero_lseek,
```

fixed time@a039d9803ade · mykernel/drivers/char/mem.c#L142-L142 · zero llseek entry。仅静态来源，不证明已编译/执行。

### FS114-E040 [C_DEV_NULL]

[VERIFIED mykernel/drivers/char/mem.c::devlist]
```c
	[3] = {"null", 0666, &null_fops, FMODE_NOWAIT},
```

fixed time@a039d9803ade · mykernel/drivers/char/mem.c#L170-L170 · devlist minor3 null。仅静态来源，不证明已编译/执行。

### FS114-E041 [C_DEV_ZERO]

[VERIFIED mykernel/drivers/char/mem.c::devlist]
```c
	[5] = {"zero", 0666, &zero_fops, FMODE_NOWAIT},
```

fixed time@a039d9803ade · mykernel/drivers/char/mem.c#L174-L174 · devlist minor5 zero。仅静态来源，不证明已编译/执行。

### FS114-E042 [C_DEV_FULL]

[VERIFIED mykernel/drivers/char/mem.c::devlist]
```c
	[7] = {"full", 0666, &full_fops, 0},
```

fixed time@a039d9803ade · mykernel/drivers/char/mem.c#L175-L175 · devlist minor7 full。仅静态来源，不证明已编译/执行。

### FS114-E043 [C_NOOP_RETURN]

[VERIFIED mykernel/fs/vfs/read_write.c::noop_llseek]
```c
	return file->f_pos;
```

fixed time@a039d9803ade · mykernel/fs/vfs/read_write.c#L44-L44 · returns existing f_pos。仅静态来源，不证明已编译/执行。

### FS114-E044 [C_DEFAULT_NOOP]

[VERIFIED mykernel/drivers/char/char_dev.c::def_chr_fops]
```c
const file_ops_s def_chr_fops = {
	.open = chrdev_open,
	.llseek = noop_llseek,
};
```

fixed time@a039d9803ade · mykernel/drivers/char/char_dev.c#L95-L98 · default char fops binds noop。仅静态来源，不证明已编译/执行。

### FS114-E045 [C_CHR_SWAP]

[VERIFIED mykernel/drivers/char/char_dev.c::chrdev_open]
```c
	// replace_fops(filp, fops);
	filp->f_op = fops;
	if (filp->f_op->open) {
		ret = filp->f_op->open(inode, filp);
```

fixed time@a039d9803ade · mykernel/drivers/char/char_dev.c#L75-L78 · char open swaps f_op。仅静态来源，不证明已编译/执行。

### FS114-E046 [C_REGISTER]

[VERIFIED mykernel/drivers/char/mem.c::chr_dev_init]
```c
		cdev_init(&memdev.cdev, memdev.fops);
		if (cdev_add(&memdev.cdev, devt, 1))
			color_printk(RED, BLACK, "Couldn't register %s driver\n", memdev.name);
		else
			myos_device_create(mem_class, devt, memdev.name);
```

fixed time@a039d9803ade · mykernel/drivers/char/mem.c#L199-L203 · attempt device registration。仅静态来源，不证明已编译/执行。

### FS114-E047 [D_NEG_GATE]

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	} else if (nsec < 0) {
```

fixed time@a039d9803ade · mykernel/time/misc/time_misc.c#L58-L58 · true negative condition。仅静态来源，不证明已编译/执行。

### FS114-E048 [D_ZERO_INIT]

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	timespec64_s ts = { 0, 0 };
	s32 rem;
```

fixed time@a039d9803ade · mykernel/time/misc/time_misc.c#L52-L53 · initialized pair and s32 rem。仅静态来源，不证明已编译/执行。

### FS114-E049 [D_POS_BODY]

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	if (likely(nsec > 0)) {
		ts.tv_sec = div_u64_rem(nsec, NSEC_PER_SEC, &rem);
		ts.tv_nsec = rem;
```

fixed time@a039d9803ade · mykernel/time/misc/time_misc.c#L55-L57 · positive div and writes。仅静态来源，不证明已编译/执行。

### FS114-E050 [D_NEG_BODY]

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
		ts.tv_sec = -div_u64_rem(-nsec - 1, NSEC_PER_SEC, &rem) - 1;
		ts.tv_nsec = NSEC_PER_SEC - rem - 1;
```

fixed time@a039d9803ade · mykernel/time/misc/time_misc.c#L64-L65 · negative unsigned quotient and signed field。仅静态来源，不证明已编译/执行。

### FS114-E051 [D_RETURN]

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	return ts;
```

fixed time@a039d9803ade · mykernel/time/misc/time_misc.c#L68-L68 · return ts for zero or nonzero。仅静态来源，不证明已编译/执行。

### FS114-E052 [D_DIV_BODY]

[VERIFIED mykernel/include/linux/kernel/math64.h::div_u64_rem]
```c
		*remainder = dividend % divisor;
		return dividend / divisor;
```

fixed time@a039d9803ade · mykernel/include/linux/kernel/math64.h#L43-L44 · u64 quotient and u32 pointer remainder。仅静态来源，不证明已编译/执行。

### FS114-E053 [D_CLK_RETURN]

[VERIFIED mykernel/time/syscall.c::clock_gettime]
```c
	error = kc->clock_get_timespec(which_clock, &kernel_tp);
```

fixed time@a039d9803ade · mykernel/time/syscall.c#L44-L44 · clock callback return into error。仅静态来源，不证明已编译/执行。

### FS114-E054 [D_CLK_GUARD]

[VERIFIED mykernel/time/syscall.c::clock_gettime]
```c
	if (!error && put_timespec64(&kernel_tp, tp))
		error = -EFAULT;
```

fixed time@a039d9803ade · mykernel/time/syscall.c#L46-L47 · short circuit !error and put。仅静态来源，不证明已编译/执行。

### FS114-E055 [D_EQUAL]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_equal]
```c
			return (a->tv_sec == b->tv_sec) && (a->tv_nsec == b->tv_nsec);
```

fixed time@a039d9803ade · mykernel/time/misc/time_misc.h#L47-L47 · two fields equal。仅静态来源，不证明已编译/执行。

### FS114-E056 [D_CMP_SEC_LT]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_compare]
```c
			if (lhs->tv_sec < rhs->tv_sec)
				return -1;
```

fixed time@a039d9803ade · mykernel/time/misc/time_misc.h#L58-L59 · seconds lower branch。仅静态来源，不证明已编译/执行。

### FS114-E057 [D_CMP_SEC_GT]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_compare]
```c
			if (lhs->tv_sec > rhs->tv_sec)
				return 1;
```

fixed time@a039d9803ade · mykernel/time/misc/time_misc.h#L60-L61 · seconds greater branch。仅静态来源，不证明已编译/执行。

### FS114-E058 [D_CMP_NSEC]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_compare]
```c
			return lhs->tv_nsec - rhs->tv_nsec;
```

fixed time@a039d9803ade · mykernel/time/misc/time_misc.h#L62-L62 · same sec nanoseconds difference。仅静态来源，不证明已编译/执行。

## POSITION_ONLY 普通位置，不计入P2

- FS114-P01 — `mykernel/fs/syscall.c#L75-91`；lseek完整函数体/参数签名和没有f_pos写入的本体范围。
- FS114-P02 — `mykernel/include/uapi/asm-generic/posix_types.h#L15-18`；__kernel_long_t定义的条件guard。
- FS114-P03 — `mykernel/include/uapi/asm-generic/posix_types.h#L88-89`；__kernel_off_t long和__kernel_loff_t long long。
- FS114-P04 — `mykernel/include/linux/kernel/types.h#L37`；off_t=__kernel_off_t。
- FS114-P05 — `mykernel/include/linux/kernel/types.h#L61-63`；loff_t=__kernel_loff_t。
- FS114-P06 — `mykernel/include/linux/fs/fs.h#L1858-1863`；llseek函数指针签名loff_t(file*,loff_t,int)。
- FS114-P07 — `mykernel/include/linux/fs/fs.h#L869-880`；file成员f_pos/f_op类型。
- FS114-P08 — `mykernel/include/linux/fs/fs.h#L581-583`；inode的i_size为loff_t。
- FS114-P09 — `mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L241-251`；宏完整体long __do_sys/__se_sys及x64 stub。
- FS114-P10 — `mykernel/arch/x86_64/entry/syscall_64.c#L26-33`；活动switch完整体。
- FS114-P11 — `mykernel/fs/fat/myos_fat32.c#L278-309`；FAT32_lseek完整体和唯一f_pos写入。
- FS114-P12 — `mykernel/fs/fat/myos_fat32.c#L354-364`；FAT32_file_ops操作表。
- FS114-P13 — `mykernel/fs/fat/inode.c#L167-181`；regular inode分支绑定。
- FS114-P14 — `mykernel/fs/vfs/open.c#L46-65`；myos_do_dentry_open的f_op绑定；正常末端无显式return。
- FS114-P15 — `mykernel/drivers/char/mem.c#L42-56`；两个早期别名及null_lseek整个函数。
- FS114-P16 — `mykernel/drivers/char/mem.c#L111-155`；后期别名与null/zero/full操作表，full .llseek为注释。
- FS114-P17 — `mykernel/drivers/char/mem.c#L166-181`；devlist表，条件注册项及active null/zero/full。
- FS114-P18 — `mykernel/fs/vfs/read_write.c#L42-45`；noop_llseek完整体，仅return f_pos。
- FS114-P19 — `mykernel/drivers/char/char_dev.c#L90-98`；def_chr_fops绑定noop；打开时可更换。
- FS114-P20 — `mykernel/drivers/char/mem.c#L183-208`；chr_dev_init注册尝试，非运行证据。
- FS114-P21 — `mykernel/time/misc/time_misc.c#L50-69`；ns_to_timespec64完整体，零路径回初始值。
- FS114-P22 — `mykernel/include/linux/kernel/math64.h#L41-45`；div_u64_rem的u32*形参及u64返回。
- FS114-P23 — `mykernel/time/misc/time_misc_types.h#L7-10`；tv_sec=time64_t，tv_nsec=long。
- FS114-P24 — `mykernel/time/syscall.c#L33-50`；clock_gettime完整体与短路语义。
- FS114-P25 — `mykernel/time/misc/time_misc.h#L44-63`；equal/compare各自完整体，int返回。
- FS114-P26 — `mykernel/include/uapi/linux/fs.h#L43-48`；六个独立宏及其位置。
- FS114-P27 — `mykernel/drivers/char/char_dev.c#L35-88`；chrdev_open完整体与f_op替换。
- FS114-P28 — `mykernel/include/linux/kernel/syscalls.h#L1383-1389`；MYOS_SYSCALL_DEFINE3展开链。

## 历史锚冻结引用

- TS113-E049～E057：112六例往返/113附记的原源位置；本批只追加五条局部前提。
- TS113-E028/E029：clock读取回调及短路；TS113-E039–E042：equal/compare的分立证明。

本批新P2标签58；原文逐字校对58；单一定义归属58；降级0；普通位置28。

