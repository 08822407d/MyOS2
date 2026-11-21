---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-INIT-PATH-SWITCH-47
record_type: correction_46_and_bounded_new_init_path_switch_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
source_pin_short12: a039d9803ade
contract_short12: df273b3b9519
result_base_short12: c176130f0473
date: 2026-11-06
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
prior_batch46_verdict: RETURN
prior_R46_01: OPEN_REVIEW_PENDING
runtime_evidence: NOT_RUN
---

# 第47批｜内核初始化的pwd/root切换与错误消费

## 第一部分：第46批文档定点勘误（R46-01；非第47批源码根因）

主线在固定`CORE-USER-FS-PATH-ACCESS-46-REVIEW-001`裁定：第46批**RETURN**，唯一未闭合的`R46-01`是文档/引用质量问题；核心静态结论须合读R46-S01/S02与R45-S01/S02。执行方无权限自行改成CLOSED或PASS。本节只给C04提交`R46-01_DOCUMENTATION_CLOSURE_REQUESTED`，**不修改旧46六件、其59段旧引文、旧facts/DOT、MANIFEST或原评论**，也不重跑所有59项。

### 1. 定点更正矩阵

| 原46位置 | 主线认定的问题 | 第47批追加的替代证据/解释 | 跨件消费、当前状态 |
|---|---|---|---|
| FP46-A050 | 混入`#ifndef DEBUG`条件与另一个独立`PREFIX_STATIC_AWLWAYS_INLINE`宏，不能作为单宏直接引文 | 新`R46-C001`仅引`#define PREFIX_STATIC_INLINE static inline`真实单宏；`myos_debug_option.h:6`的`#ifndef DEBUG`单列CONTEXT | 旧证据继续原样保存；以后引用此宏只消费C001+外部条件 |
| FP46-A051 | `#else DEBUG`与另一宏混在单直接引文 | 新`R46-C002`仅引空定义`#define PREFIX_STATIC_INLINE`；第11行`#else`只作CONTEXT | 原作者59/59仅历史保留，不能称旧A051已经成为合规直接片段 |
| FP46-A052 | 标题说getter返回void，但摘录没有void类型 | 新`R46-C003`两行：`void`与`get_fs_root(...)`签名；原getter锁/复制路径仍由原FP46-A018–A021支持 | 只有新增C003承担返回类型断言 |
| FP46-A053 | 同样缺`get_fs_pwd`的void | 新`R46-C004`包含`void`及实际函数签名；旧A022–A025仍限函数体顺序 | 不把原只有函数名的A053当完整返回类型证据 |
| facts G14 | 原证据列A019/A023/A005仅涉及getter/路径helper，不能证明DEBUG选择 | `R46-C001/C002`+外部CONTEXT `fs_struct.h:12,22`及`myos_debug_option.h:6,11`；注明**未预处理、未认证目标绑定** | G14替代引用类型是宏定义与条件上下文；不能继续用旧函数调用锚证明编译选择 |
| facts C01；旧图C3 | “init_mount_tree错误分支打印并循环”没有真实while语句的直接引文 | 新`R46-C005`为`color_printk(...)`及`while(1);`，另合读原FP46-A045的`IS_ERR(mnt)`门 | C01在IS_ERR成立、color_printk正常返回时循环；不推断机器输出/线程阻塞或所有条件必达 |
| facts C04；旧图T5 | 原把namespace的A045/A047/A048当“getter搜索没有调用者”的证据，来源类型错误 | 该项只能使用执行者当时的**GitHub默认分支索引代码搜索方法记录**：查询`get_fs_root(`、`get_fs_pwd(`，结果只发现定义，未定位独立调用点 | **没有代码片段能证明搜索没有匹配**，不设伪造的VERIFIED；未检索固定time全仓，不声称无调用者 |
| facts D09；旧图T1/T2 | 原锚只指dget/dput等函数体，不能单独证明dentry结构字段注释 | 新`R46-C006`取真实dentry_s结构体内`d_lockref`注释行，并合读原FP46-A004和固定完整结构 | T1是结构事实；T2须另从dget/dput完整活动体证明无增减；不能由T1单独推出T2 |
| 正文、facts.definition_selection、旧图T4/T5/C3、MANIFEST/评论 | 原材料将不同支持强度和59条全部合规笼统混写 | 本节以上新增引用与支持力替换，旧图不重画：T4引用条件上下文+C001/C002，T5仅方法，C3引用C005且满足分支前提 | 第47批`init-path-switch-paths.dot`只画47链，不以第47图悄悄替换46旧图 |

### 2. DEBUG选择真值场景（纯源码预处理条件模型）

`myos_debug_option.h`用`#ifndef DEBUG`挑选PREFIX宏定义，`fs_struct.h`用`#if defined(FS_STRUCT_DEFINATION) || !(DEBUG)`挑选getter定义；两个判断**并不等价**。以下只是假定DEBUG分别是未定义/数值0/非零整数文字时的条件演算，未执行编译预处理。

| DEBUG条件 | PREFIX_STATIC_INLINE选定文本 | getter定义块的条件（如果FS_STRUCT_DEFINATION未定义） |
|---|---|---|
| 未定义 | `static inline`（C001） | `!(0)`为真，定义块源码选中 |
| 定义为0 | 空宏（C002） | `!(0)`仍为真，定义块源码选中 |
| 定义为非0 | 空宏（C002） | `!(非0)`为假，定义块不由此条件选中 |

若`FS_STRUCT_DEFINATION`已定义，则`defined(FS_STRUCT_DEFINATION)`为真，可选中该块；在DEBUG已定义时，`#ifdef DEBUG`还包含extern声明。具体宏替换、目标链接及其它编译单元的定义归属一律未预处理/未认证，不能仅凭上述表推断链接有无冲突或实际getter调用绑定。

### 3. 原自查数字和正式状态

新勘误`R46-C001..C006`共**6段**，两轮固定源复读6，降级0。原46`FP46-A001..A059`的“59标签/59复读”仅保留为原执行方历史申报；**没有对剩余55项逐一重新完成P9**，不能把新6段与旧59相加成65个通过，也不能自行重签第46批PASS。旧46受影响的断言、facts、DOT与MANIFEST/交付声明统一按本矩阵及R46-S01/S02、R45-S01/S02消费。申请C04另行判定`R46-01`文档闭合，完成前原46仍RETURN。文档整改不涉及内核修复。

---

## 第二部分：第47批独立新分析（N01–N03）

### 4. 内核内部init_chdir与init_chroot的实际局部语义

固定`mykernel/fs/vfs/init.c`顶部文件级注释说明该组函数用于init及早期内核，不是用户态syscall接口。本批没有从这两函数推导用户权限/容器隔离策略。两个函数都声明局部`struct path path;`而**没有显式初值**，初次交给`kern_path`作输出；只有在其**正常返回0且确有有效path输出**等前提下，才可以使用后续path成员、setter和path_put。单纯int等于0不等于证明对象本体及被指向dentry/mount都有效。证据IP47-A001/A006、A002/A007。

| 操作 | kern_path参数和第一道真实门 | 后续源码动作（需前一步有定义、正常返回） |
|---|---|---|
| `init_chdir(filename)` | `kern_path(filename,LOOKUP_FOLLOW\|LOOKUP_DIRECTORY,&path)`；`if(error) return error` | 中间`// error=path_permission(...)`是注释；随后`if(!error)`才`set_fs_pwd(current->fs,&path)`、`path_put(&path)`、`return error` |
| `init_chroot(filename)` | 同样组合标志和`if(error) return error` | 所列能力、权限、security错误判断及`goto dput_and_out`均为注释；活动体调用`set_fs_root(current->fs,&path)`，顺序落到`dput_and_out:`标签，`path_put(&path)`，`return error` |

来源IP47-A001–A011、A047/A048。两个真实单宏为`LOOKUP_FOLLOW=0x0001`、`LOOKUP_DIRECTORY=0x0002`，所选组合位值为`0x0003`；“位被传递”不等于路径解析层已执行真实目录检查、符号链接处理和权限防护。

`init_chdir`第二道`if(!error)`发生在早期`if(error)return`之后，其间没有活动对error的重新赋值；权限行已注释。因此在`kern_path`正常返回0、set_fs_pwd与path_put正常完成的模型里，两道门之间`error`仍为0，最终返回0；第二个门**不是新的错误生产者**。`init_chroot`的`dput_and_out`在活动体没有被任何goto引用：成功调用setter后自然贯穿，不能凭标签制造权限失败回滚分支。不是因函数名就认证chroot完整隔离、或修改pwd（它只调用set_fs_root）；也不把注释权限代码直接升级为可被普通用户调用的权限漏洞。相关限定IP47-A003/A004、A008/A009。

| kern_path/后继状态 | 允许陈述的最远事件 | 必须停止的推断 |
|---|---|---|
| 正常返回非零int | 第一门直接return相同error，不执行setter及本函数的path_put | 不把未调用path_put自动称为泄漏：先要证实失败分支已产生需归还的有效path持有 |
| 正常返回0且path有效、对象和setter都有效 | chdir调用set_fs_pwd；chroot调用set_fs_root；其后path_put请求；正常返回0 | path_put实际仅走无计数活动的dput体，不能说已归还路径引用或完成物理释放 |
| 正常返回0但path未被证明有效 | 后续会**尝试**调用setter；内部path_get先访问path->dentry | 不宣称已安全更新pwd/root或真实目录切换成功 |
| kern_path或setter不返回/出现更早无效输入 | 停在首次无有效前提的操作 | 不凭源码后继位置补出path_put或return0 |

由于两个setter为void、没有可供caller检查的正常错误码，而且当前path_get/path_put→dget/dput未实际增加/减少引用计数（45/46与R45-S01、R46-S01），这里“setter调用、字段赋值、归还请求、真实路径生存期”必须分四层登记。新增源码仅补调用方与真实门，不重做全部path setter审计。

### 5. kern_path直接封装与filename_lookup/path_lookupat输出

固定`namei.c::kern_path`的完整活动体是：

`return filename_lookup(AT_FDCWD, getname_kernel(name), flags, path, NULL);`

参数`AT_FDCWD=-100`属于单宏；`getname_kernel`构造filename_s的实际源码与29号`PN29-F02`等限制合读，不复核完整字符串分配和错误链。有效C字符串/filename对象、未被其他写者改动的入参、返回值可表示和下游正常执行为局部推演前提。来源IP47-A015/A016、A028–A033、A049。

`filename_lookup`的第一道活动门只检查`IS_ERR(name)`，若编码错误则`return PTR_ERR(name)`，**不进入**set_nameidata/path_lookupat/putname；正常继续则`set_nameidata(&nd,dfd,name,NULL)`，调用`path_lookupat(&nd,flags,path)`，其后`putname(name)`，最终`return retval`。这一早门不等于对任意NULL、悬空指针或其它非法表示进行全面验证。最早无效路径须停于实际失效访问；不能预先把`getname_kernel`内部失效变成filename_lookup正常负errno。来源IP47-A017–A021、A034/A035。

`path_lookupat`先调`path_init`，按`link_path_walk`的int err与`lookup_last`的const char*分开循环。**只有**实际到达`if(err==0)`的输出分支，才`*path=nd.path`（复制path_s两个指针值）、清本地`nd.path.mnt/dentry=NULL`；到达尾部才`terminate_walk(nd)`、`return err`。若之前的callee失效/不返回，则不得补出输出、清本地、putname或最终errno。由此能证明**整数输出时点**，不能证明实际成功路径已持有有效目录项/挂载引用、LOOKUP_DIRECTORY被完全执行、符号链接已正确解析或/..切换完成。来源IP47-A022–A027、A036–A039，既有PI31-F05、LC32-F04约束不变。

失败时`init_chdir/init_chroot`本体直接返回，没有先调用`path_put`；在`path_lookupat`未成功交出有效路径的条件下，这不是凭空丢失一个已取得引用的证明。真实路径引用获取及释放helper活动体也尚不支持名字推断的加减计数，见R45/R46。

### 6. devtmpfsd调用者：仅init_mount结果被消费

固定`drivers/base/devtmpfs.c::devtmpfsd`先`int err=0`，再`err=init_mount("devtmpfs","/","devtmpfs",DEVTMPFS_MFLAGS)`，且`if(err)goto out`。因此对于`init_mount`**正常返回非零**的模型，跳过后面两次目录切换及complete/workloop，进入`out: return err`；对正常返回0且相关前提有效的模型，继续依次：

`init_chdir("/..");` → `init_chroot(".");` → `complete(&setup_done);` → `devtmpfs_work_loop();`

这里两次目录切换的**int返回值都被丢弃，没有赋回devtmpfsd局部err，没有if判断**。这不仅表示“错误不透传”，还意味着若第一个init_chdir正常返回非零，**仍会调用第二个init_chroot**；若第二个正常返回非零，也不会因该返回值而跳过后续complete/workloop。先行函数若内部失效/不返回则必须停在那里；不应假装后面的complete已经执行。路径字符串`"/.."`与`"."`只是调用字面值，不等于实际路径查找解决了跨挂载根或安全限制；该文字后的“will traverse into overmounted root”注释不能作为执行证据。来源IP47-A040–A046。

`devtmpfsd`局部err在两次目录切换之后**没有再被活动语句赋值**，所以若init_mount正常返回0并且所有后继函数最终都正常返回、落到`out:`，返回值仍为0，独立于任一次init_chdir/chroot的非零结果。但`devtmpfs_work_loop`含循环，未审是否返回；**不能无条件说devtmpfsd已经return0**。同样`complete`只是实际调用，不认证线程/等待协议或工作循环初始化成功。用户态chdir/chroot接口并非本批调用源。来源IP47-A043–A046。

**新增条件性候选`IP47-F01`：** 两个目录切换的返回值在devtmpfsd中被忽略，任何正常返回的非零int不会通过本函数的error门影响后续控制。可能使后续流程在未证pwd/root切换成功的条件下推进；要判断真实故障须查是否存在实际非零生产者及路径输出、线程进入/后继依赖等证据。本批只有源码控制事实与条件推论，没有实际运行复现，不建议直接归因于真实设备故障。

### 7. 去重、裁定申请与边界

第46批原审查状态仍`RETURN/R46-01 OPEN`，本目录六件中的勘误独立申请C04闭合；47新分析只申请独立`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`。IP47-F01暂作needs_evidence普通候选，保留29::PN29-F02、31::PI31-F05、32::LC32-F04、45::FS45-F01/F02、05::UV-10等原qualified_id/status，无重编号或关闭。没有新修复许可。

禁止审完整init_mount、getname_kernel分配器、path_init/link_path_walk/lookup_last深层、devtmpfs_work_loop/complete、完整挂载/线程/权限、SLAB、VFS生命周期；未执行源码、预处理/编译、QEMU/实机、仓库C/ASM/脚本、Graphviz或额外API。信息仅为固定源码的局部控制与有限输入模型；真实内核正确性判定未签。
