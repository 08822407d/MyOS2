---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-OPEN-FLAGS-30-REVIEW-001
record_type: independent_bounded_static_review_documentation_return
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: ChatGPT普通对话；C04主线
model_per_owner: gpt pro（沿用Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-03
read_channel: connector
reviewed_pr: 17
reviewed_head_short12: da292d44b40c
reviewed_base_short12: 239e2f8efc45
contract52_short12: 1412a1991aee
review29_short12: 1412a1991aee
source_pin_short12: a039d9803ade
lead_before_publication_short12: 1412a1991aee
master_observed_short12: e8e60cd19540
acceptance_verdict: RETURN
status: DOCUMENTATION_CORRECTION_REQUIRED_TECHNICAL_PREFIX_REUSABLE
returned_scope: [R30-01, R30-02]
return_kind: evidence_qualification_cross_artifact_consistency_and_selfcheck
kernel_correctness_verdict: NOT_ISSUED
runtime_evidence: NOT_RUN
mainline_reproduced_all_executor_checks: false
reviewer_parsed_original_JSON_YAML_DOT: false
reviewer_finite_integer_checks: 15_reported_key_value_checks_match
execution_branch_modified_by_mainline: false
owner_decisions_requested_now: []
next_followup: CORE-USER-OPEN-FLAGS-30-DOC-FIX-01
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/53-open-flags30-document-correction-contract.md
batch31_started: false
kernel_change_authorized: false
merge_authorized: false
phase3_authorized: false
self_check: {scope: six_source_anchors_in_this_receipt_only, verified_claims: 6, quotes_reconfirmed: 6, downgraded_to_inferred: 0, method: 模型对本轮连接器返回的固定源码逐段核对；不是原80条机械复跑}
authorization_ref: Owner要求按52号及29回执独立验收30；28号同轮后继安排及已确认C04主线写权
---

# 第30批：核心静态分析保留，文档证据层退回纠正

## 1. 裁定与交付身份

本次正式裁定为RETURN，不签发第30批PASS_PENDING_LOCAL。退回的是R30-01/R30-02文档范围，不是要求修内核、撤销全部分析或重做第29批。原六件、原作者交付评论及全部旧成果冻结；纠正件另存，由C04再次验收后才关闭退回项。

52号§6明确禁止以声明、include和定义外内容冒充函数/宏定义体证据，并要求逐锚点核文本与符号归属；R29-S03已经提示同类问题。本批虽然恢复了MANIFEST标准键，仍重复出现不合格的证据标签及跨件不一致。协议v2 P2/P9不能因主要结论看起来合理而被跳过。因此不沿用“只在主线附一条说明就视为原件通过”的处理。第29批既有限定裁定不追溯改写。

PR17实际头及交付评论issuecomment-6594008326均为da292d44b40c，PR仍open/Draft、未合并。compare 239e2f8efc45至da292d44b40c恰1提交、六件A新增2190行：MANIFEST125、正文106、facts733、DOT176、deferred218、evidence832，零旧件净修改/删除。执行方报告的双ref全文回读、80条文本核对和图检查仍归作者，不改记为C04复现。

## 2. 已独立支持、可有界保留的技术内容

固定源码先读取options_flags.cmake，再回核相关定义和函数。以下源事实支持本批的主要分析，而不使整份交付自动合格。

[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	open_flags_s op;
	int fd = build_open_flags(how, &op);
	filename_s *tmp;
```
[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	tmp = getname(filename);
	if (IS_ERR(tmp))
		return PTR_ERR(tmp);
```
[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	fd = get_unused_fd_flags(how->flags);
	if (fd >=0)
	{
		file_s * f = do_filp_open(dfd, tmp, &op);
```

第一份校验返回没有在所读函数中被检查；getname正常取得有效对象后，第二次fd赋值覆盖它。到do_filp_open还要求第二次fd非负；不能把这个条件链说成已经打开文件或已发生权限绕过。

[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (flags & __O_TMPFILE) {
		if ((flags & O_TMPFILE_MASK) != O_TMPFILE)
			return -EINVAL;
		if (!(acc_mode & MAY_WRITE))
			return -EINVAL;
```

按已读固定宏，旧open正规化能保留不完整__O_TMPFILE或只读O_TMPFILE输入，触发上述局部错误门；此时helper只写过op.mode。不能将所有raw-helper错误都推广为旧open入口可达。

[VERIFIED mykernel/fs/vfs/namei.c::do_filp_open]
```c
	nameidata_s nd;
	int flags = op->lookup_flags;
	file_s *filp;
```

这是上述部分初始化模型的第一处字段读取边界。结论止于尝试读取未由helper赋值的字段，不跨过不确定值消费或旧UV-04推断具体机器/文件结果。

[VERIFIED mykernel/fs/vfs/open.c::file_open_name]
```c
	open_how_s how = build_open_how(flags, mode);
	int err = build_open_flags(&how, &op);
	if (err)
		return ERR_PTR(err);
	return do_filp_open(AT_FDCWD, name, &op);
```

与用户路径不同，file_open_name确实在非零校验返回时拒绝继续。do_open_execat又是独立的聚合初始化及AT标志门，不经过build_open_flags；其省略mode字段为零的源码模型，不能被说成运行已认证。

还可保留：五字段的写入先后、旧入口resolve的聚合零初始化、ACC_MODE低两位的4/2/6/6映射、O_EXCL分支晚于op.open_flag保存，以及CACHED表达式包含O_DIRECTORY位。OF30-F02的两字段差异本身不证明设计错误；OF30-F03仍为非零resolve的helper条件模型；OF30-F04仍为覆盖缺口而非已证缺陷。既有UV-04与US-03不关闭、不重编号，普通源码问题继续延期。

## 3. R30-01：证据资格与自查表述未达到固定合同

必须纠正以下明确实例，并检查原80条是否还有同类问题；不要求重做整个打开子系统。

| 原位置 | 本轮确认的问题 | 要求 |
|---|---|---|
| evidence A038 | internal.h中是extern声明，且混入其他接口声明；不是build_open_how定义体 | 降为声明导航，实际语义引用open.c定义；不能继续计作合格定义体锚点 |
| evidence A058 | uapi/linux/fcntl.h的两条include被标为fcntl符号的VERIFIED | 改为包含链导航，不虚构名为fcntl的定义体 |
| A036、A039、A041、A043至A052等 | 前置注释、条件编译外围或相邻不同宏一起放进单符号锚点 | 分离导航与定义；需要证明的每个宏使用自己的真实定义体片段 |
| A001、A042、A054至A056、A059至A063、A066至A068、A080等 | 一个标签覆盖多个不同宏/typedef或无关邻接内容，文本命中不能替代逐符号归属 | 逐项裁定，必要时拆成有旧ID映射的新锚点；不要因本表列举而把内部合法注释也一律删掉 |
| A037 | 标题宣称五字段，片段只显示到intent，未含lookup_flags | 给出包含第五字段的结构体定义证据；不只修改标题而留下正文无依据 |
| A051、A059至A062、A068、A076 | 当前短片段未完整支持所消费的__O_TMPFILE数值、S_IALLUGO组成、LOOKUP_IN_ROOT/CACHED定义或LOOKUP_EMPTY赋值 | 从相同固定源补关键定义/赋值；可用多段各1–5行，不用标题代替缺失内容 |

上述原文在固定源中真实存在；本回执没有指控源文本被捏造，也没有把作者的字节匹配记录改判成“必然没做”。问题是证据等级与归属：80个标签和80次文本匹配不等于80条均满足P2。MANIFEST及交付摘要关于“文本证据及其归属核数80/80”的结论不能继续无条件消费。

纠正件须对原80个ID逐项写保留（已重核）/保留（未重核，原因）/修正/撤回，修正项给可直接消费的替代证据，导航项明确降级。关键结论不能用降级导航代替所需的真实函数/宏证据。同步处置正文、facts、DOT、deferred和MANIFEST中的同义表述。原件不编辑，以errata覆盖明确范围；新自查计数按新件实际标签计算，与原80条处置统计分开。

## 4. R30-02：图示、术语与条件适用范围

DOT的doint到buildflags写成build_open_flags(&how,&op)，而真实do_sys_openat2参数how已经是open_how_s指针，源码调用是build_open_flags(how,&op)。前者增加了一层并不存在的取址，须修正图标注。

DOT首部“O_TMPFILE aliases O_DIRECTORY”及CACHED边的“O_DIRECTORY alias”不能表示两者同值别名。真实关系是O_TMPFILE复合掩码包含O_DIRECTORY位；这与O_NDELAY和O_NONBLOCK的同值别名不同。修正该图及任何同义概括。A003所称“复合字面量”实际是局部聚合变量的指定初始化，不影响resolve零值结论，但应准确命名。

图的getname正常结果节点不能兼任完整错误指针生产者；错误出口若画出，应有相应结果数据来源，或明确这是省略错误返回边的局部图。未出现ERR_PTR检查失败不等于任意指针已经有效；依赖已建立的getname正常生产模型。op.lookup_flags的写入须表示“未发生任何前置return时”，不是在CACHED拒绝return之后仍执行。

facts的op_uninitialized在本批未初始化的局部op模型中成立；对任意raw-helper调用者只能断言“本次helper未写这些字段”，调用前可能已有初始化。不得无条件外推。O_PATH、未知flags/resolve、非法mode和CACHED示例继续区分旧open正规化输入与helper-only输入。补52号要求的未知位正规化前后对照及O_PATH raw/legacy对照，作为有限纸面值例，不运行用户程序。

“没有认证openat2入口”不等于“全仓不存在该入口”；只保留实际已读范围。OF30条目之间及与US-03的链接不是自动的同根证明。这些均在纠正文档范围内，不追加VFS权限或调用者穷尽审计。

## 5. 独立检查范围与限制

本轮完整读取固定1412a1991aee的52号和29回执，完整读取da292d44b40c的六件及交付评论；evidence分1–240/241–480/481–720/721至末尾，facts分1–245/246–490/491至末尾，正文与DOT分段补齐。复读协议v2全部正文。既有工作令、18/24/28/45及Mnemosyne指导沿用本C04已实际加载状态，不导入其他项目任务，不声明本轮重新刷新全部指导。

本轮独立回源：open.c 1–275；internal.h 85–110；kernel/fcntl.h全文；generic fcntl.h 1–145；uapi/linux/fcntl.h全文；openat2.h全文；kernel/stat.h全文；uapi/linux/stat.h 1–55；fs.h 35–125和3415–3435；namei.h 1–85；namei.c 675–770；exec.c 550–615；syscall.c 35–45；types.h 29–38；options_flags.cmake全文。所有来自固定time a039d9803ade。旧UV-04/US-03证据限定沿用本C04此前已读材料，没有本轮重做历史批次。

在现成临时沙箱运行自写Python有限整数核算，不执行仓库代码：原15示例的关键数字/谓词逐项与所读宏复算相符。核到ACC_MODE=[4,2,6,6]、O_PATH筛后2097152、__O_TMPFILE组合值4194304不等于4259840、只读/只写MAY_WRITE分别假/真、CACHED掩码4260416含DIRECTORY、EXCL后局部flags=131264、CLOEXEC剥离后2097152、SYNC结果1052672、mode正规化4095及带S_IFREG为36863、非create模式0、互斥resolve=24。每个示例只核其列出的关键数值，不是编译器执行、所有字段的穷尽模型或实际输入可达性证明。

未机械复跑全部80条引文、图99边、完整JSON/YAML解析或所有字段快照；未声称完整P9通过。没有预处理、编译、C/ASM、仓库脚本、QEMU/硬件、真实用户输入、外部libc/镜像或额外付费模型调用。源码文字、有限算式、作者自查和主线裁定分开；同厂分角色复核不是异族或人类技术验收。

## 6. 同轮后继与停止点

按28号同轮安排，本轮同时发布53号CORE-USER-OPEN-FLAGS-30-DOC-FIX-01，继续原C03c，限定新目录MANIFEST.md和errata.md两件。当前选择先闭合反复出现且52号已明确要求的证据缺口，不把RETURN隐藏为通过后直接放行31，也不要求Owner决定内核修复。

这不是31批，也不修改52号历史任务。Owner向现有C03c转发固定53号且预检通过后才启动；纠正交付不自动关闭R30-01/R30-02，仍交C04复核。第29批已存裁定不变，31未派发。原PR16/17、执行者、固定源码和阶段2边界保持。
