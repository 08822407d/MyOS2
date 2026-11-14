---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-BOOT-BUILD-CONTRACT-85
record_type: original_fixed_time_short_anchors
produced_by: GPT-6 (model identity in this chat, backend not independently attestable)
model_per_owner: GPT-6 + Extra
read_channel: connector
date: 2026-11-13
base_snapshot: time
source_pin_short12: a039d9803ade
status: static_draft_for_independent_mainline_review
runtime_evidence: NOT_RUN
open_questions: [BB85-U01, BB85-U02, BB85-U03, BB85-U04]
---

# 第85批：固定time原文证据索引

共119条新短锚（前32条及E119属D00补正和支持链，33–115是85新A/B/C，116–118是单语句来源补锚）。全部1–5个物理源码原行，前缀仅在本文件出现；标注路径及单一定义/宏/CMake命令/脚本唯一语句/链接指令的所在真实范围。CMake、Python和链接脚本是**只读文本**；在无C函数的文件里标签右侧用实际命令、语句或唯一标识符，而不是虚构C函数。条件块上下文只作为选择约束，不把其它宏或外层条件塞入某单个宏的锚。源码真实存在、宏展开结果、真实构建和运行结果分层。由于第84批R84-01退回，原84所谓全部49条在定义内的总括已撤回，原49/49/0仍为冻结历史作者计数，不能挪给新85当验收结论。

### BB85-E001 | D00 E06: real phys_base single-line storage definition

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::phys_base]

```asm
SYM_DATA(phys_base, .quad 0x0)
```

范围：固定time单个宏定义/数据定义/链接器顶层独立语句的连续原文；行437–437只辅助定位，不认证编译运行。

### BB85-E002 | D00 E07: initial_code only its single SYM_DATA definition

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::initial_code]

```asm
SYM_DATA(initial_code,	.quad x86_64_start_kernel)
```

范围：固定time单个宏定义/数据定义/链接器顶层独立语句的连续原文；行287–287只辅助定位，不认证编译运行。

### BB85-E003 | D00 E09: alias macro independently from preceding extern

[VERIFIED mykernel/mm/fault/memfault_api.h::swapper_pg_dir]

```c
	#define swapper_pg_dir init_top_pgt
```

范围：固定time单个宏定义/数据定义/链接器顶层独立语句的连续原文；行18–18只辅助定位，不认证编译运行。

### BB85-E004 | D00 E12: 1GiB macro in its selected conditional branch

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::KERNEL_IMAGE_SIZE]

```c
	#  define KERNEL_IMAGE_SIZE	(1024 * 1024 * 1024)
```

范围：固定time单个宏定义/数据定义/链接器顶层独立语句的连续原文；行377–377只辅助定位，不认证编译运行。

### BB85-E005 | D00 E12: 512MiB macro in alternative branch

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::KERNEL_IMAGE_SIZE]

```c
	#  define KERNEL_IMAGE_SIZE	(512 * 1024 * 1024)
```

范围：固定time单个宏定义/数据定义/链接器顶层独立语句的连续原文；行379–379只辅助定位，不认证编译运行。

### BB85-E006 | D00 E13: PMD shift macro alone

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PMD_SHIFT]

```c
	#define PMD_SHIFT			21
```

范围：固定time单个宏定义/数据定义/链接器顶层独立语句的连续原文；行260–260只辅助定位，不认证编译运行。

### BB85-E007 | D00 E13: table slot count macro alone

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PTRS_PER_PMD]

```c
	#define PTRS_PER_PMD		512
```

范围：固定time单个宏定义/数据定义/链接器顶层独立语句的连续原文；行261–261只辅助定位，不认证编译运行。

### BB85-E008 | D00 E18: unique top-level ASSERT after SECTIONS close

[VERIFIED mykernel/arch/x86_64/kernel.lds::ASSERT]

```ld
. = ASSERT((_end - 0xffffffff80000000 <= (1024 * 1024 * 1024)),
    "kernel image bigger than KERNEL_IMAGE_SIZE");
```

范围：固定time单个宏定义/数据定义/链接器顶层独立语句的连续原文；行186–187只辅助定位，不认证编译运行。

### BB85-E009 | D00 E20: page shift macro alone

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT]

```c
	#define PAGE_SHIFT				12
```

范围：固定time单个宏定义/数据定义/链接器顶层独立语句的连续原文；行9–9只辅助定位，不认证编译运行。

### BB85-E010 | D00 E20: page size macro alone

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]

```c
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```

范围：固定time单个宏定义/数据定义/链接器顶层独立语句的连续原文；行10–10只辅助定位，不认证编译运行。

### BB85-E011 | D00 E22: pgd_t typedef distinct from pgdval_t

[VERIFIED mykernel/arch/x86_64/mm/mm_type_declaration_arch.h::pgd_t]

```c
	typedef p4d_t pgd_t;
```

范围：独立真实定义/调用者内原文（行42–42）；不把相邻源码并入宏或数据定义。

### BB85-E012 | D00 E22: separate PGD value typedef

[VERIFIED mykernel/arch/x86_64/mm/mm_type_declaration_arch.h::pgdval_t]

```c
	typedef p4dval_t pgdval_t;
```

范围：独立真实定义/调用者内原文（行41–41）；不把相邻源码并入宏或数据定义。

### BB85-E013 | D00 E38: active value only, not commented history

[VERIFIED mykernel/arch/x86_64/mm/page/mmzone_const_arch.h::MAX_PHYSMEM_BITS]

```c
	#define MAX_PHYSMEM_BITS 46
```

范围：独立真实定义/调用者内原文（行139–139）；不把相邻源码并入宏或数据定义。

### BB85-E014 | D00 E07: REF input section defined by actual macro

[VERIFIED mykernel/include/linux/init/init.h::__REFDATA]

```c
	#define __REFDATA		.section	".ref.data", "aw"
```

范围：独立真实定义/调用者内原文（行116–116）；不把相邻源码并入宏或数据定义。

### BB85-E015 | D00 supplemental: startup input section macro

[VERIFIED mykernel/include/linux/init/init.h::__STARTUP]

```c
	#define __STARTUP		.section	".head.text.startup","ax"
```

范围：独立真实定义/调用者内原文（行101–101）；不把相邻源码并入宏或数据定义。

### BB85-E016 | D00 supplemental: startup code symbol and entry instructions

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::startup_64]

```asm
SYM_CODE_START_NOALIGN(startup_64)
	cli
	jmp		.code_64_start
```

范围：独立真实定义/调用者内原文（行32–34）；不把相邻源码并入宏或数据定义。

### BB85-E017 | D00 supplemental: linker KEEP of head.text and startup input

[VERIFIED mykernel/arch/x86_64/kernel.lds::output_.text]

```ld
		KEEP(*(.head.text.startup))
		KEEP(*(.head.text))
		*(.text*)
```

范围：独立真实定义/调用者内原文（行32–34）；不把相邻源码并入宏或数据定义。

### BB85-E018 | D00 supplemental: actual premap caller

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]

```c
	myos_early_map_pgt_buf_into_directmap();
```

范围：独立真实定义/调用者内原文（行242–242）；不把相邻源码并入宏或数据定义。

### BB85-E019 | D00 supplemental: actual later init_mem_mapping caller

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]

```c
	init_mem_mapping();
```

范围：独立真实定义/调用者内原文（行249–249）；不把相邻源码并入宏或数据定义。

### BB85-E020 | D00 supplemental: real root value copy after flush

[VERIFIED mykernel/arch/x86_64/mm/init.c::myos_early_map_pgt_buf_into_directmap]

```c
	*init_pgd = *early_pgd;
```

范围：独立真实定义/调用者内原文（行223–223）；不把相邻源码并入宏或数据定义。

### BB85-E021 | D00 supplemental: real va_text and va_end assignments

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::__startup_64]

```c
	va_text = physaddr - p2v_offset;
	va_end  = (ulong)rip_rel_ptr(&_end) - p2v_offset;
```

范围：固定time定义体、命名宏或唯一链接语句内的连续2原文物理行；未执行构建。

### BB85-E022 | D00 supplemental: pmd_entry permissions then physaddr addition

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::__startup_64]

```c
	pmd_entry = __PAGE_KERNEL_LARGE_EXEC & ~_PAGE_GLOBAL;
	/* Filter out unsupported __PAGE_KERNEL_* bits: */
	pmd_entry +=  physaddr;
```

范围：固定time定义体、命名宏或唯一链接语句内的连续3原文物理行；未执行构建。

### BB85-E023 | D00 R84-S01: nonDEBUG nonPIC inline function only

[VERIFIED mykernel/arch/x86_64/include/asm/asm.h::rip_rel_ptr]

```c
			static __always_inline __pure void
			*rip_rel_ptr(void *p) {
				asm("leaq %c1(%%rip), %0" : "=r"(p) : "i"(p));
				return p;
			}
```

范围：固定time定义体、命名宏或唯一链接语句内的连续5原文物理行；未执行构建。

### BB85-E024 | D00 R84-S01: distinct DEBUG macro definition

[VERIFIED mykernel/arch/x86_64/include/asm/asm.h::rip_rel_ptr]

```c
	#      define rip_rel_ptr(p) ({									\
						typeof((p)) __p;							\
						asm("leaq %a1, %0" : "=r"(__p) : "i"((p)));	\
						__p;										\
					})
```

范围：固定time定义体、命名宏或唯一链接语句内的连续5原文物理行；未执行构建。

### BB85-E025 | D00 R84-S01: __pic__ alternative shown is RIP_REL_REF macro

[VERIFIED mykernel/arch/x86_64/include/asm/asm.h::RIP_REL_REF]

```c
	#    define RIP_REL_REF(var)	(var)
```

范围：固定time定义体、命名宏或唯一链接语句内的连续1原文物理行；未执行构建。

### BB85-E026 | D00 linker entry top-level directive

[VERIFIED mykernel/arch/x86_64/kernel.lds::ENTRY]

```ld
ENTRY(phys_startup_64)
```

范围：固定time定义体、命名宏或唯一链接语句内的连续1原文物理行；未执行构建。

### BB85-E027 | D00 link physical entry expression inside SECTIONS

[VERIFIED mykernel/arch/x86_64/kernel.lds::phys_startup_64]

```ld
	phys_startup_64 = ABSOLUTE(startup_64 - __START_KERNEL_map);
```

范围：固定time定义体、命名宏或唯一链接语句内的连续1原文物理行；未执行构建。

### BB85-E028 | D00 separate final CR3 and flush source

[VERIFIED mykernel/arch/x86_64/mm/init.c::init_mem_mapping]

```c
	load_cr3(swapper_pg_dir);
	__flush_tlb_all();
```

范围：固定time定义体、命名宏或唯一链接语句内的连续2原文物理行；未执行构建。

### BB85-E029 | D00 C root actual storage definition

[VERIFIED mykernel/mm/misc/init-mm.c::init_top_pgt]

```c
pgd_t	init_top_pgt[ENT_PER_TABLE]__aligned(PAGE_SIZE);
```

范围：固定time定义体、命名宏或唯一链接语句内的连续1原文物理行；未执行构建。

### BB85-E030 | D00 retained 512MiB/1GiB conditional PMDS storage

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::level2_kernel_pgt]

```asm
	PMDS(0, __PAGE_KERNEL_LARGE_EXEC, KERNEL_IMAGE_SIZE/PMD_SIZE)
SYM_DATA_END(level2_kernel_pgt)
```

范围：固定time定义体、命名宏或唯一链接语句内的连续2原文物理行；未执行构建。

### BB85-E031 | D00 upper bound derivation start offset under kernel.lds

[VERIFIED mykernel/arch/x86_64/kernel.lds::SECTIONS]

```ld
	. += (__START_KERNEL_map + ((DMA_END + (_Page2M_Size - 1)) & ~(_Page2M_Size - 1)));
	phys_startup_64 = ABSOLUTE(startup_64 - __START_KERNEL_map);
	.text ALIGN(_Page_Size) : AT(ADDR(.text) - __START_KERNEL_map)
```

范围：固定time定义体、命名宏或唯一链接语句内的连续3原文物理行；未执行构建。

### BB85-E032 | D00 startup RIP minus linked-address operand

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::startup_64]

```asm
	leaq	common_startup_64(%rip),	%rdi
	subq	.Lcommon_startup_64(%rip),	%rdi
```

范围：固定time定义体、命名宏或唯一链接语句内的连续2原文物理行；未执行构建。

### BB85-E033 | G01 project declares languages ASM C (not build result)

[VERIFIED mykernel/CMakeLists.txt::project]

```cmake
project(MyOS2_Kernel ASM C)
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E034 | G01 SCRIPTS_PATH rooted at PROJECT_SOURCE_DIR/scripts

[VERIFIED mykernel/CMakeLists.txt::set(SCRIPTS_PATH)]

```cmake
set(SCRIPTS_PATH ${PROJECT_SOURCE_DIR}/scripts)
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E035 | G01 options file included by root

[VERIFIED mykernel/CMakeLists.txt::include(options_flags)]

```cmake
include(${SCRIPTS_PATH}/options_flags.cmake)
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E036 | G01 kernel target declaration

[VERIFIED mykernel/CMakeLists.txt::add_executable(kernel)]

```cmake
add_executable(kernel)
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E037 | G01 offset target created before source selection

[VERIFIED mykernel/CMakeLists.txt::include(target_kbuild)]

```cmake
include(${SCRIPTS_PATH}/target_kbuild.cmake)
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E038 | G01 KBUILD pattern from selected ARCH

[VERIFIED mykernel/CMakeLists.txt::file(GLOB_RECURSE_KBUILD_FILES)]

```cmake
file(GLOB_RECURSE KBUILD_FILES ${PROJECT_SOURCE_DIR}/arch/${ARCH}/kbuild/*)
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E039 | G01 C recursive glob

[VERIFIED mykernel/CMakeLists.txt::file(GLOB_RECURSE_KERNEL_C_SRCS)]

```cmake
file(GLOB_RECURSE KERNEL_C_SRCS ${PROJECT_SOURCE_DIR}/*.c)
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E040 | G01 ASM .S recursive glob

[VERIFIED mykernel/CMakeLists.txt::file(GLOB_RECURSE_KERNEL_ASM_SRCS)]

```cmake
file(GLOB_RECURSE KERNEL_ASM_SRCS ${PROJECT_SOURCE_DIR}/*.S)
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E041 | G01 KBUILD removals from C collection

[VERIFIED mykernel/CMakeLists.txt::list(REMOVE_ITEM_KERNEL_C_SRCS)]

```cmake
list(REMOVE_ITEM KERNEL_C_SRCS ${KBUILD_FILES})
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E042 | G01 KBUILD removals from ASM collection

[VERIFIED mykernel/CMakeLists.txt::list(REMOVE_ITEM_KERNEL_ASM_SRCS)]

```cmake
list(REMOVE_ITEM KERNEL_ASM_SRCS ${KBUILD_FILES})
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E043 | G01 final kernel target include after source lists

[VERIFIED mykernel/CMakeLists.txt::include(target_kernel)]

```cmake
include(${SCRIPTS_PATH}/target_kernel.cmake)
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E044 | G01 actual consumer of two source lists

[VERIFIED mykernel/scripts/target_kernel.cmake::target_sources(kernel)]

```cmake
target_sources(kernel PRIVATE ${KERNEL_C_SRCS} ${KERNEL_ASM_SRCS})
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E045 | G01 target include roots and myloader include-only path

[VERIFIED mykernel/scripts/target_kernel.cmake::target_include_directories(kernel)]

```cmake
target_include_directories(kernel PRIVATE
	${INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/../myloader
)
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E046 | G01 arch folder name chosen from ARCH variable

[VERIFIED mykernel/arch/CMakeLists.txt::add_subdirectory(ARCH)]

```cmake
add_subdirectory(${ARCH})
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E047 | G01 arch C flags attempt to append CONFIG_X86_64

[VERIFIED mykernel/arch/x86_64/CMakeLists.txt::set(CMAKE_C_FLAGS)]

```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
	-DCONFIG_X86_64 -DCONFIG_ZONE_DMA \
")
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E048 | G01 link-like switch stored in C flags in arch subdirectory

[VERIFIED mykernel/arch/x86_64/CMakeLists.txt::set(CMAKE_C_FLAGS)]

```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
	-Wl,--no-check-sections
")
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E049 | G01 bounded nested kernel CMake entrance

[VERIFIED mykernel/arch/x86_64/CMakeLists.txt::add_subdirectory(kernel)]

```cmake
add_subdirectory(kernel)
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E050 | G01 target_kernel enters arch child after target_sources

[VERIFIED mykernel/scripts/target_kernel.cmake::add_subdirectory(arch)]

```cmake
add_subdirectory(arch)
```

范围：命名CMake顶层语句或单个多行命令定义；不证明CMake实际配置执行。

### BB85-E051 | G01 ARCH default check uses nested variable expansion

[VERIFIED mykernel/scripts/options_flags.cmake::if(NOT_DEFINED_ARCH)]

```cmake
if (NOT DEFINED ${ARCH})
	set(ARCH x86_64)
endif()
```

来源：所选固定time文件中的同一命令/条件分支体；仅证明表达式文本，不代表配置/外部进程实际执行。

### BB85-E052 | G01 Debug path sets -DDEBUG text

[VERIFIED mykernel/scripts/options_flags.cmake::if_BUILD_TYPE_Debug]

```cmake
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(RELEASE_FLAG "-DDEBUG")
	message("${PROMPT} DEBUG ${PROMPT_END}")
```

来源：所选固定time文件中的同一命令/条件分支体；仅证明表达式文本，不代表配置/外部进程实际执行。

### BB85-E053 | G01 alternate path stores RELEASE and UNUSED_MACRO variables

[VERIFIED mykernel/scripts/options_flags.cmake::if(CMAKE_BUILD_TYPE)]

```cmake
	message("${PROMPT} RELEASE  ${PROMPT_END}")
	set(RELEASE_FLAG "-DRELEASE")
	set(UNUSED_MACRO "-Wunused-macros")
```

来源：所选固定time文件中的同一命令/条件分支体；仅证明表达式文本，不代表配置/外部进程实际执行。

### BB85-E054 | G01 external gcc query is configured but never executed here

[VERIFIED mykernel/scripts/options_flags.cmake::execute_process]

```cmake
execute_process(COMMAND bash -c "gcc --print-file-name=include | tr -d '\n\r'"
				OUTPUT_VARIABLE CC_INC_PATH)
```

来源：所选固定time文件中的同一命令/条件分支体；仅证明表达式文本，不代表配置/外部进程实际执行。

### BB85-E055 | G01 common ASM model and noPIE/noPIC flags

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_ASM_FLAGS)]

```cmake
set(CMAKE_ASM_FLAGS
	"${CMAKE_ASM_FLAGS} ${RELEASE_FLAG} ${UNUSED_MACRO} \
	-m64 -mcmodel=kernel -fverbose-asm \
	-fno-pie -fno-pic \
```

来源：所选固定time文件中的同一命令/条件分支体；仅证明表达式文本，不代表配置/外部进程实际执行。

### BB85-E056 | G01 ASM-only assembly-preprocessor macro strings

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_ASM_FLAGS)]

```cmake
	-D__KERNEL__ \
	-D__ASSEMBLY__ -DASM_FILE \
")
```

来源：所选固定time文件中的同一命令/条件分支体；仅证明表达式文本，不代表配置/外部进程实际执行。

### BB85-E057 | G01 C compilation flags also contain nonPIE and model strings

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]

```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
```

来源：所选固定time文件中的同一命令/条件分支体；仅证明表达式文本，不代表配置/外部进程实际执行。

### BB85-E058 | G01 C freestanding/nostdlib/section flags

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]

```cmake
	-nostdinc -nostdlib \
	-fno-stack-protector \
	-fno-unwind-tables \
	-static \
	-fdata-sections -ffunction-sections \
```

来源：所选固定time文件中的同一命令/条件分支体；仅证明表达式文本，不代表配置/外部进程实际执行。

### BB85-E059 | G01 C-only CONFIG and __KERNEL__ defines

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]

```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

来源：所选固定time文件中的同一命令/条件分支体；仅证明表达式文本，不代表配置/外部进程实际执行。

### BB85-E060 | G01 later C-only config choices including KVM guest

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]

```cmake
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
	-DGRUB2_BOOTUP_SUPPORT \
	-DCONFIG_HYPERVISOR_GUEST \
	-DCONFIG_KVM_GUEST \
```

来源：所选固定time文件中的同一命令/条件分支体；仅证明表达式文本，不代表配置/外部进程实际执行。

### BB85-E061 | G01 global executable linker switch strings

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_EXE_LINKER_FLAGS)]

```cmake
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--build-id=none,--gc-sections")
```

来源：所选固定time文件中的同一命令/条件分支体；仅证明表达式文本，不代表配置/外部进程实际执行。

### BB85-E062 | G01 output-directory variable set to CMAKE_BINARY_DIR

[VERIFIED mykernel/scripts/options_flags.cmake::set(EXECUTABLE_OUTPUT_PATH)]

```cmake
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}) # Force generate binaries to ${CMAKE_BINARY_DIR}
```

来源：所选固定time文件中的同一命令/条件分支体；仅证明表达式文本，不代表配置/外部进程实际执行。

### BB85-E063 | G01 project-root and include source roots

[VERIFIED mykernel/scripts/options_flags.cmake::set(INCLUDE_DIRS)]

```cmake
set(INCLUDE_DIRS
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/include/linux/kernel
```

来源：所选固定time文件中的同一命令/条件分支体；仅证明表达式文本，不代表配置/外部进程实际执行。

### BB85-E064 | G01 ARCH-dependent include roots

[VERIFIED mykernel/scripts/options_flags.cmake::set(INCLUDE_DIRS)]

```cmake
	${PROJECT_SOURCE_DIR}/arch/${ARCH}
	${PROJECT_SOURCE_DIR}/arch/${ARCH}/include
	${PROJECT_SOURCE_DIR}/arch/${ARCH}/include/uapi
)
```

来源：所选固定time文件中的同一命令/条件分支体；仅证明表达式文本，不代表配置/外部进程实际执行。

### BB85-E065 | G02 ARCH-specific kbuild directory

[VERIFIED mykernel/scripts/target_kbuild.cmake::set(KBUILD_DIR)]

```cmake
set(KBUILD_DIR ${PROJECT_SOURCE_DIR}/arch/${ARCH}/kbuild)
```

界限：真实顶层命令及其内部参数；未知生成器/命令返回与磁盘真实效果；未运行构建。

### BB85-E066 | G02 C source of asm-offset target

[VERIFIED mykernel/scripts/target_kbuild.cmake::set(ASM_OFFSET_C_SRCS)]

```cmake
set(ASM_OFFSET_C_SRCS ${KBUILD_DIR}/asm-offsets.c)
```

界限：真实顶层命令及其内部参数；未知生成器/命令返回与磁盘真实效果；未运行构建。

### BB85-E067 | G02 intermediate assembly .s destination declared in source tree

[VERIFIED mykernel/scripts/target_kbuild.cmake::set(ASM_OFFSET_ASM_DST)]

```cmake
set(ASM_OFFSET_ASM_DST ${KBUILD_DIR}/asm-offsets.s)
```

界限：真实顶层命令及其内部参数；未知生成器/命令返回与磁盘真实效果；未运行构建。

### BB85-E068 | G02 final header destination declared under include/generated

[VERIFIED mykernel/scripts/target_kbuild.cmake::set(ASM_OFFSET_HEADER_DST)]

```cmake
set(ASM_OFFSET_HEADER_DST ${PROJECT_SOURCE_DIR}/include/generated/asm-offsets.h)
```

界限：真实顶层命令及其内部参数；未知生成器/命令返回与磁盘真实效果；未运行构建。

### BB85-E069 | G02 producer executable target

[VERIFIED mykernel/scripts/target_kbuild.cmake::add_executable(asm-offset)]

```cmake
add_executable(asm-offset)
```

界限：真实顶层命令及其内部参数；未知生成器/命令返回与磁盘真实效果；未运行构建。

### BB85-E070 | G02 producer target consumes its single C source explicitly

[VERIFIED mykernel/scripts/target_kbuild.cmake::target_sources(asm-offset)]

```cmake
target_sources(asm-offset PRIVATE ${ASM_OFFSET_C_SRCS})
```

界限：真实顶层命令及其内部参数；未知生成器/命令返回与磁盘真实效果；未运行构建。

### BB85-E071 | G02 producer has -save-temps option string

[VERIFIED mykernel/scripts/target_kbuild.cmake::target_compile_options(asm-offset)]

```cmake
target_compile_options(asm-offset PRIVATE
    -save-temps
)
```

界限：真实顶层命令及其内部参数；未知生成器/命令返回与磁盘真实效果；未运行构建。

### BB85-E072 | G02 producer include roots

[VERIFIED mykernel/scripts/target_kbuild.cmake::target_include_directories(asm-offset)]

```cmake
target_include_directories(asm-offset PRIVATE
	${INCLUDE_DIRS}
)
```

界限：真实顶层命令及其内部参数；未知生成器/命令返回与磁盘真实效果；未运行构建。

### BB85-E073 | G02 POST_BUILD command includes rm and && characters

[VERIFIED mykernel/scripts/target_kbuild.cmake::add_custom_command(asm-offset_POST_BUILD)]

```cmake
add_custom_command(
    TARGET asm-offset POST_BUILD
    # COMMAND echo "kbuild call test"
    COMMAND rm -f ${ASM_OFFSET_ASM_DST} && rm -f ${ASM_OFFSET_HEADER_DST}
```

界限：真实顶层命令及其内部参数；未知生成器/命令返回与磁盘真实效果；未运行构建。

### BB85-E074 | G02 copy Python command invocation text

[VERIFIED mykernel/scripts/target_kbuild.cmake::add_custom_command(asm-offset_POST_BUILD)]

```cmake
    COMMAND python3 ${ASM_OFFSET_COPY_TEMP_SCRIPT} "--input_dir=${CMAKE_CURRENT_BINARY_DIR}" "--output=${ASM_OFFSET_ASM_DST}"
```

界限：真实顶层命令及其内部参数；未知生成器/命令返回与磁盘真实效果；未运行构建。

### BB85-E075 | G02 generation Python command invocation text

[VERIFIED mykernel/scripts/target_kbuild.cmake::add_custom_command(asm-offset_POST_BUILD)]

```cmake
    COMMAND python3 ${ASM_OFFSET_GEN_SCRIPT} "--input=${ASM_OFFSET_ASM_DST}" "--output=${ASM_OFFSET_HEADER_DST}"
)
```

界限：真实顶层命令及其内部参数；未知生成器/命令返回与磁盘真实效果；未运行构建。

### BB85-E076 | G02 kernel target declares asm-offset prerequisite

[VERIFIED mykernel/scripts/target_kbuild.cmake::add_dependencies(kernel)]

```cmake
add_dependencies(kernel asm-offset)
```

界限：真实顶层命令及其内部参数；未知生成器/命令返回与磁盘真实效果；未运行构建。

### BB85-E077 | G02 named input include directive; no standalone companion compilation proved

[VERIFIED mykernel/arch/x86_64/kbuild/asm-offsets.c::include_asm-offsets_64.c]

```c
# include "asm-offsets_64.c"
```

仅静态源码关系；数值ABI、产出的汇编及生成头新鲜度未因该引文得到认证。

### BB85-E078 | G02 common produces TASK_threadsp via OFFSET

[VERIFIED mykernel/arch/x86_64/kbuild/asm-offsets.c::common]

```c
static void __used common(void)
{
	BLANK();
	OFFSET(TASK_threadsp, task_struct, thread.sp);
```

仅静态源码关系；数值ABI、产出的汇编及生成头新鲜度未因该引文得到认证。

### BB85-E079 | G02 common produces PTREGS_SIZE via DEFINE

[VERIFIED mykernel/arch/x86_64/kbuild/asm-offsets.c::common]

```c
	BLANK();
	DEFINE(PTREGS_SIZE, sizeof(struct pt_regs));
```

仅静态源码关系；数值ABI、产出的汇编及生成头新鲜度未因该引文得到认证。

### BB85-E080 | G02 representative TSS and perCPU offsets

[VERIFIED mykernel/arch/x86_64/kbuild/asm-offsets.c::common]

```c
	OFFSET(TSS_sp0, tss_struct, x86_tss.sp0);
	OFFSET(TSS_sp1, tss_struct, x86_tss.sp1);
	OFFSET(TSS_sp2, tss_struct, x86_tss.sp2);
	OFFSET(X86_top_of_stack, pcpu_hot, top_of_stack);
```

仅静态源码关系；数值ABI、产出的汇编及生成头新鲜度未因该引文得到认证。

### BB85-E081 | G02 macro defines pt_regs_ entry mapping

[VERIFIED mykernel/arch/x86_64/kbuild/asm-offsets_64.c::ENTRY]

```c
#define ENTRY(entry) OFFSET(pt_regs_ ## entry, pt_regs, entry)
```

仅静态源码关系；数值ABI、产出的汇编及生成头新鲜度未因该引文得到认证。

### BB85-E082 | G02 representative pt_regs fields in main

[VERIFIED mykernel/arch/x86_64/kbuild/asm-offsets_64.c::main]

```c
	ENTRY(bx);
	ENTRY(cx);
	ENTRY(dx);
	ENTRY(sp);
	ENTRY(bp);
```

仅静态源码关系；数值ABI、产出的汇编及生成头新鲜度未因该引文得到认证。

### BB85-E083 | G02 additional explicit pt_regs offsets

[VERIFIED mykernel/arch/x86_64/kbuild/asm-offsets_64.c::main]

```c
	OFFSET(IRQ_NR, pt_regs, irq_nr);
	OFFSET(ORIG_RAX, pt_regs, orig_ax);
	OFFSET(RIP, pt_regs, ip);
	OFFSET(CS, pt_regs, cs);
	OFFSET(EFLAGS, pt_regs, flags);
```

仅静态源码关系；数值ABI、产出的汇编及生成头新鲜度未因该引文得到认证。

### BB85-E084 | G02 DEFINE inline assembly emitted marker expression

[VERIFIED mykernel/include/linux/kernel/kbuild.h::DEFINE]

```c
#define DEFINE(sym, val) \
	asm volatile("\n.ascii \"->" #sym " %0 " #val "\"" : : "i" (val))
```

仅静态源码关系；数值ABI、产出的汇编及生成头新鲜度未因该引文得到认证。

### BB85-E085 | G02 BLANK empty -> marker expression

[VERIFIED mykernel/include/linux/kernel/kbuild.h::BLANK]

```c
#define BLANK() asm volatile("\n.ascii \"->\"" : : )
```

仅静态源码关系；数值ABI、产出的汇编及生成头新鲜度未因该引文得到认证。

### BB85-E086 | G02 OFFSET feeds offsetof struct member to DEFINE

[VERIFIED mykernel/include/linux/kernel/kbuild.h::OFFSET]

```c
#define OFFSET(sym, str, mem) \
	DEFINE(sym, offsetof(struct str, mem))
```

仅静态源码关系；数值ABI、产出的汇编及生成头新鲜度未因该引文得到认证。

### BB85-E087 | G02 existing header contains TASK_threadsp 552 (freshness unknown)

[VERIFIED mykernel/include/generated/asm-offsets.h::TASK_threadsp]

```c
	#define TASK_threadsp 552
```

仅固定源头/声明/旧头值；真实ABI、编译产物、新头更新和硬件使用不由此单独证明。

### BB85-E088 | G02 existing header contains PTREGS_SIZE 176 (not a fresh build receipt)

[VERIFIED mykernel/include/generated/asm-offsets.h::PTREGS_SIZE]

```c
	#define PTREGS_SIZE 176
```

仅固定源头/声明/旧头值；真实ABI、编译产物、新头更新和硬件使用不由此单独证明。

### BB85-E089 | G02 consumer uses TASK_threadsp for task kernel stack

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::secondary_startup_64]

```asm
	movq	current_task(%rdx),			%rax
	movq	TASK_threadsp(%rax),		%rsp
```

仅固定源头/声明/旧头值；真实ABI、编译产物、新头更新和硬件使用不由此单独证明。

### BB85-E090 | G02 assembly source includes a tracked generated header path

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::include_generated_asm-offsets.h]

```asm
#include <generated/asm-offsets.h>
```

仅固定源头/声明/旧头值；真实ABI、编译产物、新头更新和硬件使用不由此单独证明。

### BB85-E091 | G02 linker script independently has literal 176 constant, not a proof of generation

[VERIFIED mykernel/arch/x86_64/kernel.lds::__top_init_kernel_stack]

```ld
		__top_init_kernel_stack = __end_init_stack - 176 /* PTREGS_SIZE */;
```

仅固定源头/声明/旧头值；真实ABI、编译产物、新头更新和硬件使用不由此单独证明。

### BB85-E092 | G02 task_struct has thread member

[VERIFIED mykernel/sched/task/task_types.h::task_struct]

```c
		thread_s		thread;
```

仅固定源头/声明/旧头值；真实ABI、编译产物、新头更新和硬件使用不由此单独证明。

### BB85-E093 | G02 thread_struct has sp field

[VERIFIED mykernel/arch/x86_64/processor/processor_types_arch.h::thread_struct]

```c
	struct thread_struct {
		/* Cached TLS descriptors: */
		// desc_s tls_array[GDT_ENTRY_TLS_ENTRIES];
		reg_t			sp;
		ushort			es;
```

仅固定源头/声明/旧头值；真实ABI、编译产物、新头更新和硬件使用不由此单独证明。

### BB85-E094 | G02 pt_regs actual typedef begins here

[VERIFIED mykernel/arch/x86_64/include/asm/ptrace.h::pt_regs]

```c
		typedef struct pt_regs {
```

仅固定源头/声明/旧头值；真实ABI、编译产物、新头更新和硬件使用不由此单独证明。

### BB85-E095 | G02 selected trailing pt_regs fields used in offsets

[VERIFIED mykernel/arch/x86_64/include/asm/ptrace.h::pt_regs]

```c
			reg_t	cs;
			reg_t	flags;
			reg_t	sp;
			reg_t	ss;
		/* top of stack page */
```

仅固定源头/声明/旧头值；真实ABI、编译产物、新头更新和硬件使用不由此单独证明。

### BB85-E096 | G02 copy recursively matches exact asm-offsets.c.s pattern

[VERIFIED mykernel/scripts/asm-offset_copy_asm.py::root_dir.glob]

```python
matching_files = list(root_dir.glob(pattern))
```

仅脚本正文语句内的连续源文；模式组合与错误分支属静态条件推导，Python脚本从未运行。

### BB85-E097 | G02 first match indexed before empty-list safety check

[VERIFIED mykernel/scripts/asm-offset_copy_asm.py::matching_files]

```python
if (matching_files[0]):
    shutil.copy(matching_files[0], args.output)
```

仅脚本正文语句内的连续源文；模式组合与错误分支属静态条件推导，Python脚本从未运行。

### BB85-E098 | G02 copy script rejects missing input_dir flag

[VERIFIED mykernel/scripts/asm-offset_copy_asm.py::args.input_dir]

```python
if not args.input_dir:
	print("Error: missing input dir!")
	exit(1)
```

仅脚本正文语句内的连续源文；模式组合与错误分支属静态条件推导，Python脚本从未运行。

### BB85-E099 | G02 copy script rejects missing output flag

[VERIFIED mykernel/scripts/asm-offset_copy_asm.py::args.output]

```python
if not args.output:
	print("Error: missing output file!")
	exit(1)
```

仅脚本正文语句内的连续源文；模式组合与错误分支属静态条件推导，Python脚本从未运行。

### BB85-E100 | G02 parser chooses first quoted ->... match per line

[VERIFIED mykernel/scripts/asm-offsets_generate.py::re.findall]

```python
		matches = re.findall(r'"->([^"]*)"', input_line)
```

仅脚本正文语句内的连续源文；模式组合与错误分支属静态条件推导，Python脚本从未运行。

### BB85-E101 | G02 parser accesses fields1 when fields0 is nonempty

[VERIFIED mykernel/scripts/asm-offsets_generate.py::fields]

```python
			if (fields[0]):
				output_line += fields[0]
				if (fields[1]):
					fields[1] = fields[1].removeprefix("$")
					output_line = "#define " + output_line + " " + fields[1]
```

仅脚本正文语句内的连续源文；模式组合与错误分支属静态条件推导，Python脚本从未运行。

### BB85-E102 | G02 header write starts after complete input scan

[VERIFIED mykernel/scripts/asm-offsets_generate.py::open(args.output)]

```python
with open(args.output, "w") as output_file:
	output_file.write(output_content)
	output_file.flush()
```

仅脚本正文语句内的连续源文；模式组合与错误分支属静态条件推导，Python脚本从未运行。

### BB85-E103 | G02 generated header prefix is literal template

[VERIFIED mykernel/scripts/asm-offsets_generate.py::output_content]

```python
output_content = '''
#ifndef __ASM_OFFSETS_H__
#define __ASM_OFFSETS_H__
/*
```

仅脚本正文语句内的连续源文；模式组合与错误分支属静态条件推导，Python脚本从未运行。

### BB85-E104 | G02 parser splits on a single literal ASCII space

[VERIFIED mykernel/scripts/asm-offsets_generate.py::pattern]

```python
pattern = r"[ ]"
```

仅脚本正文语句内的连续源文；模式组合与错误分支属静态条件推导，Python脚本从未运行。

### BB85-E105 | G02 parser requires input filename

[VERIFIED mykernel/scripts/asm-offsets_generate.py::args.input]

```python
if not args.input:
	print("Error: missing input file!")
	exit(1)
```

仅脚本正文语句内的连续源文；模式组合与错误分支属静态条件推导，Python脚本从未运行。

### BB85-E106 | G03 kernel target requests -T linker script path

[VERIFIED mykernel/scripts/target_kernel.cmake::target_link_options(kernel)]

```cmake
target_link_options(kernel PRIVATE
    -T ${PROJECT_SOURCE_DIR}/arch/${ARCH}/kernel.lds
)
```

引文范围为独立CMake命令或链接器输出节/PHDRS唯一语句块；最终命令与ELF/安装行为NOT_RUN。

### BB85-E107 | G03 postbuild header copy command string

[VERIFIED mykernel/scripts/target_kernel.cmake::add_custom_command(kernel_POST_BUILD)]

```cmake
add_custom_command(TARGET kernel POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${PROJECT_SOURCE_DIR}/include/uapi
			${CMAKE_BINARY_DIR}/../headers)
```

引文范围为独立CMake命令或链接器输出节/PHDRS唯一语句块；最终命令与ELF/安装行为NOT_RUN。

### BB85-E108 | G03 install destination boot declared, not performed

[VERIFIED mykernel/scripts/target_kernel.cmake::install(TARGETS kernel)]

```cmake
install(TARGETS kernel DESTINATION boot)
```

引文范围为独立CMake命令或链接器输出节/PHDRS唯一语句块；最终命令与ELF/安装行为NOT_RUN。

### BB85-E109 | G03 PHDRS declares text/data PT_LOAD program headers

[VERIFIED mykernel/arch/x86_64/kernel.lds::PHDRS]

```ld
PHDRS {
	text PT_LOAD FLAGS(5);
	data PT_LOAD FLAGS(6);
	/* note PT_NOTE FLAGS(0); */
}
```

引文范围为独立CMake命令或链接器输出节/PHDRS唯一语句块；最终命令与ELF/安装行为NOT_RUN。

### BB85-E110 | G03 text virtual and AT link-load formula

[VERIFIED mykernel/arch/x86_64/kernel.lds::output_.text]

```ld
	.text ALIGN(_Page_Size) : AT(ADDR(.text) - __START_KERNEL_map)
	{
		_text = .;
```

引文范围为独立CMake命令或链接器输出节/PHDRS唯一语句块；最终命令与ELF/安装行为NOT_RUN。

### BB85-E111 | G03 startup and normal text input matching and segment assignment

[VERIFIED mykernel/arch/x86_64/kernel.lds::output_.text]

```ld
		KEEP(*(.head.text.startup))
		KEEP(*(.head.text))
		*(.text*)
	} : text = 0xcccccccc
```

引文范围为独立CMake命令或链接器输出节/PHDRS唯一语句块；最终命令与ELF/安装行为NOT_RUN。

### BB85-E112 | G03 init data section explicit input glob

[VERIFIED mykernel/arch/x86_64/kernel.lds::output_.init.data]

```ld
	.init.data : AT(ADDR(.init.data) - __START_KERNEL_map)
	{
		*(.init.data init.data.*) . = ALIGN(8);
```

引文范围为独立CMake命令或链接器输出节/PHDRS唯一语句块；最终命令与ELF/安装行为NOT_RUN。

### BB85-E113 | G03 .data and .data.rel/page-aligned patterns

[VERIFIED mykernel/arch/x86_64/kernel.lds::output_.data]

```ld
		*(.data)
		*(.data.rel*)
		*(.data..page_aligned*)
		_edata = .;
	} : data
```

引文范围为独立CMake命令或链接器输出节/PHDRS唯一语句块；最终命令与ELF/安装行为NOT_RUN。

### BB85-E114 | G03 .bss and COMMON input matches plus BSS boundary symbols

[VERIFIED mykernel/arch/x86_64/kernel.lds::output_.bss]

```ld
	.bss ALIGN(_Page_Size) : AT(ADDR(.bss) - __START_KERNEL_map)
	{
		__bss_start = .;
		*(.bss* COMMON)
		__bss_stop = .;
```

引文范围为独立CMake命令或链接器输出节/PHDRS唯一语句块；最终命令与ELF/安装行为NOT_RUN。

### BB85-E115 | G03 end symbol and SECTIONS closing boundary

[VERIFIED mykernel/arch/x86_64/kernel.lds::SECTIONS]

```ld
	. = ALIGN((_Page_Size));
	_end = .;
}
```

引文范围为独立CMake命令或链接器输出节/PHDRS唯一语句块；最终命令与ELF/安装行为NOT_RUN。

## 单语句补充锚（防止跨定义合并）

### BB85-E116 | copy glob basename pattern separately from glob expression

[VERIFIED mykernel/scripts/asm-offset_copy_asm.py::pattern]

```python
pattern = f"**/asm-offsets.c.s"
```

仅单条真实Python顶层语句；未执行脚本。

### BB85-E117 | copy root parsed as Path independently

[VERIFIED mykernel/scripts/asm-offset_copy_asm.py::root_dir]

```python
root_dir = Path(args.input_dir)
```

仅单条真实Python顶层语句；未执行脚本。

### BB85-E118 | input .s opened before scanning lines

[VERIFIED mykernel/scripts/asm-offsets_generate.py::open(args.input)]

```python
with open(args.input, "r") as input_file:
```

仅单条真实Python顶层语句；未执行脚本。

### BB85-E119 | D00 R84-S02 exact two-step PUD index expressions

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::__startup_64]

```c
	i = physaddr >> PUD_SHIFT;
	pud[(i + 0) % PTRS_PER_PUD] = (pudval_t)pmd + pgtable_flags;
	pud[(i + 1) % PTRS_PER_PUD] = (pudval_t)pmd + pgtable_flags;
```

独立真实函数定义内连续三行；先令i右移，再分别求(i+0)与(i+1)取模，不能写成有C运算优先级歧义的简化式。
