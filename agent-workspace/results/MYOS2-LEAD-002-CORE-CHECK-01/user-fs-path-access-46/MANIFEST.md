---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FS-PATH-ACCESS-46
record_type: executor_manifest_and_bounded_static_receipt
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
executor_display_name: MYOS2-A-C04b 代码分析执行
produced_by: ChatGPT普通对话（Owner指定GPT-6＋Extra；实际后台身份未独立认证）
product_surface: ChatGPT普通对话／GitHub连接器
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-06
read_channel: connector
base_snapshot: time（固定内核源码分支）
source_pin_short12: a039d9803ade
contract_short12: 73454015b3e5
result_base_short12: 207c87cda0e6
write_branch: claude/dazzling-cori-q0dnyt
write_pr: 17
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  options_flags_cmake: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  panic_c: "\tthis_cpu = smp_processor_id();"
self_check:
  verified_claims: 59
  quotes_reconfirmed: 59
  downgraded_to_inferred: 0
write_retry_log:
  policy: 63-github-write-retry-and-readback.md
  status_at_manifest_preparation: not_needed
  actual_github_transient_failures_during_preparation: 0
  preparation_note: some local in-memory evidence/model validation assertions were corrected before successful blob creation; no GitHub branch write happened in those failed drafts
  remote_ref_and_delivery_comment_results: record_only_after_actual_actions_in_PR17_delivery_comment
---

# 第46批｜root/pwd getter、setter及路径引用直接体取证入口

本批沿用已完成67接收的当前C04b/CHAT-EXEC-05，不新开执行对话或PR/分支，不接管C04主线工作。MyOS2本次Owner授权、00工作令、公约及72固定任务书是业务执行源；此前加载Mnemosyne只保留适用行为约束，不授予跨项目任务或代码修复权。Owner声明GPT-6＋Extra，连接器不能确认当前后台真实模型与推理档位。第45批主线已有`PASS_PENDING_LOCAL`与`returned_scope=[]`，不由执行方重新判定；本批仅申报待独立验收。

## P00｜明确新读取及前期指导继承

**本轮主线固定73454015b3e5全文实际读取：**
- `agent-workspace/lead/MYOS2-LEAD-002/72-user-fs-path-access-contract.md`；
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FS-CONTEXT-45-review.md`，含强制合读的**R45-S01/R45-S02**；它对dget/dput无活动计数操作、路径值与调用请求及UV-10附带叙述收窄是本批先行约束；
- `agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md`。

**本轮增量实读旧证据：**第45批PR17执行交付评论6675892248、主线46同轮发布评论6676256145；第45批FS45-F01/FS45-F02及对应facts中的path_get/path_put已读模型；第31批PI31-F05及第05批UV-10历史延期条目。旧问题、原合并边界和R31/R45补充不被改号、升级或关闭，不冒称本轮重审所有路径/文件表问题。

**本轮固定time直接源码读取：** `mykernel/include/linux/fs/path.h`的path_s，`mykernel/include/linux/fs/dcache.h`中dentry_s真实定义与完整dget，`mykernel/fs/vfs/dcache.c`的完整dput，`mykernel/fs/vfs/namei.c`的完整path_get/path_put，`mykernel/sched/misc/fs_struct.h`的get_fs_root/get_fs_pwd及DEBUG/FS_STRUCT_DEFINATION选择上下文，`mykernel/sched/misc/fs_struct.c`的set_fs_root/set_fs_pwd与45已读复制/释放周边，`mykernel/sched/misc/fs_struct_types.h`的root/pwd字段，以及`mykernel/fs/vfs/namespace.c`的init_mount_tree单真实setter调用场景，`mykernel/include/linux/compiler/myos_debug_option.h`的PREFIX_STATIC_INLINE条件上下文。直接引文覆盖9个文件，每份固定源连续两轮连接器全文读取同位校对；另为预检读取time/master的两项金丝雀。GitHub默认分支代码搜索未找到getter独立调用者，**这不是对固定time全仓的无调用者证明**。

**前期已完成并沿用、没有重新全文加载的指导：**C04b第41批67交接、MyOS2 00-work-order、agent-workspace/README.md、conventions.md、tasks/00-gpt-task-protocol-v2.md P1–P13、18/24/28号及适用Mnemosyne指南。本轮为P6规范句和预检重新定点读取conventions.md、双金丝雀，不将它冒称全部规则刷新或跨项目维护授权。

## P00｜现场身份与分支金丝雀

- 固定源码`time@a039d9803ade`；master direct head=e8e60cd19540；主线PR16/agent/MYOS2-LEAD-002=73454015b3e5；原执行PR17/claude/dazzling-cori-q0dnyt写前预期**207c87cda0e6**。两PR均Open/unmerged，PR17 Draft；开放PR只有16和17。主线PR16本轮185个变更路径与46唯一新目录零交集。第46批目录在预期父提交中不存在（预期404，不是仓库权限拒绝）；已连接GitHub权限可push。
- 固定time逐字金丝雀：

```text
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	this_cpu = smp_processor_id();
```

master对应另用-mcmodel=large/-fPIE及raw_smp_processor_id，不混读分支。不运行预处理或目标构建来假设配置绑定。
- 本轮公约§1第2条原句：

> 2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。

## P01–P04｜六件限定成果及消费顺序

| 新文件 | 内部数据及复核边界 |
|---|---|
| `fs-path-access.md` | 先读：getter/setter的精确调用顺序、path值复制、path_get/put请求和dget/dput实际空操作、旧值归还门、真实init_mount_tree调用者 |
| `facts-and-boundaries.yaml` | YAML1.2兼容JSON：59来源标签、八个函数契约、44个getter/setter/caller/helper有限模型；四层效果分别标记 |
| `fs-path-access-paths.dot` | 53显式节点、62边；CALL/RETURN、源码门、字段值、分析前提、STOP与真实调用者按上下文匹配 |
| `deferred-findings.yaml` | 0项新非同义问题；FP46-O01..O04四项有界观察，旧FS45-F01/F02、PI31-F05、UV-10的qualified_id/status不变 |
| `evidence.md` | 59段每段1–5行、固定time来源与行号/单函数或结构体身份；两轮连接器同位逐字核对 |
| `MANIFEST.md` | 本入口、来源、P6与金丝雀、模型/图核验、真实GitHub重试及STOP边界 |

**必须保持的四层证据：**VALUE_COPY（只复制mnt/dentry指针值）→CALL_REQUEST（按真实源码时点调用path_get/put）→DIRECT_EFFECT（path_get→dget只return、path_put→dput空体、mnt操作注释，无所读计数增减）→OBJECT_LIFETIME（完整引用来源、其它持有者、销毁和物理状态均未知）。输入为有效path_s对象但dentry=NULL与path=NULL不等价；非NULL指针本身不保证结构体/目标合法。getter输出如果与另一内部path字段别名、setter输入与fs内部path同址均只属helper条件模型，未证明真实调用者如此操作。

## 自查三数、结构与NOT_RUN

```yaml
self_check:
  verified_claims: 59
  quotes_reconfirmed: 59
  downgraded_to_inferred: 0
  scope: evidence.md/FP46-A001..FP46-A059
  method: 固定time九个源文件经连接器两轮全文读取、1–5行同位核对、函数/类型身份检查
additional_checks:
  facts_JSON_parse: PASS
  deferred_JSON_parse: PASS
  finite_models: 44
  graph_explicit_nodes: 53
  graph_edges: 62
  graph_undefined_endpoints: 0
  Graphviz_render: NOT_RUN
  external_YAML_parser: NOT_RUN
  full_local_P9: NOT_RUN
  raw_source_file_byte_materialization: NOT_RUN
  C_preprocessing_compilation: NOT_RUN
  repository_C_ASM_scripts_QEMU_hardware: NOT_RUN
  real_path_mount_dentry_destroy_or_allocator_lifetime: NOT_RUN
  extra_paid_API_or_model: NOT_USED
```

59/59只是固定源码引文的作者静态自查，不是59个独立问题、自动完结P9或内核运行验收。重复片段、注释与条件宏有独立限制；图中分析STOP不是CPU错误动作或源码errno。dget/dput体内无活动引用操作不能推导全系统无其它资源持有机制或实际已有泄漏/悬空。

## 63号GitHub写入重试与STOP_WRITING

唯一授权写区`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fs-path-access-46/`的六件及本人在既有PR17上的**一条新正式交付评论**；所有旧文件、旧评论、lead/PR16、master/time、PR17标题/正文/Draft均冻结，不创建竞争分支/PR，不强推/合并/关闭/删除。Git提交基于expected-head=207c87cda0e6、force=false；远端变更冲突时停止而非自动变基/覆盖。暂态写故障先回读分支/目标文件/评论判断是否已生效，再依63号间隔安全重试并复用已得blob/tree/commit；没有发生故障则记录not_needed。**实际新提交、compare、固定提交/活动分支六件全文与blob核验、本人评论ID与回读结果只能在真实发生后写本人正式评论，不在提前创建的MANIFEST中预言成功。**

无内核/用户程序修改、补丁、编译运行或阶段3。只申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING；正式提交及本人评论完成远端核验后立刻STOP_WRITING，由「MYOS2-A-C04 内核分析主线续建」独立验收，不自行启动第47批。
