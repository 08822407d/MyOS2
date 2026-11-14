---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-TASK-STORAGE-LIFETIME-74
record_type: bounded_task_object_stack_and_reference_lifetime_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-09
executor_display_name: MYOS2-A-C05c 代码分析执行
produced_by: GPT-6 (model self-reported; backend not attested)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-12
source_pin_short12: a039d9803ade
contract_pin_short12: d46bab628653
read_channel: connector
D00: documentation_disposition_submitted
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第74批｜任务结构、栈空间与外部持有者的有界静态证据

**结论优先：** 固定`time@a039d9803ade`里，新任务结构与栈是分开申请的；`dup_task_struct`先按值复制当前task，再申请新栈，随后才重置`usage=1`。失败清理的`put_task_stack`不检查专门的栈引用计数，直接按`TASK_DEAD`状态决定是否尝试`kfree(tsk->stack)`；销毁task结构又是另一独立请求。PID helper的引用计数针对`pid_s`，不能证明给所取到的`task_s*`加了`usage`持有。正常退出的完成回收链仍未由选定活动源证明。

本批是阶段2的静态分析，**不是**内核已运行、内存无泄漏或有双重释放的证明。P9完整本地检查、编译/预处理、Graphviz、仓库脚本、旧夹具、QEMU及真实并发均NOT_RUN。讨论对象需有效，计数需可表示且中途无未知写者，先行函数需正常返回；碰到无效指针、未初始化值或可能不返回的callee均先记`ANALYSIS_STOP`，不能向后认证机器行为。

## D00｜R73-01的追加文档纠正（不改73原件）

本次必须优先解决主线对第73批的唯一`RETURN_DOCUMENTATION`（R73-01）；旧73的A/B/C技术主干按R73-S01/S02/S03保留，不重做73，也不追认其所有证据通过。

| 73旧锚 | 74替代锚 | 此次实际纠正 |
|---|---|---|
| A16 → 错标`task_types.h::task_s` | **D01 → `task_types.h::task_struct`** | `exit_state/exit_code/exit_signal`三成员在真正struct定义内部 |
| B13 → 错标`task_types.h::task_s` | **D02 → `task_types.h::task_struct`** | `real_parent/parent`成员归属与原语句保持 |
| B14 → 错标`task_types.h::task_s` | **D03 → `task_types.h::task_struct`** | `children/sibling`成员归属与原语句保持 |

`D04`把`sched_type_declaration.h::task_s`中的`typedef struct task_struct task_s;`独立锚定为**类型别名**，不能倒用来替代struct成员所在定义。

原73 MANIFEST已经有两条正确的金丝雀物理文本，却**缺少**`branch_canary_quotes`结构化字段。74 MANIFEST现在单设该字段：options_flags.cmake的time行含`-m64 -mcmodel=kernel -fno-pie -fno-pic`；panic.c的time行使用`this_cpu = smp_processor_id();`，master对应`raw_smp_processor_id()`且第一行不命中。旧73的`read_channel`、`startup_selfcheck_quote`早已存在，均不误记为缺失；不是“旧73从未检查分支”。

旧73 facts 的`exit_entry_status_matrix[sys_exit_group].operation`有错误措辞`then do_exit(current)`。正确的替代消费是 **`do_group_exit((error_code & 0xff) << 8) → do_exit(exit_code)`**（D05），整数实参是`exit_code`；`do_exit`内部使用当前任务对象不等于`current`是它的传入实参。原73正文的此链本来正确，因此这是facts文字纠错，非内核BUG。

**传播边界：** 原73的56是标签数，三处`task_s`错误违反定义归属门；旧73 MANIFEST/facts中“56/56均满足定义”及评论6810987102的同类总括均不再作为整体P2合格证据。此次D01–D05是**新增74证据**；未重新审查原73其它53段的定义身份，不可追认它们全部PASS、也不应倒数成53条已确认正确。旧六件、旧评论及PR元数据不改；纠正状态**`documentation_disposition_submitted`**，R73-01是否关闭只能由C05独立裁定。73的R73-S01（退出码/未初始化参数）、S02（namespace/链表/持有）、S03（BUG之后还有后备循环与finish条件）继续限制本74相关推理，58/63旧空行争议不重开。

## G01 / A｜任务对象申请、完整赋值与初始化的实际顺序

`fork.c::alloc_task_struct_node(int node)`实际只调用`kzalloc(sizeof(task_s),GFP_KERNEL)`返回task对象指针（A01）；传入的node没有在该函数体参与NUMA分配策略。独立的`free_task_struct`调用`kfree(tsk)`（A02）。函数名不是引用管理或物理回收的证明。

`dup_task_struct(orig,node)`先声明尚未赋值的`task_s *tsk`。真实源中`if (node == NUMA_NO_NODE)`之后被注释的一行使`tsk=alloc_task_struct_node(node)`成为if的唯一活动语句（A03–A05）。因此：当`node != NUMA_NO_NODE`时没有可证的tsk赋值，随后`if(!tsk)`读取未初始化局部，首先停止推演。这就是原`CORE-USER-VFS-BASELINE-05::UV-16`，不另造相同根因。已读当前实际`kernel_clone`传`NUMA_NO_NODE`调用`copy_process`（A15）；它支持这条调用者的匹配情形，不是所有未来入口都只能匹配的全仓认证。匹配node、正常kzalloc返回NULL的情形则从dup直接返回NULL；`copy_process`检测后直接`goto fork_out`，不会假装已有一个可供`bad_fork_free`处理的task（A13）。

对于合法的非NULL`tsk`与`orig`，实际执行`INIT_LIST_S(&tsk->tasks)`**在前**，`*tsk=*orig`**在后**（A06/A07）。这条整结构赋值会覆盖之前新设的`tasks`前后节点指针，也会暂时复制原task的`stack`、`usage`等字段值；**指针值拷贝本身不增加任何task、栈或其它对象的持有引用**。原`init_task`明确有`tasks=LIST_INIT(init_task.tasks)`、`stack=init_stack`、`usage=REFCOUNT_INIT(2)`（A18/A19），故应区别“源对象的静态初值”“子task暂时复制的字段”和“随后重新分配/重设的字段”。并不能从`INIT_LIST_S`曾调用就断言它的结果在结构复制后仍有效。

从这时起，`dup_task_struct`实际请求`alloc_thread_stack_node(tsk,node)`并在非零时跳`free_tsk`（A08）。若新栈申请失败，`alloc_thread_stack_node`未到达`tsk->stack=stack`，故`tsk.stack`仍是之前复制的旧指针值；本地`free_tsk`仅调用`free_task_struct(tsk)`，没有在此以继承的栈指针调用put（A12/B02）。不能说“已申请新栈后失败必丢了新栈”，也不能为了叙事补出新栈释放请求。

只有申请新栈成功、所有先行调用定义良好时，才看到`tsk.stack`被新`kmalloc`结果覆盖（B02/B03），后面`refcount_set(&tsk->usage,1)`并写`tsk->worker_private=NULL`（A09/A10），最后`return tsk`（A11）。`copy_process`更晚初始化`p->children`与`p->sibling`（A14），**不是**对`p->tasks`在dup中覆写问题的再次明确初始化；正常成功才`return p`，被`kernel_clone`取得并继续请求PID和唤醒（A21/C10）。

`task_s`只是typedef；真正持有`void* stack`、`refcount_t usage`、`List_s tasks`与`__state`成员的是`task_types.h::task_struct`（A16/A17/A24/C23）。`usage=1`只是本体写入的局部计数，不证明系统所有借用者数量、最后引用或正确的竞态生命周期。新增条件候选`TSL74-F01`仅指出`tasks`早期初始化被覆盖；未证明在后续`list_header_add_to_tail`重写节点之前存在会读到错误链的有效消费者，因此不是实测列表损坏。

## G02 / B｜独立内核栈的取得、状态检查与释放请求

固定宏`PAGE_SHIFT=12`、`PAGE_SIZE=(1UL << PAGE_SHIFT)`与`THREAD_SIZE_ORDER=3`推得所选模型`THREAD_SIZE=32768`（B12；R66-S01条件受限），不是通过预处理器或sizeof的实际编译结果。`alloc_thread_stack_node`请求`kmalloc(THREAD_SIZE,GFP_KERNEL)`；NULL时返回`-ENOMEM`，**不先写tsk.stack**；非NULL才赋`tsk->stack=stack`并返回0（B01–B03）。此处内核栈空间与`task_s`结构是两个不同分配/释放接口对象，不把内部`kmalloc/kfree`行为当成已审过的物理页/SLAB证明。

`task_struct`中的`stack_refcount`成员本身在这个固定文件中被注释（B16），`dup_task_struct`里`refcount_set(&tsk->stack_refcount,1)`也是注释；`put_task_stack`中的`if(refcount_dec_and_test(&tsk->stack_refcount))`同样为注释（B06/B21）。因此从**当前活动体**看，该helper在收到tsk后直接调用`release_task_stack(tsk)`，并非通过该字段证明这是最后持有者。它与`task.usage`计数完全不同，不能拿usage=1补上栈引用计数门。

`release_task_stack`先计算`WARN_ON(READ_ONCE(tsk->__state) != TASK_DEAD)`，条件为真即返回（B04）。固定`TASK_DEAD`是`0x80`（B13），`READ_ONCE`宏在所读固定实现中是受类型检查的单次读取（B14）；其被选定的`WARN_ON`活动宏为返回条件表达式（B15），不能凭WARN名字宣称日志输出或运行告警。**tsk必须是仍然有效的task指针**，否则先在读状态成员处停止；符合`TASK_DEAD`不表示已无人使用该栈、引用已归零、CPU已不在栈上运行。

条件允许时，源码先请求`kfree(tsk->stack)`，然后设`tsk->stack=NULL`（B05/B20）。这里的NULL写入只在前述调用有效且返回后才可认定。不能凭直觉认定`kfree(NULL)`必然无副作用（没有本批直接审阅对应定义），也不能从一次kfree请求断言发生物理释放或有双重释放。旧`init_task.stack=init_stack`是静态来源（B17），与申请成功的新`kmalloc`栈有不同的可释放性前提，不能把所有task的stack统一视为可交给kfree的分配对象。虽然`task_stack_page(task)`源级直接返回`task->stack`（B18），该辅助并不取得栈引用或延长存活时间。

`free_task(tsk)`本身（B07/B22）在选定活动体内只调用`free_task_struct(tsk)`；`stack_refcount`相关断言也是注释（B08），它不会自行再调用put_task_stack。反过来`put_task_stack`释放栈不等于也销毁task。只有调用者先后确实进入两条路径、其各自对象与时序都有效，才能讨论“两者请求了归还”；不能由其中任一名称推断另一者也已经完成。

## G03 / C｜task借用、PID引用和实际销毁调用者

`pid.c::find_get_task_by_vpid`确实从`find_task_by_vpid`得到`task_s*`并返回它（C01/C03），但`get_task_struct(task)`在当前函数中被注释（C02）。`get_pid_task`同样通过`pid_task(pid,type)`取得task，增加task引用的`get_task_struct(result)`也为注释（C04/C05）；`pid_task`依PID hlist拿到的是task指针，不能用函数名`get`当作独立持有已成立（C09）。要在返回后解引用task，必须另外证明PID/哈希对象、task、同步与最后持有者的生命周期；本批没有完整取证所有写者或所有引用者。

另一路`get_task_pid(task,type)`实际以`get_pid(...)`取得`pid_s*`（C06）；`pid.h::get_pid`在pid非NULL时增加的是`pid->count`，不是`task.usage`（C07）；`put_pid`在同一PID对象上做条件减量，最后计数为零时才请求PID cache释放（C08）。在全部对象合法、计数可表示、无中途其它操作的有限模型中，`kernel_clone`的`get_task_pid`和随后`put_pid`可形成同一PID引用的`n→n+1→n`（C10/C11），**但task持有并未因此增加**。`CLONE_VFORK`路径原本想用`get_task_struct(p)`取得task引用，此语句同样注释（C12）。这些观察并不等于全仓没有任何task持有接口；搜索工具默认分支定位，只能陈述所读函数与选定调用点。

新task的失败出口与正常成功必须分开：若`dup_task_struct`自身返回NULL，`copy_process`直接走`fork_out`；若更晚步骤失败，只有在实际进入`bad_fork_free`标签且早期fs/files/sighand等清理都有效正常返回，才按活动源码执行`WRITE_ONCE(p->__state,TASK_DEAD)`→`put_task_stack(p)`→`delayed_free_task(p)`（B10/C13）。`delayed_free_task`的实际体直接`free_task(tsk)`，`free_task`再`free_task_struct`，最后`kfree(tsk)`（B11/C22/B07/B09）。**delayed这个名称没有实现延时队列或RCU等待**；一旦它真的走到该函数，仍要证明被释放task已无读者，以及先前归还栈时对象来源可靠。旧R44-S01对失败标签最早有效前缀和UV-14潜在先行失败均继续限制，不把多级标签相连直接认证为事务式成功回滚。

如果`copy_process`成功`return p`（A21/C14），就不会自然落入本函数后面的错误标签；不能拿失败路径的释放函数证明**正常退出**会自动调用相同销毁过程。正常`do_exit`在完成其它源级请求后才可能`do_task_dead`（C20），其中置`TASK_DEAD`和`__schedule(SM_NONE)`请求是真实文本（C17/C18），但是第73批已说明`group_dead`未初始化参数等更早可证性停止点、BUG之后的后备循环及重选边界。若某次调度切换，`context_switch`里的`return finish_task_switch(prev)`必须实际恢复到相应C路径（C21/R15-S02），不能当成每次switch同步必达。更关键的是当前所读`finish_task_switch`中`put_task_stack(prev)`和`put_task_struct_rcu_user(prev)`都在注释内（C15/C16），`schedule_tail`的同名调用也不补出。因此**正常退出路径只证明写状态与发起调度请求，不证明已有task/stack析构回路完成**；旧`CORE-USER-VFS-BASELINE-05::UV-15`保留原qualified_id及`deferred_owner_not_ready`状态，避免把历史缺口再命名一个新BUG。

| 路径 | 可见活动调用或赋值 | 注释/未落实的环节 | 外部需要的证明 |
|---|---|---|---|
| 新task创建 | kzalloc task、*tsk=*orig、kmalloc stack、usage=1 | stack_refcount初始化 | task/栈来源有效、初始化前后安全、后续持有者 |
| 早期创建失败 | NULL直接fork_out或free_tsk中的task kfree | 并非所有失败都已得栈 | 返回码及先行错误/失效界限 |
| 晚期创建失败 | state DEAD→put stack→delayed_free→free task | 一些清理标签callee及stack引用计数门缺失 | 先前资源清理正常返回、没有其它栈/任务读者 |
| 普通PID查询 | pid.count增加/归还；裸task查询 | get_task_struct调用注释 | task.usage实际所有者与同步 |
| 正常退出 | do_task_dead设置DEAD并调度 | finish中的put stack/put task注释 | 真正已切出、不再重选、最终引用与物理回收 |

## 证据范围、延期与最终门

本批五份机器可读矩阵见`facts-and-boundaries.yaml`：`task_allocation_matrix`、`task_reference_matrix`、`stack_lifetime_matrix`、`destroy_caller_matrix`、`earliest_stop_matrix`，另含`prior_return_disposition`、14份固定源码台账、32个纯文本/整数/条件有限模型及历史qualified_id。图`task-storage-lifetime-paths.dot`有50个显式节点、52条分型边、0悬空端点，`ANALYSIS_STOP`不是CPU控制流或内核异常。

`evidence.md`新增D5+A24+B22+C23，共**74段**P2固定源码单定义短锚，二次直接GitHub回读74/74字面匹配（连同正确的定义归属已在首次定位窗口检查）；原73的56条未经此批全量重新审定。新增一项条件候选`TSL74-F01`（tasks早期初始化被整结构复制覆盖的使用时点问题）和8个证据缺口`TSL74-U01–U08`，均未运行复现。历史UV-15/16、PID11、R44、R15、R66及R73-S01/S02/S03均保持现有状态。

执行方仅报`D00=documentation_disposition_submitted`、A/B/C=`completed_in_scope`、`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`，不自关R73-01，不自行签74通过，不启动75；由原MYOS2-A-C05对新六件及追加纠正独立验收。所有源代码、旧文件、PR元数据未获修改授权，也没有补丁、运行或阶段3工作。
