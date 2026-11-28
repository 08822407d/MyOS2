# 第156批：kernel_write准入、方法及位置消费

源pin a039d9803ade，合同218@1937b1b16805。A/B/C七个独立有限模型，R155-01当前补读及补记见MANIFEST，待主线判闭。155原件、原历史范围和101正式窗保持。

## 共同对象和数值

F有效，f_pos=10，mode/f_op不变；A/B P=&Z，Z=10且不与F别名；C P=&F.f_pos，无Z。P不变，*P随实际对象变动；每例独立重置。选定8位字节、32位int/unsigned及64位long/ulong/long long/size_t/ssize_t/loff_t/指针，各位置与返回可表示；M=MAX_RW_COUNT=(INT_MAX & PAGE_MASK)=0x7ffff000=2147479552，N=M+0x20=0x7ffff020=2147479584。PAGE_SHIFT=12、PAGE_SIZE=4096、INT_MAX=2147483647、PAGE_MASK=0xfffffffffffff000。活动min_t经__cmp三目，可能再求值被选操作数，本例稳定无副作用。FMODE_WRITE=2、FMODE_CAN_WRITE=0x40000；方向WRITE=1、ITER_SOURCE=1另属参数常量。构型文本含64位、SysV、CONFIG_BUG，不等于真实预处理/ABI认证。锚 KW156-E024、KW156-E025、KW156-E047、KW156-E036、KW156-E032、KW156-E050、KW156-E051、KW156-E052、KW156-E053、KW156-E054、KW156-E049、KW156-E048、KW156-E072、KW156-E073、KW156-E075。

WL的有序GIVEN为F10→40、Z10→30，返回2；WI有序GIVEN为经ki_filp直接F10→90、K10→70、按场景正常返回r。不改其它受跟踪字段、P、iter/iov、缓冲；返回数与位置写独立。B仅传递符号，不分配或复制2GB缓冲。旧115/155/27/62与05/08定点范围见facts.historical_scope；这里只新增kernel接口自身消费组合，不重复报旧VFS门或推定用户write改变调用路线。

## A：双方法表的实际分派

A_VFS_BOTH从vfs_write局部入口，mode=0x40002、WL/WI同时有效。先WRITE/CAN_WRITE、GIVEN access_ok(B,N)通过；rw_verify_area(WRITE,F,P,N)首活动语句return0，后续位置/长度检查不达。局部count=N截为M，write非NULL优先WL(F,B,M,P)，不测试write_iter回退，也没有new_sync_write/kiocb/iter/iov或排队哨兵门。给定WL写F40、Z30，返回2；本层正常返回2，F40而非30。无ksys外层提交、fd取得或归还。锚 KW156-E018、KW156-E019、KW156-E020、KW156-E001、KW156-E002、KW156-E021、KW156-E022、KW156-E023、KW156-P008、KW156-P009、KW156-P010。

A_KERNEL_BOTH同表同mode，从kernel_write局部入口；rw_verify_area(WRITE,F,P,N)先0，然后__kernel_write构造iov{base=B,len=M}及KVEC（iter_type=ITER_KVEC=1，kvec=&iov，nr_segs=1，iov_offset=0，count=M），方向ITER_SOURCE=1只是实参，活动结构无data_source存储。此后才调用__kernel_write_iter(F,&iter,P)。声明kiocb/ret后先WARN_ON_ONCE(!(mode&FMODE_WRITE))，再CAN_WRITE门，均通过。init_sync_kiocb复合初始化ki_filp=F、K=0（选定C规则），后继本函数自身K=*P=10。双方法拒绝及缺方法保护仅注释，实际直接WI(&kiocb,from=&iter)，不执行WL且无legacy长度实参。给定F90/K70，Z仍10，r=3；ret>0及pos非空，源赋值*pos=K使Z70。__kernel_write_iter返回3，__kernel_write直接透传3，kernel_write实际ret=__kernel_write结果并返回3；F仍90。三层正常局部结果，并非用户态返回。锚 KW156-E015、KW156-E016、KW156-E012、KW156-E013、KW156-E006、KW156-E007、KW156-P004、KW156-P005、KW156-E008、KW156-E029、KW156-E030、KW156-E031、KW156-E044、KW156-E045、KW156-P014、KW156-P015、KW156-E009、KW156-E010。

## B：初始化先后、模式门、缺失方法

B_NO_WRITE仅CAN_WRITE，B_NO_CAN_WRITE仅WRITE，两例WL有效、WI=NULL、P=&Z；均先验证0、构造iov与KVEC，再进入__kernel_write_iter声明kiocb/ret。所选CONFIG_BUG文本WARN_ON_ONCE为(condition)，不是额外日志/计数保证。首例第一门真直接return -EBADF=-9，第二门不达；次例首门假而第二门真，直接return -EINVAL=-22。真实errno包含链从lib/errno.h经uapi/linux/errno.h、generic/errno.h到errno-base.h；数值分别9、22，-529来自lib/errno.h。两例init、K读取/赋值、*P读取、write_iter字段读取/调用及尾部均未达，局部kiocb已声明未初始化、K=NOT_INITIALIZED_AND_NOT_READ（非0，不读取），F/Z均10；已构造的iov/iter在各自寿命结束前保留历史快照。错误直接透传并赋kernel_write.ret后正常局部返回。锚 KW156-E006、KW156-E007、KW156-E060、KW156-P021、KW156-E075、KW156-E056、KW156-E057、KW156-P018、KW156-P019、KW156-P020、KW156-E013、KW156-E016。

B_MISSING_ITER两模式都通过，WL有效、WI=NULL；同样先构造KVEC，再init令K0，实际K=*P=10。到read_write.c:239间接调用动作**之前**分析停止；由给定有效表知目标NULL，不执行函数指针求值/间接调用动作，不构造fallback、错误返回、panic、实际崩溃或尾部写回。F10/Z10/K10，栈及对象在分析停止点仍有效，inner.ret未初始化未读，kernel_write.ret仍为前验证0，不是函数返回0；kernel_write_return=NOT_DERIVED。后续ret>0、pos门、赋值及所有helper返回均不达。锚 KW156-P004、KW156-P005、KW156-E008、KW156-E009、KW156-E010。

## C：显式别名、非正返回及哨兵数值

C_ALIAS_ZERO/C_ALIAS_QUEUED_VALUE均P=&F.f_pos、WL=NULL、WI有效，mode=0x40002；kernel_write局部前缀同A_KERNEL_BOTH，WI收到count=M的KVEC。GIVEN写F90、K70，*P随F同值（非独立赋值），分别正常返回0与-529。两例ret>0为假，嵌套pos门、K读取及*pos赋值不达；无ksys提交，F=*P=90。内层正常返回、__kernel_write直接返回、kernel_write赋ret后正常局部返回0/-529，K70为内层退出前历史快照。new_sync_write的BUG_ON位于同步助手对照L212–216，本批A VFS选legacy，六kernel例不调用该助手；完整__kernel_write_iter活动尾部只有正值门及return，没有该值哨兵门。不把-529数值模型说成真实异步排队、completion、后续使用kiocb或panic。锚 KW156-E005、KW156-E055、KW156-E008、KW156-E009、KW156-P006、KW156-E010、KW156-E013、KW156-E016。

## 七场景结果（限定推导）

| 场景 | mode | 局部返回 | F | Z | *P | K状态或历史值 | 终点 |
|---|---:|---|---:|---|---:|---|---|
| A_VFS_BOTH | 0x40002 | 2 | 40 | 30 | 30 | NOT_APPLICABLE | VFS_WRITE_LOCAL_RETURN |
| A_KERNEL_BOTH | 0x40002 | 3 | 90 | 70 | 70 | 70 | KERNEL_WRITE_LOCAL_RETURN |
| B_NO_WRITE | 0x40000 | -9 | 10 | 10 | 10 | NOT_INITIALIZED_AND_NOT_READ | KERNEL_WRITE_LOCAL_RETURN |
| B_NO_CAN_WRITE | 0x2 | -22 | 10 | 10 | 10 | NOT_INITIALIZED_AND_NOT_READ | KERNEL_WRITE_LOCAL_RETURN |
| B_MISSING_ITER | 0x40002 | NOT_DERIVED | 10 | 10 | 10 | 10 | ANALYSIS_STOP_BEFORE_NULL_INDIRECT_CALL |
| C_ALIAS_ZERO | 0x40002 | 0 | 90 | NOT_APPLICABLE | 90 | 70 | KERNEL_WRITE_LOCAL_RETURN |
| C_ALIAS_QUEUED_VALUE | 0x40002 | -529 | 90 | NOT_APPLICABLE | 90 | 70 | KERNEL_WRITE_LOCAL_RETURN |

除分析停止外，K/ki_filp只为内层退出前快照，iov/iter只为外层helper退出前快照。facts用初态与带from/to的有序差量，逐事件分开SOURCE控制/调用/写、GIVEN写/返回及未达读取，DOT节点/边属性逐项对应，历史比较边不是执行。停止/正常返回节点无执行后继。未运行项目程序、预处理/编译链接/Graphviz/QEMU/实机/真实IO；完整本地P9=NOT_RUN，真实生产回调、数据/映射/生命周期、真实ABI/配置及实际后台模型/effort未知。发现只记条件组合观察，不判实际故障或新漏洞。
