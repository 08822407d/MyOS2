# 第116批固定time证据账

每个新增P2标签以固定time中的单一函数、宏、结构体或数组初始化为归属，连续1–5物理行。只有注释、声明、整函数否定证明及宏实例者另列普通位置；旧115证据与验收数不改。

### DS116-E001 [D_CMAKE_SET]

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```c
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
```

固定time@a039d9803ade；mykernel/scripts/options_flags.cmake#L58–58；CMake variable correct name。只支持静态条件，编译运行NOT_RUN。

### DS116-E002 [D_CMAKE_CONFIG]

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```c
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
```

固定time@a039d9803ade；mykernel/scripts/options_flags.cmake#L65–65；wrapper switch at65。只支持静态条件，编译运行NOT_RUN。

### DS116-E003 [D_INT_MAX]

[VERIFIED mykernel/include/linux/kernel/limits.h::INT_MAX]
```c
	#define INT_MAX			((int)(~0U >> 1))
```

固定time@a039d9803ade；mykernel/include/linux/kernel/limits.h#L30–30；INT_MAX macro。只支持静态条件，编译运行NOT_RUN。

### DS116-E004 [D_PAGE_MASK]

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_MASK]
```c
	#define PAGE_MASK				(~(PAGE_SIZE-1))
```

固定time@a039d9803ade；mykernel/arch/x86_64/mm/page/page_const_arch.h#L11–11；PAGE_MASK macro。只支持静态条件，编译运行NOT_RUN。

### DS116-E005 [D_BUG_ON]

[VERIFIED mykernel/include/linux/kernel/asm-generic/bug.h::BUG_ON]
```c
	#			define BUG_ON(condition) do { if (unlikely(condition)) BUG(); } while (0)
```

固定time@a039d9803ade；mykernel/include/linux/kernel/asm-generic/bug.h#L66–66；BUG_ON macro。只支持静态条件，编译运行NOT_RUN。

### DS116-E006 [D_READ_ITER_GUARD]

[VERIFIED mykernel/fs/vfs/read_write.c::new_sync_read]
```c
	ret = filp->f_op->read_iter(&kiocb, &iter);
	BUG_ON(ret == -EIOCBQUEUED);
	if (ppos)
		*ppos = kiocb.ki_pos;
	return ret;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L88–92；read iteration BUG_ON then write local position。只支持静态条件，编译运行NOT_RUN。

### DS116-E007 [D_WRITE_ITER_GUARD]

[VERIFIED mykernel/fs/vfs/read_write.c::new_sync_write]
```c
	ret = filp->f_op->write_iter(&kiocb, &iter);
	BUG_ON(ret == -EIOCBQUEUED);
	if (ret > 0 && ppos)
		*ppos = kiocb.ki_pos;
	return ret;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L212–216；write iteration BUG_ON then conditional pos update。只支持静态条件，编译运行NOT_RUN。

### DS116-E008 [D_READ_KSYS_CALL]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_read]
```c
		ret = vfs_read(f.file, buf, count, ppos);
		if (ret >= 0 && ppos)
			f.file->f_pos = pos;
		fdput_pos(f);
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L191–194；actual VFS call and f_pos commit。只支持静态条件，编译运行NOT_RUN。

### DS116-E009 [D_WRITE_KSYS_CALL]

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]
```c
		ret = vfs_write(filp, buf, count, ppos);
		if (ret >= 0 && ppos)
			filp->f_pos = pos;
		fdput_pos(f);
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L331–334；actual VFS call and f_pos commit。只支持静态条件，编译运行NOT_RUN。

### DS116-E010 [D_VFS_READ_CAP]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_read]
```c
	ret = rw_verify_area(READ, file, pos, count);
	if (ret)
		return ret;
	if (count > MAX_RW_COUNT)
		count =  MAX_RW_COUNT;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L154–158；read check and cap。只支持静态条件，编译运行NOT_RUN。

### DS116-E011 [D_VFS_WRITE_CAP]

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	ret = rw_verify_area(WRITE, file, pos, count);
	if (ret)
		return ret;
	if (count > MAX_RW_COUNT)
		count =  MAX_RW_COUNT;
```

固定time@a039d9803ade；mykernel/fs/vfs/read_write.c#L298–302；write check and cap。只支持静态条件，编译运行NOT_RUN。

### DS116-E012 [D_ZERO_LOOP]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
	while (count) {
		size_t chunk = min_t(size_t, count, PAGE_SIZE);
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L87–88；while count and chunk。只支持静态条件，编译运行NOT_RUN。

### DS116-E013 [D_ZERO_BREAK]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
		memset(buf + cleared, 0, chunk);
		count -= chunk;
		if (count == 0)
			break;
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L90–93；memset then break test。只支持静态条件，编译运行NOT_RUN。

### DS116-E014 [D_ZERO_ACCUM]

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
		cleared += chunk;
```

固定time@a039d9803ade；mykernel/drivers/char/mem.c#L95–95；cleared increment after break。只支持静态条件，编译运行NOT_RUN。

### DS116-E015 [A_SHELL_BIND]

[VERIFIED myinitramfs/myshell/myshell.c::shell_internal_cmd]
```c
builtincmd_s shell_internal_cmd[] = 
{
	{"cd",		cd_command},
	{"ls",		ls_command},
	{"pwd",		pwd_command},
```

固定time@a039d9803ade；myinitramfs/myshell/myshell.c#L44–48；ls builtin table。只支持静态条件，编译运行NOT_RUN。

### DS116-E016 [A_SHELL_RUN]

[VERIFIED myinitramfs/myshell/myshell.c::run_cmd]
```c
void run_cmd(int index, int argc, char **argv)
{
	shell_internal_cmd[index].function(argc, argv);
}
```

固定time@a039d9803ade；myinitramfs/myshell/myshell.c#L139–142；dispatch via builtin function。只支持静态条件，编译运行NOT_RUN。

### DS116-E017 [A_SHELL_DEFAULT]

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
	if (argc <=1)
		path = ".";
	else
		path = argv[1];
```

固定time@a039d9803ade；myinitramfs/myshell/myshell.c#L219–222；default dot or explicit argv。只支持静态条件，编译运行NOT_RUN。

### DS116-E018 [A_SHELL_ERROR]

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
	dir = opendir(path);
	if (dir == NULL)
	{
		printf("ls: %s: %s\n", strerror(errno), path);
		return -1;
```

固定time@a039d9803ade；myinitramfs/myshell/myshell.c#L224–228；opendir null error early return。只支持静态条件，编译运行NOT_RUN。

### DS116-E019 [A_OPEN_FLAGS]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::opendir_oflags]
```c
enum
{
	opendir_oflags = O_RDONLY | O_DIRECTORY | O_CLOEXEC
};
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L33–36；active open flags。只支持静态条件，编译运行NOT_RUN。

### DS116-E020 [A_BAD_NAME]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::invalid_name]
```c
invalid_name(const char *name)
{
	if (name[0] == '\0')
	{
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L40–43；empty name detected。只支持静态条件，编译运行NOT_RUN。

### DS116-E021 [A_OPEN_CHECK]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::opendir]
```c
DIR *opendir(const char *name)
{
	if (invalid_name(name))
		return NULL;
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L63–66；invalid_name check and null return。只支持静态条件，编译运行NOT_RUN。

### DS116-E022 [A_OPEN_TO_ALLOC]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::opendir]
```c
	int fd = open(name, opendir_oflags, O_RDONLY);

	return __alloc_dir(fd, true, 0);
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L68–70；open fd then alloc_dir no fd guard。只支持静态条件，编译运行NOT_RUN。

### DS116-E023 [A_MAX_BUFFER]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::__alloc_dir]
```c
	enum
	{
		max_buffer_size = 1048576
	};
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L84–87；max_buffer_size constant。只支持静态条件，编译运行NOT_RUN。

### DS116-E024 [A_BUFFER_ALLOC_SIZE]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::__alloc_dir]
```c
	enum
	{
		allocation_size = 32768
	};
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L89–92；allocation_size 32768 constant。只支持静态条件，编译运行NOT_RUN。

### DS116-E025 [A_MALLOC]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::__alloc_dir]
```c
	size_t allocation = allocation_size;

	DIR *dirp = (DIR *)malloc(sizeof(DIR) + allocation);
	if (dirp == NULL)
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L100–103；allocate sizeof DIR plus fixed bytes。只支持静态条件，编译运行NOT_RUN。

### DS116-E026 [A_MALLOC_NULL]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::__alloc_dir]
```c
	if (dirp == NULL)
	{
		// if (close_fd)
		// 	__close_nocancel_nostatus(fd);
		return NULL;
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L103–107；malloc null returns without actual close。只支持静态条件，编译运行NOT_RUN。

### DS116-E027 [A_SAVE_FD]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::__alloc_dir]
```c
	dirp->fd = fd;
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L110–110；store supplied fd。只支持静态条件，编译运行NOT_RUN。

### DS116-E028 [A_INIT_FIELDS]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::__alloc_dir]
```c
	dirp->allocation = allocation;
	dirp->size = 0;
	dirp->offset = 0;
	dirp->filepos = 0;
	dirp->errcode = 0;
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L114–118；allocation size offset filepos errcode initialized。只支持静态条件，编译运行NOT_RUN。

### DS116-E029 [A_RETURN]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::__alloc_dir]
```c
	return dirp;
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L120–120；return directory stream。只支持静态条件，编译运行NOT_RUN。

### DS116-E030 [A_DIR_FD]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/dirstream.h::struct __dirstream]
```c
	struct __dirstream
	{
		int fd; /* File descriptor.  */
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/dirstream.h#L30–32；DIR fd field。只支持静态条件，编译运行NOT_RUN。

### DS116-E031 [A_DIR_CACHE]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/dirstream.h::struct __dirstream]
```c
		size_t allocation; /* Space allocated for the block.  */
		size_t size;		   /* Total valid data in the block.  */
		size_t offset;		   /* Current offset into the block.  */

		off_t filepos; /* Position of next entry to read.  */
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/dirstream.h#L36–40；allocation size offset and filepos fields。只支持静态条件，编译运行NOT_RUN。

### DS116-E032 [A_DIR_ERR]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/dirstream.h::struct __dirstream]
```c
		off_t filepos; /* Position of next entry to read.  */

		int errcode; /* Delayed error code.  */
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/dirstream.h#L40–42；filepos and errcode。只支持静态条件，编译运行NOT_RUN。

### DS116-E033 [A_DIR_DATA]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/dirstream.h::struct __dirstream]
```c
		with what malloc uses.  */
		char data[0] __attribute__((aligned(__alignof__(long double))));
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/dirstream.h#L49–50；aligned internal buffer char data0。只支持静态条件，编译运行NOT_RUN。

### DS116-E034 [B_REFILL_GATE]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c::readdir]
```c
		size_t reclen;

		if (dirp->offset >= dirp->size)
		{
			/* We've emptied out our buffer.  Refill it.  */
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L37–41；offset >= size triggers refill。只支持静态条件，编译运行NOT_RUN。

### DS116-E035 [B_REFILL_CAP]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c::readdir]
```c
			if (maxread > INT_MAX)
				maxread = INT_MAX;
			bytes = getdents64(dirp->fd, dirp->data, maxread);
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L47–49；maxread INT_MAX clamp then getdents64。只支持静态条件，编译运行NOT_RUN。

### DS116-E036 [B_REFILL_NONPOS]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c::readdir]
```c
			bytes = getdents64(dirp->fd, dirp->data, maxread);
			if (bytes <= 0)
			{
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L49–51；bytes <= 0 test。只支持静态条件，编译运行NOT_RUN。

### DS116-E037 [B_REFILL_NULL]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c::readdir]
```c
				// dp = NULL;
				// break;
				return NULL;
			}
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L61–64；NULL return in no-data branch。只支持静态条件，编译运行NOT_RUN。

### DS116-E038 [B_REFILL_INIT]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c::readdir]
```c
			dirp->size = (size_t)bytes;

			/* Reset the offset into the buffer.  */
			dirp->offset = 0;
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L65–68；positive bytes set size and reset offset。只支持静态条件，编译运行NOT_RUN。

### DS116-E039 [B_BUFFER_PTR]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c::readdir]
```c
		dp = (struct dirent64 *)&dirp->data[dirp->offset];

		reclen = dp->d_reclen;
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L71–73；entry pointer to data offset and reclen。只支持静态条件，编译运行NOT_RUN。

### DS116-E040 [B_CURSOR]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c::readdir]
```c
		reclen = dp->d_reclen;

		dirp->offset += reclen;

		dirp->filepos = dp->d_off;
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L73–77；offset += reclen and filepos=d_off。只支持静态条件，编译运行NOT_RUN。

### DS116-E041 [B_RETURN_ENTRY]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c::readdir]
```c
	return dp;
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L82–82；return stored entry pointer。只支持静态条件，编译运行NOT_RUN。

### DS116-E042 [B_ASM_SYSMACRO]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::SYSFUNC_DEF]
```c
#define SYSFUNC_DEF(name)						\
			_SYSFUNC_DEF_(name, __NR_##name)
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/syscall/syscall.c#L13–14；construct syscall function macro。只支持静态条件，编译运行NOT_RUN。

### DS116-E043 [B_ASM_GEN]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::__SYSFUNC_DEF__]
```c
#define __SYSFUNC_DEF__(name, nr)							\
			__asm__	(										\
				".global	"#name"					\n\t"	\
				".type		"#name",	@function	\n\t"	\
				#name":								\n\t"	\
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/syscall/syscall.c#L19–23；generate public function name in assembly。只支持静态条件，编译运行NOT_RUN。

### DS116-E044 [B_ASM_NUMBER]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::__SYSFUNC_DEF__]
```c
				"pushq	%rbp						\n\t"	\
				"movq	%rsp,	%rbp				\n\t"	\
				"movq	$"#nr",	%rax				\n\t"	\
				"jmp	LABEL_SYSCALL				\n\t"	\
			);
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/syscall/syscall.c#L24–28；load nr into rax and jump。只支持静态条件，编译运行NOT_RUN。

### DS116-E045 [B_ASM_SYSCALL]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::LABEL_SYSCALL]
```c
	"LABEL_SYSCALL:							\n\t"
	"leaq	sysexit_ret_addr(%rip),	%rcx	\n\t"
	"syscall								\n\t"
	"sysexit_ret_addr:						\n\t"
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/syscall/syscall.c#L71–74；execute syscall instruction。只支持静态条件，编译运行NOT_RUN。

### DS116-E046 [B_ASM_RET]

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::LABEL_SYSCALL]
```c
	"cmpq	$-0x1000,	%rax				\n\t"
	"jb	LABEL_SYSCALL_RET					\n\t"
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/syscall/syscall.c#L75–76；error check but no errno conversion in active code。只支持静态条件，编译运行NOT_RUN。

### DS116-E047 [B_NR_GETDENTS64]

[VERIFIED mykernel/arch/x86_64/include/asm/unistd_64.h::__NR_getdents64]
```c
	#define __NR_getdents64			217
```

固定time@a039d9803ade；mykernel/arch/x86_64/include/asm/unistd_64.h#L264–264；number217。只支持静态条件，编译运行NOT_RUN。

### DS116-E048 [B_K_RECORD]

[VERIFIED mykernel/include/linux/kernel/dirent.h::linux_dirent64_s]
```c
		u64		d_ino;
		s64		d_off;
		unsigned short	d_reclen;
		unsigned char	d_type;
		char	d_name[];
```

固定time@a039d9803ade；mykernel/include/linux/kernel/dirent.h#L8–12；wire record fields d_ino d_off reclen type name。只支持静态条件，编译运行NOT_RUN。

### DS116-E049 [B_K_INIT]

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
	fd_s f;
	getdents_cbk64_s buf = {
		.ctx.actor = filldir64,
		.count = count,
		.current_dir = dirent
```

固定time@a039d9803ade；mykernel/fs/vfs/readdir.c#L135–139；kernel callback and count initialization。只支持静态条件，编译运行NOT_RUN。

### DS116-E050 [B_K_FPOS]

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
		buf.ctx.pos = f.file->f_pos;
		error = f.file->f_op->iterate_shared(f.file, &buf.ctx);
		f.file->f_pos = buf.ctx.pos;
```

固定time@a039d9803ade；mykernel/fs/vfs/readdir.c#L150–152；kernel f_pos through ctx。只支持静态条件，编译运行NOT_RUN。

### DS116-E051 [B_K_RET_GATE]

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
	if (error >= 0)
		error = buf.error;
	if (buf.prev_reclen) {
```

固定time@a039d9803ade；mykernel/fs/vfs/readdir.c#L155–157；error and prev record gate。只支持静态条件，编译运行NOT_RUN。

### DS116-E052 [B_K_BYTE_RET]

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
			error = count - buf.count;
	}
	fdput_pos(f);
	return error;
```

固定time@a039d9803ade；mykernel/fs/vfs/readdir.c#L165–168；return consumed bytes。只支持静态条件，编译运行NOT_RUN。

### DS116-E053 [B_U_DIRENT64]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/dirent.h::struct dirent64]
```c
		__ino64_t	d_ino;
		__off64_t	d_off;
		unsigned short int	d_reclen;
		unsigned char		d_type;
		char				d_name[256]; /* We must not include limits.h! */
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/dirent.h#L40–44；largefile64 struct member fields。只支持静态条件，编译运行NOT_RUN。

### DS116-E054 [B_U_DIRENT]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/dirent.h::struct dirent]
```c
	__ino_t		d_ino;
	__off_t		d_off;
#else
	__ino64_t	d_ino;
	__off64_t	d_off;
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/dirent.h#L25–29；ordinary dirent member fields。只支持静态条件，编译运行NOT_RUN。

### DS116-E055 [B_U_LAYOUT]

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/dirent.h::struct dirent64]
```c
		unsigned short int	d_reclen;
		unsigned char		d_type;
		char				d_name[256]; /* We must not include limits.h! */
```

固定time@a039d9803ade；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/dirent.h#L42–44；reclen type and d_name bytes。只支持静态条件，编译运行NOT_RUN。

### DS116-E056 [C_LS_GET]

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
	while(1)
	{
		buf = readdir(dir);
		if(buf == NULL)
			break;
```

固定time@a039d9803ade；myinitramfs/myshell/myshell.c#L231–235；one readdir per loop and NULL break。只支持静态条件，编译运行NOT_RUN。

### DS116-E057 [C_LS_FILTER]

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
		if (strcmp(buf->d_name, ".") != 0 &&
			strcmp(buf->d_name, "..") != 0)
			printf("%s\t", buf->d_name);
```

固定time@a039d9803ade；myinitramfs/myshell/myshell.c#L236–238；dot and dotdot filter and printf。只支持静态条件，编译运行NOT_RUN。

### DS116-E058 [C_LS_CLOSE]

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
	printf("\n");
	closedir(dir);
	return 1;
```

固定time@a039d9803ade；myinitramfs/myshell/myshell.c#L240–242；newline closedir and return1。只支持静态条件，编译运行NOT_RUN。

### DS116-E059 [C_FILL_SIZE]

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	int reclen = ALIGN(offsetof(linux_dirent64_s, d_name) + namelen + 1,
			sizeof(u64));
```

固定time@a039d9803ade；mykernel/fs/vfs/readdir.c#L99–100；record size aligned to u64。只支持静态条件，编译运行NOT_RUN。

### DS116-E060 [C_FILL_OFF]

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	prev->d_off = offset;
	dirent->d_ino = ino;
	dirent->d_reclen = reclen;
	dirent->d_type = d_type;
	cplen = copy_to_user(dirent->d_name, (char *)name, namelen);
```

固定time@a039d9803ade；mykernel/fs/vfs/readdir.c#L113–117；predecessor d_off then other fields and name copy。只支持静态条件，编译运行NOT_RUN。

### DS116-E061 [C_FILL_ACCOUNT]

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	buf->prev_reclen = reclen;
	buf->current_dir = (void *)dirent + reclen;
	buf->count -= reclen;
	return 0;
```

固定time@a039d9803ade；mykernel/fs/vfs/readdir.c#L120–123；advance current_dir and count。只支持静态条件，编译运行NOT_RUN。

### DS116-E062 [C_KERNEL_TAIL]

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
	if (buf.prev_reclen) {
	// 	linux_dirent64_s *lastdirent;
	// 	typeof(lastdirent->d_off) d_off = buf.ctx.pos;

	// 	lastdirent = (void *) buf.current_dir - buf.prev_reclen;
```

固定time@a039d9803ade；mykernel/fs/vfs/readdir.c#L157–161；tail patch remains commented。只支持静态条件，编译运行NOT_RUN。

## POSITION_ONLY：全体/声明/条件门/宏调用实例/负向核对

- DS116-P01 `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/closedir.c#L26–51` — closedir活动函数中只有注释，没有close/free/return。
- DS116-P02 `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L62–121` — opendir/__alloc_dir完整体及错误和资源分支。
- DS116-P03 `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/dirstream.h#L30–51` — DIR结构整体及对齐的零长data。
- DS116-P04 `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L28–83` — readdir整个函数，非正返回不更新三个游标。
- DS116-P05 `myinitramfs/mylib/myglibc/dirent/dirent.h#L122–131` — 不透明DIR类型及opendir声明。
- DS116-P06 `myinitramfs/mylib/myglibc/dirent/dirent.h#L141–172` — closedir和readdir声明，后者是dirent64指针。
- DS116-P07 `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/dirent.h#L22–47` — struct dirent与dirent64字段，后者受LARGEFILE64条件。
- DS116-P08 `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/dirent_ext.h#L23–30` — getdents64声明受__USE_GNU条件约束。
- DS116-P09 `myinitramfs/mylib/myglibc/syscall/syscall.c#L66–66` — SYSFUNC_DEF(getdents64)是宏调用实例，非定义。
- DS116-P10 `mykernel/arch/x86_64/include/asm/syscalls_64.h#L67–67` — 内核__SYSCALL(getdents64)映射调用实例。
- DS116-P11 `mykernel/arch/x86_64/entry/syscall_64.c#L26–33` — 活动switch展开映射，宏定义另引旧证据。
- DS116-P12 `mykernel/fs/vfs/readdir.c#L92–129` — filldir64完整函数含前项d_off与返回长度。
- DS116-P13 `mykernel/fs/vfs/readdir.c#L132–169` — getdents64完整体，尾记录d_off补写被注释。
- DS116-P14 `mykernel/include/linux/kernel/dirent.h#L7–13` — 内核用户记录成员、柔性数组。
- DS116-P15 `mykernel/fs/fat/myos_fat32.c#L311–351` — FAT目录迭代普通有效路径来自第59批。
- DS116-P16 `myinitramfs/myshell/myshell.c#L214–242` — ls_command完整体及失败/成功返回。
- DS116-P17 `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L84–100` — max_buffer_size只定义而非实际分配上界。
- DS116-P18 `mykernel/include/linux/kernel/limits.h#L30–30` — INT_MAX本身已取短锚；目标int值域条件。
- DS116-P19 `mykernel/include/linux/kernel/asm-generic/bug.h#L56–67` — BUG_ON条件定义与panic/完整体。
- DS116-P20 `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/dirent.h#L56–61` — 结构等价判断受目标宏定义约束。
- DS116-P21 `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L103–108` — malloc失败分支关闭语句是注释。
- DS116-P22 `myinitramfs/mylib/myglibc/syscall/syscall.c#L70–82` — 汇编返回，errno写入代码是注释。
- DS116-P23 `myinitramfs/myshell/myshell.c#L44–56` — shell内建命令列表中ls函数指针。
- DS116-P24 `myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L49–69` — getdents非正返回不改size/offset/filepos。
- DS116-P25 `mykernel/fs/vfs/read_write.c#L75–93` — 同步read_iter的BUG_ON前置门。
- DS116-P26 `mykernel/fs/vfs/read_write.c#L202–217` — 同步write_iter的BUG_ON前置门。
- DS116-P27 `mykernel/drivers/char/mem.c#L82–109` — read_zero循环和零次路径的历史完整体。

历史第115批冻结：70段文字原文命中，69个原标签定义归属正确，六件整体出现72次旧协议前缀。新增DS116锚与旧数量分开。

本批新增短引文62条；普通位置27处。
