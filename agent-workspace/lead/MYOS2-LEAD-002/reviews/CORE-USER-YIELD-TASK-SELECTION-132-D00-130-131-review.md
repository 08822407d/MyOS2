---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_independent_historical_correction_review"
followup_id: "CORE-USER-YIELD-TASK-SELECTION-132"
date: "2026-11-23"
date_timezone: "Asia/Shanghai"
phase: 2
conversation_display_name: "MYOS2-A-C09 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前会话标识）"
model_per_owner: "Pro（沿用 Owner 对主线独审的选择）"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
read_channel: "connector"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "f07e60a5e8c0"
execution_pin_short12: "1021b4b7740c"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/184-user-yield-task-selection-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-INIT-SHELL-LAUNCH-YIELD-131-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-INIT-SHELL-LAUNCH-YIELD-131-D00-127-130-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/evidence.md","myinitramfs/myinit/initd.c","myinitramfs/mylib/myglibc/stdio/printf.c","myinitramfs/mylib/myglibc/stdio/vsnprintf.c","mykernel/sched/scheduler/myos_rt.c","mykernel/sched/syscall.c","mykernel/sched/scheduler/scheduler_core.c","myinitramfs/myshell/myshell.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/conventions.md","agent-workspace/README.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md"]
status: "CLOSED_DOCUMENTATION_ONLY"
R130_02: "CLOSED_DOCUMENTATION_ONLY"
R131_01: "CLOSED_DOCUMENTATION_ONLY"
R131_02: "CLOSED_DOCUMENTATION_ONLY"
combined_130: "PASS_PENDING_LOCAL"
combined_131: "PASS_PENDING_LOCAL"
self_check: {"verified_claims":3,"quotes_reconfirmed":3,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 第 132 批历史补正复审 · R130-02、R131-01、R131-02

**三项均裁定 CLOSED_DOCUMENTATION_ONLY。** 130 与已核差量合读、131 与已核差量合读，均为 PASS_PENDING_LOCAL。130/131 的原始裁定保留可追溯。132 新模型的独立缺口另以 R132 编号跟进。 [131 历史裁定](CORE-USER-INIT-SHELL-LAUNCH-YIELD-131-D00-127-130-review.md) · [131 主审](CORE-USER-INIT-SHELL-LAUNCH-YIELD-131-review.md) · [132 主审](CORE-USER-YIELD-TASK-SELECTION-132-review.md) · [184 合同](../184-user-yield-task-selection-contract.md)

## R130-02 · 从错误返回边追到真实的取参者

以 130 固定 DOT 为起点，先套 131 关于两次返回与弃用的有效差量，再套 132 五簇的旧端点、旧类型、替换和新增边。同时按旧、新 facts 独立核对类型。130 最初 10 条错误选择器均各唯一命中；132 指定的 15 条旧边在 131 facts 和 DOT 中分别各命中一次。五组旧关系各 16 条，共 80 条；65 原样保留，15 条各做一换一，每例另添 7 条。五组终态共 115 条类型边，原有 70 个节点加 15 个新节点，共 85 个。每簇终态 17/23。旧、新两种载体均能得到同一结果。[130 冻结图](https://github.com/08822407d/MyOS2/blob/7e52edee26b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics-paths.dot#L192) · [131 诊断簇](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield-paths.dot#L186) · [132 五簇](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot#L36)

printf 在自己的函数体中建立 ap 并直接调用 vsnprintf 和 write。vsnprintf 的 %s 转换两次消费 ap；次序来自格式串。vsnprintf 的 rv 同时决定请求字节数及 printf 的返回。write 的结果被丢弃，命令自己的返回来自常量或底层调用的 err；run_cmd 最终丢弃命令结果。真实 strerror 链接、locale 和实际设备输出仍 UNKNOWN。[printf 的两个直接调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/printf.c#L17) · [格式取参](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L783) · [命令所有者](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L188)

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	va_start(ap, fmt);
	rv = vsnprintf(buf, SZ_4K, fmt, ap);
	va_end(ap);
```

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				case 's':		/* String */
					sarg = va_arg(ap, const char *);
```

| 失败样本 | 给定文本拼回，末尾另加 LF | 请求 / printf 返回 | 命令返回来源 |
|---|---|---:|---|
| cd /lost | cd: No such file or directory: /lost | 37 / 37 | 常量 1 |
| ls 默认目录 | ls: Permission denied: . | 25 / 25 | 常量 −1 |
| cat note | ls: No such file or directory: /note | 37 / 37 | 常量 −1，固定格式使用 ls 前缀 |
| rmdir /tmp/d | rmdir: Directory not empty: /tmp/d | 35 / 35 | rmdir 保存到 err 的 −1 |
| rm /log | rm: Permission denied: /log | 28 / 28 | unlink 保存到 err 的 −1 |

ls 给定 write 只返回 2 时，printf 仍返回 25，命令返回 −1。rmdir 的 argc=1、rm 的 argc=3 均在删除或打印之前返回 −1。此前需要补的调用拥有者和两级弃用已全部呈现。132 独立的统计字段需按五簇/七簇分开，见 R132-03。

## R131-01 · 正式窗、配置和旧批输入归属

| 冻结选择器 | 独核依据与裁定 |
|---|---|
| E131-044 | E132-001 仅含 myos_rt.c:66 的 yield_task 表项；65 行定义头仅作 E132-003 定位。表项归属成立。 |
| E131-053 | E132-002 独引 options_flags.cmake:62–65 活动 CMAKE_C_FLAGS 内容。第 41 行编译金丝雀另记 E132-004。相应否定性判断限所读配置与函数。 |
| 虚拟 130 inputs_read | 从原 44 个正式窗提取 11 个路径。旧虚拟清单的 8 项中 7 项相合，余下的 initd.c 属 131 的新专题。旧证据另含 errlist.c、memcpy.c、memcopy.h、wordcopy.c 四条确切路径，132 均已恢复到证据归属表。 |
| P6 计数 | 130 原六件裸 45、正式 44；只改标题为 44/44；整合五个一对一替换再追加 return 为 45/45。131 原件裸/正式 48/48；E044 等量替代后仍 48。132 自有 58/58/0，七处定位另外计。 |

原 130 未留下依据的内部阅读保持 UNKNOWN。当前读到的固定源、旧批冻结文件与新增勘误分开归属。[132 引文](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/evidence.md#L19) · [机器归属与四种口径](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml#L379) · [130 原证据](https://github.com/08822407d/MyOS2/blob/7e52edee26b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/evidence.md)

## R131-02 · 三实参和两个调用拥有者

| 图的改变 | 两载体及源码的复核 |
|---|---|
| B_EXECVE 新簇 | 131 对该簇节点与边均为 0，132 新增 26 节点/26 边。execve 调用分别给出 filename、args、envs；两表各四槽，末槽空指针两处，六个不同字符串各有一个终止 NUL。filename 与 args[0] 共同指向 prog_name。 |
| A_INIT | 旧的 main→复合 fork 节点直接边唯一命中。新图 8 节点/7 边，明确 main 调 normal_boot、normal_boot 调 fork。 |
| C_YIELD | 三条旧选择器在 131 两种载体各唯一命中。新图 20 节点/24 边。系统调用体发起间接 yield_task，rq 是实参；在给定类下表项提供目标，yield_task_fair_myos 直接调用 resched_curr。 |
| C 的先后、分支和结果 | 调用体分别调用两处抢占操作和 schedule，有独立顺序边与正常返回 0；标志已设提前返回、未设同 CPU 调两个 setter、异 CPU 发送代码为注释。解锁调用也仅作注释定位。 |

由固定 initd.c 的原样字符串重算。/boot/sh 为 8 字节，五条不同测试字符串各 14 字节，去重内容为 78，含六个终止字节为 84；再计 filename 对同一字符串的第二次引用，逐引用口径为 86/93。因此地址未知也足以复核字符串字节总数。实际映像替换和运行不在本裁定范围。[参数与调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myinit/initd.c#L24) · [B 图](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot#L284)

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
		execve(prog_name, args, envs);
```

A 与 C 合计旧 21 节点和 19 条边，其中 15 边原样保留，4 边被替换。两簇终态共 28 节点/31 边；两个 ref 对应的旧标签无意外丢失。这里闭合的 yield_task 图有明确调用者；132 新专题中的 pick_next_task 发生在另一个现场，按 R132-02 单独修正。[A/C 图](https://github.com/08822407d/MyOS2/blob/1021b4b7740c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot#L219) · [系统调用体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/syscall.c#L29) · [类表项](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L66)

完整本地 P9、预处理、编译链接、仓库程序与脚本、Graphviz、QEMU、实机与真实进程、调度、文件操作均为 NOT_RUN。
