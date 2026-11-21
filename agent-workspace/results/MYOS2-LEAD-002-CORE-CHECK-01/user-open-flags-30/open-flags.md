---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-OPEN-FLAGS-30
record_type: bounded_open_flags_normalization_validation_and_caller_consumption
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
conversation_display_name: MYOS2-A-C03c 代码分析执行续建
mainline_reviewer: MYOS2-A-C04 内核分析主线续建
date: 2026-11-03
read_channel: connector
source_pin_short12: a039d9803ade
task52_short12: 1412a1991aee
review29_short12: 1412a1991aee
result_base_short12: 239e2f8efc45
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
---
# 第30批：打开标志正规化、返回检查和部分初始化对象的消费

## F00｜固定输入、接收和历史限界

本批沿用原C03c执行身份`MYOS2-LEAD-002-CHAT-EXEC-03`，已完整读取`52-user-open-flags-contract.md@1412a1991aee`及**同一固定提交**下的`CORE-USER-PATHNAME-COPY-29-review.md`。29正式回执`PASS_PENDING_LOCAL`、`returned_scope=[]`，必须消费R29-S01/S02/S03：数值门和真实访问/返回生产者分层、`getname`及资源出口有前提、真实引文和MANIFEST标准字段必须严格一致。本批不再执行29测长复制、28的ELF分析，亦不重新全面审05/08。

冻结源码`time@a039d9803ade`，起始执行成果为`239e2f8efc45`，主线属于C04，执行原PR17分支`claude/dazzling-cori-q0dnyt`，只允许`user-open-flags-30/`六件及本人PR17评论。固定配置金丝雀及master/time/执行头、开放PR、PR16路径交集和新目录预检均在MANIFEST实录。历史`CORE-USER-VFS-BASELINE-05::UV-04`的缺return具体位于`do_open`及`myos_do_dentry_open`，不是`do_filp_open`自身；本批读到后者确有`return filp`。08::US-03此前已登记close-on-exec生命周期缺口，本批只追加O_CLOEXEC参数传递事实，不重新对整个fd链或标准功能作裁定。

证据入口`evidence.md`以`OF30-A001`等编号标示固定源码`path::symbol`的1–5行真实引文。逐行被引的C文本、该函数完整选段、相关宏定义三者合读才可支持下面的条件事实。没有运行内核、系统调用或编译器。

## F01｜legacy open从int flags/mode得到open_how

### 类型、复合对象及入口身份

`mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(open)`的`flags`为`int`，`mode`为`umode_t`（所选typedef为unsigned int），它实际调用`do_sys_open(0,filename,flags,mode)`。后者构造`open_how_s how=build_open_how(flags,mode)`再传给`do_sys_openat2`；虽然helper与头文件使用`openat2`名字，所读当前syscall入口只有上述`open`，不能仅从struct/openat2头文件存在推出真实openat2系统调用已接通。证据：OF30-A035/A036/A053/A080。

`open_how_s`由三个`__u64`字段`flags/mode/resolve`组成。`build_open_how`实际使用C聚合初始化`{.flags=flags&VALID_OPEN_FLAGS,.mode=mode&S_IALLUGO}`，**没有显式列出`resolve`**；按照C聚合初始化语义，该未列出的整数成员被零初始化，而不是保留栈垃圾。旧入口首先屏蔽未知open标志与权限位，再若flags含`O_PATH`就执行`how.flags &= O_PATH_FLAGS`，最后`!WILL_CREATE(how.flags)`时把`how.mode`设0。`WILL_CREATE`检查`O_CREAT|__O_TMPFILE`；`O_PATH_FLAGS`允许`O_DIRECTORY|O_NOFOLLOW|O_PATH|O_CLOEXEC`，因此`O_PATH|O_CREAT`的**legacy**输入会在这一层先清掉O_CREAT，连带mode重置。这与“未经过正规化的直接build_open_flags输入”不同，不得把后者的O_PATH组合错误直接列为已证sys_open错误生产者。证据：OF30-A001–A005/A037/A040/A050/A053/A059。

`VALID_OPEN_FLAGS`含`O_RDONLY`（数值0）、`O_WRONLY`(1)、`O_RDWR`(2)、`O_PATH`、`__O_TMPFILE`以及`O_DIRECTORY`等位。`O_NDELAY=O_NONBLOCK`为别名，`O_SYNC=__O_SYNC|O_DSYNC`，不能重复作为不同的独立输入位；这些宏的出现是**可接收位或构造定义**，不认证其每一种文件系统行为。普通mode掩码`S_IALLUGO`在本源码为`07777`（十进制4095），`S_IFREG=0100000`（十进制32768）。原O_CREAT输入时模式位被掩盖，而无创建意图时无论原mode是多少都被清零。数据类型、别名和权限定义见OF30-A039–A065。

### 访问模式并非把四种低位值都当作非法输入

`build_open_flags`取`uint64_t flags=how->flags`，先计算`int acc_mode=ACC_MODE(flags)`，再用`flags &= ~(FMODE_NONOTIFY|O_CLOEXEC)`去掉内部通知和fd close-on-exec位。这里ACC_MODE不是简单`O_ACCMODE`原数值，而是字符数组`"\004\002\006\006"[(x)&O_ACCMODE]`：低两位0/1/2/3分别产生4/2/6/6，对应`MAY_READ=4`、`MAY_WRITE=2`位组合。特别地，低位3也被选定代码当作读写数值，不在此处直接拒绝。若后面通过O_PATH允许位检查，则`acc_mode`会被重置成0；`O_TRUNC`或`O_APPEND`又分别OR入`MAY_WRITE`或`MAY_APPEND`。这仅是权限请求整数的计算，真实权限结果不在本包认证。证据：OF30-A006/A007/A013/A015–A017/A042/A063/A065。

### 两组独立输入不能混用

1. 旧`sys_open`路径：`build_open_how`已经把未知flags屏蔽、mode限制、`resolve`设0。因此直接`build_open_flags`中的未知flags、未知resolve、BENEATH与IN_ROOT互斥、O_PATH不允许组合、非create带非零mode等错误门，并没有从这个稳定旧入口直接成立的生产者。
2. 原始helper条件模型：若某个获准调用者直接构造带未知bits或非零resolve的`open_how`，可触发相关-EINVAL/-EAGAIN返回；但**尚未证明实际user-space openat2入口调用了它**。这类算例用于解释函数定义和未来API范围，不应标记为今天真实sys_open可复现的问题。

有两类错误确实可以由旧标志位构成源码输入条件：用户`flags=__O_TMPFILE`而未带`O_DIRECTORY`时，`WILL_CREATE`按`__O_TMPFILE`为真，但helper的`O_TMPFILE_MASK`组合门拒绝；用户`flags=O_TMPFILE|O_RDONLY`时，组合完整，`ACC_MODE(0)=MAY_READ`而缺MAY_WRITE，被写访问门拒绝。这里说的是**参数经正规化后可产生-EINVAL的局部控制流**，不是已运行的用户程序或成功进入VFS。详见facts::OF30-G06/G07及值表。

## F02｜build_open_flags的顺序与错误点字段快照

### 初始化不是全有或全无

`open_flags_s`在固定internal.h里是五字段对象：`int open_flag; umode_t mode; int acc_mode; int intent; int lookup_flags`，调用者声明`open_flags_s op;`时没有自动清零。helper开始仅定义`flags`、`strip`、局部`lookup_flags=0`及`acc_mode`，这几个都是**局部变量，不是写入op结构**。随后按如下顺序触及目标：

- **第一处写op.mode：** 已通过未知flags/resolve、互斥resolve和mode检查后，create-like为`how.mode|S_IFREG`，否则为0。任何更早的-EINVAL在本层没有写过op任何字段。
- **第二处写op.open_flag：** 完成__O_TMPFILE组合/写访问、O_PATH允许位检查后，把局部flags中的`__O_SYNC`规范成包含`O_DSYNC`，再执行`op->open_flag=flags`。
- **第三/第四处写op.acc_mode与op.intent：** 在O_TRUNC、O_APPEND更新局部acc_mode后写入acc_mode，按O_PATH选择`0`或`LOOKUP_OPEN`，O_CREAT/O_EXCL还会OR入`LOOKUP_CREATE/LOOKUP_EXCL`。**此时**`O_CREAT|O_EXCL`分支会再执行`flags |= O_NOFOLLOW`，但`op.open_flag`已经写过，不会因局部flags后续变化自动同步。
- **最后写op.lookup_flags：** 使用后来的局部flags决定`LOOKUP_DIRECTORY`、`LOOKUP_FOLLOW`（没有O_NOFOLLOW才有），然后将各`RESOLVE_*`转换为`LOOKUP_NO_XDEV/NO_MAGICLINKS/NO_SYMLINKS/BENEATH/IN_ROOT`。若RESOLVE_CACHED，先做晚到的拒绝判断，未拒绝才加`LOOKUP_CACHED`；之后才执行`op->lookup_flags=lookup_flags; return0`。

证据：OF30-A006–A025/A037/A066–A068。注意在`O_CREAT|O_EXCL`且原输入无O_NOFOLLOW的模型中，`op.open_flag`保持旧值、用于计算`LOOKUP_FOLLOW`的局部flags是新值，因此`lookup_flags`不含FOLLOW；不能用“字段名称相近”反推已经同步。是否影响目标文件创建/符号链接的语义，要看后续真实字段消费与历史VFS限制；此处仅登记OF30-F02条件项。

### 严格错误门矩阵

| 返回位置及真实门 | 返回值 | 已写入的op字段 | 未写字段 |
|---|---|---|---|
| 过滤后的未知flags或未知resolve | -EINVAL | 无 | 全部五项 |
| BENEATH与IN_ROOT同时存在 | -EINVAL | 无 | 全部五项 |
| WILL_CREATE且mode非法／非create而mode非零 | -EINVAL | 无 | 全部五项 |
| __O_TMPFILE组合不完整或缺写访问位 | -EINVAL | 仅mode | open_flag、acc_mode、intent、lookup_flags |
| O_PATH携带不允许位 | -EINVAL | 仅mode | open_flag、acc_mode、intent、lookup_flags |
| RESOLVE_CACHED与源表达式中的`(O_TRUNC|O_CREAT|O_TMPFILE)`相交 | -EAGAIN | mode、open_flag、acc_mode、intent | lookup_flags |
| 所有门均通过 | 0 | 全部五项 | 无 |

详细每门的输入来源、必备前提和锚点见facts::OF30-G01..G10。特别是RESOLVE_CACHED中的`O_TMPFILE`实际定义为`(__O_TMPFILE|O_DIRECTORY)`，因此**单独O_DIRECTORY也会命中**晚到的-EAGAIN测试；该交集来自真实位表达式，而不是注释文字的“仅创建/截断/tmpfile”缩写。当前旧open的`how.resolve=0`，该条件是原始helper输入模型，不是当前已证用户sys_open错误。见OF30-A024/A051/A053及OF30-F03。

`O_CLOEXEC`作为文件描述符标志在helper局部`flags`里被去掉，所以`op.open_flag`不携带它；但`how.flags`未因helper的局部strip而改变，后续仍以`get_unused_fd_flags(how->flags)`传入。该两条边只证明数据去向，**不认证close-on-exec已经保存并生效**。其真实生命周期已有`CORE-USER-SYSCALL-STDIO-08::US-03`历史记录，不重新计为30新缺陷（OF30-A007/A030/A032/A048）。

## F03｜调用者是否消费错误、何时读取op

### 用户open的第一次校验错误被覆盖

实际入口`sys_open → do_sys_open → do_sys_openat2`：先`int fd=build_open_flags(how,&op)`，没有紧跟任何`if(fd<0)`或`if(fd)`门；随后`tmp=getname(filename)`，若`IS_ERR(tmp)`，立即以`PTR_ERR(tmp)`返回；**否则无论最初的build_open_flags是否返回负值**，继续`fd=get_unused_fd_flags(how->flags)`覆盖第一次fd并取新的结果。只有第二次`fd>=0`才调用`do_filp_open(dfd,tmp,&op)`；若第二次fd为负，直接跳过该调用并在尾部putname(tmp)后返回新错误。该路径没有消费第一次的`-EINVAL`或`-EAGAIN`。因此：可从旧open输入得到的__O_TMPFILE不完整/只读组合错误，若在getname和fd分配两个独立条件门后继续，**会把只写过op.mode的对象传进do_filp_open**。这不等于下层已成功打开文件；读取未初始化的int字段本身有未定义值问题，不能跨过它宣称具体权限或CPU结果。证据：OF30-A030–A034/A035/A036、facts::OF30-C01。

`do_filp_open`第一步用`int flags=op->lookup_flags`，接着将op传到`path_openat`；后者还会取`op->open_flag`交`alloc_empty_file`。这已经足以证明当helper失败且只写了mode，第一层至少**试图读取尚未由build_open_flags赋值的lookup_flags**；不需要为了得出该本地结论假设实际成功完成pathwalk。不能把同一个`do_filp_open`说成“没有return”：它有`return filp`，历史05::UV-04涉及更深的`do_open`和`myos_do_dentry_open`缺return（OF30-A069–A073）。这意味着本批仅有错误传播/字段消费的条件链，不是完整VFS成功/失败结果的认证。

### 内核file_open_name明确检查

`file_open_name(filename_s *name,int flags,umode_t mode)`同样先调用`build_open_how`，再用局部`int err=build_open_flags(&how,&op)`；若`err`非零直接返回`ERR_PTR(err)`，**不调用**`do_filp_open`；只有返回0，才把五个已由helper成功路径赋值的字段交给`do_filp_open(AT_FDCWD,name,&op)`。它与用户`do_sys_openat2`共享helper却不同于后者的返回消费。再往外的`filp_open`先`getname_kernel`并检查是否错误，合法时调用`file_open_name`，正常返回后`putname`；那是29号已审的filename生命周期，不在30重复开展。证据：OF30-A026–A029、facts::OF30-C02。

### do_open_execat是另一条明确构造分支

`do_open_execat(fd,filename,flags)`并不调用`build_open_flags`：其`open_flags_s open_exec_flags`局部聚合显式写`open_flag=O_LARGEFILE|O_RDONLY|__FMODE_EXEC`、`acc_mode=MAY_EXEC`、`intent=LOOKUP_OPEN`、`lookup_flags=LOOKUP_FOLLOW`，未显式列出的`mode`按C聚合初始化为0。它首先限制调用用的`flags`仅含`AT_SYMLINK_NOFOLLOW|AT_EMPTY_PATH`，不允许的位返回ERR_PTR(-EINVAL)，再按有效AT标志更新lookup_flags，然后才调`do_filp_open`。普通`open_exec`包装实际传0，因此本次是直接构造的执行意图，不套用用户open的`ACC_MODE`、O_PATH或O_TMPFILE正规化。具体执行文件是否可被打开，仍依赖下游路径与权限，不因MAY_EXEC数字已经写入就宣称被执行。证据：OF30-A074–A079、facts::OF30-C03。

### resolve意图不等于已经生效

`build_open_flags`对RESOLVE_*的转换语句都存在，但`open_how`源自旧open时`resolve=0`，当前所读`syscall.c`没有独立openat2活动入口；`do_filp_open`只表明把lookup_flags数值传入更深namei路径。没有开展`path_openat`下完整BENEATH/IN_ROOT/NO_SYMLINKS等强制执行逻辑的逐条件证明，不宣称全部openat2语义可用，也不宣称不存在。OF30-F04仅登记这个证据边界，而不是已证漏洞（OF30-A021–A025/A053/A069/A070）。

## F04｜延期、六件自查及正式停止线

本批`deferred-findings.yaml`中条件项`OF30-F01..F04`：用户open未消费build_open_flags错误并可能传部分初始化op、O_CREAT/O_EXCL之后的局部flags与op.open_flag分歧、CACHED复合O_TMPFILE掩码包含DIRECTORY、resolve映射的下游执行未闭合。这些并非四次已经发生的实际漏洞；F01错误使用还带有getname有效、fd>=0及下游定义良好执行的前提。旧`CORE-USER-VFS-BASELINE-05::UV-04`、`CORE-USER-SYSCALL-STDIO-08::US-03`及R29-S01/S02/S03仅作旧qualified_id补证，不修改其状态或重复计新独立根因。

`facts-and-boundaries.yaml`是YAML 1.2兼容JSON结构，含18个对象、10个逐返回门字段快照、4组调用者、15个纸面位算式；`open-flags-paths.dot`标清真实函数调用、宏值、字段写入和未消费返回。`evidence.md`有80段固定time原样引文，每段1–5行，源文件及字节核对和例外在MANIFEST。以上结构与源码检查均为GitHub连接器及Code Mode JavaScript的文本/有限整数工作，不能替代C04独立验收、用户程序执行或内核运行验证。

六件严格是本报告、`MANIFEST.md`、`facts-and-boundaries.yaml`、`open-flags-paths.dot`、`deferred-findings.yaml`及`evidence.md`。无额外文件/生成脚本/补丁；没有运行仓库C/ASM、预处理/编译、QEMU/硬件、真实用户open、外部libc/镜像或收费API。只申请阶段2 `READY_FOR_MAINLINE_REVIEW`，`mainline_acceptance=PENDING`，静态接受上限`PASS_PENDING_LOCAL`。最终Git提交和固定/活跃分支双重回读只以实际结果记在本人PR17交付评论，不在本正文提前编造自身提交SHA；正式交付后执行者停写，交C04验收，不自启31。
