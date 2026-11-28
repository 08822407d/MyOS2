---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
batch_id: "CORE-USER-FREE-REINIT-153"
conversation_display_name: "MYOS2-A-C11a 代码分析执行"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-25"
produced_by: "GPT-6.2（系统标识）"
model_per_owner: "GPT-6.2 Sol＋Extra／高推理（Owner指定）"
actual_backend_model: "UNATTESTED"
actual_reasoning_effort: "UNATTESTED"
date: "2026-11-26"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
record_type: "batch153_source_evidence"
evidence_class: "SOURCE_FACT"
inputs_read: ["myinitramfs/mylib/myglibc/malloc/malloc.c","myinitramfs/mylib/myglibc/malloc/_malloc.h","myinitramfs/mylib/include/utils.h","myinitramfs/mylib/util/list.c","myinitramfs/mylib/util/bitmap.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","myinitramfs/CMakeLists.txt","myinitramfs/mylib/CMakeLists.txt","myinitramfs/mylib/myglibc/CMakeLists.txt","myinitramfs/mylib/myglibc/malloc/CMakeLists.txt","myinitramfs/mylib/util/CMakeLists.txt"]
status: "AUTHOR_PREPUBLICATION"
read_channel: "connector"
full_local_P9: "NOT_RUN"
open_questions: ["实际ABI、标准、DEBUG、链接、对象/运行与外证UNKNOWN"]
---

# 第153批正式源码证据

新窗从固定time独立取证，不复制旧160账。每窗1–5物理行，CRLF/LF原样；NONE仅附一个可逆展示LF。各命令实例范围单独登记；所示源不等于给定模型中的实际执行。源码索引见facts。

## E153-001 · CONST_2M

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理15–15，定义/实例15–15；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；coverage=ROUND_ROOT；实际局部宏/状态或used页根。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::CONST_2M]
```c
	#define CONST_2M		0x200000ULL
```

## E153-002 · __round_mask

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理36–36，定义/实例36–36；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；coverage=ROUND_ROOT；实际局部宏/状态或used页根。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::__round_mask]
```c
#define __round_mask(x, y) ((__typeof__(x))((y)-1))
```

## E153-003 · round_down

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理56–56，定义/实例56–56；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；coverage=ROUND_ROOT；实际局部宏/状态或used页根。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::round_down]
```c
#define round_down(x, y) ((x) & ~__round_mask(x, y))
```

## E153-004 · PAGE_ROUND_DOWN

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理59–59，定义/实例59–59；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；coverage=ROUND_ROOT；实际局部宏/状态或used页根。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::PAGE_ROUND_DOWN]
```c
#define PAGE_ROUND_DOWN(x)	round_down((x), PAGE_SIZE)
```

## E153-005 · PAGE_SIZE

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理60–60，定义/实例60–60；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；coverage=ROUND_ROOT；实际局部宏/状态或used页根。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::PAGE_SIZE]
```c
#define PAGE_SIZE			CONST_2M
```

## E153-006 · initiated

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理62–62，定义/实例62–62；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；coverage=ROUND_ROOT；实际局部宏/状态或used页根。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::initiated]
```c
bool initiated = false;
```

## E153-007 · used_upage_lhdr

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理70–70，定义/实例70–70；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；coverage=ROUND_ROOT；实际局部宏/状态或used页根。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::used_upage_lhdr]
```c
List_hdr_s		used_upage_lhdr;
```

## E153-008 · upage_search

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理151–155，定义/实例151–169；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=SEARCH；ret初值与向下对齐键。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]
```c
uPage_s * upage_search(void * vaddr)
{
	uPage_s * ret_val = NULL;
	vaddr = (void *)PAGE_ROUND_DOWN((size_t)vaddr);
	List_s * upg_lp;
```

## E153-009 · upage_search

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理156–160，定义/实例151–169；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=SEARCH；首读anchor.next、anchor终止、next推进和container_of。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]
```c
	for (upg_lp = used_upage_lhdr.anchor.next;
		 upg_lp != &used_upage_lhdr.anchor;
		 upg_lp = upg_lp->next)
	{
		uPage_s * upgp = container_of(upg_lp, uPage_s, upage_list);
```

## E153-010 · upage_search

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理161–165，定义/实例151–169；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=SEARCH；候选vaddr比较、命中保存并break。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]
```c
		if (upgp->vaddr == vaddr)
		{
			ret_val = upgp;
			break;
		}
```

## E153-011 · upage_search

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理166–169，定义/实例151–169；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=SEARCH；正常返回ret_val，miss保留NULL。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]
```c
	}

	return ret_val;
}
```

## E153-012 · malloc

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理298–302，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=IMPORT_BOUNDARY；旧模型局部地址与计数、set所在源；本批仅导入非局部字段，不执行malloc。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			size_t offset = uscgp->obj_size * obj_idx;
			ret_val = (void *)((size_t)uslp->virt_addr + offset);
			// refresh status of slab
			uslp->free--;
			uscgp->nsobj_free_count--;
```

## E153-013 · malloc

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理303–304，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF；render_separator=NONE；coverage=IMPORT_BOUNDARY；旧模型局部地址与计数、set所在源；本批仅导入非局部字段，不执行malloc。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			uscgp->nsobj_used_count++;
			bm_set_bit(uslp->colormap, obj_idx);
```

## E153-014 · malloc

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理308–311，定义/实例249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=IMPORT_BOUNDARY；U_S未导入310临时count及311调用；本批非原malloc控制续接。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		if (uscgp->normal_slab_free.count < 1)
		{
			uscgp->normal_slab_free.count++;
			uslab_s * new_uslp = uslab_alloc(uslp);
```

## E153-015 · free

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理330–334，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=FREE_ENTRY；DEBUG条件块在NULL门前；实际预处理未知。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
void free(void * obj_p)
{
	#ifdef DEBUG
		// make sure have init slab
		while (!initiated);
```

## E153-016 · free

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理335–335，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；coverage=FREE_ENTRY；DEBUG条件块在NULL门前；实际预处理未知。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	#endif
```

## E153-017 · free

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理337–338，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF；render_separator=NONE；coverage=FREE_ENTRY；输入非NULL，早返门假。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	if (obj_p == NULL)
		return;
```

## E153-018 · free

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理340–343，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=FREE_OWNER；search调用及返回后两级对象读取。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	// find which slab does the pointer belonged to
	uPage_s * upgp = upage_search(obj_p);
	uslab_s * uslp = upgp->uslab_p;
	uslab_cache_s * uscgp = uslp->uslabcache_p;
```

## E153-019 · free

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理344–348，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=FREE_EMPTY；先判free==total，真进入while(1)。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	// of coures it should not in an empty-slab
	if (uslp->free == uslp->total)
	{
		// color_printk(WHITE, RED, "Free obj in a free %d-byte slab!\n", scgp->obj_size);
		while (1);
```

## E153-020 · free

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理349–352，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=FREE_SLOT；仅C1到达槽商与assigned门。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	}

	unsigned long obj_idx = (obj_p - uslp->virt_addr) / uscgp->obj_size;
	if (!bm_get_assigned_bit(uslp->colormap, obj_idx))
```

## E153-021 · free

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理353–356，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=FREE_SLOT；assigned拒绝体未执行。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	{
		// color_printk(WHITE, RED, "The obj already been freed : %#018lx\n!", obj_p);
		while (1);
	}
```

## E153-022 · free

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理357–361，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=FREE_UPDATE；clear在三个计数字段写之前。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c

	bm_clear_bit(uslp->colormap, obj_idx);
	uslp->free++;
	uscgp->nsobj_free_count++;
	uscgp->nsobj_used_count--;
```

## E153-023 · free

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理362–366，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=FREE_CLASS；更新后free==1门及未达摘链。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	// if the corresponding slab in full list, move it to used list
	// or if it is in used list and only use one slot, move it to free list
	if (uslp->free == 1)
	{
		list_del_init(&uslp->uslab_list);
```

## E153-024 · free

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理367–369，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF；render_separator=NONE；coverage=FREE_CLASS；未达full计数和used插入。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
		uscgp->normal_slab_full.count--;
		list_header_add_to_head(&uscgp->normal_slab_used, &uslp->uslab_list);
	}
```

## E153-025 · free

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理370–374，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=FREE_CLASS；更新后free==0门；未达摘链/used减计数。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	else if (uslp->free == 0)
	{
		list_del_init(&uslp->uslab_list);
		uscgp->normal_slab_used.count--;
		// if slp is base-slab, add it to tail
```

## E153-026 · free

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理375–379，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=FREE_CLASS；未达base选择和free插入。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
		if (uslp != uscgp->normal_base_slab_p)
			list_header_add_to_head(&uscgp->normal_slab_free, &uslp->uslab_list);
		else
			list_header_add_to_tail(&uscgp->normal_slab_free, &uslp->uslab_list);
	}
```

## E153-027 · free

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理380–384，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=FREE_RECLAIM；回收门>2及未达尾节点选择。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	// if there is too many free slab, free some of them
	if (uscgp->normal_slab_free.count > 2)
	{
		uslab_s * tmp_uslp = container_of(uscgp->normal_slab_free.anchor.prev, uslab_s, uslab_list);
		uscgp->normal_slab_free.count--;
```

## E153-028 · free

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理385–389，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；coverage=FREE_RECLAIM；回收调用和后继计数未达。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
		uslab_free(tmp_uslp);
		uscgp->normal_slab_total;
		uscgp->nsobj_free_count -= tmp_uslp->total;
	}

```

## E153-029 · free

definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理390–391，定义/实例330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,NONE；render_separator=LF；coverage=FREE_RETURN；C1条件性正常到函数尾；NONE可逆分隔。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	// unlock_recurs_lock(&slab_alloc_lock);
}
```

## E153-030 · USLAB_LEVEL

definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理7–7，定义/实例7–7；Blob 2dea1f8fc05c；source_line_endings=LF；render_separator=NONE；coverage=TYPES；实际常量/类型成员与绑定。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::USLAB_LEVEL]
```c
	#define USLAB_LEVEL			16
```

## E153-031 · USLAB_SIZE_BASE

definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理8–8，定义/实例8–8；Blob 2dea1f8fc05c；source_line_endings=LF；render_separator=NONE；coverage=TYPES；实际常量/类型成员与绑定。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::USLAB_SIZE_BASE]
```c
	#define USLAB_SIZE_BASE		32
```

## E153-032 · uPage_s

definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理19–23，定义/实例19–24；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=TYPES；实际常量/类型成员与绑定。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage_s]
```c
	typedef struct uPage
	{
		List_s	upage_list;
		void	*vaddr;
		uslab_s *uslab_p;
```

## E153-033 · uPage_s

definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理24–24，定义/实例19–24；Blob 2dea1f8fc05c；source_line_endings=LF；render_separator=NONE；coverage=TYPES；实际常量/类型成员与绑定。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage_s]
```c
	} uPage_s;
```

## E153-034 · uslab_s

definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理26–30，定义/实例26–37；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=TYPES；实际常量/类型成员与绑定。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]
```c
	typedef struct uslab
	{
		List_s			uslab_list;
		uslab_cache_s	*uslabcache_p;

```

## E153-035 · uslab_s

definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理31–35，定义/实例26–37；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=TYPES；实际常量/类型成员与绑定。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]
```c
		unsigned long	total;
		unsigned long	free;

		uPage_s			*upage_p;
		void			*virt_addr;
```

## E153-036 · uslab_s

definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理36–37，定义/实例26–37；Blob 2dea1f8fc05c；source_line_endings=LF,LF；render_separator=NONE；coverage=TYPES；实际常量/类型成员与绑定。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]
```c
		bitmap_t		*colormap;
	} uslab_s;
```

## E153-037 · uslab_cache_s

definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理39–43，定义/实例39–54；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=TYPES；实际常量/类型成员与绑定。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c
	typedef struct uslab_cache
	{
		List_s			uslabcache_list;

		unsigned long	obj_size;
```

## E153-038 · uslab_cache_s

definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理44–48，定义/实例39–54；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=TYPES；实际常量/类型成员与绑定。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c

		unsigned long	normal_slab_total;
		List_hdr_s		normal_slab_free;
		List_hdr_s		normal_slab_used;
		List_hdr_s		normal_slab_full;
```

## E153-039 · uslab_cache_s

definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理49–53，定义/实例39–54；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=TYPES；实际常量/类型成员与绑定。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c
		unsigned long	nsobj_free_count;
		unsigned long	nsobj_used_count;

		// the base slab should not be freed
		uslab_s			*normal_base_slab_p;
```

## E153-040 · uslab_cache_s

definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理54–54，定义/实例39–54；Blob 2dea1f8fc05c；source_line_endings=LF；render_separator=NONE；coverage=TYPES；实际常量/类型成员与绑定。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c
	} uslab_cache_s;
```

## E153-041 · container_of

definition；myinitramfs/mylib/include/utils.h；物理12–16，定义/实例12–17；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=TYPES；宏转换、位宽/单位及双向节点与header。

[VERIFIED myinitramfs/mylib/include/utils.h::container_of]
```c
	#define container_of(member_ptr, container_type, member_name)		\
	({																	\
		typeof(((container_type *)0)->member_name) * p = (member_ptr);	\
		(container_type *)((unsigned long)p - 							\
		(unsigned long)&(((container_type *)0)->member_name));			\
```

## E153-042 · container_of

definition；myinitramfs/mylib/include/utils.h；物理17–17，定义/实例12–17；Blob 4e3e8910ac00；source_line_endings=LF；render_separator=NONE；coverage=TYPES；宏转换、位宽/单位及双向节点与header。

[VERIFIED myinitramfs/mylib/include/utils.h::container_of]
```c
	})
```

## E153-043 · bitmap_t

definition；myinitramfs/mylib/include/utils.h；物理19–19，定义/实例19–19；Blob 4e3e8910ac00；source_line_endings=LF；render_separator=NONE；coverage=TYPES；宏转换、位宽/单位及双向节点与header。

[VERIFIED myinitramfs/mylib/include/utils.h::bitmap_t]
```c
	typedef __UINT64_TYPE__ bitmap_t;
```

## E153-044 · BITMAP_UNITSIZE

definition；myinitramfs/mylib/include/utils.h；物理20–20，定义/实例20–20；Blob 4e3e8910ac00；source_line_endings=LF；render_separator=NONE；coverage=TYPES；宏转换、位宽/单位及双向节点与header。

[VERIFIED myinitramfs/mylib/include/utils.h::BITMAP_UNITSIZE]
```c
	#define BITMAP_UNITSIZE (sizeof(bitmap_t) * 8)
```

## E153-045 · List_s

definition；myinitramfs/mylib/include/utils.h；物理35–39，定义/实例35–39；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=TYPES；宏转换、位宽/单位及双向节点与header。

[VERIFIED myinitramfs/mylib/include/utils.h::List_s]
```c
	typedef struct List
	{
		List_s *	prev;
		List_s *	next;
	} List_s;
```

## E153-046 · List_hdr_s

definition；myinitramfs/mylib/include/utils.h；物理41–45，定义/实例41–45；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=TYPES；宏转换、位宽/单位及双向节点与header。

[VERIFIED myinitramfs/mylib/include/utils.h::List_hdr_s]
```c
	typedef struct List_hdr
	{
		List_s			anchor;
		unsigned long	count;
	} List_hdr_s;
```

## E153-047 · INIT_LIST_S

definition；myinitramfs/mylib/util/list.c；物理10–14，定义/实例10–14；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=LIST_BACKGROUND；链表助手实际定义，仅背景/未达路径；本批未调用。

[VERIFIED myinitramfs/mylib/util/list.c::INIT_LIST_S]
```c
void INIT_LIST_S(List_s * src)
{
	src->prev = src;
	src->next = src;
}
```

## E153-048 · list_insert_prev

definition；myinitramfs/mylib/util/list.c；物理17–21，定义/实例17–26；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=LIST_BACKGROUND；链表助手实际定义，仅背景/未达路径；本批未调用。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_prev]
```c
void list_insert_prev(List_s * dst, List_s * src)
{
	list_del_init(src);

	src->next = dst;
```

## E153-049 · list_insert_prev

definition；myinitramfs/mylib/util/list.c；物理22–26，定义/实例17–26；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=LIST_BACKGROUND；链表助手实际定义，仅背景/未达路径；本批未调用。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_prev]
```c
	src->prev = dst->prev;

	src->next->prev = src;
	src->prev->next = src;
}
```

## E153-050 · list_insert_next

definition；myinitramfs/mylib/util/list.c；物理29–33，定义/实例29–38；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=LIST_BACKGROUND；链表助手实际定义，仅背景/未达路径；本批未调用。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_next]
```c
void list_insert_next(List_s * dst, List_s * src)
{
	list_del_init(src);

	src->next = dst->next;
```

## E153-051 · list_insert_next

definition；myinitramfs/mylib/util/list.c；物理34–38，定义/实例29–38；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=LIST_BACKGROUND；链表助手实际定义，仅背景/未达路径；本批未调用。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_next]
```c
	src->prev = dst;

	src->next->prev = src;
	src->prev->next = src;
}
```

## E153-052 · list_del_init

definition；myinitramfs/mylib/util/list.c；物理40–44，定义/实例40–45；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=LIST_BACKGROUND；链表助手实际定义，仅背景/未达路径；本批未调用。

[VERIFIED myinitramfs/mylib/util/list.c::list_del_init]
```c
void list_del_init(List_s *src)
{
	src->prev->next = src->next;
	src->next->prev = src->prev;
	INIT_LIST_S(src);
```

## E153-053 · list_del_init

definition；myinitramfs/mylib/util/list.c；物理45–45，定义/实例40–45；Blob 24f7a214a288；source_line_endings=LF；render_separator=NONE；coverage=LIST_BACKGROUND；链表助手实际定义，仅背景/未达路径；本批未调用。

[VERIFIED myinitramfs/mylib/util/list.c::list_del_init]
```c
}
```

## E153-054 · INIT_LIST_HEADER_S

definition；myinitramfs/mylib/util/list.c；物理73–77，定义/实例73–77；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=LIST_BACKGROUND；链表助手实际定义，仅背景/未达路径；本批未调用。

[VERIFIED myinitramfs/mylib/util/list.c::INIT_LIST_HEADER_S]
```c
void INIT_LIST_HEADER_S(List_hdr_s * lhdr_p)
{
	lhdr_p->count = 0;
	INIT_LIST_S(&lhdr_p->anchor);
}
```

## E153-055 · list_header_add_to_head

definition；myinitramfs/mylib/util/list.c；物理79–83，定义/实例79–83；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=LIST_BACKGROUND；链表助手实际定义，仅背景/未达路径；本批未调用。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_add_to_head]
```c
void list_header_add_to_head(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_next(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}
```

## E153-056 · list_header_add_to_tail

definition；myinitramfs/mylib/util/list.c；物理98–102，定义/实例98–102；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=LIST_BACKGROUND；链表助手实际定义，仅背景/未达路径；本批未调用。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_add_to_tail]
```c
void list_header_add_to_tail(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_prev(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}
```

## E153-057 · bm_get_assigned_bit

definition；myinitramfs/mylib/util/bitmap.c；物理4–8，定义/实例4–12；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=ASSIGNED；低位idx1的unit_idx和int mask表达式。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_assigned_bit]
```c
unsigned bm_get_assigned_bit(bitmap_t * bm, unsigned long idx)
{
	unsigned ret_val = 0;
	size_t unit_idx = idx / (8 * sizeof(bitmap_t));
	bitmap_t mask = 1 << (idx % (8 * sizeof(bitmap_t)));
```

## E153-058 · bm_get_assigned_bit

definition；myinitramfs/mylib/util/bitmap.c；物理9–12，定义/实例4–12；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF；render_separator=NONE；coverage=ASSIGNED；unit & mask赋unsigned并返回2（在C1 GIVEN下）。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_assigned_bit]
```c
	bitmap_t unit = *(bm + unit_idx);
	ret_val = unit & mask;
	return ret_val;
}
```

## E153-059 · bm_clear_bit

definition；myinitramfs/mylib/util/bitmap.c；物理19–22，定义/实例19–22；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF；render_separator=NONE；coverage=CLEAR；64位1ULL移位取反并清单word。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_clear_bit]
```c
void bm_clear_bit(bitmap_t * bm, unsigned long idx)
{
	*(bm + idx / BITMAP_UNITSIZE) &= ~(1ULL << (idx % BITMAP_UNITSIZE));
}
```

## E153-060 · set(CMAKE_C_FLAGS)

cmake_command；mykernel/scripts/options_flags.cmake；物理41–41，定义/实例39–51；Blob 6019436417ad；source_line_endings=LF；render_separator=NONE；coverage=CANARIES；time内核flags canary，仅分支辨识。

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
```

## E153-061 · panic

definition；mykernel/debug/panic.c；物理79–79，定义/实例36–214；Blob 2e9d5b195559；source_line_endings=LF；render_separator=NONE；coverage=CANARIES；time panic canary，仅分支辨识。

[VERIFIED mykernel/debug/panic.c::panic]
```c
	this_cpu = smp_processor_id();
```

## E153-062 · cmake_minimum_required

cmake_command；myinitramfs/CMakeLists.txt；物理1–1，定义/实例1–1；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::cmake_minimum_required]
```cmake
cmake_minimum_required(VERSION 3.16)
```

## E153-063 · set

cmake_command；myinitramfs/CMakeLists.txt；物理2–2，定义/实例2–2；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
```

## E153-064 · project

cmake_command；myinitramfs/CMakeLists.txt；物理3–3，定义/实例3–3；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::project]
```cmake
project(MyOS2_Initramfs ASM C)
```

## E153-065 · set

cmake_command；myinitramfs/CMakeLists.txt；物理6–6，定义/实例6–6；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(PROMPT "======>>>")
```

## E153-066 · set

cmake_command；myinitramfs/CMakeLists.txt；物理7–7，定义/实例7–7；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(PROMPT_END "<<<======")
```

## E153-067 · if

cmake_command；myinitramfs/CMakeLists.txt；物理13–13，定义/实例13–13；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::if]
```cmake
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
```

## E153-068 · set

cmake_command；myinitramfs/CMakeLists.txt；物理14–14，定义/实例14–14；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
	set(RELEASE_FLAG "-DDEBUG")
```

## E153-069 · message

cmake_command；myinitramfs/CMakeLists.txt；物理15–15，定义/实例15–15；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::message]
```cmake
	message("${PROMPT} DEBUG ${PROMPT_END}")
```

## E153-070 · elseif

cmake_command；myinitramfs/CMakeLists.txt；物理16–16，定义/实例16–16；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::elseif]
```cmake
elseif ()
```

## E153-071 · message

cmake_command；myinitramfs/CMakeLists.txt；物理17–17，定义/实例17–17；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::message]
```cmake
	message("${PROMPT} RELEASE")
```

## E153-072 · set

cmake_command；myinitramfs/CMakeLists.txt；物理18–18，定义/实例18–18；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
	set(RELEASE_FLAG "-DRELEASE")
```

## E153-073 · set

cmake_command；myinitramfs/CMakeLists.txt；物理19–19，定义/实例19–19；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
	set(UNUSED_MACRO "-Wunused-macros")
```

## E153-074 · endif

cmake_command；myinitramfs/CMakeLists.txt；物理20–20，定义/实例20–20；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::endif]
```cmake
endif()
```

## E153-075 · set

cmake_command；myinitramfs/CMakeLists.txt；物理22–24，定义/实例22–24；Blob 6757d21bc64c；source_line_endings=LF,LF,LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${RELEASE_FLAG} \
	-m64 \
")
```

## E153-076 · set

cmake_command；myinitramfs/CMakeLists.txt；物理26–29，定义/实例26–29；Blob 6757d21bc64c；source_line_endings=LF,LF,LF,LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 \
	-ggdb \
")
```

## E153-077 · if

cmake_command；myinitramfs/CMakeLists.txt；物理31–31，定义/实例31–31；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::if]
```cmake
if (${LINK_TYPE} STREQUAL "static")
```

## E153-078 · set

cmake_command；myinitramfs/CMakeLists.txt；物理32–32，定义/实例32–32；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
```

## E153-079 · endif

cmake_command；myinitramfs/CMakeLists.txt；物理33–33，定义/实例33–33；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::endif]
```cmake
endif()
```

## E153-080 · set

cmake_command；myinitramfs/CMakeLists.txt；物理38–38，定义/实例38–38；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}) # Force generate binaries to ${CMAKE_BINARY_DIR}
```

## E153-081 · set

cmake_command；myinitramfs/CMakeLists.txt；物理40–40，定义/实例40–40；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(MUSL_PATH /home/cheyh/projs/musl/build)
```

## E153-082 · set

cmake_command；myinitramfs/CMakeLists.txt；物理41–41，定义/实例41–41；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)
```

## E153-083 · set

cmake_command；myinitramfs/CMakeLists.txt；物理43–43，定义/实例43–43；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(MUSL_INC_PATH ${MUSL_PATH}/include)
```

## E153-084 · set

cmake_command；myinitramfs/CMakeLists.txt；物理44–44，定义/实例44–44；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(MUSL_LIB_PATH ${MUSL_PATH}/lib)
```

## E153-085 · include_directories

cmake_command；myinitramfs/CMakeLists.txt；物理46–46，定义/实例46–46；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::include_directories]
```cmake
include_directories("${MUSL_INC_PATH}")
```

## E153-086 · add_subdirectory

cmake_command；myinitramfs/CMakeLists.txt；物理47–47，定义/实例47–47；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myinit)
```

## E153-087 · add_subdirectory

cmake_command；myinitramfs/CMakeLists.txt；物理48–48，定义/实例48–48；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myshell)
```

## E153-088 · install

cmake_command；myinitramfs/CMakeLists.txt；物理50–50，定义/实例50–50；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::install]
```cmake
install(TARGETS init DESTINATION boot)
```

## E153-089 · install

cmake_command；myinitramfs/CMakeLists.txt；物理51–51，定义/实例51–51；Blob 6757d21bc64c；source_line_endings=NONE；render_separator=LF；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/CMakeLists.txt::install]
```cmake
install(TARGETS sh DESTINATION boot)
```

## E153-090 · add_subdirectory

cmake_command；myinitramfs/mylib/CMakeLists.txt；物理1–1，定义/实例1–1；Blob 8f6abc24095a；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myglibc)
```

## E153-091 · add_subdirectory

cmake_command；myinitramfs/mylib/CMakeLists.txt；物理2–2，定义/实例2–2；Blob 8f6abc24095a；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(csu)
```

## E153-092 · add_subdirectory

cmake_command；myinitramfs/mylib/CMakeLists.txt；物理3–3，定义/实例3–3；Blob 8f6abc24095a；source_line_endings=NONE；render_separator=LF；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(util)
```

## E153-093 · add_library

cmake_command；myinitramfs/mylib/myglibc/CMakeLists.txt；物理1–1，定义/实例1–1；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_library]
```cmake
add_library(mylibc STATIC)
```

## E153-094 · set

cmake_command；myinitramfs/mylib/myglibc/CMakeLists.txt；物理2–2，定义/实例2–2；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::set]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH}")
```

## E153-095 · target_include_directories

cmake_command；myinitramfs/mylib/myglibc/CMakeLists.txt；物理3–7，定义/实例3–16；Blob b956957a602a；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories]
```cmake
target_include_directories(mylibc PUBLIC
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/include
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sys
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/string
```

## E153-096 · target_include_directories

cmake_command；myinitramfs/mylib/myglibc/CMakeLists.txt；物理8–12，定义/实例3–16；Blob b956957a602a；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories]
```cmake
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/misc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/x86
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/generic
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/gnu
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux
```

## E153-097 · target_include_directories

cmake_command；myinitramfs/mylib/myglibc/CMakeLists.txt；物理13–16，定义/实例3–16；Blob b956957a602a；source_line_endings=LF,LF,LF,LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories]
```cmake
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux/x86
                    ${PROJECT_SOURCE_DIR}/../mykernel/include
                    ${PROJECT_SOURCE_DIR}/../mykernel/arch/${ARCH}/include
                    )
```

## E153-098 · add_subdirectory

cmake_command；myinitramfs/mylib/myglibc/CMakeLists.txt；物理18–18，定义/实例18–18；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(stdio)
```

## E153-099 · add_subdirectory

cmake_command；myinitramfs/mylib/myglibc/CMakeLists.txt；物理19–19，定义/实例19–19；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(malloc)
```

## E153-100 · add_subdirectory

cmake_command；myinitramfs/mylib/myglibc/CMakeLists.txt；物理20–20，定义/实例20–20；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(string)
```

## E153-101 · add_subdirectory

cmake_command；myinitramfs/mylib/myglibc/CMakeLists.txt；物理21–21，定义/实例21–21；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(exit)
```

## E153-102 · add_subdirectory

cmake_command；myinitramfs/mylib/myglibc/CMakeLists.txt；物理23–23，定义/实例23–23；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(syscall)
```

## E153-103 · add_subdirectory

cmake_command；myinitramfs/mylib/myglibc/CMakeLists.txt；物理24–24，定义/实例24–24；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(posix)
```

## E153-104 · add_subdirectory

cmake_command；myinitramfs/mylib/myglibc/CMakeLists.txt；物理25–25，定义/实例25–25；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(sysdeps)
```

## E153-105 · add_subdirectory

cmake_command；myinitramfs/mylib/myglibc/CMakeLists.txt；物理26–26，定义/实例26–26；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(misc)
```

## E153-106 · add_subdirectory

cmake_command；myinitramfs/mylib/myglibc/CMakeLists.txt；物理27–27，定义/实例27–27；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(time)
```

## E153-107 · add_subdirectory

cmake_command；myinitramfs/mylib/myglibc/CMakeLists.txt；物理28–28，定义/实例28–28；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(io)
```

## E153-108 · add_subdirectory

cmake_command；myinitramfs/mylib/myglibc/CMakeLists.txt；物理29–29，定义/实例29–29；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(libio)
```

## E153-109 · file

cmake_command；myinitramfs/mylib/myglibc/malloc/CMakeLists.txt；物理1–1，定义/实例1–1；Blob 3b9f22dd9ab9；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::file]
```cmake
file(GLOB LIBC_STDLIB_C_SRCS *.c)
```

## E153-110 · file

cmake_command；myinitramfs/mylib/myglibc/malloc/CMakeLists.txt；物理2–2，定义/实例2–2；Blob 3b9f22dd9ab9；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::file]
```cmake
file(GLOB LIBC_STDLIB_ASM_SRCS *.S)
```

## E153-111 · target_sources

cmake_command；myinitramfs/mylib/myglibc/malloc/CMakeLists.txt；物理3–3，定义/实例3–3；Blob 3b9f22dd9ab9；source_line_endings=NONE；render_separator=LF；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::target_sources]
```cmake
target_sources(mylibc PUBLIC ${LIBC_STDLIB_C_SRCS} ${LIBC_STDLIB_ASM_SRCS})
```

## E153-112 · file

cmake_command；myinitramfs/mylib/util/CMakeLists.txt；物理1–1，定义/实例1–1；Blob 5fd782a35629；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::file]
```cmake
file(GLOB UTIL_C_SRCS *.c)
```

## E153-113 · file

cmake_command；myinitramfs/mylib/util/CMakeLists.txt；物理2–2，定义/实例2–2；Blob 5fd782a35629；source_line_endings=LF；render_separator=NONE；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::file]
```cmake
file(GLOB UTIL_ASM_SRCS *.S)
```

## E153-114 · target_sources

cmake_command；myinitramfs/mylib/util/CMakeLists.txt；物理3–3，定义/实例3–3；Blob 5fd782a35629；source_line_endings=NONE；render_separator=LF；coverage=BUILD；该实际命令实例文本；不认证真实构建。

[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::target_sources]
```cmake
target_sources(mylibc PUBLIC ${UTIL_C_SRCS} ${UTIL_ASM_SRCS})
```
