---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-FD-ALLOCATION-INSTALL-40-REVIEW-001
record_type: independent_bounded_static_acceptance_with_mandatory_supplements
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-05
produced_by: unknown
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（沿用Owner报告）
backend_identity: unknown_or_not_attestable
read_channel: connector
source_pin_short12: a039d9803ade
reviewed_pr: 17
reviewed_base_short12: 4ea455865a47
reviewed_head_short12: 46f5327a8cdf
reviewed_delivery_comment: 6649633435
contract65_short12: a70d22015ae5
lead_before_publication_short12: a70d22015ae5
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
mandatory_supplements: [R40-S01, R40-S02]
prior_R39_closure: separate_CORE-USER-OPEN-COMPLETION-39-DOC-CLOSURE-REVIEW-001
runtime_evidence: NOT_RUN
full_local_P9_gate: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_full_55_quote_byte_scan: NOT_RUN
reviewer_original_JSON_YAML_parse: NOT_RUN
reviewer_original_DOT_blob_identity: NOT_VERIFIED
reviewer_graph_transcription: {explicit_nodes: 53, unique_nodes: 53, edges: 62, undefined_endpoints: 0}
reviewer_independent_finite_models: 24
self_check: {scope: two_added_single_definition_quotes_in_this_receipt, verified_claims: 2, quotes_reconfirmed: 2, downgraded_to_inferred: 0, method: 固定连接器文本人工核对，非全量引文机械重放}
next_followup: CORE-USER-FD-LOOKUP-REF-41
next_task_ref: 66-user-fd-lookup-ref-contract.md
next_executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
next_executor_display_name: MYOS2-A-C04b 代码分析执行
executor_rotation: true
execution_branch_modified_by_mainline: false
owner_decisions_requested_now: []
phase: 2
kernel_change_authorized: false
merge_authorized: false
phase3_authorized: false
---

# 第40批独立验收：fd选择、安装及失败消费限定接收

## 1. 裁定、身份与净差异

依据65号、39回执R39-S01/S02与第40批六件，裁定PASS_PENDING_LOCAL，无另行返工项。
原六件与评论保留，后续使用必须合读R40-S01/S02。F00的R39-01闭合另见独立回执，不由本裁定替代。
本结论不认证完整fd生命周期、实际用户open、真实并发、物理释放或内核运行。

独立compare 4ea455865a47→46f5327a8cdf：ahead=1、behind=0、恰一提交，仅本目录六件A新增1757行。
MANIFEST149、正文100、facts639、DOT119、deferred196、evidence554；没有旧件净修改、删除或写区外路径。
PR16/17实际分支仍为agent/MYOS2-LEAD-002和claude/dazzling-cori-q0dnyt，两者open/unmerged，PR17为Draft。
本轮主线起点a70d22015ae5，执行46f5327a8cdf；指定评论6649633435分别请求R39闭合及40验收，并声明STOP_WRITING。
停止声明不是旧聊天进程物理终止的证明；Owner本轮要求下一批轮换执行者，故41不再委派C04a。

## 2. 核心结论得到固定源码支持

files_struct_s内嵌fd_array[NR_OPEN_DEFAULT]，所选宏值256。fd_count是另一unsigned long字段，不自动等于物理容量。
alloc_fd在file_lock内逐槽查NULL，命中时只给局部uint fd赋值并break；解锁后返回。完整所选体没有槽位/位图预留写入，也不读取flags。
空区间或范围内全部非空时，局部fd没有赋值；不能补出确定负errno、随机整数或真实用户后果。该旧根因仍按UV-11引用，不重新编号。

fd_install从当前files取得数组，调用smp_rmb，执行BUG_ON占用表达式，再写fps[fd]=file。没有本地容量比较或file有效性检查。
put_unused_fd只有锁/解锁，没有槽位或预留位撤销。因此“查到索引”“占用槽位”“安装指针”“撤销预留”不能混同。
没有预留证据时，缺少put_unused_fd调用本身不能推出预留泄漏；也不能由此证明所有文件/路径资源均无泄漏。

do_sys_openat2的getname编码错误可在fd选择前早退。后续选择值覆盖校验返回；已识别file错误再覆盖局部fd且跳过安装。
额外给定合法索引、空槽和普通有效file时才可推演有用安装；实际源码门仍只是fd>=0及!IS_ERR(f)。
旧UV-04、OF30-F01和SB38边界未被修复，不据辅助有效输入认证真实用户open成功。

## 3. R40-S01：数值、状态、对象与错误生产者

1. alloc_fd声明返回int，局部fd是uint，转换发生在该函数return时，不是调用者才转换。
   start转int、i与uint end的比较及fd_count到uint的转换分别处理；本轮安全0..255模型不推广为任意宽度输入。
   end<=256是便于证明安全的充分范围，不是代码中的检查，也不是所有正常返回的必要条件：end=257但首槽NULL时可在界内提前退出。
   范围超出且此前没有空槽时，才在首次越界处停止；不能将更早未定义/未证行为之后的整数当已证结果。

2. 在已核合法范围内，成功索引非负；空/满路径无定义良好的返回值。所选alloc_fd没有正常负errno生产者。
   do_sys_openat2的fd<0分支作为调用者结构真实存在，但不能由满表推成已到达；独立负fd模型须标helper-only/未证生产者。
   getname及被识别file错误的正常传播则应单独保留，不能被后一条缺口一概抹去。

3. “scan没有共享状态修改”仅指所选槽/位图/预留状态；lock/unlock可操作锁自身，不能扩大为函数毫无共享副作用。
   两次选择模型要求同一files对象、初始槽状态、正常返回及说明的顺序；它是抽象顺序交错，不是实际并发C执行、锁正确性或机器竞态复现。

4. BUG_ON占用表达式不是容量门。所读备用宏受CONFIG_BUG以及HAVE_ARCH_BUG_ON等选择条件约束；未预处理认证目标绑定。
   在会求值condition的所选宏模型中，fps[fd]读取本身先要求合法下标。不能把“先读后判占用”表述成源码先做了边界检查。
   若目标宏不求值condition，不能强称最早访问仍是该读；随后实际数组存储仍要求有效索引。宏真值不自动等于确定panic或实际CPU终态。

5. 安装没有本地get_file/fput不等于已有引用绝不可能由调用者转移所有权；引用转移本来未必需要增减计数。
   此处只确认计数调用缺席，不证明完整所有权成立或不成立。屏障也不等于互斥、预留或事务性发布。
   NULL可在helper-only空槽模型中被存入，槽仍为空；没有因此找到当前do_filp_open的正常NULL生产者。

6. 原索引与后来被PTR_ERR覆盖的fd变量分开。失败后槽仍空需要没有其它写者或被调函数对该表的未证副作用；不能由一个局部扫描推出全局表恒定。
   init_files静态初值仅在该实际对象、相应时间点成立；dup_fd只提供另一来源，未覆盖完整初始化/复制和所有任务。

## 4. R40-S02：局部引文及图的合读纠正

FD40-A025含真实BUG_ON定义，也包含定义外的#ifndef/#endif选择上下文。直接定义证据只取中间第66行；两侧另记上下文，不计为宏定义体。
它与39的ENFILE/EXDEV错误身份不同：这里标签指向的真实宏确在片段内；主线直接收窄摘录范围，不再派一轮返工。

[VERIFIED mykernel/include/linux/kernel/asm-generic/bug.h::BUG_ON]
```c
	#			define BUG_ON(condition) do { if (unlikely(condition)) BUG(); } while (0)
```

FD40-A026只是BUG长宏中的局部片段，不能凭它认证完整预处理绑定。作者55标签及55/55仍按其方法申报，主线没有执行全量自动字节扫描。
FD40-A016/A047以及A020/A021为重叠片段，不代表额外独立事实或根因。
FD40-A011证明分配和按值复制，未包含新槽写入；C11和正文的槽赋值需合读dup_fd下一行：

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
			new_fds[i] = new_fp;
```

C10写成get_unused_fd_flags(0,0,flags)的调用表示应读为get_unused_fd_flags(flags)→alloc_fd(0,0,flags)；前者真实签名只有一个参数。
facts中fd_local的转换位置按R40-S01；BUG_ON/屏障所在write栏是控制或排序事件，不是已证槽写入。
图checkslot→occupied是一次宏条件的摘要与展开，不能执行两次检查；step和loop的描述只对应每轮一次实际递增。
图fdneg只是调用者备用分支，不带正常负fd生产证据；“f_dupfd (not syscall)”仅表示不同于do_sys_openat2主链，不否认它可受fcntl入口调用。
图中的bounds/idxprem/invalid是分析前提与停止点，不是源码额外门或CPU停止指令。

本主线从工具返回的标识和边逐一转录并复算53唯一显式节点、62边、无未定义端点。
没有核完整原DOT blob，也没有运行Graphviz；结构计数与逐边语义分开。上述补充统一约束正文、facts、DOT、deferred与evidence消费。

## 5. 方法、限界及后继

完整读取65号（尾部补读）、40六件及指定评论；重读39回执关闭条件和两组消费补充。63号、MyOS2工作令/公约/18/24/28和适用指导沿用既有加载。
直接固定源本轮回读：file.c1–250、fdtable.h1–75、bug.h1–150、errno-base.h15–32、fs.h76–84/3419–3430、open.c67–85/241–262、err.h14–45、file_table.c38–137、fcntl.h56–66。
file_s字段等既有同一固定源上下文继承，不冒称本轮逐条重跑所有55摘录。历史08的US-01/02/03定点读取用于去重和后继划界；旧05::UV-11等按65及历史记录继承。

本地只做标识/边转录结构检查与24组独立有限状态/整数模型：扫描命中/空满/界外及提前结束、安装NULL/占用/界外、两次选择顺序、五个64位错误编码样例、errno与进制身份。
不是作者26例全集重放。一次可选public raw文本下载因DNS失败未取得原字节；正式证据仍来自固定连接器，不将该读失败写成GitHub写入或权限故障。
未解析原JSON/YAML、未全量核55原引文字节、未核原DOT完整身份、未执行完整本地P9或仓库C/ASM/脚本、预处理/编译/QEMU/硬件、真实fd/用户open/磁盘。
同模型家族角色分离复核，不是人类或异构技术认证。三个FD40候选不是三个已复现独立漏洞；历史源码问题不自动关闭。

后继66号CORE-USER-FD-LOOKUP-REF-41交新C04b/CHAT-EXEC-05，重点补fget/__simple_fget的范围、NULL、mask及mmap入口消费，比较轻量借用和fdput配对；US-01为既有基线，不再同义立项。
后继预检读取file.h全文、read_write.c145–390，以及mmap.c定位窗口1–235/400–805/1060–1210/1250至末尾；只据所定位的mmap系统调用包装层划界，不预签VMA/映射实现。
C04a停于40，不要求其再制作交接报告；新对话通过同提交67号交接完成规则加载和现场核对后才能执行41。

固定65号：https://github.com/08822407d/MyOS2/blob/a70d22015ae5/agent-workspace/lead/MYOS2-LEAD-002/65-user-fd-allocation-install-contract.md
固定成果：https://github.com/08822407d/MyOS2/tree/46f5327a8cdf/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fd-allocation-install-40
交付评论：https://github.com/08822407d/MyOS2/pull/17#issuecomment-6649633435
