# 第157批：信号设置、共享队列与旧值回填消费

固定time a039d9803ade、219@e9f4a788b1aa；五个必要场景、十个独立重置输入行。D00=[]；R155-01已由主线判闭，不重新改旧件；156三项非阻断说明合读。新增是具体对象状态、队列、别名和错误顺序见证，不重报09/10/63/64旧根因。限定排重及旧selectors见facts/evidence。

## 入口、数值与适用边界

活动表项nr13/14分别绑定sys_rt_sigaction/sys_rt_sigprocmask；固定x64_sys_call的switch经__SYSCALL展开到__x64_sys_rt_*，__SYS_STUBx到__se_sys_rt_*，MYOS_SYSCALL_DEFINE4把name变成_+name，__MYOS_SYSCALL_DEFINEx形成__do_sys_rt_*及声明；__MAP4/__SC_DECL作类型/名字，__ARCH_MAP(4,...)展开__X86_64_MAP4，再由__SC_CAST取di、si、dx、r10。仅源文本选择，未认证实际编译/ABI。tests.c::test_all有signal(SIGTERM,sigaction_SIGKILL_test)，myshell.c::main的test_all()被注释；两文件不支持实际shell执行或全仓无其它调用者。外部libc包装、真实发起者、返回尾及handler执行未知。锚 SG157-E186、SG157-E187、SG157-E188、SG157-E189、SG157-E190、SG157-E191、SG157-E178、SG157-E174、SG157-E176、SG157-E183、SG157-E184、SG157-E185、SG157-E179、SG157-E180、SG157-E177、SG157-E181、SG157-E196、SG157-E204、SG157-P040。

按选定模型byte8、int32、long/ulong/指针64，__KERNEL__、__x86_64__且非__ILP32__，_NSIG=64、_NSIG_WORDS=1，sigset名义8字节；sigaction在两个ARCH_HAS分支未定义时四字段（handler指针、ulong flags、restorer指针、单字mask），k_sigaction仅sa；名义32字节，非实际ABI认证。sigmask(s)=1UL<<(s−1)；剔除9/19即0x40100，输入{9,10,19}=0x40300变{10}=0x200。SIG_BLOCK/UNBLOCK/SETMASK=0/1/2，EINVAL=22、EFAULT=14，PREALLOC=1。SIG_KERNEL_IGNORE_MASK由CONT18、CHLD17、WINCH28、URG23组成，SIGRTMIN32；SIG_DFL/IGN是特殊动作0/1。sigandsets/orsets/andnsets分别为 &、|、&~，一字宏先取a0/b0再赋r.sig[0]，可按本例r=a消费。所有数值以固定源及选定域为条件。锚 SG157-E135、SG157-E136、SG157-E137、SG157-E149、SG157-E171、SG157-P028、SG157-P025、SG157-E193、SG157-E194、SG157-E195、SG157-P014、SG157-E082、SG157-E083、SG157-E084、SG157-P015、SG157-E155、SG157-E157、SG157-E064、SG157-E139、SG157-E145、SG157-E151、SG157-E152、SG157-E153、SG157-E202、SG157-E203、SG157-P037、SG157-P038、SG157-P039、SG157-E075、SG157-E068、SG157-E144、SG157-E143、SG157-E147、SG157-E146、SG157-E158、SG157-E159、SG157-E073、SG157-E060、SG157-E062、SG157-E058、SG157-E061、SG157-E063、SG157-E059。

GIVEN current=T、T.sighand=H、T.signal=S和所有跟踪对象有效、无隐藏别名/并发；锁正常。共享S.shared_pending与T.pending独立。链表有效环、互指、count正确；非NULL/非poison、删链while条件假。只有B02明确给定位图与节点集合不一致，非生产者实证。输入成功须另给合法对象、完整copy_from_user及残量0；输出成功另给合法目标、instrument/raw完整与残量0。B=0xffff800000001000仅数值，U=0x400100为给定合法8字节输出/同址对象，非由数值门证明映射。linux/kernel/uaccess.h→asm/uaccess.h先uaccess_64再generic；x86 __access_ok同名宏使generic备用体guard不进入，access_ok宏调用x86。小常量门判起点符号；general门算无符号sum再判符号且不回绕，B+8=0xffff800000001008，B+32=0xffff800000001020，均不回绕且有符号负，&&右操作数未达；两支皆拒绝，不必认证constant_p值。copy_to_user实际返回原n，instrument/raw不达，未发目标写请求。from门注释只是旧事实，不推出成功。锚 SG157-P019、SG157-P021、SG157-E133、SG157-P022、SG157-P023、SG157-E134、SG157-E130、SG157-E131、SG157-E132、SG157-E197、SG157-E198、SG157-E128、SG157-E129、SG157-E125、SG157-E126、SG157-E127、SG157-P020、SG157-E122、SG157-E123。

## A01：动作已提交且共享清理，后续旧动作输出拒绝

rt_sigaction本体：size8通过，act完整32字节GIVEN复制；do_sigaction(15,&new_sa,&old_sa)号合法、非kernel_only，取H.action[14]，持siglock，旧flags不含IMMUTABLE；按值快照旧动作{DFL,0,Rold,0x800}。新/旧flags分别 &= UAPI_SA_FLAGS，同值0；BUILD_BUG_ON实际宏位置仅记录，不认证预处理，RS-01保留。新动作mask 0x40300→0x200，整项*k=*act写{IGN,0,Rnew,0x200}，随后sig_handler读该新handler；IGN使ignore谓词真，默认支短路。do_sigaction局部mask先0再加15=0x4000，和新动作sa_mask不是同一对象。flush用独立m=mask & shared.signal=0x4000，非空，清位shared.signal=0x200。qA(15,0),qB(10,0),qC(15,1)按序，安全迭代先存n，qA、qC命中；qB不中。contains分别扫描[qA]与[qB,qC]命中，__list_del_entry_valid读取prev/next及互指，while条件假；__list_del先next.prev=prev再WRITE_ONCE(prev.next,next)，list_del_init令摘出节点next/prev自指，再count--，返回节点指针，flush不检查返回。A实际3→2→1，qA后请求kmem_cache_free且GIVEN正常返回、其它节点保持，之后不读qA；qC已摘链且count减后PREALLOC早退，无free请求，不推所有权/泄漏。最终环anchor↔qB，qC摘出自链；安全迭代在q指向头哨兵时停止，不读伪容器info，名义C表示按所选模型，真实provenance未知。私有qD/位图0x4000/count1不变，私有每线程flush循环注释。核心解锁return0，外层ret=0；oact=B，copy_to_user(...,32)数值拒绝返32，本体返-14，已提交新动作及清理不回滚；B没有raw写，旧动作只为本体退出前历史快照。锚 SG157-E001、SG157-E002、SG157-E003、SG157-E004、SG157-E005、SG157-E036、SG157-E037、SG157-E038、SG157-E039、SG157-E040、SG157-E041、SG157-E042、SG157-E043、SG157-E045、SG157-E046、SG157-E054、SG157-E047、SG157-E018、SG157-E019、SG157-E020、SG157-E021、SG157-E109、SG157-E107、SG157-E106、SG157-E096、SG157-E097、SG157-E098、SG157-E094、SG157-E093、SG157-E090、SG157-E091、SG157-E092、SG157-E089、SG157-E015、SG157-E016、SG157-P008、SG157-E044、SG157-E125。

## B01/B02：默认忽略谓词与位图早退

B01两行独立新动作DFL、mask/flags0，oact=NULL，旧表项不含IMMUTABLE且handler=H0；旧未给的restorer/mask保留符号，不虚填。号17的ignore集合命中：写动作后flush mask=0x10000，m=0x10000，shared 0x10200→0x200；qM(17)摘链count2→1、一次普通free请求后正常返回，只qO(10)，私有同号对象未清理。号15的DFL不在ignore集合：动作已写，根本不进入flush，shared 0x4200及qM/qO、count2保持、无free请求。两例核心及本体return0，无oact快照或copyout。B02为GIVEN的shared位图0x200而队列qM(15)→qO(10)、count2，IGN仍写动作并调用flush，mask=0x4000，m=0，sigisemptyset门直接void return，清位、迭代初始化、逐节点sigismember、contains、摘链和free均不达；终点位图/节点/count保持，核心/本体return0。不是证明真实内核生产了不一致。锚 SG157-E042、SG157-E043、SG157-E045、SG157-E046、SG157-E068、SG157-E070、SG157-E066、SG157-E065、SG157-E018、SG157-E049、SG157-E050、SG157-E019、SG157-E020、SG157-E015、SG157-E016、SG157-P008、SG157-E044、SG157-E005。

## C01/C02：输入输出同址、相等早退及错误顺序

独立三种mask对象：入口old_set先快照T.blocked={2,10,12}=0xa02；nset非NULL时new_set为独立输入副本，去9/19成0x200；sigprocmask(how,&new_set,NULL)核心oldset=NULL不二次快照，另声明局部newset。BLOCK合成0xa02（{2,10,12}），sigequalsets一字相等，__set_current_blocked直接return，不取本函数siglock、不写blocked。UNBLOCK合成0x802（{2,12}），SETMASK合成0x200（{10}），不等则锁、__set_task_blocked；TIF_SIGPENDING测试GIVEN假，&&短路，thread_group_empty/newblocked/retarget不达，实际赋tsk->blocked=*newset；recalc注释，正常解锁。核心return0，error=0。C01三例nset=oset=U但U不与任何内核对象别名，输入已形成new_set，最后GIVEN完整copy_to_user(U,&old_set,8)将U覆盖为0xa02，不改变早先输入或已提交blocked。C02_QUERY nset=NULL/how99，整个输入块跳过，new_set未初始化未读、core newset不存在、error未赋，how不校验，U正常回填旧mask，return0。BAD_HOW V输入完整，过滤后核心default -22，外层if(error)立即返回；oset=B连地址门都未进入，不虚造-EFAULT。COMMIT_THEN_REJECT 合法SETMASK先提交0x200，之后B门拒绝返回8，本体-14，blocked仍0x200，V未改变。队列、动作表和TIF不因本体取得清理/唤醒结论。锚 SG157-E007、SG157-E008、SG157-E009、SG157-E010、SG157-E011、SG157-E012、SG157-E028、SG157-E029、SG157-E030、SG157-E031、SG157-E032、SG157-E033、SG157-E034、SG157-E026、SG157-E052、SG157-E027、SG157-E022、SG157-E057、SG157-E118、SG157-E119、SG157-E120、SG157-E024、SG157-P005、SG157-P006、SG157-E125、SG157-E202、SG157-E203。

## 十输入行结果（局部终点，各自重置）

| 行 | 本体局部return | 动作或blocked | shared bitmap/节点/count | free请求 | 输出与锁 |
|---|---:|---|---|---|---|
| A01_ACT_IGN_COPYOUT_REJECT | -14 | SIG_IGN; mask 0x200 | 0x200/qB/1 | qA | 拒绝残量32；1/1 |
| B01_CHLD | 0 | SIG_DFL; mask 0x0 | 0x200/qO/1 | qM | 未调用；1/1 |
| B01_TERM | 0 | SIG_DFL; mask 0x0 | 0x4200/qM,qO/2 | 无 | 未调用；1/1 |
| B02_BITMAP_MISS_WITH_QUEUED_NODE | 0 | SIG_IGN; mask 0x0 | 0x200/qM,qO/2 | 无 | 未调用；1/1 |
| C01_BLOCK | 0 | 0xa02 | 不跟踪/无新增清理结论 | 无 | U=0xa02；0/0 |
| C01_UNBLOCK | 0 | 0x802 | 不跟踪/无新增清理结论 | 无 | U=0xa02；1/1 |
| C01_SETMASK | 0 | 0x200 | 不跟踪/无新增清理结论 | 无 | U=0xa02；1/1 |
| C02_QUERY | 0 | 0xa02 | 不跟踪/无新增清理结论 | 无 | U=0xa02；0/0 |
| C02_BAD_HOW | -22 | 0xa02 | 不跟踪/无新增清理结论 | 无 | 未调用；0/0 |
| C02_COMMIT_THEN_REJECT | -14 | 0x200 | 不跟踪/无新增清理结论 | 无 | 拒绝残量8；1/1 |

结构化facts给每行初态、GIVEN、事件、读/写差量from/to、未达、锁/复制及终点，局部对象在各帧退出后只是历史快照，未初始化是状态而非读出的值。图分为十条独立FLOW_ORDER及单独SOURCE_OVERVIEW（调用者→被调用者、数据消费者→提供者；宏展开不当实际函数调用），与facts同ID/全属性，终点无后继。不跨越实际局部return；若更早合法性、复制、锁、链表、free返回等前提不满足，停在最早边界，不能保证后续结果。实际预处理、构建、程序、Graphviz、真实信号夹具、QEMU/实机/IO、完整本地P9均NOT_RUN；真实配置/ABI/用户映射、生产者及用户最终观察未证。
