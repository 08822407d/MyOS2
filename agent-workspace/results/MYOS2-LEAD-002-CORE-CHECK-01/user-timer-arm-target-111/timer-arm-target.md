# 第111批｜从定时器初始化到当前CPU及显式CPU的登记入口

执行身份：MYOS2-A-C07b / MYOS2-LEAD-002-CHAT-EXEC-16。合同 CORE-USER-TIMER-ARM-TARGET-111；主线lead@2735aa3116c3，执行父d7ebfb1d5e40，固定源码time@a039d9803ade，direct master@e8e60cd19540。阶段2只读源码、定点纯文本核对和普通整数纸面计算；无构建、无真实执行。新证据AT111-E001至E054、普通位置P01至P22在evidence.md，D00修正与模型在facts-and-boundaries.yaml。旧批次所有成果、评论及C07签发结论均冻结。

## D00：三项未闭合事项的合并补正

主线现行裁定为：R108-01/02均CLOSED_DOCUMENTATION_ONLY，第108批组合PASS_PENDING_LOCAL；R109-02也已文档闭合，但R109-01仍OPEN；R110-01及R110-02均OPEN。本执行者只提交CORRECTION_SUBMITTED_PENDING_C07_REVIEW，不签关闭。

**R109-01与R110-01合账修复。** 固定第110批的facts共有87条原记录、375个同位source字段。C07所指出的71条记录内223个位置写成 LNaN-L原起点，涉及72个不同证据ID、71种错误位置字面。错误按D00 23、A9行中的26、B10行中的38、C10行中的31、D10行中的35、X8行中的33、12个模型中的37次分布。此次将71条原记录保留原顺序的source_ids和错误序号，给出逐一恢复的223条section、record_id、source_locations字段内从0开始的序号、原E编号和74条固定源码窗口字典；任何一项均能按 ID 得到真实文件、整数起始行、整数终止行。74段原引文再次逐段从固定time实际文件取回，完整逐字相符。原第110批MANIFEST在P9第4条对evidence原窗的声明成立，但不能替代曾有223个无效位置的facts检查，本次专门补了事实表位置账。原件没有修改。

首项D00/108A第0位TB110-E071恢复为time:mykernel/time/syscall.c第19–22行；末项models/TB110-M12第3位TB110-E041恢复为timer.c第237–238行。同在timer.c第296行开始的TB110-E017是296–296，TB110-E051是296–298，按证据ID区分，不以原错误字面的起点强推相同终点。六条第109批剩余记录：109A、109B都恢复E070=timer.c:796–799；109E、109M1恢复E003=param.h:9；109H恢复E062=timer.h:90–91；109I恢复E063=timer.h:109–111。原109模型M01–M07另明确LVL_SHIFT的TW109-E024和LVL_BITS的TW109-E027及必要的E023–E032、E035；层选取、桶有效到期值与数组下标是不同计算结果。原109/110纸面结果冻结，非重新运行仓库代码。

**R110-02逐消费者修正。** 第110批正文和B10对两种__mod_timer入口曾将取锁、回调门、forward的顺序写乱。固定源码pending分支先lock_timer_base、检查timer->function，再forward_timer_base，之后才有再次检查pending、REDUCE和期限；normal分支同样先检查function再forward。callback为NULL在本次推进前走公共解锁。C10旧来源应联pending体内E043/E045，而E049是normal分支。C04锁后REDUCE门只讨论已通过锁前门的路径，不能用同一组稳定旧≤新数值假装到达锁后判定。

D01/D02/D05/D06、X03/X04、M10/M11的旧桶号必须由既存timer.flags经timer_get_idx解码，或显式假设该timer曾以相同旧基准入桶。给定old1064、clk1000只足以计算如果重新登记会落的桶，不能自动推出原已登记的idx。C05同桶判断还需旧E046，C09/X06不pending出口联E058/E050；D08的复用门是idx有效且clk数值相等（E053），不能推断指针恒等；D09摘链单节点可能设置next_expiry_recalc，后续enqueue是否清零又由有效桶期限是否更早决定。

旧图第109/114行所标per_cpu_ptr与this_cpu_ptr不是直接函数，而是x86头里的两个宏（新AT111-E050/E051）。候选由get_timer_cpu_base和get_timer_this_cpu_base各自的返回语句产生。迁移后第三次forward_timer_base调用在__mod_timer本体315行，不在changeBase赋值节点；两条初始__mod_timer分支分别在255、289行；add_timer_on另有独立532行直接请求。三个公开mod系列包装把options实参提供给__mod_timer，不是mod向options反向赋值。UINT_MAX初值和最终复用判断分别属于原TB110-E027、E053，证据不可混用。以上有12条独立差量行、真实旧文件和新旧源码窗口，保留R109-02已闭合的历史图结论。

## A：初始化时哪些字段被写

固定time的timer_setup和timer_setup_on_stack是两个单独定义的宏，但都展开为simple_init_timer_key(timer,callback,flags)。所读实现按顺序写timer->entry.pprev=NULL、timer->function=func；若WARN_ON_ONCE(flags & ~TIMER_INIT_FLAGS)非零，则先将局部flags与TIMER_INIT_FLAGS相与，再写timer->flags=flags | smp_processor_id()。固定CONFIG_BUG设置下活动WARN_ON_ONCE宏等价于条件表达式，本报告不将它解释成已经发生实机告警。

三种被初始化许可的位是TIMER_DEFERRABLE=0x00080000、TIMER_PINNED=0x00100000、TIMER_IRQSAFE=0x00200000，总INIT_FLAGS=0x00380000。以有效CPU号5作为独立纸面输入：

| flags实参 | 经过INIT_FLAGS筛选后 | 最终timer.flags |
|---|---:|---:|
| 0 | 0 | 0x00000005 |
| PINNED | 0x00100000 | 0x00100005 |
| PINNED或IRQSAFE两个位都置位 | 0x00300000 | 0x00300005 |
| 0x00340000，另含不被接纳的MIGRATING | 0x00300000 | 0x00300005 |

前提是所提供CPU号5合法且与flags允许位不发生异常冲突。这个初始化体没有直接给timer->expires或timer->entry.next赋值，也没有声称整块内存自动清零。因此后继登记必须已有有效绝对期限；它是否在别的调用者处赋值仍待检查。hlist_unhashed_lockless对entry.pprev做READ_ONCE并取逻辑反；timer_pending再取反。故刚初始化pprev=NULL的对象在这一局部模型中pending为false。真实并发更新与CPU来源未验证。

## B：当前CPU三个void登记入口

add_timer、add_timer_local、add_timer_global三者首先判断WARN_ON_ONCE(timer_pending(timer))。已pending的合法对象在入口直接return，尚未修改PINNED，也没有发出__mod_timer调用。对尚未登记、callback有效、此前已设置expires=1064的同一初始化对象分别代入：

- add_timer：在包装体中不主动改变PINNED，直接调用__mod_timer(timer,timer->expires,MOD_TIMER_NOTPENDING)。若初始flags=0x00000005，符号候选保持GLOBAL。
- add_timer_local：通过pending门后执行flags |= TIMER_PINNED，初始0x00000005成为0x00100005；再请求同一__mod_timer，选LOCAL候选。
- add_timer_global：通过pending门后执行flags &= ~TIMER_PINNED，若初始0x00100005则变为0x00000005；再请求同一__mod_timer，选GLOBAL候选。

三者的公开返回类型均为void，未取得__mod_timer的int返回供上层处理。它们没有显式CPU参数；正常路径下一层结合旧flags基准与当前CPU基准进行计算。NOTPENDING将pending快速优化跳过，只有回调非NULL、基准及其锁定路径可用等条件成立，__mod_timer后续才写expires并由internal_add_timer登记。本报告不从包装返回推断真实登记成功，也不重新审计整个时间轮。

## C：显式CPU的add_timer_on

add_timer_on(timer,cpu)返回void，并不调用__mod_timer。按源代码先后：先检查pending，已pending则立即return，保持入口内原flags、没有选新base；若pending为false则先把PINNED置入timer->flags，再根据显式cpu参数调用get_timer_cpu_base(timer->flags,cpu)取得new_base。随后lock_timer_base(timer,&flags)按flags保存的旧CPU位取并锁旧base，再检查timer->function。若function==NULL，跳公共unlock；**此前PINNED写入已经发生**，不能报告本次毫无字段变化，且不会请求此后的forward或internal_add_timer。

若回调有效，将旧base与new_base指针比较。若相等，跳过迁移字段操作但仍执行forward_timer_base(base)、internal_add_timer(base,timer)、公共unlock。若不同，则将TIMER_MIGRATING写进flags，解锁旧base、base=new_base并锁新base，随后执行WRITE_ONCE(timer->flags,(timer->flags & ~TIMER_BASEMASK) | cpu)。TIMER_BASEMASK将CPU低18位和MIGRATING位一并清除，最后用**显式参数cpu**重写这些位，再forward、internal_add_timer和公共解锁。

在假定原flags=0x00000005、旧CPU5、目标CPU7，且LOCAL5和LOCAL7符号候选确实为不同指针时：置PINNED得0x00100005，置MIGRATING中途得0x00140005；掩去CPU和迁移位后保留0x00100000，或入参数CPU7得0x00100007。若目标cpu=5且比较出的指针确实相同，则无需此项重写；仍会推进并请求登记。IRQSAFE之类不在BASEMASK内的位可继续保留。这里只做位域运算，不认证真实每CPU数组地址或迁移原子性。

add_timer_on的活动函数体本身**不赋值timer->expires**。它将既有期限交给internal_add_timer，后者使用已保存的expires与当前base.clk求桶。相反，先前__mod_timer的迁移写回表达式用的是base->cpu，不是此函数形参cpu；二者不得替换引用。四个公开arm入口都返回void，三个当前CPU包装最终交给__mod_timer，显式CPU入口则自己组织forward_timer_base、internal_add_timer和解锁。

## 自检及交付边界

D00按223个旧错误位置、71条记录、72个旧证据ID分别建可展开字典；另列R109-01六项和M01–M07的几何依赖、R110-02的12处不同消费者。事实区为四张有界表共34行，纸面模型12个。新增evidence有54条固定time真实定义内1–5行引文、9份新源码路径和22个普通位置；DOT区分真实函数、宏展开、参数来源、早退、字段写入、锁/迁移次序和历史勘误；deferred保留既有root且仅列条件候选与证据缺口。

本次只作静态分析。完整本地P9、编译、预处理、链接、Graphviz、仓库代码及脚本、内核/QEMU、实机硬件、CPU与并发均NOT_RUN。不存在基于运行的故障证明，也没有修改内核源码、旧成果、其他任务目录、主线、PR元数据、已有评论或分支结构。

作者交付态READY_FOR_MAINLINE_REVIEW；mainline_acceptance=PENDING；D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW，R109-01/R110-01/R110-02均保持OPEN，等待C07 Pro审核。严格六件新增、唯一正式PR17作者顶层评论及全部远端回读完成后STOP_WRITING，不自行启动第112批。
