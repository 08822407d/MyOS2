接收／执行模型：次一档（默认），建议沿用Owner既定GPT-6＋Extra。
必须新建「MYOS2-A-C05a 代码分析执行」；旧C04c/CHAT-EXEC-06不再接任务。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FAT-DIRECTORY-MUTATION-57
record_type: bounded_combined_fat_directory_mutation_contract
produced_by: unknown
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
conversation_display_name: MYOS2-A-C05 内核分析主线续建
date: 2026-11-08
base_snapshot: time
source_pin_short12: a039d9803ade
lead_before_publication_short12: 89319f0e3ac1
result_base_short12: 9b7239b29b0d
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
planning_authorized: true
external_execution_started: false
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
executor_display_name: MYOS2-A-C05a 代码分析执行
executor_rotation: true
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fat-directory-mutation-57/
allowed_files: [MANIFEST.md, fat-directory-mutation.md, facts-and-boundaries.yaml, fat-directory-mutation-paths.dot, deferred-findings.yaml, evidence.md]
activation_gate: Owner在新C05a明确转发接收并执行指令且G00通过
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
read_channel: GitHub_connector
evidence_class: Owner授权与固定GitHub证据支持的任务规划；不是执行结果或验收
open_questions: []
inputs_read: [Owner本轮指令, 85号03-CONFIRMED, 56主线回执, 63号, 79号, 固定time目录修改相关原文]
---

# 第57批：FAT目录项如何生成、创建如何发布、删除如何回收

## 1. 目标、增量和授权

Owner本轮原话：“现在模型切换到gpt pro了，你可以设计第57阶段工作了”。本主线据00工作令、18/24/28/63/79及85号CONFIRMED设计并发布本包；不把设计授权写成外部执行已经开始。
第56批仍为PASS_PENDING_LOCAL，returned_scope=[]，R56-S01/S02继续有效，不重新验收。原“未设计”的暂停状态由本包的已设计待启动状态接续；实际执行仍以Owner向新对话转发为门。
按79号把三个关联专题一次取证、六件交付、一次主线独立验收。A为名称／槽生成及落位；B为create/mkdir的返回与对象发布；C为rmdir/unlink的判空、标记与资源回收。不是完整FAT规范审计、修复、真实盘面测试或阶段3综合。
不再分析普通文件read/write簇推进、整个expand算法或完整几何矩阵。只消费35/55/56已审接口边界，补目录修改消费者的新证据。任务书提出核查问题，不预签任何新缺陷结论。

## 2. G00：独立接收、固定输入与现场预检

先按同提交87-executor-continuation/00-C04c-to-C05a-handoff-package.md及01-C05a-receiver-startup-prompt.md完成接收报告、MyOS2规则实际加载、单独适用Mnemosyne指导加载，再做本G00；不能沿用旧C04c的读取声明。
完整读取本86号、63号、79号及同提交reviews下：
- CORE-USER-FAT-ALLOCATION-GEOMETRY-56-review.md（全部R56-S01/S02）；
- CORE-USER-BLOCK-IO-CONSUMPTION-55-review.md（全部R55-S01/S02）；
- CORE-USER-FAT-DIR-ITERATOR-35-review.md（全部R35-S01/S02）；
- CORE-USER-FAT-NAME-PARSE-36-DOC-CLOSURE-review.md（R36C-S01，不重启原RETURN）；
- CORE-USER-FAT-SUPERBLOCK-BINDING-38-review.md（全部R38-S01/S02）。
使用open完成链的旧结论时，另读CORE-USER-OPEN-COMPLETION-39-review.md及其DOC-CLOSURE回执；未使用则不要求全读39/40成果。52/53闭合及53主线误判撤回保持，不再转述已撤回的空行指控。
旧成果只定点查对应MANIFEST、facts/deferred中的旧ID和必要原证据，不要求全读1–56或101段56引文。旧状态以原回执为准，作者自报不替代主线裁定。
核direct time/master/lead/执行refs、PR16/17及全部开放PR。源码只用time@a039d9803ade；读取options_flags.cmake和panic.c双金丝雀。首次执行写前预期9b7239b29b0d，新57目录应不存在。主线正常新增本包可解释；未知执行HEAD变化、同名正式成果或写区交集先停写核归属，不静默换pin。
完整保留原PR17和历史分支名；仅CHAT-EXEC-07在本包启动后写新57六件及本人新交付评论。C05不写执行成果，旧C04c继续STOP_WRITING。

## 3. 源码输入及读取预算

四个主文件完整读取：mykernel/fs/fat/namei_vfat.c、myos_dir.c、myos_fat32_namei.c、myos_fat32_iobuf.c。
定点展开mykernel/fs/fat/myos_fat32.c中的FAT32_ent_empty、FAT32_get_full_ent和所需FAT getter/setter/finder；旧接口不重复整份取证。
类型／布局输入：mykernel/include/linux/fs/fat32.h、linux/fs/fat.h、uapi/linux/msdos_fs.h；后两项路径均以mykernel/include/为前缀。补读实际使用的qstr/inode/dentry及整数类型定义。
上层消费者限mykernel/fs/vfs/namei.c内create/mkdir/unlink/rmdir的直接调用及其紧邻返回处理；必要时mykernel/fs/vfs/init.c的直接mkdir入口。不从默认分支搜索摘录签VERIFIED，定位后必须回到固定time原文。未定位的符号标unknown，不能补成Linux惯常实现。
辅助展开上限为8个额外文件，仅取本命题直接依赖的字符串、类型、错误编码、列表或操作表定义。超出仍无法闭合时记录边界并交主线，不扩全仓审计。主干不因软预算而截断定义；优先完整A/B/C关键路径，外围细节可标needs_evidence。
source ledger逐项记path、固定ref、blob短身份、真实定义、实际范围及新读/同pin原文复用；新对话不能把旧作者片段冒充自己回源。

## 4. G01／A：名称到槽，再到目录缓冲

完整追vfat_striptail_len → myos_vfat_create_shortname／vfat_build_slots → vfat_add_entry → fat_add_entries。分开qstr长度、尾点裁剪长度、NUL终止可读范围、短别名、长槽数量及最终短槽；未读原字符串helper前，不假设strrchr受len约束。
核MSDOS_NAME、MSDOS_SLOTS和真实长短槽布局，追每个分配容量、每次读写范围、终止／填充值、序号与checksum生产；“helper返回1的短名分支存在”不等于调用的helper实际产生1。ASCII到16位扩展不自动叫完整Unicode支持。
给长度0、短名字及带点、13倍数与相邻长度、超过固定槽容量的条件边界；超长只作为helper输入域时须标明，不假称可从已证用户入口到达。别名重复只判断所读生成算法及真实直接门，不声称全系统没有重名保护。
fat_add_entries以empty_off、emtpy_count、iter_cursor、iter_reach_end逐轮表格核连续空槽、被占用打断、恰好所需数量、目录上限处不足一段和扩容后的状态。不能把计数变量名当“已找到槽数”的定义，也不能把局部error直接当最终返回值。
将选中的起始off与nr_slots*sizeof(msdos_dirent_s)连接到iobuf_write的单buffer范围。比较同buffer、跨buffer、末尾不足、size0等条件；不能跨过更早的ERR_PTR/NULL、源缓冲不可读、整数或目标范围失效继续推演。
沿用BI55写入跨度／返回丢弃和AG56扩容限制，只新增目录槽消费者的放大／屏蔽条件，不换号重复计缺陷。不重审expand全体或名称解析器；必要回读36组合材料解释写入后再查找的有限关系，不认证完整读写闭环。

## 5. G02／B：create/mkdir的发布顺序与失败出口

主体FAT32_create、FAT32_mkdir、FAT32_alloc_new_dir、vfat_add_entry及vfat_dir_inode_operations绑定；上层只核直接调用者。
分别列：新簇候选、点/点点缓冲、数据WRITE请求、FAT结束标记请求、父目录槽插入、inode/dentry发布、上层返回。每个事件写“源码赋值／调用请求／callee正常返回／外层可见结果”所属层，不能合并为一次事务或持久化成功。
create的mode、sinfo和dentry分别追生产与后继消费；返回0是否伴随d_inode建立要由活动赋值与真实直接调用者证明。缺少发布只能报告所读范围，不能据此宣布所有open(O_CREAT)必然崩溃或某用户程序已失败。
mkdir核u32返回接口到int cluster、cluster<0门及传入vfat_add_entry的转换。分别给0、1、正常簇、编码错误与超出int可表示域的假设；标准层、所选x86-64实现模型与未运行的实际编译结果分开，不无条件宣称“u32错误一定漏检”或“一定变回负数”。
核新目录初始化字节、点/点点来源及失败路径资源清理；先行内存、几何、对象有效性是前提。目录槽插入失败后是否撤销已请求的分配／写入，逐对象列证据，不以setter常量返回签磁盘分配成功，也不把一扇区初始化扩成整个簇已清零。
R38的private_sb_info与s_fs_info、FAT32_inode_info_s与msdos_inode_info_s不互换。只建立某条条件合法调用链，不能绕过旧对象来源缺口补“全流程正常”。不扩挂载、权限、全dcache或文件描述符生命周期。

## 6. G03／C：判空、删除标记、回收与对外结果

主体FAT32_dir_empty、FAT32_rmdir、FAT32_unlink、FAT32_release_clusters、FAT32_mark_entry_romoved（按源码原拼写）、FAT32_iobuf_release；getter与lookup的位置元数据仅为直接依赖。
判空区分真正无普通项、只有点/点点、普通项、普通项恰在最后位置、getter错误及更早非法解引用。R35末项覆盖和错误消费只在各自条件下应用；不能把错误指针已被getter安全返回当作默认事实，也不能把所有错误统一当“空目录”。
追dentry_position/dentry_length从lookup生产到parent目录删除写入的单位与范围；原字段注释不替代实际赋值。分别核零长度、非条目整数倍、跨buffer、负off转换／加法边界及相应最早停止点。受既有lookup缺口影响的输入只作helper-only，不假称真实目录稳定提供该元数据。
列释放簇、父目录标记、缓冲DIRTY/DELETE、FAT_FREE请求和外层返回的真实先后；部分完成与后续失败须留在表内，不将源码顺序说成崩溃一致性证明。
int helper缺return时，检查调用者是否消费返回值；不凭缺return泛化整条路径必然未定义或必返随机errno。void release、被忽略的write/setter值、外层error的关系分别核实。
内存对象、缓冲数组、磁盘表项、inode/dentry及仍打开文件的生命周期分开。未展开并发／打开文件引用时，登记未证，不能宣称unlink已经保证POSIX寿命或已构成双重释放。

## 7. G04：交付、证据与验收标准

唯一六件：MANIFEST.md；fat-directory-mutation.md；facts-and-boundaries.yaml；fat-directory-mutation-paths.dot；deferred-findings.yaml；evidence.md。不得另建第七件规则回执或脚本。
正文按A/B/C给结论、前提、最早失效点和未证范围；facts包含source_ledger、name_slot_matrix、slot_search_traces、create_mkdir_event_matrix、delete_lifecycle_matrix、error_return_matrix、finite_model_cases及prior_findings_links。YAML可用JSON兼容形式；图明确CALL/RETURN/DATA/LOCAL_CONTROL/ANALYSIS_STOP，不把数据关联画成调用，不自称完整CFG。
新候选用DM57-Fxx，证据缺口DM57-Uxx；同根因沿用旧qualified_id，新增consumer证据另挂链接。没有新缺陷、引文数量、行数或模型数配额；用可反驳的命题和正反例衡量增量。
协议v2 P1–P13全部适用：每条VERIFIED含path::真实symbol和定义内连续1–5行原文；保持tab/空行/CRLF。定义归属、字面命中、命题支持和consumer各自检查；跨宏/跨定义分开，外围注释标CONTEXT。不得机械切满整个函数后以碎片数冒充发现。
强制锚点组：A的五个实际函数、槽数/布局宏与iobuf_write；B的create/mkdir/new-dir、操作表及真实直接消费者；C的判空/getter、两个删除helper、rmdir/unlink/release及位置字段生产。每个核心命题以完整实际活动体核前提，强制锚点不得用INFERRED替代。符号未能核实则相应专题不能标completed_in_scope。
MANIFEST记录接收报告、项目加载、指导刷新、model_per_owner与后端未知、read_channel、startup_selfcheck_quote、branch_canary_quotes、self_check三数及实际检查日志。P9可读/文本子集与完整本地P9分开；没有做的检查写NOT_RUN。仅允许临时环境纯文本、JSON/YAML/DOT结构与自建有限数学/状态模型；不执行仓库C/ASM/脚本或旧夹具，不把模型当真实内核复现。
A/B/C按completed_in_scope或具体blocked-by分别交代，一次正式交付。执行者只能申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING；C05独立验收最高PASS_PENDING_LOCAL，不预签本批通过。普通问题按18号入库，不要求Owner现在修复。

## 8. 发布、安全重试与停止

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。
默认间隔约10/20/40秒或安全只读工作，遵守Retry-After；复用已成功对象，ref响应不明先核HEAD，评论响应不明先按任务ID/固定提交查重。工具发现次数与真实写请求次数分开，永久认证／策略阻断不绕过。
正常交付必须核固定提交与活动分支六件全文、blob及HEAD，compare从9b7239b29b0d仅新增本六件，旧件零净改删；唯一正式交付评论列真实结果后STOP_WRITING，交回C05，不自启58。
全部旧件/旧评论、lead/PR16、master/time及PR元数据对执行者冻结。不建分支/PR、不合并关闭/强推删分支、不修内核或生成补丁、不预处理/编译/链接/运行QEMU或硬件、不访问Owner本机/凭据/外部libc镜像、不安装工具或自动使用额外付费API/子代理，不跨仓维护。
