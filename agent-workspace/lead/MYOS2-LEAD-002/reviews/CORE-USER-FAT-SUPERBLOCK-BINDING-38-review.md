---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-FAT-SUPERBLOCK-BINDING-38-REVIEW-001
record_type: independent_bounded_static_acceptance_with_mandatory_supplements
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-05
produced_by: unknown
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（沿用Owner最近报告）
backend_identity: unknown_or_not_attestable
read_channel: connector
source_pin_short12: a039d9803ade
reviewed_pr: 17
reviewed_base_short12: bcb7ecb66a06
reviewed_head_short12: 61a95d13dcb6
reviewed_delivery_comment: 6635234504
contract62_short12: 928d1d521e4e
lead_before_publication_short12: 928d1d521e4e
master_observed_short12: e8e60cd19540
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
mandatory_supplements: [R38-S01, R38-S02]
original_artifacts: PRESERVED_UNMODIFIED
runtime_evidence: NOT_RUN
full_local_P9_gate: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_original_66_quote_byte_scan: NOT_RUN
reviewer_original_JSON_YAML_parse: NOT_RUN
reviewer_original_DOT_blob_identity: NOT_VERIFIED
reviewer_graph_identifier_edge_transcription: {explicit_nodes: 52, unique_nodes: 52, edges: 59, undefined_endpoints: 0}
reviewer_finite_source_order_models: 15
self_check: {scope: six_direct_quotes_in_this_receipt, verified_claims: 6, quotes_reconfirmed: 6, downgraded_to_inferred: 0, method: 固定源码连接器文本逐段核对，不是原66段字节机械复跑}
next_followup: CORE-USER-OPEN-COMPLETION-39
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/64-user-open-completion-contract.md
new_write_guidance_ref: agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-04
executor_rotation: false
execution_branch_modified_by_mainline: false
owner_decisions_requested_now: []
phase: 2
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
---

# 第38批独立验收：超级块绑定与根/子inode来源限定接收

## 1. 裁定、身份和增量

依据固定62号、第38批六件、指定交付评论及R37-S01/S02，裁定PASS_PENDING_LOCAL，无另行返工项。
原六件及作者评论保留，后续消费必须合读R38-S01、R38-S02；不是完整挂载、FAT、分配器或内核运行认证。
第36批文档闭合及第37批限定验收维持原状，不重新验收或覆盖历史RETURN。

独立compare bcb7ecb66a06→61a95d13dcb6：ahead=1、behind=0、恰1提交，仅本目录六件A新增1580行。
分别为MANIFEST68、正文81、facts370、DOT115、deferred219、evidence727；旧件零净修改/删除、无写区外路径。
PR16实际主线928d1d521e4e，PR17实际执行61a95d13dcb6；两PR均open/unmerged，PR17为Draft。
direct branches同时核time a039d9803ade和master e8e60cd19540；已知两PR开放且开放列表第3项为空。
交付评论6635234504对应CHAT-EXEC-04、READY_FOR_MAINLINE_REVIEW和STOP_WRITING声明。
这些证据不证明另一聊天物理停止，也不认证未推送草稿。作者报告的某时点mergeable=false不等于本写区冲突；本轮PR元数据为true，未合并或处理合并冲突。

## 2. 得到独立源码支持的核心结论

A路线为FAT32_fs_type/read_fat32_superblock/FAT32_sb_ops，B路线为vfat_fs_type/vfat_fill_super/fat_sops。
register_diskfs活动体只调用init_fat32_fs；mount_fs匹配名称后调用read_super，而非mount。
表定义、注册尝试、被选调用和真实启动运行是四种不同证据，不互相替代。

A的alloc_super失败出口确实为编码错误，不是正常NULL。

[VERIFIED mykernel/fs/vfs/super.c::alloc_super]
```c
	if (s == NULL)
		return ERR_PTR(-ENOMEM);
```

A调用者只测NULL，因此不能把上述实际失败路径描述成已经安全转发给mount_fs。

[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	sbp = alloc_super(&FAT32_fs_type, 0);
	if (sbp == NULL)
		return ERR_PTR(-ENOMEM);
```

FAT32_sb_ops是文件作用域对象，其完整初始化列表如下；结合真实super_ops_s定义，未指定alloc_inode成员按所选C聚合初始化规则为NULL。
它不是const表，仍须没有后续改写的前提；这与任意堆字段缺少赋值不能混同。

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_sb_ops]
```c
{
	.write_super = fat32_write_superblock,
	.put_super = fat32_put_superblock,
	.write_inode = fat32_write_inode,
};
```

A根inode直接申请sizeof(inode_s)，所选构造体没有调用每次/一次性inode初始化或直接给i_mapping/i_data赋值。
A子inode在该表未改、父对象和前缀有效时走inode_cachep备用分配；有效new_inode返回前确实完成always与列表步骤。
但之后的FAT填充要求真实外层msdos_inode_info_s，独立inode的来源并未提供这个包含关系。

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
	inode->i_mode = 0;
	MSDOS_I(inode)->i_pos = 0;
```

B的fat_sops则真实绑定fat_alloc_inode，后者分配外层msdos_inode_info_s并返回嵌入vfs_inode。
B对象来源不能用来替A补齐容器；private_idx_info的FAT32_inode_info_s也不是同一外层对象。
private_sb_info与s_fs_info是两个不同字段，其两条已读生产链分别建立不同类型对象，不能互换。

在B先行工作及fat_read_root定义良好、非负返回且清理正常返回的条件下，尾部会落入out_invalid。
原根发布和return0是注释；这只证明局部完成路径的限制，不证明mount_bdev已调用或某次挂载已失败。

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_super]
```c
out_invalid:
	error = -EINVAL;
```

## 3. R38-S01：实际门、生产者和最早停止点

1. 注册函数在next及重名拒绝门之前，可能先初始化fs->fs_supers。
   因此正文“先检查next及重名”只描述注册接受条件，不能当成完整语句顺序，也不能推出-EBUSY路径绝无对象副作用。

[VERIFIED mykernel/fs/vfs/filesystems.c::register_filesystem]
```c
	if (fs->fs_supers.count == 0 &&
		(si_hdr->next != si_hdr ||
		si_hdr->prev != si_hdr))
		INIT_LIST_HEADER_S(&fs->fs_supers);
```

2. 启动选择须保留先行有效性、分区循环正常结束、GUID两部分及i==0门。
   某次不匹配不等于立即执行set_init_mount：仍须继续循环；其它i上的匹配也不会由本门更新根。
   没有调用mount_fs与调用后返回NULL是两条路径。前者保留myos_root_sb旧值，只有已证初始NULL且无此前写者才可判为NULL。
   set_init_mount先赋mnt_sb，再读取myos_root_sb->s_root；失败前可能已有前缀字段写入，不能称整体原子操作。

3. A子inode的停止边界包括MSDOS_I/container_of表达式求值和外层成员访问的对象合法性。
   不能保证先合法计算地址、到最终store才首次失效；缓存分配舍入或附近可访问字节不证明存在所需外层C对象。
   本批没有实测布局、负偏移、内存破坏或漏洞可利用性。B若真正绑定fat_sops且分配有效，才可引用其真实包含关系。

4. B尾部-EINVAL不是每一种失败的统一返回值。初始sbi分配NULL返回-ENOMEM；sb_bread失败时已有-EIO；
   在已读门均通过、root_inode=new_inode返回NULL的局部模型中，error仍为此前设置的-EIO，因为附近error=-ENOMEM是注释。
   fat_read_root负返回沿原值走out_fail；只有非负返回后正常落入out_invalid才被改为-EINVAL。
   所有这些返回还依赖对应清理能正常结束；不得跨过更早无效访问或未能定义良好返回的callee补出确定errno。

5. alloc_super收到type参数不等于已写sb->s_type；其所选完整活动体没有该赋值。
   A的已证s_op绑定不能扩充成全部超级块元数据完备。B清空s_fs_info并不证明整个根inode/设备/挂载资源已经回收；本轮不做完整生命周期裁定。

6. A根i_size=0不推出i_blocks=0：按正数几何的有限整数模型，bytes_per_cluster=4096、bytes_per_sector=512时该式为7。
   这是源码表达式的条件数值，不是文件系统规范要求或磁盘占用测量；非零除数、乘加和类型转换条件仍须保留。
   根mapping缺局部来源不意味着FAT32_lookup入口必先读mapping，更不意味着所有路径都会立即失败。

## 4. R38-S02：图和证据的消费限定

图为局部路径示意，不是穷尽控制流图。标识/边转录复算为52唯一显式节点、59边、0未定义端点，与作者申报一致。
该检查没有核原DOT的完整Git blob身份，也不是Graphviz解析/渲染；关键控制语义另外对照源体判断。

- select没有false边，不得由图省略推断未匹配时停止。补充读法为：未命中或i不为0→继续原分区循环→若循环正常结束，消费此前保留的myos_root_sb；不虚构read_super返回或赋值。
- rootI→lookup为后续数据/操作表关系，不是根构造函数立刻调用lookup。真实启动路径还需其后构造、正常返回、根发布和之后实际调用；直接helper调用须另列前提。
- nullgate→defnull是源码防御分支，无所选alloc_super的正常NULL生产者；不要将真正ERR_PTR失败重定向到该边。
- B的nullB→cleanupB须按R38-S01保留当前error，不能类比fat_build_inode的NULL转换而替它填入-ENOMEM。
- A/B的操作表、field/data和analysis boundary均不是新增检查或CPU停止指令；普通NULL错误返回与失去合法性前缀分开。
- A046只含标签和赋值，单段不能证明此前必然fallthrough；必须合读fat_fill_super完整尾部及被注释的发布/成功返回。
- A009单行证明file_systems初始化为&tmp；与另一个filesystem对象不同的结论合读filesystems.c和myos_vfs.c的两个实际对象定义。
- A023只展示初始化列表；静态存储期与可变性依据完整FAT32_sb_ops对象定义，不仅凭这五行推断。

上述补正统一约束正文、facts、DOT、deferred及证据消费。没有覆盖原六件，也没有把原66标签总括重签为机械复验全部通过。
SB38-F01..F06与NI37/CB34等旧候选保留条件及原状态；接收分析材料不等于六个独立已复现漏洞或源码问题已闭合。

## 5. 方法、读取范围和未执行项

本轮完整读取62号、38六件与评论6635234504；重新读取37回执85–155覆盖R37-S01/S02，36闭合及R36C-S01沿用同固定提交此前已读记录。
六件范围：MANIFEST全文；正文全文响应加65至末尾补齐；facts1–210/211至末尾；deferred全文；evidence1–250/251–500/501至末尾；DOT全文。
MyOS2工作令、18/24/28和适用Mnemosyne指导继承既有加载，不宣称本轮全量刷新；Owner新增写入指令由同提交63号独立登记。

固定time回源：myos_vfs.c全文；filesystems.c1–95；super.c99–200；myos_fat32.c413至末尾；namei_vfat.c288–325；
fat/inode.c138–348与409至末尾；vfs/inode.c85–170、274–365；fs.h1331–1386、1980–2015、2312–2345；
fat.h113–177；myos_fat32_namei.c14–96、199–215；配置14–43及panic72–82金丝雀。
66段涉及的10份直接源均有本轮定点上下文回读；不等于66段原字节自动扫描。

运行的本地检查只包含手工转录DOT标识/边的计数和15组自建有限源码顺序模型：注册的先行初始化与三种结果、A/B分配选择、
ERR_PTR与NULL门、两种根blocks数值、B四种错误来源、未选中分区保留根、A子分配NULL与容器边界。
模型采用明确有效对象/正常返回条件，不是作者12例全集重放、不是仓库C。
尝试一次公共raw镜像下载遇DNS失败，未取得原字节；证据仍来自固定GitHub连接器，未把失败当成仓库无权限。

未做原JSON/YAML机械解析、原66引文自动字节扫描、原DOT完整blob校验、完整本地P9、预处理/编译、仓库C/ASM/脚本、QEMU/硬件、真实磁盘或路径。
不认证完整挂载、分配器、锁/权限/引用/资源回收；不同对话模型复核不等于人类或异构技术验收。

## 6. 后继划界与固定入口

后继回到打开完成链，而不无限扩大FAT/挂载审计：CORE-USER-OPEN-COMPLETION-39，任务书64号。
设计预检另读固定namei.c550–780、open.c1–70/180–280/300至末尾、file_table.c55–160；默认分支search仅定位vfs_open后回固定源。
该预检不是39验收。39消费UV-04、30与13已有结论，追file字段、回调和状态返回；不假定A子inode已越过本批容器缺口。
新63号适用于主线和后续次一档执行者：暂态异常先读回确认、间隔多轮安全重试，再报告真实阻断，不因一次异常直接停止全部工作。

固定任务：https://github.com/08822407d/MyOS2/blob/928d1d521e4e/agent-workspace/lead/MYOS2-LEAD-002/62-user-fat-superblock-binding-contract.md
固定成果：https://github.com/08822407d/MyOS2/tree/61a95d13dcb6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fat-superblock-binding-38
交付评论：https://github.com/08822407d/MyOS2/pull/17#issuecomment-6635234504
固定源码：https://github.com/08822407d/MyOS2/tree/a039d9803ade/mykernel

C04仅新增主线回执/规则/任务/检查点并更新既有PR导航，不写执行成果。第39批由Owner转发固定入口并通过预检后才启动。
