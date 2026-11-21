---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-DENTRY-LOOKUP-33
record_type: bounded_dentry_lookup_static_analysis
date: 2026-11-03
source_snapshot: time
source_pin_short12: a039d9803ade
execution_base_short12: d6444512a213
lead_publication_short12: 4b39bce5c8c6
contract_package_id: MYOS2-C03C-TO-C04A-20261103-01
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第33批：目录项快查、对象分配、慢查回调与边界

## D00｜来源与身份

第33批任务由Owner明确授权接收 ZIP 内附件版 `MYOS2-C04a-batch33-startup-package.md`，包ID为 `MYOS2-C03C-TO-C04A-20261103-01`。首次接收时C04尚未将32回执/33合同发布到GitHub；本轮预检期间，C04合法向 `agent/MYOS2-LEAD-002` 提交 `4b39bce5c8c6`，正式增加56号任务书、57号执行续接、32回执和检查点。新的固定合同与附件版对本批所选源、六件文件、权限边界和D00–D04内容的要求一致。**执行者没有代写这些lead文件**，C04主线的新提交不是PR17的写入冲突。

本批执行者是 `MYOS2-LEAD-002-CHAT-EXEC-04`，仅在原PR17的 `claude/dazzling-cori-q0dnyt` 增加 `user-dentry-lookup-33/` 六件及自己的交付评论，不新建PR或分支。源码固定 `time@a039d9803ade`，首次写前预期 `d6444512a213`；PR16由C04写主线。第31批R31-S01/S02、第32批R32-S01/S02均为强制消费条件。旧30的DOC-FIX勘误、旧UV-04及LC32各ID不被本轮覆盖或独立重新裁定。控制流分析均以有效结构体、可读字符区域、合法长度及被调用函数在说明的条件下正常返回为前提；不满足时只停在实际已证的前缀。

## D01｜`__d_lookup`：查询长度并非完整名字相等

从固定 `mykernel/fs/vfs/dcache.c::__d_lookup` 全函数得出的链是：初始 `dentry=NULL`；从 `parent->d_subdirs.anchor.next` 沿双向子链表读取元素；`container_of(dir_lp,dentry_s,d_child)` 取得候选；在 `dir_p!=NULL` 且 `strncmp(name->name,dir_p->d_name.name,name->len)==0` 时记录**第一个**符合谓词的候选并停止；若迭代完毕未命中则返回NULL。直接锚点DL33-A004至A007，结构体字段DL33-A001至A003。当前选中函数没有主动生产ERR_PTR的返回语句，且未在其中检查候选长度、正inode、dget、锁获取或重验证；这些**局部未见**事项不等于全仓不存在其它相应机制。

查询方 `qstr_s` 的 `name->len` 与候选方 `d_name.len` 是两个独立字段。上述比较把查询长度传给 `strncmp`，并**未同时检查候选长度是否与查询长度相等**。在固定 `mykernel/lib/string/string.c::strncmp` 的 `#ifndef __HAVE_ARCH_STRNCMP` 条件编译体实际生效、双方名字字节有效且候选正确NUL结束的有限模型中，查询 `a`（len=1）与候选 `ab`（len=2）比较只读一个字节，结果0，快查便会把 `ab` 当作命中；反向查询 `ab`（len=2）对候选 `a` 的第二字节遇NUL而不同，返回非0。双方相同则匹配，首字节不同则不匹配；空qstr的len=0会让该strncmp立即返回0，属于helper-only而非已证用户路径。证据DL33-A008至A010、有限纸面矩阵见facts中的 `finite_string_cases`。

上述名称碰撞只是**固定源码选定函数模型中的候选误配**。完整真实路径是否存在那两个目录项、链表顺序和作用域、构建是否绑定该条件strncmp实现、上层对象是否正目录项、是否真正打开了文件，都没有本批运行证据。不能从前缀相等推出已复现权限问题。没有候选dentry的有效性、字符区有效长度或可解析列表前提时，不继续计算，也不造CPU终态。条件候选登记为DL33-F01。

## D02｜`__myos_d_alloc`：两次分配、错误指针与四重身份

`__myos_d_alloc(sb,name)` 第一步 `kzalloc(sizeof(dentry_s), GFP_KERNEL)` 申请**dentry对象**；返回NULL时立即 `ERR_PTR(-ENOMEM)`。第二步 `kzalloc(name->len+1,GFP_KERNEL)` 申请**独立名字字节缓冲**，地址写入 `dentry->d_name.name`；此处返回NULL则只看见 `kfree(dentry)` 并返回同一错误指针。注释描述“returns NULL on insufficient memory”不覆盖真实活动代码的ERR_PTR返回契约。证据DL33-A011/A012。

两次成功后，代码存储 `dentry->d_name.len=name->len`，执行 `memcpy((void*)dentry->d_name.name,name->name,name->len)`，**复制len个字符字节，不是把原name指针直接赋给d_name.name**。依次置 `d_flags=0`、`d_inode=NULL`、`d_parent=dentry`、`d_sb=sb`、`d_op=NULL`，初始化 `d_child` 与 `d_subdirs`，返回分配的原dentry指针。证据DL33-A013至A016。应分别追踪 dentry本体、另分配的名字缓冲、借来的qstr结构、qstr.name原字符区；四者不可混用。

本仓 `mykernel/mm/kmalloc/slub.h::kzalloc` 所读定义以 `__GFP_ZERO` 调kmalloc。若实际构建采用该定义、分配成功且名字缓冲长度计算 `len+1` 无溢出，同时可读源区域至少含len字节，则新缓冲中拷贝的末字节之后还有零初始化的第len字节，条件性构成NUL。它不是对坏len、溢出、无效指针、编译器实际选择的无条件担保。此处未运行分配器或测量内存。

定点对照紧邻的 `d_alloc` 包装函数：它拿到 `__myos_d_alloc` 的返回后只测试 `dentry==NULL`，与该函数实际失败返回 `ERR_PTR(-ENOMEM)` 不同（DL33-A017/A018）。这属于本次边界观察，未追踪包装器的所有调用者，也没有宣称真实发生非法地址访问。第二次分配失败的早退释放对象，不等于证明所有外部引用/释放生命周期。相关条件延期项DL33-F03。

## D03｜`__lookup_slow`：回调的返回指针和原对象填充是两回事

`__lookup_slow(name,dir,flags)` 首先调用 `__myos_d_alloc(dir->d_sb,name)`，如 `IS_ERR(dentry)` 则原样返回错误指针。随后直接调用 `dir->d_inode->i_op->lookup(dir->d_inode,dentry,flags)`；固定 `inode_ops_s` 的真实类型是 `dentry_s *(*lookup)(inode_s*,dentry_s*,unsigned)`（DL33-A019至A021、A025）。传入的是分配的**原dentry对象**。该回调的返回指针并没有赋值回局部变量dentry。

活动代码只是将回调返回值同NULL比较。为NULL时，`kfree(dentry)` 后返回 `ERR_PTR(-ENOENT)`。回调非NULL（不论其指向原对象、另一普通对象或错误编码地址）则走后续，实际写 `inode=dentry->d_inode`，因此读取的仍然是**原对象的字段**，不是回调返回对象的inode；再读取 `inode->i_mapping`、`inode->i_size`。因此“回调返回非NULL”只证明通过这个布尔条件，**不足以单独证明原dentry已绑定有效inode**。源码DL33-A021–A023，字段定义DL33-A026–A029。

有限类型矩阵分成：①分配失败 `ERR_PTR(-ENOMEM)`，不执行回调；②回调NULL，`kfree(dentry)` 后 `ERR_PTR(-ENOENT)`；③回调返回原对象并已有效填充原dentry.inode，条件性继续；④回调返回别的非NULL对象但原inode也有效，代码仍消费并返回原对象；⑤回调返回别的非NULL但原inode无效，在首次解引用处停止，不能继续推到发布或CPU终态；⑥回调返回ERR_PTR时因不是NULL而不作为此回调的错误传播，后续仍需原inode有效；⑦页数组分配若NULL，所选函数未见立即检查，后续发布属附带有效前提。真实FAT回调究竟产生哪些类别及副作用未纳入本包，详见facts，条件项DL33-F02。

在原inode、mapping、size有效的条件下，函数把 `nrpages=ALIGN(i_size,PAGE_SIZE)>>PAGE_SHIFT`，当nrpages正时用kzalloc给 `page_array` 赋值；源中该次分配后未见NULL检查。随后 `list_header_add_to_tail(&dir->d_subdirs,&dentry->d_child)`、`dentry->d_parent=dir` 并**返回原dentry**（DL33-A022至A024）。这不保证页分配成功、实际文件正查成功、引用和并发安全，亦不证明物理资源数量。回调NULL分支直接kfree原dentry，但另配名字缓冲没有在该局部出口明确释放，尚缺所有权和分配器完整证据，列DL33-F04，不能声称独立可复现泄漏量。

### 与LC32-F03及两类上层消费的限定关系

对这段完整所选定义而言，**定义良好完成的return出口只有**：分配失败时 `ERR_PTR(-ENOMEM)`，回调NULL时 `ERR_PTR(-ENOENT)`，正常走到底部时返回原来分配的dentry。没有正常NULL返回语句。这不同于 **快查__d_lookup返回NULL**，后者才是上层进入慢查的真实条件。因此LC32-F03原先“快慢查都正常返回NULL导致跌落”属于当前缺乏慢查正常NULL生产者的helper假设，需用本批结果收窄消费；旧32六件和问题ID不修改，绝不能宣称整个VFS无问题。

`walk_component` 对普通分量先调用快查，快查NULL才慢查，慢查 `IS_ERR` 时 `ERR_CAST`，否则把选择的dentry交 `step_into`。 `open_last_lookups` 对末分量在其实际 `!(open_flag&O_CREAT)` 条件下也执行快查/慢查、错误检查并在非NULL时跳 `finish_lookup`。不过R32已证明所谓open_flag实来自 `op.lookup_flags`，不能将它解释为真实用户 `O_CREAT`：0x40数值域及LC32-F01/F02前提保持不变。选定慢查没有正常NULL出口，并不会修复helper-only 0x40绕过dentry赋值的另一条条件链。证据DL33-A030至A041。

当step_into正常到达尾部，在固定 `ENOERR=0` 下返回 `ERR_PTR(-ENOERR)=NULL`；错误指针和外层 `s`/整数 `err` 的区分依旧合读R31-S01及32结果。不重做open/fput或路径最终正确性；UV-04的 `do_open` 非void返回不确定仍阻断最终文件成功断言（DL33-A038–A041及历史回执）。

## D04｜结果、自检及未执行

本次六件为MANIFEST、本文、JSON兼容YAML facts、DOT、延期YAML、直接证据evidence；直接事实的48段新引文只在evidence计数，不用原89或80条旧引文凑数。矩阵把条件、反证、对象身份、有效执行前提、真实门与停止点分开。DL33-F01至F04为普通延期候选，**不是四个已证独立漏洞**，旧qualified_id和主线裁定全保留。

没有修改内核、用户程序或旧成果；没有运行C/ASM、仓库脚本、预处理、编译、QEMU/硬件、真实用户路径、FAT回调、目录盘IO、挂载、全链权限/RCU/锁/引用或额外付费模型/API。完整本地P9闸门未跑，只有已有工具支持的静态引文/结构/有限模型自检。本批状态仅 `READY_FOR_MAINLINE_REVIEW`，`mainline_acceptance=PENDING`，正式交付后STOP_WRITING，交C04独立验收，不启动34。
