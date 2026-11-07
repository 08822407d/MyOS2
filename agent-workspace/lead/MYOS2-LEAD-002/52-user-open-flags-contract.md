执行模型：次一档，沿用现有C03c的GPT-6＋Extra。执行位置：原「MYOS2-A-C03c 代码分析执行续建」，不新建对话。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-OPEN-FLAGS-30
record_type: bounded_open_flag_normalization_and_error_consumption_contract
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: ChatGPT普通对话；C04主线
model_per_owner: gpt pro（Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-03
read_channel: github_connector
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
authorization_ref: 28号同轮验收和后继安排持续约定；Owner本轮要求29独立验收；C04已确认接任
mainline_owner: MYOS2-A-C04
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
executor_display_name: MYOS2-A-C03c 代码分析执行续建
execution_model_tier: next_tier_default
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
required_review_record: CORE-USER-PATHNAME-COPY-29-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PATHNAME-COPY-29-review.md
required_review_binding: 与本52号启动入口同一最终固定提交；不得用活动分支静默替换
required_review_supplements: [R29-S01, R29-S02, R29-S03]
prior_acceptance: PASS_PENDING_LOCAL
prior_returned_scope: []
source_ref: time
source_pin_short12: a039d9803ade
result_base_short12: 239e2f8efc45
master_observed_short12: e8e60cd19540
lead_before_publication_short12: 2a0a66faa76d
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-open-flags-30/
allowed_files: [MANIFEST.md, open-flags.md, facts-and-boundaries.yaml, open-flags-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 239e2f8efc45及此前全部文件
activation_gate: Owner向原C03c明确发送本52号执行指令；固定输入与现场预检通过
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
---

# 第30批：打开标志怎样变成查找意图，校验错误是否真的被消费

## 1. 目标和范围

29以PASS_PENDING_LOCAL限定接收，必须合读R29-S01/S02/S03，原件冻结，无单独返工。继续C03c现有接收，本批为其第6包，不重放45号交接或51号恢复，不另开聊天。主线与验收对象是C04，不再回旧C03。

本批接续29中do_sys_openat2的前缀：从用户int flags/mode或内核打开请求构造open_how，经过build_open_flags形成open_flags_s，再看直接调用者如何消费校验返回、部分初始化字段以及传入do_filp_open的接口。主线已定向读open.c、internal.h、kernel/fcntl.h、openat2.h及do_open_execat，足以设范围；不是本批已完成或缺陷后果已裁定。

不重做路径名测长/复制、ELF/参数栈、完整VFS路径遍历/符号链接/权限审计、fd表分配算法、文件引用/关闭、文件系统实际打开/创建/截断、整个uaccess或运行测试。对相关下游只取第一消费边界。普通问题入deferred，不修内核、不向Owner逐项求修复选择。

## 2. F00：固定输入、历史限定与预检

完整读取本52号及同一固定提交中的29回执；特别保留R29-S02的open前缀范围、R29-S03的引文和MANIFEST标准键要求。适用工作令、公约、协议v2全部十三条及18/24/28/45既有约定；本52号不解除任何禁止动作。

固定源码time a039d9803ade，结果起点239e2f8efc45。最先读取options_flags.cmake及panic.c金丝雀。29仅按接口定点读其pathname-copy.md中P03与回执，不重读整份大型facts。05::UV-04及其主线回执限制必须保留；08::US-03仅作O_CLOEXEC旧根因导航，并合读相关主线限定，不能把旧“所有fd”等总括词直接作为本批已核结论。不重做05/08或全仓去重。

主要固定源：mykernel/fs/vfs/open.c的build_open_how/build_open_flags/do_sys_openat2/do_sys_open/file_open_name/filp_open；mykernel/include/linux/fs/internal.h的open_flags_s；include/uapi/linux/openat2.h的open_how_s与RESOLVE_*；include/linux/kernel/fcntl.h及实际包含链中的O_*、ACC_MODE、S_IALLUGO、MAY_*和LOOKUP_*定义；fs/syscall.c的open入口；sched/forkexec/exec.c的do_open_execat/open_exec。实际下游接口在fs/vfs/namei.c，止于do_filp_open和所需第一层字段消费，不展开完整namei。

工具搜索默认分支只定位，最终证据回固定源。须验证真实定义/别名/类型/配置，不用Linux同名契约代替。若某定义读不到或编译选择未闭合，明确缩为条件模型，不虚构符号或运行绑定。

## 3. F01：从flags/mode到open_how的正规化

完整读build_open_how。逐字段核int flags、umode_t mode和__u64 flags/mode/resolve的转换、掩码与初始化；特别核指定初始化器没有显式列出的resolve在该C初始化模型中的值，不能凭“没写这一项”判成未初始化。

区分旧open入口先清理无效位，与直接helper输入未正规化这两类来源；不得仅因函数名do_sys_openat2或存在openat2.h就宣称真实openat2系统调用已接通。O_PATH优先、允许组合、create-like决定mode、同值别名和ACC_MODE的位域来源分别取证。仅定义/接受某flag不证明该功能已实现。

给必要条件值表：只读/只写/读写及访问位特殊组合，未知位经过或未经过build_open_how，O_PATH附带标志，O_CREAT/非create下mode，O_TMPFILE相关组合。每项写清入口可达来源；不为凑表构造“真实调用者必然能产生”的假输入。有限算式是纸面值，不运行C/真实用户样本。

## 4. F02：build_open_flags逐个返回门与字段写入时点

完整核flags去除位、VALID_OPEN_FLAGS/VALID_RESOLVE_FLAGS、BENEATH/IN_ROOT互斥、mode合法性、__O_TMPFILE组合及写访问门、O_PATH限制、__O_SYNC到O_DSYNC、O_TRUNC/MAY_WRITE、O_APPEND/MAY_APPEND、intent与lookup_flags产生。依源码先后建立表：每个错误返回前哪些op字段已写、哪些未写，不把失败返回的op当全部初始化或全部未初始化。

特别分开局部flags与已赋值op->open_flag：例如后续O_CREAT/O_EXCL分支对局部flags作O_NOFOLLOW变更时，须按实际赋值时间判断是否影响已保存字段，不能仅用变量名相似归并。O_CLOEXEC从open_flags去掉与原how->flags另送fd分配接口是两条不同数据路径；不据此认定close-on-exec已实现或新发现旧US-03。

RESOLVE_*转LOOKUP_*与真正下游执行约束分开。RESOLVE_CACHED的-EAGAIN条件按实际位表达式和O_TMPFILE复合定义计算，不把注释自动等同真条件；将helper构造例与旧open入口resolve来源分别说明。不扩成完整路径限制实现审计。

## 5. F03：校验返回、输出对象与直接调用者

至少对照三个接口：do_sys_open/do_sys_openat2，file_open_name（由filp_open到达），do_open_execat（直接构造open_flags，不一定调用build_open_flags）。每个分别列输入来源、错误返回的实际消费点、成功/失败后是否继续获取filename/fd或调用do_filp_open。

do_sys_openat2必须追踪fd的两次赋值，检查build_open_flags错误有没有被保留/判断、getname提前失败如何改变后续可达性；若可能继续传部分op，明确所选失败分支已经初始化的字段，以及到第一个实际读取位置为止的有效前缀。不要跨过未初始化读取、UV-04或不正常返回去声称必定打开、越权或某种CPU后果。

file_open_name的err门与do_sys_openat2对照；do_open_execat的允许AT_*门、指定初始化字段、LOOKUP_FOLLOW/EMPTY变化及传入边界另列，不套用旧open正规化。do_filp_open读取op->lookup_flags，以及向path_openat传op，只核必要第一层消费；完整权限、路径遍历、文件对象构造和fd安装不在本批。

形成有前提的返回/字段矩阵：有效输入、未知flags/resolve、互斥resolve、mode错误、TMPFILE访问模式不符、O_PATH不允许组合、CACHED拒绝、getname失败、fd分配失败、下游不返回/既有缺return。错误门要有真实输入生产者或明确helper-only标记。新候选用OF30-*；与旧qualified_id同根只追加，不再重新计独立漏洞。

## 6. F04：六件、自查和可消费证据

仅allowed_files。MANIFEST明确F00–F04完成/partial；open-flags.md给来源/顺序/字段/调用者；facts给逐返回门的字段初始化账及有界算例；DOT分调用、宏、数据、真实写入、错误返回和未消费返回；deferred给新项/旧链接与反证；evidence给固定path::symbol、1–5行原样引文及实际读取/检查方法。不得增第七件或通用验证器。

每个关键结论必须有包含关键赋值/返回/比较的直接证据；配置外围、注释、头文件include和声明不能伪装成函数体[VERIFIED]。typedef链、复合flag、宏展开须读真实定义；缺return或部分初始化后只作有界说明。每个新锚点再次回源核文本与符号归属，不把字符串匹配当语义通过。

恢复协议标准键，不继续29的同义字段漂移：MANIFEST须有produced_by、model_per_owner、read_channel（connector）、base_snapshot、inputs_read、startup_selfcheck_quote、branch_canary_quotes及self_check的verified_claims/quotes_reconfirmed/downgraded_to_inferred，最后三个数按真实检查记录并满足协议要求。实际无法独立认证界面/后台的部分写unknown，不自造型号；不得把作者声明当主线复现。逐文件列实际状态，不先写已提交或已回读。

可用现成临时环境做文本、JSON/YAML、图端点及有限类型/整数算式核对；不用外部付费模型/API，不安装、不执行仓库脚本/C/ASM、预处理、构建、QEMU或硬件。报告只申请READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING；没有问题数或引文数指标，缺口如实partial。

## 7. 写前、回读、返回和停止

Owner向原C03c发送本52号固定入口才启动本批。先核直接master/time/执行head、两个PR及全部开放PR/路径交集、目标目录和交付评论。首次写前期望239e2f8efc45；同名成果已交付则不重复执行，未知头变化或竞争写入先停写核归属，不覆盖/回退/强推。主线合法前进不构成执行冲突；固定源漂移不静默换基线。

沿用原PR17及claude/dazzling-cori-q0dnyt，唯一执行者CHAT-EXEC-03。本批仅新目录六件和本人PR17进度/交付评论；旧件全冻结，不写lead/PR16、不改PR标题/正文/Draft或他人评论，不建分支或PR。包内普通步骤连续推进，不逐项等Owner确认；真正安全/权限/输入阻断才停相应动作。

提交用正常快进，支持expected-head即使用；不声称全局锁。超时先核远端，不能盲目再写。固定最终提交全文读回六件，再按活动分支核正文/blob/head；compare起点至结果头须恰本目录六件A新增、无旧件净修改/删除或不明提交。将真实短提交与回读结果写本人交付评论；正式交付后STOP_WRITING，交C04验收，不自行开始31。

普通问题按18号延期。禁止改内核/用户程序/脚本/旧报告、生成修复补丁、运行内核/仓库脚本/C/ASM/编译/QEMU/硬件或真实用户指针样本、安装/挂盘/读凭据、操作Owner本机或读取仓库外libc/镜像、合并关闭PR、强推/删改分支、跨仓写入或进入阶段3。C04继续独占主线规划与验收；发布任务不等于自动投递或后台执行。
