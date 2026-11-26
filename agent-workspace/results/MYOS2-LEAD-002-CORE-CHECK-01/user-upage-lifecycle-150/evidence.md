---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
batch_id: "CORE-USER-UPAGE-LIFECYCLE-150"
conversation_display_name: "MYOS2-A-C10c 代码分析执行"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-24"
produced_by: "GPT-6.2"
model_per_owner: "GPT-6.2 Sol；Extra／高推理强度（Owner指定）"
actual_model_identifier_basis: "系统标识GPT-6.2；客户端具体选择不可见"
actual_backend_model: "UNATTESTED"
actual_reasoning_effort: "UNATTESTED"
date: "2026-11-26"
date_timezone: "Asia/Shanghai"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_pin_short12: "dc12f97a7a91"
execution_parent_short12: "9aa7de497eb0"
read_channel: "connector"
status: "AUTHOR_PENDING_MAINLINE_REVIEW"
full_local_P9: "NOT_RUN"
evidence_class: "SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / CONDITIONAL_FRAGMENT / OBSERVATION / UNKNOWN"
open_questions: ["R149-01作者补正待主线判闭；真实NULL brk可达性、ABI/标准/编译器/DEBUG、实际对象/映射/生命周期、链接及完整递归未知；N1547独立外证UNKNOWN继承。"]
record_type: "batch150_source_evidence"
---

# 第150批固定time正式证据
每窗1–5连续物理行；源码原LF/CRLF保留。末行NONE只在闭围栏前加一个排版LF，按source_index行尾数组和render_separator可逆去除；不是源字符。命令窗均按实际命令调用实例分界，不把相邻异类指令归入同一标签；多行调用的分窗仍位于同一实例中。D00 12新窗，其他技术/构建窗125，旧149的96窗与149主线2窗不计本批。

## E150-001 · include_directories

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行46–46，所属定义/实例范围46–46；实例=include_directories(MUSL_INC_PATH)；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=D00；root include_directories(MUSL_INC_PATH) text。

[VERIFIED myinitramfs/CMakeLists.txt::include_directories]
```cmake
include_directories("${MUSL_INC_PATH}")
```

## E150-002 · add_subdirectory

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行47–47，所属定义/实例范围47–47；实例=add_subdirectory(myinit)；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=D00；root myinit subdirectory invocation。

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myinit)
```

## E150-003 · add_subdirectory

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行48–48，所属定义/实例范围48–48；实例=add_subdirectory(myshell)；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=D00；root myshell subdirectory invocation。

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myshell)
```

## E150-004 · install

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行50–50，所属定义/实例范围50–50；实例=install(TARGETS init DESTINATION boot)；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=D00；install init target to boot invocation。

[VERIFIED myinitramfs/CMakeLists.txt::install]
```cmake
install(TARGETS init DESTINATION boot)
```

## E150-005 · install

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行51–51，所属定义/实例范围51–51；实例=install(TARGETS sh DESTINATION boot)；Blob 6757d21bc64c；source_line_endings=NONE；render_separator=LF；覆盖=D00；install sh target to boot invocation; final physical line NONE。

[VERIFIED myinitramfs/CMakeLists.txt::install]
```cmake
install(TARGETS sh DESTINATION boot)
```

## E150-006 · add_subdirectory

cmake_command_invocation；myinitramfs/mylib/CMakeLists.txt；物理行1–1，所属定义/实例范围1–1；实例=add_subdirectory(myglibc)；Blob 8f6abc24095a；source_line_endings=LF；render_separator=NONE；覆盖=D00；mylib subdirectory myglibc。

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(myglibc)
```

## E150-007 · add_subdirectory

cmake_command_invocation；myinitramfs/mylib/CMakeLists.txt；物理行2–2，所属定义/实例范围2–2；实例=add_subdirectory(csu)；Blob 8f6abc24095a；source_line_endings=LF；render_separator=NONE；覆盖=D00；mylib subdirectory csu。

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(csu)
```

## E150-008 · add_subdirectory

cmake_command_invocation；myinitramfs/mylib/CMakeLists.txt；物理行3–3，所属定义/实例范围3–3；实例=add_subdirectory(util)；Blob 8f6abc24095a；source_line_endings=NONE；render_separator=LF；覆盖=D00；mylib subdirectory util。

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(util)
```

## E150-009 · add_subdirectory

cmake_command_invocation；myinitramfs/mylib/myglibc/CMakeLists.txt；物理行18–18，所属定义/实例范围18–18；实例=add_subdirectory(stdio)；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；覆盖=D00；myglibc subdirectory stdio。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(stdio)
```

## E150-010 · add_subdirectory

cmake_command_invocation；myinitramfs/mylib/myglibc/CMakeLists.txt；物理行19–19，所属定义/实例范围19–19；实例=add_subdirectory(malloc)；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；覆盖=D00；myglibc subdirectory malloc。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(malloc)
```

## E150-011 · add_subdirectory

cmake_command_invocation；myinitramfs/mylib/myglibc/CMakeLists.txt；物理行20–20，所属定义/实例范围20–20；实例=add_subdirectory(string)；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；覆盖=D00；myglibc subdirectory string。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(string)
```

## E150-012 · add_subdirectory

cmake_command_invocation；myinitramfs/mylib/myglibc/CMakeLists.txt；物理行21–21，所属定义/实例范围21–21；实例=add_subdirectory(exit)；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；覆盖=D00；myglibc subdirectory exit。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory]
```cmake
add_subdirectory(exit)
```

## E150-013 · if

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行7–7，所属定义/实例范围7–7；实例=if (NOT DEFINED ${ARCH})；Blob 6019436417ad；source_line_endings=LF；render_separator=NONE；覆盖=CONFIG；kernel configuration conditional/directive text。

[VERIFIED mykernel/scripts/options_flags.cmake::if]
```cmake
if (NOT DEFINED ${ARCH})
```

## E150-014 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行8–8，所属定义/实例范围8–8；实例=set(ARCH x86_64)；Blob 6019436417ad；source_line_endings=LF；render_separator=NONE；覆盖=CONFIG；kernel configuration conditional/directive text。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
	set(ARCH x86_64)
```

## E150-015 · endif

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行9–9，所属定义/实例范围9–9；实例=endif()；Blob 6019436417ad；source_line_endings=LF；render_separator=NONE；覆盖=CONFIG；kernel configuration conditional/directive text。

[VERIFIED mykernel/scripts/options_flags.cmake::endif]
```cmake
endif()
```

## E150-016 · if

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行16–16，所属定义/实例范围16–16；实例=if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")；Blob 6019436417ad；source_line_endings=LF；render_separator=NONE；覆盖=CONFIG；kernel configuration conditional/directive text。

[VERIFIED mykernel/scripts/options_flags.cmake::if]
```cmake
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
```

## E150-017 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行17–17，所属定义/实例范围17–17；实例=set(RELEASE_FLAG "-DDEBUG")；Blob 6019436417ad；source_line_endings=LF；render_separator=NONE；覆盖=CONFIG；kernel configuration conditional/directive text。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
	set(RELEASE_FLAG "-DDEBUG")
```

## E150-018 · else

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行19–19，所属定义/实例范围19–19；实例=else ()；Blob 6019436417ad；source_line_endings=LF；render_separator=NONE；覆盖=CONFIG；kernel configuration conditional/directive text。

[VERIFIED mykernel/scripts/options_flags.cmake::else]
```cmake
else ()
```

## E150-019 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行21–21，所属定义/实例范围21–21；实例=set(RELEASE_FLAG "-DRELEASE")；Blob 6019436417ad；source_line_endings=LF；render_separator=NONE；覆盖=CONFIG；kernel configuration conditional/directive text。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
	set(RELEASE_FLAG "-DRELEASE")
```

## E150-020 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行22–22，所属定义/实例范围22–22；实例=set(UNUSED_MACRO "-Wunused-macros")；Blob 6019436417ad；source_line_endings=LF；render_separator=NONE；覆盖=CONFIG；kernel configuration conditional/directive text。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
	set(UNUSED_MACRO "-Wunused-macros")
```

## E150-021 · endif

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行23–23，所属定义/实例范围23–23；实例=endif()；Blob 6019436417ad；source_line_endings=LF；render_separator=NONE；覆盖=CONFIG；kernel configuration conditional/directive text。

[VERIFIED mykernel/scripts/options_flags.cmake::endif]
```cmake
endif()
```

## E150-022 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行30–34，所属定义/实例范围30–36；实例=set(CMAKE_ASM_FLAGS …)；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=CONFIG；kernel ASM flags command (one multiline instance)。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
set(CMAKE_ASM_FLAGS
	"${CMAKE_ASM_FLAGS} ${RELEASE_FLAG} ${UNUSED_MACRO} \
	-m64 -mcmodel=kernel -fverbose-asm \
	-fno-pie -fno-pic \
	-D__KERNEL__ \
```

## E150-023 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行35–36，所属定义/实例范围30–36；实例=set(CMAKE_ASM_FLAGS …)；Blob 6019436417ad；source_line_endings=LF,LF；render_separator=NONE；覆盖=CONFIG；kernel ASM flags command (one multiline instance)。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
	-D__ASSEMBLY__ -DASM_FILE \
")
```

## E150-024 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行39–43，所属定义/实例范围39–51；实例=set(CMAKE_C_FLAGS … kernel common C flags)；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=CONFIG；kernel C flags command incl canary/mabi; not user ABI。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

## E150-025 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行44–48，所属定义/实例范围39–51；实例=set(CMAKE_C_FLAGS … kernel common C flags)；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=CONFIG；kernel C flags command incl canary/mabi; not user ABI。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
	-fno-stack-protector \
	-fno-unwind-tables \
	-static \
	-fdata-sections -ffunction-sections \
	-ggdb \
```

## E150-026 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行49–51，所属定义/实例范围39–51；实例=set(CMAKE_C_FLAGS … kernel common C flags)；Blob 6019436417ad；source_line_endings=LF,LF,LF；render_separator=NONE；覆盖=CONFIG；kernel C flags command incl canary/mabi; not user ABI。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
	-mabi=sysv \
	-mno-red-zone \
")
```

## E150-027 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行58–62，所属定义/实例范围58–70；实例=set(CMAKE_C_FLAGS … kernel option macros)；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=CONFIG；kernel internal active CONFIG text; one set invocation。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

## E150-028 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行63–67，所属定义/实例范围58–70；实例=set(CMAKE_C_FLAGS … kernel option macros)；Blob 6019436417ad；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=CONFIG；kernel internal active CONFIG text; one set invocation。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
	-DGRUB2_BOOTUP_SUPPORT \
```

## E150-029 · set

cmake_command_invocation；mykernel/scripts/options_flags.cmake；物理行68–70，所属定义/实例范围58–70；实例=set(CMAKE_C_FLAGS … kernel option macros)；Blob 6019436417ad；source_line_endings=LF,LF,LF；render_separator=NONE；覆盖=CONFIG；kernel internal active CONFIG text; one set invocation。

[VERIFIED mykernel/scripts/options_flags.cmake::set]
```cmake
	-DCONFIG_HYPERVISOR_GUEST \
	-DCONFIG_KVM_GUEST \
")
```

## E150-030 · panic

function_body；mykernel/debug/panic.c；物理行79–79，所属定义/实例范围36–214；实例=panic body；Blob 2e9d5b195559；source_line_endings=LF；render_separator=NONE；覆盖=CANARY；time canary this_cpu assignment。

[VERIFIED mykernel/debug/panic.c::panic]
```c
	this_cpu = smp_processor_id();
```

## E150-031 · CONST_1M

macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行12–12，所属定义/实例范围12–12；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=CONSTANT；malloc size gate constant。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::CONST_1M]
```c
	#define CONST_1M		0x100000ULL
```

## E150-032 · CONST_2M

macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行15–15，所属定义/实例范围15–15；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=ROUND；H numeric definition。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::CONST_2M]
```c
	#define CONST_2M		0x200000ULL
```

## E150-033 · __round_mask

macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行36–36，所属定义/实例范围36–36；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=ROUND；type of x cast of y−1。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::__round_mask]
```c
#define __round_mask(x, y) ((__typeof__(x))((y)-1))
```

## E150-034 · round_down

macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行56–56，所属定义/实例范围56–56；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=ROUND；bitwise AND complement of mask。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::round_down]
```c
#define round_down(x, y) ((x) & ~__round_mask(x, y))
```

## E150-035 · PAGE_ROUND_DOWN

macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行59–59，所属定义/实例范围59–59；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=ROUND；round_down(x,PAGE_SIZE)。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::PAGE_ROUND_DOWN]
```c
#define PAGE_ROUND_DOWN(x)	round_down((x), PAGE_SIZE)
```

## E150-036 · PAGE_SIZE

macro_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行60–60，所属定义/实例范围60–60；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=ROUND；user allocator H alias。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::PAGE_SIZE]
```c
#define PAGE_SIZE			CONST_2M
```

## E150-037 · initiated

variable_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行62–62，所属定义/实例范围62–62；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=GLOBALS；actual global declaration/definition; concrete values in models GIVEN。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::initiated]
```c
bool initiated = false;
```

## E150-038 · brk_end

variable_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行63–63，所属定义/实例范围63–63；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=GLOBALS；actual global declaration/definition; concrete values in models GIVEN。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::brk_end]
```c
void *brk_end = NULL;
```

## E150-039 · uslabcache_lhdr

variable_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行65–65，所属定义/实例范围65–65；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=GLOBALS；actual global declaration/definition; concrete values in models GIVEN。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslabcache_lhdr]
```c
List_hdr_s		uslabcache_lhdr;
```

## E150-040 · uslab_cache_groups

variable_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行66–66，所属定义/实例范围66–66；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=GLOBALS；actual global declaration/definition; concrete values in models GIVEN。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_cache_groups]
```c
uslab_cache_s	uslab_cache_groups[USLAB_LEVEL];
```

## E150-041 · free_upage_lhdr

variable_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行69–69，所属定义/实例范围69–69；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=GLOBALS；actual global declaration/definition; concrete values in models GIVEN。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free_upage_lhdr]
```c
List_hdr_s		free_upage_lhdr;
```

## E150-042 · used_upage_lhdr

variable_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行70–70，所属定义/实例范围70–70；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=GLOBALS；actual global declaration/definition; concrete values in models GIVEN。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::used_upage_lhdr]
```c
List_hdr_s		used_upage_lhdr;
```

## E150-043 · USLAB_LEVEL

macro_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理行7–7，所属定义/实例范围7–7；Blob 2dea1f8fc05c；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；16 classes。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::USLAB_LEVEL]
```c
	#define USLAB_LEVEL			16
```

## E150-044 · USLAB_SIZE_BASE

macro_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理行8–8，所属定义/实例范围8–8；Blob 2dea1f8fc05c；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；32 base size。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::USLAB_SIZE_BASE]
```c
	#define USLAB_SIZE_BASE		32
```

## E150-045 · uslab_s

typedef_declaration；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理行11–11，所属定义/实例范围11–11；Blob 2dea1f8fc05c；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；struct uslab alias。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]
```c
	typedef struct uslab uslab_s;
```

## E150-046 · uslab_cache_s

typedef_declaration；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理行14–14，所属定义/实例范围14–14；Blob 2dea1f8fc05c；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；cache alias。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c
	typedef struct uslab_cache uslab_cache_s;
```

## E150-047 · uPage_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理行19–23，所属定义/实例范围19–24；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；complete uPage member definition。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage_s]
```c
	typedef struct uPage
	{
		List_s	upage_list;
		void	*vaddr;
		uslab_s *uslab_p;
```

## E150-048 · uPage_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理行24–24，所属定义/实例范围19–24；Blob 2dea1f8fc05c；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；complete uPage member definition。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage_s]
```c
	} uPage_s;
```

## E150-049 · uslab_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理行26–30，所属定义/实例范围26–37；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；complete slab member definition。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]
```c
	typedef struct uslab
	{
		List_s			uslab_list;
		uslab_cache_s	*uslabcache_p;

```

## E150-050 · uslab_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理行31–35，所属定义/实例范围26–37；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；complete slab member definition。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]
```c
		unsigned long	total;
		unsigned long	free;

		uPage_s			*upage_p;
		void			*virt_addr;
```

## E150-051 · uslab_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理行36–37，所属定义/实例范围26–37；Blob 2dea1f8fc05c；source_line_endings=LF,LF；render_separator=NONE；覆盖=LAYOUT；complete slab member definition。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_s]
```c
		bitmap_t		*colormap;
	} uslab_s;
```

## E150-052 · uslab_cache_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理行39–43，所属定义/实例范围39–54；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；complete cache member definition。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c
	typedef struct uslab_cache
	{
		List_s			uslabcache_list;

		unsigned long	obj_size;
```

## E150-053 · uslab_cache_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理行44–48，所属定义/实例范围39–54；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；complete cache member definition。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c

		unsigned long	normal_slab_total;
		List_hdr_s		normal_slab_free;
		List_hdr_s		normal_slab_used;
		List_hdr_s		normal_slab_full;
```

## E150-054 · uslab_cache_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理行49–53，所属定义/实例范围39–54；Blob 2dea1f8fc05c；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；complete cache member definition。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c
		unsigned long	nsobj_free_count;
		unsigned long	nsobj_used_count;

		// the base slab should not be freed
		uslab_s			*normal_base_slab_p;
```

## E150-055 · uslab_cache_s

structure_definition；myinitramfs/mylib/myglibc/malloc/_malloc.h；物理行54–54，所属定义/实例范围39–54；Blob 2dea1f8fc05c；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；complete cache member definition。

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uslab_cache_s]
```c
	} uslab_cache_s;
```

## E150-056 · container_of

macro_definition；myinitramfs/mylib/include/utils.h；物理行12–16，所属定义/实例范围12–17；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；active macro subtracts member offset; no new object established。

[VERIFIED myinitramfs/mylib/include/utils.h::container_of]
```c
	#define container_of(member_ptr, container_type, member_name)		\
	({																	\
		typeof(((container_type *)0)->member_name) * p = (member_ptr);	\
		(container_type *)((unsigned long)p - 							\
		(unsigned long)&(((container_type *)0)->member_name));			\
```

## E150-057 · container_of

macro_definition；myinitramfs/mylib/include/utils.h；物理行17–17，所属定义/实例范围12–17；Blob 4e3e8910ac00；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；active macro subtracts member offset; no new object established。

[VERIFIED myinitramfs/mylib/include/utils.h::container_of]
```c
	})
```

## E150-058 · bitmap_t

typedef_definition；myinitramfs/mylib/include/utils.h；物理行19–19，所属定义/实例范围19–19；Blob 4e3e8910ac00；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；bitmap_t __UINT64_TYPE__ definition。

[VERIFIED myinitramfs/mylib/include/utils.h::bitmap_t]
```c
	typedef __UINT64_TYPE__ bitmap_t;
```

## E150-059 · BITMAP_UNITSIZE

macro_definition；myinitramfs/mylib/include/utils.h；物理行20–20，所属定义/实例范围20–20；Blob 4e3e8910ac00；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；sizeof(bitmap_t)*8。

[VERIFIED myinitramfs/mylib/include/utils.h::BITMAP_UNITSIZE]
```c
	#define BITMAP_UNITSIZE (sizeof(bitmap_t) * 8)
```

## E150-060 · List_s

typedef_declaration；myinitramfs/mylib/include/utils.h；物理行34–34，所属定义/实例范围34–34；Blob 4e3e8910ac00；source_line_endings=LF；render_separator=NONE；覆盖=LAYOUT；forward alias。

[VERIFIED myinitramfs/mylib/include/utils.h::List_s]
```c
	typedef struct List List_s;
```

## E150-061 · List_s

structure_definition；myinitramfs/mylib/include/utils.h；物理行35–39，所属定义/实例范围35–39；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；prev/next。

[VERIFIED myinitramfs/mylib/include/utils.h::List_s]
```c
	typedef struct List
	{
		List_s *	prev;
		List_s *	next;
	} List_s;
```

## E150-062 · List_hdr_s

structure_definition；myinitramfs/mylib/include/utils.h；物理行41–45，所属定义/实例范围41–45；Blob 4e3e8910ac00；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LAYOUT；anchor and count。

[VERIFIED myinitramfs/mylib/include/utils.h::List_hdr_s]
```c
	typedef struct List_hdr
	{
		List_s			anchor;
		unsigned long	count;
	} List_hdr_s;
```

## E150-063 · upage_alloc

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行111–115，所属定义/实例范围111–141；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=UPAGE_ALLOC；full upage_alloc branch and ordering, windows collectively。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]
```c
uPage_s *upage_alloc()
{
	uPage_s *ret_val = NULL;
	if (free_upage_lhdr.count > 0)
	{
```

## E150-064 · upage_alloc

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行116–120，所属定义/实例范围111–141；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=UPAGE_ALLOC；full upage_alloc branch and ordering, windows collectively。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]
```c
		List_s * upg_lp = list_header_remove_head(&free_upage_lhdr);
		while (upg_lp == 0);
		
		ret_val = container_of(upg_lp, uPage_s, upage_list);
		list_header_add_to_head(&used_upage_lhdr, &ret_val->upage_list);
```

## E150-065 · upage_alloc

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行121–125，所属定义/实例范围111–141；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=UPAGE_ALLOC；full upage_alloc branch and ordering, windows collectively。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]
```c
	}
	else
	{
		uPage_s *upgp = malloc(sizeof(uPage_s));
		void *pg_vaddr = brk(brk_end + PAGE_SIZE);
```

## E150-066 · upage_alloc

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行126–130，所属定义/实例范围111–141；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=UPAGE_ALLOC；full upage_alloc branch and ordering, windows collectively。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]
```c
		if (pg_vaddr != NULL)
		{
			brk_end += PAGE_SIZE;
			upgp->vaddr = pg_vaddr;
			INIT_LIST_S(&upgp->upage_list);
```

## E150-067 · upage_alloc

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行131–135，所属定义/实例范围111–141；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=UPAGE_ALLOC；full upage_alloc branch and ordering, windows collectively。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]
```c
			list_header_add_to_tail(&used_upage_lhdr, &upgp->upage_list);
			ret_val = upgp;
		}
		else
		{
```

## E150-068 · upage_alloc

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行136–140，所属定义/实例范围111–141；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=UPAGE_ALLOC；full upage_alloc branch and ordering, windows collectively。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]
```c
			free(upgp);
			ret_val = NULL;
		}
	}
	return ret_val;
```

## E150-069 · upage_alloc

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行141–141，所属定义/实例范围111–141；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=UPAGE_ALLOC；full upage_alloc branch and ordering, windows collectively。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_alloc]
```c
}
```

## E150-070 · upage_free

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行143–147，所属定义/实例范围143–149；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=UPAGE_FREE；full upage_free direct statements。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_free]
```c
void upage_free(uPage_s * upgp)
{
	list_del_init(&upgp->upage_list);
	used_upage_lhdr.count--;
	list_header_add_to_head(&free_upage_lhdr, &upgp->upage_list);
```

## E150-071 · upage_free

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行148–149，所属定义/实例范围143–149；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF；render_separator=NONE；覆盖=UPAGE_FREE；full upage_free direct statements。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_free]
```c
	upgp->uslab_p = NULL;
}
```

## E150-072 · upage_search

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行151–155，所属定义/实例范围151–169；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=UPAGE_SEARCH；full search rounding/traversal/comparison/return。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]
```c
uPage_s * upage_search(void * vaddr)
{
	uPage_s * ret_val = NULL;
	vaddr = (void *)PAGE_ROUND_DOWN((size_t)vaddr);
	List_s * upg_lp;
```

## E150-073 · upage_search

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行156–160，所属定义/实例范围151–169；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=UPAGE_SEARCH；full search rounding/traversal/comparison/return。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]
```c
	for (upg_lp = used_upage_lhdr.anchor.next;
		 upg_lp != &used_upage_lhdr.anchor;
		 upg_lp = upg_lp->next)
	{
		uPage_s * upgp = container_of(upg_lp, uPage_s, upage_list);
```

## E150-074 · upage_search

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行161–165，所属定义/实例范围151–169；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=UPAGE_SEARCH；full search rounding/traversal/comparison/return。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]
```c
		if (upgp->vaddr == vaddr)
		{
			ret_val = upgp;
			break;
		}
```

## E150-075 · upage_search

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行166–169，所属定义/实例范围151–169；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=UPAGE_SEARCH；full search rounding/traversal/comparison/return。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::upage_search]
```c
	}

	return ret_val;
}
```

## E150-076 · init_uslab

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行194–195，所属定义/实例范围174–221；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF；render_separator=NONE；覆盖=PRIOR_BOUNDARY；class size and capacity source (not re-running init)。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
		scgp->obj_size = USLAB_SIZE_BASE << i;
		unsigned long obj_nr = PAGE_SIZE / scgp->obj_size;
```

## E150-077 · init_uslab

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行220–220，所属定义/实例范围174–221；Blob 44db9ea9d7f7；source_line_endings=CRLF；render_separator=NONE；覆盖=PRIOR_BOUNDARY；sets initiated true。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::init_uslab]
```c
	initiated = true;
```

## E150-078 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行251–255，所属定义/实例范围249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=PRIOR_BOUNDARY；init condition and size bound, not expand calls。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
	if (!initiated)
		init_uslab();

	void * ret_val = NULL;
	if (size > CONST_1M)
```

## E150-079 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行262–264，所属定义/实例范围249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF；render_separator=NONE；覆盖=PRIOR_BOUNDARY；cache selection source。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
		uslab_cache_s *	uscgp = &uslab_cache_groups[0];
		while (size > uscgp->obj_size)
			uscgp = container_of((uscgp->uslabcache_list.next), uslab_cache_s, uslabcache_list);
```

## E150-080 · malloc

function_body；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行318–321，所属定义/实例范围249–328；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=PRIOR_BOUNDARY；caller publication separate, not executed in C。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::malloc]
```c
			new_uslp->uslabcache_p = uscgp;
			list_header_add_to_head(&uscgp->normal_slab_free, &new_uslp->uslab_list);
			uscgp->normal_slab_total++;
			uscgp->nsobj_free_count += new_uslp->total;
```

## E150-081 · uslab_alloc

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行223–227，所属定义/实例范围223–236；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=USLAB_ALLOC；complete direct callee sequence and binding。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_alloc]
```c
uslab_s * uslab_alloc(uslab_s * cslp)
{
	uslab_s * nslp = (uslab_s *)malloc(sizeof(uslab_s));
	INIT_LIST_S(&nslp->uslab_list);

```

## E150-082 · uslab_alloc

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行228–232，所属定义/实例范围223–236；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=USLAB_ALLOC；complete direct callee sequence and binding。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_alloc]
```c
	nslp->colormap = (bitmap_t *)malloc(cslp->total / sizeof(bitmap_t) + sizeof(bitmap_t));
	nslp->total =
	nslp->free = cslp->total;
	nslp->upage_p = upage_alloc();
	nslp->virt_addr = nslp->upage_p->vaddr;
```

## E150-083 · uslab_alloc

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行233–236，所属定义/实例范围223–236；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=USLAB_ALLOC；complete direct callee sequence and binding。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::uslab_alloc]
```c
	nslp->upage_p->uslab_p = nslp;

	return nslp;
}
```

## E150-084 · free

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行330–334，所属定义/实例范围330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；complete free: DEBUG/NULL, search, owner, bitmap, count and list/reclaim gates。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
void free(void * obj_p)
{
	#ifdef DEBUG
		// make sure have init slab
		while (!initiated);
```

## E150-085 · free

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行335–339，所属定义/实例范围330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；complete free: DEBUG/NULL, search, owner, bitmap, count and list/reclaim gates。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	#endif

	if (obj_p == NULL)
		return;

```

## E150-086 · free

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行340–344，所属定义/实例范围330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；complete free: DEBUG/NULL, search, owner, bitmap, count and list/reclaim gates。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	// find which slab does the pointer belonged to
	uPage_s * upgp = upage_search(obj_p);
	uslab_s * uslp = upgp->uslab_p;
	uslab_cache_s * uscgp = uslp->uslabcache_p;
	// of coures it should not in an empty-slab
```

## E150-087 · free

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行345–349，所属定义/实例范围330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；complete free: DEBUG/NULL, search, owner, bitmap, count and list/reclaim gates。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	if (uslp->free == uslp->total)
	{
		// color_printk(WHITE, RED, "Free obj in a free %d-byte slab!\n", scgp->obj_size);
		while (1);
	}
```

## E150-088 · free

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行350–354，所属定义/实例范围330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；complete free: DEBUG/NULL, search, owner, bitmap, count and list/reclaim gates。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c

	unsigned long obj_idx = (obj_p - uslp->virt_addr) / uscgp->obj_size;
	if (!bm_get_assigned_bit(uslp->colormap, obj_idx))
	{
		// color_printk(WHITE, RED, "The obj already been freed : %#018lx\n!", obj_p);
```

## E150-089 · free

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行355–359，所属定义/实例范围330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；complete free: DEBUG/NULL, search, owner, bitmap, count and list/reclaim gates。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
		while (1);
	}

	bm_clear_bit(uslp->colormap, obj_idx);
	uslp->free++;
```

## E150-090 · free

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行360–364，所属定义/实例范围330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；complete free: DEBUG/NULL, search, owner, bitmap, count and list/reclaim gates。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	uscgp->nsobj_free_count++;
	uscgp->nsobj_used_count--;
	// if the corresponding slab in full list, move it to used list
	// or if it is in used list and only use one slot, move it to free list
	if (uslp->free == 1)
```

## E150-091 · free

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行365–369，所属定义/实例范围330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；complete free: DEBUG/NULL, search, owner, bitmap, count and list/reclaim gates。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	{
		list_del_init(&uslp->uslab_list);
		uscgp->normal_slab_full.count--;
		list_header_add_to_head(&uscgp->normal_slab_used, &uslp->uslab_list);
	}
```

## E150-092 · free

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行370–374，所属定义/实例范围330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；complete free: DEBUG/NULL, search, owner, bitmap, count and list/reclaim gates。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	else if (uslp->free == 0)
	{
		list_del_init(&uslp->uslab_list);
		uscgp->normal_slab_used.count--;
		// if slp is base-slab, add it to tail
```

## E150-093 · free

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行375–379，所属定义/实例范围330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；complete free: DEBUG/NULL, search, owner, bitmap, count and list/reclaim gates。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
		if (uslp != uscgp->normal_base_slab_p)
			list_header_add_to_head(&uscgp->normal_slab_free, &uslp->uslab_list);
		else
			list_header_add_to_tail(&uscgp->normal_slab_free, &uslp->uslab_list);
	}
```

## E150-094 · free

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行380–384，所属定义/实例范围330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；complete free: DEBUG/NULL, search, owner, bitmap, count and list/reclaim gates。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	// if there is too many free slab, free some of them
	if (uscgp->normal_slab_free.count > 2)
	{
		uslab_s * tmp_uslp = container_of(uscgp->normal_slab_free.anchor.prev, uslab_s, uslab_list);
		uscgp->normal_slab_free.count--;
```

## E150-095 · free

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行385–389，所属定义/实例范围330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,CRLF,CRLF,CRLF,CRLF；render_separator=NONE；覆盖=FREE；complete free: DEBUG/NULL, search, owner, bitmap, count and list/reclaim gates。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
		uslab_free(tmp_uslp);
		uscgp->normal_slab_total;
		uscgp->nsobj_free_count -= tmp_uslp->total;
	}

```

## E150-096 · free

function_definition；myinitramfs/mylib/myglibc/malloc/malloc.c；物理行390–391，所属定义/实例范围330–391；Blob 44db9ea9d7f7；source_line_endings=CRLF,NONE；render_separator=LF；覆盖=FREE；complete free: DEBUG/NULL, search, owner, bitmap, count and list/reclaim gates。

[VERIFIED myinitramfs/mylib/myglibc/malloc/malloc.c::free]
```c
	// unlock_recurs_lock(&slab_alloc_lock);
}
```

## E150-097 · INIT_LIST_S

function_definition；myinitramfs/mylib/util/list.c；物理行10–14，所属定义/实例范围10–14；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；self-loop writes。

[VERIFIED myinitramfs/mylib/util/list.c::INIT_LIST_S]
```c
void INIT_LIST_S(List_s * src)
{
	src->prev = src;
	src->next = src;
}
```

## E150-098 · list_insert_prev

function_definition；myinitramfs/mylib/util/list.c；物理行17–21，所属定义/实例范围17–26；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；tail helper (read boundary, A success insert not reached)。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_prev]
```c
void list_insert_prev(List_s * dst, List_s * src)
{
	list_del_init(src);

	src->next = dst;
```

## E150-099 · list_insert_prev

function_definition；myinitramfs/mylib/util/list.c；物理行22–26，所属定义/实例范围17–26；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；tail helper (read boundary, A success insert not reached)。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_prev]
```c
	src->prev = dst->prev;

	src->next->prev = src;
	src->prev->next = src;
}
```

## E150-100 · list_insert_next

function_definition；myinitramfs/mylib/util/list.c；物理行29–33，所属定义/实例范围29–38；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；self-delete then insert four pointer stores。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_next]
```c
void list_insert_next(List_s * dst, List_s * src)
{
	list_del_init(src);

	src->next = dst->next;
```

## E150-101 · list_insert_next

function_definition；myinitramfs/mylib/util/list.c；物理行34–38，所属定义/实例范围29–38；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；self-delete then insert four pointer stores。

[VERIFIED myinitramfs/mylib/util/list.c::list_insert_next]
```c
	src->prev = dst;

	src->next->prev = src;
	src->prev->next = src;
}
```

## E150-102 · list_del_init

function_definition；myinitramfs/mylib/util/list.c；物理行40–44，所属定义/实例范围40–45；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；neighbor splice plus self-init。

[VERIFIED myinitramfs/mylib/util/list.c::list_del_init]
```c
void list_del_init(List_s *src)
{
	src->prev->next = src->next;
	src->next->prev = src->prev;
	INIT_LIST_S(src);
```

## E150-103 · list_del_init

function_definition；myinitramfs/mylib/util/list.c；物理行45–45，所属定义/实例范围40–45；Blob 24f7a214a288；source_line_endings=LF；render_separator=NONE；覆盖=LIST；neighbor splice plus self-init。

[VERIFIED myinitramfs/mylib/util/list.c::list_del_init]
```c
}
```

## E150-104 · list_header_add_to_head

function_definition；myinitramfs/mylib/util/list.c；物理行79–83，所属定义/实例范围79–83；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；insert then count++。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_add_to_head]
```c
void list_header_add_to_head(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_next(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}
```

## E150-105 · list_header_remove_head

function_definition；myinitramfs/mylib/util/list.c；物理行85–89，所属定义/实例范围85–96；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；count gate, anchor.next, deletion and count--, return。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_remove_head]
```c
List_s * list_header_remove_head(List_hdr_s * lhdr_p)
{
	if (lhdr_p->count > 0)
	{
		List_s * ret_val = lhdr_p->anchor.next;
```

## E150-106 · list_header_remove_head

function_definition；myinitramfs/mylib/util/list.c；物理行90–94，所属定义/实例范围85–96；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；count gate, anchor.next, deletion and count--, return。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_remove_head]
```c
		list_del_init(ret_val);
		lhdr_p->count--;
		return ret_val;
	}

```

## E150-107 · list_header_remove_head

function_definition；myinitramfs/mylib/util/list.c；物理行95–96，所属定义/实例范围85–96；Blob 24f7a214a288；source_line_endings=LF,LF；render_separator=NONE；覆盖=LIST；count gate, anchor.next, deletion and count--, return。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_remove_head]
```c
	return NULL;
}
```

## E150-108 · list_header_add_to_tail

function_definition；myinitramfs/mylib/util/list.c；物理行98–102，所属定义/实例范围98–102；Blob 24f7a214a288；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=LIST；tail insert/count++; not executed in A。

[VERIFIED myinitramfs/mylib/util/list.c::list_header_add_to_tail]
```c
void list_header_add_to_tail(List_hdr_s * lhdr_p, List_s * l_p)
{
	list_insert_prev(&lhdr_p->anchor, l_p);
	lhdr_p->count++;
}
```

## E150-109 · bm_get_assigned_bit

function_definition；myinitramfs/mylib/util/bitmap.c；物理行4–8，所属定义/实例范围4–12；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=BITMAP；unsigned return, literal int 1, low bit mask/unit read。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_assigned_bit]
```c
unsigned bm_get_assigned_bit(bitmap_t * bm, unsigned long idx)
{
	unsigned ret_val = 0;
	size_t unit_idx = idx / (8 * sizeof(bitmap_t));
	bitmap_t mask = 1 << (idx % (8 * sizeof(bitmap_t)));
```

## E150-110 · bm_get_assigned_bit

function_definition；myinitramfs/mylib/util/bitmap.c；物理行9–12，所属定义/实例范围4–12；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF；render_separator=NONE；覆盖=BITMAP；unsigned return, literal int 1, low bit mask/unit read。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_get_assigned_bit]
```c
	bitmap_t unit = *(bm + unit_idx);
	ret_val = unit & mask;
	return ret_val;
}
```

## E150-111 · bm_clear_bit

function_definition；myinitramfs/mylib/util/bitmap.c；物理行19–22，所属定义/实例范围19–22；Blob 6a86523c41ba；source_line_endings=LF,LF,LF,LF；render_separator=NONE；覆盖=BITMAP；1ULL, complement and word RMW。

[VERIFIED myinitramfs/mylib/util/bitmap.c::bm_clear_bit]
```c
void bm_clear_bit(bitmap_t * bm, unsigned long idx)
{
	*(bm + idx / BITMAP_UNITSIZE) &= ~(1ULL << (idx % BITMAP_UNITSIZE));
}
```

## E150-112 · if

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行13–13，所属定义/实例范围13–13；实例=if (CMAKE_BUILD_TYPE STREQUAL "Debug")；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；user build type textual condition or assignment。

[VERIFIED myinitramfs/CMakeLists.txt::if]
```cmake
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
```

## E150-113 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行14–14，所属定义/实例范围14–14；实例=set(RELEASE_FLAG "-DDEBUG")；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；user build type textual condition or assignment。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
	set(RELEASE_FLAG "-DDEBUG")
```

## E150-114 · elseif

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行16–16，所属定义/实例范围16–16；实例=elseif ()；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；user build type textual condition or assignment。

[VERIFIED myinitramfs/CMakeLists.txt::elseif]
```cmake
elseif ()
```

## E150-115 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行18–18，所属定义/实例范围18–18；实例=set(RELEASE_FLAG "-DRELEASE")；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；user build type textual condition or assignment。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
	set(RELEASE_FLAG "-DRELEASE")
```

## E150-116 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行19–19，所属定义/实例范围19–19；实例=set(UNUSED_MACRO "-Wunused-macros")；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；user build type textual condition or assignment。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
	set(UNUSED_MACRO "-Wunused-macros")
```

## E150-117 · endif

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行20–20，所属定义/实例范围20–20；实例=endif()；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；user build type textual condition or assignment。

[VERIFIED myinitramfs/CMakeLists.txt::endif]
```cmake
endif()
```

## E150-118 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行22–24，所属定义/实例范围22–24；实例=set(CMAKE_ASM_FLAGS …)；Blob 6757d21bc64c；source_line_endings=LF,LF,LF；render_separator=NONE；覆盖=BUILD；one user ASM flags set invocation。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${RELEASE_FLAG} \
	-m64 \
")
```

## E150-119 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行26–29，所属定义/实例范围26–29；实例=set(CMAKE_C_FLAGS …)；Blob 6757d21bc64c；source_line_endings=LF,LF,LF,LF；render_separator=NONE；覆盖=BUILD；one user C flags set invocation。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 \
	-ggdb \
")
```

## E150-120 · if

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行31–31，所属定义/实例范围31–31；实例=if (${LINK_TYPE} STREQUAL "static")；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；LINK_TYPE static gate text。

[VERIFIED myinitramfs/CMakeLists.txt::if]
```cmake
if (${LINK_TYPE} STREQUAL "static")
```

## E150-121 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行32–32，所属定义/实例范围32–32；实例=set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；static flag assignment in gate。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
```

## E150-122 · endif

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行33–33，所属定义/实例范围33–33；实例=endif()；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；static gate ending。

[VERIFIED myinitramfs/CMakeLists.txt::endif]
```cmake
endif()
```

## E150-123 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行40–40，所属定义/实例范围40–40；实例=set(MUSL_PATH /home/cheyh/projs/musl/build)；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；musl configuration path variable; external content not read。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(MUSL_PATH /home/cheyh/projs/musl/build)
```

## E150-124 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行41–41，所属定义/实例范围41–41；实例=set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；musl configuration path variable; external content not read。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)
```

## E150-125 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行43–43，所属定义/实例范围43–43；实例=set(MUSL_INC_PATH ${MUSL_PATH}/include)；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；musl configuration path variable; external content not read。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(MUSL_INC_PATH ${MUSL_PATH}/include)
```

## E150-126 · set

cmake_command_invocation；myinitramfs/CMakeLists.txt；物理行44–44，所属定义/实例范围44–44；实例=set(MUSL_LIB_PATH ${MUSL_PATH}/lib)；Blob 6757d21bc64c；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；musl configuration path variable; external content not read。

[VERIFIED myinitramfs/CMakeLists.txt::set]
```cmake
set(MUSL_LIB_PATH ${MUSL_PATH}/lib)
```

## E150-127 · add_library

cmake_command_invocation；myinitramfs/mylib/myglibc/CMakeLists.txt；物理行1–1，所属定义/实例范围1–1；实例=add_library(mylibc STATIC)；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；mylibc STATIC target invocation。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_library]
```cmake
add_library(mylibc STATIC)
```

## E150-128 · set

cmake_command_invocation；myinitramfs/mylib/myglibc/CMakeLists.txt；物理行2–2，所属定义/实例范围2–2；实例=set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH}")；Blob b956957a602a；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；candidate library flags set invocation。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::set]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH}")
```

## E150-129 · target_include_directories

cmake_command_invocation；myinitramfs/mylib/myglibc/CMakeLists.txt；物理行3–7，所属定义/实例范围3–16；实例=target_include_directories(mylibc PUBLIC …)；Blob b956957a602a；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=BUILD；one multiline include command, exact invocation scope。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories]
```cmake
target_include_directories(mylibc PUBLIC
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/include
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sys
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/string
```

## E150-130 · target_include_directories

cmake_command_invocation；myinitramfs/mylib/myglibc/CMakeLists.txt；物理行8–12，所属定义/实例范围3–16；实例=target_include_directories(mylibc PUBLIC …)；Blob b956957a602a；source_line_endings=LF,LF,LF,LF,LF；render_separator=NONE；覆盖=BUILD；one multiline include command, exact invocation scope。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories]
```cmake
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/misc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/x86
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/generic
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/gnu
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux
```

## E150-131 · target_include_directories

cmake_command_invocation；myinitramfs/mylib/myglibc/CMakeLists.txt；物理行13–16，所属定义/实例范围3–16；实例=target_include_directories(mylibc PUBLIC …)；Blob b956957a602a；source_line_endings=LF,LF,LF,LF；render_separator=NONE；覆盖=BUILD；one multiline include command, exact invocation scope。

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories]
```cmake
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux/x86
                    ${PROJECT_SOURCE_DIR}/../mykernel/include
                    ${PROJECT_SOURCE_DIR}/../mykernel/arch/${ARCH}/include
                    )
```

## E150-132 · file

cmake_command_invocation；myinitramfs/mylib/myglibc/malloc/CMakeLists.txt；物理行1–1，所属定义/实例范围1–1；实例=file(GLOB LIBC_STDLIB_C_SRCS *.c)；Blob 3b9f22dd9ab9；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；GLOB C source pattern in this subdirectory。

[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::file]
```cmake
file(GLOB LIBC_STDLIB_C_SRCS *.c)
```

## E150-133 · file

cmake_command_invocation；myinitramfs/mylib/myglibc/malloc/CMakeLists.txt；物理行2–2，所属定义/实例范围2–2；实例=file(GLOB LIBC_STDLIB_ASM_SRCS *.S)；Blob 3b9f22dd9ab9；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；GLOB ASM source pattern in this subdirectory。

[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::file]
```cmake
file(GLOB LIBC_STDLIB_ASM_SRCS *.S)
```

## E150-134 · target_sources

cmake_command_invocation；myinitramfs/mylib/myglibc/malloc/CMakeLists.txt；物理行3–3，所属定义/实例范围3–3；实例=target_sources(mylibc PUBLIC ${LIBC_STDLIB_C_SRCS} ${LIBC_STDLIB_ASM_SRCS})；Blob 3b9f22dd9ab9；source_line_endings=NONE；render_separator=LF；覆盖=BUILD；target_sources(mylibc PUBLIC ...) consumes both variables。

[VERIFIED myinitramfs/mylib/myglibc/malloc/CMakeLists.txt::target_sources]
```cmake
target_sources(mylibc PUBLIC ${LIBC_STDLIB_C_SRCS} ${LIBC_STDLIB_ASM_SRCS})
```

## E150-135 · file

cmake_command_invocation；myinitramfs/mylib/util/CMakeLists.txt；物理行1–1，所属定义/实例范围1–1；实例=file(GLOB UTIL_C_SRCS *.c)；Blob 5fd782a35629；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；GLOB C source pattern in this subdirectory。

[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::file]
```cmake
file(GLOB UTIL_C_SRCS *.c)
```

## E150-136 · file

cmake_command_invocation；myinitramfs/mylib/util/CMakeLists.txt；物理行2–2，所属定义/实例范围2–2；实例=file(GLOB UTIL_ASM_SRCS *.S)；Blob 5fd782a35629；source_line_endings=LF；render_separator=NONE；覆盖=BUILD；GLOB ASM source pattern in this subdirectory。

[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::file]
```cmake
file(GLOB UTIL_ASM_SRCS *.S)
```

## E150-137 · target_sources

cmake_command_invocation；myinitramfs/mylib/util/CMakeLists.txt；物理行3–3，所属定义/实例范围3–3；实例=target_sources(mylibc PUBLIC ${UTIL_C_SRCS} ${UTIL_ASM_SRCS})；Blob 5fd782a35629；source_line_endings=NONE；render_separator=LF；覆盖=BUILD；target_sources(mylibc PUBLIC ...) consumes both variables。

[VERIFIED myinitramfs/mylib/util/CMakeLists.txt::target_sources]
```cmake
target_sources(mylibc PUBLIC ${UTIL_C_SRCS} ${UTIL_ASM_SRCS})
```
