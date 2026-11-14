---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-CLONE-GROUP-72
record_type: bounded_clone_admission_signal_ownership_and_group_publication_contract
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台配置未独立认证）
model_per_owner: gpt pro
date: 2026-11-11
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 5f3b00d9436f
lead_before_publication_short12: 18524ef4af56
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_display_name: MYOS2-A-C05b 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-08
executor_model_per_owner: 次一档 GPT-6＋Extra
executor_rotation: false
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-clone-group-72/
allowed_files: [MANIFEST.md, clone-group.md, facts-and-boundaries.yaml, clone-group-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: []
D00: NOT_APPLICABLE
mandatory_supplements: [R71-S01, R71-S02, R71-S03, R70-S01, R70-S02, R70-S03]
activation_gate: Owner转发现有C05b并要求执行本已固定发布合同且G00通过
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
owner_decisions_requested_now: []
open_questions: []
---

# 第72批：创建入口、信号对象归属与线程组发布

继续现有C05b/CHAT-EXEC-08、次一档GPT-6＋Extra；先G00，再连续G01–G04，无D00，不逐专题等待批准。六件整包一次主线验收。发布不自动向另一聊天投递或启动。

## 1. 本批增量及排除范围

71证明de_thread局部没有实现注释所述线程组隔离，但仍缺真实创建入口、共享signal/sighand与组字段的生产关系。72依次核A入口与clone标志、B信号对象复制/共享与引用、C组身份发布和有限失败清理；不预设用户已能创建CLONE_THREAD线程，也不预设一定只有单线程。
不重做11完整PID/IDR、15全部MM、43/44文件表、45/60全部fs、67/68首帧或09/10全部信号递送。signal/sighand已知旧根因必须先核原ID；44已提UV-14，不得把已知sighand指针归属缺口当新发现重新编号。一般问题按18号延期，不询问Owner逐项修复，不改内核。

## 2. G00与固定输入

完整读本103号、同发布提交的reviews/CORE-USER-EXEC-COMMIT-GATES-71-review.md及checkpoints/2026-11-11-exec-commit71-reviewed-clone-group72-ready.md。完整刷新63-github-write-retry-and-readback.md、79-work-package-sizing-and-pro-review.md及agent-workspace/tasks/00-gpt-task-protocol-v2.md全部P1–P13。其它MyOS2九份约束及适用Mnemosyne指导仅可复用本有效C05b确实独立已读原文，缺项补读，不继承别的聊天声明，不导入其它项目维护。
71固定5f3b00d9436f：完整读exec-commit-gates.md、deferred-findings.yaml；facts仅admission_gate_matrix、task_state_matrix、post_commit_failure_matrix、prior_findings_links；合读正式交付6790358648及主线R71-S01/S02/S03。71已PASS_PENDING_LOCAL，无返工，不重做71。
历史定点输入：同主线reviews/CORE-USER-PID-LIFETIME-11-review.md §3，CORE-USER-FORK-FILES-ROLLBACK-44-review.md §2–4，CORE-USER-FS-CONTEXT-45-review.md §2–3。涉及11历史退回状态则补读12回执对应处置，不从历史RETURN重开已处置事项。引用UV-14等旧ID前，沿真实历史入口取得原条目及有效回执；未取得则写身份待核、不能臆造状态或重复报告为新根因。
核PR16/17、直接lead/exec/master/time heads、开放PR、目标72目录和已存在评论。首次写前exec应为5f3b00d9436f、time固定a039d9803ade、新目录不存在。主线正常发布103不算执行分支竞争。读取公约§1第2条与time/master双金丝雀原文登记MANIFEST；未知并行exec更改或同名目标先停相关写入核归属。

## 3. G01/A：真实入口到标志准入

固定mykernel/sched/syscall.c的fork入口；mykernel/sched/forkexec/fork.c的kernel_thread、kernel_clone和copy_process入口检查。核kclone_args_s真实定义及所用CLONE_THREAD/CLONE_SIGHAND/CLONE_VM、CLONE_PARENT、PIDFD、CSIGNAL等宏，路径由实际include和mykernel/sched/misc/sched_misc_types.h、sched_type_declaration.h定位。
建立caller→参数/flags生产→接口检查→copy_process准入表；区分fork的显式成员与其余初始化、kernel_thread掩码/添加位、任意直接kernel_clone参数。copy_process的THREAD→SIGHAND→VM约束及namespace/PIDFD冲突按最早门处理，合法组合不等于已创建有效线程。
普通系统调用暴露只定点核mykernel/arch/x86_64/entry/syscall_64.c及其实际include asm/syscalls_64.h的fork/clone/vfork相关条目，必要最近包装宏。号码/声明/条件宏存在不等于活动实现；默认分支搜索只定位，必须回固定time。若选定范围找不到真实clone入口，记录覆盖和缺口，不宣称全仓不存在，不转查libc或运行程序。

## 4. G02/B：signal与sighand的对象、引用及发布

完整读fork.c::copy_sighand、copy_signal、__cleanup_sighand；定点读dup_task_struct结构复制及copy_process调用顺序。实际类型从task_types.h、signal相关include定位，只展开需要的计数、action、pending、nr_threads、sigcnt、curr_target和rlim成员/初始化帮助函数。
对共享与非共享两路分别记录：旧对象、局部新对象、tsk成员在何时实际写入、增加哪个计数、哪些写入/初始化只是注释、正常或失败返回后谁仍持有哪个对象。不得以分配成功或返回0替代tsk已经绑定新对象，也不得把signal_s、sighand_s、task.pending或shared_pending合成一物。
共享指针的来源必须合读前序结构复制、current/orig一致和无中途改写；不因clone flags声明共享就假定所有引用自动增加。旧UV-14若同根因仅补调用者/分支证据；新缺口与旧证据分账。计数归零只证明所选释放请求，非物理回收、并发安全或signal语义完整。

## 5. G03/C：组身份、可查发布与失败边界

定点读copy_process的p.pid/p.tgid、real_parent、init_task_pid_links/init_task_pid、thread_group_leader分支、signal组计数/链表更新及attach_pid、return p；thread_group_leader必须回真实活动定义，不用通用Linux实现替代。kernel_clone的错误消费与wake_up_new_task只核交接门，不展开调度器；CLONE_VFORK等待是否活动可作有限对照，不重做completion主体。
建立“对象可分配→成员已赋→组身份/引用就绪→PID/任务表挂接→请求唤醒”分层矩阵。tgid相同、指针相同、nr_threads数值、有效组成员链、已运行是不同事实。注释中的group_leader/thread_node更新与真正结构成员分开；实际已找到的创建路径与假设CLONE_THREAD参数分开。
失败清理限copy_sighand/copy_signal之后到其cleanup标签的所有权事件：按真实入口与贯穿次序核__cleanup_sighand(p->sighand)、被注释的free_signal_struct和先行callee有效性。沿44的files/fs回滚限定，不重跑整包；第一处对象有效性/计数/锁条件缺失即停止，不在失效对象上算确定二次free。将结果挂靠EC71-F01/U01、EC71-F03/U03的真实前提，不能由新资料自关旧候选或证明exec安全。

## 6. 六件、预算与P9验收

唯一六件见头部。facts至少含source_ledger、caller_flag_matrix、clone_admission_matrix、signal_ownership_matrix、group_publication_matrix、failure_ownership_matrix、finite_model_cases、prior_findings_links。新条件项CG72-Fxx、缺口CG72-Uxx；旧根因优先原qualified_id。
软上限14个核心/最近直接依赖源码文件、8组定点检索、4次相关目录浏览；必要头文件跨目录不等于目录全审，超限说明理由。输出严格6件，无代码补丁；新候选/缺口各不超过12、有限模型不超过40、证据锚软上限120；这些是预算上限而非数量目标，重复证据不得凑数，强制定义冲突不以赶数量掩盖。
强制锚：A真实caller参数/flags与每类实际准入门；B结构继承、分配/字段发布、共享计数及清理门；C真实组判据、组字段/计数或注释边界、失败标签与返回/发布调用点。引文须逐字连续1–5物理行、围栏非空、位于单一真实定义；定义声明不能替代调用证据，ID必须真实存在且支持对应命题。图区分真实调用、宏、条件数据、调用者顺序、注释和ANALYSIS_STOP。
完整适用协议v2全部十三条。P9-1检查完整40位串；2/13逐锚核真实定义/命题及强制项；3固定路径/原件及发布后双回读；4行号只作辅助并核实际范围；5按六件原始全文统计保留前缀，非证据说明勿再写同一前缀；6外部资料须真实可核，无需外部标准即可回答的本树问题不扩搜；7/8双金丝雀、公约原句和read_channel；9/10/12无代码/补丁/降级则N/A；11禁止冒称构建运行。MANIFEST有署名、Owner档位/后端未知、self_check三数、实际覆盖/复用、P9逐项、write_retry_log；完整本地P9仍NOT_RUN。

## 7. G04及停止

Owner转发固定103入口并要求执行、G00通过后连续G01–G04。仅原PR17新user-clone-group-72/六件及CHAT-EXEC-08本人唯一正式评论可新增；旧成果/评论、PR元数据、lead/PR16、master/time冻结，主线不代写执行成果。
GitHub暂态异常不得一次失败即停：先核远端效果，按63号有间隔重试，通常首次加三次、约10/20/40秒或相应独立核查，遵守Retry-After。保持expected-head/force=false，不覆盖未知并行变化；已创建对象复用，评论先查重，工具发现次数与实际写请求分账。
compare自5f3b00d9436f应严格六件新增、旧件零净改删。固定新提交与活动分支六件全文/blob、HEAD、PR状态和唯一正式评论均回读。中断后先定位已成功的提交/评论，只补确实缺失步骤，不重建已存在交付。
只签READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING及A/B/C实际范围；完成STOP_WRITING，返回真实短提交与正式评论交C05独立验收，不自签72通过、不自行开始73。
严禁内核修改/修复补丁、预处理/编译/链接、仓库程序/脚本/旧夹具/QEMU/实机、另建分支PR、强推/合并/关闭/删分支、阶段3、跨仓维护或额外付费模型/子代理。
