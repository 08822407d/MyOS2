---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LIBC-ALIGNED-WORD-COPY-127
record_type: pinned_code_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-19
produced_by: "GPT-6（会话自报，后台和推理档UNATTESTED）"
model_per_owner: "GPT-6＋Extra（178号任务书指定）"
date: 2026-11-22
date_timezone: Asia/Shanghai
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
inputs_read:
  - "myinitramfs/mylib/myglibc/string/wordcopy.c"
  - "myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h"
  - "myinitramfs/mylib/myglibc/string/memcpy.c"
  - "mykernel/scripts/options_flags.cmake"
  - "mykernel/debug/panic.c"
---

# 第127批新增源码定义体短窗

本批取自 time@a039d9803ade（GitHub对象原样短12）。每段为单一函数、宏定义或活动内核CMake配置中连续1–5个物理行，保留缩进与内容。宏入口选择与真正C函数调用分开；本证据只说明仓库固定源码，未预处理或编译。旧126补正由主报告和机器件记录，绝不将126历史前缀纳入本批新P6计数。

## WC127-E001 · helper入口按op_t整字计数并switch

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
void WORDCOPY_FWD_ALIGNED(long int dstp, long int srcp, size_t len)
{
	op_t a0, a1;

	switch (len % 8)
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:32-36；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E002 · case2预取a0和源游标负偏移

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	case 2:
		a0 = ((op_t *)srcp)[0];
		srcp -= 6 * OPSIZ;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:38-40；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E003 · case2目的游标调整len加6和do1

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
		dstp -= 7 * OPSIZ;
		len += 6;
		goto do1;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:41-43；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E004 · case3预取a1及目标游标

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	case 3:
		a1 = ((op_t *)srcp)[0];
		srcp -= 5 * OPSIZ;
		dstp -= 6 * OPSIZ;
		len += 5;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:44-48；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E005 · case3进入do2

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
		goto do2;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:49-49；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E006 · case4预取a0并调游标

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	case 4:
		a0 = ((op_t *)srcp)[0];
		srcp -= 4 * OPSIZ;
		dstp -= 5 * OPSIZ;
		len += 4;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:50-54；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E007 · case4进入do3

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
		goto do3;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:55-55；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E008 · case5预取a1并调游标

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	case 5:
		a1 = ((op_t *)srcp)[0];
		srcp -= 3 * OPSIZ;
		dstp -= 4 * OPSIZ;
		len += 3;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:56-60；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E009 · case5进入do4

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
		goto do4;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:61-61；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E010 · case6预取a0并调游标

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	case 6:
		a0 = ((op_t *)srcp)[0];
		srcp -= 2 * OPSIZ;
		dstp -= 3 * OPSIZ;
		len += 2;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:62-66；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E011 · case6进入do5

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
		goto do5;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:67-67；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E012 · case7预取a1并调游标

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	case 7:
		a1 = ((op_t *)srcp)[0];
		srcp -= 1 * OPSIZ;
		dstp -= 2 * OPSIZ;
		len += 1;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:68-72；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E013 · case7进入do6

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
		goto do6;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:73-73；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E014 · case0零字直接返回与非零预取

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	case 0:
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			return;
		a0 = ((op_t *)srcp)[0];
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:75-78；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E015 · case0进入do7

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
		srcp -= 0 * OPSIZ;
		dstp -= 1 * OPSIZ;
		goto do7;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:79-81；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E016 · case1预取a1和len减1

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	case 1:
		a1 = ((op_t *)srcp)[0];
		srcp -= -1 * OPSIZ;
		dstp -= 0 * OPSIZ;
		len -= 1;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:82-86；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E017 · case1零剩余直达do0或do8

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			goto do0;
		goto do8; /* No-op.  */
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:87-89；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E018 · 循环首对读a0写先前a1

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	do
	{
	do8:
		a0 = ((op_t *)srcp)[0];
		((op_t *)dstp)[0] = a1;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:92-96；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E019 · do7读a1写先前a0

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	do7:
		a1 = ((op_t *)srcp)[1];
		((op_t *)dstp)[1] = a0;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:97-99；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E020 · do6读a0写先前a1

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	do6:
		a0 = ((op_t *)srcp)[2];
		((op_t *)dstp)[2] = a1;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:100-102；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E021 · do5读a1写先前a0

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	do5:
		a1 = ((op_t *)srcp)[3];
		((op_t *)dstp)[3] = a0;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:103-105；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E022 · do4读a0写先前a1

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	do4:
		a0 = ((op_t *)srcp)[4];
		((op_t *)dstp)[4] = a1;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:106-108；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E023 · do3读a1写先前a0

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	do3:
		a1 = ((op_t *)srcp)[5];
		((op_t *)dstp)[5] = a0;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:109-111；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E024 · do2读a0写先前a1

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	do2:
		a0 = ((op_t *)srcp)[6];
		((op_t *)dstp)[6] = a1;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:112-114；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E025 · do1读a1写先前a0

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	do1:
		a1 = ((op_t *)srcp)[7];
		((op_t *)dstp)[7] = a0;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:115-117；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E026 · 八字推进游标并按len回环

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
		srcp += 8 * OPSIZ;
		dstp += 8 * OPSIZ;
		len -= 8;
	} while (len != 0);
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:119-122；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E027 · 独立do0最终写a1并void返回

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	/* This is the right position for do0.  Please don't move
	   it into the loop.  */
do0:
	((op_t *)dstp)[0] = a1;
}
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:124-128；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E028 · op_t为unsigned long int

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::op_t]
```c
	#define op_t unsigned long int
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:63-63；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E029 · OPSIZ取op_t字宽

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::OPSIZ]
```c
	#define OPSIZ (sizeof(op_t))
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:64-64；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E030 · 字节宏和私有计数

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::BYTE_COPY_FWD]
```c
	#define BYTE_COPY_FWD(dst_bp, src_bp, nbytes) \
		do                                        \
		{                                         \
			size_t __nbytes = (nbytes);           \
			while (__nbytes > 0)                  \
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:78-82；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E031 · 先读源再推进源后写目的及推进目的

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::BYTE_COPY_FWD]
```c
				byte __x = ((byte *)src_bp)[0];   \
				src_bp += 1;                      \
				__nbytes -= 1;                    \
				((byte *)dst_bp)[0] = __x;        \
				dst_bp += 1;                      \
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:84-88；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E032 · 对齐源选择wordcopy调用并将字节数除以OPSIZ

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::WORD_COPY_FWD]
```c
	#define WORD_COPY_FWD(dst_bp, src_bp, nbytes_left, nbytes)                \
		do                                                                    \
		{                                                                     \
			if (src_bp % OPSIZ == 0)                                          \
				_wordcopy_fwd_aligned(dst_bp, src_bp, (nbytes) / OPSIZ);      \
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:121-125；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E033 · 非对齐分支及外层两游标与尾数

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::WORD_COPY_FWD]
```c
			else                                                              \
				_wordcopy_fwd_dest_aligned(dst_bp, src_bp, (nbytes) / OPSIZ); \
			src_bp += (nbytes) & -OPSIZ;                                      \
			dst_bp += (nbytes) & -OPSIZ;                                      \
			(nbytes_left) = (nbytes) % OPSIZ;                                 \
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:126-130；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E034 · 选定页面宏空分支

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::PAGE_COPY_FWD_MAYBE]
```c
		#define PAGE_COPY_FWD_MAYBE(dstp, srcp, nbytes_left, nbytes) /* nada */
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:194-194；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E035 · 进入整字分支阈值16

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::OP_T_THRES]
```c
	#define OP_T_THRES 16
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:199-199；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E036 · 外层原参数与局部指针

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
MEMCPY(void *dstpp, const void *srcpp, size_t len)
{
	unsigned long int dstp = (long int)dstpp;
	unsigned long int srcp = (long int)srcpp;
```

固定行：myinitramfs/mylib/myglibc/string/memcpy.c:28-31；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E037 · 外层阈值判断与对齐前导门

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	/* If there not too few bytes to copy, use word copy.  */
	if (len >= OP_T_THRES)
	{
		/* Copy just a few bytes to make DSTP aligned.  */
```

固定行：myinitramfs/mylib/myglibc/string/memcpy.c:35-38；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E038 · 外层前导字节宏的位置

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
		len -= (-dstp) % OPSIZ;
		BYTE_COPY_FWD(dstp, srcp, (-dstp) % OPSIZ);
```

固定行：myinitramfs/mylib/myglibc/string/memcpy.c:39-40；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E039 · 外层空PAGE宏位置

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
		PAGE_COPY_FWD_MAYBE(dstp, srcp, len, len);
```

固定行：myinitramfs/mylib/myglibc/string/memcpy.c:45-45；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E040 · 外层WORD宏位置

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
		WORD_COPY_FWD(dstp, srcp, len, len);
```

固定行：myinitramfs/mylib/myglibc/string/memcpy.c:51-51；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E041 · 外层尾字节宏及返回原始dstpp

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	/* There are just a few bytes to copy.  Use byte memory operations.  */
	BYTE_COPY_FWD(dstp, srcp, len);

	return dstpp;
```

固定行：myinitramfs/mylib/myglibc/string/memcpy.c:56-59；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E042 · time活动CMAKE_C_FLAGS内核选项

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

固定行：mykernel/scripts/options_flags.cmake:39-43；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E043 · time活动CONFIG内核宏

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

固定行：mykernel/scripts/options_flags.cmake:58-62；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## WC127-E044 · time panic分支金丝雀

[VERIFIED mykernel/debug/panic.c::panic]
```c
	this_cpu = smp_processor_id();
```

固定行：mykernel/debug/panic.c:79-79；该行界仅用于人工导航，定义及来源以标签内路径和原文为准。

## 普通定位（不作为本批正式核证短窗）

- `wordcopy.c:21–30`包含通用头及`WORDCOPY_FWD_ALIGNED`默认符号定义；外部编译宏可能更改符号名称，实际导出符号尚未核实。
- `memcopy.h:119–120`是两个wordcopy helper的函数声明，`memcopy.h:165,192–196`区分页面宏的条件；正式模型选择该文件内空页面宏分支。没有读取额外外部pagecopy头以证实真实预处理选择。
- 用户库的构建来源：`myinitramfs/mylib/myglibc/string/CMakeLists.txt:1–3`、`myinitramfs/mylib/myglibc/CMakeLists.txt:1,20`、`myinitramfs/CMakeLists.txt:40–51`及`myinitramfs/myshell/CMakeLists.txt:4`。它们只属源码收集、CMake文本与目标定义的普通位置，无法证明真实sh使用本批模型函数。
- 旧126原图及主线审查为历史来源；127的D00更正图重建基于原126 Git Blob与固定边、C08回执及独立数据复算，不更改原旧文件。
