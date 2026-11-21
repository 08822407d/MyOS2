# 第106批｜启动延时标定、整数换算与HPET一微秒请求

执行身份：MYOS2-A-C07b / MYOS2-LEAD-002-CHAT-EXEC-16；合同 CORE-USER-BOOT-DELAY-ACCOUNTING-106。基线 time@a039d9803ade，lead@b8745d51ba7e，执行父6cf2affdba45，direct master@e8e60cd19540。只做第2阶段**源代码静态分析**与有限整数纸面计算，非内核运行、设备观测或汇编实测。BD106-E001～E079均在本包evidence.md逐项定位。

## 结果概述

固定源码中全局 unsigned long loops_per_jiffy 的**活动初值是10371364**，邻接的(1<<12)只是注释。early tsc 校频成功后 tsc_enable_sched_clock 可将它改成 floor(((u64)tsc_khz×1000)/HZ)，在HZ=1000的给定构建宏下等于tsc_khz；__const_udelay实际读取此全局，VMware的局部lpj不是它。对应BD106-E026～E039、E055。

HPET活动周期函数先向HPET_Tn_CMP(0)写cmp，接着真正调用udelay(1)，再向同一寄存器写delta并重启计数器。**一微秒是源码名义参数，并非经过的时间或设备接受了两次写的证明。**对应E064～E072。

late_time_init中 hpet_time_init 与 tsc_init 是**两个同级顺序直接调用**；所以本次HPET调用若确实到达，在晚期tsc_init之前读延时全局。早期校频未成功时可读到10371364静态值；早期成功且写入有效则可读早期计算值；后续晚期更新不可能倒流影响已经发出的本次调用。所有条件必须明确满足；并不证明实机执行路线。E024/E025/E032～E036。

## D00｜R105-01：直接消费者的证据补正

旧105六件、114/114/0自检和作者评论冻结不改；原105原始114标签、**33个不同源路径、113个不同的路径+起止行窗口**，其中SC105-E007和E102为同一窗口；旧MANIFEST把源路径写为34不能当作34份独立文件。已由C07裁定文档闭合的103/104及其原ID不重开。

| 原105直接消费者 | 新证据与正确限定 |
|---|---|
| startup_order_and_gates-03/-04 | 旧SC105-E023/E024为启用hypervisor宏别名和未启用stub两个候选；新增E001列固定set命令GUEST/KVM，E002列hypervisors[]实际VMware和条件KVM，E003～E006列检测选h、返回h、simple_init_hypervisor_platform实际调用copy_array复制非空回调槽及随后间接调用x86_init.hyper.init_platform()。不能说detector本人复制。 |
| startup_order_and_gates-10/-12 | -10需旧SC105-E013（setup_arch与perCPU顺序）联E014（timekeeping_init调用）而非只引后者；-12必须联本包E010：determine_cpu_tsc_frequencies(false)自身else分支的重新测量语句**已注释**，不能只引调用者tsc_init（E035/E036）。 |
| integer_conversion_and_continuity-09、frequency_source_and_units-14、SC105-F08 | 旧SC105-E053只列abs(cpu_khz-tsc_khz)*10比较；新E007核定两对象实际是uint，E008/E009给出abs与__abs_choose_expr定义。先作uint减法，再由macro选择signed int临时量；超出INT_MAX转换依赖实现，-INT_MIN取负和乘10存在有符号溢出门，最终与uint比较又依通常算术转换。不能直接用数学绝对值模型代证。 |
| boot_offset_consumer-02/-03/-05、ABC_join-06、SC105-M15/M16、F07/U06 | E011实际local_clock签名返回u64；E012实际ns_to_timespec64形参s64及s32 rem；E013实际div_u64_rem(u64,u32,u32*)要求u32*，和s32*调用形参存在独立类型不匹配前提；E014补真正负数分支。旧SC105-E101是 timekeeping_init 对 ns_to_timespec64(local_clock()) 的真实调用。u64到s64超范围表示仍未确定。 |
| read_path_and_fallback-03、ABC_join-03 | E015独立宏#define __cyc2ns_read __simple_cyc2ns_read，E016实际读链使用该别名，合旧SC105-E067实现与E068调用点，不能用实现体自行证明宏别名。 |
| per_cpu_scale_provenance-12 | E020～E023是simple_pcpu_setup_first_chunk的静态模板复制、pcpu_base_addr与pcpu_unit_offsets；E018/E019是setup_per_cpu_areas在helper返回后计算delta并发布per_cpu_offset(cpu)；per_cpu_ptr随后消费发布后的偏移。早期GS/地址有效性仍受86批结论限制。 |

## D00｜R105-02：旧图边差量

| 105原边/描述 | 纠正为源级操作方与真实方向 |
|---|---|
| jiffies_branch → jiffies64（DATA_FLOW） | 反向为 jiffies64 → jiffies_branch；INITIAL_JIFFIES与HZ也进入表达式；表达式 → u64返回值。E017合旧SC105-E093/E095/E098。 |
| hyper_detect → hyper_cb（把被选回调直接复制） | detector → 返回h → simple_init_hypervisor_platform接收h → copy_array对非空槽写入x86_init.hyper → 条件间接调用其init_platform回调。E003～E006/E079。 |
| pcpu_first → percpu_copy（同时“模板复制与offset发布”） | simple_pcpu_setup_first_chunk → memcpy模板，helper → unit_offsets；setup_per_cpu_areas → delta+unit_offset → per_cpu_offset(cpu)发布；per_cpu_ptr消费该偏移。两次操作属于不同真实函数，E018～E023。 |
| pcpu_offsets → percpu_ptr（短路） | 明列中间delta及per_cpu_offset对象，CPU号、映射生命周期与GS另待证。 |
| 105正文“late_time_init→hpet_time_init→tsc_init” | 改成late_time_init → hpet_time_init和late_time_init → tsc_init两个顺序同级直接调用，E024/E025；105图原两条直接边本来正确。 |

D00结论仅是**CORRECTION_SUBMITTED_PENDING_C07_REVIEW**。R105-01/R105-02仍OPEN，105仍RETURN。不得由执行者宣称关闭或更改历史原件。

## A｜延时数值来源、写者和时序

1. 全局loops_per_jiffy在init/main.c:68活动定义为10371364，:67的(1<<12)注释无效；linux/kernel/delay.h:24是extern声明，不创建第二份存储；x86 delay.c的__const_udelay读取这个全局。E026/E027/E055。
2. 源码KHZ=1000、HZ=1000；get_loops_per_jiffy先把uint tsc_khz提升至u64乘KHZ，再用do_div按HZ整除，并转换返回ulong。固定x86-64通常ulong宽64位，但没有编译与ABI证据；所有给定小输入均在正常数值域内。kHz→每秒周期数→每tick数的单位不能跳级；不是直接测得物理循环耗时。E007/E028～E031。
3. tsc_early_init请求determine(true)，返回false时不改全局；返回true时tsc_enable_sched_clock先赋 loops_per_jiffy，再cyc2ns_init_boot_cpu，最后__use_tsc=true。若失败进入late，tsc_init在!tsc_khz条件下请求determine(false)；但被调用函数真正的重新校频else语句是注释，不能因此保证会补救成功。E010/E032～E036/E076/E077。
4. vmware_platform_setup内u64 lpj是局部变量；vmware_hypercall条件通过后将频率存局部lpj和tsc_khz，然后只转换局部tsc_khz；preset_lpj赋值是注释。它更改校频回调，可能通过早期调用**间接**影响全局lpj，而不是自己写同名全局。E037～E039/E078及旧SC105-E034/E035。
5. __use_tsc属于调度时钟门；注释的use_tsc_delay()不切换延时；delay_fn实际初始指向delay_loop，use_tsc_delay实现本身亦被注释。E032/E033/E040/E041。

三种可读值条件：早期失败时固定静态初值；早期成功时早期有效tsc_khz计算值；晚期成功即使存在也只能影响随后调用，当前HPET周期调用发生在它之前。并发写者、真实已链接代码与内核路径是否走到此处仍未确认。

## B｜宏分支、mull位宽与循环分发

头文件候选链为 linux/kernel/delay.h → asm/delay.h → asm-generic/delay.h，time配置include列表含相应源树；这是源码关系，未执行预处理。四宏各自通过__builtin_constant_p(n)区分编译可判常量与变量包装，常量分支均先判断 (n)/20000 >= 1，真时引用仅已声明的__bad_*；是否产生可达符号引用以及链接错误尚未测试。n为负数或不同有符号类型时，不能将整数商判断笼统重写为“n>=20000”。E042～E054/E063。

| 宏 | 常量支路传给__const_udelay的数 | 变量支路 |
|---|---|---|
| sdelay(n) | n×0x100000000ul | __sdelay(n) |
| mdelay(n) | n×0x418938ul（源码邻近注释的0x418937并非实参） | __mdelay(n) |
| udelay(n) | n×0x10c7ul，即n×4295 | __udelay(n) |
| ndelay(n) | n×5ul | __ndelay(n) |

四个变量包装依其实际函数体转换后又调用__const_udelay。udelay(1)若判常量，1/20000=0后直接传4295；若变量当前值为1，经__udelay(1)再传4295，调用层次不同。正整型字面19999分支未超界，20000分支请求__bad_udelay；只为源级模型，非编译实验。

__const_udelay先读取全局lpj，令xloops*=4；随后汇编模板含mull %%edx，输入绑定EAX=xloops、EDX=lpj*(HZ/4)，输出绑定EDX→xloops、EAX→d0，接着__delay(++xloops)。将“32位无符号乘法把高32位放在EDX”作为**明确的x86指令语义假设**，而不是本站已运行的机器指令，则此例传入__delay的纸面参数为floor((4295×4×lpj×250)/2^32)+1。汇编约束、真实输出寄存器扩展、编译器生成需另证。E055～E058。

| lpj（指定输入） | lpj×250 | 无符号乘积高32位 | ++后传给__delay |
|---:|---:|---:|---:|
| 0 | 0 | 0 | 1 |
| 1 | 250 | 0 | 1 |
| 42000 | 10500000 | 42 | 43 |
| 2500000 | 625000000 | 2500 | 2501 |
| 3000000 | 750000000 | 3000 | 3001 |
| 10371364 | 2592841000 | 10371 | 10372 |

这些是固定小整数普通纸面运算，**不是6次程序或CPU运行**。__delay通过指针delay_fn调用delay_loop。delay_loop输入若为零，test/jz跳到标签3但仍执行末尾dec；非零先jmp到标签1再2，循环dec/jnz，退出后同样末尾dec。asm volatile只有"a"(loops)输入、**没有输出约束或显式clobber**。不得把寄存器最终值称为C变量写回，也不能从循环计数推出实耗微秒。E059～E062。

## C｜HPET对同一比较器的两次写和时间边界

start_kernel在setup_arch与时间初始化后请求late_time_init；后者先hpet_time_init，之后才tsc_init。hpet_time_init先调用hpet_enable(NULL)，后独立调用myos_HPET_init；后者准备IRQ登记，不能与周期函数混同。E024/E025/E073～E075。

hpet_enable在实际经过前置资源/返回门后登记HPET clocksource；只有 (id & HPET_ID_LEGSUP) 成立才直接调用 myos_hpet_clkevt_set_state_periodic(&clocksource_hpet)。后者以channel=0、传入clksrc的mult/shift计算 delta=((u64)(NSEC_PER_SEC/HZ)*mult)>>(shift+7)，先停计数器、读取COUNTER并构造cmp=now+(uint)delta，设置T0 CFG并向CMP写入cmp；两次CMP写之间活动源码确实调用udelay(1)，后面写入(uint)delta并重新启动计数器。邻接旧版本全部注释，不能当成另一条已活动路径。E064～E072。

到达第二次CMP写的必要条件包括：启动链/配置门成立，HPET映射与读取有有效返回，LEGACY门为真，首次写返回，宏和汇编延时链合法且返回，随后第二次写可达。源码**并未认证**设备实际执行两次写入、硬件接受值、精确1μs墙钟间隔、真实CPU频率或IRQ发生。晚期tsc_init即使在其后改全局lpj，对此调用没有逆向时序效果。

## 状态与验收界限

本包另交七张有界事实表、七组纸面模型、DOT静态关系图、六个条件风险与六个证据缺口及79条本次新短引文。旧SC105/HC102发现保留各自ID，仅登记衔接/可能重复，不夸称已经复现。全套P9、编译、预处理、链接、Graphviz、仓库模型/程序、内核、QEMU、CPU实测、设备MMIO和并发均NOT_RUN；现场结果无实际故障认定。

交付终点：READY_FOR_MAINLINE_REVIEW / mainline_acceptance=PENDING；R105-01/02=OPEN（作者D00仅提交待审）；此批后 STOP_WRITING，交由 MYOS2-A-C07 内核分析主线续建 的 Pro 独立验收，不启动第107批。
