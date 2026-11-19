# 第104批：RTC六字段采样、BCD/日期域与启动墙钟

**角色**：MYOS2-A-C07a / MYOS2-LEAD-002-CHAT-EXEC-15。固定源码 `time@a039d9803ade`，148合同 `lead@40725302f9ee`，执行分支父提交 `4d3183678a64`。本批只形成追加静态文档及有限纸面整数模型；没有打开真实CMOS端口、运行内核或构建程序。数据返回、日期合法、单次一致样本和真实设备时间均不是同一层证据。

## 结论摘要

在固定time直接源码中，`read_persistent_clock64`通过`mc146818_get_time`取得六个`rtc_time_s`字段。直接函数的`simple_mc146818_gettime`依次请求秒、分、时、日、月、年六处CMOS寄存器；`mc146818_get_time`对每字段调用`bcd2bin`，再应用两位年`<=69`时`+100`及月份`--`。调用`rtc_tm_to_time64`将六个字段映射至`mktime64`，随后把其返回秒数赋给`timespec64.tv_sec`，`tv_nsec`显式设0。`timekeeping_init`消费此`wall_time`并写`tk->xtime_sec`和`tkr_mono.xtime_nsec`。但这条链**没有执行日期有效性校验器或被注释的RTC更新期保护；没有证明六字段同瞬间、RTC配置模式、世纪、实际端口设备值或最后运行时间正确**。（主要证据RTC104-E031–E104；消费者见facts七张表）

旧103 TK103-U02 的persistent-clock **源码来源环节被缩小范围**，并非U02全局关闭：`local_clock`来源、实际链接/运行、timekeeper其它写者和真实时钟仍未证。

## D00｜102/103四项定点退回，旧原件冻结

### R102-02：真名、定义链、必要消费者与读取账

固定103旧证据字面存在不代表P2定位正确。本批新建十组定点关系：
`TK103-E001→RTC104-E001`真实`set(CMAKE_C_FLAGS)`命令；
`TK103-E007/008/009/010/011→RTC104-E002/003/004/005/006`，定义名均为`__clocksource_update_freq_scale`；
`TK103-E012/013→RTC104-E007/008`，定义名`__clocksource_register_scale`；
`TK103-E030→RTC104-E009`，真实定义名`myos_hpet_clkevt_set_state_periodic`；
`TK103-E089→RTC104-E010`，本次取真正`#define TICK_NSEC`而不是旧的宏注释。旧E089不足以支持默认jiffies的`A-F04`/`TK103-M01`数值前提；本次保留有前提的HZ=1000、JIFFIES_SHIFT=8、TICK_NSEC=1000000、mult=256000000关系，但用有效定义锚替代，非新增设备测试。

initcall链按真实宏名完整中间层接合：`fs_initcall→__define_initcall→___define_initcall→__unique_initcall→____define_initcall`，经`__initcall_section`导向有条件的段/符号生成，必要锚RTC104-E015–E021。CONFIG_HAVE_ARCH_PREL32_RELOCATIONS和其它宏分支不同；**源码声明与宏文本不能替代真实预处理、最终链接段、启动迭代和initcall执行**。

103 D00里旧`A-M03`的`new_id=TK103-E030`是错配：通道数应换指固定`TK103-E027`或`HC102-E049`，并按`((id & HPET_ID_NUMBER) >> HPET_ID_NUMBER_SHIFT)+1`消费；旧`B-P09`则消费此处重新准确定位的周期函数RTC104-E009：OR了ENABLE/PERIODIC/SETVAL不等于最终TN_32BIT必为零。已重读`clocksource.h`、`clocksource_const.h`、`clocksource_api.h`、`clocksource_types.h`、`time_const.h`、`time_type_declaration.h`、`timekeeping_types.h`的固定time实际相关范围；这只证明本轮读账，不倒填103旧作者读取过程。

### R102-03：41条原图边差量复核

旧102图的36条DIRECT_CALL、5条CALLBACK_BINDING与103已登记的G102-01至41匹配。本次保留C07已认可的G102-10/11/12/19/25/36/39/40八处既有改判，不将其重算成新故障。每条旧from/to/type、固定来源、最小支持、实际函数/字段、第一止证门均写在`facts.D00.R102_03.rows`。

必要改动集中如下：

- **G102-20**：`__clocksource_register_scale→__clocksource_update_freq_scale`真正调用位置是`HC102-E095`，本人新锚RTC104-E011；原103错用指向入队和尾部的E012/E013。旧语义保留但支持ID必须改变。
- **G102-22**：同一注册函数实际另行调用`clocksource_enqueue`，应以RTC104-E012单独支持，**不与G102-20合并**。
- **G102-21**：更新频率条件内调用`clocks_calc_mult_shift`，以RTC104-E004支持，须先满足freq非零等分支条件。
- **G102-31**：`clocksource_done_booting`函数体通过`clocksource_select()`宏请求调用真正`__clocksource_select(false)`，需调用处RTC104-E013及宏RTC104-E014合读；源中的被注释调用不能当成真正保护或已执行事件。initcall是否被运行仍未证。

原图、原103差量表和所有旧评论均不改。R102-02与R102-03继续OPEN，由C07独立判定是否闭合。

### R103-01：定位符、宏体与真实读账

上述十条替代同时服务R103-01，不重复算十项新发现。新证据真实定义名与辅助行范围分账，E089从宏注释撤回证据资格、改为RTC104-E010的真实`TICK_NSEC`宏。原103历史137个真实短锚字面命中不改，不能把本104的新引文数反填103自检。103字段来源矩阵、A-F04/M01、ABC-06及D00消费者依facts旧→新表消费。MANIFEST把`base_snapshot`仅设`time（分支名）`，短SHA独立列；区分实际获取的完整源、只读窗口、26份有本104新引文的源与同会话旧六件复用，不把一列来源路径冒充另一列的读取范围。

### R103-02：旧clock写分数、负数表示及默认源

1. `tk_setup_internals`的`old_clock==NULL`跳过旧时钟移位块；若非NULL，无论`shift_change<0`还是else，均可能正常执行对mono及raw的`xtime_nsec`复合写；`shift_change==0`的`<<=0`仍是赋值，仅数值不变。`tkr_mono.base`在所读init/setup路径没有同样写入，要单列；不能称raw分数“此路径从未写”。源码见RTC104-E024–E026。
2. 被减数是`clocksrc_s::shift`的真实u32字段（RTC104-E022），不是另一结构的`tk_read_base::shift`。先按u32减法，再把可能超出`int`表示域的值转int；若得负数，还要证`-shift_change`本身可表示，随后移位计数0..63才合法。M15/A-I05/F02按这些阶段限定，不能凭数学差、ABI印象或实际换源未证环节签真实行为。
3. `ns_to_timespec64`负分支有**两层不同类型的取负**：内层`-nsec -1`以s64计算，在INT64_MIN先发生不可表示负号而停止正常C推导；若输入例如-1且内层合法，则`div_u64_rem`返回u64商，外层`-quotient-1`按u64模运算，结果再赋给`time64_t=s64`。对-1，外层u64全1转换为signed -1仍需实现约定，不能称无条件标准C恒等式。`s32 rem`地址传给`u32 *`形参亦是单独签名约束。进入本函数前`local_clock`返回u64而`ns_to_timespec64`形参s64，超s64可表示域的实参转换也需先行判断；native_sched_clock保持opaque（RTC104-E027–E030）。
4. 默认jiffies实际连续低32位mask下delta的高32位为0且shift=8；103的高字非零shift33场景只能标为**独立构造helper输入**，不能归因于已证明的默认timekeeper真实路径。R103-02仍OPEN。

**R102-01已经被C07在独立102部分闭合件中关闭**；100/101此前文档闭合和组合PASS_PENDING_LOCAL也不重开。

## A｜六字段从CMOS请求到rtc_time

调用链源级关系为：`read_persistent_clock64→mc146818_get_time→simple_mc146818_gettime→CMOS_READ(addr)→rtc_cmos_read(addr)`。独立字段请求顺序为`RTC_SECONDS=0`、`RTC_MINUTES=2`、`RTC_HOURS=4`、`RTC_DAY_OF_MONTH=7`、`RTC_MONTH=8`、`RTC_YEAR=9`；全是`rtc_time_s`结构中的`int`字段。其余`tm_wday/tm_yday/tm_isdst`在`simple_mc146818_gettime`函数中明确未赋值，而此次后续`rtc_tm_to_time64`只消费上述六字段，不能因前者没有填充就臆断本链运行失败。（E031–E046）

按真实包含路径`asm/mc146818rtc.h→asm/insns.h→insns/insns_arch_api.h→io_arch.h`，`CMOS_READ(addr)`宏转至`rtc_cmos_read`。它先向`RTC_PORT(0)`即名义0x70执行`outb(addr,...)`，再向`RTC_PORT(1)`即0x71请求`inb(...)`，把返回unsigned char存于`val`；`BUILDIO(b,b,char)`宏提供对应x86指令包装的源定义（E031–E035、E056–E060）。**这是索引选择请求后读取数值的源码链，不是直接执行了端口或取得真实设备响应。**

`lock_cmos_prefix/suffix`在此次直达读函数中被注释；`mc146818_avoid_UIP`及异常回退也被注释；读取`RTC_CONTROL`与ACPI世纪字段的语句同样注释。虽然定义了`RTC_UIP`、`RTC_DM_BINARY`、`RTC_24H`、`RTC_ALWAYS_BCD`，本直接路径没有活动模式读取或六字段重试保护（E047–E055）。因此六次独立读取是否组成同一个RTC时刻、芯片究竟使用BCD还是binary/12h模式、实际年份世纪和设备数据准确性均是停证门。`mc146818_get_time`正常路径末尾返回0，但`read_persistent_clock64`并不检查这一状态返回值，**0不是六字段一致/有效的证明**（E038/E062–E064）。

## B｜BCD、两位年、月份与校验是否调用

`bcd2bin(x)`用`__builtin_constant_p((u8)x)`选择`const_bcd2bin`或`_bcd2bin`，两个实际候选在有效8位原始值上的整数公式相同：`(x & 0x0f) + (x >> 4)*10`。它们执行数值变换，**没有调用`bcd_is_valid/const_bcd_is_valid`做十进制数码检测**；例如0x59变59，而0xFA虽可按算式得到160，却不是两个半字节都为0..9的合法BCD。这些是给定字节的有限数学说明，不是硬件输入观测（E067–E071）。

六个字段均无条件调用`bcd2bin`。如果芯片实际输出binary值0x16（十进制22），本转换会按BCD给16；但此情景要求尚未取得的硬件模式前提，不能称现有机器读错6。12小时编码的小时PM位也没有活动判定，不由`RTC_ALWAYS_BCD=1`的定义/矛盾注释代替硬件设置（E050–E055/E072/E073）。

解码后的`tm_year<=69`加100，随后`rtc_tm_to_time64`加1900：**合法BCD两位年69→2069，70→1970**，是固定源采用的窗口映射而非世纪寄存器实测；`tm_mon--`把BCD 1..12改为0..11，`rtc_tm_to_time64`后来重新`tm_mon+1`传给历法函数。若原月0，被处理成-1，直达链没有因此主动验证/报错（E074/E075/E085）。

`rtc_valid_tm`确实定义了年、无符号月范围、每月天数、小时、分钟和秒的检查，`rtc_month_days`消费真实每月天数数组和`is_leap_year`，闰年规则包括100/400年修正（E076–E082）。但此校验器**不在**所读`read_persistent_clock64→mc146818_get_time→rtc_tm_to_time64`的活动直接调用路径。一个独立校验器若被调用会接受2000-02-29、拒绝2001-02-29；这不能推出运行中已经读到其中任何一天，更不能反向断言日期已完成校验。

## C｜历法计算的整数域与启动墙钟

`rtc_tm_to_time64`把`((unsigned int)tm_year + 1900)`、`tm_mon+1`、日、时、分、秒交给`mktime64(uint year0, uint mon0, uint day, uint hour, uint min, uint sec)`（E085、E103）。`uint`在此源码是`unsigned int`，`mktime64`把`mon/year`保存为uint；其中`if (0 >= (int)(mon -= 2))`先进行**无符号减法**，再转换成`int`决定是否走一、二月的年份减1/月份加12分支（E086/E087/E103）。例如January mon0=1时无符号减2产生UINT_MAX，转int超范围为实现定义；在常见二补码映射为-1时才会进入Jan分支。February mon0=2减2后恰好0，进入调整；March mon0=3减2后为1，不进入调整。这不是可忽略的类型细节。

其后`year/4 - year/100 + year/400 + 367*mon/12 + day`等项在显式`time64_t`转换前有uint中间表达式，`year*365`亦须逐项检查是否溢出；不允许用无限精度的Gregorian代数式消去原C类型转换。**在正常小年份、正常BCD字段、合法移位/存储和所述January实现选择下**，独立纸面历法关系得到1970-01-01为0秒、2000-02-29为951782400秒、2000-03-01为951868800秒。这些数仅是指定输入的历法算术，不是执行仓库`mktime64`，也不是设备时钟实测（E086–E090）。反向`rtc_time64_to_tm`不在本批调用链，不研究全域往返。

`read_persistent_clock64`把历法秒值给`ts->tv_sec`，主动`tv_nsec=0`（E063/E064）。`timekeeping_init`先将它接收至`wall_time`，再单独用`ns_to_timespec64(local_clock())`产生`boot_offset`，计算`wall_to_mono=timespec64_sub(boot_offset,wall_time)`；`local_clock`实际声明返回u64，`ns_to_timespec64`入口参数s64，所以如果u64值超出s64，**参数转换在函数体之前已经是实现定义问题**，而不须展开`native_sched_clock`（E027/E028/E092–E094）。

随后代码选择默认clocksource、调用可能的enable并建立readbase，`tk_set_xtime(tk,&wall_time)`写`tk->xtime_sec=ts->tv_sec`和`tkr_mono.xtime_nsec=(u64)ts->tv_nsec<<shift`；对`tv_nsec=0`且shift合法有分数0的条件关系。`tk->raw_sec=0`另有活动赋值（E095–E099）。`tk_set_wall_to_mono(tk,wall_to_mono)`的调用虽然存在，但该函数体内保存`tk->wall_to_monotonic=wtm`的行已注释；**算出/传入墙钟偏移不等于该字段真正保存**。这仍仅限本次所读定义的写者范围，不假定全树没有后续更新，也不把RTC秒值推成单调时钟或已选HPET（E096–E099）。

## 未证范围与交回

本批有 **七张关联事实表**：六字段来源、活动采样步骤、BCD表示、校验定义与调用、历法数值与C类型、进入timekeeper字段、A/B/C止证连接；有 **14个指定输入的数学／类型情景**。实际候选和证据缺口独立置于`deferred-findings.yaml`，保留此前`TK103-U02`及CA-05/V14/FM96历史身份，不把旧问题算作新独立缺陷。当前所有候选最多是**条件性静态风险**，实际硬件、RTC更新冲突和运行错误0项被观测。

本批只提交D00追加文档待审，**R102-02/03、R103-01/02仍OPEN**；第102、103批仍维持C07的RETURN历史裁定，R102-01已CLOSED_DOCUMENTATION_ONLY不重开。作者完成后终态`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`、`D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW`、`STOP_WRITING`。不自行验收通过，不启动105。

所有成果限于新的 `user-rtc-wall-clock-104/` 六件、PR17原执行分支及本人一次正式评论。未修改或删除内核代码、构建脚本、旧成果/评论、PR元数据、master/time/lead；未运行预处理、汇编/编译/链接、仓库程序/模型/脚本、Graphviz、QEMU、实机CMOS访问或真实并发。`full_local_P9=NOT_RUN`、`runtime_evidence=NOT_RUN`；C07正式独立验收需Owner在主线发送消息前选择Pro。
