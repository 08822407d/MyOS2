---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PATH-INIT-31
record_type: bounded_nameidata_and_path_start_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
conversation_display_name: MYOS2-A-C03c 代码分析执行续建
mainline_reviewer: MYOS2-A-C04 内核分析主线续建
date: 2026-11-03
source_ref: time
source_pin_short12: a039d9803ade
contract_short12: 867eba5aaff9
review30_docfix_short12: 867eba5aaff9
result_base_short12: e33103ca7d9e
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
---

# 第31批：nameidata初始化、绝对/相对起点和直接调用链

## N00｜固定输入、历史证据与可信范围

本批沿用既有C03c / `MYOS2-LEAD-002-CHAT-EXEC-03`执行身份。已完整读取54号固定合同`867eba5aaff9`与同一固定提交的30-DOC-FIX-01主线复核；C04对R30-01/R30-02签发`CLOSED_DOCUMENTATION_ONLY`，30组合成果按`PASS_PENDING_LOCAL`限定接受，原RETURN史料保留，内核运行仍`NOT_RUN`。本批读取30相关内容必须以追加`user-open-flags-30-doc-fix-01/errata.md@e33103ca7d9e`纠正原30的证据资格及图注口径；旧80个标签不能脱离勘误宣称单定义体全部通过。原29 R29-S01/02/03及05/08旧问题继续为限定边界，但不重新审29/30的整个内部，也不回写旧文件。

直接固定源码为`time@a039d9803ade`；金丝雀`options_flags.cmake`的`-mcmodel=kernel -fno-pie`与`panic.c`的`this_cpu=smp_processor_id()`已实际连接器读取。输入范围限制于`mykernel/fs/vfs/namei.c`里的`nameidata_s`、`set_nameidata/path_init`、`do_filp_open/path_openat`、`filename_lookup/path_lookupat`、三个cleanup函数、`link_path_walk`前缀，及定义其`path_s/fs_struct/filename_s/LOOKUP_*/AT_FDCWD`的必要文件。其他分量查找、挂载、完整权限、dfd查表、uaccess、file_ref、fs_struct锁以及编译运行都未审。引文在`evidence.md`，每个`PI31-A*`只引用选定函数/宏/结构体实际1–5行，不将头文件声明或Linux注释当活跃函数逻辑。

写前预检读master/time/lead/执行头、PR16/17与全部开放PR、PR16改动路径与目标目录：在原PR17头`e33103ca7d9e`时目标`user-path-init-31/`不存在、PR16交集为空；PR17仍open/Draft/unmerged且本仓连接器push授权存在。此为具体远端时点，不能当作排除其它尚未推送事务的全局锁。首次实际写前还须再次检查expected-head且只允许非强推快进。没有任何独立C04验收或运行安全结论预先签发。

## N01｜对象身份、清零及赋值先后

**对象不等价：** `filename_s`包含`name`字符区指针、`uptr`和`len`；`nameidata_s`在`namei.c`内定义，活动字段包括`path_s path/root`、`last`、`inode`、`last_type`、`unsigned flags/state`、`filename_s *name`和`int dfd`。`path_s`本身只含`vfsmount_s *mnt`和`dentry_s *dentry`，不是被复制的dentry/inode对象。`fs_struct`分别储存`path_s root,pwd`。以下所有从`current->fs`读取指针都依赖当前task、fs、root/pwd有效，而不是通过本包证明它们必然存在（PI31-A001–A008）。

`set_nameidata(p,dfd,name,root)`首先执行`memset(p,0,sizeof(nameidata_s))`；在有效目标内存条件下可清零flags/state等成员。之后写`p->name=name`（**借用对象指针**而非复制文件名字节或新增引用）、`p->dfd=dfd`、显式`p->path.mnt/dentry=NULL`。只有当可选`root != NULL`，才执行`p->root=*root`，这是含两个指针的path_s**按值复制**；当前函数没有实调`path_get`，不据此宣称增加文件系统引用、合法锁操作或完成资源归属交接（PI31-A009–A012/A005）。

两条本批指定调用链中，`do_filp_open`和`filename_lookup`都实传`set_nameidata(&nd,dfd,name,NULL)`。尽管`filename_lookup`签名自己有`path_s *root`形参，它没有把该变量传下去。**更强的局部事实**是`path_init`在任何绝对/相对分支之前都执行`nd->root=current->fs->root`，因此即使某个helper-only直接调用`set_nameidata`曾传入非NULL root并保存`p->root=*root`，正常后继`path_init`也会覆盖该值；但本批没有发现两个指定caller提供非NULL root，也不能据此直接宣称真实namespace绕过（PI31-A012/A014/A026/A034/A035）。

## N02｜path_init如何真正选起点

`path_init(nd,flags)`的实际顺序如下：先取`const char *s=nd->name->name`，写`nd->flags=flags`，直接写`nd->root=current->fs->root`；若`s[0]=='/'`则`nd->path=nd->root`，否则`nd->path=current->fs->pwd`；最后`return s`。函数主体**没有**使用`nd->dfd`或根据`LOOKUP_IN_ROOT/BENEATH/EMPTY`改变这个选择。这里的“没有”只限已全文读取的`path_init`，不外推为全仓没有使用dfd或这些标志。nd.flags被保存给后继逻辑，不等于此处已执行约束。返回s仍是同一地址；没有分配缓冲、跳过斜线或把路径字符串复制到新的存储（PI31-A013–A017/A078/A080）。

用户`sys_open`当前实参是`do_sys_open(0,filename,flags,mode)`，`do_sys_open`把该dfd原样传到`do_sys_openat2`，在有效后续条件下再传`do_filp_open`。因此在选定用户open路径中`nd.dfd=0`，但相对路径起点仍是`current->fs->pwd`，而不是此函数取fd=0映射的某目录。内核`open_exec`在有效filename条件下以`AT_FDCWD=-100`调用`do_open_execat`，它再把fd传给`do_filp_open`，正常起点同样由字符串首字节而非dfd值决定。`kern_path`另以AT_FDCWD调用`filename_lookup`；其只是有限对照，不表示所有filename_lookup调用者均传这个常量。单次调用`set_nameidata`保存dfd的事实不等于实际起点使用了它（PI31-A024/A059–A067）。这一局部事实与05代表基线已有dfd观察可能重合，条件问题按历史关系记录而非重新认定独立根因。

**具体字符串纸面模型**（对象及路径内容都假定定义良好，fs.root/pwd有效）：

| s原始内容 | path_init选定的nd.path | link_path_walk的入口前缀 | 不能证明的部分 |
|---|---|---|---|
| `/a` | current.fs.root | 在其函数内跳过首个`/`，后看到`a` | 具体分量查找和完成 |
| `a` | current.fs.pwd | 没有前导斜线可跳 | 最终file/dentry |
| `//a`、`///a` | current.fs.root | 逐个跳过前导斜线后才看到`a` | 对特殊双斜线的外部语义 |
| `""` | current.fs.pwd | 在`LAST_ROOT`状态看到NUL，前缀return0 | `LOOKUP_EMPTY`是否允许、调用者最终是否成功 |
| `/`、`///` | current.fs.root | 跳过所有斜线后遇NUL，前缀return0 | `lookup_last/open_last_lookups`后结果 |
| 非NULL root=R的单独helper模型 | `set_nameidata`先写R；`path_init`将其覆盖为current.fs.root | 和当前字符串起点规则相同 | 未证实际caller传R |

原始斜线跳过发生在后续`link_path_walk`而非`path_init`：它先将`nd->last_type=LAST_ROOT`、检查`IS_ERR(name)`，再以`while(*name=='/') name++`移动局部指针；遇`*name=='\0'`直接返回0。**不要把这一次前缀return0等同完整路径解析成功**：在`path_openat`，后面还调用`open_last_lookups`；在`path_lookupat`，仍有`lookup_last`，其本体转向`walk_component`。另在逐分量的更深循环，末组件斜线/NUL有`return -ENOERR`这样的其它分流，本包只记录分流存在，不重审其真实约定。以上为代码前缀，非字符串规范化结果或成功文件打开证明（PI31-A030/A038/A039/A042–A047/A071/A072）。

**异常/错误指针次序：** `path_init`先解引用`nd->name->name`并在选择路径时访问`s[0]`，`link_path_walk`对`IS_ERR(name)`的测试发生得更晚。因此后一函数的测试**不能证明前一个函数读取无效指针是安全的**。`filename_lookup`前置`IS_ERR(name)`，若为编码错误则返回PTR_ERR而不执行初始化；但并没有显式`name==NULL`门。`do_filp_open`所选定义内部未检查传入name是否NULL/ERR_PTR，真实主要用户调用者通常需先得到有效getname结果。对未知NULL、坏字符区或无效current/fs只能列helper条件，不能从没有门推断已发生panic、权限绕过、完整进程错误或资源损坏（PI31-A013/A035/A043/A075/A076及旧R29-S02）。

## N03｜两条调用链、局部return与清理函数

**打开链：** `do_filp_open(dfd,name,op)`先从`op->lookup_flags`取局部`int flags`，构造栈`nameidata_s nd`，以**root=NULL**调用`set_nameidata(&nd,dfd,name,NULL)`，再调`path_openat(&nd,op,flags)`。`path_openat`先执行`file=alloc_empty_file(op->open_flag)`，若为错误指针则在调用`path_init`之前直接return；只有分配接口返回非错误指针时才取`s=path_init(nd,flags)`。此后`link_path_walk`与`open_last_lookups`交替，若`error==0`再调用`do_open`。无论何种合法返回，已读路径对`terminate_walk`的调用不代表真正清理发生；`error==0`才返回file，另一路调用`fput(file)`再`ERR_PTR(error)`。但旧`CORE-USER-VFS-BASELINE-05::UV-04`已核`do_open`缺少有效返回，**不能跨过它推导确定性error值、成功打开或权限效果**。有效op的来源也受30号`OF30-F01`部分初始化条件限制，须合读已闭合的DOC-FIX勘误（PI31-A025–A033/A054/A055）。

**查找链：** `filename_lookup(dfd,name,flags,path,root)`先`IS_ERR(name)`，若真直接返回PTR_ERR；正常对象则`set_nameidata(&nd,dfd,name,NULL)`（注意签名root参数未被传下去），再`path_lookupat(&nd,flags,path)`。`path_lookupat`先`path_init`，再调用`link_path_walk`和`lookup_last`。若`err==0`，调用`complete_walk(nd)`；其活动正文目前**仅return0**，不能证明实际进行了ref/RCU合法化或文件系统最终校验。若最终err仍为0，则执行`*path=nd->path`（复制两个path_s指针值），并将`nd->path.mnt/dentry`置NULL；随后无条件调用`terminate_walk(nd)`并返回err。`filename_lookup`后再`putname(name)`并返回retval。不能把`putname`理解成释放path对象，也不能因这次指针值复制就认定已经调用path_get/获得引用。另`kern_path`以AT_FDCWD/NULL root到此链，`user_path_at_empty`保留传入dfd再走同一函数（PI31-A034–A042/A057/A058/A070）。

**三个名义退出函数**需按源码而非Linux惯例消费：

- `restore_nameidata(void)`函数体中关于`current->nameidata=old`、链接计数及栈释放均是注释，没有活动语句；打开链在`path_openat`返回后调用它并返回filp。
- `terminate_walk(nd)`当前是空函数；被上述两条后继函数调用，不能代表释放path或回收nameidata。
- `complete_walk(nd)`活动体直接`return 0`，并非已通过实际RCU退出、最终合法性或路径资源审查。

这些函数不做那些活动操作，并不自动意味着某个已有路径引用必然泄漏：`set_nameidata/path_init`本来就只做了结构赋值，本批尚未验证调用前后已有引用、另一个持有者、current.fs生命周期或底层mount引用获取。错误返回、同一指针的借用与资源物理归还是三个不同层级（PI31-A048–A051/A040/A041/A054）。

## N04｜延期、证据、自查及移交

结构化`facts-and-boundaries.yaml`给出20个独立对象、两条路径的前缀步骤、18种有效/错误/未证明来源模型及8组纸面例。`path-init-paths.dot`把函数call、真实字段write、被借用的数据data、结果与空清理体分开。`evidence.md`逐字截取80段固定time真实函数/结构/宏引文、最多5行、独立第二次回源，全部与所属定义位置和语句相符。

`deferred-findings.yaml`在第31批登记`PI31-F01..F05`五项条件候选：dfd只保存不用于当前起点、非NULL root参数不参与当前caller且后续被覆盖、path_init解引用早于link_path_walk错误检查、空串/全斜线与LOOKUP_EMPTY约束未闭合、名义cleanup空实现与引用生命周期证据缺口。均列明反证/限制，不当作五个已发生的独立漏洞，也不宣称全仓去重；05基线已指出dfd起点事实，保留其原能力/问题指针，只是补本批入口证据。

**本批未做：** 用户真实路径输入、C/ASM、仓库脚本、预处理/编译、QEMU/硬件、文件系统/权限、Mount/RCU/锁/路径引用全链、外部libc/API及运行测试。纠正文档关闭是30号的证据合规状态，不是源码修复。成果仍只申请`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`，最高静态验收`PASS_PENDING_LOCAL`，runtime`NOT_RUN`。正式Git提交、两个ref的全文回读、compare及实际评论只在完成之后的本人PR17交付评论中登记，不提前编造提交标识。严格只交六件，正式交付后停止写入，由**MYOS2-A-C04**独立验收；不自发启动32。
