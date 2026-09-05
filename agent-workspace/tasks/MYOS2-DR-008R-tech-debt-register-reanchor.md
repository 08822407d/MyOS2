# MYOS2-DR-008R · 技术债台账重锚与覆盖扩展（008 复跑：行号越界、热点图截断、永久自旋覆盖不足）

```yaml
task_id: MYOS2-DR-008R
supersedes: MYOS2-DR-008（部分：证据锚点、热点图、永久自旋类目；ID 体系保留）
mode: 普通对话 Pro（GitHub 连接器；读 time 分支源码）
priority: P2（可选：其机械部分本地 L4/L5 也能做，且更便宜；GPT 版本的增量价值在"扩覆盖＋逐条引文"。Owner 若额度紧可跳过）
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-008R/
protocol: agent-workspace/tasks/00-gpt-task-protocol-v1.md（全部十条适用，硬性）
prerequisites: 先读 conventions.md、上述协议、WAVE-1-REVIEW.md §3.4、§4.3~4.8；再读 results/MYOS2-DR-008/{MANIFEST-v2.md,debt-register.yaml,01-summary-v2.md,02-correctness-suspects.md}（只读，待勘误对象）
drafted_by: MYOS2-LEAD-001（2026-09-05）
status: draft（发射由 Owner；可选）
```

## 为什么复跑

008 无编造、可靠性 medium，其 69 条 `DR008-NNN` 稳定 ID 已被 004R 任务书当作强制输入。但：
- 69 条证据中 23 条行号越过文件范围（WAVE-1-REVIEW §3.4）——"每条可回溯"的验收判据在行号层面已失效；
- 热点图静默截断（只列 52/60）；
- 永久自旋只覆盖 5 个文件，评审全树清点为 32 处/23 文件（§4.6：`futex.c` 的 `do_futex` 首句即 `while(1)`、未知 syscall、fault.c 四处、mmap.c 三处等）；
- 评审新发现的确定性缺陷（§4.3 两个 P0、§4.4 唤醒链、§4.5 定时层、§4.8 TSC）不在台账里。

## 目标

1. **errata.md（主件）**：对 69 条逐条：`quote` 是否在 time 分支逐字存在（协议 P6）→ 存在则补 `function` 字段；不存在则状态改 `unlocatable` 并说明；`line` 字段保留但降为参考。
2. **debt-register-v2.yaml**：**ID 一个不改**（下游已引用）；新增字段 `function`、`quote_reconfirmed: true|false`、`smp_status`（并发类条目）；新增条目从 `DR008-070` 起编：
   - 永久自旋类目扩到全树（目标覆盖 ≥ 评审所述 32 处/23 文件；每处一条，附引文；找不到评审所述某处时写 open_questions，不硬凑）；
   - WAVE-1-REVIEW §4.3~4.8 的六项确定性事实各一条（附引文，`source_kind: review-confirmed`）；
   - `upstream_noise: true|false`（区分 Linux/glibc 上游注释噪声与 MyOS2 活动债务，沿用 v1 做法）。
3. **01-summary-v3.md**：热点图**完整 60 行**（或实际子系统数），统计由 YAML 机械生成并与之一致；`supersedes: 01-summary-v2.md`。
4. **02-correctness-suspects-v2.md**：按 S1→S2 排序，每条附引文与 `smp_status`；把六项评审事实并入并标注来源。

## 输入（务必消费）

- `agent-workspace/WAVE-1-REVIEW.md` §3.4、§4.3~4.8（含各处文件路径与函数名）；
- `mykernel/scripts/options_flags.cmake`（协议 P4：判断"死代码/未启用"前必读）；
- `results/MYOS2-DR-008/`（待勘误对象）。

## 交付物（放入 write_zone）

`MANIFEST.md`（必交；含 `self_check`、统计表、新增条目数）＋`errata.md`＋`debt-register-v2.yaml`＋`01-summary-v3.md`＋`02-correctness-suspects-v2.md`。

## 边界

只登记不修复；不改 results/MYOS2-DR-008/；不改任何 ID；不写 40 位 SHA；历史材料条目维持"历史材料，现状未核"；不把上游噪声计入正式统计。

## 验收判据（本地闸门按协议 P9 执行）

- 69 条旧 ID 全部保留且每条有 `quote_reconfirmed` 判定；`quote_reconfirmed: true` 的引文本地 `grep -F` 100% 命中；
- 永久自旋条目 ≥ 25 处且每条带引文（与评审 32 处的差额列入 open_questions）；
- 热点图行数 = 子系统数，统计与 YAML 一致（本地机械核对）；
- 六项评审事实各有一条带引文的新条目；
- `self_check` 自洽；全文无 40 位 SHA；`base_snapshot: time（分支名）`。
