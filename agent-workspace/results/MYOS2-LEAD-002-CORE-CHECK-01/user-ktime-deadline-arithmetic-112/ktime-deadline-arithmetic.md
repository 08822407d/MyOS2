# 第112批｜ktime 期限的表示、加法和反向转换

MYOS2-A-C07c / MYOS2-LEAD-002-CHAT-EXEC-17，合同 CORE-USER-KTIME-DEADLINE-ARITHMETIC-112。固定代码time@a039d9803ade，执行唯一父ab6c14299a76，主线160合同lead@92d7b5af084f。阶段2仅用 GitHub 实读源码、静态推导与有限整数核对。完整本地P9、编译、QEMU/内核、实机、并发、Graphviz 全部 NOT_RUN。

## D00：R110-01、R110-02、R111-01、R111-02

### R110-01：按原字段重映射223项

固定第110批事实中有87条记录、375个原位置，71条记录的223个无效LNaN窗口由第111批留下74条字典、71行序列和223条索引。此次不凭第111批修订文本重复信任：先在原110 JSON 中由 section 与 record_id 获取真正记录，再按原字段及0起位置序号取 source_ids，最后同位匹配111的字典、path 和固定 time 真实源码窗口。结果：**71/71条原字段存在，223/223处原同位证据编号正确，223/223个原路径对应，历史74/74个原引文逐字匹配**。

正确字段覆盖为D00的13条/23项用source_locations，不需改；base_selection 8条/26项、forwarding_branch 10条/38项、pending_and_options 10条/31项、bucket_reuse_and_recalc 10条/35项、cross_checks 8条/33项、models 12条/37项均用**source_paths**。即需纠正后六分区58条记录的200个字段标签，而保留全部序号、ID、整数窗口和历史正文。详见facts的D00.R110_01.rows_71：71行给出原字段、旧111错误字段、同位ID及序号，利用冻结223条和74字典可展开，不复制历史数据。

首例：D00/108A/**source_locations[0]** → TB110-E071 → mykernel/time/syscall.c:19–22。末例：models/TB110-M12/**source_paths[3]** → TB110-E041 → mykernel/time/timer/timer.c:237–238。同为296起点的TB110-E017只占296–296，TB110-E051则占296–298，不能以相同起点合并。旧110、111和早期所有成果均冻结。

### R110-02：O11包装来源及O12哨兵身份

O11的三个独立公开入口：mod_timer_pending 在 timer.c:358 以 MOD_TIMER_PENDING_ONLY 调用 __mod_timer（TB110-E055；新KD112-E006）；mod_timer 在:391以零选项调用（TB110-E056；新KD112-E007）；timer_reduce 在:417以 MOD_TIMER_REDUCE调用（TB110-E057；新KD112-E008）。图的三个DIRECT_CALL是三条独立有据的边，并与其余 NOTPENDING 包装区分。

O12的真正哨兵初值是 __mod_timer:217 的 `uint idx = UINT_MAX;`（旧AT111-E027、TW109-E007；新KD112-E004）。原111误引用的TB110-E027实际位于 timer.h:155–156，是 __forward_timer_base 发现basej过早时直接返回的门。TB110-E053才指向 __mod_timer:328–331的 `idx!=UINT_MAX && clk==base->clk`，决定复用已有桶号或通过internal_add_timer重新计算（新KD112-E005）。原O01正文53行与B10字段分别留作消费者记录；O01–O10既有审核已接收部分不重开。

### R111-01：旧短引文的函数签名交叉问题

冻结AT111-E017来自hash_list.h:109–110，109为hlist_unhashed_lockless函数签名/开括号，110才是 `return !READ_ONCE(h->pprev);`。旧54条原引文的文本54/54匹配，但完整处在定义体内的只有53段。本批新KD112-E002单独取110行，仅将这一条补为合格短引文，函数签名用KD112-P13普通位置。初始化A07、当前CPU B05、M08、旧图pprev→unhashed与pending→initPending及其他AT111-E017直接消费者在本差量中以KD112-E002替代；pending外层取反另据KD112-E003。旧54/53历史计数不改。

### R111-02：按消费者补路径、时点和宏边

M05–M07原把显式CPU选择AT111-E032错用作当前CPU路径；实际新基准候选是 __mod_timer:296 AT111-E049→get_timer_this_cpu_base:131–132 AT111-E033→return base:139 TB110-E011（新KD112-E009/E010/E011）。旧候选的lock_timer_base读取旧flags经AT111-E035/E036→get_timer_base AT111-E034（新KD112-E013/E012），与当前CPU新候选以及add_timer_on显式cpu→get_timer_cpu_base AT111-E032（新KD112-E014）分账，所有候选以锁、回调与有效对象等条件为前提。

B08的三个void返回由AT111-P02–P04及KD112-P09完整体支持，C07 add_timer_on 的void由AT111-P05及KD112-P10。C08的“函数本体没有写expires”须看495–539完整函数（KD112-P10），真正下层读取timer->expires在internal_add_timer:149–151（AT111-E031/KD112-E021）。C10/X08的base==new_base仅跳过MIGRATING/锁切换段，仍走E045推进、E046入桶和E047公共解锁。X06异基准需补E042–E046，X07空回调需补E047。原P10、P15、P16整体定义到闭括号的末行分别应为480、126、140（KD112-P09/P11/P12）。逐消费映射见facts。

CPU7有界模型：假定timer旧CPU5、初始flags0x00000005、非pending、有效callback、显式目标CPU7且基准指针不同，置PINNED后0x00100005，add_timer_on:529–530通过显式cpu写回**0x00100007**。若此后forward后的base->clk=1000且timer->expires已经是1064，则delta64，LVL_START(1)=63，层1，(1064>>3)+1=134，bucket_expiry1072，idx=64+(134&63)=**70**。enqueue_timer调用timer_set_idx写入桶号，得到 `(0x00100007 & ~0xFFC00000)|(70<<22)` = **0x11900007**。这才是该完整路径的最后旗标，不可把0x00100007称为“最终”。根据KD112-E019/E020，历史图145行 `on→onUnlock` 应从错误的DIRECT_CALL改为add_timer_on宏使用MACRO_USE以及到raw_spin_unlock_irqrestore(lock,&flags)的MACRO_EXPANSION，两条边均已在新图标明。

以上仅是作者文档纠正提交，**R110-01/02、R111-01/02继续OPEN，等待C07 Pro独立核收**。第109批R109-01/02已文档闭合但组合仅PASS_PENDING_LOCAL；第110、111批仍RETURN，不改任何历史判词。

## A：timespec64 输入域、秒门及单位

ktime_t为s64（KD112-P01）；timespec64结构的tv_sec为time64_t、tv_nsec为long（KD112-P02）。NSEC_PER_SEC=1,000,000,000（KD112-E033）。KTIME_MAX=9223372036854775807、MIN=-9223372036854775808，KTIME_SEC_MAX=9223372036，MIN=-9223372036（KD112-E034）。

timespec64_to_ktime将一个结构**按值**传给ktime_set(ts.tv_sec,ts.tv_nsec)（KD112-E036）；ktime_set形参是s64 secs和ulong nsecs，先判secs>=KTIME_SEC_MAX直接返回KTIME_MAX，否则计算 `secs*NSEC_PER_SEC+(s64)nsecs`（KD112-E035）。该函数未设置负秒下门。timespec64_to_ns形参为结构指针，先以 `tv_sec>=KTIME_SEC_MAX` 返回MAX，再以 `tv_sec<=KTIME_SEC_MIN` 返回MIN，只有两门都不命中才乘加tv_nsec（KD112-E037/E038）。

timespec64_valid先拒绝tv_sec<0、再以ulong转换拒绝tv_nsec>=1e9；valid_strict另外拒绝tv_sec>=KTIME_SEC_MAX（KD112-E039/E040/E041/E042）。它们均是**外部可调用**的校验，并未由上述两个转换体自行调用。有效输入优先采用规范区间0<=tv_nsec<1e9以及已确定可表示的乘加，异常/负值只作为有前提的数学示例。

纸面样例（输入秒,纳秒→标量）：(0,0)→0；(1,7)→1000000007；(-1,999999999)→-1（但valid拒绝负秒）；(SEC_MAX−1,999999999)→9223372035999999999；(SEC_MAX,0)两接口均提前封顶MAX（即便该秒乘法自身未越界）。重要差异：(SEC_MIN,0)时ktime_set在有限可表示乘法下得到-9223372036000000000，但timespec64_to_ns**提前封顶KTIME_MIN**。secs比SEC_MIN更负时不可把ktime_set的可能有符号溢出写成确定结果。精确门、来源、输入域见facts的A模型。

## B：ktime_add_safe 的真实三门而不只是溢出

hrtimer.c中ktime_add_safe先把 `ktime_add_unsafe(lhs,rhs)` 赋给有符号ktime_t res（KD112-E046）；unsafe宏定义为`((u64)(lhs)+(rhs))`（KD112-E044），这一路算术先于有符号res比较。随后按顺序求 `res<0 || res<lhs || res<rhs`；任一为真，就调用ktime_set(KTIME_SEC_MAX,0)返回KTIME_MAX，否则返回res（KD112-E047和KD112-E035）。超出s64范围的u64结果再赋s64是**实现定义转换**，不能把假想负值当作可移植C结果。

有限四例：1+2→3；0+KTIME_MAX→KTIME_MAX且无饱和门；KTIME_MAX+1的u64和为2^63，只有明确假设目标转换为两补码KTIME_MIN，才能推断首门触发并封顶；**-1+2的res为1且没有溢出，但1<rhs(2)为真，故仍封顶为KTIME_MAX**。普通ktime_add与ktime_add_ns另用直接+宏（KD112-E043/E045），不能把unsafe的u64规则借给它们，尤其不能为有符号溢出编造机器值。

## C：从纳秒标量反向规范化秒/纳秒

ktime_to_timespec64宏展开到ns_to_timespec64（KD112-E048）。函数初始化{0,0}并声明 `s32 rem`（KD112-E049）。正nsec通过div_u64_rem取得商给tv_sec、余数给tv_nsec（KD112-E050）；负nsec计算 `-nsec - 1`，由商q得到tv_sec=-q-1，由余数r得到tv_nsec=1e9-r-1（KD112-E051），零则沿初始化值直接返回（KD112-E052）。div_u64_rem实现对u64 dividend/u32 divisor求模，将结果写`*remainder`并返回无符号商（KD112-E053）。

**真实编译边界：** ns_to_timespec64的局部变量是s32 rem，传给函数的`&rem`类型为s32*，而div_u64_rem要求u32*。这是从固定源码能确认的指针参数不兼容问题，编译诊断、语言约束下的实际执行及目标ABI行为均NOT_RUN。因此以下不是“已执行的C程序结果”，只是在辅助调用能安全写回其非负余数的假设下的数学表：0→(0,0)，1→(0,1)，-1→(-1,999999999)，+1e9→(1,0)，-1e9→(-1,0)，-1000000001→(-2,999999999)。六组的0≤tv_nsec<1e9均经有限整数核查。若输入s64最小值，`-nsec - 1`在减1前先执行**有符号一元负号**，中间可能溢出；数学化简不构成原C求值安全性证据。

## 出处、界限与交回

本批证据账新增53条固定time真实定义体/宏/CMake短引文，分别1–5行，53/53逐字；18个普通位置另记。原110的74条、111的54条及签发的53条体内合格数各自冻结。facts记录71行历史分区规则、全部分歧消费者、16个有限模型和53条原位置索引；DOT为静态图、非Graphviz运行图。jiffies轮盘与纳秒标量的调用关系不能凭单位相似而假设存在。

终态拟交付：READY_FOR_MAINLINE_REVIEW；mainline_acceptance=PENDING；D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW；full_local_P9/编译/实际运行均NOT_RUN；独立审核权属于C07 Pro。
