---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-FIRST-RUN-FRAME-67
record_type: bounded_first_run_frame_analysis_with_R66_documentary_disposition
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-08
produced_by: GPT-6 (model self-reported, not backend-attested)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
execution_base_short12: 6cffc97057c7
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00_status: documentation_disposition_submitted_pending_C05
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
runtime_evidence: NOT_RUN
---

# 第67批：首帧生产、首次任务切入、函数与用户返回分流

本文只认固定 `time@a039d9803ade` 源文和已声明前提下的整数/分支模型。新 `evidence.md` 的D1、A33、B25、C23合计82条是直接源码短锚；每条均含有意义的非空文本、真实单一定义和1–5连续物理行。完整本地P9、预处理、编译、链接、偏移重生成、真实调度/栈/CPU/SYSRET及实机/QEMU全部NOT_RUN。以下“首次”只表示所读 `copy_thread` 构造的首帧在所选切换路径上的条件消费，不等于首次CPL3运行已经发生。

## D00 — R66-01追加纠正（不重开65/66）

原66 `user-entry-stack-binding-66/evidence.md::A10` 的标签声称证明 `task_struct.stack`，但代码围栏只有**单个空白物理行**，因此其1–5行及字面命中不能提供成员声明的命题支持。这是P2非空证据与定义归属/命题支持的问题，不是源码没有该字段、文件没下载完，也不涉及63/58已撤回的空行指控。

新67的 **D01** 独立回读固定 `mykernel/sched/task/task_types.h::task_struct` 内真实 `void *stack` 成员单行，作为旧66::A10的替代消费入口。66正文A节“task_struct.stack为void*（A10）”改按新D01理解，同时和旧66::A04 `task_stack_page` 及旧66::A11 `init_task` 的限定证据合读。66 facts.stack_address_matrix中task.stack行的旧A10支撑改按新D01；旧66 MANIFEST.self_check、facts.p9_text_subset(item2/5/13)与正式评论#6756756742里的“101/101全部合格”只能作为**历史作者的标签计数声明**，因为至少A10无有效证据。其余100条不因本次只核A10就自动通过或自动失败，不能偷换成“只需101改100”的新自检。完整影响位置逐项载于`facts.prior_return_disposition.impacted_locations`，原六件、评论及PR元数据保持原样。

第65批R65-01由主线C05独立裁定CLOSED_DOCUMENTATION_ONLY，65组合材料PASS_PENDING_LOCAL_WITH_R65C-S01；vector/bits两条补锚已由主线办理，不在本批D00重新执行。第66批的R66-S01已收窄loff_t与unsigned long的复合运算类型，R66-S02已收窄原DOT的聚合/宏边；本轮按既有回执消费，不重做66。D00状态严格为`documentation_disposition_submitted`，R66-01仍待C05闭合，绝不由执行者自签。

## G01/A — 构造新任务首帧的对象与赋值次序

`copy_process` 中 `dup_task_struct(current,...)` 先复制任务结构并为新任务申请栈，之后将`PF_KTHREAD`先清零再依`args->kthread`条件置位，再调用 `copy_thread(p,args)` 并消费非零返回值进入清理标签（A24–A28）。这三者不是所有fork资源的完整生命期认证；某次资源分配/复制若失败，不能跳到后续首帧结果。参数的`kthread`、`stack`、`fn`与`fn_arg`在`kernel_clone_args`中分别有对应成员（A29–A31）。

`copy_thread` 首先 `childregs=task_pt_regs(p)`，然后 `fork_frame=container_of(childregs,fork_frame_s,regs)`，`frame=&fork_frame->frame`（A03/A21/A23）。`fork_frame_s` 是struct fork_frame别名，内含 `task_kframe_s frame`、`pt_regs_s regs`，前者是 `inactive_task_frame` 别名（A15–A20）。用66已经保留的有效栈范围前提、64位reg_t单槽8字节和既存PTREGS_SIZE=176做**条件性布局投影**：inactive帧7槽=56字节（r15/r14/r13/r12/bx/bp/ret_addr），regs=176字节，总232字节。没有执行 `sizeof`、`offsetof` 或验证最终编译对齐/填充，不能因为当前数学大小与源码吻合就认证实际对象布局。

若栈基址为base、THREAD_SIZE=32768、padding=0、地址及对象合法，纯数值模型给出：`childregs=base+32592`，`fork_frame=childregs-56=base+32536`，`frame.ret_addr`位于`fork_frame+48`。源码随后统一写 `frame->bp=(reg_t)childregs+1`、`frame->ret_addr=(reg_t)ret_from_fork_asm`、`p->thread.sp=(reg_t)fork_frame`（A04）。由于在`+1`之前**已转换为整数reg_t**（A33），这是模型中的数值加1，不是对`pt_regs_s*`做加一个结构体的指针运算。它对真实栈展开/帧指针用途是否合适，需要另读消费者；`FR67-F02`仅为条件语义候选，未判定内核运行错误。指针转整数、符号/无符号复合运算、存储大小、对齐和生命周期继续受R66-S01与SB66-F03约束，不能凭数学投影消除最早失败前提。

在上述共同赋值之后还有`current_save_fsgs()`与ES/DS保存（A05/A06）；这些调用若不返回或对象不合法，后继分支不能被描述为必达。根据实际source assignment，`copy_thread`分支如下：

| 分支 | childregs来源与覆盖 | frame->bx / frame->r12 | 正常本地出口 |
|---|---|---|---|
| `PF_KTHREAD` 为真 | `memset(childregs,0,sizeof(pt_regs_s))`，不从当前帧复制 | `kthread_frame_init`设为args.fn / args.fn_arg | 提前`return 0` |
| 非内核任务，`args.fn==NULL` | 复制`*current_pt_regs()`、`ax=0`；非零args.stack覆盖sp | bx设0；r12本函数未显式设定 | `return ret`，当前ret初值0 |
| 非内核任务，`args.fn!=NULL` | 先同上复制/可选覆盖sp，随后强制`sp=0`、`ip=0` | 再由`kthread_frame_init`设fn/arg | 提前`return 0` |

直接证据A07–A14、A22与C20–C23分别支持条件赋值，不把这几种情形合并为“所有新任务都清零用户帧”。`frame->r15/r14/r13`及普通无fn情形的r12在所读`copy_thread`内没有显式写入，但这不证明物理内存的实际值必为零或必为垃圾；需分配器、继承、帧消费者及真实编译证据。`CLONE_SETTLS`源片处于注释，不扩展为整套TLS保存缺失的全局断言。

## G02/B — 已造首帧怎样成为第一次C调用参数

在旧任务已经拥有保存帧的普通切换与首次新fork切入之间必须区分**ret_addr来源**：前者来自老任务被切走时的实际栈与返回路径；新fork的`copy_thread`明确合成 `ret_addr=ret_from_fork_asm` 和 `thread.sp=fork_frame`。新证据B01–B07显示 `switch_to`宏含真实`__switch_to_asm`调用，后者先在旧栈push六个callee保存寄存器，写出`prev->thread.sp`，再从`next->thread.sp`载入RSP，按相反次序弹出六个寄存器，并以`jmp __switch_to`尾跳转而非call。原66的TSS/perCPU发布前提与C实现`__switch_to`先行保存FS/GS、后续更新热点数据、最终正常`return prev_p`继续有效（B08–B13）；opaque callee不自动确保成功。

在没有额外填充、每槽8字节且新frame真实有效的模型中：frame偏移0/8/16/24/32/40依次为r15/r14/r13/r12/bx/bp，偏移48是ret_addr，偏移56开始是childregs（A15–A18、B22/B23）。因此六个pop之后，假设C的`__switch_to`按预期返回且使用此ret_addr作为返回地址，RSP随ret越过8字节槽而指向childregs，执行目标才是`ret_from_fork_asm`（B14）。这仅是有界汇编/返回模型，不是实际CPU记录。

`ret_from_fork_asm`紧接着明确执行四项参数重排及一次C调用（B15）：

| 汇编值来源 | 重排至 | C形参 | 前提 |
|---|---|---|---|
| `rax`：`__switch_to`返回的prev_p | `rdi` | prev | C返回约定成立 |
| `rsp`：指向childregs的现有栈位置 | `rsi` | regs | 合法ret_addr与56字节首帧 |
| `rbx`：首帧的bx槽 | `rdx` | fn | 之前加载及保存约定有效 |
| `r12`：首帧的r12槽 | `rcx` | fn_arg | fn不空时才消费参数 |

这些是源码中寄存器移动与C参数顺序的对应；没有外部x86-64 ABI原文核证或编译结果时，仅能按明确的系统调用/函数调用约定假设成立。不能凭C形参名称或ret_addr字段名字自动宣布首次运行已达成，也不把`schedule_tail`、GS和内核栈有效性前提跳过去。

## G03/C — C层第一次fn、用户工作与汇编第二次分流

C函数`ret_from_fork`源级顺序为：`schedule_tail(prev)`；若fn非空则`fn(fn_arg)`；若该函数返回则`regs->ax=0`；随后**无论fn是否为空都**调用`syscall_exit_to_user_mode(regs)`（C01–C04）。`schedule_tail`的`finish_task_switch(prev)`在本快照是注释，活动语句是`preempt_enable()`（C05/C06）；当前只是直接callee边界，不扩审runqueue/锁持有与任务生命周期。返回工作（C07–C11）经`__syscall_exit_to_user_mode_work`进入`exit_to_user_mode_loop`，其中`arch_do_signal_or_restart(regs)`的调用活动，围绕它的ti_work条件在所读源码是注释。09的有效组合回执仅允许把这里视为可能改写寄存器帧或不返回的条件消费者，绝不宣称所有内核任务都完成用户态返回、信号必已投递或帧字段原值必不变。

`ret_from_fork_asm`在C调用后，若控制**真正返回汇编**，则测试`%rbx`（C12）。当其为0走标签2并`jmp syscall_return_via_sysret`（C13/C14）；当非0走标签1，把r12移到rdi，然后 `callq *%rbx`（C15/C16）。如果这后一次调用正常返回，源码走`jmp .`（C17）；未观察任何真实CPU循环。该汇编后继标签只证明源级请求，并不等于CPU接受SYSRET或用户指令开始执行（C18/C19）。

这条链有一个值得独立验收的**条件调用重叠**：`ret_from_fork`已存在第一次C层`fn(fn_arg)`，而`ret_from_fork_asm`在其正常返回后还存在针对非空rbx的第二个间接`callq *%rbx`。只有当`schedule_tail`、第一次fn、用户返回工作都正常返回，帧/栈仍有效且ABI保留原RBX时，第二个调用点才沿当前源码变得可达；否则必须在第一次不返回或返回工作不返回处停止，**不得直接断言内核线程执行了两次**。对应新增`FR67-F01`，而不是重编号旧SB66-F04。

分情况消费：
- **普通用户fork，fn为空：** copy的用户帧和可选栈参数、ax=0，C跳过fn，用户返回工作若正常返回，汇编向SYSRET标签跳转；最终CPU状态仍是未知。
- **内核任务或带非空fn的特别入口：** C先尝试调用一次fn，且其后的用户返回工作无条件存在；若C整体正常返回并保留rbx，汇编还会尝试第二个fn；第二次正常返回时落入`jmp .`，而非保证转用户态。
- **内核任务fn为空（未证明实际构造）：** copy_thread把pt_regs清零且bx来自NULL；C若正常返回，汇编沿空rbx去用户跳转标签，所读局部没有主动填有效用户CS/IP/SP，不能把这个反例硬说成实际可达或已发生非法SYSRET。它是`FR67-F03`的限定条件路径。
- **返回工作或fn不返回、frame被修改：** 不越过最早callee停止点，不推测随后两个分流和真实CPU指令。

源码到真实用户运行至少分为：完成copy_thread首帧赋值、上下文切换正常进入首帧、C`ret_from_fork`及返回工作正常返回、汇编分流目标可达、CPU接受SYSRET保存状态、目标用户代码成功执行。只有前四层中已明示前提满足的部分有源码依据；最后两层、首次CPL3全局引导以及生成offset ABI都`NOT_RUN`。本批只补SB66-U06的新生产者/消费者源级缺口，不自关SB66-U04/U08或09返回/信号旧未决项。

## 方法、交付与停止

事实件含`prior_return_disposition`、`source_ledger`（16固定源码）、`frame_production_matrix`、`first_dispatch_register_matrix`、`entry_branch_call_matrix`、20个纯数值/布尔模型、`prior_findings_links`。DOT有独立宏展开、CALL、TAIL_JUMP、RETURN、DATA、LOCAL_CONTROL和ANALYSIS_STOP；不是完整硬件CFG。延期文件列3项条件候选`FR67-F01–F03`与8项`FR67-U01–U08`，零实际运行复现。P1/2/3/6/9文本子检查须另行明确，完整P9与所有编译/脚本/测试NOT_RUN。

本批只签`D00=documentation_disposition_submitted_pending_C05`、`A/B/C=completed_in_scope`（有界源分析）、`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`。不修内核、不改旧件/评论、PR元数据或其它分支，不自关R66-01、不自签第67批通过、不自行开始68，发布唯一正式PR17交付评论并完整远端回读后STOP_WRITING。
