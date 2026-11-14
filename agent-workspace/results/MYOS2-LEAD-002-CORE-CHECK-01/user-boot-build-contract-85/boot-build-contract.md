---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-BOOT-BUILD-CONTRACT-85
record_type: static_build_selection_offsets_link_handoff_report
produced_by: GPT-6（本会话标识；后端不可独立认证）
model_per_owner: GPT-6＋Extra（Owner指定）
date: 2026-11-13
base_snapshot: time
read_channel: connector
status: static_complete_pending_mainline
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
open_questions: [BB85-U01, BB85-U02, BB85-U03, BB85-U04, BB85-U05, BB85-U06, BB85-U07, BB85-U08]
---

# 第85批｜启动构建来源、汇编偏移与链接交付契约

本批为原MYOS2-A-C06b/CHAT-EXEC-11在PR17执行分支追加的静态取证。121合同固定主线发布dd1e329a035c，执行原父7d53b2c1ca36，唯一源码time@a039d9803ade。证据短锚119条，保存在evidence.md；九组机器可读矩阵77行、26例新专题有限模型及完整D00对应关系见facts-and-boundaries.yaml；图仅表示源码定义/文本程序序/条件依赖。未执行CMake、编译器查询、任何Python、汇编、链接、安装、Graphviz、QEMU或内核程序。

## D00｜R84-01仅文档追加纠正

**准确裁定：第84批为RETURN，仅R84-01文档范围；技术主干在R84-S01/S02/S03限界下保留。** 原84六件与6849778226正式评论不修改，原作者49/49/0仅是冻结的作者自报计数。主线发现九处引文超出所标独立定义，故撤回84原“49条全部在单一定义体内”及类似一揽子总括；不能以新85锚替原84历史自检签PASS。D00状态为CORRECTION_SUBMITTED_PENDING_C06_REVIEW，由Pro主线独立判断闭合。

| 84原锚 | R84-01问题 | 新85严格替代源锚 | 后续消费修正 |
|---|---|---|---|
| E06 phys_base | 对齐、注释及EXPORT_SYMBOL在数据定义外 | BB85-E001 | 数据初值0仅由SYM_DATA一行证明；对齐、导出须各自定义/上下文，不能拼成一个phys_base锚 |
| E07 initial_code | 注释、__REFDATA及.balign不在SYM_DATA定义中 | BB85-E002、E014 | 初始函数指针值与.ref.data输入节宏分开，最终落段/可访问性仍未知 |
| E09 swapper_pg_dir | 先行extern不是该宏定义的一部分 | BB85-E003、E029 | 单行别名与C根实际定义分开；别名不是活动CR3的证明 |
| E12 KERNEL_IMAGE_SIZE | 两个#ifdef备选定义与条件行合成了一个宏 | BB85-E004、E005 | 1GiB/512MiB各自单行，条件选择依配置，非本轮预处理结果 |
| E13 PMD_SHIFT | PTRS_PER_PMD另有独立宏 | BB85-E006、E007 | 2MiB页粒度和512项表容量分属两个定义 |
| E18 linker ASSERT | 它位于SECTIONS右花括号之后 | BB85-E008、E115 | 顶层ASSERT独立，不能标作SECTIONS内部，更不能虚构C函数 |
| E20 PAGE_SHIFT | 空白、注释与PAGE_SIZE混在本宏 | BB85-E009、E010 | PAGE_SHIFT=12和PAGE_SIZE=4096各由独立宏合用推得 |
| E22 pgd_t | 前一行pgdval_t是另一种typedef | BB85-E011、E012、E029 | 两个类型定义分开；init_top_pgt实际数组和ABI8字节前提另证 |
| E38 MAX_PHYSMEM_BITS | 注释掉的旧定义不是活动宏 | BB85-E013 | 仅活动单行宏=46；不能把注释当已选择配置或CPU实测 |

**补齐旧84遗漏的支持链。** __STARTUP的真实节定义、入口与KEEP引用分别见BB85-E015/E016/E017/E026/E027；setup_arch调用提前映射及后续init_mem_mapping的两处真正调用者分别见E018/E019，提前映射函数的*init_pgd=*early_pgd单项赋值见E020；va_text和va_end两个赋值见E021；pmd_entry初始flags与物理值相加见E022；rip_rel_ptr的DEBUG宏、非DEBUG非PIC内联函数及__pic__分支的RIP_REL_REF见E023/E024/E025。C源码__startup_64内PUD索引实际要先算i=physaddr>>PUD_SHIFT，再用(i+0)%PTRS_PER_PUD和(i+1)%PTRS_PER_PUD分别求值，原B-ID03含糊式不能当精确C表达式；以E119为准。

**84模型与矩阵的更正性消费。** 在_text名义物理偏移至少16MiB、_end不超过链接基址后1GiB、_end>=_text及PMD_SIZE=2MiB、无回绕的同一模型中，span最大1008MiB，因此N=ceil(span/2MiB)最大504；B19的N512、B20的N513**两者都不在该声明链接模型可达范围内**，仅保留为脱离实际链接前提的纯槽位整数算术。原technical_matrix_count=56实际是9组矩阵共56行，不能解读为56张矩阵；本批新增的是另外9组77行，不回算旧84为新测试。

**证明层级纠正。** 固定options_flags.cmake显示-mcmodel=kernel -fno-pie -fno-pic等源字符串；是否进入DEBUG、__pic__如何定义、目标/源属性有无改变、真实机器码和CPU执行均是独立问题。旧ER84-F02/F03/F05及其U只按新构建定义增加边界，不判定代码机器地址、.ref.data节被丢弃或512MiB构建已经失败。旧R83-S01/S02/S03、R82-S01/S02/S03、R81-S01/S02/S03及PT81/DM82/TI83已有限定继续有效，不重做旧批。

## G01/A｜源码选择、配置与启动消费者

**入口和路径。** mykernel/CMakeLists.txt首先project(MyOS2_Kernel ASM C)，把SCRIPTS_PATH设为PROJECT_SOURCE_DIR/scripts，include options_flags，再add_executable(kernel)，随后include target_kbuild。这里的PROJECT_SOURCE_DIR、CMAKE_BINARY_DIR是CMake变量表达式，不是已经读取的某台本机绝对路径；ARCH控制arch/${ARCH}/kbuild、include根和下级add_subdirectory，不能只凭默认字符串承诺真实配置生效。尤其options_flags中的if(NOT DEFINED ${ARCH})并非字面if(NOT DEFINED ARCH)，其变量名展开和用户注入边界保留BB85-U01。

**三个代表源的不同路由。** 顶层对PROJECT_SOURCE_DIR/*.c与/*.S分别进行GLOB_RECURSE，又从两列表移除KBUILD_FILES；该KBUILD_FILES来自所选ARCH的kbuild/*。在ARCH=x86_64、glob展开正常且没有其它源属性覆盖的条件下，head64.c是kernel C候选、head_64.S是kernel ASM候选；asm-offsets.c匹配kbuild排除集合，因此不由kernel普通C列表消费，而由add_executable(asm-offset)的target_sources显式消费。asm-offsets_64.c被包含进asm-offsets.c源码，不能把它又记作已独立编译的第三个目标。最终kernel的target_sources在target_kernel.cmake消费两个列表；arch及其子目录可能还有设置，未生成实际source list/compile_commands。

**C/ASM标记不能混称。** 公共ASM变量包含-m64、-mcmodel=kernel、-fno-pie/-fno-pic、-D__KERNEL__、-D__ASSEMBLY__、-DASM_FILE；公共C变量包含同型代码模型和非PIE字符串、freestanding/nostdlib与多项CONFIG，但不在本set片段内添加ASM_FILE/__ASSEMBLY__。Debug分支设置-DDEBUG，非Debug分支设置-DRELEASE和UNUSED_MACRO；arch/x86_64子目录又对CMAKE_C_FLAGS进行追加，其中-Wl,--no-check-sections作为CFLAGS文本出现。实际变量作用域、源/目标属性、配置缓存、编译器调用未实测，不用这份文字反推出最终机器码。CONFIG_RANDOMIZE_BASE在已读options文件没有定义，但不代表项目或命令行全局不存在。rip_rel_ptr的非PIC非DEBUG函数与DEBUG宏是两条定义路径，若__pic__成立，所读分支显示RIP_REL_REF(var)而非同样的rip_rel_ptr实现，不能强行套用。

## G02/B｜汇编偏移的生产—提取—消费路径

**生产目标的声明。** target_kbuild.cmake以ARCH确定KBUILD_DIR，明确把arch/${ARCH}/kbuild/asm-offsets.c加入独立asm-offset可执行目标，附加-save-temps并给出include目录。该C文件包含asm-offsets_64.c，源内static common()采用OFFSET(TASK_threadsp, task_struct, thread.sp)、DEFINE(PTREGS_SIZE, sizeof(struct pt_regs))以及其它TSS/CPU字段；被包含的asm-offsets_64.c有自己的main()与pt_regs字段OFFSET语句。kbuild.h将OFFSET展开为DEFINE/offsetof，并用带“->”标记的asm volatile .ascii字符串表达输出偏移的意图；这些源表达式不是新ABI数字，更不是实际生成的汇编源码。

**生成链名义顺序。** producer设置ASM_OFFSET_ASM_DST在${KBUILD_DIR}/asm-offsets.s，ASM_OFFSET_HEADER_DST在${PROJECT_SOURCE_DIR}/include/generated/asm-offsets.h。其POST_BUILD文本先声明`COMMAND rm -f ${ASM_OFFSET_ASM_DST} && rm -f ${ASM_OFFSET_HEADER_DST}`，随后调用copy脚本，再调用generate脚本。是否由生成器以shell运算符处理&&、单个COMMAND的参数怎样传递、命令出错是否阻断后续、是否执行POST_BUILD，本批全部未知，因此不能只因看到rm文字就声称已删除文件。kernel有add_dependencies(kernel asm-offset)，说明声明了目标先后依赖，但该名称**不能独自证明头文件按当前类型/选项重新生成**。

**复制脚本的准确控制流。** asm-offset_copy_asm.py解析--input_dir、--output，建立Path(args.input_dir)，使用`**/asm-offsets.c.s`做递归glob，再立即在`if (matching_files[0]):`中取第0项；零匹配时没有先检查长度，会触发IndexError，多个匹配时选第一个，没有显式排序和单值确认。目标复制由shutil.copy表达，真实中间.s路径、文件匹配数、读写权限及COPY返回均未得到。脚本提供的注释本机路径只是注释，不能当当前机器路径。

**生成脚本的准确控制流。** asm-offsets_generate.py逐行搜索双引号包住的`->...`子串，对该行只取`matches[0]`，用`re.split('[ ]', ...)`按一个ASCII空格切字段。fields[0]为真时立刻访问fields[1]；`"->ONLYTOKEN"`这种无第二字段条件导致IndexError。`"->NAME  $7"`中的两个连续空格使fields[1]为空，逻辑会跳过宏定义而追加空白行；`"->NAME $7"`则在第二字段存在时移除开头的`$`并生成#define一行。缺少匹配的输入行被忽略，空payload的标记也不会生成有效define。这些只是依据代码的**独立字符串条件模型**，没有导入、执行、修改Python脚本或真实临时.s文件。

生成器先构造一个带include guard的内容字符串，完整读取、解释输入后才以`open(args.output, 'w')`打开写入目标；若解释过程提前抛异常，该Python过程尚未走到输出open，但旧头文件最终是保留、被前置rm删除还是其它状态仍取决于未运行的构建命令与文件系统，不能从单个Python语句定论。已追踪的include/generated/asm-offsets.h确实有TASK_threadsp=552、PTREGS_SIZE=176等数值，head_64.S也包含该头并用TASK_threadsp(%rax)取栈指针；kernel.lds另用字面数值176计算__top_init_kernel_stack。但这些分别是**存量头、源消费者和链接脚本文字**，不是当前生产者成功的证据，也不证明这些数字与现ABI相符。

**失效门与可用程度。** 代表路径是：CMake配置/目标创建 → asm-offset C源选择 → 目标实际产生目标外部编译结果（未证）→ 被期待命名的asm-offsets.c.s（未证）→ 脚本首个glob文件复制（未证）→ marker解析/头部写出（未证）→ kernel编译时取到新文件（未证）→ link（未证）。只有源的依赖文本可连接，不能将多个未知阶段串联成已完成生产线。无实际CMake/gcc/Python命令执行，不能补造成功日志。

## G03/C｜链接脚本选择、节匹配和镜像交付

**哪个目标引用哪个脚本。** target_kernel.cmake的target_link_options(kernel PRIVATE -T ${PROJECT_SOURCE_DIR}/arch/${ARCH}/kernel.lds)为kernel指定脚本路径；options_flags.cmake还给CMAKE_EXE_LINKER_FLAGS赋了`-Wl,--build-id=none,--gc-sections`的文字。两处都属于配置输入，不代表已经生成实际链接命令，`--gc-sections`的字符串也不能自动证明某个节被删。

**入口与物理/虚拟地址。** 固定kernel.lds声明ENTRY(phys_startup_64)，在SECTIONS体内定义`phys_startup_64 = ABSOLUTE(startup_64-__START_KERNEL_map)`，对.text等输出节使用AT(ADDR(.text)-__START_KERNEL_map)的装载地址表达；_text标记.text开端，_end位于SECTIONS结束之前。PHDRS中有text/data两类PT_LOAD声明，这是脚本期望而不是已经产生的ELF program header。没有实际link map、objdump、readelf、固件/loader消费或启动证据。

**输入节匹配与KEEP。** __STARTUP定义.head.text.startup，kernel.lds在.text输出节KEEP该输入节，并KEEP(.head.text)，普通*(.text*)是另一种匹配表达。__INITDATA为.init.data，脚本明列*(.init.data init.data.*)；.data匹配*(.data)、*(.data.rel*)及*(.data..page_aligned*)。initial_code由__REFDATA产生.ref.data输入节，而该脚本没有按名称显式列出*(.ref.data)，因而它是**孤儿节位置/保留边界需要后证的候选**，不能直接断言链接失败、内容丢失或一定不在PT_LOAD。init_top_pgt在C文件中有未初始化全局定义，脚本有*(.bss* COMMON)，但未生成C对象及最终ELF，所以只能叫BSS/COMMON候选，不能宣称已观测的物理区间。输入节→输出节→PHDR→可访问地址四层分开评价。

**链接上界与交付。** 顶层ASSERT在SECTIONS闭括号之外，限定`_end - 0xffffffff80000000 <= 1GiB`，不是对CONFIG_RANDOMIZE_BASE不定义时512MiB静态高映射的充分覆盖检查。仅以链接模型“_text起点至少基址加16MiB，_end至多基址加1GiB，_end>=_text，PMD_SIZE=2MiB且无回绕”为前提，N<=504成立，84旧N512/N513都不可借此当可达启动例。EXECUTABLE_OUTPUT_PATH被设为${CMAKE_BINARY_DIR}，该赋值只是声明；kernel POST_BUILD另有include/uapi复制到../headers的命令声明，install(TARGETS kernel DESTINATION boot)只是一条安装规则。没有任何二进制、头文件复制、安装或引导器载入实际发生。

## 与84既有问题的衔接和范围

新BB85-F01..F05五项条件候选与BB85-U01..U08八项证据缺口记录在deferred-findings.yaml。旧ER84-F02（PIC/编译条件）、F03（512MiB/1GiB的证明层级）、F05（.ref.data孤儿节）得到定点构建输入约束，但不自行关闭；其它ER84、PT81、DM82、TI83及更早历史关闭/撤回结果仍按原qualified_id保留。这里的五项候选不是五个已确认或可复现的运行故障，旧内容和新问题不得重复算独立根因。下一审查主体为MYOS2-A-C06 Pro，而不是本次执行者。



## 九组Markdown矩阵（与facts机器表逐行一致）

### G01/A1 源选择

| 行ID | 对象 | 源码/构建表达 | 条件结论 | 未知边界 | 证据ID |
|---|---|---|---|---|---|
| BB85-A-S01 | root project and scripts | project(ASM C); SCRIPTS_PATH=${PROJECT_SOURCE_DIR}/scripts; include(options_flags) | target config text rooted in mykernel project source dir | project configuration and filesystem not executed | BB85-E033, BB85-E034, BB85-E035 |
| BB85-A-S02 | kernel target creation | add_executable(kernel) before target_kbuild include | kernel named target exists in configuration declaration | not an executable disk file | BB85-E036, BB85-E037 |
| BB85-A-S03 | offset glob exclusion | file(GLOB_RECURSE ${PROJECT_SOURCE_DIR}/arch/${ARCH}/kbuild/*) | selected ARCH kbuild subtree is exclusion candidate | actual glob expansion and symlink/path normalization not tested | BB85-E038 |
| BB85-A-S04 | C candidate enumeration | GLOB_RECURSE ${PROJECT_SOURCE_DIR}/*.c then REMOVE_ITEM KBUILD_FILES | head64.c is a conditional candidate; asm-offsets.c excluded when exact match exists | glob output and all other source modifiers unenumerated | BB85-E039, BB85-E041 |
| BB85-A-S05 | ASM candidate enumeration | GLOB_RECURSE ${PROJECT_SOURCE_DIR}/*.S then REMOVE_ITEM KBUILD_FILES | head_64.S matches case-sensitive extension candidate under selected ARCH | unresolved evaluated glob set and later target changes | BB85-E040, BB85-E042 |
| BB85-A-S06 | producer target | target_sources(asm-offset PRIVATE ${KBUILD_DIR}/asm-offsets.c); #include asm-offsets_64.c in that C unit | offsets.c explicit producer; companion text included into same C translation unit in source | does not imply an independent companion target | BB85-E065, BB85-E066, BB85-E069, BB85-E070, BB85-E077 |
| BB85-A-S07 | kernel source consumer | target_sources(kernel PRIVATE ${KERNEL_C_SRCS} ${KERNEL_ASM_SRCS}) | two list variables are handed to kernel target | no generated compile database or real evaluated list | BB85-E043, BB85-E044 |
| BB85-A-S08 | architecture CMake subtree | add_subdirectory(arch)->add_subdirectory(${ARCH})->add_subdirectory(kernel) | textual nested route and explicit arch flags | other subdirectories may add target sources/settings; not exhausted | BB85-E046, BB85-E047, BB85-E049, BB85-E050 |
| BB85-A-S09 | header visibility | target_include_directories(kernel ${INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/../myloader) | include directories declared separately from kernel source glob | include existence/order and effective command unknown | BB85-E045, BB85-E063, BB85-E064 |

### G01/A2 flag及宏来源

| 行ID | 对象 | 源码/构建表达 | 条件结论 | 未知边界 | 证据ID |
|---|---|---|---|---|---|
| BB85-A-F01 | ARCH selected default | if(NOT DEFINED ${ARCH}) then set(ARCH x86_64) | condition tests expanded name, not literally DEFINED ARCH | CMake interpretation, command line/environment injection untested | BB85-E051 |
| BB85-A-F02 | Debug branch | RELEASE_FLAG=-DDEBUG via branch | Debug text token selected only if condition evaluates true | case/other configuration outcomes unknown | BB85-E052 |
| BB85-A-F03 | non-Debug branch | RELEASE_FLAG=-DRELEASE, UNUSED_MACRO=-Wunused-macros | source text has else branch assignment | unknown actual selection and cache values | BB85-E053 |
| BB85-A-F04 | ASM CMake flags | CMAKE_ASM_FLAGS includes -m64 -mcmodel=kernel -fno-pie -fno-pic -D__ASSEMBLY__ -DASM_FILE -D__KERNEL__ | text differs from C flags; source-level macro conditions may differ | actual assembler invocation/other flags NOT_RUN | BB85-E055, BB85-E056 |
| BB85-A-F05 | C CMake flags | CMAKE_C_FLAGS contains -m64 -mcmodel=kernel -fno-pie -fno-pic etc | no __ASSEMBLY__/ASM_FILE in this C set expression | actual compiler command/target overrides unknown | BB85-E057, BB85-E058 |
| BB85-A-F06 | CONFIG macros | CMAKE_C_FLAGS += CONFIG_FLATMEM,64BIT,SLUB,GRUB2_BOOTUP_SUPPORT, KVM_GUEST... | selected source text advertises these defines | CONFIG_RANDOMIZE_BASE absent only from read options; other definitions not excluded | BB85-E059, BB85-E060 |
| BB85-A-F07 | linker flag string | set(CMAKE_EXE_LINKER_FLAGS "-Wl,--build-id=none,--gc-sections") | only configuration text, not proof of gc effects | final link invocation and section GC unknown | BB85-E061 |
| BB85-A-F08 | nested arch C FLAGS | set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_X86_64 ...") | additional variable expression in child CMakeLists | no proof of when/how applied to already defined kernel target | BB85-E047 |
| BB85-A-F09 | linker-like token in C flags | -Wl,--no-check-sections appended to CMAKE_C_FLAGS | string placement is C flags rather than target_link_options | real command treatment and correctness not tested | BB85-E048 |
| BB85-A-F10 | external host query | execute_process(COMMAND bash -c gcc --print-file-name=include...) | query would be invoked by CMake configuration if reached | not invoked in this static task; CC_INC_PATH runtime unknown | BB85-E054 |

### G01/A3 消费者配置

| 行ID | 对象 | 源码/构建表达 | 条件结论 | 未知边界 | 证据ID |
|---|---|---|---|---|---|
| BB85-A-C01 | C startup | recursive C candidate + target_sources(kernel) | conditional kernel translation-unit inclusion | no compiled object or -fPIC backend attestation | BB85-E039, BB85-E044, BB85-E057 |
| BB85-A-C02 | ASM startup | recursive .S candidate and target_sources(kernel) | conditional assembly inclusion and generated header include | no preprocessor/assembler actually executed | BB85-E040, BB85-E044, BB85-E090 |
| BB85-A-C03 | offset producer | excluded from kernel C list when exact KBUILD match; explicit asm-offset target source | producer and consumer roles separated | cannot infer either target built | BB85-E038, BB85-E041, BB85-E066, BB85-E070 |
| BB85-A-C04 | nonPIC DEBUG RIP helper | !__ASSEMBLY__ && !__pic__ && DEBUG uses rip_rel_ptr macro with leaq %a1 | single source branch only | DEBUG selection and assembly validity unmeasured | BB85-E023, BB85-E024, BB85-E052 |
| BB85-A-C05 | nonPIC nonDEBUG RIP helper | !__ASSEMBLY__ && !__pic__ && !DEBUG uses inline rip_rel_ptr leaq %c1(%rip) | different source branch from DEBUG | actual codegen and address correctness unproved | BB85-E023, BB85-E057 |
| BB85-A-C06 | PIC variant | read asm.h branch shows RIP_REL_REF(var) (var), not an equivalent rip_rel_ptr definition in that branch | cannot generalize nonPIC helper to PIC case | only read file snapshot scoped conclusion, not all headers/code | BB85-E025 |
| BB85-A-C07 | KERNEL_IMAGE_SIZE | two mutually exclusive source definitions (1GiB/512MiB) | actual macro branch requires preprocessor configuration, not filenames | none of the real builds was performed | BB85-E004, BB85-E005 |

### G02/B1 offset产物谱系

| 行ID | 对象 | 源码/构建表达 | 条件结论 | 未知边界 | 证据ID |
|---|---|---|---|---|---|
| BB85-B-L01 | producer source | ASM_OFFSET_C_SRCS=${KBUILD_DIR}/asm-offsets.c; asm-offset target_sources | explicit C source producer | ARCH actual path not configured | BB85-E065, BB85-E066, BB85-E069, BB85-E070 |
| BB85-B-L02 | producer include | #include asm-offsets_64.c; static common() and included main() | two source texts in one declared C translation unit | not actual compiled output | BB85-E077, BB85-E078, BB85-E081, BB85-E082 |
| BB85-B-L03 | marker encoding | DEFINE asm volatile .ascii marker; OFFSET uses offsetof(struct str,mem); BLANK emits empty marker | intended annotated assembly text for offset generation | not proof of emitted assembly/ABI | BB85-E084, BB85-E085, BB85-E086 |
| BB85-B-L04 | representative field | OFFSET(TASK_threadsp, task_struct, thread.sp); struct has thread member and thread_struct.sp | producer expression structurally targeted at type field | current numeric offset not measured | BB85-E078, BB85-E092, BB85-E093 |
| BB85-B-L05 | representative size | DEFINE(PTREGS_SIZE, sizeof(struct pt_regs)); pt_regs type seen | producer computes type size at compile time | current ABI size not run | BB85-E079, BB85-E094, BB85-E095 |
| BB85-B-L06 | saved assembly artifact | -save-temps option and scripts expect asm-offsets.c.s | temporary assembly name is scripts' expected convention | no exact compiler output filename or encoding assured | BB85-E067, BB85-E071, BB85-E096, BB85-E116 |
| BB85-B-L07 | copy intermediate | recursive pattern, first selection, shutil.copy to ${KBUILD_DIR}/asm-offsets.s | input under binary dir becomes nominal source-tree .s | no actual file copied | BB85-E067, BB85-E074, BB85-E096, BB85-E097 |
| BB85-B-L08 | generate header | parse -> marker and emit #define; ASM_OFFSET_HEADER_DST=${PROJECT_SOURCE_DIR}/include/generated/asm-offsets.h | nominal derived header target in source tree | no actual generated bytes or freshness verified | BB85-E068, BB85-E075, BB85-E100, BB85-E101, BB85-E102 |
| BB85-B-L09 | consumer | include <generated/asm-offsets.h>; movq TASK_threadsp(%rax),%rsp | ASM uses derived symbol via include path | no assembler preprocessing or register state verified | BB85-E087, BB85-E089, BB85-E090 |
| BB85-B-L10 | old tracked header | TASK_threadsp=552, PTREGS_SIZE=176 literal stored | bytes exist at fixed time repository object only | not current ABI or successful generator certificate | BB85-E087, BB85-E088, BB85-E091 |

### G02/B2 命令顺序与新鲜度

| 行ID | 对象 | 源码/构建表达 | 条件结论 | 未知边界 | 证据ID |
|---|---|---|---|---|---|
| BB85-B-O01 | prebuild inclusion | include target_kbuild before KBUILD globs / kernel target_sources | definition order in CMake text | CMake configuration itself unexecuted | BB85-E037, BB85-E038, BB85-E043 |
| BB85-B-O02 | producer build | C source, include dirs, -save-temps | producer target declared | compiler process/object/assembly absent | BB85-E070, BB85-E071, BB85-E072 |
| BB85-B-O03 | POST_BUILD deletion expression | COMMAND rm -f ${ASM_OFFSET_ASM_DST} && rm -f ${ASM_OFFSET_HEADER_DST} | string contains rm and && token; may affect old source-tree artifact | actual generator shell interpretation, command status and side effects untested | BB85-E073 |
| BB85-B-O04 | POST_BUILD copy/generate | COMMAND python3 copy ; COMMAND python3 generate | ordered text in one add_custom_command | not proof both commands ran successfully | BB85-E074, BB85-E075 |
| BB85-B-O05 | dependency edge | add_dependencies(kernel asm-offset) | target dependency exists in source | not full stale header/parallel/implicit cache guarantee | BB85-E076 |
| BB85-B-O06 | invalidation and old content | preexisting macros in time snapshot | target may be skipped/cached; old header may remain when no postbuild | no build system executed or forced rebuild | BB85-E087, BB85-E088 |
| BB85-B-O07 | input path expectations | --input_dir=${CMAKE_CURRENT_BINARY_DIR}, glob **/asm-offsets.c.s; --output=${KBUILD_DIR}/asm-offsets.s | script matching depends on actual compiler temp artifact location | save-temps name/location uncertain | BB85-E067, BB85-E071, BB85-E074, BB85-E096 |
| BB85-B-O08 | failure and destination | header opened with write only after full source scan | if parser fails earlier, generator has not opened header; prior rm outcome separate | actual previous file state unknown | BB85-E073, BB85-E101, BB85-E102 |

### G02/B3 解析及失败边界

| 行ID | 对象 | 源码/构建表达 | 条件结论 | 未知边界 | 证据ID |
|---|---|---|---|---|---|
| BB85-B-P01 | copy no matches | if (matching_files[0]): before length guard | Python indexing raises before copy | hypothetical file set, actual glob unrun | BB85-E096, BB85-E097 |
| BB85-B-P02 | copy single match | copy first match if truthy | shutil.copy selected f to destination | path permissions and copy return unknown | BB85-E097, BB85-E117 |
| BB85-B-P03 | copy multiple matches | matching_files[0] chosen | only first match copied; source does not sort explicitly | actual glob enumeration/order unknown | BB85-E096, BB85-E097 |
| BB85-B-P04 | strict source basename | pattern literal **/asm-offsets.c.s | other basename does not match this pattern | compiler naming is not verified | BB85-E096, BB85-E116 |
| BB85-B-P05 | parser input lacks marker | re.findall produces no matched substring | no define emitted for this line | not actual generated input seen | BB85-E100, BB85-E118 |
| BB85-B-P06 | parser first match | matches[0] selected before field split | later markers on same line ignored by this code | no semantic guarantee about valid assembly strings | BB85-E100 |
| BB85-B-P07 | empty marker | matches list contains empty string; fields[0] false | writes tab+blank line, no #define | hypothetical input, no script run | BB85-E100, BB85-E101 |
| BB85-B-P08 | single token marker | fields ['NAME']; if fields[0] then tests fields[1] | IndexError condition before opening output | hypothetical unexpected marker | BB85-E101, BB85-E102 |
| BB85-B-P09 | double ASCII space | re.split('[ ]') makes empty fields[1] | no #define emitted despite later numeric token | not a validated actual compiler line | BB85-E101, BB85-E104 |
| BB85-B-P10 | normal marker | split into name and $value; removeprefix('$') | candidate #define TASK_threadsp 552 | illustrative numeric text; not evidence of fresh current ABI | BB85-E100, BB85-E101, BB85-E087 |
| BB85-B-P11 | partial failure state | generator opens output only at final stage; CMake deletion command is separate | header state may be stale/absent/partially rewritten depending command execution | never claim live file state without running authorized build | BB85-E073, BB85-E074, BB85-E075, BB85-E102 |

### G03/C1 链接脚本选择

| 行ID | 对象 | 源码/构建表达 | 条件结论 | 未知边界 | 证据ID |
|---|---|---|---|---|---|
| BB85-C-S01 | kernel link target | target_link_options(kernel PRIVATE -T ${PROJECT_SOURCE_DIR}/arch/${ARCH}/kernel.lds) | selected source script path under chosen ARCH | no actual linker command or script read by linker | BB85-E106 |
| BB85-C-S02 | global link options | CMAKE_EXE_LINKER_FLAGS -Wl,--build-id=none,--gc-sections | string exists in source text | no actual gc-section operation verified | BB85-E061 |
| BB85-C-S03 | entry | ENTRY(phys_startup_64) and phys_startup_64=ABSOLUTE(startup_64-__START_KERNEL_map) | declares intended ELF entry expression | actual ELF e_entry and loader behavior NOT_RUN | BB85-E026, BB85-E027 |
| BB85-C-S04 | program header skeleton | PHDRS text/data PT_LOAD FLAGS(5)/(6) | two PT_LOAD declarations as script text | actual program header file offsets and effective segments unknown | BB85-E109 |
| BB85-C-S05 | text input route | output .text AT link subtraction and KEEP startup/head; *( .text* ) | matching and retention expressions exist | individual object sections and GC execution not proved | BB85-E110, BB85-E111 |

### G03/C2 输入节保留

| 行ID | 对象 | 源码/构建表达 | 条件结论 | 未知边界 | 证据ID |
|---|---|---|---|---|---|
| BB85-C-I01 | head.startup | input .head.text.startup; linker KEEP(*(.head.text.startup)) | specific startup section matches protected input pattern | not actual object section verification | BB85-E015, BB85-E016, BB85-E017, BB85-E111 |
| BB85-C-I02 | head text | KEEP(*(.head.text)) | specific .head.text section match if produced | no actual contents or selected compiler path | BB85-E017, BB85-E111 |
| BB85-C-I03 | ordinary .text | *(.text*) under .text output | prefix wildcard can match .text and .text.* | final object emission unknown | BB85-E111 |
| BB85-C-I04 | early PGD static .init.data | *(.init.data init.data.*) in output .init.data | section-creation macro and matching expression both visible | final section offset and PT_LOAD mapping unknown | BB85-E112, BB85-E015 |
| BB85-C-I05 | ordinary .data and aligned | *(.data), *(.data.rel*), *(.data..page_aligned*) in .data | script explicitly lists these input patterns | phys_base alignment vs data symbol details kept separate | BB85-E001, BB85-E113 |
| BB85-C-I06 | orphan candidate .ref.data | .ref.data input directive, no explicit .ref.data match in shown kernel.lds | link orphan treatment/segment assignment unresolved | not proof of dropped section or unbootability | BB85-E002, BB85-E014, BB85-E113 |
| BB85-C-I07 | C root BSS candidate | BSS/COMMON wildcard and actual root C declaration | candidate .bss or COMMON output under successful compilation | compiler emission, init/root byte layout unmeasured | BB85-E029, BB85-E114 |
| BB85-C-I08 | end and size | _end assigned near SECTIONS end; top-level ASSERT after closure | input section aggregation affects size condition | linker not run, no actual link map | BB85-E008, BB85-E115 |
| BB85-C-I09 | section vs segment | output sections .text/.data/.init.data/.bss and PHDRS text/data | input section, output section, PHDR are distinct levels | no file-backed PT_LOAD range verified | BB85-E109, BB85-E110, BB85-E112, BB85-E113, BB85-E114 |

### G03/C3 入口及交付

| 行ID | 对象 | 源码/构建表达 | 条件结论 | 未知边界 | 证据ID |
|---|---|---|---|---|---|
| BB85-C-D01 | entry point symbol | ENTRY(phys_startup_64) | entry uses physical-expression symbol | not proof bootloader used ELF entry | BB85-E026, BB85-E027 |
| BB85-C-D02 | virtual-load address split | ADDR(.text) vs AT(ADDR(.text)-__START_KERNEL_map) | VMA and requested LMA expressions differ | actual load physical address unknown | BB85-E110 |
| BB85-C-D03 | _text/_end link bounds | _text at start .text; _end after output section closure | static link length expression can be formed | actual bytes/layout not generated | BB85-E110, BB85-E115 |
| BB85-C-D04 | ASSERT outside SECTIONS | _end-0xffffffff80000000<=1GiB | apparent 1GiB global bound, not 512MiB static mapping guarantee | actual ASSERT pass/fail not observed | BB85-E008, BB85-E030 |
| BB85-C-D05 | executable output path | set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}) | declares output-directory variable | actual on-disk target artifact not observed | BB85-E062 |
| BB85-C-D06 | POST_BUILD header export | copy_directory include/uapi to ${CMAKE_BINARY_DIR}/../headers | declared copying step after target build | command not invoked and destination not observed | BB85-E107 |
| BB85-C-D07 | install rule | install(TARGETS kernel DESTINATION boot) | declares installation destination for install action | not proof installed into host or boot media | BB85-E108 |
| BB85-C-D08 | GC vs retain | --gc-sections string plus KEEP(.head.text.startup/.head.text) | potential behavior must be checked against real linker behavior | no current linker execution or external spec; do not infer deletion/survival | BB85-E061, BB85-E111 |



## 26个独立有限条件模型（不是仓库脚本测试）

| ID/阶段 | 输入或假设 | 预期静态结果 | 首个停止证明的位置 |
|---|---|---|---|
| BB85-M01/configuration | ARCH not set；ARCH variable missing; `if(NOT DEFINED ${ARCH})` as written | cannot certify correct default selection by literal guard | CMake expression expansion and user/environment values |
| BB85-M02/configuration | ARCH=x86_64；selected ARCH path resolves to arch/x86_64 | conditional arch/kernel source and offset path joins | real CMake glob evaluation not run |
| BB85-M03/configuration | ARCH other；ARCH differs or resolves undefined dir | script path construction changes with ARCH | no actual configuration or directory check for alternate ARCH |
| BB85-M04/configuration | Debug branch；CMAKE_BUILD_TYPE text is Debug | RELEASE_FLAG becomes -DDEBUG string; no machine code claim | later target/source flags and compiler command |
| BB85-M05/configuration | non-Debug branch；CMAKE_BUILD_TYPE differs from Debug | -DRELEASE and -Wunused-macros candidate text | actual effective compile command |
| BB85-M06/source selection | kernel head64.c；ARCH=x86_64 and recursive C glob includes file; no later removal | candidate kernel C compilation unit | no evaluated target file list |
| BB85-M07/source selection | kernel head_64.S；ARCH=x86_64 and recursive ASM glob includes file; no later removal | candidate kernel assembly compilation unit | no assembly command or linked text verified |
| BB85-M08/source selection | offset producer；kbuild asm-offsets.c matched by KBUILD_FILES | excluded from kernel C list yet explicitly in asm-offset producer | other target mutation/cache not exhausted |
| BB85-M09/configuration | arch CMAKE_C_FLAGS；subdirectory CFLAGS appended after root target declared | do not infer kernel inherited these arch flags | actual scoped variable resolution and compiler command |
| BB85-M10/POST_BUILD source extraction | copy none；root_dir.glob returns empty list [] | IndexError before shutil.copy | actual filesystem glob not observed |
| BB85-M11/POST_BUILD source extraction | copy one；glob returns one matching file [f] | first matched .s copied to specified output | output path write and subsequent parser not tested |
| BB85-M12/POST_BUILD source extraction | copy several；glob yields [f1,f2] | f1 selected; source lacks explicit sorting | glob ordering and provenance not certified |
| BB85-M13/offset parsing | parser no markers；line has no double-quoted ->marker | line contributes no #define | assembly output not seen |
| BB85-M14/offset parsing | parser empty marker；input_line contains quoted -> with zero payload | blank tab line, no #define | real compiler output not seen |
| BB85-M15/offset parsing | parser single token；input_line contains quoted ->NAME, no delimiter | fields[1] access raises IndexError before output open | unobserved input shape; no execution |
| BB85-M16/offset parsing | parser double spaces；quoted ->NAME  $552 | define not emitted; tab blank line contributed | actual emitted assembly format unknown |
| BB85-M17/offset parsing | parser canonical marker；quoted ->TASK_threadsp $552 | candidate #define TASK_threadsp 552 after dollar prefix stripping | not proof current producer ABI equals old header number |
| BB85-M18/incremental build | stale header；tracked header exists, target or POST_BUILD not actually run | header visible does not prove fresh producer run | actual build graph/cache and current ABI unknown |
| BB85-M19/POST_BUILD | postbuild failed；rm step, copy or parser fails or generator treats && differently | no legitimate claim of updated consistent header | generator shell semantics, partial mutation and error propagation |
| BB85-M20/producer compile | save temps filename divergence；compiler produces no matching asm-offsets.c.s under expected glob | copy script cannot use assumed basename; no proven header | no real compiler invocation/naming evidence |
| BB85-M21/link definition | link script selection；kernel target and ARCH resolve to expected kernel.lds | candidate linker-script consumption | actual link command/e_entry/segments unknown |
| BB85-M22/link definition | orphan .ref.data；initial_code in .ref.data, script not explicitly matching it | placement and retention unknown; not a confirmed deletion | toolchain orphan handling, GC, actual references |
| BB85-M23/link definition | init_top_pgt BSS；uninitialized C global defined; script has .bss* COMMON input rule | BSS candidate only, clear_bss target coverage not certified | actual compiler object layout and final map |
| BB85-M24/artifact delivery | install not invoked；install(TARGETS kernel DESTINATION boot) text only | boot installation not established | installation prefix/toolchain and filesystem state |
| BB85-M25/link-versus-coverage | CONFIG_RANDOMIZE_BASE；macro enabled gives1GiB; disabled gives512MiB | source-level capacity depends on configuration | actual macro definition and ELF mapping unconfirmed |
| BB85-M26/link bound | 1GiB assert vs 512MiB PMD；_text>=base+16MiB, _end<=base+1GiB, 512MiB config | global ASSERT does not independently guarantee 512MiB static highmap; model 512/513 outside link range | actual ELF map and build config not checked |

## 结论与证据边界

第85批新证据为119条固定time真实短锚，九组77个唯一矩阵行及26个单独条件案例；所有原始短锚均通过本轮固定time重读的逐字比较。第84批的旧49/49/0、九组56行与28例模型均保留其原作者身份和当前RETURN结论，不算85新增证据或被本批回溯修正。五项新候选BB85-F01..F05、八项BB85-U01..U08单独登记于deferred-findings.yaml，旧ER84-F02/F03/F05及相关U只增加来源边界，不按新独立根因重复统计。

全程只读CMake文件、Python脚本文本和源码；没有运行任何仓库程序或脚本，也未配置、执行execute_process、gcc查询、rm、预处理、汇编、编译、链接、Graphviz、QEMU、实机或外部API。所述磁盘头文件存在仅是time版本中的源码事实，真实构建和ABI新鲜度仍未知。完整本地P9未运行。R84-01状态CORRECTION_SUBMITTED_PENDING_C06_REVIEW、85主线验收状态PENDING，最高级别须由C06 Pro独立审查并保留本地门；本执行者无权签84闭合或85通过。发布后仅在PR17交付一次正式评论并STOP_WRITING，不启动86。
