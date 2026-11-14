---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-DIRECTORY-ENUMERATION-59-REVIEW-001
record_type: independent_bounded_static_acceptance_with_mandatory_supplements
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: unknown
model_per_owner: gpt pro（沿用Owner最近明确报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-08
base_snapshot: time
read_channel: GitHub_connector
source_pin_short12: a039d9803ade
contract89_short12: 669231eb8e5e
reviewed_base_short12: 8de2a76f5f73
reviewed_head_short12: 2b3aec6f71e1
reviewed_delivery_comment: 6720757981
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
mandatory_supplements: [R59-S01, R59-S02, R58C-S01]
D00_disposition: CLOSED_BY_SEPARATE_58_RECEIPT_WITH_B25_REVIEWER_OBJECTION_WITHDRAWN
subjects: {A: ACCEPTED_IN_SCOPE_WITH_LIMITS, B: ACCEPTED_IN_SCOPE_WITH_LIMITS, C: ACCEPTED_IN_SCOPE_WITH_LIMITS}
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original72_automatic_byte_scan: NOT_RUN
original_JSON_parse_replay: NOT_RUN
reviewer_finite_model_cases: 43
reviewer_text_checks: 4
graph_blob_short12: 5ee60a434bbb
graph_bytes: 5783
graph_nodes: 54
graph_edges: 74
graph_undefined_endpoints: 0
graph_text_blob_match: true
self_check: {verified_claims: 3, quotes_reconfirmed: 3, downgraded_to_inferred: 0, scope: 本回执三段短引文人工回源，不是原72条自动闸门重放}
next_followup: CORE-USER-FS-CONTEXT-CWD-60
next_task_ref: ../90-user-fs-context-cwd-contract.md
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
phase: 2
owner_decisions_requested_now: []
---

# 第59批独立验收：目录枚举三专题限定接收

## 1. 裁定和实际交付

依据固定89号、协议v2及六件原件，59裁定PASS_PENDING_LOCAL，returned_scope为空。A入口/fd/返回、B FAT迭代/位置、C记录容量/写入/部分结果均限定接收；不是运行正确性、完整P9、ABI符合性或漏洞复现认证。
D00单独由58闭合回执裁定：B24已纠正，主线对B25的空行/6行指控撤回，58组合材料PASS_PENDING_LOCAL。59继承旧指令的错误B25描述必须合读R58C-S01，不作为新RETURN，也不能继续当作执行者真实缺陷。
执行compare 8de2a76f5f73→2b3aec6f71e1：ahead1/behind0，一提交，仅user-directory-enumeration-59六件A新增3582行（MANIFEST88、正文109、facts2151、evidence767、DOT145、deferred322），旧件零净修改/删除。
PR16/直接lead=669231eb8e5e，PR17/直接执行=2b3aec6f71e1；均Open/unmerged，17 Draft。direct time=a039d9803ade，master=e8e60cd19540；开放列表第三项为空。正式评论6720757981声明STOP_WRITING和PENDING，作者没有自签验收。主线不认证另一聊天所有内部读取、计数或后台模型。

## 2. 核心证成

A：所读系统调用宏调用体直接分派iterate_shared，而不是调用带方法存在性检查的iterate_dir。
[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c
		buf.ctx.pos = f.file->f_pos;
		error = f.file->f_op->iterate_shared(f.file, &buf.ctx);
		f.file->f_pos = buf.ctx.pos;
```
myos_fdget_pos直接读fd_array且返回flags=0，没有局部取引用。只有物理/逻辑输入与对象合法的NULL槽才可安全走-EBADF；越界索引、无效file或f_op/方法缺席不继承独立iterate_dir的-ENOTDIR。fdput_pos调用fdput，但当前flags=0不走fput；旧41及08::US-01身份不变。
B：FAT入口对pos==-1直接0，其余位置新建iobuf并从cursor0开始，不按正位置定位。actor后三个元数据实参固定0；正常返回后先kfree(name)，再按错误退出：
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_getdents64]
```c
		error = ctx->actor(ctx, name, namelen, 0, 0, 0);

		kfree(name);
		if (error !=0)
			break;
```
只有error==0才置ctx.pos=-1。先交一条、再容量拒绝时，上层可返回已记账字节而位置未推进；在稳定目录、有效对象及相同安全actor序列下，下次可重复。IT35末项覆盖/错误指针和解析器的早停边界仍在，不能由抽象两条记录假定真实盘面两条都能到actor。
C：filldir64先计算reclen，再做名字/容量检查；直接字段store与name的copy_to_user保护范围不同。copy返回值未作条件消费，后续NUL和记账仍存在：
[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	buf->prev_reclen = reclen;
	buf->current_dir = (void *)dirent + reclen;
	buf->count -= reclen;
	return 0;
```
首条prev==current会写本条d_off，后续条目只写前条d_off；两条及以上的最终尾条缺本函数的最终赋值，系统调用尾部修补仍是注释。不能据此推尾字段实际为0或某个随机值。buf.error在成功record后可仍为-EINVAL；prev_reclen非零时最终字节数覆盖该错误或callee负值。

## 3. R59-S01：对象、类型和进度的强制限定

1. 表内NULL槽、逻辑范围、物理数组边界和file存活是不同前提。fd由unsigned传int，超范围不选定实现结果；flags0路径不代表整个生命周期无其它引用。DE59-F01是枚举消费者记录，不把41早已记录的myos_fdget_pos同一事实重复算新根因。
2. iterate_dir只检查方法，不保证f_op非NULL；实际入口连该方法门也没有。FAT32_file_ops静态绑定不证明某次file真实选中该表。方法正常返回且后置对象仍有效，才可回写f_pos并推最终返回。
3. 非-1位置均走新iobuf；正数位置、其它负数位置只在有效输入模型中讨论，不把reset等同正常盘读取。getter必须安全返回终止才能令error保持0；先发生attr错误指针消费时不能补EOF。辅助readent/解析、name释放、iobuf_release均须正常完成；release存在不是运行成功证书。
4. offset19和align8明确是普通x86-64布局模型，未编译认证。offsetof有条件分支、ALIGN委托下层宏；不按任意精度模型掩盖C类型提升。count超int范围及极端namelen只记实现/范围边界，不把“计算在检查前”直接升级成必然溢出或运行漏洞。
5. 有效有限模型中name长1/4→24字节、5/12→32、13→40；capacity不足可在用户字段store前拒绝。零count并不保证不遍历FAT；只有actor的容量门或pos==-1等真实门决定后续动作。
6. copy非零余数后仍记账的命题要求更早直接store合法、copy正常返回且后续NUL/store也合法。首次无效访问后不再推安全errno或成功字节数。verify的name读取也可能比用户store更早失效；内嵌NUL未拒绝只限所读验证函数，不证明所有name可达。
7. facts中visible_bytes/emitted只表示数学模型的计账值，不是已观察的用户可见完整字节。填充区和尾d_off未必由函数写入；copy成功与记录所有字节初始化是两件事。部分字节覆盖错误不是单独已证缺陷，具体重试/位置影响以DE59-F03等前提消费。
8. DE59-F01至F07为七条条件记录，不是七个全新独立根因；IT35、FG41、BI55及EI28/PN29/NG58原ID与真实状态保持。DE59-U07中“待关闭R58”由本轮关闭替代，但未全核历史85条和未做完整P9的限制不随之消失。

## 4. R59-S02：证据、图及主线自查范围

72个证据标签全文阅读，强制组和关键语义回源核查；本轮不声称全72条自动逐字/定义闸门重放。D00三锚按R58C-S01计作两条宏纠正与一条原本有效B25确认，不能沿用“两条缺陷”的计数。实际新技术69标签与历史58的87标签分开。
图全文重建5783字节，Git blob与5ee60a434bbb对应完整远端身份一致；54唯一节点、74边、0未知端点，未运行Graphviz。图是关系摘要，不是完整CFG：direct→fat假设回调绑定；fdput→fput只在标志位条件满足；localerr→markend只在error==0；eofpatch是被注释步骤的信息节点而非执行动作；aux聚合调用位置，不是新增真实函数；ANALYSIS_STOP不是CPU指令或错误返回。
主线实际运行43个自建Python有限模型（size10、trace16、two_call4、offset4、fdput_flag4、return5）及4项B25文本检查，共47项通过。不是作者46个JS案例重放。原facts/deferred全文JSON解析、全72自动字节扫描、Graphviz、完整本地P9、仓库C/ASM/脚本/旧夹具、预处理/编译/链接、QEMU/硬件及真实用户缓冲均NOT_RUN。

## 5. 实际输入与后继

六件全文读取：facts1–320/321–670/671–1020/1021–1370/1371–1760/1761–2151，evidence1–240/241–500/501–767，deferred1–190/191–322；正文1–85截断由75–109补齐，MANIFEST与DOT全文，正式评论全文。固定源新读readdir.c全文、file.c140–225、file.h1–100、uaccess.h140–215、dirent.h全文、fs.h850–915及1795–1905、iobuf.c全文、myos_dir.c1–95、stddef.h1–28、align.h/limits.h全文、options_flags全文/panic68–87。FAT枚举/getter和namei宏沿本同一有效对话此前实际固定源原文复用；不是用作者引文代原始源码。
41回执全文含尾部补读，41 deferred1–150定点核FG41字段。35/36C/38/55/56及EI28/PN29限制沿既有固定回源范围使用，不重验旧批。89、58回执、28/79/协议v2及项目规则和适用Mnemosyne指导按本同一有效会话已读原文复用；63本轮全文再读，不声称新一轮全量指导刷新。
后继90号60包由同一C05a执行，覆盖工作目录/根目录更新、getcwd输出及taskfs复制共享退出三个相关专题；仅设计发布，不由C05代执行或自动投递，不修内核。
