---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-LIBC-STRING-LENGTH-COPY-124
record_type: source_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-19
produced_by: "GPT-6（会话自报；实际effort UNATTESTED）"
date: 2026-11-22
base_snapshot: "time（分支名）"
read_channel: connector
inputs_read:
  - "myinitramfs/mylib/myglibc/string/strlen.c"
  - "myinitramfs/mylib/myglibc/string/strnlen.c"
  - "myinitramfs/mylib/myglibc/string/strncpy.c"
  - "myinitramfs/mylib/myglibc/string/strcmp.c"
  - "myinitramfs/mylib/myglibc/string/memcpy.c"
  - "myinitramfs/mylib/myglibc/string/memset.c"
  - "myinitramfs/myshell/myshell.c"
  - "myinitramfs/myshell/CMakeLists.txt"
  - "myinitramfs/mylib/myglibc/string/string.h"
  - "myinitramfs/mylib/myglibc/include/string.h"
  - "myinitramfs/mylib/myglibc/string/CMakeLists.txt"
  - "myinitramfs/mylib/myglibc/CMakeLists.txt"
  - "myinitramfs/CMakeLists.txt"
  - "mykernel/scripts/options_flags.cmake"
  - "mykernel/debug/panic.c"
status: READY_FOR_MAINLINE_REVIEW
---
# 第124批独立源码锚

全部定义体和CMake活动命令直接读取自time@a039d9803ade（短12）。短窗均为连续1–5物理行，保留源文本原貌。未将123旧引文当作本批新核证。

## SC124-E001 查找比较及索引返回

[VERIFIED myinitramfs/myshell/myshell.c::find_cmd]
```c
	for(i = 0;i<sizeof(shell_internal_cmd)/sizeof(builtincmd_s);i++)
		if(!strcmp(cmd,shell_internal_cmd[i].name))
			return i;
	return -1;
```

固定源码行148–151，该语句属于find_cmd的定义或活动命令。

## SC124-E002 cd比较和chdir请求

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	/////.
	if(!strcmp(".", path))
		return 1;

	i = chdir(path);
```

固定源码行199–203，该语句属于cd_command的定义或活动命令。

## SC124-E003 readdir退出条件

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
	while(1)
	{
		buf = readdir(dir);
		if(buf == NULL)
			break;
```

固定源码行231–235，该语句属于ls_command的定义或活动命令。

## SC124-E004 ls的两个strcmp及名称输出

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
		if (strcmp(buf->d_name, ".") != 0 &&
			strcmp(buf->d_name, "..") != 0)
			printf("%s\t", buf->d_name);
```

固定源码行236–238，该语句属于ls_command的定义或活动命令。

## SC124-E005 循环之后换行和关闭

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
	}
	printf("\n");
	closedir(dir);
	return 1;
```

固定源码行239–242，该语句属于ls_command的定义或活动命令。

## SC124-E006 两个strlen长度来源

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	len = strlen(current_dir);
	i = len + strlen(argv[1]);
	filename = malloc(i + 2);
	memset(filename, 0, i + 2);
```

固定源码行260–263，该语句属于cat_command的定义或活动命令。

## SC124-E007 strcmp先行NUL出口

[VERIFIED myinitramfs/mylib/myglibc/string/strcmp.c::STRCMP]
```c
		c1 = (unsigned char)*s1++;
		c2 = (unsigned char)*s2++;
		if (c1 == '\0')
			return c1 - c2;
```

固定源码行37–40，该语句属于STRCMP的定义或活动命令。

## SC124-E008 strcmp差异出口

[VERIFIED myinitramfs/mylib/myglibc/string/strcmp.c::STRCMP]
```c
	} while (c1 == c2);

	return c1 - c2;
```

固定源码行41–43，该语句属于STRCMP的定义或活动命令。

## SC124-E009 strlen前导对齐和字节停止

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
	for (char_ptr = str; ((unsigned long int)char_ptr & (sizeof(longword) - 1)) != 0;
		 ++char_ptr)
		if (*char_ptr == '\0')
			return char_ptr - str;
```

固定源码行38–41，该语句属于STRLEN的定义或活动命令。

## SC124-E010 strlen整字地址

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
	longword_ptr = (unsigned long int *)char_ptr;
```

固定源码行46–46，该语句属于STRLEN的定义或活动命令。

## SC124-E011 strlen掩码初始化

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
	himagic = 0x80808080L;
	lomagic = 0x01010101L;
	if (sizeof(longword) > 4)
	{
```

固定源码行57–60，该语句属于STRLEN的定义或活动命令。

## SC124-E012 strlen64位掩码

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
		himagic = ((himagic << 16) << 16) | himagic;
		lomagic = ((lomagic << 16) << 16) | lomagic;
```

固定源码行63–64，该语句属于STRLEN的定义或活动命令。

## SC124-E013 strlen整字读和三项筛选

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
	for (;;)
	{
		longword = *longword_ptr++;

		if (((longword - lomagic) & ~longword & himagic) != 0)
```

固定源码行72–76，该语句属于STRLEN的定义或活动命令。

## SC124-E014 strlen候选首字节

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
			const char *cp = (const char *)(longword_ptr - 1);

			if (cp[0] == 0)
				return cp - str;
```

固定源码行81–84，该语句属于STRLEN的定义或活动命令。

## SC124-E015 strlen候选次字节

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
			if (cp[1] == 0)
				return cp - str + 1;
			if (cp[2] == 0)
				return cp - str + 2;
```

固定源码行85–88，该语句属于STRLEN的定义或活动命令。

## SC124-E016 strlen后半门

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
			if (cp[3] == 0)
				return cp - str + 3;
			if (sizeof(longword) > 4)
			{
				if (cp[4] == 0)
```

固定源码行89–93，该语句属于STRLEN的定义或活动命令。

## SC124-E017 strlen高字节确认

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
				if (cp[5] == 0)
					return cp - str + 5;
				if (cp[6] == 0)
					return cp - str + 6;
```

固定源码行95–98，该语句属于STRLEN的定义或活动命令。

## SC124-E018 strlen最后字节

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
				if (cp[7] == 0)
					return cp - str + 7;
```

固定源码行99–100，该语句属于STRLEN的定义或活动命令。

## SC124-E019 __strnlen入口预算

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
__strnlen(const char *str, size_t maxlen)
{
	const char *char_ptr, *end_ptr = str + maxlen;
	const unsigned long int *longword_ptr;
	unsigned long int longword, himagic, lomagic;
```

固定源码行33–37，该语句属于__strnlen的定义或活动命令。

## SC124-E020 零预算与端点

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
	if (maxlen == 0)
		return 0;

	if (end_ptr < str)
		end_ptr = (const char *)~0UL;
```

固定源码行39–43，该语句属于__strnlen的定义或活动命令。

## SC124-E021 未对齐字节扫描

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
	for (char_ptr = str; ((unsigned long int)char_ptr & (sizeof(longword) - 1)) != 0;
		 ++char_ptr)
		if (*char_ptr == '\0')
```

固定源码行47–49，该语句属于__strnlen的定义或活动命令。

## SC124-E022 前导NUL夹取

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
		{
			if (char_ptr > end_ptr)
				char_ptr = end_ptr;
			return char_ptr - str;
```

固定源码行50–53，该语句属于__strnlen的定义或活动命令。

## SC124-E023 __strnlen掩码初始化

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
	himagic = 0x80808080L;
	lomagic = 0x01010101L;
	if (sizeof(longword) > 4)
```

固定源码行70–72，该语句属于__strnlen的定义或活动命令。

## SC124-E024 64位掩码扩展

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
		himagic = ((himagic << 16) << 16) | himagic;
		lomagic = ((lomagic << 16) << 16) | lomagic;
```

固定源码行76–77，该语句属于__strnlen的定义或活动命令。

## SC124-E025 整字循环门

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
	while (longword_ptr < (unsigned long int *)end_ptr)
	{
```

固定源码行85–86，该语句属于__strnlen的定义或活动命令。

## SC124-E026 两项筛选与整字加载

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
		longword = *longword_ptr++;

		if ((longword - lomagic) & himagic)
		{
```

固定源码行116–119，该语句属于__strnlen的定义或活动命令。

## SC124-E027 候选字节0

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
			const char *cp = (const char *)(longword_ptr - 1);

			char_ptr = cp;
			if (cp[0] == 0)
				break;
```

固定源码行123–127，该语句属于__strnlen的定义或活动命令。

## SC124-E028 候选字节1

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
			char_ptr = cp + 1;
			if (cp[1] == 0)
				break;
```

固定源码行128–130，该语句属于__strnlen的定义或活动命令。

## SC124-E029 候选字节2/3

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
			char_ptr = cp + 2;
			if (cp[2] == 0)
				break;
			char_ptr = cp + 3;
			if (cp[3] == 0)
```

固定源码行131–135，该语句属于__strnlen的定义或活动命令。

## SC124-E030 候选字节4

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
			if (sizeof(longword) > 4)
			{
				char_ptr = cp + 4;
				if (cp[4] == 0)
```

固定源码行137–140，该语句属于__strnlen的定义或活动命令。

## SC124-E031 候选字节5/6

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
				char_ptr = cp + 5;
				if (cp[5] == 0)
					break;
				char_ptr = cp + 6;
				if (cp[6] == 0)
```

固定源码行142–146，该语句属于__strnlen的定义或活动命令。

## SC124-E032 候选字节7

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
				char_ptr = cp + 7;
				if (cp[7] == 0)
					break;
```

固定源码行148–150，该语句属于__strnlen的定义或活动命令。

## SC124-E033 循环继续时端点

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
		char_ptr = end_ptr;
	}
```

固定源码行153–154，该语句属于__strnlen的定义或活动命令。

## SC124-E034 末尾长度夹取

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
	if (char_ptr > end_ptr)
		char_ptr = end_ptr;
	return char_ptr - str;
```

固定源码行156–158，该语句属于__strnlen的定义或活动命令。

## SC124-E035 strncpy长度调用及守卫

[VERIFIED myinitramfs/mylib/myglibc/string/strncpy.c::STRNCPY]
```c
STRNCPY(char *s1, const char *s2, size_t n)
{
	size_t size = __strnlen(s2, n);
	if (size != n)
```

固定源码行27–30，该语句属于STRNCPY的定义或活动命令。

## SC124-E036 strncpy补零与memcpy返回

[VERIFIED myinitramfs/mylib/myglibc/string/strncpy.c::STRNCPY]
```c
	if (size != n)
		memset(s1 + size, '\0', n - size);
	return memcpy(s1, s2, size);
}
```

固定源码行30–33，该语句属于STRNCPY的定义或活动命令。

## SC124-E037 memcpy尾部与返回dst

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	/* There are just a few bytes to copy.  Use byte memory operations.  */
	BYTE_COPY_FWD(dstp, srcp, len);

	return dstpp;
```

固定源码行56–59，该语句属于MEMCPY的定义或活动命令。

## SC124-E038 memset短尾写入

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
	/* Write the last few bytes.  */
	while (len > 0)
	{
		((byte *)dstp)[0] = c;
```

固定源码行75–78，该语句属于memset的定义或活动命令。

## SC124-E039 memset循环结束及返回

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
		dstp += 1;
		len -= 1;
	}

	return dstpp;
```

固定源码行79–83，该语句属于memset的定义或活动命令。

## SC124-E040 string目标源码收集

[VERIFIED myinitramfs/mylib/myglibc/string/CMakeLists.txt::target_sources]
```cmake
target_sources(mylibc PUBLIC ${LIBC_STRING_C_SRCS} ${LIBC_STRING_ASM_SRCS})
```

固定源码行3–3，该语句属于target_sources的定义或活动命令。

## SC124-E041 静态mylibc目标

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_library]
```cmake
add_library(mylibc STATIC)
```

固定源码行1–1，该语句属于add_library的定义或活动命令。

## SC124-E042 字符串库目录

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(string)
```

固定源码行20–20，该语句属于add_subdirectory的定义或活动命令。

## SC124-E043 musl路径配置

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(MUSL_PATH /home/cheyh/projs/musl/build)
```

固定源码行40–40，该语句属于set的定义或活动命令。

## SC124-E044 musl-gcc路径文本

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)
```

固定源码行41–41，该语句属于set的定义或活动命令。

## SC124-E045 init子目录

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myinit)
```

固定源码行47–47，该语句属于add_subdirectory的定义或活动命令。

## SC124-E046 myshell子目录

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myshell)
```

固定源码行48–48，该语句属于add_subdirectory的定义或活动命令。

## SC124-E047 time内核C flags

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

固定源码行39–43，该语句属于CMAKE_C_FLAGS的定义或活动命令。

## SC124-E048 time内核CONFIG

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

固定源码行58–62，该语句属于CMAKE_C_FLAGS的定义或活动命令。

## 非定义体的普通来源定位

- 源码默认别名：strlen.c:21–25 STRLEN；strnlen.c:28–30 外部STRNLEN条件改写__strnlen；strncpy.c:20–24 STRNCPY。
- 公共声明：myinitramfs/mylib/myglibc/string/string.h:144 strncpy、:405 strlen、:412 strnlen；内部声明：myinitramfs/mylib/myglibc/include/string.h:13 __strnlen，包装头:62引入string/string.h。
- 构建：string/CMakeLists.txt:1–3收集*.c/*.S；myglibc/CMakeLists.txt:1,20；initramfs/CMakeLists.txt:40–51；myshell/CMakeLists.txt:4–10。不能从源目录存在推导出真实sh已链接该库。
- D00原始证据：123固定六件、C08的CORE-USER-LIBC-STRING-COMPARE-123-review.md与CORE-USER-LIBC-STRING-COMPARE-123-D00-122-review.md。原图仅被定点追加补正，不改旧件。
