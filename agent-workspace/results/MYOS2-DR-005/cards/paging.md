---
task_id: MYOS2-DR-005
produced_by: "ChatGPT Pro / GPT-5.6 Pro"
date: 2026-09-01
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"
inputs_read:
  - "mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h"
  - "mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h"
  - "mykernel/arch/x86_64/mm/mmu/mmu_const_arch.h"
  - "mykernel/arch/x86_64/mm/fault.c"
  - "mykernel/arch/x86_64/processor/processor_const_arch.h"
  - "mykernel/arch/x86_64/kernel/cpu/common.c"
status: final
open_questions:
  - "BSP 是否在安装任何 NX PTE 前置 EFER.NXE，静态路径未闭合。"
  - "页表物理地址 mask 是否在所有运行路径都使用探测到的 x86_phys_bits，而非固定掩码，需要专项追踪。"
  - "大页 PAT 位与 4 KiB PAT 位的缓存类型组合尚未做硬件一致性测试。"
---

# 速查卡：x86-64 分页与页表项

## 1. 当前层级

MyOS2 定义 `PG_LEVEL_4K/2M/1G/512G`，页表辅助接口提供 PGD/P4D/PUD/PMD/PTE 的构造、判定和写入。所读快照中使用的是常规 4 级 IA-32e 分页语义；`CR4.LA57` 的启用路径未见，P4D 在当前布局中是折叠/兼容抽象而非已确认的第五级硬件表。[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h] [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h] [INFERRED]

4 级、4 KiB 页的典型线性地址拆分：

| 位段 | 含义 |
|---|---|
| 47:39 | PML4 index |
| 38:30 | PDPT index |
| 29:21 | page-directory index |
| 20:12 | page-table index |
| 11:0 | 页内偏移 |

位 63:48 必须是位 47 的符号扩展；否则地址非 canonical。5 级分页启用后 canonical 宽度和索引上界改变。[EXTERNAL Intel SDM 253668-092US, Vol. 3A, Ch. 4]

## 2. MyOS2 页表位完整表

| 位 | 宏 | 硬件/软件意义 | 层级注意事项 |
|---:|---|---|---|
| 0 | `_PAGE_PRESENT` | present | 0 时其余位可由 OS 编码非 present 状态，但保留位规则仍需遵守 |
| 1 | `_PAGE_RW` | 1=可写 | 受 CR0.WP 与 U/S 共同约束 |
| 2 | `_PAGE_USER` | 1=用户可访问 | 各级祖先项都必须允许 |
| 3 | `_PAGE_PWT` | page-level write-through | 与 PCD/PAT 组成内存类型索引 |
| 4 | `_PAGE_PCD` | page-level cache disable | 同上 |
| 5 | `_PAGE_ACCESSED` | CPU 访问时置位 | 上级与叶项均可能使用 |
| 6 | `_PAGE_DIRTY` | CPU 写叶页时置位 | 对非叶项语义不同/保留，按表级处理 |
| 7 | `_PAGE_PSE` / `_PAGE_PAT` | 上级叶项中 PS；4 KiB PTE 中 PAT | 同一位在不同层级完全不同 |
| 8 | `_PAGE_GLOBAL` | CR4.PGE=1 时跨 CR3 保留 | non-present 时被 MyOS2 借作 PROTNONE |
| 9 | `_PAGE_SOFTW1` | OS 软件位 | SPECIAL/CPA_TEST 复用 |
| 10 | `_PAGE_SOFTW2` | OS 软件位 | UFFD_WP 复用 |
| 11 | `_PAGE_SOFTW3` | OS 软件位 | SOFT_DIRTY 复用 |
| 12 | `_PAGE_PAT_LARGE` | 2 MiB/1 GiB 叶的 PAT | 仅大页叶项 |
| 13–51/物理宽度范围 | PFN/下级表地址 | 受 `MAXPHYADDR` 限制 | 超出物理地址宽度的置位会产生 reserved-bit page fault |
| 52–57 | 保留/厂商扩展 | 按目标 CPU | 不可默认软件可用 |
| 58 | `_PAGE_SOFTW4` | OS 软件位 | DEVMAP 复用 |
| 59–62 | `_PAGE_PKEY_BIT0..3` | protection key | MyOS2 宏当前全为 0 |
| 63 | `_PAGE_NX` | execute disable | 仅 EFER.NXE=1 时合法；否则为保留位 |

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h] [EXTERNAL Intel SDM 253668-092US, Vol. 3A, Ch. 4]

## 3. 页大小与叶项

| 页大小 | 叶所在层 | PS/PSE | 地址对齐 |
|---|---|---|---|
| 4 KiB | PTE | 不适用；bit 7=PAT | 4 KiB |
| 2 MiB | PMD/PDE | bit 7=1 | 2 MiB；低地址位须为 0，bit 12 可作 PAT |
| 1 GiB | PUD/PDPTE | bit 7=1，且 CPU 报告 1G page 能力 | 1 GiB；低地址位须为 0，bit 12 可作 PAT |

MyOS2 的 `pud_ent_is_leaf()`、`pmd_ent_is_leaf()` 通过 `_PAGE_PSE` 判断，构造保护宏包含 `_PAGE_LARGE` 变体。[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h] [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h]

## 4. 当前保护模板

| 模板 | P | RW | U/S | A | D | G | NX |
|---|---:|---:|---:|---:|---:|---:|---:|
| `PAGE_SHARED` | 1 | 1 | user | 1 | 按硬件 | 0 | 1 |
| `PAGE_SHARED_EXEC` | 1 | 1 | user | 1 | 按硬件 | 0 | 0 |
| `PAGE_COPY/READONLY` | 1 | 0 | user | 1 | 按硬件 | 0 | 1 |
| `PAGE_*_EXEC` | 1 | 依模板 | user | 1 | 按硬件 | 0 | 0 |
| `PAGE_KERNEL` | 1 | 1 | supervisor | 1 | 1 | 1 | 1 |
| `PAGE_KERNEL_EXEC` | 1 | 1 | supervisor | 1 | 1 | 1 | 0 |
| `_KERNPG_TABLE` | 1 | 1 | supervisor | 1 | 1 | 0 | 0 |

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h]

一个值得专项审计的局部差异：`__PAGE_KERNEL_NOCACHE` 的 NX 位在源码中被替换为 `0`，意味着 nocache 内核映射模板可执行；如果该模板用于 MMIO，这会扩大不必要的执行面。[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h]

## 5. 缓存类型

MyOS2 用 PWT、PCD、PAT/PAT_LARGE 形成页级缓存模式，定义了 `PAGE_KERNEL_NOCACHE` 等模板，但完整 PAT 编程/枚举策略未在本卡范围形成闭环。[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h]

关键规则：

1. 同一物理页的不同别名必须使用兼容内存类型，否则可能出现机器检查或未定义一致性行为。[EXTERNAL Intel SDM 253668-092US, Vol. 3A, memory cache control]
2. 4 KiB PTE 的 PAT 是 bit 7；2 MiB/1 GiB 叶的 PAT 是 bit 12。
3. MMIO 通常不得用普通 WB；应由平台资源/驱动选择 UC/UC-/WC 等适当类型。
4. 修改映射缓存类型后需按 SDM 执行 TLB/cache 同步顺序。

## 6. 页面错误码

`#PF` 错误码的基本字段：

| 位 | 名称 | 0 | 1 |
|---:|---|---|---|
| 0 | P | non-present | protection violation |
| 1 | W/R | read | write |
| 2 | U/S | supervisor | user |
| 3 | RSVD | 普通 | 页表保留位被置 |
| 4 | I/D | data access | instruction fetch |
| 5 | PK | 非 protection-key | protection-key violation |
| 6 | SS | 非 shadow-stack | shadow-stack access |
| 15 | SGX | 非 SGX | SGX access-control violation |

MyOS2 异常汇编保存 error code，`do_page_fault` 路径可读取 CR2；但高级 PK/SS/SGX 机制未启用。[VERIFIED mykernel/arch/x86_64/mm/fault.c] [VERIFIED mykernel/arch/x86_64/entry/entry_64.S] [EXTERNAL Intel SDM 253668-092US, Vol. 3A, Ch. 6]

## 7. 更新与失效规则

- 将 non-present 改 present 前，先完全写好地址和权限，再发布条目；SMP 下需配合锁与内存屏障。
- 缩小权限、改 PFN、改缓存类型后，对所有可能缓存该翻译的 CPU 做 `INVLPG`、CR3 reload、INVPCID 或 shootdown。
- 仅清 A/D 位也可能需要失效，才能保证硬件重新置位。
- 若用 global mapping，普通 CR3 reload 不一定清除；需针对 PGE/global 的正确失效方法。
- PCID 引入后，CR3[63] no-flush 的错误使用会保留陈旧权限。
- 页表本身必须用 WB 一致内存并保持可访问；更改高层表时不能让执行 CPU看到半构造结构。

[EXTERNAL Intel SDM 253668-092US, Vol. 3A, Ch. 4 and multiprocessor memory ordering]

## 8. MyOS2 当前缺口

1. `_PAGE_NX` 无条件定义并大量进入模板，但 EFER.NXE 的 BSP 启用链未在所读快照静态闭合。
2. `_PAGE_PKEY_BIT*` 全部定义为 0；`setup_pku()` 与 PKRU 上下文切换缺失。
3. `_PAGE_ENC=0`；虽读取 AMD `0x8000001F:EAX`，但未形成 C-bit/memory encryption 支持。
4. `LA57` 能力可能被 CPUID 保存，但 `CR4.LA57` 和五级页表启动转换未实现。
5. PCID/INVPCID 的生命周期、ASID 重用与 shootdown 协议未见。
6. 需要针对 `__PAGE_KERNEL_NOCACHE` 可执行属性、PAT 组合、NXE 时序做运行时页表转储测试。
