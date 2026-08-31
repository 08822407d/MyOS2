# MYOS2-DR-006 · 内核测试与 CI 策略（外部实践→MyOS2 落地）

```yaml
task_id: MYOS2-DR-006
mode: 深度研究 Pro（外部证据为主；连接器读仓库做落地映射）
priority: P1
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-006/
prerequisites: 先读 agent-workspace/conventions.md 与 agent-workspace/repo-map.md
```

## 背景

MyOS2 目前无任何自动化测试（[INFERRED]，以你的仓库核实为准）。Owner 常遇"运行经常触发 bug"，除了调试桩（MYOS2-DR-004 另案处理），更需要回归防线：改了 mm 不知道 sched 有没有被弄坏。这是大型系统开发的普遍关注点，Owner 未明说但属于必要基建。

## 研究问题（深度研究部分）

1. 教学/业余/生产内核各自怎么做测试：xv6（课程评分脚本）、Linux（kselftest/KUnit/kernel-CI/syzkaller 思路）、SerenityOS、Redox、osdev 社区 hobby 内核的 QEMU 冒烟测试实践——各自的方法、成本、适用条件。
2. QEMU 无头自动化的成熟做法：串口输出断言、退出码约定（isa-debug-exit）、超时判死、快照复位；GitHub Actions 上跑 QEMU 的可行配置与时长/额度约束。
3. 内核内单元测试框架的轻量做法（KUnit 式内核内跑 vs 把 lib/ 代码抽出来宿主机跑双轨）。
4. bug 转回归用例的工作流（每修一个 bug 沉淀一个用例）。

## 落地映射（读仓库部分）

给 MyOS2 一份分四级的采纳方案：
- L1 冒烟：QEMU 无头启动到某检查点即 PASS（与 004 的启动检查点桩衔接）
- L2 单元：lib/（list/idr/printf/string/digit）宿主机可测化评估与首批用例
- L3 功能：经 myshell/myinit 跑 syscall 级用例（对照 user-guide/Syscall-Table.md）
- L4 CI：GitHub Actions 工作流草案（构建＋L1＋L2），给出可用的 yml 文件放 `proposed/`

## 交付物（放入 write_zone）

`MANIFEST.md`＋`01-survey.md`（外部实践综述，逐条带出处）＋`02-adoption-plan.md`（四级方案＋成本估计＋建议起步顺序）＋`proposed/ci.yml` 等实现件。

## 边界

方案不得要求先大改内核（测试基建应可增量落地）；工具链选型倾向零依赖/低依赖；深度研究引用一律带来源链接。

## 验收判据

综述至少覆盖上述四类对象各一实例且有出处；L1 方案给出具体的"检查点→串口断言字符串→退出码"设计；ci.yml 语法有效。
