---
task_id: MYOS2-DR-005
produced_by: "ChatGPT Pro / GPT-5.6 Pro"
date: 2026-09-01
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"
inputs_read:
  - "mykernel/arch/x86_64/kernel/cpu/common.c"
  - "mykernel/arch/x86_64/kernel/tsc.c"
  - "mykernel/arch/x86_64/kernel/kvmclock.c"
  - "mykernel/arch/x86_64/include/asm/cpufeatures.h"
  - "mykernel/arch/x86_64/processor/processor_types_arch.h"
  - "mykernel/arch/x86_64/processor/processor_const_arch.h"
status: final
open_questions:
  - "CPUID.07H.1 及 0x1D–0x1F 的新增位/子叶随 SDM 版本演进；扩展实现时应锁定目标 SDM revision 并生成表。"
  - "AMD 0x80000020 以上的新叶未逐位展开；所读 MyOS2 快照也未访问。"
  - "hypervisor vendor leaves 0x40000000+ 只在 KVM stub 邻域出现，未形成全局能力表。"
---

# CPUID 全景表

## 1. 标记

| 标记 | 含义 |
|---|---|
| `direct` | 当前源码直接执行该叶并使用返回值 |
| `stored` | 当前源码保存原始返回字，但未见有效策略消费者 |
| `transient` | 在局部校准/探测函数中读取，不进入 `boot_cpu_data` 能力槽 |
| `stub` | 仅在未接通的功能壳中使用 |
| `unused` | 所读快照中未见访问 |
| `reserved` | Intel 当前表中保留/未实现；不得探测为通用功能 |

所有叶都必须先受 `CPUID.0:EAX` 或 `CPUID.80000000:EAX` 最大叶门控；子叶还要遵守各自终止规则。[EXTERNAL Intel SDM 253665-092US, Vol. 1, Ch. 21]

## 2. 基本叶 `0x00000000–0x0000001F`

| 叶 | 主题/关键返回 | MyOS2 | 源码/备注 |
|---:|---|---|---|
| `0x00` | 最大基本叶；EBX/EDX/ECX vendor ID | `direct` | `cpu_detect()` 保存 max/vendor [VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c] |
| `0x01` | EAX version；EBX brand/CLFLUSH/logical IDs；ECX/EDX feature bits | `direct` | family/model/stepping、CLFLUSH size、两个能力字 |
| `0x02` | 旧式 cache/TLB descriptor bytes | `unused` | 建议不用作现代主路径，优先 leaf 4/18 |
| `0x03` | processor serial number（若支持） | `unused` | 隐私/历史功能 |
| `0x04` | deterministic cache parameters，按 subleaf 枚举到 type=0 | `unused` | 当前只用 AMD 扩展 cache 摘要 |
| `0x05` | MONITOR/MWAIT line size、extensions、C-state sub-states | `unused` | 常量 `CPUID_LEAF_MWAIT` 存在但未访问 |
| `0x06` | thermal/power management | `stored` | 仅 EAX 存入 `CPUID_6_EAX` |
| `0x07,0` | structured extended features；EAX=max subleaf | `direct/stored` | EBX/ECX/EDX 全存；部分位被名称/注释路径引用 |
| `0x07,1` | structured feature subleaf 1 | `stored` | 只存 EAX；未消费 |
| `0x07,>=2` | current/future structured subleaves | `unused` | 必须看 subleaf0 EAX 上限 |
| `0x08` | Intel 当前保留 | `reserved` | 不应当作跨厂商能力 |
| `0x09` | Direct Cache Access information | `unused` | 常量存在 |
| `0x0A` | architectural performance monitoring | `unused` | 未来 perf/PMU 入口 |
| `0x0B` | extended topology v1（SMT/core shifts） | `unused` | 更推荐 0x1F；当前 SMP topology 来自 boot tag |
| `0x0C` | Intel 当前保留 | `reserved` | — |
| `0x0D,0` | XCR0 支持位、XSAVE area sizes | `unused` | FPU 完整实现所必需 |
| `0x0D,1` | XSAVEOPT/XSAVEC/XSAVES/XFD 等 | `stored` | 仅 EAX 存 capability |
| `0x0D,n>=2` | 每个 xstate component size/offset/flags | `unused` | 当前无 xstate buffer 计算 |
| `0x0E` | Intel 当前保留 | `reserved` | — |
| `0x0F` | RDT monitoring / PQM | `unused` | 需 RMID/资源监测框架 |
| `0x10` | RDT allocation / PQE | `unused` | CAT/MBA 等 |
| `0x11` | Intel 当前保留 | `reserved` | — |
| `0x12` | SGX capabilities/EPC enumeration | `unused` | CPUID.7 可保存 SGX 位，但无启用 |
| `0x13` | Intel 当前保留 | `reserved` | — |
| `0x14` | Intel Processor Trace | `unused` | 无 PT buffer/ToPA |
| `0x15` | TSC/crystal ratio | `transient` | `tsc.c` 读取；字段计算存在量纲问题 |
| `0x16` | base/max/bus frequency MHz | `transient` | `tsc.c` fallback/标称频率路径 |
| `0x17` | System-on-Chip vendor attributes/brand | `unused` | — |
| `0x18` | deterministic address translation parameters | `unused` | TLB 枚举 |
| `0x19` | Key Locker | `unused` | 还需 CR4/MSR/XSAVE 依赖 |
| `0x1A` | native model ID / hybrid core type | `unused` | 混合核心调度未来可用 |
| `0x1B` | PCONFIG information | `unused` | 对应 CPUID.7 EDX.PCONFIG |
| `0x1C` | architectural LBR capabilities | `unused` | 对应 CPUID.7 EDX.ARCH_LBR |
| `0x1D` | AMX tile information | `unused` | tile palette/尺寸；XCR0 17/18 前置 |
| `0x1E` | TMUL information | `unused` | AMX dot-product geometry |
| `0x1F` | V2 extended topology | `unused` | 每层 type/shift/x2APIC ID；现代 topology 首选 |

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c] [VERIFIED mykernel/arch/x86_64/kernel/tsc.c] [EXTERNAL Intel SDM 253665-092US, Vol. 1, CPUID tables current to revision 092]

## 3. 基本叶的关键字段补表

### 3.1 `0x00`

```text
EAX = maximum basic leaf
EBX, EDX, ECX = 12-byte vendor identification string
```

MyOS2 按 EBX、EDX、ECX 顺序写 `x86_vendor_id`，这是正确的字符串拼接顺序。[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]

### 3.2 `0x01:EAX` version

| 位 | 字段 |
|---:|---|
| 3:0 | stepping |
| 7:4 | base model |
| 11:8 | base family |
| 13:12 | processor type |
| 19:16 | extended model |
| 27:20 | extended family |

显示 family/model 需按 family 6/15 的扩展规则合成；MyOS2 helper `x86_family/x86_model/x86_stepping` 承担该工作。[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c] [EXTERNAL Intel SDM 253665-092US, Vol. 1, CPUID.01H]

### 3.3 `0x04` deterministic cache 终止

对 subleaf `i=0..` 读取，若 EAX[4:0] cache type=0 则结束。其他字段给 level、shared logical count、core count、line size、partitions、ways、sets；cache size 为：

```text
(ways + 1) * (partitions + 1) * (line_size + 1) * (sets + 1)
```

[EXTERNAL Intel SDM 253665-092US, Vol. 1, CPUID.04H]

### 3.4 `0x0B/0x1F` topology

每个 subleaf 返回：

| 寄存器 | 字段 |
|---|---|
| EAX[4:0] | 下一层 x2APIC ID shift |
| EBX[15:0] | 该层逻辑处理器数 |
| ECX[7:0] | level number |
| ECX[15:8] | level type |
| EDX | 当前逻辑处理器 x2APIC ID |

当 EBX=0 或 level type=0 时结束。0x1F 优先于 0x0B。[EXTERNAL Intel SDM 253665-092US, Vol. 1, CPUID.1FH]

### 3.5 `0x15/0x16`

见 `cards/timers.md`。当前 TSC 校准应把 0x15 的 EAX 看作 denominator、EBX numerator、ECX crystal Hz。[VERIFIED mykernel/arch/x86_64/kernel/tsc.c] [EXTERNAL Intel SDM 253665-092US, Vol. 1, CPUID.15H/16H]

## 4. 扩展叶 `0x80000000+`

这些叶由 AMD 首创，部分已被 Intel 实现。必须按最大扩展叶与厂商手册解释。[EXTERNAL Intel SDM 253665-092US, Vol. 1, extended CPUID tables] [EXTERNAL AMD APM 24593 Rev. 3.44, Vol. 2]

| 叶 | 主题/关键返回 | MyOS2 | 源码/备注 |
|---:|---|---|---|
| `0x80000000` | 最大扩展叶；vendor string mirror | `direct` | 保存 `extended_cpuid_level` |
| `0x80000001` | extended processor info/features | `stored` | ECX/EDX 存入能力槽；含 long mode/NX 等 |
| `0x80000002` | brand string bytes 0–15 | `direct` | 与后两叶拼接 |
| `0x80000003` | brand string bytes 16–31 | `direct` | — |
| `0x80000004` | brand string bytes 32–47 | `direct` | 去首尾空白 |
| `0x80000005` | L1 cache/TLB（AMD 定义） | `direct` | 读取 L1 摘要 |
| `0x80000006` | L2/L3 cache/TLB | `direct` | 读取 L2 size/TLB 摘要 |
| `0x80000007` | advanced power management | `stored` | EBX/EDX 能力，EDX 另存 `x86_power` |
| `0x80000008` | 地址宽度、extended features | `direct/stored` | EAX phys/virt bits；EBX capability |
| `0x80000009` | AMD 保留/厂商定义 | `unused` | — |
| `0x8000000A` | SVM revision/features | `stored` | 仅 EDX；没有 SVM enable |
| `0x8000000B–0x80000018` | 厂商保留/旧厂商扩展范围 | `unused` | 不作跨厂商假设 |
| `0x80000019` | 1 GiB page TLB | `unused` | AMD |
| `0x8000001A` | performance optimization identifiers | `unused` | AMD |
| `0x8000001B` | Instruction Based Sampling | `unused` | AMD |
| `0x8000001C` | Lightweight Profiling | `unused` | AMD |
| `0x8000001D` | cache topology | `unused` | AMD deterministic cache |
| `0x8000001E` | extended APIC/topology identifiers | `unused` | AMD topology |
| `0x8000001F` | encrypted memory capabilities | `stored` | 只存 EAX；页表 `_PAGE_ENC=0` |
| `0x80000020` | platform QoS enforcement | `unused` | AMD |
| `0x80000021` | extended feature identification 2 | `unused` | AMD |
| `0x80000022` | extended performance monitoring/debug | `unused` | AMD |
| `>0x80000022` | 新增厂商定义叶 | `unused/not-surveyed` | 必须以目标厂商当期手册为准 |

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]

## 5. 扩展叶关键位

### 5.1 `0x80000001:EDX` 常用跨厂商位

| 位 | 名称 | 说明 |
|---:|---|---|
| 11 | SYSCALL/SYSRET | long-mode syscall 指令 |
| 20 | NX | EFER.NXE 与 PTE.NX |
| 26 | 1G pages | PDPTE leaf 1 GiB |
| 27 | RDTSCP | RDTSCP 指令 |
| 29 | LM | long mode |
| 30 | 3DNow! ext（AMD legacy） | 不应在 Intel 假定 |
| 31 | 3DNow!（AMD legacy） | 不应在 Intel 假定 |

MyOS2 保存整字，但没有形成这些位与 EFER/页表/指令消费者的统一依赖层。[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]

### 5.2 `0x80000007:EDX`

| 位 | 名称 |
|---:|---|
| 0 | temperature sensor |
| 1 | frequency ID control |
| 2 | voltage ID control |
| 3 | thermal trip |
| 4 | hardware thermal control |
| 5 | software thermal control |
| 6 | 100 MHz multiplier control |
| 7 | hardware P-state control |
| 8 | invariant TSC |

具体低位是 AMD 口径；Intel 主要关心 invariant TSC 的对应枚举。[EXTERNAL AMD APM 24593 Rev. 3.44, Vol. 2] [EXTERNAL Intel SDM 253665-092US, Vol. 1]

### 5.3 `0x80000008:EAX`

| 位 | 字段 |
|---:|---|
| 7:0 | physical address bits |
| 15:8 | linear/virtual address bits |
| 23:16 | guest physical bits（厂商/虚拟化语义） |
| 31:24 | reserved/厂商定义 |

MyOS2 把低 8 位和次低 8 位分别写 `x86_phys_bits/x86_virt_bits`。[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]

### 5.4 `0x8000001F:EAX`（AMD）

常见能力包括 SME、SEV、page-flush MSR、SEV-ES、SNP 等，具体位随 APM 修订演进。MyOS2 仅保存原始 EAX，没有读取 C-bit position、reduced physical bits 或建立加密页表，因此本图不把它标作“memory encryption supported”。[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c] [EXTERNAL AMD APM 24593 Rev. 3.44, Vol. 2]

## 6. Hypervisor 叶 `0x40000000+`

这不是 Intel basic/extended 范围，而是 hypervisor vendor convention：

| 叶 | 常见用途 | MyOS2 |
|---:|---|---|
| `0x40000000` | max hypervisor leaf + vendor ID | KVM helper 间接探测 |
| `0x40000001` | KVM feature bits/Hyper-V interface ID 等，依 vendor | KVM clock stub 使用 feature helper |
| vendor-specific 后续叶 | pvclock、steal time、hint 等 | 未形成统一 atlas |

`kvmclock.c` 选择 `KVM_FEATURE_CLOCKSOURCE/CLOCKSOURCE2` 对应 MSR，但所有实际时钟注册被注释，且未见调用者。[VERIFIED mykernel/arch/x86_64/kernel/kvmclock.c]

## 7. 当前能力存储槽

`NCAPINTS=21`、`NBUGINTS=1`，硬件 CPUID word 与 Linux 派生/synthetic word 混合在一个能力位空间；`get_cpu_cap()` 只填其中一部分。[VERIFIED mykernel/arch/x86_64/include/asm/cpufeatures.h] [VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]

消费时必须知道某 bit 属于：

1. 直接 CPUID 位；
2. 厂商重映射位；
3. OS 合成能力；
4. bug 位；
5. forced set/clear 位。

当前代码缺少统一 producer 表，容易把默认 0 误判为硬件不支持，或把头文件名称误判为已探测。

## 8. 新增 CPUID 支持模板

```text
1. guard maximum leaf
2. enumerate subleaves with architectural terminator
3. store raw registers in explicit u32 fields
4. decode with mask/shift, not serialized C bit-fields
5. apply vendor/family/model quirks
6. validate dependencies (other CPUID bits, CR4, EFER, XCR0, MSR)
7. intersect all online CPUs for global features
8. expose detected/enabled/usable as separate states
9. add fixed-vector unit tests
10. cite exact SDM/APM revision
```

## 9. 推荐优先级

1. **P0 correctness:** 修正 leaf 0x15 公式；完成 xstate leaf 0D.0/组件枚举；厂商识别。
2. **P1 SMP:** leaf 0x1F topology、secondary CPU 能力交集。
3. **P1 security:** leaf 7 + ARCH_CAP/SPEC_CTRL 的可用/启用分层。
4. **P2 performance:** leaf 4/18 cache/TLB；leaf A PMU。
5. **P3 optional:** SGX/RDT/PT/AMX/Key Locker 等，需各自完整子系统，不应只加 feature bit。
