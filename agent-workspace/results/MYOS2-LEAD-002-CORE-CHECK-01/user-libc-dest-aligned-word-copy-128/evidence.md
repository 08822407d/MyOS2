---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LIBC-DEST-ALIGNED-WORD-COPY-128
record_type: pinned_source_definition_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-19
produced_by: "GPT-6（本会话自述，后台与实际effort UNATTESTED）"
model_per_owner: "GPT-6＋Extra（179号指定）"
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
# 第128批固定源码短窗

每段逐字复制自 time@a039d9803ade（短12）；引文为指定函数、宏定义或活动内核配置语句中连续1至5个原物理行。重复窗口用于独立支撑分派、前导、MERGE和末尾写回，不构成运行证据。真实SH链接、预处理所选宏、CPU指令、编译、Graphviz、QEMU及实机均未经执行。

## WC128-E001 · 函数入口与四暂存字

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
void WORDCOPY_FWD_DEST_ALIGNED(long int dstp, long int srcp, size_t len)
{
	op_t a0, a1, a2, a3;
	int sh_1, sh_2;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:139-142。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E002 · 源地址低3位计算两个移位位数

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
	sh_1 = 8 * (srcp % OPSIZ);
	sh_2 = 8 * OPSIZ - sh_1;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:147-148。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E003 · 源指针下取整和四路余数选择

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
	/* Make SRCP aligned by rounding it down to the beginning of the `op_t'
	   it points in the middle of.  */
	srcp &= -OPSIZ;

	switch (len % 4)
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:150-154。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E004 · case2两个源整字预取

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
	case 2:
		a1 = ((op_t *)srcp)[0];
		a2 = ((op_t *)srcp)[1];
		srcp -= 1 * OPSIZ;
		dstp -= 3 * OPSIZ;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:156-160。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E005 · case2整数游标和do1

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
		srcp -= 1 * OPSIZ;
		dstp -= 3 * OPSIZ;
		len += 2;
		goto do1;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:159-162。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E006 · case3预取与源游标

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
	case 3:
		a0 = ((op_t *)srcp)[0];
		a1 = ((op_t *)srcp)[1];
		srcp -= 0 * OPSIZ;
		dstp -= 2 * OPSIZ;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:163-167。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E007 · case3调节与do2

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
		srcp -= 0 * OPSIZ;
		dstp -= 2 * OPSIZ;
		len += 1;
		goto do2;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:166-169。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E008 · case0空输入守卫及非空预读

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
	case 0:
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			return;
		a3 = ((op_t *)srcp)[0];
		a0 = ((op_t *)srcp)[1];
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:170-174。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E009 · case0两个预读后的跳转

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
		a0 = ((op_t *)srcp)[1];
		srcp -= -1 * OPSIZ;
		dstp -= 1 * OPSIZ;
		len += 0;
		goto do3;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:174-178。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E010 · case1两个源字与游标

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
	case 1:
		a2 = ((op_t *)srcp)[0];
		a3 = ((op_t *)srcp)[1];
		srcp -= -2 * OPSIZ;
		dstp -= 0 * OPSIZ;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:179-183。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E011 · case1一字直达do0或do4

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
		len -= 1;
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			goto do0;
		goto do4; /* No-op.  */
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:184-187。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E012 · do4读a0合成目的字

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
	do
	{
	do4:
		a0 = ((op_t *)srcp)[0];
		((op_t *)dstp)[0] = MERGE(a2, sh_1, a3, sh_2);
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:190-194。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E013 · 两种MERGE配对第1组

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
		a0 = ((op_t *)srcp)[0];
		((op_t *)dstp)[0] = MERGE(a2, sh_1, a3, sh_2);
	do3:
		a1 = ((op_t *)srcp)[1];
		((op_t *)dstp)[1] = MERGE(a3, sh_1, a0, sh_2);
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:193-197。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E014 · do3与do2的不同暂存配对

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
		a1 = ((op_t *)srcp)[1];
		((op_t *)dstp)[1] = MERGE(a3, sh_1, a0, sh_2);
	do2:
		a2 = ((op_t *)srcp)[2];
		((op_t *)dstp)[2] = MERGE(a0, sh_1, a1, sh_2);
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:196-200。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E015 · do2与do1的不同暂存配对

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
		a2 = ((op_t *)srcp)[2];
		((op_t *)dstp)[2] = MERGE(a0, sh_1, a1, sh_2);
	do1:
		a3 = ((op_t *)srcp)[3];
		((op_t *)dstp)[3] = MERGE(a1, sh_1, a2, sh_2);
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:199-203。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E016 · 四字迭代后更新及do/while守卫

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
		srcp += 4 * OPSIZ;
		dstp += 4 * OPSIZ;
		len -= 4;
	} while (len != 0);
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:205-208。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E017 · 循环外do0末写合成

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
	/* This is the right position for do0.  Please don't move
	   it into the loop.  */
do0:
	((op_t *)dstp)[0] = MERGE(a2, sh_1, a3, sh_2);
}
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:210-214。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E018 · 历史127入口case1先预读再调整

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	case 1:
		a1 = ((op_t *)srcp)[0];
		srcp -= -1 * OPSIZ;
		dstp -= 0 * OPSIZ;
		len -= 1;
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:82-86。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E019 · 历史127 case1显式goto次序

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			goto do0;
		goto do8; /* No-op.  */
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:87-89。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E020 · 历史127 do/while回边源

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
		srcp += 8 * OPSIZ;
		dstp += 8 * OPSIZ;
		len -= 8;
	} while (len != 0);
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:119-122。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E021 · 历史127 loop外do0末写

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	/* This is the right position for do0.  Please don't move
	   it into the loop.  */
do0:
	((op_t *)dstp)[0] = a1;
}
```

固定行：myinitramfs/mylib/myglibc/string/wordcopy.c:124-128。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E022 · 选定unsigned long整字类型

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::op_t]
```c
	#define op_t unsigned long int
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:63-63。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E023 · 整字字宽

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::OPSIZ]
```c
	#define OPSIZ (sizeof(op_t))
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:64-64。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E024 · 小端MERGE真实宏定义

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::MERGE]
```c
		#define MERGE(w0, sh_1, w1, sh_2) (((w0) >> (sh_1)) | ((w1) << (sh_2)))
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:70-70。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E025 · 字节宏计数与入口

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::BYTE_COPY_FWD]
```c
	#define BYTE_COPY_FWD(dst_bp, src_bp, nbytes) \
		do                                        \
		{                                         \
			size_t __nbytes = (nbytes);           \
			while (__nbytes > 0)                  \
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:78-82。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E026 · 字节宏源读目的写与两游标推进

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::BYTE_COPY_FWD]
```c
				byte __x = ((byte *)src_bp)[0];   \
				src_bp += 1;                      \
				__nbytes -= 1;                    \
				((byte *)dst_bp)[0] = __x;        \
				dst_bp += 1;                      \
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:84-88。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E027 · WORD宏按源对齐选择helper

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::WORD_COPY_FWD]
```c
	#define WORD_COPY_FWD(dst_bp, src_bp, nbytes_left, nbytes)                \
		do                                                                    \
		{                                                                     \
			if (src_bp % OPSIZ == 0)                                          \
				_wordcopy_fwd_aligned(dst_bp, src_bp, (nbytes) / OPSIZ);      \
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:121-125。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E028 · 源未对齐helper调用、宏内游标/余数

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::WORD_COPY_FWD]
```c
			else                                                              \
				_wordcopy_fwd_dest_aligned(dst_bp, src_bp, (nbytes) / OPSIZ); \
			src_bp += (nbytes) & -OPSIZ;                                      \
			dst_bp += (nbytes) & -OPSIZ;                                      \
			(nbytes_left) = (nbytes) % OPSIZ;                                 \
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:126-130。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E029 · 选定空PAGE定义

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::PAGE_COPY_FWD_MAYBE]
```c
		#define PAGE_COPY_FWD_MAYBE(dstp, srcp, nbytes_left, nbytes) /* nada */
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:194-194。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E030 · 大于等于16的阈值

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::OP_T_THRES]
```c
	#define OP_T_THRES 16
```

固定行：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:199-199。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E031 · 外层MEMCPY参数及局部游标

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
MEMCPY(void *dstpp, const void *srcpp, size_t len)
{
	unsigned long int dstp = (long int)dstpp;
	unsigned long int srcp = (long int)srcpp;
```

固定行：myinitramfs/mylib/myglibc/string/memcpy.c:28-31。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E032 · 外层进入长分支及前导对齐

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	/* If there not too few bytes to copy, use word copy.  */
	if (len >= OP_T_THRES)
	{
		/* Copy just a few bytes to make DSTP aligned.  */
```

固定行：myinitramfs/mylib/myglibc/string/memcpy.c:35-38。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E033 · 扣除前导与BYTE宏位置

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
		len -= (-dstp) % OPSIZ;
		BYTE_COPY_FWD(dstp, srcp, (-dstp) % OPSIZ);
```

固定行：myinitramfs/mylib/myglibc/string/memcpy.c:39-40。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E034 · PAGE宏调用位置

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
		PAGE_COPY_FWD_MAYBE(dstp, srcp, len, len);
```

固定行：myinitramfs/mylib/myglibc/string/memcpy.c:45-45。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E035 · WORD宏调用位置

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
		WORD_COPY_FWD(dstp, srcp, len, len);
```

固定行：myinitramfs/mylib/myglibc/string/memcpy.c:51-51。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E036 · 尾BYTE及返回原dstpp

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	/* There are just a few bytes to copy.  Use byte memory operations.  */
	BYTE_COPY_FWD(dstp, srcp, len);

	return dstpp;
```

固定行：myinitramfs/mylib/myglibc/string/memcpy.c:56-59。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E037 · time内核C配置活动语句

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

固定行：mykernel/scripts/options_flags.cmake:39-43。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E038 · time内核CONFIG活动语句

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

固定行：mykernel/scripts/options_flags.cmake:58-62。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## WC128-E039 · time panic金丝雀与源码区分

[VERIFIED mykernel/debug/panic.c::panic]
```c
	this_cpu = smp_processor_id();
```

固定行：mykernel/debug/panic.c:79-79。引文只证明本符号定义/活动配置源码，后续条件模型在主报告单列。

## 普通定位与未知（不列入本批新正式窗口）

- `wordcopy.c:130–138`有第二个helper的解释性注释和默认重命名宏；实际编译所用符号取决于构建预处理，原样定位不等于最终链接证明。
- `memcopy.h:69`的`__BYTE_ORDER == __LITTLE_ENDIAN`条件、`memcopy.h:165,192–196`的page条件分支与当前选择的宏定义一起解释本次假定，不证明实际sh编译宏值。
- `myinitramfs/mylib/myglibc/string/CMakeLists.txt:1–3`、`myinitramfs/mylib/myglibc/CMakeLists.txt:1,20`、`myinitramfs/CMakeLists.txt:40–51`与`myshell/CMakeLists.txt:4`是用户态构建普通位置，不能作为C函数或最终运行链接的正式定义体短窗。
- R127-01与R127-02是针对127冻结文档的追加纸面补正；历史127的44条原证据另有固定来源，不能计进128本批新增统计。
