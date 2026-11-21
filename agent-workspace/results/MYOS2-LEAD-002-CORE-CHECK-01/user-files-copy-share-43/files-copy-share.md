---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FILES-COPY-SHARE-43
record_type: bounded_files_struct_provenance_copy_share_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
produced_by: ChatGPT普通对话（Owner指定GPT-6＋Extra，实际后端不可独立认证）
source_pin_short12: a039d9803ade
contract_short12: 47bfd0c93b85
result_base_short12: 6303ec8e9899
date: 2026-11-05
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第43批｜文件表静态来源、共享、复制与解除共享的局部对象契约

## 1. 边界与对象身份

本批固定分析对象是 time@a039d9803ade 中的四层关系：`task_s`中的`files`指针、`files_struct_s`表对象及其`refcount/fd_count/fd_array`、各槽存放的`file_s*`、各`file_s`内的`f_count/f_pos/f_path/f_op/f_mapping`等字段。指针值复制、表引用数增加、重新分配一张表、按值另造一个file对象、给原file增加引用是**五种不同事件**，不能由相同标识符混成一种“复制文件”。来源FT43-A001–A004、A018/A019、A032–A035。

历史依赖：42主线的`R42-S01/S02`对有效对象、局部计数、原图和静态init_files绑定的限定继续有效；`CORE-USER-FD-CLOSE-42::CL42-F01`仍是needs_evidence。旧`CORE-USER-VFS-BASELINE-05::UV-09/10/11`、`CORE-USER-SYSCALL-STDIO-08::US-01/03`与40/41的各自限定保留；本批不重新立项它们已记录的按值file副本、跨缓存来源或完整关闭问题。没有实机、fork/exec/exit全链、分配器或并发认证。

## 2. T01：静态init_files与copy_files真实输入/输出

固定`init/init_task.c::init_task`的静态初始化项`.files = &init_files`，`fs/vfs/file.c::init_files`的`refcount=ATOMIC_INIT(1)`、`fd_count=0`，是确切静态绑定与初值；**它们不证明运行中的最终引用数和真实exit归还**。在本任务选定的任务复制路径，`copy_process`先调用`dup_task_struct(current,node)`获得p，再调用`copy_files(clone_flags,p)`。在有效前置分配/结构拷贝正常且无中途写者的前提下，`dup_task_struct`的实际`*tsk = *orig`使新任务p.files先带有current->files的相同指针值。注释的arch_dup_task_struct不是被调用的复制器，不能将不存在的外部helper结果当作事实。还须注意node/任务分配的先行有效路径与调用返回，不展开整个copy_process或任务分配器。证据FT43-A001–A014。

`copy_files`开始令`error=0`，读取`oldf=current->files`，分为以下三个**局部条件分支**：

| 输入（额外要求有效tsk及前置复制） | copy_files源码活动动作 | 返回、tsk.files的条件结果 |
|---|---|---|
| oldf=NULL | if(!oldf) goto out | error仍为0；本体**不写tsk->files**。若上游按值从current复制且没有其它写者，tsk.files原本即NULL |
| oldf为有效表且clone_flags含CLONE_FILES | atomic_inc(&oldf->refcount)后goto out；本体**不写tsk->files** | 正常return0；tsk.files依赖先行*tsk=*orig保持oldf地址；files表计数在有效正初值n上n→n+1；各file.f_count无本地增量 |
| oldf有效且不含CLONE_FILES | newf=dup_fd(oldf,NR_OPEN_MAX,&error)，若非NULL则tsk->files=newf并显式error=0 | newf有效时换成不同files表，原oldf表计数在本函数无增量；新表refcount=1；否则goto out返回error，tsk仍保留前置拷贝的旧指针，不能将其等价于独立持有的成功结果 |

CLONE_FILES由单宏`0x00000400`定义；本批不推导用户API flag组合的真实到达。只有先行`*tsk=*orig`确实在这个task对象上正常执行、`orig=current`相符且其`files`在相关时刻稳定时，才可解释shared分支未写tsk.files而结果仍为同址。仅看到`atomic_inc`不能证明指针继承，更不能证明跨线程并发安全。若clone在其它更早操作停止，不能把表引用计数增加当作必达。证据FT43-A005–A015。

`CL42-F01`的新增证据边界是：已证`init_task.files`静态绑定且在所选`copy_process`正常执行时可以把相同指针值交给新任务；共享分支可能按条件增加这张表的refcount，而非共享分支新建表。这提高了**来源链**的证据强度，却没有证明哪个真实运行任务最终对全局init_files作最后一次put；若运行中不存在最后归还，就没有该条件性静态对象缓存free请求。普通问题状态保持needs_evidence，不自动关闭或宣称已复现。

## 3. T02：dup_fd分配、复制范围、局部身份和错误

`dup_fd(oldf,max_fds,errorp)`先写`*errorp=-ENOMEM`；调用`kmem_cache_alloc(files_cachep,GFP_KERNEL)`。外层返回NULL时，经`goto out`返回NULL并把已写的错误值交给调用者。在新表有效时，源码执行`atomic_set(newf.refcount,1)`、`newf.fd_count=NR_OPEN_DEFAULT(256)`、初始化其`file_lock`，再令old_fds/new_fds分别指向新旧表的内嵌数组。它没有给所有新表槽位明确清NULL，分配调用本身不带明确的零化标记；files_cachep的定义由`proc_caches_init`提供且构造器实参是NULL。**这只证明在已读活动体没有空槽初始化语句，不证明实际物理数据一定非零**。旧UV-09已经记录这一来源风险，不另起新号。证据FT43-A020–A024、A030/A031。

循环用`unsigned int i`执行`i<max_fds`。没有读取oldf.fd_count或检查数组的256物理上限；`max_fds=0`时循环零次，仍可正常return newf（槽值未证）；`max_fds=256`时允许索引0..255；`max_fds>256`且前256次均有效完成时，i=256首次越过两张固定数组的物理下标范围，必须止于读取old_fds[256]，不得继续推导内层分配/返回。已读`copy_files`与`unshare_files`分别传入`NR_OPEN_MAX=NR_OPEN_DEFAULT=256`，因此helper的>256情形是输入条件模型，**不是已证这两个调用点实际越界**。循环范围与oldf.fd_count也不能互换：即使oldf.fd_count较小，仍按max_fds扫描各实际内嵌槽。证据FT43-A016/A017、A019/A020、A025。

旧槽非NULL时，源码执行`new_fp=kzalloc(sizeof(file_s),GFP_KERNEL)`、`*new_fp=*old_fp`、`new_fds[i]=new_fp`。当new_fp与old_fp均为有效对象、该下标物理合法且没有其它写者时，构造的是地址不同的一个file_s对象：f_count和f_pos被**按数值复制**而不是为旧file增加计数；f_path里的指针成员及f_op、f_mapping等指针值被复制，它们指向的对象不会因`*`赋值自动深复制。若旧两个槽都指同一个file_s，循环每个非空槽另行分配，得到两个独立file_s副本，而不是新表两槽继续别名同一个file对象；副本各有被复制的独立f_count/f_pos标量，但其内部指针仍可能与旧对象及彼此共享所指对象。此为旧UV-09的局部别名拓展，不能由该条件模型认证真实fork语义或引用配平。证据FT43-A026/A027、A032–A035。

旧槽为NULL则跳过if分支，本次循环**不对新槽写任何值**；“没有写NULL”与“新槽实际为未知值”及“新槽一定非NULL/一定NULL”分开。更重要的是内层`kzalloc`的结果在`*new_fp=*old_fp`之前没有NULL门：在旧槽非NULL、上游新表及索引都有效而`kzalloc`返回NULL的模型中，立即失去有效赋值目标，不能把它解释成`goto out_release`或正常返回NULL/-ENOMEM。选定`slub.h::kzalloc`经`kmalloc(size,flags|__GFP_ZERO)`包装，预处理绑定、真实分配失败率和实际机器行为未运行。这一**新的内层NULL处理缺口**作为`FT43-F01`条件性候选单独延期。证据FT43-A025–A029、A047。

成功时return newf；`out_release`标号存在并包含`kmem_cache_free(files_cachep,newf)`，但完整所选活动体中没有跳转到这个标号（外层失败去out，内层NULL无检查，正常提前return），不能当作一个可靠失败清理路径。注释的RCU发布并非活动执行。对任何失效旧指针、NULL新指针或越界索引都止于最早非法操作，不外推实际内存损坏/报错/完整回滚。成功返回时`errorp`仍可保持早先的-ENOMEM，但函数注释限定“返回非NULL时errorp无有效性承诺”；所读两个调用者以返回指针和各自局部error条件消费，不将这一状态误报为新bug。

## 4. T03：unshare_fd输出与unshare_files新旧表发布

`unshare_fd(unshare_flags,max_fds,new_fdp)`令`fd=current->files`、`error=0`，只有`unshare_flags & CLONE_FILES`且fd非NULL、`atomic_read(&fd->refcount)>1`时才执行`*new_fdp=dup_fd(fd,max_fds,&error)`；dup_fd返回NULL时立即返回error，否则正常返回0。**其它所有正常早跳过条件均没有`*new_fdp=NULL`语句，输出槽保持调用前原值**，不能推广为任何调用者都得到NULL。源文`fd &&`保护当前计数读取，但非NULL本身不认证有效表对象或锁/并发契约。证据FT43-A039–A041。

实际`unshare_files()`调用者先明确初始化`files_struct_s *old,*copy=NULL`，接着`unshare_fd(CLONE_FILES,NR_OPEN_MAX,&copy)`，在`error || !copy`时直接`return error`。因此（1）current.files为NULL、（2）共享表refcount<=1、（3）没有CLONE_FILES时（仅helper直接调用模型），都会留下copy原来的NULL且return0，不发生新表发布或put；（4）dup_fd正常返回NULL时，外层error经初始化/已写errorp传播，当前task.files尚未改写；（5）内层kzalloc NULL则缺有效返回前提，不能填充正常error返回。另有直接调用者若给未初始化的输出指针，helper的“无写”不能替其安全初始化。证据FT43-A039–A043。

当复制门满足且获得一个有效copy，新旧表字段在这段无其它写者时一致：`old=task->files`，`task_lock(task)`，`task->files=copy`，`task_unlock(task)`，然后`put_files_struct(old)`、`return0`。先发布新表、再归还旧表，是两个事件。局部计数模型中若旧表原值2，正常一次put变2→1不进入最后释放；若在条件检查之后计数或表指针被其它路径改写，就不能由之前>1推导后续old身份及安全性；锁名称不能证明整个过程原子。在`put_files_struct(old)`不返回或旧表已失效时，不宣称此函数最终返回0，但已发生的`task->files=copy`不能被编造成自动回滚。证据FT43-A036–A045。

一个真实调用入口位于`begin_new_exec`：在其前置控制流允许时执行`retval=unshare_files()`并在retval非零时goto out。此处说明连接端点，**不是**完整exec进入、成功替换进程映像、CLOEXEC、实际线程隔离或文件表全局生命周期认证。旧US-03仍按历史限定。证据FT43-A046。

## 5. 反证、失败止点和交回

关键条件模型：旧表NULL；共享标志且计数1或2；无共享标志的正常复制；max_fds=0/256/257；全空旧表与单个有效槽；两个槽别名；外层新表分配NULL；内层新file分配NULL；unshare无需复制/已复制/复制失败；发布之后旧表归还不返回。全部只在有效task/files/file及来源、可表示正计数、正常原子操作、无未建模写者与相应callee正常返回的有限模型内成立。具体状态转移、返回值和首个停止点见facts-and-boundaries.yaml；图中的PREMISE/STOP是分析标记，不是源码自带检查或CPU实际停机。

**新增候选只有`FT43-F01`**：内层kzalloc失败缺少NULL检查，属于有条件的本地错误处理缺口；既有CL42-F01凭静态绑定及本次局部传递链增加来源证据但仍needs_evidence，UV-09/10/11和US-01/03保持原状态。无权限实施补丁或重新选择共享语义。本批只申请READY_FOR_MAINLINE_REVIEW，主线接收上限PASS_PENDING_LOCAL；不构建、不运行、不审完整fork/exec/exit/slab/调度/锁RCU/VMA/文件系统IO。
