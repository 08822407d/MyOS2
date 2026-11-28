# 第109批｜正有限超时如何进入时间轮及更新到期记录

执行身份：MYOS2-A-C07b / MYOS2-LEAD-002-CHAT-EXEC-16。155号合同 CORE-USER-TIMER-WHEEL-PLACEMENT-109，固定time@a039d9803ade，lead@66576d5b78eb，执行父a8436da2c2b3，direct master@e8e60cd19540。只进行阶段2源码静态分析、文本比对和普通整数纸面计算，未执行仓库代码或真实内核。新证据TW109-E001～E067、位置TW109-P01～P24列在evidence.md；历史50/52/108证据仅按其有效主线核收边界消费。

## 核心结论

给定正且有限、正常可表示的long timeout，固定schedule_timeout先令ulong expire = timeout + jiffies；使用timer_setup_on_stack后向__mod_timer传入该绝对expire和MOD_TIMER_NOTPENDING。正常有效function、base、锁、迁移和前置门成立时，__mod_timer最初的idx仍为UINT_MAX，先写timer->expires，再走internal_add_timer。它用timer->expires和base->clk计算相对delta选轮级，随后**用绝对expires**经calc_index在选中的层中计算bucket_expiry、层内slot及全轮idx。入桶源级顺序是hlist_add_head写链，再timer_set_idx写flags中高位桶索引，最后比较bucket_expiry和旧next_expiry；只有更早才写base->next_expiry、timers_pending及next_expiry_recalc。以上不证明回调运行、实际硬件中断、任务睡眠、CPU唤醒或定时器持续保留在桶里。

## D00｜只追加R108-01及R108-02

C07有效历史状态：R106-02和R107-01/02均CLOSED_DOCUMENTATION_ONLY，106/107组合PASS_PENDING_LOCAL；第108批RETURN，R108-01/02仍OPEN。本批旧文件与评论不改，所有补充标CORRECTION_SUBMITTED_PENDING_C07_REVIEW。facts的D00_targeted_correction_entries包含25条定点纠正，每条列冻结原文件、旧实际表行或边、源证据、前提与首个止证点。

### R108-01：两个不同的预处理门、单位及定位

固定108的A04、S01、M01和hertz→shiftHz / secshiftgate→secshift曾混合了两种预处理条件。systick_const.h的第28行条件768≤HZ<1536选SHIFT_HZ=10，第29行才是独立宏定义（旧DT108-P25/E026）；SEC_JIFFIE_SC却先由旧E027定义为31−10=21，再由第157行的独立高位探测（旧DT108-P15）决定是否#undef/重定义为22（旧E028），NSEC_JIFFIE_SC=51由旧E029独立定义。固定HZ1000的探测数：((1000000000<<2)/1000000) <<19 = 2,097,152,000，即0x7d000000；与0x80000000按位与为0，故选择22。M01旧证据把E026错列为28–30行，真实原窗只有29行；补E029以及两个比例的旧E030/E031，不重写原件。

TICK_NSEC原始未加偏置的1000000000/1000是商1,000,000、余0；真正源码有HZ/2=500偏置，1000000500/1000仍商1,000,000但余500。因此在此HZ下**最后商数精确为百万并不表示带偏置的分子恰好整除**，见旧A02、本批TW109-E056及主线RV108-Q1。

旧U04所写的大数单位“毫秒”应为**微秒**；旧R08和DT108-M12同样必须按jiffies_to_usecs返回uint的类型和旧DT108-P12登记。旧M07的毫秒往返需要msecs_to_jiffies的(int)m非负门，含常量与变量路径（旧DT108-E037/E039）；旧U05/U06/U07往返的反向结果还须引用jiffies_to_usecs实际返回表达式旧DT108-E046和类型P12。正常999/1000/1001微秒返回1000/1000/2000微秒的数值不变。

原108 MANIFEST inputs_read里仍写24条普通位置，P9-3原列表止于P24；原108正文开头也止于P24。但原evidence实际P01–P25，共25条。新增账逐一列这三处，冻结原57引文、12个模型及历史自检，不回写旧MANIFEST或正文。

### R108-02：修正文档关系边和跨文件消费者

- 108旧DOT的putsec→putus标签退化为“CONDITIONAL_STEP: C []”。实际在gettimeofday的逻辑或表达式中，第一put_user正常store后以-ENOERR=0返回，左操作数为false才求值第二put_user；若左项非零则短路。依据主线RV108-Q5、旧DT108-E010/E016/E017及本批TW109-E059。两者是宏展开与顺序条件，不是两个普通直接C函数调用。
- 原archflag→declmacro端点错误：CONFIG_ARCH_HAS_SYSCALL_WRAPPER控制syscalls.h的asm/syscall_wrapper.h条件include；syscall_64.c第一条__SYSCALL属于另一实际源定义。新图按配置门→条件头→架构包装头；独立的第三条__SYSCALL产生case须结合旧DT108-E004、E006、P03/P04两个清单实例，而非以E006独证宏替换关系。
- 108旧delta节点的来源是timekeeping_cycles_to_ns中的(cycles−tkr->cycle_last)&tkr->mask（主线RV108-Q6、本批TW109-E060），不是本批calc_wheel_index中expires−base->clk（E016）。两种delta独立命名，不能借同名当作同一对象。
- 108旧addns→iterdiv写为“multiply time unit division helper”，真正调用是timespec64_add_ns中的__iter_div_u64_rem（本批E061）；iterdiv内u32 ret声明E062、递增/return E063，结果再进入秒加商、纳秒余数写回。要把u32商当完整数学商，仍受主线已核的商宽和秒表示门。
- 冻结108 facts中的R107-01混合消费者、R107-02-10至13，以真实文件类别拆分：107 evidence承载RT107-E/P，107 facts承载t1/t2/t4/t6表及模型，107 DOT承载旧边，107 MANIFEST承载配置读取申报。四条以DOT为消费者的记录不能全部把old_fixed_file指成facts。各真实固定路径和ID逐项列在D00差量附表，旧107/108所有件原位保全。

## A｜从相对timeout到绝对expires、由delta选层

固定time中schedule_timeout对MAX_SCHEDULE_TIMEOUT有专门case，负值也有早退；本批只跟合法正有限值。有效前提下调用时采样jiffies并形成expire=timeout+jiffies，setup栈timer，传给__mod_timer(..., MOD_TIMER_NOTPENDING)。E003–E009/E064。__mod_timer进入正常base锁定、callback非NULL、forward_timer_base与detach前置门；idx初始UINT_MAX，因此不走“已有预计算idx且clk未变化”直接enqueue分支，而是写timer->expires=expires后调用internal_add_timer（E007/E010–E015）。base->clk可由forward_timer_base另行推进，**不能自动假定它等于刚读的jiffies**；本批数值明确把两者都指定1000。

calc_wheel_index实际令ulong delta=expires−clk，按小于LVL_START(n)的严格门依次选层。常量LVL_CLK_SHIFT=3，LVL_BITS=6，因此每层64桶，LVL_SHIFT(level)=3*level，粒度=2^(3*level)tick，LVL_OFFS(level)=64*level。HZ=1000选LVL_DEPTH=9，WHEEL_SIZE=576。首两个门LVL_START(1)=63、LVL_START(2)=504；后续起点为4032、32256、258048、2064384、16515072、132120576。最高截断域有WHEEL_TIMEOUT_CUTOFF=1056964608、WHEEL_TIMEOUT_MAX=1040187392，另有负delta转long判定。这里只按没有回绕的小正值路径运行纸面模型，不以最大值路径套普通输入。证据E016–E034与P08。

## B｜绝对expires如何变成桶到期值与idx

calc_index用的自变量是选定层级lvl和**绝对expires**，不是delta。固定体内三步：

1. q = (expires >> LVL_SHIFT(lvl)) + 1。
2. bucket_expiry = q << LVL_SHIFT(lvl)。
3. slot = q & LVL_MASK；idx = LVL_OFFS(lvl) + slot。

这里先移位、再明确加1，因此即使expires正好对齐某层粒度，下一有效bucket仍会顺延一个粒度（E035）。数学公式仅适用于ulong移位与加法、数组idx和源对象均合法的范围。

| jiffies=base.clk | 相对timeout | expires | delta | level / 粒度 | bucket_expiry | 层内slot | 全数组idx | 有效到期比请求晚 |
|---:|---:|---:|---:|---|---:|---:|---:|---:|
| 1000 | 62 | 1062 | 62 | 0 / 1 | 1063 | 39 | 39 | 1 tick |
| 1000 | 63 | 1063 | 63 | 1 / 8 | 1064 | 5 | 69 | 1 tick |
| 1000 | 64 | 1064 | 64 | 1 / 8 | 1072 | 6 | 70 | 8 ticks |

其中delta=62满足<63、仍在level0；delta=63已到level1；绝对expires=1064恰对齐8却仍加8。若同时把jiffies、base.clk改为1001，保持timeout=63，则expires=1064、level仍为1，bucket=1072、slot=6、idx=70，而基准1000时idx=69；**相同相对距离不保证相同绝对桶号**。以上从固定公式推出，不代表某台机器实际入桶。详见facts中6个样本与12个有限模型。

## C｜桶头、标志位与最近到期三字段

internal_add_timer先获得idx/bucket_expiry，再直接调用enqueue_timer。后者首先调用hlist_add_head(&timer->entry,base->vectors+idx)；固定hash_list.h将hlist_add_head定义为hlist_add_to_head别名，其体写node.next、可能的旧首节点pprev、head.first及node.pprev。**节点是timer->entry，桶头是base->vectors[idx]**；不是timer->expires，也不分配新的桶。之后调用timer_set_idx(timer,idx)写timer.flags；中间的pending_map置位是注释，不可当成活动bitmap操作（E036/E048–E052）。

TIMER_ARRAYSHIFT=22，TIMER_ARRAYMASK=0xFFC00000；timer_set_idx保留旧flags的低22位并写入idx<<22，timer_get_idx反向取掩码再右移。WHEEL_SIZE=576保证有效idx能放在高10位，具体flags原值与对象布局仍有前提。示例old_flags=0x00100002、idx=69：新flags=0x11500002；解码得69，低位的0x00100002保留。该示例不声称真实timer带这些标志（E042–E045）。

无论是否成为新的最近到期，hlist入链及idx写入均位于比较之前。之后enqueue_timer实际判断time_before(bucket_expiry,base->next_expiry)。在小距离且unsigned long与signed long宽度相容的域，time_before(a,b)经time_after(b,a)使用有符号差判断a严格早于b。用bucket_expiry=1064、idx69的同一例：

| 旧next_expiry | 比较 | 已完成的动作 | 对base三个字段的本体写入 |
|---:|---|---|---|
| 1065 | 1064<1065为真 | 已入桶，已编码flags | next_expiry=1064，timers_pending=true，next_expiry_recalc=false，并调用trigger_dyntick_cpu |
| 1064 | 相等为假 | 同样已入桶和写flags | 此分支不写三个字段 |
| 1060 | 当前bucket更晚为假 | 同样已入桶和写flags | 此分支不写三个字段 |

条件真分支写的顺序为WRITE_ONCE(next_expiry)、pending=true、recalc=false、调用trigger。条件假不意味着此前pending一定为false，只能说该函数本体这次没有更新。所读trigger_dyntick_cpu定义中的IPI和唤醒语句全是注释；调用发生并不证明IPI、CPU唤醒或定时器触发。timer.c的schedule_timeout在登记后还请求del_timer_sync，而且直接schedule调用处于注释；本批止于登记和条件更新，不沿着删除、到期、回调、调度或用户可见时间继续签结论（E036–E038、E046/E047、P06/P21）。

## 验收边界与交回

本包facts的D00差量25条、六张有界表50行、12个纸面模型；DOT将D00旧边与新时间轮主链共同画为72节点85条有类型边；deferred保留50/52/108旧根号、增加条件候选与证据缺口；evidence为67条固定time源码1–5连续行及24个POSITION_ONLY。没有更改任何内核、旧结果、任务书、PR导航、分支或脚本。完整本地P9、编译、链接、仓库代码/模型、Graphviz、QEMU、内核、硬件和并发全部NOT_RUN，所有纯整数样例不冒充真实测试。

作者终点READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW，R108-01和R108-02仍OPEN，等待C07 Pro独立裁定。正式六件与作者顶层评论全部完成远端回读后立刻STOP_WRITING，不自启110批。
