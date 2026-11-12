---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-TASK-STORAGE-LIFETIME-74
record_type: bounded_executor_delivery_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-09
executor_display_name: MYOS2-A-C05c 代码分析执行
produced_by: GPT-6 (self-reported; consumer backend not independently attested)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
model_tier_per_owner: 次一档
backend_identity: unknown_or_not_attestable
date: 2026-11-12
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract_pin_short12: d46bab628653
initial_execution_head_short12: 8bc7583e45c9
lead_preflight_short12: d46bab628653
master_preflight_short12: e8e60cd19540
execution_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
lead_branch: agent/MYOS2-LEAD-002
lead_pr: 16
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-storage-lifetime-74/
allowed_files: [MANIFEST.md, task-storage-lifetime.md, facts-and-boundaries.yaml, task-storage-lifetime-paths.dot, deferred-findings.yaml, evidence.md]
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  options_flags_cmake: |-
    	-m64 -mcmodel=kernel -fno-pie -fno-pic \
  panic_c: |-
    	this_cpu = smp_processor_id();
prior_batch73: RETURN_DOCUMENTATION_WITH_R73-01
prior_batch73_technical_core: RETAINED_WITH_R73-S01/S02/S03
prior_batch72: PASS_PENDING_LOCAL_WITH_R72-S01/S02/S03
D00_status: documentation_disposition_submitted
R73_01_closure: PENDING_C05_INDEPENDENT_REVIEW
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
self_check:
  verified_claims: 74
  quotes_reconfirmed: 74
  downgraded_to_inferred: 0
  anchors_D00: 5
  anchors_A: 24
  anchors_B: 22
  anchors_C: 23
  direct_and_nearest_source_ledger_entries: 14
  pure_finite_model_cases: 32
  new_conditional_findings: 1
  new_evidence_gaps: 8
  graph_nodes: 50
  graph_edges: 52
  graph_undefined_endpoints: 0
  full40_sha_strings: 0
  full_local_P9: NOT_RUN
  Graphviz: NOT_RUN
write_retry_log:
  logical_operation: six_blobs_one_tree_one_parent_commit_nonforced_expected_head_CAS_then_one_formal_comment
  attempt: blob_preparation_in_progress_at_manifest_creation
  error_kind: none_confirmed_as_of_manifest
  remote_head_before_ref_update_short12: 8bc7583e45c9
  remote_effect_observed: new_74_dir_absent_and_no_exec09_formal_comment_at_G00
  retry_action: only_if_actual_transient_error_and_remote_readback_says_absent
  final_readback: pending_G04_to_be_recorded_in_unique_comment
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
---

# 第74批｜接收、G00、D00、六件与P9交付清单

## G00固定资料与责任边界

继续同一有效C05c/CHAT-EXEC-09，不重新接收105旧交接，不复用旧会话后台或重新开分支PR。Owner本轮转发固定106任务并指令执行，故本批开始。原MyOS2九份规则与本会话先前已经独立完整加载的适用Mnemosyne指导在同一有效会话内复用；此次独立完整刷新106任务、R73主线回执、74-ready检查点、63安全重试、79合包规范、协议v2 P1–P13，不导入跨项目维护。

- 固定主线`d46bab628653`：`agent-workspace/lead/MYOS2-LEAD-002/106-user-task-storage-lifetime-contract.md`、`reviews/CORE-USER-EXIT-REAP-73-review.md`、`checkpoints/2026-11-12-exit-reap73-reviewed-task-storage74-ready.md`。
- 固定执行`8bc7583e45c9`：`user-exit-reap-73/MANIFEST.md`、`exit-reap.md`、`deferred-findings.yaml`全文；facts定点读`exit_entry_status_matrix`、`cleanup_ownership_matrix`、`dead_task_handoff_matrix`、`prior_findings_links`；evidence实际读D00依赖的A16/B13/B14/A12。73正式评论6810987102全文核对。
- 历史原项：同固定执行旧05 deferred的`UV-15`和`UV-16`逐条回读；固定主线PID11§3、R44§2–4、R15§2–4、66入口与其67 D00纠正说明定点阅读。R72-S01/S02/S03继续有效；R73-S01/S02/S03直接作为73技术主体的限定，未重审73的旧56条。
- 活动direct refs：`time@a039d9803ade`、`master@e8e60cd19540`、lead/PR16`d46bab628653`、exec/PR17`8bc7583e45c9`；两PR保持Open/unmerged，PR17保持Draft。开放PR仅16和17。首次新74目录不存在，09本批正式交付marker未见。执行分支与主线隔离，新106发布不是竞争写入。
- 本MANIFEST头部`branch_canary_quotes`为真实P3结构化字段：time的`options_flags.cmake`、`panic.c`原物理行逐字重读；master第一行不存在，第二行改用`raw_smp_processor_id()`。公约§1第2条原句和`read_channel=connector`来自本轮直接读回。
- G00仅授权106固定范围；不签完整内核生命周期、源码编译正确或物理回收。

## D00追加纠正：R73-01

**状态只报`documentation_disposition_submitted`，不自关R73-01。** 新`evidence.md`中D01/D02/D03分别以真正`mykernel/sched/task/task_types.h::task_struct`重锚旧73的A16/B13/B14三组真实成员句；D04把`sched_type_declaration.h::task_s`别名单独定义；D05在`do_group_exit`真实body核整数实参`do_exit(exit_code)`。

新`facts.prior_return_disposition`逐项记录旧ID→新ID、错误标签、正确标签、D04别名、原73金丝雀字段替代值与time/master直接差异。旧73的`read_channel`及`startup_selfcheck_quote`原本齐备；缺的是`branch_canary_quotes`，并非原来未阅读分支。旧事实表`sys_exit_group.operation`的`do_exit(current)`改作仅对后继有效的替代读法`do_exit(exit_code)`，不改73旧facts文件。

原73合计56标签，其三条定义身份不合格；另外53条本批未全量复核。旧73正文、旧MANIFEST/facts以及旧正式评论6810987102中“56/56所有引文均为正确单一定义”的总括**降级为历史作者标签/字面回读自报**，不可继续作为全量P2定义检查通过证据。本74新D5是新证据，不会把旧73原件变成正确文件；当前所有旧文件、评论、PR信息一字不改，等待C05独立裁定闭合。

## G01–G03实际覆盖与六件

- A `completed_in_scope`：alloc_task_struct_node/free_task_struct、dup_task_struct的node匹配/不匹配和NULL分流；INIT_LIST_S早于整体赋值`*tsk=*orig`，继承字段和成功重置stack/usage/worker_private时点；copy_process与kernel_clone原始调用关系。UV-16原qualified_id保留。
- B `completed_in_scope`：THREAD_SIZE来自12位页偏移和3阶宏的有限表达；kmalloc新栈成功才发布；put_task_stack没有活动refcount减门；release_task_stack读取状态，经WARN_ON条件判断后实际请求kfree并清NULL；free_task独立释放task结构，不自动释放stack；重复/失效指针先停止。
- C `completed_in_scope`：find_get_task_by_vpid、get_pid_task的task引用增量是注释；get_task_pid/get_pid/put_pid改变的是pid_s.count而非task.usage；创建成功与bad_fork_free的单一路径比较、delayed_free_task真实直达free_task；正常退出do_task_dead与finish的死亡任务回收注释分离。
- 新`task-storage-lifetime.md`：D00与A/B/C正文和活动/注释/有效前提三栏。新`facts-and-boundaries.yaml`：`prior_return_disposition`、14个固定time直接/最近依赖源码台账、要求的task_allocation/task_reference/stack_lifetime/destroy_caller/earliest_stop五矩阵、32个只读纯有限模型和历史ID。
- 新`task-storage-lifetime-paths.dot`：50唯一显式节点、52条带类型边、0未定义端点；条件DATA与CALLER_SEQUENCE不等于无条件源码控制路径。Graphviz NOT_RUN。
- 新`deferred-findings.yaml`：`TSL74-F01`唯一新条件候选（复制覆盖先前初始化的task链节点，尚无实际错误消费者）和`TSL74-U01–U08`八证据缺口；无实测bug，UV-15/16、PID11和其它旧问题原状态不改。
- 新`evidence.md`：D5/A24/B22/C23，**74条**固定time单定义、非空、1–5连续物理行的源码短锚。第一次确定定义范围和逐字来源；第二次再从固定time GitHub连接器取全14源并逐段核74/74全部字面匹配。新源数14不超软预算；未进行整仓grep全树阴性证明。

## P9逐项实际范围

1. 新六件不得出现完整40位十六进制SHA；提交前在文本中全量扫描。
2. 新74短锚共74段，逐段非空、1–5物理行、真实单一定义且固定time第二次匹配74/74；**完整本地P9未运行**。
3. 输入及源版本可溯；发布后须对新固定提交与活动执行分支六件做12次完整正文/Blob双回读。
4. 引文辅助行号均直接从固定源码得出、范围内，不用作主定位。
5. 原始新六件实际标签总数74，MANIFEST三数74/74/0，D00的5条也计入；旧73的56不计入本批新增数。
6. 未借外部Linux实现、手册或ABI填补本树缺失的实际函数。
7. 头部P3两行time金丝雀与master对应差异已核。
8. 本轮真实MyOS2公约§1第2条整句与读取通道入头。
9. 无代码件或新增代码符号清单：N/A。
10. 无内核补丁、修改块：N/A。
11. 未预处理、编译、链接、运行仓库代码/脚本/旧夹具、Graphviz、QEMU/实机、真实并发或内存释放试验。32个仅数字/布尔假设模型，不构成内核运行。
12. 无聊天围栏降级分发，N/A；原分支GitHub六blob/一个tree/一个单父commit。
13. D00三成员及do_group_exit、A分配/复制/计数、B栈取得/状态门/真实释放请求、C借用/持有差异与失败/退出毁灭链的全部强制范围都有新直接源码短锚；不把注释当活动调用。

## G04写入前提、重试与停止

唯一写入仅`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-storage-lifetime-74/`六件及CHAT-EXEC-09本批唯一PR17正式顶层交付评论。旧文件、旧评论、PR17标题正文/Draft、lead/PR16、master/time都冻结。

发布前再核执行HEAD仍等于旧完整`8bc7583e45c9`对应实际GitHub对象且74目录不存在、正式marker为零；从原父tree造六blob新tree、单父commit，以`expected_sha`及`force=false`移动当前原分支。任何暂态异常先按63号核远端是否已成功，间隔安全重试最多首次+3；冲突/未知并行HEAD不覆盖，不重复创建已成功评论。单个Code Mode调用应拆成适量工具请求，不把调用数限制当GitHub写权限失败。

严格compare原父→新提交应一提交、六个added，旧件0改删；新固定commit和当前分支六件全文/Blob身份双回读、两PR与source refs及唯一评论核对。G04真实结果/重试/提交ID只在本人最终唯一PR17评论中填写，成文时不提前冒充已发布。`STOP_WRITING`只有确认六件和评论后成立。执行状态为`READY_FOR_MAINLINE_REVIEW`，`mainline_acceptance=PENDING`；C05独立审查后才能关闭R73-01/判74，当前不会启动75、修改内核、运行/编译、合并或进入阶段3。
