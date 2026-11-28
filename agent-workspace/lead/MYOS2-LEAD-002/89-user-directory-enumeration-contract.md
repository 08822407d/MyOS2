执行模型：次一档，继续现有「MYOS2-A-C05a 代码分析执行」，沿用Owner指定GPT-6＋Extra。
先追加58文档纠正，再连续完成目录枚举三个专题；六件一次交付，交C05独立验收。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-DIRECTORY-ENUMERATION-59
record_type: prior_documentation_correction_and_bounded_directory_enumeration_contract
produced_by: unknown
model_per_owner: gpt pro（沿用Owner最近明确报告）
backend_identity: unknown_or_not_attestable
conversation_display_name: MYOS2-A-C05 内核分析主线续建
date: 2026-11-08
base_snapshot: time
read_channel: GitHub_connector
source_pin_short12: a039d9803ade
result_base_short12: 8de2a76f5f73
lead_before_publication_short12: 8f53653ac34a
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
executor_display_name: MYOS2-A-C05a 代码分析执行
executor_rotation: false
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-directory-enumeration-59/
allowed_files: [MANIFEST.md, directory-enumeration.md, facts-and-boundaries.yaml, directory-enumeration-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R58-01]
mandatory_supplements: [R58-S01, R58-S02]
activation_gate: Owner向现有C05a转发固定任务并要求执行且G00通过
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
---

# 第59批：目录枚举如何生成用户记录与推进位置

## 1. 新增价值与权限

57文档纠正已由独立回执关闭，组合材料PASS_PENDING_LOCAL；58因R58-01文档问题RETURN。D00不重开57，不重做58全部VFS修改门。
新工作分三关联专题：A为getdents64系统调用源码入口、fd借用/归还和错误出口；B为FAT目录迭代、actor回调与位置推进；C为filldir64记录容量、字段写入和部分结果。共用一次预检、一套六件，按79号一次交回。不是全文件系统、libc、完整目录规范、SMP或实机测试，也不是阶段3。
业务权威来自Owner本轮交付处理及00工作令、18/24/28/63/79；Mnemosyne仅提供已加载适用指导，不导入其维护任务。发布文件不等于已向另一聊天投递或开始执行。

## 2. G00：固定输入与现场

完整读取本89号、同发布提交reviews/CORE-USER-VFS-MUTATION-GATES-58-review.md、CORE-USER-FAT-DIRECTORY-MUTATION-57-DOC-CLOSURE-review.md，以及63、79号。58六件/评论6720176655固定8de2a76f5f73，只定点重读D00和实际依赖位置；其原87条不要求全部重做。
同一C05a按真实已读原文/身份复用87接收、项目九规则和适用Mnemosyne指导，MANIFEST分别记复用与新读；不得以旧声明代替从未读取的内容。换了新对话必须先由主线另给独立接收，不把本合同当其它执行身份通行证。
源码固定time@a039d9803ade，回核options_flags.cmake与panic.c双金丝雀；核direct time/master/lead/执行refs、PR16/17、全部开放PR、新59目录及唯一写区。首次执行写前预期8de2a76f5f73。未知执行HEAD、同名已发布成果或并行写入先核归属，不强推、不静默换针。
历史边界：完整合读35回执R35-S01/S02、36 DOC-CLOSURE的R36C-S01、38回执R38-S01/S02和41 FD-LOOKUP-REF回执的全部消费限定；依实际调用复用55/56的块返回/数组/几何限定，不重复其验收。旧发现必须保留真实字段/值、原qualified_id，回执限制另列。

## 3. D00：仅追加R58-01纠正

按58回执关闭条件，把B24拆为LOOKUP_CREATE与LOOKUP_EXCL两条单宏定义；B25从固定init.c原文取真正连续1–5行，不插入空行。原B24/B25标替代消费，不覆盖旧evidence。
同步58原87标签、被替代2标签、85条历史保留未全重核、59 D00新纠正标签、59技术标签的计数及状态。纠正原87/87全称自查的消费范围，并给58六件与评论6720176655逐项映射。状态只可CORRECTION_SUBMITTED_PENDING_MAINLINE，不能自行签R58-01 CLOSED。
一并追加R58-S01/S02明确的foo.四到三、NULL回调不保证输入对象未改、NG58-F01上游EI28/PN29去重，以及图的控制/数据归属。所引用历史条目须定点回读真实来源；不改旧状态，不把该处置扩成重做28/29/33/34。
D00置于59正文首节、facts.prior58_disposition、deferred.prior_return_items、evidence纠正分区和MANIFEST；不增加第七件。纠正拟稿完成即可连续开展不依赖错误引文的A/B/C，不逐项索要Owner批准。

## 4. G01／A：入口、fd和最终返回

主干mykernel/fs/vfs/readdir.c中的getdents64宏调用体、iterate_dir及getdents_cbk64_s；实际myos_fdget_pos/fdput_pos定义定点读取，按41旧回执分清获取/借用/归还，不扩整个fd生命周期。
区分独立iterate_dir检查iterate_shared的路径，与系统调用体中直接调用f.file->f_op->iterate_shared的实际路径；不得把注释中的iterate_dir调用当活动调用或替实际路径补NULL门。核无fd、有效file但f_op/方法缺席、方法返回0/负值各自最早停止或返回条件。
追unsigned count到回调结构int count、ctx.actor/current_dir/prev_reclen/error的初始化、f_pos到ctx.pos及回写先后。大count转换、NULL用户缓冲、0字节请求须按实际字段类型和对象有效性分析，不凭变量名假设安全。
分别推error>=0时buf.error替代、prev_reclen非零时字节数覆盖、fdput_pos的可达性；空目录、首条拒绝、已交一条后失败及零次actor路径均应有有限例。返回字节数、局部error和真实用户内存完成分开，不签系统调用表/ABI已运行。

## 5. G02／B：FAT迭代、回调停止与重入

主干mykernel/fs/fat/myos_fat32.c::FAT32_getdents64、FAT32_file_ops绑定；直接iobuf/getter/parser只按需要回源，沿用35/36/55/56结论，不重审整套簇链或名称解析。
核ctx.pos==-1的早返、其它pos与新iobuf游标初始化的关系、每次getter/辅助readent/解析器/actor的真实顺序。先行ERR_PTR、分配/对象/长度失效必须先停；getter不正常返回时不能补出安全EOF。
追name/namelen生产、actor返回值、kfree(name)、循环break、ctx.pos置值和release；列actor成功、缓冲不足、名字拒绝及更早非法对象的不同路径。传给actor的offset/ino/d_type实际值逐项核，不从FAT字段或Linux惯例补出未传入信息。
用有限目录和有限缓冲模型追连续两次getdents64调用：第一次容纳全部、只容纳部分、连第一条都容不下、初始pos为0/正值/-1；考察重复/遗漏候选及停止状态。只签所选源码前提，不保证真实盘布局、ls/readdir用户程序可用或持久化。

## 6. G03／C：记录布局、用户写入与部分进度

主干readdir.c::verify_dirent_name、filldir64及最终getdents64返回；定点读取linux_dirent64_s、dir_ctxt_s/actor类型、ALIGN/offsetof、PATH_MAX和copy_to_user实际所用包装。不能按注释把所有名字验证、用户访问保护或尾记录修补当已经实现。
记录reclen计算发生在验证前还是后、表达式类型和窄化、count比较、prev_reclen、current_dir和count更新；给空/负/上限namelen、对齐边界、恰好容纳/不足、两记录累积等有限域。不凭Python任意精度替代C溢出或类型转换。
逐个区分直接写d_off/d_ino/d_reclen/d_type/NUL与copy_to_user名字字节；copy返回值是否消费、异常标签有无活动跳转按完整定义证明。有效用户内存是条件，不从copy函数名推保护了其它直接store。
比较第一条prev==current、后续prev指向上一条、最终尾记录d_off处理及actor给定offset来源。被注释的最后修补不作活动步骤；未赋值字段/填充字节只记来源缺口，不先签ABI规范违反。
最后给请求写入、成功记账、局部错误、部分字节返回四列表；首个非法存取之后不推后续安全返回。同根因用户复制/错误忽略沿既有ID加consumer，不机械重编号。

## 7. 输入、六件和质量门

固定主文件readdir.c完整读取、myos_fat32.c仅FAT32_getdents64及真实操作表；iobuf/name parser只展开直接依赖完整定义。额外依赖软上限8个文件，优先类型/布局、fdpos、copy及直接宏；D00和已有固定正文复用另记。不为凑工作量全仓扫描，外围无法闭合列DE59-Uxx，不截断主干函数后签完成。
六件：MANIFEST.md、directory-enumeration.md、facts-and-boundaries.yaml、directory-enumeration-paths.dot、deferred-findings.yaml、evidence.md。facts至少含source_ledger、prior58_disposition、dispatch_fd_return_matrix、position_actor_traces、dirent_capacity_write_matrix、finite_model_cases、prior_findings_links。新候选DE59-Fxx、缺口DE59-Uxx，无数量配额。
适用协议v2全部P1–P13。强制锚点为D00真实两宏及init_mkdir替代、A实际入口/iterate_dir/fdpos消费、B FAT枚举/操作表/actor、C名字验证/填充/真实布局/用户复制包装。宏调用体可用真实唯一源码标识定位，必须声明未预处理，不虚构已见的展开函数定义。
每条VERIFIED必须定义内原样连续1–5行。发布前把物理行数、逐字连续命中、单一定义归属、命题支持分别检查；跨宏拆分，内部空行计入，禁止人为添加或删除源空行。不能因总计数匹配就声称各项全通过；失败项如实更正/阻断。引用作者文本或历史状态不冒充源码VERIFIED。
MANIFEST含P6自检原句、双金丝雀、自查三数、D00/技术/历史计数分离、真实规则复用/读取与NOT_RUN。图明确CALL/RETURN/DATA/LOCAL_CONTROL/ANALYSIS_STOP及各自函数归属，不称完整CFG。只允许临时纯文本、结构检查与自建有限数学/状态模型；不执行仓库C/ASM/脚本/旧夹具。

## 8. 写入、重试和停止

只向原PR17新user-directory-enumeration-59/新增六件及本人唯一正式交付评论；全部旧件、旧评论、lead/PR16、PR元数据、master/time对执行者冻结，C05不并行写该成果区。
GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。默认10/20/40秒或安全只读间隔，遵守Retry-After；复用对象，评论查重，真实认证/策略限制不绕过。
写后固定新提交和活动分支六件全文/blob/HEAD回读，compare从8de2a76f5f73只允许新六件、旧件零净改删。正式评论分别申报D00申请与A/B/C状态，READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，随后STOP_WRITING交C05。不自闭58、不自签59PASS、不自启60。
不修内核、不出补丁、不预处理/编译/链接/运行QEMU或硬件、不安装工具、不访问Owner本机/凭据/外部libc镜像、不自动调用额外付费API或子代理、不跨仓维护、不新建分支/PR、不合并关闭或强推删分支；阶段3门不变。
