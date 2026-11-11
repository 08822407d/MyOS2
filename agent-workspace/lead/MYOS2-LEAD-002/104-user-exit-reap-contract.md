---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-EXIT-REAP-73
record_type: bounded_exit_notification_resource_and_reap_contract
evidence_class: 主线任务设计与权限边界，非执行完成记录
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台配置未独立认证）
model_per_owner: gpt pro
date: 2026-11-11
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 4cd92f424986
lead_before_publication_short12: becd64744d51
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
status: READY_FOR_NEW_EXECUTOR_RECEIVE_NOT_STARTED
executor_display_name: MYOS2-A-C05c 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-09
executor_model_per_owner: 次一档 GPT-6＋Extra
executor_rotation: true
previous_executor: MYOS2-A-C05b / MYOS2-LEAD-002-CHAT-EXEC-08
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exit-reap-73/
allowed_files: [MANIFEST.md, exit-reap.md, facts-and-boundaries.yaml, exit-reap-paths.dot, deferred-findings.yaml, evidence.md]
D00: NOT_APPLICABLE
prior_return_items: []
mandatory_supplements: [R72-S01, R72-S02, R72-S03, R71-S01, R71-S02, R71-S03]
activation_gate: Owner在全新C05c明确接收并执行，105号接收及项目规则/适用指导独立加载完成且G00通过
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
owner_decisions_requested_now: []
inputs_read: [第72批固定交付及本轮回执, Owner新对话要求, 退出链设计预读]
open_questions: []
---

# 第73批：退出入口、父子通知与回收交接

接收模型：次一档（默认），沿用Owner指定GPT-6＋Extra。必须全新普通ChatGPT对话「MYOS2-A-C05c 代码分析执行」，GitHub启用，不开Deep Research。先完整执行105-executor-continuation/01-C05c-receiver-startup-prompt.md，不沿用旧C05b已读状态。接收和G00通过后连续G01–G04，无D00，不逐专题等待批准；六件整包一次主线验收。

## 1. 增量、历史与明确排除

72核的是创建、signal/sighand来源与组发布；73核这些对象在退出路径中的实际消费。A退出入口及状态码，B父子通知和关系变更，C资源归还请求、停止调度交接与wait观察，三者共享证据但不混成“已经完整回收”。
UV-15、PID11对group_dead和namespace的既有问题、44失败清理及72的signal所有权缺口必须优先沿原qualified_id追加证据。普通问题记录后继续安全取证，不询问Owner逐项修复；不以找到旧问题冒充新根因。未核旧状态则写未知，不自行关闭或重开。
不重做整个PID/IDR、信号发送/帧、MM/页表、files/fs、调度器或分配器；不审外部libc/用户镜像，不提出或实施内核补丁。局部函数名、注释、指针清空、计数归零或调度请求均不等于真实物理释放/通知/用户返回。

## 2. G00与固定输入

首先按105/00和105/01完成：Mnemosyne接收报告→独立完整读取九份MyOS2规则→单独完整加载适用Mnemosyne指导。长响应截断须补齐；三项记录并入本批MANIFEST，不另写第七件。不重放旧00最初的新分支/新PR启动仪式，不接主线写权。
本启动URL的同一固定提交，完整读取本104号、reviews/CORE-USER-CLONE-GROUP-72-review.md、checkpoints/2026-11-11-clone-group72-reviewed-exit-reap73-c05c-ready.md、105两件；合读71回执R71-S01/S02/S03。105指明的85确认只用于主线谱系，不恢复其历史批次ON_HOLD。
在固定执行4cd92f424986完整读72的MANIFEST.md、clone-group.md、deferred-findings.yaml；facts只需signal_ownership_matrix、group_publication_matrix、failure_ownership_matrix、prior_findings_links，必要引文回evidence。核正式评论6796919213。72已PASS_PENDING_LOCAL，无返工，不重新验收72。
历史定点输入：执行分支同固定4cd92f424986的user-vfs-baseline-05/deferred-findings.yaml之UV-14/15/16；同主线reviews/CORE-USER-PID-LIFETIME-11-review.md §3、CORE-USER-FORK-FILES-ROLLBACK-44-review.md §2–4、CORE-USER-FS-CONTEXT-45-review.md §2–3。涉及MM实际回收再读CORE-USER-MM-CONTEXT-15-review.md §2–4；涉及调度死亡后是否可能再选中，只沿UV-15给出的SF-C3-37历史入口取得原项及有效回执，不重做调度算法。历史11的局部文档退回不由本任务重新打开。
现场核PR16/17、直接lead/exec/master/time、开放PR、目标73目录和本人marker。首次写前exec必须为4cd92f424986，time固定a039d9803ade，新73目录不存在；其它未知并行变化先核归属。双分支金丝雀、公约§1第2条原句、read_channel与真实接收模型来源必须写入MANIFEST。主线新发布104/105不是执行分支竞争。

## 3. G01/A：退出入口和状态码生产

固定mykernel/sched/syscall.c中exit、exit_group和wait4，以及mykernel/sched/forkexec/exit.c完整do_exit/do_group_exit。定点读mykernel/arch/x86_64/include/asm/syscalls_64.h相关条目、syscall_64.c及必要最近包装定义，只为判定真实暴露/返回类型，不重做整个系统调用入口。
列调用者实参、int/long/成员类型、位运算/转换、exit_code字段写入和真正不返回路径。exit与exit_group是否在包装层同样处理code必须回源码；group_exit注释中的全组动作不当活动实现。noreturn属性/NOTREACHED注释不等于运行不返回或组内其它任务已结束。
对do_exit的group_dead从声明到调用实参求值逐步追踪：没有活动赋值时，不凭被调用者目前不用参数就认定传未初始化值安全；不得从未定义行为继续认证确定的后续机器动作。这是已有PID11/UV-15证据消费，不自动新编号。局部文本后缀可作为有条件接口说明，与已证明可达前缀分账。

## 4. G02/B：父子关系、通知和等待接口

完整读exit_notify，按空/非空children、remove_head返回、节点和目标父对象有效性分别建立事件链。核mykernel/lib/list/double_list.h中实际remove_head/add_to_tail及最近列表结构，明确count改变、链节点转移与task.parent/real_parent字段写入是否是独立动作；不能用链表移动自动推出整个父子语义已一致。
固定mykernel/sched/task/pid.c的find_task_by_pid_ns及所需namespace/类型定义只展开最近依赖。若调用传入PIDTYPE_PID，与形参namespace对象不匹配，按实际枚举/指针和最早访问点解释，不声称必然编译失败或确定硬件表现；已有11记录须合读。
列exit_state、exit_signal、parent/real_parent、signal通知字段及所读do_notify_parent/release_task调用是活动、注释还是仅声明。只对已读范围作否定，不全仓断言任何其它通知者不存在。wait4空主体与调用返回值的消费要核真实非void类型/包装路径，不捏造0、-ECHILD或稳定状态码；不调用外部标准来补出本树没有的实现。

## 5. G03/C：归还请求与死亡任务交接

按do_exit的真实次序核exit_files、exit_fs、exit_thread及被注释的MM/namespace/信号清理。files/fs细节消费43–45有效回执，只有本批需要的新边才回固定mykernel/fs/vfs/file.c、mykernel/sched/misc/fs_struct.c；signal/sighand对象沿72矩阵，不能把创建失败的cleanup当成功任务退出自动执行的路径。
固定mykernel/arch/x86_64/sched/context/process_arch.c::exit_thread需读真实体；若为空或关键操作注释，不凭名字推断释放了栈或signal。对CG72-F02/U05只能追加退出范围证据，不自关候选，不推定全局物理泄漏。
定点核scheduler_core.c的do_task_dead和finish_task_switch死亡任务相关段及最近状态定义；区分设置TASK_DEAD、请求__schedule、实际不再选中、释放task/内核栈是四件事。按UV-15与既有调度回执保留未闭合边，禁止扩成调度器全审。只有所选helper真实到达且对象合法才继续计算引用归还；无效对象/未初始化值/不返回callee首先标ANALYSIS_STOP。
输出“实际活动调用请求/字段变化”“源码注释表达的意图”“需外部持有者/运行证据”三栏，区分普通退出、exit_group及创建失败清理路径，不把错误标签的存在当运行回滚。

## 6. 六件、强制锚和预算

唯一六件见头部，facts至少含source_ledger、exit_entry_status_matrix、notification_parent_matrix、cleanup_ownership_matrix、dead_task_handoff_matrix、wait_result_matrix、finite_model_cases、prior_findings_links。新条件项ER73-Fxx/缺口ER73-Uxx，已知根因优先旧ID。
强制证据：A实际入口实参/返回类型、exit_code写入、group_dead生产或缺失；B实际列表更新、namespace参数消费及通知/wait状态；C各实际清理/注释边界、死亡状态及调度/回收交接。每条引文非空、逐字连续1–5物理行、真实单一定义，路径/符号/ID/命题一一对应。声明或注释不能冒充活动调用；图分CALL、宏、条件数据、调用者顺序、注释与ANALYSIS_STOP。
预算：核心及最近依赖源码软上限14份、8组定点检索、4次相关目录浏览；必要跨目录须说明，不能漫游全仓。严格6件，新候选/缺口各不超过12，有限模型不超过40，证据软上限120；上限不是配额，不为凑数重复已证事实。强制证据不足先收窄结论，不假称完成。
本任务适用agent-workspace/tasks/00-gpt-task-protocol-v2.md全部十三条。P9-1全40位串禁入；2/13逐锚回源和强制项；3固定路径/源版本及发布后双回读；4辅助行号核范围；5原始六件全文前缀计数与MANIFEST三数；6外部事实须有可核出处；7/8双金丝雀、公约原句/通道；9/10/12无代码/补丁/降级则N/A；11不冒称运行。MANIFEST同时记录接收报告、九规则加载、Mnemosyne刷新及任务保持、实际覆盖、模型来源/后台未知和write_retry_log。完整本地P9仍NOT_RUN。

## 7. G04提交、故障恢复与停写

只在原PR17分支claude/dazzling-cori-q0dnyt新增user-exit-reap-73/六件和CHAT-EXEC-09本人唯一正式顶层交付评论。旧成果/评论、PR元数据、lead/PR16、master/time全部冻结；旧08无73写权，C05不代写执行成果。
暂态GitHub异常不得一次失败即停：先回读是否已成功，再按63号间隔安全重试，通常首次加三次，约10/20/40秒或相应独立工作，遵守Retry-After。expected-head/force=false，未知并行变化不覆盖；已有blob/tree/commit复用，评论marker查重，发现次数与真实写请求分账。中断后只补远端确实缺失的步骤。
compare自4cd92f424986应严格六件added、旧件零净改删。新固定提交和活动分支六件全文/blob、HEAD、PR状态及唯一正式评论均须回读。最终只报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING及A/B/C实际完成范围，STOP_WRITING后交C05独立验收，给Owner简短取件通知；不自签73通过、不开始74。
严禁内核修改/补丁、预处理/编译/链接、仓库代码/脚本/旧夹具/QEMU/实机、另建分支PR、强推/合并/关闭/删分支、阶段3、跨仓维护、额外付费模型/子代理或访问本机凭据。纯文本检查和自建有限模型须如实标注，不能替代内核运行。
