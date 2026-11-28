---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
batch_id: "CORE-USER-MALLOC-STATE-CONSUMPTION-151"
conversation_display_name: "MYOS2-A-C10c 代码分析执行"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-24"
produced_by: "GPT-6.2（系统标识）"
model_per_owner: "GPT-6.2 Sol；Extra／高推理强度（Owner指定）"
actual_backend_model: "UNATTESTED"
actual_reasoning_effort: "UNATTESTED"
date: "2026-11-26"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
read_channel: "connector"
full_local_P9: "NOT_RUN"
record_type: "batch151_source_evidence"
source_index_location: facts-and-boundaries.yaml::source_index
---

# 第151批正式源码证据
固定time；每窗1–5连续物理行，函数/宏/结构及CMake命令实例范围准确。源LF/CRLF保留，NONE仅增加声明的排版LF并可逆去除。旧96/137及主线11窗不计本批，D00=[]。

## E151-001 · CONST_1M

macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理12–12，定义/实例12–12；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=CONSTANT；size upper bound（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::CONST_1M]
```c
	#define CONST_1M		0x100000ULL
```

## E151-002 · CONST_2M

macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理15–15，定义/实例15–15；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=ROUND；H and rounding macro chain（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::CONST_2M]
```c
	#define CONST_2M		0x200000ULL
```

## E151-003 · __round_mask

macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理36–36，定义/实例36–36；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=ROUND；H and rounding macro chain（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::__round_mask]
```c
#define __round_mask(x, y) ((__typeof__(x))((y)-1))
```

## E151-004 · round_down

macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理56–56，定义/实例56–56；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=ROUND；H and rounding macro chain（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::round_down]
```c
#define round_down(x, y) ((x) & ~__round_mask(x, y))
```

## E151-005 · PAGE_ROUND_DOWN

macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理59–59，定义/实例59–59；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=ROUND；H and rounding macro chain（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::PAGE_ROUND_DOWN]
```c
#define PAGE_ROUND_DOWN(x)	round_down((x), PAGE_SIZE)
```

## E151-006 · PAGE_SIZE

macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理60–60，定义/实例60–60；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=ROUND；H and rounding macro chain（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::PAGE_SIZE]
```c
#define PAGE_SIZE			CONST_2M
```

## E151-007 · initiated

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理62–62，定义/实例62–62；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=GLOBALS；actual global definition, case values GIVEN（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::initiated]
```c
bool initiated = false;
```

## E151-008 · brk_end

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理63–63，定义/实例63–63；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=GLOBALS；actual global definition, case values GIVEN（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::brk_end]
```c
void *brk_end = NULL;
```

## E151-009 · uslabcache_lhdr

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理65–65，定义/实例65–65；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=GLOBALS；actual global definition, case values GIVEN（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslabcache_lhdr]
```c
List_hdr_s		uslabcache_lhdr;
```

## E151-010 · uslab_cache_groups

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理66–66，定义/实例66–66；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=GLOBALS；actual global definition, case values GIVEN（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_cache_groups]
```c
uslab_cache_s	uslab_cache_groups[USLAB_LEVEL];
```

## E151-011 · free_upage_lhdr

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理69–69，定义/实例69–69；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=GLOBALS；actual global definition, case values GIVEN（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free_upage_lhdr]
```c
List_hdr_s		free_upage_lhdr;
```

## E151-012 · used_upage_lhdr

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理70–70，定义/实例70–70；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=GLOBALS；actual global definition, case values GIVEN（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::used_upage_lhdr]
```c
List_hdr_s		used_upage_lhdr;
```

## E151-013 · USLAB_LEVEL

macro_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理7–7，定义/实例7–7；Blob 2dea1f8fc05c；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；16 classes（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::USLAB_LEVEL]
```c
	#define USLAB_LEVEL			16
```

## E151-014 · USLAB_SIZE_BASE

macro_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理8–8，定义/实例8–8；Blob 2dea1f8fc05c；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；32 base（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::USLAB_SIZE_BASE]
```c
	#define USLAB_SIZE_BASE		32
```

## E151-015 · uslab_s

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理11–11，定义/实例11–11；Blob 2dea1f8fc05c；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；alias（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]
```c
	typedef struct uslab uslab_s;
```

## E151-016 · uslab_cache_s

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理14–14，定义/实例14–14；Blob 2dea1f8fc05c；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；alias（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c
	typedef struct uslab_cache uslab_cache_s;
```

## E151-017 · uPage_s

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理19–23，定义/实例19–24；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；page members（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage_s]
```c
	typedef struct uPage
	{
		List_s	upage_list;
		void	*vaddr;
		uslab_s *uslab_p;
```

## E151-018 · uPage_s

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理24–24，定义/实例19–24；Blob 2dea1f8fc05c；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；page members（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage_s]
```c
	} uPage_s;
```

## E151-019 · uslab_s

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理26–30，定义/实例26–37；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；slab members（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]
```c
	typedef struct uslab
	{
		List_s			uslab_list;
		uslab_cache_s	*uslabcache_p;

```

## E151-020 · uslab_s

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理31–35，定义/实例26–37；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；slab members（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]
```c
		unsigned long	total;
		unsigned long	free;

		uPage_s			*upage_p;
		void			*virt_addr;
```

## E151-021 · uslab_s

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理36–37，定义/实例26–37；Blob 2dea1f8fc05c；source_line_endings=LF,LF；render_separator=NONE；覆盖=LAYOUT；slab members（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]
```c
		bitmap_t		*colormap;
	} uslab_s;
```

## E151-022 · uslab_cache_s

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理39–43，定义/实例39–54；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；cache members（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c
	typedef struct uslab_cache
	{
		List_s			uslabcache_list;

		unsigned long	obj_size;
```

## E151-023 · uslab_cache_s

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理44–48，定义/实例39–54；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；cache members（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c

		unsigned long	normal_slab_total;
		List_hdr_s		normal_slab_free;
		List_hdr_s		normal_slab_used;
		List_hdr_s		normal_slab_full;
```

## E151-024 · uslab_cache_s

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理49–53，定义/实例39–54；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；cache members（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c
		unsigned long	nsobj_free_count;
		unsigned long	nsobj_used_count;

		// the base slab should not be freed
		uslab_s			*normal_base_slab_p;
```

## E151-025 · uslab_cache_s

type_or_variable_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理54–54，定义/实例39–54；Blob 2dea1f8fc05c；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；cache members（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c
	} uslab_cache_s;
```

## E151-026 · container_of

macro_definition；myinitramfs/mylib/include/utils.h；物理12–16，定义/实例12–17；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；base projection by member offset（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/include/utils.h::container_of]
```c
	#define container_of(member_ptr, container_type, member_name)		\
	({																	\
		typeof(((container_type *)0)->member_name) * p = (member_ptr);	\
		(container_type *)((unsigned long)p - 							\
		(unsigned long)&(((container_type *)0)->member_name));			\
```

## E151-027 · container_of

macro_definition；myinitramfs/mylib/include/utils.h；物理17–17，定义/实例12–17；Blob 4e3e8910ac00；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；base projection by member offset（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/include/utils.h::container_of]
```c
	})
```

## E151-028 · bitmap_t

type_or_variable_definition；myinitramfs/mylib/include/utils.h；物理19–19，定义/实例19–19；Blob 4e3e8910ac00；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；typedef（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/include/utils.h::bitmap_t]
```c
	typedef __UINT64_TYPE__ bitmap_t;
```

## E151-029 · BITMAP_UNITSIZE

macro_definition；myinitramfs/mylib/include/utils.h；物理20–20，定义/实例20–20；Blob 4e3e8910ac00；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；bit unit width（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/include/utils.h::BITMAP_UNITSIZE]
```c
	#define BITMAP_UNITSIZE (sizeof(bitmap_t) * 8)
```

## E151-030 · List_s

type_or_variable_definition；myinitramfs/mylib/include/utils.h；物理34–34，定义/实例34–34；Blob 4e3e8910ac00；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；forward alias（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/include/utils.h::List_s]
```c
	typedef struct List List_s;
```

## E151-031 · List_s

type_or_variable_definition；myinitramfs/mylib/include/utils.h；物理35–39，定义/实例35–39；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；prev and next（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/include/utils.h::List_s]
```c
	typedef struct List
	{
		List_s *	prev;
		List_s *	next;
	} List_s;
```

## E151-032 · List_hdr_s

type_or_variable_definition；myinitramfs/mylib/include/utils.h；物理41–45，定义/实例41–45；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；anchor/count（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/include/utils.h::List_hdr_s]
```c
	typedef struct List_hdr
	{
		List_s			anchor;
		unsigned long	count;
	} List_hdr_s;
```

## E151-033 · init_uslab

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理181–185，定义/实例174–221；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=BOUNDARY；class chain construction identity（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
	for (int i = 0; i < USLAB_LEVEL; i++)
	{
		uslab_cache_s * scgp = &uslab_cache_groups[i];
		INIT_LIST_S(&scgp->uslabcache_list);
		list_header_add_to_tail(&uslabcache_lhdr, &scgp->uslabcache_list);
```

## E151-034 · init_uslab

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理194–195，定义/实例174–221；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF；render_separator=NONE；覆盖=BOUNDARY；class size/capacity（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		scgp->obj_size = USLAB_SIZE_BASE << i;
		unsigned long obj_nr = PAGE_SIZE / scgp->obj_size;
```

## E151-035 · init_uslab

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理220–221，定义/实例174–221；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF；render_separator=NONE；覆盖=BOUNDARY；initiated true（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
	initiated = true;
}
```

## E151-036 · upage_alloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理111–115，定义/实例111–141；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=BOUNDARY；callee identity, not entered（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]
```c
uPage_s *upage_alloc()
{
	uPage_s *ret_val = NULL;
	if (free_upage_lhdr.count > 0)
	{
```

## E151-037 · upage_free

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理143–147，定义/实例143–149；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=BOUNDARY；callee identity, not entered（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_free]
```c
void upage_free(uPage_s * upgp)
{
	list_del_init(&upgp->upage_list);
	used_upage_lhdr.count--;
	list_header_add_to_head(&free_upage_lhdr, &upgp->upage_list);
```

## E151-038 · uslab_alloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理223–226，定义/实例223–236；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=BOUNDARY；callee identity, not entered（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_alloc]
```c
uslab_s * uslab_alloc(uslab_s * cslp)
{
	uslab_s * nslp = (uslab_s *)malloc(sizeof(uslab_s));
	INIT_LIST_S(&nslp->uslab_list);
```

## E151-039 · uslab_free

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理238–242，定义/实例238–247；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=BOUNDARY；callee identity, not entered（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_free]
```c
void uslab_free(uslab_s * slp)
{
	upage_free(slp->upage_p);

	free(slp->colormap);
```

## E151-040 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理249–253，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=MALLOC；initialization/ret_val and size gate（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
void * malloc(size_t size)
{
	if (!initiated)
		init_uslab();

```

## E151-041 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理254–258，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=MALLOC；initialization/ret_val and size gate（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
	void * ret_val = NULL;
	if (size > CONST_1M)
	{
		// color_printk(RED, WHITE, "Mem required too big\n");
	}
```

## E151-042 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理259–263，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=MALLOC；class selection（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
	else
	{
		// find suitable slab group
		uslab_cache_s *	uscgp = &uslab_cache_groups[0];
		while (size > uscgp->obj_size)
```

## E151-043 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理264–264，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=MALLOC；class selection（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			uscgp = container_of((uscgp->uslabcache_list.next), uslab_cache_s, uslabcache_list);
```

## E151-044 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理268–272，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=MALLOC；used priority, remove and old free classification（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		uslab_s *	uslp = NULL;
		if (uscgp->normal_slab_used.count > 0)
		{
			List_s * uslp_lp = list_header_remove_head(&uscgp->normal_slab_used);
			while (uslp_lp == 0);
```

## E151-045 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理273–277，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=MALLOC；used priority, remove and old free classification（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			
			uslp = container_of(uslp_lp, uslab_s, uslab_list);
			if (uslp->free == 1)
				list_header_add_to_head(&uscgp->normal_slab_full, &uslp->uslab_list);
			else
```

## E151-046 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理278–278，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=MALLOC；used priority, remove and old free classification（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
				list_header_add_to_head(&uscgp->normal_slab_used, &uslp->uslab_list);
```

## E151-047 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理280–284，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=MALLOC；free alternative, head insertion and empty loop（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		else if (uscgp->normal_slab_free.count > 0)
		{
			List_s * uslp_lp = list_header_remove_head(&uscgp->normal_slab_free);
			while (uslp_lp == 0);
			
```

## E151-048 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理285–289，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=MALLOC；free alternative, head insertion and empty loop（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			uslp = container_of(uslp_lp, uslab_s, uslab_list);
			list_header_add_to_head(&uscgp->normal_slab_used, &uslp->uslab_list);
		}
		else
		{
```

## E151-049 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理290–292，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF；render_separator=NONE；覆盖=MALLOC；free alternative, head insertion and empty loop（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			// color_printk(WHITE, RED, "Slab %#x-bytes used out!\n", scgp->obj_size);
			while (1);
		}
```

## E151-050 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理295–299，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=MALLOC；scan and limit gate, offset/ret_val, three counts and set（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		unsigned long obj_idx = bm_get_freebit_idx(uslp->colormap, 0, uslp->total);
		if (obj_idx < uslp->total)
		{
			size_t offset = uscgp->obj_size * obj_idx;
			ret_val = (void *)((size_t)uslp->virt_addr + offset);
```

## E151-051 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理300–304，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=MALLOC；scan and limit gate, offset/ret_val, three counts and set（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			// refresh status of slab
			uslp->free--;
			uscgp->nsobj_free_count--;
			uscgp->nsobj_used_count++;
			bm_set_bit(uslp->colormap, obj_idx);
```

## E151-052 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理305–305，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=MALLOC；scan and limit gate, offset/ret_val, three counts and set（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		}
```

## E151-053 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理308–312，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=MALLOC；refill gate, reserve/call and publication, skipped in given models（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		if (uscgp->normal_slab_free.count < 1)
		{
			uscgp->normal_slab_free.count++;
			uslab_s * new_uslp = uslab_alloc(uslp);
			uscgp->normal_slab_free.count--;
```

## E151-054 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理313–317，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=MALLOC；refill gate, reserve/call and publication, skipped in given models（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			if (new_uslp == NULL)
			{
				// color_printk(RED, WHITE, "Alloc new slab :%#x-bytes failed!\n", scgp->obj_size);
				while (1);
			}
```

## E151-055 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理318–322，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=MALLOC；refill gate, reserve/call and publication, skipped in given models（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			new_uslp->uslabcache_p = uscgp;
			list_header_add_to_head(&uscgp->normal_slab_free, &new_uslp->uslab_list);
			uscgp->normal_slab_total++;
			uscgp->nsobj_free_count += new_uslp->total;
		}
```

## E151-056 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理324–328，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=MALLOC；actual return（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		// unlock_recurs_lock(&slab_alloc_lock);
	}

	return ret_val;
}
```

## E151-057 · free

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理330–334，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；DEBUG wait and NULL early return（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
void free(void * obj_p)
{
	#ifdef DEBUG
		// make sure have init slab
		while (!initiated);
```

## E151-058 · free

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理335–338，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；DEBUG wait and NULL early return（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	#endif

	if (obj_p == NULL)
		return;
```

## E151-059 · free

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理341–345，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；search, owner chain, empty gate and loop（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	uPage_s * upgp = upage_search(obj_p);
	uslab_s * uslp = upgp->uslab_p;
	uslab_cache_s * uscgp = uslp->uslabcache_p;
	// of coures it should not in an empty-slab
	if (uslp->free == uslp->total)
```

## E151-060 · free

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理346–349，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；search, owner chain, empty gate and loop（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	{
		// color_printk(WHITE, RED, "Free obj in a free %d-byte slab!\n", scgp->obj_size);
		while (1);
	}
```

## E151-061 · free

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理351–355，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；slot quotient, assigned gate and loop（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	unsigned long obj_idx = (obj_p - uslp->virt_addr) / uscgp->obj_size;
	if (!bm_get_assigned_bit(uslp->colormap, obj_idx))
	{
		// color_printk(WHITE, RED, "The obj already been freed : %#018lx\n!", obj_p);
		while (1);
```

## E151-062 · free

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理356–356，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=FREE；slot quotient, assigned gate and loop（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	}
```

## E151-063 · free

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理358–361，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；clear and three counters（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	bm_clear_bit(uslp->colormap, obj_idx);
	uslp->free++;
	uscgp->nsobj_free_count++;
	uscgp->nsobj_used_count--;
```

## E151-064 · free

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理364–368，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；updated free comparison and migration（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	if (uslp->free == 1)
	{
		list_del_init(&uslp->uslab_list);
		uscgp->normal_slab_full.count--;
		list_header_add_to_head(&uscgp->normal_slab_used, &uslp->uslab_list);
```

## E151-065 · free

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理369–373，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；updated free comparison and migration（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	}
	else if (uslp->free == 0)
	{
		list_del_init(&uslp->uslab_list);
		uscgp->normal_slab_used.count--;
```

## E151-066 · free

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理374–378，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；updated free comparison and migration（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
		// if slp is base-slab, add it to tail
		if (uslp != uscgp->normal_base_slab_p)
			list_header_add_to_head(&uscgp->normal_slab_free, &uslp->uslab_list);
		else
			list_header_add_to_tail(&uscgp->normal_slab_free, &uslp->uslab_list);
```

## E151-067 · free

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理379–379，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=FREE；updated free comparison and migration（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	}
```

## E151-068 · free

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理381–385，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；reclaim gate and skipped body（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	if (uscgp->normal_slab_free.count > 2)
	{
		uslab_s * tmp_uslp = container_of(uscgp->normal_slab_free.anchor.prev, uslab_s, uslab_list);
		uscgp->normal_slab_free.count--;
		uslab_free(tmp_uslp);
```

## E151-069 · free

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理386–388，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；reclaim gate and skipped body（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
		uscgp->normal_slab_total;
		uscgp->nsobj_free_count -= tmp_uslp->total;
	}
```

## E151-070 · free

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理389–391，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,NONE；render_separator=LF；覆盖=FREE；normal fallthrough（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c

	// unlock_recurs_lock(&slab_alloc_lock);
}
```

## E151-071 · upage_search

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理151–155，定义/实例151–169；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=SEARCH；rounding input and initial return（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]
```c
uPage_s * upage_search(void * vaddr)
{
	uPage_s * ret_val = NULL;
	vaddr = (void *)PAGE_ROUND_DOWN((size_t)vaddr);
	List_s * upg_lp;
```

## E151-072 · upage_search

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理156–160，定义/实例151–169；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=SEARCH；used anchor, iteration, container_of（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]
```c
	for (upg_lp = used_upage_lhdr.anchor.next;
		 upg_lp != &used_upage_lhdr.anchor;
		 upg_lp = upg_lp->next)
	{
		uPage_s * upgp = container_of(upg_lp, uPage_s, upage_list);
```

## E151-073 · upage_search

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理161–165，定义/实例151–169；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=SEARCH；first equality assign/break（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]
```c
		if (upgp->vaddr == vaddr)
		{
			ret_val = upgp;
			break;
		}
```

## E151-074 · upage_search

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理166–169，定义/实例151–169；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=SEARCH；return（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]
```c
	}

	return ret_val;
}
```

## E151-075 · INIT_LIST_S

function_body；myinitramfs/mylib/util/list.c；物理10–14，定义/实例10–14；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；self initialization（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::INIT_LIST_S]
```c
void INIT_LIST_S(List_s * src)
{
	src->prev = src;
	src->next = src;
}
```

## E151-076 · list_insert_prev

function_body；myinitramfs/mylib/util/list.c；物理17–21，定义/实例17–26；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；tail insert helper（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_prev]
```c
void list_insert_prev(List_s * dst, List_s * src)
{
	list_del_init(src);

	src->next = dst;
```

## E151-077 · list_insert_prev

function_body；myinitramfs/mylib/util/list.c；物理22–26，定义/实例17–26；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；tail insert helper（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_prev]
```c
	src->prev = dst->prev;

	src->next->prev = src;
	src->prev->next = src;
}
```

## E151-078 · list_insert_next

function_body；myinitramfs/mylib/util/list.c；物理29–33，定义/实例29–38；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；self deletion and head insert（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_next]
```c
void list_insert_next(List_s * dst, List_s * src)
{
	list_del_init(src);

	src->next = dst->next;
```

## E151-079 · list_insert_next

function_body；myinitramfs/mylib/util/list.c；物理34–38，定义/实例29–38；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；self deletion and head insert（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_next]
```c
	src->prev = dst;

	src->next->prev = src;
	src->prev->next = src;
}
```

## E151-080 · list_del_init

function_body；myinitramfs/mylib/util/list.c；物理40–44，定义/实例40–45；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；neighbor splice and self initialization（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_del_init]
```c
void list_del_init(List_s *src)
{
	src->prev->next = src->next;
	src->next->prev = src->prev;
	INIT_LIST_S(src);
```

## E151-081 · list_del_init

function_body；myinitramfs/mylib/util/list.c；物理45–45，定义/实例40–45；Blob 24f7a214a288；source_line_endings=LF；render_separator=NONE；覆盖=LIST；neighbor splice and self initialization（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_del_init]
```c
}
```

## E151-082 · list_header_add_to_head

function_body；myinitramfs/mylib/util/list.c；物理79–83，定义/实例79–83；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；insert and count++（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_add_to_head]
```c
void list_header_add_to_head(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_next(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}
```

## E151-083 · list_header_remove_head

function_body；myinitramfs/mylib/util/list.c；物理85–89，定义/实例85–96；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；count, first node, deletion count-- return（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_remove_head]
```c
List_s * list_header_remove_head(List_hdr_s * lhdr_p)
{
	if (lhdr_p->count > 0)
	{
		List_s * ret_val = lhdr_p->anchor.next;
```

## E151-084 · list_header_remove_head

function_body；myinitramfs/mylib/util/list.c；物理90–94，定义/实例85–96；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；count, first node, deletion count-- return（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_remove_head]
```c
		list_del_init(ret_val);
		lhdr_p->count--;
		return ret_val;
	}

```

## E151-085 · list_header_remove_head

function_body；myinitramfs/mylib/util/list.c；物理95–96，定义/实例85–96；Blob 24f7a214a288；source_line_endings=LF,LF；render_separator=NONE；覆盖=LIST；count, first node, deletion count-- return（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_remove_head]
```c
	return NULL;
}
```

## E151-086 · list_header_add_to_tail

function_body；myinitramfs/mylib/util/list.c；物理98–102，定义/实例98–102；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；tail wrapper（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_add_to_tail]
```c
void list_header_add_to_tail(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_prev(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}
```

## E151-087 · bm_get_assigned_bit

function_body；myinitramfs/mylib/util/bitmap.c；物理4–8，定义/实例4–12；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=BITMAP；unsigned return, int literal1 shift, unit read and mask（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_assigned_bit]
```c
unsigned bm_get_assigned_bit(bitmap_t * bm, unsigned long idx)
{
	unsigned ret_val = 0;
	size_t unit_idx = idx / (8 * sizeof(bitmap_t));
	bitmap_t mask = 1 << (idx % (8 * sizeof(bitmap_t)));
```

## E151-088 · bm_get_assigned_bit

function_body；myinitramfs/mylib/util/bitmap.c；物理9–12，定义/实例4–12；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF；render_separator=NONE；覆盖=BITMAP；unsigned return, int literal1 shift, unit read and mask（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_assigned_bit]
```c
	bitmap_t unit = *(bm + unit_idx);
	ret_val = unit & mask;
	return ret_val;
}
```

## E151-089 · bm_set_bit

function_body；myinitramfs/mylib/util/bitmap.c；物理14–17，定义/实例14–17；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF；render_separator=NONE；覆盖=BITMAP；1ULL set OR（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_set_bit]
```c
void bm_set_bit(bitmap_t * bm, unsigned long idx)
{
	*(bm + idx / BITMAP_UNITSIZE) |= (1ULL << (idx % BITMAP_UNITSIZE));
}
```

## E151-090 · bm_clear_bit

function_body；myinitramfs/mylib/util/bitmap.c；物理19–22，定义/实例19–22；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF；render_separator=NONE；覆盖=BITMAP；1ULL clear AND complement（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_clear_bit]
```c
void bm_clear_bit(bitmap_t * bm, unsigned long idx)
{
	*(bm + idx / BITMAP_UNITSIZE) &= ~(1ULL << (idx % BITMAP_UNITSIZE));
}
```

## E151-091 · bm_get_freebit_idx

function_body；myinitramfs/mylib/util/bitmap.c；物理24–28，定义/实例24–34；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=BITMAP；unsigned long loop bound, source word evaluation, 1ULL mask, break and return i（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_freebit_idx]
```c
unsigned long bm_get_freebit_idx(bitmap_t * bm, unsigned long start, unsigned long limit)
{
	unsigned long 	i;
	for (i = start; i < limit; i++)
	{
```

## E151-092 · bm_get_freebit_idx

function_body；myinitramfs/mylib/util/bitmap.c；物理29–33，定义/实例24–34；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=BITMAP；unsigned long loop bound, source word evaluation, 1ULL mask, break and return i（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_freebit_idx]
```c
		bitmap_t bm_unit = *(bm + (i / BITMAP_UNITSIZE));
		if ((bm_unit & (1ULL << i % BITMAP_UNITSIZE)) == 0)
			break;
	}
	return i;
```

## E151-093 · bm_get_freebit_idx

function_body；myinitramfs/mylib/util/bitmap.c；物理34–34，定义/实例24–34；Blob 6a86523c41ba；source_line_endings=NONE；render_separator=LF；覆盖=BITMAP；unsigned long loop bound, source word evaluation, 1ULL mask, break and return i（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_freebit_idx]
```c
}
```

## E151-094 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理39–43，定义/实例39–51；实例=set(CMAKE_C_FLAGS …；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=CONFIG；kernel C flags canary and mabi（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

## E151-095 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理44–48，定义/实例39–51；实例=set(CMAKE_C_FLAGS …；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=CONFIG；kernel C flags canary and mabi（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
	-fno-stack-protector \
	-fno-unwind-tables \
	-static \
	-fdata-sections -ffunction-sections \
	-ggdb \
```

## E151-096 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理49–51，定义/实例39–51；实例=set(CMAKE_C_FLAGS …；Blob 6019436417ad；source_line_endings=LF,LF,LF；render_separator=NONE；覆盖=CONFIG；kernel C flags canary and mabi（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
	-mabi=sysv \
	-mno-red-zone \
")
```

## E151-097 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理58–62，定义/实例58–70；实例=set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \ …；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=CONFIG；active CONFIG text（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

## E151-098 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理63–67，定义/实例58–70；实例=set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \ …；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=CONFIG；active CONFIG text（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
	-DGRUB2_BOOTUP_SUPPORT \
```

## E151-099 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理68–70，定义/实例58–70；实例=set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \ …；Blob 6019436417ad；source_line_endings=LF,LF,LF；render_separator=NONE；覆盖=CONFIG；active CONFIG text（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
	-DCONFIG_HYPERVISOR_GUEST \
	-DCONFIG_KVM_GUEST \
")
```

## E151-100 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理30–34，定义/实例30–36；实例=set(CMAKE_ASM_FLAGS …；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=CONFIG；kernel ASM flags（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
set(CMAKE_ASM_FLAGS
	"${CMAKE_ASM_FLAGS} ${RELEASE_FLAG} ${UNUSED_MACRO} \
	-m64 -mcmodel=kernel -fverbose-asm \
	-fno-pie -fno-pic \
	-D__KERNEL__ \
```

## E151-101 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理35–36，定义/实例30–36；实例=set(CMAKE_ASM_FLAGS …；Blob 6019436417ad；source_line_endings=LF,LF；render_separator=NONE；覆盖=CONFIG；kernel ASM flags（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
	-D__ASSEMBLY__ -DASM_FILE \
")
```

## E151-102 · if

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理16–16，定义/实例16–16；实例=if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")；Blob 6019436417ad；source_line_endings=LF；render_separator=NONE；覆盖=CONFIG；kernel conditional flags（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::if]
```cmake
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
```

## E151-103 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理17–17，定义/实例17–17；实例=set(RELEASE_FLAG "-DDEBUG")；Blob 6019436417ad；source_line_endings=LF；render_separator=NONE；覆盖=CONFIG；kernel conditional flags（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
	set(RELEASE_FLAG "-DDEBUG")
```

## E151-104 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理21–21，定义/实例21–21；实例=set(RELEASE_FLAG "-DRELEASE")；Blob 6019436417ad；source_line_endings=LF；render_separator=NONE；覆盖=CONFIG；kernel conditional flags（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
	set(RELEASE_FLAG "-DRELEASE")
```

## E151-105 · panic

function_body；mykernel/debug/panic.c；物理79–79，定义/实例36–214；Blob 2e9d5b195559；source_line_endings=LF；render_separator=NONE；覆盖=CANARY；time panic canary（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED mykernel/debug/panic.c::panic]
```c
	this_cpu = smp_processor_id();
```

## E151-106 · project

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理3–3，定义/实例3–3；实例=project(MyOS2_Initramfs ASM C)；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；ASM C project（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::project]
```cmake
project(MyOS2_Initramfs ASM C)
```

## E151-107 · if

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理13–13，定义/实例13–13；实例=if (CMAKE_BUILD_TYPE STREQUAL "Debug")；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；user Debug/Release textual condition/assignment（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::if]
```cmake
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
```

## E151-108 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理14–14，定义/实例14–14；实例=set(RELEASE_FLAG "-DDEBUG")；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；user Debug/Release textual condition/assignment（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
	set(RELEASE_FLAG "-DDEBUG")
```

## E151-109 · elseif

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理16–16，定义/实例16–16；实例=elseif ()；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；user Debug/Release textual condition/assignment（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::elseif]
```cmake
elseif ()
```

## E151-110 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理18–18，定义/实例18–18；实例=set(RELEASE_FLAG "-DRELEASE")；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；user Debug/Release textual condition/assignment（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
	set(RELEASE_FLAG "-DRELEASE")
```

## E151-111 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理19–19，定义/实例19–19；实例=set(UNUSED_MACRO "-Wunused-macros")；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；user Debug/Release textual condition/assignment（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
	set(UNUSED_MACRO "-Wunused-macros")
```

## E151-112 · endif

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理20–20，定义/实例20–20；实例=endif()；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；user Debug/Release textual condition/assignment（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::endif]
```cmake
endif()
```

## E151-113 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理22–24，定义/实例22–24；实例=set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${RELEASE_FLAG} \ …；Blob 6757d21bc64c；source_line_endings=LF,LF,LF；render_separator=NONE；覆盖=BUILD；ASM flags（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${RELEASE_FLAG} \
	-m64 \
")
```

## E151-114 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理26–29，定义/实例26–29；实例=set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${RELEASE_FLAG} \ …；Blob 6757d21bc64c；source_line_endings=LF,LF,LF,LF；render_separator=NONE；覆盖=BUILD；C flags（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 \
	-ggdb \
")
```

## E151-115 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理40–40，定义/实例40–40；实例=set(MUSL_PATH /home/cheyh/projs/musl/build)；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；musl configuration paths（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(MUSL_PATH /home/cheyh/projs/musl/build)
```

## E151-116 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理41–41，定义/实例41–41；实例=set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；musl configuration paths（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)
```

## E151-117 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理43–43，定义/实例43–43；实例=set(MUSL_INC_PATH ${MUSL_PATH}/include)；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；musl configuration paths（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(MUSL_INC_PATH ${MUSL_PATH}/include)
```

## E151-118 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理44–44，定义/实例44–44；实例=set(MUSL_LIB_PATH ${MUSL_PATH}/lib)；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；musl configuration paths（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(MUSL_LIB_PATH ${MUSL_PATH}/lib)
```

## E151-119 · include_directories

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理46–46，定义/实例46–46；实例=include_directories("${MUSL_INC_PATH}")；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；include command（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::include_directories]
```cmake
include_directories("${MUSL_INC_PATH}")
```

## E151-120 · add_subdirectory

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理47–47，定义/实例47–47；实例=add_subdirectory(myinit)；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；root subdirectory（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myinit)
```

## E151-121 · add_subdirectory

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理48–48，定义/实例48–48；实例=add_subdirectory(myshell)；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；root subdirectory（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myshell)
```

## E151-122 · install

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理50–50，定义/实例50–50；实例=install(TARGETS init DESTINATION boot)；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；install target（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::install]
```cmake
install(TARGETS init DESTINATION boot)
```

## E151-123 · install

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理51–51，定义/实例51–51；实例=install(TARGETS sh DESTINATION boot)；Blob 6757d21bc64c；source_line_endings=NONE；render_separator=LF；覆盖=BUILD；install target（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/CMakeLists.txt::install]
```cmake
install(TARGETS sh DESTINATION boot)
```

## E151-124 · add_subdirectory

cmake_command_invocation；myinitramfs/mylib/CMakeLists.txt；物理1–1，定义/实例1–1；实例=add_subdirectory(myglibc)；Blob 8f6abc24095a；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；mylib specific subdirectory（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myglibc)
```

## E151-125 · add_subdirectory

cmake_command_invocation；myinitramfs/mylib/CMakeLists.txt；物理2–2，定义/实例2–2；实例=add_subdirectory(csu)；Blob 8f6abc24095a；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；mylib specific subdirectory（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(csu)
```

## E151-126 · add_subdirectory

cmake_command_invocation；myinitramfs/mylib/CMakeLists.txt；物理3–3，定义/实例3–3；实例=add_subdirectory(util)；Blob 8f6abc24095a；source_line_endings=NONE；render_separator=LF；覆盖=BUILD；mylib specific subdirectory（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(util)
```

## E151-127 · add_library

cmake_command_invocation；myinitramfs/mylib/myglibc/CMakeLists.txt；物理1–1，定义/实例1–1；实例=add_library(mylibc STATIC)；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；STATIC library（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_library]
```cmake
add_library(mylibc STATIC)
```

## E151-128 · set

cmake_command_invocation；myinitramfs/mylib/myglibc/CMakeLists.txt；物理2–2，定义/实例2–2；实例=set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH}")；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；candidate C flags（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::set]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH}")
```

## E151-129 · target_include_directories

cmake_command_invocation；myinitramfs/mylib/myglibc/CMakeLists.txt；物理3–7，定义/实例3–16；实例=target_include_directories(mylibc PUBLIC …；Blob b956957a602a；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=BUILD；one complete include invocation（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories]
```cmake
target_include_directories(mylibc PUBLIC
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/include
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sys
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/string
```

## E151-130 · target_include_directories

cmake_command_invocation；myinitramfs/mylib/myglibc/CMakeLists.txt；物理8–12，定义/实例3–16；实例=target_include_directories(mylibc PUBLIC …；Blob b956957a602a；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=BUILD；one complete include invocation（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories]
```cmake
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/misc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/x86
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/generic
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/gnu
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux
```

## E151-131 · target_include_directories

cmake_command_invocation；myinitramfs/mylib/myglibc/CMakeLists.txt；物理13–16，定义/实例3–16；实例=target_include_directories(mylibc PUBLIC …；Blob b956957a602a；source_line_endings=LF,LF,LF,LF；render_separator=NONE；覆盖=BUILD；one complete include invocation（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories]
```cmake
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux/x86
                    ${PROJECT_SOURCE_DIR}/../mykernel/include
                    ${PROJECT_SOURCE_DIR}/../mykernel/arch/${ARCH}/include
                    )
```

## E151-132 · add_subdirectory

cmake_command_invocation；myinitramfs/mylib/myglibc/CMakeLists.txt；物理19–19，定义/实例19–19；实例=add_subdirectory(malloc)；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；malloc subdirectory（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(malloc)
```

## E151-133 · file

cmake_command_invocation；myinitramfs/mylib/myglibc/malloc/CMakeLists.txt；物理1–1，定义/实例1–1；实例=file(GLOB LIBC_STDLIB_C_SRCS *.c)；Blob 3b9f22dd9ab9；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；C glob（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::file]
```cmake
file(GLOB LIBC_STDLIB_C_SRCS *.c)
```

## E151-134 · file

cmake_command_invocation；myinitramfs/mylib/myglibc/malloc/CMakeLists.txt；物理2–2，定义/实例2–2；实例=file(GLOB LIBC_STDLIB_ASM_SRCS *.S)；Blob 3b9f22dd9ab9；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；ASM glob（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::file]
```cmake
file(GLOB LIBC_STDLIB_ASM_SRCS *.S)
```

## E151-135 · target_sources

cmake_command_invocation；myinitramfs/mylib/myglibc/malloc/CMakeLists.txt；物理3–3，定义/实例3–3；实例=target_sources(mylibc PUBLIC ${LIBC_STDLIB_C_SRCS} ${LIBC_STDLIB_ASM_SRCS})；Blob 3b9f22dd9ab9；source_line_endings=NONE；render_separator=LF；覆盖=BUILD；consume both variables（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::target_sources]
```cmake
target_sources(mylibc PUBLIC ${LIBC_STDLIB_C_SRCS} ${LIBC_STDLIB_ASM_SRCS})
```

## E151-136 · file

cmake_command_invocation；myinitramfs/mylib/util/CMakeLists.txt；物理1–1，定义/实例1–1；实例=file(GLOB UTIL_C_SRCS *.c)；Blob 5fd782a35629；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；C glob（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::file]
```cmake
file(GLOB UTIL_C_SRCS *.c)
```

## E151-137 · file

cmake_command_invocation；myinitramfs/mylib/util/CMakeLists.txt；物理2–2，定义/实例2–2；实例=file(GLOB UTIL_ASM_SRCS *.S)；Blob 5fd782a35629；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；ASM glob（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::file]
```cmake
file(GLOB UTIL_ASM_SRCS *.S)
```

## E151-138 · target_sources

cmake_command_invocation；myinitramfs/mylib/util/CMakeLists.txt；物理3–3，定义/实例3–3；实例=target_sources(mylibc PUBLIC ${UTIL_C_SRCS} ${UTIL_ASM_SRCS})；Blob 5fd782a35629；source_line_endings=NONE；render_separator=LF；覆盖=BUILD；consume both variables（本窗只支持所示语句；同定义分窗合读）。

[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::target_sources]
```cmake
target_sources(mylibc PUBLIC ${UTIL_C_SRCS} ${UTIL_ASM_SRCS})
```
