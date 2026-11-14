---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-FAT-DIRECTORY-MUTATION-57-REVIEW-001
record_type: independent_bounded_static_review_with_documentation_return
produced_by: unknown
model_per_owner: gpt pro（沿用Owner最近明确报告）
backend_identity: unknown_or_not_attestable
conversation_display_name: MYOS2-A-C05 内核分析主线续建
date: 2026-11-08
base_snapshot: agent/MYOS2-LEAD-002
read_channel: GitHub_connector
source_pin_short12: a039d9803ade
contract86_short12: 11c08d658b39
reviewed_base_short12: 9b7239b29b0d
reviewed_head_short12: 776297f1904b
reviewed_delivery_comment: 6717903727
acceptance_verdict: RETURN
return_kind: DOCUMENTATION_ONLY
returned_scope: [R57-01, R57-02]
mandatory_supplements: [R57-S01, R57-S02]
subjects: {A: CORE_SUPPORTED_WITH_LIMITS_PENDING_DOC_CLOSURE, B: CORE_SUPPORTED_WITH_LIMITS_PENDING_DOC_CLOSURE, C: CORE_SUPPORTED_WITH_LIMITS_PENDING_DOC_CLOSURE}
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original77_automatic_byte_scan: NOT_RUN
original_JSON_parse_replay: NOT_RUN
reviewer_independent_models: 50
reviewer_model_groups: {A: 24, B: 7, C: 19}
graph_blob_short12: e990671cfae3
graph_bytes: 4686
graph_nodes: 41
graph_edges: 74
graph_undefined_endpoints: 0
graph_text_blob_match: true
self_check: {verified_claims: 3, quotes_reconfirmed: 3, downgraded_to_inferred: 0, scope: 本回执三段短引文人工回源，不是原77项自动复验}
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
next_followup: CORE-USER-VFS-MUTATION-GATES-58
next_task_ref: ../88-user-vfs-mutation-gates-contract.md
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第57批：核心分析可保留；两项文档退回并入58

## 1. 裁定、范围与远端

Owner转发的内容已经是57正式交付，不只是首次接收。依据86号、协议v2 P2/P9及既有回执，完整读取六件和正式评论后，裁定RETURN，返工仅限R57-01/02。A/B/C核心条件链有源支持，但不能将整包签PASS_PENDING_LOCAL；关闭文档退回须由C05另行独立裁定。不是要求重做整个57、修内核或再次验收56。
compare 9b7239b29b0d→776297f1904b：ahead1/behind0，恰一个提交，六件A新增3405行（MANIFEST138、正文105、facts1665、evidence987、DOT128、deferred382），旧件零净改删。PR16=11c08d658b39，PR17=776297f1904b；均Open/unmerged，PR17 Draft。direct time=a039d9803ade、master=e8e60cd19540。PR正文仍停在“57待启动”，本轮将由主线更新，不把陈旧导航当真实未交付。
C05a接收、规则加载及其77引文/47模型/双ref回读为作者申报；本主线核远端提交、正文和相应证据，不认证另一对话内部所有动作或后台型号。正式评论已声明STOP_WRITING；保留同一C05a，不再轮换。

## 2. R57-01：P2定义边界与源台账必须追加纠正

原evidence A32的代码块以“/* Up to 13 characters of the name */”起始。固定msdos_fs.h显示该注释位于前一个结构体结束之后、新typedef之前，不属于msdos_dirslot_s定义体。文字真实存在不等于满足P2“定义体内”要求；这里只否定A32边界合格及全77项定义归属全部合格的总括，不指控原文不连续、空行被删或所有77段都伪造。
下面是位于实际定义内的可用短段（它单独不能证明完整packed布局和32字节总长，仍须合读完整结构及类型）：
[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirslot_s]
```c
	typedef struct msdos_dir_slot {
		__u8	id;					/* sequence number for slot
										-LDIR_Ord- */
		__u8	name0_4[10];		/* first 5 characters in name
```
原facts.source_ledger把namei_vfat.c的symbols列为vfat_dir_inode_operations_legacy_B；该完整所读文件没有这个实际定义名。真实操作表vfat_dir_inode_operations定义在myos_fat32_namei.c，namei_vfat.c的setup是引用它的另一关系。若legacy_B只是分析别名，必须显式标alias，不得冒充源码符号。同台账myos_fat32_iobuf.c的blob_sha_short12实际只写10位f577b7f657；真实前12位为f577b7f65786。
关闭条件：在58六件的D00追加替代A32锚点、实际定义归属和台账映射；旧A32标superseded_for_definition_scope，原件不改。其余76项可明确保留为历史，未重核不得升级为本轮重新验证；原77总数、旧字面匹配申报、新纠正标签与58新标签分别计数。同步正文、facts、MANIFEST、deferred和旧交付评论的消费说明，不编辑旧评论。evidence头的GPT-6自述只能按Owner报告/模型自述消费，不能优于MANIFEST的backend unknown。

## 3. R57-02：历史状态、问题身份和范围语义必须纠正

原facts.prior_findings_links用prior_conditional_with_R35-S01/S02等新造文字填canonical_status，与原记录实际字段和值不符。已回读固定旧条目：IT35-F01/F02的status=needs_evidence；BI55-F03/F04/F05的字段是disposition=source_only_conditional；AG56-F02的status=source_conditional_only；SB38各所引候选的status=needs_evidence。回执限定应独立作为consumption_notes，不代替原规范状态，不把批次PASS当单项问题已关闭。
原SB38-F01的真实问题是alloc_super返回ERR_PTR与调用方NULL检查不匹配，不是泛指“两套私有对象/外层inode来源”。原57正文及facts这类含义应分别链接SB38-F02（独立inode与MSDOS_I外层容器）和SB38-F04（private_sb_info/s_fs_info分离）；只有确实消费超级块错误指针时才保留F01作为该含义的来源。DM57-F04的prior链接也需说明与真实旧问题的关系。
原正文C2声称非整倍长度时“后一次起点可能超出请求长度”，这不能由所列循环推出。非负有限长度、步进/加法有效且每次只写1字节时，循环只在i<length时发请求；length33的相对起点为0、32，两者都在[0,33)内。这里可以讨论不完整槽、元数据来源或缓冲边界，不能把向上取整次数本身认作越出该字节区间。DM57-F08只能按限定接口事实/待证风险消费，不把这一数学误述算成新缺陷。
[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_mark_entry_romoved]
```c
	for (size_t i = 0; i < length; i+=sizeof(msdos_dirent_s))
	{
		char c = FAT32_DELETED_FLAG;
		FAT32_iobuf_write(iobuf, i + off, &c, 1);
	}
```
原facts负off模型的“rejected before deref”须改为“在有效iobuf的总长字段读取之后、buffers元素访问之前拒绝”；函数先读iobuf->nr_clus和bufsize。first_invalid=0在这例中不是发生了第0字节非法写，应区分API拒绝与首次无效访问。DM57-F03的“固定N字节”改为N个槽、N×sizeof(dirent)字节。
关闭条件：58 D00给逐项原位置→替代解释→源字段/原ID映射，覆盖57正文/facts/deferred/MANIFEST/DOT与旧评论的消费方式；提供length33及负off有效对象的反例说明。旧问题原件和原状态不改，执行者仅申报CORRECTION_SUBMITTED_PENDING_MAINLINE，不自签CLOSED。

## 4. 核心链的独立核验与R57-S01技术限定

A：固定~1、正常短helper不产生1、ceil(len/13)+短槽、上界261的helper-only容量模型和空槽计数核心可保留。完整短helper、build/add和扫描源已核；首空槽只设起点，后继才增计数：
[VERIFIED mykernel/fs/fat/myos_dir.c::fat_add_entries]
```c
			if (empty_off == -1)
				empty_off = iobuf->iter_cursor - sizeof(msdos_dirent_s);
			else
				emtpy_count++;
```
1. N为正且状态有效时，count==N对应第N+1个连续空槽；上界处已有起点但不足N仍请求N槽，普通E/X模型有支持。非零base模型是此前合法占用前缀之后的局部后缀状态，不是iter_init直接从L-32启动。N=1只是fat_add_entries独立helper模型：所选vfat_add_entry正长度且短helper返回0的路径至少产生2槽。
2. bufend必须严格超过所示next后游标，才能排除end→expand路径；当末项令end=true时会先expand并第二次next，不能直接套无扩容表。保留AG56/R35最早失效门，不能据目录上界认为所有缓冲合法。
3. 261长度模型还依赖短名处理正常结束。裁剪长度与对原串strrchr结果不必一致：条件串ABCDEFGHI.裁剪len9但末点后索引10，所选短helper可先得到extlen=-1；这是数学边界提示，不是本轮证明真实用户可达或运行memcpy。58应取证真实qstr生产和边界，不能跳过更早长度/指针问题签槽越界。
B：create正常返回不自动表示d_inode发布，mkdir的候选、点项、WRITE请求、FAT请求、int转换及父项插入需分层。u32错误编码的signed32映射仅是假设；实际C模式/编译转换未核，不能泛称所有实现必负数或必漏检。buf已分配的早退与buf分配失败分开，不把NULL分配出口叫已分配对象泄漏；不以局部缺少回滚推真实盘面已部分提交。
C：getter安全返回ERR_PTR后dir_empty可保持0，普通末项被覆盖的消费者链成立；更早attr非法访问不补出0。int helper缺return且值未被调用者消费，不能独立推出随机errno；零长度仍会init/iter_init/release，不等于完全没有操作。DELETE/FAT_FREE、父项单字节标记与外层结果互不等价；并发、打开文件寿命、持久化仍未证。

## 5. R57-S02：图和产物消费

原DOT全文重建4686字节，Git blob匹配e990671cfae3；41唯一节点、74边、零未知端点。它是关系摘要，不是穷尽CFG或运行轨迹。
iter→expand及expand→iter归属fat_add_entries内部控制，不是iter_next自己调用expand；省略的初始化失败和release必须由正文补读。create/mkdir→pubgap是“未证明发布”的解释边，不是源码必然停机；newdir到candidate的返回依实际失败/成功分支，不要求先发生所有WRITE。ret0节点的S_DEAD只适用于成功vfs_rmdir后置语句，unlink的0不授予该标志副作用。原图不用为此重绘全图，D00提供明确消费映射即可。
8个DM57条目不等于8个独立根因或已复现漏洞；F06等与IT35的关系继续明确，F08不能靠错误范围推论升级。6项U保持needs_evidence，56及52/53闭合和53误判撤回完全不变。

## 6. 实际方法与下一工作包

六件及评论全文已读；facts分1–300/301–600/601–920/921–1250/1251–1665，末段截断由1550–1665补齐；evidence分1–210/211–430/431–650/651–850/851–987补齐；正文及deferred截断均按范围补读至末。
本轮新回源：msdos_fs.h全文、namei.c300–450/560–740/740–1005/1006–1210、dcache.h35–100、fat.h200–310及380–418、string.c420–452、myos_fat32.c1–114；dcache.c1–185只为58划界。四个FAT主文件、fat32.h、options_flags及panic金丝雀按本同一有效对话此前实际完整/指定范围读取的同pin原文复用，非使用执行者摘录冒充原始源。
旧35 deferred1–160、55/56 deferred全文、38 deferred1–130用于核真实ID/字段；不是重做其验收。86、63、79、项目九份规则、Mnemosyne适用指导及R35/R36C/R38/R55/R56按本对话同固定身份已见范围消费，不声称本轮全库重新加载。
实际运行50个主线自建纯Python算术/状态用例（A24/B7/C19），不是作者47个JS模型重放。原77条CRLF自动扫描、原facts/deferred字节解析重放、Graphviz、完整本地P9、内核/仓库C/ASM/脚本/旧夹具、构建/QEMU/硬件均NOT_RUN。原77标签按A33/B18/C26手工枚举，不把字面存在、定义归属、语义支持混成一项。
本轮按28/79号把R57-01/02纠正并入58 D00，同时安排A父路径/末分量契约、B创建门及清理、C删除门和引用消费。继续C05a/CHAT-EXEC-07、原PR17，最高验收仍PASS_PENDING_LOCAL；58未自动启动，C05不写执行成果。
