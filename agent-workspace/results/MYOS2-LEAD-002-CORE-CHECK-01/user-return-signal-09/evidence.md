---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-RETURN-SIGNAL-09
record_type: user_return_signal_evidence
produced_by: "Claude Code desktop app (Code tab), local session f464c1b9-9b25-4af2-8f2d-d419b3b601fd (MYOS2-A-CC02 内核分析本地执行)"
model: unknown_or_not_attestable
model_declared_by_runtime: claude-opus-5-5
date: "2026-10-29"
base_snapshot: "kernel=time a039d9803ade；执行分支起点 8983a9235c8a；主线 agent/MYOS2-LEAD-002 14ecef9fc596；master e8e60cd19540（短 SHA）"
inputs_read: "见 §2"
status: final_for_user_return_signal_09
runtime_evidence: NOT_RUN
open_questions:
  - "推送后的写区复核（boundary --commit）与远端读回写在 PR17 评论与最终回复中，本文件不自引用其所在提交。"
---

# CORE-USER-RETURN-SIGNAL-09 证据与方法

本文记录本批实际做过的事：绑定核对、读取范围、命令、子代理的使用与合规审计、核对结果、审查与更正，以及写区边界。

仓库命令都只读 git 对象：开工时一次 `git fetch`，只更新四个远端跟踪引用。PR17 的状态用 gh 只读查询，其余是用 Python 处理文本、YAML、JSON。没有编译、预处理或运行 C/ASM，没有运行 cmake、内核、QEMU、宿主安装/挂盘/分区脚本或仓库原有脚本，也没有重跑 05/06/07/08 的检查器。唯一的例外是核对脚本以 import 方式复用 03 包 scan_index.py 的读取函数：不调用其 main，不运行 03 的检索或检查，见 §3。

Python 运行时都设置了 `PYTHONDONTWRITEBYTECODE=1` 和 `-B`，交付脚本在 import 之前设置 `sys.dont_write_bytecode = True`。每次运行前后，clone 内都没有 `__pycache__`。

## 1. D00 绑定

| 项 | 所见（工具输出） |
|---|---|
| 主线对象 | 在 clone 中 `git fetch` 了 `agent/MYOS2-LEAD-002`、`time`、`master` 与执行分支四个引用（只更新远端跟踪引用）；主线头 `14ecef9fc596`。相对 08 时所读 `91753f8155ee`，新增 C02→C03 交接的四个文件、23 号任务书、08 主线回执与 10-28 检查点，属主线正常追加 |
| 任务书 | 23 号全文从主线对象读取（160 行） |
| 08 主线回执 | `record_id: CORE-USER-SYSCALL-STDIO-08-REVIEW-001`，`reviewed_head_short12: 8983a9235c8a`，`returned_scope: [SR08-01, SR08-02]`，`next_followup: CORE-USER-RETURN-SIGNAL-09`，与任务书一致；RC08-01 一并读取 |
| 18 号约定 | 与 08 时所读对象相同（`git diff` 为空），全文已读 |
| 检查点 | `2026-10-28-c03-stdio08-reviewed-return-signal09-ready.md` 全文 |
| 源码 | 远端 `time` 仍为 `a039d9803ade`，与任务书固定对象相同，无需另列差异 |
| 工作区 | 远端 `master` 为 `e8e60cd19540` |
| 执行分支 | 本地与远端头都是 `8983a9235c8a`；工作树干净 |
| PR17 | open、Draft、未合并，头 `8983a9235c8a`，基 `master` |
| 隔离 clone | 沿用 06 建立的 `/home/cheyh/projs/myos2-handoff06-local/MyOS2`，所有仓库命令以它为根；没有触碰 Owner 的开发工作树 |
| 配置面 | 先读 `mykernel/scripts/options_flags.cmake`：`-D__KERNEL__`、`-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER`、`-DCONFIG_BUG` 在内核编译选项中；`CONFIG_IA32_EMULATION`、`CONFIG_X86_X32_ABI`、`CONFIG_INTEL_X64_GDT_LAYOUT` 不在其中 |

## 2. 实际读取范围

- **主线：** 23 号任务书全文、08 主线回执全文、18 号（与 08 时同一对象）、10-28 检查点全文。
- **08、07、05、04 包，以及为开放点核对的 03、02、core 与 DR-005 包（8983a9235c8a）：**
  - 08：`user-syscall-stdio.md`、`deferred-findings.yaml`、`facts-and-boundaries.yaml` 中与 SR08-01、SR08-02 相关的位置，以及衔接与旁证用到的 VT-SYSTAB、US-05、NVS-1 等条目。
  - 07：`deferred-findings.yaml`（UB-01、UB-05、UB-06、UB-07、UB-09、UB-10）；`facts-and-boundaries.yaml` 的 FP、SB、AB、QB、EB、NVB 条目，以及 VT-PTREGS、VT-KFRAME、VT-FLAGS。
  - 05：`deferred-findings.yaml`（UV-02、UV-03、UV-04、UV-05、UV-09、UV-10、UV-11、UV-14、UV-15、UV-16，以及 05 对 CORE-MM-BASELINE-04::MC-23 的状态条目）；`facts-and-dependencies.yaml` 中被引用的条目。
  - 04（mm-baseline-04）：`facts-and-dependencies.yaml` 中的 MC-22、MC-23、LB-04、LB-05、E27、RF-02、DR-01、DR-04、DR-05。
  - 03、02、core 与 DR-005：为 RS-10 与第 5 个主线开放点核对已有登记。
    - scheduler-order-02 的 `results.yaml`、`evidence.md` 与 core 包中的 FPU 文本；
    - scheduler-integration-03 `facts-and-regressions.yaml` 中的 SF-C5-18 与 GAP-03；
    - 终审时补查的 `MYOS2-DR-005/MANIFEST.md` 高风险 correctness 第 5 条与 `cards/fpu-xstate.md` §4。

  全部由只读子代理与本会话按需定点读取。
- **源码（time a039d9803ade）：**
  - D01：`arch/x86_64/entry/common.c` 全文；`entry/common.c` 全文；`include/linux/kernel/entry-common.h` 中被调用的部分；`arch/x86_64/entry/entry_64.S` 的 `entry_SYSCALL_64`、`syscall_return_via_sysret`、`POP_REGS`；`arch/x86_64/sched/context/process_arch.c::ret_from_fork`。
  - D02：
    - 全文：`lock_IPC/syscall.c`、`arch/x86_64/lock_IPC/signal/signal.c`、`signal_64.c`。
    - `lock_IPC/signal/signal.c` 中的 `get_signal`、`dequeue_signal`、`dequeue_synchronous_signal`、`collect_signal`、`do_sigaction`、`sigprocmask`、`set_current_blocked`，以及它们直接使用的宏。
    - 相关定义：`lock_IPC/signal/signal_const.h`、`signal_types.h`、`signal.h`、`arch/x86_64/include/asm/signal.h`、`include/uapi/linux/signal.h`、`include/uapi/asm-generic/signal-defs.h`。
    - 实际使用的访问函数：`arch/x86_64/include/asm/uaccess.h`、`uaccess_64.h`、`include/linux/kernel/uaccess.h`。
    - `sched/forkexec/fork.c` 中 `copy_sighand`、`copy_process` 的相关行；`arch/x86_64/mm/fault.c` 的缺页去向。
  - D03：`arch/x86_64/include/asm/syscall.h` 全文；`include/linux/lib/errno.h`、`include/linux/kernel/err.h`、`include/uapi/asm-generic/errno-base.h` 中的相关定义；`asm/unistd_64.h` 与 `asm/syscalls_64.h`。
  - D00：
    - 相关函数：`include/linux/fs/file.h`、`fs/vfs/read_write.c`、`fs/syscall.c`、`fs/vfs/fcntl.c`、`fs/vfs/file.c`、`fs/vfs/file_table.c`。
    - 相关行：`include/linux/kernel/asm-generic/bug.h`、`debug/panic.c`、`include/linux/debug/panic.h`、`include/linux/compiler/compiler-gcc.h`、`include/linux/fs/fs.h`。
    - 指定行：`block/fops.c` 第 68–75 行、`fs/vfs/open.c` 第 326 行、`include/linux/compiler/compiler.h` 第 106–107 行。
  - 超出一层直接辅助函数的定点读取。这是依赖边界，按任务书 §3 列出；只为闭合所列条目的前提，不代表覆盖相应子系统：
    - 发送链：`lock_IPC/syscall.c::sys_kill` → `lock_IPC/signal/signal.c` 的 `kill_something_info`、`kill_proc_info`、`group_send_sig_info`、`send_signal_locked`、`__send_signal_locked`（RS-03、RS-14）。
    - 唤醒与等待：唤醒置位点 `signal_wake_up_state`、`calculate_sigpending`；等待点 `kactive/completion/completion.c::wait_for_completion`（RS-04、D03.5）。
    - 异常与缺页去向：`arch/x86_64/myos/interrupt.c` 的 `exception_handler`、`excep_gen_prot`、`excep_hwint_context`；`mm/vm_map/vm_map.h`、`arch/x86_64/mm/page/page_const_arch.h`（RS-06、RS-08、RS-11）。
    - 锁与抢占：`lock_IPC/spinlock/spinlock_smp.h`、`sched/scheduler/preempt_macro.h`（D01.5、D02-b）。
    - FPU：`arch/x86_64/sched/context/process64_arch.c::__switch_to`，以及 `arch/x86_64/kernel/fpu/init.c` 的一行（RS-10）。
    - 任务初值：`init/init_task.c`、`arch/x86_64/include/asm/processor.h::INIT_THREAD`、`arch/x86_64/processor/processor_types_arch.h`（FF-12、RS-04、RS-12、RS-13）。
    - 相关定义：`arch/x86_64/lock_IPC/signal/signal_types_arch.h`、`include/uapi/asm-generic/ucontext.h`、`include/uapi/asm-generic/access_ok.h`、`lock_IPC/signal/signal_macro.h`、`lib/list/double_list_macro.h`。
    - 其他：`arch/x86_64/entry/syscall_64.c`（RS-05）；`mykernel/CMakeLists.txt` 与 `arch/x86_64/CMakeLists.txt`（RS-01）；`block/fops.c::blkdev_llseek`、`fs/vfs/libfs.c::dcache_dir_lseek`（D03.5，SD-41、SD-42）；仓库内用户程序 `myinitramfs/myshell/tests.c`（RS-01、RS-09、RS-12）。
  - 审查与修正轮补读的源码，都是定点读取：
    - 为 RT-36 读了 `sched/forkexec/exec.c` 第 1195–1221、1258–1271 行，`fs/vfs/binfmt_elf.c` 第 1115、1131–1134、1218 行，以及 `arch/x86_64/sched/context/process64_arch.c` 第 269–304 行的 `start_thread_common`。
    - 为核对锚点 role 读了 `include/linux/kernel/types.h` 中 `reg_t` 的定义，以及 `sched/task/task_const.h` 中的任务状态取值。
    - 为核对 AD1-04 的槽位推演读了仓库内已提交的 `include/generated/asm-offsets.h`。它是固定对象中的文本，与 07 VT-PTREGS 同源，不是本批的构建产物。
    - 为核对 D01.5 的推断方向读了 `sched/scheduler/myos_rt.c` 中 `resched_curr` 的调用点。
  - 只作为按名检索的命中、没有通读：
    - `sched/forkexec/exit.c`、`sched/forkexec/kthread.c`、`sched/syscall.c`（QD-179、QD-180）。
    - `sched/scheduler/scheduler_core.c`（QD-166、QD-224）。
    - `lock_IPC/spinlock/spinlock_smp.c`（QD-31）。
    - `lib/errname.c`（QD-16、QD-53）。
    - `scripts/target_kbuild.cmake`（QD-147）。
    - `myinitramfs/myshell/myshell.c`（QD-127、QD-150、QD-188）。

    检索范围与返回码见 facts 的 queries。
- **没有读：** 仓库外 libc、历史主题分支 libc、构建产物与镜像、FPU/XSAVE 内部、copy_user_generic 的汇编后端；发送端与唤醒的完整实现也没有读，只按上列依赖边界定点读过 sys_kill 一条发送链和唤醒置位点。

## 3. 方法

- **执行方式与模型。**
  - **偏离默认档。** 任务书默认“次一档”，08 回执 §7 也写明“Deep Research和额外并行评审当前不需要”。Owner 在本地会话 f464c1b9 中明确开启 ultracode，要求子任务以 Opus 5.5、max 推理强度运行；这是执行方转述，主线不能直接核验。本批据此偏离上述默认。
  - **规模。** 用七个工作流组织只读子代理，共 130 个完成的代理。前两组成稿，后五组审查、修正与终审：
    1. 追踪组 41 个：7 个追踪者分区读源码（返回尾、设置接口、get_signal、建帧、sigreturn、重启、D00）；1 个合并者去重，并对照 05/07/08；32 个核验者从“源码属实”与“范围与是否说过头”两个视角逐条反驳 16 条候选；另 1 个复核 D00 草稿。
    2. 写作组 8 个：按同一套 ID 骨架写 D00–D03 各节与条目正文，并采纳上一组的全部更正。
    3. 独立审查 55 个：5 个视角各 1 个审查者，50 个复核者逐条复核中、高级问题。另有首次启动的 5 个审查代理因订阅周限额失败，没有产出；额度重置后从断点续跑。
    4. 修正一轮 5 个、修正二轮 6 个、修正三轮 12 个。分工与结果见 §5。
    5. 提交前的终审 3 个：分别通读主说明、D00 处置与勘误、清单与证据，找出跨轮修正留下的矛盾与过时数字；结果见 §5。
  - **额度与模型。** 全部在订阅额度内，没有转 API、额外付费或外部研究。子代理按要求不写文件，只返回结构化结果，由本会话汇总与落地；审计发现两处向会话临时目录写中间文件的例外，见 §5。子代理的模型都只是运行时声明，实际后端 unknown_or_not_attestable。
- **组装。**
  - **编号。** 一次性组装程序没有交付，它把写作者的占位引用换成编号。组装时被引用的源码行共 266 段。
  - **锚点分配。** 取其中 75 段作锚点：D01、D03 全部；D00 只取 SR08-02 的 22 段；D02 22 段，以用户内存访问、缺页去向和设置接口为主。这一分配与任务书 §7 的优先次序不一致，详见 facts 的 anchor_policy。其中 6 个锚点被其他锚点完全包含，不被包含的为 69 个；这 69 个中仍有 4 对部分重叠（AD1-01 与 AD3-01、AD3-07 与 AD1-07、AD3-10 与 AD1-10、AD0-13 与 AD3-12），把有公共行的锚点合并后是 64 段互不重叠的行范围。
  - **整行定位。** 其余 191 段改由按文件分组的整行锚定检索 QD-L01…QD-L41（`git grep -P '^\Q…\E$'`）给出命中。审查更正后，这 41 个检索共有 182 条整行模式。结构化字段中有 353 处命中引用，落在 179 个不同的行上，每处都带 `expect` 文字（该行必须含有的标识或表达式）。
  - **引文代码块。** 正文中的 `[VERIFIED]` 代码块按引用行从固定对象逐字取出。
- **核对脚本覆盖什么。** `rs_check.py` 机械核对以下各项：
  - 锚点：引文逐字、所在符号、#if 条件、至少一行活动。
  - 锚点 `role` 文字：只查格式——不得为空或占位，不得有空括号或悬空的顿号，全角括号成对，反引号成对，不得是另一条 role 的真前缀。
  - QD-L 命中：存在、为活动行，并含所标 `expect` 文字。
  - 编号唯一、引用可解析、计数一致。

  它不核对的部分：QD-L 定位行是否落在所述符号内、是否足以支撑所述结论；正文行内的“文件:行（QD-Lxx）”；role 的语义。这些由审查与第三轮修正逐条复核（§5）。
- **核对脚本的依赖。**
  - **例程来源。** `scripts/rs_check.py` 的锚点、引文、检索与结构检查例程抄自并精简自 08 的 `ss_check.py`，不导入、不运行它。
  - **复用 03 的读取函数。** 与 07、08 相同，脚本以 `importlib` 从 clone 工作树载入 03 包 `scheduler-integration-03/scripts/scan_index.py`，只调用 `show`、`analyse`、`enclosing`（注释掩码、#if 条件栈与符号范围；`analyse` 内部调用 `lex`）。这样，活动/注释与所在符号的判定沿用 05/07/08 的同一词法口径。
  - **载入的边界。** 载入只执行该模块的顶层定义，不调用其 `main()`，不运行 03 的检索、检查或输出。载入后先核对其 PIN 为 a039d9803ade，再把其 REPO 从云端路径 `/home/user/MyOS2` 重绑到本 clone 根。载入前已设置 `sys.dont_write_bytecode`，冻结的 03 目录中不产生 `__pycache__`。
  - **旧件不变。** 这份工作树副本属于 8983a9235c8a 的冻结旧件。提交前 `boundary --staged` 对全部旧文件做完整对象比较，并要求没有未暂存改动，同样覆盖它。
- **核对口径。**
  - **锚点与引文。** 锚点与正文引文要求逐字一致，并落在所标符号内。
  - **检索期望值。** 检索的期望返回码与命中数取自实际运行。取值前，先逐条比对写作者与修正者声明的语义（正向 188 个、无活动命中 31 个、只有注释命中 43 个）：262 个检索没有一个相矛盾。
  - **结构检查。** 110 项结构检查的期望计数由写作者手工给出，与脚本重算全部相同。
  - **值表。** VT-RESTART 由脚本从 `#define` 与活动分派表重算。
  - **分栏记录。** 机械计数、文本匹配、语义审查和运行验证分栏记录（§4）；`all_ok` 不代替条件完整性。
- **审查更正怎么落地。**
  - **补丁形式。** MANIFEST.md 与 evidence.md 由本会话手工编辑。其余每一处更正都写成补丁，交给未交付的生成程序应用：精确子串替换，或按条目 ID 与字段定位的增删改。补丁在组装输出之上依次应用，任何一条的原文找不到、或出现不止一次，整次生成就中止。所以另外五个交付文件可以从组装结果和补丁表完整重放。
  - **主会话的检查。** 每轮之后都重跑核对脚本，再与声明语义比对检索结果。
- **否定结论**都带检索词、路径范围、返回码与未排除项（`records/queries.json`）。“未见”只表示所读快照与检索范围内未见。
- **不做的事：** 不预处理，不编译；不读仓库外 libc；不把 Linux 的通常做法当作 MyOS2 现状；不把“ip -= 2、号写回”的重启意图当成已重新执行。

## 4. 核对结果

记录在 `records/rs_check.json`，命令与返回码在 `records/run.json`，逐条检索在 `records/queries.json`。

**机械计数与文本匹配**

| 项 | 结果 |
|---|---|
| 锚点 | 75/75 通过（asm_range 2、in_body 60、macro 13）；role 格式检查 75 条，0 处问题 |
| 正文 `[VERIFIED]` 引文 | 16/16（user-return-signal.md 9、stdio-08-disposition.md 7） |
| 检索 | 262 个，工具失败 0，返回码与总、活动命中数全部等于期望值 |
| 命中引用 | 353 处全部是所属检索的活动命中，并且 353 处都含所标 `expect` 文字 |
| 结构检查 | 110/110 |
| 值表 VT-RESTART | 重算一致：ERESTART* 与 EINTR 的值，SA_RESTART/SA_RESTORER 位，MAX_ERRNO、__NR_syscalls，219、15 号的定义，以及 219、15、13、14 号是否在活动表中 |
| 事实与延期文件结构 | 0 处错误：ID 唯一、引用可解析、计数一致；从 8983a9235c8a 读入 05/07/08 的 732 个 ID 用于解析；勘误原文 63 段逐段与原件比对一致 |
| 上限 | 能力 10/15、边 21/40、锚点 75/75、候选 4/4 |
| DOT | 15 个节点、21 条边，与事实文件 ID 一致 |
| 40 位十六进制 | 包内 0 处 |
| `all_ok` | true |

**语义审查：** 见 §5。机械核对通过不等于结论正确；条件、取值与推断标注由审查与三轮修正逐条复核。

**运行验证：** NOT_RUN。没有运行内核、用户程序或 QEMU，没有观察任何寄存器或计数。

## 5. 自查、审查与更正

**独立审查**

- **审查设置。** 成稿后按五个视角各派 1 个审查者：R1（D01、D03 与衔接）、R2（D02）、R3（D00 与 08 处置）、R4（入库条目）、R5（与任务书的一致性）。
- **审查结果。** 共提出 90 个问题：高 7、中 43、低 40。50 个中、高级问题逐条交给复核者，结果如下：
  - 确认 48 个，其中 46 个需要调整修法；
  - 驳回 2 个：一是 evidence 的 §4–§6 占位，最终运行时填写；二是把 ED-20 记为间接边。

  40 个低级问题由后面的修正者自行回源判断。
- **一次失败的处理。** 首次启动的 5 个审查代理因订阅周限额失败，没有产出；额度重置后从断点续跑。没有转 API。

**修正与落实**

1. **第一轮（5 个，每个视角 1 个）。**
   - **补丁去向。** 把确认意见转成 278 个补丁：249 个经生成程序应用，21 个是 MANIFEST 与 evidence 的文字，由本会话手工落实。另有 8 个没有用：
     - SR08-01.summary 上两个视角的冲突，取 R5 的写法；
     - 1 个手写的 counts 块与 6 个手写的期望命中数，都改为由脚本重算。
   - **互相推让而漏掉的项。** 合并时逐条对照各视角的未落实清单，发现几处被互相推让，没有任何一方落地：
     - L2 的必要前提；
     - L5、L6 与 RT-10 的内核访问用户地址去向；
     - 18 条锚点 role；
     - RS-01、RS-02、RS-08、RS-09 对 signal_64.c:171 的误引；
     - 需要新增整行模式的帧字段行号。

     这些都转入第二轮。
2. **第二轮（6 个）。**
   - **分区与范围。** 按区各派 1 个负责者与 1 个复核者：D01/D03 与衔接、D02 与条目、D00。负责者先对照源码复核第一轮在本区的全部改动，再补上漏掉的项。
   - **补丁去向。** 负责者共出 178 个补丁，复核者保留 167 个、改写 11 个、丢弃 0 个，另补 3 个，共应用 181 个。
   - **漏项的落实。** 第一轮漏掉的项在这一轮全部落实。例如：
     - L2 末段写出“返回尾部不写帧”的必要与充要条件；
     - 内核访问用户地址出错时按地址分为 myos_bad_area、do_kern_addr_fault、excep_gen_prot 三种去向，命中 VMA 而处理失败时在 do_user_addr_fault 中自旋，全部标为推断；
     - QD-L09、QD-L10、QD-L14、QD-L31、QD-L07 追加了 20 条整行模式，用来改正 FF-01、FF-03、FF-11、FF-15 等的错行；
     - 新增检索 QD-223～QD-225（RS-04 的等待与唤醒调用分布）与 QD-233（HAVE_ARCH_BUG 的判断）。
3. **第三轮（12 个）。**
   - **锚点 role。** 3 个复核者逐条复核全部 75 条锚点 role，改写 24 条，其余 51 条核对后不改；3 个复核者再逐条核对改写，保留 22 条、修订 2 条。
   - **expect 文字。** 4 个标注者给全部 354 处命中引用标注 expect。其中 344 处直接标注；10 处引用了相邻行或错行，已改正，涉及 RS-01、RS-06、RS-07、RS-08、RS-10、RS-13、ED-07、ED-10、ED-13 与 arch.x86_64.user_access。其中 RS-10 的一处是用单行支撑否定结论，改为只引 QD-136，所以最终为 353 处。
   - **跨文件一致性。** 1 个修正者把 L2、stdio-08-disposition.md §3.3、SR08-02.summary 与 ER-02.correction 中的条件统一成同一套编号与口径，1 个复核者改写其中 6 条。
4. **本会话。**
   - **核对脚本的修改。** 按 R5 两条已确认意见给核对脚本加了两项检查：
     - 命中引用的 `expect` 文字核对；
     - role 格式检查。
   - **生成程序与收尾。**
     - 表格生成改为把“|”转义为“\|”。
     - 删除 4 条已无人引用的整行模式。
     - 补正 RT-04 与 D01.2、D02-a 两处措辞，在对 UB-09 的证据追加中补上 get_signal 放锁处的抢占入口（D01.5）。
     - 改写 facts 的 execution_note 与 anchor_policy，并统一日期。
5. **提交前的终审（3 个）。** 三个只读复核者分别通读：
   - 主说明；
   - D00 处置与勘误；
   - 清单、证据与两份 YAML 的头部。

   共提出 27 处问题（中 4、低 23），本会话逐条回源核对后全部改正。中级的四处是：
   - D02-c 把“没有内核侧活动产生者”说成无条件事实，已按 D03.5 补上检索范围与“不能排除”的部分；
   - stdio-08-disposition.md §3.5 表第 1 行漏了 (int)orig_ax ≠ -1 的门限，与 15 号 rt_sigreturn 的情形矛盾，已补；
   - “不重叠的锚点 69 个”不对，69 个中仍有 4 对部分重叠，合并后是 64 段，已在 anchor_policy、本文 §3 与 MANIFEST 改正；
   - 第 5 个开放点说“已有包都没有登记”任务切换的 FPU 缺口，实际 MYOS2-DR-005 已有速查结论，已把范围限定为 CORE-CHECK-01 的包并注明 DR-005。

   其余是 FF-08 的旧条件、RT-10 与 RT-16 的悬空指向、对 07、08 原句的非逐字引用（ER-02、ER-03、UB-09 的证据追加、D01.7）、本文与 MANIFEST 的几处措辞和指向。

**有意没有采纳的建议（均为低级或可选项）**

| 建议 | 不采纳的理由 |
|---|---|
| 新增 L1-36 | 会改动 35/26/8/1 的计数口径，改为在 stdio-08-disposition.md §2.5 加不计数的读法说明 |
| 把 L2-06 改判为“不受影响” | 改为定义结论列口径，并在 ER-02.unchanged_parts 中说明 |
| 把“衔接”各节改名为 J1–J8 | 牵动三个文件的多处引用，收益小于风险 |
| 删去 D00 草稿计数等过程叙述 | 它们是更正记录，删去会失去可追溯性 |
| 把 AD3-02、AD3-03、AD3-08…AD3-13 的 theme 改为 D02 | 改为在 anchor_policy 中说明编号与 theme 按分配批次给出 |
| 删去被包含的 6 个锚点，重排名额 | 可选项，改为如实说明 |
| 把 ER-03.original_locations 扩到 07 facts 的 failure_path | 与 ER-03.unchanged_parts 自相矛盾 |
| RS-12 的并入目标、FN-01 与 FN-02 的登记口径 | 属主线决定，列为开放点 |

**组装前的自查更正**

- **整行检索的写法。** git grep 没有 `-x` 选项，整行检索改用 `-P '^\Q…\E$'`。
- **核对脚本的两处缺陷。**
  - 条目证据中的命中字典原本不能解析，现已改正；
  - 行条目的 concerns 原是散文，改为 ID 列表，原文保留在 concerns_note。
- **锚点名额重排。** 第一版锚点分配偏向 D00（43 个）、D02 只有 1 个，重排后为现在的 22、11、22、20。

**子代理合规审计**

- **审计范围与方法。** 逐条检查七个工作流全部 135 份代理记录（含失败的 5 份）中的工具调用：
  - Bash 6683 次；
  - Read 1693 次；
  - 结构化输出 130 次。
- **结论：** 没有写 clone，没有 git 写操作，没有运行 Python、构建或 QEMU，没有访问 Owner 的开发工作树。所有对 MyOS2 仓库的命令都以隔离 clone 为根。
- **偏离与例外：**
  - 大量只读的显示管道，如 sed -n、cat -n、head、grep；
  - 读过运行环境为长输出另存的 tool-results 文件，内容是它们自己命令的输出；
  - 2 个代理各用一次 shell 重定向，在会话临时目录写了检索中间文件：审查组的 refs.txt、第一轮修正的 other_ff.txt。都在 clone 之外，没有交付；
  - 1 个审查代理用 bash here-doc 组织只读的 git grep/show 循环；
  - 2 次调用了本环境没有的 Grep 工具，报错，没有任何效果；
  - 第三轮有 1 个代理调用过一次会话任务清单的显示工具，不涉及任何文件。

## 6. 写区与回传

- **写区范围。** 本批只在 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-return-signal-09/` 下新增文件，没有改动 8983a9235c8a 中的任何文件。
- **提交前核对（RC08-01）。** 在暂存区上运行 `rs_check.py boundary <out> --staged`，按完整对象比较 8983a9235c8a 与暂存区，并用明确的布尔条件判定：
  - 旧文件逐个完全相同；
  - 有新增；
  - 新增与差异都只在本目录下；
  - 没有未暂存或未跟踪的改动；
  - 本地头与远端执行分支头都是 8983a9235c8a。

  任一条件不满足就以退出码 2 停止，不提交。记录写在 `records/boundary_staged.json`，只含路径、计数、布尔值与 12 位短标识。
- **提交后核对。** 推送后运行 `boundary --commit` 复核远端提交，并逐个读回远端文件。结果写在 PR17 的评论中，不放进本提交，以免记录自引用所在提交。

## 7. 限度

- 这是静态源码说明与机械一致性核对，不是语义证明，也不是内核或用户程序的运行验证；所有运行层仍为 NOT_RUN。
- `expect` 文字只保证命中行含有所标标识，不保证这一行足以支撑结论；行与结论的对应由审查与修正轮的逐条复核负责。
- RS-01 由头文件包含次序推出，没有预处理产物核实；它是否成立，决定建帧侧事实在运行中是否可达。
- 硬件语义没有一手资料核对：sysretq、swapgs、sti、ip-2 是否正对 syscall 指令、#PF 与 #GP 由哪类地址触发。生成偏移的数值同样没有一手资料核对。
- libc 的行为在仓库外，GAP-U10 保持 open：是否注册 handler、restorer 是什么、是否发出越界号。
