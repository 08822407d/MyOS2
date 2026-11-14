---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-OPEN-FLAGS-30-DOC-FIX-01-REVIEW-001
record_type: independent_additive_documentation_correction_acceptance
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: ChatGPT普通对话；C04主线
model_per_owner: gpt pro（沿用Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-03
read_channel: connector
reviewed_pr: 17
reviewed_head_short12: e33103ca7d9e
reviewed_base_short12: da292d44b40c
contract53_short12: 7fb723be2344
original_contract52_short12: 1412a1991aee
source_pin_short12: a039d9803ade
lead_before_publication_short12: 7fb723be2344
master_observed_short12: e8e60cd19540
parent_review_record: CORE-USER-OPEN-FLAGS-30-REVIEW-001
parent_review_historical_verdict: RETURN
acceptance_verdict: PASS_PENDING_LOCAL
status: DOCUMENTATION_RETURN_ITEMS_CLOSED_COMPOSITE_BATCH30_ACCEPTED
closed_returned_scope: [R30-01, R30-02]
returned_scope: []
R30_01: CLOSED_DOCUMENTATION_ONLY
R30_02: CLOSED_DOCUMENTATION_ONLY
effective_batch30_verdict: PASS_PENDING_LOCAL
original_artifacts_retroactively_certified: false
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
full_local_P9_gate: NOT_RUN
review_method: 连接器固定正文及定义上下文逐项模型复核；Python只核手工转录的ID分组与有限算式
source_byte_regex_or_hash_scan: NOT_RUN
original_all_80_byte_matching_reproduced: false
new_75_definition_contexts_reviewed: true
execution_branch_modified_by_mainline: false
owner_decisions_requested_now: []
next_followup: CORE-USER-PATH-INIT-31
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/54-user-path-init-contract.md
kernel_change_authorized: false
merge_authorized: false
phase3_authorized: false
self_check: {scope: ten_direct_source_anchors_in_this_receipt_only, verified_claims: 10, quotes_reconfirmed: 10, downgraded_to_inferred: 0, method: 模型对照本轮实际连接器源码返回文本核对，非原件全文自动扫描}
authorization_ref: Owner要求独立复核53号交付并关闭原退回项；28号同轮后继约定及已确认C04主线写权
---

# 30文档退回项闭合：原六件与追加勘误组成限定接收材料

## 1. 正式裁定与有效消费集合

独立复核后关闭R30-01、R30-02。本次纠正包及第30批组合材料的有效接受上限为PASS_PENDING_LOCAL，returned_scope=[]。关闭的是证据资格与跨件表述的文档退回项，不是修复OF30源码问题或认证内核运行。

必须合读：原user-open-flags-30/六件@da292d44b40c、原30主线回执@7fb723be2344、user-open-flags-30-doc-fix-01/的MANIFEST.md与errata.md@e33103ca7d9e，以及本回执。errata仅在supersedes_scope列明的标签、补证和同义表述上优先；原技术条件和未完成范围继续有效。原80项旧标签不能脱离勘误直接全部作为合格定义证据使用。

原RETURN文件和原六件保持字节冻结；本新增记录使退回状态向前闭合，不回写历史为PASS。第29批原有限裁定不变。原作者自查、C04本轮模型复核、未来本地机械门和内核运行分别归属，不互相替代。

## 2. GitHub身份、增量与实际阅读

PR17实际head为e33103ca7d9e，open/Draft、未合并；交付评论issuecomment-6594795900同时申报READY_FOR_MAINLINE_REVIEW和STOP_WRITING，Owner当前消息与之对应。compare da292d44b40c至e33103ca7d9e恰1提交，仅纠正目录MANIFEST.md153行、errata.md857行，两件A新增1010行，旧件零净修改/删除。主线核此增量；执行方所述双ref全文读回2/2不冒充主线重跑。

本轮完整读取53号、纠正两件及交付评论正文。errata首次长响应被截断，后续从60–100、101–210、211–420、421–630、631–775、776至末尾连续补齐，与首响应已见部分重叠，无用截断代替完整阅读。原30六件、原回执、52号及29回执沿用本C04上一轮已完整读取的同一固定对象；本轮以纠正表逐项对照，不声称又全文重跑旧六件。

独立回源均固定time a039d9803ade：open.c20–270；internal.h85–105；kernel/fcntl.h全文；generic fcntl.h15–100；uapi/linux/fcntl.h1–8；openat2.h全文；kernel/stat.h1–15；uapi/linux/stat.h7–43；fs.h44–120及3418–3430；namei.h30–70；namei.c696–766；exec.c550–615；syscall.c35–43；types.h29–38。另先读time的options_flags.cmake全文、panic.c59–85；对照master固定e8e60cd19540的配置与panic全文，并读该master公约1–45核原句。两句金丝雀与master区别确实可见。

后继范围预检另读namei.c33–55、139–196、360–620，固定fs_struct_types.h与path.h全文。默认分支fs_struct搜索仅定位路径，结论回固定time。该预检不是执行31或签发31成果。

## 3. R30-01闭合依据

原OF30-A001至A080表逐项有对应身份；处置为47项保留、31项修正、2项转声明/包含链导航，合计80，无未核或遗漏项。A038和A058不再作为定义体证据；声明存在与调用语义成立明确分层。保留项的定义归属对照同一固定旧引文及本轮实际定义上下文；原80段全文字节匹配仍不记为主线自动复现。

新OF30-X001至X075逐段检查了标签、片段长度、所标符号与实际定义上下文。13份新直接来源按编号分组覆盖75项；长宏片段仍在同一宏定义内，结构体拆段覆盖全部五字段，宏自身的行内/跨行注释与定义外围分开。未再发现R30-01所列的声明/include冒充定义、混合多个独立宏的标签或强制关键语句遗漏。

补证已覆盖：X005/X006第五字段；X019/X025/X026/X027的四个TMPFILE组件；X036/X037/X039–X044权限位组成；X060/X061的IN_ROOT/CACHED；X062的LOOKUP_EMPTY赋值；X064–X073的调用者、返回门及字段消费。新75标签与旧80处置分别清点，不把历史两个导航降级塞进新标签的INFERRED计数。

以下是主线本轮直接回源复核的代表证据；其余新锚点按上述完整分组已检查定义上下文，并非仅抽看这几项。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
```

[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	open_flags_s op;
	int fd = build_open_flags(how, &op);
	filename_s *tmp;
```

[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (flags & __O_TMPFILE) {
		if ((flags & O_TMPFILE_MASK) != O_TMPFILE)
			return -EINVAL;
		if (!(acc_mode & MAY_WRITE))
			return -EINVAL;
```

[VERIFIED mykernel/include/linux/fs/internal.h::open_flags_s]
```c
	int open_flag;
	umode_t mode;
	int acc_mode;
	int intent;
	int lookup_flags;
```

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_TMPFILE]
```c
	#define O_TMPFILE			(__O_TMPFILE | O_DIRECTORY)
```

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_DIRECTORY]
```c
	#	define O_DIRECTORY		00200000	/* must be a directory */
```

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::__O_TMPFILE]
```c
	#	define __O_TMPFILE		020000000
```

[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	if (flags & AT_EMPTY_PATH)
		open_exec_flags.lookup_flags |= LOOKUP_EMPTY;
```

[VERIFIED mykernel/fs/vfs/open.c::file_open_name]
```c
	open_how_s how = build_open_how(flags, mode);
	int err = build_open_flags(&how, &op);
	if (err)
		return ERR_PTR(err);
	return do_filp_open(AT_FDCWD, name, &op);
```

[VERIFIED mykernel/fs/vfs/namei.c::do_filp_open]
```c
	nameidata_s nd;
	int flags = op->lookup_flags;
	file_s *filp;
```

## 4. R30-02闭合依据

六件逐件消费表和原DOT的指定边/注释均有替代说明：调用为build_open_flags(how,&op)，复合掩码包含关系不再写成同值别名，getname有效结果以真实有效生产者为前提，错误返回明确为另一个未展开支路。图不是新增完整DOT；允许按53号用文字增量消费，不把说明冒称重画或全图99边认证。

op.lookup_flags的赋值改为所有前置return未发生时才到达；op_uninitialized限定于调用前确实未初始化的局部对象，一般helper仅能说本次未写。指定初始化术语已改；raw-helper、legacy正规化、原OF30-F02字段差异与确定bug、未知openat2覆盖与全仓不存在之间均分开。既有UV-04、US-03和OF30问题编号、条件、延期状态均保留。

独立Python只对从实际文本手工转录的分组和常量做有限复算：80项分区47/31/2、新编号1至75闭合且13个来源组；不是对原文件字节运行grep。五组纸面算例相符：未知0x08000000经legacy掩码为0、raw仍拒绝；O_PATH|O_CREAT经legacy为0x200000、raw在写mode后被拒；mode=0x1000经0xFFF为0、raw被拒；FMODE_NONOTIFY两路线均被去除；CACHED掩码与O_DIRECTORY交集为0x10000。字段先后仍由已读函数控制流支持，不把整数计算当C执行或实际输入可达性证明。

## 5. 自查资格、未完成范围及下一步

本轮对两件新增正文和75段定义上下文完成模型逐项复核；没有完整本地P9执行环境，没有逐文件原字节正则/哈希、全部旧80片段自动匹配、YAML解析或原99边全量功能复跑。尝试把公开固定errata读入临时沙箱用于自动扫描：HTTP读取因DNS失败，下载工具亦未提供文件；未据此编造文件级自动检查结果，实际成功取证始终走GitHub连接器。Python只执行前述手工转录数据的有限计算，不运行仓库脚本。

MANIFEST必要键、supersedes目标及限定范围、公约原句、配置金丝雀和明示的PENDING/RETURN归属均已检查。produced_by是产品面说明，模型选择仅按Owner报告，不能解释为后台认证。局部复核未发现这两项文档退回的剩余阻断，但不签整个P9自动闸门全PASS，也不追认原80标签独立全合格。未来本地机械/构建/运行仍需相应授权；本轮没有内核、C/ASM、预处理、编译、QEMU、硬件或真实用户输入运行。

同轮发布54号CORE-USER-PATH-INIT-31，继续原C03c、GPT-6＋Extra，不新建执行对话或PR。限nameidata初始化、路径起点选择、dfd/root/lookup标志的局部消费与直接调用边界；不重做29/30、不展开整套路径遍历/权限。Owner转发54号并通过预检才启动；当前e33103ca7d9e纠正成果继续冻结，C04不写执行目录。
