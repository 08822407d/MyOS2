# MYOS2-DR-005R · x86 查表资料包勘误与出处补全（005 有位域错值与假 [VERIFIED]）

```yaml
task_id: MYOS2-DR-005R
supersedes: MYOS2-DR-005（部分：只对有变更的卡出 -v2；原目录保留不改）
mode: 普通对话 Pro（GitHub 连接器＋开浏览核对 Intel SDM / AMD APM 官方页面）
priority: P2（Owner 目的④；查表包最忌错值，但不阻塞其他任务）
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-005R/
protocol: agent-workspace/tasks/00-gpt-task-protocol-v1.md（全部十条适用，硬性；P5 出处纪律对本任务尤其关键）
prerequisites: 先读 conventions.md、上述协议、WAVE-1-REVIEW.md §3.3；再读 results/MYOS2-DR-005/ 全部（只读，待勘误对象）
drafted_by: MYOS2-LEAD-001（2026-09-03）
status: draft（发射由 Owner）
```

## 为什么勘误

005 的框架（用途普查→速查卡→邻近可用项→陷阱）正是 Owner 要的"贴着实现给的查表包"，非平凡推导抽 28 条 20 条确认可用；但**查表包里有错值就比没有更糟**。评审证实：
- `cards/apic-ioapic.md`：ICR destination shorthand 写成 `17:16`，实为 `19:18`（仓库自己的 `apicdef.h` 里 `APIC_DEST_SELF 0x40000` 就是 bit 18）；
- `cards/msr-control-registers.md`：CR4 的 19–28 写 Reserved，bit 19 实为 Key Locker（KL），而 `cpuid-atlas.md` 自己又列了 leaf 0x19 Key Locker——自相矛盾；
- 页表项物理地址字段写自 bit 13，实为 bit 12；
- CPU 能力字数量写 11，仓库 `get_cpu_cap()` 实填 15 槽；
- `00-usage-census.md` 的 inputs 列了不存在的 `kernel/apic/lapic.c` 与 `insns/special_insns.h`（真名 `special_insns_arch.h`）；
- `cpuid-atlas.md`、`cards/timers.md`、`open_questions.md`(OQ-027) 三处把 kvmclock 标为 "[VERIFIED] 未见调用者/stub"，实际 `kvm.c` 中 `kvmclock_init()` 有活动调用链。

## 目标

1. **errata.md（主件）**：
   - 逐条修正上述六类已知错误，每条给：旧值 / 新值 / 出处（SDM 卷、章、表/图编号＋URL＋修订号，协议 P5）或仓库引文（协议 P2）；
   - 对 **全部 12 张卡＋cpuid-atlas** 的每一张位域/寄存器表做一次对照官方手册的自查，结果逐表登记"已核对无误 / 已修正 / 未能核对（写明原因）"。
2. **[VERIFIED] 引文自检**：对 005 全部 `[VERIFIED path]` 标签按协议 P6 重新核对；核不过的降级为 [INFERRED] 或撤回，kvmclock 三处必须撤销并改写为带引文的正确描述。
3. **cards-v2/**：只交有实质变更的卡，每张头部 `supersedes: results/MYOS2-DR-005/cards/<原卡>`；未变更的卡不重抄。
4. **open_questions-v2.md**：对 005 的 34 条 OQ 逐条写 已闭合（附证据）/ 仍开 / 撤回（如 OQ-027）。
5. **一致性检查**：cpuid-atlas 与各卡之间（如 Key Locker、0x15/0x16 与 timers 卡）不得再有互相矛盾。

## 输入（务必消费）

- `agent-workspace/WAVE-1-REVIEW.md` §3.3；
- time 分支：`mykernel/arch/x86_64/include/asm/apicdef.h`、`mykernel/arch/x86_64/kernel/cpu/common.c`（`get_cpu_cap`）、`mykernel/arch/x86_64/kernel/kvm.c`、`kvmclock.c`、页表常量头（`mykernel/arch/x86_64/mm/pgtable/`）、`mykernel/scripts/options_flags.cmake`（协议 P4）；
- 官方手册：Intel SDM（写明修订号）、AMD APM、IA-PC HPET 规范——每处引用给可解析 URL。

## 交付物（放入 write_zone）

`MANIFEST.md`（必交；含 `read_channel`、`self_check`、`supersedes`、外部出处统计 URL/DOI 条数）＋`errata.md`＋`cards-v2/…`（仅变更卡）＋`open_questions-v2.md`。

## 边界

不修改内核源码；不改 results/MYOS2-DR-005/；不写 40 位 SHA；不新增主题卡（补全在 errata 范围内）；不把厂商/型号相关 MSR 写成通用事实；不做运行时探测（那是 open_questions 里的 probe，由本地任务做）。

## 验收判据（本地闸门按协议 P9 执行）

- 六类已知错误全部修正且各带官方出处（URL＋卷/章/表号）或仓库引文；
- 12 卡＋atlas 的自查表齐全，无"未核对"却未说明原因者；
- kvmclock 三处假 [VERIFIED] 撤销；inputs_read 只含 time 上实际存在的路径（本地校验）；
- 全部 [VERIFIED] 引文闸门 100% 命中；全部 [EXTERNAL] 带可解析 URL/DOI；
- MANIFEST `self_check` 自洽；全文无 40 位 SHA；`base_snapshot: time（分支名）`。
