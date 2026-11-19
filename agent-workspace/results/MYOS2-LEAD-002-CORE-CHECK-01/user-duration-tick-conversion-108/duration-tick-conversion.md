# 第108批｜普通时长与节拍的双向换算

执行者 MYOS2-A-C07b / MYOS2-LEAD-002-CHAT-EXEC-16；固定time@a039d9803ade；154合同lead@b089a6e48c7b；原执行父988632b3757e。仅阶段2静态源码及有限普通整数计算；源码短锚 DT108-E001～E057 和位置DT108-P01～P24 均见evidence.md，消费者差量及表格见facts-and-boundaries.yaml。

## 结论

固定HZ=1000时，名义一个tick为1ms、1000µs或1000000ns。正常小输入毫秒转tick数值相同；微秒转tick按1000µs为单位向上取整；timespec64前向采用预计算的定点比例并先给纳秒加TICK_NSEC−1，反向再将jiffies乘TICK_NSEC后整除与取余。**上述只说明整数单位和量化，不保证实际等待、时钟精度或调度唤醒。**

## D00：三个仍OPEN的文档纠正

主线已独立关闭R105-01/02与R106-01；R106-02仅余两个直接引用，R107-01和R107-02仍OPEN。旧105/106/107六件、评论和原自检均原位冻结。作者只能标CORRECTION_SUBMITTED_PENDING_C07_REVIEW。

**R106-02-08。** 原107差量中 determine→early_true 只引SC105-E053/E054的频率调整和零值false门、BD106-E034的调用者检查，遗漏被调函数正向返回。现用主线D107-Q1与本批DT108-E056证明 determine_cpu_tsc_frequencies 末尾执行到return true；只有此前未因零频率早退，tsc_early_init的if(!determine(true))才不会返回，继而请求tsc_enable_sched_clock。真返回在被调体内，调用者的门单独定位。硬件校频仍未观察。

**R106-02-16。** 固定linux/kernel/delay.h中的include asm/delay.h（P01）经架构搜索根可定位arch/x86_64/include/asm/delay.h；该文件的include asm-generic/delay.h（P02）经普通include/linux/kernel根可定位include/linux/kernel/asm-generic/delay.h。DT108-E007来自同一个set(INCLUDE_DIRS)的第80–83行，DT108-E008是target_include_directories(kernel PRIVATE)对该变量的真实消费；联原RT107-P09、D107-Q2/Q3和BD106-E027/E063。这是ARCH=x86_64下可解析的候选路径，不认证实际编译器搜索/最终选中的头文件。

**R107-01。** 固定CMake C_FLAGS同一命令中CONFIG_ARCH_HAS_SYSCALL_WRAPPER的强制锚现为DT108-E001，条件导入还须原RT107-P05及P06；不能从文本推断已链接。三处宏定义的真实名称全是__SYSCALL，DT108-E002用于extern、E003用于sys_call_table表项、E004用于switch case。对应include/undef见P05、P06、P07；第三次include位于switch内部，没有虚构第三次#undef。E005给真正数组初始化，E006给真正switch体，原RT107-P01/P02给两个映射实例。t1-03至08、t2-08以及旧图sysmap三条边均分别映射这些证据。

**R107-02。** 原t4-07、t7-05、RT107-M04的归一化调用__iter_div_u64_rem，该函数内部使用u32 ret作为商计数器（E011）并递增、写余数及返回（E012）。设S=原纳秒+nsecs在u64域无回绕，B=10^9（E013），q=floor(S/B)、r=S mod B，则只有q≤UINT32_MAX、最终秒数加q可在time64_t表示，并满足循环/运算前提，才可把源码结果称为完整数学归一化。S=B时q=1、r=0，原M04的(13秒,0ns)成立；数学边界S=B×2^32的商不再可由u32容纳，未运行该路径。真正最终字段写入还须E057，不能只引用源秒初值。

t4-05与旧图to_ns→sample_ns应分成delta>clock->max_cycles的大值门（E018）、大值后两个不同返回（E019）及其余普通乘加返回（E020）。五个旧表消费者追加：t2-06的tz注释块用E014；t2-08的数组和switch用E002～E006；t5-09的UAPI字段初始化用E015；t6-06的实时钟回调成功返回用E009；t6-11的ENOERR=0与put_user的宏体分别用E010/E016。旧图tvnonnull→ktime是条件边，真正函数调用来自gettimeofday；do_gt→putsec/putusec是宏展开，第二次受逻辑或短路控制；最终ts秒与纳秒需要商余路径E011/E012/E057，不能将初值当最终值。以上D00各行记录原固定文件、旧ID、修订证据、前提与止证点，不重做旧报告。

## A：单位与定点常量

time_const.h通过param.h、time_misc_const.h和systick_const.h引入定义；systick.h仍受DEBUG与SYSTICK_DEFINATION源码条件影响，完整预处理未执行（P16～P18）。固定宏HZ=1000、MSEC_PER_SEC=1000L、USEC_PER_SEC=1000000L、NSEC_PER_SEC=1000000000L。TICK_NSEC=(NSEC_PER_SEC+HZ/2)/HZ=1000000ns，在这个HZ下恰整除。E021～E025。

SHIFT_HZ因768≤HZ<1536取10。SEC_JIFFIE_SC初值31−10=21，条件最高位探测在本组整数代入后为0，故源码#undef并重定义为32−10=22；NSEC_JIFFIE_SC=51。两个定点比例按先加分母减一的向上整除宏分别给SEC_CONVERSION=4194304000，NSEC_CONVERSION=2251799814（E026～E031）。不能沿用SEC_JIFFIE_SC最初的21计算。

在显式LP64、long64/ulong64且中间量合法的纸面假设下，LONG_MAX=9223372036854775807，MAX_JIFFY_OFFSET=(LONG_MAX>>1)-1=4611686018427387902，MAX_SEC_IN_JIFFIES通过SH_DIV减一为2199023255（E032～E035/E055）。这些是源码算术限定，并非机器类型已编译验证。

## B：毫秒、微秒与tick

在普通非负小uint域，HZ=1000选择_msecs_to_jiffies的第一分支，计算(m+1-1)/1=m。外层msecs_to_jiffies用__builtin_constant_p分流常量/变量；常量和__msecs_to_jiffies运行期包装均先检查(int)m<0，合法后共用同一helper，所以0ms→0tick、1ms→1tick。uint大值转int可能属于实现定义域，不能冒充普通负输入（E036～E039；P09）。

微秒第一分支为(u+999)/1000，故向上取整的加999发生在前向。usecs_to_jiffies常量路径与__usecs_to_jiffies变量包装先检查u是否超过jiffies_to_usecs(MAX_JIFFY_OFFSET)的实际返回值，然后调用同一_usecs_to_jiffies。999、1000、1001µs分别变为1、1、2tick。源E040～E043；不是无限精度的无界整数式。

jiffies_to_msecs的当前返回表达式为(1000/1000)*j，jiffies_to_usecs为(1000000/1000)*j；两者返回类型均为uint（E044/E046及P11/P12）。对于j=0、1、2，分别得到0/1/2ms和0/1000/2000µs；往返999µs多1µs，1000µs不变，1001µs多999µs。对大的j不能把数学乘积直接当作uint返回。在LP64且uint32的纸面域，jiffies_to_usecs(MAX_JIFFY_OFFSET)的返回uint是4294965296，不是全部的乘积；此前先取这项实际返回再作边界判断非常重要。

## C：timespec64与tick

内核timespec64含time64_t秒与long纳秒（E054）。约定非负秒、0≤纳秒<10^9、有效指针和全部中间整数算式合法。前向timespec64_to_jiffies先将秒读为u64，纳秒先加TICK_NSEC−1；若秒≥MAX_SEC_IN_JIFFIES则钳制秒且清零纳秒。正常未钳制时，按秒乘SEC_CONVERSION、纳秒乘NSEC_CONVERSION后先右移29，再合并并最终右移22，返回ulong的tick数（E049～E051）。这是有界定点源码实现，而非所有输入上的无穷精度向上除法。

反向jiffies_to_timespec64把(u64)jiffies×TICK_NSEC交div_u64_rem，返回u64秒商、通过u32 rem写纳秒，再分别赋到结构字段。函数返回void；需要乘积和输出类型有效（E052/E053、P14/P19）。

| 非负时长输入 | 转换tick | 反向时长 | 向上偏差 |
|---|---:|---|---:|
| 0秒999999ns | 1 | 0秒1000000ns | 1ns |
| 0秒1000000ns | 1 | 原值 | 0 |
| 0秒1000001ns | 2 | 0秒2000000ns | 999999ns |
| 1秒0ns | 1000 | 原值 | 0 |
| 1秒1ns | 1001 | 1秒1000000ns | 999999ns |

所有结果只是有限整数计算；不表示真实内核等待长度或中断次数已被测量。

## 交付边界与状态

七张事实矩阵57行，D00差量2+4+13行，12个纸面样例，DOT80节点99条关系边；evidence有57条固定time短锚和25条普通定位。旧105/106/107文件、评论、PR元数据、内核及脚本均未修改；本轮不进行编译、预处理、链接、仓库模型、Graphviz、QEMU、设备或实际等待，完整本地P9与运行均NOT_RUN。

作者交付终点READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING，D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW，R106-02/R107-01/R107-02由C07决定闭合前保持OPEN。固定六件和本人PR17顶层正式评论全部远端回读后STOP_WRITING，交回MYOS2-A-C07 内核分析主线续建，由Pro独立验收；本执行者不自行启动第109批。
