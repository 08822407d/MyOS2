---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_independent_mainline_review"
followup_id: "CORE-USER-INIT-SHELL-LAUNCH-YIELD-131"
date: "2026-11-23"
date_timezone: "Asia/Shanghai"
phase: 2
conversation_display_name: "MYOS2-A-C09 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前会话标识）"
model_per_owner: "Pro（沿用 Owner 对 C09 正式验收的选择）"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "5a22d13876bc"
execution_pin_short12: "2089bfff6f4a"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/183-user-init-shell-launch-yield-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/evidence.md","myinitramfs/myinit/initd.c","myinitramfs/CMakeLists.txt","myinitramfs/myinit/CMakeLists.txt","myinitramfs/myshell/CMakeLists.txt","myinitramfs/myshell/myshell.c","mykernel/arch/x86_64/include/asm/unistd_64.h","mykernel/arch/x86_64/include/asm/syscalls_64.h","mykernel/arch/x86_64/entry/syscall_64.c","mykernel/sched/syscall.c","mykernel/sched/scheduler/scheduler_types.h","mykernel/sched/scheduler/myos_rt.c","mykernel/sched/scheduler/scheduler_core.c","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/errno.h","myinitramfs/mylib/myglibc/stdio/errlist_internal.c","myinitramfs/mylib/myglibc/stdio/errlist.c","myinitramfs/mylib/myglibc/include/stdio.h","myinitramfs/mylib/myglibc/string/string.h","myinitramfs/mylib/myglibc/stdio/printf.c","myinitramfs/mylib/myglibc/stdio/vsnprintf.c","myinitramfs/mylib/myglibc/string/wordcopy.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/conventions.md","agent-workspace/README.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md"]
status: "RETURN"
open_returns: ["R131-01","R131-02"]
inherited_open: ["R130-02"]
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 第 131 批独立验收 · init 启动 shell 与一次 yield

**C09 裁定 RETURN。** A 的分支、B 的参数及字节、C 的有条件调度路径核收。B 缺三实参图，C 的机器图错置调用者；另有一个正式引文跨出定义体，以及旧批输入重建、配置证据口径需要修订。分别登记 R131-02、R131-01。D00 的 R127-02 和 R130-01 本轮文档闭合，R130-02 的调用拥有者仍需补正。[183 合同](../183-user-init-shell-launch-yield-contract.md) · [历史三项独裁](CORE-USER-INIT-SHELL-LAUNCH-YIELD-131-D00-127-130-review.md) · [184 合同](../184-user-yield-task-selection-contract.md) · [PR17 #7090247003](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7090247003)

## 原件与远端

原 C08c／CHAT-EXEC-20 作者提交 2089bfff6f4a 的唯一父为 7e52edee26b4，比较领先 1、落后 0、1 个提交。下表全部为新增，共 5,221 行、185,935 UTF-8 字节。固定提交与活动原 exec 各取全文，两读逐字相同，按 Git blob 头和内容重算的 SHA-1 与两次 GitHub 返回的对象逐件相同。PR17 发布前共 251 条顶层评论，无 since 分页 100/100/51/0 至空，作者正式标记仅 #7090247003，独立按 ID 两读与分页正文一致。[原始比较](https://github.com/08822407d/MyOS2/compare/7e52edee26b4...2089bfff6f4a) · [PR17 #7090247003](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7090247003)

| 文件 | 新增行 | 字节 | Blob 前 12 位 |
|---|---:|---:|---|
| MANIFEST.md | 135 | 11,962 | 4b54c0f3cd7b |
| init-shell-launch-yield.md | 80 | 9,963 | 3eb3d21e78cb |
| facts-and-boundaries.yaml | 3885 | 120,917 | 4ad31d127234 |
| init-shell-launch-yield-paths.dot | 392 | 23,902 | 8630c517cac0 |
| deferred-findings.yaml | 99 | 4,987 | 6867a35d4e09 |
| evidence.md | 630 | 14,204 | 0c4b9b5d413e |

## A · 同一个 fork 调用的四个终点

固定 CMake 文本建立 init、sh 两个目标并安装到 boot。init 的 main 依次请求把 /dev/console 以 r、w、w 接到 stdin、stdout、stderr，三次均丢弃返回，再打印带两个换行的欢迎语，最后返回 normal_boot(argc, argv) 的结果。normal_boot 只调用一次 fork。以下 getpid 每条选中路径各一次，结果进分支内局部变量。[initd.c](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myinit/initd.c#L9) · [目标与安装](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/CMakeLists.txt#L47)

| 给定 fork / execve 结果 | 选中路径与边界 |
|---|---|
| 0，execve 成功 | 子分支取 pid，传入三个实参；若成功替换，调用现场没有后续返回。 |
| 0，execve 返回 | 子分支落到 int normal_boot 尾部，main 使用该结果；源码无法给出确定的退出值。 |
| +1234 | else 取 pid，反复请求 sched_yield；该函数体无 wait、再次 fork 或重启动作。 |
| −1 | 同一 else、同一取 pid 和循环。源码没有为 fork 失败设置单独恢复分支。 |

[VERIFIED myinitramfs/myinit/initd.c::main]
```c
	return normal_boot(argc, argv);
```

## B · 对象、别名与字节

独读 initd.c:24–35，核 ASCII、十六进制、两个指针表的槽序和终止。filename 与 args[0] 都指向同一局部 prog_name，内容 /boot/sh 为 8+1 字节。args[1..2] 为 userarg_test_1/2，envs[0..2] 为 userenv_test_1/2/3，五个不同字面量各 14+1 字节。两表第 3 槽（从 0 数）均为空指针；六个字符串各有一个字节 NUL。外层 argc/argv 没有代替局部 args/envs。[实参及对象](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myinit/initd.c#L24) · [机器表](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/facts-and-boundaries.yaml#L1693)

| 口径 | 串引用或目标数 | 内容字节 | 连同串尾 NUL |
|---|---:|---:|---:|
| filename、args 三项、envs 三项逐次计 | 7 | 2×8＋5×14＝86 | 93 |
| 按不同目标去重 | 6 | 8＋5×14＝78 | 84 |

已核的表和正文足以重算。183 还要求分别画出三个实参。DOT 仅有历史六簇、五个 ERR 簇、A_INIT 和 C_YIELD，两枚独立说明节点也无指针图。facts.B 有表；facts.graph 与 DOT 均无 B 的节点或边。R131-02 要求用两种图载体呈现三根、两张表、槽位、两条指向同一 prog_name 的边、末槽及字符串终止语义。真实地址、栈和实际进入 sh 仍 UNKNOWN。[DOT 从 A 直接进入 C](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield-paths.dot#L347)

## C · 谁发起调度调用

固定 x86-64 号为 24。活动映射项与 switch 宏给出内核候选入口，真实用到的用户态库及调用桥接 UNKNOWN。系统调用体取得 rq，按 current 的调度类间接调用 yield_task(rq)；回调正常返回后，依源码顺序分别调用 preempt_disable、sched_preempt_enable_no_resched、schedule，返回 0。rq_unlock_irq 是注释。给定 myos_rt 类，表项指向 yield_task_fair_myos，该函数调用 resched_curr。[系统调用体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/syscall.c#L29) · [类表项](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c#L66) · [标志门](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/scheduler_core.c#L739)

| 给定 resched_curr 输入 | 源码所达语句 |
|---|---|
| 标志已设、同 CPU | 先返回，跳过 CPU 比较和两个 setter。 |
| 标志未设、同 CPU | 取 cpu_of、与当前 CPU 比较，依次调用两个 setter 后返回。 |
| 标志未设、异 CPU | 比较后落到函数尾；远端发送语句为注释。 |

[VERIFIED mykernel/sched/scheduler/myos_rt.c::myos_rt]
```c
	.yield_task			= yield_task_fair_myos,
```

[VERIFIED mykernel/sched/syscall.c::sched_yield]
```c
		schedule();
```

R131-02 的另一处是两个载体都把 C_YIELD_preempt → C_YIELD_schedule 标成 DIRECT_CALL。调用 schedule 的源码拥有者是系统调用体。rq 到 callback 提供的是实参，不能充当系统调用体发起的间接调用。要分别画出调用者、函数指针、给定类表项、回调的直接调用和语句先后。A 的现有复合节点也须按 main 调 normal_boot、后者调 fork 来读。锁与抢占实际效果、特定子进程获选、唤醒与公平性均未获运行证据。[两条 C 边](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield-paths.dot#L381) · [机器边](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/facts-and-boundaries.yaml#L3799)

## R131-01 · 引文质量、配置依据与旧输入归属

60 个独立记录来自 22 个固定 time 文件。48 个带正式标记的短窗、12 个定位窗各自的 1–5 物理行均逐字匹配（含制表符与 CR）。六件机械前缀 48，全部在 evidence，作者 P6 数字 48/48/0 相加成立。E131-044 包含 myos_rt.c:65 的类表定义头；支持 yield 回调的是初始化器内部第 66 行。将 65 行改作定位，按旧 ID 给出第 66 行为替代正式窗。E131-042 位于结构体体内的成员声明可保留。P4 所需的编译配置，在 E131-053 只有 CMake 第 41 行的编译器金丝雀定位。对内核路径的否定性判断还需给与该范围相关的活动配置正文短窗，明确这是配置依据，并把判断限定于所读函数。[E131-044](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/evidence.md#L449) · [活动配置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L58)

本次主审从固定 CMAKE_C_FLAGS 配置正文核对了架构及系统调用包装器的配置记录。下列短窗说明所读内核配置的范围，具体调用仍由相应函数体支持。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
```

旧 130 的虚拟 inputs_read 把 131 新专题的 initd.c 列作从旧证据重建的输入，并漏掉四份有旧短窗的源码。184 要求给出有出处的归属和适用口径；详见同轮历史裁定。131 自己列的 49 条路径独立核实存在（lead 17、旧 exec 10、time 22）。直接 master 公约原句、两条 time 金丝雀及 master 反向差异成立。六件未检出 40 位十六进制串、协议 P9-11 的四个禁语或内部引用句柄。两份 YAML 均可按 JSON 解析。DOT 声明 174（172 个模型节点＋2 个说明节点），边 190；模型标签与类型边和 facts.graph 对应，缺端点 0。[作者 MANIFEST](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/MANIFEST.md#L128) · [虚拟读件](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/facts-and-boundaries.yaml#L1345)

文档补正限 184 D00。第 131 批原件的作者交付有效，主线状态 RETURN。完整本地 P9、预处理、编译链接、仓库程序与脚本、Graphviz、QEMU、实机及真实进程、调度、文件操作均为 NOT_RUN。
