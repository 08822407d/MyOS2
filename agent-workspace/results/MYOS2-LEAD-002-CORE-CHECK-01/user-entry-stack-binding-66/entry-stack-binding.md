---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ENTRY-STACK-BINDING-66
record_type: bounded_entry_stack_analysis_and_R65_documentation_disposition
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-08
produced_by: GPT-6 (self-reported, not backend attested)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-10
source_pin_short12: a039d9803ade
execution_base_short12: 4b7b0b9642c3
base_snapshot: time
read_channel: connector
D00: documentation_disposition_submitted
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第66批：内核栈地址、TSS/perCPU发布与入口消费

本报告仅以固定 `time@a039d9803ade` 的源码文字建立条件性的结构关系。以下“写入”只指所读代码中存在的 store/call 语句，不代表本次运行了该代码。编译、链接、CPU实际切栈、TR/IDTR状态、硬件异常帧、GS、实机与QEMU均NOT_RUN。

## D00｜R65-01 的三组追加纠正

**甲：13项定义边界。** 原65的 `evidence.md` A01/A16/A17/A18/A20/A21/A22/A23/B11/B12/B13/B14/C04 共13个标签需要在**新**evidence逐旧号映射为单独宏、单独结构/对象定义或函数体内真正片段，详见 `facts.prior_return_disposition.R65_01_old_anchor_mapping` 的 `D01–D23`。A01拆idt_table和idt_descr；A16删除load_idt之前的注释；A17、A18、A20、A21、B12、B13分别拆独立宏；A22只取FIRST_EXTERNAL_VECTOR定义；A23从真正的SYM_CODE_START(idt_handler_array)起，前置.align只列上下文；B11仅取reg_t typedef；B14的OFFSET调用实际属于asm-offsets_64.c::main；C04仅取in_atomic三行宏。未改变真实源码文字及本来正确的常量值，定义外注释不冒充定义的一部分。所有原65旧引文、评论与对象保持冻结，不倒改。

**乙：命题与索引。** 旧65 `facts.idt_binding_matrix` 的gate_data曾指向原A15/A16（实际写表及load），gate_pack指原A17/A18（实际常量），table_write指原A19（mask），load_request指原A20（trap宏），stubs列A27/A30而原65的A最多只有A26。这些错位**独立于13条定义问题**，不能只替换成碰巧存在的任意ID。新表 `R65_01_ref_matrix_overlay` 逐命题重新建好：gate_data由真正init_idt_data源片D24（含present位）支撑；gate_pack由真正idt_init_desc源片D25（含offset_low）支撑；table_write由write_idt_entry的D26支撑；load_request由load_idt的D03支撑；stubs由D04/D05/D07/D13/D27/D28支撑，完整考虑独立mask和源码汇编标签。旧有效原65 A11/A12/A13/A14/A15/A24–A26仅保留为历史限定，不作为本次自动重新认证。原65不存在的A27/A30在本次记录为`DO_NOT_EXIST`，不声称漏传。原65正文A15–A20、A23–A25及A26–A30的错位引用全部改为按上述命题与新证据解释，而非复制旧编号。65原DOT中common→push的CALL实为宏展开/局部控制，而idt→gate→write是聚合摘要，不能冒充直接函数调用；旧图的省略条件/RETURN亦不自动填实。

**丙：清单元数据。** 原65 MANIFEST缺 `read_channel`、`startup_selfcheck_quote`、`branch_canary_quotes` 三个显式字段，尽管它叙述了曾读取规则与金丝雀；这三项不能靠叙述代替。新66的MANIFEST以本次GitHub实际回读值给出三字段和完整公约原句。原65的produced_by、model_per_owner与self_check并不缺，不制造不存在的错误。原65作者63/63是标签数，不再用来证明原63个定义/编号全合格；其余50条旧锚按`retained_unrechecked`而非“本轮通过”处理。原65正式评论#6743139361中长SHA是冻结历史，不编辑也不把它冒充本批六件P1命中；新正式评论只写实得短SHA。D00状态仅为`documentation_disposition_submitted`，R65-01关闭权仍属MYOS2-A-C05。

第64批R64-01已由C05独立闭合为`CLOSED_DOCUMENTATION_ONLY`、组合包`PASS_PENDING_LOCAL`，本次不撤销；第63和58批空行指控已撤回，不重开。

## G01／A｜地址公式、声明与实际stack来源

固定 `switch_to_macro_arch.h` 的三个**互相独立**的宏为 `TOP_OF_INIT_STACK`、`task_pt_regs` 和 `task_top_of_stack`（新A01–A03）。`task_stack_page(task)` 直接返回 `task->stack`（A04）；`task_struct.stack` 为void*（A10），`init_task` 初值为 `init_stack`（A11）。新任务的所读路径 `alloc_thread_stack_node` 请求 `kmalloc(THREAD_SIZE,GFP_KERNEL)`，失败返回-ENOMEM，成功时将指针写入 `tsk->stack`（A12/A13），`dup_task_struct` 确实调用该方法（A14）。这不等同保证后续分配体积、对象身份/对齐、生命周期和所有fork/exit完成。

源常量 `PAGE_SHIFT=12`、`PAGE_SIZE=1<<12=4096`、`THREAD_SIZE_ORDER=3`、`THREAD_SIZE=4096<<3=32768`、`TOP_OF_KERNEL_STACK_PADDING=0`（A05–A09）。在内存地址合法且运算定义良好的假设下，若base=task_stack_page(task)，则 `task_pt_regs` 的整数投影是 `base+32768-176=base+32592`，`task_top_of_stack` 是 `base+32768`。176来自仓库**现存**generated/asm-offsets.h的PTREGS_SIZE（A17），`pt_regs_s` 的尾帧声明见A18；实际sizeof、offsetof没有重新生成或编译。init专用宏使用 `(unsigned long)&init_stack + sizeof(init_stack)-padding`，并有`extern char init_stack[THREAD_SIZE]`；所读linker script将`init_stack = .`放在.data预留区域。没有运行链接器，不能把linker符号当作已经证实存在的运行时数组对象或保证所得空间已映射。

尤其 `task_pt_regs` 先把指针转为 **`loff_t`**（A15），该类型经`__kernel_loff_t`为`long long`（A16），而后加减再转回`pt_regs_s *`并执行`-1`。对超出有符号可表示范围的内核虚拟地址，转换行为依赖实现；有符号加法不可溢出，指针减一必须在有效、对齐和大小足够的对象范围内。有限示例只对小的合法抽象base作整数演算；绝不凭数值模型证明高半区真实任务栈都可安全定位。A的完整source-ledger、符号/地址矩阵和反例前提见facts。

## G02／B｜TSS初始化、描述符与任务切换发布顺序

`percpu_area.c` 静态定义了独立的perCPU `current_task` 指针和 `pcpu_hot` 聚合体：后者初始化 `current_task=&init_task`、preempt计数与 `top_of_stack=TOP_OF_INIT_STACK`（B01/B02）。另有 `cpu_tss_rw` 的TSS嵌入对象（B03/B04）；其 `x86_tss.sp0` 初值为意图性poison表达式，`io_bitmap_base`为invalid配置；而 `x86_hw_tss` 定义还独立包含 `sp1` 与 `sp2`（B05–B08）。**TSS的sp0和sp2不是同一槽**，也不是pcpu_hot.top_of_stack。

`common.c::cpu_init_exception_handling` 从当前CPU取 `per_cpu(cpu_tss_rw,cpu).x86_tss` 的地址，调用 `set_tss_desc`、`load_TR_desc`，随后请求当前IDT装载（B09–B11）。所读 `desc_arch.h` 直接实现层依次由 `get_cpu_gdt_rw(cpu)` 取得GDT、`set_tssldt_descriptor` 分拆TSS对象地址与limit/present、`write_gdt_entry` 按DESC_TSS大小复制描述符到GDT，`load_TR_desc` 先 `load_direct_gdt(cpu)` 再产生`ltr`指令请求（B16–B26）。这是源码意图与直接定义，不能直接认证CPU TR实际装载或描述符二进制格式。`cpu_init` 是另一独立定义，先调用 `syscall_init`、末尾装载direct GDT，但其旧`load_sp0` trampoline调用在注释里（B12–B15）。未独立追踪两初始化函数的真实调用顺序，也未证明硬件访问了何种栈。

在 `process64_arch.c::__switch_to` 中，须先满足`save_fsgs(prev_p)`、DS/ES和`x86_fsgsbase_load`等前置工作成功返回；此后按文本依次赋值：
`*this_cpu_ptr(&pcpu_hot.current_task)=next_p`，
`*this_cpu_ptr(&pcpu_hot.top_of_stack)=task_top_of_stack(next_p)`，
`update_task_stack(next_p)`（B30–B32）。
后者真正调用 `load_sp0(task_top_of_stack(task))`（B28/B29），其直接定义对本CPU `cpu_tss_rw.x86_tss.sp0` 写入所给地址（B27）。当前CPU与next_p是各个写入所用变量，不能因为目标数值同为task_top_of_stack就宣称数据对象相同。三个不同store的先后不证明发布具有原子性，也不证明异步异常可在所有中间点安全运行；未观察到实际并发错误。B33的pcpu_hot结构体成员说明热点变量的归属，但没有完成全perCPU/GS机制审计。init sp0的poison到后继有效sp0的**启动先后链**尚未在本包闭合，是条件缺口SB66-F01/U04，不是实际首入栈错误报告。

## G03／C｜软件切换帧、SYSCALL与硬件异常的不同层级

`switch_to(prev,next,last)` 宏调用真实 `__switch_to_asm` 并保存返回的last（C01）。汇编入口先push 6个callee-saved寄存器（rbp、rbx、r12、r13、r14、r15），在这一保存点将当前RSP写入prev->thread.sp，随后从next->thread.sp读回新的RSP（C04–C07、C10）。恢复则pop相反顺序的6个寄存器，`jmp __switch_to`而非当前源级已见的call（C08/C09）。`thread_struct.sp`是独立保存字段（C12），`TASK_threadsp=552`是仓库现存生成偏移常量（C10），生成源写于`asm-offsets.c::common`（C11），没有运行偏移生成器。新任务的`next->thread.sp`若不指向与六个保存寄存器+返回地址匹配的`inactive_task_frame`，就不能推断合法恢复；该结构记录了6寄存器和`ret_addr`（C02/C03），后者在六次pop后才进入返回角色。再跳到`__switch_to`的C体，待其正常return `prev_p`（C22）才能建立普通已切出任务的resume模型。未读新fork构造帧的全部生产者，故首次新任务切入与首次用户态进入单列为needs_evidence，不能被旧任务resume路径代替。

`entry_SYSCALL_64` 的限定**软件**入口，在`swapgs`后将原**用户RSP**存入 `PER_CPU_VAR(cpu_tss_rw+TSS_sp2)`，接着把 `PER_CPU_VAR(pcpu_hot+X86_top_of_stack)` 载入RSP，之后自行push用户返回的SS/SP/RFLAGS/CS/IP，以及orig_ax、0x80 vector，再PUSH_REGS（C13–C16）。生成头确有TSS_sp2=20及X86_top_of_stack=24两个不同偏移（C19/C20）。因此所读软件SYSCALL前缀的切栈值来自pcpu_hot.top_of_stack，**不是直接从TSS.sp0读取**；TSS.sp2在这里只是保存原用户RSP的scratch。全部仍需有效GS、perCPU基址、栈映射与ABI前提，未执行指令。

相比之下，`sa_entintr_retp` 的已读范围仅从已经存在的RSP上执行cli/PUSH_REGS、取CS及向C分发（C17/C18），**没有该范围内的软件选栈语句**。硬件异常何时、根据TSS.sp0或IST怎样选择RSP，以及实际x86特权转换何时发生，此次没有已核官方硬件规范或真实CPU验证；不可将SYSCALL软件切栈机制直接套在#PF/#GP硬件入口，也不能因为源码里出现TSS.sp0就声称真实异常已安全入栈。原65对于可能schedule/原帧稳定性的限定仍有效，旧03 integration.md §1 C5仅作历史调度边界导航，不重用其DV/GAP作为新根因。

## 自建有限模型和停止门

新facts含14项纯整数/布尔投影：4个给定合法低地址base的栈顶/PTREGS算式、3个保存/恢复thread.sp抽象场景、3个初始/更新sp0阶段假设以及4个旧65条件schedule门。它们都**没有运行任何仓库程序**，只用于自检静态数学或未认证状态边界。技术新增4项SB66-F01–F04均为条件风险，8项SB66-U01–U08为证据缺口；未宣称已复现内核故障。历史EE65、UR64、UC63保留原ID及原状态，不重复计数。图中宏展开与CALL、数据依赖、source selection、返回门和ANALYSIS_STOP（分析证据停止，不是CPU停止）分开；并包含对65聚合CALL误读的补充说明，不是完整CFG。

本包只交 `D00=documentation_disposition_submitted`、`A/B/C=completed_in_scope`、`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`。完整本地P9、编译/预处理/链接、Graphviz、CPU/GDT/TR/TSS、真实fork/syscall/异常、QEMU/实机全部NOT_RUN；主线C05独立审核R65-01及本包A/B/C。旧六件、旧评论、PR元数据冻结，完成评论后STOP_WRITING，不自签66通过或启动67。
