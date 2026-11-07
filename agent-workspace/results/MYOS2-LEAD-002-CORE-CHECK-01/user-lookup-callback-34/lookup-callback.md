---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LOOKUP-CALLBACK-34
record_type: bounded_FAT_lookup_callback_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-04
date: 2026-11-03
base_snapshot: time
source_pin_short12: a039d9803ade
result_base_short12: a6d39cdb22f0
fixed_contract58_short12: a87124c1cf6c
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第34批：FAT32 lookup回调如何返回、如何写入原dentry的inode

## B00｜任务固定、上游限定与执行范围

Owner明确指派58号 `CORE-USER-LOOKUP-CALLBACK-34`；58号和固定提交 `a87124c1cf6c` 中的33主线回执已完整读取，强制合读R33-S01/S02。新执行仍由C04a/CHAT-EXEC-04承担，主线独立验收只归C04。固定time源码 `a039d9803ade`；原PR17执行分支首次写前预期 `a6d39cdb22f0`。不重新接收33、不轮换聊天、不重做其48条证据或快查。第33批原DOT真实计数依R33-S02为**30个显式节点、36条边**，不是原作者自报的32个节点。

适用已读取的MyOS2工作令、conventions、P1–P13、18/24/28及适用Mnemosyne通用行为指导。按58号要求另补读取固定57/01启动指令与45号历史执行轮换文件，二者只借用角色/单写者与证据纪律，不恢复25/33旧任务。R31-S01/S02、R32-S01/S02、30 DOC-FIX-01、UV-04、OF30原限定不变。文中「代码检查门」只指真实活动条件；未由代码验证的有效指针、字段、长度、返回、所有权与生命周期均为**分析前提**。一旦前提失败便停止继续推演，不声称确定CPU终态。

GitHub现场已核：仓库public且有push权限，time/master/lead固定头匹配；PR16/PR17均open、PR17仍Draft；全部开放PR仅16与17。PR16当前144个变更路径不与本批新写区相交，目标`user-lookup-callback-34/`目录初始不存在。两条time配置/ panic金丝雀与master不同；正式提交前还需重复PR17 HEAD租约核对，未知并行改动必须停写。

## B01｜回调操作表的**静态绑定链**与对象身份

`mykernel/fs/vfs/namei.c::__lookup_slow` 在构造原dentry后以 `dir->d_inode->i_op->lookup(dir->d_inode,dentry,flags)` 调用间接函数，真实`inode_ops_s.lookup`类型是`dentry_s* (*)(inode_s*,dentry_s*,unsigned int)`。上层不保存这个回调返回值，只与NULL比较（CB34-A001/A002/A005）。`mykernel/fs/fat/myos_fat32_namei.c::vfat_dir_inode_operations`的**活动初始化**明确有`.lookup=FAT32_lookup`（CB34-A008）。`fat_fill_inode`在目录和非目录分支都将`inode->i_op`设置为该操作表；`namei_vfat.c::setup`把`MSDOS_SB(sb)->dir_ops`设为它，`fat_read_root`从`sbi->dir_ops`写根inode的`i_op`（CB34-A009–A012/A016/A017）。

这仅证明多个真实赋值和间接调用关系，尚须实际父inode的`i_op`指向这张表才会进入本次所选FAT32_lookup；不把操作表名称、extern声明或根目录的其它历史初始化路径冒充已观测运行绑定。该静态链以适用条件消费。

本批对象分层：`parent_inode`是上层`dir->d_inode`借入，`parent_inode->i_sb`是其superblock；从`i_sb->private_sb_info`借入的`FAT32_SBinfo_s *fsbi`是FAT32回调私有字段，**不同于**`MSDOS_SB(sb)`使用的`s_fs_info`字段，不能无证据视作同一地址。原`dest_dentry`是`__lookup_slow`此前分配并交给回调的对象，`dest_dentry.d_name`是其名字成员；`p`是可能另行分配的新inode，`finode`再是另一份私有inode信息缓冲。`tmpde`为`FAT32_get_full_ent`返回的临时目录项指针，`tmplde`为辅助长目录项指针，`iobuf`是持有`clusters`、`bufsize`、`iter_cursor`及回调`iter_init`的独立对象；`name`是按每轮解析产生并在比较后kfree的临时字符串，`destname`则指向原dentry的名字。实际对象字段定义见CB34-A006/A007/A018–A024/A047–A056，所有权/真实释放的全部下层保持opaque。

## B02｜FAT32_lookup的活动目录扫描和**不同退出原因**

函数读取 `int destlen=dest_dentry->d_name.len`、`const char* destname=dest_dentry->d_name.name`，将`tmpde`初始化为NULL，获取`iobuf=FAT32_iobuf_init(parent_inode)`后直接调用`iobuf->iter_init(iobuf)`。函数没有先检查iobuf是否NULL/ERR_PTR，也未检查iter_init所返回的int；要求指针、函数指针和初始化正常是本批有效分析的前提（CB34-A018–A021）。`flags`虽作为形参出现，在完整所选FAT32_lookup活动定义中没有被读取，不将此局部结论推广至pathwalk或FAT权限。

循环源码是 `while ((de_start=iobuf->iter_cursor)<FAT_MAX_DIR_SIZE && !IS_ERR(tmpde=FAT32_get_full_ent(iobuf)))`（CB34-A022）。两个操作数从左到右短路：若`de_start`已达上界，右侧调用**根本不发生**，`tmpde`仍保留之前赋的值。`FAT_MAX_DIR_SIZE`来自`FAT_MAX_DIR_ENTRIES(65536) << MSDOS_DIR_BITS(5)`，纸面整数为**2097152字节**；没有证明某一真实iobuf达到该上界或底层函数的运行结果（CB34-A042–A044）。

进入循环体时从iobuf读取`tmplde`、更新`off`、将临时`name=NULL,namelen=0`重置。若`FAT32_ent_empty(tmpde)`为真，直接continue；否则在`!IS_ERR(tmplde)`、`ATTR_LONG_NAME`与非空条件成立时调用长名解析，余下走短名解析（CB34-A023–A026）。这些辅助读目录/解析函数只确认其实际接口类型与传参，不展开FAT簇链、盘I/O和大小写/编码。即使IS_ERR(tmplde)为假，也不能当作它非NULL或可读；类似地若`tmpde=NULL`在所选指针编码下`IS_ERR(NULL)`为假，依然缺少可交给空项处理函数的有效对象前提，不能推导必然CPU故障。

名字比较用`int len=(namelen>destlen?namelen:destlen)`，调用`strncmp(name,destname,len)`，立即`kfree(name)`，布尔equal为真才break（CB34-A027）。这与33的仅用查询长度比较不同；对**实际非负且准确、名字缓冲可读、按需终止**的有限模型，较大长度有助于区分不同长度名字，但不能自动推断VFAT完整名字语义。源头的`size_t d_name.len`窄化为int、解析器向int namelen写值，以及int转换为strncmp的size_t计数均应保留有效范围前提；未证真实返回负长度、坏指针或未终止串时，不把假想大计数提升为实际内核缺陷。当前所选strncmp仍受`#ifndef __HAVE_ARCH_STRNCMP`编译条件，未做预处理/链接确认（CB34-A062–A064）。

**不能把三种循环退出合并**：
- **匹配break**：本轮取得的非错误`tmpde`仍作为所选匹配条目，在其数据/生命周期有效的条件下可进入inode构造。
- **get_full_ent错误**：右侧正常执行且返回错误编码指针，`tmpde`被写成该错误值，while退出；随后的`if(IS_ERR(tmpde))`把局部`dest_dentry=NULL`并跳共同清理/返回路径。回调**返回NULL**，并不直接返回那个ERR_PTR或保留其errno（CB34-A022/A028/A036）。
- **游标上界退出**：左条件为假时，`FAT32_get_full_ent`未调用，也没有新的`tmpde`赋值。若此前曾处理过一项非匹配、但仍是非错误的`tmpde`，循环后单独的`IS_ERR(tmpde)`为假，就可能直接以这条**尚未证实名字匹配的旧目录项**调用`fat_build_inode`。当前函数没有为此引入独立`matched`状态/上界失败检查。因此这是**带完整有效性前提的错误选取风险**，不是已复现错开文件。若首次while就触界，`tmpde`仍是NULL，`IS_ERR(NULL)`不阻止向fat_build_inode传NULL；若new_inode先失败则可能正常产生错误，若成功而fat_fill_inode消费NULL de则必须在首次无效访问处停止。空项continue后触界亦不能将该空项自动提升为匹配成功（CB34-F01/F03）。

`error=0`、`off`、`de_start`、`tmpde`彼此含义不同；没有任何一个现有量在所有退出情形下自动成为「找到目标」的独立证据。`FAT32_parse_long/short`的返回是否允许NULL/ERR_PTR或无效长度，留待其它任务，不凭类型构造实际坏输入。此次不做真实目录布局或完整分区/盘读实验。

## B03｜inode构造、原对象副作用与回调返回类别

退出后若`IS_ERR(tmpde)`为假，调用`p=fat_build_inode(parent_inode->i_sb,(msdos_dirent_s*)tmpde,0)`，随后`IS_ERR(p)`若为真，则**局部形参指针**`dest_dentry=NULL`并跳`build_inode_fail`（CB34-A029/A030）。`fat_build_inode`执行`new_inode(sb)`，返回NULL时本层构造`ERR_PTR(-ENOMEM)`；正常返回inode才调用`fat_fill_inode`，若后者返回非零则`iput(inode)`并构造`ERR_PTR(err)`，否则统一返回同一inode对象（CB34-A037–A041）。这些是实际控制语句，不能凭它们证明new_inode初始化、iput释放或分配后值的完整生命周期。

`fat_fill_inode`的活动体先`inode->i_mode=0`。如果`(de->attr&ATTR_DIR)&&!FAT32_IS_FREE(de->name)`，目录分支写`i_op=&vfat_dir_inode_operations`、`i_fop=&FAT32_file_ops`；非目录分支写`i_size=de->size`并写同样操作表和`i_mapping->a_ops=&fat_aops`，正常尾部直接返回0（CB34-A009–A015）。目录分支的选定活动代码没有在该处直接设置`i_size`或`i_mapping->a_ops`；这只是局部来源缺口，不能直接声称new_inode没有事先初始化。更深分配器、新inode默认i_mapping和根inode来源仍不在本批验证范围（CB34-F05）。

若p有效，FAT32_lookup接着计算`p->i_blocks=(p->i_size+fsbi->bytes_per_cluster-1)/fsbi->bytes_per_sector`。在`bytes_per_sector==0`、大小/指针不合法或算术缺少有效性时，推断必须止步于该表达式，不能声称已经进入私有字段写入。成功前提下，它`kzalloc(sizeof(FAT32_inode_info_s))`给`p->private_idx_info`，紧接着通过`finode`写入来自`tmpde`/iobuf的first_cluster、dentry_location、dentry_position及日期/时间/dentry_length；**在本函数该次分配和第一处finode字段写入之间未见NULL门**，但没有实际分配失败证据（CB34-A031–A034、CB34-F04）。

只在这些字段和源对象均已有效且正常执行后，源码才写`dest_dentry->d_inode=p`。它修改**传进来的原dentry**，而非把另一个dentry作为回调成功结果。紧接着两个标签`build_inode_fail:`、`ent_not_found:`落在同一`FAT32_iobuf_release(iobuf)`，最后`return dest_dentry`（CB34-A035/A036）。因此在所选函数**定义良好地到达正常返回**时，直接出口只有：
1. 迭代错误或fat_build_inode错误先将局部dest_dentry改成NULL，正常release后返回NULL；原dentry的d_inode赋值语句未到达。
2. 有效inode构造、块数与私有字段写入及`dest_dentry->d_inode=p`正常完成后，release正常返回，回调返回**原dentry的非NULL指针**。

本批完整FAT32_lookup定义没有直接返回另一个dentry或ERR_PTR；未定义指针访问、溢出、失败后不返回等不属于上述两类，不可当成已证CPU终态或推造指针返回值。若iobuf_release不正常返回，不能由此前d_inode已被赋值推导最终return也已执行。相关有类型矩阵见facts的12种条件情形。

### 与33上层的接口结论

`__lookup_slow`将回调返回值只同NULL比较。由本批可定点确认：**FAT32回调正常NULL退出**是上层`kfree(原dentry)`并返回`ERR_PTR(-ENOENT)`的真实局部生产者；例如在有效前提下，`fat_build_inode`因`new_inode`返回NULL而产生`ERR_PTR(-ENOMEM)`，回调本身把它变成NULL后，上层把该失败类别变为`-ENOENT`。同样地，目录helper的编码错误一旦按本批的IS_ERR分支变成NULL也被上层合并成ENOENT。这是**限定源码的错误码压缩**，没有证实真实磁盘错误或分配失败已出现（CB34-A001/A002/A028/A030/A038、CB34-F02）。

**FAT32成功回调返回同一原dentry，而且在成功前缀已执行`original.d_inode=p`**，故33的DL33-F02关于“若返回另一对象或ERR_PTR”的模型在此所选回调没有直接生产者；但原对象和新inode仍须有效，且上层`inode->i_mapping`、`i_size`、页数/页数组及引用语义不因此获认证。33的DL33-F04则获得更具体的FAT回调NULL来源，上层kfree原dentry的代码未显式释放单独分配的名字缓冲，但物理回收与所有权未审，不能宣称已测泄漏。

LC32-F03在33被限定为`__lookup_slow`没有定义良好正常NULL出口：**本次“下层callback返回NULL”并不等于“__lookup_slow正常返回NULL”**，因为上层把它改为`ERR_PTR(-ENOENT)`。旧LC32-F01/F02的标志域和未赋dentry风险也不由本轮解决；旧UV-04/OF30等继续按原编号延期，绝不增加实际已证独立漏洞数量。

## B04｜证据、自检、未做和停止线

本批只交六件：MANIFEST、本文、`facts-and-boundaries.yaml`、`lookup-callback-paths.dot`、`deferred-findings.yaml`、`evidence.md`。本次64段固定time、十份源文件、单定义体1–5行直接引文全部另立账，不抄33的48标签。DOT对真实控制门、静态绑定、分析前提、缺失matched门、返回和停止点分别表示；类型矩阵保留对象身份、十二种条件回调情形和上层消费。五个CB34-F01..F05均只是普通延期候选，不代表五个已证漏洞。

**NOT_RUN**：仓库C/ASM/脚本、预处理、编译、QEMU/硬件、真实FAT目录读取/盘I/O、长短名算法/簇链、完整权限/挂载/RCU/锁/引用/资源回收、new_inode/iput/iobuf内部、额外付费模型/API。无任何内核或用户程序修改、补丁、旧文件覆盖、PR结构变更、分支/PR创建、合并/强推/删分支或阶段3。主线验收PENDING，最终本地机械P9 NOT_RUN，仅作可读/结构/有限模型自查。正式远端双读完成后，执行者在原PR17写本人交付评论并STOP_WRITING；第35批不由本对话自行启动。
