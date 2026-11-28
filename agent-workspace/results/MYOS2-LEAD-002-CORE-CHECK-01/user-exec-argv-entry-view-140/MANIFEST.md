---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-EXEC-ARGV-ENTRY-VIEW-140"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-21"
record_type: "phase2_executor_manifest"
date: "2026-11-23"
phase: 2
produced_by: "GPT-6.2（模型族标识，实际后端及effort未独立认证）"
model_per_owner: "原192号 GPT-6＋Extra；193号允许已启动继续；不声称已切换"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
self_check: {"verified_claims":98,"quotes_reconfirmed":98,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
status: "READY_FOR_MAINLINE_REVIEW"
mainline_acceptance: "PENDING"
write_retry_log: "not_needed"
owner_decisions_requested_now: []
open_questions: []
---

# 第140批 · 192号固定合同交付清单

当前执行席位 MYOS2-A-C09a／CHAT-EXEC-21。原合同技术范围不变，仅原执行分支指定目录 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argv-entry-view-140/` 新增六件。无旧件、内核、主线、PR标题/正文修改；独立验收PENDING。所有短SHA均从连接器输出复制，源码基线仍写分支名。

## G00 现场记录与本轮有权变更

```json
{
  "first_start": {
    "lead": "28f8646df32f",
    "exec": "0a3075f3f861",
    "time": "a039d9803ade",
    "direct_master": "e8e60cd19540",
    "open_PR_pages": [
      2,
      0
    ],
    "PR16": "Open/Ready -> master",
    "PR17": "Open/Draft -> master; original title",
    "comment_pages": [
      100,
      100,
      68,
      0
    ],
    "unique_count": 268,
    "previous_author_id": 7103660739,
    "previous_author_double_read_body_equal": true,
    "previous_release_id": 7102746870,
    "current_release_id": 7105872704,
    "review_and_release_markers_same_unique_comment": true,
    "new_marker_count": 0,
    "new_dir_404": true
  },
  "old139": {
    "commit": "0a3075f3f861",
    "parent": "38a5184142d7",
    "ahead": 1,
    "behind": 0,
    "only_six_adds": true,
    "lines": 6097,
    "utf8_bytes": 214822,
    "fulltext_fixed_active_equal": 6,
    "independent_blob_rehash": 6
  },
  "original_lead_publication": {
    "commit": "28f8646df32f",
    "parent": "61daf1431705",
    "only_four_adds": true
  },
  "authorized_lead_progress": {
    "head": "f6ee4306546d",
    "parent": "28f8646df32f",
    "ahead": 1,
    "behind": 0,
    "changes": [
      {
        "path": "agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md",
        "additions": 8,
        "deletions": 0
      },
      {
        "path": "agent-workspace/lead/MYOS2-LEAD-002/193-mainline-context-thresholds-and-model-availability.md",
        "status": "added",
        "additions": 68,
        "deletions": 0
      }
    ],
    "comment_id": 7119940390,
    "contract192_fixed_active_equal": true,
    "explicitly_authorized_in_comment": true,
    "other_authority_and_exec_unchanged": true,
    "comment_pages_at_recheck": [
      100,
      100,
      69,
      0
    ],
    "unique_count_at_recheck": 269
  },
  "canary_time_exact": [
    true,
    true
  ],
  "direct_master_negative": [
    true,
    true
  ],
  "startup_master_exact": true
}
```

初次启动符合192号固定发令，后续PR补核发现主线正常单父前进；直接比较、读新193及00工作令、读取C09 #7119940390，确认仅Owner提醒和模型版本续记，**明确允许本140继续**。192合同固定及活动全文与Blob一致。新预期lead=f6ee4306546d，不把已授权主线更新误判并行冲突；执行基点和范围不改。193提醒只适用主线设计数，执行席不计入；本轮已启动，不切换或重开模型。主线C09 Pro负责后续验收/设计及其门槛提醒。非Pro可用版本仅按Owner报告，实际后台未认证。

## 真实输入与分支范围

- `agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/191-user-exec-argument-vectors-contract.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/192-user-exec-argv-entry-view-contract.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-ARGUMENT-VECTORS-139-review.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-ARGUMENT-VECTORS-139-D00-138-review.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-argv139-reviewed-entry-view140.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ELF-STACK-TABLES-25-review.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ENTRY-STACK-BINDING-66-review.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-INIT-EXEC-HANDOFF-69-review.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-INIT-SHELL-LAUNCH-YIELD-131-review.md` — lead publication 28f8646df32f
- `agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md` — lead supplement f6ee4306546d
- `agent-workspace/lead/MYOS2-LEAD-002/193-mainline-context-thresholds-and-model-availability.md` — lead supplement f6ee4306546d
- `agent-workspace/conventions.md` — master（分支名）
- `agent-workspace/README.md` — master（分支名）
- `agent-workspace/tasks/00-gpt-task-protocol-v2.md` — master（分支名）
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/MANIFEST.md` — fixed 0a3075f3f861 and active execution
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors.md` — fixed 0a3075f3f861 and active execution
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/facts-and-boundaries.yaml` — fixed 0a3075f3f861 and active execution
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors-paths.dot` — fixed 0a3075f3f861 and active execution
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/deferred-findings.yaml` — fixed 0a3075f3f861 and active execution
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/evidence.md` — fixed 0a3075f3f861 and active execution
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/facts-and-boundaries.yaml` — fixed 38a5184142d7
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/task-comm-fork-exec.md` — fixed 38a5184142d7
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/evidence.md` — fixed 38a5184142d7
- `mykernel/fs/vfs/binfmt_elf.c` — time（分支名）; a039d9803ade
- `mykernel/sched/forkexec/exec.c` — time（分支名）; a039d9803ade
- `mykernel/sched/syscall.c` — time（分支名）; a039d9803ade
- `mykernel/lib/string/strnlen_user.c` — time（分支名）; a039d9803ade
- `mykernel/include/uapi/linux/auxvec.h` — time（分支名）; a039d9803ade
- `mykernel/scripts/options_flags.cmake` — time（分支名）; a039d9803ade
- `mykernel/debug/panic.c` — time（分支名）; a039d9803ade
- `mykernel/include/linux/kernel/syscalls.h` — time（分支名）; a039d9803ade
- `mykernel/arch/x86_64/include/asm/syscall_wrapper.h` — time（分支名）; a039d9803ade
- `myinitramfs/mylib/csu/crt1_c.c` — time（分支名）; a039d9803ade
- `myinitramfs/myinit/initd.c` — time（分支名）; a039d9803ade
- `myinitramfs/myshell/myshell.c` — time（分支名）; a039d9803ade
- `myinitramfs/CMakeLists.txt` — time（分支名）; a039d9803ade
- `myinitramfs/myinit/CMakeLists.txt` — time（分支名）; a039d9803ade
- `myinitramfs/myshell/CMakeLists.txt` — time（分支名）; a039d9803ade
- `myinitramfs/mylib/CMakeLists.txt` — time（分支名）; a039d9803ade
- `myinitramfs/mylib/csu/CMakeLists.txt` — time（分支名）; a039d9803ade

直接master公约启动原句逐字命中，两个金丝雀在固定time命中且master反验不命中。旧137-139历史源窗物理核收结论引用C09裁定；本140新源窗14个实际源文件重新读取核对98/98。正式标签及机器source_index逐字段相符，98处不同物理区间；旧139的92/91及旧138的47/46另作冻结索引与历史裁定，未混计。接收/指导范围仅适用于当前MyOS2阶段2，不导入其它项目自身任务与写权。

## 六件清单

| 文件 | LF行 | UTF-8字节 |
|---|---:|---:|
| MANIFEST.md | 发评时实际统计 | 发评时实际统计 |
| exec-argv-entry-view.md | 208 | 20297 |
| facts-and-boundaries.yaml | 7028 | 206300 |
| exec-argv-entry-view-paths.dot | 362 | 32520 |
| deferred-findings.yaml | 127 | 5448 |
| evidence.md | 1079 | 29954 |

图与机器同源：193个唯一节点ID、165条唯一from/to/kind/label四元组，端点完整。三例入口表共7+6+9=22槽，全部与合同检算值匹配；139旧30字节和本批93字节模型及各段NUL均复算。

## 五项补正和三个新专题

R138-03历史余项：四处冻结原文/错误转录分别登记，两个实际旧表73、47项；完整 syscall 源宏调用定义身份与活动return同标签索引。R139-01：五次复制实际hex、T末端、时序/地址表互核，完整30字节与旧机器值相符。R139-02：A0外门与无取槽早返、A1–A3终止槽、复制再读槽，原三条真实边四元组及别名解析。R139-03：四例argmin=T-6291432；用户合成／现成、内核现成空串，K0/K1释放前游标T与正常清理后的条件返回分开。R139-04：92个冻结ID索引，AT_EXECFN使用与定义、ret赋值与实际return、argc写入与前置门各自窄窗。五项均 CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW，不重开已判闭项。

A1槽2,U-30,U-22,0,U-19,U-15,0；A2槽1,U-20,0,U-19,U-15,0。S独立表底，U=T-Δ。B1/B2/B3 envs分别S+32/S+24/V+16，候选_start三标准流给定F0,NULL,F2，main实参为给定argc/argv、exit(7)；无实际CRT绑定结论。C：init新prog_name,args,envs；源filename与args[0]同指，分别写出U-9与U-93，完整93字节，九槽3,U-93,U-84,U-69,0,U-54,U-39,U-24,0。shell入口与每行解析向量区别、CMake实际声明与选中CRT区别均登记。保留25/66/69/131前置及未证边界。

## P9 连接器可达核查

1. 六件扫描40位hex零命中（交付前最后总核）。
2. 98条新正式引文1–5连续物理行，重新从固定time取源逐窗比较98/98，符号或完整宏调用/构建指令定位同索引；手工区分活动、声明和配置；完整本地定义范围机械闸门未运行。
3. 上列实际输入及源路径均连接器读取；无旧件修改或补丁。
4. 附带物理行范围按实际文件长度和逐行读取核实。
5. 98/98/0；旧冻结窗口不计本批正式标记。
6. 无外部事实依赖、无内部检索句柄作证据。
7. 两条time正命中/direct master反验无命中。
8. startup原句及connector读通道明示。
9. 不适用：无内核实现代码件。
10. 不适用：无代码改动块或补丁。
11. 禁止的确定性编译运行措辞全件扫描零命中（最后总核）。
12. 不适用：不是聊天降级交付。
13. D00与A/B/C强制锚点已定位且分窗；实际后台及模型强度不猜测。

完整本地P9、预处理、编译链接、仓库程序/脚本、Graphviz、QEMU、实机及真实文件、进程、PID、调度、时钟操作全部NOT_RUN。未运行解析器、未打开真实设备、未观察真实入口寄存器或最终用户态。提交与正式作者回执后的G04须独立实际远端核验，本清单不预填尚未取得的提交或评论ID。发评后只读，完成即STOP_WRITING，交原C09用Pro独审并同轮发布后继。
