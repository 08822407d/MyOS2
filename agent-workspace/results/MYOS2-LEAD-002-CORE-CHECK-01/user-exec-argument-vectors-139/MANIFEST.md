---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-EXEC-ARGUMENT-VECTORS-139
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-21
record_type: phase2_executor_manifest
date: 2026-11-23
date_timezone: Asia/Shanghai
phase: 2
produced_by: "GPT-6（文本模型族；后台未认证）"
model_per_owner: "GPT-6＋Extra（Owner指定）"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
read_channel: connector
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
self_check: {"verified_claims":92,"quotes_reconfirmed":92,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
write_retry_log: "not_needed"
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
owner_decisions_requested_now: []
open_questions: []
---

# 第139批正式交付清单 · 191号合同

**状态 READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING。** 当前同一作者席位C09a／MYOS2-LEAD-002-CHAT-EXEC-21；按Owner要求次一档GPT-6＋Extra执行，无法独立认证实际后台模型与effort。本批只在 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/` 新增指定六件；旧138、旧C09主线及内核源码完全冻结，不改旧评论、不改PR标题/正文、不创建新PR。阶段2、所有运行禁令继续有效。

## G00独立远端预检（真实接收本轮190→191）

```json
{
  "fresh": true,
  "read_channel": "connector",
  "direct_refs_before": {
    "lead": "61daf1431705",
    "execution": "38a5184142d7",
    "source_time": "a039d9803ade",
    "direct_master": "e8e60cd19540"
  },
  "lead_publication": {
    "one_parent": "4777e07269dd",
    "compare_ahead": 1,
    "compare_behind": 0,
    "exactly_four_added": true
  },
  "open_pr_pages": [
    2,
    0
  ],
  "PR16": "Open/Ready; agent/MYOS2-LEAD-002 -> master, unmerged",
  "PR17": "Open/Draft; claude/dazzling-cori-q0dnyt -> master, unmerged, original long title",
  "PR17_top_comment_pages": [
    100,
    100,
    66,
    0
  ],
  "PR17_top_comment_unique_count": 266,
  "previous_author_receipt_id": 7099213925,
  "previous_author_receipt_read_twice": true,
  "previous_author_full_marker_unique": true,
  "C09_review_and_publication_comment_id": 7102746870,
  "C09_review_and_publication_markers_both_unique_same_comment": true,
  "this_author_marker_before": 0,
  "new_directory_before": "404 NOT_FOUND",
  "previous_batch138": {
    "head": "38a5184142d7",
    "one_parent": "4ae5e3ea1af0",
    "compare": "ahead1 behind0 exactly six added zero deletions/modifications",
    "lines": 4257,
    "utf8_bytes": 133838,
    "fixed_active_fulltext": 6,
    "fixed_active_blob_match": 6,
    "independent_Git_blob_SHA1_rehash": 6
  },
  "canary": {
    "time_positive": [
      true,
      true
    ],
    "direct_master_negative": [
      true,
      true
    ],
    "direct_master_rule_exact": true
  },
  "full_local_P9": "NOT_RUN"
}
```

同一C09a已经有前轮接收/指导刷新记录。本批仅重读191合同、C09裁定和项目约束，保留`task_local_only=true`；该约束仅允许通用行为指导作用于MyOS2本任务，不导入Mnemosyne的建制、维护与写权限。138作者/本139作者同一身份，139四项补正不是作者自行签主线PASS。136/137历史组合PASS_PENDING_LOCAL、138冻结RETURN，各自裁定独立保持。

## 来自直接master的启动句及两条time金丝雀

direct master `agent-workspace/conventions.md` §1第2条逐字原文：

> 2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。

- time `mykernel/scripts/options_flags.cmake` 原行：`	-m64 -mcmodel=kernel -fno-pie -fno-pic \`；time正命中，direct master反验不命中。
- time `mykernel/debug/panic.c` 原行：`	this_cpu = smp_processor_id();`；time正命中，direct master反验不命中。

## 严格六件

| 文件 | 行数 | UTF-8字节 | 作用 |
|---|---:|---:|---|
| MANIFEST.md | 正式作者回执附准确数字 | 正式作者回执附准确数字 | 当前G00、启动及P9/自检 |
| exec-argument-vectors.md | 126 | 13390 | D00及A/B/C中文报告，逐字节/地址表 |
| facts-and-boundaries.yaml | 4313 | 131817 | 机器事实，4个D00、4个A、5段B、6个C及120/103图 |
| exec-argument-vectors-paths.dot | 227 | 19570 | 同源有向带类型图 |
| deferred-findings.yaml | 131 | 5700 | 四项待主线判闭、七项未证边界 |
| evidence.md | 1094 | 32103 | 92条新正式窗口与47条旧窗对照 |

机器事实/DOT同源 **120节点、103条各自唯一的有向类型边**，端点存在、同ID/kind/label/from/to。证据 **92个正式窗口/91个不同物理区间**，本批固定time源码按实际标签和符号再读 **92/92**，降级0；上一批冻结的 **47个窗口/46个物理区间** 已单独再读 **47/47**，不混入本批正式计数。正文和facts中的所有D00索引均使用真实字面键，包括 `D00["R137-02"].all_73_old_windows` 与 `D00["R137-02"].new_47_window_index` 的历史路径。

## 本批D00与技术任务

- **R138-01**：按源码把A1/A4的末字节覆零移到strscpy内部、位于负返回之前；strscpy_pad的负数分支只返回不补零；A3在正返回后才由包装层memset补零。完整保留五例10个标量返回、40输出字节。
- **R138-02**：P分别独立赋值复制到K、J；仅K供getter取数；C1/C2按K的setter、K.alloc_lock及随后getter建图，C3独立早返后读取原K，不借C1/C2的setter条件。
- **R138-03**：旧47窗与真实两个旧表键完整可定位，区分旧47与新92、旧E138窗口中独立注释/宏定义、活动源、原行内文字及配置结论适用范围。
- **R138-04**：F、F<<1、调用返回后RQCF_UPDATED=0x04三时点；原时钟采样999/1000/1001与K→Q/J→J结果保留，已关闭的R137-01不重开。
- **A**：四种argv的用户`get_user`与内核数组直接读槽，分别0/1/2/3槽、返回0/0/1/2；只有复制阶段才读字符串字节。
- **B**：argc=2/envc=2，指针预算32，字符串限额6,291,424，argmin=T−6,291,424；五段顺序写入，bprm.exec=T−10、最终p=T−30，及完整30字节地址顺序。
- **C**：用户零计数U0/U1补一字节空串，U2的非空指针直接复制已有NUL；内核K0/K1提前-EINVAL、K2正常复制单NUL。用户三例和K2末p=T−20、argc=1/envc=2；内核前二场景env/budget/exec都SKIPPED。

四项作者状态统一为 `CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW`。本批不声明任何后续elf_loader已成功、真实物理写入、实际转入用户模式或日志已投递。

## P9项目级约束（仅连接器可达部分）

```json
{
  "P9-1": "six new files full forty-hex strings check: 0; fixed commit only shortened in artifacts",
  "P9-2": "92 formal windows, 92 exact physical fixed-time line matches, 91 distinct physical windows; actual backend binary/preprocess NOT_RUN",
  "P9-3": "all listed sources and frozen inputs fetched through GitHub connector",
  "P9-4": "source windows line range each 1–5 and within true definition, old 47/47 independently re-read",
  "P9-5": "verified_claims=92, quotes_reconfirmed=92, downgraded_to_inferred=0",
  "P9-6": "not treating external URLs, comment numbers, internal handles or unsupported runtime as evidence",
  "P9-7": "two branch canaries each time positive and direct master negative",
  "P9-8": "startup_selfcheck_quote exact from direct master, read_channel connector",
  "P9-9": "N/A no kernel code implementation, symbol implementation or import introduced",
  "P9-10": "N/A no patch or implementation diff",
  "P9-11": "forbidden compile/run certainty phrases absent",
  "P9-12": "N/A no model downgrade to deliver artifact",
  "P9-13": "D00 source order, A pointer count, B layout and limits, C warning/synthesis/kernel rejection all independently anchored",
  "full_local_P9": "NOT_RUN",
  "build_execution": "NOT_RUN",
  "Graphviz": "NOT_RUN",
  "QEMU_real_machine": "NOT_RUN"
}
```

完整本地P9、预处理、编译、链接、仓库程序/脚本、Graphviz、QEMU、实机与真实文件、进程、PID、调度、时钟操作一律 `NOT_RUN`。链接器和内核是否实际采用相关宏配置仍不认证；数据表和DOT不等于实际CPU执行。

## 实际输入路径与既有裁定

- `agent-workspace/conventions.md`
- `agent-workspace/README.md`
- `agent-workspace/tasks/00-gpt-task-protocol-v2.md`
- `agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md`
- `agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md`
- `agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md`
- `agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md`
- `agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md`
- `agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md`
- `agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md`
- `agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md`
- `agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md`
- `agent-workspace/lead/MYOS2-LEAD-002/190-user-task-comm-fork-exec-contract.md`
- `agent-workspace/lead/MYOS2-LEAD-002/191-user-exec-argument-vectors-contract.md`
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TASK-COMM-FORK-EXEC-138-review.md`
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TASK-COMM-FORK-EXEC-138-D00-136-137-review.md`
- `agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-comm138-reviewed-exec-argv139.md`
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ARG-PAGES-24-review.md`
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ELF-STACK-TABLES-25-review.md`
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-STACK-23-review.md`
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-INIT-SHELL-LAUNCH-YIELD-131-review.md`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/MANIFEST.md`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/task-comm-fork-exec.md`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/facts-and-boundaries.yaml`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/task-comm-fork-exec-paths.dot`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/deferred-findings.yaml`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/evidence.md`
- `mykernel/sched/forkexec/exec.c`
- `mykernel/lib/string/strnlen_user.c`
- `mykernel/lib/string/string.c`
- `mykernel/lib/string/string_helpers.c`
- `mykernel/sched/syscall.c`
- `mykernel/sched/forkexec/fork.c`
- `mykernel/sched/scheduler/scheduler_core.c`
- `mykernel/sched/scheduler/scheduler_const.h`
- `mykernel/scripts/options_flags.cmake`
- `mykernel/include/uapi/linux/resource.h`
- `mykernel/include/uapi/linux/limits.h`
- `mykernel/include/uapi/linux/auxvec.h`
- `mykernel/fs/vfs/binfmt_elf.c`
- `mykernel/sched/task/task.h`
- `mykernel/sched/task/task_const.h`
- `mykernel/sched/task/task_types.h`
- `mykernel/sched/scheduler/myos_rt.c`
- `mykernel/sched/scheduler/scheduler_types.h`
- `mykernel/sched/runqueue/runqueue_types.h`
- `mykernel/lib/list/double_list.h`
- `mykernel/debug/panic.c`

固定Git提交、唯一PR17正式作者评论、评论后的G04终检属于上述六件发布**之后**的远端操作，本清单不预填未取得的提交SHA或评论ID、不提前声称G04已通过。快进前新读exec expected_sha，使用force=false单父新增；快进后6/6全文与SHA1对照、作者回执按ID双读、PR17顶层评论无since翻至空页，再重核四refs和两张原PR。完成后STOP_WRITING，交原C09用Pro独立验收并同轮下达后继。
