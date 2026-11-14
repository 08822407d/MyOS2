---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-EARLY-ROOT-HANDOFF-84
record_type: six_file_batch_manifest
produced_by: "GPT-6（当前会话标识；后端模型标识与努力档不可独立认证）"
model_per_owner: "GPT-6＋Extra（Owner指定接收与执行档）"
date: 2026-11-13
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 18047d54b0a2
mainline_release_short12: 6b5aabc24e2a
phase: 2
status: final
execution_completion_target: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
open_questions: [ER84-U01, ER84-U02, ER84-U03, ER84-U04, ER84-U05, ER84-U06, ER84-U07, ER84-U08]
inputs_read:
  - "119-executor-continuation/00-C06a-to-C06b-handoff-package.md@6b5aabc24e2a"
  - "119-executor-continuation/01-MYOS2-A-C06b-receiver-startup-prompt.md@6b5aabc24e2a"
  - "120-user-early-root-handoff-contract.md@6b5aabc24e2a"
  - "110-owner-communication-and-delivery-continuity.md@6b5aabc24e2a"
  - "reviews/CORE-USER-EARLY-PGTABLE-STORAGE-81-review.md@6b5aabc24e2a"
  - "reviews/CORE-USER-DIRECT-MAP-RANGE-82-review.md@6b5aabc24e2a"
  - "reviews/CORE-USER-DIRECT-MAP-TABLE-INSTALL-83-review.md@6b5aabc24e2a"
  - "checkpoints/2026-11-13-table-install83-reviewed-early-root84-exec11-ready.md@6b5aabc24e2a"
  - "batch83 six original artifacts@18047d54b0a2; formal PR17 comment 6847954250"
  - "nine MyOS2 project rules independently read in full@6b5aabc24e2a (named individually in facts)"
  - "Mnemosyne receive procedure, human approved spec, core and conditional task guards independently read@8b58dd7b07c1"
  - "mykernel/arch/x86_64/kernel/head_64.S@a039d9803ade"
  - "mykernel/arch/x86_64/kernel/head64.c@a039d9803ade"
  - "mykernel/arch/x86_64/kernel.lds@a039d9803ade"
  - "mykernel/mm/misc/init-mm.c@a039d9803ade"
  - "mykernel/mm/fault/memfault_api.h@a039d9803ade"
  - "mykernel/arch/x86_64/kernel/setup.c@a039d9803ade"
  - "mykernel/arch/x86_64/mm/init.c@a039d9803ade"
  - "mykernel/init/main.c@a039d9803ade"
  - "mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h@a039d9803ade"
  - "mykernel/arch/x86_64/mm/page/page_const_arch.h@a039d9803ade"
  - "mykernel/arch/x86_64/include/asm/asm.h@a039d9803ade"
  - "mykernel/scripts/options_flags.cmake@time and master direct canaries"
  - "mykernel/debug/panic.c@time and master direct canaries"
  - "additional definition and type/include sources listed completely in facts-and-boundaries.yaml source_ledger"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  time_cflags: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  master_cflags: "\t-m64 -mcmodel=large -fPIE \\"
  time_panic: "\tthis_cpu = smp_processor_id();"
  master_panic: "\tthis_cpu = raw_smp_processor_id();"
formal_delivery_marker: "MYOS2-LEAD-002-CHAT-EXEC-11 / CORE-USER-EARLY-ROOT-HANDOFF-84 / FORMAL_DELIVERY"
self_check:
  verified_claims: 49
  quotes_reconfirmed: 49
  downgraded_to_inferred: 0
  finite_model_cases: 28
  technical_matrix_rows: 56
  actual_source_files: 23
  source_plus_g00_canaries: 25
  source_short_anchors: 49
  findings_candidates: 5
  uncertainty_gaps: 8
  forty_hex_occurrences_expected_in_six: 0
  original_prior_findings_recounted: 0
  P9_full_local: PENDING_LOCAL_NOT_RUN
  executable_repo_code_test: NOT_RUN
  other_model_or_paid_API: NOT_USED
---

# 第84批六件清单与写入边界

**唯一身份** MYOS2-LEAD-002-CHAT-EXEC-11；新对话C06b承接旧C06a/CHAT-EXEC-10在第83批交付后的STOP_WRITING，不继承旧方已读声明、不接管主线写权。固定119/120、110、83回执和六件、R83/R82/R81以及Mnemosyne记录均在本对话真实处理，原先G00前只读，G00通过后仅启动本84定界写权。

| 文件 | 完成内容 / 明确限制 |
|---|---|
| MANIFEST.md | 身份/源/门禁、公约与time/master逐字金丝雀、三整数自检、六件范围；正式远端回读只能在实际发布之后进行，不在此倒造已完成记录 |
| early-root-handoff.md | G01静态表存储/链接覆盖、G02参数修正及临时恒等、G03清零和单项根复制/CR3交接，九组Markdown矩阵、28例条件模型要点；没有内核修复或运行结论 |
| facts-and-boundaries.yaml | 23个固定time源码文件（另两个G00金丝雀来源）、九矩阵56行、28模型、原回执继承、真实Mnemosyne接收/规则加载/指导刷新日志及G00记录 |
| early-root-handoff-paths.dot | 汇编静态表、低地址身份页、早期CR3与C/后续CR3的源语句依赖图；虚线仅代表条件，不是实测；未执行Graphviz |
| deferred-findings.yaml | ER84-F01..F05五项条件性候选、ER84-U01..U08八项缺口、旧81/82/83 ID保留；不宣布已确认bug或自闭合历史缺口 |
| evidence.md | 49条实际固定time定义/数据标签块/宏内连续1–5行原文，逐条与所支持的声明同位；无跨函数拼贴、无硬件引文冒充 |

## 发布门禁与许可

G00真实核对：主线lead@6b5aabc24e2a，执行exec@18047d54b0a2，master直接头e8e60cd19540，time直接头a039d9803ade。PR16与PR17均Open/unmerged；PR17为Draft。G00时84目录GitHub明确404未存在，本人正式marker命中数0，旧正式83评论6847954250存在。无需旧10/09确认；D00 NOT_REQUIRED。

本次写入计划只许在原claude/dazzling-cori-q0dnyt、PR17新增84目录内上述六件；另发本人一次正式PR17顶层评论。不得动旧成果/评论、PR元数据、主线、master/time或内核；不创建分支/PR、不合并、不运行程序、不启用附加模型/API。使用单父提交、真实预期HEAD和force=false，不覆盖未知并行推进。

## 本次实证及尚未完成的检查

接收、九规则与110、Mnemosyne指导是本对话真实完成动作，并入facts；未把旧私有上下文当事实。六件构成和源码段摘录的静态计数已做，完整P9本地套件未执行，所以不得写作PASS。此MANIFEST在GitHub发布前生成：最终commit、PR评论ID和后置完整远端回读须在真正写入后才能从GitHub取得，并由唯一正式PR17评论记录，不能在此预造ID。

正式发布并回读后进入STOP_WRITING，仅交原MYOS2-A-C06主线Pro独立复核；mainline_acceptance=PENDING、接受上限PASS_PENDING_LOCAL。不得自签84、启动85，Owner无需自己审PR或编译。
