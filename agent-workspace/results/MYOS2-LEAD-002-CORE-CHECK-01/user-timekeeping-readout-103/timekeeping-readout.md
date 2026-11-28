# 第103批：Timekeeper 初始化、计数换算及两条时间读取链

**执行身份**：MYOS2-LEAD-002-CHAT-EXEC-15（MYOS2-A-C07a），沿用现执行对话；任务 CORE-USER-TIMEKEEPING-READOUT-103。固定源码 `time@a039d9803ade`，任务合同 `lead@7564e79a9b02`，父成果 `exec@1a92f9839e53`。本件为 **静态源码分析＋明示类型前提的有限整数推导**，不是编译、设备、并发或ELF实证。

## D00｜第102批三项文档退回追加纠正

### R102-01：全六件原始标签与真实证据分账

第102批冻结六件的所有原始验证标签共有 **131** 次：其中 `evidence.md` 的真实源码证据 **130** 条，`MANIFEST.md` P9-5说明中的示例字符串 **1** 次。原作者声称整个六件的130/130/0完全正确是过宽的；本次将旧总括**撤回**，保留历史130条短锚全部字面存在及旧130/130/0的原始作者申报，不篡改旧六件，也不伪造所谓第131条源码引文。主线review的R102-01仍OPEN，D00只提交待C07核收的纠正。

本103自己的源证据 `TK103-E001`—`TK103-E137`各在新`evidence.md`出现一次，且不在另外五件重复原始验证标签。**103全六件新原始计数137，真正源码证据137，降级0；137=137+0。** 这个机械计数不证明动态正确性，完整本地P9仍NOT_RUN。

### R102-02：定义资格、来源字段与消费者补锚

原102引文存在、部分主张也有条件成立，但它未能提供147规定的必要直接定义。此次不重写旧130条，新增如下对应证据链，详细的旧消费者ID及新证据ID逐项映射见 `facts-and-boundaries.yaml::D00.R102_02.source_definitions_new_103`：

| 补正范围 | 本103实际独立来源 | 原102消费者/证明界限 |
|---|---|---|
| `clocks_calc_mult_shift`精度搜索、商、移位和结果写回 | E002–E005 | B-P04、C-R03、M01–M07及F01/F02；不只靠调用点 |
| 11%调整、频率参数、sec归一和后续循环 | E006–E011 | 需要非零除数和合法类型；不把作者旧七例当本批新运行 |
| `HPET_MASK→CLOCKSOURCE_MASK→GENMASK_ULL→__GENMASK_ULL`及ULL、64位常量宏 | E019–E023、E133–E135 | 按独立宏及构建条件连接；只能在相应编译宏选入的前提下推导连续低32位mask |
| `hpet_base`类型与同名静态对象、channel字段 | E024–E026 | 结构声明不等于对象定义或存储生命周期证明 |
| `list_add`别名、真实 `list_add_to_next→__list_add_between`、合法性循环、列表头与迭代 | E014–E016、E039–E049、E137 | C-R04/05/06/08；有效链和重复入队没有默认认证；`__list_add_valid`源中不合法时循环不是已经观察到阻塞 |
| `clocksource_select`宏、register函数末尾注释调用及返回0 | E012/E013/E017/E018 | 原列表登记并不自动触发select，注释不是运行代码 |
| `tk_setup_internals` mono/raw clock、mask、cycle_last及双方shift/mult | E062–E068 | timekeeper缓存的对象、指针、参数须与后续读取分层 |
| `readl/writel`及 `__raw_readl/__raw_writel` | E053–E056 | 这些真实条件定义使用u32与volatile型转换；未预处理不能签选中分支、实际总线宽度、设备完成或原子事务 |
| HPET独立bit/offset、源对象/legacy处理、fs_initcall与list宏 | E027–E038、E050–E052 | ID_LEGSUP能力、CFG_LEGACY位、mode软件字段互不替代；声明initcall不证明实际运行 |
| IRQ描述符的handler保存与分发、真实HPET注册调用点 | E057–E059、E136 | 登记传入的handler不是中断dispatcher本身；硬件送达另证 |

**R102-S01/S02/S03同步落实**：`hpet_enable`中实际活动代码清 `CFG_LEGACY`，另一个`hpet_enable_legacy_int`虽有置位代码，但在本次读取的直达路径没有调用；`channel.mode=LEGACY`是软件赋值。旧A-M03源表达式须按 `((id & HPET_ID_NUMBER) >> HPET_ID_NUMBER_SHIFT)+1`消费，而非旧失括号记法；旧B-P09应改为“周期函数没有显式OR新增TN_32BIT”，并**不能**推出最终该bit=0，因为cfg输入若已有该位则可能保留。TSC的无符号差值自然按模域回绕，不据此声称C UB，也不据200000门宣称实测经过时间。

`HC102-F06`保留为`IR101-F08/U05`旧根跟进；旧8个条目不是8个已经确认的新独立内核bug。旧`MYOS2-LEAD-002::CA-05` / `MYOS2-LEAD-002-CORE-CHECK-01::V14`及`FM96-F03/U08`身份保持，已关闭的101/100文档项不重新OPEN。

### R102-03：41条调用/回调边逐项附加纠正

原102 DOT 的 **36条DIRECT_CALL＋5条CALLBACK_BINDING**已逐一登记旧from/to/type、真实语句或字段、保留/改型处置及证据来源，置于 `facts.D00.R102_03.table`，不覆盖原图。41条中定点纠正8条；其余33条保留所证的静态关系和到达限制。典型错误及新图处置：

- `channels_decode→kcalloc`、`cfg_sanitize→hpet_readl/writel`、`legacy→periodic`：旧from是**摘要/分支节点**，不能作为真正调用者。实际相应调用在`hpet_enable`函数体；摘要只用于DATA_FLOW/CONDITIONAL_STEP。
- `hz_macro→register_scale`应是`MACRO_EXPANSION`，不是宏对象自身执行的DIRECT_CALL。
- `register_irq→irq_handler`不能写成绑定中断dispatcher：它给`irq_descriptors[irq].handler`保存调用者传入的指针；`myos_HPET_init`传入`HPET_handler`；真正`hwint_irq_handler`在向量/对象有效等前提下通过描述符执行 **CALLBACK_INVOKE**。
- `irq_handler→hpet_handler`和`tk_clock_read→jiffies_clock`不该被误写为直接静态绑定：一个是中断handler字段的条件间接调用，另一个是所读clock对象`read`字段的条件间接调用；`clocksource_jiffies.read=jiffies_read`才是静态绑定。
- 原`timekeeping_get_ns→tk_clock_read`是有实参求值的合法DIRECT_CALL，可保留；同时必须新增`timekeeping_get_ns→timekeeping_cycles_to_ns`实际直接调用。

新103图使用分开的`DIRECT_CALL`、`CALLBACK_BINDING`、`CALLBACK_INVOKE`、`MACRO_EXPANSION`、`DATA_FLOW`、`CONDITIONAL_STEP`、`UNPROVEN_JOIN`，真实函数调用方/条件抽象节点/对象字段分别标注。

本D00终态只能是 **CORRECTION_SUBMITTED_PENDING_C07_REVIEW**。第102批仍RETURN，R102-01、R102-02、R102-03均保持OPEN；第101批R101-01已独立CLOSED_DOCUMENTATION_ONLY、组合PASS_PENDING_LOCAL，第100批同样保持既有闭合及组合PASS_PENDING_LOCAL。

## A｜默认源、初始化顺序与timekeeper字段来源

`timekeeping_init`实际执行请求的源码顺序为：`read_persistent_clock64(&wall_time)`、`boot_offset=ns_to_timespec64(local_clock())`、`wall_to_mono=timespec64_sub(boot_offset,wall_time)`、`clock=clocksource_default_clock()`、若有回调则`clock->enable(clock)`、`tk_setup_internals(tk,clock)`、`tk_set_xtime(tk,&wall_time)`、`raw_sec=0`、`tk_set_wall_to_mono(tk,wall_to_mono)`。未审的RTC与`local_clock`输入只作为opaque返回值处理，**必须合法写入且函数正常返回**才可推进；不能由名称推定真实日期或稳定时钟。（E074–E078）

`tk_core`是静态对象，包含`timekeeper_s timekeeper`；`shadow_timekeeper`另有独立静态对象，但本轮两个读接口直接取得`tk_core.timekeeper`，不能因为存在shadow就假定它参与刷新。C的静态零初始化规则仅是抽象C对象前态，和实际loader/页表/硬件可用性不同（E069–E071）。

固定`clocksource_default_clock`是**weak**定义，当前所读函数体返回`&clocksource_jiffies`；实际ELF是否存在强定义覆盖未证。`clocksource_jiffies` 的`read=jiffies_read`、`mask=CLOCKSOURCE_MASK(32)`、`max_cycles=10`、`shift=JIFFIES_SHIFT`、`mult=TICK_NSEC<<JIFFIES_SHIFT`：按真实源配置的`HZ=1000`分支，有`TICK_NSEC=(10^9+HZ/2)/HZ=1,000,000`、`JIFFIES_SHIFT=8`、`mult=256,000,000`。这些是源常量和编译条件的推导，非实测IRQ/时间精度（E083–E090）。

`tk_setup_internals`先取得旧`tkr_mono.clock`，写mono.clock/mask，再用`tk_clock_read`取`cycle_last`；随后把clock/mask/cycle_last给raw，旧clock不空时转换mono/raw的`xtime_nsec`移位，最终写两套shift与mult（E062–E068、E079–E082）。一个重要的类型门：`clock->shift`与`old_clock->shift`皆u32，`clock->shift - old_clock->shift`先作无符号差，再转换到`int shift_change`；例如8−32先得4294967272，然后转int超范围属于**实现定义**，不能直接把数学−24当作可证明的运行值。若移位量不在0..63，后继计算没有正常标准C语义。

`tk_set_xtime`仅将`ts->tv_sec`写入无符号`tk->xtime_sec`，并把`ts->tv_nsec`转换成u64后左移mono.shift，故要先证墙钟字段/位移合法。**实际`tk_set_wall_to_mono`函数中写`tk->wall_to_monotonic=wtm`的语句是注释**，所以“调用过保存函数”不足以证明字段得到wtm。本轮所读初始化函数中没有写`tkr_mono.base`或`tkr_raw.xtime_nsec`的活动赋值；在明确静态初态且无其它写者条件下它们保留初值，但绝不能从局部没写外推成全树永久零值或成功初始化（E072/E073/E078/E079）。

## B｜计数差值、最大阈值、转换与C整数域

`tk_clock_read`通过`READ_ONCE(tkr->clock)`取**一个指针**，然后调用`clock->read(clock)`；宏实现包含类型宽度断言及一次volatile读取请求，但它不自动给`clock/mask/mult/shift/cycle_last`整组字段提供一致快照。`timekeeping_cycles_to_ns`在返回后又读取`tkr->clock->max_cycles`，这与helper捕获的局部clock不是同一次读取。正常执行须保证对象存活、字段可读及回调正常返回；目前没有任何真实并发竞态观测（E093–E095、E097/E098）。

源表达式先计算`delta=(cycles-cycle_last)&mask`，两者均为u64，减法是无符号模2^64；当mask为连续低32位时，相当于低32位周期差，例如last=0xfffffffe、now=1给delta=3。对默认jiffies的`max_cycles=10`有三条源码返回链（E097–E101）：

| 真实条件 | 结果表达式 | 证明范围 |
|---|---|---|
| `delta <= max_cycles` | `((delta*mult)+xtime_nsec)>>shift` | u64先乘加再右移；多字段有效与乘积不丢有效信息需额外证明 |
| `delta > max_cycles`且`delta&~(mask>>1)`非零 | `xtime_nsec>>shift` | 返回旧分数而非本次delta换算；不是已观察到倒退 |
| `delta > max_cycles`且高位条件为零 | `delta_to_ns_safe` | 只是转入分拆辅助；并非所有移位参数都安全 |

例如默认条件下delta=1为1,000,000ns、delta=10为10,000,000ns；delta=11转向辅助，若输入合法其条件值为11,000,000ns；delta=0x80000000时返回旧`xtime_nsec>>shift`。这些全部是**指定输入的闭式整数和类型推导**，不是执行仓库函数或作者模型。

`mul_u64_u32_add_u64_shr(a,mul,b,shift)`将a拆为`ah=a>>32`及`al=(u32)a`，以`mul_u32_u32(al,mul)`生成u64低部乘积，再将b通过`__builtin_add_overflow`求ret和溢出指示，依次对ret右移、可能补`1ULL<<(64-shift)`、若ah非零再加`mul_u32_u32(ah,mul)<<(32-shift)`（E102–E106）。据源级C表达式分域：shift=0不会执行溢出补偿支，ah非零时高部左移32可合法；shift在1..32且对象/其它运算有效时上述位移数值合法；shift=33..63且ah非零时`32-shift`作为unsigned产生超大无效移位，**在该语句停止正常C推导**；shift>=64则更早`ret>>=shift`就非法。ah为零时不执行高字分支，某些33..63案例仍有合法的已达移位。不能将“safe”函数名或另一INT128 helper语义移入本函数，更不能说CPU已经陷阱。

普通路径中`max_cycles`只是比较门，不能单靠它保证`xtime_nsec`不会令u64乘加回绕；例如人工给b=UINT64_MAX与delta=1时，u64模域的结果不同于无限整数结果。数学领域、实际源硬件、已选编译类型和返回C存储转换均分别列在facts的23个有限模型。

## C｜ktime_get、real_ts64、归一和一致性

`ktime_get`读取`tk->tkr_mono.base`（`ktime_t=s64`）及`timekeeping_get_ns`（`u64`），经`ktime_add_ns(base,nsecs)`宏直接执行`base+nsecs`后返回s64。C通常算术转换把base转u64，再作无符号加法和模2^64；若无符号结果超出s64可表示范围，返回转换为**实现定义**，不能未经类型审计就称作“有符号加法溢出”。如base=−2、ns=3的有界运算结果1；base=S64_MAX、ns=1的回写超s64域，需要具体实现才能认证（E091/E111–E115）。

`ktime_get_real_ts64`从`tk->xtime_sec`取u64秒并赋给`time64_t`（s64）目标，另求nsecs，显式令`tv_nsec=0`再调用`timespec64_add_ns`归一；若xtime_sec大于S64_MAX同样存在实现定义的转换。正常输入条件下（sec=7、ns=1500000001）归一为(sec=8,ns=500000001)，不能当作真实日期或clock精度（E107–E110/E116/E118）。

`timespec64_add_ns`调用`__iter_div_u64_rem(a->tv_nsec+ns, NSEC_PER_SEC, &ns)`。底层用`u32 ret`作为商计数，每减一次divisor便ret++，返回低32位的商，写`u64 *remainder`；所以余数规范化不保证所有超大输入下的秒数完整。对`tv_nsec+ns`还需分辨long与u64的通常算术转换/无符号溢出，以及累加到有符号`tv_sec`时的范围；有界例不曾真正执行数十亿次循环（E118/E127–E129）。

`timespec64_sub`先分别做秒/纳秒的有符号减法，再将结果交给`set_normalized_timespec64`：正纳秒超1秒时依次减1e9并加秒，负纳秒时相反；归一后仅在每次运算合法且正常终止前提下得到`0<=tv_nsec<1e9`。归一调用不能倒过来替前面已经发生的signed溢出提供安全性（E117/E119–E122）。`ns_to_timespec64`正分支经`div_u64_rem`，但本源码中局部`s32 rem`传给签名为`u32 *remainder`的helper，存在**静态参数指针类型不一致**；没有编译结果，不能声称具体诊断或二进制行为。负分支`-nsec - 1`在nsec=INT64_MIN时先执行不可表示的有符号负号操作，在此点终止正常C推导，即使无限整数代数形式可改写也不能据此声称原C表达式安全（E123–E126/E130）。

最后，两读接口的源级`seqcount`循环/重试均被注释；`ktime_get`存在活动`WARN_ON(timekeeping_suspended)`调用，但`real_ts64`对应WARN行也被注释。一个READ_ONCE指针不保证多个base/秒/缓存字段在同一原子时刻；条件性的旧/新值混合可能性不代表已经发现真实并发故障。本批不扩到完整timekeeping更新、syscall或用户态ABI。

## 统计、证据和交回

- 137条新增固定time连续1–5行短锚，源文件30份，均以path、真实符号/宏及辅助行号索引；新原始计数按六件总量单独自检。
- 七组矩阵共55行：字段来源10、初始化阶段7、delta分支8、整数域9、读接口6、归一一致性9、A/B/C连接6；另23个有前提的有限数学/类型场景，没有执行作者模型。
- 第102批41条调用/绑定边旧→新逐项审理；新图正确类型分类；问题按既有旧根/本批条件候选/证据缺口分别收存。
- **禁令**：没有修改内核、loader、旧报告、任务书、主线/PR导航或旧评论；没有运行仓库脚本/程序、配置/预处理/编译/链接、Graphviz、QEMU、硬件、真实并发、外部付费模型或API。所有本地动态验证`NOT_RUN`，验收上限`PASS_PENDING_LOCAL`。

**作者终态**仅为`READY_FOR_MAINLINE_REVIEW`和`mainline_acceptance=PENDING`；第102批保持`RETURN`，R102-01/02/03保持`OPEN`，D00仅`CORRECTION_SUBMITTED_PENDING_C07_REVIEW`。待GitHub固定六件、PR17本人正式评论及远端回读全部完成后进入`STOP_WRITING`。交回`MYOS2-A-C07 内核分析主线续建`由Pro独立验收本纠正和103；不自签102关闭/103通过，不启动104。
