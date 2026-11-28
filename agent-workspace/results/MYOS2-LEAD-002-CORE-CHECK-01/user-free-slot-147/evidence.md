---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: batch147_fixed_source_evidence
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-23
produced_by: GPT-6.2
model_per_owner: "GPT-6.2 Sol Extra/high requested; backend and actual effort UNATTESTED"
date: 2026-11-25
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
read_channel: connector
status: AUTHOR_PENDING_MAINLINE_REVIEW
---

# 第147批固定源码正式窄窗

仅固定time原物理行。保留CRLF/LF；源末行NONE时只在闭围栏前增加一枚排版LF，source_index逐窗记录source_line_endings与render_separator，还原时只去该分隔。外证与GIVEN不属于正式源码前缀计数，详见facts.external_basis和正文。旧145/146正式窗不重贴。

## E147-001 · RELEASE_FLAG

kernel Debug condition sets release flag；configuration_directive；mykernel/scripts/options_flags.cmake 16–18；Blob 6019436417ad；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/scripts/options_flags.cmake::RELEASE_FLAG]
```cmake
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(RELEASE_FLAG "-DDEBUG")
	message("${PROMPT} DEBUG ${PROMPT_END}")
```

## E147-002 · RELEASE_FLAG

kernel other build-type flag；configuration_directive；mykernel/scripts/options_flags.cmake 19–23；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/scripts/options_flags.cmake::RELEASE_FLAG]
```cmake
else ()
	message("${PROMPT} RELEASE  ${PROMPT_END}")
	set(RELEASE_FLAG "-DRELEASE")
	set(UNUSED_MACRO "-Wunused-macros")
endif()
```

## E147-003 · CMAKE_C_FLAGS

active kernel C flags including m64 kernel model no PIE/PIC and freestanding；configuration_directive；mykernel/scripts/options_flags.cmake 39–43；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

## E147-004 · CMAKE_C_FLAGS

kernel C stack/unwind/static/sections/debug flags；configuration_directive；mykernel/scripts/options_flags.cmake 44–48；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-fno-stack-protector \
	-fno-unwind-tables \
	-static \
	-fdata-sections -ffunction-sections \
	-ggdb \
```

## E147-005 · CMAKE_C_FLAGS

sysv ABI and no red zone；configuration_directive；mykernel/scripts/options_flags.cmake 49–51；Blob 6019436417ad；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-mabi=sysv \
	-mno-red-zone \
")
```

## E147-006 · CMAKE_C_FLAGS

kernel C macros include CONFIG_64BIT；configuration_directive；mykernel/scripts/options_flags.cmake 58–62；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

## E147-007 · CMAKE_C_FLAGS

kernel zone/SLUB/syscall/BUG/GRUB configuration；configuration_directive；mykernel/scripts/options_flags.cmake 63–67；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
	-DGRUB2_BOOTUP_SUPPORT \
```

## E147-008 · CMAKE_C_FLAGS

kernel guest option macros；configuration_directive；mykernel/scripts/options_flags.cmake 68–70；Blob 6019436417ad；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_HYPERVISOR_GUEST \
	-DCONFIG_KVM_GUEST \
")
```

## E147-009 · panic

time panic CPU canary；function_body；mykernel/debug/panic.c 79–79；Blob 2e9d5b195559；source_line_endings=LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/debug/panic.c::panic]
```c
	this_cpu = smp_processor_id();
```

## E147-010 · RELEASE_FLAG

user top-level Debug text sets -DDEBUG; actual configuration not known；build_directive；myinitramfs/CMakeLists.txt 13–15；Blob 6757d21bc64c；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=BUILD,DEBUG。

[VERIFIED myinitramfs/CMakeLists.txt::RELEASE_FLAG]
```cmake
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(RELEASE_FLAG "-DDEBUG")
	message("${PROMPT} DEBUG ${PROMPT_END}")
```

## E147-011 · RELEASE_FLAG

other conditional build-type text sets -DRELEASE and unused macro flag；build_directive；myinitramfs/CMakeLists.txt 16–20；Blob 6757d21bc64c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=BUILD,DEBUG。

[VERIFIED myinitramfs/CMakeLists.txt::RELEASE_FLAG]
```cmake
elseif ()
	message("${PROMPT} RELEASE")
	set(RELEASE_FLAG "-DRELEASE")
	set(UNUSED_MACRO "-Wunused-macros")
endif()
```

## E147-012 · CMAKE_C_FLAGS

top-level user C flags m64 and ggdb；build_directive；myinitramfs/CMakeLists.txt 26–29；Blob 6757d21bc64c；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖=BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::CMAKE_C_FLAGS]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 \
	-ggdb \
")
```

## E147-013 · LINK_TYPE

conditional static flag；build_directive；myinitramfs/CMakeLists.txt 31–33；Blob 6757d21bc64c；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::LINK_TYPE]
```cmake
if (${LINK_TYPE} STREQUAL "static")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
endif()
```

## E147-014 · MUSL_PATH

external musl toolchain path and compiler expression；build_directive；myinitramfs/CMakeLists.txt 40–41；Blob 6757d21bc64c；source_line_endings=LF,LF, render_separator=NONE；覆盖=BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::MUSL_PATH]
```cmake
set(MUSL_PATH /home/cheyh/projs/musl/build)
set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)
```

## E147-015 · MUSL_INC_PATH

external include/lib variable definitions and include_directories consumption；build_directive；myinitramfs/CMakeLists.txt 43–46；Blob 6757d21bc64c；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖=BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::MUSL_INC_PATH]
```cmake
set(MUSL_INC_PATH ${MUSL_PATH}/include)
set(MUSL_LIB_PATH ${MUSL_PATH}/lib)

include_directories("${MUSL_INC_PATH}")
```

## E147-016 · add_subdirectory

only direct subdirectories myinit and myshell in root file；build_directive；myinitramfs/CMakeLists.txt 47–48；Blob 6757d21bc64c；source_line_endings=LF,LF, render_separator=NONE；覆盖=BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myinit)
add_subdirectory(myshell)
```

## E147-017 · install

install init and sh；build_directive；myinitramfs/CMakeLists.txt 50–51；Blob 6757d21bc64c；source_line_endings=LF,NONE, render_separator=LF；覆盖=BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::install]
```cmake
install(TARGETS init DESTINATION boot)
install(TARGETS sh DESTINATION boot)
```

## E147-018 · add_subdirectory

independent mylib enters myglibc/csu/util；build_directive；myinitramfs/mylib/CMakeLists.txt 1–3；Blob 8f6abc24095a；source_line_endings=LF,LF,NONE, render_separator=LF；覆盖=BUILD。

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myglibc)
add_subdirectory(csu)
add_subdirectory(util)
```

## E147-019 · mylibc

standalone static target and C flag definitions；build_directive；myinitramfs/mylib/myglibc/CMakeLists.txt 1–2；Blob b956957a602a；source_line_endings=LF,LF, render_separator=NONE；覆盖=BUILD。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::mylibc]
```cmake
add_library(mylibc STATIC)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH}")
```

## E147-020 · add_subdirectory

standalone mylibc enters malloc subdir；build_directive；myinitramfs/mylib/myglibc/CMakeLists.txt 18–21；Blob b956957a602a；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖=BUILD。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(stdio)
add_subdirectory(malloc)
add_subdirectory(string)
add_subdirectory(exit)
```

## E147-021 · LIBC_STDLIB_C_SRCS

C GLOB definition to target_sources consumption; ASM separate；build_directive；myinitramfs/mylib/myglibc/malloc/CMakeLists.txt 1–3；Blob 3b9f22dd9ab9；source_line_endings=LF,LF,NONE, render_separator=LF；覆盖=BUILD。

[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::LIBC_STDLIB_C_SRCS]
```cmake
file(GLOB LIBC_STDLIB_C_SRCS *.c)
file(GLOB LIBC_STDLIB_ASM_SRCS *.S)
target_sources(mylibc PUBLIC ${LIBC_STDLIB_C_SRCS} ${LIBC_STDLIB_ASM_SRCS})
```

## E147-022 · UTIL_C_SRCS

C GLOB definition to target_sources consumption; ASM separate；build_directive；myinitramfs/mylib/util/CMakeLists.txt 1–3；Blob 5fd782a35629；source_line_endings=LF,LF,NONE, render_separator=LF；覆盖=BUILD。

[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::UTIL_C_SRCS]
```cmake
file(GLOB UTIL_C_SRCS *.c)
file(GLOB UTIL_ASM_SRCS *.S)
target_sources(mylibc PUBLIC ${UTIL_C_SRCS} ${UTIL_ASM_SRCS})
```

## E147-023 · CONST_2M

local H numeric constant；macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 15–15；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=H,A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::CONST_2M]
```c
	#define CONST_2M		0x200000ULL
```

## E147-024 · __round_mask

mask uses type of x and y-1；macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 36–36；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=H,A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::__round_mask]
```c
#define __round_mask(x, y) ((__typeof__(x))((y)-1))
```

## E147-025 · round_down

round-down expression x & complement mask；macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 56–56；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=H,A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::round_down]
```c
#define round_down(x, y) ((x) & ~__round_mask(x, y))
```

## E147-026 · PAGE_ROUND_DOWN

PAGE_ROUND_DOWN expands using PAGE_SIZE；macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 59–59；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=H,A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::PAGE_ROUND_DOWN]
```c
#define PAGE_ROUND_DOWN(x)	round_down((x), PAGE_SIZE)
```

## E147-027 · PAGE_SIZE

local PAGE_SIZE maps to CONST_2M；macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 60–60；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=H,A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::PAGE_SIZE]
```c
#define PAGE_SIZE			CONST_2M
```

## E147-028 · used_upage_lhdr

used upage list header declaration；global_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 70–70；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::used_upage_lhdr]
```c
List_hdr_s		used_upage_lhdr;
```

## E147-029 · upage_search

return local initialized NULL; address round-down; list cursor；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 151–155；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]
```c
uPage_s * upage_search(void * vaddr)
{
	uPage_s * ret_val = NULL;
	vaddr = (void *)PAGE_ROUND_DOWN((size_t)vaddr);
	List_s * upg_lp;
```

## E147-030 · upage_search

anchor-based for loop and container_of conversion；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 156–160；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]
```c
	for (upg_lp = used_upage_lhdr.anchor.next;
		 upg_lp != &used_upage_lhdr.anchor;
		 upg_lp = upg_lp->next)
	{
		uPage_s * upgp = container_of(upg_lp, uPage_s, upage_list);
```

## E147-031 · upage_search

exact vaddr equality, pointer return candidate, break；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 161–165；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]
```c
		if (upgp->vaddr == vaddr)
		{
			ret_val = upgp;
			break;
		}
```

## E147-032 · upage_search

loop end and return ret_val；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 166–169；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]
```c
	}

	return ret_val;
}
```

## E147-033 · free

free declaration, DEBUG conditional and wait while not initiated；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 330–334；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=DEBUG,C。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
void free(void * obj_p)
{
	#ifdef DEBUG
		// make sure have init slab
		while (!initiated);
```

## E147-034 · free

end preprocessor DEBUG block then NULL early void return；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 335–338；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=DEBUG,C。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	#endif

	if (obj_p == NULL)
		return;
```

## E147-035 · free

NULL condition then void return；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 337–338；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF, render_separator=NONE；覆盖=C。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	if (obj_p == NULL)
		return;
```

## E147-036 · free

search call then direct two ownership reads (no NULL guard here)；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 340–343；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=C。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	// find which slab does the pointer belonged to
	uPage_s * upgp = upage_search(obj_p);
	uslab_s * uslp = upgp->uslab_p;
	uslab_cache_s * uscgp = uslp->uslabcache_p;
```

## E147-037 · free

empty slab equality and explicit while(1)；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 345–349；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=C。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	if (uslp->free == uslp->total)
	{
		// color_printk(WHITE, RED, "Free obj in a free %d-byte slab!\n", scgp->obj_size);
		while (1);
	}
```

## E147-038 · free

pointer-difference quotient and assigned nonzero decision；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 351–352；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF, render_separator=NONE；覆盖=C。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	unsigned long obj_idx = (obj_p - uslp->virt_addr) / uscgp->obj_size;
	if (!bm_get_assigned_bit(uslp->colormap, obj_idx))
```

## E147-039 · free

false assigned value enters explicit while(1)；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 352–356；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=C。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	if (!bm_get_assigned_bit(uslp->colormap, obj_idx))
	{
		// color_printk(WHITE, RED, "The obj already been freed : %#018lx\n!", obj_p);
		while (1);
	}
```

## E147-040 · free

clear then S.free++, CF++, CU-- in order；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 358–361；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=C。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	bm_clear_bit(uslp->colormap, obj_idx);
	uslp->free++;
	uscgp->nsobj_free_count++;
	uscgp->nsobj_used_count--;
```

## E147-041 · free

updated free==1 branch: unlink full node, full.count-- and add used head；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 364–368；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=C。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	if (uslp->free == 1)
	{
		list_del_init(&uslp->uslab_list);
		uscgp->normal_slab_full.count--;
		list_header_add_to_head(&uscgp->normal_slab_used, &uslp->uslab_list);
```

## E147-042 · free

else-if uses updated free==0, unlink used and decrement if entered；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 370–374；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=C。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	else if (uslp->free == 0)
	{
		list_del_init(&uslp->uslab_list);
		uscgp->normal_slab_used.count--;
		// if slp is base-slab, add it to tail
```

## E147-043 · free

nested base identity guard; head vs tail free insertion if reached；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 375–379；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=C。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
		if (uslp != uscgp->normal_base_slab_p)
			list_header_add_to_head(&uscgp->normal_slab_free, &uslp->uslab_list);
		else
			list_header_add_to_tail(&uscgp->normal_slab_free, &uslp->uslab_list);
	}
```

## E147-044 · free

free-list count>2 recovery gate; body not taken in any normal modeled C；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 380–382；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF, render_separator=NONE；覆盖=C。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	// if there is too many free slab, free some of them
	if (uscgp->normal_slab_free.count > 2)
	{
```

## E147-045 · free

end of void function after recovery conditional；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 389–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,NONE, render_separator=LF；覆盖=C。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c

	// unlock_recurs_lock(&slab_alloc_lock);
}
```

## E147-046 · bitmap_t

bitmap_t defined by __UINT64_TYPE__; given width separately；typedef_definition；myinitramfs/mylib/include/utils.h 19–19；Blob 4e3e8910ac00；source_line_endings=LF, render_separator=NONE；覆盖=B,C,TYPE。

[VERIFIED myinitramfs/mylib/include/utils.h::bitmap_t]
```c
	typedef __UINT64_TYPE__ bitmap_t;
```

## E147-047 · BITMAP_UNITSIZE

bitmap unit equals sizeof(bitmap_t)*8；macro_definition；myinitramfs/mylib/include/utils.h 20–20；Blob 4e3e8910ac00；source_line_endings=LF, render_separator=NONE；覆盖=B,C,TYPE。

[VERIFIED myinitramfs/mylib/include/utils.h::BITMAP_UNITSIZE]
```c
	#define BITMAP_UNITSIZE (sizeof(bitmap_t) * 8)
```

## E147-048 · bm_get_assigned_bit

unsigned return type, unsigned local, unit index and unsuffixed 1 shift into bitmap_t mask；function_body；myinitramfs/mylib/util/bitmap.c 4–8；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=B,C。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_assigned_bit]
```c
unsigned bm_get_assigned_bit(bitmap_t * bm, unsigned long idx)
{
	unsigned ret_val = 0;
	size_t unit_idx = idx / (8 * sizeof(bitmap_t));
	bitmap_t mask = 1 << (idx % (8 * sizeof(bitmap_t)));
```

## E147-049 · bm_get_assigned_bit

load selected word, assign unit & mask to unsigned and return；function_body；myinitramfs/mylib/util/bitmap.c 9–12；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖=B,C。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_assigned_bit]
```c
	bitmap_t unit = *(bm + unit_idx);
	ret_val = unit & mask;
	return ret_val;
}
```

## E147-050 · bm_clear_bit

void helper uses 1ULL shift and complement, AND assignment；function_body；myinitramfs/mylib/util/bitmap.c 19–22；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖=B,C。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_clear_bit]
```c
void bm_clear_bit(bitmap_t * bm, unsigned long idx)
{
	*(bm + idx / BITMAP_UNITSIZE) &= ~(1ULL << (idx % BITMAP_UNITSIZE));
}
```

## E147-051 · bm_set_bit

separate helper type contrast 1ULL; not an invoked free helper；function_body；myinitramfs/mylib/util/bitmap.c 14–17；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖=B。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_set_bit]
```c
void bm_set_bit(bitmap_t * bm, unsigned long idx)
{
	*(bm + idx / BITMAP_UNITSIZE) |= (1ULL << (idx % BITMAP_UNITSIZE));
}
```

## E147-052 · uPage

page list, vaddr, uslab_p fields；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 19–23；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=A,C,TYPE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage]
```c
	typedef struct uPage
	{
		List_s	upage_list;
		void	*vaddr;
		uslab_s *uslab_p;
```

## E147-053 · uPage

vaddr, uslab_p and uPage_s alias；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 22–24；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=A,C,TYPE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage]
```c
		void	*vaddr;
		uslab_s *uslab_p;
	} uPage_s;
```

## E147-054 · uslab

slab list and owning cache pointer；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 26–30；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=C,TYPE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab]
```c
	typedef struct uslab
	{
		List_s			uslab_list;
		uslab_cache_s	*uslabcache_p;

```

## E147-055 · uslab

total/free and upage/virtual address fields；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 31–35；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=C,TYPE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab]
```c
		unsigned long	total;
		unsigned long	free;

		uPage_s			*upage_p;
		void			*virt_addr;
```

## E147-056 · uslab

virt_addr/colormap and uslab_s alias；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 34–37；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖=C,TYPE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab]
```c
		uPage_s			*upage_p;
		void			*virt_addr;
		bitmap_t		*colormap;
	} uslab_s;
```

## E147-057 · uslab_cache

cache list and object size；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 39–43；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=C,TYPE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache]
```c
	typedef struct uslab_cache
	{
		List_s			uslabcache_list;

		unsigned long	obj_size;
```

## E147-058 · uslab_cache

slab total, three list headers, nsobj_free_count；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 45–49；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=C,TYPE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache]
```c
		unsigned long	normal_slab_total;
		List_hdr_s		normal_slab_free;
		List_hdr_s		normal_slab_used;
		List_hdr_s		normal_slab_full;
		unsigned long	nsobj_free_count;
```

## E147-059 · uslab_cache

object free/used counters and base-slab pointer；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 49–53；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=C,TYPE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache]
```c
		unsigned long	nsobj_free_count;
		unsigned long	nsobj_used_count;

		// the base slab should not be freed
		uslab_s			*normal_base_slab_p;
```

## E147-060 · container_of

active container_of expression through member offset；macro_definition；myinitramfs/mylib/include/utils.h 12–16；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=A,TYPE。

[VERIFIED myinitramfs/mylib/include/utils.h::container_of]
```c
	#define container_of(member_ptr, container_type, member_name)		\
	({																	\
		typeof(((container_type *)0)->member_name) * p = (member_ptr);	\
		(container_type *)((unsigned long)p - 							\
		(unsigned long)&(((container_type *)0)->member_name));			\
```

## E147-061 · container_of

container_of expression tail；macro_definition；myinitramfs/mylib/include/utils.h 15–17；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=A,TYPE。

[VERIFIED myinitramfs/mylib/include/utils.h::container_of]
```c
		(container_type *)((unsigned long)p - 							\
		(unsigned long)&(((container_type *)0)->member_name));			\
	})
```

## E147-062 · List

prev/next and List_s alias；struct_definition；myinitramfs/mylib/include/utils.h 35–39；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=A,C,LIST。

[VERIFIED myinitramfs/mylib/include/utils.h::List]
```c
	typedef struct List
	{
		List_s *	prev;
		List_s *	next;
	} List_s;
```

## E147-063 · List_hdr

anchor/count and List_hdr_s alias；struct_definition；myinitramfs/mylib/include/utils.h 41–45；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=A,C,LIST。

[VERIFIED myinitramfs/mylib/include/utils.h::List_hdr]
```c
	typedef struct List_hdr
	{
		List_s			anchor;
		unsigned long	count;
	} List_hdr_s;
```

## E147-064 · INIT_LIST_S

direct prev/next self-link；function_body；myinitramfs/mylib/util/list.c 10–14；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LIST,C。

[VERIFIED myinitramfs/mylib/util/list.c::INIT_LIST_S]
```c
void INIT_LIST_S(List_s * src)
{
	src->prev = src;
	src->next = src;
}
```

## E147-065 · list_insert_prev

actual tail-side insertion: unlink then next assignment；function_body；myinitramfs/mylib/util/list.c 17–21；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LIST,C。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_prev]
```c
void list_insert_prev(List_s * dst, List_s * src)
{
	list_del_init(src);

	src->next = dst;
```

## E147-066 · list_insert_prev

prev assignment and neighbor updates；function_body；myinitramfs/mylib/util/list.c 22–26；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LIST,C。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_prev]
```c
	src->prev = dst->prev;

	src->next->prev = src;
	src->prev->next = src;
}
```

## E147-067 · list_insert_next

actual head-side insertion: unlink then next assignment；function_body；myinitramfs/mylib/util/list.c 29–33；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LIST,C。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_next]
```c
void list_insert_next(List_s * dst, List_s * src)
{
	list_del_init(src);

	src->next = dst->next;
```

## E147-068 · list_insert_next

prev assignment and neighbor updates；function_body；myinitramfs/mylib/util/list.c 34–38；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LIST,C。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_next]
```c
	src->prev = dst;

	src->next->prev = src;
	src->prev->next = src;
}
```

## E147-069 · list_del_init

unlink via both neighbor pointers and self-initialize；function_body；myinitramfs/mylib/util/list.c 40–44；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LIST,C。

[VERIFIED myinitramfs/mylib/util/list.c::list_del_init]
```c
void list_del_init(List_s *src)
{
	src->prev->next = src->next;
	src->next->prev = src->prev;
	INIT_LIST_S(src);
```

## E147-070 · INIT_LIST_HEADER_S

count=0 and anchor self-init；function_body；myinitramfs/mylib/util/list.c 73–77；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LIST,A。

[VERIFIED myinitramfs/mylib/util/list.c::INIT_LIST_HEADER_S]
```c
void INIT_LIST_HEADER_S(List_hdr_s * lhdr_p)
{
	lhdr_p->count = 0;
	INIT_LIST_S(&lhdr_p->anchor);
}
```

## E147-071 · list_header_add_to_head

head insertion completes before count increment；function_body；myinitramfs/mylib/util/list.c 79–83；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LIST,C。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_add_to_head]
```c
void list_header_add_to_head(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_next(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}
```

## E147-072 · list_header_add_to_tail

tail insertion completes before count increment；function_body；myinitramfs/mylib/util/list.c 98–102；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LIST,C。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_add_to_tail]
```c
void list_header_add_to_tail(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_prev(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}
```

