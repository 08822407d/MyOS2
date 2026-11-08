---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-VFS-MUTATION-GATES-58-REVIEW-001
record_type: independent_bounded_static_review_with_documentation_return
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: unknown
model_per_owner: gpt pro（沿用Owner最近明确报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-08
base_snapshot: time
read_channel: GitHub_connector
source_pin_short12: a039d9803ade
contract88_short12: 8f53653ac34a
reviewed_base_short12: 776297f1904b
reviewed_head_short12: 8de2a76f5f73
reviewed_delivery_comment: 6720176655
acceptance_verdict: RETURN
return_kind: DOCUMENTATION_ONLY
returned_scope: [R58-01]
mandatory_supplements: [R58-S01, R58-S02]
D00_disposition: CLOSED_BY_SEPARATE_57_DOC_CLOSURE_RECEIPT
subjects: {A: CORE_SUPPORTED_WITH_LIMITS_PENDING_DOC_CLOSURE, B: CORE_SUPPORTED_WITH_LIMITS_PENDING_DOC_CLOSURE, C: CORE_SUPPORTED_WITH_LIMITS_PENDING_DOC_CLOSURE}
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original87_full_byte_scan_replay: NOT_RUN
original_JSON_parse_replay: NOT_RUN
reviewer_finite_model_cases: 47
reviewer_excerpt_checks: 4
graph_blob_short12: 653e86856119
graph_bytes: 5497
graph_nodes: 52
graph_edges: 77
graph_undefined_endpoints: 0
graph_text_blob_match: true
self_check: {verified_claims: 4, quotes_reconfirmed: 4, downgraded_to_inferred: 0, scope: 本回执四段固定源短引文人工核对}
next_followup: CORE-USER-DIRECTORY-ENUMERATION-59
next_task_ref: ../89-user-directory-enumeration-contract.md
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
phase: 2
owner_decisions_requested_now: []
---

# 第58批：核心条件链保留，一项文档RETURN并入59

## 1. 独立裁定及远端

依据88号与协议v2，58整包裁定RETURN / DOCUMENTATION_ONLY，仅R58-01要求追加纠正。A/B/C核心静态链在下述前提内有源支持，但本轮不签整包PASS_PENDING_LOCAL。57的D00已由另一份独立闭合回执处理，不能将58的新问题重开为R57-01/02。
执行compare 776297f1904b→8de2a76f5f73为ahead1/behind0、恰一提交、六件A新增3359行：MANIFEST84、正文149、facts1634、evidence948、DOT146、deferred398；旧件零净改删。PR16/直接lead为8f53653ac34a，PR17/直接执行为8de2a76f5f73，均Open/unmerged、PR17 Draft；time=a039d9803ade，master=e8e60cd19540，开放PR第三项为空。
作者正式评论6720176655声明STOP_WRITING；其规则读取、87/87自检和56模型仍是作者申报，不认证另一聊天全部内部行为或后台身份。主线完整读取六件与正式评论，检查固定源码和下列反例；不改执行成果。

## 2. R58-01：B24跨宏、B25非原样且超长

B24标签是LOOKUP_CREATE，却同时把独立的LOOKUP_EXCL定义放在同一段下。两行都真实存在，但不属于一个宏定义。必须拆为两个真实符号锚点，不能用“相邻且能匹配”代替定义归属。
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_CREATE]
```c
	#define LOOKUP_CREATE			0x0200	/* ... in object creation */
```
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_EXCL]
```c
	#define LOOKUP_EXCL				0x0400	/* ... in exclusive creation */
```
B25在return PTR_ERR(dentry);与error = vfs_mkdir(...)之间多出一个空行，共6个源码行位置；固定time实际连续片段为以下5行。已通过fetch_file定点原文与连接器raw文件入口两次确认；不是把CRLF与LF转换、Markdown围栏分隔或显示换行误当成源空行。
[VERIFIED mykernel/fs/vfs/init.c::init_mkdir]
```c
	dentry = kern_path_create(AT_FDCWD, pathname, &path, LOOKUP_DIRECTORY);
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);
	error = vfs_mkdir(path.dentry->d_inode, dentry, mode);
	done_path_create(&path, dentry);
```
因此原“87段全部定义内1–5行、87/87原文匹配”的总括不能继续直接消费。只否定已证B24/B25及相应全称自查，不指控其余85条全部错误，也不认证其余85条已全量重验；不恢复53那次已撤回的空行指控。
关闭条件：在59 D00追加B24两条单宏替代、B25真实连续片段、逐项原ID→新ID映射；原58标签87、被替代两标签、其余85历史保留、D00新标签与59新标签分别计数。覆盖58 MANIFEST/正文/facts/deferred/evidence/DOT和评论6720176655的消费说明；不修改原件或旧评论，不自行签CLOSED。
重新核新引文时分别检查物理行数（包括片段内空行）、原样连续命中、真实定义归属、命题支持。不得人工插删空行后仍报原样；源文本没有取到时如实留缺口，而非预填通过。此为既有P2/P9落实，不变更协议。

## 3. R58-S01：技术消费限定与核查

A：父path移交、last切片、dentry名字拷贝及尾点差异有源支持。正文A3的foo.写成“6→5”是笔误，实际4→3；同批facts的该条为4/3，本轮独立模型亦为4/3。后续D00追加改正消费即可，不重做整个名字专题。
__lookup_hash缓存命中前仍已执行__d_lookup并读取base->d_inode；不能表述为完全不访问父对象。miss_lookup_null/HASH-3只有在回调没有修改或撤销新dentry的d_inode及对象有效性时，才可延续构造时的NULL。回调返回NULL本身不约束它先前的原位副作用；通用helper模型与已核具体FAT回调分别使用。
NG58-F01的新增价值是do_*或kern_path_create等上层向putname传递编码错误的消费链。__myos_getname失败后未检查ERR_PTR的上游根因已有EI28-F01，29的historical_additions明确保留；不再次计作独立新根因。getname的测长/无符号错误与再次复制须合读PN29-F01/F03/F04，不因本批未展开深层helper就把已知上游记录当不存在，也不凭恒假的len<0门编造真实可达复制失败。
[VERIFIED mykernel/fs/vfs/namei.c::putname]
```c
void putname(filename_s *name)
{
	kfree((void *)name->name);
	kfree((void *)name);
}
```
B：is_dir在flags裁剪前保存；do_creatat/do_mkdirat均传LOOKUP_DIRECTORY，故其所选前缀不能靠!is_dir门拒绝末斜线。独立flags0 helper与真实do入口必须分开；EXCL位存在不证明已消费排他语义。方法缺席时直接return -EPERM跳过公共清理成立，但不是实测资源泄漏计数。有效name失败出口和putname(ERR_PTR)的最早无效消费不能混成一个安全errno模型。
C：rmdir显式负dentry门与unlink的inode->i_mode读取差异有源支持。本轮定点核stat.h及其uapi头，所选S_ISDIR宏实际消费参数；只在该源级选择和对象前提下推演，不声称完整预处理/ABI或系统调用运行。dget仅返回原指针、dput活动体为空、iput仅NULL门、path_put调用dput而mntput注释，都不能按函数名扩成完整引用协议。S_DEAD只在成功vfs_rmdir后置处成立，回调后inode仍须有效。
-ECHILD重试仅能确认同一nd再调用path_parentat，path_init重建部分字段；不签完整回滚或成功。原NG58-U01–U07的真实运行/对象/并发缺口保留，局部新读宏不自动关闭整项U05。

## 4. R58-S02：图、计数和证据范围

DOT按连接器全文重建，5497字节的Git blob与653e86856119匹配；52个唯一节点、77边、零未定义端点。未渲染Graphviz，不认证完整CFG。
last→parentat的RETURN是输出数据传递，不是数据对象调用/返回函数；cachehit→hash表示lookup返回值选择。alloc→callback的控制归属__lookup_hash，allocator自身不调用lookup。namebad节点是错误值/分析停止说明，不能把namebad→putname画成首次非法访问之后还能继续调用。仅条件有效路径可连到下游清理；原图与这些限定组合读取。
A27按LAST_NORM所在同一个匿名enum定义上下文使用，不把其余枚举成员当独立宏；这与B24跨两个独立宏不是同一情况。dput/iput的局部短片段需合读完整活动体，单个注释片段不足以证明全函数无操作。
7个NG58条目是记录数，不是7个全新独立根因或已复现漏洞。DL33/CB34/PN29/EI28及DM57原问题身份保持，完整历史去重未做。

## 5. 实际读取与未执行

58六件和评论全文已读；facts按1–320/321–650/651–1000/1001–1320/1321–1634，evidence按1–240/241–480/481–720/721–948，正文1–80/81–149，deferred1–220/221–398补齐。
本轮新回源：namei.c1–290及478–648；dcache.c108–215（主函数完整，末尾其它wrapper只定位）；namei.h全文；init.c100–140并另从连接器raw入口全文核对；dcache.h260–288；inode.c310–355；kernel/stat.h全文及uapi/linux/stat.h1–70。namei.c的__lookup_hash、filename_create、do_*/vfs_*完整定义及其余D00/FAT/类型/金丝雀，复用本同一有效C05对话已实际取得的相同固定源原文，不用作者摘录代替源码。
旧33 deferred全文、34与29 deferred定点读，用于真实ID/状态和上游去重；35/38/55/56的D00字段沿用上一轮已回读的原件。88、57回执及63号在本有效对话内全文复用；本轮另全文重读28、79和协议v2，不宣称全库指导重新加载。
主线实际执行47个自建有限模型（D00 8、A16、B11、C12），另4个B25短片段行数/文本差异检查；不是作者56个JS模型重放，不运行仓库代码。原87全量字节/定义扫描、原facts/deferred解析重放、完整P9、Graphviz、预处理/编译、仓库C/ASM/脚本/旧夹具、QEMU/硬件均NOT_RUN。
下一包按28/79号把R58-01纠正与三个目录枚举专题合并；C05a继续原PR17。主线不代执行59，不要求Owner当前修复内核。
