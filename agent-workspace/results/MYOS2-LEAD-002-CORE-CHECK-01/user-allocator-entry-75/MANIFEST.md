---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ALLOCATOR-ENTRY-75
record_type: bounded_executor_delivery_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-09
executor_display_name: MYOS2-A-C05c 代码分析执行
produced_by: GPT-6 (self-reported; backend not independently attested)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
model_tier_per_owner: 次一档
backend_identity: unknown_or_not_attestable
date: 2026-11-12
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract_pin_short12: 1ba6acf132a0
initial_execution_head_short12: 6f792023d592
lead_preflight_short12: 1ba6acf132a0
master_preflight_short12: e8e60cd19540
execution_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
lead_branch: agent/MYOS2-LEAD-002
lead_pr: 16
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-allocator-entry-75/
allowed_files: [MANIFEST.md, allocator-entry.md, facts-and-boundaries.yaml, allocator-entry-paths.dot, deferred-findings.yaml, evidence.md]
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  options_flags_cmake: |-
    	-m64 -mcmodel=kernel -fno-pie -fno-pic \
  panic_c: |-
    	this_cpu = smp_processor_id();
branch_canary_master_difference:
  options_flags_cmake: target_time_literal_absent_in_master
  panic_c: "this_cpu = raw_smp_processor_id();"
prior_R73_01: CLOSED_DOCUMENTATION_ONLY_BY_C05
prior_batch73: PASS_PENDING_LOCAL_WITH_R73C-S01_AND_R73-S01/S02/S03
prior_batch74: RETURN_DOCUMENTATION_ONLY_R74-01
D00_status: documentation_disposition_submitted
R74_01_closure: PENDING_C05_INDEPENDENT_REVIEW
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
self_check:
  verified_claims: 87
  quotes_reconfirmed: 87
  downgraded_to_inferred: 0
  anchors_D00: 5
  anchors_A: 31
  anchors_B: 26
  anchors_C: 25
  direct_and_nearest_source_ledger_entries: 14
  pure_finite_model_cases: 39
  size_class_boundary_cases: 28
  new_conditional_findings: 0
  new_evidence_gaps: 8
  graph_nodes: 65
  graph_edges: 77
  graph_undefined_endpoints: 0
  full40_hex_sha_strings: 0
  full_local_P9: NOT_RUN
  Graphviz: NOT_RUN
write_retry_log:
  logical_operation: prepare_six_blobs_then_one_tree_single_parent_commit_nonforced_expected_head_update_and_one_formal_comment
  attempts_in_manifest: publication_not_yet_requested_at_blob_creation
  actual_transient_errors_before_manifest: none
  expected_ref_before_write_short12: 6f792023d592
  remote_effect_at_G00: target_directory_absent_and_formal_marker_count_0
  retry_policy: recheck_remote_before_delayed_retry; never_overwrite_unknown_concurrent_ref
  final_readback: to_be_reported_in_sole_postpublication_PR17_comment
owner_decisions_requested_now: []
kernel_change_authorized: false
patch_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
---

# 第75批｜C05c G00、D00及三个分配器入口专题交付清单

## G00固定来源与身份

沿用本有效C05c/CHAT-EXEC-09，在Owner本轮明确转发固定107号后启动。现有同一会话早已独立接收105交接、完整读取九份MyOS2工作规则、另行载入适用Mnemosyne指导；新任务不重新轮转执行者、不复用其他聊天的已读声明、不重开PR/分支、不维护其它项目。此轮重新完整取得固定`1ba6acf132a0`中的`107-user-allocator-entry-contract.md`、`reviews/CORE-USER-EXIT-REAP-73-DOC-CLOSURE-review.md`、`reviews/CORE-USER-TASK-STORAGE-LIFETIME-74-review.md`以及`checkpoints/2026-11-12-task-storage74-reviewed-allocator75-ready.md`；同步完整刷新主线63号安全重试、79号合包与`agent-workspace/tasks/00-gpt-task-protocol-v2.md`全部P1–P13。

直接G00核对：`time@a039d9803ade`、`master@e8e60cd19540`、主线/PR16`1ba6acf132a0`、执行/PR17`6f792023d592`；两PR均Open且未合并，PR17保持Draft，开放PR仅16和17。目标75目录首次不存在、本人75正式comment marker为0，新主线107发布未改变执行HEAD。本MANIFEST头部真实`branch_canary_quotes`取自固定time两物理原行；master不存在time的options_flags精确行，panic改为raw_smp_processor_id。项目公约§1第2条逐字自检原句、`read_channel: connector`与真实模型来源均在头部。G00通过只代表租约和范围门，不认证运行成功。

第74批固定成果`6f792023d592`：`MANIFEST.md`、`task-storage-lifetime.md`、`deferred-findings.yaml`全文，以及facts中`prior_return_disposition`、`task_allocation_matrix`、`stack_lifetime_matrix`、`destroy_caller_matrix`、M14/M23、证据B12及必要A/B/C旧锚，正式PR17评论6812545638已读。主线73独立文档闭合`R73C-S01`及74的`R74-S01/S02/S03`全文合读。原73的R73-S01/S02/S03维持，73组合`PASS_PENDING_LOCAL`，R73-01已独立关闭、**不再启动纠错**。固定04的mm.kmalloc/§3.1与MC-07/08、DR-04/RF-02，05对MR-01/02的闭合以及关联历史deferred状态，均定位并按后续限定消费；旧04总括不是当前完整内核真值，也不执行旧检查脚本。

## D00｜R74-01原件冻结、追加纠正

旧74 B12以`THREAD_SIZE`为标签却直接引`THREAD_SIZE_ORDER`宏定义，违反单一真实定义归属；本批D01重锚真实`THREAD_SIZE_ORDER`，D02独立锚`THREAD_SIZE`，D03/D04分别锚`PAGE_SIZE`/`PAGE_SHIFT`，D05独立锚定C分支`_AC`的token-paste定义。五个宏分别在自己的定义内，固定物理行1–5、非空。来源表达式推导`(1UL<<12)<<3=32768`只限于有效C源码整数模型，非编译或实测值。

新`facts.prior_return_disposition`明确传播到旧74的`task-storage-lifetime.md::G02`、`facts.stack_lifetime_matrix[storage_size]`、有限模型`TSL74-M14`、DOT`K48`以及旧MANIFEST/body/facts/evidence引言和正式评论6812545638的“74/74均全定义合格”表述。74原引文标签数仍74，其中B12错误、其余73条没有这次全量重审；不能把当前5条新增宏锚算成“旧74原件都已正确”。旧六件、旧评论、PR标题/Draft等元数据一字不改。`D00=documentation_disposition_submitted`，只有C05能裁定关闭R74-01；R73-01独立闭合维持。

## G01/G02/G03限定研究

- **A completed_in_scope：** `kmalloc`按`KMALLOC_MAX_CACHE_SIZE=PAGE_SIZE`的4096模型分缓存与大块；`__kmalloc`直接越界循环（旧MC-09）、`kmalloc_slab`零尺寸/1–192特殊索引/193–4096 `fls`、`kmalloc_caches`与`kmalloc_info`定义及`create_kmalloc_caches`写入门；8192号表槽不等于普通kmalloc路由；task大小`sizeof(task_s)`保持未知，stack宏投影32768进入大块。直接callee接到`alloc_pages`但不审buddy全链。
- **B completed_in_scope：** `kzalloc`传播`__GFP_ZERO`，`slab_alloc`非空后才调用hook按`s->object_size`清零；大块`kmalloc_large`在页有效且ZERO时只在当前本体按`size`清零；NULL缓存、NULL页与返回/不返回callee分开，未认证底层页尾内容。task与stack两种原请求分开。
- **C completed_in_scope：** `kfree(NULL)`源级早退，非空参数先`virt_to_folio`再`folio_test_slab`判分到`kmem_cache_free`或`free_large_kmalloc`；直接cache free不借用kfree(NULL)门；`folio_order==0`警告之后仍有`__free_pages`请求；有效对象来源、cache/folio/order、最后持有与真实物理释放均为边界。旧MC-07/08/DR-04/RF-02及TSL74-U04保留原号不自关。

交付六件固定：`MANIFEST.md`本清单；`allocator-entry.md`四段自然语言事实；`facts-and-boundaries.yaml`有prior_return_disposition、14份固定源码台账、六要求矩阵、39个纯有限模型及历史链接；`allocator-entry-paths.dot`有65显式节点/77分型边/0悬空；`deferred-findings.yaml`本批仅AE75-U01..U08八缺口、无新增独立根因；`evidence.md`中D5+A31+B26+C25共**87条**单定义连续1–5行非空原文。全部14份对应固定time文件再独立取得，逐锚字面与单定义归属核对87/87，无降级。完整本地P9、Graphviz、构建或实机均NOT_RUN。默认分支搜索只做有界定位，全部真实语义回固定time。

## 协议P9条目实账（完整本地检查仍NOT_RUN）

1. 六件原始文本不含任何完整40位十六进制commit串（最终发布前逐件再检）。
2. 新87条短锚在固定time的同一函数/宏/结构/全局定义内、非空、1–5连续原样物理行，二轮源码回读87条逐字命中。
3. 写回在原分支单父提交后，以固定commit和活动分支各全文/Blob回读六件，结果由正式交付评论登记。
4. 行号只是固定源码实际行辅助索引，不替代路径::真实symbol。
5. 六件原始标签前缀总数应87，头部`verified_claims/quotes_reconfirmed/downgraded_to_inferred = 87/87/0`；D00新5段一并计入，旧74的74条不混算。
6. 没有依赖外部标准、通用Linux经验或工具命令来补MyOS2实现缺口。
7. 头部真实time双金丝雀各在time原文件命中，master对应不等同。
8. 头部真实公约唯一写区整句和读取通道存在。
9. 无内核代码新增、头文件/符号引用代码件：N/A。
10. 无patch/修改块，`git apply`：N/A。
11. 没有C/ASM预处理、编译、链接或运行，未执行仓库脚本/旧夹具、QEMU/真实机、完整P9或Graphviz；39个纯数值/布尔模型不算内核运行。
12. 无对话降级文件分发，N/A。
13. D00四宏和_AC各自定义、A尺寸比较和index/callee、B清零位/真正memset长度/失败门、C NULL/folio分类/释放直接callee，都有对应真正单定义短锚，禁止跨宏拼段或将注释当活动CALL。

## G04单写者、重试、核验和停止边界

仅允许原执行分支`claude/dazzling-cori-q0dnyt`/PR17新增`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-allocator-entry-75/`六件与09本批唯一正式顶层PR17评论。其他历史成果、旧评论、PR16/17元数据、主线lead、master/time冻结。首次真实分支推进前重新核旧执行完整HEAD以`6f792023d592`开头且目录/marker仍不存在；从原父tree/六个blob造一个单父commit，使用GitHub`expected_sha=旧完整HEAD`、`force=false`。未知并行变更停止涉及写动作不覆盖；暂态异常依63号先读远端是否成功，间隔安全重试通常首次+三轮，遵Retry-After。已得到blob/tree/commit须复用、已存在评论不得重复创建；真实请求次数与单纯工具发现/预检分别登记。

新固定提交与活动分支六件全文/blob双回读合12次；对旧HEAD比较必须恰一个提交、六件added且旧文件0改删；再核direct HEAD、PR16/17状态、唯一正式comment。真实commit/comment与重试及后验只在正式评论填写，不提前伪造。完成交回`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`并`STOP_WRITING`。R74-01由C05独立关闭，第75批不自签，不启动76；无内核修改、补丁、编译运行、跨仓维护、新分支PR、合并或阶段3。
