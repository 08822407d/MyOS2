---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-ELF-STACK-TABLES-25
record_type: bounded_static_elf_stack_tables_report
date: 2026-11-02
source_pin_short12: a039d9803ade
result_base_short12: c0f4d6a603f8
executor: MYOS2-LEAD-002-CHAT-EXEC-03
status: READY_FOR_MAINLINE_REVIEW
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---
# 第25批：ELF初始栈表与auxv的实际代码边界

## T00 · 已冻结的来源与前提
- 任务与授权：44号固定任务书、45号C03c交接包；结果基线c0f4d6a603f8；内核固定time a039d9803ade；交付只面向原PR17。23/24的主线补充回执作为**必须合读**的消费边界，不修改已审原件。
- 已读固定time中`binfmt_elf.c::create_elf_tables`函数体125–311行及其真实调用、上游exec字段、ELF/auxv/mm类型、x86-64对齐和直接用户写入封装。证据见`evidence.md` ET25-A01–A62。
- R23-S01/S02/S03、R24-S01/S02/S03继续限制解释：此前参数页GUP无活动return、初始NULL迭代和页表/引用契约未闭合；故“已有参数字符串”只能是带明确前提的**局部对象模型**，不是证明整条exec已运行成功。

## T01 · 真正调用和栈表地址的不同阶段
在`do_execveat_common`中，计数后的`bprm.argc/envc`被保存，文件名、env、argv字符串先后搬到参数空间并使用`bprm.p`表示向下推进的字符串位置（ET25-A02–A04）。`setup_arg_pages`先将`mm.arg_start=bprm.p`（ET25-A01）。所选`load_elf_binary`源文本依次调用`begin_new_exec`、`setup_new_exec`、`setup_arg_pages`，经过ELF段和可选解释器处理后才调用`create_elf_tables`（ET25-A05,A06,A11）。该先后顺序不填补23/24的调用/内存有效性缺口。

主程序的`e_entry=elf_ex->e_entry+load_bias`、`phdr_addr`按符合条件的PT_LOAD段偏移求得并加装载偏移（ET25-A07,A08），传入辅助向量的`AT_ENTRY`使用**主程序**e_entry；若有解释器，最终`elf_entry`由解释器装载和它自己的e_entry决定，若无解释器，`elf_entry=e_entry`（ET25-A09,A10）。两者不能无条件画等号。`create_elf_tables`实参依次是`bprm, elf_ex, interp_load_addr, e_entry, phdr_addr`；其结果只做`retval<0`判断，之后写`mm.start_stack=bprm.p`，最后传`elf_entry,bprm.p`给`START_THREAD`（ET25-A11–A13）。

函数入口局部`mm=current->mm`、`ulong p=bprm->p`、`int argc=bprm->argc`、`int envc=bprm->envc`，其`p`不是新的字符串拷贝，也不能自动与`mm.arg_start`混同。x86`arch_align_stack`在当前活动体将p向下按16字节取整，注释的随机减法不执行；字符串`ELF_PLATFORM="x86_64"`预留7字节含NUL（ET25-A14,A16,A17,A22）。`STACK_ALLOC`对`ulong p`按**字节**减；`STACK_ADD/STACK_ROUND`分别对`elf_addr_t*`按**8字节元素**减，并按16字节取整；`elf_addr_t=Elf64_Off=__u64`（ET25-A19–A21,A45,A46）。

**局部未定义行为门（重要）：** 函数声明`vma_s *vma;`没有赋初值；唯一原本看似要赋值的`vma=find_extend_vma_locked(mm,bprm->p)`在注释中，随即活动语句`if(!vma)`读取它（ET25-A15,A35,A36）。这个C层读前未写不能判成“vma必为NULL”或“必为一个有效VMA”，也不能认为程序能确定继续写入argc。因此本报告后续关于正常构表的部分**只是给定有效对象并规避此未定义门的独立条件算术模型**，绝不表示真实运行已经跨过此门。此门之前的实际源语句仍能独立分析顺序：平台copy、saved_auxv生产/清尾、bprm.p新值写入。

### 局部栈表布局公式
令`p0`为初进函数的bprm.p，`p1=p0&~15`，`p2=p1-Lplatform-Lbase`（每个实际启用的字符串均含NUL；选定平台字节数7）；N=非终止(aux_type,aux_value)对数；`ei_index=2*(N+1)`个elf_addr_t元素，`aux_bytes=8*ei_index`；`items=(argc+1)+(envc+1)+1=argc+envc+3`个elf_addr_t；`sp0=p2-aux_bytes`，`bprm.p=(sp0-8*items)&~15`。全部假设输入尺寸非负且有定义、减法未溢出且有效。`items`涵盖argc单槽、argv计数槽+终止、envp计数槽+终止，**不**含辅助向量；`ei_index`已经包括AT_NULL终止一对。

| 低地址到高地址 | 内容 | 长度 |
|---|---|---|
| `bprm.p` | argc单个元素 | 8字节 |
| 紧接 | argv[0..argc−1]地址、NULL地址 | 8×(argc+1)字节 |
| 紧接 | envp[0..envc−1]地址、NULL地址 | 8×(envc+1)字节 |
| 紧接 | auxv N对键值及终止的AT_NULL双零 | 16×(N+1)字节 |
| auxv末端至p2 | 栈表16字节取整引入的间隙；源码未专门清零 | 0–15字节 |
| p2至p1 | 可选base/platform字符串的真实字节 | `strlen+1`之和 |
| 更高地址 | 此前已经准备的argv/env/文件名字符串区域 | 本批不搬迁、不再写一次 |

**有限纸面示例（不是实测）：** 设p0=`0x7fffffffeff8`、argc=2、envc=1、平台仅`x86_64\0`、无execfd、其他可选宏不启用。此时N=12、ei_index=26、aux_bytes=208、items=6、p1=`0x7fffffffeff0`、p2=`0x7fffffffefe9`、sp0=`0x7fffffffef19`、bprm.p=`0x7fffffffeee0`。槽位：argc@eee0，argv两个指针@eee8/eef0，argv-NULL@eef8，env指针@ef00，env-NULL@ef08，auxv@ef10..efe0，9字节对齐间隙@efe0..efe9，平台字符串@efe9..eff0。所有短尾地址省略了公共`0x7fffffff`前缀。本示例不证明执行到表写入，也不证明任何字节实际存在用户页。

## T02 · saved_auxv条目、容量与用户副本
`mm.saved_auxv`是**内核mm_struct数组**，宏`AT_VECTOR_SIZE_BASE=22`、缺省`AT_VECTOR_SIZE_ARCH=0`，`AT_VECTOR_SIZE=2*(22+0+1)=46`个ulong元素，所选x86-64宽度8字节，总共368字节（ET25-A43,A44）。`NEW_AUX_ENT`每次在内核缓冲区依次写入id和value并推进elf_info两元素（ET25-A24）。当前活动的**11个无条件条目**为AT_HWCAP、AT_PAGESZ、AT_CLKTCK、AT_PHDR、AT_PHENT、AT_PHNUM、AT_BASE、AT_FLAGS、AT_ENTRY、AT_SECURE、AT_EXECFN（ET25-A25–A28）。

| 类别 | 实际写入值与差异 |
|---|---|
| 能力、页与时钟 | AT_HWCAP取`boot_cpu_data.x86_capability[CPUID_1_EDX]`，不推测CPU内容；AT_PAGESZ取4096；所读固定time`CLOCKS_PER_SEC=USER_HZ=250`，不是默认分支资料中的其他可能值 |
| ELF与解释器 | AT_PHDR取本次`phdr_addr`，AT_PHENT取`sizeof(struct elf_phdr)`，AT_PHNUM取`exec->e_phnum`；AT_BASE取`interp_load_addr`，AT_ENTRY取主程序`e_entry` |
| 标志与字符串 | AT_FLAGS初为0，`bprm.interp_flags`具有PRESERVE_ARGV0时OR其位；AT_SECURE取`bprm.secureexec`；AT_EXECFN取此前`bprm.exec`；AT_PLATFORM取实际平台字符串的用户地址 |
| 条件项 | `bprm.have_execfd`时追加AT_EXECFD；`k_base_platform`非NULL时追加AT_BASE_PLATFORM；ELF_HWCAP2宏定义时才追加AT_HWCAP2；CONFIG_RSEQ时加两对；ARCH_DLINFO若定义可先追加 |
| 当前已读配置的空项 | `ELF_BASE_PLATFORM`缺省NULL；asm/elf.h的HWCAP2及ARCH_DLINFO宏定义为注释；源中的AT_UID/EUID/GID/EGID、AT_RANDOM及随机字节复制被注释；未实际构建/预处理，不冒称穷尽外部编译定义 |

已读组合的12对（11基础+PLATFORM）加AT_NULL一对为26个元素=208字节；若have_execfd则28元素=224字节，二者均小于46元素容量。一般条件式为`N=11+P+BP+H+F+2R+A`（P/BP/H/F/R分别为platform/base-platform/HWCAP2/EXECFD/RSEQ开关，A为额外arch对数）；只在`N<=22+AT_VECTOR_SIZE_ARCH`时证明相应容量满足。NEW_AUX_ENT无活动越界检查；未读配置组合发生改变时应另验，不能把“容量预留很大”当全配置认证（ET25-A29–A31,A44）。

`memset`从内核缓冲区当前写指针清到数组末尾，`elf_info+=2`跨过已清零的AT_NULL型和值，`ei_index`因而含终止对（ET25-A32,A33）。`STACK_ADD(p,ei_index)`预留`8*ei_index`字节，后续`copy_to_user(sp,mm->saved_auxv,ei_index*sizeof(elf_addr_t))`才尝试写入用户副本（ET25-A34,A42）。内核数组值已写、清尾、用户副本成功、用户库正确消费是四个互不等价的断言。

## T03 · 实际写入宏、边界、错误与最终交接
按源码写入次序：`put_user(argc,sp++)`；从`mm.arg_start`出发，逐个argv先写指针、再测`strnlen_user`、再推进p；写argv终止NULL，更新`mm.arg_end`；从p开始设置`mm.env_start`，逐个envp同样处理；写envp NULL，更新`mm.env_end`；最后copy auxv（ET25-A36–A42）。

**不能以“put_user”的惯例推断其保护能力。** 目前活动定义直接执行`*(ptr)=(__typeof(*(ptr)))(x)`，然后返回`-ENOERR`，且`ENOERR=0`；在有效指针的定义良好模型下`sp++`实参由赋值表达式求值一次，槽指针自增，返回0。但遇用户地址无效时没有宏内恢复/返回-EFAULT的保证，不能根据调用者`if(put_user(...))`直接宣称安全失败路径或写入成功（ET25-A54,A55）。相比之下，`copy_to_user`包装走`_inline_copy_to_user`，其活动分支执行access_ok检查并返回raw_copy未拷贝的剩余字节数；只有0才表示在该接口局部语义下全部复制完，非零被函数转换成-EFAULT且此前可能已有部分写入（ET25-A56,A57）。

`strnlen_user`当前活动定义直接`return strnlen(str,count)+1`；注释里的用户异常到0与访问探测不在这条活动体中。在`count=MAX_ARG_STRLEN>0`、可访问且结果有定义的模型中，`len>=1`，因此真实活动实现不证明“返回0”的可达性；无终止符导致`strnlen=count`时返回`count+1`，调用者`len>MAX_ARG_STRLEN`进-EINVAL。这里不认定非法指针可正常返回，更不能把未验证字符串视为实际已复制正确（ET25-A38,A40,A58,A59）。

| 前置门/返回条件 | 允许认定的前缀 | 局部后果与限制 |
|---|---|---|
| 平台copy返回非零 | p本地已扣除长度，先行其他平台拷贝可能已发生 | 提前-EFAULT；最终bprm.p还未新赋值 |
| mmap写锁获取非零 | 内核auxv写入/清尾和bprm.p赋值可能已发生 | 源文本返回-EINTR；真实锁返回来源未展开 |
| 执行`if(!vma)` | 已完成上述前缀文本顺序 | **未初始化读取，UB**；不可预测必返-EFAULT或继续写表 |
| 有效假定下`put_user` | 一次直接存储及sp递增 | 正常返回0；非法地址无可证-EFAULT兜底 |
| 有效假定下argv/envp长度越限 | 已写相应指针槽，部分边界字段可能更新 | -EINVAL；未见事务回滚 |
| 给定被调返回0长度 | 仅是假想返回值；当前体对正count及有效字符串不产生0 | 调用者源码检查为-EINVAL，生产可达性未证 |
| auxv copy返回剩余字节非零 | saved_auxv已写、部分用户表/元数据可能已写 | -EFAULT；先行写入不撤销 |
| 源码末句正常return0 | 各门需**事先有定义且通过** | 仅有效对象条件模型；本批真实vma门仍不能跨过 |

返回消费：调用者检查`retval<0`，只有通过后才继续更新`mm.start_stack=bprm.p`并调用`START_THREAD(elf_ex,regs,elf_entry,bprm.p)`；此宏展开为`start_thread`，再到`start_thread_common`，其固定源码把传来的`new_sp`赋给`regs->sp`（ET25-A11–A13,A51–A53）。这里只证明**参数流至寄存器结构字段的源代码边**，不证明CPU实际RSP、用户页安全、异常返回、外部libc或真实入口已经执行。

## T04 · 自查与待主线消化
新增延期候选为ET25-F01（vma读前未写）、ET25-F02（put_user简化语义）、ET25-F03（strnlen_user简化）、ET25-F04（辅助向量未来条件容量），均带前提、不称为已运行漏洞。ES23-F02/F03及R24-S01/S02/S03只作既有依赖，不重复编号。所有逐字源码证据与限定在`evidence.md`，结构化对象及算例在`facts-and-boundaries.yaml`，边类别在`elf-stack-tables-paths.dot`。本批无源码修改、无构建运行、无C/ASM、无第三阶段授权；状态最多是`READY_FOR_MAINLINE_REVIEW`，由C03裁定`PASS_PENDING_LOCAL`或返修，当前mainline_acceptance=PENDING。
