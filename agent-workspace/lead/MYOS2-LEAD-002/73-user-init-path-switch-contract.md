执行模型：次一档，沿用现有「MYOS2-A-C04b 代码分析执行」的GPT-6＋Extra。
本批合并第46批定点文档勘误与独立新取证；不新建对话、分支或PR，不上传附件。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-INIT-PATH-SWITCH-47
record_type: bounded_init_directory_switch_with_prior_documentation_correction
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-06
produced_by: ChatGPT主线规划
model_per_owner: gpt pro（沿用Owner报告）
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
read_channel: GitHub连接器
base_snapshot: time
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
mainline_owner: MYOS2-A-C04
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
executor_display_name: MYOS2-A-C04b 代码分析执行
source_pin_short12: a039d9803ade
result_base_short12: c176130f0473
lead_before_publication_short12: 73454015b3e5
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
required_review: CORE-USER-FS-PATH-ACCESS-46-REVIEW-001
prior_verdict: RETURN
prior_return_items: [R46-01]
mandatory_supplements: [R46-S01, R46-S02, R45-S01, R45-S02]
required_write_guidance: 63-github-write-retry-and-readback.md
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-path-switch-47/
allowed_files: [MANIFEST.md, init-path-switch.md, facts-and-boundaries.yaml, init-path-switch-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: c176130f0473及此前全部文件和旧评论
executor_rotation: false
external_execution_started: false
activation_gate: Owner转发固定73号且N00预检通过
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
open_questions: []
---

# 第47批：初始化代码如何切换pwd/root，以及错误如何被消费

## 1. 增量目标与范围

46已核四个fs路径getter/setter。本批先纠正R46-01文档，然后打开真实调用者一侧：fs/vfs/init.c中的init_chdir/init_chroot，以及kern_path直接入口和devtmpfsd的局部调用消费。
这是内核初始化内部函数，不据名称把它们当成用户态chdir/chroot系统调用。只分析参数、查找结果、path输出、setter调用、局部清理和返回值。

不重做29–33完整路径获取/解析、45的fs复制/退出、46四个访问函数全审；不展开挂载实现、完整devtmpfs工作线程、completion/调度、权限模型、命名空间/容器隔离、VFS创建、slab或全局引用生命周期。
init_mount的内部、devtmpfs_work_loop和complete实现只作明确边界；不能从调用点扩成线程启动专项。普通问题依18号延期，不修内核、不生成补丁、不编译运行。
纠正文档与新代码取证分区、分别申请主线裁定；安全的新分析不依赖A050/A051等争议标签。

## 2. N00：固定输入、现场预检与46追加勘误

完整读本73号、同提交46主线回执及63号；46正式RETURN，R46-01尚未关闭。合读R46-S01/S02及其引用的R45限定，不自行改为CLOSED/PASS。
沿用现有C04b的67接收、MyOS2工作令/公约/P1–P13、18/24/28和适用Mnemosyne指导；新读与继承分栏，不引入其它项目维护。
固定time a039d9803ade，核options_flags.cmake/panic.c金丝雀、direct refs、PR16/17、开放PR及写区交集。首次写前预期PR17 c176130f0473；读46交付评论6676575935和本次C04裁定。
47目录应不存在；未知后继/同名成果先核归属，不强推、不覆盖、不静默替换固定源码。主线正常推进不构成执行分支冲突。

按R46-01关闭条件完成定点处置：
1. A050/A051的条件上下文另列，分别提供单宏真实定义；不把另一个宏合算。区别DEBUG未定义、定义为0和定义非0的源码选择前提，不预处理或认证链接。
2. A052/A053补足void返回类型或更正标题并给出明确支持映射。
3. G14/C01/C04/D09修正关联；C01补实际循环语句；C04用真实搜索范围/方法记录，不以namespace源码证明“搜索未找到”；D09回实际结构，G14回选择上下文。
4. 对受影响正文、facts、DOT节点及旧自查声明作统一追加说明。原59标签作为历史保留，未重核项不能追认为全部重新通过。

在init-path-switch.md开首专属“46文档勘误”节给出旧位置→问题→替代解释/证据→跨件影响→本次核验矩阵；facts/evidence也独立分区，MANIFEST分别登记勘误与47新取证的实际self_check三数。
不另添第七件，不改原46六件或旧评论。DOT仅画47的新链，本次不要求重画46整图。
完成N00后继续N01–N04，不另等Owner确认；最终只申请R46-01文档闭合，由C04独立复核。

## 3. N01：init_chdir/init_chroot的实际控制与返回

完整读取固定fs/vfs/init.c两个函数，以及说明其内部接口用途的最小上下文；源码注释可作语境，不作为活动权限操作。
逐条列局部path是否初始化、kern_path的实参/flags/output、错误返回门、setter调用、path_put及最终error返回。LOOKUP_FOLLOW和LOOKUP_DIRECTORY、AT_FDCWD如需数值各取单宏证据。

区分kern_path正常非零、正常0且有效输出、正常0但输出有效性未证、callee不正常返回四种情形。源码if(error)针对所有非零，不只负值；实际正值生产者未证则只标helper条件模型。
init_chdir在前面的非零早退之后又检查!error，其间是否有活动赋值须回源；不把被注释的权限调用当成真实错误来源。
init_chroot内dput_and_out标签须核实际goto和贯穿；不因为有标签就构造一个额外失败路径。
区分pwd更新与root更新；一次init_chroot不能由函数名推断同时改变pwd或实现完整隔离。被注释权限/能力检查的观察不能直接升级成用户态提权漏洞：还须真实入口、威胁边界及授权前提，本批不审。
setter为void、当前path辅助体无活动引用增减；正常返回0只证所读局部流程完成，不代表目录约束、实际引用取得或所有路径解析语义正确。

## 4. N02：kern_path的有界输入/输出契约

完整读namei.c::kern_path及filename_lookup的直接入口/返回包装，按必要范围复读path_lookupat中输出path赋值门与返回，不展开其path_init/link_path_walk/lookup_last全实现。
说明getname_kernel、filename_lookup、AT_FDCWD、flags和output实际传递。getname_kernel只消费29既有结论并必要定点回源，不重做路径名复制批次。

分别说明哪个函数生产整数返回、哪个分支写出path；符号0和“path对象已满足后续使用前提”不是同一证明。输入错误指针与NULL不得混同；没有正常返回时不补出error或setter调用。
flags被传递不等于LOOKUP_DIRECTORY/FOLLOW已经在下游完整执行，保留31/32既有路径边界及PI31-F05。不得以这两个标志认证目录类型、符号链接或逃逸防护。
查找失败时wrapper直接返回而未调用path_put，不凭这个局部缺席认定泄漏；先区分是否确实产生了须由wrapper归还的持有。查找成功后的路径值和请求仍合读45/46的实际辅助体。
无需证明所有filename/路径生命期，只形成输入→正常返回/输出→调用者消费→未知项的限定矩阵。

## 5. N03：一个真实调用场景与错误消费

定点完整读drivers/base/devtmpfs.c::devtmpfsd，限init_mount之后的两次目录切换及紧接后继。
核init_mount返回的err如何被检查；核init_chdir("/..")与init_chroot(".")的先后、返回值是否保存/检查/透传，以及对本地err的实际赋值位置。
第一个正常返回非零时后续是否继续第二个调用，第二个失败是否影响后继，均依据活动语句；某一步内部失效/不返回则停在更早处，不构造后续完成事件。

不把/..或.的字面含义等同于当前不完整解析链实际结果；注释中will traverse等语句不证明实际越过挂载根。要推断路径值，必须有真实生产者证据，否则保留未知。
complete/setup_done和devtmpfs_work_loop只作后继调用边界，不分析唤醒、等待协议、工作循环或整体初始化成功；不因两次切换调用返回就认证线程/挂载系统正常。
新增非同义候选使用IP47-Fxx，数量不限也无配额；先查05/31及45/46相关旧条目，保持qualified_id/status，不把已知根因换号。
形成“内核内部调用者→查找结果/输出→pwd或root发布→局部归还请求→返回值及是否被消费”的矩阵。

## 6. N04：六件、自查、提交和交回

六件严格限定allowed_files。勘误与新取证分别统计、分别申请裁定；不得将N00文档完成写成内核问题已修复。
VERIFIED每段固定源单函数/结构/对象/单宏/单typedef的1–5行；片段含标题声称的关键语句，条件编译/extern/定义外注释另列上下文。模型条目逐项检查锚点支持，不整组机械粘贴不相干证据。
DOT分真实检查、CALL/RETURN、贯穿、数据值与分析STOP；失败必须在发生处退出，不先认证已完成相同的无效操作。没有额外errno或错误回滚不得补写。
完整体支持否定断言；引文字节、符号身份、支持强度、结构解析、图语义与有限模型分项登记。未执行写NOT_RUN，不假称完整本地P9或运行验收。
仅用现有临时环境做纯文本/有限模型，不安装、不执行仓库C/ASM/脚本、预处理/编译/QEMU/硬件或额外付费API。

## 7. 63号安全重试与停止线

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号通常首次加三次、约10/20/40秒或穿插安全只读工作间隔重试，遵守Retry-After。
写动作暂缺也多轮正规发现，不推断Owner没有权限；发现次数与实际写请求次数分别记录。已得blob/tree/commit复用，评论响应不明先回读查重，不伪造尝试或拒绝。
保持expected-head、force=false，不覆盖未知并行写入，不绕过真实认证/策略/安全限制；多轮仍不可用才准确保全已完成部分，不默认让Owner上传长附件。

仅C04b/CHAT-EXEC-05可向原PR17的user-init-path-switch-47/新增六件及本人新交付评论。旧件、旧评论、lead/PR16/master/time及PR标题/body/Draft冻结；不新建分支/PR，不合并/关闭/强推/删分支。
写前再核c176130f0473；写后固定新提交和活动分支六件全文及blob回读、compare仅六件A新增且旧件零净改删。
交付申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，分别请求R46-01闭合与47独立验收，附真实重试/回读结果，随后STOP_WRITING交C04；不自行开始48。
Owner转发且N00预检通过后同轮连续完成N00–N04；发布不等于自动跨对话投递或启动。
