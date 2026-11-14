---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-TASK-STORAGE-LIFETIME-74
record_type: bounded_task_object_stack_and_reference_lifetime_contract
evidence_class: 有界任务设计与权限；非执行或验收完成
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台配置未独立认证）
model_per_owner: gpt pro
date: 2026-11-12
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 8bc7583e45c9
lead_before_publication_short12: e482eb76bbba
fixed_contract_commit: PINNED_BY_OWNER_FORWARDING_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_display_name: MYOS2-A-C05c 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-09
executor_model_per_owner: 次一档 GPT-6＋Extra
executor_rotation: false
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-storage-lifetime-74/
allowed_files: [MANIFEST.md, task-storage-lifetime.md, facts-and-boundaries.yaml, task-storage-lifetime-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R73-01]
D00: REQUIRED_APPEND_ONLY_DOCUMENTATION_DISPOSITION
mandatory_supplements: [R73-S01, R73-S02, R73-S03, R72-S01, R72-S02, R72-S03]
activation_gate: Owner转发现有C05c并要求执行本固定合同且G00通过
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
owner_decisions_requested_now: []
inputs_read: [73固定成果与主线回执, task及stack相关源码设计预读, 28及79号]
open_questions: []
---

# 第74批：任务结构存储、内核栈和持有者生命周期

继续现有C05c/CHAT-EXEC-09，次一档GPT-6＋Extra，无需新开对话。先G00，再D00及连续G01–G04，不逐专题等待批准。D00与三个技术专题共用六件、一次主线验收；不改内核，不要求Owner处理普通缺陷。

## 1. 增量与排除

73已核退出调用链，但没有证明task_struct、内核栈与外部借用者之间的存储寿命。本批分别追踪A任务结构取得/初始化，B独立内核栈的取得/释放门，C引用持有者和实际销毁调用者。分配、结构复制、字段覆盖、持有计数、释放请求和实际物理回收分层。
不重做11完整PID/IDR、15 MM、43–45 files/fs、66/67全部切栈首帧、72信号复制、73退出正文或调度算法。UV-15、UV-16、PID11的裸task借用等已知根因优先原qualified_id；未核历史状态写未知，不自关或新号重复。kfree/kmalloc的内部算法与实际物理释放不在本包，必要时止于接口与来源。

## 2. G00固定输入及规则

完整读取本106号、同固定发布提交的reviews/CORE-USER-EXIT-REAP-73-review.md和checkpoints/2026-11-12-exit-reap73-reviewed-task-storage74-ready.md；73当前RETURN_DOCUMENTATION，仅R73-01，技术主干按R73-S01/S02/S03保留。
完整刷新63-github-write-retry-and-readback.md、79-work-package-sizing-and-pro-review.md、agent-workspace/tasks/00-gpt-task-protocol-v2.md全部P1–P13。其它九份MyOS2规则与适用Mnemosyne指导，仅复用本有效C05c确实独立读取过的正文，缺项补读，不借其它聊天已读声明；不重放105接收或旧建分支PR仪式，不导入其它项目维护。
固定执行8bc7583e45c9下完整读user-exit-reap-73/MANIFEST.md、exit-reap.md、deferred-findings.yaml；facts读exit_entry_status_matrix、cleanup_ownership_matrix、dead_task_handoff_matrix、prior_findings_links；evidence读R73-01指定锚及本批依赖段。核正式评论6810987102。
历史：同执行固定提交的user-vfs-baseline-05/deferred-findings.yaml之UV-15/UV-16；同主线reviews/CORE-USER-PID-LIFETIME-11-review.md §3、CORE-USER-FORK-FILES-ROLLBACK-44-review.md §2–4。涉及旧切栈/栈顶公式仅定点合读CORE-USER-ENTRY-STACK-BINDING-66-review.md及其DOC-CLOSURE，涉及MM切换只读CORE-USER-MM-CONTEXT-15-review.md对应限定；不重新打开历史退回。
核PR16/17、直接lead/exec/master/time、开放PR、目标74目录及本人marker。首次写前exec应为8bc7583e45c9，time固定a039d9803ade，新目录不存在。主线发布106不算执行分支竞争；未知exec前进或同名文件先核归属，不强推。公约§1第2条与time/master双金丝雀必须实际回读并写入结构化MANIFEST字段。

## 3. D00：R73-01追加纠正

不改旧73六件或评论。新正文设D00，facts设prior_return_disposition，MANIFEST记录纠正范围与状态，证据用D前缀。
一、将73 A16/B13/B14的成员引文分别重新锚到mykernel/sched/task/task_types.h::task_struct；保留原真实语句、单一定义内连续1–5行，列三组旧→新ID映射。需要类型别名时，单独引用sched_type_declaration.h::task_s，不与struct成员拼成一个定义。
二、在本批MANIFEST实际提供branch_canary_quotes字段，并在73追加处置中给出缺字段的替代值、两分支回读来源及差异。旧73正文已有两行原文，不得写成从未检查金丝雀或错误分支；read_channel和startup_selfcheck_quote不属于缺项。
三、给facts.exit_entry_status_matrix的sys_exit_group.operation追加替代表项：实际do_group_exit调用do_exit(exit_code)，不是do_exit(current)。用真实调用锚支持，区分当前任务对象和整数实参。
四、覆盖原正文/事实/清单及评论6810987102的相关引用与56/56全部定义合格声明。原56是标签数，不把新锚或未复核的其余53条当作旧包全量通过。新74原始六件总计数包括D00实际新标签，说明文字不得制造额外前缀命中。
只提交documentation_disposition_submitted，关闭由C05裁定。D00不要求重做73、改变ER73或UV候选状态、不重开58/63空行误判。

## 4. G01/A：任务结构的取得、复制与计数初始化

固定mykernel/sched/forkexec/fork.c完整读alloc_task_struct_node、free_task_struct、dup_task_struct；task_types.h及sched_type_declaration.h只取真实struct/alias、usage、stack、tasks等最近成员。定点核copy_process实际调用者的node来源及紧邻错误门，必要时kernel_clone参数来源。
逐阶段列原task与新task身份、分配结果、*tsk=*orig对先前初始化和借用字段的覆盖、stack尚为旧值还是已替换、usage何时重设、worker_private等后续赋值与失败出口。INIT_LIST_S在整体复制前后的顺序必须按活动代码，不凭注释补回初始化。若同根因已有旧条目，仅追加本批影响链。
node非NUMA_NO_NODE时未初始化tsk属于UV-16，不能继续演算为可靠NULL或错误码；正常node路径仍须验证orig和分配对象前提。分配失败、结构复制完成、栈取得成功、返回task指针、随后发布可查状态是不同阶段。usage=1不证明全局持有者只有一个。

## 5. G02/B：内核栈取得、释放与死亡状态门

固定fork.c完整读alloc_thread_stack_node、put_task_stack、release_task_stack、free_task；核THREAD_SIZE、READ_ONCE、WARN_ON及状态常量的最近实际定义/包含路径，仅用于本调用门，不运行预处理、不靠通用Linux实现替代。
建立对象/状态矩阵：task有效性、stack来源、分配失败是否写字段、旧继承指针和新分配指针分离、TASK_DEAD门、stack_refcount初始化/递减是否活动、kfree请求与后续NULL存储顺序。不得把put名字当作必有减计数，也不得把状态等于TASK_DEAD当作已无人使用这块栈。
free_task是否独立释放stack、put_task_stack是否释放task、重复调用同一task与已经free的task之间的差异必须分开。任何无效task、未初始化stack值或不返回callee先停止；kfree(NULL)的效果没有读最近定义就不凭习惯补写，不展开分配器全审。不要提出实际修复或构造运行触发程序。

## 6. G03/C：借用任务指针、持有引用与销毁调用者

固定mykernel/sched/task/pid.c定点读find_get_task_by_vpid、get_pid_task、get_task_pid及最近get_pid定义来源；区分PID对象引用、返回裸task指针和task.usage增量。get_task_struct/put_task_struct/put_task_struct_rcu_user的定义和活动调用仅做有界定点检索；默认分支搜索只定位，必须回固定time。找不到时记具体覆盖与缺口，不能用搜索0结果认证全仓不存在。
固定fork.c读delayed_free_task、copy_process中bad_fork_free前后的终端清理以及直接入口；kernel_clone中相关获取/归还只核task/PID对象，不重做PID号分配。建立调用者→状态写入→stack释放请求→task释放请求→返回/发布矩阵。按44限定保留前序sighand/fs/files等可能更早停止，不把标签相连说成已成功释放。
对正常退出路径，消费73已核的do_task_dead、finish_task_switch、schedule_tail和release_task注释界限，只为区分正常退出与创建失败。存储free的函数存在不证明退出已接通；PID哈希摘除、scheduler持有者消失和物理存储释放不是同一步。实际栈上运行者与被释放栈的身份未证时写缺口，不以TASK_DEAD或usage数值替代。

## 7. 六件、预算与P9

六件固定见头部。facts至少含prior_return_disposition、source_ledger、task_allocation_matrix、task_reference_matrix、stack_lifetime_matrix、destroy_caller_matrix、earliest_stop_matrix、finite_model_cases、prior_findings_links。新条件项TSL74-Fxx、缺口TSL74-Uxx；旧根因优先原ID，无新问题数量配额。
软上限14份核心/最近依赖源码、8组定点检索、4次相关目录浏览；必要超限说明原因，不能全仓漫游。输出严格6件、证据软上限120（含D00）、有限模型不超过40、新候选和缺口各不超过12；上限不是目标。模型必须写对象有效性、计数时点、可表示性与不可达组合，不从失效对象继续推出确定结果。
本任务适用agent-workspace/tasks/00-gpt-task-protocol-v2.md全部十三条。强制锚：D00三成员实际定义和真实调用；A分配/复制/usage初始化及失败出口；B栈发布/状态门/真实释放请求；C借用与持有增量、实际销毁调用点和正常退出断点。每锚非空、逐字连续1–5物理行、真实单一定义；路径、符号、ID和命题一致。
P9-1禁全文40位串；2/13逐锚回源及强制证据；3固定路径和写后读回；4行号仅辅助并核范围；5六件原始全文前缀计数与MANIFEST三数一致；6外部事实真实来源，不用外部标准补本树实现；7/8必须有branch_canary_quotes、startup_selfcheck_quote、read_channel；9/10/12无代码/补丁/降级则N/A；11不能声称编译运行。署名与Owner模型/后台未知分开，记录实际覆盖、D00和write_retry_log；完整本地P9仍NOT_RUN。

## 8. G04写回、重试与停止

仅原执行分支claude/dazzling-cori-q0dnyt/PR17新user-task-storage-lifetime-74/六件及09本人唯一正式顶层交付评论可新增。所有旧成果/旧评论、PR元数据、lead/PR16、master/time冻结；C05不代写成果，旧08不恢复写权。
首次写前再次核exec=8bc7583e45c9及目标不存在，使用expected-head和force=false。GitHub暂态异常不得一次失败即停：按63号先核远端效果，再间隔安全重试，通常首次加三次，遵守Retry-After。复用已创建对象，评论marker查重；工具发现次数与真正写请求分账。未知并行变更不覆盖；中断只补确实缺失步骤，不重建已有交付。
compare自8bc7583e45c9应恰六件added，旧文件零净改删；新固定提交与活动分支六件全文/blob、HEAD、PR状态及唯一评论全部回读。完成后STOP_WRITING，返回真实短提交、正式评论及D00/A/B/C范围，状态仅READY_FOR_MAINLINE_REVIEW/mainline_acceptance=PENDING，交C05独立验收；不自关R73-01、不自签74、不自行开始75。
严禁内核修改/修复补丁、预处理/编译/链接、仓库程序/脚本/旧夹具/QEMU/实机、另建分支PR、强推/合并关闭/删分支、阶段3、访问Owner本机凭据、跨项目维护或额外付费模型/子代理。临时纯文本检查和自建有限模型须如实记账，不等于内核运行。发布任务不自动唤醒或投递到另一聊天。
