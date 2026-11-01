---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-MMAP-PLACEMENT-21
record_type: bounded_mmap_address_placement_static_analysis_contract
evidence_class: 后续执行授权和验收条件；不是已完成的代码分析
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-11-01
read_channel: connector
authorization_ref: Owner报告20完成；28号同轮验收及后继派发持续约定
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-02
executor_display_name: MYOS2-A-C03b 代码分析执行续建
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
instruction_base_short12: 3f092c02f88f
required_review_record: CORE-USER-MMAP-PROTECTION-20-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-MMAP-PROTECTION-20-review.md
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；lead=agent/MYOS2-LEAD-002
source_pin_short12: a039d9803ade
result_base_short12: 0e139c6ca86d
master_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-mmap-placement-21/
allowed_files: [MANIFEST.md, mmap-placement.md, facts-and-boundaries.yaml, mmap-placement-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 0e139c6ca86d及此前全部文件
writer_assignment: C03b仅本批六件与自己的PR17进度或交付评论；正式交付后停写
activation_gate: Owner向已有C03b转发本任务，固定输入与现场写前检查通过
external_execution_started: false
phase: 2
runtime_evidence: NOT_RUN
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_change_authorized: false
kernel_patch_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
inputs_read:
  - 第20批六件、39号及本轮20主线回执；18/24/25/28/36既有规则。
  - 固定time do_mmap和simple_mmap_region；此前同源mmap.c选址主体和VMA辅助函数作为范围预检。
  - 本轮回固定time读取mmap_arch.c的arch_pick_mmap_layout；默认分支搜索仅导航。
open_questions:
  - 地址回调绑定、区间选择及真实返回的系统性取证尚待本批；主线预检不是执行者完成证据。
---

# 第21批：mmap选出的地址是否满足整段区间条件

## 1. 目标与连续推进方式

第20批按PASS_PENDING_LOCAL限定接收，须合读R20-S01至S04；不另交20勘误，不重新审查整套权限、引用或文件模式。继续已有C03b及GPT-6＋Extra，沿用25/36接收，不新开对话，不恢复C03a或CC02的委派。

本批只回答：提示地址和固定地址怎样经过选择与检查；非固定请求怎样找空洞；候选起点、完整区间、页对齐、地址上限及冲突判断是否分别得到实际源码支持。结果进入simple_mmap_region的边界需要说清，但不重做14的分割/摘链/清理、20的权限链或完整地址空间审计。

Owner启动句：

> 执行CORE-USER-MMAP-PLACEMENT-21。完整读取40号固定任务及20主线回执，沿用C03b接收和GPT-6＋Extra。按A00–A04连续完成mmap回调绑定、提示地址与空洞搜索、固定地址及区间校验的静态分析、自查、原PR17提交和远端回读。仅交指定六件，旧件保留；普通问题入库，不修内核、不重做19/20。完成后停写交C03验收。

工作包内部连续推进，不逐步索取确认；实际访问、授权或未知并行写入阻断才停止相应操作。证据不足记partial及具体边界，不以文档齐全替代分析完成。发布文件不自动唤醒执行聊天。

## 2. A00：固定输入及已接受边界

完整读取本任务和20回执。源码time固定a039d9803ade、结果0e139c6ca86d与指令基线3f092c02f88f分开。先核options_flags.cmake；默认分支搜索只导航，正式断言回固定对象。18号延期、24/25分工、28号同轮派发及36号C03b身份继续有效。

20回执的R20-S01转换先后、R20-S02对象与宏值、R20-S03分支和类型转换、R20-S04函数归属必须消费，不复制其错误旧读法。14回执只取先行解除映射及错误不等于完整回滚的限定；12回执只取simple_find_vma的实际包含谓词。需要历史问题ID时定点读相应原件与回执，不全量搬运08–20大型YAML。

主要导航：mykernel/mm/vm_map/mmap.c、vm_map.h、vm_map_macro.h、util.c、相关类型/常量，以及arch/x86_64/mm/vm_map/mmap_arch.c。按真实include和符号读取必要直接依赖；不要用Linux同名函数的典型语义补出MyOS2行为。

## 3. A01：地址选择接口和实参来源

核do_mmap到__get_unmapped_area的实参和前置len、PAGE_ALIGN、pgoff、MAP_FIXED_NOREPLACE处理。只消费20已核的权限接口，不重跑系统调用包装或完整文件权限矩阵。

完整核__get_unmapped_area：函数指针get_area从何处取值，文件f_op怎样覆盖，匿名共享路径实际调整了什么，哪些替代回调只是注释。核arch_pick_mmap_layout中mm->get_unmapped_area和mmap_base的实际赋值，并定点追一个必要的调用者或初始化来源；未闭合的时序、复制继承和全部文件回调明确标记。

查清TASK_SIZE、TASK_SIZE_MAX、TASK_UNMAPPED_BASE、DEFAULT_MAP_WINDOW、PAGE_SIZE/PAGE_MASK及选定info字段类型的真实定义与配置前提。地址字节、页号、长度和上下界分列；需要算术时先注明位宽、无符号回绕及对象有效前提。函数指针非NULL不证明目标正确，定义存在不证明该路径实际被调用。

## 4. A02：提示地址与非固定空洞搜索

完整读simple_get_unmapped_area、其实际调用的搜索函数和相关迭代/相邻VMA帮助函数。区分“起点未落入现有VMA”和“整个[start,start+len)不与VMA相交”；已有快速返回是否只检查起点、上层是否补查全区间，都要按真实语句说明。

对topdown与bottomup分别登记：实际调用者或仅定义、循环初始化、迭代停止、候选区间计算、长度检验、回退和返回。逐个追踪gap、gap_end、tmp、prev/next等局部变量，区别赋值与比较、候选已找到与后来被覆盖。不要依函数名推定遍历方向、对齐保证或找到最优区间。

必要的for_each_vma宏、mm_next_vma/mm_prev_vma、vma_next_vma/vma_prev_vma及列表帮助函数按真实依赖取证。空表、首尾、NULL、anchor与计数不同步等情形，须区分前提有效、源码不返回、无效对象读取及无法保证的后果；不能在发生未定义或未闭合行为后继续推导一个确定返回值。

如某个搜索函数没有在本批所选路径被调用，保留其局部事实但不得计入活动执行链。数组、列表和其它线程状态不做全局认证，不开展通用链表库或SMP锁审计。

## 5. A03：固定地址、冲突和返回结果消费

分开追MAP_FIXED与MAP_FIXED_NOREPLACE。明确__get_unmapped_area各个early return相对于回调、对齐和TASK_SIZE检查的位置；前置校验和后置校验不能互相冒充。固定地址若跳过某些检查，只说明该接口路径，继续定点核do_mmap后续是否有补充门，不直接宣布全系统没有检查。

核find_vma_intersection及其查询依赖的实际区间谓词与边界。以半开区间说明贴邻、部分覆盖、完全覆盖、起点空闲但尾部重叠；对addr+len、上界减len、对齐和pgoff左移分别保留溢出/回绕前提。所选固定请求、提示请求和无提示请求不能套同一套检查顺序。

返回的地址值、编码错误值、零地址及不返回路径分别记账。回调给出地址不等于它满足全部下游条件；没有完整区间验证证据时不能称其为“已经确认的安全空洞”。只衔接simple_mmap_region入口及其先行旧映射处理，合读14回执，不重做整个解除映射/回滚或新VMA合并算法。

## 6. A04：六件交付与有界自查

严格仅allowed_files。MANIFEST逐阶段completed/partial；mmap-placement.md讲清输入→回调选择→候选→校验→返回/下游接口；facts给对象身份、条件矩阵、调用/data/effect及有限算术案例；DOT对应；deferred登记新证据或旧问题追加；evidence包含实际读取、逐字锚点、检查方法及未做项。

建议新ID AP21-*。矩阵按实际发现覆盖：无提示/有效提示/起点空但尾部碰撞、空表/单个/多个VMA、首尾空洞/内部空洞/边界贴邻、固定/不替换、非对齐、长度和上界算术、文件回调覆盖、错误值/零值/正常返回、候选被覆盖或控制流未完成。无真实生产者的场景保持条件性，不硬称运行可达；不设置引文或问题数量目标。

每条强制源码断言需path::symbol及1–5行原文，足以展示实际条件或动作。完整阅读相应控制流后才能给局部否定。引文应位于标签所指函数/宏体，不把外层调用者当作字段赋值的实际作用者。

call是实际调用或明确宏绑定；间接回调用调用点、槽和绑定证据说明，表项不是中间函数。data表示消费方到提供方，effect单独记录赋值/状态变化；同一变量不同时间的值分阶段表示，不用一个“归一化值”抹掉先后顺序。图ID一致不等于语义成立。

允许在自己的临时沙箱使用已有文本工具作字符串、JSON/YAML、图ID及有限算术检查，不安装、不执行仓库旧脚本或C/ASM。纸面/文本算例不是内核运行反例；没有证据就收窄，不靠反复增加日志或通用验证器弥补。仅申请READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING，上限PASS_PENDING_LOCAL。

## 7. 单写者、提交和恢复

C03b仅写既有PR17/claude/dazzling-cori-q0dnyt，本批首次写前期望0e139c6ca86d。现场查master/time/执行头、开放PR、路径交集及目标目录；已存在内容先核归属与完成状态，不重复起批或盲覆盖。time变化记录但不静默换源码；lead合法前进不是执行冲突。

全部历史文件冻结；正式交付前可按当前blob身份修订本批自己的新件。仅普通快进，不新建或重命名分支/PR，不修改PR标题、正文、Draft状态或他人评论。比较更新不等于全局锁；写入结果不明先读取远端路径和历史再补余项。

最终固定提交全文读回六件，再与活动分支核blob/头；compare 0e139c6ca86d到交付头仅本批六件A新增，无历史文件净改动/删除或未知提交。后置验证记自己的PR17交付评论和真实短提交，不为自引用最终提交反复改文件。交付后停写，Owner只需通知C03，由主线独立验收并同轮安排后继。

中断进度只保存在这六件适当位置或自己的评论；不加第七件、不承诺后台续跑。C03独占lead、PR导航及验收；旧执行者与导览/教学对话不获得本批写权。

## 8. 禁止动作不变

只做阶段2静态分析。不修改内核、用户程序、旧成果或旧脚本，不生成修复补丁，不预处理、编译、运行C/ASM、内核/QEMU或硬件实验，不安装/挂盘/读取凭据或仓库外libc/镜像，不操作Owner本机，不调用额外付费模型/API或Deep Research。不写master/time/lead、不合并关闭PR、不删除或强推分支、不进入阶段3。普通问题按18号延期，Owner当前无需作修复决定。
