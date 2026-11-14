---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-SCHED-ORDER-02
phase: scheduler_order_witness
record_type: scheduler_order_manifest
produced_by: "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection"
execution_model_selection: unknown_or_not_attestable
execution_model_selection_source: "执行者所在平台的规则不允许在推送到仓库的产物中写模型标识（执行者自述）；不认证具体后端"
execution_surface: "claude.ai/code 托管云端会话容器；普通用户态宿主进程（gcc 13.3.0、Python 3.11.15，均为容器既有工具）；与 pilot/core/recheck 同一会话、同一执行分支"
lead_attribution:
  source: "15 号任务书与 RECHECK-02 收口回执的 YAML 头"
  model_per_owner: gpt6
  effort_per_owner: pro
  scope: originating_lead_only_not_executor
date: "2026-10-02"
base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a；agent/MYOS2-LEAD-002 6706013a079a（15 号任务书、RECHECK-02 收口回执，开工时固定）；冻结执行头 0d62c4d19711。均为短 SHA"
work_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
reviewed_execution_short12: 0d62c4d19711
results_commit_short12: 5078686e8267
read_channel: mixed
read_channel_detail: "15 号任务书与回执经 raw URL 读取并与 git 对象逐字节比较；14 号分析按主线对象 git show；源码与冻结夹具按提交对象读取；PR 状态经 GitHub MCP；results.yaml 回读经 raw.githubusercontent.com 与 api.github.com"
inputs_read:
  - "agent/MYOS2-LEAD-002 @ 6706013a079a：15-scheduler-ordering-witness-contract.md（全文）"
  - "agent/MYOS2-LEAD-002 @ 6706013a079a：reviews/CORE-CHECK-01-recheck-02-review.md（全文）；同批检查点只确认存在"
  - "agent/MYOS2-LEAD-002 @ 6706013a079a：14-scheduler-ordering-followup.md（全文）"
  - "time @ a039d9803ade：mykernel/sched/scheduler/myos_rt.c（全文）；夹具实际引用的 list、类型与宏定义（由抽取清单列出）"
  - "0d62c4d19711：core/fixtures/fx_sched.c、fx_common.h、fx_list.inc.c、locate.py；core/recheck-01/fixtures/build2.py、harness2.py、readback2.py"
status: final_for_scheduler_order_02
witness_conclusion: W01_W08_COMPLETE_CURSOR_AND_ACCOUNTING_CANDIDATES_WITNESSED_LEAD_DERIVATIONS_AGREE
kernel_correctness_verdict: NOT_ISSUED
acceptance_ceiling: PASS_PENDING_LOCAL
startup_selfcheck_quote: '2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。'
write_authority_note: "只在 scheduler-order-02/ 新增，由 Owner 本轮指令与 15 号任务书授权（allowed_write_prefix 由 guard4 机械核对）。0d62c4d19711 中全部文件不变；未写 agent/MYOS2-LEAD-002、master、time。PR #17 由 Ready 转回 Draft 后复用。"
self_check:
  verified_claims: 0
  quotes_reconfirmed: 0
  downgraded_to_inferred: 0
  note: "本批不新增源码断言标签。data_summary 与 delivered_files 由 fixtures/final_check4.py 对照 results.yaml 与暂存文件核对（evidence.md §9.2）。"
data_summary:
  scenarios_completed: [W01, W02, W03, W04, W05, W06, W07, W08]
  candidate_P: {W01: VIOLATED, W02: VIOLATED, W03: SATISFIED, W04: SATISFIED, W05: SATISFIED, W06: SATISFIED, W07: VIOLATED, W08: SATISFIED}
  P_violations_by_step:
    W01:
      '1': [VIOLATED, VIOLATED]
    W02:
      '1': [VIOLATED, HOLDS]
    W07:
      '1': [VIOLATED, HOLDS]
  all_structure_ok: true
  all_accounting_matches_oracle: true
  lead_derivations_all_agree: true
  selected_task_not_queue_minimum:
  - - W07
    - 2
    - 2
    - [4, 20]
  controls_all_met: true
  pick_next_task_myos_sha256_0: 2ff36b570d0422f3
delivered_files:
  - MANIFEST.md
  - evidence.md
  - fixtures/build_evidence4.py
  - fixtures/common4.py
  - fixtures/controls4.py
  - fixtures/evaluate_order.py
  - fixtures/final_check4.py
  - fixtures/frozen_0d62.py
  - fixtures/fx_order.c
  - fixtures/guard4.py
  - fixtures/make_results4.py
  - fixtures/run_order.py
  - observations/after_results/guard_after_push.json
  - observations/after_results/readback_results.branch.json
  - observations/after_results/readback_results.object.json
  - observations/controls.json
  - observations/frozen_0d62_manifest.json
  - observations/fx_order.expanded.c
  - observations/guard_pre_results.json
  - observations/guard_start.json
  - observations/runs.json
  - results.yaml
kernel_modified: false
kernel_patch_produced: false
repo_scripts_run: false
full_kernel_build: false
qemu_run: false
tools_installed: false
privilege_escalation: false
credentials_output: false
merge_or_branch_delete: false
open_questions:
  - "候选约束 P 由 15 号提出，不是 Owner 已选的调度政策；按更新后 vruntime 排序是否成为目标由主线与 Owner 决定。"
  - "这些状态在真实运行的 MyOS2 中是否可达（CA-02 全局可达性）、真实时钟/IRQ/SMP/上下文切换，都不在本次范围内。"
---

# CORE-SCHED-ORDER-02：两个回插候选都得到有限实测见证，主线推导逐项相符

**W01–W08 八组场景全部完成。每组在独立进程中运行两次，两次的完整输出都已保存且逐字节相同。W01 见证了游标问题，W02 与 W07 第 1 步见证了计费时点问题；W03、W04、W05、W06、W08 都满足候选约束 P。主线在 15 号中写下的推导逐项相符，没有反证。** 这是一组有限的宿主见证，说明的是一个原函数的边界行为，不是对 MyOS2 内核是否正确的结论。

## 1. 限制（先读）

- **见证完成 ≠ 内核正确。** 夹具是宿主上的普通用户态进程。`pick_next_task_myos` 与 list 原语从 time a039d9803ade 逐字复制，没有改一个字符；但 current、need_resched、jiffies 都是显式输入，不是时钟、中断或抢占。W07/W08 中“返回的任务成为 current”是序列模型，不是上下文切换。
- **结构缩减**：task_struct 与 runqueue 只保留原函数访问的成员，类型选择沿用 0d62 的 core/fixtures/fx_sched.c。单 CPU，无并发修改，time_slice=100，last_jiffies 每个场景只设一次初值 100。
- **没有沿用**旧 fx_sched.c 中 pick() 在每次调用前重置 last_jiffies 的做法；两次调用之间只由原函数更新它。
- **观察只在调用前后**：没有在原函数内部插探针；遍历快照先与 anchor 比较，再按地址识别节点，最多 16 个链接。
- **P 是候选约束**：回队的非 idle 任务按计入本轮消耗之后的 vruntime 非递减，idle 在尾部。它不是 Owner 已决定的政策，也不意味着 MyOS2 必须实现完整公平调度。
- **未执行**：全局可达性（CA-02）、真实时钟、IRQ、SMP、上下文切换；没有修改内核，也没有生成补丁。
- 执行模型未知；没有 CI 或人工逐行复核。

## 2. W01–W08 结果

队列写作“任务+vruntime”，I 为 idle。每个结果都是两次运行的共同输出（runs.json 中逐字节相同）。

| W | 输入 | 调用后队列（返回） | 记账 | P 非 idle / idle 尾 | 与主线推导 |
|---|---|---|---|---|---|
| W01 游标 | A25，jiffies 100，基准队列 | C20 D30 I0 A25（返回 B） | 无（used 0） | 违反 / 违反 | 相符：A 落在 D 与 idle 之后 |
| W02 计费 | A15，jiffies 110 | A25 C20 D30 I0（返回 B） | A +10 | 违反 / 成立 | 相符：先按 15 插在 C20 前，再变 25 |
| W03 零增量 | A15，jiffies 100 | A15 C20 D30 I0（返回 B） | 无 | 成立 / 成立 | 相符 |
| W04 相等键 | A15，jiffies 105 | A20 C20 D30 I0（返回 B） | A +5 | 成立 / 成立 | 相符：相等不算失序 |
| W05 阻塞 | A15 UNINTERRUPTIBLE，jiffies 110 | C20 D30 I0（返回 B），A 不回队 | A +10（A=25） | 成立 / 成立 | 相符 |
| W06 idle | current=I（vruntime 7），队列 B C D，jiffies 110 | C20 D30 I7（返回 B） | idle 不计费 | 成立 / 成立 | 相符 |
| W07 不重复计费 | 同 W02，再以 B 为 current 调用一次 | 第 1 步 A25 C20 D30 I0（违反）；第 2 步 B10 C20 D30 I0（返回 A） | 第 1 步 A +10，第 2 步 0；共 10 | 第 1 步违反，第 2 步成立 | 相符 |
| W08 累计 | A15，jiffies 105，先 need_resched=0 再 =1 | 第 1 步不变（返回 A，last 仍 100）；第 2 步 A20 C20 D30 I0（返回 B） | 第 2 步 A +5 | 成立 / 成立 | 相符 |

在每一步中，结构、记账、选择、成员四项都与独立 oracle 一致；输入保真全部成立。

## 3. 发现

- **游标问题（W01）**：比较对象停在首节点 C20，游标却前进到 anchor，A25 被插到 idle 之后、队尾。
- **计费时点问题（W02、W07 第 1 步）**：回插用计费前的键 15 定位（不大于 C20，循环一次也不进入），计费在插入之后，于是 A25 排在 C20 之前。只改循环内部的游标改变不了这种情形，见 evidence.md §5。
- **失序的后果（W07 第 2 步）**：下一次选择取到队首的 A25，而队列中最小的是 C20。
- **对照**：零增量（W03）与相等键（W04）满足 P；阻塞任务（W05）不回队但仍计费；idle（W06）回到队尾、不计费；第二次调用不重复计费（W07）；不切换时不丢计费，下一次切换才记入累计的 5（W08）。
- **源码推断（未执行）**：循环先比较 vruntime，再检查是否到达 anchor。若只把比较对象改成随游标移动、却不调换这两个条件的顺序，走到 anchor 时会读取 anchor 的“容器”。

## 4. 三项消费正负控

| 控制 | 结果 |
|---|---|
| C1 完整的 W03 样本 | VALID，P 成立 |
| C2 删去一次调用快照 | INCOMPLETE_EVIDENCE，不给排序结论（不是“无失败”） |
| C3 完整但与预测不同（W03 调用后队列改为 C20 A15 D30 I） | VALID，P 违反，与主线判定不同；作为有效反证，不是 COMPARATOR_INVALID |

C2/C3 是 HARNESS_META_TEST 变造，只作用于内存副本，没有写回 runs.json。

## 5. 文件清单

路径相对于 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-order-02/`，共 22 个文本文件，没有二进制。

| 文件 | 内容 |
|---|---|
| `MANIFEST.md`、`evidence.md` | 本清单；由 build_evidence4.py 生成的证据（含八组第 1 次运行的原始 stdout） |
| `results.yaml` | 由 make_results4.py 从 observations/ 计算：W01–W08 逐条完成情况、保真边界、结构/记账/排序检查、P、与主线推导比较、三项正负控 |
| `fixtures/fx_order.c` | 新夹具模板（//@@ORIG 行在构建时从 time 逐字展开） |
| `fixtures/run_order.py` | 一次构建；每个场景独立进程运行两次，完整保存两次输出 |
| `fixtures/evaluate_order.py`、`controls4.py`、`make_results4.py` | 记录完整性检查与独立 oracle；三项正负控；results.yaml 生成 |
| `fixtures/common4.py`、`frozen_0d62.py`、`guard4.py` | 固定对象与辅助；0d62 冻结文件逐字节解出并核对；本任务自己的绑定与保护门 |
| `fixtures/final_check4.py`、`build_evidence4.py` | 发布前检查；evidence 生成 |
| `observations/runs.json` | 构建记录（抽取清单、编译）与八组各两次的完整 stdout、stderr、退出码、超时、终态 |
| `observations/fx_order.expanded.c` | 实际编译的展开源码（含原函数逐字副本） |
| `observations/controls.json` | 三项正负控 |
| `observations/guard_start.json`、`guard_pre_results.json`、`after_results/`（3 个） | 编译前、结果提交前、推送后的保护门；results.yaml 的 object/branch 回读 |
| `observations/frozen_0d62_manifest.json` | 复用的 0d62 冻结文件清单（字节数与 SHA-256） |

复用的 0d62 文件：core/fixtures/fx_common.h、fx_list.inc.c、locate.py、fx_sched.c（仅作类型参照，不编译）；core/recheck-01/fixtures/build2.py（展开、编译参数与上限）、harness2.py（有界运行）、readback2.py（回读）。

## 6. 重建命令

```bash
git clone https://github.com/08822407d/MyOS2 && cd MyOS2 && git checkout claude/dazzling-cori-q0dnyt
export MYOS2_REPO=$PWD PYTHONDONTWRITEBYTECODE=1
cd agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-order-02/fixtures
P=$(mktemp -d)                                    # 新建空目录；程序不复用、不清理已有目录
python3 guard4.py $P/guard.json                   # 退出码 0 开门，3 关门
python3 frozen_0d62.py $P/frozen                  # 0d62 冻结文件逐字节解出
mkdir $P/obs && python3 run_order.py $P/frozen $P/work $P/obs     # 一次构建，W01-W08 各运行两次
python3 controls4.py $P/obs/runs.json $P/obs/controls.json
cp ../observations/guard_start.json $P/frozen/frozen_manifest.json $P/obs/ && mv $P/obs/frozen_manifest.json $P/obs/frozen_0d62_manifest.json
python3 make_results4.py $P/obs $P/results.yaml   # 由新运行生成；与已交 results.yaml 比较时差异只应在路径类字段
python3 make_results4.py ../observations $P/delivered.yaml && cmp $P/delivered.yaml ../results.yaml   # 由已交观测逐字节重建
```

- 需要 git、Python 3 + PyYAML、gcc；单次编译上限 60 s，单次场景运行上限 5 s，遍历上限 16 个链接。
- guard4.py 要求在执行分支上、0d62 是 HEAD 的祖先，并且相对 0d62 只在 scheduler-order-02/ 下新增。

## 7. 回读与交付

- 结果批 `5078686e8267`：results.yaml 36524 字节。object 与 branch 两种模式、raw 与 api 两个通道都满足条件：curl 退出 0、未超时、HTTP 200、与提交 blob 逐字节相同、packet_id 正确。推送后保护门打开，相对 0d62 的 16 项变化全部是 scheduler-order-02/ 下的新增。
- 文档批：本清单、evidence.md、build_evidence4.py 与 after_results/ 三份记录。提交前运行 `final_check4.py docs 5078686e8267`，输出嵌入 evidence.md §9.2。
- PR #17 开工时为 Ready，已转回 Draft 并复用；没有新开 PR。不合并，执行分支保留到主线审查完成并明确解除。
