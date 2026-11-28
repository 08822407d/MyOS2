---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-OPEN-COMPLETION-39
record_type: bounded_file_open_completion_and_return_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-04
date: 2026-11-05
base_snapshot: time
source_pin_short12: a039d9803ade
execution_base_short12: 61a95d13dcb6
contract64_short12: 08d383bb2b2c
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第39批｜从path到file：字段写者、open回调、非void返回与上层消费

## O00｜固定输入、上批限制和写区

原执行者仍为`MYOS2-LEAD-002-CHAT-EXEC-04`，沿用同一C04a接收及Owner报告GPT-6＋Extra，不重新启动聊天。完整读取固定主线`08d383bb2b2c`的**64号任务书、第38批验收回执及63号GitHub写入重试规则**。38为`PASS_PENDING_LOCAL`、`returned_scope=[]`；**R38-S01/R38-S02**必须合读：真实启动中FAT32根inode的mapping首值与子inode的`MSDOS_I`外层包含关系未闭合；图的条件边、根/子来源不能提升为可达实际用户open。第36批文档闭合、第37批限定接收维持，不重作历史验收。

本包只以**有效的file、path、dentry、inode和mapping的条件输入**研究打开完成链，或者明确称为helper-only。历史`UV-04`已指出两级无return，今次只扩大其真实字段和调用消费条件，不以另一个新发现重复报缺return。`OF30-F01`关于do_sys_openat2忽略build_open_flags错误与DOC-FIX已由旧批限定，本批保留已核生产者与输入有效性。`FC13-F04/R13-S01`的截断门必须读实际`f_mode&O_TRUNC`而非由用户O_TRUNC自动替代。第32批`LC32-F02`已将open_last_lookups传入的`op->lookup_flags`与用户`O_CREAT`区分，今次不重作末分量查找。第38批`SB38-F02/F03`仍是独立根/子对象来源条件，绝不假设修好了再推出用户open成功。历史ID不覆盖。

O00实际预检：time`a039d9803ade`的options_flags.cmake及panic.c金丝雀逐字命中，master不命中；PR16/lead`08d383bb2b2c`，PR17执行分支初始`61a95d13dcb6`，master`e8e60cd19540`，time未漂移。PR16/17均open、PR17仍Draft/unmerged，开放PR仅这两个；PR16共161个变更路径与`user-open-completion-39/`零交集，目录首次不存在。第38批本人评论`6635234504`已远端回读。正式写前再独立查HEAD和路径；若有未知后继不覆盖，不强推。63号具体节奏和真实写入日志在MANIFEST登记。

## O01｜file分配、两个旗标域、对象别名

`path_openat(nd,op,flags)`先`alloc_empty_file(op->open_flag)`，若`IS_ERR(file)`直接返回该错误指针；否则才`path_init`与路径循环（OC39-A013–A015）。`alloc_empty_file`活动体只执行`f=__alloc_file(flags);return f;`。`__alloc_file`向`kmem_cache_zalloc(filp_cachep,GFP_KERNEL)`请求一个`file_s`对象，NULL时返回`ERR_PTR(-ENOMEM)`，正常分配才执行`atomic_long_set(&f->f_count,1)`、`spin_lock_init`、`f_flags=flags`及`f_mode=OPEN_FMODE(flags)`，再返回f（OC39-A001–A006）。请求zalloc与cache创建最后构造器参数NULL是实际源码，**没有运行底层分配器**，不能把全部零值作为本批物理内存事实。`alloc_empty_file`下部仍保留`over: return ERR_PTR(-ENFILE)`代码，但活动体前面已有无条件return且没有到over的活动跳转，本条所选普通调用不会构造-ENFILE；不能凭上面的注释声称活跃容量检查能返回它（OC39-A007）。缓存/文件数量策略不扩审。

对象不混同：`file_s`包含单独的`f_flags:uint`、`f_mode:fmode_t`、`f_count:atomic_long_t`、`f_pos:loff_t`、`f_path:path_s`、`f_inode:inode_s*`、`f_op:file_ops_s*`、`f_mapping:addr_spc_s*`（OC39-A046–A048）；它们不是同一个位域，也不是从`file`外再创建的独立mapping。真实`file->f_flags`接收**op.open_flag**；真实`file->f_mode`接收`OPEN_FMODE(op.open_flag)`。后续`do_filp_open`却取`int flags=op->lookup_flags`传给`path_openat`，`path_openat`又把这个flags传给`open_last_lookups`及`do_open`。这些是**lookup旗标**，不该因下层参数名叫open_flag就说成用户`O_CREAT`；LC32-F02已经按成功输入的实际位域划过这条界（OC39-A019–A024，I结构OC39-A054/A055）。

## O02｜调用、字段副作用、回调结果与三层返回缺口

`path_openat`的路径循环写`error=link_path_walk(s,nd)`，左操作数非零时do_open被跳过，先`terminate_walk`，在合法清理前提下`fput(file)`后`ERR_PTR(error)`正常返回；这是一个**拥有实际整数错误生产者**的条件分支（OC39-A015–A018），不应因下文缺return一概否定该早期错误出口。若路径环正常到达`error==0`，才调用`error=do_open(nd,file,flags)`，随后使用error比较是否返回file或执行fput+ERR_PTR。其`FMODE_OPENED`判断处于**注释**，不属于现行成功门（OC39-A016/A017）。

真正`do_open`的完整活动体只是一句`vfs_open(&nd->path,file);`，声明返回`int`却**没有return**（OC39-A011/A012）。`vfs_open`先`file->f_path=*path`（浅值拷贝，不是新dentry分配），再`return myos_do_dentry_open(file,path->dentry->d_inode)`（OC39-A030）。它的下层`myos_do_dentry_open`也声明返回`int`，完整所选活动体并无return。三者区别：`myos_do_dentry_open`的缺return在内层发生；`vfs_open`有return表达式但表达式消费下层**没有定义良好的int结果**；`do_open`即使丢弃vfs_open的值，本身仍缺return，而`path_openat`使用它的值作为error。C语义下这些结果不能由“能走到结尾”替换为0或1，亦不代表函数必然不返回，不能凭ABI寄存器猜测errno或成功分支。旧`CORE-USER-VFS-BASELINE-05::UV-04`持续保留，不以OC39-F新号重报该两个缺return。

在每个输入与调用都有效、且每个被调用函数此前正常完成的受限前缀里，`myos_do_dentry_open`按顺序：`ulong error=-ENOERR`；`path_get(&f->f_path)`；`f->f_inode=inode`、`f->f_mapping=inode->i_mapping`；`f->f_op=inode->i_fop`；如果`f->f_op && f->f_op->open`，则调用`f_op->open(f->f_path.dentry->d_inode,f)`并把**整数返回赋给局部ulong error**（OC39-A026/A027）。因此f_mapping只是**既有inode.i_mapping的别名**；若SB38-F03对应根mapping来源无效，或SB38-F02的子inode未通过容器有效性门，必须在先行读取处停止，不把file字段已赋值等同实际可用路径。

`FAT32_file_ops.open`实际绑定`FAT32_open`，在此具体条件下它执行`file_p->f_pos=0;return 1;`（OC39-A063/A064）。但`myos_do_dentry_open`之后**没有检查或return局部error**；无f_op、无open成员、callback返回0、返回1或正常返回其它值都只能证明相应前缀，之后仍按`f_mode`条件执行并走到缺return处。若callback不正常返回，截断/追加语句不能跨过去宣称已执行。不能从FAT32_open返回1认定整个vfs_open返回1；也不能把非零当作系统必定认定打开失败。新候选`OC39-F02`只记录**回调值丢失/未门控**，与旧UV-04返回契约相互关联，不按两项独立已复现故障计算。

## O03｜O_TRUNC、O_APPEND、f_mode、f_pos和外层安装门

源`O_ACCMODE=00000003`、`O_RDONLY=0`、`O_WRONLY=1`、`O_RDWR=2`、`O_TRUNC=00001000`（八进制512）、`O_APPEND=00002000`（八进制1024）。`OPEN_FMODE(flags)=(((flags+1)&O_ACCMODE)|(flags&__FMODE_NONOTIFY))`，而`FMODE_NONOTIFY=0x4000000`。按固定所选宏和无额外写者的受限整数模型，`__alloc_file`产出的`f_mode`只能含低两位（0、1、2、3）及可选0x4000000位，**不含0x200或0x400**。因此即使用户`O_TRUNC`或`O_APPEND`位已经保存进`f_flags`，`myos_do_dentry_open`的两个**实际测试却是`f->f_mode & O_TRUNC`和`f->f_mode & O_APPEND`**，对此选定正常生产者均为假（OC39-A004/A028/A029/A053/A056–A062）。这不是把开放位域解释成f_mode，也不是声称全仓其它修改者不存在；`OC39-F01`保存这个新限定发现。`R13-S01`原截断赋值门仍保留，但已进一步找出当前正常f_mode来源不满足该门。不能据此证明用户磁盘文件确实被截断、未截断或缓存刷新。

如果另造一个**helper-only**有效file并直接设置f_mode的0x200/0x400位，且此前path_get和callback均正常返回，代码才条件性执行`f->f_path.dentry->d_inode->i_size=0`或`f->f_pos=f->f_path.dentry->d_inode->i_size`。若两位均被人为置位，源码顺序为**先截断i_size=0，后append取当前i_size**；这不证明真实调用者存在这样的f_mode生产者，也不包含磁盘数据、缓存或持久化处理。`FAT32_open`单独写f_pos=0发生在两门之前；其回调1被忽略后不自动传播为成功或错误。没有选择FAT32_open时f_pos的首值只具有kmem_cache_zalloc清零请求，未认证其实际位模式。`FMODE_OPENED=0x80000`有定义，但所选路径没有活动写入；`path_openat`中相关门被注释，不给真实打开成功额外保障（OC39-F03）。

更外层，`do_filp_open`只传递`path_openat`的file_s*或错误指针，并`restore_nameidata`后返回。kernel侧`file_open_name`对`build_open_flags`的整数错误显式执行`if(err)return ERR_PTR(err)`，否则才`do_filp_open`（OC39-A038/A039）。syscall侧`do_sys_openat2`却将`build_open_flags`的返回写进fd后**不查错**，若getname正常又把fd改成`get_unused_fd_flags`返回值；fd>=0才调用do_filp_open，并依`IS_ERR(f)`选择`PTR_ERR`或`fd_install(fd,f)`，最后返回fd（OC39-A040–A044）。这是已登记**OF30-F01**，不冒充新发现或证明权限绕过。fd_install条件只在**确实有定义良好的普通file指针结果**时可作条件推导；旧UV-04阻断本selected完成链中的真实成功结果认证。fd表分配和安装实现未审，故不进一步推断已安装描述符或资源配平。

`fput`在所读活动体内对f_count调用`atomic_long_dec_and_test`，为真时`kmem_cache_free`；路径相关引用、path_put、inode引用、页缓存的完整平衡不在本包审计范围（OC39-A008–A010）。path_openat的fput+ERR_PTR错误路径仅对前提有效及callee正常返回的分支成立，不证明资源物理回收的全部效果。

## O04｜交付形式和严格停止条件

仅在已有PR17的`user-open-completion-39/`新增六件：MANIFEST、本文、facts-and-boundaries.yaml（YAML1.2兼容JSON）、open-completion-paths.dot、deferred-findings.yaml、evidence.md。facts完整记录22个对象身份、12个函数返回/消费类别、12行字段写者矩阵、22组有限状态/位模型；图从真实调用和赋值分开数据/前提与尚无可用返回的边界，不画成已发生CPU停止或固定返回0。新`OC39-F01..F03`是三个有前提普通延期候选，历史UV-04、OF30-F01、FC13/LC32/SB38/NI37保持原编号与状态，不把候选数认作漏洞数。

evidence给出69段固定time单定义体短引文，来自9个源文件，1–5行。相邻重叠段可以支持同一事实，不能按69个不同根因计数。宏、函数、结构体和绑定表各自有确切归属；声明、注释、条件编译上下文不冒充活动代码。所有未核真实生产者、文件初始值、错误传播与生命周期均写出停止点，不借额外运行证据补齐。

**NOT_RUN**：仓库C/ASM及脚本、预处理/编译、QEMU/硬件、真实文件打开/磁盘/驱动、完整fd表/挂载/权限/文件系统规范、分配器、引用/回收、pagecache全链、外部libc和额外付费模型/API。没有修补内核或用户程序、旧件、PR标题/body/Draft；没有新分支/PR或合并关闭、强推删分支、进入阶段3。完整本地P9必须记NOT_RUN；仅有文本/图/整数模型自检。

63号写入规则：GitHub暂态故障先回读判断是否已成功，再按约10/20/40秒最多三次安全重试，遵守限流；保留expected-head和force=false，未知后继不覆盖。没有真实暂态故障不编造次数；MANIFEST内`write_retry_log`如实记录，最终GitHub comment记录实际提交号/净差异与读回。正式提交后从**新固定提交及活动分支**全文回读六件，确认compare只六件added、旧件零净改删、HEAD未意外变化，再由本执行者在PR17追加唯一正式交付评论。评论读回后`STOP_WRITING`交C04，mainline_acceptance仍PENDING，不自行启动40。
