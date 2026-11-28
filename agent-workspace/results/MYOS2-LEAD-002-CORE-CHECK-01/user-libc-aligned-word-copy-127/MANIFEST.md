---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LIBC-ALIGNED-WORD-COPY-127
record_type: batch127_executor_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-19
conversation_display_name: "MYOS2-A-C08b 代码分析执行"
produced_by: "GPT-6（当前会话自知；实际后台具体型号和effort UNATTESTED）"
model_per_owner: "GPT-6＋Extra（178号固定合同与Owner指定，实际后台/界面推理档不可独立认证）"
date: 2026-11-22
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_release_short12: 8ad13b4fdfc7
execution_parent_short12: 345d0f32374c
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
write_directory: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/"
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00_status: CORRECTION_SUBMITTED_PENDING_C08_REVIEW
D00_open: [R126-01]
R125_01_and_R125_02: CLOSED_DOCUMENTATION_ONLY_BY_C08
prior_batch125_composite: PASS_PENDING_LOCAL
prior_batch126_review: RETURN
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  time_options: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  time_panic: "\tthis_cpu = smp_processor_id();"
  master_options: "\t-m64 -mcmodel=large -fPIE \\"
  master_panic: "\tthis_cpu = raw_smp_processor_id();"
self_check: {verified_claims: 44, quotes_reconfirmed: 44, downgraded_to_inferred: 0}
per_file_literal_prefix: {MANIFEST.md: 0, libc-aligned-word-copy.md: 0, facts-and-boundaries.yaml: 0, libc-aligned-word-copy-paths.dot: 0, deferred-findings.yaml: 0, evidence.md: 44}
new_source_definition_windows: 44
full_local_P9: NOT_RUN
preprocess_compile_link_Graphviz_QEMU_hardware_real_files: NOT_RUN
inputs_read:
  - "agent-workspace/lead/MYOS2-LEAD-002/178-user-libc-aligned-word-copy-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-MEMORY-COPY-FILL-126-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-MEMORY-COPY-FILL-126-D00-125-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-memory-copy-fill126-reviewed-aligned-word-copy127.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/177-user-libc-memory-copy-fill-contract.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/MANIFEST.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/facts-and-boundaries.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill-paths.dot"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/deferred-findings.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/evidence.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md"
  - "agent-workspace/tasks/00-gpt-task-protocol-v2.md"
  - "agent-workspace/conventions.md"
  - "myinitramfs/mylib/myglibc/string/wordcopy.c"
  - "myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h"
  - "myinitramfs/mylib/myglibc/string/memcpy.c"
  - "mykernel/scripts/options_flags.cmake"
  - "mykernel/debug/panic.c"
---

# 第127批 · 六件执行成果清单

## 接收、身份与适用指导

- 原C08b会话继续使用CHAT-EXEC-19；Owner发令来源178号固定合同与PR17发布回执#7077713021。原第126批交付#7075488931为已读输入而非新写权限；没有新执行分支、PR或额外模型/付费服务授权。
- 任务所写`GPT-6＋Extra`是Owner指定值；本会话可自述GPT-6但无法从连接器认证后台子型号和推理档。`produced_by`不冒充Pro或审核者。
- 复用**同一会话**已读的MyOS2基本约束；本批独立读取178、C08在同一提交的126独审/125勘误裁定/127检查点、177、126作者评论和原126六件，重读直接master公约、63号全文和110/124号。18/24/28/79仍在本会话已读范围，未声称这是独立新主线接收。
- Mnemosyne指导刷新：原同一会话已加载的本任务适用护栏维持；当前Mnemosyne master直接ref仍为`e581eb9df6c4`（短12，无变化）。仅任务局部核权限、单PR谱系、模型来源、证据和Owner下一步，不执行Mnemosyne写入或另立任务源。

## fresh G00（任何执行分支改动前的远端事实）

- 四直接ref核验：lead=`8ad13b4fdfc7`、exec=`345d0f32374c`、time=`a039d9803ade`、master=`e8e60cd19540`（均真实固定对象输出短12）。
- PR16 Open/Ready，head`agent/MYOS2-LEAD-002`至master；PR17原题Open/Draft，head`claude/dazzling-cori-q0dnyt`至master；两者未合并。开放PR页`2/0`。
- 执行父126提交`345d0f32374c`唯一父`bd1372961eaa`；compare领先1/落后0，仅六件纯新增3578行、164917 UTF-8字节，无旧文件改删。126六件固定提交、活动分支、Git Blob全文相同，37条历史新引文计数独立于本批。
- 本批127写前新目录不存在；PR17所有顶层评论按无since分页`100/100/41/0`读至空，共241条，126作者正式#7075488931与C08审查/127发布#7077713021均唯一，127任何执行身份正式作者标记为0。
- 直接master公约原句和两分支两处独立金丝雀由对应源码回读；time有`mcmodel=kernel -fno-pie -fno-pic`与`smp_processor_id`，master有`mcmodel=large -fPIE`与`raw_smp_processor_id`。当前全部应用为固定time静态源码，无实际sh链接证据。
- C08对125组合`PASS_PENDING_LOCAL`、R125-01/02文档关闭；对126裁定`RETURN`，新开放仅R126-01。作者本次只新增R126-01补正，不自签CLOSED。

## 六件唯一交付物与审查消费

| 文件 | 内容 |
|---|---|
| MANIFEST.md | 本身份、模型证据边界、完整G00/输入、源码P6自检、真实写入及恢复边界 |
| libc-aligned-word-copy.md | D00精确补正；A入口7例、B交替和末写3例、C外层memcpy三请求 |
| facts-and-boundaries.yaml | JSON兼容YAML；原126图件精确节点/边差异、修正阅读计数、13个127案例的word/byte访问、游标、返回和未知 |
| libc-aligned-word-copy-paths.dot | D00完整126修正阅读图和127 A/B/C独立模型图，宏/C调用/寄存器数据/goto分开 |
| deferred-findings.yaml | 历史R126-01开放待C08、有效预处理/链接/其它helper/对象有效性及运行证据的最早未知 |
| evidence.md | **44个**本批新定义/宏/活动内核C配置正式短窗；构建声明/默认宏别名另为普通位置 |

## 验证统计与边界

- D00历史126原图**184唯一节点、194声明、211边、40直接调用、12选定复制宏位置**。从固定旧DOT和C08退回逐操作重建的修正阅读图为**192唯一节点/声明、218边、40直接调用、13宏位置**；旧126文件保持冻结。原126有13个案例，旧文字和数值维持；本批不重新算成127新证据。
- 127新分析：A入口7例（0/1/2/7/8/9/16整字）、B循环3例（2/8/16）、C外层memcpy三请求（16对齐、16非对齐、129对齐）；**13个新case_id**。S和D分别384字节、8字节对齐，合法独立对象与非零目的哨兵，可复算每个read/write有效字节及范围。
- DOT追加的127图为174唯一节点、223有类型边；含3条外层MEMCPY拥有的直接helper调用和12条新宏展开。与D00图共计366唯一节点、441边、366次声明，无悬空端点；纯文本检查，不声称Graphviz实际渲染通过。
- 新P2正式源证据的字面前缀与结构一致：只在`evidence.md`有44次，其余五件均零；本批`self_check=44/44/0`，44处均从固定time源码定义体原文再核，1–5连续物理行、5份来源文件。原126的37条历史证据不混进127的P6。
- P9第1–5/13项在可行的连接器及文本范围核；P9第6–8项核公约、可定位输入、两支金丝雀、read_channel及无不明外部引文；本批没有补丁/脚本/降级件，第9/10/12项按交付形态不适用。**完整本地P9、预处理、C编译链接、Graphviz、QEMU、实机和真实文件操作均NOT_RUN**。
- 模型入口条件：通用`memcopy.h`、8字节op_t、OP_T_THRES=16、空PAGE宏与有效源目标输入均为给定，真实sh所用版本和动态编译配置`UNKNOWN`。helper参数按字数、函数返回void、memcpy外层返回原dstpp，不能混淆。

## 写入与安全停机日志（清单序列化前）

- `write_retry_log`：{tool_discovery: "原GitHub正规入口可用，没有权限发现阻断", staging_objects: "仅创建未挂分支Git Blob；包括两组对齐helper/外层条件机器表，D00精确补正和可重建图对象，新P2证据、整合DOT、facts、正文、deferred，部分编排修正只留下未引用旧Blob", read_only_or_local_corrections: "有限JavaScript纸面模型有一次变量名和一次输出日志引用错误，均未移动分支/发布评论，重新复算后恢复", git_branch_update_before_this_manifest: "NONE", target_parent_short12: "345d0f32374c", next_commit_requirements: "single parent, six new entries, expected-head, force=false", github_transient_write_errors: 0, genuine_write_retries: 0, formal_PR17_author_comment: "PENDING after fixed+active readback and duplicate check", final_readback: "to be completed and reported separately in formal comment"}。
- GitHub暂态错误如出现，必须遵守63号先检查HEAD/文件/评论远端效果，再按Retry-After或10/20/40秒安全重试；不盲目重复已完成写入、不强推。
- 旧126、lead、tasks、内核/用户库源码、PR标题及Draft状态均禁止修改；仅可向原执行分支增加本批六件，完成后由C08独立审核，作者`READY_FOR_MAINLINE_REVIEW`与`mainline_acceptance=PENDING`并存，不自签R126-01关闭。

**作者终态交付目标：在PR17作者评论唯一发布、按ID双读、PR17全页查重、四ref/两PR/六件再次回读之后 `STOP_WRITING`。**
