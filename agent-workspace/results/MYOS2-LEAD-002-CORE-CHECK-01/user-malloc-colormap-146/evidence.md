---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: batch146_evidence
produced_by: GPT-6.2
model_per_owner: "GPT-6.2 Sol / Extra or high requested; backend and actual effort UNATTESTED"
date: 2026-11-24
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
read_channel: connector
status: AUTHOR_PENDING_MAINLINE_REVIEW
open_questions: []
---

# 固定源码新正式窄窗

原源码物理行逐字保留，包括 CRLF。若源末物理行无行尾，围栏前只增加一枚排版 LF；source_index 对每窗保存 source_line_endings 和 render_separator，按该字段逆向去除该额外 LF。旧145的55窗是本轮重核记录而不是新前缀，见 facts.D00.R145-01；其3个 NONE 逐ID还原见 R145-02。

## E146-001 · CMAKE_C_FLAGS

active kernel C flag block begins: m64/kernel model/no PIE/no PIC, freestanding and no standard includes/libs。configuration_directive；mykernel/scripts/options_flags.cmake 39–43；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 R145-02,BUILD。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

## E146-002 · CMAKE_C_FLAGS

active kernel C flags stack/unwind/static/sections/debug。configuration_directive；mykernel/scripts/options_flags.cmake 44–48；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 R145-02,BUILD。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-fno-stack-protector \
	-fno-unwind-tables \
	-static \
	-fdata-sections -ffunction-sections \
	-ggdb \
```

## E146-003 · CMAKE_C_FLAGS

sysv ABI and no red zone, close C flags。configuration_directive；mykernel/scripts/options_flags.cmake 49–51；Blob 6019436417ad；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖 R145-02,BUILD。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-mabi=sysv \
	-mno-red-zone \
")
```

## E146-004 · CMAKE_C_FLAGS

kernel option definitions incl x86_64, FLATMEM, NR_CPUS, CONFIG_64BIT。configuration_directive；mykernel/scripts/options_flags.cmake 58–62；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 R145-02,BUILD。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

## E146-005 · CMAKE_C_FLAGS

kernel zones/SLUB/syscall wrapper/BUG and GRUB support。configuration_directive；mykernel/scripts/options_flags.cmake 63–67；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 R145-02,BUILD。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
	-DGRUB2_BOOTUP_SUPPORT \
```

## E146-006 · CMAKE_C_FLAGS

guest options at end of active kernel macro block。configuration_directive；mykernel/scripts/options_flags.cmake 68–70；Blob 6019436417ad；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖 R145-02,BUILD。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_HYPERVISOR_GUEST \
	-DCONFIG_KVM_GUEST \
")
```

## E146-007 · panic

time branch canary uses smp_processor_id。function_body；mykernel/debug/panic.c 79–79；Blob 2e9d5b195559；source_line_endings=LF, render_separator=NONE；覆盖 BUILD。

[VERIFIED mykernel/debug/panic.c::panic]
```c
	this_cpu = smp_processor_id();
```

## E146-008 · CMAKE_C_FLAGS

user initramfs C flags m64 and ggdb。build_directive；myinitramfs/CMakeLists.txt 26–29；Blob 6757d21bc64c；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖 BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::CMAKE_C_FLAGS]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 \
	-ggdb \
")
```

## E146-009 · LINK_TYPE

static flag conditional on LINK_TYPE。build_directive；myinitramfs/CMakeLists.txt 31–33；Blob 6757d21bc64c；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖 BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::LINK_TYPE]
```cmake
if (${LINK_TYPE} STREQUAL "static")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
endif()
```

## E146-010 · MUSL_PATH

external musl path literal。build_directive；myinitramfs/CMakeLists.txt 40–40；Blob 6757d21bc64c；source_line_endings=LF, render_separator=NONE；覆盖 BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::MUSL_PATH]
```cmake
set(MUSL_PATH /home/cheyh/projs/musl/build)
```

## E146-011 · CMAKE_C_COMPILER

compiler expression uses MUSL_PATH。build_directive；myinitramfs/CMakeLists.txt 41–41；Blob 6757d21bc64c；source_line_endings=LF, render_separator=NONE；覆盖 BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::CMAKE_C_COMPILER]
```cmake
set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)
```

## E146-012 · add_subdirectory

include directory then direct myinit/myshell subdirectory entries。build_directive；myinitramfs/CMakeLists.txt 46–48；Blob 6757d21bc64c；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖 BUILD。

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory]
```cmake
include_directories("${MUSL_INC_PATH}")
add_subdirectory(myinit)
add_subdirectory(myshell)
```

## E146-013 · add_subdirectory

independent mylib enters myglibc/csu/util。build_directive；myinitramfs/mylib/CMakeLists.txt 1–3；Blob 8f6abc24095a；source_line_endings=LF,LF,NONE, render_separator=LF；覆盖 BUILD。

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myglibc)
add_subdirectory(csu)
add_subdirectory(util)
```

## E146-014 · mylibc

static mylibc target and flags。build_directive；myinitramfs/mylib/myglibc/CMakeLists.txt 1–2；Blob b956957a602a；source_line_endings=LF,LF, render_separator=NONE；覆盖 BUILD。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::mylibc]
```cmake
add_library(mylibc STATIC)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH}")
```

## E146-015 · add_subdirectory

malloc source subdir included by independent target。build_directive；myinitramfs/mylib/myglibc/CMakeLists.txt 18–21；Blob b956957a602a；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖 BUILD。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(stdio)
add_subdirectory(malloc)
add_subdirectory(string)
add_subdirectory(exit)
```

## E146-016 · LIBC_STDLIB_C_SRCS

C GLOB definition and target_sources variable consumption (ASM separate)。build_directive；myinitramfs/mylib/myglibc/malloc/CMakeLists.txt 1–3；Blob 3b9f22dd9ab9；source_line_endings=LF,LF,NONE, render_separator=LF；覆盖 R145-02,BUILD。

[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::LIBC_STDLIB_C_SRCS]
```cmake
file(GLOB LIBC_STDLIB_C_SRCS *.c)
file(GLOB LIBC_STDLIB_ASM_SRCS *.S)
target_sources(mylibc PUBLIC ${LIBC_STDLIB_C_SRCS} ${LIBC_STDLIB_ASM_SRCS})
```

## E146-017 · UTIL_C_SRCS

C GLOB definition and target_sources variable consumption (ASM separate)。build_directive；myinitramfs/mylib/util/CMakeLists.txt 1–3；Blob 5fd782a35629；source_line_endings=LF,LF,NONE, render_separator=LF；覆盖 R145-02,BUILD。

[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::UTIL_C_SRCS]
```cmake
file(GLOB UTIL_C_SRCS *.c)
file(GLOB UTIL_ASM_SRCS *.S)
target_sources(mylibc PUBLIC ${UTIL_C_SRCS} ${UTIL_ASM_SRCS})
```

## E146-018 · CONST_2M

local numeric constant 0x200000ULL。macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 15–15；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 R145-02,A,B,C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::CONST_2M]
```c
	#define CONST_2M		0x200000ULL
```

## E146-019 · PAGE_SIZE

local PAGE_SIZE expands to CONST_2M。macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 60–60；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 R145-02,A,B,C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::PAGE_SIZE]
```c
#define PAGE_SIZE			CONST_2M
```

## E146-020 · CONST_1M

local size upper bound 0x100000ULL。macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 12–12；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 B。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::CONST_1M]
```c
	#define CONST_1M		0x100000ULL
```

## E146-021 · initiated

global initiated initialized false; B independently gives true。global_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 62–62；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 B。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::initiated]
```c
bool initiated = false;
```

## E146-022 · USLAB_LEVEL

16 groups。macro_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 7–7；Blob 2dea1f8fc05c；source_line_endings=LF, render_separator=NONE；覆盖 A,B。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::USLAB_LEVEL]
```c
	#define USLAB_LEVEL			16
```

## E146-023 · USLAB_SIZE_BASE

32 byte starting size under given model。macro_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 8–8；Blob 2dea1f8fc05c；source_line_endings=LF, render_separator=NONE；覆盖 A,B。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::USLAB_SIZE_BASE]
```c
	#define USLAB_SIZE_BASE		32
```

## E146-024 · bitmap_t

bitmap_t uses __UINT64_TYPE__; 64 bits as given model。typedef_definition；myinitramfs/mylib/include/utils.h 19–19；Blob 4e3e8910ac00；source_line_endings=LF, render_separator=NONE；覆盖 A,B,C。

[VERIFIED myinitramfs/mylib/include/utils.h::bitmap_t]
```c
	typedef __UINT64_TYPE__ bitmap_t;
```

## E146-025 · BITMAP_UNITSIZE

unit is sizeof(bitmap_t)*8。macro_definition；myinitramfs/mylib/include/utils.h 20–20；Blob 4e3e8910ac00；source_line_endings=LF, render_separator=NONE；覆盖 A,B,C。

[VERIFIED myinitramfs/mylib/include/utils.h::BITMAP_UNITSIZE]
```c
	#define BITMAP_UNITSIZE (sizeof(bitmap_t) * 8)
```

## E146-026 · cm_1MB

actual declared bitmap_t[1] without explicit initializer; not actual payload。global_array_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 72–72；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_1MB]
```c
bitmap_t		cm_1MB[1];		// 2		bits
```

## E146-027 · cm_512KB

actual declared bitmap_t[1] without explicit initializer; not actual payload。global_array_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 73–73；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_512KB]
```c
bitmap_t		cm_512KB[1];	// 4		bits
```

## E146-028 · cm_256KB

actual declared bitmap_t[1] without explicit initializer; not actual payload。global_array_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 74–74；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_256KB]
```c
bitmap_t		cm_256KB[1];	// 8		bits
```

## E146-029 · cm_128KB

actual declared bitmap_t[1] without explicit initializer; not actual payload。global_array_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 75–75；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_128KB]
```c
bitmap_t		cm_128KB[1];	// 16		bits
```

## E146-030 · cm_64KB

actual declared bitmap_t[1] without explicit initializer; not actual payload。global_array_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 76–76；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_64KB]
```c
bitmap_t		cm_64KB[1];		// 32		bits
```

## E146-031 · cm_32KB

actual declared bitmap_t[1] without explicit initializer; not actual payload。global_array_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 77–77；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_32KB]
```c
bitmap_t		cm_32KB[1];		// 64		bits
```

## E146-032 · cm_16KB

actual declared bitmap_t[2] without explicit initializer; not actual payload。global_array_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 78–78；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_16KB]
```c
bitmap_t		cm_16KB[2];		// 128		bits
```

## E146-033 · cm_8KB

actual declared bitmap_t[4] without explicit initializer; not actual payload。global_array_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 79–79；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_8KB]
```c
bitmap_t		cm_8KB[4];		// 256		bits
```

## E146-034 · cm_4KB

actual declared bitmap_t[8] without explicit initializer; not actual payload。global_array_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 80–80；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_4KB]
```c
bitmap_t		cm_4KB[8];		// 512		bits
```

## E146-035 · cm_2KB

actual declared bitmap_t[16] without explicit initializer; not actual payload。global_array_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 81–81；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_2KB]
```c
bitmap_t		cm_2KB[16];		// 1024		bits
```

## E146-036 · cm_1KB

actual declared bitmap_t[32] without explicit initializer; not actual payload。global_array_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 82–82；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_1KB]
```c
bitmap_t		cm_1KB[32];		// 2048		bits
```

## E146-037 · cm_512B

actual declared bitmap_t[64] without explicit initializer; not actual payload。global_array_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 83–83；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_512B]
```c
bitmap_t		cm_512B[64];	// 4096		bits
```

## E146-038 · cm_256B

actual declared bitmap_t[128] without explicit initializer; not actual payload。global_array_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 84–84；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_256B]
```c
bitmap_t		cm_256B[128];	// 8192		bits
```

## E146-039 · cm_128B

actual declared bitmap_t[256] without explicit initializer; not actual payload。global_array_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 85–85；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_128B]
```c
bitmap_t		cm_128B[256];	// 16384	bits
```

## E146-040 · cm_64B

actual declared bitmap_t[512] without explicit initializer; not actual payload。global_array_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 86–86；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_64B]
```c
bitmap_t		cm_64B[512];	// 32768	bits
```

## E146-041 · cm_32B

actual declared bitmap_t[1024] without explicit initializer; not actual payload。global_array_definition；myinitramfs/mylib/myglibc/malloc/malloc.c 87–87；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 A。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::cm_32B]
```c
bitmap_t		cm_32B[1024];	// 65536	bits
```

## E146-042 · base_colormaps

ordered pointer initializer entries; no inference of bitmap payload。global_array_initializer；myinitramfs/mylib/myglibc/malloc/malloc.c 89–93；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖 A,C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::base_colormaps]
```c
bitmap_t *		base_colormaps[USLAB_LEVEL] = {
	cm_32B,
	cm_64B,
	cm_128B,
	cm_256B,
```

## E146-043 · base_colormaps

ordered pointer initializer entries; no inference of bitmap payload。global_array_initializer；myinitramfs/mylib/myglibc/malloc/malloc.c 94–98；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖 A,C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::base_colormaps]
```c
	cm_512B,
	cm_1KB,
	cm_2KB,
	cm_4KB,
	cm_8KB,
```

## E146-044 · base_colormaps

ordered pointer initializer entries; no inference of bitmap payload。global_array_initializer；myinitramfs/mylib/myglibc/malloc/malloc.c 99–103；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖 A,C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::base_colormaps]
```c
	cm_16KB,
	cm_32KB,
	cm_64KB,
	cm_128KB,
	cm_256KB,
```

## E146-045 · base_colormaps

ordered pointer initializer entries; no inference of bitmap payload。global_array_initializer；myinitramfs/mylib/myglibc/malloc/malloc.c 104–106；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF, render_separator=NONE；覆盖 A,C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::base_colormaps]
```c
	cm_512KB,
	cm_1MB,
};
```

## E146-046 · init_uslab

brk query and list header initializations; not payload writes。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 176–179；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖 C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
	brk_end = brk(0);
	INIT_LIST_HEADER_S(&uslabcache_lhdr);
	INIT_LIST_HEADER_S(&free_upage_lhdr);
	INIT_LIST_HEADER_S(&used_upage_lhdr);
```

## E146-047 · init_uslab

per-i cache group, node init and tail insertion。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 181–185；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖 A,B,C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
	for (int i = 0; i < USLAB_LEVEL; i++)
	{
		uslab_cache_s * scgp = &uslab_cache_groups[i];
		INIT_LIST_S(&scgp->uslabcache_list);
		list_header_add_to_tail(&uslabcache_lhdr, &scgp->uslabcache_list);
```

## E146-048 · init_uslab

three slab header initializations。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 187–189；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF, render_separator=NONE；覆盖 C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		INIT_LIST_HEADER_S(&scgp->normal_slab_free);
		INIT_LIST_HEADER_S(&scgp->normal_slab_used);
		INIT_LIST_HEADER_S(&scgp->normal_slab_full);
```

## E146-049 · init_uslab

base slab selection and O/N production。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 191–195；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖 A,C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		uslab_s * bslp = &base_uslabs[i];
		INIT_LIST_S(&bslp->uslab_list);

		scgp->obj_size = USLAB_SIZE_BASE << i;
		unsigned long obj_nr = PAGE_SIZE / scgp->obj_size;
```

## E146-050 · init_uslab

O=BASE<<i; N=PAGE_SIZE/O; local cm_size expression with sizeof(bitmap_t)。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 194–196；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF, render_separator=NONE；覆盖 A,C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		scgp->obj_size = USLAB_SIZE_BASE << i;
		unsigned long obj_nr = PAGE_SIZE / scgp->obj_size;
		unsigned long cm_size = (obj_nr + sizeof(bitmap_t) - 1) / sizeof(bitmap_t);
```

## E146-051 · init_uslab

base free list link, base pointer and cache counts。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 198–202；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖 C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		list_header_add_to_tail(&scgp->normal_slab_free, &bslp->uslab_list);
		scgp->normal_base_slab_p = bslp;
		scgp->nsobj_free_count = obj_nr;
		scgp->nsobj_used_count = 0;
		scgp->normal_slab_total = 1;
```

## E146-052 · init_uslab

colormap pointer and owner pointer, free/total chain assignment。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 204–207；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖 C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		bslp->colormap = base_colormaps[i];
		bslp->uslabcache_p = scgp;
		bslp->free =
		bslp->total = obj_nr;
```

## E146-053 · init_uslab

base page metadata/list and brk-dependent writes, not a modeled runtime path。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 210–214；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖 C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		uPage_s * upgp = &base_upages[i];
		INIT_LIST_S(&upgp->upage_list);
		upgp->vaddr =
		brk_end = brk(brk_end + PAGE_SIZE);
		list_header_add_to_tail(&free_upage_lhdr, &upgp->upage_list);
```

## E146-054 · init_uslab

upage_alloc output binding, vaddr/backpointer; initiated=true outside loop。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 216–220；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖 C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		bslp->upage_p = upage_alloc();
		bslp->virt_addr = bslp->upage_p->vaddr;
		bslp->upage_p->uslab_p = bslp;
	}
	initiated = true;
```

## E146-055 · uslab_alloc

metadata malloc(sizeof(uslab_s)) then INIT_LIST_S。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 225–226；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF, render_separator=NONE；覆盖 C_DYNAMIC。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_alloc]
```c
	uslab_s * nslp = (uslab_s *)malloc(sizeof(uslab_s));
	INIT_LIST_S(&nslp->uslab_list);
```

## E146-056 · uslab_alloc

bitmap malloc request first total read and free/total second total read。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 228–230；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF, render_separator=NONE；覆盖 B,C_DYNAMIC。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_alloc]
```c
	nslp->colormap = (bitmap_t *)malloc(cslp->total / sizeof(bitmap_t) + sizeof(bitmap_t));
	nslp->total =
	nslp->free = cslp->total;
```

## E146-057 · uslab_alloc

upage_alloc, virt_addr and page backpointer then return nslp。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 231–235；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖 C_DYNAMIC。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_alloc]
```c
	nslp->upage_p = upage_alloc();
	nslp->virt_addr = nslp->upage_p->vaddr;
	nslp->upage_p->uslab_p = nslp;

	return nslp;
```

## E146-058 · malloc

init gate before local result initialization and size bound。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 251–255；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖 B。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
	if (!initiated)
		init_uslab();

	void * ret_val = NULL;
	if (size > CONST_1M)
```

## E146-059 · malloc

size>CONST_1M empty branch with NULL initialized before it。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 254–258；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖 B。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
	void * ret_val = NULL;
	if (size > CONST_1M)
	{
		// color_printk(RED, WHITE, "Mem required too big\n");
	}
```

## E146-060 · malloc

group0 and while advancement until size<=obj_size。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 262–264；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF, render_separator=NONE；覆盖 B。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		uslab_cache_s *	uscgp = &uslab_cache_groups[0];
		while (size > uscgp->obj_size)
			uscgp = container_of((uscgp->uslabcache_list.next), uslab_cache_s, uslabcache_list);
```

## E146-061 · malloc

condition, temporary count++ and uslab_alloc call; count-- after return。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 308–312；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖 C_PUBLISH,R145-03。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		if (uscgp->normal_slab_free.count < 1)
		{
			uscgp->normal_slab_free.count++;
			uslab_s * new_uslp = uslab_alloc(uslp);
			uscgp->normal_slab_free.count--;
```

## E146-062 · malloc

normal return then decrement; NULL conditional and loop。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 312–316；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖 C_PUBLISH,R145-03。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			uscgp->normal_slab_free.count--;
			if (new_uslp == NULL)
			{
				// color_printk(RED, WHITE, "Alloc new slab :%#x-bytes failed!\n", scgp->obj_size);
				while (1);
```

## E146-063 · malloc

caller owner binding, free list insertion, total and free-count increase。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 318–322；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF, render_separator=NONE；覆盖 C_PUBLISH,R145-03。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			new_uslp->uslabcache_p = uscgp;
			list_header_add_to_head(&uscgp->normal_slab_free, &new_uslp->uslab_list);
			uscgp->normal_slab_total++;
			uscgp->nsobj_free_count += new_uslp->total;
		}
```

## E146-064 · malloc

function return exists after refill block; C_PUBLISH stops earlier。function_body；myinitramfs/mylib/myglibc/malloc/malloc.c 327–327；Blob 44db9ea9d7f7；source_line_endings=CRLF, render_separator=NONE；覆盖 C_PUBLISH。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
	return ret_val;
```

## E146-065 · uPage

page list/address/owner members。struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 19–23；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 C_DYNAMIC,C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage]
```c
	typedef struct uPage
	{
		List_s	upage_list;
		void	*vaddr;
		uslab_s *uslab_p;
```

## E146-066 · uPage

vaddr/uslab_p members and uPage_s alias。struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 22–24；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖 C_DYNAMIC,C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage]
```c
		void	*vaddr;
		uslab_s *uslab_p;
	} uPage_s;
```

## E146-067 · uslab

slab list and cache owner。struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 26–30；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 C_DYNAMIC,C_PUBLISH。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab]
```c
	typedef struct uslab
	{
		List_s			uslab_list;
		uslab_cache_s	*uslabcache_p;

```

## E146-068 · uslab

total/free and upage_p/virt_addr。struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 31–35；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 C_DYNAMIC,C_PUBLISH。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab]
```c
		unsigned long	total;
		unsigned long	free;

		uPage_s			*upage_p;
		void			*virt_addr;
```

## E146-069 · uslab

upage_p, virt_addr, colormap and alias。struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 34–37；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖 C_DYNAMIC,C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab]
```c
		uPage_s			*upage_p;
		void			*virt_addr;
		bitmap_t		*colormap;
	} uslab_s;
```

## E146-070 · uslab_cache

cache list node and obj_size。struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 39–43；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 A,B。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache]
```c
	typedef struct uslab_cache
	{
		List_s			uslabcache_list;

		unsigned long	obj_size;
```

## E146-071 · uslab_cache

total, three slab headers, free count。struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 45–49；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 C_PUBLISH,C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache]
```c
		unsigned long	normal_slab_total;
		List_hdr_s		normal_slab_free;
		List_hdr_s		normal_slab_used;
		List_hdr_s		normal_slab_full;
		unsigned long	nsobj_free_count;
```

## E146-072 · uslab_cache

object free and used aggregate fields。struct_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h 49–50；Blob 2dea1f8fc05c；source_line_endings=LF,LF, render_separator=NONE；覆盖 C_PUBLISH,C_BASE。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache]
```c
		unsigned long	nsobj_free_count;
		unsigned long	nsobj_used_count;
```

## E146-073 · List

List prev/next members and List_s alias。struct_definition；myinitramfs/mylib/include/utils.h 35–39；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 R145-02,C_DYNAMIC,C_PUBLISH。

[VERIFIED myinitramfs/mylib/include/utils.h::List]
```c
	typedef struct List
	{
		List_s *	prev;
		List_s *	next;
	} List_s;
```

## E146-074 · List_hdr

List_hdr anchor/count and alias。struct_definition；myinitramfs/mylib/include/utils.h 41–45；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 C_PUBLISH,C_BASE。

[VERIFIED myinitramfs/mylib/include/utils.h::List_hdr]
```c
	typedef struct List_hdr
	{
		List_s			anchor;
		unsigned long	count;
	} List_hdr_s;
```

## E146-075 · container_of

container_of active macro expression; tail shown separately。macro_definition；myinitramfs/mylib/include/utils.h 12–16；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 B。

[VERIFIED myinitramfs/mylib/include/utils.h::container_of]
```c
	#define container_of(member_ptr, container_type, member_name)		\
	({																	\
		typeof(((container_type *)0)->member_name) * p = (member_ptr);	\
		(container_type *)((unsigned long)p - 							\
		(unsigned long)&(((container_type *)0)->member_name));			\
```

## E146-076 · container_of

container_of macro tail。macro_definition；myinitramfs/mylib/include/utils.h 15–17；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF, render_separator=NONE；覆盖 B。

[VERIFIED myinitramfs/mylib/include/utils.h::container_of]
```c
		(container_type *)((unsigned long)p - 							\
		(unsigned long)&(((container_type *)0)->member_name));			\
	})
```

## E146-077 · INIT_LIST_S

direct prev/next self-link writes。function_body；myinitramfs/mylib/util/list.c 10–14；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 R145-02,C_DYNAMIC。

[VERIFIED myinitramfs/mylib/util/list.c::INIT_LIST_S]
```c
void INIT_LIST_S(List_s * src)
{
	src->prev = src;
	src->next = src;
}
```

## E146-078 · list_insert_prev

unlink then next/prev assignments。function_body；myinitramfs/mylib/util/list.c 19–22；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖 R145-02,C_BASE。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_prev]
```c
	list_del_init(src);

	src->next = dst;
	src->prev = dst->prev;
```

## E146-079 · list_insert_prev

neighbor back-links for insert prev。function_body；myinitramfs/mylib/util/list.c 24–25；Blob 24f7a214a288；source_line_endings=LF,LF, render_separator=NONE；覆盖 R145-02,C_BASE。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_prev]
```c
	src->next->prev = src;
	src->prev->next = src;
```

## E146-080 · list_insert_next

unlink then next/prev assignments。function_body；myinitramfs/mylib/util/list.c 31–34；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖 R145-02,C_PUBLISH。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_next]
```c
	list_del_init(src);

	src->next = dst->next;
	src->prev = dst;
```

## E146-081 · list_insert_next

neighbor back-links for insert next。function_body；myinitramfs/mylib/util/list.c 36–37；Blob 24f7a214a288；source_line_endings=LF,LF, render_separator=NONE；覆盖 R145-02,C_PUBLISH。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_next]
```c
	src->next->prev = src;
	src->prev->next = src;
```

## E146-082 · list_del_init

unlink and reinitialize entry。function_body；myinitramfs/mylib/util/list.c 40–44；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 C_PUBLISH,C_BASE,R145-02。

[VERIFIED myinitramfs/mylib/util/list.c::list_del_init]
```c
void list_del_init(List_s *src)
{
	src->prev->next = src->next;
	src->next->prev = src->prev;
	INIT_LIST_S(src);
```

## E146-083 · INIT_LIST_HEADER_S

count=0 and nested anchor initialization。function_body；myinitramfs/mylib/util/list.c 73–77；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 C_BASE。

[VERIFIED myinitramfs/mylib/util/list.c::INIT_LIST_HEADER_S]
```c
void INIT_LIST_HEADER_S(List_hdr_s * lhdr_p)
{
	lhdr_p->count = 0;
	INIT_LIST_S(&lhdr_p->anchor);
}
```

## E146-084 · list_header_add_to_head

insert next then header count++。function_body；myinitramfs/mylib/util/list.c 79–83；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 C_PUBLISH。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_add_to_head]
```c
void list_header_add_to_head(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_next(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}
```

## E146-085 · list_header_remove_head

count check and head selection。function_body；myinitramfs/mylib/util/list.c 85–89；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 R145-03。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_remove_head]
```c
List_s * list_header_remove_head(List_hdr_s * lhdr_p)
{
	if (lhdr_p->count > 0)
	{
		List_s * ret_val = lhdr_p->anchor.next;
```

## E146-086 · list_header_remove_head

selected head, unlink, count-- and return。function_body；myinitramfs/mylib/util/list.c 89–93；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 R145-03。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_remove_head]
```c
		List_s * ret_val = lhdr_p->anchor.next;
		list_del_init(ret_val);
		lhdr_p->count--;
		return ret_val;
	}
```

## E146-087 · list_header_add_to_tail

insert prev then header count++。function_body；myinitramfs/mylib/util/list.c 98–102；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 C_BASE。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_add_to_tail]
```c
void list_header_add_to_tail(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_prev(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}
```

## E146-088 · bm_get_freebit_idx

exclusive i<limit scan。function_body；myinitramfs/mylib/util/bitmap.c 24–28；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 A,B。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_freebit_idx]
```c
unsigned long bm_get_freebit_idx(bitmap_t * bm, unsigned long start, unsigned long limit)
{
	unsigned long 	i;
	for (i = start; i < limit; i++)
	{
```

## E146-089 · bm_get_freebit_idx

word index i/BITMAP_UNITSIZE and zero-bit condition; break and return i。function_body；myinitramfs/mylib/util/bitmap.c 29–33；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF,LF, render_separator=NONE；覆盖 A,B。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_freebit_idx]
```c
		bitmap_t bm_unit = *(bm + (i / BITMAP_UNITSIZE));
		if ((bm_unit & (1ULL << i % BITMAP_UNITSIZE)) == 0)
			break;
	}
	return i;
```

## E146-090 · bm_set_bit

word and mask for separate set helper。function_body；myinitramfs/mylib/util/bitmap.c 14–17；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF, render_separator=NONE；覆盖 R145-03。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_set_bit]
```c
void bm_set_bit(bitmap_t * bm, unsigned long idx)
{
	*(bm + idx / BITMAP_UNITSIZE) |= (1ULL << (idx % BITMAP_UNITSIZE));
}
```

