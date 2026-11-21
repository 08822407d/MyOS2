---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-FAT-NAME-PARSE-36-REVIEW-001
record_type: independent_bounded_review_with_documentation_return
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: unknown
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（沿用Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-04
read_channel: connector
source_pin_short12: a039d9803ade
reviewed_pr: 17
reviewed_base_short12: a7cb8bf8d2e6
reviewed_head_short12: 3c02d6c020e3
reviewed_delivery_comment: 6624419067
contract60_short12: d5b6bf140e03
review35_short12: d5b6bf140e03
lead_before_publication_short12: d5b6bf140e03
master_observed_short12: e8e60cd19540
acceptance_verdict: RETURN
returned_scope: [R36-01, R36-02]
return_scope_kind: DOCUMENTATION_ONLY
core_static_findings: RETAINED_WITH_EXPLICIT_LIMITS_NOT_PACKAGE_PASS
original_artifacts: PRESERVED_UNMODIFIED
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
full_local_P9_gate: NOT_RUN
reviewer_reproduced_original_56_byte_checks: false
reviewer_parsed_original_JSON_YAML: false
reviewer_dot_blob_identity_verified: true
reviewer_dot_check: {explicit_nodes: 45, unique_nodes: 45, edges: 65, undefined_endpoints: 0}
reviewer_independent_finite_models: 26
reviewer_model_scope: 人工按固定源码建立的有限Python字节/整数/顺序模型；不是仓库C或作者20例全量机械重放
self_check: {scope: five_direct_source_quotes_in_this_receipt, verified_claims: 5, quotes_reconfirmed: 5, downgraded_to_inferred: 0, method: 本轮连接器固定源码逐段模型核对}
next_followup: CORE-USER-INODE-INIT-37
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/61-user-inode-init-contract.md
next_task_first_step: N00_R36_DOCUMENTATION_CORRECTION
independent_work_may_proceed_with_prior_RETURN_open: true
closure_authority: MYOS2-A-C04
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-04
executor_rotation: false
execution_branch_modified_by_mainline: false
owner_decisions_requested_now: []
kernel_change_authorized: false
merge_authorized: false
phase3_authorized: false
authorization_ref: Owner本轮独立验收、工作令及18/28号持续授权
---

# 第36批独立验收：限定文档RETURN；第37批合并纠正和独立inode取证

## 1. 裁定与交付身份

正式裁定为RETURN，仅退回R36-01、R36-02两项文档问题；不要求重做两个解析器的整批分析，不要求修改内核。核心静态事实在本回执列出的条件下可保留，但原六件作为整体尚未通过，不使用PASS_PENDING_LOCAL代替这个RETURN。原成果和作者评论冻结，执行者不能自行关闭退回项。

依据18号和28号，AI报告问题不转给Owner作技术决定，也不因此暂停不依赖争议的安全分析。下一工作包37先追加定点纠正，再独立核inode初始化；两个目标同轮交付、同一后继验收回合处理。N00纠正提交不等于关闭RETURN，关闭权仍归C04。

PR17实际head为3c02d6c020e3，open/Draft、未合并；指定评论6624419067对应CHAT-EXEC-04、READY_FOR_MAINLINE_REVIEW与STOP_WRITING。独立compare a7cb8bf8d2e6→3c02d6c020e3为1提交，六件均A新增，共1902行：MANIFEST146、正文82、facts674、DOT114、deferred317、evidence569。无旧件净修改、删除或写区外路径。

主线实际head d5b6bf140e03；direct branches核time a039d9803ade、master e8e60cd19540和执行头一致。已知PR16/17均open，开放列表第3项为空。本次取证不证明其它聊天物理停止或不存在未推送草稿，也不把作者的56/56检查和双ref回读冒认为主线执行。

## 2. 得到源码支持、可有界保留的内容

短名函数主动将输出长度清零，再请求15字节。合法源字节、可写输出及无相关别名破坏的模型下，基名最多8字节，非目录另有点号和最多3字节扩展名，最终计数最多12；无扩展名会撤销点号并显式写0。计数字节、存储次数和首次NUL位置并非同一个量。

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_short]
```c
char *FAT32_parse_short(int *namelen, const msdos_dirent_s *de)
{
	int x;
	*namelen = 0;
	char *ret_val = kzalloc(15, GFP_KERNEL);
```

长名第一遍确实先访问attr，再按短路规则访问空项谓词，最后才测试cursor。错误指针与负偏移不能由表达式末尾的条件追溯保护；此前readent或对象操作不能正常完成时，只保留已证前缀。

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
	while(lde_tmp->attr == ATTR_LONG_NAME &&
		!FAT32_ent_empty((msdos_dirent_s *)lde_tmp) &&
		cursor >= 0)
```

长名第二遍以16位读取结果作筛选，却为每个接受单元只写一个低地址字节。在明确小端字节模型下可发生不同16位值投影为同一输出字节或写入嵌入零。这里不是Unicode/VFAT规范或实际磁盘结论；对齐、别名、类型宽度、输入/分配和生命周期仍是前提。

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
		for(y = 0; y < 10; y += 2)
			if(*(u16 *)(&lde->name0_4[y]) != 0xffff &&
				*(u16 *)(&lde->name0_4[y]) != 0x0000)
				ret_val[(*namelen)++] = lde->name0_4[y];
```

末次for更新仍调用readent，但随后若x<i为假，那个返回值不再由循环正文解引用。它与第一遍错误值直接被attr消费是不同路径。readent内部是否正常返回与最终解析器能否返回仍须分别判定。

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
	for(x = 0; x < i; x++, cursor -= entlen, lde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf, cursor))
```

真实lookup先把namelen置0；两个解析器和已读readent没有直接赋共享iter_cursor/end。比较计数先取int最大值，再转换为size_t；比较后先kfree，再按equal分派。未证深层副作用不能升级为整个调用图无副作用。

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
		int len = namelen > destlen ? namelen : destlen;
		bool equal = !strncmp(name, destname, len);
		kfree(name);
		if (equal)
			break;
```

以上保留不关闭NP36-F01..F08或旧IT/CB/DL/LC/UV/OF源码问题，不按编号计独立漏洞。

## 3. R36-01：证据身份、关联和自查计数须追加纠正

状态：OPEN，DOCUMENTATION_ONLY。60号要求每个直接标记对应一个真实定义体；原件却对下列三条使用56/56单定义体全部合格的口径，不能整体采信。

| 原ID/消费处 | 实际问题 | 要求的追加处置 |
|---|---|---|
| NP36-A032 | 单一标记写LOWERCASE_BASE/LOWERCASE_EXT，片段包含两个独立宏定义 | 拆成两个独立路径::真实宏名的直接证据，不再把斜杠组合当一个符号 |
| NP36-A036 | 标记MSDOS_NAME但片段同时含FAT_LFN_LEN、MSDOS_NAME、MSDOS_SLOTS三定义 | 用MSDOS_NAME自己的单行定义；其它宏仅在实际有结论依赖时独立取证 |
| NP36-A043 | #ifndef是真实选择条件，不是名为strncmp conditional compiler gate的函数或宏定义体 | 保留原始文字为条件编译上下文，不计单定义体VERIFIED；实现正文继续由A044–A046等真实定义支持 |
| NP36-F02/F07的A038/A039关联 | A038是短目录项lcase，A039是短目录项size/类型尾部，不能充当长槽名字区或序号/校验字段的证据 | 对应长槽字段用A040–A042及必要独立补段；行为否定须合读完整解析器，不由字段存在推出已校验 |

上述源字节本身可在固定源码找到，问题是标签单位、证明范围和消费关系，不是指控编造了整段源码。回件要给原56个ID的处置索引，允许未涉及的53条标为保留历史、不重跑，不要求再做一轮全量源码分析。新短引文单独统计；条件编译导航、旧保留条数与本次直接复核计数分开，不能用凑总数维持“原56条全部单定义合格”。

MANIFEST、正文/evidence的总括声称、deferred相关引用和作者评论的消费口径须通过追加勘误统一收窄；不得覆盖原文件或编辑旧评论。关闭条件是纠正件具有真实单定义证据、明确替代关系和诚实计数，经C04独立复核。

## 4. R36-02：图示必须与已证的调用顺序和零写入路径一致

状态：OPEN，DOCUMENTATION_ONLY。原图全文重建后与远端Git blob身份一致，独立锚定正则计数为45个显式且唯一节点、65条边、零未定义端点。数量正确不代表下列控制关系正确；在追加替代图获复核前，不把原DOT当作实际控制流依据。

1. outer节点已写“cursor<L and getter !IS_ERR”，却在true边之后再次进入getter节点。应拆为外层左条件→唯一getter调用→返回后的IS_ERR判断；左条件假不调用getter，右侧错误不进解析正文，不形成逻辑上预知返回结果或重复调用。
2. long_back标签包含cursor减量和readent，随后又连到含readent的long_read；须消除一次回退对应两次读取的歧义，明确一处真实调用及返回后重新求while条件。
3. long_units只有eligible→long_write→long_update路径，遗漏所有13个单元均被过滤时的零次写入路径。必须能表达原C16所述“输出分配NULL、零次存储但其它有效调用正常完成后返回NULL”的条件模型，不通过虚构一次写入让图走通。
4. short_init→short_base的“if output writable”不是源码测试。可写性是分析前提，并且仅在实际存储时必须满足；目录首字节为空格的无写分支不能被这个伪门无条件排除。应明确源码门与分析停止点，并覆盖相应NULL返回/首次非法存储两条互斥路径。
5. lookup的namelen=0声明每次在辅助readent和off赋值之后、空项判断之前执行，不是函数入口一次初始化。long选择门须展开实际IS_ERR、attr和空项条件；long第一遍的cursor检查次序不得被图上的合并节点反转。
6. 同步纠正文档之间的模型口径：C18/C19负长度没有本批正常解析器生产者，分类按helper-only而非泛称source-derived可达输入；F03的“非法存储前缀”与“零写入正常返回NULL后被lookup消费”不能共用一个要求二者同时发生的前提集合。先失效的路径不得补出后继调用。

回件须提供一份可整体替代36原DOT消费的完整DOT代码块及对应原节点/边替代说明，放在37正文的专属勘误节，不新增第七文件。更新矩阵/正文的指定位置通过精确替代文字给出，不重写36原六件。计数、端点与语义检查分开记录；图的核对不扩大为真实执行证明。

## 5. 后继合读的附加条件

- 末尾可用NUL与首次NUL位置不同。长名的0/0xffff测试是跳过单元而非break；例如A、0、B的有限投影仍为AB。短名普通字节模型也可能包含内部零，不应把所有首次NUL都固定在namelen位置；原ASCII例的数值不因此作废。
- 对长名初始长度n和接受单元数K，写入范围是n至n+K-1。非零n不自动代表越界：i=1、n=1、K=13时容量14，写入仍在界内但没有可用的output[namelen]尾字节；n=2、K=13才会在最后一次存储前失去容量前提。真实lookup的n=0仍是已证入口，非零/负数仅helper模型。
- 若零次写入且分配返回NULL，只有其它字段访问和额外readent均正常完成才有正常NULL返回；不能以“没写输出”豁免输入读取。所选strncmp的count0体内不读取字符，也不构成对任意C调用环境或kfree行为的普遍认证。
- 无需新增问题编号配额。F02/F08、F01/F07有依赖，不双计独立根因；本轮不要求Owner决定修复。

## 6. 实际检查、限制及下一步

完整读取固定60号、36六件和评论6624419067；本轮重读35回执115至末尾，覆盖全部R35-S01/S02，前部沿用本C04既有记录。六件分段为MANIFEST1–115/116至末尾，正文1–52/53至末尾，facts1–230/231–460/461至末尾，evidence1–200/201–400/401至末尾，deferred1–175/176至末尾，DOT全文。另重读18、28号及工作令§12已显示选段，不宣称重新加载完整MyOS2或Mnemosyne规则。

固定time回源：options_flags.cmake25至末尾、panic.c67–82；myos_dir.c1–95；myos_fat32_namei.c14–96；fat32.h74–179；uapi/linux/msdos_fs.h1–215；string.c354–376；slub.h160–174；myos_fat32_iobuf.c64–85；err.h1–44；errno-base.h1–22。56个原片段对应9份直接源的上下文已逐段模型对照，已发现的规范问题不能被原作者56/56字节声称抵消。

本轮Python对完整DOT重建文本作Git blob身份验证后计数；另运行26组独立有限字节/计数/顺序模型。使用成功清零、小端字节等显式前提；E=32只用于末次更新顺序例，不是目标编译器sizeof实测。并非机械重放原20例或原56引文，不宣称解析了原JSON/YAML。没有运行仓库C/ASM/脚本、预处理/编译、QEMU/硬件、真实目录/磁盘、VFAT/Unicode规范验证或完整分配/回收/权限审计。本轮没有尝试raw下载，不沿用旧轮DNS失败当本轮事实。

为下一包划界，默认分支search仅定位后，读取固定time的vfs/inode.c1–240、260–430。只据此选定new_inode→alloc_inode→inode_init_always和一次性初始化区别，不预签37结论。61号将R36纠正放N00，N01–N03的inode字段来源取证不依赖争议名字图或三条旧标签，N04一并交回C04。36的RETURN持续至未来明确闭合；本轮只发布任务，不自动投递或启动。
