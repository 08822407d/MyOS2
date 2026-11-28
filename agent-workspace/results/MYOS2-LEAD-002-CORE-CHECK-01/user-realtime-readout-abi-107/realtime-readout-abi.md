# 第107批｜两个墙钟读取接口与用户 ABI

执行身份：MYOS2-A-C07b / MYOS2-LEAD-002-CHAT-EXEC-16；合同 CORE-USER-REALTIME-READOUT-ABI-107。固定源码 time@a039d9803ade；主线合同 lead@120f1597cfdd；执行父11971296e4f3；direct master@e8e60cd19540。仅第2阶段静态源分析及给定普通整数纸面模型，非编译或运行结果。源锚 RT107-E001～E090 见 evidence.md；非函数体定位和宏实例 RT107-P01～P19 单列。

## 结论先行

两个接口都能在所列正常前提下到达内核墙钟读取函数 ktime_get_real_ts64，但**系统调用号、分派路径、UAPI结构和分数单位不同**。gettimeofday 是系统调用96，直接取得时间后将纳秒除以1000写为微秒；clock_gettime(CLOCK_REALTIME) 是228，先经 posix_clocks[0] 的 k_clock 回调再取得时间，并将纳秒数写为用户 timespec。两个不同调用的实际取样时刻可不同；以下数值比较只使用同一份假定、已归一化且字段可表示的墙钟样本。依据E041～E090。

## D00：三个RETURN文档事项的限定纠正

主线有效状态：R105-02已经CLOSED_DOCUMENTATION_ONLY；R105-01仍OPEN两处；R106-01/02均OPEN。第105、106批原文件、评论及原自检计数冻结，本轮只在第107批新目录追加。作者只能登记 CORRECTION_SUBMITTED_PENDING_C07_REVIEW。

**R105-01余两点。** 一是不能再将 cpu_khz/tsc_khz 合写为一条对象定位：固定tsc.c第36行独立定义 cpu_khz（RT107-E001），第38行独立定义 tsc_khz（E002），均为uint。原105的 integer_conversion_and_continuity-09、frequency_source_and_units-14及SC105-F08另合SC105-E053、BD106-E008/E009和新E040，分别判断uint减法、转signed临时量、abs取负和整数乘10的合法域。二是 ns_to_timespec64(s64 nsec) 的真实函数中，ts初始为{0,0}（E003），正分支门nsec>0（E004）、负分支门nsec<0（E005）、负分支体（E006）、末尾return ts（E007）须配齐。零输入时两门均假，在正常C域返回(0,0)。函数签名P07和div_u64_rem的u32*形参P19只作类型定位；E009提供真正被调函数体的商余。不能因为未编译的指针类型差异就宣称实际失败。

**R106-01的25项旧证据窗口。** 原17个混合函数头的ID 011、012、013、016、020、030、032、037、051–055、058、059、064、074，在本次逐项替换为真正函数体内短锚：011→E008，012→E003～E007，013→E009，016→E010，020→E011，030→E012，032→E013，037→E014，051～054→E015～E018，055→E019，058→E020，059→E021/E033/E034，064→E022，074→E023。函数名、签名和返回类型可列普通位置，不能把函数头算作体内引文。旧混合声明ID 007拆两对象E001/E002；027为extern及include位置P09；028把KHZ活动宏E026和DUMMY宏位置P11分开；029把活动HZ宏E027与#undef/注释P12分开；042的四条独立bad函数extern为P10，真实宏体调用证据E029/E030。旧活动/注释混合ID 026、040、041分别由活动初值E024、活动delay_fn赋值E025和注释位置P14/E013纠正。每一行的旧固定路径、原窗口、受影响真实消费者、新锚和原状态均在facts的D00_R106_01中。原106的79/79/0及18份源、79窗口数字留作历史，不混入本包90条新引文。

**R106-02的整数与图表。** __const_udelay必须先按unsigned long类型算出 xloops×4 和 lpj×(HZ/4)，分别取低32位为 a、b。只有在显式假定x86无符号32×32 MUL且结果绑定有效时，高字为floor((a×b)/2^32)，__delay(++xloops)实参是高字＋1。原六个小lpj输入的1、1、43、2501、3001、10372仍成立；但一般式不能直接使用未经低32截取的全部lpj×250。区分例：按64位unsigned long、HZ=1000、lpj=2^31，得到 b=low32(125×2^32)=0、a=17180，高字0、递增后1；无穷精度大乘积代式不正确。E027/E031/E032。

普通C整数前提下(n/20000)>=1与n>=20000等价，负整数时两者都假；后面带ul常数的乘法、转换或回绕必须另设门。活动udelay(1)除以20000商0，不能把泛型bad分支画成这一具体调用可达。旧M05–M08的递增需E032，M09、loop_and_dispatch-04和delayloop→loop_asm同时用E033/E034，不能只引带函数头的旧E059。

旧start→late须由固定105的SC105-E015证明直接调用，SC105-E014和E016证先后，晚期两个同级子调用用E038/E039；原BD106-E024只在late体中不能替start体。旧determine→early_true还须SC105-E053/E054和BD106-E034的返回门。VMware平台对象初绑定、条件覆盖、determine内的实际回调读取分开；局部lpj赋值用BD106-E038，而非只有声明的E037。local_clock应把native结果向外返回，合旧SC105-E100和本包E008，不倒转数据流。HPET活动clksrc->mult/shift读取须E022，计算delta须E037，首次CMP写、udelay(1)、第二次CMP写及重启须E035/E036分别支持；右移shift+7及uint窄化均有合法域和设备门。mdelay注释0x418937只作P15位置，活动0x418938用E028；三层include分别P09定位。原正文“七组纸面模型”应更正为九个M01～M09；七张事实表是另一项。完整19行修订在facts的D00_R106_02，不重画106原图。

## A：两个x86-64普通系统调用入口

固定unistd_64.h给出 __NR_gettimeofday=96（E041）与__NR_clock_gettime=228（E042），asm/syscalls_64.h在P01/P02两个实例中将其接到sys_gettimeofday及sys_clock_gettime。entry/syscall_64.c对同一清单先展开 extern __x64_*声明，再生成sys_call_table数组条目，最后在x64_sys_call的switch里展开case。E043～E046。**声明、静态表项和活动switch是三种用途，不能因为表存在就声称此次运行经表分派。**

MYOS_SYSCALL_DEFINE2通过__MYOS_SYSCALL_DEFINEx生成 x64 stub、se包装和do主体；E047～E056列出宏体。__X86_64_MAP2按两个形参从regs->di、regs->si提取并经__SC_CAST转换。给定真实编译选择、参数合法和目标函数链接前提，96进入gettimeofday(tv:__kernel_old_timeval*,tz:timezone*)；228进入clock_gettime(which_clock:clockid_t,tp:__kernel_timespec*)。具体宏实例与形参为P03/P04，不当作函数体P2锚。返回值宏源级为long；没有实际系统调用/ELF运行证据，不延伸到libc或入口汇编。

## B：同一类墙钟读取的两种直达路径

gettimeofday在tv非NULL时直接调用ktime_get_real_ts64(&ts)，E057；clock_gettime先调用clockid_to_kclock(which_clock)，若kc为NULL返回-EINVAL。固定CLOCK_REALTIME=0（E064），posix_clocks[0]=&clock_realtime（E067），该对象的clock_get_timespec字段绑定posix_get_realtime_timespec（E066），字段类型是接受clockid、timespec64*并返回int（E070）。查询返回数组项后，由clock_gettime通过字段间接调用回调；回调真正直接调用ktime_get_real_ts64(tp)并返回0。E060/E061/E065/E068/E069。数组索引、回调字段写入、间接调用、直接调用分别归属真实函数，不承诺运行时已绑定或已到达。

共同读取体在固定timekeeping.c中使用tk_core.timekeeper，取xtime_sec及timekeeping_get_ns(&tkr_mono)，通过tk_clock_read→clock->read(clock)及timekeeping_cycles_to_ns计算纳秒增量，然后将tv_nsec设0并调用timespec64_add_ns，将每满10^9纳秒的商计入秒、余数写纳秒字段。E071～E080。实际归一化函数位于固定time的mykernel/time/misc/time_misc.h，迭代除法辅助体位于固定time的mykernel/include/linux/kernel/math64.h；不能改用默认分支同名文件。前提为选定clocksource有效、指针/shift/算术合法且回调返回。该读函数中seqcount retry原文处于注释，不宣称并发一致快照；实际频率、设备读数和两个系统调用的时间差均未实测。

## C：两个用户结构、字段写入与舍弃量

gettimeofday：tv为NULL时活动体不读取或写tv；tv可写且非NULL时，先put_user(ts.tv_sec,&tv->tv_sec)，再put_user(ts.tv_nsec/1000,&tv->tv_usec)，出错请求-EFAULT，正常走到return0。tz的写入分支已经注释，不能说本函数填充tz。E057～E059。固定x86 put_user宏目前是直接store后返回-ENOERR（固定errno-base定义ENOERR=0）；因此在合法指针和正常存储前提下可按0建模，**不证明任意坏指针都会安全返回EFAULT**。E088及已独立闭合的63/64用户复制边界仍有效。

clock_gettime：kc非NULL且callback返回0时，调用put_timespec64，按源级局部kts逐字段赋值tv_sec/tv_nsec，随后请求copy_to_user(uts,&kts,sizeof(kts))。复制残量非0则返回-EFAULT，成功0，调用者最后返回error。E060～E063/E086/E087/E089/E090。copy_to_user到access_ok/raw_copy的下探只支持条件性数据复制请求，不作CPU/设备或真实虚拟地址验证。

内核timespec64_s有time64_t秒、long纳秒（E081）；旧用户__kernel_old_timeval有两个__kernel_long_t（long）字段tv_sec和tv_usec（E082/E084）；用户__kernel_timespec有__kernel_time64_t秒（long long）、long long纳秒（E083/E085）。按64位long、long long均8字节、8字节自然对齐且无额外填充的明确条件，两种UAPI输出结构各16字节；不是在此运行sizeof确认。

给同一归一化、可表示的假定样本秒=12，纳秒为999、1000、999999999，gettimeofday的(sec,usec)依次是(12,0)、(12,1)、(12,999999)，clock_gettime的(sec,nsec)依次是(12,999)、(12,1000)、(12,999999999)。前者的整数除1000对正常非负纳秒舍弃0～999ns，不四舍五入。若公共归一化合法收到12秒及新加10^9纳秒，则成13秒0纳秒。输出字段能表示纳秒不等于真实时钟具有1ns准确度；两次独立采样不保证完全一致。

## 交付边界

facts包含三项D00差量、七张有界矩阵63行和9项模型；DOT 58节点75有类型边、无未声明端点；deferred保留旧根并登记6条件候选/6证据缺口；evidence有90条连续1～5行新引文和19条签名/宏实例等普通位置。全批只读源码，未改任何旧105/106件、内核、PR元数据、lead、loader或脚本。完整本地P9、预处理、编译、链接、仓库程序、Graphviz、内核/QEMU、真实用户内存和设备均NOT_RUN。

作者终态 READY_FOR_MAINLINE_REVIEW / mainline_acceptance=PENDING / D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW；R105-01/R106-01/R106-02继续OPEN，R105-02既有文档闭合。六件远端发布并回读后本16立即STOP_WRITING，交 MYOS2-A-C07 内核分析主线续建 使用Pro独立验收，第108批不由本对话自行启动。
