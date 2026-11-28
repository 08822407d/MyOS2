---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
batch_id: "CORE-USER-MALLOC-REFILL-149"
conversation_display_name: "MYOS2-A-C10c 代码分析执行"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-24"
produced_by: "GPT-6.2"
model_per_owner: "GPT-6.2 Sol；Extra／高推理强度（Owner指定）"
actual_model_identifier_basis: "系统标识GPT-6.2；客户端选择不可见"
actual_backend_model: "UNATTESTED"
actual_reasoning_effort: "UNATTESTED"
date: "2026-11-25"
date_timezone: "Asia/Shanghai"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_pin_short12: "95d47a823b80"
execution_parent_short12: "d2c99a1f285b"
read_channel: "connector"
status: "AUTHOR_PENDING_MAINLINE_REVIEW"
evidence_class: "SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / CONDITIONAL_FRAGMENT / UNKNOWN"
open_questions: ["完整补充递归及终止性、实际对象建立/映射、bitmap载荷、真实ABI/标准/编译器/DEBUG、sh实际链接与运行均未证；N1547独立外证沿旧UNKNOWN。"]
full_local_P9: "NOT_RUN"
record_type: "batch149_source_evidence"
---

# 固定 time 源码窗（新增149）
每窗1–5连续物理行；原CRLF/LF保留。末行NONE仅在闭围栏前加一枚排版LF，按source_index的source_line_endings/render_separator还原时仅去该分隔LF。行号辅助，路径、符号及原文定位。相邻窗合读完整函数。短SHA自连接器输出复制。旧证据另账。

## E149-001 · ARCH

configuration_directive；mykernel/scripts/options_flags.cmake 7–9；Blob 6019436417ad；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=CONFIG；ARCH默认文本（分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::ARCH]
```cmake
if (NOT DEFINED ${ARCH})
	set(ARCH x86_64)
endif()
```

## E149-002 · RELEASE_FLAG

configuration_directive；mykernel/scripts/options_flags.cmake 16–20；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONFIG；Debug/else配置文本（分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::RELEASE_FLAG]
```cmake
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(RELEASE_FLAG "-DDEBUG")
	message("${PROMPT} DEBUG ${PROMPT_END}")
else ()
	message("${PROMPT} RELEASE  ${PROMPT_END}")
```

## E149-003 · RELEASE_FLAG

configuration_directive；mykernel/scripts/options_flags.cmake 21–23；Blob 6019436417ad；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=CONFIG；Debug/else配置文本（分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::RELEASE_FLAG]
```cmake
	set(RELEASE_FLAG "-DRELEASE")
	set(UNUSED_MACRO "-Wunused-macros")
endif()
```

## E149-004 · CMAKE_ASM_FLAGS

configuration_directive；mykernel/scripts/options_flags.cmake 30–34；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONFIG；kernel ASM配置（分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_ASM_FLAGS]
```cmake
set(CMAKE_ASM_FLAGS
	"${CMAKE_ASM_FLAGS} ${RELEASE_FLAG} ${UNUSED_MACRO} \
	-m64 -mcmodel=kernel -fverbose-asm \
	-fno-pie -fno-pic \
	-D__KERNEL__ \
```

## E149-005 · CMAKE_ASM_FLAGS

configuration_directive；mykernel/scripts/options_flags.cmake 35–36；Blob 6019436417ad；source_line_endings=LF,LF, render_separator=NONE；覆盖=CONFIG；kernel ASM配置（分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_ASM_FLAGS]
```cmake
	-D__ASSEMBLY__ -DASM_FILE \
")
```

## E149-006 · CMAKE_C_FLAGS

configuration_directive；mykernel/scripts/options_flags.cmake 39–43；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONFIG；kernel C flags、canary；不外推用户ABI（分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

## E149-007 · CMAKE_C_FLAGS

configuration_directive；mykernel/scripts/options_flags.cmake 44–48；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONFIG；kernel C flags、canary；不外推用户ABI（分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-fno-stack-protector \
	-fno-unwind-tables \
	-static \
	-fdata-sections -ffunction-sections \
	-ggdb \
```

## E149-008 · CMAKE_C_FLAGS

configuration_directive；mykernel/scripts/options_flags.cmake 49–51；Blob 6019436417ad；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=CONFIG；kernel C flags、canary；不外推用户ABI（分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-mabi=sysv \
	-mno-red-zone \
")
```

## E149-009 · CMAKE_C_FLAGS

configuration_directive；mykernel/scripts/options_flags.cmake 58–62；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONFIG；活动kernel CONFIG宏文本（分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

## E149-010 · CMAKE_C_FLAGS

configuration_directive；mykernel/scripts/options_flags.cmake 63–67；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONFIG；活动kernel CONFIG宏文本（分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
	-DGRUB2_BOOTUP_SUPPORT \
```

## E149-011 · CMAKE_C_FLAGS

configuration_directive；mykernel/scripts/options_flags.cmake 68–70；Blob 6019436417ad；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=CONFIG；活动kernel CONFIG宏文本（分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_HYPERVISOR_GUEST \
	-DCONFIG_KVM_GUEST \
")
```

## E149-012 · panic

function_body；mykernel/debug/panic.c 79–79；Blob 2e9d5b195559；source_line_endings=LF, render_separator=NONE；覆盖=CANARY；第二canary（分窗合读）。

[VERIFIED mykernel/debug/panic.c::panic]
```c
	this_cpu = smp_processor_id();
```

## E149-013 · CONST_1M

macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 12–12；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=A_GATE；尺寸上限（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::CONST_1M]
```c
	#define CONST_1M		0x100000ULL
```

## E149-014 · CONST_2M

macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 15–15；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=H；2MiB数值（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::CONST_2M]
```c
	#define CONST_2M		0x200000ULL
```

## E149-015 · PAGE_SIZE

macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 60–60；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=H；局部H别名（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::PAGE_SIZE]
```c
#define PAGE_SIZE			CONST_2M
```

## E149-016 · initiated

variable_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 62–62；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=A_GATE；初始化标记定义；案例另给true（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::initiated]
```c
bool initiated = false;
```

## E149-017 · uslab_cache_groups

variable_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 66–66；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=A_SELECT；cache数组（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_cache_groups]
```c
uslab_cache_s	uslab_cache_groups[USLAB_LEVEL];
```

## E149-018 · USLAB_LEVEL

macro_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 7–7；Blob 2dea1f8fc05c；source_line_endings=LF, render_separator=NONE；覆盖=A_SELECT；16档（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::USLAB_LEVEL]
```c
	#define USLAB_LEVEL			16
```

## E149-019 · USLAB_SIZE_BASE

macro_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 8–8；Blob 2dea1f8fc05c；source_line_endings=LF, render_separator=NONE；覆盖=A_SELECT；32基数（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::USLAB_SIZE_BASE]
```c
	#define USLAB_SIZE_BASE		32
```

## E149-020 · uslab_s

typedef_declaration；myinitramfs/mylib/myglibc/malloc/_malloc.h 10–11；Blob 2dea1f8fc05c；source_line_endings=LF,LF, render_separator=NONE；覆盖=LAYOUT；uslab别名（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]
```c
	struct  uslab;
	typedef struct uslab uslab_s;
```

## E149-021 · uslab_cache_s

typedef_declaration；myinitramfs/mylib/myglibc/malloc/_malloc.h 13–14；Blob 2dea1f8fc05c；source_line_endings=LF,LF, render_separator=NONE；覆盖=LAYOUT；cache别名（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c
	struct	uslab_cache;
	typedef struct uslab_cache uslab_cache_s;
```

## E149-022 · uPage_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 19–23；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LAYOUT；uPage完整成员与别名（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage_s]
```c
	typedef struct uPage
	{
		List_s	upage_list;
		void	*vaddr;
		uslab_s *uslab_p;
```

## E149-023 · uPage_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 24–24；Blob 2dea1f8fc05c；source_line_endings=LF, render_separator=NONE；覆盖=LAYOUT；uPage完整成员与别名（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage_s]
```c
	} uPage_s;
```

## E149-024 · uslab_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 26–30；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LAYOUT；uslab完整成员/别名（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]
```c
	typedef struct uslab
	{
		List_s			uslab_list;
		uslab_cache_s	*uslabcache_p;

```

## E149-025 · uslab_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 31–35；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LAYOUT；uslab完整成员/别名（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]
```c
		unsigned long	total;
		unsigned long	free;

		uPage_s			*upage_p;
		void			*virt_addr;
```

## E149-026 · uslab_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 36–37；Blob 2dea1f8fc05c；source_line_endings=LF,LF, render_separator=NONE；覆盖=LAYOUT；uslab完整成员/别名（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]
```c
		bitmap_t		*colormap;
	} uslab_s;
```

## E149-027 · uslab_cache_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 39–43；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CACHE_LAYOUT；cache链、尺寸、计数、表头和base成员（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c
	typedef struct uslab_cache
	{
		List_s			uslabcache_list;

		unsigned long	obj_size;
```

## E149-028 · uslab_cache_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 44–48；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CACHE_LAYOUT；cache链、尺寸、计数、表头和base成员（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c

		unsigned long	normal_slab_total;
		List_hdr_s		normal_slab_free;
		List_hdr_s		normal_slab_used;
		List_hdr_s		normal_slab_full;
```

## E149-029 · uslab_cache_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 49–53；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CACHE_LAYOUT；cache链、尺寸、计数、表头和base成员（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c
		unsigned long	nsobj_free_count;
		unsigned long	nsobj_used_count;

		// the base slab should not be freed
		uslab_s			*normal_base_slab_p;
```

## E149-030 · uslab_cache_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 54–54；Blob 2dea1f8fc05c；source_line_endings=LF, render_separator=NONE；覆盖=CACHE_LAYOUT；cache链、尺寸、计数、表头和base成员（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c
	} uslab_cache_s;
```

## E149-031 · container_of

macro_definition；myinitramfs/mylib/include/utils.h 12–16；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONTAINER；活动宏：整数减成员地址投影；不建立对象（分窗合读）。

[VERIFIED myinitramfs/mylib/include/utils.h::container_of]
```c
	#define container_of(member_ptr, container_type, member_name)		\
	({																	\
		typeof(((container_type *)0)->member_name) * p = (member_ptr);	\
		(container_type *)((unsigned long)p - 							\
		(unsigned long)&(((container_type *)0)->member_name));			\
```

## E149-032 · container_of

macro_definition；myinitramfs/mylib/include/utils.h 17–17；Blob 4e3e8910ac00；source_line_endings=LF, render_separator=NONE；覆盖=CONTAINER；活动宏：整数减成员地址投影；不建立对象（分窗合读）。

[VERIFIED myinitramfs/mylib/include/utils.h::container_of]
```c
	})
```

## E149-033 · bitmap_t

typedef_definition；myinitramfs/mylib/include/utils.h 19–19；Blob 4e3e8910ac00；source_line_endings=LF, render_separator=NONE；覆盖=BITMAP；bitmap类型（分窗合读）。

[VERIFIED myinitramfs/mylib/include/utils.h::bitmap_t]
```c
	typedef __UINT64_TYPE__ bitmap_t;
```

## E149-034 · BITMAP_UNITSIZE

macro_definition；myinitramfs/mylib/include/utils.h 20–20；Blob 4e3e8910ac00；source_line_endings=LF, render_separator=NONE；覆盖=BITMAP；sizeof*8（分窗合读）。

[VERIFIED myinitramfs/mylib/include/utils.h::BITMAP_UNITSIZE]
```c
	#define BITMAP_UNITSIZE (sizeof(bitmap_t) * 8)
```

## E149-035 · List_s

structure_definition；myinitramfs/mylib/include/utils.h 34–38；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LAYOUT；两指针声明与别名（分窗合读）。

[VERIFIED myinitramfs/mylib/include/utils.h::List_s]
```c
	typedef struct List List_s;
	typedef struct List
	{
		List_s *	prev;
		List_s *	next;
```

## E149-036 · List_s

structure_definition；myinitramfs/mylib/include/utils.h 39–39；Blob 4e3e8910ac00；source_line_endings=LF, render_separator=NONE；覆盖=LAYOUT；两指针声明与别名（分窗合读）。

[VERIFIED myinitramfs/mylib/include/utils.h::List_s]
```c
	} List_s;
```

## E149-037 · List_hdr_s

structure_definition；myinitramfs/mylib/include/utils.h 41–45；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LIST；anchor/count（分窗合读）。

[VERIFIED myinitramfs/mylib/include/utils.h::List_hdr_s]
```c
	typedef struct List_hdr
	{
		List_s			anchor;
		unsigned long	count;
	} List_hdr_s;
```

## E149-038 · init_uslab

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 181–185；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=A_SELECT；cache链顺序（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
	for (int i = 0; i < USLAB_LEVEL; i++)
	{
		uslab_cache_s * scgp = &uslab_cache_groups[i];
		INIT_LIST_S(&scgp->uslabcache_list);
		list_header_add_to_tail(&uslabcache_lhdr, &scgp->uslabcache_list);
```

## E149-039 · init_uslab

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 194–195；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF, render_separator=NONE；覆盖=A_SELECT；obj_size及容量（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		scgp->obj_size = USLAB_SIZE_BASE << i;
		unsigned long obj_nr = PAGE_SIZE / scgp->obj_size;
```

## E149-040 · init_uslab

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 220–220；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=A_GATE；init尾置true（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
	initiated = true;
```

## E149-041 · upage_alloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 111–115；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=UPAGE；free-page门、复用无metadata malloc、动态sizeof（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]
```c
uPage_s *upage_alloc()
{
	uPage_s *ret_val = NULL;
	if (free_upage_lhdr.count > 0)
	{
```

## E149-042 · upage_alloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 116–120；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=UPAGE；free-page门、复用无metadata malloc、动态sizeof（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]
```c
		List_s * upg_lp = list_header_remove_head(&free_upage_lhdr);
		while (upg_lp == 0);
		
		ret_val = container_of(upg_lp, uPage_s, upage_list);
		list_header_add_to_head(&used_upage_lhdr, &ret_val->upage_list);
```

## E149-043 · upage_alloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 121–124；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=UPAGE；free-page门、复用无metadata malloc、动态sizeof（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]
```c
	}
	else
	{
		uPage_s *upgp = malloc(sizeof(uPage_s));
```

## E149-044 · upage_alloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 125–129；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=UPAGE；动态分支后续brk与返回；A不展开（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]
```c
		void *pg_vaddr = brk(brk_end + PAGE_SIZE);
		if (pg_vaddr != NULL)
		{
			brk_end += PAGE_SIZE;
			upgp->vaddr = pg_vaddr;
```

## E149-045 · upage_alloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 130–134；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=UPAGE；动态分支后续brk与返回；A不展开（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]
```c
			INIT_LIST_S(&upgp->upage_list);
			list_header_add_to_tail(&used_upage_lhdr, &upgp->upage_list);
			ret_val = upgp;
		}
		else
```

## E149-046 · upage_alloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 135–139；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=UPAGE；动态分支后续brk与返回；A不展开（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]
```c
		{
			free(upgp);
			ret_val = NULL;
		}
	}
```

## E149-047 · upage_alloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 140–140；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=UPAGE；动态分支后续brk与返回；A不展开（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]
```c
	return ret_val;
```

## E149-048 · uslab_alloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 223–227；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=USLAB_ALLOC；完整直接调用/字段写入及对象门（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_alloc]
```c
uslab_s * uslab_alloc(uslab_s * cslp)
{
	uslab_s * nslp = (uslab_s *)malloc(sizeof(uslab_s));
	INIT_LIST_S(&nslp->uslab_list);

```

## E149-049 · uslab_alloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 228–232；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=USLAB_ALLOC；完整直接调用/字段写入及对象门（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_alloc]
```c
	nslp->colormap = (bitmap_t *)malloc(cslp->total / sizeof(bitmap_t) + sizeof(bitmap_t));
	nslp->total =
	nslp->free = cslp->total;
	nslp->upage_p = upage_alloc();
	nslp->virt_addr = nslp->upage_p->vaddr;
```

## E149-050 · uslab_alloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 233–236；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=USLAB_ALLOC；完整直接调用/字段写入及对象门（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_alloc]
```c
	nslp->upage_p->uslab_p = nslp;

	return nslp;
}
```

## E149-051 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 249–253；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=MALLOC_GATE；init、上限、ret、选档（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
void * malloc(size_t size)
{
	if (!initiated)
		init_uslab();

```

## E149-052 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 254–258；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=MALLOC_GATE；init、上限、ret、选档（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
	void * ret_val = NULL;
	if (size > CONST_1M)
	{
		// color_printk(RED, WHITE, "Mem required too big\n");
	}
```

## E149-053 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 259–263；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=MALLOC_GATE；init、上限、ret、选档（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
	else
	{
		// find suitable slab group
		uslab_cache_s *	uscgp = &uslab_cache_groups[0];
		while (size > uscgp->obj_size)
```

## E149-054 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 264–264；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=MALLOC_GATE；init、上限、ret、选档（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			uscgp = container_of((uscgp->uslabcache_list.next), uslab_cache_s, uslabcache_list);
```

## E149-055 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 268–272；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=MALLOC_LIST；used/free优先、摘首、NULL等待、last-free预迁移、无slab循环（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		uslab_s *	uslp = NULL;
		if (uscgp->normal_slab_used.count > 0)
		{
			List_s * uslp_lp = list_header_remove_head(&uscgp->normal_slab_used);
			while (uslp_lp == 0);
```

## E149-056 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 273–277；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=MALLOC_LIST；used/free优先、摘首、NULL等待、last-free预迁移、无slab循环（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			
			uslp = container_of(uslp_lp, uslab_s, uslab_list);
			if (uslp->free == 1)
				list_header_add_to_head(&uscgp->normal_slab_full, &uslp->uslab_list);
			else
```

## E149-057 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 278–282；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=MALLOC_LIST；used/free优先、摘首、NULL等待、last-free预迁移、无slab循环（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
				list_header_add_to_head(&uscgp->normal_slab_used, &uslp->uslab_list);
		}
		else if (uscgp->normal_slab_free.count > 0)
		{
			List_s * uslp_lp = list_header_remove_head(&uscgp->normal_slab_free);
```

## E149-058 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 283–287；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=MALLOC_LIST；used/free优先、摘首、NULL等待、last-free预迁移、无slab循环（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			while (uslp_lp == 0);
			
			uslp = container_of(uslp_lp, uslab_s, uslab_list);
			list_header_add_to_head(&uscgp->normal_slab_used, &uslp->uslab_list);
		}
```

## E149-059 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 288–292；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=MALLOC_LIST；used/free优先、摘首、NULL等待、last-free预迁移、无slab循环（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		else
		{
			// color_printk(WHITE, RED, "Slab %#x-bytes used out!\n", scgp->obj_size);
			while (1);
		}
```

## E149-060 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 294–298；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=MALLOC_SLOT；扫描、idx条件、ret投影、三计数和set（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		// count the virtual addr of suitable object
		unsigned long obj_idx = bm_get_freebit_idx(uslp->colormap, 0, uslp->total);
		if (obj_idx < uslp->total)
		{
			size_t offset = uscgp->obj_size * obj_idx;
```

## E149-061 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 299–303；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=MALLOC_SLOT；扫描、idx条件、ret投影、三计数和set（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			ret_val = (void *)((size_t)uslp->virt_addr + offset);
			// refresh status of slab
			uslp->free--;
			uscgp->nsobj_free_count--;
			uscgp->nsobj_used_count++;
```

## E149-062 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 304–305；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF, render_separator=NONE；覆盖=MALLOC_SLOT；扫描、idx条件、ret投影、三计数和set（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			bm_set_bit(uslp->colormap, obj_idx);
		}
```

## E149-063 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 307–311；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=MALLOC_REFILL；补充门、预留count、callee、回减、NULL检查、发布及return（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		// make sure free slab more than one
		if (uscgp->normal_slab_free.count < 1)
		{
			uscgp->normal_slab_free.count++;
			uslab_s * new_uslp = uslab_alloc(uslp);
```

## E149-064 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 312–316；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=MALLOC_REFILL；补充门、预留count、callee、回减、NULL检查、发布及return（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			uscgp->normal_slab_free.count--;
			if (new_uslp == NULL)
			{
				// color_printk(RED, WHITE, "Alloc new slab :%#x-bytes failed!\n", scgp->obj_size);
				while (1);
```

## E149-065 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 317–321；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=MALLOC_REFILL；补充门、预留count、callee、回减、NULL检查、发布及return（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			}
			new_uslp->uslabcache_p = uscgp;
			list_header_add_to_head(&uscgp->normal_slab_free, &new_uslp->uslab_list);
			uscgp->normal_slab_total++;
			uscgp->nsobj_free_count += new_uslp->total;
```

## E149-066 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 322–326；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=MALLOC_REFILL；补充门、预留count、callee、回减、NULL检查、发布及return（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		}

		// unlock_recurs_lock(&slab_alloc_lock);
	}

```

## E149-067 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 327–327；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=MALLOC_REFILL；补充门、预留count、callee、回减、NULL检查、发布及return（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
	return ret_val;
```

## E149-068 · INIT_LIST_S

function_body；myinitramfs/mylib/util/list.c 10–14；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=INIT_LIST；双指针self-loop（分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::INIT_LIST_S]
```c
void INIT_LIST_S(List_s * src)
{
	src->prev = src;
	src->next = src;
}
```

## E149-069 · list_insert_next

function_body；myinitramfs/mylib/util/list.c 29–33；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=INSERT；先del_init再四指针接入（分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_next]
```c
void list_insert_next(List_s * dst, List_s * src)
{
	list_del_init(src);

	src->next = dst->next;
```

## E149-070 · list_insert_next

function_body；myinitramfs/mylib/util/list.c 34–38；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=INSERT；先del_init再四指针接入（分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_next]
```c
	src->prev = dst;

	src->next->prev = src;
	src->prev->next = src;
}
```

## E149-071 · list_del_init

function_body；myinitramfs/mylib/util/list.c 40–44；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=DELETE；邻居写后self-loop（分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_del_init]
```c
void list_del_init(List_s *src)
{
	src->prev->next = src->next;
	src->next->prev = src->prev;
	INIT_LIST_S(src);
```

## E149-072 · list_del_init

function_body；myinitramfs/mylib/util/list.c 45–45；Blob 24f7a214a288；source_line_endings=LF, render_separator=NONE；覆盖=DELETE；邻居写后self-loop（分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_del_init]
```c
}
```

## E149-073 · list_header_add_to_head

function_body；myinitramfs/mylib/util/list.c 79–83；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=ADD_HEAD；插入后count++（分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_add_to_head]
```c
void list_header_add_to_head(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_next(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}
```

## E149-074 · list_header_remove_head

function_body；myinitramfs/mylib/util/list.c 85–89；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=REMOVE_HEAD；按count取anchor.next、del_init、count--、否则NULL（分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_remove_head]
```c
List_s * list_header_remove_head(List_hdr_s * lhdr_p)
{
	if (lhdr_p->count > 0)
	{
		List_s * ret_val = lhdr_p->anchor.next;
```

## E149-075 · list_header_remove_head

function_body；myinitramfs/mylib/util/list.c 90–94；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=REMOVE_HEAD；按count取anchor.next、del_init、count--、否则NULL（分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_remove_head]
```c
		list_del_init(ret_val);
		lhdr_p->count--;
		return ret_val;
	}

```

## E149-076 · list_header_remove_head

function_body；myinitramfs/mylib/util/list.c 95–96；Blob 24f7a214a288；source_line_endings=LF,LF, render_separator=NONE；覆盖=REMOVE_HEAD；按count取anchor.next、del_init、count--、否则NULL（分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_remove_head]
```c
	return NULL;
}
```

## E149-077 · bm_set_bit

function_body；myinitramfs/mylib/util/bitmap.c 14–17；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖=BM_SET；word索引、1ULL及OR（分窗合读）。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_set_bit]
```c
void bm_set_bit(bitmap_t * bm, unsigned long idx)
{
	*(bm + idx / BITMAP_UNITSIZE) |= (1ULL << (idx % BITMAP_UNITSIZE));
}
```

## E149-078 · bm_get_freebit_idx

function_body；myinitramfs/mylib/util/bitmap.c 24–28；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=BM_SCAN；线性扫描、1ULL、遇零break、返回i（分窗合读）。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_freebit_idx]
```c
unsigned long bm_get_freebit_idx(bitmap_t * bm, unsigned long start, unsigned long limit)
{
	unsigned long 	i;
	for (i = start; i < limit; i++)
	{
```

## E149-079 · bm_get_freebit_idx

function_body；myinitramfs/mylib/util/bitmap.c 29–33；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=BM_SCAN；线性扫描、1ULL、遇零break、返回i（分窗合读）。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_freebit_idx]
```c
		bitmap_t bm_unit = *(bm + (i / BITMAP_UNITSIZE));
		if ((bm_unit & (1ULL << i % BITMAP_UNITSIZE)) == 0)
			break;
	}
	return i;
```

## E149-080 · bm_get_freebit_idx

function_body；myinitramfs/mylib/util/bitmap.c 34–34；Blob 6a86523c41ba；source_line_endings=NONE, render_separator=LF；覆盖=BM_SCAN；线性扫描、1ULL、遇零break、返回i（分窗合读）。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_freebit_idx]
```c
}
```

## E149-081 · RELEASE_FLAG

configuration_directive；myinitramfs/CMakeLists.txt 13–17；Blob 6757d21bc64c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=BUILD；用户Debug/Release文本（分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::RELEASE_FLAG]
```cmake
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(RELEASE_FLAG "-DDEBUG")
	message("${PROMPT} DEBUG ${PROMPT_END}")
elseif ()
	message("${PROMPT} RELEASE")
```

## E149-082 · RELEASE_FLAG

configuration_directive；myinitramfs/CMakeLists.txt 18–20；Blob 6757d21bc64c；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=BUILD；用户Debug/Release文本（分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::RELEASE_FLAG]
```cmake
	set(RELEASE_FLAG "-DRELEASE")
	set(UNUSED_MACRO "-Wunused-macros")
endif()
```

## E149-083 · CMAKE_ASM_FLAGS

configuration_directive；myinitramfs/CMakeLists.txt 22–24；Blob 6757d21bc64c；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=BUILD；用户ASM flags（分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::CMAKE_ASM_FLAGS]
```cmake
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${RELEASE_FLAG} \
	-m64 \
")
```

## E149-084 · CMAKE_C_FLAGS

configuration_directive；myinitramfs/CMakeLists.txt 26–30；Blob 6757d21bc64c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=BUILD；用户C flags与static条件（分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::CMAKE_C_FLAGS]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 \
	-ggdb \
")

```

## E149-085 · CMAKE_C_FLAGS

configuration_directive；myinitramfs/CMakeLists.txt 31–33；Blob 6757d21bc64c；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=BUILD；用户C flags与static条件（分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::CMAKE_C_FLAGS]
```cmake
if (${LINK_TYPE} STREQUAL "static")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
endif()
```

## E149-086 · MUSL_PATH

configuration_directive；myinitramfs/CMakeLists.txt 40–44；Blob 6757d21bc64c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=BUILD；外部工具路径配置；不读取外部内容（分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::MUSL_PATH]
```cmake
set(MUSL_PATH /home/cheyh/projs/musl/build)
set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)

set(MUSL_INC_PATH ${MUSL_PATH}/include)
set(MUSL_LIB_PATH ${MUSL_PATH}/lib)
```

## E149-087 · add_subdirectory(myinit)

configuration_directive；myinitramfs/CMakeLists.txt 46–50；Blob 6757d21bc64c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=BUILD；根入口列myinit/myshell及安装init/sh（分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory(myinit)]
```cmake
include_directories("${MUSL_INC_PATH}")
add_subdirectory(myinit)
add_subdirectory(myshell)

install(TARGETS init DESTINATION boot)
```

## E149-088 · add_subdirectory(myinit)

configuration_directive；myinitramfs/CMakeLists.txt 51–51；Blob 6757d21bc64c；source_line_endings=NONE, render_separator=LF；覆盖=BUILD；根入口列myinit/myshell及安装init/sh（分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory(myinit)]
```cmake
install(TARGETS sh DESTINATION boot)
```

## E149-089 · add_subdirectory(myglibc)

configuration_directive；myinitramfs/mylib/CMakeLists.txt 1–3；Blob 8f6abc24095a；source_line_endings=LF,LF,NONE, render_separator=LF；覆盖=BUILD；myglibc/csu/util（分窗合读）。

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory(myglibc)]
```cmake
add_subdirectory(myglibc)
add_subdirectory(csu)
add_subdirectory(util)
```

## E149-090 · mylibc

configuration_directive；myinitramfs/mylib/myglibc/CMakeLists.txt 1–3；Blob b956957a602a；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=BUILD；mylibc STATIC及flags（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::mylibc]
```cmake
add_library(mylibc STATIC)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH}")
target_include_directories(mylibc PUBLIC
```

## E149-091 · target_include_directories

configuration_directive；myinitramfs/mylib/myglibc/CMakeLists.txt 4–8；Blob b956957a602a；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=BUILD；include文本（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories]
```cmake
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/include
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sys
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/string
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/misc
```

## E149-092 · target_include_directories

configuration_directive；myinitramfs/mylib/myglibc/CMakeLists.txt 9–13；Blob b956957a602a；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=BUILD；include文本（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories]
```cmake
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/x86
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/generic
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/gnu
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux/x86
```

## E149-093 · target_include_directories

configuration_directive；myinitramfs/mylib/myglibc/CMakeLists.txt 14–16；Blob b956957a602a；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=BUILD；include文本（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories]
```cmake
                    ${PROJECT_SOURCE_DIR}/../mykernel/include
                    ${PROJECT_SOURCE_DIR}/../mykernel/arch/${ARCH}/include
                    )
```

## E149-094 · add_subdirectory(malloc)

configuration_directive；myinitramfs/mylib/myglibc/CMakeLists.txt 18–21；Blob b956957a602a；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖=BUILD；malloc接入（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory(malloc)]
```cmake
add_subdirectory(stdio)
add_subdirectory(malloc)
add_subdirectory(string)
add_subdirectory(exit)
```

## E149-095 · LIBC_STDLIB_C_SRCS

configuration_directive；myinitramfs/mylib/myglibc/malloc/CMakeLists.txt 1–3；Blob 3b9f22dd9ab9；source_line_endings=LF,LF,NONE, render_separator=LF；覆盖=BUILD；malloc GLOB C/ASM到target_sources（分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::LIBC_STDLIB_C_SRCS]
```cmake
file(GLOB LIBC_STDLIB_C_SRCS *.c)
file(GLOB LIBC_STDLIB_ASM_SRCS *.S)
target_sources(mylibc PUBLIC ${LIBC_STDLIB_C_SRCS} ${LIBC_STDLIB_ASM_SRCS})
```

## E149-096 · UTIL_C_SRCS

configuration_directive；myinitramfs/mylib/util/CMakeLists.txt 1–3；Blob 5fd782a35629；source_line_endings=LF,LF,NONE, render_separator=LF；覆盖=BUILD；util GLOB C/ASM到target_sources（分窗合读）。

[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::UTIL_C_SRCS]
```cmake
file(GLOB UTIL_C_SRCS *.c)
file(GLOB UTIL_ASM_SRCS *.S)
target_sources(mylibc PUBLIC ${UTIL_C_SRCS} ${UTIL_ASM_SRCS})
```
