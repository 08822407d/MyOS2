执行模型：次一档，沿用现有C03c的GPT-6＋Extra。执行位置：原「MYOS2-A-C03c 代码分析执行续建」，不新建对话。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LAST-COMPONENT-32
record_type: bounded_last_component_dispatch_flags_and_return_contract
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: unknown
product_surface: ChatGPT普通聊天
model_per_owner: gpt pro（Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-03
read_channel: connector
base_snapshot: source=time；write=claude/dazzling-cori-q0dnyt；lead=agent/MYOS2-LEAD-002
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
mainline_owner: MYOS2-A-C04
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
executor_display_name: MYOS2-A-C03c 代码分析执行续建
execution_model_tier: next_tier_default
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
required_review_record: CORE-USER-PATH-INIT-31-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PATH-INIT-31-review.md
required_review_binding: 与本55号同一最终固定提交
required_review_supplements: [R31-S01, R31-S02]
prior_acceptance: PASS_PENDING_LOCAL
prior_returned_scope: []
source_pin_short12: a039d9803ade
result_base_short12: 45a065270e28
master_observed_short12: e8e60cd19540
lead_before_publication_short12: 867eba5aaff9
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-last-component-32/
allowed_files: [MANIFEST.md, last-component.md, facts-and-boundaries.yaml, last-component-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 45a065270e28及此前全部文件
activation_gate: Owner向现有C03c明确发送本55号执行指令，固定输入及现场预检通过
external_execution_started: false
executor_rotation: false
phase: 2
runtime_evidence: NOT_RUN
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_change_authorized: false
kernel_patch_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
evidence_class: 后继静态工作合同，不是执行成果
inputs_read: [第31批固定六件及主线回执, 固定namei直接函数与相关定义的范围预检, 28号持续授权]
open_questions: [末分量实际返回生产者及标志域消费待执行者取证]
---

# 第32批：最后一个路径分量怎样分派、消费标志并回到外层循环

## 1. 目标和范围

31以PASS_PENDING_LOCAL限定接受，无待退回项，但使用原件必须合读R31-S01/S02。继续原C03c，不重放接收，不改31或30旧件。主线仍C04，发布任务不等于聊天已收到或后台启动。

本批沿31路径起点之后的直接边界，核nd.last.name/len/last_type怎样形成，lookup_last与open_last_lookups怎样处理最后一个分量、传入的flags究竟属于哪一标志域，以及NULL/错误指针/普通指针与外层整数err如何衔接。重点是参数生产者、赋值先后、分派和返回，不是整个文件系统审计。

不展开__d_lookup、__lookup_slow、文件系统lookup回调的完整内部，不展开挂载穿越、符号链接目标追踪、follow_dotdot/choose_mountpoint全链、完整权限/RCU/锁/引用生命周期、文件创建或截断效果。无内核修复、补丁、运行。普通问题按18号登记，不要求Owner选择修复方案。

## 2. L00：固定输入和预检

完整读取本55号和同提交31主线回执，重点R31-S01的指针/整数返回区别、R31-S02的真实门与有效分析前提。固定结果45a065270e28，固定源码time a039d9803ade。先读options_flags.cmake及panic.c金丝雀；适用工作令、公约、协议v2全部十三条及18/24/28/45和C04单写者约定。

31材料只定点读path-init.md的N02/N03、facts中的M04/M05/M16/M17及相应证据；不重做31起点。30需保留OF30-F01、OF30-F04及其DOC-FIX组合消费限制，不能以原80标签独立证明。05正文C4的fs.vfs.path_open及UV-04作定点历史导航，不重做05或全仓去重。

主要固定源：mykernel/fs/vfs/namei.c中的link_path_walk、lookup_last、walk_component、open_last_lookups、handle_dots、path_lookupat、path_openat、do_filp_open；nameidata定义只作必要字段接口。step_into仅为所需直接返回生产者及入参前提，内部跨挂载调用保持opaque。follow_dotdot、__d_lookup、__lookup_slow、do_open作为有类型的边界，不递归扩审。

定义配套：mykernel/include/linux/fs/namei.h的LAST_*与LOOKUP_*，include/uapi/asm-generic/fcntl.h及include/linux/kernel/fcntl.h的O_*，include/linux/kernel/err.h的错误指针转换，include/linux/lib/errno.h及其实际包含链的ENOERR/错误码。所需qstr_s字段定义在仓库实际定位、回固定time读取，不凭惯例自造路径。open.c及exec.c仅在追溯直接调用者实际标志生产时读已选接口，不重做正规化或ELF。

默认分支搜索只定位，所有语义回固定源。宏条件或源码路径无法闭合时列partial/证据缺口，不靠Linux同名函数或编译器猜测补齐。

## 3. L01：末分量元数据的生产者

完整读link_path_walk的所需控制流，区分入口LAST_ROOT、前导斜线、非空分量测长、点/点点分类、nd.last.name/len/type写入、跨分量继续与末分量返回。明确局部name移动与借入字符串存储、nd.last.name所指位置是不同概念；不要把最后一段自动称为新分配或NUL终止的独立字符串。

有限样例可取空串、仅斜线、普通单分量、尾斜线、点、点点和双分量。双分量中间的实际dentry查找保持有条件opaque；只在该调用正常返回的前提下继续推演。LAST_NORM/ROOT/DOT/DOTDOT取实际枚举定义，ENOERR取真实宏，不把-ENOERR凭名字判成非零错误。

这不是重新分析getname或完整pathwalk。扫描、指针与长度的有效性须保留有效NUL串及已读上游限制；未定义访问、未初始化值、下层非返回出现时停在有效前缀，不构造真实攻击路径样本。

## 4. L02：两种末分量分派与标志域

比较lookup_last→walk_component和open_last_lookups的真实参数、LAST_*分支、普通名字快查/慢查调用及返回门。handle_dots只核本函数的LAST_DOTDOT分支和其它类型的直接返回；其调用的父目录与挂载处理不向下穷尽。

逐层追踪do_filp_open的op.lookup_flags、path_openat的flags、open_last_lookups的open_flag形参及其O_CREAT测试。函数参数名不能决定它属于O_*还是LOOKUP_*；op.open_flag、op.intent、nd.flags也是不同字段。按实际宏值核有关位是否重合、由真实调用者还是helper-only输入产生，不把“O_CREAT用户输入”自动等同“该helper测试位为真”。

对open_last_lookups的局部dentry建立声明/各分支赋值/首次消费账，特别核被注释分支与标签跳转。若某条件下对象未获确定赋值，只登记该前缀风险及真实生产条件，不跨过不确定值使用断言必然创建、打开、越权或CPU终态。相似旧条目用原qualified_id补证，新的候选用LC32-*，不设置漏洞数量配额。

## 5. L03：返回生产者与外层消费

逐函数写明int、const char*、dentry_s*等返回类型；NULL、ERR_PTR(-errno)、普通可读续串指针和未定义/不返回必须分开。只因接口能容纳普通指针，不能声称当前实现确实生产该续串；需要读step_into等直接返回语句或明确为helper模型。ERR_PTR(0)、-ENOERR等必须由固定定义求值，不能按名字归类。

分别核path_lookupat和path_openat的短路while：谁赋err/error，谁赋s，何时不调用末分量函数，何时返回的非NULL错误指针在下一轮由link_path_walk转换，何时正常终止。link_path_walk处理中间walk_component返回值的方式与外层末分量循环另作对照，不把两层消费逻辑合并。

结果到complete_walk、输出path、terminate_walk、do_open/fput/外层return的边只需说明真实前提。旧UV-04缺return仍是确定性结果的边界，不能把“缺返回值”当成“已证明不会返回”；无需重审文件引用或物理清理。形成有限矩阵：输入来源、last_type、标志域及值、各局部变量赋值状态、返回类别、下一次消费者、停止推断位置。

## 6. L04：六件、自查与强制证据

仅allowed_files六件。MANIFEST按L00–L04列完成/partial；last-component.md说明生产者与分派；facts给有类型的输入/赋值/返回矩阵；DOT区分调用门、分析前提、数据/赋值和结果；deferred给前提、反证、旧ID；evidence给单符号真实定义体短引文。不得增第七件、通用检查器或脚本。

每个VERIFIED标签只对应一个函数/宏/结构体/typedef，逐字1–5行且含标题所声称的关键语句；标题涉及后继return或实参时，片段必须实际含它，或者明确缩小标题并另设直接锚点。否定断言依完整所选定义或明确的有界搜索，不靠五行片段证明全函数不存在。声明/include只作导航。

强制证据：LAST_*真实定义；link_path_walk的分类、last字段赋值和两类返回；lookup_last实际调用；walk_component和open_last_lookups的分支与返回门；open_last_lookups中局部dentry赋值/消费；path_openat实传flags；do_filp_open的flags来源；被比较的O_CREAT与相关LOOKUP位真实定义；外层两组while条件；所用错误指针与ENOERR定义；handle_dots及step_into所消费的直接返回来源。关键缺口不能用推断或导航替代。

MANIFEST使用规范produced_by、product_surface、model_per_owner、read_channel、base_snapshot、精确inputs_read、startup_selfcheck_quote、branch_canary_quotes、self_check三计数。无法独立读取精确界面型号时produced_by写unknown，产品面另列，Owner选择单列。标签计数与新增六件实际内容一致，旧批引文不混计；不要为凑80条等数量复制无关片段。

可在现成临时环境做纯文本、结构、计数及有限整数/条件模型；不得安装、运行仓库脚本/C/ASM、预处理/编译/QEMU/硬件或调用额外付费模型/API。逐项记录P9-1/2/3/4/5/7/8/11/13及其余真实适用性，无法机械复现就明示，不把字节匹配冒充语义验收。只申请READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING。

## 7. 写前、提交、回读和停止

Owner向原C03c发送本55号执行指令后，核master/time/lead/执行实际head、PR16/17、全部开放PR和路径交集、新目录及交付评论。首次写前预期45a065270e28。同名正式成果、不明头变化或竞争写入先停写核归属；主线合法前进不构成执行冲突，固定源码变化不静默换源。

沿用claude/dazzling-cori-q0dnyt及原PR17，唯一执行者CHAT-EXEC-03。仅本批新目录六件及本人进度/交付评论；旧件全冻结，不写lead/PR16、不改PR标题/正文/Draft或他人评论，不建分支或PR。

正常快进且支持expected-head即使用，禁止force/reset。写入超时先核远端，不盲目重复写。提交后从最终固定提交全文回读六件，再按活动分支核正文/blob/head；compare起点至结果必须恰本目录六件A新增、无旧件净修改/删除或不明后继。真实短号与核验结果记本人PR17交付评论；正式交付后STOP_WRITING，交C04验收，不自行开始33。

L00–L04普通内部步骤连续推进，不逐项等Owner确认；真实权限、输入身份或数据安全阻断才停。阶段2与全部禁令不变：不修内核/用户程序/旧成果、不生成补丁、不编译运行、不访问Owner本机或仓库外libc/镜像、不安装/挂盘/读凭据、不合并关闭、不强推删分支、不跨仓维护、不进入阶段3。
