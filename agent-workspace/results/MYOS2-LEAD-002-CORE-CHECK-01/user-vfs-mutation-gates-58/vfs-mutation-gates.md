---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-VFS-MUTATION-GATES-58
record_type: bounded_vfs_mutation_analysis_with_prior_documentation_correction
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
conversation_display_name: MYOS2-A-C05a 代码分析执行
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-08
source_pin_short12: a039d9803ade
base_snapshot: time
execution_base_short12: 776297f1904b
read_channel: GitHub_connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
prior57_correction_status: CORRECTION_SUBMITTED_PENDING_MAINLINE
subjects: {D00: correction_submitted_pending_mainline, A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
phase: 2
runtime_evidence: NOT_RUN
---

# 第58批：VFS路径、创建/删除门与对象清理（包含57追加纠正）

限定静态取证，非运行实测。每个局部结论均依赖正确的回调绑定、有效对象、整数范围、调用返回和先行门；遇ERR_PTR/NULL、越界、未定义访问或尚未认证的callee，不跨越最早停止点推演。原文短锚见evidence.md；结构化模型见facts-and-boundaries.yaml。图不是完整CFG。

## D00：R57-01/R57-02文档纠正，申请主线独立闭合

**状态只能是CORRECTION_SUBMITTED_PENDING_MAINLINE**。57六件、正式评论6717903727及历史发现均未编辑；此段是58新文件中的替代消费说明，而不是执行者签发57 PASS。

### R57-01：A32定义边界、符号归属与源身份

旧57/evidence.md::A32以定义前的“/* Up to 13 characters of the name */”注释开头。原文虽然连续存在，却不符合P2定义体内引文。原A32应记superseded_for_definition_scope；新证据D01从msdos_dirslot_s真实typedef起，D02/D03分别给出内部字符、checksum、start及packed结尾。单个片段不证明完整布局/编译后sizeof；应合读packed短/长槽与所选类型宽度。

旧57/facts.source_ledger的namei_vfat.c::vfat_dir_inode_operations_legacy_B并非真实符号；它只是需要明示的分析别名。真实操作表定义位于myos_fat32_namei.c::vfat_dir_inode_operations（D05）；namei_vfat.c::setup引用该对象（D04），不是在此定义。旧iobuf blob短值f577b7f657仅10位，真实前12位f577b7f65786。旧evidence的GPT-6署名属于Owner报告/模型自述，不认证后端；旧MANIFEST中的backend_identity=unknown_or_not_attestable不变。

计数隔离：第57批原77标签是历史作者申报；A32定义归属失格，其余76标签历史保留但本批未全量重核。新增D00定义内纠正8锚；第58批A/B/C另有79新锚。不能拿“原77/77字面匹配”充当“原77全部P2定义边界通过”，也不把切片数当独立技术发现。

### R57-02：真实旧字段、长度/类型边界和消费误映射

| 旧项 | 原始来源字段和值 | 限定单独列明 |
|---|---|---|
| IT35-F01/F02 | status=needs_evidence | R35-S01/S02 |
| BI55-F03/F04/F05 | disposition=source_only_conditional | R55-S01/S02 |
| AG56-F02 | status=source_conditional_only | R56-S01/S02 |
| SB38-F01/F02/F04 | status=needs_evidence | R38-S01/S02 |

原57的canonical_status自造聚合描述不得替代source_field/source_value。SB38-F01仅涉及alloc_super产生ERR_PTR而调用者只测NULL，不能表示全部容器/私有字段问题。对于57::DM57-F04，若要引用对象前提，分别依赖SB38-F02（独立inode不证明MSDOS_I外层容器）和SB38-F04（private_sb_info与s_fs_info独立）。F01只有真的涉及该超级块ERR_PTR门时才是其原有问题ID。旧ID、原status不更改，参见facts.prior57_disposition.R57_02。

**length33反例。** FAT32_mark_entry_romoved在长度为33、每槽32字节、非负off及正常算术模型下，循环发出相对起点0与32两次单字节写，两个起点都在[0,33)内。不能说ceil(33/32)的第二次起点“超出请求长度”；该旧句撤回。可继续讨论不完整槽、位置元数据来源或缓冲容量风险，但这些不由“起点超过33”证明（D07）。

**负off反例。** 在合法iobuf前提下，FAT32_iobuf_write先读取iobuf->nr_clus与bufsize组成bufend，再通过off<0拒绝-ENOENT，尚未读buffers[bufidx]。原57“before deref”必须收窄到“iobuf字段读取之后、数组元素访问之前”；原facts negative-off里的first_invalid=0应为null/not_applicable——API拒绝不等于第0字节非法写。若iobuf本身失效，最早是字段读取，不补安全返回（D06）。

**N及终止门。** nr_slots=N表示N个目录槽，写入N×sizeof(msdos_dirent_s)字节；不是N字节。N=1仅fat_add_entries helper-only，按所读vfat_add_entry正常正长度名字/短helper返回0，至少产生2个槽。非零base的后缀E序列必须先合法走过前缀，不能把iter_init想成在L-32起跑。只有bufend严格超过所示next后的游标，才可忽略iter_reach_end导致的expand与第二次next；否则先合读R35/R56最早停止点。条件串ABCDEFGHI.的原qstr长度10，尾点裁剪9但strrchr仍得到ext_idx10，使长分支extlen=-1；早期memcpy有效性问题优先于任何槽越界，不把此模型直接签成用户可达（D08及R57-S01）。

### 七个冻结入口逐项消费

| 旧入口 | 本批追加替代口径 |
|---|---|
| 57/MANIFEST.md | 77是历史自检声明，不能称全部定义边界合格；模型后端身份仍unknown |
| 57/fat-directory-mutation.md | C2“后一次起点越过length”撤回；其余条件链按R57-S01/S02阅读 |
| 57/facts-and-boundaries.yaml | canonical_status不用，改source_field/source_value+consumption_notes；修正虚构定义、短blob、负off分类 |
| 57/deferred-findings.yaml | DM57-F04源边界改SB38-F02/F04；DM57-F08不借错误区间推断升级 |
| 57/evidence.md | 仅A32替换为D01–D03；另76条不冒称本轮逐字核定 |
| 57/fat-directory-mutation-paths.dot | iter→expand归属fat_add_entries内部；pubgap不是CPU止步；S_DEAD仅成功vfs_rmdir |
| PR17评论6717903727 | 原交付事实保留，必须合读主线57 RETURN与此D00，READY不等于主线验收 |

映射同时完整记入facts.prior57_disposition和deferred.prior_return_items。旧原件及原交付评论无净改。执行者只申请C05核收两项文档纠正，不签CLOSED。

## A / G01：父路径、最后分量及三种名字对象

### A1. 真实入口和状态移交

getname先调用strlen_user，长度超过PAGE_SIZE时返回ERR_PTR(-ENAMETOOLONG)；__myos_getname分配结果在getname中未检查IS_ERR便被当作filename_s取字段。getname_kernel同样先取得对象再处理长度。filename_s.name是指针、len是int；getname中将strncpy_from_user的结果放入size_t len然后测试len<0，在所选无符号模型下负值编码不能这样被识别；深层usercopy返回与实际机器实现未审，不能声称某个用户输入已失败（A01–A04）。对入口本身返回ERR_PTR的情形，filename_parentat有IS_ERR门，但之后上层仍可能putname(ERR_PTR)，这是另一消费层，见B。

set_nameidata初次清零nd并保存filename及dfd。path_parentat由path_init以filename.name选root/pwd，再经link_path_walk逐分量计算int i，直到'/'或NUL；LAST_NORM初值依名字恰为.、..分别改LAST_DOT、LAST_DOTDOT。nd.last.len=i、nd.last.name指向原filename中的切片起始，不承诺在长度i处是NUL；例如foo/的last.name[last.len]仍是'/'。最后跳过斜线使末分量被保留为last，父path正常完成后拷出到parent并将nd.path字段清空。filename_parentat最后输出last/type；restore_nameidata正文无实际恢复动作（A05–A16/A28）。

-ECHILD时filename_parentat不重新调用set_nameidata，而用同一个nd再调用path_parentat；但path_parentat内path_init会重新设置根/path/flags、link_path_walk重置last_type并在路径循环更新last。故不得说“任何状态都不重建”或“自动完整回滚并重试成功”；真正-ECHILD生产者、前次副作用及callee语义仍需证据（NG58-U07）。

### A2. dentry持有的拷贝及lookup门

__lookup_hash先调用__d_lookup，得到缓存dentry则立即返回，后续IS_DEADDIR、分配和i_op->lookup不再发生。__d_lookup只按查询qstr.len执行strncmp，未独立对候选d_name.len做等长比较；这是既有DL33-F01加R33-S01/S02，不新增同义根因。缓存未命中，__lookup_hash先核死目录，再经__myos_d_alloc分配并将新dentry挂父目录，然后传flags给->lookup；若__myos_d_alloc返回ERR_PTR(-ENOMEM)，此调用者只查!dentry而随后直接写dentry->d_parent，最早先失去合法性，不能编成后来filename_create安全收到ERR_PTR（A17–A21，NG58-F07）。

__myos_d_alloc对名字另行申请len+1、复制len字节，设置d_name.len并将d_inode置NULL。kzalloc的所读定义将__GFP_ZERO传给kmalloc；在底层兑现清零、len+1计算/分配有效时，新dentry名的索引len处可得到NUL。这个有条件生产不等于所有qstr都NUL，也不消除已证“先按qstr长度裁尾点、后对未裁尾原串strrchr”的差异。对于ABCDEFGHI.，假设创建回调已取得有效拷贝和父目录并正常到达FAT短别名函数，裁剪len9而原串点后索引10，于是extlen=-1；memcpy的长度及源有效性可能先失效，不能继续推出已写22个目录槽。真实创建是否抵达此层仍受旧CB34/R38与当前VFS对象门约束（A22–A27、D08）。

### A3. 条件输入矩阵

| 条件名字 | 最后一项和裁剪 | 当前层可以得出的结论 |
|---|---|---|
| foo | LAST_NORM、len3 | 正常切片，dentry拷贝另需分配/源有效性 |
| . / .. | LAST_DOT / LAST_DOTDOT | filename_create在FAT前从非NORM门退出 |
| ... | LAST_NORM、尾点全部裁为0 | 只有真正进入vfat_add_entry时才会-ENOENT |
| foo. | LAST_NORM、6→5 | 短别名len≤8不走负extlen分支 |
| ABCDEFGH. | LAST_NORM、9→8 | 与下一行长短分支不同 |
| ABCDEFGHI. | LAST_NORM、10→9 | 长分支出现extlen=-1候选；应先停在memcpy前提 |
| foo/ | LAST_NORM、last[len]=='/' | 尾斜线是否拒绝取决于is_dir保存的值 |
| / 或空串 | LAST_ROOT | 不进入普通最终分量的__lookup_hash |

255/261属于57已审FAT helper容量模型；58仅补充所读VFS对象/长度可达性的有限前缀，不宣称合法用户创建成功或全局FAT规范遵循。

## B / G02：creat/mkdir进入门、返回和退出清理

### B1. LOOKUP_DIRECTORY、LOOKUP_EXCL及缓存存在性

filename_create先保存is_dir=(lookup_flags & LOOKUP_DIRECTORY)，再把flags裁为LOOKUP_REVAL，调用filename_parentat；LAST非NORM时去out，原dentry初值ERR_PTR(-EEXIST)就成为结果。这是局部错误码初值，不代表实际发现同名文件。LAST_NORM下才OR LOOKUP_CREATE|LOOKUP_EXCL并进入__lookup_hash。符号位模型：输入0x2→is_dir=true、parent flags0、hash flags0x600；输入0→is_dir=false、hash flags0x600；0x22→is_dir=true、hash flags0x620。这是源码位运算，不是编译运行（B01–B06、B23/B24）。

**do_creatat与do_mkdirat均初始化lookup_flags=LOOKUP_DIRECTORY。** 因此creat路径也令is_dir为真，filename_create内的尾斜线拒绝条件!is_dir && last.name[last.len]!='\0'在该直接路径不能成立。独立调用filename_create(flags0)的helper模型可进入该门，但不能倒灌成creat用户路径已拒绝；更早parent/lookup失败依然可阻断（NG58-F04）。同理，LOOKUP_EXCL只是送入__lookup_hash的flag；缓存命中会直接返回positive或negative dentry，filename_create未加d_inode为正就-EEXIST的本地门。若__d_lookup/回调对象错误，更早终止。这里新增的是创建消费者NG58-F02，旧DL33-F01名字比较根因照旧。

### B2. method门、mode及三类对象

do_creatat/do_mkdirat从成功的filename_create取得dentry与parent path，从path.dentry取父inode，检查i_op->create/mkdir，按S_IRWXUGO|S_ISVTX掩码处理mode后直接调用对应method；POSIXACL/umask/security行均为注释。vfs_mkdir作为独立函数有相同模式掩码，但**do_mkdirat直接调用dir->i_op->mkdir，不通过vfs_mkdir**；所读init.c::init_mkdir才调用vfs_mkdir作内核对照。creat/mkdir的MYOS_SYSCALL_DEFINE调用仅视为已读文本入口，未预处理宏、未审fd安装，也不等同open(O_CREAT)。do_creatat的源码返回的是error，不自动称为“返回一个文件描述符”（B08–B17、B25）。

| 条件出口 | parent path / target dentry | filename对象 |
|---|---|---|
| filename_create报错，name有效 | 由filename_create实际失败路径处理父path；do_*不调用done_path_create | do_*转公共putname，再返回error |
| method存在且返回0/负值 | do_*调用done_path_create→dput(dentry)、path_put(path) | 再putname |
| method不存在且其它对象有效 | do_* **直接return -EPERM**；跳过done_path_create | **也跳过putname** |
| getname/getname_kernel已返回ERR_PTR | filename_parentat能转换错误，但外层仍可能调用putname(ERR_PTR) | putname访问name->name是更早非法消费点 |

缺method的直接return分支是NG58-F03，它不能凭“done写在函数后半部”自动补出释放。错误名字指针的层间失配是NG58-F01；getname本体在__myos_getname失败时可能在返回前就先错误解引用。putname活动体发出两次kfree请求，但不能把其余dput/path_put函数名当物理释放的实证。filename持有内存、parent path、目标dentry及其独立name buffer须分开记账（B07/B09–B22）。

## C / G03：rmdir/unlink类型门、返回与引用

### C1. LAST类型及负dentry首次访问

do_rmdir在filename_parentat成功后对LAST_DOTDOT返回-ENOTEMPTY、LAST_DOT返回-EINVAL、LAST_ROOT返回-EBUSY；do_unlinkat对一切非LAST_NORM统一设置-EISDIR。二者对IS_ERR(__lookup_hash)都可走PTR_ERR出口，但此后有关键不同：**rmdir显式if(!dentry->d_inode)→-ENOENT**，并随后检查S_ISDIR，否则-ENOTDIR；**unlink直接将dentry->d_inode放入inode并访问S_ISDIR(inode->i_mode)**，没有与rmdir对称的NULL门。有效negative dentry条件下，unlink首次失去有效性的是NULL inode的i_mode访问，不得补出下游iput或确定errno；此前注释掉的末尾斜线检查不是活动检查（C01–C08、C12–C18；NG58-F05）。

### C2. VFS回调后的标志不是盘面

vfs_rmdir先测试rmdir方法存在；若存在则dget，发起回调，只有它返回0且dentry->d_inode仍有效时才设置i_flags|=S_DEAD；非零结果直接跳过该赋值再dput。vfs_unlink只按有无方法返回-EPERM或调用该方法并返回error，未见相同S_DEAD赋值。S_DEAD是内存标志，不是FAT表项/盘面完成的证明；57中先释放簇再标父项仍是受R57-S01/S02约束的条件链（C09–C11、C19–C21）。

### C3. 实际dget/dput/iput，不按注释想象引用计数

rmdir外层有dput(dentry)→path_put→putname；其vfs_rmdir内部另有dget/dput。unlink正常对象路径在调用vfs_unlink后dput，若inode非NULL则iput，之后path_put与putname。已读活动体却显示：dget仅返回原dentry，引用增加行是注释；dput函数体仅有注释；iput只在NULL时早返回，剩余引用减少/最终释放逻辑为注释；path_put只调当前无动作的dput，mntput被注释。另一方面putname确实请求kfree(name->name)与kfree(name)。因此不能仅凭看到dget/dput/iput就断言真实引用计数正确增减、父mount已释放或正在打开的文件已按POSIX时序回收；更不能无证推出双重free、内存泄漏具体数量。所读定义给的是**局部无操作或部分动作**，完整生命周期、并发和所有权须留NG58-U06（B21/B22，C16/C17/C22–C26；NG58-F06）。

| 输入/返回模型 | 可证明的上层出口 | 必须停止的夸大 |
|---|---|---|
| rmdir negative dentry合法 | -ENOENT，共同清理请求 | 无真实盘删除 |
| unlink negative dentry合法 | S_ISDIR处最早非法对象使用 | 不补安全errno或iput |
| rmdir回调缺席 | vfs_rmdir直接-EPERM，不执行本层dget | 外层path_put仍只是无操作dput |
| rmdir回调成功 | 条件下S_DEAD和0返回 | 不是介质完成或POSIX保证 |
| unlink回调缺席/返回负值 | vfs_unlink返回-EPERM/错误，外层仍可到条件iput | iput当前无实际释放 |
| unlink回调返回0 | 外层源码error=0，调用相同清理helper | 不是所有文件描述符、引用与磁盘状态已完成 |

## 本批边界与C05验收入口

D00申请独立闭合R57-01/R57-02，但当前57仍为RETURN_DOCUMENTATION_ONLY。58 A/B/C各completed_in_scope，仅表示有界静态证据闭合；NG58-F01至F07是有前提的7条候选，NG58-U01至U07是7个缺口，既有DL33/CB34/DM57/SB38等不重号、不重验。facts包含source_ledger、prior57_disposition、name_provenance_matrix、create_gate_cleanup_matrix、delete_gate_reference_matrix、finite_model_cases及prior_findings_links，evidence分D00与A/B/C列87个源锚，DOT标CALL/RETURN/DATA/LOCAL_CONTROL/ANALYSIS_STOP。

未运行仓库C/ASM/脚本、旧夹具、预处理/编译/链接、QEMU/硬件、真实文件与设备、Graphviz、完整本地P9，也未重验57原77条字节扫描。执行者仅READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，交C05独立审定57文档纠正与58，**不自签57闭合或58通过，不启动59**。
