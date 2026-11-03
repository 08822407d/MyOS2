执行模型：次一档，沿用现有C04a的GPT-6＋Extra。执行位置：原「MYOS2-A-C04a 代码分析执行」，不新建对话或重复交接。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LOOKUP-CALLBACK-34
record_type: bounded_FAT_lookup_callback_binding_and_dentry_inode_contract
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
execution_model_tier: next_tier_default
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
required_review_record: CORE-USER-DENTRY-LOOKUP-33-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DENTRY-LOOKUP-33-review.md
required_review_binding: 与本58号同一最终固定提交
required_review_supplements: [R33-S01, R33-S02]
prior_acceptance: PASS_PENDING_LOCAL
prior_returned_scope: []
source_ref: time
source_pin_short12: a039d9803ade
result_base_short12: a6d39cdb22f0
lead_before_publication_short12: 4b39bce5c8c6
master_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-lookup-callback-34/
allowed_files: [MANIFEST.md, lookup-callback.md, facts-and-boundaries.yaml, lookup-callback-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: a6d39cdb22f0及此前全部文件
executor_rotation: false
external_execution_started: false
activation_gate: Owner向原C04a发送本58号执行指令，固定输入和现场预检通过
phase: 2
runtime_evidence: NOT_RUN
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_change_authorized: false
kernel_patch_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
authorization_ref: Owner当前33验收、28号同轮后继持续授权及C04主线职责
---

# 第34批：实际lookup回调怎样返回，并填充原dentry的inode

## 1. 目标与范围

33限定接收，须合读R33-S01/S02。第33批已分开回调返回值和原dentry字段，但没有深入实际文件系统回调；本批跟进所选FAT回调的静态绑定、局部控制流、原对象填充与返回到__lookup_slow的契约。由同一C04a继续，不重新搬运交接附件，不替主线验收。

主要固定源为mykernel/fs/fat/myos_fat32_namei.c中的FAT32_lookup和vfat_dir_inode_operations，以及fs/fat/inode.c的fat_build_inode/fat_fill_inode中直接相关字段和返回。到必要定义与一层直接消费者为止，不完整展开目录盘IO、长短名编码/解析算法、簇链、挂载、权限、RCU/锁/引用、创建/删除/重命名、分配器或new_inode全生命周期，不修内核、不生成补丁、不编译运行。

## 2. B00：固定输入、规则与现场

完整读取本58号和同一固定提交的33主线回执。定点消费33的dentry-lookup.md D02/D03、facts::slow_lookup_matrix、DL33-F02/F04以及相关证据；不重做快查、分配和48标签。31 R31-S01/S02、32 R32-S01/S02、30 DOC-FIX及UV-04/OF30原限定继续有效。33图应按30显式节点/36边消费，旧件冻结。

沿用既有接收与已加载项目/Mnemosyne规则。补核此前清单未明确列出的57/01固定启动指令和45号轮换文件中适用的通用纪律；只补实际未读部分，记录路径、ref和范围，不重放历史25批或创建PR步骤。不把“applicable guards”概括当精确读取日志。仍适用工作令、协议v2 P1–P13、公约及18/24/28号；无需新一轮只读接收后再等Owner批准。

源码固定time a039d9803ade，先核CMake和panic金丝雀。首写前预期PR17 a6d39cdb22f0；现场核master/time/lead/执行头、PR16/17、所有开放PR和路径交集、新目录及最新交付评论。未知后继或同名正式成果先核归属，不覆盖、不回退；主线正常前进不是执行冲突。默认分支搜索仅定位，证据必须回固定time。

## 3. B01：回调绑定与对象身份

以__lookup_slow的实际函数指针调用、inode_operations.lookup真实类型、vfat_dir_inode_operations.lookup初始化及所选inode.i_op赋值组成可追溯链。分别记录调用点、操作表绑定、对象字段赋值，不用函数名或extern声明冒充运行绑定；仅有静态赋值不证明当前真实parent来自这条路径。

建立parent_inode、其superblock/私有字段、原dest_dentry、dest_dentry.d_name、局部p inode、tmpde目录项、iobuf及私有inode信息对象的身份表。区别别名、借入指针、单独分配和结构字段赋值。输入指针、大小、函数指针、返回值和对象存活均保留真实前提。

## 4. B02：FAT32_lookup的局部控制流与退出

完整读FAT32_lookup活动正文。核destlen/name取得、iobuf初始化及iter_init调用、while短路条件、tmpde赋值时点、空项continue、长短名选择、有限名字比较、释放临时name和匹配break。解析/读目录的底层函数保持有类型opaque；如需类型/宏定义，可在实际定位的fat32.h、fat.h、msdos_fs.h等固定文件定点读，不递归展开它们的算法或IO。

必须区分因匹配break、因条目帮助函数错误、因游标达到FAT_MAX_DIR_SIZE而短路退出，以及首次条件就不进入循环的helper模型。核循环后究竟测试什么、tmpde/长度/游标哪些仍有确定值；没有真实生产者的边界输入标helper-only，不把到达循环尾等同找到目标。

名字比较取实际count及有符号/无符号转换来源；strncmp可读范围和条件编译选择沿33边界。不得把max长度比较自动签为完整文件名合规，也不得跨过未初始化/NULL/ERR_PTR/无效访问推定确定结果。flags形参是否被使用只对完整所选函数作有界结论，不扩展全仓。

## 5. B03：inode构造、原dentry填充及返回传播

跟踪fat_build_inode调用、错误检查、局部dest_dentry变为NULL、两处退出标签、iobuf_release及最终return。判明回调的正常返回类别：原dentry、另一个对象、NULL、ERR_PTR或未能定义良好继续；不存在直接生产者的类别不能凭类型补出。

定点读取fat_build_inode和fat_fill_inode的实际返回门、inode.i_op/i_fop/i_size/i_mapping等相关赋值，区分注释和活动语句。new_inode、iput、iobuf释放和更深分配器保持opaque；需要其保证才能继续的地方明确为前提，不签已获取或已释放全部资源。

核FAT32_lookup中i_blocks、private_idx_info分配、finode各字段写入和dest_dentry->d_inode=p的顺序。明确赋值发生之前的失败或不返回是否已有原对象副作用；分配失败无检查、无效tmpde、非法长度/除数等只记录最早停止点和所需证据，不制作真实攻击样本。

将回调每类实际返回与33的__lookup_slow接收逻辑对接：哪种错误会转换成NULL，哪种可被上层翻译为-ENOENT，何时原对象真的有inode，哪些仍需有效mapping/生命周期。补充DL33-F02/F04和LC32-F03的证据关系，不改历史ID、不自动关闭源码问题；新候选用CB34-*，不设问题数量配额。

至少提供调用者—回调退出原因—返回值—原对象已写字段—上层下一动作—停止推断处的有限矩阵。结合56/33不重做目录项快查或全部路径遍历，不把静态材料接受当运行成功。

## 6. B04：六件、证据、自查与停止

只新增allowed_files六件。MANIFEST逐B00–B04记complete/partial和准确输入；lookup-callback.md讲实际顺序和条件；facts区分静态绑定与运行前提、指针返回与原对象副作用；DOT区分实际调用门、赋值、返回和分析前提；deferred保留反证/旧ID；evidence使用单定义体逐字1–5行引文。

强制证据：__lookup_slow实际回调调用、lookup字段类型、操作表绑定、相关inode.i_op赋值；FAT32_lookup循环条件/匹配和退出测试、inode构造门、私有对象分配/关键字段写、原dentry.inode赋值、两个标签与最终返回；fat_build_inode/fat_fill_inode实际选定返回；所用错误指针/errno和循环上界定义。不能以声明/include或标题代替关键语句。否定结论来自完整所选定义或有界检索，不由短引文证明全仓不存在。

规范记录produced_by、product_surface、model_per_owner、read_channel、base_snapshot、精确inputs_read、金丝雀、startup_selfcheck_quote和self_check三计数。未知精确型号写unknown。图的graph/node默认属性声明不计具体节点；边端点与语义分别核。P9-1/2/3/4/5/7/8/11/13及其他适用项如实记，文本检查不能替代运行。

允许现成临时环境的纯文本/结构/有限整数模型，不安装、不运行仓库脚本/C/ASM、不预处理/编译/QEMU/硬件、不用额外付费API模型。仅CHAT-EXEC-04可在原PR17分支claude/dazzling-cori-q0dnyt的user-lookup-callback-34/新增六件及本人评论；所有旧件、lead/PR16、master/time、PR标题/body/Draft及他人评论保持不改，不建分支/PR。

使用非强推正常快进和可用expected-head。超时先核远端，不盲目重复写。最终提交和活动分支全文回读六件并核正文/blob/head；compare a6d39cdb22f0至新提交须仅本目录六件A新增、旧件零净修改/删除且无不明后继。实际短提交与核验结果写本人PR17交付评论，只申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，然后STOP_WRITING交C04，不自行开始35。

普通B00–B04内部步骤连续推进；真实权限、固定输入、并行写入或安全阻断才停相应动作。阶段2全部禁止边界继续有效：无内核/用户程序/旧成果修改、补丁、构建运行、Owner本机或仓库外libc/镜像、凭据/挂盘、合并关闭、强推删分支、跨仓维护或阶段3。发布任务不代表自动投递或后台启动。
