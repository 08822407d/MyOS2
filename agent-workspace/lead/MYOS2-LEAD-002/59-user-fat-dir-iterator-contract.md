执行模型：次一档，沿用现有C04a的GPT-6＋Extra。执行位置：原「MYOS2-A-C04a 代码分析执行」，不新建对话或重复交接。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FAT-DIR-ITERATOR-35
record_type: bounded_FAT_directory_iterator_state_and_return_contract
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
required_review_record: CORE-USER-LOOKUP-CALLBACK-34-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LOOKUP-CALLBACK-34-review.md
required_review_binding: 与本59号同一最终固定提交
required_review_supplements: [R34-S01, R34-S02]
prior_acceptance: PASS_PENDING_LOCAL
prior_returned_scope: []
source_ref: time
source_pin_short12: a039d9803ade
result_base_short12: bce8404efe6a
lead_before_publication_short12: a87124c1cf6c
master_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fat-dir-iterator-35/
allowed_files: [MANIFEST.md, fat-dir-iterator.md, facts-and-boundaries.yaml, fat-dir-iterator-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: bce8404efe6a及此前全部文件
executor_rotation: false
external_execution_started: false
activation_gate: Owner向原C04a发送本59号执行指令，固定输入及现场预检通过
phase: 2
runtime_evidence: NOT_RUN
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_change_authorized: false
kernel_patch_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
authorization_ref: Owner当前34验收、28号同轮后继持续授权及C04主线职责
---

# 第35批：目录迭代器的游标、终止条件与错误返回是否真的到达lookup

## 1. 目标与范围

第34批已限定接收，必须合读R34-S01/S02；其iobuf/条目帮助函数曾是opaque，本批只把这段直接边界具体化。核FAT32_iobuf_init的局部状态和函数指针绑定、FAT32_iobuf_iter_init/iter_next、FAT32_get_full_ent、FAT32_ent_empty及FAT32_iobuf_readent的有类型返回，与FAT32_lookup的下一次循环消费对接。

固定主要源：mykernel/fs/fat/myos_fat32_iobuf.c、mykernel/fs/fat/myos_fat32.c、mykernel/fs/fat/myos_fat32_namei.c；字段和常量到fat32.h、msdos相关真实结构定义及err/errno所需定义。new_inode、inode填充、完整分配器、挂载/权限/锁/引用和物理资源回收不扩审；不重做29–34，不修内核、不生成补丁、不编译运行。

get_cluster_chain仅允许确认本函数明示的返回形态及iobuf_init对它的消费；FAT32_read_FAT_Entry、链表实现、FAT簇链走访和驱动仍为opaque。readent允许核实际读请求表达式和传参，但不展开ROOTBLK_TRANSFER之后的驱动、中断、调度或真实磁盘结果。长短名字编码算法、iobuf_write/expand/release全链及创建删除不属于本批。

## 2. I00：输入、继承规则与预检

完整读取本59号和同一固定提交34主线回执。定点读34正文B02/B03、facts::source_conditions和B34-C02/C04/C05/C06/C07/C08、CB34-F01/F02/F03及相关证据；保留R33/R32/R31、30 DOC-FIX及UV-04等历史限定，不改旧ID或重做整批。

沿用C04a已完成接收、MyOS2工作令/公约/协议v2 P1–P13及18/24/28和适用Mnemosyne指导；不重新创建分支/PR，不导入其它项目任务。当前实际新读与继承规则分栏列精确路径/ref/范围，未知产品型号写unknown；不把工具能力等同权限。

先核固定time a039d9803ade的options_flags.cmake和panic.c金丝雀。现场核master/time/lead/执行头、PR16/17、全部开放PR、路径交集、本人最新交付与新目录缺席。首次执行写前预期PR17 bce8404efe6a。主线正常前进不等于执行冲突；未知结果头、同名正式成果、并行写入或固定输入冲突先核归属，不静默换源、覆盖或回退。默认分支搜索只作定位，语义证据必须回固定time。

## 3. I01：iobuf对象和真实迭代函数绑定

完整读取FAT32_iobuf_init局部活动定义，列dir、私有inode/superblock信息、bufsize、max_nr/count、数组、iobuf、iter_cursor/iter_reach_end及iter_init/next字段的来源和初始化顺序。核分配/错误门与各返回，不递归认证底层内存、簇链或回收。get_cluster_chain的NULL/ERR_PTR/普通返回分类须有实际语句，未读到生产者就保持假设，不从函数名补契约。

分别记录“iobuf_init写入函数指针”和“lookup实际调用该指针”两段证据。iter_init的实际返回类型、游标重置及标志写入，须与调用者是否检查返回区分。即使函数指针绑定已证，仍须对象有效且执行到该位置，不能把已赋字段当作所有路径一定完成。

## 4. I02：next/get_full_ent的状态转换及最早无效消费点

完整读取FAT32_iobuf_iter_next、FAT32_get_full_ent及FAT32_ent_empty。按真实顺序登记：读取当前offset得到ret_val、游标增加、终止标志设置、属性/空项判断、do-while短路、最后是否覆盖ret_val、return的实际类别。ret_val/临时cast/游标/终止标志不是同一对象。

检查每次访问之前究竟有何实际门：readent若给ERR_PTR或NULL，get_full_ent是否在读attr前识别；不要把外层lookup的IS_ERR当作内部解引用的提前保护。若已失去定义良好继续条件，停在那里，不推断错误一定传回lookup、必然panic或最终文件结果。

比较最后一个有效条目、最后一个长名/空项、零个条目、提前读失败、已经标end仍调用等有界状态。真实构造/调用能产生的状态与raw helper-only输入分栏；所用sizeof(msdos_dirent_s)、结构字段与常量必须据固定定义，布局不能闭合就保留符号，不靠常见FAT知识硬填32。

## 5. I03：readent边界与回到FAT32_lookup

完整读取FAT32_iobuf_readent：bufend表达式、off负值/上界门、bufidx/bufoff、buffer已存在/缺失、分配结果、请求调用参数、buffer发布和返回地址。保留整数宽度、转换/溢出、bufsize非零、索引范围及整个结构体可读字节的条件；off本身在范围内不能自动证明整个目录项跨界安全。

区分选取buffer数组的下标与读请求选取clusters的下标，以及读请求返回值在本函数是否被消费。宏调用若未闭合实际绑定，只写“源码宏调用及其参数”，不伪造直接驱动调用边。任何不一致都只登记本批有界观察，不运行磁盘、不制作修复补丁或扩审I/O全链。

把具体iter_init/next/get_full_ent结果接回34循环：首次判界失败是否仍有真实生产者、游标上界与iobuf末尾是否是同一条件、get_full_ent是否可能先跨上界再返回、哪些错误实际可到达回调NULL分支。保留旧CB34-F01/F03为条件记录；允许新增有证据的适用范围收窄，但不回写旧件或自动关闭主线裁定。

对于解析器可能改变游标的地方，仅按必要范围定点核cursor写入或声明opaque，不完整审长短名编码。至少形成输入来源—旧游标—读结果类别—新游标/end—getter下一次访问—lookup消费者—停止点矩阵；不从假设buffer内容推出真实目录或文件成功。

## 6. I04：六件、证据、自查、写入和STOP

只新增allowed_files六件。MANIFEST逐I00–I04写complete/partial及精确读取；正文讲实际顺序；facts保存类型/状态矩阵与可达性级别；DOT区分静态绑定、调用、字段赋值、真实门、额外前提与条件结果；deferred用IT35-*并保留旧qualified_id、反证和恢复条件；evidence给单定义体1–5行逐字引文。不设问题或锚点数量配额，不加脚本或第七件。

强制证据：iobuf字段/函数指针类型；构造时状态和回调赋值、实际调用点；iter_init与iter_next所有关键状态/返回；get_full_ent属性访问、短路、终止覆盖和返回；ent_empty判据；readent范围/分配/请求参数/返回；所用结构大小依据、常量、ERR_PTR/IS_ERR/errno。不能用extern/include/注释或标题替代活动关键语句；否定结论需完整定义或明确有界搜索。

规范字段包括produced_by、product_surface、model_per_owner、read_channel、base_snapshot、精确inputs_read或明确分栏、startup_selfcheck_quote、branch_canary_quotes、self_check三计数。图默认属性不计具体节点，计数/端点与语义核查分别记录。P9-1/2/3/4/5/7/8/11/13及其它实际适用项如实记录，未做写NOT_RUN/unchecked；文本匹配不是功能通过。

允许现有临时环境的纯文本/结构/有限整数状态模型，不安装、不运行仓库C/ASM/脚本、不预处理/编译/QEMU/硬件、不用额外付费API模型。唯一执行者CHAT-EXEC-04仅在原claude/dazzling-cori-q0dnyt/PR17的user-fat-dir-iterator-35/写六件及本人进度/交付评论。旧件、master/time、lead/PR16、PR标题正文/Draft及他人评论不改，不新建分支或PR。

使用非强推正常快进和可用expected-head；超时先核远端，不盲目重写。最终固定提交与活动分支分别全文读回六件、核正文/blob/head；compare bce8404efe6a至结果须恰本目录六件A新增、旧件零净改删、没有不明后继。实际短提交及后置核验写本人PR17评论，只申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，然后STOP_WRITING交C04，不自行开始36。

I00–I04内部步骤连续推进，无需逐项Owner确认；真正权限、输入或安全阻断才停相应动作。如写工具不可用，只保全只读成果并精确报告，不冒称提交。阶段2全部禁止边界继续有效：无内核/用户程序/旧成果修改、补丁、构建运行、Owner本机或仓库外libc/镜像、凭据/挂盘、合并关闭、强推删分支、跨仓维护或阶段3。发布不是自动投递或后台启动。
