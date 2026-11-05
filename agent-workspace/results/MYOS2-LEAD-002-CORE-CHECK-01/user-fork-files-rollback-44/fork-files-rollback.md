---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FORK-FILES-ROLLBACK-44
record_type: bounded_fork_files_owner_failure_rollback_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
produced_by: ChatGPT普通对话（Owner声明GPT-6＋Extra；实际backend不可独立认证）
source_pin_short12: a039d9803ade
contract_short12: 2cabad52880a
result_base_short12: 9cd9e0ab4d03
date: 2026-11-05
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第44批｜fork文件表取得之后：错误标签与持有回滚的有限契约

## 1. 结论及范围

固定time@a039d9803ade的`copy_process`中，`copy_files`**正常返回非零**时走`goto bad_fork_cleanup_semundo`；它位于`bad_fork_cleanup_files`（实际调用`exit_files(p)`）之后，因而该条直接跳转**跳过本次显式文件表归还**。相反，`copy_files`正常返回0且随后`copy_fs`失败，跳到`bad_fork_cleanup_files`；更后失败经相应前置标签贯穿，只有各先行opaque清理/对象条件满足才可到达`exit_files(p)`。这正好区分“结构体内预置旧指针”与“已经取得新增文件表持有”，不是统一调用exit_files的伪回滚。直接证据FR44-A010/A011、A027/A028、A036–A040、A049/A050。

三类已知持有模型必须独立处理：旧表NULL而不增加表引用；CLONE_FILES共享时给旧表`refcount`增加1（但不逐file增引用）；非共享时`dup_fd`成功创建并发布一个`refcount=1`的新表。若`dup_fd`外层表分配正常失败，`copy_files`将错误返回给`copy_process`，此前`p->files`可能仍是从`*p=*current`拷贝的借用指针，未取得新的拥有引用；不能在这种失败路径上反向要求exit_files。若旧FT43-F01的内层`kzalloc`返回NULL而随后直接解引用，则在callee内失去有效源程序前提，**没有可供caller消费的正常错误返回**，不得伪造`goto bad_fork_cleanup_semundo`。这些均为局部静态条件，不证明实际fork运行、资源最终归还或内核整体安全。证据FR44-A001–A003、A010、A036–A045。

## 2. R01｜任务表指针与取得持有不是同一事实

`kernel_clone`实际调用`copy_process(NULL,trace,NUMA_NO_NODE,args)`。所选`dup_task_struct`只有在`node==NUMA_NO_NODE`时才执行`tsk=alloc_task_struct_node(node)`；当node不满足时局部tsk未被赋值，不能仍当作有定义良好的分配返回。该函数在有效对象/前置调用条件下执行`*tsk=*orig`，将`orig->files`的指针值复制给新task，不执行文件表计数增量。其后栈分配、copy_process中copy_files前的`set_kthread_struct`、`sched_fork`等均有先行有效性/正常完成前提，不能由函数名或指针非NULL推定所有步骤都成功。证据FR44-A001–A009、A034。

文件表取得状态：

| 取得阶段 | 可能的p.files指针值 | 本选定路径取得的新持有 | 后续失败时允许归还什么 |
|---|---|---|---|
| 尚未调用copy_files，且有效`*p=*current`已完成 | 源任务的旧表指针或NULL | 没有因为复制指针值而加refcount | 本局部路径无新增表持有，早期错误标签跳过exit_files |
| copy_files旧表NULL并正常返回0 | 预置NULL（须保持同一有效源/无中途改写） | 无 | 后续错误可调用exit_files，但其`if(files)`为假 |
| copy_files正常共享成功 | p.files仍是原表地址（来自前序拷贝） | 原表refcount +1 | 后续一次exit_files/put可在正初值与无其它写者模型中配平一次 |
| copy_files非共享成功 | p.files已写到新files_struct | 新表refcount初值1；父表不因本调用增加 | 后续归还新表最后引用，进入条件遍历/缓存free请求 |
| copy_files非共享外层分配NULL，正常返回-ENOMEM | p.files可能仍保存借用旧表地址 | 0（新表未发布） | 跳过exit_files，避免用未取得的表引用进行本次put |
| dup_fd内层NULL失效 | 无已认证的正常完成态 | 不可由source断定一个可正常rollback的持有状态 | 停于更早的无效赋值，不推出下游goto或errno |

`copy_files`自身的`oldf=current->files`、`if(!oldf)`、`if(clone_flags & CLONE_FILES)`和`dup_fd`返回检查均经43号和本次定点直接回源；但共享分支并未写`p->files`，只有有效先行`*p=*current`及身份稳定才可解释为同址。复制成功的文件对象`f_count`来自各独立file_s副本而不是文件表refcount；复制指针成员不是资源深拷贝，不重开UV-09。证据FR44-A036–A045、A051。

## 3. R02｜copy_process的活动goto、标签贯穿及最终返回

完整阅读选定`copy_process`函数，纳入文件表取得周边所有直接影响到文件归还的活动错误边（不递归深入各资源子系统）：

| 正常错误入口（只有callee正常返回错误才成立） | 源码goto目标 | 到exit_files之前的活动/opaque路径 | 本次exit_files |
|---|---|---|---|
| dup_task_struct未得到p | `fork_out` | fork_out中当前任务锁操作，再ERR_PTR | 不经过 |
| set_kthread_struct失败（在copy_files前） | `bad_fork_cleanup_delayacct` | 下部任务/栈清理与fork_out | 不经过 |
| sched_fork返回非零（在copy_files前） | `bad_fork_cleanup_policy` | 下部任务/栈清理与fork_out | 不经过 |
| copy_files正常返回非零 | `bad_fork_cleanup_semundo` | 跳过files标签，进入下部清理/最终返回 | 不经过 |
| copy_fs返回非零 | `bad_fork_cleanup_files` | 直接调用exit_files | 可直接到达 |
| copy_sighand返回非零 | `bad_fork_cleanup_fs` | exit_fs→files标签 | 需要exit_fs正常返回 |
| copy_signal返回非零 | `bad_fork_cleanup_sighand` | __cleanup_sighand→exit_fs→files | 两个opaque清理必须正常完成 |
| copy_mm返回非零 | `bad_fork_cleanup_signal` | 标签内信号清理注释→__cleanup_sighand→exit_fs→files | 同上 |
| copy_namespaces返回非零 | `bad_fork_cleanup_mm` | 条件mmput→__cleanup_sighand→exit_fs→files | 须mmput（若被调用）等正常完成 |
| copy_thread返回非零 | `bad_fork_cleanup_io` | IO与namespace注释标签→条件mmput→sighand→fs→files | 须全部先行清理正常完成，**没有先进入cleanup_thread** |
| alloc_pid返回可识别ERR_PTR | `bad_fork_cleanup_thread`（retval=PTR_ERR） | exit_thread→cleanup_io→cleanup_mm→sighand→fs→files | 须全部先行调用正常完成 |

必须注意标签自然贯穿，而不是根据`bad_fork_cleanup_...`的名称猜测调用内容。`bad_fork_cleanup_files:`实际调用`exit_files(p)`，`bad_fork_cleanup_semundo:`仅含注释并紧随其后；从后者直接跳入时不会逆行到上一个标签。反之若从cleanup_files进入、exit_files正常返回，才继续贯穿semundo和下部清理。`copy_process`正常成功直接`return p`，位于所有错误标签之前，不会自动落入错误区。证据FR44-A019–A032。

`bad_fork_cancel_cgroup`、`bad_fork_put_pidfd`及`bad_fork_free_pid`的活动标号和部分调用确在源码，但在本次已核的相关直接错误goto中没有将它们作为目标；不能将其存在当所有失败都执行了pidfd/free_pid等处理。特别是`alloc_pid`报IS_ERR时，源码直接goto`bad_fork_cleanup_thread`，并不自动跳到位于其上面的`bad_fork_free_pid`。`bad_fork_cleanup_signal`/io/namespaces内部的注释不算额外释放。已有`CORE-USER-VFS-BASELINE-05::UV-14`指出共享sighand清理的前提问题：如果`__cleanup_sighand`内部失效或未正常返回，之后`exit_fs`和`exit_files`就没有可认证的到达路径；**不能把上述表中“可到达”说成实机已经完成归还**。

成功/失败返回的`retval`由各被调资源函数赋值；本批不展开copy_fs/copy_mm等的实际错误生产者，因此不任意指定其errno。copy_files外层分配正常失败时`dup_fd`先写int输出参数`*errorp=-ENOMEM`（**不是ERR_PTR**），copy_files将其作为int返回，copy_process随后经过正常清理才在fork_out执行`ERR_PTR(retval)`。kernel_clone调用完copy_process后用`IS_ERR(p)`判断，在真分支才`return PTR_ERR(p)`。`ERR_PTR`只是long转指针，`IS_ERR`依MAX_ERRNO/位模式识别；对确已生产的可表示-ENOMEM可作有限正确编码模型，不能把任意非零正数、超范围负值或先前未定义的结果自动判断为errno。alloc_pid错误仅在其返回IS_ERR指针的条件门内用PTR_ERR取retval。证据FR44-A017/A018、A032、A035、A052–A056。

## 4. R03｜exit_files前置门与文件表引用配对

只为本次fork失败路径，定点复读`file.c::exit_files`与`put_files_struct`：保存`files=tsk->files`（位于task_lock之前），若非NULL则lock、清`tsk->files=NULL`、unlock、put_files_struct(saved_files)；表计数减到零时才遍历`i<fd_count`并对非NULL槽调用filp_close，最后发出files_cachep缓存free请求。整个清理过程中，`file_s`的计数与`files_struct.refcount`不是同一个计数。所选具体路径只核已读`exit_files(p)`这个显式调用，不排除未展开opaque函数可能有其他资源动作。证据FR44-A046–A051。

在有效p/files、引用初值为正、原子运算正常、同一表身份稳定、无额外写者的**有限局部模型**中，共享旧表原值n经`copy_files`正常增1，再由后续错误中的一次`exit_files(p)`使n+1→n；不是file.f_count逐槽加减，也不证明完整task生命周期和全局计数配平。特别是对于静态init_files条件模型，若初值1且fork共享后失败回滚，局部1→2→1并不触发最后表free；旧CL42-F01是否存在另一条实际最后归还路径仍未证，保持needs_evidence。

非共享新表来自`dup_fd`，成功时新表refcount1。若后续错误使`exit_files`正常进入，它请求表计数1→0并遍历/关闭槽，最终请求释放表对象；但43已证明旧槽NULL时新槽未由本体写NULL、非NULL槽各自按值复制独立file_s及其f_count和指针成员。旧UV-09跨缓存来源、未知新槽初值、物理边界和file引用是否有效，可能在遍历中形成更早停止点；“源码调用exit_files”绝不等于“整个表资源已被正确回收”。若更早的mmput、sighand、exit_fs、exit_thread等不正常返回，停止在前面，不将预期减计数误记为已发生。

copy_files失败且未取得新持有时，直接goto semundo跳过显式exit_files；被复制但未拥有的旧表指针可以在任务结构内一直存在到随后任务结构释放代码，**不因此自动构成重复归还或泄漏**。任务对象的`put_task_stack/delayed_free_task`与文件表`exit_files`是分开的活动函数调用；不深入task allocator，也不声称间接函数绝无额外行为。源末`fork_out`对current的sighand加解锁之后才返回错误指针，仍须所有活动中途操作有定义且正常完成。实际编译、优化、副作用及物理回收均未运行。

## 5. 反证、延期和执行边界

本批用35个有限输入/事件状态模型区分：尚未取得files持有、成功NULL/共享/复制三种状态、外层正常错误/内层失效、各后续错误goto、清理前的opaque停止、正常ERR_PTR编码与未证返回。详见facts-and-boundaries.yaml，DOT的GOTO/FALLTHROUGH/CALL/ANALYTIC分别表示真实控制与额外前提。历史`FT43-F01`、`CL42-F01`、`UV-09/10/11/14`、`US-01`均保持原qualified_id/status。

**本批新增独立延期缺陷项为0**：在当前被授权的局部范围内，已找到与取得持有阶段相配的不同错误标签，没有充分证据建立一个不与旧编号重复的新根因。另以`FR44-O01..O03`记录三组有界观察、反证与未证项，供C04审阅，不强行造问题数量。下一步的修复/完整fork生命周期、异常注入、分配器检查需独立授权，不在本次执行。

本阶段不修内核、不出补丁、不编译运行、不运行仓库C/ASM/脚本、QEMU/硬件、真实fork/exec/exit/并发/完整slab和额外付费API。未做完整P9或实际物理释放验收。本执行者只能申报READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING；最高由C04独立裁定PASS_PENDING_LOCAL。
