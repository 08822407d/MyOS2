---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-PATHNAME-COPY-29-REVIEW-001
record_type: bounded_static_review_with_additive_consumption_corrections
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: ChatGPT普通对话；C04主线
model_per_owner: gpt pro（沿用Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-03
read_channel: github_connector
reviewed_pr: 17
reviewed_head_short12: 239e2f8efc45
reviewed_base_short12: 427604c818a5
resume_contract_short12: 2a0a66faa76d
original_contract_short12: 324927712a98
source_pin_short12: a039d9803ade
lead_before_publication_short12: 2a0a66faa76d
master_observed_short12: e8e60cd19540
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
status: BOUNDED_ACCEPTED_WITH_MANDATORY_MAINLINE_SUPPLEMENTS
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
consumption_corrections: [R29-S01, R29-S02, R29-S03]
acceptance_scope: 49号P00至P04的路径取得、复制与局部生命期静态材料；原六件必须合读本回执，不是原件全量机械合规认证
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_reproduced_all_68_quote_checks: false
reviewer_parsed_json_yaml_or_dot: false
human_technical_review_attested: false
execution_branch_modified_by_mainline: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-OPEN-FLAGS-30
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/52-user-open-flags-contract.md
authorization_ref: Owner报告29完成并要求按51及49独立验收；28号验收后同轮安排持续约定；已确认C04主线写权
self_check: {scope: this_receipt_only, verified_claims: 16, quotes_reconfirmed: 16, downgraded_to_inferred: 0, method: 对照本轮实际返回的固定源码逐段人工式文本核对；由模型完成，未运行机械扫描}
---

# 29限定接收：请求长度、数值门和真实访问不可混成同一证明

## 1. 已核交付与裁定

按51号及49号P00–P04，以PASS_PENDING_LOCAL接收本批有界材料，returned_scope=[]；下列R29-S01/S02/S03为强制消费补正。普通代码风险继续按18号延期，旧六件冻结，不另派一轮小型文档返工。这不是把缺陷隐藏成通过：原报告中需要收窄和替换的具体断言在下文逐项列出，严格原件schema/全量机械闸门未被签PASS。

PR17实际头与交付评论issuecomment-6593129232均为239e2f8efc45；PR仍open/Draft、未合并。compare 427604c818a5至239e2f8efc45恰1提交，只有user-pathname-copy-29/六件A新增2114行：MANIFEST103、正文92、facts789、DOT177、deferred230、evidence723；零旧路径净修改/删除。作者声明的双ref全文回读和68段复核仍归作者，本轮没有把它们改记为主线重跑。

P00固定身份及C04返回路线成立；P01两种getname的顺序、本体/字符区分离成立，但失败前提须按S02细分；P02测长先于检查、unsigned转换和条件NUL来源成立，地址门解释须按S01；P03选定调用者的借用/释放顺序可作为有条件局部事实，不认证完整open/exec；P04六件齐备、增量范围成立，证据标签与字段按S03有界修正，不认证所有68引文和105图边均严格合规。

本轮全文读51、49和六件：正文首次长响应后补64至末尾；facts按1–240/241–480/481–710/711至末尾；evidence按1–245/246–485/486至末尾；MANIFEST、deferred、DOT的正文完整可见。28回执R28-S01/S02/S03沿用本C04前轮已完整读取的同一固定材料，不冒称本轮重验28。

独立回源包括：time的options_flags.cmake和panic.c全文；namei.c1–195及720–790；utils.c全文；string.c1–130及462–493；open.c1–285；exec.c550–620、1070–1165、1290至末尾；fs.h2510–2533；err.h全文；syscall.c1–75；uaccess_64.h1–125、asm/uaccess.h1–65、generic access_ok.h全文；slub.h85至末尾；types.h1–200、generic posix_types.h1–95、arch_proto.h90至末尾、linux/lib/string.h1–100；页大小实际包含链及page_const_arch.h全文。后继范围预检另读internal.h1–155、kernel/fcntl.h和openat2.h全文。

历史定点读05主线回执全文及05 deferred前230行（含UV-04），08 deferred首次响应的可见部分并补80–125（US-03及相邻条目）；仅作去重/范围导航，不重做旧批。默认分支PAGE_SHIFT搜索只用于定位，实际结论回固定time。曾尝试include/asm-generic/page.h得到404，随后按真实include路径读include/linux/kernel/asm-generic/page.h，不把错误路径当证据。

## 2. R29-S01：access_ok分支、端点和第二次检查

### 两个源码分支不是一个无条件的区间检查

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::__access_ok]
```c
	static inline bool
    __access_ok(const void __user *ptr, ulong size) {
		if (__builtin_constant_p(size <= PAGE_SIZE) && size <= PAGE_SIZE) {
			return valid_user_address(ptr);
```
[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::__access_ok]
```c
		} else {
			ulong sum = size + (__force ulong)ptr;
			return valid_user_address(sum) && sum >= (__force ulong)ptr;
		}
	}
```

正常strlen返回n时，请求size=n确实没有把终止NUL计入字节数。但慢分支显式检查ptr+n这一端点的数值；稳定字符串模型中它正是源NUL地址。因此PN29-F03、正文和X01/X05中“不包括NUL”只能作为请求长度/名义半开字节区间的说明，不能进一步说数值谓词从未检查NUL地址。快分支只检查起始地址；慢分支检查端点和回绕；两者都不认证页映射、访问权限、规范地址或异常恢复。

[VERIFIED mykernel/arch/x86_64/myos/utils.c::strlen_user]
```c
inline __always_inline long
strlen_user(void *src) {
	ulong size = strlen(src);
	if (!access_ok(src, size))
		return -EFAULT;
```

“先扫描后检查”保持成立，不依赖上述区别。不得用后到的数值门倒推前面的源读取已经安全；PN29-F03保留为条件风险而不是实测故障。

M13及helper_consumer_corrections关于“第二次-EFAULT没有生产者”的排除，必须限定为两次调用使用同一有效数值谓词/已建立相容分支、指针与长度未变、此前正常返回的模型。源码含__builtin_constant_p；本轮和执行者都没有展开实际构建，不能仅凭参数相同证明每个调用点必然采用同一编译分支。作为纯数值区分例，64位模型p=2^63-2、n=3会让起点检查和端点检查不同；这不是可访问用户地址样本，也不证明当前getname真实能走该差异。

因此既不把len<0恒假升级为已发生吞错，也不把有条件的排除升级为所有配置/输入下第二检查恒不失败。PN29-F01的-EFAULT转无符号后走超长门仍是以strlen先正常返回、后门确实产生-EFAULT为前提的本地错误传播模型。

## 3. R29-S02：资源矩阵和调用图须带上真正前置门

### M10首次分配失败分两入口、两条后路

[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
	filename_s *name = __myos_getname();
```
[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
	size_t len = strlen((void *)k_filename);
	if (len > PAGE_SIZE)
		return ERR_PTR(-ENAMETOOLONG);
	name->len = len;
```

getname先过长度门再申请本体；getname_kernel则申请后仍要strlen并经过长度门。因此M10对内核入口还必须要求strlen正常返回且len<=PAGE_SIZE，才走向未检查ERR_PTR的成员访问。若len>PAGE_SIZE，可先返回-ENAMETOOLONG；若strlen不正常返回，成员访问尚未发生。不得说首次分配失败必然立即解引用。已有EI28-F01不关闭、不另造同根ID。

M22“通常分配失败没有filename”也须拆开：第一次本体分配失败不能一般化成被getname_kernel安全返回的-ENOMEM；第二次字符区分配失败才有明确的本体kfree和-ENOMEM出口。M09/PN29-F02限定为首次分配成功后超长早退，本地本体没有释放调用；不推广为PT_INTERP受限来源可达或全局永久泄漏。

### NUL、释放和借用

两入口的len分别用于len+16和len+1，稳定源时复制不含末NUL，目标末零依赖成功的清零分配。slub.h只证明kzalloc传__GFP_ZERO及所读钩子存在memset，未穷尽所有分配后端和物理容量；报告的条件前提不可省略。若复制期间仍有其他写者，不能把strncpy遇到零后余下每个字节都会稳定补零当无条件事实：所选循环会再次读取同一源位置。此前“变短后补零”仅限该后续源字节保持稳定的模型。

所有M17/M18/M20/M23里的“freed/release”只消费为到达对应调用且发起putname/kfree，不是物理回收证明。M21覆盖bprm存在的共同清理，但count/copy等前置错误同样可在未调用bprm_execve时走out_free；到putname还要求free_bprm正常返回。kernel_execve的PF_KTHREAD拒绝发生在getname_kernel之前，不把已取得filename假设套给整个入口。

alloc_bprm的filename->name借用与独立fdpath二选一不能合成所有调用都会分配fdpath：本批sys_execve、kernel_execve向公共流程传AT_FDCWD，走前一种；fdpath支路只是扩展helper模型。free_bprm并不直接putname；其内部对象/文件释放未在本批闭合。

图按下列限定消费：strlenuser的count允许0而非仅positive；doOpenExec到do_filp_open还需flags门通过；tmpExec到bprmFilename只表示name分支的借用，fdpath分支的数据来源不同；strncpy真正写字符区，namebuf到strncpy的数据边不替代写入关系。图不是穷尽控制流或105边全部独立认证。

### 打开标志的前缀没有被本批认证

[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	open_flags_s op;
	int fd = build_open_flags(how, &op);
	filename_s *tmp;
```
[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	fd = get_unused_fd_flags(how->flags);
```

在这两次赋值之间确有getname/IS_ERR门，但没有消费build_open_flags错误的返回门。本批“正确处理getname错误指针”不等于整个open输入检查正确。其输入来源、可达错误和op已初始化字段留第30批核，不在29中预判完整后果。05::UV-04具体是do_open和myos_do_dentry_open的缺return；本轮读取的do_filp_open自身有return filp，不可把简称误解成该外层函数缺return。

## 4. R29-S03：锚点和MANIFEST的直接补正

A019的#ifndef与注释是定义外配置导航，不是strncpy函数体证据；A030的“uaccess header”不是唯一符号定义；A068是声明而非定义。三者降为导航/声明证据，函数语义分别以实际函数体A020–A022及utils.c为准。A023含函数外注释尾，使用A024或下列纯函数体片段，不把原片段整体算严格P2通过。

[VERIFIED mykernel/lib/string/string.c::strlen]
```c
		for (sc = s; *sc != '\0'; ++sc)
			/* nothing */;
		return sc - s;
```

A031的宏正文可用，但外围#ifndef/空行不是宏定义体；A037没有闭合宏，补实际三行定义：
[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR_VALUE]
```c
	#	define IS_ERR_VALUE(x) (								\
					(size_t)(void *)(x) >= (size_t)-MAX_ERRNO	\
				)
```

A013标题包含“正常返回”而片段只有假想错误出口；正常返回另补：
[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
	return name;
}
```

M11的第二次分配失败应引用含-ENOMEM的实际分支，不用属于复制错误分支的A007代替：
[VERIFIED mykernel/fs/vfs/namei.c::getname]
```c
	name->name = kzalloc(len + 16, GFP_KERNEL);
	if (name->name == NULL)
	{
		kfree(name);
		return ERR_PTR(-ENOMEM);
```

M22的ERR_CAST不能只用PTR_ERR的A035证明，补真实定义：
[VERIFIED mykernel/include/linux/kernel/err.h::ERR_CAST]
```c
		ERR_CAST(const void *ptr) {
			/* cast away the const */
			return (void *) ptr;
		}
```

A067只显示中间typedef，必须补全size_t至unsigned long链；PAGE_SIZE=4096也不能由不含该定义的A031单独支持：
[VERIFIED mykernel/include/linux/kernel/types.h::size_t]
```c
			typedef __kernel_size_t		size_t;
```
[VERIFIED mykernel/include/uapi/asm-generic/posix_types.h::__kernel_ulong_t]
```c
		typedef unsigned long	__kernel_ulong_t;
```
[VERIFIED mykernel/include/uapi/asm-generic/posix_types.h::__kernel_size_t]
```c
			typedef __kernel_ulong_t	__kernel_size_t;
```
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	#define PAGE_SHIFT				12
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```

A045的前置注释不是sys_open定义体；实际宏调用及其返回do_sys_open的函数体已本轮核读。A055“不包含putname”不能由首5行证明，本轮完整free_bprm支持该有界否定。通用strlen/strncpy仍按所选定义及宏前提消费，未通过实际构建排除一切其他绑定。

MANIFEST用了startup_rule/source_canaries/self_checks而非协议的startup_selfcheck_quote/branch_canary_quotes/self_check，且缺少独立produced_by及inputs_read标准键。这是元数据与机械消费缺口，不把同义键冒充字面符合。主线追加消费映射：startup_rule为已读公约原句；source_canaries为本轮也核到的固定time两行；执行者为C03c/CHAT-EXEC-03，产品来源按交付记录为ChatGPT普通聊天，GPT-6与Extra仅为Owner选择记录；实际输入清单以其MANIFEST正文及本回执读取清单分别归属。68、68/68、图计数和JSON.parse仍是作者自查声明；不伪造缺失的标准字段或主线机械复现。严格按原MANIFEST键读取的自动消费者须视为不兼容，不能据本限定接受签P6/P9全PASS；后续30须恢复规范字段。原件不改，补正范围不修改协议。

## 5. 结论、限制与后继

四项PN29-F01至F04按上述前提继续登记，不把四项称四个独立漏洞；EI28-F01/F05和UV-04保持原qualified_id与历史限制。无全仓去重、内核修复、攻击/用户指针样本、编译、预处理、C/ASM、仓库脚本、QEMU、硬件、外部libc或额外模型API运行。未解析两份JSON兼容YAML或DOT，未机械复跑68引文、24资源模型、10算例；本回执16个新增锚点为模型对实际源码返回文本的核对，不是人类技术验收。

C03c现完成25至29五包、30件、10561净新增行；不是token余量或产品上限。本轮不轮换执行者。下一批CORE-USER-OPEN-FLAGS-30按52号核open_how/open_flags转换、返回消费与局部接口，不重做29/ELF/完整路径遍历或文件引用。原29成果交付后停写，30只有Owner向现有C03c转发固定52号且现场预检通过才启动。主线仍C04，原PR16/17保留，阶段2不变。
