---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-STDIO-OPEN-FLAG-ABI-122
record_type: source_windows_and_ordinary_positions
produced_by: "GPT-6（当前会话自报，后端/effort不可独立认证）"
model_per_owner: "GPT-6＋Extra（Owner明确指定）"
date: 2026-11-22
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
open_questions: []
inputs_read: ["myinitramfs/mylib/myglibc/libio/fileops.c","myinitramfs/mylib/myglibc/libio/iofopen.c","myinitramfs/mylib/myglibc/bits/fcntl.h","myinitramfs/mylib/myglibc/include/features.h","myinitramfs/mylib/myglibc/libio/libio.h","myinitramfs/mylib/myglibc/syscall/syscall.c","myinitramfs/myshell/myshell.c","mykernel/fs/vfs/namei.c","mykernel/include/uapi/asm-generic/fcntl.h","mykernel/include/linux/kernel/fcntl.h","mykernel/fs/vfs/open.c","mykernel/fs/syscall.c","mykernel/include/uapi/linux/openat2.h","mykernel/include/linux/kernel/stat.h","mykernel/include/uapi/linux/stat.h","mykernel/arch/x86_64/include/asm/unistd_64.h","mykernel/arch/x86_64/entry/syscall_64.c","mykernel/scripts/options_flags.cmake"]
---

# 第122批真实源码短窗

仅取固定time短12=`a039d9803ade`源的本批函数/宏/结构体内1–5行连续短窗；`set(CMAKE_C_FLAGS)`依172合同作为活动构建命令自身范围单独列。条件宏的定义位置是源码事实，某次编译是否真的定义宏仍需选定预处理配置。全局调用实例、extern声明、include/构建来源导航全部另作普通位置，不拿行号冒充枚举数值。

### ABI122-E001 · 底层helper五形参原型包含FILE*、filename、posix_mode、prot、read_write

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_file_open]
```c
FILE *_IO_file_open(FILE *fp, const char *filename,
		int posix_mode, int prot, int read_write)
{
	int fdesc;

```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L173-L177`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E002 · 实际open只有filename、flags、prot三参及负fd/写_fileno

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_file_open]
```c
	fdesc = open(filename, posix_mode | O_LARGEFILE, prot);
	if (fdesc < 0)
		return NULL;
	fp->_fileno = fdesc;
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L178-L181`，原物理行4行；仅支持该符号内局部源码命题。

### ABI122-E003 · 底层成功返回同一个fp

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_file_open]
```c
	return fp;
}
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L196-L197`，原物理行2行；仅支持该符号内局部源码命题。

### ABI122-E004 · 模式函数进入及0666保护模式

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
FILE *_IO_new_file_fopen(FILE *fp, const char *filename, const char *mode)
{
	int oflags = 0, omode;
	int read_write;
	int oprot = 0666;
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L199-L203`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E005 · FILE已有fd早退及模式首字符入口

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	if (fp->_fileno != -1)
		return 0;
	switch (*mode)
	{
	case 'r':
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L209-L213`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E006 · r初始O_RDONLY

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	case 'r':
		omode = O_RDONLY;
		read_write = _IO_NO_WRITES;
		break;
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L213-L216`，原物理行4行；仅支持该符号内局部源码命题。

### ABI122-E007 · w初始O_WRONLY和O_CREAT/O_TRUNC

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	case 'w':
		omode = O_WRONLY;
		oflags = O_CREAT | O_TRUNC;
		read_write = _IO_NO_READS;
		break;
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L217-L221`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E008 · a初始O_APPEND并计算内部追加标志

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	case 'a':
		omode = O_WRONLY;
		oflags = O_CREAT | O_APPEND;
		read_write = _IO_NO_READS | _IO_IS_APPENDING;
		break;
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L222-L226`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E009 · 非法首字符直接NULL

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	default:
		// __set_errno(EINVAL);
		return NULL;
	}
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L227-L230`，原物理行4行；仅支持该符号内局部源码命题。

### ABI122-E010 · 后缀扫描入口及递增游标

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	last_recognized = mode;
	for (i = 1; i < 7; ++i)
	{
		switch (*++mode)
		{
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L231-L235`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E011 · 加号读写与内部标志

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
		case '+':
			omode = O_RDWR;
			read_write &= _IO_IS_APPENDING;
			last_recognized = mode;
			continue;
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L238-L242`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E012 · x增加O_EXCL

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
		case 'x':
			oflags |= O_EXCL;
			last_recognized = mode;
			continue;
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L243-L246`，原物理行4行；仅支持该符号内局部源码命题。

### ABI122-E013 · b只更新游标last_recognized

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
		case 'b':
			last_recognized = mode;
			continue;
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L247-L249`，原物理行3行；仅支持该符号内局部源码命题。

### ABI122-E014 · 解析出口调用五参helper

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
		break;
	}

	result = _IO_file_open(fp, filename, omode | oflags, oprot, read_write);
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L264-L267`，原物理行4行；仅支持该符号内局部源码命题。

### ABI122-E015 · 模式解析最终返回内层result

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	return result;
}
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L339-L340`，原物理行2行；仅支持该符号内局部源码命题。

### ABI122-E016 · 仓内外层仅赋FILE._fileno再传下层

[VERIFIED myinitramfs/mylib/myglibc/libio/iofopen.c::_IO_new_fopen]
```c
_IO_new_fopen(const char *filename, const char *mode)
{
	FILE *new_f = malloc(sizeof(FILE));
	new_f->_fileno = -1;
	return _IO_new_file_fopen(new_f, filename, mode);
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/iofopen.c#L83-L87`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E017 · 用户头只读数值0

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_RDONLY]
```c
#define	O_RDONLY	0	/* Open read-only.  */
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/bits/fcntl.h#L25-L25`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E018 · 用户头只写数值1

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_WRONLY]
```c
#define	O_WRONLY	1	/* Open write-only.  */
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/bits/fcntl.h#L26-L26`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E019 · 用户头读写数值2

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_RDWR]
```c
#define	O_RDWR		2	/* Open read/write.  */
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/bits/fcntl.h#L27-L27`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E020 · 用户头创建位512

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_CREAT]
```c
#define	O_CREAT		0x0200	/* Create file if it doesn't exist.  */
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/bits/fcntl.h#L31-L31`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E021 · 用户头排他位2048

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_EXCL]
```c
#define	O_EXCL		0x0800	/* Fail if file already exists.  */
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/bits/fcntl.h#L32-L32`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E022 · 用户头截断位1024

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_TRUNC]
```c
#define	O_TRUNC		0x0400	/* Truncate file to zero length.  */
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/bits/fcntl.h#L33-L33`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E023 · 用户头追加位8

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_APPEND]
```c
#define	O_APPEND	0x0008	/* Writes append to the file.  */
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/bits/fcntl.h#L58-L58`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E024 · 用户O_LARGEFILE条件宏值0

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_LARGEFILE]
```c
	# define O_LARGEFILE	0
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/bits/fcntl.h#L54-L54`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E025 · 用户O_CLOEXEC条件宏数值

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_CLOEXEC]
```c
	# define O_CLOEXEC		0x00400000      /* Set close_on_exec.  */
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/bits/fcntl.h#L45-L45`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E026 · GNU_SOURCE分支选_XOPEN_SOURCE700

[VERIFIED myinitramfs/mylib/myglibc/include/features.h::_XOPEN_SOURCE]
```c
		#define _XOPEN_SOURCE				700
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/include/features.h#L213-L213`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E027 · GNU_SOURCE分支选largefile64

[VERIFIED myinitramfs/mylib/myglibc/include/features.h::_LARGEFILE64_SOURCE]
```c
		#define _LARGEFILE64_SOURCE			1
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/include/features.h#L217-L217`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E028 · 功能宏XOPEN2K8定义条件

[VERIFIED myinitramfs/mylib/myglibc/include/features.h::__USE_XOPEN2K8]
```c
		#define __USE_XOPEN2K8	1
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/include/features.h#L345-L345`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E029 · 功能宏largefile64条件

[VERIFIED myinitramfs/mylib/myglibc/include/features.h::__USE_LARGEFILE64]
```c
		#define __USE_LARGEFILE64	1
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/include/features.h#L381-L381`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E030 · 内部追加值与O_APPEND区分

[VERIFIED myinitramfs/mylib/myglibc/libio/libio.h::_IO_IS_APPENDING]
```c
	#define _IO_IS_APPENDING		0x1000
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/libio.h#L81-L81`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E031 · 内部不可读位

[VERIFIED myinitramfs/mylib/myglibc/libio/libio.h::_IO_NO_READS]
```c
	#define _IO_NO_READS			0x0004		/* Reading not allowed.  */
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/libio.h#L71-L71`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E032 · 内部不可写位

[VERIFIED myinitramfs/mylib/myglibc/libio/libio.h::_IO_NO_WRITES]
```c
	#define _IO_NO_WRITES			0x0008		/* Writing not allowed.  */
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/libio.h#L72-L72`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E033 · 用户syscall桩宏定义起始

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::__SYSFUNC_DEF__]
```c
#define __SYSFUNC_DEF__(name, nr)							\
			__asm__	(										\
				".global	"#name"					\n\t"	\
				".type		"#name",	@function	\n\t"	\
				#name":								\n\t"	\
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/syscall/syscall.c#L19-L23`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E034 · 用户syscall桩装nr至rax并跳转

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::__SYSFUNC_DEF__]
```c
				"pushq	%rbp						\n\t"	\
				"movq	%rsp,	%rbp				\n\t"	\
				"movq	$"#nr",	%rax				\n\t"	\
				"jmp	LABEL_SYSCALL				\n\t"	\
```

固定 `time@a039d9803ade` `myinitramfs/mylib/myglibc/syscall/syscall.c#L24-L27`，原物理行4行；仅支持该符号内局部源码命题。

### ABI122-E035 · 内核创建门宏

[VERIFIED mykernel/fs/vfs/open.c::WILL_CREATE]
```c
#define WILL_CREATE(flags)	(flags & (O_CREAT | __O_TMPFILE))
```

固定 `time@a039d9803ade` `mykernel/fs/vfs/open.c#L23-L23`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E036 · 内核O_PATH过滤宏

[VERIFIED mykernel/fs/vfs/open.c::O_PATH_FLAGS]
```c
#define O_PATH_FLAGS		(O_DIRECTORY | O_NOFOLLOW | O_PATH | O_CLOEXEC)
```

固定 `time@a039d9803ade` `mykernel/fs/vfs/open.c#L24-L24`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E037 · 内核build_open_how函数与flags/mode初始化

[VERIFIED mykernel/fs/vfs/open.c::build_open_how]
```c
open_how_s build_open_how(int flags, umode_t mode)
{
	struct open_how how = {
		.flags = flags & VALID_OPEN_FLAGS,
		.mode = mode & S_IALLUGO,
```

固定 `time@a039d9803ade` `mykernel/fs/vfs/open.c#L27-L31`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E038 · how.flags和how.mode指定初始化

[VERIFIED mykernel/fs/vfs/open.c::build_open_how]
```c
	struct open_how how = {
		.flags = flags & VALID_OPEN_FLAGS,
		.mode = mode & S_IALLUGO,
	};
```

固定 `time@a039d9803ade` `mykernel/fs/vfs/open.c#L29-L32`，原物理行4行；仅支持该符号内局部源码命题。

### ABI122-E039 · O_PATH额外过滤条件

[VERIFIED mykernel/fs/vfs/open.c::build_open_how]
```c
	/* O_PATH beats everything else. */
	if (how.flags & O_PATH)
		how.flags &= O_PATH_FLAGS;
```

固定 `time@a039d9803ade` `mykernel/fs/vfs/open.c#L34-L36`，原物理行3行；仅支持该符号内局部源码命题。

### ABI122-E040 · 无创建位则mode=0并return

[VERIFIED mykernel/fs/vfs/open.c::build_open_how]
```c
	/* Modes should only be set for create-like flags. */
	if (!WILL_CREATE(how.flags))
		how.mode = 0;
	return how;
}
```

固定 `time@a039d9803ade` `mykernel/fs/vfs/open.c#L37-L41`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E041 · do_sys_open构造how并传给下一层

[VERIFIED mykernel/fs/vfs/open.c::do_sys_open]
```c
long do_sys_open(int dfd, const char * filename, int flags, umode_t mode)
{
	open_how_s how = build_open_how(flags, mode);
	return do_sys_openat2(dfd, filename, &how);
}
```

固定 `time@a039d9803ade` `mykernel/fs/vfs/open.c#L266-L270`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E042 · 有效标志宏前五行

[VERIFIED mykernel/include/linux/kernel/fcntl.h::VALID_OPEN_FLAGS]
```c
	#define VALID_OPEN_FLAGS (											\
				O_RDONLY | O_WRONLY | O_RDWR | O_CREAT |				\
				O_EXCL | O_NOCTTY | O_TRUNC |							\
				O_APPEND | O_NDELAY | O_NONBLOCK | __O_SYNC | O_DSYNC | \
				FASYNC	| O_DIRECT | O_LARGEFILE |						\
```

固定 `time@a039d9803ade` `mykernel/include/linux/kernel/fcntl.h#L9-L13`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E043 · 有效标志宏尾四行

[VERIFIED mykernel/include/linux/kernel/fcntl.h::VALID_OPEN_FLAGS]
```c
				FASYNC	| O_DIRECT | O_LARGEFILE |						\
				O_DIRECTORY | O_NOFOLLOW |								\
				O_NOATIME | O_CLOEXEC | O_PATH | __O_TMPFILE			\
			)
```

固定 `time@a039d9803ade` `mykernel/include/linux/kernel/fcntl.h#L13-L16`，原物理行4行；仅支持该符号内局部源码命题。

### ABI122-E044 · 内核O_ACCMODE值3

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_ACCMODE]
```c
	#define O_ACCMODE			00000003
```

固定 `time@a039d9803ade` `mykernel/include/uapi/asm-generic/fcntl.h#L19-L19`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E045 · 内核只读数值0

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_RDONLY]
```c
	#define O_RDONLY			00000000
```

固定 `time@a039d9803ade` `mykernel/include/uapi/asm-generic/fcntl.h#L20-L20`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E046 · 内核只写数值1

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_WRONLY]
```c
	#define O_WRONLY			00000001
```

固定 `time@a039d9803ade` `mykernel/include/uapi/asm-generic/fcntl.h#L21-L21`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E047 · 内核读写数值2

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_RDWR]
```c
	#define O_RDWR				00000002
```

固定 `time@a039d9803ade` `mykernel/include/uapi/asm-generic/fcntl.h#L22-L22`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E048 · 内核创建位八进制0100

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_CREAT]
```c
	#	define O_CREAT			00000100	/* not fcntl */
```

固定 `time@a039d9803ade` `mykernel/include/uapi/asm-generic/fcntl.h#L24-L24`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E049 · 内核排他位八进制0200

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_EXCL]
```c
	#	define O_EXCL			00000200	/* not fcntl */
```

固定 `time@a039d9803ade` `mykernel/include/uapi/asm-generic/fcntl.h#L27-L27`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E050 · 内核控制终端位八进制0400

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_NOCTTY]
```c
	#	define O_NOCTTY			00000400	/* not fcntl */
```

固定 `time@a039d9803ade` `mykernel/include/uapi/asm-generic/fcntl.h#L30-L30`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E051 · 内核截断位八进制01000

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_TRUNC]
```c
	#	define O_TRUNC			00001000	/* not fcntl */
```

固定 `time@a039d9803ade` `mykernel/include/uapi/asm-generic/fcntl.h#L33-L33`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E052 · 内核追加位八进制02000

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_APPEND]
```c
	#	define O_APPEND			00002000
```

固定 `time@a039d9803ade` `mykernel/include/uapi/asm-generic/fcntl.h#L36-L36`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E053 · 内核非阻塞位八进制04000

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_NONBLOCK]
```c
	#	define O_NONBLOCK		00004000
```

固定 `time@a039d9803ade` `mykernel/include/uapi/asm-generic/fcntl.h#L39-L39`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E054 · 内核大文件位八进制0100000

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_LARGEFILE]
```c
	#	define O_LARGEFILE		00100000
```

固定 `time@a039d9803ade` `mykernel/include/uapi/asm-generic/fcntl.h#L51-L51`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E055 · 内核CLOEXEC位

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_CLOEXEC]
```c
	#	define O_CLOEXEC		02000000	/* set close_on_exec */
```

固定 `time@a039d9803ade` `mykernel/include/uapi/asm-generic/fcntl.h#L63-L63`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E056 · 内核临时文件创建控制位

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::__O_TMPFILE]
```c
	#	define __O_TMPFILE		020000000
```

固定 `time@a039d9803ade` `mykernel/include/uapi/asm-generic/fcntl.h#L89-L89`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E057 · open_how三字段结构声明

[VERIFIED mykernel/include/uapi/linux/openat2.h::open_how_s]
```c
	typedef struct open_how {
		__u64 flags;
		__u64 mode;
		__u64 resolve;
	} open_how_s;
```

固定 `time@a039d9803ade` `mykernel/include/uapi/linux/openat2.h#L19-L23`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E058 · 内核mode有效范围的宏

[VERIFIED mykernel/include/linux/kernel/stat.h::S_IALLUGO]
```c
	#define S_IALLUGO	(S_ISUID|S_ISGID|S_ISVTX|S_IRWXUGO)
```

固定 `time@a039d9803ade` `mykernel/include/linux/kernel/stat.h#L10-L10`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E059 · setuid模式位

[VERIFIED mykernel/include/uapi/linux/stat.h::S_ISUID]
```c
	#	define S_ISUID		0004000		// ---- 1--- ---- ---- b
```

固定 `time@a039d9803ade` `mykernel/include/uapi/linux/stat.h#L16-L16`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E060 · setgid模式位

[VERIFIED mykernel/include/uapi/linux/stat.h::S_ISGID]
```c
	#	define S_ISGID		0002000		// ---- -1-- ---- ---- b
```

固定 `time@a039d9803ade` `mykernel/include/uapi/linux/stat.h#L17-L17`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E061 · sticky模式位

[VERIFIED mykernel/include/uapi/linux/stat.h::S_ISVTX]
```c
	#	define S_ISVTX		0001000		// ---- --1- ---- ---- b
```

固定 `time@a039d9803ade` `mykernel/include/uapi/linux/stat.h#L18-L18`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E062 · owner rwx位

[VERIFIED mykernel/include/uapi/linux/stat.h::S_IRWXU]
```c
	#	define S_IRWXU		00700		// ---- ---1 11-- ---- b
```

固定 `time@a039d9803ade` `mykernel/include/uapi/linux/stat.h#L28-L28`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E063 · group rwx位

[VERIFIED mykernel/include/uapi/linux/stat.h::S_IRWXG]
```c
	#	define S_IRWXG		00070		// ---- ---- --11 1--- b
```

固定 `time@a039d9803ade` `mykernel/include/uapi/linux/stat.h#L33-L33`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E064 · other rwx位

[VERIFIED mykernel/include/uapi/linux/stat.h::S_IRWXO]
```c
	#	define S_IRWXO		00007		// ---- ---- ---- -111 b
```

固定 `time@a039d9803ade` `mykernel/include/uapi/linux/stat.h#L38-L38`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E065 · open syscall三个用户态参数并额外给dfd0

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(open)]
```c
MYOS_SYSCALL_DEFINE3(open, const char *, filename,
		int, flags, umode_t, mode)
{
	return do_sys_open(0, filename, flags, mode);
}
```

固定 `time@a039d9803ade` `mykernel/fs/syscall.c#L38-L42`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E066 · open系统调用号2

[VERIFIED mykernel/arch/x86_64/include/asm/unistd_64.h::__NR_open]
```c
	#define __NR_open			2
```

固定 `time@a039d9803ade` `mykernel/arch/x86_64/include/asm/unistd_64.h#L6-L6`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E067 · 内核活动switch的宏定义

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
```

固定 `time@a039d9803ade` `mykernel/arch/x86_64/entry/syscall_64.c#L26-L26`，原物理行1行；仅支持该符号内局部源码命题。

### ABI122-E068 · switch分派实际包含映射实例

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```c
long x64_sys_call(const pt_regs_s *regs, unsigned int nr)
{
	switch (nr) {
	#include <asm/syscalls_64.h>
	default: return __x64_sys_ni_syscall(regs);
```

固定 `time@a039d9803ade` `mykernel/arch/x86_64/entry/syscall_64.c#L27-L31`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E069 · create方法缺失直接负EPERM早退

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
		dir = path.dentry->d_inode;
		error = -ENOENT;
		if (!dir->i_op->create)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
```

固定 `time@a039d9803ade` `mykernel/fs/vfs/namei.c#L916-L920`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E070 · 有create方法才回调并done_path_create

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
		mode &= (S_IRWXUGO|S_ISVTX);
		error = dir->i_op->create(dir, dentry, mode);
	done_path_create(&path, dentry);
```

固定 `time@a039d9803ade` `mykernel/fs/vfs/namei.c#L920-L922`，原物理行3行；仅支持该符号内局部源码命题。

### ABI122-E071 · mkdir方法缺失直接负EPERM早退

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
		dir = path.dentry->d_inode;
		error = -ENOENT;
		if (!dir->i_op->mkdir)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
```

固定 `time@a039d9803ade` `mykernel/fs/vfs/namei.c#L983-L987`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E072 · 有mkdir方法才回调及done_path_create

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
		mode &= (S_IRWXUGO|S_ISVTX);
		error = dir->i_op->mkdir(dir, dentry, mode);
	done_path_create(&path, dentry);
```

固定 `time@a039d9803ade` `mykernel/fs/vfs/namei.c#L987-L989`，原物理行3行；仅支持该符号内局部源码命题。

### ABI122-E073 · creat普通清理putname及返回

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
out_putname:
	putname(name);
	return error;
```

固定 `time@a039d9803ade` `mykernel/fs/vfs/namei.c#L927-L929`，原物理行3行；仅支持该符号内局部源码命题。

### ABI122-E074 · mkdir普通清理putname及返回

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
out_putname:
	putname(name);
	return error;
```

固定 `time@a039d9803ade` `mykernel/fs/vfs/namei.c#L994-L996`，原物理行3行；仅支持该符号内局部源码命题。

### ABI122-E075 · touch的argc拒绝

[VERIFIED myinitramfs/myshell/myshell.c::touch_command]
```c
	int err = 0, nmode = 0;
	if (argc != 2)
		return -1;
```

固定 `time@a039d9803ade` `myinitramfs/myshell/myshell.c#L327-L329`，原物理行3行；仅支持该符号内局部源码命题。

### ABI122-E076 · touch合法参数creat返回值被忽略并返回0

[VERIFIED myinitramfs/myshell/myshell.c::touch_command]
```c
	nmode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	err = creat(argv[1], nmode);

	return 0;
}
```

固定 `time@a039d9803ade` `myinitramfs/myshell/myshell.c#L331-L335`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E077 · mkdir的argc拒绝

[VERIFIED myinitramfs/myshell/myshell.c::mkdir_command]
```c
	int err = 0, nmode = 0;
	if (argc != 2)
		return -1;
```

固定 `time@a039d9803ade` `myinitramfs/myshell/myshell.c#L339-L341`，原物理行3行；仅支持该符号内局部源码命题。

### ABI122-E078 · mkdir合法参数的局部err最终被忽略

[VERIFIED myinitramfs/myshell/myshell.c::mkdir_command]
```c
	nmode = (S_IRWXU | S_IRWXG | S_IRWXO);
	err = mkdir(argv[1], nmode);

	return 0;
}
```

固定 `time@a039d9803ade` `myinitramfs/myshell/myshell.c#L343-L347`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E079 · rm argc拒绝

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
	int err = 0;
	if (argc != 2)
		return -1;
```

固定 `time@a039d9803ade` `myinitramfs/myshell/myshell.c#L365-L367`，原物理行3行；仅支持该符号内局部源码命题。

### ABI122-E080 · rm自己的unlink与错误printf

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
	char *filename = argv[1];
	err = unlink(filename);
	if (err != 0)
		printf("rm: %s: %s\n", strerror(errno), filename);

```

固定 `time@a039d9803ade` `myinitramfs/myshell/myshell.c#L369-L373`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E081 · rmdir argc拒绝

[VERIFIED myinitramfs/myshell/myshell.c::rmdir_command]
```c
	int err = 0;
	if (argc != 2)
		return -1;
```

固定 `time@a039d9803ade` `myinitramfs/myshell/myshell.c#L351-L353`，原物理行3行；仅支持该符号内局部源码命题。

### ABI122-E082 · rmdir自己的rmdir与错误printf

[VERIFIED myinitramfs/myshell/myshell.c::rmdir_command]
```c
	char *dirname = argv[1];
	err = rmdir(dirname);
	if (err != 0)
		printf("rmdir: %s: %s\n", strerror(errno), dirname);

```

固定 `time@a039d9803ade` `myinitramfs/myshell/myshell.c#L355-L359`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E083 · run_cmd通过函数指针调用但不保存内建返回值

[VERIFIED myinitramfs/myshell/myshell.c::run_cmd]
```c
void run_cmd(int index, int argc, char **argv)
{
	shell_internal_cmd[index].function(argc, argv);
}
```

固定 `time@a039d9803ade` `myinitramfs/myshell/myshell.c#L139-L142`，原物理行4行；仅支持该符号内局部源码命题。

### ABI122-E084 · 活动内核C配置的第一个set命令原文

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

固定 `time@a039d9803ade` `mykernel/scripts/options_flags.cmake#L39-L43`，原物理行5行；仅支持该符号内局部源码命题。

### ABI122-E085 · CONFIG_ARCH_HAS_SYSCALL_WRAPPER位于活动C配置

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```

固定 `time@a039d9803ade` `mykernel/scripts/options_flags.cmake#L64-L66`，原物理行3行；仅支持该符号内局部源码命题。

## 普通位置（非本批P2合格窗口）

| 位置ID | 固定源码 | 定位与限定 |
|---|---|---|
| ABI122-P01 | `myinitramfs/mylib/myglibc/libio/fileops.c:27–37` | 调用方`#include <fcntl.h>`为包含实例，不是宏定义体；头源选择以条件模型列 |
| ABI122-P02 | `myinitramfs/mylib/myglibc/include/fcntl.h:1–4` | 包装包含`<io/fcntl.h>`，独立于实际编译选哪个头 |
| ABI122-P03 | `myinitramfs/mylib/myglibc/io/fcntl.h:24–35,201–209` | 包含features/bits头及`open`的三参声明；声明不当作函数体短窗 |
| ABI122-P04 | `myinitramfs/mylib/myglibc/bits/fcntl.h:38–58` | O_LARGEFILE和O_CLOEXEC受__USE条件宏限制，不能由声明位置证明实际编译可见 |
| ABI122-P05 | `myinitramfs/mylib/myglibc/CMakeLists.txt:1–19` | mylibc静态目标与-D_GNU_SOURCE/include路径，非真实sh链接证据 |
| ABI122-P06 | `myinitramfs/CMakeLists.txt:3,40–51` | 顶层project后才设置仓外musl-gcc路径，且只加入myinit/myshell；链接产物未知 |
| ABI122-P07 | `mykernel/fs/vfs/open.c:8–20` | 内核直接包含linux/kernel/fcntl.h，沿内核而非用户定义解释整数 |
| ABI122-P08 | `mykernel/include/linux/kernel/fcntl.h:5–16` | 内核包含linux/kernel/stat.h及uapi/linux/fcntl.h，宏定义另见P2 |
| ABI122-P09 | `mykernel/include/uapi/linux/fcntl.h:2–7` | 直接包含uapi/asm-generic/fcntl.h与openat2.h，不把include当fcntl宏定义 |
| ABI122-P10 | `myinitramfs/mylib/myglibc/syscall/syscall.c:30` | 用户SYSFUNC_DEF(open)实例；不是实际sh链接的保证 |
| ABI122-P11 | `mykernel/arch/x86_64/include/asm/syscalls_64.h:6` | __SYSCALL(__NR_open,sys_open)映射实例，不是宏定义体 |
| ABI122-P12 | `mykernel/fs/vfs/open.c:23–41, 266–270` | WILL_CREATE宏及build_open_how函数实际体，和用户头/O_LARGEFILE选择分开 |
| ABI122-P13 | `mykernel/include/linux/kernel/fcntl.h:9–16` | VALID_OPEN_FLAGS连续宏体，不能把全体宏调用位置当额外定义 |
| ABI122-P14 | `mykernel/include/uapi/linux/openat2.h:19–23` | 三字段结构及aggregate省略resolve时的C零初始化语义；无运行声明 |
| ABI122-P15 | `myinitramfs/mylib/myglibc/libio/fileops.c:199–267` | r/w/a+与wbx的首/后缀及辅助调用；last_recognized仅记录游标，不生成新open数值 |
| ABI122-P16 | `mykernel/scripts/options_flags.cmake:39–70` | time内核活动C配置，不能替代用户侧myglibc/仓外musl构建条件 |
| ABI122-P17 | `myinitramfs/mylib/myglibc/syscall/syscall.c:71–81` | 共享用户桩的返回地址和errno两句注释，不能证明实机errno/用户返回 |
| ABI122-P18 | `myinitramfs/myshell/myshell.c:44–56` | 全局命令数组，静态引用与真实函数调用分层 |

## 历史材料及条件桥

C08已将R117-02/R120-01判CLOSED_DOCUMENTATION_ONLY，117原件以其88原窗/89字面及C08指定五窗补记合读。121固定69短窗虽逐字相符，R121-01的E023标题误写参数数目；P19缺活动内核C配置正式短窗，故本批在上面新增活动set短窗。三项R120-02、R121-01、R121-02仍只是作者补正待审。已读源码、条件整数模型、真实用户头/内核头、实际链接/运行是不同层次；完整本地P9、构建、链接、QEMU、磁盘操作都NOT_RUN。
