---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_independent_mainline_review"
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
status: "RETURN"
author_comment_id: 7103660739
new_findings: ["R139-01","R139-02","R139-03","R139-04"]
historical_remaining: ["R138-03"]
combined_138: "RETURN"
combined_136_137: "PASS_PENDING_LOCAL"
reliability: "high_for_bounded_numeric_models_and_direct_artifact_differences"
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 第 139 批独立裁定 · RETURN

主线确认两套计数、五段字符串的长度和位置，以及六个空参数样本在指定边界的结果。提交仅新增合同约定六件。退回的直接原因包括时间顺序表的字节列全部缺值、A0 的终止原因与源码矛盾、C 图和机器件的地址与清理前提不清，以及有正式引文落在所标符号的定义体外。第 140 批接续四项新补正与一项历史余项。[191 号合同](https://github.com/08822407d/MyOS2/blob/61daf1431705/agent-workspace/lead/MYOS2-LEAD-002/191-user-exec-argument-vectors-contract.md) · [139 作者回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7103660739) · [138 补正裁定](CORE-USER-EXEC-ARGUMENT-VECTORS-139-D00-138-review.md) · [192 号合同](../192-user-exec-argv-entry-view-contract.md)

## 六件计量与证据范围

固定执行提交 0a3075f3f861，唯一父 38a5184142d7，比较 ahead=1、behind=0，六文件均 added、零删改。下表按同一时点的固定提交和活动执行分支两份全文独立统计 LF 与 UTF-8 字节，将 Git 的 blob 类型、字节长度、NUL 和内容一起重算 SHA-1，与目录原 SHA 和作者表相符。发布主线回执前，PR17 无 since 分页为 100/100/67/0，267 个不同 ID，作者完整标记唯一，按 ID 两次读回相同。四个直接分支引用及仅有的两个开放 PR 均符合 191。[#7103660739](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7103660739)

| 文件 | 行 | UTF-8 字节 | Git Blob 前十二位 |
|---|---:|---:|---|
| [MANIFEST.md](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/MANIFEST.md) | 206 | 12242 | `32f693f7a194` |
| [deferred-findings.yaml](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/deferred-findings.yaml) | 131 | 5700 | `9cff14f11c8f` |
| [evidence.md](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/evidence.md) | 1094 | 32103 | `1a4919913b33` |
| [exec-argument-vectors-paths.dot](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors-paths.dot) | 227 | 19570 | `8ae1b5533fca` |
| [exec-argument-vectors.md](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors.md) | 126 | 13390 | `9e75839fb257` |
| [facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/facts-and-boundaries.yaml) | 4313 | 131817 | `69c92ba66090` |
| 总计 | 6,097 | 214,822 | 固定／活动 6/6 |

两份 JSON 形式的 YAML 均可解析。120 个节点逐个比对 ID、kind、label，103 条唯一边逐个比对 from、to、kind、label，DOT 与 facts.graph 完全相同且没有悬空端点。报告第 124 行对边另列了 ID，实物中节点有 ID，边以四元组判重。新 92 个编号各自的路径、标题、角色、标识、起止、物理文本与索引对得上；共 91 种物理区间（E082/E090 共窗）。另核旧 47 个编号／46 种物理区间。P6 原始计数为 92/92/0。六件的全长 SHA、禁用确定性用语及内部检索句柄均零命中；两条金丝雀 time 正验与直接 master 反验、master 公约第 6 行启动句均吻合。P9-2 的定义体归属及 P9-13 的窄断言支撑存在下列缺口，不能由物理计数替代。本批为同模型族不同会话的独立审查。[原自检](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/MANIFEST.md#L115) · [边计量声明](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors.md#L124) · [正式证据](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/evidence.md#L19)

## A · 槽读取与字符串读取

| 独立输入 | 计数时实际访问的用户／内核槽 | 两入口返回 | 参数复制时再取槽 |
|---|---|---:|---|
| A0 整个向量为 NULL | 均无 | 0 | 无 |
| A1 有效数组首槽为 NULL | 均为 0 | 0 | 无 |
| A2 首槽指向有效单 NUL 字符串 | 均为 0、1 | 1 | 0 |
| A3 两串后接 NULL | 均为 0、1、2 | 2 | 1、0 |

用户版在外层判断后才进入循环并调用 get_user_arg_ptr；内核版在整个向量为空时直接返回 0。计数检查指针，读取字符串字节发生在复制阶段。四样本的八条计数路径和重读顺序均与事实表相符。图的 A0 两条终止边另见 R139-02。[用户外门](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L203) · [内核空指针返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L230) · [样本原表](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors.md#L59)

## B · 文件名、环境和参数的 30 个字节

| 复制次序 | 源 | 含 NUL 字节 | 新区间 |
|---:|---|---|---|
| 1 | bprm->filename | 2F 62 69 6E 2F 74 6F 6F 6C 00 | [T−10, T) |
| 2 | envp[1] | 46 3D 78 79 00 | [T−15, T−10) |
| 3 | envp[0] | 45 3D 31 00 | [T−19, T−15) |
| 4 | argv[1] | 2D 71 00 | [T−22, T−19) |
| 5 | argv[0] | 64 69 73 70 6C 61 79 00 | [T−30, T−22) |

逐个从输入串生成 ASCII 和 NUL，独立验证两种顺序、五个半开区间及合计 30 个字节。低地址起依次是 display、-q、E=1、F=xy、/bin/tool，各自 NUL 终止。_STK_LIM 为 8,388,608，三分之四为 6,291,456，大于 ARG_MAX=131,072。扣除四个八字节槽的预算后是 6,291,424；argmin=T−6,291,424，预算步骤的 p 仍为 T，文件名复制后 bprm.exec=T−10，最后 p=T−30。机器件 chrono、address_order 和 complete_relative_bytes 中的字节均通过；作者中文时序表第 78–82 行五处写成了字面 `undefined`。[预算](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L291) · [保存文件名地址](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L1339) · [时序表](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors.md#L76) · [完整字节](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/facts-and-boundaries.yaml#L2625)

## C · 六个独立样本

| 样本 | 第一次计数／参数来源 | 成立的边界 |
|---|---|---|
| U0、U1 | 0。先复制文件名和两个环境串，再由内核字面空串补一字节，argc 由 0 写为 1 | 警告调用点到达；p=T−20、exec=T−10；已到 bprm_execve 调用 |
| U2 | 1。普通用户 argv[0] 提供一个 NUL | 无合成写入；同一相对布局、同一调用边界 |
| K0、K1 | 0。retval 设为 −EINVAL 并跳 out_free，argc/envc 的显式赋值均未到达 | 清理前最后可读 p 为 T；通常清理正常返回时，函数返回 −EINVAL |
| K2 | 1。内核数组 argv[0] 的现成空串提供一个 NUL | 同一相对布局，已到 bprm_execve 调用 |

四个到达调用点的样本均为 argc=1、envc=2、预算 24、argmin=T−6,291,432。分别核了警告调用点、envp 计数、两种复制、合成、第二次 argc 赋值与相应的“跳过”。未实际装载或观察用户态。[用户合成](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L1358) · [内核拒绝零参数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L1399) · [正常清理后返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L1429) · [作者六例](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors.md#L105)

## 四项新退回

| 编号 | 可定位差异和影响 | 新件关闭条件 |
|---|---|---|
| R139-01 | B 时序表五段的“本次字节”皆为 `undefined`；报告 78、94 与 DOT 76 和机器节点写未定义端点 T0。 | 五段在两张表均能独立还原，数值偏移 0 以 T 表示；图、机器数据、正文全部核回 30 个字节。[原表](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors.md#L78) · [图节点](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors-paths.dot#L76) |
| R139-02 | A0 的两条边误用 NULL_SLOT_TERMINATES_NOT_COUNTED，实际未读任何槽。另有 D00 辅助边表用角色别名，无法与真实图四元组直接匹配；报告将边的计量多写一个 ID。 | 用户外门与内核早返各有真实关系，A1–A3 有真正读到的终止槽；附两条复制边及 K→getter 的真实选择器，或给明确的别名映射。计量以实际四元组为准。[A0 边](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors-paths.dot#L167) · [辅助边表](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/facts-and-boundaries.yaml#L327) · [原实边](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors-paths.dot#L143) |
| R139-03 | C 四个预算节点写成 argmin −6291432，省了基准 T。U2/K2 的 STRINGS 节点都写 before synthesis；U2 跳过合成，内核路径没有该分支。K0/K1 的表与机器返回字段省去正常清理前提；T 是释放 bprm 前最后游标。 | 分列地址表达式／相对偏移、各分支的实参来源、清理前可读字段、给定正常清理后的返回；六例图表统一。[预算节点](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors-paths.dot#L92) · [内核节点](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/exec-argument-vectors-paths.dot#L121) · [机器返回字段](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/facts-and-boundaries.yaml#L2920) · [bprm 对象释放](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L1094) |
| R139-04 | E057 将 ELF 内使用 AT_EXECFN 的语句标成 AT_EXECFN 定义体；此处属于 create_elf_tables。E043 只引 ret=0，真正 return 在 393；E067 只引 argc 赋值，正数前提须联读 E066。 | 分开使用点与宏定义、赋值与 return、守卫与受守卫赋值。各自的窄标题单独成立，组合推论列明所用编号。E023/E089 标签及原键抄录归历史 R138-03。[E057](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/evidence.md#L645) · [E043](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/evidence.md#L490) · [E066/E067](https://github.com/08822407d/MyOS2/blob/0a3075f3f861/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-vectors-139/evidence.md#L741) |

ELF 中此语句引用已保存的文件名地址，所在定义为 create_elf_tables。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_EXECFN, bprm->exec);
```

copy_strings 在公共出口返回 ret。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
	return ret;
```

内核入口遇零计数置错后进入清理。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = count_strings_kernel(argv);
	if (retval == 0)
		retval = -EINVAL;
	if (retval < 0)
		goto out_free;
```

固定源码配置中明确列出系统调用包装选项。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```c
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```

阶段 2。完整本地 P9、预处理、编译链接、仓库脚本或程序、Graphviz、QEMU、实机及真实进程、文件、PID、调度、时钟操作全部 NOT_RUN。
