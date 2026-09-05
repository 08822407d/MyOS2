# MYOS2-DR-005R · x86 查表资料包勘误与出处补全（005 有位域错值与假 [VERIFIED]；任务书 v2）

```yaml
task_id: MYOS2-DR-005R
supersedes: agent-workspace/tasks/MYOS2-DR-005R-x86-reference-pack-errata.md（v1 任务书，保留不改）；产出部分 supersedes MYOS2-DR-005（只对有变更的卡出 -v2；results/MYOS2-DR-005/ 保留不改）
mode: 普通对话 Pro（GitHub 连接器＋开浏览核对 Intel SDM / AMD APM 官方页面）
priority: P2（Owner 目的④；查表包最忌错值，但不阻塞其他任务）
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-005R/
protocol: agent-workspace/tasks/00-gpt-task-protocol-v2.md（全部十三条适用，硬性；P5 出处纪律对本任务尤其关键）
prerequisites: 先读 conventions.md、上述协议、WAVE-1-REVIEW.md §3.3；再按 §2 输入清单读 005 的指定文件
drafted_by: MYOS2-LEAD-001（v2：2026-09-05）
status: draft（发射由 Owner）
```

## 1. 为什么勘误

005 的框架（用途普查→速查卡→邻近可用项→陷阱）正是 Owner 要的"贴着实现给的查表包"，非平凡推导抽 28 条 20 条确认可用；但查表包里有错值就比没有更糟。评审证实六类错误：ICR destination shorthand 写 `17:16`（实为 19:18；仓库 `apicdef.h` 里 `APIC_DEST_SELF 0x40000` 即 bit 18）；CR4 的 19–28 写 Reserved（bit 19 是 Key Locker，且 `cpuid-atlas.md` 自己列了 leaf 0x19）；页表项物理地址写自 bit 13（实为 bit 12）；能力字数量写 11（`get_cpu_cap()` 实填 15 槽）；inputs 列了不存在的 `kernel/apic/lapic.c` 与 `insns/special_insns.h`（真名 `special_insns_arch.h`）；三处把 kvmclock 标为 "[VERIFIED] 未见调用者/stub"（实际 `kvm.c` 中调用 `kvmclock_init()`）。

## 2. 输入清单（文件级；协议 P11；按优先顺序，读不完弃尾并在覆盖表声明）

1. `results/MYOS2-DR-005/cards/apic-ioapic.md`、`cards/msr-control-registers.md`、`cards/paging.md`、`cards/cpuid.md`、`cards/timers.md`、`cpuid-atlas.md`（六类已知错误所在，各 7~13 KB）；
2. `results/MYOS2-DR-005/00-usage-census.md`（10,327 B；inputs 路径勘误对象）与 `MANIFEST.md`（只取文件表）；
3. `results/MYOS2-DR-005/open_questions.md`（8,740 B；34 条 OQ）；
4. 其余六张卡（`atomic-locks`、`fpu-xstate`、`interrupts-exceptions`、`io-ports`、`segmentation-gdt-idt-tss`、`syscall-context`）；
5. `WAVE-1-REVIEW.md` §3.3；`mykernel/scripts/options_flags.cmake` 全文（协议 P4）。

源码起点：`mykernel/arch/x86_64/include/asm/apicdef.h`；`mykernel/arch/x86_64/kernel/cpu/common.c::get_cpu_cap`；`mykernel/arch/x86_64/kernel/kvm.c`（含 `kvmclock_init();` 调用的函数，自行核实函数名）与 `kvmclock.c::kvmclock_init`；`mykernel/arch/x86_64/mm/pgtable/`（`PTE_PFN_MASK` / `PHYSICAL_PAGE_MASK` 的定义处）；`mykernel/arch/x86_64/insns/special_insns_arch.h`；目录 `https://github.com/08822407d/MyOS2/tree/time/mykernel/arch/x86_64/kernel/apic`（证明无 `lapic.c`）。

## 3. 强制 [VERIFIED] 锚点（不得降级、不得省略）

`mykernel/arch/x86_64/include/asm/apicdef.h` 中含 `APIC_DEST_SELF` 的行（所读快照中为注释行，仍是位域值证据，引文须含 `//`）；`mykernel/arch/x86_64/kernel/cpu/common.c::get_cpu_cap`（函数定义行＋填充能力槽的语句）；`mykernel/arch/x86_64/kernel/kvm.c` 中含 `kvmclock_init();` 的行（`path::function` 写实际函数名）；`mykernel/arch/x86_64/kernel/kvmclock.c::kvmclock_init`；`PTE_PFN_MASK` 或 `PHYSICAL_PAGE_MASK` 的定义行（写明文件）；`mykernel/arch/x86_64/insns/special_insns_arch.h` 任一定义行；`mykernel/scripts/options_flags.cmake` 含 `-DCONFIG_KVM_GUEST` 的行。

## 4. 目标

1. **errata.md（主件）**：逐条修正六类已知错误，每条给旧值 / 新值 / 出处（SDM 卷、章、表/图编号＋URL＋修订号，协议 P5）或仓库引文（P2）；对全部 12 张卡＋atlas 的每张位域/寄存器表做一次对照官方手册的自查，结果逐表登记"已核对无误 / 已修正 / 未能核对（原因）"；errata 条目上限 60。
2. **[VERIFIED] 引文自检**：对 005 的每个 `[VERIFIED path]` 按协议 P6 重核；核不过的降级或撤回；kvmclock 三处必须撤销并改写为带引文的正确描述。
3. **cards-v2/**：只交有实质变更的卡，每张头部 `supersedes: results/MYOS2-DR-005/cards/<原卡>`；未变更的卡不重抄。
4. **open_questions-v2.md**：对 34 条 OQ 逐条写 已闭合（附证据）/ 仍开 / 撤回（如 OQ-027）。
5. **一致性检查**：cpuid-atlas 与各卡之间（Key Locker、0x15/0x16 与 timers 卡）不得再互相矛盾。

## 5. 交付物与 MANIFEST

`MANIFEST.md`（必交；`base_snapshot: time（分支名）`、`read_channel`、`startup_selfcheck_quote`、`branch_canary_quotes`、`self_check`、`supersedes` 列表、外部出处统计 URL/DOI 条数、覆盖表）＋`errata.md`＋`cards-v2/…`（仅变更卡）＋`open_questions-v2.md`。降级交付按协议 P12（顺序 MANIFEST → errata → open_questions-v2 → cards-v2 逐张）。

## 6. 边界

不修改内核源码；不改 results/MYOS2-DR-005/；不写 40 位 SHA；不新增主题卡；不把厂商/型号相关 MSR 写成通用事实；不做运行时探测；不转抄 005 的 inputs_read 与文件头字段。

## 7. 验收判据（本地闸门按协议 P9 执行；任一项不过整份退回）

1. MANIFEST 五字段齐全；`startup_selfcheck_quote` 命中；金丝雀两行 time 命中、master 不命中（P9-5、7、8）。
2. §3 强制锚点全部存在、引文 100% 命中（P9-2、13）；`[VERIFIED` 计数 = `self_check.verified_claims`。
3. 六类已知错误全部修正且各带官方出处（URL＋卷/章/表号）或仓库引文；kvmclock 三处撤销；errata 的 inputs 勘误只含 time 上实际存在的路径（P9-3）。
4. 每个 `[EXTERNAL]` 带可解析 URL/DOI，内部句柄 0（P9-6）；12 卡＋atlas 自查表齐全。
5. 正则 `[0-9a-f]{40}` 0 命中；每个 `supersedes` 路径存在（P9-1、3）。
