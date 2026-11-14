执行模型：次一档，沿用现有C04a的GPT-6＋Extra。执行位置：原「MYOS2-A-C04a 代码分析执行」，不新建对话或重复交接。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FAT-NAME-PARSE-36
record_type: bounded_FAT_short_long_name_buffer_length_and_cursor_contract
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: unknown
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-04
read_channel: connector
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
mainline_owner: MYOS2-A-C04
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-04
executor_display_name: MYOS2-A-C04a 代码分析执行
execution_model_tier: next_tier_default
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
required_review_record: CORE-USER-FAT-DIR-ITERATOR-35-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FAT-DIR-ITERATOR-35-review.md
required_review_binding: 与本60号同一最终固定提交
required_review_supplements: [R35-S01, R35-S02]
prior_acceptance: PASS_PENDING_LOCAL
prior_returned_scope: []
source_ref: time
source_pin_short12: a039d9803ade
result_base_short12: a7cb8bf8d2e6
lead_before_publication_short12: 98608f6c58df
master_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fat-name-parse-36/
allowed_files: [MANIFEST.md, fat-name-parse.md, facts-and-boundaries.yaml, fat-name-parse-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: a7cb8bf8d2e6及此前全部文件
executor_rotation: false
external_execution_started: false
activation_gate: Owner向原C04a发送本60号执行指令，固定输入及现场预检通过
phase: 2
runtime_evidence: NOT_RUN
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_change_authorized: false
kernel_patch_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
authorization_ref: Owner当前35验收、28号同轮后继持续授权及C04主线职责
---

# 第36批：长短名字的字节缓冲、长度输出及游标消费

## 1. 范围与目标

35已限定接收，须合读R35-S01/S02。35将名字解析器保留为边界，本批具体核FAT32_parse_short、FAT32_parse_long以及FAT32_lookup对返回指针、namelen、共享游标和临时缓冲的消费，回答“名字怎样构成、长度怎样产生、哪些访问/终止/返回有真实条件”。不重做35迭代器全批，不做完整VFAT标准符合性或Unicode规范审计。

主要固定源为mykernel/fs/fat/myos_dir.c中的两个解析函数，myos_fat32_namei.c::FAT32_lookup的直接调用/比较/释放，必要的msdos_dirent_s/msdos_dirslot_s、fat32.h宏和string.c::strncmp。readent仅消费35回执及必要定点定义；分配器、磁盘、FAT簇链、创建删除、完整VFS/权限/锁/引用和物理资源回收保持边界，不修内核、不生成补丁、不编译运行。

## 2. P00：固定输入、规则与现场

完整读取本60号和同一固定提交35回执。定点读35正文I02/I03、facts的getter/readent及两个上界、IT35-F01/F02/F05、R35全部补正；34的名字比较、B34-C09及R34-S01第4/5条继续合读。33/32/31、30 DOC-FIX、UV-04/OF30等旧限定保留，不覆盖历史条目。

沿用C04a既有接收及已加载MyOS2工作令、公约、协议v2 P1–P13、18/24/28及适用Mnemosyne约束。实际新读与继承指导分栏写精确路径/ref/范围，不把概括清单当本轮重新加载；未知后台身份写unknown。保持现有执行身份，不重演创建PR或接收仪式。

先核固定time a039d9803ade的options_flags.cmake与panic.c金丝雀，再核master/time/lead/执行头、PR16/17、开放PR及写区交集、本人35交付评论、新目录不存在。首次执行写前预期a7cb8bf8d2e6。未知执行后继、同名正式成果、并行写入或输入冲突先核归属，不强推覆盖、回退或静默换源；主线合法前进本身不是执行冲突。默认分支搜索只用于定位，语义回固定time。

## 3. P01：短名的长度生产、写入上界及返回

完整读FAT32_parse_short。逐语句核namelen输入/归零、15字节分配、8字节基名循环、空格终止、lcase分支的实际数值操作、目录与非目录分支、点号添加、扩展名循环、无扩展名的回退写0、最终return。用实际分支而不是惯用8.3知识替代证据。

分别记录分配容量、已写字节数、*namelen计数和首次NUL位置；终止若依赖kzalloc清零，必须保留底层满足该请求的前提。分配失败后是否必然立即写坏地址，要结合具体分支；无写入便返回NULL的helper模型与实际lookup可达性分开。代码缺检查不等于所有输入都已复现崩溃。

有界示例至少覆盖：无扩展名、带扩展名、目录、基名/扩展名长度边界、lowercase位设置、空格开头或人为空名的helper模型。对字符加32只记录源码和所声明字节模型，不自动认证对所有字符正确的大小写转换或名字规范。

## 4. P02：长名的两遍扫描、长度与访问顺序

完整读FAT32_parse_long。建立i、局部cursor、共享iobuf.iter_cursor、entlen、lde_tmp/lde、返回缓冲和*namelen的身份；核初始回看位置、第一遍属性/空项/cursor条件的求值顺序、id位0x40终止、cursor回退和readent返回使用。不要用位于后面的cursor>=0或外层lookup的检查来保护更早的字段访问。

逐项核第二遍kzalloc(i*13+1)、cursor重置、for条件与更新表达式、三个名字字节区及16位0/0xffff判断、实际复制到char缓冲的值、*namelen递增和最终return。区分“两个字节组成的值用于筛选”和“实际复制了几个字节”，保留字节序、对齐/别名、类型宽度与可读范围前提；不声称完成Unicode或VFAT合规验证。

特别区分解析器自身是否初始化*namelen与真实FAT32_lookup调用前是否置0；非零初值只能按有生产者的调用或helper-only模型消费。i*13+1、int计数、非负长度及容量范围的推导须列前提，不从kzalloc函数名替代成功或清零证据。

for更新表达式中的最后一次回退/readent也须按C求值顺序记录：函数被调用不等于返回指针随后一定被解引用。错误值已产生但未再被消费、错误值马上被读字段、读取函数内部先失效、没有正常返回，四者分开。只核必要的readent边界，不进入磁盘驱动或名字写入算法。

## 5. P03：直接调用者的比较、释放与共享状态

完整核FAT32_lookup的选择长短名条件、namelen=0、name赋值、max(namelen,destlen)、strncmp、kfree(name)与if(equal)顺序。name指针、namelen输出、destlen窄化和strncmp计数的合法性分别记录；第三参数实际为strncmp的size_t，不能把单个负长度等同最终count为负。不得跳过kfree是否正常返回就声称已按equal分派。

核两个解析函数对共享iobuf.iter_cursor/end是否有实际写入，区分仅操作局部cursor和通过readent可能更新缓存。结论必须依据所选完整定义和直接callee，不能由变量同名推断共享状态改变；将结果有界接回R35-S01的内部跨上界与下一轮上界模型，不扩全部目录遍历。

建立调用者输入—解析分支—缓冲容量/实际字节—namelen—NUL—返回类别—比较/释放—共享状态—最早停止点矩阵。非ASCII、嵌入零、缺终止标志或非法slot只作显式字节模型；实际目录是否产生它们未证就标helper-only，不制作真实磁盘破坏样本、不声称实际文件误开。可补充CB34-F03、IT35相关限制，但不自动关闭旧ID。新发现用NP36-*，不设发现数量配额。

## 6. P04：六件、证据、提交与STOP

只新增allowed_files规定六件：MANIFEST登记P00–P04与真实输入；正文讲顺序和停止点；facts保存对象和有限矩阵；DOT分清调用者/对象数据、实际控制门和分析前提；deferred保存反证/旧ID/恢复条件；evidence以一个真实定义体内逐字1–5行支撑各标题，完整语义合读全函数。

强制锚点包含两个解析器入口、分配、计数初始化或调用者来源、循环条件/更新、关键字符写入、终止和return；lookup的实际分派、max/strncmp/kfree/equal；相关packed字段与宏定义、所用strncmp活动定义及条件选择；依赖readent或错误编码的地方给实际定义，不以extern/include/注释冒充。否定结论只限已读完整定义或明确有界检索。

规范保留produced_by、product_surface、model_per_owner、read_channel、base_snapshot、精确inputs_read分栏、金丝雀、startup_selfcheck_quote及self_check三计数。图默认属性不计节点，计数/端点和语义分开；不得沿用35的对象call边或上界退出重新进入循环体的错误边。P9-1/2/3/4/5/7/8/11/13及其它实际适用项如实记录，未做写NOT_RUN/unchecked。

仅允许现成临时环境的纯文本/结构/有限整数或字节模型；不安装、不运行仓库C/ASM/脚本、不预处理/编译/QEMU/硬件、不用额外付费模型/API。CHAT-EXEC-04只向原claude/dazzling-cori-q0dnyt/PR17的user-fat-name-parse-36/写六件及本人评论。全部旧件、master/time、lead/PR16、PR标题/body/Draft及他人评论不改，不创建竞争分支/PR。

正常非强推快进，使用可用expected-head；超时先回读远端，不盲目重写。最终固定提交和活动分支分别全文回读六件并核正文/blob/head；compare a7cb8bf8d2e6至结果须仅本目录六件A新增、旧件零净改删且无不明后继。实际短提交及后置核验记本人PR17交付评论，只申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，然后STOP_WRITING交C04，不自行开始37。

普通P00–P04内部步骤连续推进，真正权限/输入/安全阻断才停相关动作；写工具不可用则保全安全材料并准确报告，不伪称提交。阶段2所有边界保持：不改内核/用户程序/旧成果、不出补丁、不操作Owner本机或仓库外libc/镜像/凭据/挂盘、不合并关闭/强推删分支、不跨仓维护或进入阶段3。发布任务不等于自动投递或后台启动。
