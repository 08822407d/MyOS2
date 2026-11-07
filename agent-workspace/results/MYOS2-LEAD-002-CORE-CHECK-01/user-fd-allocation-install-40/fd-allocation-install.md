---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FD-ALLOCATION-INSTALL-40
record_type: R39_document_return_correction_and_independent_fd_state_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-04
date: 2026-11-05
read_channel: connector
base_snapshot: time
source_pin_short12: a039d9803ade
result_base_short12: 4ea455865a47
contract65_short12: a70d22015ae5
prior39_verdict: RETURN
prior39_returned_scope: [R39-01]
prior39_return_kind: DOCUMENTATION_ONLY
prior39_correction_status: CORRECTION_SUBMITTED_PENDING_MAINLINE
prior39_closure: PENDING_C04_REVIEW
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第40批｜第39批定点文档纠正 + fd空槽选择、安装与错误消费的两条独立证据线

## F00｜第39批追加勘误：R39-01仍为RETURN、交C04裁定闭合

Owner授权本对话原`MYOS2-LEAD-002-CHAT-EXEC-04`继续，固定65号任务书、同提交`a70d22015ae5`的第39批`RETURN`回执（R39-01、R39-S01、R39-S02）及63号GitHub写入重试规则三件**均完整读取**。第39批退回仅文档而非已证真实内核运行失败，`returned_scope=[R39-01]`，当前只申请`CORRECTION_SUBMITTED_PENDING_MAINLINE`、`closure=PENDING_C04_REVIEW`；不能由本执行者标为CLOSED_DOCUMENTATION_ONLY或PASS。之前36/37/38的独立验收保持既有状态。

### F00.1｜旧OC39-A069的宏身份纠正

固定time源文件`mykernel/include/uapi/asm-generic/errno-base.h`确切定义：
- 第29行**`ENFILE=23`**，由本次`evidence.md::R39C-A001`逐字单宏验证。
- 第24行**`EXDEV=18`**，由本次`evidence.md::R39C-A002`逐字单宏验证。

第39批旧证据`OC39-A069`却用标签`::ENFILE`标记第24行的`EXDEV=18`文本。**其原文作为EXDEV摘录可以逐字相符，但作为ENFILE证据身份错误**；不能据此断言第39批作者此前的字节对比必未执行，也不得再用A069支持ENFILE值或源码有无。旧`OC39-A001..A068`68条仅保留为历史摘录，**本轮未重新逐条认证其字节、声明归属或语义**；不通过“原69/69”笼统追认全部69条为合格单定义体。

原第39批所有旧文件和本人旧评论`6648914733`不改写，追加消费如下，均以本节及`facts.prior39_consumption`为准：

| 旧材料 | 追加后正确消费方式 |
|---|---|
| 原39`evidence.md` | `OC39-A069`作错配历史标签，正确ENFILE引用改用`R39C-A001`；`R39C-A002`仅说明旧摘录其实是EXDEV |
| 原39`MANIFEST.md::self_check` | 原`verified_claims=69, quotes_reconfirmed=69`可记录其作者旧文本检查申报，但**不能证明69个符号身份都正确**；本次仅新证据8条，旧68未新核 |
| 原39`open-completion.md`及其`alloc_empty_file`说明 | 该函数完整活动体已有`return f`，后置`over: return ERR_PTR(-ENFILE)`没有可见活动跳转；原活动路径不能由该标签自动产生`-ENFILE`。宏值应按`ENFILE=23`消费 |
| 原39`facts/deferred/DOT` | 数值/控制解释受本节替代限定，不将旧错误标签当作正确宏定义，不重做整批39图 |
| 原PR17评论`6648914733` | “69/69引文核证”只按**旧作者文本比对申报**读；其中一项标签/符号身份错误已由本次证据替换，不能理解为本轮69条全部重核或已重新验收第39批 |

本轮新`R39C-A001..A008`为**8段**直接定义体引文，与另行新增`FD40-A001..A047`的**47段**完全分开计数。旧69条不计入本轮55条。旧A069的错配不更改`alloc_empty_file`所选活动流无ENFILE生产者的静态观察。

### F00.2｜R39-S01/S02精确附注

`R39C-A003`直接证明`__FMODE_NONOTIFY=((__force int)FMODE_NONOTIFY)`，`R39C-A004`锚定`OPEN_FMODE`真实宏；`R39C-A007/A008`说明`build_open_flags`先提取要剥离的FMODE_NONOTIFY，再经`flags &= ~strip`处理，故成功通过该规范化helper的位集合比对任意int进行OPEN_FMODE的helper-only输入更窄。缺少实际编译/预处理验证，不把这个静态模型推广到任意配置或存在其它中途写者的情形。`R39C-A005/A006`分别证明内部`FMODE_32BITHASH=0x200`、`FMODE_64BITHASH=0x400`的真实宏：**八进制01000=十进制512=0x200；八进制02000=十进制1024=0x400**。它们在内部f_mode中不是“非法位”，只是由本轮选定的OPEN_FMODE生产者**不生成**；不能把两个域名称相似当语义相同。任意int输入还需要`flags+1`无溢出等有效性前提。

39的`f_mapping=inode->i_mapping`只复制指针值，即使是合法NULL值也可被复制；该赋值不是对mapping的立即解引用，更不认证后继访问的对象或生命周期。第38批的root mapping与子inode包含前提仍未闭合。39的无返回值缺口应在最早使用缺失`int`值的位置停止：`vfs_open`的return表达式、`do_open`、`path_openat`的状态读取不能被后续分支当已证正常运行。`IS_ERR(NULL)`为假，但这不是普通有效file指针的检验；同样不虚构实际NULL生产者。原`terminate_walk`活动体为空、`fput`局部体仅能证明相应调用，不作引用完整配平。以上均为限定解释，不重新打开39的其它验收问题。

---

## F01｜从当前files表找到一个整数：不是预留

`mykernel/include/linux/kernel/fdtable.h`中`NR_OPEN_DEFAULT=256`且`NR_OPEN_MAX=NR_OPEN_DEFAULT`，所选`files_struct_s`带`spinlock_t file_lock`、`unsigned long fd_count`、**内嵌`file_s* fd_array[256]`**，其中数组合法下标仅0..255（FD40-A001..A005）。这是所选结构的固定容量，不是Linux标准实现的动态fdtable假设。

`get_unused_fd_flags(uint flags)`活动体只是`return alloc_fd(0,current->files->fd_count,flags)`（FD40-A016/A047）。`alloc_fd(uint start,uint end,uint flags)`首先借入`current->files`，声明**未初始化的`uint fd;`**和未使用的`int error;`；在`spin_lock(&files->file_lock)`后执行`for(int i=start;i<end;i++)`。每一轮测试`files->fd_array[i]==NULL`，命中则**仅局部`fd=i`并break**，之后`spin_unlock`，无条件`return fd`（FD40-A012..A016）。完整活动体**不写**`files->fd_array`、open位图、next_fd或其它预留标志，且形式参数`flags`不被读；“mark it busy”仅是函数上方注释，不能凭函数名或注释补出预留效果。

在有效`start=0,end<=256`、无先行状态变化、数组均可读的有限模型中，如果第k个空槽被找到，本次仅返回整数k、**共享槽仍是NULL**；若范围为空或全部非NULL，则没有执行`fd=i`，最终表达式读取未初始化的局部`uint fd`，不能推称得到`-EMFILE`、`-ENFILE`、随机某数或负值。空表/满表均属此前`CORE-USER-VFS-BASELINE-05::UV-11`登记的旧根因，**本批不再另立新号**。

数值边界必须独立保留：`fd_count`是`unsigned long`而`alloc_fd`的end是`uint`，涉及转换；循环变量却为`int i=start`，需要start可表示与递增有效。源码只用`i<end`，**没有额外`i<256`边界**。若无空槽直到i=256但end仍更大，首次无效访问就在`files->fd_array[256]`，不得越过它制造一个正常fd结果。对init_files静态聚合可见`fd_count=0`；但只有在`current->files`实际就是此对象且无其它修改时才影响当前调用，不能推广为所有task的初值。另一选定入口`dup_fd`使用`kmem_cache_alloc`并显式设置`newf->fd_count=NR_OPEN_DEFAULT`，拷贝循环只为发现的旧非NULL槽赋值；这**不能**从分配器名字推出其它槽已清NULL，也不研究完整fork复制/文件引用（FD40-A006..A011）。

`f_dupfd`在另一条活动体里依据`rlimit(RLIMIT_NOFILE)`调用`alloc_fd(from,nofile,flags)`，返回非负才`get_file(file)`并`fd_install`。旧`CORE-USER-SYSCALL-STDIO-08::US-02`已经记录其较高rlimit上界与256槽容量之间的条件风险；当前只确认直接调用/传参，不重做fcntl、限制值或refcount全链。另旧`US-03`已经记录close-on-exec问题：本轮只证明当前alloc_fd不使用`flags`且没有本地CLOEXEC位写入，绝不单凭这里升级为全系统exec继承结论。

## F02｜fd_install的真实检查、写入与put_unused_fd

`fd_install(uint fd,file_s* file)`取`files=current->files`，`file_s**fps=files->fd_array`，执行`smp_rmb()`后是**`BUG_ON(fps[fd] != NULL)`**，最后才**`fps[fd]=file`**（FD40-A019..A021）。该函数**不对fd进行本地0..255上界判断**；因此`fps[fd]`本身的读取在任何BUG_ON占用判断之前就需要合法索引前提。合法空槽且宏执行后正常继续，才有真实的指针安装赋值；不是“查找整数”本身安装，也没有新申请file对象。`smp_rmb`是一个活动的内存屏障调用，并不自动等同获得file_lock、预留槽位或完成引用所有权转移。

`BUG_ON`在所选`asm-generic/bug.h`里有`#ifndef HAVE_ARCH_BUG_ON`条件下的宏体，条件为真时会调用`BUG()`，其备用定义体会调用panic（FD40-A025/A026）。但具体编译配置/体系结构宏绑定没有预处理认证；只能说“**有效索引且槽已占用时，BUG_ON条件为真**”，不能断定实机已经触发panic、固定CPU结局或能被此检查拦住无效索引。正常成功路径没有对`file->f_count`的局部增减；注释“consumes refcount”不是活动代码已执行引用转移的独立证明，完整所有权语义超出授权。

`put_unused_fd(uint fd)`完整活动体仅借入files，执行`spin_lock`和`spin_unlock`；清除open_fd位图、重置next_fd等是注释，**没有实际写入槽或位图**（FD40-A017/A018）。但由于本轮`alloc_fd`本身没预留，在这个选择器之后没有被保存的预留状态可证明必须“释放”；故不能凭缺少clear就宣称真实fd资源泄漏，也不能将put_unused_fd混成已安装file的close方法。

有限的**两次选择交错**只作分析模型：若A和B共享同一个合法`files_struct_s`，槽0在两个scan期间均为空，两次选择都可返回0，因为选择时不写槽；假设A先完成`fd_install(0,F)`，B再安装G时可读到原槽非NULL，进入BUG_ON条件。相反，若B在A安装后才开始扫描并另有空槽1，B可选1。这说明本地扫描与安装之间的**预留缺口**，不证明任何现实并发调度、共享files身份、实际竞争结果或机器panic（新`FD40-F01`）。边界测试`fd=256`与冲突检查也是helper-only/另有上游有效生产者才成立的条件，不能跳过首次数组越界消费（`FD40-F02`）。`put_unused_fd`的活跃撤销缺失单独存`FD40-F03`，不等于确认资源丢失。

## F03｜do_sys_openat2如何消费索引、文件错误与安装结果

`mykernel/fs/vfs/open.c::do_sys_openat2`先执行`int fd=build_open_flags(how,&op)`，紧接`tmp=getname(filename)`。若`IS_ERR(tmp)`为真会**早退`PTR_ERR(tmp)`，没有走到fd扫描**。若name有效，则`fd=get_unused_fd_flags(how->flags)`，覆盖了刚取得的校验值；随后仅在`fd>=0`才调用`f=do_filp_open(dfd,tmp,&op)`，若`IS_ERR(f)`则`fd=PTR_ERR(f)`，否则`fd_install(fd,f)`，最后`putname(tmp);return fd`（FD40-A027..A031）。这保留旧`OF30-F01`关于校验返回被覆盖的事实，不新增同义候选；也保留`UV-04`中`do_open/myos_do_dentry_open`缺有效int返回的缺口，不能用helper-only正常file强行证明用户open已成功到达fd_install。

分开消费几类前缀：
- 若**getname先产生编码错误**，直接返回该错误；本次未扫描或安装fd。
- 若扫描遇空区间/满表，alloc_fd返回未赋值局部读，后继`fd>=0`没有可签的定义良好结果；不能说它确定分配失败并返回某个errno，也不能走到fd_install以后再声称成功。
- 若合法选择一个fd而`do_filp_open`**确定返回被IS_ERR识别的错误指针**（如受限有效输入下的ERR_PTR(-ENOENT)），则do_sys_openat2会将局部fd改为PTR_ERR值并跳过fd_install。**已读alloc_fd从未改变该槽**；在没有其它共享写者前提下该槽仍为空。函数里没有put_unused_fd调用，但这里没有已证的预留资源可泄漏。
- 若另设**helper-only的有效普通file指针**且先前选中槽仍NULL、idx合法，`IS_ERR(f)`为假，`fd_install(fd,f)`才实际把指针发布到数组。该有效file需要额外证明；历史UV-04以及SB38-F02/F03限制当前真实用户路径的可达性。若helper-only返回NULL，`IS_ERR(NULL)`为假，调用fd_install可能把NULL写到空槽，不能称已安装有效文件，也无本包真实NULL生产者。
- 在抽象共享表双扫描模型，fd选择时只有锁保护读取，没有预留，fd_install没有相同加锁；其它行为不能被当作“单个原子分配+安装”。

本函数看不到`put_unused_fd`活动调用；错误路径缺该调用不能单独证明泄漏，因为源码根本没有预留。失败以后是否存在文件对象引用、路径引用、关闭或执行期间线程共享状态，均不展开。整个F03的最外层返回整数的条件必须包括被调用函数正常返回、有效索引与指针/文件、占用状态和必要宏绑定；**不展开fd安装以后的fget/close/dup/exec生命周期**。

`facts-and-boundaries.yaml`有22项身份、13行状态转移、8类函数返回与26组有限模型，分别标注source_conditional/helper_only/two_call_interleaving；`fd-allocation-install-paths.dot`只画第40批新fd链，**39勘误不画在DOT也不额外生成第七件**。图中来源箭头/有效性前提采用注释化数据关系而非C中的新函数调用或CPU终止命令。三个新`FD40-F01..F03`只是有前提的普通延期事项，已有`UV-11、US-02、US-03、OF30-F01、UV-04`继续按原状态消费，不重复报告为五个新的漏洞。

## F04｜六件证据、自查、提交规则与独立验收

规定且仅新增六件：`MANIFEST.md`、本文`fd-allocation-install.md`、`facts-and-boundaries.yaml`（JSON兼容YAML）、`fd-allocation-install-paths.dot`、`deferred-findings.yaml`、`evidence.md`。新引文**R39C=8、FD40=47、共55段**、9份固定time源定义，每段单定义体1–5行，经本轮第二次固定源比对后才作为作者核数；历史旧69个OC39标签不重算。片段文本相同与符号身份准确、索引有效和实际故障是不同审查维度。新报告只能申请`R39-01纠正已提交、PENDING_C04_REVIEW`以及`40分析READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING`，不能替C04自行CLOSED或PASS。

**NOT_RUN**：仓库C/ASM/脚本、预处理/编译、QEMU/硬件、真实fd/用户open/磁盘、完整文件描述符生命周期、fork/exec/close/dup/权限/调度/全局锁/分配器/引用/回收；未用额外付费模型/API。无内核或用户程序修补/补丁、旧文档或他人评论改写，未授权修改lead/PR16/master/time或PR标题/body/Draft，不创建分支/PR、合并关闭强推删分支、进入阶段3。自建有限整数/流程模型不等于内核测试或真实并发复现；完整本地P9门仍记`NOT_RUN`。

GitHub63号重试规程：暂态失败先回读活动HEAD、目标六件或评论是否已成功，再按默认10/20/40秒间隔或真实只读工作至多三次安全重试，遵守限流；已有blob/tree/commit身份复用，所有更新使用`expected-head`与`force=false`；未知执行并行写入停止覆盖。真实尝试、错误、等待和后置效果写MANIFEST/write_retry_log与本人评论，没有故障不编造次数。写前仍要最后确认原PR17 HEAD`4ea455865a47`、新目录不存在、PR16零交集；成功后**固定新提交与活动分支各六件全文/blob双读**，比较只能1个提交、六件A新增、旧件零净改删；最后本人在PR17追加唯一交付评论并读回。正式交付后`STOP_WRITING`，第39批文档闭合与第40批验收分开交C04，不自行开始41。
