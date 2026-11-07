---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FD-CLOSE-42
record_type: bounded_close_executor_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
executor_display_name: MYOS2-A-C04b 代码分析执行
produced_by: ChatGPT普通对话（Owner声明GPT-6＋Extra；后台身份未独立证明）
product_surface: ChatGPT普通对话／GitHub连接器
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-05
read_channel: connector
base_snapshot: time（分支名）
source_pin_short12: a039d9803ade
contract_short12: d7d7477c3a15
result_base_short12: ee0882d46bbf
write_branch: claude/dazzling-cori-q0dnyt
write_pr: 17
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
kernel_change_authorized: false
merge_authorized: false
phase3_authorized: false
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  options_flags_cmake: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  panic_c: "\tthis_cpu = smp_processor_id();"
self_check:
  verified_claims: 40
  quotes_reconfirmed: 40
  downgraded_to_inferred: 0
write_retry_log:
  policy: 63-github-write-retry-and-readback.md
  status_before_final_commit: not_needed
  observed_transient_write_errors_before_final_commit: 0
  actual_ref_and_comment_results: see_actual_postwrite_PR17_comment
---

# CORE-USER-FD-CLOSE-42｜执行交付总入口

第42批由本对话已接收的CHAT-EXEC-05继续；不新建执行对话/分支/PR，不重新进行67号接收。MyOS2直接授权为本轮Owner指令、00工作令和固定68任务书；此前MyOS2公约/协议v2、18/24/28及适用Mnemosyne行为约束均由同对话第41批实际加载，**本次沿用不是声称重新全文阅读或取得跨项目权限**。源和PR状态在本批重新核对；后续只交C04主线独立验收。

## C00 新读取与继承资料分开

**本轮重新全文读取：**固定d7d7477c3a15的68-user-fd-close-contract.md、reviews/CORE-USER-FD-LOOKUP-REF-41-review.md（含R41-S01/S02）、63-github-write-retry-and-readback.md。回读41交付评论6652894020及后续主线评论6653854840、41的引用矩阵，定点读取05::UV-09/10/11和08::US-01，按任务引用历史40 R40-S01/S02及更早38/39约束，不再次修改或重新验收。

**继承且未重新全文加载：**第41批的67接收报告、MyOS2 00-work-order.md、agent-workspace/README.md、conventions.md、tasks/00-gpt-task-protocol-v2.md（P1–P13）、18/24/28号、Mnemosyne README/唯一执行源及适用指导。在本轮为了引文又定点重新读取conventions.md §1第2条和双金丝雀，但不将此冒充整个项目规则刷新。MyOS2规则优先，Mnemosyne仅为行为指导。无Claude/Meta-Agent/Alaya维护任务导入，实际后台模型unknown。

**固定time直接读源：**mykernel/fs/syscall.c::close；fs/vfs/open.c::filp_close；fs/vfs/file_table.c::fput/__alloc_file/files_init；fs/vfs/file.c::put_files_struct/exit_files/dup_fd/init_files/f_dupfd；include/linux/kernel/fdtable.h::files_struct_s/NR_OPEN_DEFAULT；include/linux/fs/fs.h::file_s/get_file；mm/kmalloc/slub.h::kfree选定定义；sched/forkexec/fork.c::files_cachep/proc_caches_init/copy_files；sched/forkexec/exit.c::do_exit定点入口。未对整仓fork/exit或完整分配器做源码审计。

**现场预检通过：**固定source为time@a039d9803ade，master为e8e60cd19540；发布主线HEAD=d7d7477c3a15；写前执行预期PR17=ee0882d46bbf。PR16与PR17均open/unmerged、PR17 Draft，开放PR仅16/17；PR16更改路径与本次目录无交集；目标user-fd-close-42目录原来不存在。预期目录404并非仓库拒绝/认证失败；旧文件冻结。

**两项金丝雀全文原行（固定time；master相应文本不同）：**
```text
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	this_cpu = smp_processor_id();
```

**写入公约§1第2条精确原句：**

> 2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。

## C01–C04 成果与阅读次序

| 文件 | 用途 |
|---|---|
| fd-close.md | C01 close无符号比较、空槽及kfree前后；C02 file与files两种计数；C03退出文件表及别名模型，列出停止点 |
| facts-and-boundaries.yaml | JSON兼容YAML，六组活动函数、各来源缓存/类型、24个有前提的有限模型 |
| fd-close-paths.dot | 48个显式节点、51边；区分实际源码控制/调用、数据、分析前提及无证止点 |
| deferred-findings.yaml | 本批仅一个有条件新候选CL42-F01；旧UV-09/10/11、US-01与FG41仍按原qualified_id登记，不重编号或关闭 |
| evidence.md | 40段单定义体1–5行固定time逐字引文，两轮连接器文本对照；上下文选择宏/注释另列 |
| MANIFEST.md | 本入口、读取来源、双金丝雀、P6和停写边界 |

## 静态自检 / 验证等级

```yaml
self_check:
  verified_claims: 40
  quotes_reconfirmed: 40
  downgraded_to_inferred: 0
  counted_scope: evidence.md中CL42-S001至S040
  quote_check: two independent connector reads of 9 fixed-time source files; exact source lines, function identity and claim-support reviewed
  overlapping_slices_not_independent_facts: true
additional_checks:
  facts_JSON_parse: PASS
  deferred_JSON_parse: PASS
  DOT_explicit_nodes: 48
  DOT_edges: 51
  DOT_undefined_endpoints: 0
  Graphviz_render: NOT_RUN
  external_YAML_parser: NOT_RUN
  full_local_P9: NOT_RUN
  original_user_input_bytes_materialized: NOT_REQUIRED
  source_preprocessor_or_compiler: NOT_RUN
  kernel_C_ASM_repo_script_QEMU_hardware: NOT_RUN
  actual_fd_file_table_or_slab_runtime: NOT_RUN
  extra_paid_API_or_model: NOT_USED
```

40段引文只是固定文本/符号匹配，不能据数量认定40个独立语义正确性证明。所有局部整数/引用状态模型只假定有效对象、正初始计数及原子操作正常、没有其他写者/opaque副作用；若分支/对象前提不满足，在首个失效点停止，不以代码顺序冒充运行必然结果。对于kfree的#if选择未运行预处理，不把NULL早退概括为所有目标配置/输入均已证安全；对kmem_cache_free调用不宣称真实物理释放。

## 63号安全写入及停止

本批允许唯一变更`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fd-close-42/`中的六件、原PR17上的本人新进度/交付评论。实际Git对象通过create_blob/create_tree/create_commit创建后，移动原PR17 ref时必须先回读expected-head并用force=false快进；未知并行写不覆盖。一次GitHub暂态故障先查远端真实结果，再按63号有间隔安全重试并复用已有对象，遇真实权限/安全冲突停受影响动作。对本轮写动作的真实重试次数、固定commit/活动分支六件回读、compare及本人交付评论ID在**实际发生后**写PR17评论，本MANIFEST不预言SHA或成功。

只在主线独立验收前申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING。不得修改PR标题/body/Draft、lead/PR16/master/time、旧结果/他人评论；不得新建分支PR或合并关闭/强推/删分支。不修内核、不产生补丁、不构建运行、不进入阶段3、不自行启动第43批。正式交付与评论远端确认后STOP_WRITING。
