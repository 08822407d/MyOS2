接收模型：次一档（默认），建议 GPT-6＋Extra。执行位置：MyOS2辅助分析Project新普通聊天「MYOS2-A-C04a 代码分析执行」。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-DENTRY-LOOKUP-33
record_type: bounded_dentry_lookup_allocation_and_callback_contract
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: unknown
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-03
read_channel: connector
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
mainline_owner: MYOS2-A-C04
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-04
executor_display_name: MYOS2-A-C04a 代码分析执行
source_ref: time
source_pin_short12: a039d9803ade
result_base_short12: d6444512a213
lead_before_publication_short12: bf856f2ddb2d
canonical_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dentry-lookup-33/
allowed_files: [MANIFEST.md, dentry-lookup.md, facts-and-boundaries.yaml, dentry-lookup-paths.dot, deferred-findings.yaml, evidence.md]
prior_review: CORE-USER-LAST-COMPONENT-32-REVIEW-001
mandatory_supplements: [R32-S01, R32-S02, R31-S01, R31-S02]
prior_review_verdict: PASS_PENDING_LOCAL
return_items: []
executor_rotation: true
old_executor_future_delegation: NO_BATCH33
activation_gate: Owner转发本固定任务入口给新CHAT-EXEC-04，接收/规则/现场预检通过
phase: 2
runtime_evidence: NOT_RUN
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_patch_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
---

# 第33批：目录项名字匹配、分配、慢查和回调返回契约

## 1. 范围

32已把末分量的__d_lookup和__lookup_slow当作下游边界，本批只向内追这两个查找函数及__myos_d_alloc的定义和直接消费者。目标是查明名字匹配、对象/字符缓冲初始化、NULL/ERR_PTR/普通返回值、回调结果与原对象副作用如何对应。

不重做29–32，不完整展开FAT目录扫描、挂载、符号链接、实际盘I/O、完整权限、RCU/锁、文件引用/全局生命周期或创建/删除/重命名效果。不修内核、不生成补丁、不编译运行。普通发现延续18号延期机制。

## 2. D00：新对话接收、规则加载、固定输入

首先完整读取57/00交接包和57/01启动指令、56号任务书及本次32回执；Mnemosyne接收报告先于实质执行，MyOS2项目规则与Mnemosyne适用指导必须分开加载。沿用工作令、公约、协议v2 P1–P13、18/24/28和45号轮换历史中适用的通用纪律；不重做历史创建PR仪式。

固定time a039d9803ade。先实读mykernel/scripts/options_flags.cmake和mykernel/debug/panic.c两个金丝雀，核当前master/time/lead/执行头、PR16/17、所有开放PR、写区交集、PR17最新交付评论、repo读/写能力与本批目录不存在。预期首次写前执行头d6444512a213；主线正常推进不等于执行冲突，未知结果头/同名正式成果/并发写先停写核归属。搜索默认分支仅用于定位，语义回固定time。

前置消费：32的MANIFEST、last-component.md L02/L03、deferred LC32-F01..F04与相关证据；本轮32回执R32-S01/S02；31回执R31-S01/S02。30材料须合读e33103ca7d9e的DOC-FIX勘误；旧UV-04、OF30-F01等问题不因文档接收而自动关闭。05基线及deferred只定点查旧目录项/路径条目，不进行全仓去重。

## 3. D01：__d_lookup名字匹配

完整读取mykernel/fs/vfs/dcache.c::__d_lookup、mykernel/include/linux/fs/dcache.h中qstr_s/dentry_s字段，以及mykernel/lib/string/string.c::strncmp活动定义、需要的宏/选择条件。核parent、name、父目录d_subdirs链表、候选dentry、查询name->len和候选d_name.len、strncmp的三个参数、匹配赋值及break、未命中return。

必须区分“查询长度内相等”和“完整名字等长相等”；以活动源码而非POSIX/Linux惯例裁定。有限纸面例：查询a与候选ab、反向、同名、首字节不同、空qstr、多个候选的顺序。空qstr及人为链表必须标helper-only；指针、字符串和长度有效是前提，不推断真实用户打开了错误文件。

区分返回NULL、候选普通指针和本函数是否有错误指针生产者；非NULL dentry不代表inode有效、已加引用或权限检查完成。不完整扩审链表/锁/引用。

## 4. D02：__myos_d_alloc身份、赋值、失败回收

完整读取dcache.c::__myos_d_alloc实际活动体，核两次kzalloc、每次失败门、kfree对象、名字缓冲长度/复制、d_name.name/len及d_flags/d_inode/d_parent/d_sb/d_op/链表字段初始化、最终返回。注释中描述的NULL不能覆盖实际ERR_PTR返回。

独立区分dentry对象、名字缓冲、调用者借入的qstr。只在源/长度/目标和分配成功前提下讨论memcpy与结尾NUL；不跨过长度溢出、无效内存或不确定下游继续推断。可定点核d_alloc包装的直接检查作对照，不开展分配器或物理回收的完整审计。

## 5. D03：__lookup_slow回调、对象副作用及上层消费

完整读取mykernel/fs/vfs/namei.c::__lookup_slow。逐语句核__myos_d_alloc结果检查、dir->d_inode->i_op->lookup调用参数和返回值、NULL分支的kfree/ERR_PTR(-ENOENT)、后续dentry->d_inode/i_mapping读取、nrpages/page_array设置、链表发布、d_parent写入及返回对象。

严格区分：传给callback的原始dentry、callback的返回指针值、callback对原始dentry字段的副作用。以“返回同一对象/另一个对象/NULL/ERR_PTR”作为条件模型，不将未证明的回调生产者当作实际运行路径。即使callback返回非NULL，也不能直接推出原dentry已有合法inode。必要时到fs.h定位inode和lookup的实际类型及回调绑定；无合法来源则留opaque。

定点回查上层walk_component/open_last_lookups，区分快查NULL触发慢查与慢查自身返回NULL。对LC32-F03补充真实返回契约后，明确它在本次所读定义良好路径下是否仍有NULL生产者；历史条目不覆盖、只是追加条件边界。对回调编码错误未被正确消费/字段可能未赋值的路径，停止于最早无法定义良好继续的位置，不编造实际CPU或访问效果。

形成有类型的输入—分配—回调—对象字段—上层使用—停止点矩阵，区分真实代码门、分析前提和不可达/未证情形。

## 6. D04：六件、证据、自查及提交

只新增MANIFEST.md、dentry-lookup.md、facts-and-boundaries.yaml、dentry-lookup-paths.dot、deferred-findings.yaml、evidence.md六件；不增加回执PR、脚本或第七件。MANIFEST登记接收/规则刷新、准确身份、model_per_owner、produced_by（未知则unknown）、read_channel、固定输入、配置/公约金丝雀、self_check三计数、P9适用性。

每条VERIFIED只锚固定time的一个真实函数/宏/结构定义及逐字1–5行，片段含标题声称的关键语句；长定义分多段，不把include/extern/注释冒充定义体。完整读所选函数来判断否定断言，不由短引文推导全仓不存在。强制证据涵盖__d_lookup的循环/比较/返回、strncmp所选定义、__myos_d_alloc两次分配与失败/初始化/返回、__lookup_slow分配/回调/失败/字段/发布/返回、qstr/dentry关键字段、上层消费者、ERR_PTR/IS_ERR和相关errno。

按P9-1/2/3/4/5/7/8/11/13及其余实际适用性检查；未执行写NOT_RUN/unchecked。可用现有纯文本/数值/结构工具，不运行仓库C/ASM/脚本/预处理/编译/QEMU/硬件，亦不调用额外付费模型/API。

新执行者仅可向原PR17分支claude/dazzling-cori-q0dnyt的user-dentry-lookup-33/写以上六件及本人进度/交付评论。支持expected-head即用，force=false；旧件全部冻结，不写lead/PR16、master/time、不改PR标题正文/Draft或他人评论、不建分支/PR。写后从固定提交和活动分支完整回读六件并核blob/head；compare d6444512a213至新头必须恰六件A新增、旧件零净修改/删除。超时先核远端，禁止盲目重复写。

正式交付后STOP_WRITING，交「MYOS2-A-C04 内核分析主线续建」独立验收，不自行开始34。若读写工具、输入身份、竞争写入出现真实阻断，如实报告并只保全安全成果；没有可用写工具时可完成只读材料并报告ANALYSIS_PREPARED_WRITE_BLOCKED，绝不声称已提交PR17。阶段2及所有权限边界不变。
