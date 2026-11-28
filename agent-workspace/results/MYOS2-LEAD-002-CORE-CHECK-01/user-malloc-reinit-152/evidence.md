---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "SOURCE_EVIDENCE"
evidence_class: "SOURCE_FACT"
produced_by: "GPT-6.2（系统标识）"
model_per_owner: "GPT-6.2 Sol＋Extra／高推理"
actual_client_model: "UNATTESTED"
actual_backend_model: "UNATTESTED"
actual_effort: "UNATTESTED"
phase: 2
base_snapshot: "time@a039d9803ade"
inputs_read: ["myinitramfs/mylib/myglibc/malloc/malloc.c","myinitramfs/mylib/myglibc/malloc/_malloc.h","myinitramfs/mylib/include/utils.h","myinitramfs/mylib/util/list.c","myinitramfs/mylib/util/bitmap.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","myinitramfs/CMakeLists.txt","myinitramfs/mylib/CMakeLists.txt","myinitramfs/mylib/myglibc/CMakeLists.txt","myinitramfs/mylib/myglibc/malloc/CMakeLists.txt","myinitramfs/mylib/util/CMakeLists.txt"]
status: "AUTHOR_PREPUBLICATION"
read_channel: "GitHub connector"
open_questions: ["实际编译、链接、运行及完整本地P9未执行"]
date: "2026-11-26"
full_local_P9: "NOT_RUN"
self_check: {verified_claims: 160, quotes_reconfirmed: 160, downgraded_to_inferred: 0}
---

# 152 固定源码证据

每窗保留源物理行（含 CRLF/LF）；末行 NONE 以单个展示 LF 与闭围栏隔开，不属于源字节。索引见 facts-and-boundaries.yaml。命令标记为实际命令和实例范围，不推导实际构建。

## E152-001

物理行 12–12；定义/实例 12–12；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。尺寸、状态或静态根的实际定义。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::CONST_1M]  
```
	#define CONST_1M		0x100000ULL
```

## E152-002

物理行 15–15；定义/实例 15–15；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。尺寸、状态或静态根的实际定义。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::CONST_2M]  
```
	#define CONST_2M		0x200000ULL
```

## E152-003

物理行 36–36；定义/实例 36–36；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。尺寸、状态或静态根的实际定义。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::__round_mask]  
```
#define __round_mask(x, y) ((__typeof__(x))((y)-1))
```

## E152-004

物理行 56–56；定义/实例 56–56；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。尺寸、状态或静态根的实际定义。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::round_down]  
```
#define round_down(x, y) ((x) & ~__round_mask(x, y))
```

## E152-005

物理行 59–59；定义/实例 59–59；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。尺寸、状态或静态根的实际定义。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::PAGE_ROUND_DOWN]  
```
#define PAGE_ROUND_DOWN(x)	round_down((x), PAGE_SIZE)
```

## E152-006

物理行 60–60；定义/实例 60–60；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。尺寸、状态或静态根的实际定义。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::PAGE_SIZE]  
```
#define PAGE_SIZE			CONST_2M
```

## E152-007

物理行 62–62；定义/实例 62–62；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。尺寸、状态或静态根的实际定义。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::initiated]  
```
bool initiated = false;
```

## E152-008

物理行 63–63；定义/实例 63–63；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。尺寸、状态或静态根的实际定义。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::brk_end]  
```
void *brk_end = NULL;
```

## E152-009

物理行 65–65；定义/实例 65–65；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。尺寸、状态或静态根的实际定义。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslabcache_lhdr]  
```
List_hdr_s		uslabcache_lhdr;
```

## E152-010

物理行 66–66；定义/实例 66–66；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。尺寸、状态或静态根的实际定义。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_cache_groups]  
```
uslab_cache_s	uslab_cache_groups[USLAB_LEVEL];
```

## E152-011

物理行 67–67；定义/实例 67–67；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。尺寸、状态或静态根的实际定义。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::base_uslabs]  
```
uslab_s			base_uslabs[USLAB_LEVEL];
```

## E152-012

物理行 68–68；定义/实例 68–68；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。尺寸、状态或静态根的实际定义。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::base_upages]  
```
uPage_s			base_upages[USLAB_LEVEL];
```

## E152-013

物理行 69–69；定义/实例 69–69；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。尺寸、状态或静态根的实际定义。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free_upage_lhdr]  
```
List_hdr_s		free_upage_lhdr;
```

## E152-014

物理行 70–70；定义/实例 70–70；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。尺寸、状态或静态根的实际定义。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::used_upage_lhdr]  
```
List_hdr_s		used_upage_lhdr;
```

## E152-015

物理行 72–72；定义/实例 72–72；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。静态位图容量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_1MB]  
```
bitmap_t		cm_1MB[1];		// 2		bits
```

## E152-016

物理行 73–73；定义/实例 73–73；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。静态位图容量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_512KB]  
```
bitmap_t		cm_512KB[1];	// 4		bits
```

## E152-017

物理行 74–74；定义/实例 74–74；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。静态位图容量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_256KB]  
```
bitmap_t		cm_256KB[1];	// 8		bits
```

## E152-018

物理行 75–75；定义/实例 75–75；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。静态位图容量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_128KB]  
```
bitmap_t		cm_128KB[1];	// 16		bits
```

## E152-019

物理行 76–76；定义/实例 76–76；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。静态位图容量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_64KB]  
```
bitmap_t		cm_64KB[1];		// 32		bits
```

## E152-020

物理行 77–77；定义/实例 77–77；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。静态位图容量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_32KB]  
```
bitmap_t		cm_32KB[1];		// 64		bits
```

## E152-021

物理行 78–78；定义/实例 78–78；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。静态位图容量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_16KB]  
```
bitmap_t		cm_16KB[2];		// 128		bits
```

## E152-022

物理行 79–79；定义/实例 79–79；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。静态位图容量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_8KB]  
```
bitmap_t		cm_8KB[4];		// 256		bits
```

## E152-023

物理行 80–80；定义/实例 80–80；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。静态位图容量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_4KB]  
```
bitmap_t		cm_4KB[8];		// 512		bits
```

## E152-024

物理行 81–81；定义/实例 81–81；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。静态位图容量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_2KB]  
```
bitmap_t		cm_2KB[16];		// 1024		bits
```

## E152-025

物理行 82–82；定义/实例 82–82；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。静态位图容量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_1KB]  
```
bitmap_t		cm_1KB[32];		// 2048		bits
```

## E152-026

物理行 83–83；定义/实例 83–83；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。静态位图容量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_512B]  
```
bitmap_t		cm_512B[64];	// 4096		bits
```

## E152-027

物理行 84–84；定义/实例 84–84；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。静态位图容量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_256B]  
```
bitmap_t		cm_256B[128];	// 8192		bits
```

## E152-028

物理行 85–85；定义/实例 85–85；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。静态位图容量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_128B]  
```
bitmap_t		cm_128B[256];	// 16384	bits
```

## E152-029

物理行 86–86；定义/实例 86–86；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。静态位图容量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_64B]  
```
bitmap_t		cm_64B[512];	// 32768	bits
```

## E152-030

物理行 87–87；定义/实例 87–87；blob 44db9ea9d7f7；行尾 CRLF；展示分隔 NONE。静态位图容量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_32B]  
```
bitmap_t		cm_32B[1024];	// 65536	bits
```

## E152-031

物理行 89–93；定义/实例 89–106；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。位图按档映射。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::base_colormaps]  
```
bitmap_t *		base_colormaps[USLAB_LEVEL] = {
	cm_32B,
	cm_64B,
	cm_128B,
	cm_256B,
```

## E152-032

物理行 94–98；定义/实例 89–106；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。位图按档映射。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::base_colormaps]  
```
	cm_512B,
	cm_1KB,
	cm_2KB,
	cm_4KB,
	cm_8KB,
```

## E152-033

物理行 99–103；定义/实例 89–106；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。位图按档映射。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::base_colormaps]  
```
	cm_16KB,
	cm_32KB,
	cm_64KB,
	cm_128KB,
	cm_256KB,
```

## E152-034

物理行 104–106；定义/实例 89–106；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF；展示分隔 NONE。位图按档映射。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::base_colormaps]  
```
	cm_512KB,
	cm_1MB,
};
```

## E152-035

物理行 111–115；定义/实例 111–141；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。复用分支及未进入的动态分支。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]  
```
uPage_s *upage_alloc()
{
	uPage_s *ret_val = NULL;
	if (free_upage_lhdr.count > 0)
	{
```

## E152-036

物理行 116–120；定义/实例 111–141；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。复用分支及未进入的动态分支。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]  
```
		List_s * upg_lp = list_header_remove_head(&free_upage_lhdr);
		while (upg_lp == 0);
		
		ret_val = container_of(upg_lp, uPage_s, upage_list);
		list_header_add_to_head(&used_upage_lhdr, &ret_val->upage_list);
```

## E152-037

物理行 121–125；定义/实例 111–141；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。复用分支及未进入的动态分支。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]  
```
	}
	else
	{
		uPage_s *upgp = malloc(sizeof(uPage_s));
		void *pg_vaddr = brk(brk_end + PAGE_SIZE);
```

## E152-038

物理行 140–141；定义/实例 111–141；blob 44db9ea9d7f7；行尾 CRLF,CRLF；展示分隔 NONE。复用正常返回。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]  
```
	return ret_val;
}
```

## E152-039

物理行 151–155；定义/实例 151–169；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。查找、对齐、遍历、命中/未命中正常返回。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]  
```
uPage_s * upage_search(void * vaddr)
{
	uPage_s * ret_val = NULL;
	vaddr = (void *)PAGE_ROUND_DOWN((size_t)vaddr);
	List_s * upg_lp;
```

## E152-040

物理行 156–160；定义/实例 151–169；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。查找、对齐、遍历、命中/未命中正常返回。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]  
```
	for (upg_lp = used_upage_lhdr.anchor.next;
		 upg_lp != &used_upage_lhdr.anchor;
		 upg_lp = upg_lp->next)
	{
		uPage_s * upgp = container_of(upg_lp, uPage_s, upage_list);
```

## E152-041

物理行 161–165；定义/实例 151–169；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。查找、对齐、遍历、命中/未命中正常返回。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]  
```
		if (upgp->vaddr == vaddr)
		{
			ret_val = upgp;
			break;
		}
```

## E152-042

物理行 166–169；定义/实例 151–169；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。查找、对齐、遍历、命中/未命中正常返回。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]  
```
	}

	return ret_val;
}
```

## E152-043

物理行 174–178；定义/实例 174–221；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。再初始化全部有界源码、重置、赋值和调用。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]  
```
void init_uslab()
{
	brk_end = brk(0);
	INIT_LIST_HEADER_S(&uslabcache_lhdr);
	INIT_LIST_HEADER_S(&free_upage_lhdr);
```

## E152-044

物理行 179–183；定义/实例 174–221；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。再初始化全部有界源码、重置、赋值和调用。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]  
```
	INIT_LIST_HEADER_S(&used_upage_lhdr);

	for (int i = 0; i < USLAB_LEVEL; i++)
	{
		uslab_cache_s * scgp = &uslab_cache_groups[i];
```

## E152-045

物理行 184–188；定义/实例 174–221；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。再初始化全部有界源码、重置、赋值和调用。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]  
```
		INIT_LIST_S(&scgp->uslabcache_list);
		list_header_add_to_tail(&uslabcache_lhdr, &scgp->uslabcache_list);

		INIT_LIST_HEADER_S(&scgp->normal_slab_free);
		INIT_LIST_HEADER_S(&scgp->normal_slab_used);
```

## E152-046

物理行 189–193；定义/实例 174–221；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。再初始化全部有界源码、重置、赋值和调用。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]  
```
		INIT_LIST_HEADER_S(&scgp->normal_slab_full);

		uslab_s * bslp = &base_uslabs[i];
		INIT_LIST_S(&bslp->uslab_list);

```

## E152-047

物理行 194–198；定义/实例 174–221；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。再初始化全部有界源码、重置、赋值和调用。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]  
```
		scgp->obj_size = USLAB_SIZE_BASE << i;
		unsigned long obj_nr = PAGE_SIZE / scgp->obj_size;
		unsigned long cm_size = (obj_nr + sizeof(bitmap_t) - 1) / sizeof(bitmap_t);
		// init 3 status of slab list
		list_header_add_to_tail(&scgp->normal_slab_free, &bslp->uslab_list);
```

## E152-048

物理行 199–203；定义/实例 174–221；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。再初始化全部有界源码、重置、赋值和调用。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]  
```
		scgp->normal_base_slab_p = bslp;
		scgp->nsobj_free_count = obj_nr;
		scgp->nsobj_used_count = 0;
		scgp->normal_slab_total = 1;

```

## E152-049

物理行 204–208；定义/实例 174–221；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。再初始化全部有界源码、重置、赋值和调用。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]  
```
		bslp->colormap = base_colormaps[i];
		bslp->uslabcache_p = scgp;
		bslp->free =
		bslp->total = obj_nr;

```

## E152-050

物理行 209–213；定义/实例 174–221；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。再初始化全部有界源码、重置、赋值和调用。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]  
```
		// init upages
		uPage_s * upgp = &base_upages[i];
		INIT_LIST_S(&upgp->upage_list);
		upgp->vaddr =
		brk_end = brk(brk_end + PAGE_SIZE);
```

## E152-051

物理行 214–218；定义/实例 174–221；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。再初始化全部有界源码、重置、赋值和调用。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]  
```
		list_header_add_to_tail(&free_upage_lhdr, &upgp->upage_list);

		bslp->upage_p = upage_alloc();
		bslp->virt_addr = bslp->upage_p->vaddr;
		bslp->upage_p->uslab_p = bslp;
```

## E152-052

物理行 219–221；定义/实例 174–221；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF；展示分隔 NONE。再初始化全部有界源码、重置、赋值和调用。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]  
```
	}
	initiated = true;
}
```

## E152-053

物理行 223–227；定义/实例 223–236；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。311处调用目标定义，仅定位，不展开。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_alloc]  
```
uslab_s * uslab_alloc(uslab_s * cslp)
{
	uslab_s * nslp = (uslab_s *)malloc(sizeof(uslab_s));
	INIT_LIST_S(&nslp->uslab_list);

```

## E152-054

物理行 228–232；定义/实例 223–236；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。311处调用目标定义，仅定位，不展开。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_alloc]  
```
	nslp->colormap = (bitmap_t *)malloc(cslp->total / sizeof(bitmap_t) + sizeof(bitmap_t));
	nslp->total =
	nslp->free = cslp->total;
	nslp->upage_p = upage_alloc();
	nslp->virt_addr = nslp->upage_p->vaddr;
```

## E152-055

物理行 233–236；定义/实例 223–236；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。311处调用目标定义，仅定位，不展开。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_alloc]  
```
	nslp->upage_p->uslab_p = nslp;

	return nslp;
}
```

## E152-056

物理行 249–253；定义/实例 249–328；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。入口、档位、列表、扫描、更新、补充边界及未执行尾部。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]  
```
void * malloc(size_t size)
{
	if (!initiated)
		init_uslab();

```

## E152-057

物理行 254–258；定义/实例 249–328；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。入口、档位、列表、扫描、更新、补充边界及未执行尾部。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]  
```
	void * ret_val = NULL;
	if (size > CONST_1M)
	{
		// color_printk(RED, WHITE, "Mem required too big\n");
	}
```

## E152-058

物理行 259–263；定义/实例 249–328；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。入口、档位、列表、扫描、更新、补充边界及未执行尾部。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]  
```
	else
	{
		// find suitable slab group
		uslab_cache_s *	uscgp = &uslab_cache_groups[0];
		while (size > uscgp->obj_size)
```

## E152-059

物理行 264–268；定义/实例 249–328；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。入口、档位、列表、扫描、更新、补充边界及未执行尾部。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]  
```
			uscgp = container_of((uscgp->uslabcache_list.next), uslab_cache_s, uslabcache_list);

		// find a usable slab and if it is in free list, move it to used list
		// or if it is in used list and has only one free slot, move it to full list
		uslab_s *	uslp = NULL;
```

## E152-060

物理行 269–273；定义/实例 249–328；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。入口、档位、列表、扫描、更新、补充边界及未执行尾部。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]  
```
		if (uscgp->normal_slab_used.count > 0)
		{
			List_s * uslp_lp = list_header_remove_head(&uscgp->normal_slab_used);
			while (uslp_lp == 0);
			
```

## E152-061

物理行 274–278；定义/实例 249–328；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。入口、档位、列表、扫描、更新、补充边界及未执行尾部。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]  
```
			uslp = container_of(uslp_lp, uslab_s, uslab_list);
			if (uslp->free == 1)
				list_header_add_to_head(&uscgp->normal_slab_full, &uslp->uslab_list);
			else
				list_header_add_to_head(&uscgp->normal_slab_used, &uslp->uslab_list);
```

## E152-062

物理行 279–283；定义/实例 249–328；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。入口、档位、列表、扫描、更新、补充边界及未执行尾部。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]  
```
		}
		else if (uscgp->normal_slab_free.count > 0)
		{
			List_s * uslp_lp = list_header_remove_head(&uscgp->normal_slab_free);
			while (uslp_lp == 0);
```

## E152-063

物理行 284–288；定义/实例 249–328；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。入口、档位、列表、扫描、更新、补充边界及未执行尾部。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]  
```
			
			uslp = container_of(uslp_lp, uslab_s, uslab_list);
			list_header_add_to_head(&uscgp->normal_slab_used, &uslp->uslab_list);
		}
		else
```

## E152-064

物理行 289–293；定义/实例 249–328；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。入口、档位、列表、扫描、更新、补充边界及未执行尾部。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]  
```
		{
			// color_printk(WHITE, RED, "Slab %#x-bytes used out!\n", scgp->obj_size);
			while (1);
		}
			
```

## E152-065

物理行 294–298；定义/实例 249–328；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。入口、档位、列表、扫描、更新、补充边界及未执行尾部。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]  
```
		// count the virtual addr of suitable object
		unsigned long obj_idx = bm_get_freebit_idx(uslp->colormap, 0, uslp->total);
		if (obj_idx < uslp->total)
		{
			size_t offset = uscgp->obj_size * obj_idx;
```

## E152-066

物理行 299–303；定义/实例 249–328；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。入口、档位、列表、扫描、更新、补充边界及未执行尾部。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]  
```
			ret_val = (void *)((size_t)uslp->virt_addr + offset);
			// refresh status of slab
			uslp->free--;
			uscgp->nsobj_free_count--;
			uscgp->nsobj_used_count++;
```

## E152-067

物理行 304–308；定义/实例 249–328；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。入口、档位、列表、扫描、更新、补充边界及未执行尾部。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]  
```
			bm_set_bit(uslp->colormap, obj_idx);
		}

		// make sure free slab more than one
		if (uscgp->normal_slab_free.count < 1)
```

## E152-068

物理行 309–313；定义/实例 249–328；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。入口、档位、列表、扫描、更新、补充边界及未执行尾部。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]  
```
		{
			uscgp->normal_slab_free.count++;
			uslab_s * new_uslp = uslab_alloc(uslp);
			uscgp->normal_slab_free.count--;
			if (new_uslp == NULL)
```

## E152-069

物理行 314–318；定义/实例 249–328；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF,CRLF,CRLF；展示分隔 NONE。入口、档位、列表、扫描、更新、补充边界及未执行尾部。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]  
```
			{
				// color_printk(RED, WHITE, "Alloc new slab :%#x-bytes failed!\n", scgp->obj_size);
				while (1);
			}
			new_uslp->uslabcache_p = uscgp;
```

## E152-070

物理行 319–321；定义/实例 249–328；blob 44db9ea9d7f7；行尾 CRLF,CRLF,CRLF；展示分隔 NONE。入口、档位、列表、扫描、更新、补充边界及未执行尾部。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]  
```
			list_header_add_to_head(&uscgp->normal_slab_free, &new_uslp->uslab_list);
			uscgp->normal_slab_total++;
			uscgp->nsobj_free_count += new_uslp->total;
```

## E152-071

物理行 327–328；定义/实例 249–328；blob 44db9ea9d7f7；行尾 CRLF,CRLF；展示分隔 NONE。未达到的正常返回。
[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]  
```
	return ret_val;
}
```

## E152-072

物理行 7–7；定义/实例 7–7；blob 2dea1f8fc05c；行尾 LF；展示分隔 NONE。静态类型/字段及常量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::USLAB_LEVEL]  
```
	#define USLAB_LEVEL			16
```

## E152-073

物理行 8–8；定义/实例 8–8；blob 2dea1f8fc05c；行尾 LF；展示分隔 NONE。静态类型/字段及常量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::USLAB_SIZE_BASE]  
```
	#define USLAB_SIZE_BASE		32
```

## E152-074

物理行 19–23；定义/实例 19–24；blob 2dea1f8fc05c；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。静态类型/字段及常量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage_s]  
```
	typedef struct uPage
	{
		List_s	upage_list;
		void	*vaddr;
		uslab_s *uslab_p;
```

## E152-075

物理行 24–24；定义/实例 19–24；blob 2dea1f8fc05c；行尾 LF；展示分隔 NONE。静态类型/字段及常量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage_s]  
```
	} uPage_s;
```

## E152-076

物理行 26–30；定义/实例 26–37；blob 2dea1f8fc05c；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。静态类型/字段及常量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]  
```
	typedef struct uslab
	{
		List_s			uslab_list;
		uslab_cache_s	*uslabcache_p;

```

## E152-077

物理行 31–35；定义/实例 26–37；blob 2dea1f8fc05c；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。静态类型/字段及常量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]  
```
		unsigned long	total;
		unsigned long	free;

		uPage_s			*upage_p;
		void			*virt_addr;
```

## E152-078

物理行 36–37；定义/实例 26–37；blob 2dea1f8fc05c；行尾 LF,LF；展示分隔 NONE。静态类型/字段及常量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]  
```
		bitmap_t		*colormap;
	} uslab_s;
```

## E152-079

物理行 39–43；定义/实例 39–54；blob 2dea1f8fc05c；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。静态类型/字段及常量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]  
```
	typedef struct uslab_cache
	{
		List_s			uslabcache_list;

		unsigned long	obj_size;
```

## E152-080

物理行 44–48；定义/实例 39–54；blob 2dea1f8fc05c；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。静态类型/字段及常量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]  
```

		unsigned long	normal_slab_total;
		List_hdr_s		normal_slab_free;
		List_hdr_s		normal_slab_used;
		List_hdr_s		normal_slab_full;
```

## E152-081

物理行 49–53；定义/实例 39–54；blob 2dea1f8fc05c；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。静态类型/字段及常量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]  
```
		unsigned long	nsobj_free_count;
		unsigned long	nsobj_used_count;

		// the base slab should not be freed
		uslab_s			*normal_base_slab_p;
```

## E152-082

物理行 54–54；定义/实例 39–54；blob 2dea1f8fc05c；行尾 LF；展示分隔 NONE。静态类型/字段及常量。
[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]  
```
	} uslab_cache_s;
```

## E152-083

物理行 12–16；定义/实例 12–17；blob 4e3e8910ac00；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。转换、位宽和链表结构。
[VERIFIED myinitramfs/mylib/include/utils.h::container_of]  
```
	#define container_of(member_ptr, container_type, member_name)		\
	({																	\
		typeof(((container_type *)0)->member_name) * p = (member_ptr);	\
		(container_type *)((unsigned long)p - 							\
		(unsigned long)&(((container_type *)0)->member_name));			\
```

## E152-084

物理行 17–17；定义/实例 12–17；blob 4e3e8910ac00；行尾 LF；展示分隔 NONE。转换、位宽和链表结构。
[VERIFIED myinitramfs/mylib/include/utils.h::container_of]  
```
	})
```

## E152-085

物理行 19–19；定义/实例 19–19；blob 4e3e8910ac00；行尾 LF；展示分隔 NONE。转换、位宽和链表结构。
[VERIFIED myinitramfs/mylib/include/utils.h::bitmap_t]  
```
	typedef __UINT64_TYPE__ bitmap_t;
```

## E152-086

物理行 20–20；定义/实例 20–20；blob 4e3e8910ac00；行尾 LF；展示分隔 NONE。转换、位宽和链表结构。
[VERIFIED myinitramfs/mylib/include/utils.h::BITMAP_UNITSIZE]  
```
	#define BITMAP_UNITSIZE (sizeof(bitmap_t) * 8)
```

## E152-087

物理行 35–39；定义/实例 35–39；blob 4e3e8910ac00；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。转换、位宽和链表结构。
[VERIFIED myinitramfs/mylib/include/utils.h::List_s]  
```
	typedef struct List
	{
		List_s *	prev;
		List_s *	next;
	} List_s;
```

## E152-088

物理行 41–45；定义/实例 41–45；blob 4e3e8910ac00；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。转换、位宽和链表结构。
[VERIFIED myinitramfs/mylib/include/utils.h::List_hdr_s]  
```
	typedef struct List_hdr
	{
		List_s			anchor;
		unsigned long	count;
	} List_hdr_s;
```

## E152-089

物理行 10–14；定义/实例 10–14；blob 24f7a214a288；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。精确链表写入顺序、读根和计数。
[VERIFIED myinitramfs/mylib/util/list.c::INIT_LIST_S]  
```
void INIT_LIST_S(List_s * src)
{
	src->prev = src;
	src->next = src;
}
```

## E152-090

物理行 17–21；定义/实例 17–26；blob 24f7a214a288；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。精确链表写入顺序、读根和计数。
[VERIFIED myinitramfs/mylib/util/list.c::list_insert_prev]  
```
void list_insert_prev(List_s * dst, List_s * src)
{
	list_del_init(src);

	src->next = dst;
```

## E152-091

物理行 22–26；定义/实例 17–26；blob 24f7a214a288；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。精确链表写入顺序、读根和计数。
[VERIFIED myinitramfs/mylib/util/list.c::list_insert_prev]  
```
	src->prev = dst->prev;

	src->next->prev = src;
	src->prev->next = src;
}
```

## E152-092

物理行 29–33；定义/实例 29–38；blob 24f7a214a288；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。精确链表写入顺序、读根和计数。
[VERIFIED myinitramfs/mylib/util/list.c::list_insert_next]  
```
void list_insert_next(List_s * dst, List_s * src)
{
	list_del_init(src);

	src->next = dst->next;
```

## E152-093

物理行 34–38；定义/实例 29–38；blob 24f7a214a288；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。精确链表写入顺序、读根和计数。
[VERIFIED myinitramfs/mylib/util/list.c::list_insert_next]  
```
	src->prev = dst;

	src->next->prev = src;
	src->prev->next = src;
}
```

## E152-094

物理行 40–44；定义/实例 40–45；blob 24f7a214a288；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。精确链表写入顺序、读根和计数。
[VERIFIED myinitramfs/mylib/util/list.c::list_del_init]  
```
void list_del_init(List_s *src)
{
	src->prev->next = src->next;
	src->next->prev = src->prev;
	INIT_LIST_S(src);
```

## E152-095

物理行 45–45；定义/实例 40–45；blob 24f7a214a288；行尾 LF；展示分隔 NONE。精确链表写入顺序、读根和计数。
[VERIFIED myinitramfs/mylib/util/list.c::list_del_init]  
```
}
```

## E152-096

物理行 73–77；定义/实例 73–77；blob 24f7a214a288；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。精确链表写入顺序、读根和计数。
[VERIFIED myinitramfs/mylib/util/list.c::INIT_LIST_HEADER_S]  
```
void INIT_LIST_HEADER_S(List_hdr_s * lhdr_p)
{
	lhdr_p->count = 0;
	INIT_LIST_S(&lhdr_p->anchor);
}
```

## E152-097

物理行 79–83；定义/实例 79–83；blob 24f7a214a288；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。精确链表写入顺序、读根和计数。
[VERIFIED myinitramfs/mylib/util/list.c::list_header_add_to_head]  
```
void list_header_add_to_head(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_next(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}
```

## E152-098

物理行 85–89；定义/实例 85–96；blob 24f7a214a288；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。精确链表写入顺序、读根和计数。
[VERIFIED myinitramfs/mylib/util/list.c::list_header_remove_head]  
```
List_s * list_header_remove_head(List_hdr_s * lhdr_p)
{
	if (lhdr_p->count > 0)
	{
		List_s * ret_val = lhdr_p->anchor.next;
```

## E152-099

物理行 90–94；定义/实例 85–96；blob 24f7a214a288；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。精确链表写入顺序、读根和计数。
[VERIFIED myinitramfs/mylib/util/list.c::list_header_remove_head]  
```
		list_del_init(ret_val);
		lhdr_p->count--;
		return ret_val;
	}

```

## E152-100

物理行 95–96；定义/实例 85–96；blob 24f7a214a288；行尾 LF,LF；展示分隔 NONE。精确链表写入顺序、读根和计数。
[VERIFIED myinitramfs/mylib/util/list.c::list_header_remove_head]  
```
	return NULL;
}
```

## E152-101

物理行 98–102；定义/实例 98–102；blob 24f7a214a288；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。精确链表写入顺序、读根和计数。
[VERIFIED myinitramfs/mylib/util/list.c::list_header_add_to_tail]  
```
void list_header_add_to_tail(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_prev(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}
```

## E152-102

物理行 14–17；定义/实例 14–17；blob 6a86523c41ba；行尾 LF,LF,LF,LF；展示分隔 NONE。单比特OR写入。
[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_set_bit]  
```
void bm_set_bit(bitmap_t * bm, unsigned long idx)
{
	*(bm + idx / BITMAP_UNITSIZE) |= (1ULL << (idx % BITMAP_UNITSIZE));
}
```

## E152-103

物理行 24–28；定义/实例 24–34；blob 6a86523c41ba；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。逐位扫描和返回边界。
[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_freebit_idx]  
```
unsigned long bm_get_freebit_idx(bitmap_t * bm, unsigned long start, unsigned long limit)
{
	unsigned long 	i;
	for (i = start; i < limit; i++)
	{
```

## E152-104

物理行 29–33；定义/实例 24–34；blob 6a86523c41ba；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。逐位扫描和返回边界。
[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_freebit_idx]  
```
		bitmap_t bm_unit = *(bm + (i / BITMAP_UNITSIZE));
		if ((bm_unit & (1ULL << i % BITMAP_UNITSIZE)) == 0)
			break;
	}
	return i;
```

## E152-105

物理行 34–34；定义/实例 24–34；blob 6a86523c41ba；行尾 NONE；展示分隔 LF。逐位扫描和返回边界。
[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_freebit_idx]  
```
}
```

## E152-106

物理行 41–41；定义/实例 39–51；blob 6019436417ad；行尾 LF；展示分隔 NONE。time分支金丝雀；实际命令实例39–51。
[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]  
```
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
```

## E152-107

物理行 79–79；定义/实例 36–214；blob 2e9d5b195559；行尾 LF；展示分隔 NONE。time分支金丝雀。
[VERIFIED mykernel/debug/panic.c::panic]  
```
	this_cpu = smp_processor_id();
```

## E152-108

物理行 1–1；定义/实例 1–1；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::cmake_minimum_required]  
```
cmake_minimum_required(VERSION 3.16)
```

## E152-109

物理行 2–2；定义/实例 2–2；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::set]  
```
set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
```

## E152-110

物理行 3–3；定义/实例 3–3；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::project]  
```
project(MyOS2_Initramfs ASM C)
```

## E152-111

物理行 6–6；定义/实例 6–6；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::set]  
```
set(PROMPT "======>>>")
```

## E152-112

物理行 7–7；定义/实例 7–7；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::set]  
```
set(PROMPT_END "<<<======")
```

## E152-113

物理行 13–13；定义/实例 13–13；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::if]  
```
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
```

## E152-114

物理行 14–14；定义/实例 14–14；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::set]  
```
	set(RELEASE_FLAG "-DDEBUG")
```

## E152-115

物理行 15–15；定义/实例 15–15；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::message]  
```
	message("${PROMPT} DEBUG ${PROMPT_END}")
```

## E152-116

物理行 16–16；定义/实例 16–16；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::elseif]  
```
elseif ()
```

## E152-117

物理行 17–17；定义/实例 17–17；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::message]  
```
	message("${PROMPT} RELEASE")
```

## E152-118

物理行 18–18；定义/实例 18–18；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::set]  
```
	set(RELEASE_FLAG "-DRELEASE")
```

## E152-119

物理行 19–19；定义/实例 19–19；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::set]  
```
	set(UNUSED_MACRO "-Wunused-macros")
```

## E152-120

物理行 20–20；定义/实例 20–20；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::endif]  
```
endif()
```

## E152-121

物理行 22–24；定义/实例 22–24；blob 6757d21bc64c；行尾 LF,LF,LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::set]  
```
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${RELEASE_FLAG} \
	-m64 \
")
```

## E152-122

物理行 26–29；定义/实例 26–29；blob 6757d21bc64c；行尾 LF,LF,LF,LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::set]  
```
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 \
	-ggdb \
")
```

## E152-123

物理行 31–31；定义/实例 31–31；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::if]  
```
if (${LINK_TYPE} STREQUAL "static")
```

## E152-124

物理行 32–32；定义/实例 32–32；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::set]  
```
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
```

## E152-125

物理行 33–33；定义/实例 33–33；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::endif]  
```
endif()
```

## E152-126

物理行 38–38；定义/实例 38–38；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::set]  
```
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}) # Force generate binaries to ${CMAKE_BINARY_DIR}
```

## E152-127

物理行 40–40；定义/实例 40–40；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::set]  
```
set(MUSL_PATH /home/cheyh/projs/musl/build)
```

## E152-128

物理行 41–41；定义/实例 41–41；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::set]  
```
set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)
```

## E152-129

物理行 43–43；定义/实例 43–43；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::set]  
```
set(MUSL_INC_PATH ${MUSL_PATH}/include)
```

## E152-130

物理行 44–44；定义/实例 44–44；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::set]  
```
set(MUSL_LIB_PATH ${MUSL_PATH}/lib)
```

## E152-131

物理行 46–46；定义/实例 46–46；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::include_directories]  
```
include_directories("${MUSL_INC_PATH}")
```

## E152-132

物理行 47–47；定义/实例 47–47；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory]  
```
add_subdirectory(myinit)
```

## E152-133

物理行 48–48；定义/实例 48–48；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory]  
```
add_subdirectory(myshell)
```

## E152-134

物理行 50–50；定义/实例 50–50；blob 6757d21bc64c；行尾 LF；展示分隔 NONE。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::install]  
```
install(TARGETS init DESTINATION boot)
```

## E152-135

物理行 51–51；定义/实例 51–51；blob 6757d21bc64c；行尾 NONE；展示分隔 LF。根构建命令实例，未据此认定真实链接。
[VERIFIED myinitramfs/CMakeLists.txt::install]  
```
install(TARGETS sh DESTINATION boot)
```

## E152-136

物理行 1–1；定义/实例 1–1；blob 8f6abc24095a；行尾 LF；展示分隔 NONE。mylib子目录命令实例。
[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory]  
```
add_subdirectory(myglibc)
```

## E152-137

物理行 2–2；定义/实例 2–2；blob 8f6abc24095a；行尾 LF；展示分隔 NONE。mylib子目录命令实例。
[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory]  
```
add_subdirectory(csu)
```

## E152-138

物理行 3–3；定义/实例 3–3；blob 8f6abc24095a；行尾 NONE；展示分隔 LF。mylib子目录命令实例。
[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory]  
```
add_subdirectory(util)
```

## E152-139

物理行 1–1；定义/实例 1–1；blob b956957a602a；行尾 LF；展示分隔 NONE。myglibc构建命令实例。
[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_library]  
```
add_library(mylibc STATIC)
```

## E152-140

物理行 2–2；定义/实例 2–2；blob b956957a602a；行尾 LF；展示分隔 NONE。myglibc构建命令实例。
[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::set]  
```
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH}")
```

## E152-141

物理行 3–7；定义/实例 3–16；blob b956957a602a；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。myglibc构建命令实例。
[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories]  
```
target_include_directories(mylibc PUBLIC
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/include
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sys
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/string
```

## E152-142

物理行 8–12；定义/实例 3–16；blob b956957a602a；行尾 LF,LF,LF,LF,LF；展示分隔 NONE。myglibc构建命令实例。
[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories]  
```
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/misc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/x86
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/generic
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/gnu
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux
```

## E152-143

物理行 13–16；定义/实例 3–16；blob b956957a602a；行尾 LF,LF,LF,LF；展示分隔 NONE。myglibc构建命令实例。
[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories]  
```
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux/x86
                    ${PROJECT_SOURCE_DIR}/../mykernel/include
                    ${PROJECT_SOURCE_DIR}/../mykernel/arch/${ARCH}/include
                    )
```

## E152-144

物理行 18–18；定义/实例 18–18；blob b956957a602a；行尾 LF；展示分隔 NONE。myglibc构建命令实例。
[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]  
```
add_subdirectory(stdio)
```

## E152-145

物理行 19–19；定义/实例 19–19；blob b956957a602a；行尾 LF；展示分隔 NONE。myglibc构建命令实例。
[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]  
```
add_subdirectory(malloc)
```

## E152-146

物理行 20–20；定义/实例 20–20；blob b956957a602a；行尾 LF；展示分隔 NONE。myglibc构建命令实例。
[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]  
```
add_subdirectory(string)
```

## E152-147

物理行 21–21；定义/实例 21–21；blob b956957a602a；行尾 LF；展示分隔 NONE。myglibc构建命令实例。
[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]  
```
add_subdirectory(exit)
```

## E152-148

物理行 23–23；定义/实例 23–23；blob b956957a602a；行尾 LF；展示分隔 NONE。myglibc构建命令实例。
[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]  
```
add_subdirectory(syscall)
```

## E152-149

物理行 24–24；定义/实例 24–24；blob b956957a602a；行尾 LF；展示分隔 NONE。myglibc构建命令实例。
[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]  
```
add_subdirectory(posix)
```

## E152-150

物理行 25–25；定义/实例 25–25；blob b956957a602a；行尾 LF；展示分隔 NONE。myglibc构建命令实例。
[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]  
```
add_subdirectory(sysdeps)
```

## E152-151

物理行 26–26；定义/实例 26–26；blob b956957a602a；行尾 LF；展示分隔 NONE。myglibc构建命令实例。
[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]  
```
add_subdirectory(misc)
```

## E152-152

物理行 27–27；定义/实例 27–27；blob b956957a602a；行尾 LF；展示分隔 NONE。myglibc构建命令实例。
[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]  
```
add_subdirectory(time)
```

## E152-153

物理行 28–28；定义/实例 28–28；blob b956957a602a；行尾 LF；展示分隔 NONE。myglibc构建命令实例。
[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]  
```
add_subdirectory(io)
```

## E152-154

物理行 29–29；定义/实例 29–29；blob b956957a602a；行尾 LF；展示分隔 NONE。myglibc构建命令实例。
[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]  
```
add_subdirectory(libio)
```

## E152-155

物理行 1–1；定义/实例 1–1；blob 3b9f22dd9ab9；行尾 LF；展示分隔 NONE。源文件候选与目标归属。
[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::file]  
```
file(GLOB LIBC_STDLIB_C_SRCS *.c)
```

## E152-156

物理行 2–2；定义/实例 2–2；blob 3b9f22dd9ab9；行尾 LF；展示分隔 NONE。源文件候选与目标归属。
[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::file]  
```
file(GLOB LIBC_STDLIB_ASM_SRCS *.S)
```

## E152-157

物理行 3–3；定义/实例 3–3；blob 3b9f22dd9ab9；行尾 NONE；展示分隔 LF。源文件候选与目标归属。
[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::target_sources]  
```
target_sources(mylibc PUBLIC ${LIBC_STDLIB_C_SRCS} ${LIBC_STDLIB_ASM_SRCS})
```

## E152-158

物理行 1–1；定义/实例 1–1；blob 5fd782a35629；行尾 LF；展示分隔 NONE。源文件候选与目标归属。
[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::file]  
```
file(GLOB UTIL_C_SRCS *.c)
```

## E152-159

物理行 2–2；定义/实例 2–2；blob 5fd782a35629；行尾 LF；展示分隔 NONE。源文件候选与目标归属。
[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::file]  
```
file(GLOB UTIL_ASM_SRCS *.S)
```

## E152-160

物理行 3–3；定义/实例 3–3；blob 5fd782a35629；行尾 NONE；展示分隔 LF。源文件候选与目标归属。
[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::target_sources]  
```
target_sources(mylibc PUBLIC ${UTIL_C_SRCS} ${UTIL_ASM_SRCS})
```
