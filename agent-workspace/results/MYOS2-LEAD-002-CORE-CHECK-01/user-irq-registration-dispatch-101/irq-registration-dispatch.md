---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-IRQ-REGISTRATION-DISPATCH-101
record_type: bounded_irq_registration_route_dispatch_and_R100_documentary_correction
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
mainline_owner: MYOS2-A-C07
produced_by: GPT-6（可见模型家族；后台不可独立认证）
model_per_owner: GPT-6＋Extra（Owner指定次一档）
date: 2026-11-18
base_snapshot: time（分支名）
read_channel: connector
source_pin_short12: a039d9803ade
lead_contract_short12: 0cd745b92280
exec_parent_short12: dd07c3b19fa1
mainline_acceptance: PENDING
R100_01: CORRECTION_SUBMITTED_PENDING_C07_REVIEW
status: AUTHOR_STATIC_COMPLETE_PENDING_C07_PRO
runtime_evidence: NOT_RUN
---

# 第101批：第100批单命令勘误、IRQ描述符生命周期及HPET/LVT分发

**结论。** 固定time源可以分开证明：C代码中的IRQ注册/卸载、描述符读写和条件回调；IOAPIC路由索引与寄存器读写的请求；以及IDT/汇编入口、HPET/LVT注册、handler/ack/调度门的有界接合。**不能由这些请求证明**已产生真实外设中断、控制器配置成功、应答完成、发生上下文切换或CPU正常iret。此包是只读静态分析，不修改任何实现，不配置、编译或运行内核。

本批独立新证据105条、7组55行条件矩阵、21个有限模型、8条条件候选和9项证据缺口。每条新短锚的逐字1–5行及定义/命令边界见`evidence.md`，数据与每个模型的证据引用见`facts-and-boundaries.yaml`，分类图见`irq-registration-dispatch-paths.dot`。不同栏目不相加为实验或可复现bug数量。

## D00：R100-01六条旧锚追加纠正

第100批原六件与原作者评论、原101/101/0全冻结；原回执结论仍**RETURN**、R100-01仍**OPEN**。此包只提交追加文档补正，状态为`CORRECTION_SUBMITTED_PENDING_C07_REVIEW`，不能自签旧批通过。六条旧短引文可逐字匹配，但原选择符/证据单元有失格，纠正如下：

| 旧证据ID | 错误类别或定位 | 本101替代锚及真实定位 |
|---|---|---|
| `SP100-E063` | 把两条独立`list(REMOVE_ITEM...)`当作单个唯一命令块 | **`IR101-E001`**：`list(REMOVE_ITEM KERNEL_C_SRCS ...)`；**`IR101-E002`**：独立`list(REMOVE_ITEM KERNEL_ASM_SRCS ...)` |
| `SP100-E032` | 自造的`set(CMAKE_C_FLAGS_CONFIG)`源码名 | **`IR101-E005`**：`options_flags.cmake::set(CMAKE_C_FLAGS)`中的CONFIG_NR_CPUS宏追加命令，区别文件中前一个同名初始flags命令 |
| `SP100-E061` | 自造`file(GLOB_RECURSE_KC)` | **`IR101-E003`**：`file(GLOB_RECURSE KERNEL_C_SRCS ...)` |
| `SP100-E062` | 自造`file(GLOB_RECURSE_KASM)` | **`IR101-E004`**：`file(GLOB_RECURSE KERNEL_ASM_SRCS ...)` |
| `SP100-E069/E070` | 虚作实际节名`.text_output` | **`IR101-E006/E007`**：实际链接器输出节`.text`同一节的两个合法连续片段 |

`facts-and-boundaries.yaml::D00.six_corrections`记录每个原完整qualified_id、旧选择符/类别、真实源、保留与撤回、替代编号、全部可定位矩阵消费者及止证点。撤回第100批MANIFEST/facts/正文和评论“101条全具唯一命令/定义资格”的总括，**但不倒改旧101/101/0，也不机械改报100/101或102/102**；文字真实、符合资格、支持命题是不同检查。

强制消费R100-S01：`setup_per_cpu_areas`真实调用`load_direct_gdt(cpu)`（`IR101-E009`），`fill_cputopo_info`末尾有游标推进返回（`IR101-E010`）；`target_include_directories(kernel)`是实际CMake命令，列出`../myloader`候选搜索目录（`IR101-E008`），不能证明预处理必选择哪个头或真实ABI。本执行者本轮**实际全文读取**`percpu_area.c`，并阅读`asm/apic.h`相关接口/extern上下文，不把主线补读和作者101的新读倒造为100原稿已读。R100-S02继续要求计数/分配/稳定表项/最早无效访问门；R100-S03要求`x86_64_start_kernel`是分别调用`myos_early_init_system`和`start_kernel`，不能压缩成前者调用后者，也不能由文本图推断机器上线。R99-01已由主线另件关闭，第99批组合PASS_PENDING_LOCAL，不因100旧原始RETURN文字而重开。

## A．描述符注册、替换、撤销与回调所有权

`mykernel/arch/x86_64/myos/interrupt.c`中，`irq_descriptors[NR_VECTORS]`是静态源级数组，`NR_VECTORS`宏数值256。`register_irq`对`irq_descriptors[irq]`先保存controller、`irq_name`、parameter、flags=0、handler，随后在controller非空时分别检查是否存在install、enable回调：**install先于enable**。install的声明返回`unsigned long`，但此调用点**没有保存/检查它的结果**；只要实际执行正常到达函数尾就返回常量1。这证明源码的返回传播缺口，不代表已经失败或安装成功。（`IR101-E011/E013–E021/E030`）

`arg`是临时传给controller.install的指针，**未被注册函数写进IRQ描述符**；`irq_name`和controller指针直接保存，而非复制字串或深拷贝controller对象。来源对象的生命周期、回调重入和并发可见性都不因字段赋值而得到保证。重复在同一向量注册会先覆写既有字段；本函数中未先调用旧controller的disable/uninstall，能否构成设备行为异常还需实际重复调用与控制器契约。（`IR101-E015–E020`）

`unregister_irq`先对仍存在的controller分别请求disable与uninstall，再清零五个描述符字段并返回1；函数中看不到等待在途handler完成或释放外部controller/name的代码。清零只是这条执行路径的字段写入，**不是原子摘除、设备断流或历史请求排空证明**。（`IR101-E022–E025`）

`hwint_irq_handler`以`regs->irq_nr`作为描述符下标；仅在handler非空时调用`handler(parameter,regs)`，若handler正常返回则**重新读取**`irq_d->controller`决定是否发`ack(irq_nr)`。这里局部`int irq_nr=0`，与实参vector并非同一个值。若handler合法地改变/撤销本槽，后续ack的选择随第二次读取变化；这是条件模型，未观察到真实重入或数据竞争。（`IR101-E026/E027`）

这些函数内未见向量范围检查；若`irq`或`vec=256`，在合法256元素数组模型中可区别形成尾后一指针与随后访问无效成员，正常执行证明停在首个无效操作，不能凭静态模型声称处理器已陷阱。返回值1也不能证明此前所有控制器回调或硬件行为成功。

## B．IOAPIC域转换、RTE表示与读写顺序

本内核宏给出`FIRST_EXTERNAL_VECTOR=0x20`、`NR_VECTORS=256`；`VECTOR_IRQ(n)`增加起点，HPET0的路由向量按表达式为`0x22`。`IOAPIC_enable/disable/install/uninstall`都使用低索引表达式`(irq_nr-FIRST_EXTERNAL_VECTOR)*2+0x10`，前段按`unsigned long`参数计算，交给下级`uint8_t index`的`ioapic_rte_read/write`时可能窄化。**向量本身、IDT描述符下标、路由索引、真实IOAPIC输入脚和设备寄存器不是同一域**。（`IR101-E030–E037/E059/E060`）

`IOAPIC_init`从固定物理基址常量`0xfec00000`请求`phys_to_virt`并构造selector/data/EOI指针；其RTE初始化循环按`i=0x10;i<0x40;i+=2`覆盖24个低寄存器索引`0x10..0x3e`。这证明已读函数的初始化请求范围，不证明真实IOAPIC最大输入恰24或全部写入成功。隔离数值模型中：向量`0x20→0x10`、`0x22→0x14`、`0x37→0x3e`、`0x38→0x40`（后者超出这段初始化范围）；`0x98→0x100`在交给u8时得到`0x00`，`0xa0→0x110`则得到`0x10`，与最小向量候选撞数。必须另证这些输入确被IOAPIC实例调用、内存对象有效及设备存在，才能评估真实错误。（`IR101-E032/E048–E052/E058`）

`ioapic_rte_read`先写`index+1`读高32位并左移32，再选`index`读低32并合并；`ioapic_rte_write`先写低32，再右移`value`写高32，中间调用`mb()`并在尾部请求`udelay(1)`。它们是多次单独寄存器操作，**不能从代码推出原子64位事务或真实锁定**；外部映射、屏障实现、设备反应和同步必须单独取证。`IOAPIC_enable`在读改写中清`0x10000`掩码位，disable则置位。`IOAPIC_edge_ack`只请求`apic_msr_write(APIC_EOI,0)`，它不使用传入IRQ号；`IOAPIC_level_ack`先调用edge，再写`*virt_EOI_addr=irq_nr`，后者实际消费参数。（`IR101-E032–E047`）

`ioapic_retentry_T`是packed的C位域/联合类型。HPET的`myos_HPET_init`构造局部`entry`，逐项只写vector、deliver_mode、mask、reserved及部分physical dst字段，再把其地址作为`arg`传入`register_irq`，最后由`IOAPIC_install`以`*(unsigned long*)arg`重解释读入RTE写请求。**未在这些赋值覆盖的字段、位域打包、别名规则、实际对象对齐、字节序和机器码/寄存器值**各是独立止证门；声明packed不自动认证一个可移植的64位IOAPIC RTE字节布局。（`IR101-E036/E053–E057/E067–E070`）

## C．HPET/LVT启动接合、分发/ack和返回

`start_kernel`的活动函数体先调用`init_IRQ`，随后在时间初始化阶段调用`late_time_init`，再在后续调用`myos_init_percpu_intr`。前者`init_IRQ`请求`idt_setup_apic_and_irq_gates`，随后调用`myos_init_bsp_intr`，该函数内确有`IOAPIC_init()`；这些是同一调用链中的不同函数关系。IDT配置函数对`FIRST_EXTERNAL_VECTOR..NR_VECTORS-1`构造指向`irq_entries_start`的门，汇编stub按该向量push哑错误码与vector并跳到`sa_entintr_retp`，再call C入口`excep_hwint_context`。**这只是源码/ASM的入口连接意图**；未证IDTR实际加载、帧/GS/CPU布局正确或硬件触发事件。（`IR101-E051/E091–E102`）

**HPET**：`late_time_init→hpet_time_init`源码先请求`hpet_enable(NULL)`，其返回值未在该调用点检查，若正常返回随后请求`myos_HPET_init`。该注册函数把HPET0向量与controller/handler交给register_irq。HPET controller的enable/disable/install/uninstall绑定IOAPIC函数，ack绑定`IOAPIC_edge_ack`；HPET handler先`jiffies++`后`do_timer(1)`，旧HPET/jiffies双增问题仅沿历史审核范围保留，不能不审do_timer又新增一个独立已确认根因或真实频率结论。（`IR101-E063–E070/E103/E104`）

**LVT**：`myos_init_percpu_intr`的真实函数体调用`LVT_timer_init`，其中按宏关系LAPIC LVT timer vector候选为`0xee`。LVT controller的install/enable/disable/uninstall**均为NULL**，只有ack绑定`LVT_timer_ack`；故不能错误地把它经IOAPIC的`(vector-0x20)*2+0x10`计算成真实RTE登记。初始化先构造lvt_timer的vector/periodic值并`register_irq`，再**直接**调用一次`LVT_timer_ack(0)`，与未来某中断触发后的dispatcher ack不是同一次调用。`LVT_timer_ack`分别请求TMICT/LVTT/TDCR三个APIC寄存器操作，最后调用忽略参数的`IOAPIC_edge_ack`。实际MSR投递、定时频率、CPU0/AP状态、真实次数均未知。`LVT_timer_handler`增加`lvt_count`及条件显示只是源码计数，不证明物理周期。（`IR101-E071–E090`）

**从硬件向C再到汇编的末端**：`excep_hwint_context`根据`regs->irq_nr`是否小于0x20选择exception_handler或hwint_irq_handler，返回后才根据`!in_atomic()`请求schedule。C handler不正常返回则后续ack与调度请求均无正常可达证明；ack对象也按当前controller指针再次读取。两种已读HPET/LVT controller的ack路径都不使用传入的本地零IRQ号来选择IOAPIC输入，因此**不能单由ack(0)推断这两个实际注册实例错误**；另一个`IOAPIC_level_ack`会使用该参数，但不是这两个controller已读绑定。旧R65-S01/S02及R65C-S01对向量/错误码、`IRQ_NR`帧、GS和`iretq`及调度边界继续适用；从`in_atomic()==false`不能推出真实上下文切换或成功回到CPU。（`IR101-E026–E029/E038–E040/E044/E066/E100–E105`）

## 有界矩阵、延期与执行禁令

`facts-and-boundaries.yaml`保存**7组55行矩阵、21项有限模型**（索引/调用/位域/回调/返回分别立账），`deferred-findings.yaml`保存**IR101-F01–F08八项条件风险、IR101-U01–U09九项缺口**；均非真实故障确认。旧65异常入口与03调度集成的历史HPET/jiffies、CPU0/时基问题只保留已核来源；旧qualified_id不凭猜测创造，旧R65/R100文档问题不重复计为新内核缺陷。 `irq-registration-dispatch-paths.dot`使用不同类型边区分直接函数调用、回调绑定、数据/字段流、潜在到达及未证连接，**不是完整机器CFG**。

固定time新增105条真实单定义/唯一命令/汇编局部短引文逐字回核与路径索引对应，完整本地P9、配置/预处理/汇编/编译/链接、仓库程序/脚本、Graphviz、QEMU、硬件、真实并发和MMIO操作均**NOT_RUN**。没有外部一手版本化Intel/AMD手册，故不为HPET/APIC寄存器含义、EOI及周期作规范认证；所有涉及硬件的文字仅限源码常量和请求，硬件是否接受仍开放。

本作者只提交新101目录严格六件及本人一次PR17顶层正式评论。不修改100六件或旧评论、lead/PR16、master/time、PR元数据、内核/loader、脚本和任何既有执行者目录。不合并关闭、不强推、不新开分支PR。只有compare/六件固定与活动回读、四refs、PR16/17及评论全文与唯一性全部通过后才报`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`、`R100-01=CORRECTION_SUBMITTED_PENDING_C07_REVIEW`并`STOP_WRITING`。Owner交回**“MYOS2-A-C07 内核分析主线续建”**前选择Pro，由C07同轮独立核第100批纠正、101新成果和下一包；本执行者不自签100/101通过、不启动102。
