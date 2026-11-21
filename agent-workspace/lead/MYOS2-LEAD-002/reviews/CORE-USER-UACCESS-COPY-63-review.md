---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-UACCESS-COPY-63-REVIEW-001
record_type: bounded_static_review_with_documentation_return
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: unknown
model_per_owner: gpt pro（沿用Owner最近明确报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-09
base_snapshot: time
read_channel: GitHub_connector
source_pin_short12: a039d9803ade
contract93_short12: 3044d74381f0
reviewed_base_short12: 54101196eb11
reviewed_head_short12: 51477ed51ee2
reviewed_delivery_comment: 6736044109
acceptance_verdict: RETURN_DOCUMENTATION
returned_scope: [R63-01]
subjects: {A: CORE_RETAINED_WITH_LIMITS, B: CORE_RETAINED_WITH_LIMITS, C: CORE_RETAINED_PENDING_DOCUMENTATION_GATE}
mandatory_supplements: [R63-S01, R63-S02]
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original64_full_automatic_byte_definition_scan: NOT_RUN
original_facts_deferred_full_JSON_parse: NOT_RUN
reviewer_finite_model_cases: 51
selected_original_quote_failures: [C14, C15, C16]
graph_blob_short12: cdffe2b6b217
graph_bytes: 5765
graph_nodes: 56
graph_edges: 64
graph_undefined_endpoints: 0
graph_text_blob_match: true
self_check: {verified_claims: 4, quotes_reconfirmed: 4, downgraded_to_inferred: 0, scope: 本回执四段替代摘录，非原64条全量重放}
next_followup: CORE-USER-UACCESS-RECOVERY-64
next_task_ref: ../94-user-uaccess-recovery-contract.md
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
phase: 2
owner_decisions_requested_now: []
---

# 第63批文档退回；A/B/C技术主干按限定保留

## 1. 裁定、真实交付和权限

完整读取固定六件及正式评论6736044109，结合93号合同及P1–P13，裁定RETURN_DOCUMENTATION，唯一合并退回项R63-01。不是内核修复任务，也不要求重新执行整个63；但现有明确P2失败未处置前，不签整包PASS_PENDING_LOCAL。原件冻结，后继D00追加纠正并交主线独立裁定，执行者不能自关退回项。
compare 54101196eb11→51477ed51ee2为ahead1/behind0、一个提交，严格六件A新增2623行：MANIFEST72、正文56、facts1414、evidence656、DOT134、deferred291。旧文件零净改删，无内核或越界路径。
直接lead/PR16=3044d74381f0，执行/PR17=51477ed51ee2；两PR Open/unmerged，17 Draft，开放第三项为空。direct time=a039d9803ade、master=e8e60cd19540。PR的缓存base_sha不替代direct master。
交付评论申报STOP_WRITING；这是固定交付及状态证据，不是主线对另一会话隐藏模型、全部读取或历史工具过程的认证。第62及更早验收、57/58文档闭合和58 B25误判撤回均保持。

## 2. R63-01：三段引文原样性、自检口径和署名补录

evidence.md固定blob短号3b90fd8cbd04；已用固定路径、独立blob入口、定点再读三种读取核同一原件，并与time的readdir.c原文及此前同pin全文对照。局部Python只处理连接器文本转录，不运行仓库代码。

| 原标签 | 原片段物理行数 | 保持同一首尾的真实源码区间 | 确认差异 |
|---|---:|---:|---|
| C14 | 5（含末尾空行） | 6 | prev_reclen赋值之后的一个空行被删除 |
| C15 | 5 | 8 | d_off、d_ino、d_reclen赋值之后的三个空行被删除 |
| C16 | 5 | 6 | copy_to_user赋值与终止NUL写入之间的一个空行被删除 |

三段非空语句顺序与源码一致，但原片段不是原样连续的源码文本。问题不是“当前围栏内超过5行”，而是通过删掉源码空行形成了不连续摘录；不得混称制造了不存在的非空语句。58 B25的旧指控已经撤回，本轮不重新使用它，也没有要求为它再次返工。
原MANIFEST和交付评论的64/64逐字通过、P9 item2全通过声明不能继续作为成立的验收证据。64是标签数量；本轮至少确认上述三项不合格，其余61条不因此自动成为主线全量通过。后继必须明确追加修正这些总括声明，不倒改原件或原评论。
另，63 MANIFEST未给produced_by字段。D00补记此P13元数据缺项；新64 MANIFEST按实际可声明的界面/来源填写，无法认证则明确unknown，与Owner所选GPT-6＋Extra另列，不能猜测后台或伪造旧时配置。

以下四段为同一定义内可用替代，执行者仍须独立回源并登记旧标签到新锚的对应关系。

C14替代（4行）：
[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	prev_reclen = buf->prev_reclen;

	dirent = buf->current_dir;
	prev = (void *)dirent - prev_reclen;
```

C15拆为两个片段，不删除中间源空行：
[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	prev->d_off = offset;

	dirent->d_ino = ino;

	dirent->d_reclen = reclen;
```

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	dirent->d_type = d_type;
	cplen = copy_to_user(dirent->d_name, (char *)name, namelen);
```

C16替代（5行）；后续current_dir/count更新由已有C17或另外的合法片段支持：
[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	cplen = copy_to_user(dirent->d_name, (char *)name, namelen);

	dirent->d_name[namelen] = '\0';

	buf->prev_reclen = reclen;
```

R63-01完成条件：64新包内追加原位置/原说法、保留或修正裁定、真实替代锚、影响范围与实际自检数；明确旧64/64不得沿用及署名补录限制。只处置文档，不改63六件、旧评论或内核；关闭权仍在C05。

## 3. 得到源码支持的主干与R63-S01

A：在所读include顺序及宏状态前提下，x86同名对象宏使generic的#ifndef保护跳过备用函数；并非运行时先后做两种地址检查。小尺寸快捷分支取起点符号，一般分支取无符号sum的符号和sum>=起点；generic备用门按TASK_SIZE_MAX判断。数值真不证明映射、对象、访问权限或canonicality。UC63-F01是选择依赖记录，F02是条件差异，不据此自动定性为可利用漏洞。
B：from包装调用raw后，res非零会请求尾部memset；to包装只有access_ok为真才到instrument/raw，否则返回原n。前缀复制、残量范围及真实内存行为仍需底层契约。源码的两种asm候选、元数据字符串、stac/clac和返回操作数不等于实际CPU替换或异常恢复已经成立。
C：活动get_user/put_user为直接load/store之后求-ENOERR；正常0有对象与求值前提。rt_sigreturn在两次标量读取后还可能由restore_sigcontext的copy_from_user非零结果走badframe；filldir64的直接字段store、名字复制和终止符store是不同事件，cplen未被用作错误门。三段坏引文不能继续单独作直接证据，主线从完整定义保留这一核心观察。

强制限定：
1. r>n时n-r的无符号回绕本身在所选模型中有定义；缺失的是其作为目的缓冲偏移/填零范围的合法依据，不能直接等同有符号溢出、确定CPU异常或实际越界写入。n0/r0仍不认证底层汇编/指针全部有效。
2. “source有效、raw正常返回”与“raw履行0..n残量及前缀契约”分开；只有后者加目的有效与memset正常完成，才能得到填零后的缓冲结论。to方向的instrument调用也有正常完成前提。
3. get_user除了ptr读取还要求目标x为合法可写左值，put_user要求值表达式、类型转换及目标store均合法；不能仅看ptr非NULL。任意helper的类型错配不等于所列真实调用者已经错配。
4. deferred的UC63-F06写“access_ok(frame,sizeof frame)”应统一按实际sizeof(*frame)消费，不是指针大小；其“dentry字段”应读作用户dirent记录字段，不是VFS dentry对象。两个正常get_user为0不认证完整frame、恢复成功或最终用户返回。
5. filldir64在直接store前还有名称、容量、回调对象和reclen算术前提。copy_to_user正常返回非零后，也要终止符地址及后续计账合法才能继续；不能越过更早无效访问签return0。badframe本地return0和SR08-02最终返回仍分离。
6. asm字符串中的call不是按声明形参执行的C调用；uint声明不单独证明长度窄化。rep_movs_alternative仅在本批已读来源未定位实现，未签全仓不存在。六项UC63为条件记录，八项缺口及旧RS/DE59/FC60/BI55/WV62身份状态均不改变；不宣称穷尽历史去重。

## 4. R63-S02：图和方法口径

原DOT全文重建5765字节/134行，完整Git blob与cdffe2b6b217一致；正则核56唯一节点64边、0未知端点。没有运行Graphviz，结构一致不免除下列语义限制。
include/guard边是源码选择关系，不是运行时分支；numericresult→notmapped是证据边界，不是程序停止或CPU异常。inlineto→rawto须access_ok为真且前缀正常；inlinefrom→frompadding须res非零且对象/范围有效。clac位于正常asm完成之后，不证明所有异常均返回这里。
restore还可能返回false而回到badframe，原图没有画出这条独立错误边；不能只凭frameguard→badframe声称该标签只有一个来源。restorefrom/copyname是摘要节点，不是额外真实helper；copyres→nul不是按cplen判定的门。
主线实际运行51项自建有限Python投影：地址18、残量/填零12、to方向门8、rt_sigreturn事件5、filldir事件5、残量越域3，全部通过所设断言。另做上述3项原引文的文本行数/连续性核查及4段替代核对。不是作者52例JS重放、原64引文全量机械验收或真实复制测试。
完整本地P9、原facts/deferred全文JSON解析、原64全量字节/定义扫描、Graphviz、预处理/编译/链接、仓库C/ASM/脚本/旧夹具、QEMU/硬件均NOT_RUN。一次可选public raw读取遇DNS失败，没有取得该通道字节；连接器读取正常，不记作GitHub写失败或权限阻断。

## 5. 实际输入和后继

六件及正式评论全文已读。facts首段截断后以301–600及后续601–850、851–1150、1151至EOF1414补齐；evidence分1–330/331至EOF656，另以blob和510–600定点核三段；其余四件全文读取。未把“取得blob身份”冒称全量字节扫描。
本轮新回源：readdir.c中filldir64完整体及相邻入口；uaccess_64.h1–150中全部本任务函数；smap.h全文；alternative.h1–225含相关C宏；asm.h170至末尾；page_const_arch.h1–100；signal_64.c45–99含完整restore_sigcontext。generic access_ok、linux uaccess包装、x86标量宏、rt_sigreturn完整体与类型/金丝雀采用本有效C05已取得的同pin原始内容，不以执行者摘录代替源文件。
28/79及协议v2本轮全文再读；93、62回执和63号写入规则沿本有效会话固定全文复用，既有项目/Mnemosyne行为边界不变；未全量刷新指导或导入其它项目维护。
为64划界新读固定fault.c1至EOF、target_kernel.cmake和其指定kernel.lds全文；12回执的技术限定及原MF12条目定点读取，默认分支搜索仅作导航。64用D00处置R63-01，随后合包核异常表记录、内核访问用户地址时的页故障衔接、alternative元数据与启用链。旧12的VMA/PTE根因不再重号；不预签下一批结果或实际恢复能力。
