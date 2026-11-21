---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LIBC-STRING-COMPARE-123
record_type: static_source_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-18
produced_by: "GPT-6（会话自报，后端不认证）"
model_per_owner: "GPT-6＋Extra（Owner指定）"
date: 2026-11-22
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
open_questions: []
inputs_read: ["myinitramfs/mylib/myglibc/string/strcmp.c","myinitramfs/mylib/myglibc/string/strncmp.c","myinitramfs/myshell/myshell.c","mykernel/fs/vfs/open.c","mykernel/fs/vfs/namei.c","myinitramfs/mylib/myglibc/libio/fileops.c","myinitramfs/mylib/myglibc/libio/iofopen.c","mykernel/scripts/options_flags.cmake"]
---

# 第123批定义体短窗与普通来源分账

所有新P2引文均为固定time`a039d9803ade`的单个真实函数/宏定义或活动CMake命令中连续1–5物理行。指针可读、字符串NUL终止、已选择本仓libc实现和实际命令运行，是独立的前提。声明、include、静态命令表与构建目标另列普通位置，不能以字节匹配替代证据资格。历史122原85窗不纳入本批计数。

### SC123-E001｜default STRCMP macro alias

[VERIFIED myinitramfs/mylib/myglibc/string/strcmp.c::STRCMP macro]
```c
	#define STRCMP strcmp
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strcmp.c#L23-L23`；连续1行，取自该符号定义体。

### SC123-E002｜char arguments converted to unsigned-byte pointers

[VERIFIED myinitramfs/mylib/myglibc/string/strcmp.c::STRCMP function]
```c
int STRCMP(const char *p1, const char *p2)
{
	const unsigned char *s1 = (const unsigned char *)p1;
	const unsigned char *s2 = (const unsigned char *)p2;
	unsigned char c1, c2;
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strcmp.c#L29-L33`；连续5行，取自该符号定义体。

### SC123-E003｜two unsigned-byte loads, NUL test

[VERIFIED myinitramfs/mylib/myglibc/string/strcmp.c::STRCMP function]
```c
	do
	{
		c1 = (unsigned char)*s1++;
		c2 = (unsigned char)*s2++;
		if (c1 == '\0')
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strcmp.c#L35-L39`；连续5行，取自该符号定义体。

### SC123-E004｜increment pointers and loop while equal

[VERIFIED myinitramfs/mylib/myglibc/string/strcmp.c::STRCMP function]
```c
		c1 = (unsigned char)*s1++;
		c2 = (unsigned char)*s2++;
		if (c1 == '\0')
			return c1 - c2;
	} while (c1 == c2);
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strcmp.c#L37-L41`；连续5行，取自该符号定义体。

### SC123-E005｜NUL exit versus unequal exit

[VERIFIED myinitramfs/mylib/myglibc/string/strcmp.c::STRCMP function]
```c
		if (c1 == '\0')
			return c1 - c2;
	} while (c1 == c2);

	return c1 - c2;
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strcmp.c#L39-L43`；连续5行，取自该符号定义体。

### SC123-E006｜return byte difference

[VERIFIED myinitramfs/mylib/myglibc/string/strcmp.c::STRCMP function]
```c
	return c1 - c2;
}
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strcmp.c#L43-L44`；连续2行，取自该符号定义体。

### SC123-E007｜default STRNCMP macro alias

[VERIFIED myinitramfs/mylib/myglibc/string/strncmp.c::STRNCMP macro]
```c
	#define STRNCMP strncmp
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strncmp.c#L24-L24`；连续1行，取自该符号定义体。

### SC123-E008｜entry initializes byte variables to NUL

[VERIFIED myinitramfs/mylib/myglibc/string/strncmp.c::STRNCMP function]
```c
int STRNCMP(const char *s1, const char *s2, size_t n)
{
	unsigned char c1 = '\0';
	unsigned char c2 = '\0';

```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strncmp.c#L31-L35`；连续5行，取自该符号定义体。

### SC123-E009｜four-byte group count n4=n>>2

[VERIFIED myinitramfs/mylib/myglibc/string/strncmp.c::STRNCMP function]
```c
	if (n >= 4)
	{
		size_t n4 = n >> 2;
		do
		{
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strncmp.c#L36-L40`；连续5行，取自该符号定义体。

### SC123-E010｜group byte pair1 can return

[VERIFIED myinitramfs/mylib/myglibc/string/strncmp.c::STRNCMP function]
```c
			c1 = (unsigned char)*s1++;
			c2 = (unsigned char)*s2++;
			if (c1 == '\0' || c1 != c2)
				return c1 - c2;
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strncmp.c#L41-L44`；连续4行，取自该符号定义体。

### SC123-E011｜group byte pair2 can return

[VERIFIED myinitramfs/mylib/myglibc/string/strncmp.c::STRNCMP function]
```c
			c1 = (unsigned char)*s1++;
			c2 = (unsigned char)*s2++;
			if (c1 == '\0' || c1 != c2)
				return c1 - c2;
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strncmp.c#L45-L48`；连续4行，取自该符号定义体。

### SC123-E012｜group byte pair3 can return

[VERIFIED myinitramfs/mylib/myglibc/string/strncmp.c::STRNCMP function]
```c
			c1 = (unsigned char)*s1++;
			c2 = (unsigned char)*s2++;
			if (c1 == '\0' || c1 != c2)
				return c1 - c2;
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strncmp.c#L49-L52`；连续4行，取自该符号定义体。

### SC123-E013｜group byte pair4 can return

[VERIFIED myinitramfs/mylib/myglibc/string/strncmp.c::STRNCMP function]
```c
			c1 = (unsigned char)*s1++;
			c2 = (unsigned char)*s2++;
			if (c1 == '\0' || c1 != c2)
				return c1 - c2;
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strncmp.c#L53-L56`；连续4行，取自该符号定义体。

### SC123-E014｜group decrement and residual n&3

[VERIFIED myinitramfs/mylib/myglibc/string/strncmp.c::STRNCMP function]
```c
		} while (--n4 > 0);
		n &= 3;
	}
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strncmp.c#L57-L59`；连续3行，取自该符号定义体。

### SC123-E015｜remaining budget reads only when n>0

[VERIFIED myinitramfs/mylib/myglibc/string/strncmp.c::STRNCMP function]
```c
	while (n > 0)
	{
		c1 = (unsigned char)*s1++;
		c2 = (unsigned char)*s2++;
		if (c1 == '\0' || c1 != c2)
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strncmp.c#L61-L65`；连续5行，取自该符号定义体。

### SC123-E016｜remainder comparison and n decrement

[VERIFIED myinitramfs/mylib/myglibc/string/strncmp.c::STRNCMP function]
```c
		c1 = (unsigned char)*s1++;
		c2 = (unsigned char)*s2++;
		if (c1 == '\0' || c1 != c2)
			return c1 - c2;
		n--;
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strncmp.c#L63-L67`；连续5行，取自该符号定义体。

### SC123-E017｜after budget exhausted return c1-c2

[VERIFIED myinitramfs/mylib/myglibc/string/strncmp.c::STRNCMP function]
```c
	}

	return c1 - c2;
}
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/string/strncmp.c#L68-L71`；连续4行，取自该符号定义体。

### SC123-E018｜function-pointer dispatch does not store result

[VERIFIED myinitramfs/myshell/myshell.c::run_cmd]
```c
void run_cmd(int index, int argc, char **argv)
{
	shell_internal_cmd[index].function(argc, argv);
}
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L139-L142`；连续4行，取自该符号定义体。

### SC123-E019｜find_cmd loop initialization

[VERIFIED myinitramfs/myshell/myshell.c::find_cmd]
```c
int find_cmd(char *cmd)
{
	// printf("parsing command: %s ... ", cmd);
	int i = 0;
	for(i = 0;i<sizeof(shell_internal_cmd)/sizeof(builtincmd_s);i++)
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L144-L148`；连续5行，取自该符号定义体。

### SC123-E020｜full strcmp zero selects index, otherwise -1

[VERIFIED myinitramfs/myshell/myshell.c::find_cmd]
```c
	for(i = 0;i<sizeof(shell_internal_cmd)/sizeof(builtincmd_s);i++)
		if(!strcmp(cmd,shell_internal_cmd[i].name))
			return i;
	return -1;
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L148-L151`；连续4行，取自该符号定义体。

### SC123-E021｜parse returns result of find_cmd

[VERIFIED myinitramfs/myshell/myshell.c::parse_command]
```c
	}

	return find_cmd(**argv);
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L183-L185`；连续3行，取自该符号定义体。

### SC123-E022｜cd argument variables

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
int cd_command(int argc, char **argv)
{
	char *path = NULL;
	int len = 0;
	int i = 0;
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L188-L192`；连续5行，取自该符号定义体。

### SC123-E023｜argc path selection

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	if (argc <= 1)
		path = homedir;
	else
		path = argv[1];

```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L194-L198`；连续5行，取自该符号定义体。

### SC123-E024｜literal dot before path and short-circuit chdir

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	/////.
	if(!strcmp(".", path))
		return 1;

	i = chdir(path);
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L199-L203`；连续5行，取自该符号定义体。

### SC123-E025｜chdir request and conditional cache update

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	i = chdir(path);
	if(!i)
	{
		memset(current_dir, 0, PATH_MAX);
		getcwd(current_dir, PATH_MAX);
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L203-L207`；连续5行，取自该符号定义体。

### SC123-E026｜cd error line and final return

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	}
	else
		printf("cd: %s: %s\n", strerror(errno), path);
	return 1;
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L208-L211`；连续4行，取自该符号定义体。

### SC123-E027｜ls directory variables

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
int ls_command(int argc, char **argv)
{
	DIR* dir = NULL;
	struct dirent* buf = NULL;
	char *path = NULL;
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L214-L218`；连续5行，取自该符号定义体。

### SC123-E028｜ls path selection before opendir

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
	if (argc <=1)
		path = ".";
	else
		path = argv[1];

```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L219-L223`；连续5行，取自该符号定义体。

### SC123-E029｜ls open-error early return

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
	dir = opendir(path);
	if (dir == NULL)
	{
		printf("ls: %s: %s\n", strerror(errno), path);
		return -1;
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L224-L228`；连续5行，取自该符号定义体。

### SC123-E030｜ls readdir loop and termination

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
	while(1)
	{
		buf = readdir(dir);
		if(buf == NULL)
			break;
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L231-L235`；连续5行，取自该符号定义体。

### SC123-E031｜ls chained strcmp and per-name printf

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
		if (strcmp(buf->d_name, ".") != 0 &&
			strcmp(buf->d_name, "..") != 0)
			printf("%s\t", buf->d_name);
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L236-L238`；连续3行，取自该符号定义体。

### SC123-E032｜second strcmp if reached and independent LF

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
			strcmp(buf->d_name, "..") != 0)
			printf("%s\t", buf->d_name);
	}
	printf("\n");
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L237-L240`；连续4行，取自该符号定义体。

### SC123-E033｜final LF and closedir

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
	}
	printf("\n");
	closedir(dir);
	return 1;
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L239-L242`；连续4行，取自该符号定义体。

### SC123-E034｜do_sys_open owns both direct calls

[VERIFIED mykernel/fs/vfs/open.c::do_sys_open]
```c
long do_sys_open(int dfd, const char * filename, int flags, umode_t mode)
{
	open_how_s how = build_open_how(flags, mode);
	return do_sys_openat2(dfd, filename, &how);
}
```

`time@a039d9803ade` `mykernel/fs/vfs/open.c#L266-L270`；连续5行，取自该符号定义体。

### SC123-E035｜build_open_how computes how not next caller

[VERIFIED mykernel/fs/vfs/open.c::build_open_how]
```c
open_how_s build_open_how(int flags, umode_t mode)
{
	struct open_how how = {
		.flags = flags & VALID_OPEN_FLAGS,
		.mode = mode & S_IALLUGO,
```

`time@a039d9803ade` `mykernel/fs/vfs/open.c#L27-L31`；连续5行，取自该符号定义体。

### SC123-E036｜build_open_how returns how

[VERIFIED mykernel/fs/vfs/open.c::build_open_how]
```c
	/* Modes should only be set for create-like flags. */
	if (!WILL_CREATE(how.flags))
		how.mode = 0;
	return how;
}
```

`time@a039d9803ade` `mykernel/fs/vfs/open.c#L37-L41`；连续5行，取自该符号定义体。

### SC123-E037｜missing create method returns early

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
		dir = path.dentry->d_inode;
		error = -ENOENT;
		if (!dir->i_op->create)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
```

`time@a039d9803ade` `mykernel/fs/vfs/namei.c#L916-L920`；连续5行，取自该符号定义体。

### SC123-E038｜do_creatat owns callback and done

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
		mode &= (S_IRWXUGO|S_ISVTX);
		error = dir->i_op->create(dir, dentry, mode);
	done_path_create(&path, dentry);
```

`time@a039d9803ade` `mykernel/fs/vfs/namei.c#L920-L922`；连续3行，取自该符号定义体。

### SC123-E039｜do_creatat owns putname

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
out_putname:
	putname(name);
	return error;
```

`time@a039d9803ade` `mykernel/fs/vfs/namei.c#L927-L929`；连续3行，取自该符号定义体。

### SC123-E040｜missing mkdir method returns early

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
		dir = path.dentry->d_inode;
		error = -ENOENT;
		if (!dir->i_op->mkdir)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
```

`time@a039d9803ade` `mykernel/fs/vfs/namei.c#L983-L987`；连续5行，取自该符号定义体。

### SC123-E041｜do_mkdirat owns callback and done

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
		mode &= (S_IRWXUGO|S_ISVTX);
		error = dir->i_op->mkdir(dir, dentry, mode);
	done_path_create(&path, dentry);
```

`time@a039d9803ade` `mykernel/fs/vfs/namei.c#L987-L989`；连续3行，取自该符号定义体。

### SC123-E042｜do_mkdirat owns putname

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
out_putname:
	putname(name);
	return error;
```

`time@a039d9803ade` `mykernel/fs/vfs/namei.c#L994-L996`；连续3行，取自该符号定义体。

### SC123-E043｜rm argc gate

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
int rm_command(int argc, char **argv)
{
	int err = 0;
	if (argc != 2)
		return -1;
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L363-L367`；连续5行，取自该符号定义体。

### SC123-E044｜rm owns unlink,strerror,printf

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
	char *filename = argv[1];
	err = unlink(filename);
	if (err != 0)
		printf("rm: %s: %s\n", strerror(errno), filename);

```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L369-L373`；连续5行，取自该符号定义体。

### SC123-E045｜rm own error branch and return

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
	if (err != 0)
		printf("rm: %s: %s\n", strerror(errno), filename);

	return err;
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L371-L374`；连续4行，取自该符号定义体。

### SC123-E046｜rmdir argc gate

[VERIFIED myinitramfs/myshell/myshell.c::rmdir_command]
```c
int rmdir_command(int argc, char **argv)
{
	int err = 0;
	if (argc != 2)
		return -1;
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L349-L353`；连续5行，取自该符号定义体。

### SC123-E047｜rmdir owns rmdir,strerror,printf

[VERIFIED myinitramfs/myshell/myshell.c::rmdir_command]
```c
	char *dirname = argv[1];
	err = rmdir(dirname);
	if (err != 0)
		printf("rmdir: %s: %s\n", strerror(errno), dirname);
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L355-L358`；连续4行，取自该符号定义体。

### SC123-E048｜rmdir own return

[VERIFIED myinitramfs/myshell/myshell.c::rmdir_command]
```c
	if (err != 0)
		printf("rmdir: %s: %s\n", strerror(errno), dirname);

	return err;
```

`time@a039d9803ade` `myinitramfs/myshell/myshell.c#L357-L360`；连续4行，取自该符号定义体。

### SC123-E049｜helper formal five parameters

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_file_open]
```c
FILE *_IO_file_open(FILE *fp, const char *filename,
		int posix_mode, int prot, int read_write)
{
	int fdesc;

```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L173-L177`；连续5行，取自该符号定义体。

### SC123-E050｜helper calls open with three arguments and negative check

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_file_open]
```c
	fdesc = open(filename, posix_mode | O_LARGEFILE, prot);
	if (fdesc < 0)
		return NULL;
	fp->_fileno = fdesc;
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L178-L181`；连续4行，取自该符号定义体。

### SC123-E051｜helper returns FILE pointer on success

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_file_open]
```c
	// 		return NULL;
	// 	}
	// }
	return fp;
}
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L193-L197`；连续5行，取自该符号定义体。

### SC123-E052｜parse function signature

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
FILE *_IO_new_file_fopen(FILE *fp, const char *filename, const char *mode)
{
	int oflags = 0, omode;
	int read_write;
	int oprot = 0666;
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L199-L203`；连续5行，取自该符号定义体。

### SC123-E053｜fd already present gate and switch

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	if (fp->_fileno != -1)
		return 0;
	switch (*mode)
	{
	case 'r':
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L209-L213`；连续5行，取自该符号定义体。

### SC123-E054｜bad mode NULL result

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	default:
		// __set_errno(EINVAL);
		return NULL;
	}
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L227-L230`；连续4行，取自该符号定义体。

### SC123-E055｜five actual ordered helper arguments

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
		break;
	}

	result = _IO_file_open(fp, filename, omode | oflags, oprot, read_write);
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L264-L267`；连续4行，取自该符号定义体。

### SC123-E056｜inner parser returns helper result

[VERIFIED myinitramfs/mylib/myglibc/libio/fileops.c::_IO_new_file_fopen]
```c
	return result;
}
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/fileops.c#L339-L340`；连续2行，取自该符号定义体。

### SC123-E057｜outer wrapper passes same FILE P and returns parser result

[VERIFIED myinitramfs/mylib/myglibc/libio/iofopen.c::_IO_new_fopen]
```c
_IO_new_fopen(const char *filename, const char *mode)
{
	FILE *new_f = malloc(sizeof(FILE));
	new_f->_fileno = -1;
	return _IO_new_file_fopen(new_f, filename, mode);
```

`time@a039d9803ade` `myinitramfs/mylib/myglibc/libio/iofopen.c#L83-L87`；连续5行，取自该符号定义体。

### SC123-E058｜active time kernel C flag command

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

`time@a039d9803ade` `mykernel/scripts/options_flags.cmake#L39-L43`；连续5行，取自该符号定义体。

### SC123-E059｜active CONFIG wrapper flag command

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```

`time@a039d9803ade` `mykernel/scripts/options_flags.cmake#L64-L66`；连续3行，取自该符号定义体。

## 普通位置（不计P2合格窗）

| ID | 固定time来源 | 用途与限制 |
|---|---|---|
| SC123-P01 | `string/strcmp.c:18–24` | include/undef/ifndef是预处理上下文；STRCMP宏定义另作短窗 |
| SC123-P02 | `string/strncmp.c:18–25` | include、undef与ifndef是普通导航；STRNCMP宏定义另列 |
| SC123-P03 | `myinitramfs/mylib/myglibc/string/string.h:155–161` | strcmp和strncmp的extern声明，不是实现体 |
| SC123-P04 | `myinitramfs/mylib/myglibc/include/string.h:62` | 包装头包含string/string.h的include实例 |
| SC123-P05 | `myinitramfs/mylib/myglibc/string/CMakeLists.txt:1–3` | GLOB源/target_sources是构建目标文本，不认证sh真实链接 |
| SC123-P06 | `myinitramfs/mylib/myglibc/CMakeLists.txt:1–21` | mylibc STATIC声明与add_subdirectory(string) |
| SC123-P07 | `myinitramfs/CMakeLists.txt:3,40–51` | 顶层project后才设外部musl-gcc路径，仅加入myinit/myshell |
| SC123-P08 | `myinitramfs/myshell/CMakeLists.txt:1–10` | sh可用源glob和add_executable，没有已证mylibc链接结果 |
| SC123-P09 | `myinitramfs/myshell/myshell.c:1–9` | shell的string.h include实例，不等于本仓strcmp已被实际链接 |
| SC123-P10 | `myinitramfs/myshell/myshell.c:44–56` | 全局十项内建命令表，静态顺序位置而不是find_cmd定义体 |
| SC123-P11 | `myinitramfs/myshell/myshell.c:94–102` | main对parse返回的有效/负索引分支；不保证两支都执行 |
| SC123-P12 | `myinitramfs/mylib/myglibc/libio/fileops.c:173–197,199–340` | 五参helper和三参open与外层返回，跨函数归属按D00 |
| SC123-P13 | `mykernel/fs/vfs/open.c:266–270` | caller do_sys_open拥有两个直接调用点，返回how到后续的传值为DATA/SEQUENCE |
| SC123-P14 | `mykernel/fs/vfs/namei.c:916–929,983–996` | create/mkdir各自拥有间接方法回调与直接清理调用 |
| SC123-P15 | `mykernel/scripts/options_flags.cmake:39–70` | 固定time活动内核CMake命令，与用户mylibc/string源或实际sh编译不混淆 |

## 正式裁定与历史限定

R120-02、R121-01、R121-02已由C08判CLOSED_DOCUMENTATION_ONLY，120及121组合为PASS_PENDING_LOCAL。122整体RETURN，唯一文档纠正R122-01仍待主线独立验收。此处短窗只为当前批次，旧原件与评论冻结；完整本地P9、预处理/编译链接、Graphviz、QEMU/实机和文件操作NOT_RUN。
