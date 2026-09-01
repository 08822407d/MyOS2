# MYOS2-DR-010 · 补做外部测试实践综述（006 未达标部分重做）

```yaml
task_id: MYOS2-DR-010
mode: 深度研究 Pro（纯外部调研，不需要仓库访问）
priority: P1
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-010/
supersedes_partially: MYOS2-DR-006 的 01-survey.md（其落地方案部分另案处理，不在本任务范围）
prerequisites: 先读 agent-workspace/conventions.md 与 agent-workspace/WAVE-1-REVIEW.md §3.5
```

## 为什么有这个任务

MYOS2-DR-006 的落地件（QEMU 冒烟 harness）质量不错，但其**外部综述部分未达任务书验收判据**：任务书要求"至少覆盖 xv6、Linux（kselftest/KUnit/kernel-CI/syzkaller）、SerenityOS、Redox、osdev 社区 hobby 内核各一实例且有出处"，实际全文对 xv6 / SerenityOS / Redox / syzkaller / KernelCI / osdev / hobby 的命中数**均为 0**，只覆盖了 Linux 的 KUnit/kselftest/KTAP ＋ QEMU ＋ GitHub Actions。任务书点名的 `isa-debug-exit` 退出码约定同样零覆盖。

另一个必须纠正的问题：006/007/009 三份深度研究的引用全部是内部检索句柄（形如 `fileciteturnNfileN`，共 175 处），**没有一个可解析的 URL 或 DOI**，导致外部结论无法独立复核。本任务对此有强制要求。

## 研究问题

1. **教学/业余内核的测试实践**，每类至少一个具体实例，说明其方法、成本与适用条件：
   - xv6（MIT 6.1810/6.S081）：课程评分脚本 `grade-lab*.py` 的机制——如何用 QEMU 跑内核并判定每道题通过与否；
   - Pintos / OS161：其自带测试框架与判分方式；
   - SerenityOS：其 CI 与测试体系（含 Lagom 宿主机构建这一模式）；
   - Redox：其 CI 与测试方式；
   - osdev 社区 hobby 内核的常见做法（osdev wiki 的 testing 相关条目、公开 hobby 内核仓库实例）。
2. **Linux 侧**（006 已覆盖 KUnit/kselftest/KTAP，本任务只补其未覆盖者）：syzkaller 的原理与对小内核的适用性判断、KernelCI 的组织方式。
3. **QEMU 无头自动化的退出码约定**（006 完全漏掉且任务书点名）：
   - `isa-debug-exit` 设备的用法、退出码计算规则（`(code << 1) | 1`）、与 `-device isa-debug-exit,iobase=,iosize=` 的参数关系；
   - `debugcon` / 端口 0xE9 调试输出通道；
   - QEMU 快照复位、超时判死的成熟做法；
   - 上述各通道对 UEFI 引导内核的适用性差异。
4. **GitHub Actions 上跑 QEMU 的现实约束**：公共 runner 是否可用 KVM（结论要有出处）、纯 TCG 的速度量级、单 job 时长上限、免费额度计费方式、需要自行安装哪些包（qemu-system-x86、ovmf 等）。
5. **bug 转回归用例的工作流**：把每个修好的 bug 沉淀成用例的成熟做法（Linux 的 regression tracking、其他项目的实践）。

## 出处纪律（硬性，不满足即判不合格）

- 每条外部结论必须附**可解析的 URL 或 DOI**（形如 `https://...`）。内部检索句柄、"据某文档"、无链接的项目名一律不计。
- 优先一手来源：官方文档、项目仓库源码/README、课程官方页面。二手博客可用但须标注为二手。
- 拿不到出处的结论**不要写**；确实重要但查不到的，列进 `open_questions` 并说明查过哪些地方。

## 交付物（放入 write_zone）

- `MANIFEST.md`（必交；含出处统计：URL/DOI 条数）
- `01-teaching-kernel-testing.md`（研究问题 1）
- `02-linux-supplement.md`（研究问题 2）
- `03-qemu-headless-contract.md`（研究问题 3——本任务最关键的一份，须给出可直接落地的"检查点 → 断言字符串 → 退出码"三段式设计选项与取舍表）
- `04-ci-constraints.md`（研究问题 4）
- `05-bug-to-regression.md`（研究问题 5）

## 边界

- **不读仓库、不分析 MyOS2 源码**（本任务纯外部；MyOS2 侧的落地由本地会话做）；
- 不重做 006 已完成的 KUnit/kselftest/KTAP 综述；
- 不设计 MyOS2 的具体 CI 文件（那是本地任务）。

## 验收判据

研究问题 1 的五类对象各至少一个具体实例且各带可解析 URL；`isa-debug-exit` 退出码规则写明且带官方出处；GitHub Actions 的 KVM 可用性结论带出处；全文可解析 URL/DOI 数 ≥ 25 且内部句柄数 = 0。
