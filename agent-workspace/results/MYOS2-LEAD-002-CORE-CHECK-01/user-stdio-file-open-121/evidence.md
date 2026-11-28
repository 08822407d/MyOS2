---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-STDIO-FILE-OPEN-121
record_type: source_evidence_short_windows_and_ordinary_positions
produced_by: "GPT-6（后端与effort未独立认证）"
model_per_owner: "GPT-6＋Extra（Owner指定）"
date: 2026-11-22
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
inputs_read: ["myinitramfs/mylib/myglibc/libio/fileops.c","myinitramfs/mylib/myglibc/libio/iofopen.c","myinitramfs/mylib/myglibc/libio/libio.h","myinitramfs/myshell/myshell.c","mykernel/fs/vfs/namei.c","myinitramfs/mylib/myglibc/bits/fcntl.h","myinitramfs/mylib/myglibc/bits/types/struct_FILE.h","myinitramfs/mylib/myglibc/stdio/printf.c","myinitramfs/mylib/myglibc/stdio/errlist.c","myinitramfs/mylib/myglibc/syscall/syscall.c","myinitramfs/mylib/myglibc/include/stdio.h","mykernel/arch/x86_64/entry/syscall_64.c"]
open_questions: []
---

# 第121批源码定义体短窗

本节每段来自time短12=`a039d9803ade`，函数/宏/结构体同一真实定义内连续1–5行。仅作为固定源码静态证据；未预处理/编译运行。CMake目标与用户态实际链接分离；普通位置不计入P2。

### FO121-E001｜FILE底层open入口

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_file_open]
```c
FILE *_IO_file_open(FILE *fp, const char *filename,
		int posix_mode, int prot, int read_write)
{
	int fdesc;

```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L173-L177`；原文5行，未运行。

### FO121-E002｜fd负值立即返NULL

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_file_open]
```c
	int fdesc;

	fdesc = open(filename, posix_mode | O_LARGEFILE, prot);
	if (fdesc < 0)
		return NULL;
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L176-L180`；原文5行，未运行。

### FO121-E003｜open结果写入_fileno

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_file_open]
```c
	fdesc = open(filename, posix_mode | O_LARGEFILE, prot);
	if (fdesc < 0)
		return NULL;
	fp->_fileno = fdesc;

```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L178-L182`；原文5行，未运行。

### FO121-E004｜mask flags被注释

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_file_open]
```c
	// _IO_mask_flags(fp, read_write, _IO_NO_READS + _IO_NO_WRITES + _IO_IS_APPENDING);
	// /* For append mode, send the file offset to the end of the file.  Don't
	//    update the offset cache though, since the file handle is not active.  */
	// if ((read_write & (_IO_IS_APPENDING | _IO_NO_READS)) ==
	// 	(_IO_IS_APPENDING | _IO_NO_READS))
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L183-L187`；原文5行，未运行。

### FO121-E005｜seek与close片段注释

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_file_open]
```c
	// {
	// 	off64_t new_pos = _IO_SYSSEEK(fp, 0, _IO_seek_end);
	// 	if (new_pos == _IO_pos_BAD && errno != ESPIPE)
	// 	{
	// 		__close_nocancel(fdesc);
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L188-L192`；原文5行，未运行。

### FO121-E006｜返回同一FILE对象

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_file_open]
```c
	// 		return NULL;
	// 	}
	// }
	return fp;
}
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L193-L197`；原文5行，未运行。

### FO121-E007｜初始化解析状态和0666

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
FILE *_IO_new_file_fopen(FILE *fp, const char *filename, const char *mode)
{
	int oflags = 0, omode;
	int read_write;
	int oprot = 0666;
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L199-L203`；原文5行，未运行。

### FO121-E008｜FILE准入检查位置

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	FILE *result;
	const char *cs;
	const char *last_recognized;

	if (fp->_fileno != -1)
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L205-L209`；原文5行，未运行。

### FO121-E009｜已有fd早返与首字符

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	if (fp->_fileno != -1)
		return 0;
	switch (*mode)
	{
	case 'r':
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L209-L213`；原文5行，未运行。

### FO121-E010｜r只读

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	case 'r':
		omode = O_RDONLY;
		read_write = _IO_NO_WRITES;
		break;
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L213-L216`；原文4行，未运行。

### FO121-E011｜w创建截断

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	case 'w':
		omode = O_WRONLY;
		oflags = O_CREAT | O_TRUNC;
		read_write = _IO_NO_READS;
		break;
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L217-L221`；原文5行，未运行。

### FO121-E012｜a追加模式

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	case 'a':
		omode = O_WRONLY;
		oflags = O_CREAT | O_APPEND;
		read_write = _IO_NO_READS | _IO_IS_APPENDING;
		break;
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L222-L226`；原文5行，未运行。

### FO121-E013｜非法首字符返NULL

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	default:
		// __set_errno(EINVAL);
		return NULL;
	}
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L227-L230`；原文4行，未运行。

### FO121-E014｜六次后缀循环和指针增加

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	last_recognized = mode;
	for (i = 1; i < 7; ++i)
	{
		switch (*++mode)
		{
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L231-L235`；原文5行，未运行。

### FO121-E015｜NUL退出

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
		case '\0':
			break;
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L236-L237`；原文2行，未运行。

### FO121-E016｜加号读写并取APPENDING位

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
		case '+':
			omode = O_RDWR;
			read_write &= _IO_IS_APPENDING;
			last_recognized = mode;
			continue;
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L238-L242`；原文5行，未运行。

### FO121-E017｜x排他

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
		case 'x':
			oflags |= O_EXCL;
			last_recognized = mode;
			continue;
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L243-L246`；原文4行，未运行。

### FO121-E018｜b只记游标

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
		case 'b':
			last_recognized = mode;
			continue;
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L247-L249`；原文3行，未运行。

### FO121-E019｜m标志2映射

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
		case 'm':
			fp->_flags2 |= _IO_FLAGS2_MMAP;
			continue;
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L250-L252`；原文3行，未运行。

### FO121-E020｜c标志2取消控制

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
		case 'c':
			fp->_flags2 |= _IO_FLAGS2_NOTCANCEL;
			continue;
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L253-L255`；原文3行，未运行。

### FO121-E021｜e同时加O_CLOEXEC和标志2

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
		case 'e':
			oflags |= O_CLOEXEC;
			fp->_flags2 |= _IO_FLAGS2_CLOEXEC;
			continue;
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L256-L259`；原文4行，未运行。

### FO121-E022｜未知后缀忽略

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
		default:
			/* Ignore.  */
			continue;
		}
		break;
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L260-L264`；原文5行，未运行。

### FO121-E023｜下层调用与四参数

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
		break;
	}

	result = _IO_file_open(fp, filename, omode | oflags, oprot, read_write);
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L264-L267`；原文4行，未运行。

### FO121-E024｜仅返回下层result

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	return result;
}
```

固定time `myinitramfs/mylib/myglibc/libio/fileops.c#L339-L340`；原文2行，未运行。

### FO121-E025｜malloc后只设置_fileno并传下层

[VERIFIED myinitramfs/mylib/myglibc/libio/iofopen.c::_IO_new_fopen]
```c
_IO_new_fopen(const char *filename, const char *mode)
{
	FILE *new_f = malloc(sizeof(FILE));
	new_f->_fileno = -1;
	return _IO_new_file_fopen(new_f, filename, mode);
```

固定time `myinitramfs/mylib/myglibc/libio/iofopen.c#L83-L87`；原文5行，未运行。

### FO121-E026｜FILE禁读位

[VERIFIED myinitramfs/mylib/myglibc/libio/libio.h::_IO_NO_READS]
```c
	#define _IO_NO_READS			0x0004		/* Reading not allowed.  */
```

固定time `myinitramfs/mylib/myglibc/libio/libio.h#L71-L71`；原文1行，未运行。

### FO121-E027｜FILE禁写位

[VERIFIED myinitramfs/mylib/myglibc/libio/libio.h::_IO_NO_WRITES]
```c
	#define _IO_NO_WRITES			0x0008		/* Writing not allowed.  */
```

固定time `myinitramfs/mylib/myglibc/libio/libio.h#L72-L72`；原文1行，未运行。

### FO121-E028｜FILE内部追加标志

[VERIFIED myinitramfs/mylib/myglibc/libio/libio.h::_IO_IS_APPENDING]
```c
	#define _IO_IS_APPENDING		0x1000
```

固定time `myinitramfs/mylib/myglibc/libio/libio.h#L81-L81`；原文1行，未运行。

### FO121-E029｜flags2映射位

[VERIFIED myinitramfs/mylib/myglibc/libio/libio.h::_IO_FLAGS2_MMAP]
```c
	#define _IO_FLAGS2_MMAP			1
```

固定time `myinitramfs/mylib/myglibc/libio/libio.h#L87-L87`；原文1行，未运行。

### FO121-E030｜flags2不取消位

[VERIFIED myinitramfs/mylib/myglibc/libio/libio.h::_IO_FLAGS2_NOTCANCEL]
```c
	#define _IO_FLAGS2_NOTCANCEL	2
```

固定time `myinitramfs/mylib/myglibc/libio/libio.h#L88-L88`；原文1行，未运行。

### FO121-E031｜flags2 CLOSE_ON_EXEC位

[VERIFIED myinitramfs/mylib/myglibc/libio/libio.h::_IO_FLAGS2_CLOEXEC]
```c
	#define _IO_FLAGS2_CLOEXEC		64
```

固定time `myinitramfs/mylib/myglibc/libio/libio.h#L91-L91`；原文1行，未运行。

### FO121-E032｜FILE结构体描述符和标志2字段

[VERIFIED myinitramfs/mylib/myglibc/bits/types/struct_FILE.h::_IO_FILE]
```c
		int _fileno;
		int _flags2;
		__off_t _old_offset; /* This used to be _offset but it's too small.  */
```

固定time `myinitramfs/mylib/myglibc/bits/types/struct_FILE.h#L73-L75`；原文3行，未运行。

### FO121-E033｜仓内头fopen宏替换

[VERIFIED myinitramfs/mylib/myglibc/include/stdio.h::fopen]
```c
	#define fopen(fname, mode) _IO_new_fopen(fname, mode)
```

固定time `myinitramfs/mylib/myglibc/include/stdio.h#L193-L193`；原文1行，未运行。

### FO121-E034｜用户O_RDONLY

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_RDONLY]
```c
#define	O_RDONLY	0	/* Open read-only.  */
```

固定time `myinitramfs/mylib/myglibc/bits/fcntl.h#L25-L25`；原文1行，未运行。

### FO121-E035｜用户O_WRONLY

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_WRONLY]
```c
#define	O_WRONLY	1	/* Open write-only.  */
```

固定time `myinitramfs/mylib/myglibc/bits/fcntl.h#L26-L26`；原文1行，未运行。

### FO121-E036｜用户O_RDWR

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_RDWR]
```c
#define	O_RDWR		2	/* Open read/write.  */
```

固定time `myinitramfs/mylib/myglibc/bits/fcntl.h#L27-L27`；原文1行，未运行。

### FO121-E037｜用户O_CREAT

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_CREAT]
```c
#define	O_CREAT		0x0200	/* Create file if it doesn't exist.  */
```

固定time `myinitramfs/mylib/myglibc/bits/fcntl.h#L31-L31`；原文1行，未运行。

### FO121-E038｜用户O_EXCL

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_EXCL]
```c
#define	O_EXCL		0x0800	/* Fail if file already exists.  */
```

固定time `myinitramfs/mylib/myglibc/bits/fcntl.h#L32-L32`；原文1行，未运行。

### FO121-E039｜用户O_TRUNC

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_TRUNC]
```c
#define	O_TRUNC		0x0400	/* Truncate file to zero length.  */
```

固定time `myinitramfs/mylib/myglibc/bits/fcntl.h#L33-L33`；原文1行，未运行。

### FO121-E040｜条件O_CLOEXEC

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_CLOEXEC]
```c
	# define O_CLOEXEC		0x00400000      /* Set close_on_exec.  */
```

固定time `myinitramfs/mylib/myglibc/bits/fcntl.h#L45-L45`；原文1行，未运行。

### FO121-E041｜条件O_LARGEFILE为零

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_LARGEFILE]
```c
	# define O_LARGEFILE	0
```

固定time `myinitramfs/mylib/myglibc/bits/fcntl.h#L54-L54`；原文1行，未运行。

### FO121-E042｜用户O_APPEND

[VERIFIED myinitramfs/mylib/myglibc/bits/fcntl.h::O_APPEND]
```c
#define	O_APPEND	0x0008	/* Writes append to the file.  */
```

固定time `myinitramfs/mylib/myglibc/bits/fcntl.h#L58-L58`；原文1行，未运行。

### FO121-E043｜main先LF再解析

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
		printf("\n");

		int __argc = 0;
		char ** __argv = NULL;
		index = parse_command(buf, &__argc, &__argv);
```

固定time `myinitramfs/myshell/myshell.c#L94-L98`；原文5行，未运行。

### FO121-E044｜main选择run_cmd或错误printf

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
		index = parse_command(buf, &__argc, &__argv);
		if(index < 0)
			printf("Input Error, No Command Found!\n");
		else
			run_cmd(index, __argc, __argv);	//argc,argv
```

固定time `myinitramfs/myshell/myshell.c#L98-L102`；原文5行，未运行。

### FO121-E045｜执行分发而不消费返回值

[VERIFIED myinitramfs/myshell/myshell.c::run_cmd]
```c
void run_cmd(int index, int argc, char **argv)
{
	shell_internal_cmd[index].function(argc, argv);
}
```

固定time `myinitramfs/myshell/myshell.c#L139-L142`；原文4行，未运行。

### FO121-E046｜touch参数门

[VERIFIED myinitramfs/myshell/myshell.c::touch_command]
```c
int touch_command(int argc, char **argv)
{
	int err = 0, nmode = 0;
	if (argc != 2)
		return -1;
```

固定time `myinitramfs/myshell/myshell.c#L325-L329`；原文5行，未运行。

### FO121-E047｜touch返回0无条件

[VERIFIED myinitramfs/myshell/myshell.c::touch_command]
```c
	nmode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	err = creat(argv[1], nmode);

	return 0;
```

固定time `myinitramfs/myshell/myshell.c#L331-L334`；原文4行，未运行。

### FO121-E048｜mkdir参数门

[VERIFIED myinitramfs/myshell/myshell.c::mkdir_command]
```c
int mkdir_command(int argc, char **argv)
{
	int err = 0, nmode = 0;
	if (argc != 2)
		return -1;
```

固定time `myinitramfs/myshell/myshell.c#L337-L341`；原文5行，未运行。

### FO121-E049｜mkdir返回0无条件

[VERIFIED myinitramfs/myshell/myshell.c::mkdir_command]
```c
	nmode = (S_IRWXU | S_IRWXG | S_IRWXO);
	err = mkdir(argv[1], nmode);

	return 0;
```

固定time `myinitramfs/myshell/myshell.c#L343-L346`；原文4行，未运行。

### FO121-E050｜rm参数门

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
int rm_command(int argc, char **argv)
{
	int err = 0;
	if (argc != 2)
		return -1;
```

固定time `myinitramfs/myshell/myshell.c#L363-L367`；原文5行，未运行。

### FO121-E051｜rm错误格式

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
	char *filename = argv[1];
	err = unlink(filename);
	if (err != 0)
		printf("rm: %s: %s\n", strerror(errno), filename);

```

固定time `myinitramfs/myshell/myshell.c#L369-L373`；原文5行，未运行。

### FO121-E052｜rmdir参数门

[VERIFIED myinitramfs/myshell/myshell.c::rmdir_command]
```c
int rmdir_command(int argc, char **argv)
{
	int err = 0;
	if (argc != 2)
		return -1;
```

固定time `myinitramfs/myshell/myshell.c#L349-L353`；原文5行，未运行。

### FO121-E053｜rmdir错误格式

[VERIFIED myinitramfs/myshell/myshell.c::rmdir_command]
```c
	char *dirname = argv[1];
	err = rmdir(dirname);
	if (err != 0)
		printf("rmdir: %s: %s\n", strerror(errno), dirname);
```

固定time `myinitramfs/myshell/myshell.c#L355-L358`；原文4行，未运行。

### FO121-E054｜creat缺少方法立即-EPERM

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
		dir = path.dentry->d_inode;
		error = -ENOENT;
		if (!dir->i_op->create)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
```

固定time `mykernel/fs/vfs/namei.c#L916-L920`；原文5行，未运行。

### FO121-E055｜creat正常回调及done

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
		mode &= (S_IRWXUGO|S_ISVTX);
		error = dir->i_op->create(dir, dentry, mode);
	done_path_create(&path, dentry);
```

固定time `mykernel/fs/vfs/namei.c#L920-L922`；原文3行，未运行。

### FO121-E056｜creat公共putname和error返回

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
out_putname:
	putname(name);
	return error;
```

固定time `mykernel/fs/vfs/namei.c#L927-L929`；原文3行，未运行。

### FO121-E057｜mkdir缺少方法立即-EPERM

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
		dir = path.dentry->d_inode;
		error = -ENOENT;
		if (!dir->i_op->mkdir)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
```

固定time `mykernel/fs/vfs/namei.c#L983-L987`；原文5行，未运行。

### FO121-E058｜mkdir回调及done

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
		if (!dir->i_op->mkdir)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
		error = dir->i_op->mkdir(dir, dentry, mode);
	done_path_create(&path, dentry);
```

固定time `mykernel/fs/vfs/namei.c#L985-L989`；原文5行，未运行。

### FO121-E059｜mkdir公共putname和error返回

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
out_putname:
	putname(name);
	return error;
```

固定time `mykernel/fs/vfs/namei.c#L994-L996`；原文3行，未运行。

### FO121-E060｜printf格式化

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	va_start(ap, fmt);
	rv = vsnprintf(buf, SZ_4K, fmt, ap);
	va_end(ap);
```

固定time `myinitramfs/mylib/myglibc/stdio/printf.c#L17-L19`；原文3行，未运行。

### FO121-E061｜printf请求write

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	// buffered IO has unsolved bug, so derectly use the write() function
	write(stdout->_fileno, buf, rv);
	// rv = (fputs(buf, stdout) < 0) ? 0 : rv;
	// fflush(stdout);
	return rv;
```

固定time `myinitramfs/mylib/myglibc/stdio/printf.c#L21-L25`；原文5行，未运行。

### FO121-E062｜errlist取负号及表索引

[VERIFIED myinitramfs/mylib/myglibc/stdio/errlist.c::__get_errlist]
```c
{
   // if (mapped >= 0 && mapped < array_length(_sys_errlist_internal))
   if (errnum < 0)
      errnum = -errnum;
   return _sys_errlist_internal[errnum];
```

固定time `myinitramfs/mylib/myglibc/stdio/errlist.c#L25-L29`；原文5行，未运行。

### FO121-E063｜用户生成桩宏

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::__SYSFUNC_DEF__]
```c
#define __SYSFUNC_DEF__(name, nr)							\
			__asm__	(										\
				".global	"#name"					\n\t"	\
				".type		"#name",	@function	\n\t"	\
				#name":								\n\t"	\
```

固定time `myinitramfs/mylib/myglibc/syscall/syscall.c#L19-L23`；原文5行，未运行。

### FO121-E064｜用户桩装号和跳转

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::__SYSFUNC_DEF__]
```c
				"pushq	%rbp						\n\t"	\
				"movq	%rsp,	%rbp				\n\t"	\
				"movq	$"#nr",	%rax				\n\t"	\
				"jmp	LABEL_SYSCALL				\n\t"	\
```

固定time `myinitramfs/mylib/myglibc/syscall/syscall.c#L24-L27`；原文4行，未运行。

### FO121-E065｜汇编共同入口单标签

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::LABEL_SYSCALL]
```c
	"LABEL_SYSCALL:							\n\t"
	"leaq	sysexit_ret_addr(%rip),	%rcx	\n\t"
	"syscall								\n\t"
```

固定time `myinitramfs/mylib/myglibc/syscall/syscall.c#L71-L73`；原文3行，未运行。

### FO121-E066｜汇编返回地址与比较

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::sysexit_ret_addr]
```c
	"sysexit_ret_addr:						\n\t"
	"cmpq	$-0x1000,	%rax				\n\t"
	"jb	LABEL_SYSCALL_RET					\n\t"
```

固定time `myinitramfs/mylib/myglibc/syscall/syscall.c#L74-L76`；原文3行，未运行。

### FO121-E067｜汇编返回标签

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::LABEL_SYSCALL_RET]
```c
	"LABEL_SYSCALL_RET:						\n\t"
	"leaveq									\n\t"
	"retq									\n\t"
```

固定time `myinitramfs/mylib/myglibc/syscall/syscall.c#L79-L81`；原文3行，未运行。

### FO121-E068｜内核分派宏定义

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
```

固定time `mykernel/arch/x86_64/entry/syscall_64.c#L26-L26`；原文1行，未运行。

### FO121-E069｜内核活动分派函数

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```c
long x64_sys_call(const pt_regs_s *regs, unsigned int nr)
{
	switch (nr) {
	#include <asm/syscalls_64.h>
	default: return __x64_sys_ni_syscall(regs);
```

固定time `mykernel/arch/x86_64/entry/syscall_64.c#L27-L31`；原文5行，未运行。

## 普通位置（非定义体P2）

| ID | 固定time路径与范围 | 用途及不能推断的事 |
|---|---|---|
| FO121-P01 | `myinitramfs/CMakeLists.txt:3,31–33,40–51` | project在工具链赋值前；配置写MUSL_PATH与两子目录，不认证编译器实际采用或最终链接 |
| FO121-P02 | `myinitramfs/myshell/CMakeLists.txt:1–10` | 用GLOB组建sh，但不见此文件调用target_link_libraries(sh mylibc) |
| FO121-P03 | `myinitramfs/mylib/CMakeLists.txt:1–3` | 单独的mylib子树接myglibc/csu/util，在已读顶层没有递归调用 |
| FO121-P04 | `myinitramfs/mylib/myglibc/CMakeLists.txt:1–29` | 声明mylibc STATIC及add_subdirectory(libio)而非实际目标已链接 |
| FO121-P05 | `myinitramfs/mylib/myglibc/libio/CMakeLists.txt:1–3` | 源码GLOB与target_sources(mylibc)文本，不代表sh已纳入此库 |
| FO121-P06 | `myinitramfs/mylib/myglibc/libio/stdio.h:243–246` | `fopen`普通外部声明，没有实现来源证明 |
| FO121-P07 | `myinitramfs/mylib/myglibc/include/stdio.h:191–194` | `fopen`调用式宏及`_IO_new_fopen`声明；宏定义本身另P2 |
| FO121-P08 | `myinitramfs/mylib/myglibc/libio/iofopen.c:90–97` | strong_alias、versioned_symbol、weak_alias都是注释，不可声称导出相同别名 |
| FO121-P09 | `myinitramfs/mylib/myglibc/libio/libioP.h:523–525` | `_IO_new_file_fopen`的extern是声明而不是定义 |
| FO121-P10 | `myinitramfs/mylib/myglibc/bits/types/FILE.h:5–8` | typedef(FILE)与结构前向声明，成员定义由struct_FILE.h证明 |
| FO121-P11 | `myinitramfs/mylib/myglibc/libio/fileops.c:173–197,199–340` | 两个完整函数，函数体中看不到活动seek/close、free及mask_flags；不能外推全库 |
| FO121-P12 | `myinitramfs/mylib/myglibc/bits/fcntl.h:42–58` | O_CLOEXEC受__USE_XOPEN2K8条件；O_LARGEFILE受__USE_LARGEFILE64条件，未实际预处理 |
| FO121-P13 | `mykernel/arch/x86_64/include/asm/unistd_64.h:103,104,106,108` | mkdir=83、rmdir=84、creat=85、unlink=87，修旧P03误用编号当行号 |
| FO121-P14 | `mykernel/arch/x86_64/include/asm/syscalls_64.h:44–48` | 内核__SYSCALL的调用实例，和编号宏/宏定义是不同证据 |
| FO121-P15 | `myinitramfs/mylib/myglibc/syscall/syscall.c:53–57` | myglibc用户SYSFUNC_DEF实例，仅仓内若选中该实现才参与用户代码 |
| FO121-P16 | `mykernel/fs/vfs/namei.c:770–823` | filename_create完整体修复旧P10混入825的另一函数；任何负值还要有效name前提 |
| FO121-P17 | `myinitramfs/mylib/myglibc/syscall/syscall.c:71–81` | 共同入口71–73、返回地址74–76、最终返回79–81分别真实短窗；77–78的errno赋值/−1归一化是注释 |
| FO121-P18 | `myinitramfs/mylib/myglibc/stdio/errlist.c:23–31` | `__get_errlist`实际会负号转正再以索引取表，但不能证明shell的errno来源 |
| FO121-P19 | `mykernel/scripts/options_flags.cmake:39–70` | 仅time源配置，实际使用哪个initramfs工具链和库未证 |
| FO121-P20 | `myinitramfs/myshell/myshell.c:44–56` | 全局内建命令表是位置证据，不属于定义体P2 |

## 历史计数与唯一纠正

原117 88个窗/89次字面、118 91/92且两处文件域对象降级为89候选、119 46/46且三处全局表降级为43候选、120 81/81且原E072跨LABEL_SYSCALL和sysexit_ret_addr后只有80个单符号合格窗。121新证据分别锚共同入口、返回地址及第三个返回标签；旧件和旧评论保持冻结。R117-02仍由C08裁定；R118-02与R119-01已文档闭合，不重开。真实FD、终端及完整本地P9/构建/链接均NOT_RUN。
