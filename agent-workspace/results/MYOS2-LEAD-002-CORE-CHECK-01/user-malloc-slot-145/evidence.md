---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: batch145_evidence
conversation_display_name: MYOS2-A-C10b 代码分析执行
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-23
produced_by: "GPT-6（当前系统标识；后台/effort未认证）"
model_per_owner: "GPT-6.2 Sol and Extra/high requested, not independently attested"
date: 2026-11-24
base_snapshot: "time@a039d9803ade (short SHA)"
read_channel: connector
status: AUTHOR_PENDING_MAINLINE_REVIEW
---

# Batch145 · 55 个固定源码短证据窗口

本页只引用time固定源码，窗口各1–5个物理行；源码行尾（尤其malloc.c的CRLF）原样保留在代码围栏内。每窗引述长度不证明真实运行。其余五件不再放置正式证据前缀。

## E145-001 CONST_1M

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 12–12; Git blob 44db9ea9d7f7. Intended use: macro_definition.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::CONST_1M]
```c
	#define CONST_1M		0x100000ULL
```

## E145-002 PAGE_SIZE

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 58–60; Git blob 44db9ea9d7f7. Intended use: macro_definition.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::PAGE_SIZE]
```c
#define PAGE_ROUND_UP(x)	round_up((x), PAGE_SIZE)
#define PAGE_ROUND_DOWN(x)	round_down((x), PAGE_SIZE)
#define PAGE_SIZE			CONST_2M
```

## E145-003 USLAB_LEVEL

Source: `myinitramfs/mylib/myglibc/malloc/_malloc.h` physical lines 7–8; Git blob 2dea1f8fc05c. Intended use: macro_definition.

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::USLAB_LEVEL]
```c
	#define USLAB_LEVEL			16
	#define USLAB_SIZE_BASE		32
```

## E145-004 init_uslab

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 181–185; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
	for (int i = 0; i < USLAB_LEVEL; i++)
	{
		uslab_cache_s * scgp = &uslab_cache_groups[i];
		INIT_LIST_S(&scgp->uslabcache_list);
		list_header_add_to_tail(&uslabcache_lhdr, &scgp->uslabcache_list);
```

## E145-005 init_uslab

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 187–189; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		INIT_LIST_HEADER_S(&scgp->normal_slab_free);
		INIT_LIST_HEADER_S(&scgp->normal_slab_used);
		INIT_LIST_HEADER_S(&scgp->normal_slab_full);
```

## E145-006 init_uslab

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 191–195; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		uslab_s * bslp = &base_uslabs[i];
		INIT_LIST_S(&bslp->uslab_list);

		scgp->obj_size = USLAB_SIZE_BASE << i;
		unsigned long obj_nr = PAGE_SIZE / scgp->obj_size;
```

## E145-007 init_uslab

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 198–202; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		list_header_add_to_tail(&scgp->normal_slab_free, &bslp->uslab_list);
		scgp->normal_base_slab_p = bslp;
		scgp->nsobj_free_count = obj_nr;
		scgp->nsobj_used_count = 0;
		scgp->normal_slab_total = 1;
```

## E145-008 init_uslab

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 204–208; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		bslp->colormap = base_colormaps[i];
		bslp->uslabcache_p = scgp;
		bslp->free =
		bslp->total = obj_nr;

```

## E145-009 init_uslab

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 216–220; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		bslp->upage_p = upage_alloc();
		bslp->virt_addr = bslp->upage_p->vaddr;
		bslp->upage_p->uslab_p = bslp;
	}
	initiated = true;
```

## E145-010 malloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 249–252; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
void * malloc(size_t size)
{
	if (!initiated)
		init_uslab();
```

## E145-011 malloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 254–258; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
	void * ret_val = NULL;
	if (size > CONST_1M)
	{
		// color_printk(RED, WHITE, "Mem required too big\n");
	}
```

## E145-012 malloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 261–265; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		// find suitable slab group
		uslab_cache_s *	uscgp = &uslab_cache_groups[0];
		while (size > uscgp->obj_size)
			uscgp = container_of((uscgp->uslabcache_list.next), uslab_cache_s, uslabcache_list);

```

## E145-013 malloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 268–272; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		uslab_s *	uslp = NULL;
		if (uscgp->normal_slab_used.count > 0)
		{
			List_s * uslp_lp = list_header_remove_head(&uscgp->normal_slab_used);
			while (uslp_lp == 0);
```

## E145-014 malloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 274–278; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			uslp = container_of(uslp_lp, uslab_s, uslab_list);
			if (uslp->free == 1)
				list_header_add_to_head(&uscgp->normal_slab_full, &uslp->uslab_list);
			else
				list_header_add_to_head(&uscgp->normal_slab_used, &uslp->uslab_list);
```

## E145-015 malloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 280–283; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		else if (uscgp->normal_slab_free.count > 0)
		{
			List_s * uslp_lp = list_header_remove_head(&uscgp->normal_slab_free);
			while (uslp_lp == 0);
```

## E145-016 malloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 284–286; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			
			uslp = container_of(uslp_lp, uslab_s, uslab_list);
			list_header_add_to_head(&uscgp->normal_slab_used, &uslp->uslab_list);
```

## E145-017 malloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 288–292; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		else
		{
			// color_printk(WHITE, RED, "Slab %#x-bytes used out!\n", scgp->obj_size);
			while (1);
		}
```

## E145-018 malloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 294–297; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		// count the virtual addr of suitable object
		unsigned long obj_idx = bm_get_freebit_idx(uslp->colormap, 0, uslp->total);
		if (obj_idx < uslp->total)
		{
```

## E145-019 malloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 298–302; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			size_t offset = uscgp->obj_size * obj_idx;
			ret_val = (void *)((size_t)uslp->virt_addr + offset);
			// refresh status of slab
			uslp->free--;
			uscgp->nsobj_free_count--;
```

## E145-020 malloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 301–305; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			uslp->free--;
			uscgp->nsobj_free_count--;
			uscgp->nsobj_used_count++;
			bm_set_bit(uslp->colormap, obj_idx);
		}
```

## E145-021 malloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 307–311; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		// make sure free slab more than one
		if (uscgp->normal_slab_free.count < 1)
		{
			uscgp->normal_slab_free.count++;
			uslab_s * new_uslp = uslab_alloc(uslp);
```

## E145-022 malloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 312–316; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			uscgp->normal_slab_free.count--;
			if (new_uslp == NULL)
			{
				// color_printk(RED, WHITE, "Alloc new slab :%#x-bytes failed!\n", scgp->obj_size);
				while (1);
```

## E145-023 malloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 318–322; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			new_uslp->uslabcache_p = uscgp;
			list_header_add_to_head(&uscgp->normal_slab_free, &new_uslp->uslab_list);
			uscgp->normal_slab_total++;
			uscgp->nsobj_free_count += new_uslp->total;
		}
```

## E145-024 malloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 324–327; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		// unlock_recurs_lock(&slab_alloc_lock);
	}

	return ret_val;
```

## E145-025 uslab_alloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 223–226; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_alloc]
```c
uslab_s * uslab_alloc(uslab_s * cslp)
{
	uslab_s * nslp = (uslab_s *)malloc(sizeof(uslab_s));
	INIT_LIST_S(&nslp->uslab_list);
```

## E145-026 uslab_alloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 228–232; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_alloc]
```c
	nslp->colormap = (bitmap_t *)malloc(cslp->total / sizeof(bitmap_t) + sizeof(bitmap_t));
	nslp->total =
	nslp->free = cslp->total;
	nslp->upage_p = upage_alloc();
	nslp->virt_addr = nslp->upage_p->vaddr;
```

## E145-027 uslab_alloc

Source: `myinitramfs/mylib/myglibc/malloc/malloc.c` physical lines 231–235; Git blob 44db9ea9d7f7. Intended use: function_body.

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_alloc]
```c
	nslp->upage_p = upage_alloc();
	nslp->virt_addr = nslp->upage_p->vaddr;
	nslp->upage_p->uslab_p = nslp;

	return nslp;
```

## E145-028 uslab

Source: `myinitramfs/mylib/myglibc/malloc/_malloc.h` physical lines 26–30; Git blob 2dea1f8fc05c. Intended use: struct_member.

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab]
```c
	typedef struct uslab
	{
		List_s			uslab_list;
		uslab_cache_s	*uslabcache_p;

```

## E145-029 uslab

Source: `myinitramfs/mylib/myglibc/malloc/_malloc.h` physical lines 31–35; Git blob 2dea1f8fc05c. Intended use: struct_member.

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab]
```c
		unsigned long	total;
		unsigned long	free;

		uPage_s			*upage_p;
		void			*virt_addr;
```

## E145-030 uslab

Source: `myinitramfs/mylib/myglibc/malloc/_malloc.h` physical lines 34–37; Git blob 2dea1f8fc05c. Intended use: struct_member.

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab]
```c
		uPage_s			*upage_p;
		void			*virt_addr;
		bitmap_t		*colormap;
	} uslab_s;
```

## E145-031 uslab_cache

Source: `myinitramfs/mylib/myglibc/malloc/_malloc.h` physical lines 39–43; Git blob 2dea1f8fc05c. Intended use: struct_member.

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache]
```c
	typedef struct uslab_cache
	{
		List_s			uslabcache_list;

		unsigned long	obj_size;
```

## E145-032 uslab_cache

Source: `myinitramfs/mylib/myglibc/malloc/_malloc.h` physical lines 45–49; Git blob 2dea1f8fc05c. Intended use: struct_member.

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache]
```c
		unsigned long	normal_slab_total;
		List_hdr_s		normal_slab_free;
		List_hdr_s		normal_slab_used;
		List_hdr_s		normal_slab_full;
		unsigned long	nsobj_free_count;
```

## E145-033 uslab_cache

Source: `myinitramfs/mylib/myglibc/malloc/_malloc.h` physical lines 49–53; Git blob 2dea1f8fc05c. Intended use: struct_member.

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache]
```c
		unsigned long	nsobj_free_count;
		unsigned long	nsobj_used_count;

		// the base slab should not be freed
		uslab_s			*normal_base_slab_p;
```

## E145-034 bitmap_t

Source: `myinitramfs/mylib/include/utils.h` physical lines 19–20; Git blob 4e3e8910ac00. Intended use: type_definition.

[VERIFIED myinitramfs/mylib/include/utils.h::bitmap_t]
```c
	typedef __UINT64_TYPE__ bitmap_t;
	#define BITMAP_UNITSIZE (sizeof(bitmap_t) * 8)
```

## E145-035 container_of

Source: `myinitramfs/mylib/include/utils.h` physical lines 12–16; Git blob 4e3e8910ac00. Intended use: macro_definition.

[VERIFIED myinitramfs/mylib/include/utils.h::container_of]
```c
	#define container_of(member_ptr, container_type, member_name)		\
	({																	\
		typeof(((container_type *)0)->member_name) * p = (member_ptr);	\
		(container_type *)((unsigned long)p - 							\
		(unsigned long)&(((container_type *)0)->member_name));			\
```

## E145-036 List_hdr

Source: `myinitramfs/mylib/include/utils.h` physical lines 41–45; Git blob 4e3e8910ac00. Intended use: struct_member.

[VERIFIED myinitramfs/mylib/include/utils.h::List_hdr]
```c
	typedef struct List_hdr
	{
		List_s			anchor;
		unsigned long	count;
	} List_hdr_s;
```

## E145-037 bm_get_freebit_idx

Source: `myinitramfs/mylib/util/bitmap.c` physical lines 24–28; Git blob 6a86523c41ba. Intended use: function_body.

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_freebit_idx]
```c
unsigned long bm_get_freebit_idx(bitmap_t * bm, unsigned long start, unsigned long limit)
{
	unsigned long 	i;
	for (i = start; i < limit; i++)
	{
```

## E145-038 bm_get_freebit_idx

Source: `myinitramfs/mylib/util/bitmap.c` physical lines 29–33; Git blob 6a86523c41ba. Intended use: function_body.

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_freebit_idx]
```c
		bitmap_t bm_unit = *(bm + (i / BITMAP_UNITSIZE));
		if ((bm_unit & (1ULL << i % BITMAP_UNITSIZE)) == 0)
			break;
	}
	return i;
```

## E145-039 bm_set_bit

Source: `myinitramfs/mylib/util/bitmap.c` physical lines 14–17; Git blob 6a86523c41ba. Intended use: function_body.

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_set_bit]
```c
void bm_set_bit(bitmap_t * bm, unsigned long idx)
{
	*(bm + idx / BITMAP_UNITSIZE) |= (1ULL << (idx % BITMAP_UNITSIZE));
}
```

## E145-040 INIT_LIST_HEADER_S

Source: `myinitramfs/mylib/util/list.c` physical lines 73–77; Git blob 24f7a214a288. Intended use: function_body.

[VERIFIED myinitramfs/mylib/util/list.c::INIT_LIST_HEADER_S]
```c
void INIT_LIST_HEADER_S(List_hdr_s * lhdr_p)
{
	lhdr_p->count = 0;
	INIT_LIST_S(&lhdr_p->anchor);
}
```

## E145-041 list_header_add_to_head

Source: `myinitramfs/mylib/util/list.c` physical lines 79–83; Git blob 24f7a214a288. Intended use: function_body.

[VERIFIED myinitramfs/mylib/util/list.c::list_header_add_to_head]
```c
void list_header_add_to_head(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_next(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}
```

## E145-042 list_header_remove_head

Source: `myinitramfs/mylib/util/list.c` physical lines 85–89; Git blob 24f7a214a288. Intended use: function_body.

[VERIFIED myinitramfs/mylib/util/list.c::list_header_remove_head]
```c
List_s * list_header_remove_head(List_hdr_s * lhdr_p)
{
	if (lhdr_p->count > 0)
	{
		List_s * ret_val = lhdr_p->anchor.next;
```

## E145-043 list_header_remove_head

Source: `myinitramfs/mylib/util/list.c` physical lines 89–93; Git blob 24f7a214a288. Intended use: function_body.

[VERIFIED myinitramfs/mylib/util/list.c::list_header_remove_head]
```c
		List_s * ret_val = lhdr_p->anchor.next;
		list_del_init(ret_val);
		lhdr_p->count--;
		return ret_val;
	}
```

## E145-044 list_header_add_to_tail

Source: `myinitramfs/mylib/util/list.c` physical lines 98–102; Git blob 24f7a214a288. Intended use: function_body.

[VERIFIED myinitramfs/mylib/util/list.c::list_header_add_to_tail]
```c
void list_header_add_to_tail(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_prev(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}
```

## E145-045 list_del_init

Source: `myinitramfs/mylib/util/list.c` physical lines 40–44; Git blob 24f7a214a288. Intended use: function_body.

[VERIFIED myinitramfs/mylib/util/list.c::list_del_init]
```c
void list_del_init(List_s *src)
{
	src->prev->next = src->next;
	src->next->prev = src->prev;
	INIT_LIST_S(src);
```

## E145-046 CMakeLists.txt

Source: `myinitramfs/CMakeLists.txt` physical lines 40–40; Git blob 6757d21bc64c. Intended use: build_directive.

[VERIFIED myinitramfs/CMakeLists.txt::CMakeLists.txt]
```cmake
set(MUSL_PATH /home/cheyh/projs/musl/build)
```

## E145-047 CMakeLists.txt

Source: `myinitramfs/CMakeLists.txt` physical lines 41–41; Git blob 6757d21bc64c. Intended use: build_directive.

[VERIFIED myinitramfs/CMakeLists.txt::CMakeLists.txt]
```cmake
set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)
```

## E145-048 CMakeLists.txt

Source: `myinitramfs/CMakeLists.txt` physical lines 47–47; Git blob 6757d21bc64c. Intended use: build_directive.

[VERIFIED myinitramfs/CMakeLists.txt::CMakeLists.txt]
```cmake
add_subdirectory(myinit)
```

## E145-049 CMakeLists.txt

Source: `myinitramfs/CMakeLists.txt` physical lines 48–48; Git blob 6757d21bc64c. Intended use: build_directive.

[VERIFIED myinitramfs/CMakeLists.txt::CMakeLists.txt]
```cmake
add_subdirectory(myshell)
```

## E145-050 CMakeLists.txt

Source: `myinitramfs/mylib/CMakeLists.txt` physical lines 1–1; Git blob 8f6abc24095a. Intended use: build_directive.

[VERIFIED myinitramfs/mylib/CMakeLists.txt::CMakeLists.txt]
```cmake
add_subdirectory(myglibc)
```

## E145-051 CMakeLists.txt

Source: `myinitramfs/mylib/CMakeLists.txt` physical lines 3–3; Git blob 8f6abc24095a. Intended use: build_directive.

[VERIFIED myinitramfs/mylib/CMakeLists.txt::CMakeLists.txt]
```cmake
add_subdirectory(util)
```

## E145-052 CMakeLists.txt

Source: `myinitramfs/mylib/myglibc/CMakeLists.txt` physical lines 1–1; Git blob b956957a602a. Intended use: build_directive.

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::CMakeLists.txt]
```cmake
add_library(mylibc STATIC)
```

## E145-053 CMakeLists.txt

Source: `myinitramfs/mylib/myglibc/CMakeLists.txt` physical lines 19–19; Git blob b956957a602a. Intended use: build_directive.

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::CMakeLists.txt]
```cmake
add_subdirectory(malloc)
```

## E145-054 CMakeLists.txt

Source: `myinitramfs/mylib/myglibc/malloc/CMakeLists.txt` physical lines 3–3; Git blob 3b9f22dd9ab9. Intended use: build_directive.

[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::CMakeLists.txt]
```cmake
target_sources(mylibc PUBLIC ${LIBC_STDLIB_C_SRCS} ${LIBC_STDLIB_ASM_SRCS})
```

## E145-055 CMakeLists.txt

Source: `myinitramfs/mylib/util/CMakeLists.txt` physical lines 3–3; Git blob 5fd782a35629. Intended use: build_directive.

[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::CMakeLists.txt]
```cmake
target_sources(mylibc PUBLIC ${UTIL_C_SRCS} ${UTIL_ASM_SRCS})
```

