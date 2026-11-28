---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SHELL-FILE-COMMANDS-120
record_type: source_windows_and_position_index
produced_by: "GPT-6（后端与effort未独立认证）"
model_per_owner: "GPT-6＋Extra（Owner选择）"
date: 2026-11-21
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
inputs_read: ["myinitramfs/myshell/myshell.c","mykernel/fs/vfs/namei.c","myinitramfs/mylib/myglibc/io/sys/stat.h","myinitramfs/mylib/myglibc/bits/stat.h","mykernel/include/linux/kernel/stat.h","mykernel/include/uapi/linux/stat.h","myinitramfs/mylib/myglibc/syscall/syscall.c","mykernel/arch/x86_64/entry/syscall_64.c","mykernel/arch/x86_64/include/asm/syscall_wrapper.h","mykernel/scripts/options_flags.cmake","myinitramfs/mylib/myglibc/stdio/printf.c","myinitramfs/mylib/myglibc/stdio/errlist.c"]
open_questions: []
---

# 第120批源码定义体短窗

所有新P2均从固定time短12=`a039d9803ade`的实际函数/宏/结构体定义体中原样读取1–5行；命题限源码局部行为，未经过编译/真实系统调用。旧件单独保全、不复制历史标签加入本批。

## shell调用/参数门与派发

### FC120-E001 · builtincmd_s 源码 22–26

[VERIFIED myinitramfs/myshell/myshell.c::builtincmd_s]
```c
typedef struct builtincmd
{
	char *name;
	int (*function)(int,char**);
} builtincmd_s;
```

固定time `myinitramfs/myshell/myshell.c#L22-L26`。短窗5行，仅源码证据。

### FC120-E002 · main 源码 94–98

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
		printf("\n");

		int __argc = 0;
		char ** __argv = NULL;
		index = parse_command(buf, &__argc, &__argv);
```

固定time `myinitramfs/myshell/myshell.c#L94-L98`。短窗5行，仅源码证据。

### FC120-E003 · main 源码 98–102

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
		index = parse_command(buf, &__argc, &__argv);
		if(index < 0)
			printf("Input Error, No Command Found!\n");
		else
			run_cmd(index, __argc, __argv);	//argc,argv
```

固定time `myinitramfs/myshell/myshell.c#L98-L102`。短窗5行，仅源码证据。

### FC120-E004 · run_cmd 源码 139–142

[VERIFIED myinitramfs/myshell/myshell.c::run_cmd]
```c
void run_cmd(int index, int argc, char **argv)
{
	shell_internal_cmd[index].function(argc, argv);
}
```

固定time `myinitramfs/myshell/myshell.c#L139-L142`。短窗4行，仅源码证据。

### FC120-E005 · find_cmd 源码 147–151

[VERIFIED myinitramfs/myshell/myshell.c::find_cmd]
```c
	int i = 0;
	for(i = 0;i<sizeof(shell_internal_cmd)/sizeof(builtincmd_s);i++)
		if(!strcmp(cmd,shell_internal_cmd[i].name))
			return i;
	return -1;
```

固定time `myinitramfs/myshell/myshell.c#L147-L151`。短窗5行，仅源码证据。

### FC120-E006 · parse_command 源码 166–170

[VERIFIED myinitramfs/myshell/myshell.c::parse_command]
```c
		if(buf[i] != ' ' && (buf[i+1] == ' ' || buf[i+1] == '\0'))
			(*argc)++;
	}
	if(!*argc)
		return -1;
```

固定time `myinitramfs/myshell/myshell.c#L166-L170`。短窗5行，仅源码证据。

### FC120-E007 · parse_command 源码 183–185

[VERIFIED myinitramfs/myshell/myshell.c::parse_command]
```c
	}

	return find_cmd(**argv);
```

固定time `myinitramfs/myshell/myshell.c#L183-L185`。短窗3行，仅源码证据。

### FC120-E008 · touch_command 源码 325–329

[VERIFIED myinitramfs/myshell/myshell.c::touch_command]
```c
int touch_command(int argc, char **argv)
{
	int err = 0, nmode = 0;
	if (argc != 2)
		return -1;
```

固定time `myinitramfs/myshell/myshell.c#L325-L329`。短窗5行，仅源码证据。

### FC120-E009 · touch_command 源码 331–334

[VERIFIED myinitramfs/myshell/myshell.c::touch_command]
```c
	nmode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	err = creat(argv[1], nmode);

	return 0;
```

固定time `myinitramfs/myshell/myshell.c#L331-L334`。短窗4行，仅源码证据。

### FC120-E010 · mkdir_command 源码 337–341

[VERIFIED myinitramfs/myshell/myshell.c::mkdir_command]
```c
int mkdir_command(int argc, char **argv)
{
	int err = 0, nmode = 0;
	if (argc != 2)
		return -1;
```

固定time `myinitramfs/myshell/myshell.c#L337-L341`。短窗5行，仅源码证据。

### FC120-E011 · mkdir_command 源码 343–346

[VERIFIED myinitramfs/myshell/myshell.c::mkdir_command]
```c
	nmode = (S_IRWXU | S_IRWXG | S_IRWXO);
	err = mkdir(argv[1], nmode);

	return 0;
```

固定time `myinitramfs/myshell/myshell.c#L343-L346`。短窗4行，仅源码证据。

### FC120-E012 · rmdir_command 源码 349–353

[VERIFIED myinitramfs/myshell/myshell.c::rmdir_command]
```c
int rmdir_command(int argc, char **argv)
{
	int err = 0;
	if (argc != 2)
		return -1;
```

固定time `myinitramfs/myshell/myshell.c#L349-L353`。短窗5行，仅源码证据。

### FC120-E013 · rmdir_command 源码 355–358

[VERIFIED myinitramfs/myshell/myshell.c::rmdir_command]
```c
	char *dirname = argv[1];
	err = rmdir(dirname);
	if (err != 0)
		printf("rmdir: %s: %s\n", strerror(errno), dirname);
```

固定time `myinitramfs/myshell/myshell.c#L355-L358`。短窗4行，仅源码证据。

### FC120-E014 · rmdir_command 源码 357–360

[VERIFIED myinitramfs/myshell/myshell.c::rmdir_command]
```c
	if (err != 0)
		printf("rmdir: %s: %s\n", strerror(errno), dirname);

	return err;
```

固定time `myinitramfs/myshell/myshell.c#L357-L360`。短窗4行，仅源码证据。

### FC120-E015 · rm_command 源码 363–367

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
int rm_command(int argc, char **argv)
{
	int err = 0;
	if (argc != 2)
		return -1;
```

固定time `myinitramfs/myshell/myshell.c#L363-L367`。短窗5行，仅源码证据。

### FC120-E016 · rm_command 源码 369–373

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
	char *filename = argv[1];
	err = unlink(filename);
	if (err != 0)
		printf("rm: %s: %s\n", strerror(errno), filename);

```

固定time `myinitramfs/myshell/myshell.c#L369-L373`。短窗5行，仅源码证据。

### FC120-E017 · rm_command 源码 371–374

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
	if (err != 0)
		printf("rm: %s: %s\n", strerror(errno), filename);

	return err;
```

固定time `myinitramfs/myshell/myshell.c#L371-L374`。短窗4行，仅源码证据。

## 用户宏mode推导

### FC120-E018 · S_IRUSR 源码 164–164

[VERIFIED myinitramfs/mylib/myglibc/io/sys/stat.h::S_IRUSR]
```c
	#define S_IRUSR	__S_IREAD  /* Read by owner.  */
```

固定time `myinitramfs/mylib/myglibc/io/sys/stat.h#L164-L164`。短窗1行，仅源码证据。

### FC120-E019 · S_IWUSR 源码 165–165

[VERIFIED myinitramfs/mylib/myglibc/io/sys/stat.h::S_IWUSR]
```c
	#define S_IWUSR	__S_IWRITE /* Write by owner.  */
```

固定time `myinitramfs/mylib/myglibc/io/sys/stat.h#L165-L165`。短窗1行，仅源码证据。

### FC120-E020 · S_IRGRP 源码 176–176

[VERIFIED myinitramfs/mylib/myglibc/io/sys/stat.h::S_IRGRP]
```c
	#define S_IRGRP	(S_IRUSR >> 3) /* Read by group.  */
```

固定time `myinitramfs/mylib/myglibc/io/sys/stat.h#L176-L176`。短窗1行，仅源码证据。

### FC120-E021 · S_IWGRP 源码 177–177

[VERIFIED myinitramfs/mylib/myglibc/io/sys/stat.h::S_IWGRP]
```c
	#define S_IWGRP	(S_IWUSR >> 3) /* Write by group.  */
```

固定time `myinitramfs/mylib/myglibc/io/sys/stat.h#L177-L177`。短窗1行，仅源码证据。

### FC120-E022 · S_IROTH 源码 182–182

[VERIFIED myinitramfs/mylib/myglibc/io/sys/stat.h::S_IROTH]
```c
	#define S_IROTH	(S_IRGRP >> 3) /* Read by others.  */
```

固定time `myinitramfs/mylib/myglibc/io/sys/stat.h#L182-L182`。短窗1行，仅源码证据。

### FC120-E023 · S_IWOTH 源码 183–183

[VERIFIED myinitramfs/mylib/myglibc/io/sys/stat.h::S_IWOTH]
```c
	#define S_IWOTH	(S_IWGRP >> 3) /* Write by others.  */
```

固定time `myinitramfs/mylib/myglibc/io/sys/stat.h#L183-L183`。短窗1行，仅源码证据。

### FC120-E024 · S_IRWXU 源码 168–168

[VERIFIED myinitramfs/mylib/myglibc/io/sys/stat.h::S_IRWXU]
```c
	#define S_IRWXU	(__S_IREAD | __S_IWRITE | __S_IEXEC)
```

固定time `myinitramfs/mylib/myglibc/io/sys/stat.h#L168-L168`。短窗1行，仅源码证据。

### FC120-E025 · S_IRWXG 源码 180–180

[VERIFIED myinitramfs/mylib/myglibc/io/sys/stat.h::S_IRWXG]
```c
	#define S_IRWXG	(S_IRWXU >> 3)
```

固定time `myinitramfs/mylib/myglibc/io/sys/stat.h#L180-L180`。短窗1行，仅源码证据。

### FC120-E026 · S_IRWXO 源码 186–186

[VERIFIED myinitramfs/mylib/myglibc/io/sys/stat.h::S_IRWXO]
```c
	#define S_IRWXO	(S_IRWXG >> 3)
```

固定time `myinitramfs/mylib/myglibc/io/sys/stat.h#L186-L186`。短窗1行，仅源码证据。

## 用户权限基础八进制位

### FC120-E027 · __S_IREAD 源码 82–82

[VERIFIED myinitramfs/mylib/myglibc/bits/stat.h::__S_IREAD]
```c
	#define __S_IREAD	0400	/* Read by owner.  */
```

固定time `myinitramfs/mylib/myglibc/bits/stat.h#L82-L82`。短窗1行，仅源码证据。

### FC120-E028 · __S_IWRITE 源码 83–83

[VERIFIED myinitramfs/mylib/myglibc/bits/stat.h::__S_IWRITE]
```c
	#define __S_IWRITE	0200 /* Write by owner.  */
```

固定time `myinitramfs/mylib/myglibc/bits/stat.h#L83-L83`。短窗1行，仅源码证据。

### FC120-E029 · __S_IEXEC 源码 84–84

[VERIFIED myinitramfs/mylib/myglibc/bits/stat.h::__S_IEXEC]
```c
	#define __S_IEXEC	0100	/* Execute by owner.  */
```

固定time `myinitramfs/mylib/myglibc/bits/stat.h#L84-L84`。短窗1行，仅源码证据。

## 内核文件权限掩码

### FC120-E030 · S_IRWXUGO 源码 9–9

[VERIFIED mykernel/include/linux/kernel/stat.h::S_IRWXUGO]
```c
	#define S_IRWXUGO	(S_IRWXU|S_IRWXG|S_IRWXO)
```

固定time `mykernel/include/linux/kernel/stat.h#L9-L9`。短窗1行，仅源码证据。

## 内核sticky位

### FC120-E031 · S_ISVTX 源码 18–18

[VERIFIED mykernel/include/uapi/linux/stat.h::S_ISVTX]
```c
	#	define S_ISVTX		0001000		// ---- --1- ---- ---- b
```

固定time `mykernel/include/uapi/linux/stat.h#L18-L18`。短窗1行，仅源码证据。

## 内核用户命令的VFS入口及方法回调

### FC120-E032 · do_creatat 源码 899–903

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
long do_creatat(int dfd, filename_s *name, umode_t mode)
{
	dentry_s *dentry;
	path_s path;
	inode_s *dir;
```

固定time `mykernel/fs/vfs/namei.c#L899-L903`。短窗5行，仅源码证据。

### FC120-E033 · do_creatat 源码 904–908

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
	int error;
	unsigned int lookup_flags = LOOKUP_DIRECTORY;

retry:
	dentry = filename_create(dfd, name, &path, lookup_flags);
```

固定time `mykernel/fs/vfs/namei.c#L904-L908`。短窗5行，仅源码证据。

### FC120-E034 · do_creatat 源码 909–911

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
	error = PTR_ERR(dentry);
	if (IS_ERR(dentry))
		goto out_putname;
```

固定time `mykernel/fs/vfs/namei.c#L909-L911`。短窗3行，仅源码证据。

### FC120-E035 · do_creatat 源码 916–920

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
		dir = path.dentry->d_inode;
		error = -ENOENT;
		if (!dir->i_op->create)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
```

固定time `mykernel/fs/vfs/namei.c#L916-L920`。短窗5行，仅源码证据。

### FC120-E036 · do_creatat 源码 920–922

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
		mode &= (S_IRWXUGO|S_ISVTX);
		error = dir->i_op->create(dir, dentry, mode);
	done_path_create(&path, dentry);
```

固定time `mykernel/fs/vfs/namei.c#L920-L922`。短窗3行，仅源码证据。

### FC120-E037 · do_creatat 源码 927–929

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
out_putname:
	putname(name);
	return error;
```

固定time `mykernel/fs/vfs/namei.c#L927-L929`。短窗3行，仅源码证据。

### FC120-E038 · MYOS_SYSCALL_DEFINE2(creat) 源码 932–935

[VERIFIED mykernel/fs/vfs/namei.c::MYOS_SYSCALL_DEFINE2(creat)]
```c
MYOS_SYSCALL_DEFINE2(creat, const char *, pathname, umode_t, mode)
{
	return do_creatat(AT_FDCWD, getname(pathname), mode);
}
```

固定time `mykernel/fs/vfs/namei.c#L932-L935`。短窗4行，仅源码证据。

### FC120-E039 · filename_create 源码 774–778

[VERIFIED mykernel/fs/vfs/namei.c::filename_create]
```c
	qstr_s last;
	int type;
	int err2;
	int error;
	bool is_dir = (lookup_flags & LOOKUP_DIRECTORY);
```

固定time `mykernel/fs/vfs/namei.c#L774-L778`。短窗5行，仅源码证据。

### FC120-E040 · filename_create 源码 784–788

[VERIFIED mykernel/fs/vfs/namei.c::filename_create]
```c
	lookup_flags &= LOOKUP_REVAL;

	error = filename_parentat(dfd, name, lookup_flags, path, &last, &type);
	if (error)
		return ERR_PTR(error);
```

固定time `mykernel/fs/vfs/namei.c#L784-L788`。短窗5行，仅源码证据。

### FC120-E041 · done_path_create 源码 835–839

[VERIFIED mykernel/fs/vfs/namei.c::done_path_create]
```c
void done_path_create(path_s *path, dentry_s *dentry)
{
	dput(dentry);
	path_put(path);
}
```

固定time `mykernel/fs/vfs/namei.c#L835-L839`。短窗5行，仅源码证据。

### FC120-E042 · do_mkdirat 源码 966–970

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
long do_mkdirat(int dfd, filename_s *name, umode_t mode)
{
	dentry_s *dentry;
	path_s path;
	inode_s *dir;
```

固定time `mykernel/fs/vfs/namei.c#L966-L970`。短窗5行，仅源码证据。

### FC120-E043 · do_mkdirat 源码 971–975

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
	int error;
	unsigned int lookup_flags = LOOKUP_DIRECTORY;

retry:
	dentry = filename_create(dfd, name, &path, lookup_flags);
```

固定time `mykernel/fs/vfs/namei.c#L971-L975`。短窗5行，仅源码证据。

### FC120-E044 · do_mkdirat 源码 976–978

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
	error = PTR_ERR(dentry);
	if (IS_ERR(dentry))
		goto out_putname;
```

固定time `mykernel/fs/vfs/namei.c#L976-L978`。短窗3行，仅源码证据。

### FC120-E045 · do_mkdirat 源码 983–987

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
		dir = path.dentry->d_inode;
		error = -ENOENT;
		if (!dir->i_op->mkdir)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
```

固定time `mykernel/fs/vfs/namei.c#L983-L987`。短窗5行，仅源码证据。

### FC120-E046 · do_mkdirat 源码 985–989

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
		if (!dir->i_op->mkdir)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
		error = dir->i_op->mkdir(dir, dentry, mode);
	done_path_create(&path, dentry);
```

固定time `mykernel/fs/vfs/namei.c#L985-L989`。短窗5行，仅源码证据。

### FC120-E047 · do_mkdirat 源码 994–996

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
out_putname:
	putname(name);
	return error;
```

固定time `mykernel/fs/vfs/namei.c#L994-L996`。短窗3行，仅源码证据。

### FC120-E048 · MYOS_SYSCALL_DEFINE2(mkdir) 源码 999–1002

[VERIFIED mykernel/fs/vfs/namei.c::MYOS_SYSCALL_DEFINE2(mkdir)]
```c
MYOS_SYSCALL_DEFINE2(mkdir, const char *, pathname, umode_t, mode)
{
	return do_mkdirat(AT_FDCWD, getname(pathname), mode);
}
```

固定time `mykernel/fs/vfs/namei.c#L999-L1002`。短窗4行，仅源码证据。

### FC120-E049 · vfs_mkdir 源码 955–959

[VERIFIED mykernel/fs/vfs/namei.c::vfs_mkdir]
```c
int vfs_mkdir(inode_s *dir, dentry_s *dentry, umode_t mode)
{
	int error = -ENOENT;
	if (!dir->i_op->mkdir)
		return -EPERM;
```

固定time `mykernel/fs/vfs/namei.c#L955-L959`。短窗5行，仅源码证据。

### FC120-E050 · vfs_mkdir 源码 961–963

[VERIFIED mykernel/fs/vfs/namei.c::vfs_mkdir]
```c
	mode &= (S_IRWXUGO|S_ISVTX);
	error = dir->i_op->mkdir(dir, dentry, mode);
	return error;
```

固定time `mykernel/fs/vfs/namei.c#L961-L963`。短窗3行，仅源码证据。

### FC120-E051 · do_rmdir 源码 1042–1046

[VERIFIED mykernel/fs/vfs/namei.c::do_rmdir]
```c
long do_rmdir(int dfd, filename_s *name)
{
	int error;
	dentry_s *dentry;
	path_s path;
```

固定time `mykernel/fs/vfs/namei.c#L1042-L1046`。短窗5行，仅源码证据。

### FC120-E052 · do_rmdir 源码 1067–1071

[VERIFIED mykernel/fs/vfs/namei.c::do_rmdir]
```c
	dentry = __lookup_hash(&last, path.dentry, lookup_flags);
	error = PTR_ERR(dentry);
	if (IS_ERR(dentry))
		goto exit3;
	if (!dentry->d_inode) {
```

固定time `mykernel/fs/vfs/namei.c#L1067-L1071`。短窗5行，仅源码证据。

### FC120-E053 · do_rmdir 源码 1075–1079

[VERIFIED mykernel/fs/vfs/namei.c::do_rmdir]
```c
	if (!S_ISDIR(dentry->d_inode->i_mode))
	{
		error = -ENOTDIR;
		goto exit4;
	}
```

固定time `mykernel/fs/vfs/namei.c#L1075-L1079`。短窗5行，仅源码证据。

### FC120-E054 · do_rmdir 源码 1081–1085

[VERIFIED mykernel/fs/vfs/namei.c::do_rmdir]
```c
	error = vfs_rmdir(path.dentry->d_inode, dentry);
exit4:
	dput(dentry);
exit3:
exit2:
```

固定time `mykernel/fs/vfs/namei.c#L1081-L1085`。短窗5行，仅源码证据。

### FC120-E055 · do_rmdir 源码 1085–1089

[VERIFIED mykernel/fs/vfs/namei.c::do_rmdir]
```c
exit2:
	path_put(&path);
exit1:
	putname(name);
	return error;
```

固定time `mykernel/fs/vfs/namei.c#L1085-L1089`。短窗5行，仅源码证据。

### FC120-E056 · MYOS_SYSCALL_DEFINE1(rmdir) 源码 1092–1095

[VERIFIED mykernel/fs/vfs/namei.c::MYOS_SYSCALL_DEFINE1(rmdir)]
```c
MYOS_SYSCALL_DEFINE1(rmdir, const char *, pathname)
{
	return do_rmdir(AT_FDCWD, getname(pathname));
}
```

固定time `mykernel/fs/vfs/namei.c#L1092-L1095`。短窗4行，仅源码证据。

### FC120-E057 · vfs_rmdir 源码 1022–1026

[VERIFIED mykernel/fs/vfs/namei.c::vfs_rmdir]
```c
{
	int error = 0;

	if (!dir->i_op->rmdir)
		return -EPERM;
```

固定time `mykernel/fs/vfs/namei.c#L1022-L1026`。短窗5行，仅源码证据。

### FC120-E058 · vfs_rmdir 源码 1028–1032

[VERIFIED mykernel/fs/vfs/namei.c::vfs_rmdir]
```c
	dget(dentry);

	error = -EBUSY;
	error = dir->i_op->rmdir(dir, dentry);
	if (error)
```

固定time `mykernel/fs/vfs/namei.c#L1028-L1032`。短窗5行，仅源码证据。

### FC120-E059 · do_unlinkat 源码 1151–1155

[VERIFIED mykernel/fs/vfs/namei.c::do_unlinkat]
```c
long do_unlinkat(int dfd, filename_s *name)
{
	int error;
	dentry_s *dentry;
	path_s path;
```

固定time `mykernel/fs/vfs/namei.c#L1151-L1155`。短窗5行，仅源码证据。

### FC120-E060 · do_unlinkat 源码 1156–1160

[VERIFIED mykernel/fs/vfs/namei.c::do_unlinkat]
```c
	qstr_s last;
	int type;
	inode_s *inode = NULL;
	unsigned int lookup_flags = 0;
retry:
```

固定time `mykernel/fs/vfs/namei.c#L1156-L1160`。短窗5行，仅源码证据。

### FC120-E061 · do_unlinkat 源码 1161–1165

[VERIFIED mykernel/fs/vfs/namei.c::do_unlinkat]
```c
	error = filename_parentat(dfd, name, lookup_flags, &path, &last, &type);
	if (error)
		goto exit1;

	error = -EISDIR;
```

固定time `mykernel/fs/vfs/namei.c#L1161-L1165`。短窗5行，仅源码证据。

### FC120-E062 · do_unlinkat 源码 1169–1173

[VERIFIED mykernel/fs/vfs/namei.c::do_unlinkat]
```c
	dentry = __lookup_hash(&last, path.dentry, lookup_flags);
	error = PTR_ERR(dentry);
	if (!IS_ERR(dentry)) {
// 		/* Why not before? Because we want correct error value */
// 		if (last.name[last.len])
```

固定time `mykernel/fs/vfs/namei.c#L1169-L1173`。短窗5行，仅源码证据。

### FC120-E063 · do_unlinkat 源码 1175–1179

[VERIFIED mykernel/fs/vfs/namei.c::do_unlinkat]
```c
		inode = dentry->d_inode;
		if (S_ISDIR(inode->i_mode))
		{
			error = -EISDIR;
			goto exit3;
```

固定time `mykernel/fs/vfs/namei.c#L1175-L1179`。短窗5行，仅源码证据。

### FC120-E064 · do_unlinkat 源码 1182–1186

[VERIFIED mykernel/fs/vfs/namei.c::do_unlinkat]
```c
		error = vfs_unlink(path.dentry->d_inode, dentry);
exit3:
		dput(dentry);
	}
	if (inode)
```

固定time `mykernel/fs/vfs/namei.c#L1182-L1186`。短窗5行，仅源码证据。

### FC120-E065 · do_unlinkat 源码 1190–1194

[VERIFIED mykernel/fs/vfs/namei.c::do_unlinkat]
```c
exit2:
	path_put(&path);
exit1:
	putname(name);
	return error;
```

固定time `mykernel/fs/vfs/namei.c#L1190-L1194`。短窗5行，仅源码证据。

### FC120-E066 · MYOS_SYSCALL_DEFINE1(unlink) 源码 1197–1200

[VERIFIED mykernel/fs/vfs/namei.c::MYOS_SYSCALL_DEFINE1(unlink)]
```c
MYOS_SYSCALL_DEFINE1(unlink, const char *, pathname)
{
	return do_unlinkat(AT_FDCWD, getname(pathname));
}
```

固定time `mykernel/fs/vfs/namei.c#L1197-L1200`。短窗4行，仅源码证据。

### FC120-E067 · vfs_unlink 源码 1125–1129

[VERIFIED mykernel/fs/vfs/namei.c::vfs_unlink]
```c
int vfs_unlink(inode_s *dir, dentry_s *dentry)
{
	inode_s *target = dentry->d_inode;
	int error = 0;

```

固定time `mykernel/fs/vfs/namei.c#L1125-L1129`。短窗5行，仅源码证据。

### FC120-E068 · vfs_unlink 源码 1130–1134

[VERIFIED mykernel/fs/vfs/namei.c::vfs_unlink]
```c
	if (!dir->i_op->unlink)
		return -EPERM;

	// error = security_inode_unlink(dir, dentry);
	// if (!error) {
```

固定time `mykernel/fs/vfs/namei.c#L1130-L1134`。短窗5行，仅源码证据。

### FC120-E069 · vfs_unlink 源码 1135–1135

[VERIFIED mykernel/fs/vfs/namei.c::vfs_unlink]
```c
		error = dir->i_op->unlink(dir, dentry);
```

固定time `mykernel/fs/vfs/namei.c#L1135-L1135`。短窗1行，仅源码证据。

## 用户syscall汇编桩宏

### FC120-E070 · __SYSFUNC_DEF__ 源码 19–23

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::__SYSFUNC_DEF__]
```c
#define __SYSFUNC_DEF__(name, nr)							\
			__asm__	(										\
				".global	"#name"					\n\t"	\
				".type		"#name",	@function	\n\t"	\
				#name":								\n\t"	\
```

固定time `myinitramfs/mylib/myglibc/syscall/syscall.c#L19-L23`。短窗5行，仅源码证据。

### FC120-E071 · __SYSFUNC_DEF__ 源码 24–27

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::__SYSFUNC_DEF__]
```c
				"pushq	%rbp						\n\t"	\
				"movq	%rsp,	%rbp				\n\t"	\
				"movq	$"#nr",	%rax				\n\t"	\
				"jmp	LABEL_SYSCALL				\n\t"	\
```

固定time `myinitramfs/mylib/myglibc/syscall/syscall.c#L24-L27`。短窗4行，仅源码证据。

### FC120-E072 · LABEL_SYSCALL 源码 71–75

[VERIFIED myinitramfs/mylib/myglibc/syscall/syscall.c::LABEL_SYSCALL]
```c
	"LABEL_SYSCALL:							\n\t"
	"leaq	sysexit_ret_addr(%rip),	%rcx	\n\t"
	"syscall								\n\t"
	"sysexit_ret_addr:						\n\t"
	"cmpq	$-0x1000,	%rax				\n\t"
```

固定time `myinitramfs/mylib/myglibc/syscall/syscall.c#L71-L75`。短窗5行，仅源码证据。

## 内核活动switch

### FC120-E073 · __SYSCALL 源码 26–26

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
```

固定time `mykernel/arch/x86_64/entry/syscall_64.c#L26-L26`。短窗1行，仅源码证据。

### FC120-E074 · x64_sys_call 源码 27–31

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```c
long x64_sys_call(const pt_regs_s *regs, unsigned int nr)
{
	switch (nr) {
	#include <asm/syscalls_64.h>
	default: return __x64_sys_ni_syscall(regs);
```

固定time `mykernel/arch/x86_64/entry/syscall_64.c#L27-L31`。短窗5行，仅源码证据。

## syscall x64 wrapper生成

### FC120-E075 · __SYS_STUBx 源码 104–108

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__SYS_STUBx]
```c
	#define __SYS_STUBx(abi, name, ...)							\
				long __##abi##_##name(const pt_regs_s *regs);	\
				long __##abi##_##name(const pt_regs_s *regs)	\
				{												\
					return __se_##name(regs);					\
```

固定time `mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L104-L108`。短窗5行，仅源码证据。

### FC120-E076 · __MYOS_SYSCALL_DEFINEx 源码 241–245

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
	#define __MYOS_SYSCALL_DEFINEx(x, name, ...)						\
				long __se_sys##name(const pt_regs_s *regs);				\
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));	\
				__SYS_STUBx(x64, sys##name)								\
				long __se_sys##name(const pt_regs_s *regs)				\
```

固定time `mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L241-L245`。短窗5行，仅源码证据。

### FC120-E077 · __MYOS_SYSCALL_DEFINEx 源码 245–249

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
				long __se_sys##name(const pt_regs_s *regs)				\
				{														\
					long ret = __do_sys##name(							\
								__ARCH_MAP(x,__SC_CAST,__VA_ARGS__));	\
					return ret;											\
```

固定time `mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L245-L249`。短窗5行，仅源码证据。

## printf写入请求

### FC120-E078 · printf 源码 18–22

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	rv = vsnprintf(buf, SZ_4K, fmt, ap);
	va_end(ap);

	// buffered IO has unsolved bug, so derectly use the write() function
	write(stdout->_fileno, buf, rv);
```

固定time `myinitramfs/mylib/myglibc/stdio/printf.c#L18-L22`。短窗5行，仅源码证据。

## errlist入口

### FC120-E079 · __get_errlist 源码 23–27

[VERIFIED myinitramfs/mylib/myglibc/stdio/errlist.c::__get_errlist]
```c
const char *
__get_errlist(int errnum)
{
   // if (mapped >= 0 && mapped < array_length(_sys_errlist_internal))
   if (errnum < 0)
```

固定time `myinitramfs/mylib/myglibc/stdio/errlist.c#L23-L27`。短窗5行，仅源码证据。

## 固定time C配置

### FC120-E080 · set(CMAKE_C_FLAGS) 源码 39–43

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

固定time `mykernel/scripts/options_flags.cmake#L39-L43`。短窗5行，仅源码证据。

### FC120-E081 · set(CMAKE_C_FLAGS) 源码 64–66

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```

固定time `mykernel/scripts/options_flags.cmake#L64-L66`。短窗3行，仅源码证据。

## 普通位置（不按P2资格计）

| 位置ID | 固定time来源 | 正确读法 |
|---|---|---|
| FC120-P01 | `myshell.c:44–56` | 全局数组初始化，touch索引4、rm5、mkdir6、rmdir7；真实绑定，不是函数/宏/结构体定义体 |
| FC120-P02 | `myinitramfs/mylib/myglibc/syscall/syscall.c:53–57` | SYSFUNC_DEF四个宏实例，不能将调用展开当宏定义 |
| FC120-P03 | `mykernel/arch/x86_64/include/asm/unistd_64.h:83–87` | __NR_mkdir=83/rmdir=84/creat=85/unlink=87，独立宏定义位置及编号查表 |
| FC120-P04 | `mykernel/arch/x86_64/include/asm/syscalls_64.h:44–48` | 活动__SYSCALL映射实例，非宏定义 |
| FC120-P05 | `mykernel/fs/vfs/namei.c:899–935` | do_creatat及系统入口完整体；仅对本函数体的fd安装/关闭可做局部否定观察 |
| FC120-P06 | `mykernel/fs/vfs/namei.c:955–1002` | vfs_mkdir和do_mkdirat是两个不同函数，后者直接调用i_op->mkdir |
| FC120-P07 | `mykernel/fs/vfs/namei.c:1042–1095` | rmdir完整功能体/分支/清理门，不能从名称引申系统行为 |
| FC120-P08 | `mykernel/fs/vfs/namei.c:1151–1200` | unlink完整体；dentry->d_inode非空是类型测试路径的独立对象前提 |
| FC120-P09 | `myinitramfs/mylib/myglibc/syscall/syscall.c:74–81` | errno赋值与归一成−1两句处于注释，不能当活动映射 |
| FC120-P10 | `mykernel/fs/vfs/namei.c:770–825` | filename_create完整体的选择与返回条件，路径最终效果未运行 |
| FC120-P11 | `myinitramfs/mylib/myglibc/stdio/errlist_internal.c:23–969` | 信息表存在不证明shell的errno来自刚刚失败的调用 |
| FC120-P12 | `mykernel/fs/vfs/d_path.c:196–231` | D00所需getcwd完整体，长/短条件证据按固定119历史与本轮新图合读 |

## 历史P2资格和消费范围

117原88窗/89字面次，额外旧MANIFEST:113；118原91窗/92次，E051/E052全局文件对象转普通位置后为89个候选合格窗；119原46窗/46次，E020/E021/E022全局数组初值转普通位置后为43个候选合格窗（E019结构体定义保留）。资格修正只在120追加，不改旧件。计数与P6中仅本批六件的字面标签独立计算。

第57/58批已有FAT与VFS内部评估及正式文档闭合，以主线裁定和必要限定为先；本批不据局部回调返回替全系统宣称真实文件创建/删除、磁盘落盘或POSIX标准一致性。源码外硬件/编译/Graphviz/本地P9均NOT_RUN。
