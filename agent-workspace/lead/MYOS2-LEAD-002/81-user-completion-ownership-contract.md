执行模型：次一档，沿用现有「MYOS2-A-C04c 代码分析执行」、GPT-6＋Extra。
一次补齐52记录并完成三个相关所有权专题，统一交回；不逐专题要求Pro派发，不新建对话或上传附件。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-COMPLETION-OWNERSHIP-53
record_type: bounded_combined_completion_ownership_contract_with_documentation_correction
evidence_class: 阶段2固定源码任务书
produced_by: unknown
model_per_owner: gpt pro（沿用Owner最近确认）
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-07
base_snapshot: time
read_channel: GitHub_connector
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
executor_display_name: MYOS2-A-C04c 代码分析执行
mainline_owner: MYOS2-A-C04
executor_rotation: false
source_pin_short12: a039d9803ade
result_base_short12: ae6ce5353844
lead_before_publication_short12: c7f6cad5316f
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
required_review: CORE-USER-WAIT-WAKE-CONSISTENCY-52-REVIEW-001
prior_verdict: RETURN
prior_return_items: [R52-01]
mandatory_supplements: [R52-S01, R52-S02, R51-S01, R51-S02, R50-S01, R50-S02, R49C-S01]
planning_record: 79-work-package-sizing-and-pro-review.md
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-completion-ownership-53/
allowed_files: [MANIFEST.md, completion-ownership.md, facts-and-boundaries.yaml, completion-ownership-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: ae6ce5353844及此前全部原件和旧评论
activation_gate: Owner转发固定81号且O00预检通过
phase: 2
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
open_questions: []
---

# 第53批：通知发生后，谁还可以读取、释放或再次使用对象

## 1. 目标和边界

52已补A队列/B计时器/C调用点，但WW52-U04及各调用者仍缺“对象来自哪里、谁最后使用、通知后谁释放/再用”的局部证据。本包选择三种不同协议作有界分析，不再花一轮重扫205个C文件或复述全部completion内部。
A核devtmpfs请求，B核kthread创建握手，C核静态keyboard completion与一次性IDE请求的对照。合用一次预检、一个源台账和六件；一次交回C04，不自行选择54。
本任务适用协议v2全部P1–P13及00/18/24/28/63/79。仍为阶段2，不是全内核生命周期认证、实施修复或阶段3综合。
不修内核、不生成补丁、不编译运行、不运行仓库C/ASM/脚本或历史夹具，不读取Owner本机/外部libc镜像、不安装工具、不调用额外付费模型/API。

## 2. O00：共同预检与R52-01外置补充

完整读本81号、同固定提交52回执含R52-01关闭条件/R52-S01/S02、79号、63号。按需要消费既有51/50/49C和48限定，不重做已完成接收和验收。
继续同一C04c的76接收及本人49 MANIFEST已加载项目/指导记录，保持身份和单写者边界，不导入其它项目维护。
固定time a039d9803ade；核双金丝雀、direct master/time/lead/执行头、PR16/17、开放PR和唯一写区。首次执行写前预期ae6ce5353844，新目录应不存在；未知后继或同名成果先核归属，不静默改pin/强推。
共同源台账记路径、固定ref、blob短身份、实际读取范围、定义符号及本轮新读/已读复用；不反复下载同一固定文件来充当额外工作量。

先在本批新六件内完成R52-01：
- 本批MANIFEST自身必有produced_by、model_per_owner、read_channel、startup_selfcheck_quote、branch_canary_quotes、self_check三个计数和未执行项；界面身份不可直接读取则produced_by=unknown并解释，不认证后台。
- 增设prior_batch52_manifest_supplement，明确supersedes_fields仅补52 MANIFEST的署名与branch_canary_quotes，不能覆盖52原件。两条金丝雀从固定源码重新确认，原文保留tab和反斜杠。
- 正文首节/evidence勘误区保存C14/C16的原范围→正确范围（kthread164–215、kernel_init270–309），独立核函数头尾和原短引文，列消费者；不重审无关65条，不追认67/67全部重验。
- UV-01仍沿用deferred_owner_not_ready，52的deferred_not_ready说明为缩写错误；其它历史状态不变。映射MANIFEST/正文/facts/evidence/deferred/交付评论的消费位置。
- 勘误与53新取证分开计数，只申请R52-01_DOCUMENTATION_CLOSURE_REQUESTED，不能自签CLOSED/PASS。DOT只画53新链，不增加第七件。
普通记录补齐后同轮继续O01–O04，不等第二次Owner指令。若某修正实际无法完成，保全真实缺口，仍可执行不依赖它的安全独立专题。

## 3. O01／A：devtmpfs请求来源、发布和结果读取

固定mykernel/drivers/base/devtmpfs.c：完整读取req_s、devtmpfs_create_node、devtmpfs_delete_node、devtmpfs_submit_req、devtmpfs_work_loop；handle及创建/删除处理函数只展开到足以判断所读字段、错误返回与最早停止点，不扩完整VFS/挂载。
分别追create与delete的请求存储来源：分配返回、栈对象、字段已写/未写、传参对象身份；不能把两条生产者都套成堆分配。列name/tmp/dev指针与req本体、内嵌done、next/err的独立寿命。
核发布到requests的锁、next保存和批量摘链、worker何时读各字段/写err、complete/kfree请求顺序、submitter wait后释放tmp及读取err的顺序。
明确每条路径的首次潜在无效求值；未初始化字段、分配失败或下层不正常返回时，不跳到更晚释放动作宣称确定已发生。
分别形成“caller读结果先于worker释放请求”“worker释放请求先于caller读结果”的条件偏序，判明源码有无足以排除其中一种的同步/所有权依据。通知不自动转移所有权；也不得仅因一个函数叫kfree就冒称实际机器回收已证。
只定点读取必要分配/释放入口契约与已有同根因记录，不展开整个slab。区分非法释放参数候选、结果读取寿命候选、未初始化参数、泄漏候选，避免将同一根因重复编号。

## 4. O02／B：kthread创建信息、栈done及失败生产者

固定mykernel/sched/forkexec/kthread.c：完整读取__kthread_create_on_node、create_kthread、kthread及kthreadd的请求队列消费；按真实包含链定位kthd_create_info_s和必要公开包装/宏。
区分create分配对象、full_name、创建方栈done、被创建任务与self字段；追create->done的写入和xchg到NULL、result的成功/ERR_PTR生产、通知者以及各分支释放责任。
核分配/命名失败、kernel_thread正常返回负值、正常新线程入口、done为NULL的条件分支；源码中被注释的killable等待和取消代码不得当活动机制，也不据注释假定NULL分支可由所选公开路径触达。
若使用xchg的交换/原子语义，定位实际宏/直接实现或明确为条件前提，不扩全架构原子库/内存序证明。
对通知前后最后一次访问create/full_name/done的节点建立偏序。不可把“等待者恢复”泛化为整个生产函数已返回，也不可反过来要求生产函数必须完全退出才能证明每一个对象不再被访问；需要逐对象最后访问证据。
schedule_preempt_disabled、完整fork/任务调度/退出及引用回收保持边界；本题不重做29–44或完整kthread生命周期。

## 5. O03／C：静态复用与一次性设备请求对照

主对象一：mykernel/arch/x86_64/myos/keyboard.c的getcode_done声明/初始化、keyboard_handler、kbd_get_scancode和足以证明重复调用的直接上层。区分completion.done、swait header.count与键盘缓冲count，不能用同名count互相替代。
核同一静态completion能否在所读接口中被反复通知/等待，有无明确重初始化；只审指定路径，不能声称全系统无其它重置。把52已核队列序列接到这个实际命名对象，分别列预先积累done的快路、实际入队后被唤醒、下一通知/下一等待、无重复事件等反例条件。
主对象二：mykernel/arch/x86_64/myos/ide.c的IDE_make_request、ATA_disk_transfer、end_request及直接调用这些结束函数的局部工作循环；必要时读取blkbuf_node_s定义。
追node与DevSpecParams分配、局部done地址发布、req_in_using等共享指针、complete前后对对象的最后访问、等待方结果处理和释放请求。硬件PIO/中断实现、块层完整调度与DMA安全不在范围。
NVMe/XHCI只沿用52已核调用点作尚未展开的对照，不复制三份同样的驱动审计；若确有决定本题结论的差异，最多定点读取相关函数并说明新增信息。
C的产出是静态可复用与一次性对象协议的差异，以及真实后果仍需哪些证据；不把所有completion、所有设备请求一律判坏或判安全，不重新建立CA-04同义缺陷。

## 6. O04：统一交付与验收门

六件严格限定allowed_files，正文/facts/evidence按R52勘误和A/B/C分区；每专题给completed_in_scope或明确blocked-by及覆盖范围。某子项冲突只暂停相应依赖结论，不能删去任务后照抄全完成模板。
提供逐对象表：storage_origin、初始化点、发布点、最后读/写、通知点、释放请求者、所读同步、反例、未知。图边分源码顺序、匹配调用返回与有条件跨上下文关系；一次性/静态对象不得合成一个实例。
强制锚点覆盖每个主要生产者、提交者、通知者、结果读者和释放请求者的真实定义/关键语句，包含本题实际分配、字段赋值、条件门及错误出口。每条VERIFIED仍是固定源单一定义内连续1–5行；声明/条件编译另列CONTEXT，函数头尾独立定位，不能用调用命中当定义。
语义审查与字节/标签计数/JSON/图端点检查分别报告。所有要求字段先做存在性检查；自查与作者声明不冒充主线验收，未执行项写NOT_RUN。无行数、引文数、模型数、新缺陷数配额。
只为新边界设计必要纯文本/有限状态/偏序模型，不重复铺排timeout恒等式或全目录扫描，不运行仓库源码。以既有CA/UV/DS及WW52-U04定位同根因；新非同义候选用CO53-Fxx并明确前提，不自动关闭历史项。

## 7. GitHub发布、重试与停止

仅原PR17分支user-completion-ownership-53/六件和本人新交付评论可写；全部旧件/旧评论、lead/PR16/master/time及PR元数据冻结。C04不并行写执行成果。不新建分支/PR、不合并关闭/强推删分支。
完整执行63号：GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再通常首次加三次、约10/20/40秒或安全只读间隔重试，遵守Retry-After。工具暂缺多轮正规发现，发现次数与真实写请求分开。
保持expected-head/force=false，复用真实对象并查重评论，不覆盖未知并行写入，不绕过认证/策略限制。预期404、本地编排问题和真实远端写失败分记；没有重试不得虚报。
首次执行写前预期ae6ce5353844；优先六件一次提交，写后固定提交/活动分支全文和blob核对，compare仅规定六件A新增、旧件零净改删。本人交付评论单独回读后STOP_WRITING，不自行54。
最终回复首行纯文本：“第53批组合包已交付；PR17提交〔实际短提交〕；交付评论〔实际ID〕；R52-01申请文档闭合；A/B/C〔各自真实状态〕；READY_FOR_MAINLINE_REVIEW。”未完成则写真实部分/阻断状态，不照填完成模板。
统计详情留MANIFEST/评论，回复只用短段落，不让Owner搬运长表格或接收日志。Owner一次转发，包内O00–O04连续执行，最终一次交C04分别复核文档闭合与整包技术验收。
