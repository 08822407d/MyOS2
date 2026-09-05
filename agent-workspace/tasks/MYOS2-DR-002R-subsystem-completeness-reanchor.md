# MYOS2-DR-002R · 子系统完成度矩阵重锚与双轴化（002 复跑：证据路径失实、成熟度轴混淆正确性、未分 UP/SMP）

```yaml
task_id: MYOS2-DR-002R
supersedes: MYOS2-DR-002（原目录 results/MYOS2-DR-002/ 保留不改；本任务产出 -v2 文件并写 supersedes）
mode: 普通对话 Pro（GitHub 连接器；读 time 分支源码）
priority: P0（Owner 目的②的地基；与 003R 并行，互不依赖；阶段 3 交叉综合的输入）
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-002R/
protocol: agent-workspace/tasks/00-gpt-task-protocol-v2.md（全部十三条适用，硬性；与本任务书冲突时以协议为准）
prerequisites: 先读 conventions.md（尤其 §3 词汇表）、上述协议、WAVE-1-REVIEW.md §3.4、§4、§5；再读 results/MYOS2-DR-002/{MANIFEST.md,completeness.yaml,completeness-matrix.md}（只读，待勘误对象）与 results/MYOS2-DR-008/debt-register.yaml（只取 id）
drafted_by: MYOS2-LEAD-001（2026-09-05）
status: draft（发射由 Owner）
```

## 为什么复跑

002 没有编造，覆盖也完整（53/53），是第一波质量较好的一份；但它的证据在新的验收闸门下过不去，且有两处结构性缺陷：
- 5 条证据路径在 time 分支上不存在，被引用 16 次，其中 4 处打了 `[VERIFIED]`（WAVE-1-REVIEW §3.4）；
- **成熟度轴衡量的是"代码完整度"而非"正确性"**：被判 maturity-3 的 5 个子系统全是库与启动期登记（mm.early、lock.atomic、time.ktime、lib.digit、lib.string），而 lock.atomic 恰恰被查出确定语义 bug（`arch_atomic_add_test_negative` 汇编为 subl；WAVE-1-REVIEW §4.3、§5）；
- 所有"基本路径可跑"判断只对单 BSP 成立（SMP 未上线，AP trampoline 原地 `jmp .`），但矩阵未区分 UP/SMP（WAVE-1-REVIEW §4.7）；
- 未读配置面 `mykernel/scripts/options_flags.cmake`，"存在/生效"判断缺依据。

## 目标

1. **errata.md（主件）**：对 `completeness.yaml` 的每条 `evidence` 路径逐条核对存在性；对每个 `implemented`/`partial` 节点的证据按协议 P2 补 `path::function`＋逐字引文；核不过的按协议 P6 降级并登记。列出 5 条不存在路径的修正。
2. **completeness-v2.yaml**：节点集合沿用 v1（53 个子系统、全部能力节点，ID 不改），每条新增/改写字段：
   ```yaml
   - node: mm.page_alloc.buddy_system
     completeness: 0-4            # 沿用 v1 的 0~4 语义，明确只表示"代码完整度"
     evidence: [mykernel/mm/page_alloc/xxx.c::function_name]   # 协议 P2 格式
     evidence_quote: "逐字引文 1~5 行"
     correctness_flag: none | suspect | known_bug     # 只允许引用 008 的 DR008-NNN 或 WAVE-1-REVIEW §4 的编号事实，附引文；不做新的 bug 猜测
     correctness_ref: [DR008-012, WAVE-1-REVIEW §4.3]
     smp_status: up_only | smp_claimed_unverified | not_applicable
     config_dependency: [CONFIG_SLUB]                  # 来自 options_flags.cmake，无则空
     notes: 一句话
   ```
   子系统级汇总表同样双轴：`completeness`（0~4）与 `correctness_flag` 分列。
3. **completeness-matrix-v2.md**：只写有变更的子系统章节＋新的双轴汇总表；未变更章节引用 v1 不重抄。
4. **词汇表缺口**：`mykernel/time/misc/`（313 行实码）在 v1 词汇表无 ID；本任务用临时 ID `time.misc` 登记并在 MANIFEST 标 `vocabulary_gap: [time.misc]`，不改公约。

## 输入（务必消费）

- `agent-workspace/WAVE-1-REVIEW.md` §3.4（002 证据缺陷）、§4（十一条独立发现，其中 §4.1 配置面、§4.3~4.8 正确性事实、§4.7 SMP）、§5（成熟度≠正确性的元结论）；
- `mykernel/scripts/options_flags.cmake`（协议 P4）；
- `results/MYOS2-DR-008/debt-register.yaml`（只取 `id` 与 `path` 做 correctness_ref，不复述其内容）；
- `results/MYOS2-DR-002/`（待勘误对象）。

## 输入范围、强制锚点与 MANIFEST（v2 协议增补，据对抗评审）

**输入清单（文件/节级；协议 P11）**：`results/MYOS2-DR-002/completeness.yaml`（66,969 B，是勘误对象，按子系统分块读：mm → sched → lock_IPC → kactive → time → fs → arch.x86_64 → block/drivers → 其余；读不完按此顺序弃尾并在覆盖表声明）；`results/MYOS2-DR-002/MANIFEST.md`（只取覆盖统计表）；`results/MYOS2-DR-002/completeness-matrix.md` 的子系统级汇总表（不逐章读）；`results/MYOS2-DR-008/debt-register.yaml` 只取 `id`/`path`/`subsystem` 三列；`WAVE-1-REVIEW.md` §4；`mykernel/scripts/options_flags.cmake` 全文。

**强制 [VERIFIED] 锚点（不得降级、不得省略）**：`mykernel/scripts/options_flags.cmake` 中含 `-DCONFIG_SLUB` 的那一行；`mykernel/arch/x86_64/lock_IPC/atomic/atomic_arch.h::arch_atomic_add_test_negative`（生效汇编为 `subl` 的那一行）；`mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h::arch_spin_trylock`；`mykernel/sched/scheduler/scheduler_core.c::try_to_wake_up`（含 `success = 0` 的那一行）；`mykernel/time/timer/timer.c::msleep`；`mykernel/lock_IPC/futex/futex.c::do_futex`（首句 `while (1);`）；`mykernel/arch/x86_64/kernel/myos_APboot.S` 中 `jmp .` 所在行；`mykernel/time/misc/time_misc.c`（任一函数定义行，证明 time.misc 有实码）。这些锚点对应 `correctness_flag: known_bug` 与 `smp_status: up_only` 的判定依据。

`MANIFEST.md` 必备字段（协议 P3/P6/P13）：`base_snapshot: time（分支名）`、`read_channel`、`startup_selfcheck_quote`、`branch_canary_quotes`（读源码的任务）、`self_check`（`verified_claims` 须等于全部交付文件的 `[VERIFIED` 标签数）、`produced_by`（界面显示的模型名原样）、覆盖表。

降级交付按协议 P12：每条回复只含一个文件、独立围栏、围栏前一行写目标路径；读不完按本任务书的优先顺序弃尾并在覆盖表声明。

## 交付物（放入 write_zone）

`MANIFEST.md`（必交；含 `read_channel`、`self_check`、`supersedes`、`vocabulary_gap`、双轴统计表）＋`errata.md`＋`completeness-v2.yaml`＋`completeness-matrix-v2.md`。

## 边界

不做修改建议；不做新的 bug 挖掘（correctness_flag 只引用已有编号事实）；不改 results/MYOS2-DR-002/；不写 40 位 SHA；不读 bugs_record.md/todo.txt 当现状；不评价代码风格。

## 验收判据（本地闸门按协议 P9 执行）

- 53 个子系统 ID 全部保留；每个 `implemented`/`partial` 节点至少一条 `path::function` 证据且引文本地 `grep -F` 100% 命中；
- v1 的 5 条不存在路径全部在 errata 中修正或撤回；
- 每条 `correctness_flag != none` 都有 `correctness_ref` 指向 DR008-NNN 或 WAVE-1-REVIEW 编号事实；
- 每条并发/调度/锁相关节点有 `smp_status`；
- MANIFEST 双轴统计与 YAML 一致（本地机械核对）；`self_check` 自洽；全文无 40 位 SHA；`base_snapshot: time（分支名）`。
- MANIFEST 含 `startup_selfcheck_quote`、`branch_canary_quotes`（time 命中、master 不命中）、`read_channel`、`self_check`（`verified_claims` = 标签计数）；强制锚点八组齐全（协议 P9-5、7、8、13）；正则 `[0-9a-f]{40}` 0 命中。

