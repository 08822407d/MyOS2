---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LAST-COMPONENT-32
record_type: bounded_last_component_dispatch_and_return_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
conversation_display_name: MYOS2-A-C03c 代码分析执行续建
mainline_reviewer: MYOS2-A-C04 内核分析主线续建
date: 2026-11-03
read_channel: connector
source_branch: time
source_pin_short12: a039d9803ade
task55_short12: bf856f2ddb2d
review31_short12: bf856f2ddb2d
result_base_short12: 45a065270e28
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
---
# 第32批：最后一个路径分量的元数据、两类分派与类型化返回

## L00｜冻结输入及R31-S01/S02强制口径

仅分析固定`time@a039d9803ade`的`link_path_walk → walk_component`前缀及`lookup_last/open_last_lookups`的末分量分派，以及到`path_lookupat/path_openat`外层循环的返回消费。已完整读取固定55任务`bf856f2ddb2d`与同一固定提交31主线验收回执，31为`PASS_PENDING_LOCAL`、`returned_scope=[]`，必须合读：

**R31-S01**：`link_path_walk`返回**int**；`lookup_last/open_last_lookups`返回**const char***。外层`err/error`并不是末分量函数的返回值。最后一次整数结果为0且末分量返回NULL才在当前循环自然退出；非NULL编码错误指针在下一轮转换为整数错误。前缀返回0不是完整路径查找已成功。`do_open`缺少非void返回值不意味着函数已证“永不返回”。

**R31-S02**：只有源码真正表达的门才能叫“控制门”。有效filename、合法op、有效dentry/current/fs、下游正常返回都是**分析前提**而非函数内额外检查。`do_filp_open`实际只从`op->lookup_flags`取flags，不能将其等同`op.open_flag`，也不能凭`O_CREAT`参数名称断定真实用户创建标志触发。需以直接定义体短引文核每条语义，标题、图与证据相互匹配。

上游31仅定点读取N02/N03、facts中M04/M05/M16/M17及相应证据；30旧材料如需引用`OF30-F01/F04`必须合读已关闭的DOC-FIX勘误`e33103ca7d9e`，其文档退回关闭不构成内核运行认证。05::UV-04和旧能力路径以原ID导航，不重做整批。固定源码金丝雀、PR17头、单写区和工作协议P1–P13见MANIFEST。以下均要求路径字符可读且NUL终止、nameidata/dentry/文件对象有效以及所有被调用子函数按所述条件正常返回；任一未定义访问或非返回发生就停在该已证前缀。

## L01｜last.len/name/type的真实生产者

`nameidata_s`有`qstr_s last`和单独的`int last_type`。`qstr_s`在固定`mykernel/include/linux/fs/dcache.h`有`size_t len`、`const unsigned char *name`，**不是单独动态分配的分量字符串**。`LAST_NORM/LAST_ROOT/LAST_DOT/LAST_DOTDOT`依其匿名枚举出现次序在通常C枚举数值模型中为0/1/2/3，直接定义引文分拆为LC32-A004/A087/A088/A089；不会从Linux习惯而不是本仓定义补数值（LC32-A001–A004/A087–A089）。

进入`link_path_walk(const char *name,nameidata_s *nd)`时，先`nd->last_type=LAST_ROOT`，再`IS_ERR(name)`，错误编码指针则直接`PTR_ERR(name)`给整数返回；只有经过这个门才跳过局部`name`前导的`/`。此后如果看到NUL（空字符串或只有斜线），`return 0`且last_type仍LAST_ROOT，尚未进入`last.len/name`分量写入（LC32-A020–A022）。这发生在当前函数内，不能把空串直接认定为被`LOOKUP_EMPTY`允许，也不能把斜线前缀当已完成最终路径解析。

非空串先用`int i=0`计数直到NUL或斜线；每段默认`LAST_NORM`，仅当前段以点开头时看长度：长度**恰为1**写LAST_DOT；长度**恰为2且第二字符也是点**写LAST_DOTDOT；如`"..a"`仍LAST_NORM。随后按顺序赋`nd->last.len=i`、`nd->last.name=name`、`nd->last_type=type`，再推进**局部**`name += len`并跳过相邻多个斜线。借用的`nd->last.name`保存这一段的起点地址，不是被推进的局部name的新地址；该起点通常仍指原始C字符串中含有后继`/`的存储，不能自动当作独立NUL终止字符串（LC32-A023–A028）。

若推进后是NUL，实际`return -ENOERR`；固定`uapi/asm-generic/errno-base.h`的`#define ENOERR 0`，所以这是**int 0**，不是错误或非零哨兵（LC32-A005/A029）。这里常被直觉误读：该返回0只说明`link_path_walk`结束一个分量扫描，末分量的普通/点/点点分派仍由外层另一函数完成。若还有后继分量，该函数调用`link=walk_component(nd)`，仅检查`IS_ERR(link)`并可直接返回`PTR_ERR(link)`；正常NULL则继续扫描原始字符串。假设出现普通非NULL续串指针，此中间层也没有赋给局部name，因而不能把它和外层`s = lookup_last(...)`的更新混同；当前直接`walk_component`定义未展示这种普通续串生产者（LC32-A030/A037/A045）。进一步的真实dentry快慢查与字符串追随不在本包扩审。

**纸面有限例：**

| 输入 | prefix后的last_type和last.len/name | link_path_walk这一轮结果 | 后续仍需什么 |
|---|---|---|---|
| `""`、`"/"`、`"///"` | LAST_ROOT；没有新的分量写入 | int 0 | 外层调用末分量函数 |
| `"a"` | LAST_NORM，len1，name指原串`a` | `-ENOERR=0` | dentry查询/step_into |
| `"a/"` | LAST_NORM，len1，name仍指`a/`首字节 | `-ENOERR=0` | 尾斜线约束并未在前缀完整证明 |
| `"."` | LAST_DOT，len1 | `-ENOERR=0` | handle_dots直接NULL分支 |
| `".."` | LAST_DOTDOT，len2 | `-ENOERR=0` | follow_dotdot/step_into条件 |
| `"a/b"` | 先记录`a`并调用中间walk_component，后在其正常非错误返回下记录`b` | 最后组件`-ENOERR=0` | 中间dentry有效性未证 |
| `"a/.."` | 首段同上，末段LAST_DOTDOT | 条件int0 | parent/mount为不透明边界 |

上述样例均只是固定代码前缀与有效字符串/对象的局部模型，并非执行了文件系统或证明某一路径成功。

## L02｜lookup_last与open_last_lookups的实参数值域

`lookup_last(nd)`的全部活动内容是`return walk_component(nd)`（LC32-A031）。`walk_component`在`nd->last_type!=LAST_NORM`时直接`return handle_dots(nd)`；普通末分量先从`__d_lookup(parent,&nd->last)`获取`dentry_s*`，IS_ERR则ERR_CAST返回；若NULL，调用`__lookup_slow(&nd->last,parent,nd->flags)`，其错误指针也ERR_CAST；只有未提前返回时才`return step_into(nd,dentry)`（LC32-A032–A037）。快查、慢查的类型/错误门已核到此，内部dentry分配、FAT查询、挂载和权限结果均保持不透明；不能凭“可返回普通dentry_s*”证明实际发现了任何文件。

`handle_dots(nd)`**只在LAST_DOTDOT**进入follow_dotdot/step_into：`follow_dotdot`的结果如果IS_ERR便ERR_CAST；NULL时对当前nd.path.dentry调用step_into，否则对parent调用step_into；若`error`非NULL就把它作为`const char*`返回。LAST_ROOT和LAST_DOT都跳过该分支，执行函数尾`return NULL`（LC32-A038–A042）。即便这条NULL成功分派已经在源码中，不能由此保证root/pwd/挂载有效或文件权限得到检查。

`open_last_lookups(nd,int open_flag)`一开始声明`dentry_s *dentry, *dir=nd->path.dentry`和`const char *res`。同样对非LAST_NORM直接`handle_dots`；普通分量则在`!(open_flag & O_CREAT)`**成立时**进行快查`__d_lookup`、NULL时慢查`__lookup_slow`、IS_ERR提前ERR_CAST、非NULL可goto finish_lookup。代码后面的`else { dentry=lookup_open(...); }`块**全部注释**。最终有一个标签`finish_lookup:`，执行`res=step_into(nd,dentry); return res`（LC32-A048–A054）。因此若该O_CREAT位条件为真，活动分支可绕过dentry唯一已读赋值来源，随后尝试使用**未确定赋值的本地dentry**。但这项风险必须再受“传给它的值实际来自何处”限制，不可立刻声称已复现。

最重要的数值/命名错位：

- `do_filp_open`实际写`int flags=op->lookup_flags`，将它交给`path_openat(&nd,op,flags)`；path_openat实际`open_last_lookups(nd,flags)`。因此open_last_lookups的`open_flag`**实际是LOOKUP标志域**，不是`op->open_flag`中的O标志，也不是`op->intent`。
- 固定`O_CREAT=00000100`（八进制64＝0x40），恰与`LOOKUP_RCU=0x0040`同值；但`LOOKUP_CREATE=0x0200`（512）与它不同。真实`build_open_flags`对于用户`O_CREAT`在`op.intent`置LOOKUP_CREATE（对O_EXCL另置LOOKUP_EXCL），`op.lookup_flags`主要依据O_DIRECTORY/NOFOLLOW/RESOLVE设置FOLLOW、DIRECTORY及约束位。当前选定正常成功的`build_open_flags`调用和`do_open_execat`直接构造路径**没有已证生产LOOKUP_RCU=0x40**的语句。
- 所以对于普通`O_CREAT`输入且有效flags构造成功的纸面场景，`open_last_lookups`的`flags&O_CREAT`仍可能为0，走实际dentry快慢查分支。若是单独helper输入`op.lookup_flags=LOOKUP_RCU`，数值门会为真，并进入上述dentry未赋值前缀；**这个helper-only输入未在所选实际开放入口得到合法生产者证明**。原`OF30-F01`可使op.lookup_flags未被初始化，但不等于确定它将等于0x40，不能拿未定义值作为真实位生产者。

证据：LC32-A013–A017/A066–A079及固定30勘误的限定；更深`do_open`实际参数也是外层flags，不能按其形参`open_flag`名称自动还原O_CREATE。LC32-F01、LC32-F02在deferred中作为同一潜在标志域失配的条件链保留，不宣称两个已证独立漏洞。

另一个有限分支：若快查NULL且慢查**也**正常返回NULL，open_last_lookups会越过`if(dentry) goto finish_lookup`并自然落到同一标签，把NULL交给step_into。该情形在C控制图中成立，但`__lookup_slow`是否可在这一路定义良好地返回NULL，没有通过本包的允许范围证实；不能凭这个helper模型判断发生了实际空指针fault。见LC32-F03。

## L03｜指针返回如何被外层while消费

**真实返回生产者：** `step_into(nd,dentry)`调用`int err=handle_mounts(nd,dentry,&path)`。若err负数，直接`return ERR_PTR(err)`；否则`nd->path=path`并`return ERR_PTR(-ENOERR)`，而`ENOERR=0`，`ERR_PTR(0)`是**NULL指针**，不是非NULL正常续串（LC32-A005/A009/A043–A046）。实际`handle_mounts`只是将当前mount/dentry写入path并返回`traverse_mounts(path)`，其可否正常完成、返回何种有效值及存储状态，本任务不扩审。显式`handle_dots`尾部同样返回NULL。结合已读`walk_component/open_last_lookups`的所有定义良好普通出口，当前没有找到真正生成**普通非NULL、可读的续串字符串指针**的直接语句；只是`const char*`类型允许这种值，不能凭类型制造该生产者。

**查找外层循环**在`path_lookupat`中为`while (!(err = link_path_walk(s,nd)) && (s=lookup_last(nd)) != NULL);`。前一项给`err`写**整数**；仅当其为0时才调用末分量函数，把返回的**指针**写给`s`。若该指针NULL，循环自然终止且保留err0，之后才进入`if(err==0) complete_walk(nd)`、条件性输出`*path=nd->path`及局部清空（LC32-A055–A059）。成功还需下层路径对象和输出指针有效，`complete_walk`当前活动体直接return0，并非最终验证已实际执行。

**打开外层循环**在`path_openat`中为`while (!(error=link_path_walk(s,nd)) && (s=open_last_lookups(nd,flags))!=NULL);`。在它之前必须先有`alloc_empty_file(op->open_flag)`正常非错误返回，否则在`path_init`之前就直接返回编码错误指针。循环若以error0和s=NULL终止，才调用`error=do_open(nd,file,flags)`；历史`CORE-USER-VFS-BASELINE-05::UV-04`已经指出这个非void do_open函数体调用vfs_open后缺少确定return，所以`error`后续值、文件返回和fput分流在真实编译结果未证前**不能**被断言为成功或失败。缺return亦**不等于永不返回**（LC32-A060–A065/A086）。

两套外层while都可在以下情况作清晰的局部类型矩阵：

| 末分量结果来源及前提 | s指针类别 | 下一次while行为 | 整数err/error归属 |
|---|---|---|---|
| link_path_walk已直接返回负整数 | 不调用末分量；s原值没有在右项重写 | 循环立即退出 | link_path_walk负数 |
| link_path_walk返回0，末分量给NULL | NULL | s!=NULL为假，退出 | 保留link_path_walk本次0 |
| link_path_walk返回0，末分量给非NULL ERR_PTR(-errno) | 错误编码指针（仍非NULL） | 先继续，下一轮link_path_walk设置LAST_ROOT，再经IS_ERR转换PTR_ERR | **下一轮**得到负整数 |
| 假想普通非NULL续串 | 可读字符串指针，但未有所选真实生产者 | 下一轮link_path_walk读取新串（如仍有效） | 取决于下一轮返回，不预判 |
| 分支使用未确定dentry或下层异常/非返回 | 不保证C定义良好的可用s | 在已证前缀停止 | 不宣称后续return、清理或CPU终态 |

作为对照，`link_path_walk`对**中间**分量`link=walk_component(nd)`只检查`IS_ERR(link)`，不会把普通非NULLlink赋给它自己的局部name；只有外层while才把末分量结果赋给s。两层都允许错误指针类型，但消费与赋值的发生位置不同（LC32-A030/A056/A062）。`IS_ERR(NULL)`在选定MAX_ERRNO=4095及64位数值模型中为假，不能由此宣布任意NULL对象可安全解引用，也不能倒推运行时getname一定产生NULL。

空串、单独斜线或多个斜线的`LAST_ROOT`经`handle_dots`普通返回NULL可使外层loop在int0时自然退出；不等于已经测试`LOOKUP_EMPTY`或权限。末分量LAST_DOT也直接NULL；LAST_DOTDOT需经过follow_dotdot/step_into的条件链。若出现中间组件错误编码，link_path_walk内部已直接返回负int，**外层根本不会调用末分量函数**。所有下游资源与mount状态保持前提而非认证。

## L04｜延期、交付六件与停止点

新条件候选`LC32-F01`到`LC32-F04`记在deferred：末分量形参O_CREAT与lookup标志域错位、0x40为真时局部dentry未确定赋值、快慢查双NULL假设下的fallthrough，以及NULL/ERR_PTR到整数循环的证据边界。F01/F02显式同源，F03需要尚未确证的NULL返回生产者，F04属于31 R31-S01边界延续；四个编号**不代表四次已复现漏洞或四个独立根因**。历史`UV-04`、`PI31-F03/F04`、`OF30-F01/F04`与R31-S01/S02均保留原限定ID，不改旧记录，不作全仓去重。

本批六件为`MANIFEST.md`、本`last-component.md`、`facts-and-boundaries.yaml`、`last-component-paths.dot`、`deferred-findings.yaml`、`evidence.md`。facts包含24个独立对象、9条真实返回生产者/消费者、6种标志源模型、15种指针与整数消费案例、12个纸面例；DOT区分实际函数调用、数据/赋值、真假控制门、NULL和ERR_PTR及不透明VFS部分。evidence有89段来自固定源9个定义文件的1–5行直接引文，已另行二次回读并核函数/宏/枚举/结构体定义归属。MANIFEST登记P9门和实际输入，实际Git写入/远端回读只在完成时填本人PR17交付评论，不预填伪SHA。

**NOT_RUN：** 未修改内核/旧件、运行C/ASM/仓库脚本、预处理/编译、QEMU/硬件、真实用户路径、完整dcache/FAT/挂载/权限/RCU/引用生命周期、外部libc/镜像或额外付费模型/API。不证明任何具体文件成功打开、创建或权限效果，也不宣称空清理函数造成全局泄漏。当前只申请`READY_FOR_MAINLINE_REVIEW`、主线验收`PENDING`，上限`PASS_PENDING_LOCAL`；正式交付后C03c停写，由C04独立验收本批，不开始33。
