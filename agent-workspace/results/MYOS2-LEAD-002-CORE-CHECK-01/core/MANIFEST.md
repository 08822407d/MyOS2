---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
phase: core
record_type: cloud_core_verification_manifest
produced_by: "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection"
execution_model_selection: unknown_or_not_attestable
execution_model_selection_source: "执行者所在平台的规则不允许在推送到仓库的产物中写模型标识（执行者自述；主线回执已记为执行者解释）"
execution_surface: "claude.ai/code 托管云端会话容器（Ubuntu 24.04.4 LTS，x86_64）；与 pilot 同一会话、同一执行分支"
date: "2026-09-27"
base_snapshot: "kernel=time（分支名，短 SHA a039d9803ade）；workspace=master（分支名，短 SHA de3bb1df906a）；taskbook=agent/MYOS2-LEAD-002（技术输入短 SHA 57a7c3e0eebf，回执头短 SHA f79b3a281616）；短 SHA 均自本会话 git rev-parse --short=12 输出复制"
work_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
results_commit_short12: 7e2fa84a9823
read_channel: mixed
read_channel_detail: "回执经 raw URL 读取并与 Git 对象逐字节一致；其余输入经 git fetch 后按固定提交 git show；open PR 经 GitHub MCP；results.yaml 回读经 raw.githubusercontent.com 与 api.github.com"
inputs_read:
  - "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-CHECK-01-pilot-review.md"
  - "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-09-27-pilot-reviewed-core-ready.md"
  - "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/11-core-verification-cloud.md"
  - "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/09-local-verification-contract.md"
  - "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/07-scheduler-wakeup-timer-audit.md"
  - "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/07-core-audit-map.yaml"
  - "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/MANIFEST.md"
  - "master:agent-workspace/conventions.md"
  - "master:agent-workspace/tasks/00-gpt-task-protocol-v2.md"
  - "time:V00 所列 47 个锚点源文件与夹具抽取清单中的源文件（见 evidence.md §3.1、§4.1）"
status: final_for_core
core_conclusion: CORE_CHECKS_DELIVERED_WITH_COUNTEREVIDENCE
acceptance_ceiling: PASS_PENDING_LOCAL
startup_selfcheck_quote: '2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。'
write_authority_note: "写入 core/ 与本执行分支、更新 PR #17 由 Owner 本轮继续指令与主线回执 §4 授权；未写 agent/MYOS2-LEAD-002、master、time；pilot 原件未改。"
self_check:
  verified_claims: 0
  quotes_reconfirmed: 0
  downgraded_to_inferred: 0
  note: "本批不新增源码断言标签；对 07 报告 47 条锚点的核对结果在 results.yaml V00 与 evidence.md §3.1。"
branch_canary_quotes:
  source: "agent/MYOS2-LEAD-002 MANIFEST.md front matter，PyYAML 解析，未重打"
  time:
    options_flags_cmake: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
    panic_c_panic: "\tthis_cpu = smp_processor_id();"
  cloud_mechanical_check: "DISCRIMINATES（两条均 time 整行命中、master 不命中；V00 重做）"
kernel_modified: false
repo_scripts_run: false
full_kernel_build: false
qemu_run: false
tools_installed: false
open_questions:
  - "idle 上下文是否可能以非 RUNNING 状态被切出：决定 CA-02 的组合是否可达（V08）。"
  - "有限 timeout 的活动使用者：msleep 未见活动调用者；semaphore 的 down_timeout 路径调用者本轮未查（V09）。"
  - "真实 ELF、时基、IRQ、SMP 与上下文切换仍待本地（V11 动态层、V14 ELF 层未执行）。"
---

# CORE-CHECK-01 core：十五项已执行，两项反证需主线处理

**V00–V14 全部在本云端会话中执行（V11 的真实上下文切换层、V14 的 ELF 层未执行）：11 项与预测一致，2 项串行范围内无失败，2 项给出反证。** 反证是 A46 引文越出所标符号定义体（1/47），以及 CA-02“阻塞 current + 空队列”组合在 idle 重新入队不变量下不可达（有前提）。结果不是 MyOS2 的任何范围通过，验收上限仍为 PASS_PENDING_LOCAL。

## 范围与输入

主线回执 `ALLOW_CORE` 已机械核对并绑定本执行：packet、PR #17、分支、pilot 头 `0851af4fc08b` 与回执中的输入短标识全部一致；主线分支从 `57a7c3e0eebf` 前进到 `f79b3a281616` 只新增回执与检查点，六份原技术输入逐字节相同（回执 §4）。源码固定为 time `a039d9803ade`，所有读取按固定提交进行。

本轮先完成回执 §3 的三项驱动加固，然后直接执行 V00–V14。只在 `core/` 新增文件；未改内核、未运行原仓库脚本、未构建内核、未启动 QEMU、未安装工具、未输出凭据。

## 交付文件

| 文件 | 内容 | 边界 |
|---|---|---|
| `results.yaml` | V00–V14 每项的执行状态、结果、证据类型、输入身份、前提、实际值与引用；CA-01…CA-07 裁定；准入与输入身份；加固结果 | 由 `make_results.py` 从正式运行输出生成；于 `7e2fa84a9823` 提交后冻结，不含回读结论 |
| `evidence.md` | 准入与身份、加固探针、47 引文逐项表、V01、夹具构建与替换清单、每个案例的命令/退出码/原始事件行/判定、静态核查、开发期失败与修正、推送与远端回读、范围检查 | 由 `build_evidence.py` 生成；§1.1 两段为手工转录并已标注 |
| `MANIFEST.md` | 本清单 | 不替代主线审查 |
| `fixtures/`（20 个文本文件） | `harness.py` `locate.py` `build.py` `readback.py` `h00_hardening.py` `v00_anchors.py` `v00_semantic_review.yaml` `v01_structure.py` `static_checks.py` `evaluate.py` `run_all.py` `make_results.py` `build_evidence.py` `final_check.py` `fx_common.h` `fx_list.inc.c` `fx_wait.c` `fx_sched.c` `fx_prims.c` `fx_jiffies.c` | 夹具模板在构建时才从固定 time 提交逐字复制原函数；仓库中不含二进制或整份源码 |

复现：在本仓库检出本分支后，`cd core/fixtures && CORE_WORK=<新目录> python3 run_all.py`，再运行 `make_results.py`。需要 git、Python 3 + PyYAML、gcc；V12/V13 需要 x86-64 主机。

## 逐项结论

| 项 | 执行 | 结果 | 证据类型 | 要点 |
|---|---|---|---|---|
| V00 | 是 | COUNTEREVIDENCE | SOURCE_MATCH | 47 条全部逐字连续命中；46 条在所标定义体内；A46 第二行是 `KERNEL_ASM_SRCS` 的定义，越出 `KERNEL_C_SRCS`；A15 含一行有意引用的注释；A38 的三行在文件中出现两处，靠边界定位 |
| V01 | 是 | NO_FAILURE_IN_SCOPE | SOURCE_MATCH | 六文件可解析；CA-01…07 齐全；A/V 引用无悬空；旧 MANIFEST 自报 47 与机械计数一致 |
| V02 | 是 | OBSERVED_AS_PREDICTED | HOST_ORIGINAL_SLICE | 唤醒摘链后 count 保持 1、链上 0 节点；finish 不补减 |
| V03 | 是 | OBSERVED_AS_PREDICTED | HOST_ORIGINAL_SLICE | 第二次唤醒、全唤醒第三轮都把 anchor 当作 waiter，其 task 字段就是队列锁字（空闲为 0，complete 持锁时非零）；在交给真实函数前停下 |
| V04 | 是 | OBSERVED_AS_PREDICTED | HOST_ORIGINAL_SLICE | 非 current 唤醒：入自有队列、置 RUNNING，返回 0 |
| V05 | 是 | OBSERVED_AS_PREDICTED | HOST_ORIGINAL_SLICE, SOURCE_MATCH | 状态不在 mask 内（含 TASK_NEW）仍被唤醒入队；与函数注释合同及 sched_fork 注释冲突，单列为设计冲突 |
| V06 | 是 | NO_FAILURE_IN_SCOPE | HOST_ORIGINAL_SLICE | 串行连续唤醒三次只一个节点；并发未测 |
| V07 | 是 | OBSERVED_AS_PREDICTED | HOST_ORIGINAL_SLICE, SOURCE_MATCH | set_task_cpu 不更新任务 CPU 元数据；唤醒与新任务都放 CPU0；所读快照中未见 `__set_task_cpu` 或 `thread_info.cpu` 的活动写入 |
| V08 | 是 | COUNTEREVIDENCE | HOST_ORIGINAL_SLICE, STATIC_COUNTEREXAMPLE, SOURCE_MATCH | 函数层面确实返回阻塞的 current；但 idle 以 RUNNING 切出时会被挂回队列，因此非 idle current 下 count=0 不可达，除非 idle 以非 RUNNING 状态被切出（夹具序列显示一旦如此即可到达） |
| V09 | 是 | OBSERVED_AS_PREDICTED | HOST_ORIGINAL_SLICE | 有限 timeout 原样返回、不调度、状态保持 UNINTERRUPTIBLE；msleep 100 步不前进；负值返回 0；MAX 调度一次 |
| V10 | 是 | OBSERVED_AS_PREDICTED | HOST_ORIGINAL_SLICE | 预置 done 走快路径不调度；MAX 路径调度一次后返回；返回后队列头 count=1、0 节点 |
| V11 | 局部序列 | OBSERVED_AS_PREDICTED | HOST_ORIGINAL_SLICE | 先入队后通知的状态序列成立；复用同一 completion 再 complete 即触发 anchor 误用；真实上下文切换层 NOT_RUN |
| V12 | 是 | OBSERVED_AS_PREDICTED | HOST_ORIGINAL_SLICE | 原 asm 在 x86-64 上实测为减法：(-1,1)→-2/真，(1,2)→-1/真，(2,-1)→3/假 |
| V13 | 是 | OBSERVED_AS_PREDICTED | HOST_ORIGINAL_SLICE | trylock 成功不改锁字且可连续成功；真实持锁时返回失败（对照） |
| V14 | 部分 | OBSERVED_AS_PREDICTED | SOURCE_MATCH, MODEL_ONLY | 源码与构建引用成立；宿主别名模型一次 HPET_handler 计数 +2（对照 +1）；无 ELF，EXISTING_ELF_EVIDENCE 未执行 |

CA 裁定：CA-01、CA-03、CA-04 在范围内得到支持；CA-02 为“函数层面支持、可达性反证”；CA-05 只到源码与模型层；CA-06 只到源码层；CA-07 由原片段执行支持。09 §5.4 的遗漏路径问题：未见遗漏的活动入队；swait 的两处摘链都不维护 count；到期分发链在所列名称下无活动命中，仍未闭合。

## 反证与主线须知

- **A46 边界**：按 P2 规则算一处部分失败；其语义（根 CMake 递归收集源码）仍成立。
- **V08 可达性**：07 的有条件反例在函数层面成立，但现有 idle 重新入队机制让它依赖“idle 以非 RUNNING 被切出”。rest_init 中 kernel_thread 等路径本轮未读，所以没有证明 idle 绝不睡眠。
- **V03 后果更具体**：误当作 waiter 的“任务指针”就是队列锁字，complete() 持锁时为非零值；真实 try_to_wake_up 会对它写入。本轮没有执行这次写入，也不声称观察到内核崩溃。
- **影响面**：所读快照中 `msleep` 没有活动调用者；`swake_up_all_locked` 与 `finish_swait` 在 swait.c 外没有调用者。CA-04 的实际触发路径是同一个 completion 被多次 complete 或复用。
- **额外观察（不在 07 中）**：pick_next_task_myos 按 vruntime 回插时只和最初的首项比较，实测顺序为 [4,5,2]，而按 vruntime 排序应为 [4,2,5]。
- **开发期失败**：检查脚本与夹具自身的五处问题在正式运行前已修正，均不涉及被测原函数，详见 evidence.md §6。

阻断项：无。

## 远端回读范围

`results.yaml` 在提交 `7e2fa84a9823` 推送后，由 `readback.py` 从 raw.githubusercontent.com 与 api.github.com 两个通道读取。两者都满足以下条件：curl 退出 0、未超时、HTTP 200、远端分支头等于该提交、44237 字节与提交 blob 逐字节一致、`packet_id` 解析正确。本文件与 evidence.md 在第二次提交中加入，不回填 results.yaml；第二次推送后的核对写在 PR 正文中。

## 消费方式

主线可从 PR #17 读取 `core/` 下的三个文件与 `fixtures/`：先看 evidence.md §3.1 的 47 条逐项表与 §4.2 的原始事件行，再按 results.yaml 的 `cases` 与 `ca_rulings` 决定哪些结论可以消费。PR 保持 Draft、暂不合并；执行分支保留到主线审查完成并明确解除。
