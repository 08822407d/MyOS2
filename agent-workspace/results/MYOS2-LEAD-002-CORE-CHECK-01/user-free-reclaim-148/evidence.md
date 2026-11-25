---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: batch148_source_evidence
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-23
produced_by: GPT-6.2
model_per_owner: "GPT-6.2 Sol Extra/high requested; backend and actual effort UNATTESTED"
date: 2026-11-25
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
read_channel: connector
status: AUTHOR_PENDING_MAINLINE_REVIEW
---

# 固定time正式窄窗

源物理行保留原CRLF/LF；末行NONE只在闭围栏前添加一枚排版LF，source_index记每行行尾及render_separator，逆向还原只移除该额外LF；不计split末尾空项。仅148新正式源码前缀。

## E148-001 · RELEASE_FLAG

kernel debug flag condition；configuration_directive；mykernel/scripts/options_flags.cmake 16–18；Blob 6019436417ad；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/scripts/options_flags.cmake::RELEASE_FLAG]
```cmake
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(RELEASE_FLAG "-DDEBUG")
	message("${PROMPT} DEBUG ${PROMPT_END}")
```

## E148-002 · RELEASE_FLAG

other build type flag；configuration_directive；mykernel/scripts/options_flags.cmake 19–23；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/scripts/options_flags.cmake::RELEASE_FLAG]
```cmake
else ()
	message("${PROMPT} RELEASE  ${PROMPT_END}")
	set(RELEASE_FLAG "-DRELEASE")
	set(UNUSED_MACRO "-Wunused-macros")
endif()
```

## E148-003 · CMAKE_C_FLAGS

kernel C flags, m64 and freestanding；configuration_directive；mykernel/scripts/options_flags.cmake 39–43；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

## E148-004 · CMAKE_C_FLAGS

kernel stack/unwind/static/section/debug flags；configuration_directive；mykernel/scripts/options_flags.cmake 44–48；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-fno-stack-protector \
	-fno-unwind-tables \
	-static \
	-fdata-sections -ffunction-sections \
	-ggdb \
```

## E148-005 · CMAKE_C_FLAGS

sysv ABI and no red zone；configuration_directive；mykernel/scripts/options_flags.cmake 49–51；Blob 6019436417ad；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-mabi=sysv \
	-mno-red-zone \
")
```

## E148-006 · CMAKE_C_FLAGS

kernel macro block incl CONFIG_64BIT；configuration_directive；mykernel/scripts/options_flags.cmake 58–62；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

## E148-007 · CMAKE_C_FLAGS

kernel zones/SLUB/BUG flags；configuration_directive；mykernel/scripts/options_flags.cmake 63–67；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
	-DGRUB2_BOOTUP_SUPPORT \
```

## E148-008 · CMAKE_C_FLAGS

kernel guest options；configuration_directive；mykernel/scripts/options_flags.cmake 68–70；Blob 6019436417ad；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_HYPERVISOR_GUEST \
	-DCONFIG_KVM_GUEST \
")
```

## E148-009 · panic

fixed time panic CPU canary；function_body；mykernel/debug/panic.c 79–79；Blob 2e9d5b195559；source_line_endings=LF, render_separator=NONE；覆盖=CONFIG。

[VERIFIED mykernel/debug/panic.c::panic]
```c
	this_cpu = smp_processor_id();
```

## E148-010 · RELEASE_FLAG

user root Debug conditional sets -DDEBUG；build_directive；myinitramfs/CMakeLists.txt 13–15；Blob 6757d21bc64c；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::RELEASE_FLAG]
```cmake
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(RELEASE_FLAG "-DDEBUG")
	message("${PROMPT} DEBUG ${PROMPT_END}")
```

## E148-011 · RELEASE_FLAG

other build-type text sets release flag；build_directive；myinitramfs/CMakeLists.txt 16–20；Blob 6757d21bc64c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::RELEASE_FLAG]
```cmake
elseif ()
	message("${PROMPT} RELEASE")
	set(RELEASE_FLAG "-DRELEASE")
	set(UNUSED_MACRO "-Wunused-macros")
endif()
```

## E148-012 · CMAKE_C_FLAGS

user C flags m64 and ggdb；build_directive；myinitramfs/CMakeLists.txt 26–29；Blob 6757d21bc64c；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖=BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::CMAKE_C_FLAGS]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 \
	-ggdb \
")
```

## E148-013 · LINK_TYPE

conditional static build flag；build_directive；myinitramfs/CMakeLists.txt 31–33；Blob 6757d21bc64c；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::LINK_TYPE]
```cmake
if (${LINK_TYPE} STREQUAL "static")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
endif()
```

## E148-014 · MUSL_PATH

external musl path and compiler；build_directive；myinitramfs/CMakeLists.txt 40–41；Blob 6757d21bc64c；source_line_endings=LF,LF, render_separator=NONE；覆盖=BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::MUSL_PATH]
```cmake
set(MUSL_PATH /home/cheyh/projs/musl/build)
set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)
```

## E148-015 · MUSL_INC_PATH

external include/lib paths；build_directive；myinitramfs/CMakeLists.txt 43–46；Blob 6757d21bc64c；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖=BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::MUSL_INC_PATH]
```cmake
set(MUSL_INC_PATH ${MUSL_PATH}/include)
set(MUSL_LIB_PATH ${MUSL_PATH}/lib)

include_directories("${MUSL_INC_PATH}")
```

## E148-016 · add_subdirectory

top-level direct myinit and myshell entries；build_directive；myinitramfs/CMakeLists.txt 47–48；Blob 6757d21bc64c；source_line_endings=LF,LF, render_separator=NONE；覆盖=BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myinit)
add_subdirectory(myshell)
```

## E148-017 · install

install init and sh；build_directive；myinitramfs/CMakeLists.txt 50–51；Blob 6757d21bc64c；source_line_endings=LF,NONE, render_separator=LF；覆盖=BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::install]
```cmake
install(TARGETS init DESTINATION boot)
install(TARGETS sh DESTINATION boot)
```

## E148-018 · add_subdirectory

independent mylib enters myglibc/csu/util；build_directive；myinitramfs/mylib/CMakeLists.txt 1–3；Blob 8f6abc24095a；source_line_endings=LF,LF,NONE, render_separator=LF；覆盖=BUILD。

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myglibc)
add_subdirectory(csu)
add_subdirectory(util)
```

## E148-019 · mylibc

standalone static target and flags；build_directive；myinitramfs/mylib/myglibc/CMakeLists.txt 1–2；Blob b956957a602a；source_line_endings=LF,LF, render_separator=NONE；覆盖=BUILD。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::mylibc]
```cmake
add_library(mylibc STATIC)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH}")
```

## E148-020 · add_subdirectory

standalone malloc subdirectory；build_directive；myinitramfs/mylib/myglibc/CMakeLists.txt 18–21；Blob b956957a602a；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖=BUILD。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(stdio)
add_subdirectory(malloc)
add_subdirectory(string)
add_subdirectory(exit)
```

## E148-021 · LIBC_STDLIB_C_SRCS

C glob and target_sources；build_directive；myinitramfs/mylib/myglibc/malloc/CMakeLists.txt 1–3；Blob 3b9f22dd9ab9；source_line_endings=LF,LF,NONE, render_separator=LF；覆盖=BUILD。

[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::LIBC_STDLIB_C_SRCS]
```cmake
file(GLOB LIBC_STDLIB_C_SRCS *.c)
file(GLOB LIBC_STDLIB_ASM_SRCS *.S)
target_sources(mylibc PUBLIC ${LIBC_STDLIB_C_SRCS} ${LIBC_STDLIB_ASM_SRCS})
```

## E148-022 · UTIL_C_SRCS

C glob and target_sources；build_directive；myinitramfs/mylib/util/CMakeLists.txt 1–3；Blob 5fd782a35629；source_line_endings=LF,LF,NONE, render_separator=LF；覆盖=BUILD。

[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::UTIL_C_SRCS]
```cmake
file(GLOB UTIL_C_SRCS *.c)
file(GLOB UTIL_ASM_SRCS *.S)
target_sources(mylibc PUBLIC ${UTIL_C_SRCS} ${UTIL_ASM_SRCS})
```

## E148-023 · base_uslabs

static storage array of base slabs (no explicit initializer)；global_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 67–67；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::base_uslabs]
```c
uslab_s			base_uslabs[USLAB_LEVEL];
```

## E148-024 · base_upages

base page descriptor array；global_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 68–68；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::base_upages]
```c
uPage_s			base_upages[USLAB_LEVEL];
```

## E148-025 · free_upage_lhdr

free page header；global_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 69–69；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=B。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free_upage_lhdr]
```c
List_hdr_s		free_upage_lhdr;
```

## E148-026 · used_upage_lhdr

used page header；global_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 70–70；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=B。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::used_upage_lhdr]
```c
List_hdr_s		used_upage_lhdr;
```

## E148-027 · init_uslab

base slab chosen by index and list node initialized；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 191–192；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF, render_separator=NONE；覆盖=BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		uslab_s * bslp = &base_uslabs[i];
		INIT_LIST_S(&bslp->uslab_list);
```

## E148-028 · init_uslab

base slab tail insertion and base pointer/counters assignment；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 198–202；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		list_header_add_to_tail(&scgp->normal_slab_free, &bslp->uslab_list);
		scgp->normal_base_slab_p = bslp;
		scgp->nsobj_free_count = obj_nr;
		scgp->nsobj_used_count = 0;
		scgp->normal_slab_total = 1;
```

## E148-029 · upage_free

complete first part: unlink, decrement used count, call head insertion；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 143–147；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=B,C0。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_free]
```c
void upage_free(uPage_s * upgp)
{
	list_del_init(&upgp->upage_list);
	used_upage_lhdr.count--;
	list_header_add_to_head(&free_upage_lhdr, &upgp->upage_list);
```

## E148-030 · upage_free

clear page owner and end; no slab reverse pointer write；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 148–149；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF, render_separator=NONE；覆盖=B,C0。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_free]
```c
	upgp->uslab_p = NULL;
}
```

## E148-031 · uslab_free

page call then free(colormap) boundary；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 238–242；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=C0,C1,C4。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_free]
```c
void uslab_free(uslab_s * slp)
{
	upage_free(slp->upage_p);

	free(slp->colormap);
```

## E148-032 · uslab_free

post-call unlink, read current owner, decrement count then free(slp)；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 243–247；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=C1,C4。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_free]
```c
	list_del_init(&slp->uslab_list);
	slp->uslabcache_p->normal_slab_free.count--;

	free(slp);
}
```

## E148-033 · free

if count>2, tail/container_of, caller count-- and opaque uslab_free call；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 381–385；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖=A,C4。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	if (uscgp->normal_slab_free.count > 2)
	{
		uslab_s * tmp_uslp = container_of(uscgp->normal_slab_free.anchor.prev, uslab_s, uslab_list);
		uscgp->normal_slab_free.count--;
		uslab_free(tmp_uslp);
```

## E148-034 · free

discarded normal_slab_total expression, then CF -= tmp->total; no total decrement；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 386–388；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF, render_separator=NONE；覆盖=C2,C3。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
		uscgp->normal_slab_total;
		uscgp->nsobj_free_count -= tmp_uslp->total;
	}
```

## E148-035 · free

function end following single if; no second loop in this tail；function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 389–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,NONE, render_separator=LF；覆盖=A,C3。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c

	// unlock_recurs_lock(&slab_alloc_lock);
}
```

## E148-036 · free

comment describes excess free slab intent, not executable condition；comment；myinitramfs/mylib/myglibc/malloc/malloc.c 380–380；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖=A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	// if there is too many free slab, free some of them
```

## E148-037 · uPage

page node, vaddr, owner fields；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 19–23；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=TYPE,B。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage]
```c
	typedef struct uPage
	{
		List_s	upage_list;
		void	*vaddr;
		uslab_s *uslab_p;
```

## E148-038 · uPage

vaddr, owner and uPage_s alias；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 22–24；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=TYPE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage]
```c
		void	*vaddr;
		uslab_s *uslab_p;
	} uPage_s;
```

## E148-039 · uslab

slab node and current cache pointer；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 26–30；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=TYPE,C1。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab]
```c
	typedef struct uslab
	{
		List_s			uslab_list;
		uslab_cache_s	*uslabcache_p;

```

## E148-040 · uslab

total/free, reverse page pointer, virtual address；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 31–35；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=TYPE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab]
```c
		unsigned long	total;
		unsigned long	free;

		uPage_s			*upage_p;
		void			*virt_addr;
```

## E148-041 · uslab

page pointer, virt_addr, colormap, alias；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 34–37；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖=TYPE,C0。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab]
```c
		uPage_s			*upage_p;
		void			*virt_addr;
		bitmap_t		*colormap;
	} uslab_s;
```

## E148-042 · uslab_cache

cache list and obj_size；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 39–43；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=TYPE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache]
```c
	typedef struct uslab_cache
	{
		List_s			uslabcache_list;

		unsigned long	obj_size;
```

## E148-043 · uslab_cache

slab total, three headers and CF；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 45–49；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=TYPE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache]
```c
		unsigned long	normal_slab_total;
		List_hdr_s		normal_slab_free;
		List_hdr_s		normal_slab_used;
		List_hdr_s		normal_slab_full;
		unsigned long	nsobj_free_count;
```

## E148-044 · uslab_cache

CF/CU object counters；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 49–50；Blob 2dea1f8fc05c；source_line_endings=LF,LF, render_separator=NONE；覆盖=TYPE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache]
```c
		unsigned long	nsobj_free_count;
		unsigned long	nsobj_used_count;
```

## E148-045 · uslab_cache

base slab protection intent only；comment；myinitramfs/mylib/myglibc/malloc/_malloc.h 52–52；Blob 2dea1f8fc05c；source_line_endings=LF, render_separator=NONE；覆盖=BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache]
```c
		// the base slab should not be freed
```

## E148-046 · uslab_cache

normal_base_slab_p field and alias；struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 53–54；Blob 2dea1f8fc05c；source_line_endings=LF,LF, render_separator=NONE；覆盖=BASE,TYPE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache]
```c
		uslab_s			*normal_base_slab_p;
	} uslab_cache_s;
```

## E148-047 · container_of

actual macro member offset expression；macro_definition；myinitramfs/mylib/include/utils.h 12–16；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=TYPE,A。

[VERIFIED myinitramfs/mylib/include/utils.h::container_of]
```c
	#define container_of(member_ptr, container_type, member_name)		\
	({																	\
		typeof(((container_type *)0)->member_name) * p = (member_ptr);	\
		(container_type *)((unsigned long)p - 							\
		(unsigned long)&(((container_type *)0)->member_name));			\
```

## E148-048 · container_of

macro tail；macro_definition；myinitramfs/mylib/include/utils.h 15–17；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖=TYPE,A。

[VERIFIED myinitramfs/mylib/include/utils.h::container_of]
```c
		(container_type *)((unsigned long)p - 							\
		(unsigned long)&(((container_type *)0)->member_name));			\
	})
```

## E148-049 · bitmap_t

bitmap_t type, pointer field only in this batch；typedef_definition；myinitramfs/mylib/include/utils.h 19–19；Blob 4e3e8910ac00；source_line_endings=LF, render_separator=NONE；覆盖=TYPE。

[VERIFIED myinitramfs/mylib/include/utils.h::bitmap_t]
```c
	typedef __UINT64_TYPE__ bitmap_t;
```

## E148-050 · List

node prev/next and alias；struct_definition；myinitramfs/mylib/include/utils.h 35–39；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=TYPE,LIST。

[VERIFIED myinitramfs/mylib/include/utils.h::List]
```c
	typedef struct List
	{
		List_s *	prev;
		List_s *	next;
	} List_s;
```

## E148-051 · List_hdr

anchor/count members；struct_definition；myinitramfs/mylib/include/utils.h 41–45；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=TYPE,LIST。

[VERIFIED myinitramfs/mylib/include/utils.h::List_hdr]
```c
	typedef struct List_hdr
	{
		List_s			anchor;
		unsigned long	count;
	} List_hdr_s;
```

## E148-052 · INIT_LIST_S

complete self-loop helper writes prev then next；function_body；myinitramfs/mylib/util/list.c 10–14；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LIST,B,C1。

[VERIFIED myinitramfs/mylib/util/list.c::INIT_LIST_S]
```c
void INIT_LIST_S(List_s * src)
{
	src->prev = src;
	src->next = src;
}
```

## E148-053 · list_insert_next

complete entry to insert next: unlink/self-init then next write；function_body；myinitramfs/mylib/util/list.c 29–33；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LIST,B。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_next]
```c
void list_insert_next(List_s * dst, List_s * src)
{
	list_del_init(src);

	src->next = dst->next;
```

## E148-054 · list_insert_next

prev then neighbor writes and helper end；function_body；myinitramfs/mylib/util/list.c 34–38；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LIST,B。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_next]
```c
	src->prev = dst;

	src->next->prev = src;
	src->prev->next = src;
}
```

## E148-055 · list_del_init

neighbor prev/next rewrites and INIT_LIST_S；function_body；myinitramfs/mylib/util/list.c 40–44；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LIST,B,C1。

[VERIFIED myinitramfs/mylib/util/list.c::list_del_init]
```c
void list_del_init(List_s *src)
{
	src->prev->next = src->next;
	src->next->prev = src->prev;
	INIT_LIST_S(src);
```

## E148-056 · list_del_init

definition end；function_body；myinitramfs/mylib/util/list.c 45–45；Blob 24f7a214a288；source_line_endings=LF, render_separator=NONE；覆盖=LIST,B,C1。

[VERIFIED myinitramfs/mylib/util/list.c::list_del_init]
```c
}
```

## E148-057 · list_header_add_to_head

complete wrapper calls insert next then count++；function_body；myinitramfs/mylib/util/list.c 79–83；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖=LIST,B。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_add_to_head]
```c
void list_header_add_to_head(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_next(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}
```

