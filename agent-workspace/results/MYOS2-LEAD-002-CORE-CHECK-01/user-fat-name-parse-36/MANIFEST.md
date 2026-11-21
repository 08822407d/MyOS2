---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FAT-NAME-PARSE-36
record_type: bounded_executor_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-04
executor_display_name: MYOS2-A-C04a 代码分析执行
mainline_owner: MYOS2-A-C04 内核分析主线续建
produced_by: unknown
product_surface: ChatGPT普通对话
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-04
read_channel: connector
base_snapshot: time
source_pin_short12: a039d9803ade
execution_branch: claude/dazzling-cori-q0dnyt
execution_base_short12: a7cb8bf8d2e6
lead_branch: agent/MYOS2-LEAD-002
lead_observed_short12: d5b6bf140e03
master_observed_short12: e8e60cd19540
contract60_short12: d5b6bf140e03
review35_short12: d5b6bf140e03
pr_execution: 17
pr_mainline: 16
prior_review_verdict: PASS_PENDING_LOCAL
prior_returned_scope: []
mandatory_review_supplements: [R35-S01, R35-S02]
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fat-name-parse-36/
allowed_files: [MANIFEST.md, fat-name-parse.md, facts-and-boundaries.yaml, fat-name-parse-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen_since: a7cb8bf8d2e6
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
owner_decisions_requested_now: []
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  options_flags_cmake: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  panic_c: "\tthis_cpu = smp_processor_id();"
self_check:
  verified_claims: 56
  quotes_reconfirmed: 56
  downgraded_to_inferred: 0
  verified_source_files: 9
  quote_width_1_to_5: true
  typed_object_records: 27
  finite_byte_and_state_cases: 20
  deferred_new_candidates: 8
  deferred_historical_links: 9
  graph_explicit_nodes: 45
  graph_edges: 65
  graph_undefined_endpoints: 0
  complete_40hex_SHA_count: 0
  delivery_files_expected: 6
  source_C_ASM_repo_scripts_run: false
  preprocessor_or_compiler_run: false
  full_local_P9: NOT_RUN
inputs_read_current_batch:
  - "agent-workspace/lead/MYOS2-LEAD-002/60-user-fat-name-parse-contract.md @ d5b6bf140e03 (全文)"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FAT-DIR-ITERATOR-35-review.md @ d5b6bf140e03 (全文，R35-S01/S02)"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fat-dir-iterator-35/fat-dir-iterator.md @ a7cb8bf8d2e6 (I02/I03定点，实取全文)"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fat-dir-iterator-35/facts-and-boundaries.yaml @ a7cb8bf8d2e6 (getter/readent/lookup及状态定点，实取全文)"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fat-dir-iterator-35/deferred-findings.yaml @ a7cb8bf8d2e6 (IT35-F01/F02/F05，实取全文)"
  - "PR17 CHAT-EXEC-04第35批本人交付评论 6622471344，远端全文取得"
  - "mykernel/scripts/options_flags.cmake @ time a039d9803ade 和 master (金丝雀)"
  - "mykernel/debug/panic.c @ time a039d9803ade 和 master (金丝雀)"
  - "mykernel/fs/fat/myos_dir.c @ time a039d9803ade (两个解析函数活动定义均全文)"
  - "mykernel/fs/fat/myos_fat32_namei.c @ time a039d9803ade (FAT32_lookup完整定义)"
  - "mykernel/include/linux/fs/fat32.h @ time a039d9803ade (长名属性、小写位、iobuf字段)"
  - "mykernel/include/uapi/linux/msdos_fs.h @ time a039d9803ade (packed短/长目录项、数组、MSDOS_NAME)"
  - "mykernel/lib/string/string.c @ time a039d9803ade (strncmp条件编译门与完整所选函数)"
  - "mykernel/mm/kmalloc/slub.h @ time a039d9803ade (kzalloc清零flag包装)"
  - "mykernel/fs/fat/myos_fat32_iobuf.c @ time a039d9803ade (readent必要完整定义)"
  - "mykernel/include/linux/kernel/err.h @ time a039d9803ade (ERR_PTR/IS_ERR类型化定义)"
  - "mykernel/include/uapi/asm-generic/errno-base.h @ time a039d9803ade (ENOENT/ENOMEM常量)"
  - "GitHub repo权限与public、PR16/PR17、open PR全集、master/time/lead/执行分支、PR16 changed paths、本批目录初始不存在"
inputs_inherited_from_prior_batches:
  - "MyOS2@bf856f2ddb2d: agent-workspace/README.md、conventions.md、tasks/00-gpt-task-protocol-v2.md（P1–P13）"
  - "MyOS2@bf856f2ddb2d: lead/MYOS2-LEAD-002/00-work-order.md、18-deferred-findings-and-resume.md、24-chatgpt-specialist-conversation-separation.md、25-chatgpt-executor-startup.md、28-review-and-next-batch-same-turn.md"
  - "MyOS2@a87124c1cf6c: 57-executor-continuation/01-C04a-receiver-startup-prompt.md、45-c03b-to-c03c-executor-handoff.md（前一批历史补读；本轮未重读）"
  - "MyOS2@a87124c1cf6c: CORE-USER-DENTRY-LOOKUP-33-review.md含R33-S01/S02、以及R34-S01/S02与R31/R32补充（先前已读取，本次只按35回执继承）"
  - "MyOS2@e33103ca7d9e: user-open-flags-30-doc-fix-01/errata.md（此前阅读）"
  - "Mnemosyne@master: README.md、current/human-approved-spec.md、commands/receive-mnemosyne-handoff.md、commands/load-mnemosyne-guidance.md及此前实际载入的单写者、证据、跨对话、交付和源保留适用guard；本批未重载或另开Mnemosyne任务"
open_questions:
  - "真实FAT目录是否提供0x40终止、有非ASCII/畸形槽/错误指针，未证。"
  - "目标构建能否保证u16重解释访问的对齐/别名/字节序与清零分配，未预处理/编译。"
  - "真实长名被映射成什么Unicode或VFAT合法文件名，不做规范/盘IO分析。"
  - "readent局部缓存副作用和资源所有权不等于共享游标修改；完整驱动/回收未验证。"
---

# 第36批 MANIFEST｜限定来源、六件交付、静态自查及STOP

## P00｜输入、角色与现场

固定60号及同提交35号主线回执已完整读取，R35-S01/S02强制合读；35经C04限定接收为PASS_PENDING_LOCAL、returned_scope=[]。本批沿用已接收的C04a CHAT-EXEC-04以及Owner选择的GPT-6＋Extra；不重新进行交接。实际源码固定time`a039d9803ade`，两个金丝雀与master逐字差异已确认。PR17首次写前HEAD `a7cb8bf8d2e6`，保持open/Draft/unmerged；PR16/lead `d5b6bf140e03`，现有150个路径对本批写区无交集。PR16/17是全部开放PR，目录原先不存在。写前预期HEAD后续提交前还须重新检查，绝不掩盖未知并行写入。

MyOS2与Mnemosyne既有规则分别继承；精确当轮输入及历史范围在上面的两个清单分栏，未直接证明的另一对话行为不能认证，后台型号写unknown。用户操作能力和GitHub写能力仅本批所许六件与本人评论可使用，不以可调用工具越过授权。

## P01–P04逐项覆盖

| 步骤 | 当前作者侧完成状态 | 直接内容与必须保留的边界 |
|---|---|---|
| P00 | COMPLETE_INPUT_AND_PREFLIGHT | 固定60及35回执R35-S01/S02、定点旧结果、双金丝雀与PR/写区预检 |
| P01 | COMPLETE_STATIC_CONDITIONAL | short的15字节kzalloc、*namelen=0、8基名/3扩展名、dir分支、dot撤回、大小写位、NUL条件 |
| P02 | COMPLETE_STATIC_CONDITIONAL | long的局部逆向cursor、while先attr再检查cursor、0x40、i*13+1、两遍readent、13个u16筛选但逐单元只拷一字节、额外for更新 |
| P03 | COMPLETE_STATIC_CONDITIONAL | lookup实际选择、caller namelen=0、max再转换size_t、strncmp、kfree在equal前、解析器对共享游标无可见直接赋值 |
| P04 | SIX_BLOBS_PREPARED_FOR_DELIVERY | 引文和有限字节模型、DOT、普通延期记录、局部P9自查；实际提交/双回读写本人PR17评论 |

## 交付的六件和读取顺序

- `MANIFEST.md`：本文件，角色、精确本轮与继承输入、金丝雀、自查、STOP。
- `fat-name-parse.md`：P00–P04源码控制语义、阶段界限、名称/长度/游标判别。
- `facts-and-boundaries.yaml`：JSON兼容YAML1.2，27个有类型对象和20个有限字节及状态模型。
- `fat-name-parse-paths.dot`：45个显式节点、65条边，已区分实际调用、字段数据、条件门及分析前提；默认graph/node属性不作实体节点。
- `deferred-findings.yaml`：NP36-F01..F08八个条件候选、9个历史qualified_id消费链接，反证和恢复条件齐全，不等于八个独立实测漏洞。
- `evidence.md`：56段独立固定time单定义体1–5行逐字引文，来自9个源码/类型文件。旧35、34证据与本批计数完全分开。

## P9当前实际覆盖与未作项

P9-1：六件不含完整40位Git SHA；实际提交短号以后从GitHub返回读取，在本人评论真实记录，MANIFEST不编造结果。

P9-2/4/13：56段直接引文从固定time完整所读定义体、宏定义/条件编译控制门选取，每段1–5行。已通过第二次connector读取9份源文件逐段**56/56字节一致**核对；不凭标题、include或声明证明行为。全函数否定断言以实际完整定义有界推论。

P9-3：新旧输入精确分栏，35强制R35-S01/S02与旧34-R34-S01第4/5条、历史规则保持适用，没有假称本次重新全文读过所有guidance。

P9-5：仅`evidence.md`有56个直接标签，`verified_claims=56`、`quotes_reconfirmed=56`、`downgraded_to_inferred=0`；任何引用被条件削弱写入正文/facts，不通过增添VERIFIED来偷换证明级别。

P9-6：没有私有外部research或URL检索句柄，不适用。

P9-7/8：options_flags.cmake与panic.c逐字金丝雀、与master不同，startup_selfcheck_quote采用conventions中的实际定句，工具`read_channel=connector`。

P9-9/10/12：没有新脚本、代码或内核修改、补丁、聊天降级交付，适用部分为NOT_APPLICABLE。

P9-11：从未运行仓库C/ASM、仓库脚本、预处理/编译、QEMU/硬件、真实目录路径；仅已读文本和有限字节/整数模型。完整本地P9闸门仍`NOT_RUN`，即使局部数据结构检查通过，也不宣称内核运行已验收。

## 写入与冻结

仅原PR17分支`claude/dazzling-cori-q0dnyt`的`user-fat-name-parse-36/`新增这六件及本人PR17正式交付评论。全部历史件、MyOS2主线PR16、master/time、内核/用户程序、PR17标题/body/Draft/他人评论保持未动。不得新建分支PR、强推、合并、删分支、编译运行、访问Owner本地/libc/镜像/凭据或进入阶段3。普通问题按18号延期，不向Owner请求本轮修复决策。

将使用expected-head`a7cb8bf8d2e6`及非强推快进。远端必须回读新固定提交与活动分支的六件正文和blob、检查PR17 head、compare只有一新提交/六件added/零旧文件修改删除/零写区外路径。实际完成与失败必须如实记录在本人评论或最后报告；正式交付后`STOP_WRITING`，交原C04独立验收，不能自行开始37。
