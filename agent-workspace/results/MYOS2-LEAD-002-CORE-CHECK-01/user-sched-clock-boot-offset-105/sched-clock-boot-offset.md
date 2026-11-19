# 第105批｜启动调度时钟、每CPU换算与boot_offset

**执行身份**：MYOS2-A-C07a / MYOS2-LEAD-002-CHAT-EXEC-15。固定源码`time@a039d9803ade`、固定主线合同`87ad6b6c914b`、原执行父`51f1f5b5dc08`；仅阶段2源码静态分析。GitHub连接器取得的数据、源程序文本、给定有限输入的整数推导均不是实际CPU指令执行、ELF链接、RTC读取或时钟标定。具体事实、七张表、17个有限样例、114个新源锚及D00差量账见另外五件。

## 结论先行

固定启动顺序是：`start_kernel→setup_arch`（平台分支选择，`tsc_early_init`），架构设置返回后`setup_per_cpu_areas`，再`timekeeping_init`读取`local_clock()`计算`boot_offset`，最后`late_time_init→hpet_time_init→tsc_init`。真实运行路径要先满足条件和返回门。**TSC调度时钟在early_init成功时，可以在每CPU新存储区分配和CPU0 GS基址切换之前请求读取、计算并发布cyc2ns参数**。源码呈现早期顺序，不证明此时GS指针/模板已正确映射或设备数据有效（SC105-E013–E018、E055/E056、E077–E083、E111–E114；原86批独立审核边界继续适用）。

`local_clock`确实返回`native_sched_clock`：`__use_tsc`真时调用`rdtsc→cycles_2_ns`，从当前CPU的`cyc2ns_offset/mul/shift`形成u64纳秒；假时返回`(jiffies_64-INITIAL_JIFFIES)*(1000000000/HZ)`的u64式。然后`timekeeping_init`将此u64数作为`ns_to_timespec64(s64)`参数，经过适用的整数转换/商余前提形成`boot_offset`，并计算**`wall_to_mono=boot_offset−wall_time`**。但`tk_set_xtime`真正消费的是独立RTC来的`wall_time`；`tk_set_wall_to_mono`内部目标字段赋值仍是注释。**调度时钟门、TSC clocksource注册/不稳定标记、内核墙钟字段及真实硬件计数器是不同状态**（SC105-E060–E062、E091–E108）。

## D00｜只修三个仍OPEN的文档事项

### R103-01：十个替代锚的直接消费者逐行收口

不重做原103正文和十个已被C07接受的真实定义。固定旧103的七张矩阵、有限模型、候选/缺口和G102旧边共核得如下**直接引用**，均以固定`user-timekeeping-readout-103/facts-and-boundaries.yaml`旧ID、旧`evidence.md`及固定104`evidence.md`为准：

| 原103证据→104替代 | 旧103确实直接引用它的行 | 本次归类 |
|---|---|---|
| E001→RTC104-E001 | ABC-06 | 保留实际配置命令支持 |
| E007→RTC104-E002 | 无 | 只有组级/定义性依赖 |
| E008→RTC104-E003 | G102-21 | 旧边历史引用，现行以RTC104-E002/E004及freq条件支撑 |
| E009→RTC104-E004 | G102-21 | 同上 |
| E010→RTC104-E005 | 无 | 只列组级 |
| E011→RTC104-E006 | 无 | 只列组级 |
| E012→RTC104-E007 | G102-20、G102-22 | G102-20已换HC102-E095、RTC104-E011 |
| E013→RTC104-E008 | G102-20、G102-22 | G102-22已换RTC104-E012 |
| E030→RTC104-E009 | 无 | 周期函数B-P09作为跨102专题消费，不制造“C-periodic-setting”行ID |
| E089→RTC104-E010 | A-F04、TK103-M01 | 保留；ABC-01只可列间接常量依赖，撤回虚构的旧103 A-M01 |

HC102的M01–M07、F01/F02若从D00频率更新分组消费，只登记“**分组级**”，不能把每条旧103 E007–E011都硬称被这些行直接引用。这里`A-M01`在旧103七表并不存在；旧102的A-M01是HPET映射地址，非TICK_NSEC。G102-20/21/22分别列历史引用与104现行替代，已闭合的R102-02/03不重开。十行全账见`facts.D00.R103_01.ten_consumer_reconciliation`；实际消费者必须能被固定批号、路径和局部ID解引用。

### R104-01：P2定义、调用与消费者对齐

1. 旧RTC104-E047跨`lock_cmos_prefix`和`lock_cmos_suffix`两条**独立宏**；现在各以SC105-E002、E003一条定义原文替代，实际`rtc_cmos_read`调用处SC105-E012核到两侧锁调用已注释，E033的`return val`不再充当该证据。
2. 旧E060`BUILDIO(b,b,char)`是**实例化行**，不是定义体。SC105-E005/E006单列真正BUILDIO宏体；原`mykernel/arch/x86_64/insns/io_arch.h`第239行只作`POSITION_ONLY`定位，不登记成新增P2宏定义。本批图也区分“实例化可生成出入包装”与实际端口执行。
3. 旧E063只说明`rtc_time_s tm`局部变量，E064才写秒和零纳秒，都不能单独支持“忽略状态返回”。SC105-E004直接读到`mc146818_get_time(&tm);`与随后写入的真正函数体；与旧RTC104-E062的callee返回0及RTC104-E038体内采样配对。用于`rtc_field_origin_consumer_matrix-08`、`active_sampling_steps_matrix-08`、F05与原图真正`persistent→mc_get`调用。未校验日期的定点否定严格只针对所读直达链。
4. 旧字段来源行07应以RTC104-E036/E037/E046/E061/E085证未写/未消费的weekday、yday、isdst；E031只定义val。原时间字段矩阵04–08分别按调用`RTC104-E092`、差值`E093/E100`、选源设置`E094`、写墙钟`E095/E096`、注释赋值`E097/E098`校正原错位。M12与U06的入口采用SC105-E100/E101/E103联原104`E027/E028/E102`支持；不从单个调用或单个字段声明外推值。旧默认jiffies的mask/shift要合旧`TK103-E083/E084/E088/E097/E105`与`RTC104-E010`，不是只引用E023缓存字段。
5. 公约§1第2条此次实际从**direct master**独立取回；lead上相同blob仅作身份对照。不将104当年的读取通道倒造为master。原104 104条字面引文、位置及旧文件全部冻结，新105 114条单独计数。

### R104-02：方向、未写字段、寄存器请求与午夜输入

旧图的`timekeeping_init→timespec64_sub`文字把减法说反。新图让`boot_offset`接**lhs**、`wall_time`接**rhs**，节点和SC105-E007/E008/102一致：`boot_offset − wall_time`。旧`simple_get→tm_others`改为`ANNOTATION`（局部未写），而不是伪造DATA_FLOW写入；RTC_CONTROL被注释读取应归`simple_mc146818_gettime`函数的SC105-E010，不当作mc getter直接读了控制寄存器。RTC_SECONDS/RTC_MINUTES等定义是**选择索引0/2/4/7/8/9**，`CMOS_READ`真正返回的是每次独立的未知设备字节，两者不能用等号混为“寄存器编号就是读到的数值”。

前批正常日历秒数在**明确00:00:00**条件下保留：
- RTC104-M07/M10：2000-02-29 00:00:00，`rtc_time={tm_year:100,tm_mon:1,tm_mday:29,tm_hour:0,tm_min:0,tm_sec:0}`；`rtc_valid_tm`若独立调用则可接受，历法关系951782400秒。
- RTC104-M09：1970-01-01 00:00:00，`tm_year:70,tm_mon:0,tm_mday:1`及三项0；`mktime64(year0:1970,mon0:1,day:1,hour:0,min:0,sec:0)`在January无符号减2转int选对分支的实现假设下为0秒。
- RTC104-M11：2000-03-01 00:00:00，`tm_year:100,tm_mon:2,tm_mday:1`及三项0；正常历法关系951868800秒。
- RTC104-M08：2001-02-29 00:00:00，`tm_year:101,tm_mon:1,tm_mday:29`；独立的`rtc_valid_tm`会拒绝该日，但未调用校验的日历算式在满足整数前提时可能与2001-03-01 00:00:00重合。此处不声称设备曾返回这样的字段。

完整按旧ID纠正的样例记录在facts.D00.R104_02中，原104图、文字、样例、原作者计数和评论均不改。R103-01、R104-01、R104-02仍**OPEN，提交待C07核收**。

## A｜启动校频与两种供应商回调

`x86_platform`对象的初值是`calibrate_cpu=native_calibrate_cpu_early`、`calibrate_tsc=native_calibrate_tsc`；并不意味着这两个函数必然是启动时最终调用对象。设有`CONFIG_HYPERVISOR_GUEST`时，`init_hypervisor_platform`可展开到`simple_init_hypervisor_platform`，从候选表调用detector、按返回优先级选对象、复制非空init/runtime回调并请求`x86_init.hyper.init_platform()`。若候选是VMware且其`vmware_platform_setup`的`ebx!=UINT_MAX`成立，它把查询得到的合成u64频率用`do_div(...,1000)`改成kHz并替换**两个**校频回调为`vmware_get_tsc_khz`。真实选用的CONFIG分支、vendor、hypercall结果和内核链接状态未证（SC105-E019–E036）。

**原生CPU校频与TSC校频的输入不能混为一谈。** `cpu_khz_from_cpuid`先要求Intel厂商和`cpuid_level≥CPUID_LEAF_FREQ(0x16)`，但其`cpuid(CPUID_LEAF_FREQ,...)`在固定源码中被注释，活动调用是`cpuid(CPUID_LEAF_TSC(0x15),...)`，随后拿EAX乘1000作为返回候选kHz。不能仅因变量名eax_base_mhz或旧注释就认证0x15 EAX等于MHz，也不能根据尚未引用的外部CPUID规范直接推实机错误（SC105-E040–E043）。

`native_calibrate_cpu_early`若候选为零则返回`DUMMY_TSC_KHZ=3*1000*1000`，对应3000000kHz的**人工兜底**，不是实测3GHz。`native_calibrate_tsc`则单独要求Intel及0x15可读；把EAX作为分母、EBX作为分子、ECX作为Hz，任一分子/分母为0便返回0。先整数计算`crystal_khz=ecx_hz/1000`；若为0且能请求0x16，回退到`eax_base_mhz*1000*denominator/numerator`计算晶振kHz（uint中间乘法可能先按宽度回绕），最终返回`crystal_khz*numerator/denominator`，仍是整数截断和宽度有门的源码公式。`CONFIG_X86_LOCAL_APIC`另影响可见lapic计算，不转向中断专题（E044–E050）。

`determine_cpu_tsc_frequencies(true)`按当前函数指针填`cpu_khz`，从`tsc_early_khz`非零时优先取它，否则请求`calibrate_tsc`，并**只在后者分支**写`clocksource_tsc.freq_khz`。随后`tsc_khz==0`时可能回退`cpu_khz`；所以缓存字段的赋值时点不能自动当作最终频率或当前选中clocksource。其它差值分支调用`abs(cpu_khz-tsc_khz)*10`，两个变量都是uint，必须考虑减法先在无符号域发生及实际abs宏/ABI，不把它直接说成数学实数域的绝对值。若最终仍为0就返回false；`tsc_early_init`据此跳过或调用`tsc_enable_sched_clock`。晚期`tsc_init`只有在`!tsc_khz`时尝试`determine(false)`，其原始重新校频代码在这条else路径已经注释，不能声称必定恢复成功（E051–E058）。

`tsc_init`尾部的TSC clocksource注册语句实际为注释，活动`clocksource_mark_unstable(&clocksource_tsc)`与`__clocksource_unstable`清除部分flags、置unstable（必要时改rating）的代码属于另一条状态链。**置不稳定标记不等于写`__use_tsc=false`，也不认证timekeeper使用何种source**。该区分见E059–E062和C路径。

## B｜每CPU cyc2ns参数从模板到写入

`struct cyc2ns_data`真实定义为`u32 cyc2ns_mul`、`u32 cyc2ns_shift`、`u64 cyc2ns_offset`。静态`DEFINE_PER_CPU(cyc2ns_s,cyc2ns)`模板初始化mul=1431655765、shift=32、offset=0；`DECLARE_PER_CPU`的**宏定义**与其在percpu_area.h的调用实例是不同对象。数据在特定CPU的可访问地址还需要分配、复制及GS/offset链条合法，不能从模板存在推出运行有效（E063–E066）。

`__simple_cyc2ns_read`用`this_cpu_ptr(&cyc2ns)`获得指针，然后分别`READ_ONCE`读出offset/mul/shift；`this_cpu_ptr→arch_raw_cpu_ptr`后者经`raw_cpu_read_long`得到GS基址并与静态perCPU符号地址作整数加法。`per_cpu_ptr(&cyc2ns,cpu)`则经`per_cpu_offset(cpu)`取得指定CPU写入地址。**这三个独立读取不等于锁住同一个三字段版本**，而保存端`c2n->data=data`周围的seqcount相关写语句已注释。没有进行并发复现（E067–E081）。

`__set_cyc2ns_scale(khz,cpu,tsc_now)`先以当前CPU**旧表示**对同一`tsc_now`求`ns_now=cycles_2_ns(tsc_now)`；`__cycles_2_ns`的表达是`old_offset+mul_u64_u32_shr(cycles,old_mul,old_shift)`。之后请求`clocks_calc_mult_shift(&new_mul,&new_shift,khz,NSEC_PER_MSEC,0)`，该函数从sft=32开始按uint64中间量做移位、加`from/2`、用`do_div`改写商，直到满足32位精度条件，再把结果存到u32 mult/shift。`khz`参数须在正常可表示、非零分母域；优先的启动输入来自uint tsc_khz。若新shift==32，代码把它降到31并把mul右移1，另引入一次舍入（E068–E075/E084–E090）。

新的offset写为`ns_now - mul_u64_u32_shr(tsc_now,new_mul,new_shift)`，然后将整个data赋给`per_cpu_ptr(&cyc2ns,cpu)->data`。在**相同tsc_now、同一合法CPU对象、全部运算合法、发布被一致消费**的前提下，“offset + 新换算”与旧`ns_now`在u64模意义下于该点相同；不意味着随后的任何时刻都精确相同，也不是跨核同步保证。若INT128分支被选，helper用无符号128位乘积后右移并截回u64；若分拆分支被选，取a高/低32位，低字乘积先右移，高字非零时另有`<<(32-shift)`。后一表达需要shift≤32且另满足合法位移，否则不能代入无限精度代数；实际CONFIG和编译分支未证（E071–E090）。

重点时序是：`setup_arch`中的`tsc_early_init`可能已请求`cyc2ns_init_boot_cpu`、`__set_cyc2ns_scale`；`start_kernel`在`setup_arch`返回后才调用`setup_per_cpu_areas`。后者的真正函数体调用`simple_pcpu_setup_first_chunk`，该helper从模板进行memcpy，然后`setup_per_cpu_areas`发布各CPU的`per_cpu_offset`，并在CPU0分支请求写GS_BASE。静态顺序说明**早期换算的地址归属不能直接由之后的模板复制/基址更新反推**。此处只消费86独立审核R86-S01/S02/S03的范围与其DOC-CLOSURE，不重新定性内存分配、GS寄存器或实机成功/失败（E013/E016/E055/E077–E083/E111–E114）。

## C｜local_clock分支、jiffies回退与boot_offset

`local_clock`的源级返回`native_sched_clock()`，它按初始为false的`__use_tsc`布尔量分支。TSC门真时请求`rdtsc`得到u64组合结果，再用`cycles_2_ns`读取当前CPU换算参数。`tsc_enable_sched_clock`先请求`get_loops_per_jiffy`与`cyc2ns_init_boot_cpu`，最后`__use_tsc=true`；并不检查`clocksource_tsc`当前是否已经登记或是否被标记不稳定。实际TSC计数、频率与CPU一致性全未观测（E055/E056/E063–E094/E109/E110）。

门假时的源码等式是`(jiffies_64 - INITIAL_JIFFIES)*(1000000000/HZ)`。`jiffies_64`在timer.c是实际u64对象，按`INITIAL_JIFFIES`初始化；systick_const.h和systick_macro.h各有同表达式`((ulong)(uint)(-300*HZ))`，不当作两个独立初值对象。HZ在固定asm-generic/param.h显式1000。因此对正常零差、加一tick的**指定有限算术输入**，返回分别0及1000000ns。-300000经过uint32变成4294667296，再转ulong/u64作为初始位型；jiffies减法和乘法均可能在u64模域回绕，不能直接当有符号负时差。arch/x86_64/kernel.lds有`jiffies=jiffies_64`声明，但实际构建选择了哪个链接脚本、别名地址与实时计数器都未验证（E093/E095–E099）。

无论走哪条，`local_clock()`的返回是u64，`timekeeping_init`却把它交给`ns_to_timespec64(s64 nsec)`。`u64>S64_MAX`的调用前转换为实现定义；即使输入在signed域，函数使用`s32 rem`变量而`div_u64_rem`声明需要`u32*`，存在独立的指针类型前提。对给定非负纳秒值可以列数学商余，但不在未编译源上承诺完整严格C正常执行；负值两层取负的R103-02已闭合，本批不重跑旧模型（E100–E105）。

`timekeeping_init`在读取RTC的`wall_time`后另取得这个`boot_offset`，按`timespec64_sub(boot_offset,wall_time)`计算局部`wall_to_mono`。固定RTC数据来自104已审核的六字段/BCD/mktime源级链；本批不重读端口。之后`tk_set_xtime`写入`wall_time`秒数与零纳秒分数；`tk_set_wall_to_mono`有调用，但函数体真正保存`wall_to_monotonic`的行被注释。**获取boot_offset、计算偏移和保存单调时钟偏移是三个不同动作，第三个在已读函数里不是活动赋值。** 不证明真实启动时间、时钟精度、已链接的clocksource或后继字段维护（E004/E007/E008/E100–E108）。

## 证明范围和状态

七张矩阵依次为`startup_order_and_gates`、`frequency_source_and_units`、`per_cpu_scale_provenance`、`integer_conversion_and_continuity`、`read_path_and_fallback`、`boot_offset_consumer`、`ABC_join`：共79行有界静态结论。另有17个显式输入的普通整数模型、8项条件候选和8项证据缺口；没有实际复现的硬件错误。源证据114个新1–5行短锚均独立对应固定time源定义、目标调用点和事实索引。图只以源关系分类，未运行Graphviz。

**不变状态**：102三项及103的R103-02保持C07的CLOSED_DOCUMENTATION_ONLY，第102批组合PASS_PENDING_LOCAL；R103-01余消费者映射及R104-01/02仍**OPEN**（本批D00仅为`CORRECTION_SUBMITTED_PENDING_C07_REVIEW`），第103、104批先前的RETURN不由作者更改。第105批只有六件与本人PR17评论交付后才可标`READY_FOR_MAINLINE_REVIEW`，`mainline_acceptance=PENDING`，之后本人`STOP_WRITING`。不启动106。

本包没有内核代码、补丁、原数据写回、旧提交或评论改写、PR元数据修改、额外模型/子代理/付费API、仓库脚本/模型执行、编译/预处理/链接、Graphviz、QEMU/虚拟机、CPU指令/RTC端口或真实并发实验。完整本地P9、运行证据均`NOT_RUN`。本轮交付仍须由**MYOS2-A-C07 内核分析主线续建**独立审核，Owner正式验收发令前选择**Pro**。
