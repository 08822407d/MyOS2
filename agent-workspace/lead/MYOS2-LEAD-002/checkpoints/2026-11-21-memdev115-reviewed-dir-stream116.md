---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-DIR-STREAM-LS-116"
record_type: "mainline_batch_review_and_successor_checkpoint"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro（沿用 Owner 在本主线已明确选择）"
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
date: "2026-11-21"
date_timezone: "Asia/Shanghai"
phase: 2
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "a5b40470646a"
execution_pin_short12: "452444a1c4b4"
status: "READY_FOR_OWNER_FORWARDING_NOT_STARTED"
full_local_P9: "NOT_RUN"
build_and_runtime: "NOT_RUN"
open_questions: []
owner_decisions_requested_now: []
batch112_113: "PASS_PENDING_LOCAL"
batch114_composite: "PASS_PENDING_LOCAL"
R114_01: "CLOSED_DOCUMENTATION_ONLY"
R114_02: "CLOSED_DOCUMENTATION_ONLY"
batch115: "RETURN"
R115_01: "OPEN"
R115_02: "OPEN"
R115_03: "OPEN"
batch116: "DESIGNED_PENDING_OWNER_FORWARD"
executor: "MYOS2-LEAD-002-CHAT-EXEC-17"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/164-user-memdev-scalar-io-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/memdev-scalar-io.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/memdev-scalar-io-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FILE-SEEK-POSITION-114-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIRECTORY-ENUMERATION-59-review.md","myinitramfs/myshell/myshell.c","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c"]
---

# C08 主线检查点 · 第115批已验收，第116批待转发

| 批次 | 裁定与证据 |
|---|---|
| 112、113 | 既有组合 PASS_PENDING_LOCAL。 |
| 114 | R114-01、R114-02 均 CLOSED_DOCUMENTATION_ONLY，旧原件与追加差量合读为 PASS_PENDING_LOCAL。[独立裁定](../reviews/CORE-USER-MEMDEV-SCALAR-IO-115-D00-114-review.md) |
| 115 | 作者提交452444a1c4b4，正式评论 #7044709177，C07c 已停写。C08 复算七个页长例，保留零设备返回值及模式/位置模型。三项文档退回分别涉及报告缺值、证据和计数、图的调用主体与迭代返回条件。[独立验收](../reviews/CORE-USER-MEMDEV-SCALAR-IO-115-review.md) |
| 116 | 165号任务书把115三项补正同 shell 的目录流分析合为一包。使用原 C07c／CHAT-EXEC-17，GPT-6＋Extra。Owner 转发固定入口后启动。[任务书](../165-user-dir-stream-ls-contract.md) |

选题依据：第59批止于内核生成目录项，未给出 `ls` 实际如何消费。仓内 shell 明确调用 opendir/readdir/closedir，用户库有独立的32 KiB缓冲、逐条游标、非正返回处理及关闭函数。下一批以一次正常的三记录目录读取为共同模型，把源码与程序可见结果连接起来。[第59批验收](https://github.com/08822407d/MyOS2/blob/a5b40470646a/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIRECTORY-ENUMERATION-59-review.md) · [shell 调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L214-L242)

发布前 lead a5b40470646a，exec 452444a1c4b4，time a039d9803ade，direct master e8e60cd19540。开放PR仅16/17，均未合并。六件115固定/活动全文与blob一致，PR17作者正式评论按ID双读及全分页一致；主线发布前217条，分页100/100/17/0。已核本次四个lead目标与执行116目录缺席，两项主线发布标记均缺席。[115作者交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7044709177) · [115固定六件](https://github.com/08822407d/MyOS2/tree/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115) · [前次发布](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7043832344)

本轮在原lead新增本检查点、两份裁定和165任务书，PR17同一正式评论记录115验收与116发布，随后更新原两PR导航并回读。真实发布提交与评论ID见相应导航。完整本地P9、预处理、编译、Graphviz、QEMU及实机均 NOT_RUN。

唯一人工操作是把固定165及C08正式回执转给原 C07c 并选 GPT-6＋Extra。执行者依约交付、停写后，返回本主线用 Pro 同轮验收。
