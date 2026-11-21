# 第110批｜基准选择、clock推进和已登记timer的同桶更新

执行者：MYOS2-A-C07b / MYOS2-LEAD-002-CHAT-EXEC-16；156合同 CORE-USER-TIMER-BASE-FORWARDING-110。固定time@a039d9803ade、主线lead@c06abde41e91、执行父3b2efdee5d85、direct master@e8e60cd19540。只读阶段2源码及有界普通整数纸面代入；新锚TB110-E001～E074及普通位置P01～P26见evidence.md。旧批次文件及评论一字不改。

## 结论先行

固定time的timer_base并非由“一个全局base”直接取得。lock_timer_base取timer->flags快照，若未在TIMER_MIGRATING状态则按CPU掩码和TIMER_PINNED选择候选，然后请求自旋锁，并在flags仍等于快照时才返回。forward_timer_base再读取一份jiffies快照，它与之前schedule_timeout算expires时读到的jiffies不能自动视为相同。时钟推进先有“不应回退”的返回门，然后在旧next_expiry与新样本之间选一个值；旧next_expiry早于旧clk还会在选中的WARN_ON_ONCE宏域提前返回。已登记timer的__mod_timer可以仅改timer->expires而不摘链；桶号不同才进入摘链/入链流程，预计算下标的重用取决于idx有效及**数值**clk未改变。

## D00｜R108-02、R109-01、R109-02三个开放事项

C07已经将R108-01文档闭合，R108-02继续OPEN；109的A/B/C普通路径及6个数值样本获源级核收，但R109-01/02仍OPEN。本批28条新增定点记录（R108-02七条、R109-01十八条、R109-02三条）均为CORRECTION_SUBMITTED_PENDING_C07_REVIEW。完整旧消费文件、ID、源窗与待证条件见facts-and-boundaries.yaml。

**R108-02：真正修正新图与证据。** 原109图第155行的firstok→putus只写CONDITIONAL_STEP: C []，新图明确写入“第一次 put_user 正常返回0才求值第二项 [TW109-E059,DT108-E016,DT108-E010]”；前提是tv非NULL且第一put_user访问成功。gettimeofday→第二宏另标为词法使用，不等于不经条件的实际执行（新E071～E073）。109旧配置边误引用TW109-E002（HZ），本次改为实际包装选项新E001/旧TW109-E001、syscalls.h的93–102行正向头文件包含；同文件314–317行另有反向原型门，因此不再声称“只影响此处”。独立的__SYSCALL源宏及switch实例仍看DT108-E004/E006、P03/P04，不把配置门连成__SYSCALL的定义者。

109追加LOC-R107-02-12沿用的iterdiv→outputs只是检索简称。固定107图的四个真实边分别为iterdiv→sample_sec（108行）、iterdiv→sample_nsec（109行）、ktimeread→sample_sec（110行）、ktimeread→sample_nsec（111行）；新图以HISTORICAL_EDGE_ENDPOINT保留精确两端及旧证据号，不将原来证据不足的最终字段流量自动升级为运行结论。timekeeping的掩码delta与时间轮expires−clk各属不同对象；timespec64_add_ns的实际辅助调用及u32商余已有独立109与108证据，本轮不重新打开已核算部分。

**R109-01：让旧表/模型真正指回源码。** 109的A08和X08必须读schedule_timeout源码的active __mod_timer、注释掉的schedule及后续del_timer_sync（E070、RV109-Q1），而不只读登记前半段。N06条件假需TW109-E037/E038合读，N07关于trigger无活动IPI需109 P06的真实函数体范围。G03用固定HZ、timer_const.h独立#if HZ>100位置P05和实际LVL_DEPTH9；G07的两个高层门分属TW109-E019/E020。X04与TW109-M01至M07按期限来源、严格选层、移位粒度、独立calc_index的bucket_expiry和返回idx逐项补证，旧数值不改。C03辨明timer_set_idx是内联函数，而ARRAYMASK/ARRAYSHIFT是两个宏。N08有效bucket1072的值必须配calc_index和层级粒度，不能只由下游比较断言。109新增旧R108-01-F将毫秒返回签名误指DT108-P12；正确为jiffies_to_msecs的P11/E044，微秒才是P12/E046。旧TW109-F/U身份保持历史原位，不冒充新发现。

**R109-02：调用主体与作用域。** 109旧图base_lock→base_forward误写DIRECT_CALL。实际调用者__mod_timer在已锁定且callback非NULL后分别从pending分支255行或正常分支289行请求forward_timer_base；lock_timer_base只选择并返回基准，二者在调用方之间属于源码先后，不是lock函数体内的调用。trigger_dyntick_cpu由enqueue_timer严格较早的bucket分支调用，而trigger节点→自身函数体仅是作用域/注释关联，不是递归或新调用。HZ→depth须补timer_const.h第114行的独立预处理门P05，再连活动宏TW109-E031。新DOT对此分别标DIRECT_CALL、SOURCE_ORDER、ANNOTATION和CONDITIONAL_SOURCE。

## A｜从flags到候选timer_base，锁后复核

已读的四种重要位域：TIMER_CPUMASK=0x0003FFFF用于取得低18位CPU号；TIMER_MIGRATING=0x00040000使取锁函数先避开该轮选择并cpu_relax重试；TIMER_PINNED=0x00100000决定基准索引在BASE_LOCAL和BASE_GLOBAL之间切换；TIMER_BASEMASK组合CPU与迁移两类位，供迁移后改写。get_timer_base(tflags)将(tflags & TIMER_CPUMASK)作为cpu交给get_timer_cpu_base；显式CPU版本可传另一个cpu；get_timer_this_cpu_base不从低位字段提CPU，而请求this_cpu_ptr取当前CPU候选（E004～E012）。两个选择函数的BASE_DEF分支在所读正文中为注释，不拿它当活动路径。

普通标志例：flags=0x00000005时，CPU掩码得5、PINNED为0，因此候选per_cpu_ptr(&timer_bases[BASE_GLOBAL],5)；增加PINNED后flags=0x00100005，CPU仍为5但候选变per_cpu_ptr(&timer_bases[BASE_LOCAL],5)。若显式传cpu7则用7，而当前CPU版按this_cpu_ptr所隐含的CPU执行。timer_api.h的timer_bases[NR_BASES]只是extern声明（P01），此范围未找到数值化BASE_GLOBAL/BASE_LOCAL及对象定义，不编造数组元素数或具体地址；可声明的只是符号化候选选择表达式。

lock_timer_base通过READ_ONCE(timer->flags)取tf，排除tf中的MIGRATING后调用get_timer_base(tf)、请求spin_lock_irqsave。只有timer->flags==tf才return base；否则unlock并cpu_relax重试（E013～E016）。源码可描述调用与检查，不认证并发下的原子性或真实自旋锁时序。__mod_timer之后另选当前CPU基准；若指针不等、并且base->running_timer!=timer，先置MIGRATING，再unlock旧锁、换base指针并锁新基准，WRITE_ONCE重写base相关CPU位，然后forward新的base（E017～E020）。是否发生迁移与CPU地址仍为未证。

## B｜从READ_ONCE(jiffies)决定clk是否改变

forward_timer_base(base)以READ_ONCE(jiffies)为独立basej样本，再调用__forward_timer_base(base,basej)。time_before_eq、time_after、time_before引用time_after_eq及long差符号测试，以下全部限定小、不跨wrap、long/ulong比较表示成立（E023～E031）。

真实代码顺序：若basej早于或等于旧base->clk，立刻return且没有clk写入。否则若旧next_expiry晚于basej，写base->clk=basej。否则进入else，如果WARN_ON_ONCE(time_before(next_expiry,旧clk))为真则提前return；否则写base->clk=next_expiry。固定CMake含CONFIG_BUG，bug.h当前被选的WARN_ON_ONCE定义是(condition)，不是旧注释的告警计数实现（E002/E032，P06/P07），因此本次普通有界模型按返回布尔条件解释。

旧clk均为1000时：

| 新basej | 旧next_expiry | 源码分支 | 是否写clk | 结果clk |
|---:|---:|---|---|---:|
| 999 | 1030 | basej不晚于clk | 否 | 1000 |
| 1000 | 1010 | 相等提前返回 | 否 | 1000 |
| 1010 | 999 | 警告条件为真并return | 否 | 1000 |
| 1010 | 1000 | 警告为假，else赋值 | 是，写同值 | 1000 |
| 1010 | 1005 | else赋next_expiry | 是 | 1005 |
| 1010 | 1010 | else赋next_expiry | 是 | 1010 |
| 1010 | 1020 | next晚于basej | 是，赋basej | 1010 |

只有在basej>旧clk、且next_expiry>=旧clk等限定条件下，才能简写为把clk设成min(basej,next_expiry)，还必须保留“可能写入同数值”与“上游提前返回”的区分。这里的basej是推进函数读的jiffies，不是schedule_timeout早先用来形成expires的同一次采样。

## C｜已登记timer的同桶更新、复用预计算桶号与重算

三种公开接口分别为mod_timer_pending(timer,expires)→__mod_timer(...,MOD_TIMER_PENDING_ONLY)，mod_timer→options=0，timer_reduce→MOD_TIMER_REDUCE（E035～E037/E055～E057）。原timer_pending通过entry的hlist哈希状态取得（E038），用户案例限定pending为真、timer.function非NULL、锁和选基准有效。当前__mod_timer在未设NOTPENDING且pending为真时先做无锁短门：新旧expires相同则ret1立即返回；REDUCE且新期限不早于旧期限也ret1。其ret1只表示已有pending对象被保留或处理，不能都解释成“摘链重入桶”（E039～E041）。

若继续，取基准锁并forward；再次检查function，REDUCE还可能在锁后因time_before_eq(旧,新)跳出（E042～E044）。然后保存clk=base->clk、计算新idx和bucket_expiry、与timer_get_idx(timer)的旧桶号比对。如果idx相同，普通mod_timer只改timer->expires，REDUCE也只有time_after(旧,新)才改；两者ret1并goto解锁，无detach也无enqueue（E045～E047/E061）。本批固定clk1000的源码模型中：expires1064和1065都落在level1、idx70、bucket1072；expires1063落idx69、bucket1064。因而pending old1064→new1065的普通mod_timer只更新expires并返回1；old1064→new1063跨桶，要进一步摘链并重新入桶。old1064→new1065用timer_reduce会在早期REDUCE门拒绝延长，返回1但未写expires。old1065→new1064用REDUCE则在有效前提下同桶、将expires减小并返回1。

新旧idx不同后，通过detach_if_pending取ret：不pending直接return0，pending在旧桶为单元素时还可能令next_expiry_recalc=true，然后detach并return1。若ret=0且options含PENDING_ONLY，__mod_timer提前解锁返回0，**不会写timer->expires**；但此前选择基准及forward仍可能对base->clk有源级作用，不说“所有对象完全没改”（E050/E058～E060）。

一般后路选get_timer_this_cpu_base(timer->flags)，必要时迁移；随后总是先写timer->expires=新值，再测试idx!=UINT_MAX且初算clk==最终base->clk。**相等只是数值测试，不能由源码推出base指针一定同一对象**。若相等，调用enqueue_timer直接重用预计算idx/bucket_expiry；否则进入internal_add_timer按最终基准重算（E051–E053）。pending old1063→new1064、同CPU同base且clk始终1000，旧idx69、新idx70，detach ret1后可直接复用idx70及bucket1072入桶。若迁移或clock推进使最终base.clk不同，便走internal_add_timer重算；真实CPU交错与运行结果不在此合同中验证。

旧bucket上的next_expiry_recalc和新bucket的最近期限更新是另一个条件层，不能因返回1就宣称它们必有同一值。旧109与本110的结论只在明确给定timer状态、合法hlist、锁以及桶索引范围内成立。

## 交付与审查边界

facts记录28项D00差量、五张有界事实表47行、12个有限纸面模型，原109全部12模型与67原锚冻结；新DOT共92节点121条有类型边，特别把D00第二次put_user完整条件和107真实四组端点写明；deferred保留TW109-U01及既有各根，登记本批条件候选与证据缺口；evidence为74条来自12份固定time源码的连续1–5行短引文及26条单独位置记录。未改内核、旧文件、任务书、PR元数据或评论。

所有结果仅源级和独立整数推导；完整本地P9、仓库程序/脚本、C/ASM编译和预处理、链接、Graphviz、内核、QEMU、并发、CPU及物理硬件均NOT_RUN。第110批作者状态READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW，R108-02/R109-01/R109-02仍OPEN待C07 Pro审定。六件及唯一PR17正式评论完整远端回读后本人STOP_WRITING，不自行开始111。
