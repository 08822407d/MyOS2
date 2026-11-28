---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PATHNAME-COPY-29
record_type: bounded_pathname_copy_and_filename_lifetime_static_analysis
date: 2026-11-03
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
conversation_display_name: MYOS2-A-C03c 代码分析执行续建
mainline_reviewer: MYOS2-A-C04
source_ref: time
source_pin_short12: a039d9803ade
recovery_contract_short12: 2a0a66faa76d
original_task_short12: 324927712a98
review28_short12: 0a94f32ad00e
result_base_short12: 427604c818a5
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
---

# 第29批：从用户／内核路径字符串到filename_s及其局部归还

## P00｜恢复、固定源与历史限界

已按Owner转发51号恢复说明解除29此前交接暂停，严格以原49号技术任务P00–P04为实施范围。固定引用分别为51号`2a0a66faa76d`、49号`324927712a98`、28主线验收回执`0a94f32ad00e`；读取R28-S01/S02/S03，与`time@a039d9803ade`绑定。C04已接任原lead主线，取代49号旧文字的“交C03验收”；本执行者仍为C03c/CHAT-EXEC-03，仅可以向原PR17的user-pathname-copy-29/新增六件及本人交付评论。未重新执行28的ELF解释器审计、没有初始化新会话或切换内核基线。

历史`CORE-USER-ELF-INTERPRETER-28::EI28-F01`已经登记`__myos_getname`产生ERR_PTR(-ENOMEM)而`getname_kernel`直接写name成员的条件边界；本批扩到`getname`仍按该原ID追加，不再另造同根漏洞。`CORE-USER-ELF-INTERPRETER-28::EI28-F05`的PT_INTERP内部提前NUL及尾NUL受限范围持续适用。旧`CORE-USER-VFS-BASELINE-05::UV-04`的`do_filp_open`下层缺return契约依旧阻断全链“必定成功”推理。现有历史未做全仓去重，不改任何旧件。以上主线回执只证明限定静态接受（PASS_PENDING_LOCAL），不是底层运行认证。

## P01｜两种getname的实际取得顺序与对象身份

### 对象、类型与分配有先后差异

固定`mykernel/include/linux/fs/fs.h::filename_s`拥有`const char *name`、`const char *uptr`和`int len`三项活动成员（历史refcnt为注释）。`__myos_getname()`只调用`kmalloc(sizeof(filename_s),GFP_KERNEL)`取得**本体对象**；NULL时返回`ERR_PTR(-ENOMEM)`，否则返回本体指针。它不分配字符串字节区。两个入口中都没有在取得本体后、第一次写`name->len`或`name->name`之前执行`IS_ERR(name)`：若这次kmalloc实际失败且错误编码指针被照常使用，不能推导出正常的错误返回、正常putname或任何确定CPU后果。该根因已经由EI28-F01登记，不重复计数。见PN29-A001/A002/A004/A009/A025/A034/A036。

`getname(const char *u_filename)`先调用`strlen_user((void*)u_filename)`，将其返回的`long`直接存入**无符号`size_t len`**，再以`len>PAGE_SIZE`判长；只有通过门才用`__myos_getname`取得本体，写入`name->len=len`，再`kzalloc(len+16,GFP_KERNEL)`取得独立字节缓冲。若第二次分配NULL，本函数调用`kfree(name)`后返回`ERR_PTR(-ENOMEM)`；这是本体与字符区两个不同分配的失败门。所选PAGE_SIZE为4096。在此长度门通过的正常模型中len处于0..4096，故赋给`int name->len`在所选x86-64类型宽度下不会丢失数值；`len+16`最大4112，不从此正常域推断算术溢出。证据PN29-A003–A008/A025/A067。

`getname_kernel(const char *k_filename)`的顺序不同：**先**`__myos_getname`，然后用`strlen(k_filename)`计算不含终止NUL的`size_t len`，再判断`len>PAGE_SIZE`；若超长则直接`return ERR_PTR(-ENAMETOOLONG)`，没有本地`kfree(name)`。因此在本体已分配成功且strlen正常产生4097等值的条件下，本地早返仍有该本体未由此函数归还；这不认证跨整个内核的永久泄漏或真实输入可达性。若长度合格，则写`name->len`，用`kzalloc(len+1)`分配字符区；如果第二次分配失败，则`kfree(name)`并返回`ERR_PTR(-ENOMEM)`。固定PT_INTERP来源已由28回执R28-S02限定为`p_filesz<=4096`且末字节NUL，若字节稳定且读完整，`strlen`最多4095；不能拿通用超长分支证明来自该解释器路径的自然失败。证据PN29-A009–A013、历史R28-S02。

两个入口在成功分配时都设置`name->name`到新字符区，而不是直接保存调用者传入的`u_filename`/`k_filename`作为拥有所有权的地址。`uptr`虽为结构体成员，但两个选定函数没有活动的赋值；`kmalloc`不等于把结构体所有字节初始化为零，`uptr`是否被其它调用者读取未在本批认证。只记“本批未初始化、所选caller未读取”对象界限，不另记漏洞。释放接口`putname(filename_s*)`依次尝试`kfree(name->name)`、`kfree(name)`，自身不先`IS_ERR`或查NULL；调用者必须只在可靠有效对象前提下使用。代码到此为止没有证实实际分配器物理回收或其它隐藏持有者。证据PN29-A001/A005/A011/A014/A025/A026。

## P02｜扫描、复制、NUL与返回码应分别计证

### strlen_user与直接strlen/strncpy不是Linux标准uaccess实现

选定的`arch/x86_64/myos/utils.c::strlen_user`先执行`ulong size = strlen(src)`，**然后**才执行`if (!access_ok(src,size)) return -EFAULT`，否则以`long`返回size。选定的`lib/string/string.c::strlen`逐字节扫描直到源中第一个`'\0'`，返回它之前字符个数；因此此`strlen_user`正常返回长度不含终止NUL，也不会在扫描前先对该源地址作`access_ok`。当扫描本身不返回、读取不可访问对象或发生异常时，不能虚构已经从其后来的门获得`-EFAULT`。这和仓库中注释的Linux`strnlen_user`契约不同，不通过函数名称重建Linux风格的故障恢复。证据PN29-A017/A018/A023/A024/A068。

选定的`strncpy_from_user`收到`(to,from,size)`后先用`access_ok(from,size)`检验地址区间，若为假返回`-EFAULT`；否则直接`strncpy(to,from,size)`，**忽略strncpy的返回指针**并以`long`返回原size。所选通用`strncpy`循环不超过count：每次复制源字符；若遇NUL则不前进源而继续将剩余目标字节置零；函数正常返回始终是传入的dest。它的存在受`!__HAVE_ARCH_STRNCPY`宏条件约束，本次未展开完整编译配置，结论限该选定源码/条件。对内核`getname_kernel`而言，`dest=strncpy(name->name,k_filename,len)`后`if(dest!=name->name)`的错误出口，在所选正常strncpy实现下**没有非目标指针生产者**；不得计作已成立独立失败，也不得从假想`ERR_PTR(len)`推断实测 errno。证据PN29-A012/A013/A015/A016/A019–A022。

`access_ok`宏在所选x86-64包含链中把测试委托给`__access_ok`，后者使用用户地址范围符号判定及`size+ptr`无符号回绕判断。在`strlen_user`里它发生于扫描之后，在`strncpy_from_user`里发生于受限复制之前；无论何时执行，这一**数值门**不能认证页表已映射、目标访问权限、SMAP状态或硬件异常能够安全恢复。传入的size是第一次`strlen`得到的**不含NUL长度**，所以范围判断也没有覆盖位于`src[size]`的NUL读取位置。源中即使有NUL，目标实际已写入、后续不返回或用户字符串改变等可能情况也须分开。证据PN29-A027–A031、PN29-A017/A018。

### 有符号结果转无符号后的两次门

第一处转换：`strlen_user`的`long -EFAULT`若在正常完成`strlen`后被`access_ok`生产，`getname`把它赋给`size_t`；所选64位unsigned long模型下负14转换成`18446744073709551602`，再由`len>4096`走`ERR_PTR(-ENAMETOOLONG)`。源错误信息由此在上层表现为“超长”，不是原`-EFAULT`；该条件分支在分配filename本体之前。不能据此声称任意无效用户指针都安全地产生ENAMETOOLONG——因为前面的strlen可能根本不会正常返回。证据PN29-A003/A017/A018/A034/A067，见PN29-F01。

第二处转换：`len = strncpy_from_user(...,len)`同样将有符号`long`的结果写入无符号`size_t`，随后`if (len<0)`按无符号类型为恒假；其内`kfree(name->name)`/`kfree(name)`及`ERR_PTR(len)`不能由正常负值检查进入。但这里必须审实际错误**生产者**：`strncpy_from_user`本体在`!access_ok(from,size)`时可返回`-EFAULT`，然而当前`getname`之前`strlen_user`已对同一源指针及相同第一次长度执行**同一纯数值`access_ok`**并正常通过。在有效对象、调用实参未变、纯数值函数和无其它未定义行为的模型中，第二次检查没有来自正常`getname`路径的失败生产者。单独可列直接helper条件，不应把“`len<0`恒假”升级成这一程序当前必然吞掉实际用户复制EFAULT的内核漏洞。证据PN29-A003/A006/A015–A018/A028，详见facts::PN29-M13。

### 终止字节来自何处

按正常strlen语义，`len`不含用户源的终止NUL，因此`strncpy_from_user(...,len)`和`getname_kernel`中的`strncpy(...,len)`不会复制该位置的源NUL。它们分别分配**len+16**与**len+1**，而所选`kzalloc`将`__GFP_ZERO`送到kmalloc，已读分配钩子在该标志下对目标对象执行`memset(...,0,zero_size)`。在分配器正常归还足够字节且正常完成清零的条件下，目标的`[len]`字节保持零，因而可作为C字符串终止；这是**分配器清零而非源NUL成功复制**的证据。没有执行分配器/底层平台测试，不对物理内存全链作无条件保证。对长度上限4096，用户版本分配4112而内核版本4097；正常`int name->len`可存4096。参见PN29-A005/A011/A016/A021/A022/A032/A033及facts::PN29-X01–X08。

若用户源在两次观察之间改变，第一次`strlen_user`得到的长度可能与后次复制的源内容不一致：若后次内容变短，`strncpy`可在早NUL后补零；若变长，仍只复制**旧len**字节，终止位置主要由预先分配清零提供。原有name->len记录第一次长度，而复制的字节来自第二次观察；没有实际并发写入或用户指针样本证据，不能宣称竞态已发生、更不能认定路径权限被绕过。至多把两次读取不构成原子快照作为带前提的语义边界。见PN29-F04。

## P03｜三个主要调用者与内核对照，归还在谁手里

### 用户sys_open→do_sys_open→do_sys_openat2

固定`mykernel/fs/syscall.c`的`MYOS_SYSCALL_DEFINE3(open,...)`调用`do_sys_open(0,filename,flags,mode)`，下层通过`do_sys_openat2`从用户路径调用`getname(filename)`。该函数以`IS_ERR(tmp)`检测错误指针，失败直接`PTR_ERR(tmp)`返回，不对错误指针`putname`；有效`filename_s*`时继续`get_unused_fd_flags`，若fd>=0则将同一tmp指针传入`do_filp_open(dfd,tmp,&op)`，并根据返回文件或错误处理fd。无论fd分配非负/负，只要流程正常回到共享尾部都调用`putname(tmp)`再返回。文件引用和fd本身是另一所有权域，不能把`putname`算成释放了`file_s`或完成VFS权限验证。既有05::UV-04的下层缺return和非正常控制流可能阻断完整可达性；本包只核caller本地前缀。证据PN29-A040–A045/A064及facts::PN29-M16–M18。

### 用户sys_execve→do_execve→do_execveat_common

`MYOS_SYSCALL_DEFINE3(execve)`实调用`do_execve(getname(filename),argv,envp)`；`do_execve`把同一`filename_s*`传进`do_execveat_common`。函数首先以`IS_ERR(filename)`/PTR_ERR处理错误指针并**直接返回**，不调用putname。有效对象继续`alloc_bprm(fd,filename,flags)`；若返回错误指针则取PTR_ERR后跳`out_ret`，最终`putname(filename)`；若分配bprm成功，正常完成或中途错误经`out_free`先`free_bprm(bprm)`，再落入`out_ret`的`putname(filename)`。这只是给定中间帮助函数正常返回/跳转的控制流，不能跨越已知open缺return、后续VMA/参数页错误推断一定完成exec。证据PN29-A046–A050/A035/A036。

`alloc_bprm`取得的`filename`是借入对象：`do_open_execat(fd,filename,...)`使用它，普通fd/路径情况下`bprm->filename=filename->name`只**保存字符区指针**，并未复制字符串或增加filename对象引用；另一些fd情形创建独立`bprm->fdpath`并让`bprm->filename`指向它。`bprm->interp`初设为`bprm->filename`，但名称相似不代表解释器file。出错/释放时`free_bprm`负责mm/参数页及自己拥有的`fdpath`、可能另分配的interp等，**没有直接putname原filename**；共同外层`do_execveat_common`才在free_bprm之后putname。保存指针、复制目标字节和转移所有权不可互相替代。证据PN29-A051–A056。

### 内核open_exec及两个有限对照

`open_exec(const char *name)`调用`getname_kernel`并将其转换为初始`file_s *f=ERR_CAST(filename)`，只有`!IS_ERR(filename)`才执行`do_open_execat(AT_FDCWD,filename,0)`，然后`putname(filename)`并返回f。通过解释器28路径传入的字符区域来自已读PT_INTERP且尾NUL有效时，`getname_kernel`字节长度受上游4096约束；若该函数在已取得filename本体后遇内存错误指针非法访问而不返回，则不能继续假设外层已进入`putname`。这里也没有把该filename对象与最后得到的`file_s*`混为一体。证据PN29-A057–A060、EI28旧证及R28-S02。

同接口的有限对照：`kernel_execve`取`getname_kernel(kernel_filename)`，以IS_ERR门处理错误指针，成功后走`alloc_bprm`，在其共有结束标签先`free_bprm`（若已存在）后`putname`。`filp_open`也先`getname_kernel`，IS_ERR后只在正常对象支路`file_open_name`再`putname`。这两项只说明函数直接调用者如何消费对象，不构成仓库调用者穷尽清单，也未重新审内核exec和文件打开完整流程。证据PN29-A038/A039/A061–A063/A065。

### 分配/错误/资源矩阵的结果边界

完整矩阵见`facts-and-boundaries.yaml::return_and_resource_matrix`，共24个分支模型：空串、3字符、4096/4097边界、原`-EFAULT`变成超长、第一次/第二次分配失败、用户扫描不能正常返回、第二复制帮助函数负返回但上层当前未建立生产者、`strncpy`非目标指针不成立、源修改、各caller的ERR_PTR/有效对象/下层返回，以及与各自`putname`的关系。每行都说明必要前提与真实还是假设的生产者。绝不把`kfree`调用自动说成物理归还，也不从C源没有运行的下游路径推断一定成功或一定失败。

## P04｜登记、验证范围、移交

新增条件项`PN29-F01`为首次signed返回错误被超长条件覆盖；`PN29-F02`为getname_kernel在已分配本体后超长早返未见本地释放；`PN29-F03`为strlen_user先扫描、后数值检查及NUL超出检查长度；`PN29-F04`为两次观察不构成稳定快照。这四项是有前提、非运行、未完成全仓去重的候选，不等于四个独立内核漏洞。`EI28-F01`错误编码指针无提前校验作为**原ID**延伸到getname；`EI28-F05`已有字符串前置NUL问题作为历史引用，05::UV-04仍阻断全链VFS。完整限定与反证在`deferred-findings.yaml`。

六件：本报告、`MANIFEST.md`、`facts-and-boundaries.yaml`、`pathname-copy-paths.dot`、`deferred-findings.yaml`及`evidence.md`。`evidence.md`源自固定time的68段逐字1–5行证据，经过二次读取核定；使用GitHub连接器与Code Mode JavaScript做文本/JSON、图端点和有限整数检查，没有安装或运行内核/C/ASM、编译、QEMU/硬件、真实用户指针、仓库脚本、仓库外libc或其它API模型。全局内存管理/错误恢复及权限/路径遍历未完成。

当前仅请求`READY_FOR_MAINLINE_REVIEW`，主线验收`PENDING`、上限`PASS_PENDING_LOCAL`，运行`NOT_RUN`。真实Git提交与固定提交/活动分支回读及compare在写入后由本人原PR17交付评论登记；交付后本C03c执行者`STOP_WRITING`，验收对象明确为**「MYOS2-A-C04 内核分析主线续建」**，不再交C03，不自行启动30。
