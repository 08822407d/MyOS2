---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_independent_mainline_review"
followup_id: "CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136"
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
lead_before_publication_short12: "9d333c5959a8"
execution_pin_short12: "95b6af84d8f6"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md","agent-workspace/lead/MYOS2-LEAD-002/188-user-new-task-runqueue-entry-contract.md","agent-workspace/lead/MYOS2-LEAD-002/188-executor-continuation/00-C08c-to-C09a-handoff-package.md","agent-workspace/lead/MYOS2-LEAD-002/188-executor-continuation/01-MYOS2-A-C09a-receiver-startup-prompt.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FORK-RETURN-IDENTITIES-135-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FORK-RETURN-IDENTITIES-135-D00-133-134-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-fork135-reviewed-new-task136-C09a.md","agent-workspace/conventions.md","agent-workspace/README.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/new-task-runqueue-entry-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/new-task-runqueue-entry.md","mykernel/sched/task/pid.c","mykernel/sched/forkexec/fork.c","mykernel/lock_IPC/signal/signal.h","mykernel/arch/x86_64/sched/context/process_arch.c","mykernel/sched/scheduler/scheduler_core.c","mykernel/lib/list/double_list.h","mykernel/sched/task/task_const.h","mykernel/sched/scheduler/myos_rt.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","mykernel/sched/task/pid.h","mykernel/sched/syscall.c","mykernel/lib/list/double_list_types.h","mykernel/sched/runqueue/runqueue_types.h","mykernel/lib/list/double_list.c","mykernel/include/linux/kernel/asm-generic/bug.h"]
status: "RETURN"
author_commit_short12: "95b6af84d8f6"
author_receipt_id: 7096187292
review_findings: ["R136-01","R136-02","R136-03","R136-04"]
self_check: {"verified_claims":3,"quotes_reconfirmed":3,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 第 136 批独立裁定 · RETURN

C09 按 188 号合同独立读回六件及固定源码。两次调度选择、两种插头顺序和三项历史补正的技术结果核收。136 原件有四项定点文档缺陷，列为 RETURN。当前 C09a 在 137 新目录追加修订，136 六件冻结。135 原件合并本轮已核补正为 PASS_PENDING_LOCAL。[188 号合同](https://github.com/08822407d/MyOS2/blob/9d333c5959a8/agent-workspace/lead/MYOS2-LEAD-002/188-user-new-task-runqueue-entry-contract.md) · [唯一作者回执 #7096187292](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7096187292) · [135 三项逐项裁定](CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136-D00-135-review.md)

## 实物与接收

执行为 C09a／MYOS2-LEAD-002-CHAT-EXEC-21，沿用 claude/dazzling-cori-q0dnyt。固定提交 95b6af84d8f6 的唯一父为 d01efe19ccec，compare ahead 1、behind 0。仅六新增，无旧件修改、删除。下表以两次按 ref 读取的全文逐件比较，并由主线以 UTF-8 原始字节及 Git blob 头独立复算。[固定提交](https://github.com/08822407d/MyOS2/commit/95b6af84d8f6)

| 文件 | 行数 | UTF-8 字节 | Git Blob 前十二位 |
|---|---:|---:|---|
| [MANIFEST.md](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/MANIFEST.md) | 240 | 10382 | 11c8d72208bd |
| [deferred-findings.yaml](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/deferred-findings.yaml) | 103 | 3552 | bf9556cd0650 |
| [evidence.md](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/evidence.md) | 413 | 10204 | 5a97277bddd5 |
| [facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml) | 2455 | 69962 | 03edfdb91d2d |
| [new-task-runqueue-entry-paths.dot](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/new-task-runqueue-entry-paths.dot) | 210 | 15155 | af3b27437b88 |
| [new-task-runqueue-entry.md](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/new-task-runqueue-entry.md) | 85 | 7221 | 786abe3e28ac |
| 合计 | 3,506 | 116,476 | 6/6 |

MANIFEST 分别载有交接接收与指导刷新，有包号、C09a 身份、指导版本、11 份项目输入路径及 G00 基线。Owner 从新对话转交的首轮回复也记录了发评后的只读终检。作者评论本身记的是发评时的后续计划。主线独立观察到发布前 PR17 无 since 页长 100/100/61/0，261 个不同 ID，完整作者标记仅 #7096187292，按该 ID 两读正文一致。四条直接 refs 及 PR 分别核对，记录见检查点。[接收](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/MANIFEST.md#L24) · [指导刷新](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/MANIFEST.md#L62) · [G00](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/MANIFEST.md#L107)

## 按固定源码复演

A 的活动写点为子任务 se.vruntime=0，rt.run_list.next 和 prev 分别指向自己的节点，__state=TASK_NEW（0x800），on_cpu=0。time_slice、on_rq 的相应行是注释。成功 copy_process 返回后，kernel_clone 先取临时 PID 引用与编号，调用 wake_up_new_task，再归还引用、返回编号。wake 函数在 CPU 0 的 rq 上写 TASK_RUNNING（0），传入 running_lhdr 头对象与子任务自身的链节点。它不写 rq->curr、on_cpu 或当前任务的重排标志。[调度初始化](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L574) · [状态与入队](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L688) · [编号与唤醒顺序](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/fork.c#L1497) · [链节点](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L149)

| B 的独立调用 | 四次写入，依源序 | 正向／反向（省去两端 H） | count |
|---|---|---|---:|
| K 后 J，第一步 K | Q.prev=K，K.next=Q，K.prev=H，H.next=K | K Q R／R Q K | 2→3 |
| K 后 J，第二步 J | K.prev=J，J.next=K，J.prev=H，H.next=J | J K Q R／R Q K J | 3→4 |
| J 后 K，第一步 J | Q.prev=J，J.next=Q，J.prev=H，H.next=J | J Q R／R Q J | 2→3 |
| J 后 K，第二步 K | J.prev=K，K.next=J，K.prev=H，H.next=K | K J Q R／R Q J K | 3→4 |

四次 tail 均为 R。源语句在四写之后执行 count++。有效输入使六项异常谓词皆假，校验函数退出空循环并返回真，外层拒绝分支跳过。此项是主线的补充复演。[插入有效性](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L162) · [写指针](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L189) · [增计数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L646)

| C 独立初态均为 K Q R、count=3、rq->curr=P | 103 | 110 |
|---|---|---|
| used_jiffies、外门 | 3。need_resched、idle、3≥10、状态不为 RUNNING 依次为假，count 跳过 | 10。前两项假，10≥10 为真，状态项跳过，count>0 为真 |
| 选首／回插 | 跳过，P 继续 | 取 K，count=2，K 节点自连。P 旧键 3>Q 的 20 为假，anchor 比较跳过。将 P 插在 Q 前，count=3 |
| 计费、时钟、末队列 | P 键 3，last=100，K Q R | 入链后 P 键 3+10=13，last=110，P Q R |
| 返回、rq->curr、K.__state | P、P、TASK_RUNNING | K、K、TASK_RUNNING |

C1/C2 都执行 picker 末尾的 rq->curr 赋值。C2 给定 P 为 RUNNING，BUG_ON 的左项为假，右项跳过。真实切换、执行首条指令、回到用户态及父子时序为 UNKNOWN。[picker 两层门](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L14) · [回插与计费](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L37) · [末尾写回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L56)

## 四项勘误与验收标准

| 编号 | 冻结原件的问题 | 137 新件的闭合条件 |
|---|---|---|
| R136-01 · 分支、启动与接收记录 | MANIFEST 129 行只记两版金丝雀不同，全件缺 branch_canary_quotes、startup_selfcheck_quote。P9-7/8 对作者件不通过。188/00 要求的 task_local_only 在刷新记录缺显式值。旧问题列了批次但未明记旧作者 EXEC-20。 | 逐字列两条 time 金丝雀和 direct master 公约 §1 第 2 条；分别复核 time 正命中和 master 反验。保留 read_channel、P6 三整数。点名 C08c/EXEC-20 原作者和 C09a/EXEC-21 补正者，区分 136 旧记录、137 新读与本地任务适用范围。无须重新开对话。[原清单](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/MANIFEST.md#L129) · [接收约定](https://github.com/08822407d/MyOS2/blob/9d333c5959a8/agent-workspace/lead/MYOS2-LEAD-002/188-executor-continuation/00-C08c-to-C09a-handoff-package.md#L79) |
| R136-02 · 正式依据与报告索引 | 38/38 窗的物理坐标和文本均吻合，E022、023 全是注释，E014 将活动与注释混用。evidence 没有独立的活动配置依据。报告 D00-03、A/B/C 的末尾范围遗漏相关 E011、018/019、027、036–038。 | 逐个映射旧 38 个编号，区分保留、拆窗与纯定位。补固定配置及列表定义条件，标清对 BUG_ON 和注释判断的适用范围，以活动语句支持窄断言；重连报告、机器件与证据。新件按实际标签重算 P6，历史 38 个物理命中原数保留。[混合窗](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/evidence.md#L155) · [注释窗](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/evidence.md#L231) · [原报告](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/new-task-runqueue-entry.md#L44) |
| R136-03 · B 的输入归属和校验 | B.base 及四个后态共五处写 need_resched=false。188 只给 C 两例这个初值。B 的四次指针写及计数正确，却省去了给定合法输入如何通过校验的证明。 | B 按“初值未给、该函数未写、后值沿用”记账。C 保留给定 false。对四次 (new,prev,next) 列六个无效谓词的求值及真值，while、return、外 if、四写和 count++；保留所有正反链、尾及计数。[五处字段所在段](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml#L1032) · [校验与四写](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L162) |
| R136-04 · C 的谓词标签与后态 | C136_103 两载体把比较写作 used3<time_slice10 OR false，缺少源码所测 3>=10 为假的含义。C1.after 未列 K.__state。C2 记 3>20 假却没有单列右侧 anchor 比较未求值。 | 以源码 >= 标明操作数、是否求值、结果和外 OR。C1、C2 补齐三时点；保留两个原返回和末队列，标明 C1 K 仍为 TASK_RUNNING；C2 的 while 右项 NOT_EVALUATED。同步带 ID、label、kind 的图与机器件。[图中 C136_103](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/new-task-runqueue-entry-paths.dot#L108) · [机器标签](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml#L1832) · [C2 比较](https://github.com/08822407d/MyOS2/blob/95b6af84d8f6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml#L1282) |

[VERIFIED mykernel/lib/list/double_list.h::__list_add_valid]
```c
			while ((prev == NULL) || (next == NULL) ||
					(next->prev != prev) || (prev->next != next) ||
					(new == prev || new == next));
```

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	if ((need_resched() ||
		curr_task == rq->idle ||
		used_jiffies >= rt->time_slice ||
		curr_task->__state != TASK_RUNNING) &&
		myos_rq->running_lhdr.count > 0)
```

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```

## 核验边界

111 个节点的 ID/type/label 和 93 条唯一边的起点、终点、kind、label 在两个载体逐项相同，无重复 ID、无悬空端点。同一错误标签在两件一致出现。两份 JSON 形式的 YAML 均可读。六件的 40 位十六进制串、外部检索句柄和 P9-11 禁用措辞均零命中，38/38/0 满足原件字面计数。分支金丝雀由 C09 在固定 time 和直接 master 重新交叉核实，不能追记为作者已填写。完整本地 P9、预处理、编译链接、仓库脚本／程序、Graphviz、QEMU、实机及真实进程、PID、调度、时钟和文件操作均为 NOT_RUN。
