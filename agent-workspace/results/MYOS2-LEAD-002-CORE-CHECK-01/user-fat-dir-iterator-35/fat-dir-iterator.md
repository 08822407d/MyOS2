---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FAT-DIR-ITERATOR-35
record_type: bounded_fat_directory_iterator_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-04
date: 2026-11-04
read_channel: connector
base_snapshot: time
source_pin_short12: a039d9803ade
result_base_short12: bce8404efe6a
contract59_short12: 98608f6c58df
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第35批：目录迭代器的游标、终止条件、目录项读取和回调消费

## I00｜固定任务、34主线补充和现场身份

Owner激活固定`59-user-fat-dir-iterator-contract.md@98608f6c58df`，继续本执行对话C04a（`MYOS2-LEAD-002-CHAT-EXEC-04`），不启动新交接。已完整读取同提交的34主线回执`CORE-USER-LOOKUP-CALLBACK-34-review.md`，34限定结论`PASS_PENDING_LOCAL`、`returned_scope=[]`；其R34-S01/S02必须合读，原34六件全部冻结。历史33的R33-S01/S02、32的R32-S01/S02、31的R31-S01/S02、30-DOC-FIX-01及UV-04/OF30边界沿用，不重做其它批次。

R34-S01本轮强制消费：FAT32_lookup首次左上界不成立时`tmpde=NULL`，并不保证后来立即解引用NULL；先有`fat_build_inode/new_inode`分配门，可能先生成错误。其次，`tmplde=FAT32_iobuf_readent`在`FAT32_ent_empty(tmpde)`之前，任何下层不返回或无效数据应止步，不能越过。其`destlen`和`namelen`取`max`后才转换为size_t，单个负整数不自动代表巨大count。`tmpde`、`de_start`、`off`是不同赋值时点。R34-S02另限定：`__lookup_slow`不直接调用step_into；直接调用属于walk_component/open_last_lookups的后继。旧DOT的静态绑定/分析前提不是额外运行时检查。旧`fat_fill_inode`目录体中`i_mode=0`的直接赋值和正常仅0返回亦不由35覆判。

第34批正文B02/B03、facts的`source_conditions`及B34-C02/C04/C05/C06/C07/C08、CB34-F01/F02/F03和回执已定点消费。当前仓库public，GitHub push能力存在但不等于自动获得额外权限。冻结源码`time@a039d9803ade`，先读取`mykernel/scripts/options_flags.cmake`与`mykernel/debug/panic.c`的两条真实金丝雀并对照master；首次结果写前PR17`bce8404efe6a`、PR16/lead`98608f6c58df`，master`e8e60cd19540`，time不变。全部开放PR只有16和17，PR17为open/Draft/unmerged，PR16有147个变更路径、与本新写区无交集；`user-fat-dir-iterator-35/`起初不存在，已读取本人的34最终评论。最终写前还须重新租约核实；若结果头改变或同名目录出现，停止覆盖而非并行强推。

沿用此前完整读取的MyOS2工作令、公约、协议v2、18/24/28及Mnemosyne适用行为指导；本轮不重复加载整个已完成交接，不把历史规则继承冒称本轮重新全文获取。模型由Owner报告GPT-6＋Extra，实际后端不可认证，产品面与作者身份须分列。所有证据来自固定time源码，未执行任何仓库源码/脚本。

## I01｜`get_cluster_chain` 与 `FAT32_iobuf_init` 的状态生产者

`FAT32_iobuf_init(dir)`从借入`dir->private_idx_info`定位`finode`，从`dir->i_sb->private_sb_info`定位`fsbi`，然后读`bufsize=fsbi->bytes_per_cluster`，计算`max_nr=FAT32_MAX_FILE_SIZE/bufsize`（IT35-A014、A036/A037）。`FAT32_MAX_FILE_SIZE=(SZ_1M*64)=64MiB`，而`SZ_1M=0x00100000`；实际max_nr的除数为非零和数值可表达是前提，不从这些赋值认证真实缓存容量。内部`get_cluster_chain(dir)`取得`clus_lhdrp`，读取其`count`并把count赋给`nr_clus`；数组仅在count>0分配，数组分配**容量来自max_nr而不是count**（IT35-A014–A020）。若count>max_nr，本函数没有显式比较拒绝，索引可用性需要另证，不能由alloc成功断言安全。

只限当前`get_cluster_chain`的直接返回形态：它对链表头分配NULL返回`ERR_PTR(-ENOMEM)`；节点分配NULL也释放链表并返回`ERR_PTR(-ENOMEM)`；读到FAT_ENT_BAD时释放后记录`ERR_PTR(-EIO)`；其它定义良好完成路径返回链表头普通指针，哪怕初始cluster不大于0、链表计数可能为0（IT35-A001–A005）。真实`FAT32_read_FAT_Entry`、簇链及链表实现不在本轮审查；不能把该类型列表说成真实磁盘簇数已测试。

**关键门不匹配：**`FAT32_iobuf_init`仅`if (clus_lhdrp==NULL) return ERR_PTR(-ENOMEM)`，然后直接使用`clus_lhdrp->count`；而上面的错误出口是**非NULL的错误编码指针**，不是正常NULL（IT35-A015及A001/A002/A004）。若这些错误在本入口真实发生，函数不能据NULL门安全传播它，而在首次取count处已经失去有效指针前提；不能越过它推称iobuf_init一定返回某种错误或CPU必定故障。此为IT35-F03。

对有效普通链表，iobuf本体`kzalloc`成功后依次赋`nr_clus=count`、`max_nr_clus=max_nr`、`bufsize=bufsize`；count>0时分别分配`buffers`、`clusters`、`flags`，各自失败设置error=-ENOMEM并走对应释放标签，最后`return ERR_PTR(error)`。然后把链表节点的cluster复制到按索引的`iobuf->clusters[i]`，将对应flags置0并释放这些临时节点（IT35-A016–A022/A024/A025）。这些字段赋值不自动认证列表、数组容量、指针及物理资源所有权。

**成功分支尾部赋值顺序：**`iobuf->fsbi=fsbi`，`iter_cursor=-1`，`iter_init=FAT32_iobuf_iter_init`，`next=FAT32_iobuf_iter_next`，返回原iobuf指针（IT35-A023）。这些是静态字段绑定，需实际对象有效、已经到该尾部才生效。返回类别是有效普通iobuf或ERR_PTR(error)；定义良好完成出口未主动返回NULL。第34批`FAT32_lookup`直接执行`iobuf->iter_init(iobuf)`而没有`IS_ERR(iobuf)`门，所以如果数组分配失败返回错误指针，消费者不能自动把它当合法回调指针（IT35-A031）。同时count0分支不进入数组分配和节点复制，仍可为正常iobuf完成绑定；它的`nr_clus=0`导致下游`bufend=0`，但真实父inode是否产生count0另须证。

**`FAT32_iobuf_iter_init`**真实定义只把游标设置为0、把`iter_reach_end=false`，返回`-ENOERR`。所选宏`ENOERR=0`，所以是**int0**而不是负错误；第34批调用者忽略该返回值，不能由“未检查返回”声称这个所选函数已证会返回非零错误（IT35-A006/A056）。在正常字段绑定和调用返回的前提下，**首次 lookup 上界判断的游标确实为0**，独立上界`FAT_MAX_DIR_SIZE=65536<<5=2097152`大于0，所以第34批「首次就不满足上界」不是此实际初始化路径的正常生产状态；仍可保留为raw helper-only/无效状态模型，不回写旧条目。

## I02｜迭代与getter：先读、再加游标、最后可能抹掉返回

`FAT32_iobuf_iter_next(iobuf)`先计算`bufend=nr_clus*bufsize`，执行`ret_val=FAT32_iobuf_readent(iobuf, iter_cursor)`；**无论该函数返回什么类别，当前函数并未立即测试**，而是先令`iter_cursor += sizeof(msdos_dirent_s)`，若新游标大于等于bufend则将`iter_reach_end=true`，再返回**原ret_val**（IT35-A012/A013）。`ret_val`是指针返回值，`iter_cursor`是字节偏移，`iter_reach_end`是bool标志，彼此不可混同。iter_init只在初始化时清除end；后续next没有在入口检查end，也不在正常前移时把true重新置false。

`FAT32_get_full_ent(iobuf)`的do-while首先`ret_val=iobuf->next(iobuf)`，然后把它强转给临时`tmpval`；条件中**立即读取`ret_val->attr`**（遇长名则为真），必要时还调用`FAT32_ent_empty(ret_val)`读`name[0]`，直到条目非长非空，或end标志为真。函数没有在`attr`访问前用`IS_ERR(ret_val)`或NULL检查（IT35-A027–A029）。`tmpval`与`len`在所选函数中不作为合法性验证，强转类型不是检查。

因此**readent自身返回ERR_PTR并不意味着getter会安全返回该错误给lookup**：如果下一层给`ERR_PTR(-ENOENT)`或`ERR_PTR(-ENOMEM)`，在正常执行到getter的前提下，程序会先尝试从该错误编码地址读取`attr`，从而失去定义良好继续的前提；只有给出额外可读性证据才可以继续，不能直接推断getter一定到达后面的end覆写或上层`IS_ERR(tmpde)`。NULL亦不被IS_ERR识别为已编码错误，但不是可读的目录项。IT35-F01记录这处**错误传播前的最早无效消费点**；无panic或确定机器终态认证。

另一项区别是**终止覆盖**：循环结束之后，只要`iobuf->iter_reach_end`为真，函数就执行`ret_val=ERR_PTR(-ENOENT)`后返回，不论此前`ret_val`是什么普通条目（IT35-A030）。假设某目录缓冲合法、完整最后一个条目位于`bufend-E`、`E=sizeof(msdos_dirent_s)`，`iter_next`在正常读取该条目后把游标推进到bufend并设置end=true。即使条目是普通非长非空名字，getter对它的attr访问有效，随后仍覆写为`ERR_PTR(-ENOENT)`，所选lookup右操作数的`IS_ERR`就把它当终止错误而不消费此条目。最后一个长名/空项亦会在end为真时被覆盖；这是有条件的**末项被丢弃**源码行为（IT35-F02），不是实际用户文件失踪或磁盘错误已复现。

`FAT32_ent_empty`真实谓词只检查`de->name[0]`是否为`0xe5`、`0x00`或`0x05`，并不独立检查读取的指针有效、实体大小或所有长短名算法（IT35-A026）。如果读到中间长名/空项且end仍false，则do-while继续取下一项。若已标end又被raw helper直接调用next，它仍先尝试readent；不能把end==true凭直觉当“再调用就直接返回-ENOENT”。有类型状态矩阵详见facts的IT35-T01至T15，所有数据/指针都只作有限纸面模型。

## I03｜readent的索引、读请求、错误门与lookup真实两种上界

`FAT32_iobuf_readent(iobuf,off)`先算`bufend=nr_clus*bufsize`，真实活动检查为`off>=bufend || off<0`才返回`ERR_PTR(-ENOENT)`。因此它**检查的是开始offset**，没有在完整所选函数显式测试`off+sizeof(msdos_dirent_s)<=bufend`或记录不跨单个`bufsize`缓冲区（IT35-A007）；返回类型为`const msdos_dirent_s*`，与`iter_next`和`get_full_ent`一致，而不是整数错误码。若bufsize0、乘法/转换溢出、对象不完整，停止于最早无效表达式。结构体定义在`uapi/linux/msdos_fs.h::msdos_dirent_s`中有packed布局和固定宽度字段，`iter_next`增量来源是实际`sizeof`；未运行目标编译器sizeof，不把外部常识无条件当测值（IT35-A044–A047）。

对于入界的off，代码计算`int bufidx=off/bufsize`与`int bufoff=off%bufsize`，选择`buffers[bufidx]`。若已有缓冲则直接返回`(msdos_dirent_s*)&buffers[bufidx][bufoff]`；若为空，`kmalloc(bufsize,GFP_KERNEL)`返回NULL时`ERR_PTR(-ENOMEM)`，否则执行下面的宏读请求并把buf写入`buffers[bufidx]`，返回相同形式的目录项地址（IT35-A008/A009/A011）。必须另外要求数组分配容量、bufidx整数范围、整个目录项字段字节区合法；开始off在bufend内不等于`sizeof(msdos_dirent_s)`个字节完全可读。特殊手工off=bufend-1仅是helper-only模型，不能声称普通iter_next产生该不对齐地址。

**索引与底层请求的差异：**缓存槽按`bufidx`选，但读请求传入`FAT32_clus_to_blknr(iobuf->fsbi, iobuf->clusters[0])`，使用的是`clusters[0]`而非`clusters[bufidx]`（IT35-A010）。在`bufidx>0`的有效多簇且缓存未命中纸面条件下，源码请求仍取第0簇地址，却把结果发布到第bufidx槽。这个本地索引不一致不需要扩审整个FAT链，但不能由它证明实际磁盘数据已读错、文件损坏或权限绕过。`ROOTBLK_TRANSFER(CMD_READ, ..., iobuf->fsbi->sector_per_cluster,buf)`的**返回值在readent函数中未被使用或检查**，不能把这个宏调用标为“已成功读取数据”；具体宏展开、驱动、DMA、中断/调度和物理I/O完全opaque（IT35-F04/F05）。

`FAT_MAX_DIR_SIZE=2097152`是**lookup自身**的循环左侧上界；`bufend=nr_clus*bufsize`是**readent/iter_next自身**的偏移及end标准。两者来自不同变量和宏，没有源码等式保证相等。按正常`iter_init`、持续有效普通迭代、没有解析器更改cursor的附加前提：
- 若`bufend<=FAT_MAX_DIR_SIZE`，可先在iterator到达bufend的最后一次读取中把end设真，从getter返回`ERR_PTR(-ENOENT)`，使lookup遇到错误终止；这**不需要**先命中外层2MiB上界。完整末项还受到前述覆盖问题。
- 若`bufend>FAT_MAX_DIR_SIZE`并且连续跳过非匹配条目，可能在getter仍正常、iter_reach_end仍false时先达到lookup左侧上界；后者因短路不再调用getter，`tmpde`保留前一次已经返回的普通指针。34的CB34-F01所说“非匹配旧目录项可能被下游消费”仍属于需要这种**两个上界大小关系**、对象仍活着、parser/比较与inode构造全部正常的条件推导，不能宣称已达真实执行。
- 若count0，`bufend=0`，在成功返回iobuf并iter_init0的模型下，第一次readent(0)已产生`ERR_PTR(-ENOENT)`，但getter的`ret_val->attr`在返回前先消费错误指针。不能从读helper返回ENOENT就推断lookup也拿到这个值（IT35-T05/IT35-F01）。

这给34回执的CB34-F01/F02/F03增添真实生产者与**限制条件**：目前已找到了getter主动制造的`ERR_PTR(-ENOENT)`正常出口、读helper的`-ENOENT/-ENOMEM`错误出口、以及它们**不能被混合**的事实。CB34-F02下层错误统一变上层ENOENT的前提是下层getter确已**定义良好地返回**编码错误，不能借readent过早失效路径直接补完该链。第34批callback返回NULL后上层`__lookup_slow`转`ERR_PTR(-ENOENT)`的已有层次关系仍有效；不重做任何inode或文件打开后果，旧CB34/DL33/LC32/UV-04编号均不改。

## I04｜交付、自查和边界

只新增MANIFEST、本文、JSON兼容YAML facts、DOT状态图、延期问题账及证据，共六件。本批有56个新直接定义体短引文；旧34的64条不混入计数。facts按24对象/字段身份、15个旧游标→读取类别→新游标/end→getter→lookup→停止点的有限情形登记；DOT区分直接函数调用、字段赋值、错误/终止门与额外分析前提。IT35-F01至F06只是条件候选及证据缺口，不等于六个已证独立漏洞或六个已复现错误。

**明确未执行：**仓库C/ASM、脚本、预处理/编译、QEMU/硬件、真实FAT目录或物理盘I/O、长短名算法、完整FAT簇链/磁盘驱动/调度、new_inode/iput/iobuf释放和分配器全链、权限/挂载/锁/引用/RCU、额外付费模型/API。没有修复内核或用户程序、补丁或旧文档覆盖；没有创建PR、合并、强推或改变Draft。完整本地P9机械门未运行，局部文本/结构/整数模型不是内核运行认证。`mainline_acceptance=PENDING`，只申请`READY_FOR_MAINLINE_REVIEW`。最终提交并从固定新提交及活动分支双回读六件以后写本执行者PR17交付评论，再STOP_WRITING，将验收交C04，不自行开始36。
