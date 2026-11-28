---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_historical_correction_review"
followup_id: "CORE-USER-EXEC-ARGUMENT-VECTORS-139"
date: "2026-11-23"
date_timezone: "Asia/Shanghai"
phase: 2
conversation_display_name: "MYOS2-A-C09 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话标识）"
model_per_owner: "Pro（主线独立验收）"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "61daf1431705"
execution_pin_short12: "0a3075f3f861"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TASK-COMM-FORK-EXEC-138-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TASK-COMM-FORK-EXEC-138-D00-136-137-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-comm138-reviewed-exec-argv139.md","agent-workspace/lead/MYOS2-LEAD-002/191-user-exec-argument-vectors-contract.md","agent-workspace/conventions.md","agent-workspace/README.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/task-comm-fork-exec-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/task-comm-fork-exec.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ELF-STACK-TABLES-25-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ENTRY-STACK-BINDING-66-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-INIT-EXEC-HANDOFF-69-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-INIT-SHELL-LAUNCH-YIELD-131-review.md","mykernel/sched/forkexec/exec.c","mykernel/sched/forkexec/fork.c","mykernel/sched/syscall.c","mykernel/lib/string/string.c","mykernel/lib/string/string_helpers.c","mykernel/sched/scheduler/scheduler_core.c","mykernel/sched/scheduler/scheduler_const.h","mykernel/sched/runqueue/runqueue_types.h","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","mykernel/lib/string/strnlen_user.c","mykernel/include/uapi/linux/resource.h","mykernel/include/uapi/linux/limits.h","mykernel/include/uapi/linux/auxvec.h","mykernel/fs/vfs/binfmt_elf.c","mykernel/sched/task/task_const.h","mykernel/sched/task/task_types.h","mykernel/sched/task/task.h","mykernel/sched/scheduler/myos_rt.c","mykernel/sched/scheduler/scheduler_types.h","mykernel/lib/list/double_list.h","myinitramfs/CMakeLists.txt","myinitramfs/myinit/CMakeLists.txt","myinitramfs/myshell/CMakeLists.txt","myinitramfs/mylib/CMakeLists.txt","myinitramfs/mylib/csu/CMakeLists.txt","myinitramfs/myinit/initd.c","myinitramfs/myshell/myshell.c","myinitramfs/mylib/csu/crt1_c.c","mykernel/arch/x86_64/include/asm/syscall_wrapper.h","mykernel/include/linux/kernel/syscalls.h"]
status: "RETURN_PARTIAL_DOCUMENTATION_CLOSURE"
closed: ["R138-01","R138-02","R138-04"]
remaining: ["R138-03"]
combined_138: "RETURN"
combined_136_137: "PASS_PENDING_LOCAL"
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 138 四项历史补正的独立裁定

本次将冻结 138 作者件、原 C09 裁定、191 号逐项条件和 139 增量独立对读。作者身份均为 C09a／CHAT-EXEC-21。三项文档关闭，R138-03 保留一个范围明确的余项；第 138 批冻结原判及组合状态均为 RETURN。136、137 此前的组合状态均为 PASS_PENDING_LOCAL。[原独审](https://github.com/08822407d/MyOS2/blob/61daf1431705/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TASK-COMM-FORK-EXEC-138-review.md) · [191 号 D00](https://github.com/08822407d/MyOS2/blob/61daf1431705/agent-workspace/lead/MYOS2-LEAD-002/191-user-exec-argument-vectors-contract.md#L55) · [冻结作者回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7099213925) · [增量作者回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7103660739) · [139 独审](CORE-USER-EXEC-ARGUMENT-VECTORS-139-review.md)

| 项 | 本次裁定 | 经独立核收的内容 |
|---|---|---|
| R138-01 | CLOSED_DOCUMENTATION_ONLY | A1/A4 内层覆零和外层传递，A3 的真实外层补零，五例十个返回及 40 字节 |
| R138-02 | CLOSED_DOCUMENTATION_ONLY | 真实 DOT 和 facts.graph 的 P→K、P→J、K→getter；C3 早返后独立读；三个 getter 的 48 字节、锁和返回对象 |
| R138-03 | PARTIAL · OPEN | 两个真实索引、47/46 计量、E010、活动行／注释拆分及配置范围已核收。冻结旧键转录和两处 syscall 符号标签仍待补 |
| R138-04 | CLOSED_DOCUMENTATION_ONLY | 初始 F、左移后的 F<<1 与调用后确定的 0x04，保留三种时钟样本及原选择结果 |

## R138-01 · 两层拷贝的写入顺序

139 的 frozen_old_nodes、frozen_old_edges 所列 12 个旧节点和 9 条旧边与冻结 138 机器图逐字段相符。新图 A1 的 COPY 写 a，REWRITE 在同一位置 0 覆零，INNER_RET 才负返回，随后外层直接传播；A4 先写 abcd，再覆零位置 3，次序相同。A3 在内层返回 2 后外层补位置 3、4。五份结果依次为从零容量开始保留全 5A、首字节 00、61 62 00、61 62 00 00 00、61 62 63 00，各补足 8 字节的尾部仍是 5A；两层返回每组相同，依次 −E2BIG、−E2BIG、2、2、−E2BIG。符合原关闭条件。[旧图](https://github.com/08822407d/MyOS2/blob/38a5184142d7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/task-comm-fork-exec-paths.dot#L28) · [新节点和边](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors-paths.dot#L124) · [内层覆零](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/string/string.c#L225) · [外层传播或补零](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/string/string_helpers.c#L835)

## R138-02 · 实际对象、锁及返回

冻结三条 B 边和旧 C.shared_premises 与新件收录的原对象全等。在 139 实图，D139_P 分别指向 D139_K、D139_J，边类型分别为 INDEPENDENT_P_TO_K_STRUCT_COPY、INDEPENDENT_P_TO_J_STRUCT_COPY；GETTER_DATA_SOURCE_K_ONLY 从 D139_K 指向 D139_KGET，三个 label 都是空串。J 的名称写入独立。C1/C2 均有其 setter 的 K.alloc_lock 和后续 getter，C3 的错误指针早返，然后独立 getter 读取原 K。三个返回对象均是各自原 buf。[真实图边](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors-paths.dot#L143) · [三组读取](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors-paths.dot#L29) · [读取函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L820) · [错误返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L1297)

| 旧样本 | 读入 buf 的完整结果 | 内部返回 |
|---|---|---|
| C1 | 2F 6F 70 74 2F 64 65 00 | −E2BIG |
| C2 | ASCII /opt/demo/abcdefghijklmnopqrstu，后接 00，共 32 字节 | 31 |
| C3 | 69 6E 69 74 00 00 00 00 | 4 |

原 191 合同指定复核的实际 DOT、facts.graph、C_profile 与冻结样本均达到对象／字节条件，故关闭。139 额外生成的 independent_copy_edges 把角色名和边名缩写成另两组三元组；该字段作为机器导航仍有歧义。新开 R139-02 要求给出实际四元组或明示别名关系。[辅助表](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/facts-and-boundaries.yaml#L327) · [冻结读者](https://github.com/08822407d/MyOS2/blob/38a5184142d7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/task-comm-fork-exec.md#L73)

## R138-03 · 物理文本成立，元数据尚有余项

实际可解析的旧键为 D00["R137-02"].all_73_old_windows（73 项）和同级 new_47_window_index（47 项）。主线按原 47 条窗、冻结 138 索引和 139 的 old_47_crosswalk 逐个核对路径、符号字段、标题、首尾行、逐字内容；共 46 种物理区间，E138-037/045 共窗。139 另有 92 条正式记录，E139-082/090 共窗。E010 收窄为 max=count、res=0；E006/010/014/018/034 的独立注释已另列，E040 的原行内文字保留。E087 以固定配置原行限定所读快照，未运行预处理。[原 73 项](https://github.com/08822407d/MyOS2/blob/38a5184142d7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/facts-and-boundaries.yaml#L300) · [原 47 项](https://github.com/08822407d/MyOS2/blob/38a5184142d7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/facts-and-boundaries.yaml#L1266) · [真实导航](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/facts-and-boundaries.yaml#L448) · [拆分后的新锚](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/evidence.md#L975)

仍需在同一份增量证据中解决以下可复核的余项。

1. 冻结 138 报告 41 的实际文本为 `D00.R137-02.old_window_crosswalk`。139 facts 第 443 行把中间的连字符抄成下划线。138 evidence 第 17 行使用 `facts.D00.R137_02`，第 552 行单写 `old_window_crosswalk`。三个原字符串分别对应到真实机器键，不能合并成同一个旧拼写。[原报告](https://github.com/08822407d/MyOS2/blob/38a5184142d7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/task-comm-fork-exec.md#L41) · [旧证据导言](https://github.com/08822407d/MyOS2/blob/38a5184142d7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/evidence.md#L17) · [旧证据结尾](https://github.com/08822407d/MyOS2/blob/38a5184142d7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-task-comm-fork-exec-138/evidence.md#L552) · [139 误录](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/facts-and-boundaries.yaml#L443)
2. E139-023 的引文确实完整覆盖 exec.c:1460–1464 及活动 return，但正式 symbol 和机器索引仍以 `filename,` 截止。E139-089 虽保留 fork 中真正活动的三行，正式 symbol 缺右括号，固定源码给出完整 `MYOS_SYSCALL_DEFINE0(fork)`。在 140 新件中逐条登记完整可定位的 syscall 定义身份、对应原宏调用与活动语句，索引和证据使用同一身份。若使用拼接后的函数名，须以宏调用、包装声明、定义与配置的实际源窗解释。[E023](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/evidence.md#L260) · [E089](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/evidence.md#L995) · [execve 源定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L1460) · [fork 源定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/syscall.c#L51)

E139-057 的 ELF 使用点等新增窄断言问题归 R139-04。R138-03 的 73/47 两个旧表、原字节及已经核收的分拆结果无需重做。[E057](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/evidence.md#L645)

## R138-04 · 调用后的标志值

139 机器件给出初值 F 的数值未给、左移后 F<<1 数值未给、函数返回后调用者赋 RQCF_UPDATED（十六进制 04，十进制 4）。固定 source 的左移、调用和赋值三行及宏值均吻合。三项独立输入以 rq.clock=1000 起步，local_clock 999/1000/1001 时分别为不写／写零／写一，末值 1000/1000/1001。原 jiffies 106 与 last 100 仍相差 6，片 4 的 K 选 Q，片 9 的 J 继续 J。各样本和旧时钟数据相符。[标志与样本](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/facts-and-boundaries.yaml#L2068) · [调用者写点](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L1065) · [宏值](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_const.h#L48)

阶段 2。本裁定限文档范围，完整本地 P9 及全部运行操作均为 NOT_RUN。
