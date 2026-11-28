---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LIBC-STRING-COPY-APPEND-125
record_type: pinned_definition_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-19
produced_by: "GPT-6（本会话自报；后台与推理档UNATTESTED）"
date: 2026-11-22
date_timezone: Asia/Shanghai
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
inputs_read:
  - "myinitramfs/mylib/myglibc/string/strcpy.c"
  - "myinitramfs/mylib/myglibc/string/strcat.c"
  - "myinitramfs/mylib/myglibc/string/strlen.c"
  - "myinitramfs/mylib/myglibc/string/memcpy.c"
  - "myinitramfs/mylib/myglibc/string/strncpy.c"
  - "myinitramfs/myshell/myshell.c"
  - "myinitramfs/mylib/myglibc/CMakeLists.txt"
  - "myinitramfs/mylib/myglibc/string/CMakeLists.txt"
  - "myinitramfs/CMakeLists.txt"
  - "myinitramfs/myshell/CMakeLists.txt"
  - "mykernel/scripts/options_flags.cmake"
  - "mykernel/debug/panic.c"
---
# 第125批独立源码短窗

所有短窗于本轮从 `time@a039d9803ade`（短12）直接读取，严格为符号定义内部或单条活动CMake命令的连续1–5个物理行。说明文字与引文分离；不从124自报复制P2标签。只证源文本，不证实际程序执行或链接。CMake的源码收集并非sh最终链接凭据。

## SC125-E001 · A 内层函数入口、strlen +1和memcpy返回

[VERIFIED myinitramfs/mylib/myglibc/string/strcpy.c::STRCPY]
```c
STRCPY(char *dest, const char *src)
{
	return memcpy(dest, src, strlen(src) + 1);
}
```

定位：myinitramfs/mylib/myglibc/string/strcpy.c:29-32。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E002 · A 长度数据送入复制

[VERIFIED myinitramfs/mylib/myglibc/string/strcpy.c::STRCPY]
```c
{
	return memcpy(dest, src, strlen(src) + 1);
```

定位：myinitramfs/mylib/myglibc/string/strcpy.c:30-31。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E003 · B 外层原始dest返回

[VERIFIED myinitramfs/mylib/myglibc/string/strcat.c::STRCAT]
```c
STRCAT(char *dest, const char *src)
{
	strcpy(dest + strlen(dest), src);
	return dest;
}
```

定位：myinitramfs/mylib/myglibc/string/strcat.c:28-32。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E004 · B 旧NUL定位和内层复制

[VERIFIED myinitramfs/mylib/myglibc/string/strcat.c::STRCAT]
```c
	strcpy(dest + strlen(dest), src);
	return dest;
```

定位：myinitramfs/mylib/myglibc/string/strcat.c:30-31。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E005 · A 字节前导及首NUL长度

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
	for (char_ptr = str; ((unsigned long int)char_ptr & (sizeof(longword) - 1)) != 0;
		 ++char_ptr)
		if (*char_ptr == '\0')
			return char_ptr - str;
```

定位：myinitramfs/mylib/myglibc/string/strlen.c:38-41。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E006 · A 整字筛选

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
		longword = *longword_ptr++;

		if (((longword - lomagic) & ~longword & himagic) != 0)
```

定位：myinitramfs/mylib/myglibc/string/strlen.c:74-76。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E007 · A 候选首字节确认

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
			const char *cp = (const char *)(longword_ptr - 1);

			if (cp[0] == 0)
				return cp - str;
```

定位：myinitramfs/mylib/myglibc/string/strlen.c:81-84。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E008 · A 候选次字节确认

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
			if (cp[1] == 0)
				return cp - str + 1;
			if (cp[2] == 0)
				return cp - str + 2;
```

定位：myinitramfs/mylib/myglibc/string/strlen.c:85-88。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E009 · A memcpy目的与源记录

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
void *
MEMCPY(void *dstpp, const void *srcpp, size_t len)
{
	unsigned long int dstp = (long int)dstpp;
	unsigned long int srcp = (long int)srcpp;
```

定位：myinitramfs/mylib/myglibc/string/memcpy.c:27-31。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E010 · A memcpy短尾和原目的地址返回

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	/* There are just a few bytes to copy.  Use byte memory operations.  */
	BYTE_COPY_FWD(dstp, srcp, len);

	return dstpp;
```

定位：myinitramfs/mylib/myglibc/string/memcpy.c:56-59。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E011 · A 上批n=源长对比

[VERIFIED myinitramfs/mylib/myglibc/string/strncpy.c::STRNCPY]
```c
STRNCPY(char *s1, const char *s2, size_t n)
{
	size_t size = __strnlen(s2, n);
	if (size != n)
```

定位：myinitramfs/mylib/myglibc/string/strncpy.c:27-30。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E012 · A size==n不补终止符

[VERIFIED myinitramfs/mylib/myglibc/string/strncpy.c::STRNCPY]
```c
	if (size != n)
		memset(s1 + size, '\0', n - size);
	return memcpy(s1, s2, size);
}
```

定位：myinitramfs/mylib/myglibc/string/strncpy.c:30-33。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E013 · C cat参数与filename局部指针

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
int cat_command(int argc, char **argv)
{
	int len = 0;
	char *filename = NULL;
	int fd = 0;
```

定位：myinitramfs/myshell/myshell.c:252-256。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E014 · C 显式两次strlen和malloc

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	int i = 0;

	len = strlen(current_dir);
	i = len + strlen(argv[1]);
	filename = malloc(i + 2);
```

定位：myinitramfs/myshell/myshell.c:258-262。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E015 · C 两长度分配并清零

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	len = strlen(current_dir);
	i = len + strlen(argv[1]);
	filename = malloc(i + 2);
	memset(filename, 0, i + 2);
```

定位：myinitramfs/myshell/myshell.c:260-263。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E016 · C malloc memset和第一次strcpy

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	i = len + strlen(argv[1]);
	filename = malloc(i + 2);
	memset(filename, 0, i + 2);
	strcpy(filename, current_dir);
```

定位：myinitramfs/myshell/myshell.c:261-264。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E017 · C 从清零到条件斜杠和strcat

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	memset(filename, 0, i + 2);
	strcpy(filename, current_dir);
	if(len > 1)
		filename[len] = '/';
	strcat(filename, argv[1]);
```

定位：myinitramfs/myshell/myshell.c:263-267。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E018 · C 分隔符覆盖旧NUL的条件

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	strcpy(filename, current_dir);
	if(len > 1)
		filename[len] = '/';
	strcat(filename, argv[1]);
```

定位：myinitramfs/myshell/myshell.c:264-267。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E019 · C 分隔符写入及随后追加

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	if(len > 1)
		filename[len] = '/';
	strcat(filename, argv[1]);
```

定位：myinitramfs/myshell/myshell.c:265-267。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E020 · C 追加到open调用

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	strcat(filename, argv[1]);
	// printf("cat_command filename:%s\n", filename);

	fd = open(filename, O_RDONLY, 0);	
```

定位：myinitramfs/myshell/myshell.c:267-270。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E021 · C open实参和调用点

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	fd = open(filename, O_RDONLY, 0);	
```

定位：myinitramfs/myshell/myshell.c:270-270。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E022 · build 字符串源收集

[VERIFIED myinitramfs/mylib/myglibc/string/CMakeLists.txt::target_sources]
```cmake
target_sources(mylibc PUBLIC ${LIBC_STRING_C_SRCS} ${LIBC_STRING_ASM_SRCS})
```

定位：myinitramfs/mylib/myglibc/string/CMakeLists.txt:3-3。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E023 · build mylibc STATIC

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_library]
```cmake
add_library(mylibc STATIC)
```

定位：myinitramfs/mylib/myglibc/CMakeLists.txt:1-1。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E024 · build 字符串库目录

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(string)
```

定位：myinitramfs/mylib/myglibc/CMakeLists.txt:20-20。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E025 · build musl-gcc路径配置

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(MUSL_PATH /home/cheyh/projs/musl/build)
```

定位：myinitramfs/CMakeLists.txt:40-40。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E026 · build musl头

[VERIFIED myinitramfs/CMakeLists.txt::include_directories]
```cmake
include_directories("${MUSL_INC_PATH}")
```

定位：myinitramfs/CMakeLists.txt:46-46。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E027 · build init目标入口

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myinit)
```

定位：myinitramfs/CMakeLists.txt:47-47。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E028 · build myshell目标入口

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myshell)
```

定位：myinitramfs/CMakeLists.txt:48-48。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E029 · build sh目标

[VERIFIED myinitramfs/myshell/CMakeLists.txt::add_executable]
```cmake
add_executable(sh ${SHELL_C_SRCS} ${SHELL_ASM_SRCS})
```

定位：myinitramfs/myshell/CMakeLists.txt:4-4。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E030 · time 活动内核C选项

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

定位：mykernel/scripts/options_flags.cmake:39-43。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E031 · time 活动内核CONFIG选项

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

定位：mykernel/scripts/options_flags.cmake:58-62。本证据对相邻调用的真实拥有者和返回表达式提供源码依据。

## SC125-E032 · 字符串C源GLOB活动命令

[VERIFIED myinitramfs/mylib/myglibc/string/CMakeLists.txt::file]
```cmake
file(GLOB LIBC_STRING_C_SRCS *.c)
```

定位：myinitramfs/mylib/myglibc/string/CMakeLists.txt:1-1。C源候选收集，不自动证明最终链接。

## SC125-E033 · 字符串汇编源GLOB活动命令

[VERIFIED myinitramfs/mylib/myglibc/string/CMakeLists.txt::file]
```cmake
file(GLOB LIBC_STRING_ASM_SRCS *.S)
```

定位：myinitramfs/mylib/myglibc/string/CMakeLists.txt:2-2。仅源码收集规则。

## SC125-E034 · initramfs编译器活动命令

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)
```

定位：myinitramfs/CMakeLists.txt:41-41。CMake文本，不等于观察实际sh链接。

## 普通定位（非定义体短窗、不得充作P2证据）

- `strcpy.c:21–25`为默认STRCPY名称；`strcat.c:20–24`为默认STRCAT名称。外部宏改名及真实链接导出符号未证。
- `myinitramfs/mylib/myglibc/string/string.h:139,144,149,405`分别有strcpy/strncpy/strcat/strlen公共声明；`myinitramfs/mylib/myglibc/include/string.h:62`包含该公共头。
- `myinitramfs/mylib/myglibc/string/CMakeLists.txt:1–3`收集*.c/*.S；静态mylibc与initramfs的musl-gcc路径是不同构建来源；`myinitramfs/myshell/CMakeLists.txt:4`建立sh目标而无所见直接mylibc链接声明。
- time代码金丝雀：`mykernel/debug/panic.c::panic:79`的`this_cpu = smp_processor_id();`；直接master同符号为`raw_smp_processor_id()`。
- C08已裁定124的PASS_PENDING_LOCAL，并文档关闭R123-01；本批无D00返修。源范围止于cat_command第270行的open调用请求。
