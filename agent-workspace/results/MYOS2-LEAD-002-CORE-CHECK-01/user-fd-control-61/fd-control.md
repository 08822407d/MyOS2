---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-FD-CONTROL-61
record_type: bounded_static_fd_control_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
conversation_display_name: MYOS2-A-C05a 代码分析执行
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-08
base_snapshot: time
source_pin_short12: a039d9803ade
result_base_short12: 1a6ef1c7a5aa
fixed_contract91_short12: 4de8f7b487d3
read_channel: GitHub_connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
prior_return_items: []
phase: 2
runtime_evidence: NOT_RUN
---

# 第61批：fcntl命令、描述符复制与lseek位置控制

所有结论限定于固定time的源级有界链，不能用接口名或Linux惯例填补本仓注释掉的操作。到达后置return、fdput、fput、get_file、fd_install或lseek调用都分别要求有效对象、合法数组索引、可表示的有符号数和被调函数正常返回。首次无效指针/数组/整数计算后停止，不将源代码后半段冒充机器上已发生的动作。源码直接摘录见evidence.md；条件矩阵和63个有限数学/状态例见facts-and-boundaries.yaml。

## G00及历史消费

固定91号及同提交60主线回执、63/79号已完整读取。60按PASS_PENDING_LOCAL限定接收，R60-S01/S02继续；没有D00、没有新RETURN，不重验60。58 B25的主线错误指控已撤回（R58C-S01），旧审查/评论和全部旧文件冻结。40至43主线回执均完整重读，R40/41/42/43各S01/S02约束可用但不再次验收。08::US-01、05::UV-09/10/11由实际目录定位后定点回读原source字段，其中US-01、UV-10/11均deferred_owner_not_ready，UV-09是needs_evidence。旧FG41、CL42、FT43、DE59的消费身份在facts.prior_findings_links单列；旧问题不改状态，不认定所有根因已去重。

G00直接time=a039d9803ade、master=e8e60cd19540、lead/PR16=4de8f7b487d3、执行/PR17=1a6ef1c7a5aa；PR16/17 Open/unmerged且17 Draft，仅这两项开放。新user-fd-control-61目录写前404，time双金丝雀存在而master对应文本不存在。写ref前仍须再次核原HEAD及唯一写区，不能因为当前检查通过就忽略未知并行写入。第61批不修内核、不预处理/编译/运行。

## A / G01：fcntl命令准入和实际控制流

### A1. 原始fd借用与早停

syscall.c唯一MYOS_SYSCALL_DEFINE3(fcntl)宏调用体先执行fdget_raw(fd)，将err初值置-EBADF，f.file为NULL则goto out直接返回，不调用fdput。在所读file.h中fdget_raw只调用__to_fd，__to_fd直接从current->files->fd_array[v]取槽，并把fd_s.flags赋3；没有fd_count或固定256项的实际下标门，也没有f_count增量操作。因此只有真实下标合法且槽内NULL时，后置!f.file才是安全-EBADF来源。若下标越过物理数组、current/files无效，或槽指针非NULL但非法，应停在更早数组访问或随后的f.file->f_mode，不能声称安全报错。此范围属于旧UV-11与US-01的新消费者，不重复编号。证据A01/A02/A18–A22、B16/B17，R40/41。

f.file有效时，只有f_mode & FMODE_PATH为真才调用check_fcntl_cmd。若未获准，直接goto out1，保留外层-EBADF但**仍执行fdput(f)**；没有FMODE_PATH时直接调用do_fcntl而不执行该白名单。选定fdput检查flags & FDPUT_FPUT；flags=3且FDPUT_FPUT=1，于是请求fput(file)。所读fput在file.f_count递减归零时请求kmem_cache_free(filp_cachep,file)。这与fget对有效file加引用不是同一取回路径，不能靠名字fdget认定借用/归还已配平。正常对象上的-EBADF/-EINVAL/F_GETFL结果均可能伴随这次归还请求；归零后源槽是否仍指向活对象不能由此前有效性沿用。旧08::US-01（deferred_owner_not_ready）已经记录该根因。证据A03–A06/A19–A24/A32/A33。

### A2. 白名单和switch的两张不同表

check_fcntl_cmd在所读完整函数内仅对F_DUPFD、F_DUPFD_CLOEXEC、F_GETFD、F_SETFD和F_GETFL返回1，其余返回0。do_fcntl的活动switch只有F_DUPFD→f_dupfd(argi,filp,0)，F_DUPFD_CLOEXEC→f_dupfd(argi,filp,O_CLOEXEC)，以及F_GETFL→filp->f_flags；F_GETFD、F_SETFD、F_SETFL和其它lock/owner/pipe等case在当前文件中是注释，不执行。没有活动case时经过default提示语句而保留初始err=-EINVAL；提示打印的正常返回是模型前提，不是必然日志成功。

| 命令及真实值 | check_fcntl_cmd | 有效普通file执行 | 有效FMODE_PATH file执行 |
|---|---|---|---|
| F_DUPFD=0 | 1 | 调f_dupfd(argi,file,0) | 同样调用 |
| F_DUPFD_CLOEXEC=1030 | 1 | 调f_dupfd(argi,file,O_CLOEXEC) | 同样调用 |
| F_GETFD=1 | 1 | 注释case，不做GETFD，default -EINVAL | 白名单放行，但仍default -EINVAL |
| F_SETFD=2 | 1 | 注释case，不做SETFD，default -EINVAL | 同样default -EINVAL |
| F_GETFL=3 | 1 | 读file_s.f_flags返回该值 | 同样 |
| F_SETFL=4及其它未列命令 | 0 | do_fcntl default -EINVAL | 外层先拒绝，保留-EBADF |

F_GETFL读的是file对象的f_flags；F_GETFD原应关乎某个fd槽的FD_CLOEXEC，但所读实现没有活动GETFD路径，不能把两个字段混成一类状态。F_GETFL源码返回不证明f_flags是有效POSIX组合或某个用户实际读到了它；返回后仍有fdput。宏值分别回固定uapi头，F_DUPFD_CLOEXEC是F_LINUX_SPECIFIC_BASE+6，后者1024；O_CLOEXEC为独立八进制宏，不按推测替换。证据A07–A17/A23–A31；DC61-F01。

### A3. 参数转换与结果边界

do_fcntl签名的fd为int、cmd为uint、arg为ulong，进入函数时无条件执行argi=(int)arg，哪怕随后选F_GETFL根本不用argi。fcntl宏入口fd是unsigned int，传给do_fcntl的int fd也存在可能的超域转换，但这个fd在活动实现的三个case中未被读取（其它用途在注释中）；不代表转换未发生。对arg超INT_MAX，转换到int结果依实现，不能用Javascript任意精度断言一定是-1；下游f_dupfd还将argi转为uint from，必须保留第二次类型转换和有效rlimit门。facts给出明确LP64、signed32二补数的仅示范数值，覆盖0、1、255、256、2^31、2^32及大ULONG值，不等于真实ABI/编译器测试。早期fd无效时不应把尚未执行的argi转换当已发生。证据A01/A07/A08及R41-S01。

## B / G02：F_DUPFD复制fd的安装、别名和引用

### B1. from、rlimit和alloc_fd的最早停止点

f_dupfd(from:uint,file,flags:uint)先通过rlimit(RLIMIT_NOFILE)取得ulong nofile；所读rlimit包装读取current->signal->rlim[limit].rlim_cur，必须先有效求值current/signal。若from>=nofile立即返回-EINVAL，没有get_file、fd_install或新的fd槽。否则调用alloc_fd(from,nofile,flags)；但alloc_fd的start/end参数是uint，因此ulong nofile进入callee会发生窄化，不能未经说明把真实rlimit值当固定256或认为end总可表示。alloc_fd在file_lock下以int i从start扫描到end，访问files->fd_array[i]，首个NULL时给局部uint fd赋值并break；解锁后无条件return fd。既没有预留位写入，也不消费flags；找不到槽或空区间时fd没有已证赋值，不能补自动-EMFILE/-ENFILE，不能继续认证err>=0和后续安装。256是物理fd_array长度而非本函数执行上界；end=257但0号槽就空可提前安全命中，若0..255全忙并尝试256才在数组访问处失效。无证不能宣称所有end>256调用都必然越界，必须列首次非法访问。R40-S01/S02与UV-11的历史根因不重编号。证据B01–B10/B16–B20。

### B2. 成功复制的持有事件链

只有alloc_fd返回已证合法、非负空槽且file有效，f_dupfd才调用get_file(file)，该函数向file.f_count请求+1，然后调用fd_install(err,file)。fd_install取current->files及内嵌数组，执行smp_rmb，按所选BUG_ON宏读取槽占用，最后写fps[fd]=file；无本地容量比较、无新file_s分配、也无本地fcntl flag写入。BUG_ON不等于物理边界门，宏绑定和中断/异常仍未预处理运行；下标、对象或callee若更早失效，后面的fdput和新槽持有均不成立。

有效无其它写者的局部模型：源file.f_count初值1，fcntl通过fdget_raw借到flags3但没有+1；F_DUPFD成功在get_file时1→2；fd_install使原fd与新fd两槽指向同一file_s；外层fcntl走out1的fdput触发fput，计数2→1。因此**一个新槽出现，但此包装链净计数不增加**。这不是f_dupfd单独不加引用；f_dupfd确实+1，差额来自已有US-01上层额外fput。初值2/3同理n→n+1→n；如果第二次再正常成功，第三个槽又指向同对象，但计数仍可回到初值。没有全系统引用所有权规范与真实生命周期，不据此直接签已发生UAF/内存泄漏；跨close时须另读既有UV-10/R42，不能让一个空槽变动模型同时扮演物理释放事件。证据B03–B05/B11–B15、A18–A22，DC61-F02。

失败分支也不同。from>=nofile正常返回-EINVAL时没有get_file，但fcntl外层若先成功借到file，仍会fdput：初计数1→0将请求filp_cachep释放，源fd槽未由该函数删除。这是US-01旧根因在错误命令上的消费者，而不是一个新发现的独立文件系统错误。alloc_fd的未赋fd、fd_install越界或可能BUG_ON中断则先行停止，不能假造再做一次fput配平。

### B3. CLOEXEC及两个dup宏调用体

F_DUPFD_CLOEXEC确实向f_dupfd传O_CLOEXEC，再向alloc_fd传flags；但当前alloc_fd只做扫描、锁调用与返回，并不使用flags，fd_install也只写file指针。files_struct_s的close_on_exec相关位图字段仅出现在注释里，不能仅从函数实参O_CLOEXEC认证新fd记录了FD_CLOEXEC，更不能推断真实exec一定保留或一定关闭该fd。F_GETFD/F_SETFD的设置/读取活动体缺失另见A。证据A10/A26/A27、B04/B07/B12/B16/B23/B24；DC61-F03。

syscall.c的dup和dup2各自唯一MYOS_SYSCALL_DEFINE宏调用体里仅有ALERT_DUMMY_SYSCALL活动宏调用，其余所谓获取file、fd安装或ksys_dup3以及return全部是注释。固定syscalls.h定义IF_ALERT_DUMMY_SYSCALL=0；ALERT_DUMMY_SYSCALL本体是if(show_alert)时打印，并没有提供return。所选x86-64 wrapper宏的函数声明返回long。故在选定宏绑定、正常到达函数末尾的条件下，源码未提供可认证的返回值；不能从注释推断dup/dup2成功安装或必然返回某个errno，也不能宣称编译器生成了某种特定机器返回。没调用预处理器，不把dummy展开作为已编译事实。此处是dup/dup2 wrapper与f_dupfd真正活动链之间的区别，和fork.c::dup_fd按值复制files表/各个file对象更是不同概念。证据B25–B36；DC61-F04。

## C / G03：lseek与FAT文件位置

### C1. lseek系统调用前置门

syscall.c的lseek宏调用体参数fd为unsigned int、offset为off_t、whence为unsigned int，局部ret为ulong并初始化0。先比较fd<0或fd>=current->files->fd_count，因fd是unsigned，fd<0在所选类型语义下恒假；有效files与比较正常完成且fd>=fd_count时可以-EBADF。之后whence<0或whence>=SEEK_MAX，whence也是unsigned，前一条件恒假。真实宏SEEK_SET=0、CUR=1、END=2、DATA=3、HOLE=4、SEEK_MAX=4。因此whence3通过外层范围门，whence4在外层返回-EINVAL。仅在两个门通过之后，直接读取current->files->fd_array[fd]并执行if(fp->f_op && fp->f_op->llseek)。物理数组仍只有256项，逻辑fd_count不能独立保证访问合法；槽值NULL时首次fp->f_op成员读取先失效，不能安全得到ret初值0。证据C01–C06/C18–C23/C28，原UV-11。

若fp有效而f_op为NULL或llseek回调为NULL，活动if为假，未调用任何定位函数，ulong ret仍为0并由宏返回；即使原f_pos为5，也不能把返回0说成已把位置定位到0。只有实际file.f_op已绑定FAT32_file_ops且对象仍有效，才允许进入下面的FAT方法。这个操作表确实设.llseek=FAT32_lseek，但静态声明不证明所有file都选择该表。证据C05/C06/C14–C17；DC61-F05。

### C2. FAT32_lseek的目标位置与失败是否写回

FAT32_lseek形参offset为loff_t、origin为int，本地先读取filp->f_path.dentry->d_inode，再初始化long pos=0。有效FAT文件与inode/i_size为前提；它不是对空指针的防御检查。SEEK_SET将offset赋给pos；SEEK_CUR计算filp->f_pos+offset；SEEK_END计算inode.i_size+offset；默认返回-EINVAL。随后只有在pos>=0且pos<=inode.i_size时才执行filp->f_pos=pos并返回pos，否则返回-EOVERFLOW且在所读方法内未修改f_pos。该实现不允许该文件位置超出EOF，但规范是否要求更大值不由此认定。

以有效i_size=10、原f_pos=5、类型有界且无其它写者为例：SET0→新位置0、SET10→10、SET11/SET-1→-EOVERFLOW且保持5；CUR+3→8、CUR-5→0、CUR-6→负目标被拒；END-2→8、END0→10、END+1→拒绝。外层放行的SEEK_DATA3在FAT默认分支返回-EINVAL，SEEK_HOLE4被外层先拒绝，不能说所有whence<4都由FAT支持。所有算术须在可表示范围内成立：CUR/END目标加法若先产生有符号溢出，则在那一步停止，不能跨过去认定后续-EOVERFLOW；pos由loff_t计算值写入long也需目标可表示范围。证据C07–C13/C18–C23、R60-S01类型限制；DC61-F06。

### C3. 返回类型、别名与59位置语义的分离

所读typedef显示off_t通过__kernel_off_t为long、loff_t通过__kernel_loff_t为long long，FAT位置本地为long，syscall暂存ret为ulong，宏wrapper定义返回long；即使典型x86-64 LP64下它们宽度相同，这仍是不同的有/无符号类型。FAT方法负返回例如-EOVERFLOW被赋给ulong会先按无符号模数转换，再由long返回上下文消费；超出long可表示值转回有符号依目标实现，不能未经编译/ABI确认直接签用户收到原负错误码。有效正位置属于可表示范围可按源码保存，错误和正常位置两条路径分离。证据C01/C02/C06/C24–C27及B35/B36；DC61-F07。

若F_DUPFD两个fd槽在合法模型中都指向同一file_s，二者也共享这个file对象的f_pos；一个有效FAT32_lseek成功赋值后，另一槽若仍合法地访问同对象可见相同位置值。这不是复制两个file对象，也不是完整SMP/引用生命周期证明；若US-01或close导致对象已失效，就必须在较早访问停止。第59批FAT32_getdents64另有ctx.pos==-1和非-1时重新init iobuf游标的逻辑，不能将其等同通用lseek设置f_pos的规则。这里只按R59-S01/S02作接口消费对照，不重验目录枚举。DE59-F03等旧状态保持。证据B05/C13/C14/C16；R59-S01/S02。

## 有界验收入口

A、B、C各completed_in_scope，表示本批固定time的源码调用和失败前缀审查完成，不意味着内核功能可靠或已运行。新DC61-F01–F07共有7条有前提的延期记录，DC61-U01–U09共有9项证据缺口；其中F02明确作为既有US-01的复制消费者，数组边界与直接关闭沿UV-11/UV-10，不机械制造独立根因。facts里有17条source_ledger、fcntl_dispatch_matrix、dup_alias_reference_traces、seek_position_return_matrix、63个自建有限数学/状态模型和真实旧qualified_id。

证据文件有97段（A33、B36、C28）固定time单定义内、原样连续1–5行短引文。关系图是有限CALL/RETURN/DATA/LOCAL_CONTROL/ANALYSIS_STOP摘要，不是完整CFG，解释节点不是新增活动门，也不显示无效访问后一定继续执行。没有内核补丁或旧文件改动；未运行仓库C/ASM/脚本、旧夹具、预处理/编译/链接、完整本地P9、Graphviz、QEMU/硬件、实机fd、实际FAT或用户空间ABI。仅READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING，最高由MYOS2-A-C05独立限定接收；不自签PASS、不自行启动62。
