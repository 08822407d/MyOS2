# 第102批：HPET enable、计数与clocksource消费的有界静态分析

**执行身份**：MYOS2-LEAD-002-CHAT-EXEC-15；Owner新对话：MYOS2-A-C07a 代码分析执行。  
**固定来源**：`time@a039d9803ade`；独立接收头：`lead@75338cd7d005`；旧执行结果：`exec@14e8f1580f5a`。  
**成果**：D00追加第101批20项资格纠正；A/B/C共用七组矩阵54行、18个有限模型、130条新固定短锚（§7强制独立定义补充10条，超过软上限，详见MANIFEST）。全为静态源码与明确条件推导，不是构建/动态/硬件认证。

## D00 — R101-01仅文档追加纠正，不改第101批原件

C07独立复审原件指出：第101批原始105个短引文虽然105条字面命中，但其中20条**未满足函数体内部的定义边界资格**，`IR101-E017`仅是函数参数头，另外19条跨函数头及体内。旧101的六件和105/105/0以及评论6976123017保持冻结。本次完全重新读取固定`time`，以各自**同一函数实际体内**连续1–5行替换证据资格，不假称旧证据物理删除或编辑。

以下旧ID均属于固定原 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-irq-registration-dispatch-101/evidence.md`；新ID全部指向本102 `evidence.md`。消费者来自原101 facts七组矩阵、21个模型与deferred中的实际引用，映射全文在本102 facts 的 `D00.supersedes`。

| 旧101证据 | 原定义 | 新102锚 | 追加处置 | 实际受影响消费者ID |
|---|---|---|---|---|
| `IR101-E017` | `register_irq` | `HC102-E001` | 仅参数头 → 实际定义体内 | A-L02, A-R03, B-V08, IR101-M01, IR101-M02, IR101-M03, IR101-F01, IR101-U01 |
| `IR101-E022` | `unregister_irq` | `HC102-E002` | 跨函数头 → 实际定义体内 | A-L07, A-R02, IR101-M06, IR101-F02, IR101-F04, IR101-U02, IR101-U08 |
| `IR101-E026` | `hwint_irq_handler` | `HC102-E003` | 跨函数头 → 实际定义体内 | A-L09, A-R05, A-R06, C-R01, C-R02, ABC-04, IR101-M04, IR101-M07, IR101-M08, IR101-M20, IR101-F01, IR101-F04, IR101-U01, IR101-U08, IR101-U09 |
| `IR101-E032` | `IOAPIC_enable` | `HC102-E004` | 跨函数头 → 实际定义体内 | B-V05, B-V06, B-V07, B-V08, B-R06, IR101-M09, IR101-M10, IR101-M11, IR101-M12, IR101-M13, IR101-M14, IR101-M15, IR101-M16, IR101-M18, IR101-F05, IR101-F07 |
| `IR101-E034` | `IOAPIC_disable` | `HC102-E005` | 跨函数头 → 实际定义体内 | B-V05, B-V07, B-R06, IR101-F05, IR101-F07 |
| `IR101-E036` | `IOAPIC_install` | `HC102-E006` | 跨函数头 → 实际定义体内 | A-R03, B-V05, B-V06, B-V07, B-V08, B-R08, ABC-01, IR101-M09, IR101-M14, IR101-M15, IR101-M16, IR101-M18, IR101-F05, IR101-F06, IR101-U04 |
| `IR101-E037` | `IOAPIC_uninstall` | `HC102-E007` | 跨函数头 → 实际定义体内 | B-V05 |
| `IR101-E038` | `IOAPIC_level_ack` | `HC102-E008` | 跨函数头 → 实际定义体内 | A-R06, B-R07, C-R05, IR101-M20, IR101-U08 |
| `IR101-E040` | `IOAPIC_edge_ack` | `HC102-E009` | 跨函数头 → 实际定义体内 | A-R06, B-R07, C-R03, IR101-M20, IR101-U08 |
| `IR101-E041` | `ioapic_rte_read` | `HC102-E010` | 跨函数头 → 实际定义体内 | B-R03, IR101-M13, IR101-M14, IR101-F05, IR101-F07, IR101-U03 |
| `IR101-E044` | `apic_msr_write` | `HC102-E011` | 跨函数头 → 实际定义体内 | B-R07, C-R03 |
| `IR101-E045` | `ioapic_rte_write` | `HC102-E012` | 跨函数头 → 实际定义体内 | B-R05, IR101-M17, IR101-F05, IR101-F07, IR101-U03 |
| `IR101-E048` | `IOAPIC_init` | `HC102-E013` | 跨函数头 → 实际定义体内 | B-R02, B-R09, IR101-U03 |
| `IR101-E051` | `myos_init_bsp_intr` | `HC102-E014` | 跨函数头 → 实际定义体内 | B-R09, C-S02, IR101-M11 |
| `IR101-E061` | `myos_init_percpu_intr` | `HC102-E015` | 跨函数头 → 实际定义体内 | C-S07 |
| `IR101-E065` | `HPET_handler` | `HC102-E016` | 跨函数头 → 实际定义体内 | C-S09, ABC-05, IR101-U05 |
| `IR101-E067` | `myos_HPET_init` | `HC102-E017` | 跨函数头 → 实际定义体内 | B-R08, C-S05, C-S06, IR101-M10, IR101-M18, IR101-F06, IR101-F08, IR101-U04, IR101-U05 |
| `IR101-E076` | `LVT_timer_handler` | `HC102-E018` | 跨函数头 → 实际定义体内 | C-R08, ABC-05, IR101-U06 |
| `IR101-E078` | `LVT_timer_init` | `HC102-E019` | 跨函数头 → 实际定义体内 | C-S07, C-S08, IR101-M19, IR101-U06 |
| `IR101-E081` | `LVT_timer_ack` | `HC102-E020` | 跨函数头 → 实际定义体内 | C-S08, C-R04, IR101-M20, IR101-U06 |

**新增强制末端宏**：`HC102-E024`独立锚实证 `LOCAL_TIMER_VECTOR=0xec`，`HC102-E025`为 `FIRST_SYSTEM_VECTOR=LOCAL_TIMER_VECTOR`，`HC102-E026`为 `VECTOR_IPI(irq)=(irq+FIRST_SYSTEM_VECTOR)`，`HC102-E027`为 `LAPIC_LVT_TIMER_IRQ=VECTOR_IPI(2)`。代数得到`0xee`；必须追加消费原101的 `B-V03`、`C-S07`、`IR101-M19` 和同一数值的正文与图。旧101的`IR101-E084/E085/E062`仍是历史索引，**不能用它们替代缺失的LOCAL_TIMER_VECTOR定义**。源级链成立不代表实际IDT/路由已运行。

**R101-S01**：`irq==256` 的 `&irq_descriptors[256]` 是one-past地址，随后成员访问失去正常C对象语义；`irq>256` 则更早在越界指针运算处止证。旧102不复写101原表，只对原`A-L02/A-R03/A-R05/A-R06/C-R01/C-R02/M01–M08`等消费者增加先决门。注册回调 `install` 和注销回调 `disable` 正常返回后，`p->controller` 必须在下一次取 `enable/uninstall` 时仍有效、非NULL且字段可读；同理 handler 后重新读取controller再ack。锚`HC102-E021/E022/E023`提供新回源，**不据此宣布现实竞态或必然崩溃**。

**R101-S02**：`index+1`先按整数提升规则求值，再在赋给 `uint8_t` 目标时发生窄化；如256→0是目标存储转换，并非加法已在8位溢出。新`HC102-E028`独立覆盖 `IOAPIC_map` 的 `uint8_t *virt_idx_addr` 与两个 `uint32_t*` data/EOI字段；`mb`不是互斥锁，多个RTE高低请求不等于硬件原子写，packed对象/指针别名/映射/ABI仍按老限定。

**R101-S03**：旧HPET/jiffies身份必须保留 `MYOS2-LEAD-002::CA-05` 与 `MYOS2-LEAD-002-CORE-CHECK-01::V14`，不改名为 `CORE-SCHED-INTEGRATION-03::IR-01`。只有链接别名及中断实际执行等前提都成立，才可能进一步推论；真实ELF、寄存器与IRQ送达本批没测。原`IR101-F08/U05`的`hpet_enable`返回值缺口由C专题接上，而不是新造一个双增root。

**D00结论**：`CORRECTION_SUBMITTED_PENDING_C07_REVIEW`。第101批仍 `RETURN`，`R101-01=OPEN`，待C07独立关闭。第100批`R100-01=CLOSED_DOCUMENTATION_ONLY`、组合`PASS_PENDING_LOCAL`不重开。本102的130/130/0与原101的105/105/0分账。

## A — 映射与HPET对象所有权

`hpet_set_mapping()`无条件把`hpet_address`设为`0xfed00000`，随后请求 `myos_ioremap(hpet_address,HPET_MMAP_SIZE)`，把返回赋给`hpet_virt_address`并调用`flush_tlb_local()`（`HC102-E035/E036/E042/E080`）。但旧96已审 `myos_ioremap` 的下级返回是物理端点 `paddr_last`，上层把它强制转换成`void*`，不是返回已校验的虚拟MMIO基址（`HC102-E030/E031`;旧`FM96-F03/U08`）。非NULL只可继续在“有效映射、有效MMIO对象、访问正常返回”**条件模型**内向下推导；不宣布实机必然地址错误。调用 `iounmap` 与真正解除页表映射分开：此固定`iounmap`函数体为空（`HC102-E122`），`hpet_clear_mapping`仅再把保存指针置NULL（`HC102-E043`）。

对象方面，`hpet_channel` 含 `num/cpu/irq/in_use/mode/boot_cfg/name`，`hpet_base`含 `nr_channels/nr_clockevents/boot_cfg/channels`；`hpet_mode`定义UNUSED/LEGACY/CLOCKEVT/DEVICE（`HC102-E033/E034/E121`）。ID通道数公式 `((id & HPET_ID_NUMBER) >> HPET_ID_NUMBER_SHIFT) + 1`，在声明宏的数学有效域内为**1至32**；`kcalloc(channels,sizeof(*hc),GFP_KERNEL)`失败跳`out_nohpet`，成功后发布global channels和nr_channels（`HC102-E049/E050/E051/E126`）。只将源码确实设置的字段视为本次发布，不把`boot_cfg`或其它模式字段等同设备实际状态。

全局CFG保存初读 `boot_cfg`，清 `ENABLE|LEGACY` 后**实际调用写寄存器**；通道循环保存本地 `boot_cfg`，解路由IRQ并写掉部分中断使能位，但随后`cfg &= ~(PERIODIC|PERIODIC_CAP|...)`的更宽清位仅用于warning，**该第二次本地修改没有后续writel**（`HC102-E052–E055`）。缓存字段不是已经接受的设备配置。

| `hpet_enable`出口 | 源码请求的最早判据 | channels / nr_channels | 指针及物理地址 | 频率 / clocksource | 语义上限 |
|---|---|---|---|---|---|
| 映射返回数值NULL，直接0 | `!hpet_virt_address` | 没有本次分配，旧静态值未触碰 | hpet_address已设固定值；未运行清理 | 没有本次频率转换/登记 | 非NULL和映射合法性仍分账 |
| cfg检查失败或period不合法，0 | `goto out_nohpet` | `kfree(hpet_base.channels)`、NULL、nr_channels=0 | `iounmap`调用、指针NULL、地址0 | 本次未登记；已有前态不自动消除 | iounmap空体，不保证页表撤销 |
| kcalloc失败，0 | `!hc` | 未发布新数组，清理当前global | 清指针与地址 | 频率已计算，尚未本次注册 | 既有global若非NULL可能影响重复调用的清理对象 |
| 计数未推进，0 | `!hpet_counting()` | 分配后统一清理 | 清指针与地址 | hpet_freq已更新，尚未本次登记 | 重启计数请求已发生，出口没有新的stop请求 |
| 计数通过，**无LEGSUP，0** | `!(id & HPET_ID_LEGSUP)` | 新分配保留、nr_channels保留 | 保存地址和指针 | **register_hz已发**，无统一清理 | 返回0不等于未登记 |
| 计数通过，**有LEGSUP，1** | legacy分支中周期helper正常返回 | channels[0].mode赋LEGACY，其余字段按既有状态 | 保留 | 已注册，periodic寄存器请求已发 | 不保证设备定时或IRQ到达 |

重复调用必须先给前态门：如果前次channels非NULL，本次成功`kcalloc`会覆盖全局指针，原对象所有权可能丢失；若本次在发布前跳清理可能清此前的global。没有找到已实际发生重复启动的证据，不把条件模型说成真实泄漏/双释放。访问域方面，`HPET_MMAP_SIZE=1024`、`HPET_Tn_CFG(n)=0x100+0x20n`、`HPET_Tn_CMP(n)=0x108+0x20n`；例如通道24 CFG名义偏移0x400达到1024界，通道31偏移0x4e0，但底层myos_ioremap页粒度可扩展到4KiB，**不能凭1024名义长度推出实际越界**，页表属性与真实MMIO范围另需证据。

## B — 计数、单位、转换和寄存器请求序列

`hpet_cfg_working`在每次寄存器读取合法且返回的前提下，至多作1000次 `HPET_CFG` 值比对；每次得到非`0xffffffff`即提前true，全部相同则false。单次MMIO非法/不返回时“1000次完成”前提不成立。`hpet_counting`先 `stop→reset COUNTER低/高→start`，随后读初始COUNTER和TSC，循环读取COUNTER；只要返回值变化就判true，否则对 `now-start <200000UL`继续循环。200000是**TSC周期差阈值**，没有独立证实TSC进展、回绕/非单调和MMIO完成就不能证明墙钟上限，更不能引用源码注释中的50–200µs当外部认证（`HC102-E044/E062–E067/E086/E127`）。

period在`100000`至`100000000`飞秒（含端点）被代码接受；频率先用 `u64 freq=FSEC_PER_SEC` 再 `do_div(freq,hpet_period)`作整数商，保存`hpet_freq`，**注册时才以(u32)窄化**（`HC102-E047/E048/E057/E082/E083`）。以下是有界整数推导，须独立满足32位无符号存储、合法非零除数、64位乘积不溢出和位移量有效等前提；`mult/shift`为本次已读 `clocks_calc_mult_shift` 的整数计算结果，不能把表当芯片测试。

| period（fs） | 原Hz整数商 | 进入注册的u32 Hz | mult / shift | 源码周期delta | 理想1ms计数差（整数） |
|---:|---:|---:|---:|---:|---:|
| 100000 | 10000000000 | 1410065408 | 3045934800 / 32 | 5540 | 10000000 |
| 200000 | 5000000000 | 705032704 | 3045934800 / 31 | 11081 | 5000000 |
| 232830 | 4294979169 | 11873 | 2759875347 / 15 | 658005558 | 4294979 |
| 232831 | 4294960722 | 4294960722 | 1000001531 / 32 | 1818 | 4294960 |
| 1000000 | 1000000000 | 1000000000 | 2147483648 / 31 | 7812 | 1000000 |
| 10000000 | 100000000 | 100000000 | 2684354560 / 28 | 78125 | 100000 |
| 100000000 | 10000000 | 10000000 | 3355443200 / 25 | 781250 | 10000 |

高频侧的`period=100000、200000、232830`在**允许筛选范围**内，但`floor(10^15/period)`超过32位上界，强转后`freq32`会变化；例如100000对应10GHz整值，进入clocksource注册却变成1,410,065,408Hz。period=232831已在32位频率域内。以上是源码类型/算术条件性缺口，真实芯片寄存器是否给这些period及硬件计数另证。

转换方向同样需要区分：`clocks_calc_mult_shift`从Hz计算`mult≈(10^9×2^shift)/Hz`，`clocksource`用于**cycles→ns**；但 `myos_hpet_clkevt_set_state_periodic` 当前计算 `delta=((NSEC_PER_SEC/HZ)×mult)>>(shift+7)`，将这个cycles→ns系数乘在纳秒持续时间上，量纲方向不是常规ns→cycles的反算。源中`NSEC_PER_SEC/HZ`首先是整数除法，`shift+7`与移位范围要先合法；`delta`再窄成uint参与 `cmp=now+(uint)delta`，并在第二次写compare时再以uint输出。上表展示这种差异，但**不能把理想计数差与源码delta的不同直接等同于已经观测的错误中断频率**（`HC102-E068–E071/E083–E085/E093/E094`）。

寄存器请求完整次序：`hpet_stop_counter` → 计算delta → read COUNTER(now) → read channel0 CFG → OR ENABLE|PERIODIC|SETVAL → 写 channel0 CFG → 第一次写CMP为cmp → `udelay(1)` → 第二次写CMP为delta → `hpet_start_counter`。其中`HPET_TN_PERIODIC_CAP`能力位没有单独经本次周期函数判断，`HPET_TN_32BIT`在该函数中仅见注释掉的候选代码。写序列不等于设备接受、比较器稳定或IRQ送达。`read_hpet`通过`hpet_readl(HPET_COUNTER)`构造`u64`返回值，而source mask为32位：返回类型宽不等于取了64位硬件主计数高半；低32位回绕与实际采样周期仍未证（`HC102-E072/E088/E123/E129/E130`）。

## C — 注册 → 候选选择 → timekeeper绑定（严禁偷换）

1. **结构绑定**：`clocksource_hpet` 初始化了`name=hpet`、`rating=250`、`read=read_hpet`、`mask=CLOCKSOURCE_MASK(32)`、`flags=CONTINUOUS`、`enable=hpet_enable`、list。字段`.enable`只是回调指针，不能倒推登记函数主动调用enable（`HC102-E038/E039/E089–E091`）。
2. **登记链**：合法计数检查后 `clocksource_register_hz(&clocksource_hpet,(u32)hpet_freq)` 宏到 `__clocksource_register_scale(cs,1,freq)`。先 `__clocksource_update_freq_scale`（freq非零时计算`sec`、`clocks_calc_mult_shift`及maxadj），随后`clocksource_enqueue`按rating排序 `list_add` 并增加count；register函数结尾的`clocksource_select()` **是注释掉的代码**。重复登记同一个已入队list节点的合法性不能从此单次路径假设；错误/回调返回未自动上抛（`HC102-E057/E087/E092–E098/E119/E120`）。
3. **选择链断点**：`clocksource_find_best`只是在列表里按条件返回候选。虽然`__clocksource_select`得到`best`，但该函数内调用`timekeeping_notify(best)`及把best赋给`curr_clocksource`的整个关键提交块都已注释（`HC102-E099–E102`）。`clocksource_done_booting`初始化`curr_clocksource=clocksource_default_clock()`后调用select，仍不因函数名而补出本次切换。initcall宏声明/注册和真实执行到达分开；不能从所审局部函数升级为“全树绝无其它选源途径”。
4. **实际消费链**：`start_kernel` 的 `timekeeping_init` 在后续 `late_time_init` 之前；`late_time_init`再调用`hpet_time_init`。timekeeper早期用`clocksource_default_clock()`返回对象，调用其可用的`.enable`后由`tk_setup_internals`把该对象保存到`tkr_mono.clock/tkr_raw.clock`并缓存mask/cycle_last/mult/shift；之后 `tk_clock_read` 通过`tkr->clock->read`调用，`timekeeping_get_ns`消费它（`HC102-E105–E112/E116–E118`）。所读`clocksource_default_clock`是**weak**定义，默认返回`clocksource_jiffies`；若链接中强定义覆盖则实际对象另证。不能把`curr_clocksource`的指针声明、列表最大rating或hpet_enable的返回直接称为timekeeper已改用HPET。
5. **IRQ侧仍是单独链**：`hpet_time_init`执行`hpet_enable(NULL)`却**不消费返回值**，无条件接着请求`myos_HPET_init`→`register_irq`，这说明代码可能在HPET enable返回0时仍发起中断注册请求，但请求、实际安装、投递及handler执行仍各需前提（`HC102-E114/E115/E017`；继承101 S01–S03和65入口限定）。`HPET_handler`的`jiffies++`和`do_timer(1)`属于旧`CA-05/V14`来源语境，不再作为本批新双增故障宣告。

因此，**“HPET被登记到clocksource候选列表”不是“HPET已成为系统时钟源”**。本固定text强烈支持一个有界断点：`timekeeping_init`早于HPET登记且以默认对象作为读时基；当前所读`__clocksource_select`没有活动commit或timekeeper通知语句。是否存在其它强链接实现和其它运行时改源途径仍需独立证据。

## 综合条件和新发现分流

本批`facts-and-boundaries.yaml`七组矩阵54行将有效映射、cfg与period、合法整数和位移、函数正常返回、登记/候选、timekeeper对象、实际IRQ的先决门逐行分开。18个有限模型区分period端点/窄化、通道域、NULL/无legacy/计数失败、TSC不推进、时源选择及D00资格。静态候选和证据缺口在`deferred-findings.yaml`登记：先引用旧`FM96-F03/U08`和`CA-05/V14`身份再提出新的不同根因，不重开已闭合事项。

本批**未运行**任何构建、配置/预处理、项目程序、Python/Graphviz验证器、QEMU、实机、真实中断或付费外部模型。未取HPET硬件规范一手修订，因此不下硬件可靠性/兼容性结论。`READY_FOR_MAINLINE_REVIEW`是作者交付状态，不等于主线验收；`mainline_acceptance=PENDING`，`D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW`。
