---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-PATH-INIT-31-REVIEW-001
record_type: bounded_static_acceptance_with_explicit_consumption_corrections
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: unknown
product_surface: ChatGPT普通聊天
model_per_owner: gpt pro（沿用Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-03
read_channel: connector
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
source_pin_short12: a039d9803ade
reviewed_pr: 17
reviewed_base_short12: e33103ca7d9e
reviewed_head_short12: 45a065270e28
contract54_short12: 867eba5aaff9
required_prior_review_short12: 867eba5aaff9
lead_before_publication_short12: 867eba5aaff9
master_observed_short12: e8e60cd19540
status: BOUNDED_ACCEPTED_WITH_MANDATORY_MAINLINE_SUPPLEMENTS
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
mandatory_supplements: [R31-S01, R31-S02]
acceptance_scope: N00至N04的路径起点和局部调用退出材料；原六件必须合读本回执
kernel_correctness_verdict: NOT_ISSUED
runtime_evidence: NOT_RUN
full_local_P9_gate: NOT_RUN
reviewer_reproduced_executor_80_byte_checks: false
reviewer_parsed_original_JSON_YAML_DOT: false
execution_branch_modified_by_mainline: false
owner_decisions_requested_now: []
next_followup: CORE-USER-LAST-COMPONENT-32
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/55-user-last-component-contract.md
kernel_change_authorized: false
merge_authorized: false
phase3_authorized: false
evidence_class: GitHub固定正文、提交差异、固定源码模型复核；不含运行认证
inputs_read:
  - 54号及30-DOC-FIX-01回执，固定867eba5aaff9，全文
  - 31六件及交付评论6597248342，固定45a065270e28，全文
  - 固定time的10份直接源相关定义及配置金丝雀，范围见正文
  - 05正文固定e33103ca7d9e的1至115及205至315行；仅历史定位
  - 28号全文；既有工作令补充可见段；其它已加载指导沿用
self_check: {scope: this_receipt_only, verified_claims: 12, quotes_reconfirmed: 12, downgraded_to_inferred: 0, method: 模型对本轮连接器固定源码返回文本逐段核对；不是执行方80条自动扫描的复现}
open_questions: [完整本地机械门与运行仍未执行, 下游末分量的生产者及真实标志消费留给第32批]
---

# 31限定接收：起点已核，完整查找和资源正确性没有随之通过

## 1. 裁定与保存身份

按54号及前序闭合回执，以PASS_PENDING_LOCAL接收第31批有界材料，returned_scope=[]。原六件必须合读R31-S01/S02；本回执直接给出局部替代解释，不派一轮整包返工，也不修改原件。第30批组合材料的既有裁定及历史RETURN不变，普通源码问题仍按18号延期。

PR17实际head及执行交付评论均为45a065270e28，open/Draft、未合并。compare e33103ca7d9e至45a065270e28恰1提交，只有user-path-init-31/六件A新增2230行：MANIFEST133、正文81、facts744、DOT168、deferred269、evidence835，旧件零净修改/删除。执行方的双ref全文回读、80条文本检查、JSON.parse及图计数保持作者归属，不改记为主线重跑。

N01的对象/赋值顺序、N02的root/pwd选择和dfd局部未消费、N03两个调用者的实参及空清理体有固定源支持。N04六件及规范字段存在，单定义归属较30原件改善；未发现上轮那种声明/include冒充函数定义的系统性问题。仍存在少数标题超过片段、矩阵类型混写和图注混淆真实门与有效执行前提，按以下明确补正消费，不把原件笼统签成全部P9通过。

## 2. 核心事实的独立回核

先读固定配置和panic金丝雀。配置文本只证明选定快照，不能替代实际构建：
[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
```

set_nameidata先清零，再借入filename指针和保存dfd；这不是复制字符串或取得路径引用：
[VERIFIED mykernel/fs/vfs/namei.c::set_nameidata]
```c
		memset(p, 0, sizeof(nameidata_s));

		p->name = name;
		p->dfd = dfd;
```

path_init的起点选择支持绝对串取root、非斜线首字节取pwd，包含空串模型；所读完整函数未按dfd或LOOKUP位选择起点：
[VERIFIED mykernel/fs/vfs/namei.c::path_init]
```c
	nd->root = current->fs->root;
	if (s[0] == '/')
		nd->path = nd->root;
	else
		nd->path = current->fs->pwd;
```

filename_lookup确有root形参，但实际向set_nameidata传NULL；该错误指针门不是任意指针有效性的证明：
[VERIFIED mykernel/fs/vfs/namei.c::filename_lookup]
```c
	if (IS_ERR(name))
		return PTR_ERR(name);

	set_nameidata(&nd, dfd, name, NULL);
	retval = path_lookupat(&nd, flags, path);
```

完整所选定义还支持：path_s按值复制只是复制mnt/dentry两个指针；restore_nameidata只有注释，terminate_walk为空，complete_walk直接返回0。没有由这些空操作推导全局泄漏、锁安全或物理回收。05正文C4已经明确记录sys_open传dfd=0及path_init不使用dfd，本轮核到原文；PI31-F01应作为旧能力观察的细化，而非独立新根因计数。该能力ID不是UV问题编号。

## 3. R31-S01：返回类型、退出条件及无效输入的统一替代解释

### M16与S06：指针返回不能说成“lookup_last产生err=0”

lookup_last返回const char*；err来自link_path_walk。循环的实际关系为：
[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	while (!(err = link_path_walk(s, nd)) &&
		   (s = lookup_last(nd)) != NULL)
		;
```

仅在循环正常退出且err==0时才调用complete_walk：
[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	if (err == 0)
		err = complete_walk(nd);
```

因此，M16应读为“最后一次link_path_walk返回0且随后lookup_last返回NULL，使循环退出；complete_walk正常返回0；输出指针有效可写”。若末分量返回非NULL编码错误，循环先继续，下一次link_path_walk才转换为整数错误；普通非NULL字符串也是继续条件，不能在此宣称结束，其真实生产者留32。下游不返回时，不主张已到complete_walk、terminate_walk或putname。

输出与局部清零属于到达该分支后的操作，不能仅由空串前缀return0推出：
[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	if (err == 0)
	{
		*path = nd->path;
		nd->path.mnt = NULL;
		nd->path.dentry = NULL;
```
[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	terminate_walk(nd);
	return err;
```

一般helper模型还需输出path不与被清零的nd.path别名，才可进一步说调用者保留了复制前的值；本批真实外层的局部nd和输出实参分开记录。不据此扩大到引用所有权。

### M17、M13与清理路径

M17的“positive path-prefix result”改为明确的“最后一次整数error为0且末分量返回NULL，才到do_open”；不是正整数成功。旧UV-04是非void函数缺少确定的返回值，不等于已证明函数永不返回。两者均阻止某些后续断言，但不能混用。

path_openat的编码错误分支在path_init和该函数尾部terminate_walk之前返回：
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	file = alloc_empty_file(op->open_flag);
	if (IS_ERR(file))
		return file;
```

若此错误正常返回do_filp_open，外层仍会调用其空的restore_nameidata后返回filp。正文“无论何种合法返回”的清理概括须限于实际到达相应尾部的路径。A031的do_open调用有error==0条件，而terminate_walk在到达尾部后并不要求error==0；A070的清零片段也不能单独证明已调用清理。

### M09、M11、M12和标志输入来源

M09的“valid filename but nd.name is ERR_PTR or NULL”应改为“nd本体有效，但nd.name本体无效”，不能同时称filename本体有效。另分name本体有效而字符区无效、current/fs无效；根字段读取先于s[0]，更早无效访问可以阻断后面的字符访问。所选主要调用者的有效对象生产前提与raw helper假设不混用，/bin/init只是M07纸面字符串，不是本机首个程序路径证明。

IS_ERR依数值编码判断，不是NULL或可读性检查：
[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR_VALUE]
```c
	#	define IS_ERR_VALUE(x) (								\
					(size_t)(void *)(x) >= (size_t)-MAX_ERRNO	\
				)
```

在所选定义MAX_ERRNO=4095及目标整数模型下，NULL不被这个谓词当作编码错误；不由此声称真实getname返回了NULL。M15同时组合IN_ROOT/BENEATH/EMPTY只作raw path_init条件例，不能当作经过30校验门的已证用户输入。root覆盖模型需目标及独立输入root有效、set_nameidata和path_init正常完成；不推广为实际非NULL root调用已存在。

以上同时约束正文N02/N03、facts的S03/S05/S06及M09至M18、deferred的PI31-F03/F04/F05和DOT的退出边，不改任何旧源码问题状态。

## 4. R31-S02：引文支持范围、图中真实门及来源字段

原80片段的路径/单定义上下文已逐段对照本轮固定源；以下标题或引用关系必须收窄，不能用标题补出片段没有的语句：

| 原位置 | 必须采用的消费关系 |
|---|---|
| A016及O02/O13/S03 | 返回原s由A017及完整path_init支持，A016单独只有起点分支。 |
| A036 | 调用path_lookupat由该片段支持；putname须合读A037。 |
| A039/A043 | complete_walk实际调用合读A052；IS_ERR检查合读A044，不从仅if或LAST_ROOT赋值推出。 |
| A053 | 标题的“有效name门”改为“编码错误检查门”；正常对象仍是额外前提。 |
| A056 | 是path_openat调用点；path_init内部读取/写入须引用A013至A017，不能改称此片段来自path_init。 |
| A066及O18/M07/PI31-F01 | AT_FDCWD和0实参必须合读A067；A066只含getname_kernel及ERR_CAST前缀。 |
| A068及O10/PI31-F04 | 函数签名和局部声明不支持特殊末分量分支，使用下面实际分支补证。 |
| A070/A041 | 输出和清零、terminate_walk调用、return err分别按本回执S01的真实语句消费。 |
| A078 | “未读dfd”来自完整path_init定义的有界检查，不由这五行独立证明。 |

A068的直接补证，仍只取末分量分派边界：
[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
	if (nd->last_type != LAST_NORM)
	{
		return handle_dots(nd);
	}
```

DOT的sysopen2到dofilp边混合了真实控制门与有效分析前提。实际代码只在getname错误检查之后以新fd>=0决定调用；没有另设“op完整初始化”门：
[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	fd = get_unused_fd_flags(how->flags);
	if (fd >=0)
	{
		file_s * f = do_filp_open(dfd, tmp, &op);
```

故该边替代解释为“实际条件是未走getname错误返回且新fd>=0；本批安全推导另要求op有效”。不能借图注抹掉30::OF30-F01。openexec到doopenexec的!IS_ERR也不是一般有效性门；doopenexec到dofilp对一般调用者还需AT标志检查通过，open_exec传0的所选路径满足该门。图中复用的errptr节点仅为同类谓词图例，filename_s*与char*是不同对象、不同检查时点，不可当成一项共享保护。其它图边仍限本包局部关系，不签88边全量语义认证。

MANIFEST的produced_by按产品面说明消费，精确界面模型名仍unknown，Owner的GPT-6＋Extra选择与后台认证分开。deferred的8条历史链接与facts的9条historical_links是两个范围不同的列表，不相加成独立问题数。五个PI31候选也不是五个已证漏洞或五个新根因。原六件使用本回执替代解释，不追溯覆盖作者历史自查。

## 5. 实际复核方法、限制与后继

完整读54和前序闭合回执；31六件均读完：evidence分1–240/241–480/481–720/721至末尾，facts分1–250/251–500/501至末尾，deferred及DOT分段，正文首次截断后补68至末尾，MANIFEST全文。交付评论正文可见且与Owner提交一致。

本轮固定time回源：namei.c33–196、478–766；path.h与fs_struct_types.h全文；fs.h2515–2530；namei.h1–72；uapi/linux/fcntl.h90–116；open.c241–270；syscall.c38–42；exec.c550–615；err.h全文。配置options_flags.cmake全文及panic.c55–85先于上述源码语义核对。另对下一批预检读namei.c300–477，仅作范围选择，不提前签32结论。

05正文1–115和205–315定点核历史能力及dfd原观察，不重新验收05或采用其其它强断言；30勘误和29限定沿用本C04已经完整读过的固定材料。本轮复读28号；工作令相关补充可见段与尾段已核，其余MyOS2/Mnemosyne规则沿用已加载状态，不宣称全新刷新或导入其它项目任务。

尝试把公开固定evidence下载到临时沙箱作原字节机械核验，DNS失败，未取得原字节文件。实际成功取证均为GitHub连接器；没有据此虚报全文regex、哈希、YAML或DOT解析。Python仅对手工转录的6个短字符串和5类循环返回情形作有限模型核对，不运行C、不认证真实指针或原文件机械计数。作者80/80和全图端点检查不变成主线自动复跑。完整本地P9、编译、仓库脚本、QEMU/硬件及真实用户路径均未执行；不是人类或异族技术审查。

本轮同发55号CORE-USER-LAST-COMPONENT-32，继续现有C03c及GPT-6＋Extra，不轮换。限末分量元数据、两种末分量分派、真实标志来源与指针/整数返回消费；排除完整dcache/挂载/权限审计。Owner转发固定55号并通过预检才启动。旧六件冻结，C04不写执行结果目录。
