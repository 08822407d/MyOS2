---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LIBC-STRING-LENGTH-COPY-124
record_type: batch124_executor_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-19
conversation_display_name: "MYOS2-A-C08b 代码分析执行"
produced_by: "GPT-6（本会话可知；实际后台和effort UNATTESTED）"
model_per_owner: "GPT-6＋Extra（175号指定，界面实际effort不可独立认证）"
date: 2026-11-22
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_release_short12: 88968361c2fa
execution_parent_short12: 16398b8b96c6
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00_status: CORRECTION_SUBMITTED_PENDING_C08_REVIEW
D00_open: [R123-01]
C08_prior_closed: [R122-01, R120-02, R121-01, R121-02]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  time_options: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  time_panic: "\tthis_cpu = smp_processor_id();"
  master_options: "\t-m64 -mcmodel=large -fPIE \\"
  master_panic: "\tthis_cpu = raw_smp_processor_id();"
self_check: {verified_claims: 48, quotes_reconfirmed: 48, downgraded_to_inferred: 0}
per_file_literal_prefix: {MANIFEST.md: 0, libc-string-length-copy.md: 0, facts-and-boundaries.yaml: 0, libc-string-length-copy-paths.dot: 0, deferred-findings.yaml: 0, evidence.md: 48}
new_source_definition_windows: 48
full_local_P9: NOT_RUN
inputs_read:
  - "agent-workspace/lead/MYOS2-LEAD-002/175-user-libc-string-length-copy-restart-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/175-executor-continuation/00-C08a-to-C08b-handoff-package.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COMPARE-123-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COMPARE-123-D00-122-review.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/facts-and-boundaries.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare-paths.dot"
  - "agent-workspace/conventions.md"
  - "agent-workspace/tasks/00-gpt-task-protocol-v2.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md"
  - "myinitramfs/mylib/myglibc/string/strlen.c"
  - "myinitramfs/mylib/myglibc/string/strnlen.c"
  - "myinitramfs/mylib/myglibc/string/strncpy.c"
  - "myinitramfs/mylib/myglibc/string/strcmp.c"
  - "myinitramfs/mylib/myglibc/string/memcpy.c"
  - "myinitramfs/mylib/myglibc/string/memset.c"
  - "myinitramfs/myshell/myshell.c"
  - "myinitramfs/myshell/CMakeLists.txt"
  - "myinitramfs/mylib/myglibc/string/string.h"
  - "myinitramfs/mylib/myglibc/include/string.h"
  - "myinitramfs/mylib/myglibc/string/CMakeLists.txt"
  - "myinitramfs/mylib/myglibc/CMakeLists.txt"
  - "myinitramfs/CMakeLists.txt"
  - "mykernel/scripts/options_flags.cmake"
  - "mykernel/debug/panic.c"
preprocess_compile_link_Graphviz_QEMU_hardware_real_files: NOT_RUN
---
# 第124批六件清单 · CHAT-EXEC-19

## 接收与指导
- 任务源为175号固定合同、PR17换发回执#7073604860及175-executor-continuation/00-C08a-to-C08b-handoff-package.md，package_id=MYOS2-C08a-EXEC18-TO-C08b-EXEC19-BATCH124，mnemosyne_handoff_receive=RECEIVED_READY，执行者与旧CHAT-EXEC-18分离。
- mnemosyne_guidance_refresh: {project_guidance: required, mnemosyne_guidance: yes, task_local_only: true, current_task_preserved: true, mnemosyne_master_short12: e581eb9df6c4, Mnemosyne_write_authority: false, MyOS2_lead_write_authority: false}
- 本次实读MyOS2：00-work-order、agent-workspace/README、conventions、tasks/00-gpt-task-protocol-v2、18/24/28/63/79/110/124、173/174/175、175交接包、123两份C08裁定及检查点、123固定六件。Mnemosyne：README、human-approved-spec、receive/load指令、两份onboarding、guard-registry、显示名登记与本轮适用护栏。未将Mnemosyne当作MyOS2写入授权。
- 本次源码入口：`myinitramfs/mylib/myglibc/string/strlen.c`、`myinitramfs/mylib/myglibc/string/strnlen.c`、`myinitramfs/mylib/myglibc/string/strncpy.c`、`myinitramfs/mylib/myglibc/string/memcpy.c`、`myinitramfs/mylib/myglibc/string/memset.c`、`myinitramfs/mylib/myglibc/string/strcmp.c`、`myinitramfs/myshell/myshell.c`、`mykernel/scripts/options_flags.cmake`、`myinitramfs/CMakeLists.txt`、`myinitramfs/mylib/myglibc/CMakeLists.txt`、`myinitramfs/mylib/myglibc/string/CMakeLists.txt`；以及myinitramfs/myshell/CMakeLists.txt、string/string.h、include/string.h、mykernel/debug/panic.c。读取通道均为connector。

## fresh G00写前实测
- 完整四ref直接读取：lead=88968361c2fa，exec=16398b8b96c6，time=a039d9803ade，master=e8e60cd19540；本件只登记真实短12。直接master公约原句与time/master两套options_flags/panic金丝雀回源。
- 开放PR分页2/0：PR16 Open/Ready原head agent/MYOS2-LEAD-002→master；PR17 Open/Draft原head claude/dazzling-cori-q0dnyt→master；均未合并。
- 执行父123单父7d0e810ec4ff、compare前进1/落后0、六文件纯新增。逐件核固定/活动正文与Git Blob相符；全部旧文件冻结。
- 124写前目录与六件不存在。PR17顶层无since分页100/100/35/0共235条；123正式作者#7067185003、124旧发布#7073310725、175换发#7073604860各一；旧/新124作者正式交付标记0。
- 第123批RETURN；R123-01仍OPEN；历史R122-01已C08文档关闭。D00 F6/D2/L3共11例，A4/B6/C5共15个新例，合计26例；F六例38次strcmp按旧机器表复核。

## 六件与消费者
| 文件 | 内容 |
|---|---|
| MANIFEST.md | 接收/身份、G00、来源、P6、写入前边界 |
| libc-string-length-copy.md | D00小型替代说明及A/B/C三专题条件案例 |
| facts-and-boundaries.yaml | JSON兼容YAML，实际字节/预算/返回/可达及未调用字段 |
| libc-string-length-copy-paths.dot | 小型有类型图，真正的函数实例发出CALL_INSTANCE |
| deferred-findings.yaml | 保留独审待裁与源码/链接未知 |
| evidence.md | 48条定义体或活动CMake命令的连续1–5行新源码短窗 |

## 可行文本核验 / P9边界
- 六件每份40位十六进制串0；evidence是唯一包含正式短窗标签之文件，P6总48/48/0；行界逐窗对应已读固定文件、短窗不超过五个物理行。强制覆盖D00的实际调用与返回、A字节及整字扫描、B零预算/候选/夹取、C三个调用/辅助返回、time内核C配置。
- 图205唯一节点、262有类型边、62正向源case→实际调用点、5未调用真实端点不可达、5组禁止直接边及所有CALL所有者检查。仅文本模型，Graphviz NOT_RUN。
- P9第6–8项：外部标签无引用；直接master公约和双分支金丝雀、读通道均实读；P9第9/10/12无内核补丁与降级件，按不适用；第11及完整本地P9均NOT_RUN。
- write_retry_log: {tool_discovery: "one earlier composition call exceeded tool-call cap; no branch movement, remote HEAD reconfirmed unchanged", attempted_branch_writes_before_this_batch: 0, blob_writes: "being created with SHA verification before branch move", single_parent_commit: "pending", expected_head: 16398b8b96c6, force: false, formal_comment: "pending after remote readback", terminal_readback: "to be reported in formal PR17 comment"}
- 只允许向本新写区追加六件、原执行分支一次单父快进。旧123/lead/tasks/kernel/source及PR标题和Draft状态均不得更改。作者终态提交待C08 Pro独立验收，STOP_WRITING。
