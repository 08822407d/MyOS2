---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-DIRECTORY-ENUMERATION-59
record_type: bounded_directory_enumeration_with_prior58_correction
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
conversation_display_name: MYOS2-A-C05a 代码分析执行
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-08
base_snapshot: time
source_pin_short12: a039d9803ade
execution_base_short12: 8de2a76f5f73
read_channel: GitHub_connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00_status: CORRECTION_SUBMITTED_PENDING_MAINLINE
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
phase: 2
runtime_evidence: NOT_RUN
---

# 第59批：目录枚举fd分派、FAT游标和用户dirent记录

本报告只审89号限定的固定time静态源码。源码调用请求、对象/指针合法性、函数正常返回、真正写入用户缓冲、介质上的真实目录状态必须分开；任何结论都不得跨越其第一个未定义访问、未证回调或整数范围。逐字锚点见evidence.md，机器可读矩阵和46个有限模型见facts-and-boundaries.yaml。

## D00 / 第58批R58-01文档纠正（仅申请，不自签闭合）

**CORRECTION_SUBMITTED_PENDING_MAINLINE；58原裁定仍RETURN_DOCUMENTATION_ONLY。** 57号R57-01/R57-02已由C05独立闭合，组合57为PASS_PENDING_LOCAL，本轮不重新验收或修改其记录。

**R58-01定义与物理行替代。** 原58/evidence.md的B24把独立LOOKUP_CREATE与LOOKUP_EXCL两宏并列置于一个LOOKUP_CREATE标签下，属跨独立宏定义。新59/evidence.md的D01、D02严格各自只引一条真实宏定义；这两条不能算同一宏的两行。原58的B25被主线判为非精确源码连续片段并超过要求行数；新D03直接复制固定mykernel/fs/vfs/init.c::init_mkdir的连续5个实际物理行，自kern_path_create起，经IS_ERR、PTR_ERR和vfs_mkdir至done_path_create，没有插入、删除空行。这一段仅支持内核mkdir的直接调用关系，不证明运行创建成功。

**计数与状态。** 原58作者87条标签中B24/B25两条被本追加替代，余85条按历史保留而本批未全量核验。59新增D00为3标签，业务A/B/C为69标签，总新72条。原87/87字面/定义归属全合格声明不能继续直接消费；新证据不让85条历史片段自动升级。新MANIFEST与facts.prior58_disposition、deferred.prior_return_items、evidence.D01–D03相互索引，C05仍有唯一闭合权。

**R58-S01技术限定。** 58正文的foo.写作6→5是笔误；实际字符串foo.长4，删除尾点后长3，58 facts原模型4/3不变。__lookup_hash的callback返回NULL不能直接证明其先前未原位修改、撤销或改变传入dentry，若要沿用构造时d_inode=NULL的模型，必须另设回调后对象有效及字段未改变的前提。58::NG58-F01的独立增量限上层do_*或kern_path_create向putname传递编码错误指针；__myos_getname错误指针未检查的上游来源另有EI28-F01（status=needs_evidence）、PN29-F01/F03（status=needs_evidence）和PN29-F04（status=deferred_owner_not_ready）。不把旧根因重编号为58/59新问题；NG58-F01原status=needs_evidence不变。

**R58-S02图语义限定。** 原58图中last→parentat是qstr来源/传递而非last这个数据对象调用或函数RETURN；cachehit→hash是缓存命中后__lookup_hash的return选择；alloc→callback条件归属于__lookup_hash在分配正常完成后的控制，不是allocator主动调用lookup；namebad是错误值与分析停止提示，首次非法访问之后没有能安全继续执行putname的路径。原图不覆盖，本批图采用独立的59调用与数据边。

**原58六件与旧评论替代消费表**

| 冻结入口 | 新消费要求 |
|---|---|
| 58/MANIFEST.md | 原87仅作者历史申报；B24/B25不能继续作为P2合格，余85未全重验 |
| 58/vfs-mutation-gates.md | B24依D01/D02单宏、B25依D03精确源；foo.正确4→3，NULL回调须补对象状态前提 |
| 58/facts-and-boundaries.yaml | 旧status沿真实来源字段，回调副作用/图语义合读S01/S02 |
| 58/vfs-mutation-gates-paths.dot | 上述数据/控制归属不得当实际新程序步骤 |
| 58/deferred-findings.yaml | NG58-F01仅上层putname错误消费；EI28/PN29保留真实原ID/status |
| 58/evidence.md | B24/B25历史保留但定义归属/精确性被D01/D02/D03替代 |
| 58评论6720176655 | 不改原READY/STOP_WRITING交付记录，须合读C05的58 RETURN及59 D00 |

第58批其它A/B/C核心技术链按R58-S01/S02保持限定消费；本次并非重做58，也不把D00的三条纠正算成三个新漏洞。

## A / G01：系统调用fd借用、直接分派及最后返回

readdir.c::iterate_dir是**独立helper**，活动体先查看file->f_op->iterate_shared是否存在；没有则保留-ENOTDIR，仍在有效对象条件下把file.f_pos赋给ctx.pos并在出口回写。然而系统调用MYOS_SYSCALL_DEFINE3(getdents64)内调用iterate_dir的语句是注释，活动语句直接执行f.file->f_op->iterate_shared(f.file,&buf.ctx)，没有上述方法存在性门。因此即使FAT32_file_ops表静态绑定了方法，也不能认定任意有效file的f_op/method都有效；f_op=NULL或method=NULL时须停在首次成员访问/间接调用处，不补iterate_dir的-ENOTDIR。证据A03–A10/A21、B10；DE59-F02。

系统调用首先通过myos_fdget_pos取得fd_s。该函数的int fd直接索引current->files->fd_array[fd]，未见索引范围门，也没有fget式f_count增加；它返回file指针及flags=0。只有当前files和索引合法且数组槽值恰为NULL的情况下，后续if(!f.file)才能安全返回-EBADF。非法物理索引或超出int可表示域的unsigned fd不能在更早失效后偷换为-EBADF；这与41号__simple_fget先比较fd_count再做atomic_long_inc是不同函数，R41-S01/S02不能被覆盖。正常call后fdput_pos只是调用fdput，解锁语句为注释，fdput仅当FDPUT_FPUT位设置才fput；当前myos_fdget_pos的flags=0，所以局部正常路径没有fput动作。“函数到达fdput_pos”并不等于真实引用减少或文件寿命已认证。证据A08/A13–A20；DE59-F01。

getdents_cbk64_s把dir_ctxt_s ctx、current_dir、prev_reclen、count、error放在同一callback对象中，count/prev_reclen/error为int。系统调用的unsigned count赋给该int字段，超出有符号int范围后的值为实现定义，不可将32位二补数模型当成所有C实现。初始化列表只显式设置actor=filldir64、count、current_dir；在正常聚合初始化规则下其余字段初值0。接着先ctx.pos=f.file->f_pos，再调用FAT方法，callee正常返回后先将ctx.pos回写f_pos；若error>=0，error被buf.error替代；最后若prev_reclen非0，则将error改为count-buf.count，无论原先是否为负。此处记账字节数不等于用户缓冲真实全部复制成功；对callee异常/无效指针、超大count或算术溢出不推结果。证据A01/A02/A07/A09–A13、C22/C26。

## B / G02：FAT目录迭代及两次getdents64

FAT32_file_ops的iterate_shared静态指向FAT32_getdents64，但目标file确实绑定该表仍是前提。FAT32_getdents64只有ctx.pos==-1才直接返回0；否则为当前inode新建iobuf并调用iter_init，将iobuf.iter_cursor置0、iter_reach_end置false。**正数ctx.pos没有用于定位iobuf迭代起点**。实际游标由FAT32_iobuf_iter_next的readent和sizeof(msdos_dirent_s)步进驱动，不是ctx.pos每条递增。早先iobuf_init的ERR_PTR/NULL失配、簇数组容量/几何及真实iobuf对象有效性保留R35/R55/R56限制，不能在输入异常时补“从头成功遍历”。证据B01–B03/B10–B13。

每轮循环先调用FAT32_get_full_ent，其内部可能先读错误指针的attr，或在最后一项后以end标志覆盖成ERR_PTR(-ENOENT)；这些是IT35-F01/F02及R35-S01/S02，不能把外层IS_ERR看作深层错误已经安全传播。成功取得普通tmpde后，还先调用辅助FAT32_iobuf_readent(iter_cursor-2个目录项宽度)，然后才初始化name=NULL、namelen=0并检查empty标记，进长短名选择。辅助readent为ERR_PTR可被短路条件排除长名分支，但不能替代tmpde、short parser和name对象有效性。name长度、字符过滤与可能零长度按已闭合36文档修正和R36C-S01消费，不从其它Linux实现补校验。证据B03–B07/B14–B20。

有效name交给actor时，**FAT传入的offset、ino、d_type实参均固定0**，不随当前簇号、目录项或游标改变。actor返回后先请求kfree(name)，若error!=0才break。退出循环时只有error==0才赋ctx.pos=-1，其后请求FAT32_iobuf_release并返回error；错误时ctx.pos不由本函数推进。若第一次容纳一条、第二条由于缓冲不足返回- EINVAL，FAT保持原ctx.pos，而外层getdents64因已有prev_reclen而返回首条字节数；下次仍可从同一FAT游标0开始，产生重复候选。实际磁盘记录、对象与长度必须满足多次actor调用且无更早故障，才可使用此模型。证据B08/B09/B21/B22、A10–A13；DE59-F03/F04。

**连续两次调用的有限例**：假设两条合法普通短名各namelen1，C的显式x86-64字段布局模型每条reclen=24，getter确实安全送达两条及后续正常终止，用户缓冲直接store/copy全有效：

| 初始f_pos / count | 首次报告、f_pos | 第二次同条件 |
|---|---|---|
| 0 / 48 | 报48字节，f_pos=-1 | FAT入口早返0 |
| 0 / 24 | 第一条成功，第二条- EINVAL；报24，f_pos仍0 | 可再报同一首条24，重复候选 |
| 0 / 23 | 第一条拒绝，报- EINVAL，f_pos仍0 | 若不变仍- EINVAL |
| 64 / 24 | 忽略正数64对iobuf初始化的影响，报24、f_pos仍64 | 可重复同一首条 |
| -1 / 48 | 入口直接报0，f_pos保持-1 | 继续0 |
| 0 / 足够大、无actor | 安全EOF后0，f_pos=-1 | 继续0 |

所谓两条可读是**独立条件**，不能跨IT35末项覆盖或实际读盘错误强行推出两次actor。若第一条甚至不能安全解析，表中后续结果不成立；故此处是源码条件模型而非实际readdir用户程序测试。

## C / G03：记录容量、字段写入、copy残量和partial返回

linux_dirent64_s的实际字段依次为u64 d_ino、s64 d_off、unsigned short d_reclen、unsigned char d_type和柔性数组d_name[]。在普通x86-64 ABI、字段大小及对齐为显式模型且**未测编译后布局**时，d_name偏移为19。filldir64在验证name之前就计算int reclen=ALIGN(offsetof(linux_dirent64_s,d_name)+namelen+1,sizeof(u64))；ALIGN定义还转调__ALIGN_KERNEL，宏实际展开未运行。对有限非负普通长度采用按8字节向上对齐模型：namelen1/4→24、5/12→32、13→40、255→280、4095→4120。verify_dirent_name之后才拒绝len<=0或>=PATH_MAX(4096)和名字包含斜线；名字内嵌NUL的检查在注释中而非活动体。极端负/大namelen可能先影响reclen类型/运算，不能因后来有verify而自动签安全- EIO。证据C01–C06/C12–C18/C23/C24；DE59-F07。

名字与容量门通过后，filldir64读取prev_reclen/current_dir，计算prev=(void*)current_dir-prev_reclen，**先直接写prev->d_off=offset**，再直接写当前dirent的d_ino、d_reclen、d_type；仅名字内容使用copy_to_user，返回的未复制数被赋给cplen但未作条件检查；随后又直接写终止NUL并更新buf.prev_reclen/current_dir/count、返回0。第一条的prev_reclen初始0，所以prev=current，d_off当次被赋offset（FAT为0）；后继记录的prev指向上一条，最后一个记录的d_off没有被该次填充明确写入，getdents64尾部更新lastdirent.d_off的代码又被注释。不能声称尾记录一定为零或具体随机值，只能指出本函数缺少赋值来源。见C07–C10/C25、A12/A13；DE59-F04/F06。

这些用户存储中只有name内容走copy_to_user包装，to方向活动内联先access_ok再raw_copy_to_user并返回残余；prev.d_off、d_ino、d_reclen、d_type和尾NUL都由filldir64直接赋值，没有统一的用户写入保护。cplen即使非0，源码也没有相应EFAULT门或撤销已完成的局部记账；efault标签位于正常return0之后，没有活动goto跳转到它。若用户指针早在第一项prev->d_off等直接store失效，就必须停止，不能继续推出“后来copy返回残余但整体返回成功”。只能在前置直接store均合法、copy能正常返回部分残余的条件模型下陈述忽略残余与count记账。证据C07–C11/C19–C21；DE59-F05。

**返回矩阵**（前提均为有效fd/FAT入口、整型有界、有效对象与相应安全退出）：

| 输入/回调前缀 | filldir64/FAT局部 | 系统调用报告 |
|---|---|---|
| count=24，一条namelen1 | 一条被接受；FAT安全结束返回0，buf.error仍保持- EINVAL | prev_reclen非0，按字节覆盖为24 |
| count=23，一条namelen1 | 第一条reclen24被容量拒绝- EINVAL，FAT正常回传负值 | prev0，报告- EINVAL |
| count=48，两条namelen1 | 两条各24成功，FAT安全终止 | 报48，f_pos=-1 |
| count=24，两条namelen1 | 第一条成功，第二条拒绝；FAT error负、ctx.pos未推进 | 报24，但f_pos不推进 |
| 第一条namelen0，计算合法 | verify拒绝- EIO，未完成记录 | prev0，报- EIO |
| 第一条成功、第二条名字拒绝 | 第一条已记账，第二条负错误 | prev非0，最终报告先前字节数 |
| 空目录/安全终止，无actor | buf.error/prev初始0；FAT ctx.pos=-1 | 报0 |
| copy_to_user正常返回非零余数而直存合法 | 本体未检查cplen，仍推进count | 可能报已记账字节，不能证明用户实际收到全部数据 |

buf.error不是“actor本轮成功即0”：filldir64在名字通过后将其设- EINVAL作失败默认值，正常record返回0没有把buf.error复位0。系统调用先可能把callee非负error替换为buf.error，又被prev_reclen非0的字节数覆盖。记录请求、成功记账、局部错误和用户缓冲真实完成必须分别保留；超大unsigned count→int的转换为实现定义，只提供有前提signed32例。图与facts区分控制/Data/返回以及错误后的分析停止。

## 消费边界与交C05

D00只申请R58-01纠正闭合，58当前仍RETURN；A/B/C均completed_in_scope仅指本合同内源码取证。DE59-F01–F07为7条可反驳的条件候选，不是7个实测漏洞；DE59-U01–U07仍需证据，旧FG41、IT35、PN29、EI28、NG58的原字段/编号及R41/R35/R36C/R38限定不变。目录枚举的真实盘面、fd引用/打开文件寿命、用户程序ls/readdir行为、SMP、系统调用表、编译后layout或POSIX符合性均未认证。

G04交付六件在本批唯一目录中。证据为72条新增定义内源码短锚（D00 3、业务69），14源文件ledger、46个临时数学/状态模型、独立图；完整本地P9、原58剩余85引文重验、预处理/编译、仓库C/ASM或旧脚本、Graphviz、QEMU、硬件、真实目录与存储设备全部NOT_RUN。执行者只签READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING；C05独立验收后才可裁定R58-01与59。不自行开始60。
