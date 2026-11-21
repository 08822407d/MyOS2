---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FILES-COPY-SHARE-43
record_type: executor_delivery_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
executor_display_name: MYOS2-A-C04b 代码分析执行
produced_by: ChatGPT普通对话（Owner选择GPT-6＋Extra，不能独立证明后端）
product_surface: ChatGPT普通对话/GitHub连接器
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-05
read_channel: connector
base_snapshot: time（分支名）
source_pin_short12: a039d9803ade
contract_short12: 47bfd0c93b85
result_base_short12: 6303ec8e9899
canonical_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
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
  verified_claims: 49
  quotes_reconfirmed: 49
  downgraded_to_inferred: 0
write_retry_log:
  rule_ref: 63-github-write-retry-and-readback.md
  status_at_manifest_creation: not_needed
  observed_transient_github_write_errors_before_branch_commit: 0
  actual_branch_commit_and_comment_results: see_postwrite_PR17_comment
---

# CORE-USER-FILES-COPY-SHARE-43 · 第43批六件交付总入口

本轮沿用现有「MYOS2-A-C04b 代码分析执行」及CHAT-EXEC-05身份，不新建对话、分支、PR或跨项目任务。当前Owner指令、MyOS2 00工作令、本批69号和既有本项目公约是执行权威；Mnemosyne仅为已装载的行为指导，不能替换MyOS2业务权限、改派任务或接管Mnemosyne维护。本轮仅申请READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，正式裁定留给独立C04主线。

## T00：本轮实际重新读取与本对话已加载材料

**本轮在主线固定提交47bfd0c93b85全文重新读取：**
- `agent-workspace/lead/MYOS2-LEAD-002/69-user-files-copy-share-contract.md`；
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FD-CLOSE-42-review.md`，其中R42-S01、R42-S02是本批所有局部模型的强制消费限定；
- `agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md`。

**本轮回读定点旧材料：**第42批正式评论6654442468及其后主线42裁定评论6655050664；固定执行6303ec8e9899的CL42-F01、第05批UV-09/UV-10/UV-11、第08批US-01/US-03及41有关引用/对象界限；相关旧R40/R41和主线审查状态按实际已读历史继续合读，不冒称本轮重做40–42或全部旧证据。

**沿用已在本C04b第41批实际加载、但本轮没有重新全文阅读的指导：**67号接收、MyOS2 00-work-order、agent-workspace/README.md、conventions.md、tasks/00-gpt-task-protocol-v2.md P1–P13、18/24/28，以及适用Mnemosyne唯一执行源和相应guards。本轮为了机械自检额外定点重读了conventions.md §1第2条和两份金丝雀，未将它称为全套规则刷新。

**本轮固定time源码直接阅读：**init/init_task.c的init_task定义；sched/forkexec/fork.c的dup_task_struct、copy_process调用周边、copy_files、unshare_fd、unshare_files、files_cachep/proc_caches_init；fs/vfs/file.c的dup_fd、init_files与put_files_struct定点；include/linux/kernel/fdtable.h、fs/fs.h、uapi/linux/sched.h、kernel/types.h最小必要定义；sched/forkexec/exec.c的begin_new_exec调用周边；mm/kmalloc/slub.h的kzalloc包装定义。源文件直接按固定time读取两遍供本批引文核对；不使用master同名源码替代或完整审计fork/exec/exit/slab。

## T00：身份、金丝雀、仓库权限及单写者预检

- 源码time仍为a039d9803ade；master= e8e60cd19540；主线PR16 HEAD=47bfd0c93b85；首次执行写前PR17 HEAD预期6303ec8e9899。两PR均Open/unmerged，PR17 Draft；开放PR恰16/17。PR16当时176个变化路径与本批新目录零交集。第43批目录在预期父提交下404（预期的不存在，不是认证故障）。只复用原PR17分支，不动PR16。
- time双金丝雀逐字为：

```text
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	this_cpu = smp_processor_id();
```

master在相应位置为`-mcmodel=large/-fPIE`及`this_cpu = raw_smp_processor_id();`，故可判别错误分支；未执行预处理、CMake或实际目标配置组合。

- 公约P6指定整句本轮精确回读：

> 2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。

## T01–T04：六件入口、数据范围和限定成果

| 文件（仅新增本目录） | 主要用途 |
|---|---|
| files-copy-share.md | 先读：静态init_files、任务结构预置指针、共享/复制、逐槽file_s构造、unshare发布及错误前缀 |
| facts-and-boundaries.yaml | JSON兼容YAML：七组身份、五组函数契约、34项有限输入/计数/边界模型，49个证据ID |
| files-copy-share-paths.dot | 59显式节点、68边、零未定义端点；真实控制/CALL/RETURN与分析前提/数据别名分开 |
| deferred-findings.yaml | 唯一新增普通候选FT43-F01（内层kzalloc返回NULL未检）；原UV/US/CL42状态不改，CL42-F01只补已证来源与未证最终归零 |
| evidence.md | 49个1–5行固定time定义体摘录；第二轮源码逐字和符号身份核验 |
| MANIFEST.md | 本入口、真实读取、继承、固定金丝雀、P6/安全写入及停止状态 |

**关键限定：**`copy_files`共享分支的tsk.files地址要由先前`*tsk=*orig`及有效任务对象推得，不能虚构当前分支显式赋值；`dup_fd`中new_fds旧槽NULL时的内容在本选定函数内未写，不据此认定实际脏值；`max_fds`与fd_count、256物理槽各是不同量。`unshare_fd`在无需复制时不写输出，本调用者先令copy=NULL；`unshare_files`的新表发布在put旧表之前，不由名字推出原子事务或完整回滚。非法数组、NULL内层分配或无效对象处只报最早失效边界。静态init_task.files=&init_files已证，实机最后归零与CL42-F01未闭合。

## P6自查、证据级别和未运行范围

```yaml
self_check:
  verified_claims: 49
  quotes_reconfirmed: 49
  downgraded_to_inferred: 0
  counted_scope: evidence.md/FT43-A001..A049
  method: 固定time的9个实际源码文件通过GitHub连接器先后两轮全文读取，逐段1–5行及所属真实符号检查
  semantic_check: only bounded claims, not machine execution or exact compiler normalization
additional_checks:
  facts_and_deferred_JSON_parse: PASS
  source_quote_ids: 49
  finite_models: 34
  DOT_nodes: 59
  DOT_edges: 68
  DOT_undefined_endpoints: 0
  Graphviz_render: NOT_RUN
  external_YAML_parser: NOT_RUN
  whole_P9_local_gate: NOT_RUN
  real_compiler_or_CPP: NOT_RUN
  kernel_C_ASM_shell_build_QEMU_hardware: NOT_RUN
  real_fork_exec_unshare_exit_or_allocator_run: NOT_RUN
  paid_external_model_or_API: NOT_USED
```

49段证据是重复确认的静态片段，不是49个独立根因或已认证C/CPU语义。完整远端固定提交与活动分支6/6 blob/文本回读、compare净变更和正式交付评论ID，必须在**实际发布后**由PR17本人评论登记，不能在本未提交的MANIFEST预先声称完成。GitHub暂态错误遵循63号：先核远端是否已经成功、再间隔多轮安全重试、复用既得对象和expected-head/force=false；无暂态时记not_needed，工具发现次数和实际写请求分开。

## 执行写区、下一门和STOP_WRITING

唯一可写区`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-files-copy-share-43/`恰六件，另只允许本人在原PR17追加正式交付评论。原批次/他人评论、lead/PR16、master/time及PR17标题/body/Draft全部冻结。禁修内核、补丁、构建运行、仓库C/ASM/脚本、额外付费API、分支PR新建/合并/关闭/强推/删除、跨项目维护或阶段3。结果交C04依据69号及R42-S01/S02独立验收；正式评论及回读完成后执行方STOP_WRITING，不擅自开始44。
