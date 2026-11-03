执行模型：次一档，沿用现有C03c的GPT-6＋Extra。执行位置：原「MYOS2-A-C03c 代码分析执行续建」，不新建对话。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PATH-INIT-31
record_type: bounded_nameidata_initialization_and_path_start_contract
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: ChatGPT普通对话；C04主线
model_per_owner: gpt pro（Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-03
read_channel: connector
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
mainline_owner: MYOS2-A-C04
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
executor_display_name: MYOS2-A-C03c 代码分析执行续建
execution_model_tier: next_tier_default
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
required_review_record: CORE-USER-OPEN-FLAGS-30-DOC-FIX-01-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-OPEN-FLAGS-30-DOC-FIX-01-review.md
required_review_binding: 与本54号同一最终固定启动提交
prior_batch30_verdict: PASS_PENDING_LOCAL
prior_documentation_returned_scope: []
prior_mandatory_errata_short12: e33103ca7d9e
source_ref: time
source_pin_short12: a039d9803ade
result_base_short12: e33103ca7d9e
master_observed_short12: e8e60cd19540
lead_before_publication_short12: 7fb723be2344
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-path-init-31/
allowed_files: [MANIFEST.md, path-init.md, facts-and-boundaries.yaml, path-init-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: e33103ca7d9e及此前全部文件，包括30原六件与DOC-FIX两件
activation_gate: Owner向现有C03c转发本54号执行指令，固定输入和现场预检通过
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
authorization_ref: 28号同轮后继约定，当前纠正包独立复核及C04主线写权
---

# 第31批：路径从哪里开始——nameidata、根目录、当前目录与入口参数

## 1. 目标与不可扩大的边界

30原六件合读DOC-FIX-01及本轮复核回执后，以PASS_PENDING_LOCAL限定接收；R30-01/R30-02已由C04关闭，历史RETURN不改。无需再修30文档或重放接收。继续C03c和原PR17，主线与验收对象仍C04。

本批接续30传入do_filp_open的边界，只分析nameidata怎样初始化、filename字符串怎样交给path_init、绝对/相对/空路径怎样选取起点，以及dfd、可选root、lookup标志在这些直接函数中怎样传递或消费。主线仅做固定源码范围预检，不预填31的全部结论。

不展开逐分量查找算法、dcache/FAT查找、挂载穿越、符号链接/点点全链、完整权限或openat2合规、完整fs_struct生命周期/锁竞争、fd分配、文件引用、内核修复和运行。不重做getname复制、open flags正规化或ELF。普通发现按18号延期，不能因为发现代码问题就要求Owner马上裁定修复。

## 2. N00：输入、配置、已关闭项与历史限制

完整读取本54号及同一固定提交的30-DOC-FIX-01主线回执。消费已完成的errata@e33103ca7d9e：旧30关键引用必须通过其映射，不把原80标签重新整体认定合格；无需为启动31全文重做旧六件。29主线回执R29-S01/S02/S03及30原回执限定继续适用。OF30-F01的部分初始化前提、OF30-F04覆盖缺口、05::UV-04和08::US-03按已读固定原ID保留，不把文档关闭当内核问题关闭。

本任务适用agent-workspace/tasks/00-gpt-task-protocol-v2.md全部十三条，以及工作令、公约、18/24/28/45与本次单写者约定。先读time固定options_flags.cmake及panic.c金丝雀，不静默换master或动态源码。默认分支搜索只定位，正式证据一律回a039d9803ade。

主要输入（均读所选定义及必要上下文，不强制整文件其他内容）：mykernel/fs/vfs/namei.c中的nameidata_s、set_nameidata、path_init、do_filp_open、path_openat、filename_lookup、path_lookupat、restore_nameidata、terminate_walk、complete_walk；link_path_walk仅入口错误指针、初始last_type、跳过斜线与空串早返前缀。定义配套为mykernel/include/linux/fs/path.h、mykernel/sched/misc/fs_struct_types.h、mykernel/include/linux/fs/namei.h、mykernel/include/uapi/linux/fcntl.h。所需filename_s定义在include/linux/fs/fs.h；用户open及内核open_exec的实际参数只定点回fs/syscall.c、fs/vfs/open.c、sched/forkexec/exec.c已读接口。

05的user-vfs-baseline.md及deferred-findings.yaml只定点查所选路径起点/根与当前目录相关旧条目，再决定是否已有同根ID；无法完成去重时如实说明范围，不宣称全仓去重。新候选用PI31-*，已有条目用原qualified_id追加。

## 3. N01：对象及初始化顺序

建立filename_s/name->name、nameidata本体、nd.path、nd.root、current->fs->root/pwd、flags/state/dfd的身份与赋值账。完整核set_nameidata中清零、借入指针、dfd保存、path空值和可选root赋值的先后；不要把存下dfd等同后面使用了它，也不要把路径结构体按值复制等同复制inode/dentry对象或已经增加引用。

把正常有效filename、错误指针、NULL、字符区无效、current或fs无效分别作为有条件输入，不假定所有调用者都先做相同检查。对本批真实调用路径已有生产者的情况，与单独helper假设分开；遇未定义访问或非返回即停在有效前缀，不继续推导确定CPU后果。

## 4. N02：path_init的起点与参数消费

完整核path_init读取字符串首字节、设置nd.flags/nd.root/nd.path及返回字符串的真实顺序。分别跟踪绝对路径、相对路径、空串、仅斜线和多前导斜线；斜线跳过发生在哪个函数必须有实际语句，不把传回同一char指针说成已经分配或正规化路径。

对dfd=AT_FDCWD、普通非负dfd、实际sys_open传值及open_exec传值作有限对照；说明在所读函数中dfd只是被保存、实际用于选择，还是需更深未读代码，不能用Linux函数名补入fd查找。可选root输入要分调用者传递、set_nameidata赋值与path_init后续赋值，不把helper可传root等同实际caller传了非NULL。

LOOKUP_IN_ROOT/BENEATH/EMPTY等只核这些起点函数的实际分支和字段写入；“某处只存标志”不等于全仓不存在实现，也不证明边界约束已生效。不同起点或相同起点的纸面模型必须明确其输入身份和有效前提，不构造真实用户攻击样本。

## 5. N03：两个直接调用链与局部退出

至少覆盖do_filp_open→set_nameidata→path_openat→path_init，以及filename_lookup→set_nameidata→path_lookupat→path_init。逐项核实root与dfd实参，而非仅抄函数签名；同名局部flags、nd.flags及open_flags_s的字段不能合并成一个值。path_openat的file取得在path_init之前，须保留正常返回前提，但不展开分配器或重做文件引用。

link_path_walk只核入口的IS_ERR、LAST_ROOT、前导斜线和空串返回，明确边界后交给既有walk/last-component接口，不深入walk_component/step_into/lookup_slow/跟随挂载等实现。需要解释终态时保留opaque调用和未闭合条件，不能声称路径查找成功或访问控制完整。

核restore_nameidata、terminate_walk、complete_walk的活动正文与注释区别；函数名称或空函数调用不证明引用已恢复/释放。path_lookupat将nd.path写给输出并清空局部字段的条件可定向记录，putname与path资源不是同一对象。只做局部退出账，不能升级成全局泄漏、双重释放或并发安全结论。

形成有限输入/字段/返回矩阵：至少覆盖有效绝对与相对串、空串和仅斜线、真实caller的dfd、可选root传递、已有错误指针门、下层非返回。前缀有效、helper-only和未证生产者分栏；不设置漏洞数或表格条数配额。

## 6. N04：六件、自查和强制证据

仅allowed_files六件：MANIFEST逐N00–N04列完成/partial；path-init.md给顺序和对象账；facts-and-boundaries.yaml给输入/字段/别名与资源边界矩阵；DOT区分实际函数调用、字段写、借用数据、条件返回和不透明下游；deferred保留原ID、前提与反证；evidence给真实固定源码证据。不得增第七件或通用检查器。

每条VERIFIED只锚一个真实函数、宏、typedef或结构体定义，片段逐字1–5行且含所声称的关键语句；声明、include和定义外注释仅导航。长定义可以多个连续短片段，不能拼接不连续代码或把其他宏放进一个标签。保留实际宏实例作为该入口定义时要说明，不冒充已展开编译。图中参数层级和数据来源须对照真实语句，不从“不是错误指针”无条件推出有效对象。

强制直接证据：nameidata_s的path/root/name/dfd和flags字段；set_nameidata的清零及name/dfd/root赋值；path_init的字符串来源、root/pwd选择和flags写入；do_filp_open与filename_lookup的实际set_nameidata调用；path_openat/path_lookupat的path_init调用；link_path_walk所选前缀；path_s与fs_struct的root/pwd定义；restore_nameidata/terminate_walk/complete_walk的所选活动正文。强制项不得以INFERRED或声明导航代替，读不到即partial/阻断相应结论。

MANIFEST使用produced_by、model_per_owner、read_channel、base_snapshot、精确inputs_read、startup_selfcheck_quote、branch_canary_quotes及self_check三计数。无法独立知道界面准确型号时写unknown并另列product_surface，不把产品面当后台认证。实际标签计数与新件自查一致，历史引用数另计；未发生的提交/回读只能写pending。P9-1/2/3/4/5/7/8/11/13逐项核适用范围，其余按实际适用性登记，不自签内核运行PASS。

可用现成临时环境做纯文本、结构、计数及有限整数/条件模型，不安装、不执行仓库脚本或C/ASM/预处理/编译/硬件，不调用额外付费模型/API。缺机械工具如实报告，不能把文本匹配替代定义归属与语义支持。

## 7. 写前、回读与停止

Owner向现有C03c发送本54号后才启动。核直接master/time/lead/执行头、PR16/17、全部开放PR及路径交集、新目录和交付评论；首次写前预期e33103ca7d9e。未知执行头变化、同名正式成果或竞争写入先停写核归属，不覆盖、回退或强推。主线正常前进不视为执行冲突，源码变化不静默换固定输入。

唯一执行分支仍claude/dazzling-cori-q0dnyt、PR17、CHAT-EXEC-03，仅本批新目录六件和本人进度/交付评论。所有旧件冻结，不写lead/PR16、不改PR标题/正文/Draft或他人评论，不建分支或PR。普通内部步骤N00–N04连续推进，不逐项等Owner确认；真实安全/权限/输入阻断才停相应动作。

正常非强推快进，支持expected-head即使用；写超时先核远端，不能盲目重复写。固定最终提交全文回读六件，按活动分支核正文/blob/head；compare e33103ca7d9e至交付头须恰本目录六件A新增、旧件零净修改/删除且无未知后继。真实短提交、回读及compare结果记本人PR17交付评论。只申请READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING；正式交付后STOP_WRITING，交C04验收，不自行开始32。

阶段2、普通问题延期及全部禁令不变：无内核/用户程序/脚本/旧成果修改、修复补丁、构建/QEMU/硬件/真实用户输入、安装/挂盘/凭据、Owner本机或仓库外libc/镜像访问、合并关闭PR、强推删分支、跨仓维护或阶段3权限。发布文档不是自动投递或后台执行。
