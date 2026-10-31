---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FILE-CACHE-13
record_type: chat_file_cache_static_analysis_contract
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-10-31
read_channel: connector
evidence_class: 后继工作包授权与验收条件；不是第13批执行结果
authorization_ref: Owner报告12完成；28号验收后同轮发布下一工作包的持续授权
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: MYOS2-A-C03a 代码分析执行
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
receiving_model_tier: next_tier_default
instruction_base_short12: 64c80dc4912b
required_review_record: CORE-USER-MM-FAULT-12-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-MM-FAULT-12-review.md
source_branch: time
source_pin_short12: a039d9803ade
result_base_short12: 16b77f23003f
default_branch: master
default_branch_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-file-cache-13/
allowed_files: [MANIFEST.md, file-cache.md, facts-and-boundaries.yaml, cache-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 16b77f23003f及之前全部文件；包括12、11和所有历史勘误
writer_assignment: C03a仅为本批六件及自己的PR17评论单写者；交付后停写待C03验收
previous_executor_delegation: CC02旧委派不恢复；旧本机进程和未推送内容仍unknown
activation_gate: Owner转发本任务入口；C03a核对绑定和写前状态后C00至C04连续推进
external_execution_started: false
prior_status: {SR11-01: CLOSED_IN_BOUNDED_REVIEW, fault12: PASS_PENDING_LOCAL_WITH_REVIEWER_SUPPLEMENTS}
phase: 2
kernel_change_authorized: false
kernel_patch_production_authorized: false
preprocessing_or_build_authorized: false
c_or_asm_execution_authorized: false
qemu_authorized: false
external_libc_read_authorized: false
phase3_authorized: false
merge_authorized: false
runtime_evidence: NOT_RUN
acceptance_ceiling: PASS_PENDING_LOCAL
owner_decisions_requested_now: []
inputs_read:
  - 第12批交付评论、六件全文及facts结构化主体；本轮12主线回执；05主说明第245-350行的既有读/缓存/缺页边界。
  - 固定time的filemap.c第90-200、395-425行及fault.c相关调用方；namei.c第1-200行只确认固定文件与上下文，未读到缓存分配点。
  - page_array默认分支搜索定位filemap.c、fs/vfs/namei.c和include/linux/fs/fs.h；搜索仅导航，13必须回固定time核实具体定义。
  - 当前两PR元数据、直接refs及12增量compare；只作任务设计预检，不冒充13已取证。
open_questions:
  - 缓存数组创建/容量、数据填充完成与指针发布、清理/引用对应尚须本批实际取证。
  - 不以本轮设计预检预判文件读、页缓存或清理一定成功/失败；完整并发和运行仍在边界外。
---

# 第13批：缓存页存在，是否就代表文件数据已经可用

## 1. 目标和启动

12与SR11-01已由主线限定接收，无需再做11/12小型返工。先合读12回执R12-S01/S02，直接使用主线已经给出的字段/条件纠正；不重写旧件或为了这些补注重做整包。

本批接续12的文件回调边界：以mapping->page_array为中心，说明谁创建缓存容器、索引以什么为界、谁填页和消费读结果、缓存与PTE/文件对象怎样持有或释放资源。05已有read迭代器、文件位置及FAT/ATA概览，12已有缺页路由；这些只作为接口和旧问题入口，不复盘全VFS、完整MM、调度或块驱动。

Owner向已有C03a发送：

> 执行CORE-USER-FILE-CACHE-13。完整读取31号固定任务及12主线回执，沿用已有接收和GPT-6＋Extra。按C00–C04连续完成文件页缓存的容量/索引、填页与错误消费、引用和释放边界分析、自查与原PR17回传。只写本批六件，旧件冻结；普通问题入库，不修内核、不重做旧批。完成后停写交C03验收。

这是一个工作包，不是五次审批。C03a自行组织内部读取、核对和文档修订，正常发现记录后继续；只有实际访问/权限/输入冲突或未知并行写入才突出最小阻断，其他安全部分继续。GitHub任务发布不自动唤醒聊天。

## 2. C00：绑定与增量读取

核任务ID、12回执record_id、result_base、source_pin与自己的执行身份。18号问题延期、24/25号执行验收分工、28号同轮派发持续适用；不重做25号接收，不沿用12结束的写权。指令取lead，旧结果固定16b77f23003f，源码固定a039d9803ade；先读options_flags.cmake，保留外部编译变量和未预处理的限制。

旧结果基目录为agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。12仅读文件回调段、有关矩阵/effects与本轮回执；不全量重读202锚点库。05仅读C4/C5、UV-05/UV-06/UV-08及实际相关缓存条目，并合读既有审查/后继限定；08只有涉及file引用/close时才按已审条件定点引用。不得把旧“每个调用都成功/一定不返回”等概括作为新源码前提。

从固定time定位filemap.c的simple_filemap_read、simple_filemap_fault、simple_filemap_map_page、generic_file_mmap/VM回调；沿真实字段定义定位address_space、inode.i_mapping、file.f_mapping和page_array创建。已导航到fs/vfs/namei.c及include/linux/fs/fs.h，但具体函数、配置和活动路径由本批核实，不根据默认分支片段签发结论。

## 3. C01：容器、容量与偏移

先画最小对象表：inode、file、mapping、page_array、page/folio和VMA/PTE之间的指针来源与共享关系。区别容器容量、文件长度、已填缓存页数和对象引用计数；不能凭nrpages字段名字套用Linux含义。给缓存创建的实际调用点、大小公式、初始值、分配失败处理，以及文件零长度/部分末页时的行为。

跟踪vmf.pgoff、文件pos、iov_iter/kiocb位置怎样形成数组下标，列页内偏移、长度、signed/unsigned转换和上界检查的实际位置。至少区分首/中/末页、页对齐结束、零长度、文件末尾及越界索引、已有槽/空槽、数组或页分配失败；可合并同源行，不为凑数生成笛卡尔积。实际源码没有检查就限定到已读函数，不推定标准接口会自动检查。

本批新增重点是容量与状态生命周期；05已记录的迭代器类型/长度公式，只核本批真实依赖的条件，避免重新生成整张旧值表。越界、无效指针、缺返回或未初始化前提下不计算确定返回码、不宣称必然硬件崩溃。

## 4. C02：填页、发布与错误消费

将普通缓存读取、缺页cache-hit、缺页fallback、私有文件写缺页调用同一fault回调的情况分开。沿12已核的do_read_fault/do_cow_fault入口，只展开缓存状态所需调用，不重新分析完整COW或页表分派。

逐语句列：分配page、槽指针赋值、开始/完成后端读、校验返回、标记数据就绪、映射或拷贝给消费者的先后顺序。检查是否有显式有效/加载中/错误状态、引用和锁，或仅以非NULL作为可用判据；是否存在断言/循环/回退及它们真实的控制流位置。特别检查已有缓存页与再次直接调用simple_filemap_fault的契约，不能假设所有调用都先经过map_single_page。

对file->f_op->read/read_iter等间接边给调用点、实际赋值绑定、目标签名三项证据；若不同文件操作集不能穷尽，选一个已核实际后端并明确覆盖。允许定点读取实际FAT32读入口的长度、位置与返回语义，但在ROOTBLK_TRANSFER/块设备接口停边界，不追PIO、中断、ATA线程或重新验证旧completion/调度。

返回与副作用分别登记：负值、零、短读、完整读、未返回/无法定义；调用者是否使用返回值、是否仍发布缓存槽/返回vm_fault_t、残余清零字节是否被误称文件真实数据。只说明实际可达或有前提路径，不能把“源码不检查读结果”自动变成已经发生的磁盘错误。

形成有限状态表：未建容器、空槽、分配后尚未完成读、数据就绪或没有就绪证据、读取失败/短读后、PTE/读迭代器消费、解除使用。每一状态分开列源码事实、推导和unknown；未做并发试验，不声称已证明竞态时序或线性化性质。

## 5. C03：共享、引用与释放边界

给page_array槽持有的page指针、映射到PTE的页、私有cow页、file/inode/mapping的最小所有权表。实测代码中的get/put操作和仅复制指针分开；接口实际返回类型、内部借用对象、引用增减不得共用模糊字段。不能由page_folio调用推断一定加引用。

只定点核已选调用链及对应清理接口：分配/读失败回退、重复填同槽、close/file最终释放、实际存在的mapping/缓存页清理或失效调用；munmap/退出仅追到能证明是否触及缓存的接口边界。定义存在、注释调用和活动调用分开；没有找到连接时记所读函数/关键词范围，不扩张为全树永不释放。对所有共享别名、LRU/回收、并发truncate和完整VMA生命周期不作穷尽要求。

将05 UV-06/UV-08、12文件回调等旧项做qualified_id证据追加，不重复登记同一根因。若旧“文件与mapping有效即可成功”等宽泛摘要与新证据冲突，列准确原文和erratum候选，交C03裁定；不覆盖旧件、不自行撤销旧验收。不要求Owner现在修复或决定策略。

## 6. C04：六件交付与验收

只交付六件：MANIFEST.md、file-cache.md、facts-and-boundaries.yaml、cache-paths.dot、deferred-findings.yaml、evidence.md。无需第七份旧批处置文件，也不另造检查器/日志库。MANIFEST写C00–C04实际完成及限制；正文先给缓存状态与错误消费结论；facts集中保存精简对象/状态/接口/边/锚点；图与facts相符；问题仅新增及旧证据增量；evidence写实际方法、范围、自查和NOT_RUN。

关键源码结论给实际路径、符号及1–5行逐字引文，并核引用是否真正支持条件或边。没有锚点数量指标，不拆空行/闭括号来扩充计数；同一代码段复用ID，需更多上下文记录源范围。call=调用方→被调方，间接调用附绑定，中间层不伪装直调；data=消费方→提供方；init_order=后使用者→先决条件；写入/发布/清理另列effects。active只表示源语句活动，不等于所有输入无条件可达；前置早退与调用者条件须明确。

主动反证：非NULL槽是否已填数据；返回0是否读取成功；cache hit是否安全索引；私有写fault是否满足缓存回调入口契约；持file引用是否等于持page引用；cleanup函数存在是否等于当前路径执行。unknown不伪装成不存在或正确。必要源码获取失败标PARTIAL，不能把TODO当完成；普通内核缺陷不阻断无依赖分析。

可在自己的临时沙箱使用已有文本工具处理已取得固定文本、核YAML/ID/图/引文；不安装、不执行仓库脚本、不跑C/ASM、不操作Owner工作树。人工核对、机械检查及运行观察分开；没有实际调用就NOT_RUN，不复制旧成功计数。无需新模型、Deep Research、额外API或付费服务。

验收门为固定身份/写区正确、缓存状态与返回/所有权表有依据、关键调用条件和图语义一致、旧项不被越权改判、六件齐全及远端回读。最终回传READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING；不自签PASS、不自动关闭旧运行缺口。

## 7. 写入、恢复和交回

继续PR17/claude/dazzling-cori-q0dnyt；首次写前应为16b77f23003f。C03a本批唯一执行写入者，只写指定六件及自己的PR17进度/交付评论；C03独占lead、PR导航和最终验收。禁止新分支/PR、改名、修改PR正文/标题/Draft状态或他人评论，CC02旧委派不恢复。

写前重新核master直接ref、time、执行头、可见开放PR和路径交集。若目录已存在或头不明漂移，先确认是否同批已交付，不覆盖/重置/重复启动。time前进时保留本固定对象作历史分析并登记新头，不静默换证据。lead正常前进不算执行写区冲突。

六件可顺序提交，交付前仅按当前blob SHA修订本批自己新建文件；以后一次自己已核执行提交为期望头。16b77f23003f全部旧件冻结，正式交付后本批也冻结。接口无原子分支头锁须如实保留竞态限制；写调用超时先读远端确认，不能盲目重试。可信部分及时写本批文件或自己的评论，标PARTIAL与安全恢复点，不另造第七件。

写后按最终固定提交完整读回六件，再按分支核blob/头；compare基线至交付头只能有六路径A新增、无旧件净修改/删除及不明提交。后置核验只放交付评论，给实际短提交、六件入口、diff和读取结果，避免循环自引用。交付后停写，Owner只通知C03完成，正文由主线直接读GitHub；C03继续依28号同轮验收和派发后继。

## 8. 原禁止动作不变

不修内核/用户程序/旧脚本/旧成果，不生成内核补丁，不预处理/编译/运行C/ASM/内核/QEMU，不安装、挂盘、读凭据或仓库外libc/镜像，不直写master/time/lead，不合并/关闭PR、删除分支、force-push，不进入阶段3。阶段2的静态材料完成不等于文件数据、缓存一致性或硬件运行正确性已经通过。
