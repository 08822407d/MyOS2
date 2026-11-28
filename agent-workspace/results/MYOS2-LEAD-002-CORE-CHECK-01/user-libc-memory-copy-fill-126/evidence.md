---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LIBC-MEMORY-COPY-FILL-126
record_type: pinned_function_and_macro_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-19
produced_by: "GPT-6（会话可知；后台型号与推理档 UNATTESTED）"
model_per_owner: "GPT-6＋Extra（177号指定）"
date: 2026-11-22
date_timezone: Asia/Shanghai
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
inputs_read:
  - "myinitramfs/mylib/myglibc/string/memcpy.c"
  - "myinitramfs/mylib/myglibc/string/memset.c"
  - "myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h"
  - "myinitramfs/mylib/myglibc/string/strcpy.c"
  - "myinitramfs/mylib/myglibc/string/strcat.c"
  - "myinitramfs/myshell/myshell.c"
  - "mykernel/scripts/options_flags.cmake"
  - "mykernel/debug/panic.c"
---
# 第126批独立源函数和宏短窗

每个SC126短窗均从本轮直接读取的time固定源码复制，物理行连续且不超过5行。仅覆盖同一函数体或宏定义内的代码；用户态构建、公共声明和历史125普通定位均不列为本批正式短窗。未预处理或编译；宏解析按给定通用头作为条件性纸面模型。

## SC126-E001 · A 原参数与本地游标初始化

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
MEMCPY(void *dstpp, const void *srcpp, size_t len)
{
	unsigned long int dstp = (long int)dstpp;
	unsigned long int srcp = (long int)srcpp;
```

固定位置：myinitramfs/mylib/myglibc/string/memcpy.c:28-31；证据限于所选源码快照的实际定义或活动命令。

## SC126-E002 · A 长复制进入阈值判定

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	/* If there not too few bytes to copy, use word copy.  */
	if (len >= OP_T_THRES)
	{
		/* Copy just a few bytes to make DSTP aligned.  */
```

固定位置：myinitramfs/mylib/myglibc/string/memcpy.c:35-38；证据限于所选源码快照的实际定义或活动命令。

## SC126-E003 · A 目标对齐字节宏及剩余量扣减

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
		len -= (-dstp) % OPSIZ;
		BYTE_COPY_FWD(dstp, srcp, (-dstp) % OPSIZ);
```

固定位置：myinitramfs/mylib/myglibc/string/memcpy.c:39-40；证据限于所选源码快照的实际定义或活动命令。

## SC126-E004 · A 长分支PAGE宏调用

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
		/* Copy whole pages from SRCP to DSTP by virtual address manipulation,
	   as much as possible.  */

		PAGE_COPY_FWD_MAYBE(dstp, srcp, len, len);
```

固定位置：myinitramfs/mylib/myglibc/string/memcpy.c:42-45；证据限于所选源码快照的实际定义或活动命令。

## SC126-E005 · A WORD宏调用

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c

		WORD_COPY_FWD(dstp, srcp, len, len);
```

固定位置：myinitramfs/mylib/myglibc/string/memcpy.c:50-51；证据限于所选源码快照的实际定义或活动命令。

## SC126-E006 · A 短尾宏及原始指针返回

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	/* There are just a few bytes to copy.  Use byte memory operations.  */
	BYTE_COPY_FWD(dstp, srcp, len);

	return dstpp;
```

固定位置：myinitramfs/mylib/myglibc/string/memcpy.c:56-59；证据限于所选源码快照的实际定义或活动命令。

## SC126-E007 · A 使用的unsigned long整字类型

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::op_t]
```c
	#define op_t unsigned long int
```

固定位置：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:63-63；证据限于所选源码快照的实际定义或活动命令。

## SC126-E008 · A 类型字宽计算

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::OPSIZ]
```c
	#define OPSIZ (sizeof(op_t))
```

固定位置：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:64-64；证据限于所选源码快照的实际定义或活动命令。

## SC126-E009 · A BYTE类型

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::byte]
```c
	typedef unsigned char byte;
```

固定位置：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:67-67；证据限于所选源码快照的实际定义或活动命令。

## SC126-E010 · A 字节宏名及nbytes私有计数

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::BYTE_COPY_FWD]
```c
	#define BYTE_COPY_FWD(dst_bp, src_bp, nbytes) \
		do                                        \
		{                                         \
			size_t __nbytes = (nbytes);           \
			while (__nbytes > 0)                  \
```

固定位置：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:78-82；证据限于所选源码快照的实际定义或活动命令。

## SC126-E011 · A 每轮源读取与源游标前进

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::BYTE_COPY_FWD]
```c
			{                                     \
				byte __x = ((byte *)src_bp)[0];   \
				src_bp += 1;                      \
				__nbytes -= 1;                    \
				((byte *)dst_bp)[0] = __x;        \
```

固定位置：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:83-87；证据限于所选源码快照的实际定义或活动命令。

## SC126-E012 · A 每轮目的写入与目的游标前进

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::BYTE_COPY_FWD]
```c
				__nbytes -= 1;                    \
				((byte *)dst_bp)[0] = __x;        \
				dst_bp += 1;                      \
			}                                     \
		} while (0)
```

固定位置：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:86-90；证据限于所选源码快照的实际定义或活动命令。

## SC126-E013 · A 整字宏条件选择helper

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::WORD_COPY_FWD]
```c
	#define WORD_COPY_FWD(dst_bp, src_bp, nbytes_left, nbytes)                \
		do                                                                    \
		{                                                                     \
			if (src_bp % OPSIZ == 0)                                          \
				_wordcopy_fwd_aligned(dst_bp, src_bp, (nbytes) / OPSIZ);      \
```

固定位置：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:121-125；证据限于所选源码快照的实际定义或活动命令。

## SC126-E014 · A helper参数是字数及源游标更新

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::WORD_COPY_FWD]
```c
			if (src_bp % OPSIZ == 0)                                          \
				_wordcopy_fwd_aligned(dst_bp, src_bp, (nbytes) / OPSIZ);      \
			else                                                              \
				_wordcopy_fwd_dest_aligned(dst_bp, src_bp, (nbytes) / OPSIZ); \
			src_bp += (nbytes) & -OPSIZ;                                      \
```

固定位置：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:124-128；证据限于所选源码快照的实际定义或活动命令。

## SC126-E015 · A helper返回后两游标与len尾数

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::WORD_COPY_FWD]
```c
				_wordcopy_fwd_dest_aligned(dst_bp, src_bp, (nbytes) / OPSIZ); \
			src_bp += (nbytes) & -OPSIZ;                                      \
			dst_bp += (nbytes) & -OPSIZ;                                      \
			(nbytes_left) = (nbytes) % OPSIZ;                                 \
		} while (0)
```

固定位置：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:127-131；证据限于所选源码快照的实际定义或活动命令。

## SC126-E016 · A 选定空PAGE宏

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::PAGE_COPY_FWD_MAYBE]
```c
		#define PAGE_COPY_FWD_MAYBE(dstp, srcp, nbytes_left, nbytes) /* nada */
```

固定位置：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:194-194；证据限于所选源码快照的实际定义或活动命令。

## SC126-E017 · A 阈值16

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::OP_T_THRES]
```c
	#define OP_T_THRES 16
```

固定位置：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:199-199；证据限于所选源码快照的实际定义或活动命令。

## SC126-E018 · B 参数、本地游标和长分支守卫

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
memset(void *dstpp, int c, size_t len)
{
	long int dstp = (long int)dstpp;

	if (len >= 8)
```

固定位置：myinitramfs/mylib/myglibc/string/memset.c:22-26；证据限于所选源码快照的实际定义或活动命令。

## SC126-E019 · B 填充值低字节及重复过程开始

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
		size_t xlen;
		op_t cccc;

		cccc = (unsigned char)c;
		cccc |= cccc << 8;
```

固定位置：myinitramfs/mylib/myglibc/string/memset.c:28-32；证据限于所选源码快照的实际定义或活动命令。

## SC126-E020 · B 重复字的64位扩展

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
		cccc |= cccc << 8;
		cccc |= cccc << 16;
		if (OPSIZ > 4)
			/* Do the shift in two steps to avoid warning if long has 32 bits.  */
			cccc |= (cccc << 16) << 16;
```

固定位置：myinitramfs/mylib/myglibc/string/memset.c:32-36；证据限于所选源码快照的实际定义或活动命令。

## SC126-E021 · B 未对齐前导逐字节写

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
		while (dstp % OPSIZ != 0)
		{
			((byte *)dstp)[0] = c;
			dstp += 1;
			len -= 1;
```

固定位置：myinitramfs/mylib/myglibc/string/memset.c:40-44；证据限于所选源码快照的实际定义或活动命令。

## SC126-E022 · B 八词组计数与循环入口

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
		/* Write 8 `op_t' per iteration until less than 8 `op_t' remain.  */
		xlen = len / (OPSIZ * 8);
		while (xlen > 0)
		{
			((op_t *)dstp)[0] = cccc;
```

固定位置：myinitramfs/mylib/myglibc/string/memset.c:47-51；证据限于所选源码快照的实际定义或活动命令。

## SC126-E023 · B 八词组中间四字

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
			((op_t *)dstp)[1] = cccc;
			((op_t *)dstp)[2] = cccc;
			((op_t *)dstp)[3] = cccc;
			((op_t *)dstp)[4] = cccc;
			((op_t *)dstp)[5] = cccc;
```

固定位置：myinitramfs/mylib/myglibc/string/memset.c:52-56；证据限于所选源码快照的实际定义或活动命令。

## SC126-E024 · B 八词组末尾及游标跨64字节

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
			((op_t *)dstp)[4] = cccc;
			((op_t *)dstp)[5] = cccc;
			((op_t *)dstp)[6] = cccc;
			((op_t *)dstp)[7] = cccc;
			dstp += 8 * OPSIZ;
```

固定位置：myinitramfs/mylib/myglibc/string/memset.c:55-59；证据限于所选源码快照的实际定义或活动命令。

## SC126-E025 · B 八词组结束后的len剩余

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
			dstp += 8 * OPSIZ;
			xlen -= 1;
		}
		len %= OPSIZ * 8;
```

固定位置：myinitramfs/mylib/myglibc/string/memset.c:59-62；证据限于所选源码快照的实际定义或活动命令。

## SC126-E026 · B 单词循环和写入

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
		/* Write 1 `op_t' per iteration until less than OPSIZ bytes remain.  */
		xlen = len / OPSIZ;
		while (xlen > 0)
		{
			((op_t *)dstp)[0] = cccc;
```

固定位置：myinitramfs/mylib/myglibc/string/memset.c:64-68；证据限于所选源码快照的实际定义或活动命令。

## SC126-E027 · B 单词后的游标和尾数

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
			dstp += OPSIZ;
			xlen -= 1;
		}
		len %= OPSIZ;
```

固定位置：myinitramfs/mylib/myglibc/string/memset.c:69-72；证据限于所选源码快照的实际定义或活动命令。

## SC126-E028 · B 短尾逐字节写

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
	/* Write the last few bytes.  */
	while (len > 0)
	{
		((byte *)dstp)[0] = c;
		dstp += 1;
```

固定位置：myinitramfs/mylib/myglibc/string/memset.c:75-79；证据限于所选源码快照的实际定义或活动命令。

## SC126-E029 · B 每次尾字节递增和原指针返回

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
		dstp += 1;
		len -= 1;
	}

	return dstpp;
```

固定位置：myinitramfs/mylib/myglibc/string/memset.c:79-83；证据限于所选源码快照的实际定义或活动命令。

## SC126-E030 · C shell两长度与初始化请求

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	len = strlen(current_dir);
	i = len + strlen(argv[1]);
	filename = malloc(i + 2);
	memset(filename, 0, i + 2);
```

固定位置：myinitramfs/myshell/myshell.c:260-263；证据限于所选源码快照的实际定义或活动命令。

## SC126-E031 · C 清零复制条件斜杠及追加

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	memset(filename, 0, i + 2);
	strcpy(filename, current_dir);
	if(len > 1)
		filename[len] = '/';
	strcat(filename, argv[1]);
```

固定位置：myinitramfs/myshell/myshell.c:263-267；证据限于所选源码快照的实际定义或活动命令。

## SC126-E032 · C 追加后最终open调用

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	strcat(filename, argv[1]);
	// printf("cat_command filename:%s\n", filename);

	fd = open(filename, O_RDONLY, 0);	
```

固定位置：myinitramfs/myshell/myshell.c:267-270；证据限于所选源码快照的实际定义或活动命令。

## SC126-E033 · C 内层源长加一和MEMCPY请求

[VERIFIED myinitramfs/mylib/myglibc/string/strcpy.c::STRCPY]
```c
STRCPY(char *dest, const char *src)
{
	return memcpy(dest, src, strlen(src) + 1);
}
```

固定位置：myinitramfs/mylib/myglibc/string/strcpy.c:29-32；证据限于所选源码快照的实际定义或活动命令。

## SC126-E034 · C 内外两层函数返回归属

[VERIFIED myinitramfs/mylib/myglibc/string/strcat.c::STRCAT]
```c
STRCAT(char *dest, const char *src)
{
	strcpy(dest + strlen(dest), src);
	return dest;
}
```

固定位置：myinitramfs/mylib/myglibc/string/strcat.c:28-32；证据限于所选源码快照的实际定义或活动命令。

## SC126-E035 · time活动内核C选项与金丝雀

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

固定位置：mykernel/scripts/options_flags.cmake:39-43；证据限于所选源码快照的实际定义或活动命令。

## SC126-E036 · time活动CONFIG门

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

固定位置：mykernel/scripts/options_flags.cmake:58-62；证据限于所选源码快照的实际定义或活动命令。

## SC126-E037 · time panic金丝雀

[VERIFIED mykernel/debug/panic.c::panic]
```c
	this_cpu = smp_processor_id();
```

固定位置：mykernel/debug/panic.c:79-79；证据限于所选源码快照的实际定义或活动命令。

## 普通位置及前提说明（非正式源码短窗）

- 用户库源清单：`myinitramfs/mylib/myglibc/string/CMakeLists.txt:1–3`；静态`mylibc`：`myinitramfs/mylib/myglibc/CMakeLists.txt:1,20`；独立initramfs配置：`myinitramfs/CMakeLists.txt:40–51`；sh构建目标：`myinitramfs/myshell/CMakeLists.txt:4`。它们不是实际sh最终链接证明。
- `memcpy.c:20–25`的头文件包含及MEMCPY默认别名、`memset.c:18–19`包含链、`memcopy.h:42–44`通用头包含及pagecopy.h注释、`memcopy.h:165,192–196`条件分支，均为普通上下文定位。选用通用头且PAGE_COPY_THRESHOLD未从其它活动头设为非零时，空PAGE宏才是本次模型条件。
- 对字复制函数`_wordcopy_fwd_aligned`，本批只检该通用宏的声明与调用边界，并在正常返回、复制两字的给定前提下记录外层宏的游标更新；未审汇编指令或真实链接体。
- `myinitramfs/mylib/myglibc/string/string.h`中`memcpy`和`memset`的公开声明不替代上述函数定义体，实际sh链接的libc保持UNKNOWN。
- 历史R125-01/02从125固定原件重建为追加阅读视图，原MANIFEST、evidence和PR评论均不改动。
