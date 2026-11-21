---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-INODE-INIT-37-REVIEW-001
record_type: independent_bounded_static_acceptance_with_mandatory_supplements
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-04
produced_by: unknown
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（沿用Owner报告）
backend_identity: unknown_or_not_attestable
read_channel: connector
source_pin_short12: a039d9803ade
reviewed_pr: 17
reviewed_base_short12: 3c02d6c020e3
reviewed_head_short12: bcb7ecb66a06
reviewed_delivery_comment: 6632057217
fixed_contract61_short12: 345b997470f7
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
mandatory_supplements: [R37-S01, R37-S02]
prior_R36_closure: separate_CORE-USER-FAT-NAME-PARSE-36-DOC-CLOSURE-REVIEW-001
runtime_evidence: NOT_RUN
full_local_P9_gate: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
original_evidence_full_byte_recheck: NOT_RUN
original_JSON_YAML_mechanical_parse: NOT_RUN
original_DOT_full_mechanical_validation: NOT_RUN_IN_THIS_COMPLETION
publication_status: PUBLISHED_IN_CONTAINING_COMMIT
repository_status_update: VERDICT_RECORDED_BY_THIS_COMMIT
mainline_head_observed_short12: 345b997470f7
execution_branch_modified_by_mainline: false
next_followup: CORE-USER-FAT-SUPERBLOCK-BINDING-38
next_task_status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
owner_decisions_requested_now: []
---

# 第37批独立验收：inode初始化路径限定接收

> 本文件承接上一验收回合已完成并保全的独立裁定，本次仅恢复发布，不增加技术检查或改变裁定。
> 技术章节中的“本轮”指原验收回合；正式发布状态与同提交的发布恢复检查点合读。

## 1. 裁定、身份与交付差异

依据61号，对N01–N03的inode初始化分析作PASS_PENDING_LOCAL限定接收，无另行返工项。
原六件保持不变，后续消费必须合读R37-S01、R37-S02。
N00的R36文档闭合在独立回执中裁定，不能用本批新分析通过替代旧退回项复核。

独立GitHub compare 3c02d6c020e3至bcb7ecb66a06：
ahead=1、behind=0、恰1提交，仅user-inode-init-37/六件新增3171行：
MANIFEST133，inode-init正文298，facts1554，DOT114，deferred222，evidence850。
没有旧件净修改、删除或写区外路径。

PR17保持open/Draft、未合并，实际执行头与Owner报告一致。
指定评论6632057217分别申报R36勘误待C04闭合、37新分析READY_FOR_MAINLINE_REVIEW，并声明STOP_WRITING。
这证明远端交付状态，不证明另一个聊天进程已物理停止或不存在未推送草稿。

## 2. 可接收的核心事实

### 实际调用顺序与对象身份

new_inode调用alloc_inode；inode_init_always在alloc_inode内部、成功返回给new_inode之前执行。
alloc_inode依据sb->s_op->alloc_inode是否存在选择回调或inode_cachep备用分配，并先检查NULL。
new_inode随后初始化i_sb_list，再调用inode_sb_list_add，最后返回该inode。
列表动作只按可见调用顺序接收，不认证全局锁、引用计数或并发发布正确性。

所选fat_sops回调fat_alloc_inode分配外层msdos_inode_info_s，写入五个外层字段，并返回嵌入的vfs_inode地址。
MSDOS_I需要真实有效的包含对象；它不等同于inode->private_idx_info。
后者在FAT32_lookup中另行分配FAT32_inode_info_s，是不同对象、不同来源。

### 每次初始化实际写入字段

inode_init_always令mapping指向inode内嵌i_data，并明确写入：
i_sb、默认i_op/i_fop、i_size=0、i_blocks=0，以及mapping.host、mapping.a_ops=&empty_aops等字段；
最终把inode.i_mapping设为该mapping。正常完成出口是整数0。

因此在所选对象有效、所需调用正常返回的路径中，不能继续把i_size和i_mapping当作完全没有上游来源。
反过来，当前函数没有写i_mode、private_idx_info、mapping.nrpages或page_array，
也不能直接升级成所有路径上它们必定是垃圾值或一定未初始化。

### 一次性初始化的存在不等于执行

inode_init_once会清零整个inode并调用相应初始化；address_space_init_once有自己的清零范围。
这些定义与inode_init_always不是同一个操作。

已读FAT分配路径没有直接调用一次性初始化；已读inode_init中的cache创建参数传NULL构造器。
这只限制所选路径的证据，不能推出全仓从不调用一次性初始化，也不能凭分配器名称认定清零或不清零。

### 回接FAT与CB34-F05

在有效new_inode后，fat_fill_inode明确把i_mode置0，并按完整目录谓词分支覆盖字段。
目录分支局部不改i_size或mapping.a_ops，因此在无其它未证写者的所选路径上，继承i_size=0和empty_aops。
else分支设置de->size及fat_aops，但else只是对整个目录谓词的否定，不是合法普通文件认证。

CB34-F05由“上游来源未展开”收窄为有明确初始化来源及仍未证的字段/路径条件。
旧条目保留，不自动认定源码问题已修或全局关闭。
__lookup_slow后来按i_size设置nrpages，只有nrpages>0时才赋page_array；
nrpages不是已填充缓存页的数量，零页分支也不能推出page_array必为NULL。

## 3. R37-S01：路径绑定、字段和返回条件

1. 本批fat_alloc_inode的嵌入对象结论，以sb实际采用fat_sops为前提。
   fat_fill_super中能找到赋值，并不证明当前系统所有FAT32根目录、子目录或打开路径均使用它。
   固定源码还存在另一组FAT32_fs_type/read_fat32_superblock/FAT32_sb_ops入口；不得混为同一条构造链。

2. 当前研究保留备用kmem_cache_alloc分配路径。备用独立inode与FAT外层msdos_inode_info_s不能互换；
   后续MSDOS_I的合法性需要重新闭合实际操作表和分配来源，而不是依据“名字叫FAT”补全。

3. inode_init_always正常出口为0。alloc_inode对非零返回的清理分支是实际存在的防御代码，
   不是当前完整定义已经找到的正常错误生产者。无效对象访问或不返回不能伪造一个非零返回值。

4. NULL分配失败是正常错误返回链，不能与非法解引用混同：
   fat_alloc_inode/备用分配NULL → alloc_inode NULL → new_inode NULL → fat_build_inode ERR_PTR(-ENOMEM)。
   只有输入对象、回调或其它先行访问失去有效性时，才停止相应前缀推导。

5. 字段矩阵只陈述已核写入与继承。i_mapping=&i_data和mapping.host=inode不证明nrpages/page_array、锁、页数组内容
   或引用生命周期已经全部初始化；也不签发完整页缓存正确性结论。
   目录i_size=0是在所选构造正常完成路径中的字段结果，不是所有实际目录大小均为零的全局结论。

6. FAT32_lookup对private_idx_info的分配与随后finode字段使用、对原dentry.d_inode的赋值及清理返回须分层。
   分配请求不是成功证据，写d_inode不是callback已返回的证据；上层非NULL消费还需要实际inode和mapping有效。

7. 本轮后继划界已读到另一静态注册/超级块入口，但没有完成其整体绑定审计，不预签第38批。
   源码中注册、条件分派和赋值的存在不代表实机启动已经执行该路径。

## 4. R37-S02：图、锚点和计数的消费补正

### 控制关系

- 原37图fatalloc→stop中“kmalloc failure”应拆义：正常NULL走上述正常错误返回链，
  不能当作已发生非法访问或内核停止。stop仅用于失去有效操作/正常返回前提的推导边界。
- 备用分配也可能返回NULL；其正常指针边不应被消费成“备用分配一定成功”。
- fatcallback→lookupslow是返回值被上层消费的关系，不是回调反向调用慢查函数。
  仅在回调非NULL且共享清理正常返回后才进入上层inode/mapping消费；
  callback NULL仍由__lookup_slow转为ERR_PTR(-ENOENT)，不得走页数组初始化分支。
- noonce及其推导停止节点表示证据缺口，不是实际代码执行了一条停止指令。
  一次性函数、静态操作表、数据字段和运行时调用边继续区分。

### 定义与字段证据

- NI37-A038的empty_aops对象定义是其前两行；后面的EXPORT_SYMBOL属于定义外的导出上下文，
  不计入该单定义体证据。对象定义本身真实，不据此重签所有片段合格。
- NI37-A007与A059重复支持同一段代码；59个新分析标签不是59个彼此独立的事实。
- i_sb矩阵应结合实际赋值锚点A032/A022，而不能仅由原列A021/A031的前置上下文推出写入。
- i_fop的目录/非目录写入应分别合读A050/A052；A051本身不包含所声称的i_fop赋值。
- private_idx_info分配片段不单独证明后续解引用；须合读固定FAT32_lookup中的finode字段访问。
- fat_aops表是否为空，须合读固定fat/inode.c的表定义；赋值给a_ops的片段只证明赋值，不证明表内容或操作能力。

### 数量口径

N36C纠正来源21标签、NI37新分析59标签为作者分区计数；本主线不冒称重新运行其80条全量字节检查。
事实文件中的12个有限案例包含8个inode案例及4个旧36相关模型，不宜统称12个独立inode案例。
作者申报新37图50节点/60边、替代36图53节点/81边；关键控制语义已逐项回核，
不将作者计数、结构解析或模型声明升级为本主线已执行相同机械程序。

上述消费限定覆盖正文、facts、DOT、deferred和证据，不回写原六件。
本批核心字段来源和有界路径结论不依赖这些计数/标签歧义，故不另开返工批次。

## 5. 实际读取和未完成范围

完整读取61号、第37批六件、指定交付评论；定点合读原36RETURN、R35、R34和此前13号页缓存审查，
避免把已分析的页缓存内容重新包装为新任务。此前MyOS2/Mnemosyne约束沿用，不宣称本轮重新刷新全部指导。

固定time主要回源：
- mykernel/fs/vfs/inode.c：new_inode、alloc_inode、inode_init_always、一次性初始化和cache创建参数；
- mykernel/fs/fat/inode.c：fat_alloc_inode/fat_sops、fat_build_inode/fat_fill_inode和所需fat_fill_super赋值；
- include/linux/fs/fs.h、fat.h：inode/i_data/address_space、超级块/操作表及外层msdos对象定义；
- fs/vfs/namei.c::__lookup_slow、fs/fat/myos_fat32_namei.c::FAT32_lookup；
- N00所需myos_dir.c、fat32.h、msdos_fs.h及readent定义；配置和panic金丝雀；
- 后继划界另读fs/fat/myos_fat32.c、fs/vfs/myos_vfs.c的注册及read_super链。

未执行原始交付JSON/YAML机械解析、全部80引文字节自动匹配、原图全文机械复验或完整本地P9。
有限Python检查仅为本主线人工转录的长度/字节模型，不是执行者案例全集重放。
未运行仓库C/ASM/脚本、预处理、编译、QEMU/硬件、真实磁盘或路径，不认证完整分配器、页缓存、权限或回收。

## 6. 固定入口

61号：
https://github.com/08822407d/MyOS2/blob/345b997470f7/agent-workspace/lead/MYOS2-LEAD-002/61-user-inode-init-contract.md

本批成果：
https://github.com/08822407d/MyOS2/tree/bcb7ecb66a06/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-inode-init-37

交付评论：
https://github.com/08822407d/MyOS2/pull/17#issuecomment-6632057217

直接源：
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/inode.c
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/fat/inode.c
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/fs/fat.h

## 7. 历史阻断与本次恢复发布

上一验收回合完成裁定后，GitHub写动作缺失；当时主线345b997470f7、执行bcb7ecb66a06未改，
未生成新提交、PR导航或评论。第38批只形成暂停草案，该历史事实不追溯改写。

本次依据Owner继续推进指令，重新取得正规GitHub写入口并核对两个HEAD、开放PR、最新评论和新路径。
确认无新交付或同名回执冲突后，保留本回执全部技术裁定与限制，只把发布元数据和本节更新为正式入库版本。
同提交62号现为READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED；仍须Owner转发原C04a并通过预检后执行。
本次不修改执行分支、原36/37成果或旧RETURN，不编译运行，不把补发布说成重做验收。
实际提交身份和PR同步结果以远端回读为准，不在本文件预造自身SHA。
