---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SHELL-INPUT-DISPATCH-119
record_type: source_windows_and_ordinary_positions
produced_by: "GPT-6（后端不可独立认证）"
model_per_owner: "GPT-6＋Extra（Owner指定）"
date: 2026-11-21
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
inputs_read: [myinitramfs/myshell/myshell.c, myinitramfs/mylib/myglibc/string/strcmp.c, mykernel/fs/vfs/open.c, mykernel/fs/vfs/d_path.c, mykernel/scripts/options_flags.cmake]
open_questions: []
---

# 第119批：源代码短窗与普通位置（未运行）

全部真正P2定义体短窗在本文件；其余五件只按ID引用。本文件没有将旧作者证据标签的错误身份直接继承为新引文。引文对应冻结time，位置为当次读到的实际源码行；函数/宏内物理连续1–5行，非整体运行保证。

## 源码短窗（46项）

### IN119-E001｜启动时开启console并清零/请求cwd

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
	console_fd = open("/dev/console", O_RDWR);

	memset(current_dir, 0, PATH_MAX);
	getcwd(current_dir, PATH_MAX);
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L73-L76；定义体短窗4行，未运行。

### IN119-E002｜提示符前独立LF与fflush

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
	while (1)
	{
		printf("\n");
		printf("MyOS2:%s # _", current_dir);
		fflush(stdout);
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L83-L87；定义体短窗5行，未运行。

### IN119-E003｜主循环每轮清buf并读一行

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
		memset(buf, 0, SZ_4K);

		// buf[0] = 'l';
		// buf[1] = 's';
		read_line(buf);
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L88-L92；定义体短窗5行，未运行。

### IN119-E004｜读后LF以及argc/argv初始化与解析调用

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
		printf("\n");

		int __argc = 0;
		char ** __argv = NULL;
		index = parse_command(buf, &__argc, &__argv);
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L94-L98；定义体短窗5行，未运行。

### IN119-E005｜未知命令错误消息及成功派发

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
		index = parse_command(buf, &__argc, &__argv);
		if(index < 0)
			printf("Input Error, No Command Found!\n");
		else
			run_cmd(index, __argc, __argv);	//argc,argv
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L98-L102；定义体短窗5行，未运行。

### IN119-E006｜逐字节读取循环、EOT停止和首个退格请求

[VERIFIED myinitramfs/myshell/myshell.c::read_line]
```c
	int count = 0;

	while(read(console_fd, &key, 1) == 1 && key != 4)
	{
		write(console_fd, "\b", 1);
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L109-L113；定义体短窗5行，未运行。

### IN119-E007｜NUL继续及LF显式返回count

[VERIFIED myinitramfs/myshell/myshell.c::read_line]
```c
		case 0:
			continue;
		
		case '\n':
			return count;
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L116-L120；定义体短窗5行，未运行。

### IN119-E008｜退格时count>0额外回退/清空/请求退格

[VERIFIED myinitramfs/myshell/myshell.c::read_line]
```c
		case '\b':
			if (count > 0)
			{
				buf[--count] = 0;
				write(console_fd, "\b", 1);
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L122-L126；定义体短窗5行，未运行。

### IN119-E009｜普通字节写buf并回显

[VERIFIED myinitramfs/myshell/myshell.c::read_line]
```c
		default:
			buf[count++] = key;
			write(console_fd, &key, 1);
			break;
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L130-L133；定义体短窗4行，未运行。

### IN119-E010｜switch后无条件追加下划线请求

[VERIFIED myinitramfs/myshell/myshell.c::read_line]
```c
		}
		write(console_fd, "_", 1);
	}
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L134-L136；定义体短窗3行，未运行。

### IN119-E011｜ASCII空格的前导跳过

[VERIFIED myinitramfs/myshell/myshell.c::parse_command]
```c
	int j = 0;

	while(buf[j] == ' ')
		j++;
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L157-L160；定义体短窗4行，未运行。

### IN119-E012｜在256上限内以词尾判词数

[VERIFIED myinitramfs/myshell/myshell.c::parse_command]
```c
	{
		if(!buf[i])
			break;
		if(buf[i] != ' ' && (buf[i+1] == ' ' || buf[i+1] == '\0'))
			(*argc)++;
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L163-L167；定义体短窗5行，未运行。

### IN119-E013｜空参数提前失败、分配表达式及清零

[VERIFIED myinitramfs/myshell/myshell.c::parse_command]
```c
	if(!*argc)
		return -1;

	*argv = (char **)malloc(sizeof(char **) * (*argc + 1));
	memset(*argv, 0, sizeof(char **) * (*argc + 1));
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L169-L173；定义体短窗5行，未运行。

### IN119-E014｜argc加一槽并把argv指向原buf

[VERIFIED myinitramfs/myshell/myshell.c::parse_command]
```c
	*argv = (char **)malloc(sizeof(char **) * (*argc + 1));
	memset(*argv, 0, sizeof(char **) * (*argc + 1));
	for(i = 0; i < *argc && j < 256; i++)
	{
		*((*argv)+i) = &buf[j];
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L172-L176；定义体短窗5行，未运行。

### IN119-E015｜原buf分隔/终止位置写NUL及跳后续空格

[VERIFIED myinitramfs/myshell/myshell.c::parse_command]
```c
		while(buf[j] && buf[j] != ' ')
			j++;
		buf[j++] = '\0';
		while(buf[j] == ' ')
			j++;
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L178-L182；定义体短窗5行，未运行。

### IN119-E016｜指向第一个词进入find_cmd

[VERIFIED myinitramfs/myshell/myshell.c::parse_command]
```c
	}

	return find_cmd(**argv);
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L183-L185；定义体短窗3行，未运行。

### IN119-E017｜void派发函数内唯一间接调用且不使用返回值

[VERIFIED myinitramfs/myshell/myshell.c::run_cmd]
```c
void run_cmd(int index, int argc, char **argv)
{
	shell_internal_cmd[index].function(argc, argv);
}
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L139-L142；定义体短窗4行，未运行。

### IN119-E018｜有界十项线性strcmp与-1出口

[VERIFIED myinitramfs/myshell/myshell.c::find_cmd]
```c
	int i = 0;
	for(i = 0;i<sizeof(shell_internal_cmd)/sizeof(builtincmd_s);i++)
		if(!strcmp(cmd,shell_internal_cmd[i].name))
			return i;
	return -1;
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L147-L151；定义体短窗5行，未运行。

### IN119-E019｜内建表结构名称/函数指针成员

[VERIFIED myinitramfs/myshell/myshell.c::builtincmd_s]
```c
typedef struct builtincmd
{
	char *name;
	int (*function)(int,char**);
} builtincmd_s;
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L22-L26；定义体短窗5行，未运行。

### IN119-E020｜内建命令表cd ls pwd

[VERIFIED myinitramfs/myshell/myshell.c::shell_internal_cmd]
```c
builtincmd_s shell_internal_cmd[] = 
{
	{"cd",		cd_command},
	{"ls",		ls_command},
	{"pwd",		pwd_command},
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L44-L48；定义体短窗5行，未运行。

### IN119-E021｜内建命令表cat touch rm mkdir rmdir

[VERIFIED myinitramfs/myshell/myshell.c::shell_internal_cmd]
```c
	{"cat",		cat_command},
	{"touch",	touch_command},
	{"rm",		rm_command},
	{"mkdir",	mkdir_command},
	{"rmdir",	rmdir_command},
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L49-L53；定义体短窗5行，未运行。

### IN119-E022｜内建命令表exec reboot及结束

[VERIFIED myinitramfs/myshell/myshell.c::shell_internal_cmd]
```c
	{"exec",	exec_command},
	{"reboot",	reboot_command},
};
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L54-L56；定义体短窗3行，未运行。

### IN119-E023｜strcmp函数的unsigned字节指针入口

[VERIFIED myinitramfs/mylib/myglibc/string/strcmp.c::STRCMP]
```c
int STRCMP(const char *p1, const char *p2)
{
	const unsigned char *s1 = (const unsigned char *)p1;
	const unsigned char *s2 = (const unsigned char *)p2;
	unsigned char c1, c2;
```

固定time短12=`a039d9803ade`；myinitramfs/mylib/myglibc/string/strcmp.c#L29-L33；定义体短窗5行，未运行。

### IN119-E024｜逐字节比较并检测字符串结束

[VERIFIED myinitramfs/mylib/myglibc/string/strcmp.c::STRCMP]
```c
	do
	{
		c1 = (unsigned char)*s1++;
		c2 = (unsigned char)*s2++;
		if (c1 == '\0')
```

固定time短12=`a039d9803ade`；myinitramfs/mylib/myglibc/string/strcmp.c#L35-L39；定义体短窗5行，未运行。

### IN119-E025｜相等继续、不等返回字节差

[VERIFIED myinitramfs/mylib/myglibc/string/strcmp.c::STRCMP]
```c
		if (c1 == '\0')
			return c1 - c2;
	} while (c1 == c2);

	return c1 - c2;
```

固定time短12=`a039d9803ade`；myinitramfs/mylib/myglibc/string/strcmp.c#L39-L43；定义体短窗5行，未运行。

### IN119-E026｜cd的argv或homedir选路

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	if (argc <= 1)
		path = homedir;
	else
		path = argv[1];
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L194-L197；定义体短窗4行，未运行。

### IN119-E027｜点命令早退与chdir调用

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	/////.
	if(!strcmp(".", path))
		return 1;

	i = chdir(path);
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L199-L203；定义体短窗5行，未运行。

### IN119-E028｜cd成功后清零缓存并请求getcwd

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	i = chdir(path);
	if(!i)
	{
		memset(current_dir, 0, PATH_MAX);
		getcwd(current_dir, PATH_MAX);
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L203-L207；定义体短窗5行，未运行。

### IN119-E029｜cd失败输出及统一返回1

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	}
	else
		printf("cd: %s: %s\n", strerror(errno), path);
	return 1;
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L208-L211；定义体短窗4行，未运行。

### IN119-E030｜pwd只使用shell缓存作格式化输入

[VERIFIED myinitramfs/myshell/myshell.c::pwd_command]
```c
int pwd_command(int argc, char **argv)
{
	if(current_dir)
		printf("%s\n", current_dir);
	return 1;
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L245-L249；定义体短窗5行，未运行。

### IN119-E031｜cat读取缓存并分配拼接缓冲

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	len = strlen(current_dir);
	i = len + strlen(argv[1]);
	filename = malloc(i + 2);
	memset(filename, 0, i + 2);
	strcpy(filename, current_dir);
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L260-L264；定义体短窗5行，未运行。

### IN119-E032｜cat条件斜杠和strcat

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	if(len > 1)
		filename[len] = '/';
	strcat(filename, argv[1]);
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L265-L267；定义体短窗3行，未运行。

### IN119-E033｜内核chdir的lookup_flags初始化

[VERIFIED mykernel/fs/vfs/open.c::MYOS_SYSCALL_DEFINE1(chdir)]
```c
{
	path_s path;
	task_s *curr = current;
	int error;
	unsigned int lookup_flags = LOOKUP_FOLLOW | LOOKUP_DIRECTORY;
```

固定time短12=`a039d9803ade`；mykernel/fs/vfs/open.c#L273-L277；定义体短窗5行，未运行。

### IN119-E034｜内核chdir查找及成功写pwd

[VERIFIED mykernel/fs/vfs/open.c::MYOS_SYSCALL_DEFINE1(chdir)]
```c
	error = user_path_at(AT_FDCWD, filename, lookup_flags, &path);
	if (error)
		goto out;

	curr->fs->pwd = path;
```

固定time短12=`a039d9803ade`；mykernel/fs/vfs/open.c#L279-L283；定义体短窗5行，未运行。

### IN119-E035｜内核chdir直接返回error

[VERIFIED mykernel/fs/vfs/open.c::MYOS_SYSCALL_DEFINE1(chdir)]
```c
out:
	return error;
```

固定time短12=`a039d9803ade`；mykernel/fs/vfs/open.c#L291-L292；定义体短窗2行，未运行。

### IN119-E036｜内核getcwd读取pwd/root两个来源

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
	int		error;
	task_s *curr = current;
	path_s	pwd = curr->fs->pwd,
			root = curr->fs->root;
	prpndbuf_s b;
```

固定time短12=`a039d9803ade`；mykernel/fs/vfs/d_path.c#L198-L202；定义体短窗5行，未运行。

### IN119-E037｜内核getcwd从PATH_MAX设定长度并调用kzalloc

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		unsigned len;
		b.len = PATH_MAX;
		bufhead = kzalloc(b.len, GFP_KERNEL);
		b.buf = bufhead + PATH_MAX;
```

固定time短12=`a039d9803ade`；mykernel/fs/vfs/d_path.c#L209-L212；定义体短窗4行，未运行。

### IN119-E038｜内核getcwd直接调用两类prepend

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		myos_prepend(&b, "", 1);
		if (myos_prepend_path(&pwd, &root, &b) > 0)
			myos_prepend(&b, "(unreachable)", 13);
```

固定time短12=`a039d9803ade`；mykernel/fs/vfs/d_path.c#L214-L216；定义体短窗3行，未运行。

### IN119-E039｜内核getcwd长度门

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		len = PATH_MAX - b.len;
		if (len > PATH_MAX)
			error = -ENAMETOOLONG;
		else if (len > size)
			error = -ERANGE;
```

固定time短12=`a039d9803ade`；mykernel/fs/vfs/d_path.c#L218-L222；定义体短窗5行，未运行。

### IN119-E040｜复制用户字节、错误条件与指针转换

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		else if (copy_to_user(buf, b.buf, len))
			error = -EFAULT;
		else
			error = (long)buf;
```

固定time短12=`a039d9803ade`；mykernel/fs/vfs/d_path.c#L223-L226；定义体短窗4行，未运行。

### IN119-E041｜内核getcwd释放并返回int

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
	kfree(bufhead);
	return error;
```

固定time短12=`a039d9803ade`；mykernel/fs/vfs/d_path.c#L229-L230；定义体短窗2行，未运行。

### IN119-E042｜prepend_path内部调用__prepend_path

[VERIFIED mykernel/fs/vfs/d_path.c::myos_prepend_path]
```c
restart:
	b = *p;
	error = __prepend_path(path->dentry, real_mount(path->mnt), root, &b);

```

固定time短12=`a039d9803ade`；mykernel/fs/vfs/d_path.c#L161-L164；定义体短窗4行，未运行。

### IN119-E043｜prepend_path根路径处理及返回

[VERIFIED mykernel/fs/vfs/d_path.c::myos_prepend_path]
```c
	if (b.len == p->len)
		myos_prepend(&b, "/", 1);

	*p = b;
	return error;
```

固定time短12=`a039d9803ade`；mykernel/fs/vfs/d_path.c#L171-L175；定义体短窗5行，未运行。

### IN119-E044｜固定time活动C编译选项

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

固定time短12=`a039d9803ade`；mykernel/scripts/options_flags.cmake#L39-L43；定义体短窗5行，未运行。

### IN119-E045｜固定time打开的CONFIG宏

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```

固定time短12=`a039d9803ade`；mykernel/scripts/options_flags.cmake#L64-L66；定义体短窗3行，未运行。

### IN119-E046｜shell PATH_MAX宏定义

[VERIFIED myinitramfs/myshell/myshell.c::PATH_MAX]
```c
	#define PATH_MAX        4096	/* # chars in a path name including nul */
```

固定time短12=`a039d9803ade`；myinitramfs/myshell/myshell.c#L15-L15；定义体短窗1行，未运行。

## 普通位置、历史引用与反事实边界（不计P2短窗）

| 新位置ID | 固定来源与行 | 用途与限界 |
|---|---|---|
| IN119-P01 | `myinitramfs/myshell/myshell.c:59` | `char *homedir = "/";`是文件作用域指针对象定义，纠正旧CW118-E051；不作为函数体引文 |
| IN119-P02 | `myinitramfs/myshell/myshell.c:60` | `char current_dir[PATH_MAX];`是文件作用域数组对象定义，纠正旧CW118-E052；它与main/cd/pwd/cat实际消费分开 |
| IN119-P03 | `myinitramfs/myshell/myshell.c:39–41` | getcwd/chdir/reboot的文件作用域声明；不是函数调用体 |
| IN119-P04 | `myinitramfs/myshell/myshell.c:44–56` | 十项数组初始化整体；本文件另给聚合初始化内部短窗 |
| IN119-P05 | `mykernel/fs/vfs/open.c:272–293` | kernel chdir整段，仅用于对照条件式否定主张，不作为P2短窗 |
| IN119-P06 | `mykernel/fs/vfs/d_path.c:196–230` | kernel getcwd整段，支撑序列及错误出口全域观察 |
| IN119-P07 | `myinitramfs/myshell/myshell.c:106–137` | read_line完整体，含结束大括号前无显式return的范围观察 |
| IN119-P08 | `myinitramfs/myshell/myshell.c:154–186` | parse_command完整体，用于仅按当前函数内可见分配/未见释放的限定语 |
| IN119-P09 | `myinitramfs/myshell/myshell.c:66–104` | main完整体，针对变量重置和循环内未消费返回值的范围观察 |
| IN119-P10 | `mykernel/scripts/options_flags.cmake:39–70` | time活动C配置上下文；不冒充编译现场 |

历史不变：117固定提交短12=`52c51a3752d3`，118固定提交短12=`cb96288abb8b`；原CW118-E051/E052因文件作用域身份降级，旧evidence.md原件不动。第60批R60-S01第三项限定返回转换和复制独立性，R60-S02限定图与证据消费；并非同一结论。C08独立验收前R117-02/R118-01/R118-02均OPEN。

外部事实使用：无；源码、历史PR、验收与固定合同均来自GitHub连接器。编译、预处理、仓库脚本、真实设备、终端与Graphviz均NOT_RUN。
