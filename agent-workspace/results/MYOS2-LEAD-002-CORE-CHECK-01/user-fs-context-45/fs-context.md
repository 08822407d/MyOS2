---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FS-CONTEXT-45
record_type: bounded_task_fs_context_copy_share_exit_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
produced_by: ChatGPT普通对话（Owner选择GPT-6＋Extra，实际后端未独立认证）
date: 2026-11-06
source_pin_short12: a039d9803ade
contract_short12: 1a33d35f634c
result_base_short12: 2d2bcab1348e
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第45批｜任务文件系统上下文 taskfs_s 的局部共享、复制、路径持有和退出

## 1. 三种对象和证据上限

本批只审 `task_s.fs` 及其所指 `taskfs_s` 的`users`、`in_exec`、`umask`、`root`和`pwd`，以及复制/退出中实际调用的`path_get/path_put`包装层。固定time@a039d9803ade下，`taskfs_s`来自`struct fs_struct`，`users`为普通`int`；与`files_struct_s.refcount`的`atomic_t`、`file_s.f_count`的`atomic_long_t`是不同对象、不同计数。一个task的fs指针、表对象和root/pwd中指向的dentry/mount都是不同身份，不能将`copy_fs`的users增量说成fd表或file引用的增加。证据FS45-A004–A006；两种旧计数的定点源码及44/43边界另外记录，不混计本批引文。

路径身份`path_s`本体包含`mnt`和`dentry`两个指针；`taskfs_s.root`与`taskfs_s.pwd`是两个**内嵌path_s值**。`fs->root=old->root`是复制两个指针值，不是复制dentry/vfsmount对象，也没有仅凭赋值完成引用取得。源码随后调用的`path_get`必须独立核实其实际活动体。证据FS45-A006、A037–A041。

## 2. S01：静态来源、任务指针预置和copy_fs真实三路

固定`sched/misc/fs_struct.c`定义全局静态`init_fs`，初始`users=1`，`umask=0022`；`init/init_task.c`的全局`init_task.fs=&init_fs`建立真实静态绑定。不能据此认定实际运行时users仍为1、root/pwd未改变、init_task必然执行退出或该对象必然归还到最后引用。内核历史其他路径可设置fs成员，本批不审完整启动/切换。证据FS45-A001–A003。

在44限定的有效fork前序中，`dup_task_struct`的`*tsk=*orig`可令新任务`tsk->fs`最初只是复制了旧任务的指针值；这一步**不**自行增加`taskfs_s.users`。同一实际`copy_process`调用`copy_files`成功后才到`copy_fs`，必须保留node条件、有效tsk、orig/current身份稳定以及中间调用正常返回等前提，不能把预置指针称为已取得新持有。证据FS45-A010–A018、R44-S01/S02。

完整`copy_fs(clone_flags,tsk)`先设置`fs=current->fs`，然后按`CLONE_FS`位分流：

| 输入条件 | 真实控制与对象事件 | 局部返回、任务fs状态 |
|---|---|---|
| CLONE_FS有位，fs有效且in_exec非零 | 读取`fs->in_exec`，先`return -EAGAIN` | users未在本函数增加；tsk.fs仍为前序借入指针。copy_process可按其非零检查goto cleanup_files，**跳过exit_fs** |
| CLONE_FS有位，fs有效、in_exec=0、users有效正初值n | `fs->users++`、return0；**本体没有`tsk->fs=`赋值** | 在先行`*tsk=*orig`前提下tsk.fs同址，users局部n→n+1；无逐路径或逐file本地增量 |
| CLONE_FS无位，有效old且copy_fs_struct正常返回有效newfs | `tsk->fs=copy_fs_struct(fs)`，之后`if(!tsk->fs)`门为假 | 独立动态taskfs_s，users初值1，新旧对象地址不同；return0 |
| CLONE_FS无位、copy_fs_struct正常返回NULL | **先写`tsk->fs=NULL`，再检查NULL返回`-ENOMEM`** | 与第44批`copy_files`外层失败时仍可能保留借入旧表指针的情况不同；没有新增fs持有 |
| CLONE_FS有位但current.fs为NULL/无效 | 源码先尝试访问`fs->in_exec`，没有NULL门 | 最早在对象成员访问处失去有效模型；不得补出-EAGAIN或用户错误码 |
| CLONE_FS无位、old为NULL但新分配对象有效 | copy_fs_struct成功分配后需读取`old->umask` | 在umask读取处停止；若分配先返回NULL则走上述正常ENOMEM分支，不把两者合并为单一结果 |

本层`if(fs->in_exec)`是实际共享分支条件，而非有效对象门；源码用于同步的spin_lock/spin_unlock是注释，不是活动调用。不能仅凭本层未加锁就认定真实并发竞态或资源泄漏。`EAGAIN=11`、`ENOMEM=12`为所读独立单宏，不借Linux惯例补出全局调用/ABI保证。证据FS45-A007–A009、A018–A026、A050–A054。

## 3. S02：copy_fs_struct逐字段、两次path_get与实际包装层

`copy_fs_struct(old)`先`kmem_cache_alloc(fs_cachep,GFP_KERNEL)`，若分配返回NULL，`if(fs)`为假并`return NULL`，**还未访问old对象字段**。仅在分配出有效fs后，活动顺序为：
1. 设置`fs->users=1`、`fs->in_exec=0`，初始化`fs->lock`；
2. **取得old锁之前**读`old->umask`到`fs->umask`；
3. `spin_lock(&old->lock)`；
4. `fs->root=old->root`，调用`path_get(&fs->root)`；
5. `fs->pwd=old->pwd`，调用`path_get(&fs->pwd)`；
6. `spin_unlock(&old->lock)`，返回新fs指针。
这里仅`umask`在旧锁之前读取，root/pwd是在实际锁内按值拷贝；没有对old有效性的局部NULL门，也没有在`path_get`失败/不返回时的本地错误回滚。不同于声称所有旧字段都在同一锁保护内。证据FS45-A023–A031、A045–A049。

`path_get`（固定`fs/vfs/namei.c`）活动体只有`dget(path->dentry)`；`// mntget(path->mnt)`是注释。`path_put`的活动体只有`dput(path->dentry)`；`// mntput(path->mnt)`也只是注释。因而本批可证实**上层两次path_get调用→包装层两次dget请求**，以及未来释放时相应两次path_put→两次dput请求；**本包装层没有活动的mount引用取得/归还调用**。但dget/dput具体是否对各自dentry增减计数、空指针处理和物理资源回收，本轮未递归读取，不许从名称恢复操作结果。代码中的解释注释不能替代实际mnt调用。证据FS45-A037–A041。

若root与pwd内的mnt/dentry指针值恰好相同，在两个path_get都正常完成的假设下，仍是**两个独立的调用事件**，而非一次取得；用户数仍是一个新fs对象的1，不是两次增加users。有效dentry对象、dget正常返回、没有其他调用改变路径生命周期，是建+1/+1事件模型的额外条件，不能凭`dget`名字推断实际数值。若第一次path_get不正常返回，停止于root请求，不保证pwd赋值、第二次请求、解锁或return。若第二次中途失效，root请求虽在源码先发生，不能继续推导一个已完成的成功新fs或错误回滚。root/pwd的指针指向对象仍可与其它fs对象共享，按值复制不会自动深复制对象。

动态fs来自`fs_cachep`，此缓存在fork.c::proc_caches_init里用`sizeof(taskfs_s)`和NULL构造器创建；分配接口没有显式零化标志，但所需users/in_exec/lock/umask/root/pwd在正常成功分支均有活动赋值。不能由此推定所有未读字段或物理返回内存的初始内容，也不能把静态init_fs当成由这个cache动态分配的对象。证据FS45-A042–A043。

## 4. S03：exit_fs的users事件、free_fs_struct顺序及fork回滚

`exit_fs(tsk)`最先保存`fs=tsk->fs`（此处在任何task_lock之前），仅在fs非NULL时进入：初始化局部kill，`task_lock(tsk)`、`spin_lock(&fs->lock)`、`tsk->fs=NULL`、`kill=!--fs->users`，随后`spin_unlock`和`task_unlock`，最后`if(kill) free_fs_struct(fs)`。真正的测试是**users先减一、再判断是否为零**；“最后持有者”仅对合法正初值和一致的所有权记账作条件解释。对象有效性并非非NULL检查自动认证，保存指针在锁之前，锁名称不证明跨线程整体原子。证据FS45-A029–A032、A050。

`free_fs_struct(fs)`的真实顺序是`path_put(&fs->root)`，再`path_put(&fs->pwd)`，最后`kmem_cache_free(fs_cachep,fs)`。前者不返回/对象失效时不能声称后一步发生；两个path_put均正常返回后也只能证明**请求调用缓存free**，不是已经物理回收。源代码没有为root/pwd同一dentry特别合并两个归还请求。证据FS45-A033–A036、A055。

| 有效输入与无其它写者模型 | 局部users事件 | 条件后置行为 |
|---|---|---|
| tsk.fs=NULL | 无 | 不加锁、不减users、不调用free_fs_struct |
| 有效fs，users=2 | 2→1，kill=0 | tsk.fs清NULL；不请求两次path_put及fs缓存释放 |
| 有效fs，users=1且root/pwd、callee均有效 | 1→0，kill=1 | 依次请求path_put(root)、path_put(pwd)、kmem_cache_free(fs_cachep,fs)，真实物理回收未证 |
| 共享时原users=n正且in_exec=0，后续fork错误正常贯穿exit_fs | copy_fs n→n+1，再exit_fs n+1→n | 不触发本次最后free；不代表全生命周期配平 |
| 新复制fs，users=1，后续fork错误正常进入exit_fs | 1→0 | 请求路径归还及动态缓存free；需每一步有效与正常返回 |
| 初值0、负值、整数溢出、失效对象或锁/路径callee不返回 | 无有效正计数前提 | 停于首个不合法/未证事件，不给确定最终状态 |

**新的有条件来源候选FS45-F01：** 静态全局`init_fs`不是经`kmem_cache_alloc(fs_cachep,...)`生产，然而`exit_fs`在最后用户分支会无差别调用`free_fs_struct`，后者最终尝试`kmem_cache_free(fs_cachep,fs)`。如果实际某个task拥有指向静态init_fs的最后一次合法users且root/pwd两次path_put均正常返回，该静态对象地址会进入动态cache释放调用。此处已证初始静态绑定与源码释放请求，不曾证明真实运行到最后引用、root/pwd有效或缓存实际做了什么；因此只登记`needs_evidence`，不宣称已复现非法free。它与旧`CORE-USER-FD-CLOSE-42::CL42-F01`属于**不同对象**的相似来源/释放形态，不能合并或关闭旧号。

**另一有界候选FS45-F02：** path_get/path_put包装体实际只有dget/dput、没有活动mntget/mntput；这与path_s仍携带mnt指针值的事实组合，形成挂载引用完整性待证问题。没有审mnt生命周期、其它合法已有引用、规范及实际运行，不升级为已发生挂载泄漏/UAF。历史`CORE-USER-PATH-INIT-31::PI31-F05`聚焦路径起点/空清理体及局部输出，不等于本批挂载腿缺席；仍保留原历史候选。

回到第44批已核失败标签：若`copy_fs`本身返回-EAGAIN（共享且in_exec非零），它没有增加users；若非共享分配返回NULL，`tsk->fs`已被写NULL。二者在`copy_process`的`if(retval)`后均走`bad_fork_cleanup_files`，**跳过紧邻上方的bad_fork_cleanup_fs/exit_fs**，不归还尚未取得的新fs持有，同时后续仍可按实际代码条件性执行文件表exit_files。若copy_fs正常成功后`copy_sighand`或更后步骤失败，才可能先经过`bad_fork_cleanup_fs`调用exit_fs，再继续到files标签。旧UV-14的sighand清理可能形成更早停止点，故不能把后两次清理写成无条件完成。证据FS45-A013–A017。

## 5. 去重、范围与方法

本批新增候选仅`FS45-F01`（静态init_fs最后归还的条件来源不匹配）和`FS45-F02`（path包装层挂载引用操作未活动、实际生命周期未知），均属有条件观察，不是两个已复现缺陷。旧`PI31-F05`、`CL42-F01`、`UV-10/14/15`及44错误贯穿限定保留原qualified_id和状态，不自行关闭。其它源输入如fs=NULL、root/pwd无效、无合法users、old->umask读前失效只作有限模型停止点，不人为新增缺陷数量。

本批只审fork.c::copy_fs、fs_struct.c::copy_fs_struct/exit_fs/free_fs_struct、init_fs与直接path_get/path_put等，未审set_fs_root/set_fs_pwd/get_fs_root/chdir/chroot、dget/dput深层、挂载/命名空间/完整fork/exec/exit、并发协议、slab、权限或路径解析。仅申请READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING；无内核修改、补丁、编译运行或阶段3授权。
