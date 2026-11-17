---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-BOOTINFO-TAG-BUILD-97
record_type: batch97_manifest_and_R96_01_02_documentary_append
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
executor_display_name: MYOS2-A-C06e 代码分析执行
produced_by: GPT-6（可见模型家族；后台UNATTESTED）
model_per_owner: GPT-6＋Extra（Owner明确指定次一档；后台强度不可独立认证）
date: 2026-11-17
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
master_direct_short12: e8e60cd19540
lead_publication_short12: b878cc3784da
execution_parent_short12: 6a1fa78d0356
existing_write_branch: claude/dazzling-cori-q0dnyt
existing_pr: 17
lead_pr: 16
phase: 2
status: AUTHOR_STATIC_COMPLETE_PENDING_C06_PRO
mainline_acceptance: PENDING
old95_composite: PASS_PENDING_LOCAL
old95_R95_01_02: CLOSED_BY_MAINLINE
old96_review: RETURN
old96_R96_01_02: CORRECTION_SUBMITTED_PENDING_C06_REVIEW
old96_R96_S01_S02_S03: RETAINED
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
open_questions:
  - 实际EFI服务状态、尺寸/对齐ABI、共享32页真实容量与每个指针的有效extent未认证
  - MMAP DescriptorSize、输入顺序、尾槽清零、e820最多128条的动态前提未认证
  - ACPI与MP实际返回和ExitBootServices后的服务阶段合约未认证
  - 96两项文档纠正与97结果由C06 Pro正式验收，执行者不签
inputs_read:
  - agent-workspace/lead/MYOS2-LEAD-002/138-executor-continuation/00-C06d-to-C06e-handoff-package.md
  - agent-workspace/lead/MYOS2-LEAD-002/138-executor-continuation/01-MYOS2-A-C06e-receiver-startup-prompt.md
  - agent-workspace/lead/MYOS2-LEAD-002/138-executor-continuation/02-batch97-executor-authority-and-checkpoint.md
  - agent-workspace/lead/MYOS2-LEAD-002/139-user-bootinfo-tag-build-contract.md
  - agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FRAMEBUFFER-MAPPING-96-review.md
  - agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-CONSOLE-REGISTRATION-OUTPUT-95-DOC-CLOSURE-review.md
  - agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-17-framebuffer96-reviewed-bootinfo97-new-executor.md
  - agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-17-bootinfo97-restored-scope-and-executor14-ready.md
  - agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-BOOT-MEMORY-MAP-80-review.md
  - agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md
  - agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md
  - agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md
  - agent-workspace/README.md
  - agent-workspace/conventions.md
  - agent-workspace/tasks/00-gpt-task-protocol-v2.md
  - agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md
  - agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md
  - agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md
  - agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md
  - agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-framebuffer-mapping-96/MANIFEST.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-framebuffer-mapping-96/framebuffer-mapping.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-framebuffer-mapping-96/facts-and-boundaries.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-framebuffer-mapping-96/framebuffer-mapping-paths.dot
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-framebuffer-mapping-96/deferred-findings.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-framebuffer-mapping-96/evidence.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-memory-map-80/MANIFEST.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-memory-map-80/deferred-findings.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-memory-map-80/facts-and-boundaries.yaml
  - MyOS2 PR17 mainline comment 6943545977; batch96 comment 6941872312; batch96 release 6938385614
  - Mnemosyne receive command, README, human-approved-spec, independent relevant guard-registry and applicable guards
startup_selfcheck_quote: |-
  2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。
branch_canary_quotes:
  time_options_flags: |-
    -m64 -mcmodel=kernel -fno-pie -fno-pic \
  time_panic: |-
    this_cpu = smp_processor_id();
  master_options_flags: |-
    -m64 -mcmodel=large -fPIE \
  master_panic: |-
    this_cpu = raw_smp_processor_id();
self_check:
  verified_claims: 85
  quotes_reconfirmed: 85
  downgraded_to_inferred: 0
  counting_scope: only_new97_six_file_raw_VERIFIED_BT97_headings
  evidence_definition_literal_fixed_time_match: 85_of_85
  source_quote_index_entries: 85
  source_files_with_new_quotes: 9
  extra_direct_helpers_read: 3
  raw_prefix_counts:
    MANIFEST.md: 0
    bootinfo-tag-build.md: 0
    facts-and-boundaries.yaml: 0
    bootinfo-tag-build-paths.dot: 0
    deferred-findings.yaml: 0
    evidence.md: 85
  original96_selfcheck_literal_count_immutable: 110
  original96_semantic_all_single_definition_claim: WITHDRAWN_FOR_FM96_E036
  original95_claims_immutable: true
  matrix_groups: 8
  matrix_rows: 51
  matrix_rows_by_section: {D00: 5, A: 12, B: 17, C: 13, ABC: 4}
  finite_models: 20
  finite_model_inputs_not_executed: true
  new_conditional_candidates: 8
  new_evidence_gaps: 9
  graph_nodes: 55
  graph_edges: 87
  graph_dangling_nodes: 0
  full_commit_sha_40_lower_hex_matches_six_file_text: 0
  complete_local_P9: NOT_RUN
  compile_preprocess_link: NOT_RUN
  repository_programs_scripts: NOT_RUN
  Graphviz_QEMU_hardware: NOT_RUN
  external_EFI_ABI_validation: NOT_RUN
  six_files_remote_blob_and_branch_readback: PENDING_POST_PUBLICATION
  single_formal_PR17_comment: PENDING_POST_PUBLICATION
---

# 第97批 MANIFEST｜原接收、D00纠正与四层证据门

## 1. 正式身份、接收程序和G00

新对话 **MYOS2-A-C06e 代码分析执行** 按Owner明确指令使用身份 `MYOS2-LEAD-002-CHAT-EXEC-14`，模型级别按Owner声明 GPT-6＋Extra，后台身份/努力等级 **UNATTESTED**。旧C06d/13等旧执行者正式停用；旧状态与已读声明没有继承，不能代替本轮自行回读，也没有向旧对话请求确认。

**真实接收报告**：在读取九项项目约束之前，于本会话局部临时报告 `/mnt/data/myos2_exec14_scratch/receive.yaml` 留存 `MYOS2-C06d-EXEC13-TO-C06e-EXEC14-BATCH97-HANDOFF-20261117` 规范化接收状态，接收阶段将九规则、110/124及独立Mnemosyne刷新明确列为pending；报告后来追加真实顺序的已读和G00信息，原pending不倒填。按新138接收程序独立全文加载九份MyOS2约束（协议v2 P1–P13完整）、110及124。之后才单独加载Mnemosyne的核心规范与适用的条件guard，局部真实刷新记录在 `/mnt/data/myos2_exec14_scratch/guidance-refresh.yaml`；未导入Mnemosyne跨项目维护活态或其它项目任务，未使用子代理或额外模型API。

**G00只读通过**：发布入口lead `b878cc3784da`、执行起点 `6a1fa78d0356`、直接master `e8e60cd19540`、源码time `a039d9803ade` 四refs全与Owner给定一致；PR16 Open、PR17 Open/Draft且均未merge。旧 `user-boot-info-transfer-97` 与新 `user-bootinfo-tag-build-97` 结果目录均不存在，旧/新执行者14正式marker均无以该marker开头的实际提交评论；本批起点与活动HEAD compare identical，未见未知推进。time与master原文canary两组各自匹配且互相区分。**所有G00前操作严格只读**。G00之后连续D00、A、B、C，不等待第二道Owner发令。

## 2. 固定输入范围和旧审核继承

- 唯一源 `time@a039d9803ade`；访问固定源9件形成本批新短锚、另3份直接头文件助读。除时间/master canary双对照，不读活动master为本批内核事实，更不分析已弃用137的寄存器搬运/BSS专题。
- 第95批R95-01/02已被主线正式 `CLOSED_DOCUMENTATION_ONLY`，组合 `PASS_PENDING_LOCAL`；不重开。第96批审查是 `RETURN`，仅R96-01/02文档补正，R96-S01–03主体保留。本D00只追加纠正，不改旧96六件、110/110/0历史数、不改历史评论，不宣布96闭合。
- 本次按139号分别覆盖A共享MBI缓冲与游标/END/总长，B EFI MMAP转换、归类、顺序合并、排序、容量和终止约定，C ACPI、CPU拓扑tag、EFI服务阶段以及kernel本处直接消费。旧BM80-F01…F04与其U项（源extent/哨兵/128容量等）只作有条件先门，不重做第80批。

## 3. 严格六件和独立计数

| 新路径相对本目录 | 源码声明标签次数 | 内容边界 |
|---|---:|---|
| `MANIFEST.md` | 0 | 身份、九规则、接收与刷新、G00、真实自检和发布界限 |
| `bootinfo-tag-build.md` | 0 | D00与A/B/C静态结论、算术输入阶段与实际消费者 |
| `facts-and-boundaries.yaml` | 0 | 85项来源索引、8组51行矩阵、20隔离有限模型、旧新消费者映射 |
| `bootinfo-tag-build-paths.dot` | 0 | 55节点/87语义边/0悬挂，Graphviz未运行 |
| `deferred-findings.yaml` | 0 | 8候选/9缺口，按真实源关系去重与历史qualified_id交接 |
| `evidence.md` | 85 | 固定time逐定义1–5行的新证据锚 |
| **合计** | **85** | 与旧第96批110及旧第80批92 **完全分账** |

**引文类别**：单定义内结构体、独立typedef、宏、loader函数、kernel直接消费者分开核验。D00旧E036实有跨独立typedef的定义边界失败：本批E001/E002分别补强；FM96-F05三个旧元数据错误是引用关系失败：只用旧FM96-E035/E041/E044和本批E003/E004/E005支持，将CN95-U05放prior关联，撤回旧“全部证据引用存在”的无条件总结。报告的`source_quote_index`是85条新源定位映射，不把外部EFI、实际地址或0..7bpp发生性变为已证。

**真实G04前文本自检**：在旧执行分支更新之前，从GitHub固定time九份源码再次按给定行段回读，生成85条连续短锚；再从生成的Git blob中独立解析85条并逐段与源行对照，结果字面 **85/85** 且每段1–5行。facts的51行矩阵、20有限模型与deferred的8/9记录引用过的新BT97证据ID做了存在性核验，**零悬挂**；图构造时校验55节点和87条端点引用均存在，**Graphviz未运行**。按本六件原始标签最终应为85/85/0而非先前110/110/0；全套本地P9自动扫描未执行，不能伪写P9 PASS。

## 4. 模型输入、实际执行状态和首要停门

所有有限模型明确隔离 C/EFI/指针/内存合法性前提：32页=131072只是请求预算；`size+7` u32 回绕可变零步长；排序后仍未二次merge；额外MMAP尾哨兵的零值依赖前序清零；数组最多128（来自旧BM80）；CPU表大小含外部返回nCores；`ExitBootServices`源码位于ACPI/CPU调用之前但状态未认证。**没有执行真实程序、使用其他模型、发出PR以外写入或调用付费外部API。** 20个隔离样本与51行事实不是20或51次测试。

首个不能证实的EFI status、ABI `sizeof`、UINTN/u32转换、DescriptorSize、服务时点、MBI数据对象、ram_map哨兵、32页空间、E820目标容量，都必须停止把静态输入推成成功路径。先行旧95/96/80的未决条件保留。源文件软限：本批9项短锚源+3份声明助读=12，未超过24；未全树搜索。

## 5. 发布前与发布后门

只许既有PR17执行分支本次六个新路径、同一单父提交、完整expected-head且force=false；不新建branch/PR，不修改旧96原件或评论、主线、PR16、master、time、内核、loader或Owner脚本，不改变PR元数据、不得合并。发布前重新检查新旧97目录、两套marker、四refs及PR状态，不自动变基或覆盖未知HEAD。GitHub暂态异常遵照63号先核实际远端效果再安全间隔重试；成功的写动作只回读不二次创建。

**本MANIFEST在Git对象准备阶段写成，不能把未来的Git ref更新和PR评论回读当成已完成。** 更新后必须按G04回读提交树中精确六条新path及固定文件与活动blob字节同一性、PR16/17状态、四条direct refs和唯一正式评论全文/唯一性。真实Git写操作数、状态和最终commit只在唯一正式PR17顶层交付评论中报告。G04全核实后STOP_WRITING并交回原“MYOS2-A-C06 内核分析主线续建”；Owner发送正式独立验收消息前选择 **Pro**。由原C06同轮审核R96-01/02的补正、第97批成果和后继安排；执行者不自签96/97通过，不启动第98批。

唯一正式marker：`MYOS2-LEAD-002-CHAT-EXEC-14 / CORE-USER-BOOTINFO-TAG-BUILD-97 / FORMAL_DELIVERY`。
