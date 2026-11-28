---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-EXIT-REAP-73
record_type: bounded_exit_notification_and_reaping_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-09
executor_display_name: MYOS2-A-C05c 代码分析执行
produced_by: GPT-6 (model self-reported; backend not independently attested)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
source_pin_short12: a039d9803ade
read_channel: connector
D00: NOT_APPLICABLE
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第73批｜退出入口、父子通知与回收交接的有界静态分析

本报告沿固定 `time@a039d9803ade` 观察活动语句；证据为 `evidence.md` 中A01–A16、B01–B15、C01–C25的56条固定路径/单一符号、连续1–5原样物理行。每条通过第二次GitHub连接器从同一固定源码回读核对。对于C的未初始化局部、对象失效、无返回callee，先记录**ANALYSIS_STOP**；不把后来顺序出现的源码动作当成已确定发生的机器动作。没有预处理、编译、执行仓库脚本、QEMU、实机或旧夹具。

前置：Mnemosyne先接收、新对话独立加载九份MyOS2约束、再刷新适用Mnemosyne指导，G00已核原PR17执行头、旧72正式回执和主线。第72批 `PASS_PENDING_LOCAL` 及R72-S01/S02/S03；71的R71-S01/S02/S03均继续生效，`D00=NOT_APPLICABLE`。72不重做；受历史UV-15、PID11§3、R44§2–4、R45§2–3、R15§2–4、SF-C3-37、CG72-F01/F02/U05约束。此处不是完整exit/reap子系统验收。

## G01 / A｜实际退出入口、状态码与不返回边界

`mykernel/arch/x86_64/include/asm/syscalls_64.h`在固定表中有活动`__NR_exit`、`__NR_wait4`、`__NR_exit_group`的`__SYSCALL`映射；`entry/syscall_64.c::x64_sys_call`的宏case把入口转给long型x86包装（A13），`asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx`生成long `__do_sys`与`__se_sys`包装（A14），`kernel/syscalls.h`里的`MYOS_SYSCALL_DEFINE1`宏将它绑定到用户出口（A15）。这是固定文本入口形态，不是预处理后链接正确的承诺。

- `sched/syscall.c::MYOS_SYSCALL_DEFINE1(exit, int, error_code)`实际调用`do_exit(error_code)`，没有就地取低8位或左移8位（A01/A02）。`do_exit(long code)`接受long，之后执行`tsk->exit_code = code`；任务结构的`exit_state`、`exit_code`、`exit_signal`均为独立int字段（A06/A08/A16）。
- `MYOS_SYSCALL_DEFINE1(exit_group, int, error_code)`在源码中执行`do_group_exit((error_code & 0xff) << 8)`（A03/A04），而`do_group_exit(int exit_code)`关于`SIGNAL_GROUP_EXIT`、写`sig->group_exit_code`以及`zap_other_threads(current)`的分支皆为注释（A10/A11），真正的活动路径是`do_exit(exit_code)`（A12）。不能因入口名而证明全线程组结束、其他线程被发信号或组共享码已发布。
- `do_exit`先声明`int group_dead;`，唯一在所读体内的赋值`// group_dead = atomic_dec_and_test(&tsk->signal->live);`是注释（A06/A07）。稍后`exit_notify(tsk, group_dead)`把该局部作为实参（A09）。读取未初始化自动局部构成C语言层面的未定义值问题：即使现有`exit_notify`主体目前未活动消费形参，也**不能**据此证明该调用安全，更不能把后面的`do_task_dead`必达当做确定的机器行为。这是早已存在的PID11/UV-15限制；不自建新编号。
- `MYOS_SYSCALL_DEFINE4(wait4,...)`是实在的、空体非void宏生成函数（A05），没有`return`、孩子查找、填写`start_addr`或释放子任务。其long包装本身并不补出返回值。因此既不能假定等待返回0、`-ECHILD`，也不能说已经观察到稳定的`pid`或wait状态。

在假设入口实参和指针均有效、一路到达`tsk->exit_code`的**纯算术模型**里，输入1时exit保存1而exit_group保存256；输入256时前者保存256而后者保存0；负值及0xff以有限整数算术单独记录于facts。这仅生成新的条件候选`ER73-F01`：两个接口不同的编码**可能**与未来wait状态契约不一致，也可能是有意区分的接口约定；当前没有指定的同码要求或可用wait4消费者，不作为已经验证的内核bug。

## G02 / B｜子任务链表、父指针、通知与wait

`exit.c::exit_notify(task_s *tsk, int group_dead)`的实际主体极短而断点集中（B01–B07/B15）：

1. `while (tsk->children.count != 0)`（B02）成立时，以`list_header_remove_head(&tsk->children)`请求摘下头节点，紧跟`while(child_lp == 0);`空体自旋（B03）。`double_list.h::list_header_remove_head`在有计数时从anchor摘实际节点、重置该节点，计数减一并返回节点（B11）；如果列表计数与链节点不一致，不能由count非零保证结果是有效的child。`count==0`则不进入这个迁移段，不能凭退出名字推断所有任务都重新挂父。
2. 节点有效后，代码求`find_task_by_pid_ns(1, PIDTYPE_PID)`（B04）。真正声明`find_task_by_pid_ns(pid_t nr, pid_ns_s *ns)`（B08）的第二形参是指向namespace对象的指针，`find_pid_ns`沿该指针访问`ns->idr`（B09）；`pid_const.h::enum pid_type`把`PIDTYPE_PID`定义为首个枚举值、数值0（B10）。因此原调用把**PID类型枚举值**当作**PID命名空间指针**，不是一个真正的namespace生产者。编译器可能诊断，也可能接受转换；没有运行证据，不能宣告必然编译失败或已在物理机崩溃。原PID11§3早已记录该类型/指针边界；本批只补独立局部事件链。非空children路径必须在此无效namespace前提处停止，不能再认证随后task_initd有效或成功移入。
3. 只在假设给定有效的目标task、有效双链节点等条件下，`list_header_add_to_tail(&task_initd->children, child_lp)`可由`double_list.h::list_header_add_to_tail`证明**目标链表节点挂接及count增加请求**（B04/B12）。`task_s`真实字段同时有`parent`和`real_parent`（B13），以及独立的`children`、`sibling`（B14），但`exit_notify`当前所读活动体没有给这些parent字段赋值。故链表搬移≠parent/real_parent更新≠真实parent继承语义的一致完成。
4. 源码中`// tsk->exit_state = EXIT_ZOMBIE`、`// autoreap = do_notify_parent(...)`、`// tsk->exit_state = EXIT_DEAD`、`// release_task(p)`均为注释（B05/B06/B15/B07）；所谓fork子进程退出后的ZOMBIE、通知SIGCHLD、自动reap并未由这些局部语句实施。它们仍是设计意图的证据，不能写成活动CALL。未穷举全仓其他通知者，不主张全内核绝对不存在任何通知途径。
5. `wait4`的入口实际源为空非void函数（A05），不能从`tsk->exit_code`值存在推出父任务能获得终止状态，更不能用`release_task`的注释或PID查询helper存在替代实际reap调用。

原`CORE-USER-VFS-BASELINE-05::UV-15`明确是退出/等待/回收缺口，状态原为`deferred_owner_not_ready`。本73将其“会把子任务挂到1号任务名下”限定为**只存在尝试改挂的代码**，要求parent对象与namespace有效且代码到达；将“每次退出必留下全部地址空间和task”限定为**在选定路径看不到完整清理请求，不证明每次物理泄漏**。不覆盖UV-15旧报告，也不作为新的根因再编号。

## G03 / C｜归还请求、TASK_DEAD与剩余持有者

`do_exit`（A08/A09、C01–C07）在源顺序上先写`tsk->exit_code=code`，随后活动调用`exit_files(tsk)`、`exit_fs(tsk)`、`exit_thread(tsk)`、`exit_notify(tsk,group_dead)`、`do_task_dead()`。其中`// exit_signals(tsk)`、`// exit_mm()`、`// exit_task_namespaces(tsk)`、`// exit_task_stack_account(tsk)`仅为注释。不能把“顺序写在文本后面”提升为“已越过未初始化/无效对象停止点成功执行”，不能把创建失败时的`bad_fork_cleanup_*`标签当成正常退出暗中走过。

**文件表（files_struct）**：`fs/vfs/file.c::exit_files`读取`tsk->files`并在非空时通过锁保护把`tsk->files=NULL`，继而请求`put_files_struct`（C08/C09）。后者仅在`atomic_dec_and_test(files->refcount)`减后为零时遍历文件槽、对非NULL `file_s*`调用`filp_close`并最终请求cache释放（C10/C11）。如果旧表共享且计数大于1，则本次不遍历槽；一个文件表指针被清空不等于file全部关闭、slab实际释放。R44、历史UV-09的槽初始化、file副本来源、引用及cache风险继续约束。

**FS上下文（taskfs_s）**：`sched/misc/fs_struct.c::exit_fs`非空时加锁、清空`tsk->fs`、执行普通int的`kill=!--fs->users`，减后为零时才请求`free_fs_struct`（C12–C14）。后者依次请求`path_put(root)`、`path_put(pwd)`与cache free（C15）。`fs.users`不是`files.refcount`或`file.f_count`，每个对象/路径的物理引用效果与最后持有者取决于R45-S01/S02所限定的实际辅助体，不能按函数名称自动配平。数字1→0与2→1的有限模型仅在合法正持有者条件下成立。

**信号、MM、namespace及架构资源**：`exit_signals`、`exit_mm`、`exit_task_namespaces`在此源级函数均为注释；不能因此断定全系统其它释放者一概不存在（C01/C02/C04）。`arch/x86_64/sched/context/process_arch.c::exit_thread`虽然是活动callee，但其所读体的bitmap、vm86和FPU清理也是注释（C05/C16/C17）。CG72的`signal_s/sighand_s`来源账与`CG72-F02/U05`（后期fork失败对新signal的本地清理）不能用本正常退出路径关闭，也不应据局部缺口直接判定物理泄漏。MM引用回收沿R15§2–4保留`mmget/mmput/mmdrop`对象及切换上下文限定，不重做MM。

**死亡任务与调度交接**：在假设到达条件下，`scheduler_core.c::do_task_dead`实际`set_special_state(TASK_DEAD)`，随后`__schedule(SM_NONE)`；若调用返回则进入`BUG()`（C18/C19）。这不等于必定切换成功或未来再也不能选中当前任务；老`SF-C3-37`对队列为空、pick返回当前task的条件分析仍有效。`scheduler.h::context_switch`存在`switch_to`后`return finish_task_switch(prev)`（C24），但只有实际恢复到该调用者才可推断进入finish；R15-S02禁止把切换当常规同步同栈CALL。而`schedule_tail`里的`finish_task_switch(prev)`为注释（C25）。在`finish_task_switch`里，读取`prev->__state`、`if(prev_state == TASK_DEAD)`、`put_task_stack(prev)`及`put_task_struct_rcu_user(prev)`是完整注释块（C20–C23）。因此**写TASK_DEAD**、**调度请求**、**不再入队**、**实际完成switch**、**最后释放旧task/栈**为不同证据。选定exit直链中没有从这些动作导向`detach_pid`或活动`release_task`，也没有完整的wait reap与PID对象摘除链；并不替代对全仓回收者的未来审计。

### 三栏审计表

| 专题 | 已见实际活动CALL/写入（均带正常前提） | 源码注释表达的意图 | 未证及最早停点 |
|---|---|---|---|
| A退出 | exit原样传int；exit_group低8位左移；exit_code写入 | 组退出flag/zap其它线程 | group_dead未初始化；wait4无返回 |
| B关系 | 非空children摘头；以pid类型枚举查namespace；拟尾挂 | ZOMBIE/DEAD、通知、reap | 实参ns无效；child.parent未同步 |
| C files / fs | 指针清空、files引用减量、fs.users减量及条件释放请求 | 完整FD/路径资源语义 | 最后持有者、有效槽/路径、物理释放 |
| C mm/信号/线程 | 调用exit_thread；读取其他持有者有限 | exit_mm、exit_signals和架构FPU操作 | 未见局部活动清理，未穷举其它释放者 |
| C 死亡切换 | TASK_DEAD、__schedule请求、可条件进入finish | finish中的prev_state/put_stack/put_task | 非重选、实际switch与最终回收不成立 |

## 结果、延期与验收边界

- `facts-and-boundaries.yaml`以JSON兼容YAML1.2承载15份直接/最近上下文源码台账（高于14软上限1份，仅因需要额外确定PID枚举数值和系统调用包装返回类型）、五个要求矩阵、`prior_findings_links`及28个纯有限模型，全部为文本/整数/布尔推演，非编译运行。
- `exit-reap-paths.dot`含46个唯一显式节点、47条分型边、0未定义端点；`COMMENT`仅为注释，`ANALYSIS_STOP`仅为可证性停止，不是源码分支/内核陷入；Graphviz NOT_RUN。
- `evidence.md`共A16+B15+C25=56段非空单一定义连续1–5行，全部第二次固定time连接器回读逐字核对；`MANIFEST`自检为56/56/0，完整本地P9 NOT_RUN。
- `deferred-findings.yaml`仅1个新的待证条件候选`ER73-F01`（状态码编码合同）和9个证据缺口`ER73-U01`至`ER73-U09`，没有运行复现bug；`UV-14/15/16`、`CG72`、PID11、R44/R45、R15及旧调度记录保持原qualified_id和状态。

本次仅在第73批范围内将A/B/C标为`completed_in_scope`，整体`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`，验收上限`PASS_PENDING_LOCAL`，无Owner逐项修复决策请求。正式提交与唯一PR17交付评论后STOP_WRITING，由MYOS2-A-C05独立验收。没有修改内核、补丁、构建运行、合并、主线代写、阶段3或第74批授权。
