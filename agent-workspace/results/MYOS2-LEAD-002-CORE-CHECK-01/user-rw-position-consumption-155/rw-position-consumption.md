# 第155批：标量read/write的位置消费与kernel_read别名

固定time a039d9803ade；七个独立有限模型，D00=[]。R152-01沿C11既有闭合，152历史RETURN与NOT_RECORDED保持。旧115的九例排除回调直接改F及并发，旧27已有kernel_read不回写ki_pos及legacy原count事实，旧62已有向量直接写/覆盖限定。这里只加入同一明确GIVEN回调写与包装层消费、别名的组合，未重算旧账、未另报已证根因。

## 公共数值和对象

每例独立有效F、初始F.f_pos=10、给定指针B及方法表；A/B count=8，C count=N。选定byte8、int/unsigned32、long/ulong/long long/size_t/ssize_t/loff_t/指针64位模型，所有位置及返回可表示。PAGE_SHIFT=12，PAGE_SIZE=4096，INT_MAX=2147483647，PAGE_MASK=0xfffffffffffff000，M=MAX_RW_COUNT=0x7ffff000=2147479552，N=M+0x20=0x7ffff020=2147479584。活动min_t转__cmp，条件表达式会再次求值被选实参；本例N/M稳定无副作用，结果M，不推广通用单次求值。配置文本含64位/SysV/CONFIG_BUG，但不是实际预处理或ABI证书。锚 RW155-E076、RW155-E077、RW155-E078、RW155-E079、RW155-E052、RW155-E080、RW155-E081、RW155-E093、RW155-E094、RW155-E096。

A/B外层L是ksys本地pos，K为helper局部kiocb.ki_pos；非流ppos先指F.f_pos，后改指&L，故回调直接写F与L是不同对象。stream ppos=NULL，L未初始化也不读。C参数P=&F.f_pos，没有独立L；P本身不变，*P与F字段同一对象。helper的K/iter/iov在所属函数退出后仅保留退出前历史快照，不再读取失效栈。锚 RW155-E048、RW155-E049、RW155-E046、RW155-E047、RW155-E022、RW155-E024、RW155-E038。

## A/B共同真实前缀

读getter为myos_fdget_pos，写为fdget_pos。题设正常返回f.file=F后开始，外层初始化ret=-EBADF，写侧filp=F，非空门通过。flags/引用内部不展开，不设置额外flags。file_ppos按STREAM位选NULL或字段地址，非流L=10且ppos改为&L。读vfs要求READ，CAN_READ门注释；写vfs要求WRITE与CAN_WRITE；access_ok=true为GIVEN不证明实际映射；rw_verify_area首活动语句return 0，后续检查死区；8不超过M。读方法RI/RL同时存在，read_iter优先实际调用new_sync_read；写write=NULL、write_iter=WI，实际调用new_sync_write，legacy优先规则与read相反。锚 RW155-E023、RW155-E037、RW155-E070、RW155-P001、RW155-E071、RW155-E022、RW155-E024、RW155-E038、RW155-E015、RW155-E030、RW155-E016、RW155-E031、RW155-E001、RW155-E002、RW155-E017、RW155-E032、RW155-E018、RW155-E033。

init_sync_kiocb的复合初始化仅显式ki_filp=F，选定C规则K=0；实际下一条赋K，非流读L=10，stream取0。UBUF活跃字段iter_type=ITER_UBUF(5)、ubuf=B、count=8、iov_offset=0、nr_segs=1；方向实参读ITER_DEST=0、写ITER_SOURCE=1，data_source字段与赋值均为注释。真正间接调用者是同步helper；给定回调经ki_filp直接F10→90、再K→70、正常返回指定r；不改迭代器、其它字段或缓冲，不把给定写当生产回调语句。BUG_ON(ret==-EIOCBQUEUED=-529)五例皆假；读helper只需ppos非空便K→L，写helper需ret>0且ppos非空；外层条件独立ret>=0&&ppos。锚 RW155-E051、RW155-E004、RW155-E028、RW155-E055、RW155-E058、RW155-E059、RW155-E061、RW155-E062、RW155-E063、RW155-E064、RW155-E065、RW155-E005、RW155-E029、RW155-E073、RW155-E074、RW155-E025、RW155-E039。

## 七例终点结果

| 场景 | 模式(hex) | 局部ret | L | K(历史) | F.f_pos | 位置指针 | 终点 |
|---|---:|---:|---|---:|---:|---|---|
| A_READ_ZERO | 0x1 | 0 | 70 | 70 | 70 | &L | BEFORE_FDPUT_POS |
| A_WRITE_ZERO | 0x40002 | 0 | 10 | 70 | 10 | &L | BEFORE_FDPUT_POS |
| B_READ_NEG | 0x1 | -5 | 70 | 70 | 90 | &L | BEFORE_FDPUT_POS |
| B_WRITE_NEG | 0x40002 | -5 | 10 | 70 | 90 | &L | BEFORE_FDPUT_POS |
| B_READ_STREAM | 0x200001 | 3 | NOT_INITIALIZED_AND_NOT_READ | 70 | 90 | NULL | BEFORE_FDPUT_POS |
| C_KERNEL_ITER_ALIAS | 0x1 | 3 | 不适用 | 70 | 90 | &F.f_pos | KERNEL_READ_LOCAL_RETURN |
| C_KERNEL_LEGACY_ALIAS | 0x1 | 3 | 不适用 | 10 | 70 | &F.f_pos | KERNEL_READ_LOCAL_RETURN |

A_READ_ZERO：读helper在r=0仍写L=70，外层ret>=0且ppos真，实际F90→70。A_WRITE_ZERO：写helper ret>0假，短路ppos，L保持10；外层ret>=0且ppos真，仍实际F90→10。两例零返回均可有位置覆盖；helper门不同，不能概括成零返回不赋值。

B_READ_NEG：r=-5，读helper写L=70，外层ret>=0假短路ppos，F保持回调给定90。B_WRITE_NEG：写helper ret>0假短路ppos，L=10，外层ret>=0亦假，F=90。负返回不等于回滚已发生的给定写。B_READ_STREAM：ppos=NULL，不读F来初始化L；K=0后回调F=90、K=70、r=3；helper ppos假、外层先ret>=0真再ppos假，均不赋位置；L全程未初始化不读，F=90。stream不保证回调不改F。

A/B五例统一在fdput_pos调用前停止（read_write.c L194/L334），只记外层局部ret，不把其写作实际ksys返回，不证明归还、fput、用户态返回或真实对象生命周期。证据 RW155-E025、RW155-E039、RW155-E072、RW155-E026、RW155-E040。

## C两例显式别名

kernel_read调用rw_verify_area(READ,F,P,N)，ret=0，直接return __kernel_read(F,B,N,P)。__kernel_read先构造kvec{base=B,len=M}，再过WARN_ON_ONCE(!(F.f_mode&FMODE_READ))假门，CAN_READ注释。init_sync_kiocb设ki_filp=F、K先0再读*P=10；iov_iter_kvec(...READ,&iov,1,M)设ITER_KVEC(1)、kvec=&iov、count=M、iov_offset=0、nr_segs=1，READ=0仅实参，不保存direction。锚 RW155-E014、RW155-E006、RW155-E008、RW155-E075、RW155-E009、RW155-E066、RW155-E067、RW155-E068、RW155-E010、RW155-E012、RW155-E013。

C_KERNEL_ITER_ALIAS：read_iter=RI优先，read=RL不测试；RI收到&kiocb、&iter，count和iov.len均M。给定经ki_filp直接F=90，再K=70，返回3；P指针没变，*P因别名已为90。尾部位置回写注释，未执行*P=K；F=*P=90，K历史70。不能写成pos没回写所以*P仍10。

C_KERNEL_LEGACY_ALIAS：read_iter=NULL、read=RL。局部iov/KVEC初始化仍发生，K=10，实际调用RL(F,B,N,P)，原count=N而非M；iter未传给回调且保持M。给定先F10→90，再*P=70，同一对象90→70，不是两独立位置；K保持10，返回3。尾部没有活动位置回写，两C均无A/B同步helper的BUG_ON门；__kernel_read正常局部返回3，kernel_read直接返回3，外层ret变量仍是验证0。终点不扩展上层调用者、系统调用或用户态。N/M区别不是实际越界IO证明。

## 边界和交付

facts的逐步记录区分SOURCE_FACT、GIVEN_CALLBACK_EFFECT、GIVEN_RETURN、DERIVED_CONDITIONAL及UNKNOWN；位置写与回调返回独立、实际读取和短路未达分账。DOT的边为到下一事件的有序关系，事件类型和调用主体/被调者区分；五分析停无后继、两C局部返回无用户态连接。回调仅模型给定，真实FS行为、数据、映射、ABI、对象有效性/生产可达性及实际后台模型/推理档未证。完整本地P9、仓库程序/脚本、预处理、编译链接、Graphviz、QEMU、实机、真实IO均NOT_RUN；自写有限文本/整数账核验不等同真实执行。
