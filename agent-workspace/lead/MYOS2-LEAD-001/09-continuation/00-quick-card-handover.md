# 接管快卡（Quick Card）· MyOS2 分析计划主线（供任何后继主导会话：ChatGPT Pro / 本地 Claude / 未来 Meta-Agent）

```yaml
task_id: MYOS2-LEAD-001
track_id: MYOS2-LEAD-001
record_type: handover_quick_card
evidence_class: B
produced_by: claude-fable-5.1@claude-code-vscode（主导会话 MYOS2-LEAD-001）
date: 2026-09-07
base_snapshot: 工作区 origin/master @ 22098048（PR #7 合并后）；内核 time @ a039d9803ade2a1613d620bda375e028530d5242
status: living（每次门后由现任主导会话更新；改用新文件 supersedes，不改旧文）
```

## 目标
Owner 借 LLM 分析自制内核 MyOS2，为学习内核设计定方向。五个目的（不增不减）：①外围脚本整理；②完成度＋重要度图＋依赖图（含裁剪基础）；③基本调试桩；④查表工作代劳；⑤未来以自制内核为教学蓝本。存储底线：后续更完善的 agent 能无损接手（工作令 §1.3 六条）。

## 状态（2026-09-07）
- 第一波九任务已回收、评审、归档（Alaya 批次 1）；四份含编造，全部结论按 WAVE-1-REVIEW 降权。
- 第二波：10 个 GPT 深度研究任务的任务书、协议 v2、发射块已在 master（WAVE-2-LAUNCH.md §7）；发射由 Owner 手动，**尚未回收任何一份**。
- 本地任务 L1~L11 / A~E：全部未启动；本地 A（引文闸门脚本）是回收验收的前置，待 Owner 授权取号（下一个任务号 012）。
- 阶段：0 完成；1（事实基线冻结）未开始；2（第二波发射与回收）进行中。

## 已定的决定（Owner）
"开工"（2026-09-03，接受工作令 §7/§12 全部默认）；PR #6、#7 已合并；第一波 GPT 任务按需重做（第 3 条）；全部重做由深度研究执行、暂不评审任务书、GPT 工作标 gpt6（第 4 条）；探索 GPT Pro 接手主线（第 5 条）。

## 待定的决定（Owner，不答按默认）
WAVE-2-PLAN §3 的 12 条（自制 libc、BOOTX64 来源、out/、物理盘同步范围、console 通道、词汇表、分级口径、主攻 1/4/6 亲手、ROOTBLK、aarch64、SMP 优先级、路线 A）；OI-1 004R 执行者（默认 GPT 执行本地核验）；OI-2 PR #1~#5 去留；OI-5 脱敏保留否；OI-9 Alaya 写入授权；本地 A 取号。人话版见 `01-orientation-report.md` §3~§4。

## 禁令（不因补充说明放松）
agent-workspace/ 之外一律不改；主检出 time 不动（本地会话）；不写 40 位 SHA；results/<既有任务号>/ 原件不改（修订用 -v2＋supersedes）；一任务一分支一 PR、Owner 合并、禁直推 master；不关 PR #1~#5；危险脚本绝不执行（`scripts/phys_nvme_install.sh`、`scripts/part_vdisk.sh`、`make_install.sh`）；Alaya 写入需 Owner 单独同意；自动记忆只存 Owner 偏好。

## 未知（诚实登记）
第二波回收件质量未知；Mnemosyne 主导会话（UUID f0e60aba…）是否还会回来做 §10 的移交记录未知；决策 5（console 通道）未答则冒烟测试挂起；本地 master ref 仍是过期的 63f0785c。

## Oracle（去哪里能核实什么）
- 现行规则：`lead/MYOS2-LEAD-001/00-owner-work-order-verbatim.md`（Owner 原话＋§13 补充）＞ `03-provisional-execution-sources.md`（S1~S13、W1~W5）。
- 第一波谁编造了什么：`WAVE-1-REVIEW.md` §3；机械取证 `05-wave1-fabrication-ledger-and-redo-plan.md` §2。
- 第二波发什么、怎么收：`WAVE-2-LAUNCH.md`（§1 总表、§4 回收流程、§7 发射块）；协议 `tasks/00-gpt-task-protocol-v2.md`（P9 闸门清单）。
- 内核事实的唯一基线：分支 `time`（`git diff time...master -- mykernel/ myloader/ myinitramfs/` 为空，master 只多工作区提交）；配置面真相 `mykernel/scripts/options_flags.cmake`。
- 分支金丝雀（判断读的是 time 还是 master）：`options_flags.cmake` 的 `-mcmodel=kernel -fno-pie -fno-pic` 行；`debug/panic.c::panic` 的 `this_cpu = smp_processor_id();` 行。
- 归档收据：`results/ARCHIVE-RECEIPTS.md`（批次 1 完成；批次 2 待）。

## 来源指针（按需读）
`01-orientation-report.md`（现状盘点、分工、open items、12 条决策人话版）；`02-analysis-facets-register.md`（25 个分析维度）；`06-wave2-redo-scope-decision.md`（重做范围与评审结果）；`07-gpt-lead-takeover-feasibility.md`；`checkpoints/`（最新一份即当前状态）；`predecessor/`（Mnemosyne 两份记录镜像）。

## 隐藏依赖
- 回收验收依赖本地闸门脚本（尚不存在）；没有它，PASS 只能是 PASS_PENDING_LOCAL。
- 009R 的教学单元过滤依赖 002/002R 的 completeness 值；004R 的检查点依赖 003/003R 的函数序列。
- 所有并发结论只对单 BSP 成立（SMP 未上线）。
- 三套分级口径未统一前，002/007/008 的 YAML 不能直接叉乘（决策 7）。
- 后继会话的机器名会变；跨会话消息以 UUID 为准；ChatGPT 表面无可靠模型自识别，标 `model_per_owner` 以 Owner 告知为准。
