---
task_id: MYOS2-DR-005
produced_by: "ChatGPT Pro / GPT-5.6 Pro"
date: 2026-09-01
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"
inputs_read:
  - "mykernel/arch/x86_64/kernel/cpu/common.c"
  - "mykernel/arch/x86_64/kernel/cpu/cpu.h"
  - "mykernel/arch/x86_64/include/asm/cpufeatures.h"
  - "mykernel/arch/x86_64/processor/processor_types_arch.h"
  - "mykernel/arch/x86_64/processor/processor_const_arch.h"
  - "mykernel/arch/x86_64/kernel/tsc.c"
  - "mykernel/arch/x86_64/insns/fsgsbase.h"
  - "mykernel/arch/x86_64/kernel/fpu/init.c"
status: final
open_questions:
  - "CPUID.07H.1:EAX 的快速演进位域尚未在代码中消费；本卡只记录原始保存行为，新增功能应按目标 CPU 对照当期 SDM。"
  - "是否要将 C bit-field 叠加结构改成显式 mask/shift，以消除编译器布局依赖。"
---

# 速查卡：CPUID 探测、能力存储与消费

## 1. MyOS2 当前数据流

```text
early_cpu_init()
  ├─ cpu_detect(&boot_cpu_data)         # leaf 0 / leaf 1 version
  ├─ get_cpu_cap(&boot_cpu_data)        # 原始能力字
  └─ get_cpu_address_sizes()            # 0x80000008:EAX

check_bugs()
  └─ identify_boot_cpu()
       ├─ generic_identify()
       │   ├─ cpu_detect()
       │   ├─ get_cpu_cap()
       │   ├─ get_cpu_address_sizes()
       │   └─ get_model_name()          # 0x80000002..4
       └─ this_cpu->c_identify()         # 当前默认设备基本无厂商规范化
```

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]

`cpuinfo_x86` 用联合体把 `u32 x86_capability[NCAPINTS + NBUGINTS]` 与 `struct x86_cap_bits` 叠加。数组是可靠的原始存储接口；C bit-field 名称便于读代码，但其内存布局依赖编译器 ABI，不应当作跨编译器/跨语言序列化格式。[VERIFIED mykernel/arch/x86_64/processor/processor_types_arch.h] [INFERRED]

## 2. 已读取叶与落槽

| CPUID 输入 | 返回寄存器 | MyOS2 落点 | 当前消费者/用途 |
|---|---|---|---|
| `0x00000000` | EAX, EBX, ECX, EDX | `cpuid_level`、`x86_vendor_id` | 最大基本叶门控；厂商字符串被保存，但厂商设备注册逻辑被注释 |
| `0x00000001` | EAX | family/model/stepping | CPU 标识 |
| `0x00000001` | ECX/EDX | `CPUID_1_ECX/EDX` | x2APIC、XSAVE、FPU、TSC、MSR 等能力位 |
| `0x00000006` | EAX | `CPUID_6_EAX` | 原始保存；所读快照中未见策略消费者 |
| `0x00000007,0` | EBX/ECX/EDX | `CPUID_7_0_EBX`、`CPUID_7_ECX`、`CPUID_7_EDX` | FSGSBASE、SMEP/SMAP/UMIP 等位可被头文件/注释路径引用 |
| `0x00000007,1` | EAX | `CPUID_7_1_EAX` | 原始保存；未见运行时消费 |
| `0x0000000D,1` | EAX | `CPUID_D_1_EAX` | 原始保存；xstate 初始化被注释 |
| `0x80000000` | EAX | `extended_cpuid_level` | 最大扩展叶门控 |
| `0x80000001` | ECX/EDX | 两个能力槽 | 长模式/NX/AMD 扩展能力原始保存 |
| `0x80000002..4` | 全寄存器 | `x86_model_id[48]` | 品牌字符串 |
| `0x80000005/06` | 多寄存器 | cache/TLB 摘要字段 | `cpu_detect_cache_sizes()` |
| `0x80000007` | EBX/EDX | 能力槽与 `x86_power` | invariant/nonstop TSC 等原始位 |
| `0x80000008` | EAX | `x86_phys_bits`、`x86_virt_bits` | 页表物理/虚拟地址宽度 |
| `0x80000008` | EBX | 能力槽 | 扩展特性原始保存 |
| `0x8000000A` | EDX | 能力槽 | SVM 特性原始保存 |
| `0x8000001F` | EAX | 能力槽 | AMD 内存加密能力原始保存；页表加密宏当前固定 0 |

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]

## 3. 叶门控规则

1. 先执行 `CPUID.0:EAX`，仅在 `max_basic >= leaf` 时访问基本叶。[EXTERNAL Intel SDM 253665-092US, Vol. 1, Ch. 21]
2. 先执行 `CPUID.80000000H:EAX`，仅在 `max_extended >= leaf` 时访问扩展叶。[EXTERNAL Intel SDM 253665-092US, Vol. 1, Ch. 21]
3. 有子叶的叶必须先遵守该叶自己的终止规则；例如 leaf 7 的 `subleaf 0:EAX` 给出最大子叶，deterministic cache/topology 叶则用“类型为 0”等终止值。[EXTERNAL Intel SDM 253665-092US, Vol. 1, Ch. 21]
4. “CPUID 报告硬件能力”不等于“OS 已启用”。典型三段门：
   - `CPUID.1:ECX.XSAVE=1`
   - `CR4.OSXSAVE=1`
   - `XCR0` 中相应状态位被 OS 置位  
   AVX 还要求 SSE/AVX 状态同时可用。[EXTERNAL Intel SDM 253665-092US, Vol. 1, Ch. 13 and Ch. 21]
5. 对 MSR/控制寄存器位先做对应 CPUID 探测；保留位必须维持手册要求值。[EXTERNAL Intel SDM 335592-092US, Vol. 4]

## 4. 当前最重要的完整位表

### 4.1 `CPUID.01H:ECX`

| 位 | 名称 | 含义 | MyOS2 |
|---:|---|---|---|
| 0 | SSE3 | SSE3 指令 | 原始保存 |
| 1 | PCLMULQDQ | 无进位乘法 | 原始保存 |
| 2 | DTES64 | 64 位 DS 区域 | 原始保存 |
| 3 | MONITOR | MONITOR/MWAIT | 原始保存 |
| 4 | DS-CPL | CPL 限制的调试存储 | 原始保存 |
| 5 | VMX | VMX | 原始保存，未启用 CR4.VMXE |
| 6 | SMX | SMX/TXT | 原始保存 |
| 7 | EIST | Enhanced SpeedStep | 原始保存 |
| 8 | TM2 | Thermal Monitor 2 | 原始保存 |
| 9 | SSSE3 | SSSE3 | 原始保存 |
| 10 | CNXT-ID | L1 context ID | 原始保存 |
| 11 | SDBG | Silicon debug | 原始保存 |
| 12 | FMA | FMA | 原始保存，xstate 未启用 |
| 13 | CMPXCHG16B | 16 字节 CAS | 原始保存；未见基于此的门控 |
| 14 | xTPR | xTPR update control | 原始保存 |
| 15 | PDCM | perf/debug capability MSR | 原始保存 |
| 16 | Reserved | 保留 | 不得作为能力 |
| 17 | PCID | Process-context identifiers | 原始保存；CR4.PCIDE 未见启用 |
| 18 | DCA | Direct Cache Access | 原始保存 |
| 19 | SSE4.1 | SSE4.1 | 原始保存 |
| 20 | SSE4.2 | SSE4.2 | 原始保存 |
| 21 | x2APIC | x2APIC | APIC 路径读取/使用 |
| 22 | MOVBE | MOVBE | 原始保存 |
| 23 | POPCNT | POPCNT | 原始保存 |
| 24 | TSC_DEADLINE | TSC deadline timer | 原始保存；未见 clockevent 实装 |
| 25 | AESNI | AES 指令 | 原始保存 |
| 26 | XSAVE | XSAVE/XRSTOR | 原始保存；系统初始化被注释 |
| 27 | OSXSAVE | OS 已设置 CR4.OSXSAVE | 原始保存；当前不应据此假定完成 |
| 28 | AVX | AVX | 原始保存；XCR0 未配置 |
| 29 | F16C | half conversion | 原始保存 |
| 30 | RDRAND | RDRAND | 原始保存 |
| 31 | Reserved by Intel | 虚拟机常用作 hypervisor-present 约定，但不是 Intel 架构能力字段 | MyOS2 不应把它当通用 Intel 位 |

[EXTERNAL Intel SDM 253665-092US, Vol. 1, CPUID.01H feature table]

### 4.2 `CPUID.01H:EDX`

| 位 | 名称 | 位 | 名称 |
|---:|---|---:|---|
| 0 | FPU | 16 | PAT |
| 1 | VME | 17 | PSE-36 |
| 2 | DE | 18 | PSN |
| 3 | PSE | 19 | CLFSH |
| 4 | TSC | 20 | Reserved |
| 5 | MSR | 21 | DS |
| 6 | PAE | 22 | ACPI |
| 7 | MCE | 23 | MMX |
| 8 | CX8 | 24 | FXSR |
| 9 | APIC | 25 | SSE |
| 10 | Reserved | 26 | SSE2 |
| 11 | SEP | 27 | SS |
| 12 | MTRR | 28 | HTT |
| 13 | PGE | 29 | TM |
| 14 | MCA | 30 | Reserved |
| 15 | CMOV | 31 | PBE |

MyOS2 将全部 32 位原样保存；实际可见消费者包括 FPU、TSC、MSR/APIC 类能力，但缺少统一的“能力依赖→控制寄存器启用→替代实现”层。[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c] [EXTERNAL Intel SDM 253665-092US, Vol. 1, CPUID.01H feature table]

### 4.3 `CPUID.07H.0:EBX`

| 位 | 名称 | 位 | 名称 |
|---:|---|---:|---|
| 0 | FSGSBASE | 16 | AVX512F |
| 1 | IA32_TSC_ADJUST | 17 | AVX512DQ |
| 2 | SGX | 18 | RDSEED |
| 3 | BMI1 | 19 | ADX |
| 4 | HLE | 20 | SMAP |
| 5 | AVX2 | 21 | AVX512_IFMA |
| 6 | FDP_EXCPTN_ONLY | 22 | Reserved/deprecated slot |
| 7 | SMEP | 23 | CLFLUSHOPT |
| 8 | BMI2 | 24 | CLWB |
| 9 | ERMS | 25 | Intel PT |
| 10 | INVPCID | 26 | AVX512PF |
| 11 | RTM | 27 | AVX512ER |
| 12 | RDT-M/PQM | 28 | AVX512CD |
| 13 | Deprecated FPU CS/DS | 29 | SHA |
| 14 | MPX | 30 | AVX512BW |
| 15 | RDT-A/PQE | 31 | AVX512VL |

MyOS2 的 `FSGSBASE/SMEP/SMAP` 位有名称或调用点，但 `setup_smep/setup_smap` 与 BSP 的 `CR4.FSGSBASE` 启用路径被注释；因此必须把“位已采集”标作 detected，而非 enabled。[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c] [VERIFIED mykernel/arch/x86_64/insns/fsgsbase.h] [EXTERNAL Intel SDM 253665-092US, Vol. 1, CPUID.07H table]

### 4.4 `CPUID.07H.0:ECX` 与 `EDX`（当前扩展策略位）

| 寄存器/位 | 名称 | 典型 OS 动作 | MyOS2 |
|---|---|---|---|
| ECX.2 | UMIP | 置 `CR4.UMIP` | 探测位保存；启用被注释 |
| ECX.3 | PKU | 配置 `CR4.PKE`、PKRU 与页表 pkey | 页表 pkey 宏固定 0 |
| ECX.4 | OSPKE | 表示 OS 已启用 PKE | 不应由内核伪造 |
| ECX.5 | WAITPKG | 可能配置 UMWAIT MSR | 未使用 |
| ECX.7 | CET shadow stack | 配置 CET MSR/页表 | 未使用 |
| ECX.16 | LA57 | 置 `CR4.LA57` 并使用 5 级表 | 未使用 |
| ECX.22 | RDPID | 可读 TSC_AUX/CPU id | 未使用 |
| ECX.27 | MOVDIRI | 直接存储指令 | 未使用 |
| ECX.28 | MOVDIR64B | 64 字节直接存储 | 未使用 |
| ECX.29 | ENQCMD | 共享虚拟地址设备队列 | 未使用 |
| ECX.31 | PKS | supervisor protection keys | 未使用 |
| EDX.4 | FSRM | Fast Short REP MOV | 未使用 |
| EDX.5 | UINTR | User interrupts | 未使用 |
| EDX.10 | MD_CLEAR | 缓冲区清理能力 | 漏洞缓解逻辑被注释 |
| EDX.13 | RTM_FORCE_ABORT | RTM force-abort control | 未使用 |
| EDX.14 | SERIALIZE | 序列化指令 | 未使用 |
| EDX.15 | HYBRID | 混合核心 | 未使用 |
| EDX.16 | TSXLDTRK | TSX suspend/resume load-address tracking | 未使用 |
| EDX.18 | PCONFIG | 平台配置指令 | 未使用 |
| EDX.19 | ARCH_LBR | 架构 LBR | 未使用 |
| EDX.20 | CET_IBT | 间接分支跟踪 | 未使用 |
| EDX.22 | AMX_BF16 | AMX bfloat16 | 未使用 |
| EDX.23 | AVX512_FP16 | AVX-512 FP16 | 未使用 |
| EDX.24 | AMX_TILE | AMX tile architecture | 未使用 |
| EDX.25 | AMX_INT8 | AMX int8 | 未使用 |
| EDX.26 | IBRS_IBPB | 推测控制 | 缓解逻辑被注释 |
| EDX.27 | STIBP | sibling 线程预测隔离 | 缓解逻辑被注释 |
| EDX.28 | L1D_FLUSH | L1D flush MSR | 未使用 |
| EDX.29 | ARCH_CAPABILITIES | `IA32_ARCH_CAPABILITIES` | 定义/处理被注释 |
| EDX.30 | CORE_CAPABILITIES | `IA32_CORE_CAPABILITIES` | 未使用 |
| EDX.31 | SSBD | speculative-store bypass disable | 缓解逻辑被注释 |

未列出的位须按目标 CPU 与当期 SDM 处理，不可把旧 Linux 头文件当作永久规范。[EXTERNAL Intel SDM 253665-092US, Vol. 1, CPUID.07H table]

### 4.5 `CPUID.0DH.1:EAX`

| 位 | 名称 | 含义 | MyOS2 |
|---:|---|---|---|
| 0 | XSAVEOPT | 支持 XSAVEOPT | 原始保存 |
| 1 | XSAVEC | 支持压缩布局 XSAVEC | 原始保存 |
| 2 | XGETBV_ECX1 | 支持 `XGETBV(ECX=1)` | 原始保存 |
| 3 | XSAVES | 支持 supervisor state 的 XSAVES/XRSTORS | 原始保存 |
| 4 | XFD | 支持 extended feature disable | 原始保存 |
| 5–31 | Reserved/current-SDM-defined extensions | 必须按当期 SDM | 未消费 |

只读取 subleaf 1 不能获得完整 xstate 尺寸与组件偏移；subleaf 0 与每个置位组件的 subleaf 才提供 XCR0 支持集和布局信息。[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c] [EXTERNAL Intel SDM 253665-092US, Vol. 1, CPUID.0DH tables]

## 5. 当前缺失的规范化阶段

下列 Linux 式阶段在源码中存在轮廓但被注释：

- `get_cpu_vendor()` 与厂商设备注册；
- forced caps 在完整识别后的统一应用；
- `filter_cpuid_features()` 依赖过滤；
- `setup_smep()`、`setup_smap()`、`setup_umip()`、FSGSBASE enable；
- secondary CPU 与 `boot_cpu_data` 的能力交集；
- bug bit、推测执行缓解、alternative patching；
- `/proc/cpuinfo` 或等价用户态导出；
- CPUID 单元测试/模拟叶测试。

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c] [VERIFIED mykernel/arch/x86_64/kernel/cpu/bugs.c]

## 6. 已知陷阱与建议验证

1. **TSC 叶 0x15 字段方向。** `EAX=denominator`、`EBX=numerator`、`ECX=crystal Hz`，TSC Hz 为 `ECX × EBX / EAX`（三者非零时）。当前 `tsc.c` 的计算/返回量纲与该定义不一致，且常落入固定 3 GHz 回退，应写针对叶 0x15 的纯函数测试。[VERIFIED mykernel/arch/x86_64/kernel/tsc.c] [EXTERNAL Intel SDM 253665-092US, Vol. 1, CPUID.15H]
2. **OSXSAVE 位是状态回报。** `CPUID.1:ECX[27]` 反映 OS 已置 CR4.OSXSAVE；不能把它当作单纯硬件支持位。[EXTERNAL Intel SDM 253665-092US, Vol. 1, CPUID.01H]
3. **扩展叶有厂商语义。** `0x8000000A`、`0x8000001F` 等主要是 AMD 语义；Intel 上仅在最大扩展叶覆盖时读取，仍不应套用 AMD 字段到未知厂商。[EXTERNAL AMD APM 24593 Rev. 3.44, Vol. 2]
4. **跨 CPU 能力。** SMP 内核应以所有在线 CPU 的安全交集驱动全局替代路径；当前交集代码被注释。[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]
5. **测试最小集。**
   - 人工 CPUID backend：最大叶边界、subleaf 终止、全零叶；
   - Intel/AMD/KVM 三套固定向量；
   - TSC 0x15 正常、分母零、晶振零与 0x16 回退；
   - secondary CPU 缺少某位时的全局交集；
   - 检测位与 CR4/XCR0 未启用时，消费者不得执行受控指令。
