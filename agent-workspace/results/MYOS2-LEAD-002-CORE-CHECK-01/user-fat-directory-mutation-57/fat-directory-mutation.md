---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-FAT-DIRECTORY-MUTATION-57
record_type: bounded_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
conversation_display_name: MYOS2-A-C05a 代码分析执行
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-08
base_snapshot: time
source_pin_short12: a039d9803ade
execution_base_short12: 9b7239b29b0d
read_channel: GitHub_connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
phase: 2
runtime_evidence: NOT_RUN
scope:
  A: completed_in_scope
  B: completed_in_scope
  C: completed_in_scope
---

# 第57批：FAT目录修改静态链——名称/落位、创建/发布、判空/回收

本稿的“完成”只指86号合同内固定time源代码的限定静态分析完成。每个行为结论都以函数选择、对象/指针合法、有关整数未越界、条件分支进入以及callee正常返回为前提；跨越首次未定义访问、错误指针消费、分配失败或算术失效点后，不再臆测下游返回值。FAT/ATA/NVMe请求、缓存标志和内存变更都不是实际盘面验证。请和 evidence.md 的77条直接源码短引文、facts-and-boundaries.yaml 及 deferred-findings.yaml 合读。

## A / G01 名字→别名→长短槽→父目录扫描

### A1. 三种名字长度不能合并

vfat_add_entry 首先经 vfat_striptail_len 以 qstr.len 为范围删尾点；裁剪为0直接返回-ENOENT。这不是strrchr的扫描范围：myos_vfat_create_shortname接收整数len，为内部大写暂存申请len+1字节，接着调用strrchr(name,'.')；所读fallback strrchr直到NUL才停止，不带qstr.len参数。qstr_s仅存size_t len与name指针。实际名字是否在相关内存中NUL终止、有效读取是否跨越qstr边界，必须另证；不能把“名字长度足够”直接等同“strrchr合法且只扫描该名字”。其调用所见实现在条件编译下，非当前构建宏的选择仍未知。证据 A01–A04、A07、A32–A33。

长为1–8的名字经短别名helper的直接memcpy(len)路径；例如条件输入“A.B”会将点作为原字节写进11字节name字段，而非由该分支作8+3拆分。长于8时使用至多6字节前缀、至多3字节扩展，并固定设置末两字节为“~1”。短helper的活动声明ret_val初始化0，所读正常路径没有将它改成1；原代码中的“ret_val=1”为注释。因此 vfat_build_slots 存在的“若短helper返回1则直接使用短槽”的判断，在所选helper正常返回路径上没有返回1的生产者，简单短名字也进入长槽构造。没有逐目录查重/升级“~2”等操作出现在所审短helper及直接生成链中，但没有穷尽独立VFS/dcache门；固定别名冲突只列条件候选DM57-F01，不签已发生。证据 A02–A06、A09。

### A2. 槽布局和容量的两个不同上界

在有效整数、输入及内存前提下，长槽数为 ceil(len/13) 即 (len+12)/13，最终总槽数另加1个短槽。namebuf按长槽数×13字节清零申请，随后只复制len字节；每个长槽从倒序 (i-1)×13 字节偏移取5+6+2个字符并用ascii_to16逐字节写低字节/零高字节，不能描述为已做UTF-16/Unicode规范化。长槽位置由ps递增安排；id为i，首槽再OR 0x40；alias_checksum来自fat_checksum(msdos_name)，而非由文件名逐字节直接校验；尾部de指向短项，写入11字节名字、属性、父目录扩展时间字段、起始簇以及size=0。具体布局来自packed msdos_dirent_s 与 msdos_dirslot_s；条目32字节是字段尺寸的显式类型模型，本轮未以编译后sizeof实测。证据 A08–A15、A29–A31、B11。

固定分配为 MSDOS_SLOTS×sizeof(msdos_dirent_s)，即在条目32字节条件模型下21×32=672字节。条件表：len=1/8/13→1长+1短=2槽；14/26→2+1=3槽；27→3+1=4槽；255/260→20+1=21槽；261→21+1=22槽，所选函数未见在写槽前将len≤260作为前置检查。FAT_LFN_LEN=255与MSDOS_LONGNAME=256为不同宏事实；宏存在不自动证明当前helper输入上限。261一例仅作为helper输入域容量反例，不能签从真实用户路径可达；窄化size_t→unsigned int→int、len+12和分配乘法还可能先失效，须先停止，见DM57-F02/U01。证据 A10、A13、A14、A16、A29、A30。

临时大写name_upper和namebuf的所读函数正常出口没有对应kfree，vfat_add_entry的cleanup只释放slots；这是局部缺乏释放动作的源码事实，不是内核实测泄漏计数。两个kzalloc结果在helper及构槽中没有紧随使用前的NULL检查，内存申请失败可能导致更早停止；任何“写满22槽”的预测都须排除此类先行无效性。证据 A02、A10、A18。

### A3. 搜索状态与首空槽、连续空槽数量

fat_add_entries从iobuf->next取得一条并将iter_cursor前进。若iter_reach_end已真，会请求FAT32_iobuf_expand(iobuf,1)后再次next，但既不检查扩容返回值也不确保下一指针有效；对next得到的错误指针直接调用FAT32_ent_empty。此处关联已审AG56-F02扩容容量/回滚和IT35-F01错误指针，而不是重做它们。后续状态分析仅在条目对象有效、expand不构成先行停止时成立。证据 A19–A23、C22–C23。

用E代表可识别空槽、X代表普通占用槽；为避免把遍历末尾错误当作合法盘序，表格模型假设 bufend 超出所示游标、每槽32字节、终止原因按所给FAT_MAX_DIR_SIZE L=2097152约束。

| 序列及请求N | empty_off / emtpy_count轨迹 | 实际break | 局部结果 |
|---|---|---|---|
| E E E，N=2 | (0,0)→(0,1)→(0,2) | 第3个E | 发起2槽写；正常cleanup返回0 |
| E X E E E，N=2 | (0,0)→(-1,0)→(64,0)→(64,1)→(64,2) | 第5项 | 只选最后连续空段 |
| L-64和L-32两槽都为E，N=2 | (L-64,0)→(L-64,1) | 不满足count==2，上界退出 | 仍请求写2槽；这里恰有2个空槽，但break条件未满足 |
| L-32一槽为E，N=2 | (L-32,0)，cursor=L | 上界退出 | 仍请求写2槽，第二槽从未确认空闲 |
| L-64和L-32均X，N=2 | (-1,0)→(-1,0) | 上界退出 | 局部error=-ENOENT，但最后固定return0 |

核心是首个E仅设empty_off，不令emtpy_count从0变1；第N个连续E对应count=N-1，须第N+1个E才命中break。但末尾另一个分支只按 empty_off 是否存在决定写入固定N槽，未核已收集空槽数量。故“break条件偏一”与“末尾不足仍请求写”是两个同根因的可区分现象，合并记DM57-F03，不按例子数增加缺陷数。若扫描中提前遇ERR_PTR、对象越界、簇扩容异常或内存问题，则从最早非法处停止推演，不签后续写和返回。证据 A20–A26。

### A4. 选择的off不等于可写的整段

选定empty_off后调用FAT32_iobuf_write(iobuf, off, slots, N×sizeof(dirent))。所读iobuf_write仅检查“起始off”属于整个iobuf总范围并计算bufidx、bufoff；整个size范围、单buffer剩余容量和末尾没有另设门。条件模型bufsize=512、bufoff=480、N=2、size=64：前32字节落在当前buffer，后32字节越过该分配；首个不合法的目标下标为512。bufidx不自动换到下一buffer；不能将“跨到下一缓存块”理解为代码已经切换了数组元素。同buffer bufoff=480,size=32在该范围模型内正好到边界；size=0并不执行逐字store/设置DIRTY，但cache miss读/分配仍可先发生。上述越界情况所能断言的是最早失效边界，不能推断被越界覆盖的具体内容、后续release结果或磁盘持久化。既有BI55-F03覆盖iobuf写跨度及忽略返回；本批新增的是目录槽计算N和empty_off如何把该缺口暴露给创建/删除消费者，不重复编一个BI55同义编号。证据 A25、A27、A28。

## B / G02 create与mkdir的真实出版边界

### B1. create只请求父目录项，不直接建立新inode

vfat_dir_inode_operations中确实将create、lookup、mkdir、rmdir、unlink绑定到本专题FAT32_*回调；这是操作表关系，不证明某次启动实际选择或整个superblock合规。已核的do_creatat在filename_create给出有效dentry后调用dir->i_op->create(dir,dentry,mode)并消费返回err；紧邻done_path_create仅dput/path_put，不建立d_inode。FAT32_create把qstr、false、cluster=0和栈上sinfo交给vfat_add_entry；函数体未使用mode，未填dentry->d_inode。vfat_add_entry也没有给传入的sinfo赋值，其调用的fat_add_entries接口没有sinfo参数。因fat_add_entries正常退出固定返回0，即使它先设置局部错误或忽略iobuf_write错误，FAT32_create也可能返回0。结论限“已读直接回调及紧邻消费者没有inode/dentry发布”；不是全dcache审计或真实open(O_CREAT)崩溃证明，也不能越过R38对象来源缺口。证据 B01、B02、B14、B15、B17、B18、A17、A26；DM57-F04。

### B2. mkdir不是原子事务

FAT32_mkdir先调用返回类型u32的FAT32_alloc_new_dir并放进int cluster。后者从父inode获取私有fsbi和FAT32_inode_info，按bytes_per_sector分配buf，在安全容量至少64字节的模型下写入“.”、“..”两个packed目录项：name分别为11字节填充字串，attr为DIR，size=0，“.”指向本次候选cluster，“..”取父finode->first_cluster。接着请求一扇区ROOTBLK WRITE，若该请求返回非零则以u32(-EIO)提前结束；若返回零，再请求FAT32_write_FAT_Entry(...,FAT_ENT_EOF)，不检查setter结果，返回候选cluster。setter自身对预读和写NumFATs份也不核返回，正常固定返回1（BI55-F05）。这里仅有调用与局部错误门；不能把WRITE返回0等同实际扇区持久化，更不能将“初始化1个扇区”扩大为整簇初始化。证据 B05–B13。

u32→int的溢出域须单列。条件输入0、1：FAT32_alloc_new_dir可以经cluster<2直接返回0或1；FAT32_mkdir的cluster<0为假，因此会向vfat_add_entry提交0或1，但不证明真实有效簇。合法候选2及一般int正域值允许继续。把-ENOMEM/-EIO先转换成u32会生成高位编码（在32位u32模型下分别为4294967284/4294967291）；转入int若超出范围，C层结果实现定义。假定x86-64常见32位二补数int映射，它们分别转成-12/-5并被cluster<0阻断；本轮没有预处理/编译确认实际选择，不声称“一定漏检”或“一定负数”。超出INT_MAX的其它u32也属实现定义模型，需避免为其补一个确定分支。证据 B03、B06、B09；DM57-U04。

缓冲buf在上述newdir成功及部分早退路径下没有在当前函数中释放；见不到调用后释放/资源撤销。更重要的是顺序：候选取得→准备点项→一扇区数据WRITE请求→FAT EOF请求→mkdir返回转换门→父目录槽插入→返回。父目录插入失败并没有在FAT32_mkdir已读活动体中恢复前序已请求写入或回收簇，fat_add_entries又会掩盖自身一些错误。不能将这一顺序写成已实现两阶段提交、事务或崩溃一致性，也不能凭源码顺序推定介质已经做了哪一步。FAT32_create/mkdir及紧邻do_mkdirat未直接将新inode挂到目标dentry；sinfo未被这个路径用作补齐对象。仍不展开R38的A/B超级块容器混用及真实初始化选择。证据 B04、B07–B10、B16、B17；DM57-F05/U03。

## C / G03 判空、删除元数据、回收顺序

### C1. rmdir判空与最后一项的条件门

FAT32_dir_empty令result=ENOERR，获取iobuf并初始化迭代器；每次仅在FAT32_get_full_ent返回非错误指针后查看当前项。条目不为空且名字既不是11字节“.”也不是“..”时令result=-ENOTEMPTY并break，之后release；如果getter以ERR_PTR正常返回且前面没有识别到普通项，while直接终止，result保持0。这并非“任何错误都当空”：原getter从next取得ret_val后先读ret_val->attr，错误指针可能在这里已成为非法对象；只有像正常有效末条使iter_reach_end为真而 getter安全地把ret_val覆盖为ERR_PTR(-ENOENT)等路径，才有“错误返回→while假→0”这个具体模型。如果一个普通项恰好在尾部，该覆盖还可使rmdir没有看到它；这属于原IT35-F02末项遗漏的消费者扩展，旧根因原ID保持，不重计第35批。真正普通项在此前被识别则-ENOTEMPTY提前阻断释放。证据 C01–C07、C22；DM57-F06。

### C2. 被删记录位置来自条件性的lookup，而非字段注释

在所读FAT32_lookup成功前缀中，de_start在调用getter之前取当前迭代游标；off在getter之后取新游标；新inode私有FAT32_inode_info的dentry_position=de_start，dentry_length=off-de_start，另存dentry_location取clusters[de_start/bufsize]。因此position是当次iobuf的绝对字节游标源，不能直接当作“已转成物理簇内off”；长度是该扫描跨越的条目跨度，并非总可被证明的一个32字节槽。R35的末项覆盖、跨上界与R38对象容器问题仍是上游有效性门：本批只建立满足条件的helper模型，不宣称真实unlink必拿到完全正确元数据。证据 C15–C17。

FAT32_mark_entry_romoved对length的size_t循环每次步进sizeof(msdos_dirent_s)，在32字节模型下对非负length会请求ceil(length/32)次写入；每次把0xe5写到i+off字节位置，仅写一字节。length=0写0次、31写1次、32写1次、33写2次；若长度不是完整条目倍数，后一次起点可能超出请求长度。负off与size_t的混合加法需要先检查类型提升和有符号转换/越界，不能直接把负值送入安全范围；单字节写跨buffer问题主要落在off定位/初始化，不是把每次size1误称为跨buffer整块复制。iobuf_write返回值被忽略；回收后是否真正落盘另受DIRTY门、驱动返回和释放路径约束。证据 C09、C10、C15–C17、A27；DM57-F08。

### C3. 子簇释放先于父项删除，但成功码不是磁盘完成

FAT32_rmdir先要求dir_empty返回0；然后FAT32_release_clusters(inode)，再FAT32_mark_entry_romoved(parent,position,length)，最终返回先前error。FAT32_unlink不调用dir_empty，直接执行相同两步并返回初值0。两个内部helper都声明为int，但所读活动体末尾没有return；两个调用者并不使用其返回值。不能凭“缺return”把已读调用者一定判成随机errno或每次都发生可观测未定义行为；若另有调用者真的消费它们的返回值则须另外分析。证据 C08–C14。

FAT32_release_clusters通过iobuf_init获取子簇数组，倒序把flags逐个设为DELETE，再交FAT32_iobuf_release。release中每簇先读取bufp和flags；若bufp非NULL且flags==DIRTY才发数据WRITE，随后释放buffer；flags==DELETE时独立发送FAT_ENT_FREE的setter请求。由于DELETE=2和DIRTY=1的相等门，DELETE项本身不经DIRTY数据WRITE；而flags=NEWCLUS(3)也不自动等于其中一种已授权动作。局部clus_end_change每轮重新置false，并且只有flags==DELETE分支把它置true，所读后续“flags!=DELETE && clus_end_change”门在没有隐藏改写的合法同轮里不可成立；继承R55-S01/S02而非误报EOF修补已经发生。释放数据块缓存、请求FAT表释放和实际磁盘回收是三种不同事件；不能用void release当成功码，也不能把内存数组free当磁盘对象已被回收。证据 C18–C21。

最后父项标记是另一套parent iobuf的写入和release，之后rmdir返回先前error、unlink返回0。若底层某次WRITE失败但函数按正常调用返回、被忽略，外层成功码可能与数据实际结果脱节；如果更早发生非法解引用/缓冲越界，后面的“成功码”不得假定可达。紧邻vfs_rmdir在收到0时会给dentry对应inode追加S_DEAD标志；vfs_unlink只消费回调错误码，do_unlinkat在直接调用后还有dput与条件iput，但这不足以签开放文件引用、延迟回收、并发锁或完整POSIX unlink语义。证据 C11–C14、C19–C21、C24–C26；DM57-F07/U05/U06。

## 归档归属、反例与验收界面

本批新增候选DM57-F01至F08均仅为可反驳的源码条件候选，不是8个已复现漏洞；DM57-U01至U06为需要额外证据的边界。旧IT35、BI55、AG56、SB38及第36批文档勘误保持原qualified_id和主线status。特别是：IT35-F01/F02记录错误指针和末项；BI55-F03/F04/F05记录iobuf跨度、标志及磁盘返回；AG56-F02记录扩容算法；本批只是将这些边界连接到新的目录消费者。旧第56批PASS_PENDING_LOCAL、R56-S01/S02不变，不进行补验。

校验方法仅包括固定GitHub文本回源、77条1–5行短引文机械摘录核对、47个自建有限长度/状态/整数/跨度模型以及DOT文本结构自查。没有预处理、编译、仓库C/ASM或脚本、旧执行者夹具、QEMU、硬件、真实盘、页/扇区吞吐测试，也没有完整本地P9。模型是已列前提下的算术与控制流推导，不是实际内核运行证据。

交付状态仅 READY_FOR_MAINLINE_REVIEW；接受/退回权由MYOS2-A-C05独立行使，最高验收档PASS_PENDING_LOCAL。下一步不是第58批、不是修复、更不是内核发布或PR合并。
