执行模型：次一档，沿用现有C04a的GPT-6＋Extra。执行位置：原「MYOS2-A-C04a 代码分析执行」，不新建对话，不上传附件。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-INODE-INIT-37
record_type: bounded_inode_initialization_with_prior_documentation_correction
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: unknown
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（沿用Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-04
read_channel: connector
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
mainline_owner: MYOS2-A-C04
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-04
executor_display_name: MYOS2-A-C04a 代码分析执行
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
source_ref: time
source_pin_short12: a039d9803ade
result_base_short12: 3c02d6c020e3
lead_before_publication_short12: d5b6bf140e03
required_review_record: CORE-USER-FAT-NAME-PARSE-36-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FAT-NAME-PARSE-36-review.md
required_review_binding: 与本61号同一最终固定提交
prior_acceptance: RETURN
prior_return_items: [R36-01, R36-02]
prior_return_scope: DOCUMENTATION_ONLY
prior_return_closure_authority: MYOS2-A-C04
prior_return_may_remain_open_during_independent_inode_analysis: true
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-inode-init-37/
allowed_files: [MANIFEST.md, inode-init.md, facts-and-boundaries.yaml, inode-init-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 3c02d6c020e3及此前全部文件
executor_rotation: false
external_execution_started: false
activation_gate: Owner转发本61号给原C04a，固定输入和现场预检通过
phase: 2
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_change_authorized: false
kernel_patch_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
authorization_ref: Owner当前验收指令、18号问题延期、28号同轮纠正与独立后继授权
---

# 第37批：先追加36文档纠正，再核inode初始化链

## 1. 两条工作线及依赖边界

第36批正式RETURN，仅R36-01/R36-02文档问题；其核心静态观察可按主线回执限定保留。此任务不把它擅自改成PASS，也不要求旧成果返写。N00负责定点纠正；N01–N03研究独立的inode构造与字段初始化，不以36争议标签、图或错误自查口径作真值。按N00→N01→N02→N03→N04同轮推进，不为完成N00再等待一条Owner指令。

新取证目标是补CB34-F05曾保留的上游来源：new_inode返回之前究竟初始化了哪些i_size、i_mapping、i_data和操作表字段；FAT所选构造路径如何取得嵌入对象；一次性初始化定义存在与实际调用是否相同。普通源码问题继续记录，不修内核，不生成补丁，不展开完整inode引用/回收、页缓存、权限、挂载或slab实现。

固定主要源：mykernel/fs/vfs/inode.c的new_inode、alloc_inode、inode_init_always、inode_init_once、address_space_init_once及直接相关局部helper；mykernel/fs/fat/inode.c的fat_alloc_inode、fat_sops、fat_build_inode/fat_fill_inode所选边界；include/linux/fs/fs.h和fat.h的实际结构/转换。只在需要证实初始化来源时定点读inode_init的cache创建参数，不递归整个内存管理或文件系统初始化轨道。

## 2. 固定输入与现场预检

完整读取本61号及同提交36主线回执。36原六件固定3c02d6c020e3，按N00所列位置读取，不重做36所有源码和全部56字节检查。R35-S01/S02固定d5b6bf140e03继续有效；34主线回执固定98608f6c58df的R34-S01/S02及34正文/CB34-F05固定bce8404efe6a定点合读。33/32/31、30 DOC-FIX、UV-04/OF30等原限定不改。

沿用C04a已完成的接收、MyOS2工作令/公约/协议v2 P1–P13与18/24/28号及适用Mnemosyne指导；实际新读和继承范围分栏精确记录，不假称刷新所有guard、不导入其它项目维护。

先核time a039d9803ade的options_flags.cmake和panic.c金丝雀，再核master/time/lead/执行头、PR16/17、全部开放PR、写区交集、本人36评论6624419067、新目录缺席。首次执行写前预期PR17 3c02d6c020e3。主线正常推进不等于执行冲突；未知执行头、同名正式成果、并行写入或输入矛盾须先核归属，不换源、不覆盖或回退。默认分支搜索仅定位，语义证据回固定time。

## 3. N00：R36-01/R36-02追加纠正，不能自行关闭RETURN

在inode-init.md开头设置独立章节“第36批追加勘误”，注明原六件固定提交、两退回ID、替代消费位置和本次状态CORRECTION_SUBMITTED_PENDING_MAINLINE。本包仍只有规定六件：36替代DOT放本节的完整dot代码块；inode-init-paths.dot只用于新的37图，二者不混计。

R36-01必须完成：原NP36-A001..A056逐项处置索引；A032拆成两个真实单宏证据；A036只取MSDOS_NAME自身定义，其它宏有使用才另取；A043降为条件编译上下文而非伪造定义符号；F02/F07对短目录项A038/A039的错误支持关系改到正确长槽字段和活动代码。其余原ID可按实际范围保留历史、不冒称重新全量复核。证据文件分“36纠正来源”和“37新取证来源”，新标签前缀分别N36C-A和NI37-A，旧ID不重编号。

同步给MANIFEST/evidence/正文、相关deferred和旧交付评论总括声称的替代消费说明，不编辑任何旧件/旧评论。旧保留数、条件上下文数、实际新直接引文及复核数分开，禁止把原56标签重新笼统签为全部合格。

R36-02必须完成：提供完整替代36图和原节点/边到新图的映射，拆开lookup左门、唯一getter调用与返回判断；一次long回退只对应一次readent；表达所有单元被过滤的零存储分支；不把output writable当源码实际检查；精确放置每轮namelen=0及long选择/检查次序。给C18/C19 helper-only分类、F03两种互斥前缀的精确替代文字，并合读36回执§5的NUL/计数补充。

按实际工具条件分别检查新旧标签索引、两张图的节点/边/端点和关键路径；未执行项如实标unchecked。不设新的固定节点数配额，不为图“能连通”虚构控制边。N00结果同时在MANIFEST和facts.prior_return_items登记，closure=PENDING_C04_REVIEW；执行者不得写CLOSED或把36原RETURN改成PASS。

如N00存在真实输入或证据阻断，保留准确未完成项；仅当N01–N03固定输入和写区均独立安全时可继续这部分，不使用未纠正的内容推结论。安全/权限/并发冲突按相应范围停写，不以“纠正已提交”冒充主线闭合。

## 4. N01：实际分配入口、返回类型与嵌入对象

完整读new_inode、alloc_inode活动体。列superblock、s_op、alloc_inode回调、fallback缓存、返回inode和s_inodes发布的真实次序；区分NULL门、ERR_PTR可能性、普通指针与未能定义良好继续的前缀。只把实际所选生产者的返回作为事实，不由Linux惯例补契约。

定点读fat_sops.alloc_inode绑定和fat_alloc_inode：确认分配的是何种外层对象、返回的是不是嵌入vfs_inode、哪些字段被直接写入。用fat.h::MSDOS_I及msdos_inode_info_s定义说明container_of所需前提；private_idx_info与包含inode的外层对象不得混同。操作表静态赋值不证明任意真实sb必使用它，fallback独立inode与FAT嵌入对象不能无证据混用。

new_inode的列表初始化/加入只核调用及发布顺序、返回和必需前提，不深入链表实现、锁/引用计数或挂载。分配器和kmem_cache_alloc的深层行为保持opaque；不得仅因名字有alloc就推导清零、构造器执行或生命周期完整。

## 5. N02：每次初始化与一次性初始化不能互相替代

完整读inode_init_always，按写入顺序记录i_sb、i_op/i_fop、i_size、i_blocks、i_mapping指向的对象及mapping.host/a_ops等所选字段。对i_mode、private_idx_info、mapping.nrpages/page_array等直接相关但未在该函数赋值的字段，区分“本函数没写”与“整体未初始化”。相关内联helper只核实际需要的赋值，不扩UID/GID或权限审计。

定点合读inode_init_once、address_space_init_once及__address_space_init_once，确认其清零/字段初始化的实际范围；再找本包所选分配链是否真实调用它们。允许检查inode_init里的cache创建参数和必要的一层静态绑定，不能凭定义存在或注释说构造器必执行；没有在已读路径调用不等于整个仓库永不调用。

追踪inode_init_always返回值的实际生产者和alloc_inode防御分支，避免重复“存在非零检查就证明当前实现会返回非零”的错误。正常返回、非返回、无效指针访问不得混写。内存分配失败回收只核本函数可见调用，不签物理释放或并发正确性。

## 6. N03：回接FAT构造与CB34-F05，形成字段来源矩阵

定点回到fat_build_inode/fat_fill_inode及第34批消费者，形成“分配来源→初始化函数→字段写者→返回时状态→FAT覆盖→后继使用→未证前提”矩阵。至少覆盖i_size、i_mapping、i_data.host/a_ops/nrpages/page_array、i_op/i_fop、i_mode、private_idx_info和对象包含关系，明确每一结论的路径与条件。

对CB34-F05只追加更精确的来源解释：若新证据证实某字段在有效new_inode路径已经写入，可以收窄“缺少局部赋值”的风险范围，但不得回写旧文件或自动关闭历史问题；未找到完整生产链则保持缺口。第36批RETURN与这部分初始化结论独立，不能借37取证宣布R36已解决。

不重做FAT目录扫描、名字解析、完整VFS/pathwalk、new_inode之外的全局生命周期、页面读写、iput全链或安全漏洞利用。新条件发现用NI37-Fxx，无数量配额；源码缺陷和报告勘误分栏，不双计问题。

## 7. N04：六件、证据、自查、交付与停止

只新增allowed_files六件。MANIFEST分别登记N00纠正状态和N01–N03取证状态；正文先36勘误再37分析；facts有独立prior_return_items及新字段矩阵；DOT文件只为37；deferred保留旧qualified_id；evidence两个来源区分别统计。每个新VERIFIED只能一个真实函数/结构/宏定义体内逐字1–5行，不混宏，不把#ifndef、extern、include或注释当定义。

规范保留produced_by、product_surface、model_per_owner、read_channel、base_snapshot、inputs_read新旧分栏、金丝雀、公约引用及self_check三数；未知后台身份写unknown。P9各项按实际适用范围记录；图的计数/端点与控制语义分开。允许现有临时环境的纯文本/结构/有限整数模型，不安装、不运行仓库C/ASM/脚本、不预处理/编译/QEMU/硬件、不用额外付费API模型。

唯一执行者CHAT-EXEC-04只向原claude/dazzling-cori-q0dnyt/PR17的user-inode-init-37/新增六件及本人进度/交付评论。全部旧件、lead/PR16、master/time、PR标题/body/Draft及他人评论不改，不创建分支或PR。使用可用expected-head、force=false正常快进；超时先核远端，不盲目重写。

最终固定提交和活动分支分别全文回读六件、核正文/blob/head；compare 3c02d6c020e3至结果只能本目录六件A新增、旧件零净改删且无不明后继。本人交付评论必须分别报告“R36纠正已提交、仍待C04闭合”和“37新分析READY_FOR_MAINLINE_REVIEW”，mainline_acceptance=PENDING。交付后STOP_WRITING，只交C04独立复核，不自行开始38或关闭36的RETURN。

阶段2全部边界不变：无内核/用户程序/旧成果修改、修复补丁、构建运行、本机/仓库外libc/镜像/凭据/挂盘、合并关闭、强推删分支、跨仓维护或阶段3。发布此任务不等于自动投递或后台启动。
