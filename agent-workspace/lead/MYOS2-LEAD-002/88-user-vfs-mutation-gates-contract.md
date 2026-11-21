执行模型：次一档，继续现有「MYOS2-A-C05a 代码分析执行」，沿用Owner指定GPT-6＋Extra。
先追加第57批文档纠正，再连续完成三个相关VFS专题；共六件交付，一次交回C05。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-VFS-MUTATION-GATES-58
record_type: documentation_correction_and_bounded_vfs_mutation_contract
produced_by: unknown
model_per_owner: gpt pro（沿用Owner最近明确报告）
backend_identity: unknown_or_not_attestable
conversation_display_name: MYOS2-A-C05 内核分析主线续建
date: 2026-11-08
base_snapshot: time
read_channel: GitHub_connector
source_pin_short12: a039d9803ade
result_base_short12: 776297f1904b
lead_before_publication_short12: 11c08d658b39
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
executor_display_name: MYOS2-A-C05a 代码分析执行
executor_rotation: false
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-vfs-mutation-gates-58/
allowed_files: [MANIFEST.md, vfs-mutation-gates.md, facts-and-boundaries.yaml, vfs-mutation-gates-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R57-01, R57-02]
mandatory_supplements: [R57-S01, R57-S02]
activation_gate: Owner向现有C05a转发本固定任务并要求执行，且G00通过
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

# 第58批：VFS创建/删除门、名字边界和退出清理

## 1. 范围与工作量

57底层FAT目录修改核心可保留，但因证据和消费记录问题RETURN_DOCUMENTATION_ONLY，不能当整包已通过。先做D00的有限追加纠正，再做A父路径/末分量传递、B创建/建目录条件和退出、C删除条件及引用消费。不是重做57，不复跑全部FAT、完整dcache或文件生命周期，不实施修复。
工作源是Owner当前交付处理指令、00工作令及18/24/28/63/79，不是Mnemosyne维护任务。阶段2、原主线C05和执行C05a分工不变；不新开会话/分支/PR。用户仅需转发本固定入口，发布本文件不代表外部对话已启动。

## 2. G00：读取、独立性及现场

完整读本88号、同发布提交reviews/CORE-USER-FAT-DIRECTORY-MUTATION-57-review.md（全部R57-01/02与S01/S02）、63号和79号。57六件及评论6717903727固定776297f1904b；只对D00涉及旧位置及实际依赖定点取证，不重做原77全部引文。56维持PASS_PENDING_LOCAL和R56-S01/S02，52/53闭合及53误判撤回不重启。
同一C05a可按实际已读取的固定原文/身份复用87接收、项目九规则及适用指导，并在58 MANIFEST说明；不能把未读的文件称已读，换了新对话则必须重新独立接收，本合同不预授权其它执行者。
核direct time/master/lead/执行refs、PR16/17、全部开放PR、新58目录和写区交集。固定源码仍time@a039d9803ade；回核双金丝雀。首次写前执行头预期776297f1904b；未知变化先核归属，不覆盖、不静默换pin。
只允许新user-vfs-mutation-gates-58六件及本人唯一新正式评论；全部既有结果包括57六件、旧评论、lead/PR16、master/time、PR元数据冻结。C05不并行写执行目录。

## 3. D00：57文档纠正，不能自签闭合

按57回执的关闭条件逐条完成：
1. A32用真实msdos_dirslot_s定义内连续1–5行替代；前置注释单列CONTEXT。必要的布局依据分别锚定，不把一个片段包装成全部布局证明。源台账纠正legacy_B分析别名/实际操作表归属和iobuf blob短身份。
2. 历史状态按真实source_field/source_value保存；R35/R55/R56等限定另列，不再制造canonical_status。SB38-F01仅是超级块ERR_PTR问题，外层inode/私有字段分别回读F02/F04，纠正57跨件关联。
3. 改正length33的删除区间误述、负off模型“before deref”及拒绝/非法访问混淆、N槽/字节单位；合读R57-S01对N=1 helper-only、非零base后缀、end/expand和尾点裁剪的限定。
4. 给57正文、facts、deferred、MANIFEST、evidence、DOT及旧评论的逐项消费/替代索引。旧文件不覆盖；57原77标签、其余76历史保留、新纠正标签、58新证据分开计数，原77/77字面申报不升为定义归属全合格。

纠正放58正文D00节、facts.prior57_disposition、deferred.prior_return_items、evidence纠正分区和MANIFEST；不为纠正多交第七件。图只需提供明确消费映射，必要时在正文内给局部替代图。申请状态只能CORRECTION_SUBMITTED_PENDING_MAINLINE；C05独立核查后才可关闭R57-01/02。
完成D00拟稿即可连续做安全独立A/B/C，不逐项等待Owner；不得使用尚错误的A32/旧状态映射支撑新结论。有未闭合依赖仅暂停该命题，其余继续。

## 4. G01／A：父路径、末分量与名字对象来源

主干mykernel/fs/vfs/namei.c的path_parentat、filename_parentat、filename_create、kern_path_create/user_path_create及直接__lookup_hash；dcache.c的__myos_d_alloc为名字复制来源。完整读取所用定义，定点取link_path_walk产生nd.last/last_type的语句，不重做整套路径解析。
追name错误指针入口、set/restore_nameidata、父path移交/清空、last.name/last.len/type的实际生产；-ECHILD重试的输入/状态是否重建按活动代码判断，不能按“retry”标签猜自动恢复。
分别追路径切片qstr、dentry拥有的名字副本、NUL的生产条件、长度类型及申请len+1/复制len的边界。不是只读qstr结构就宣布所有输入NUL终止，也不把FAT_LFN_LEN宏当真实VFS长度门。实际有效域、溢出、申请失败和ERR_PTR检查的最早停止点须列明。
连接到57所述尾点裁剪/原串strrchr差异：给普通名字、全点、尾点、末尾斜线和长度边界的条件模型；区分真实调用链已证、借用合法对象的helper-only与未知。不能凭模型宣布用户可达或完整规范违反。
消费旧lookup结论时完整合读同固定主线reviews/CORE-USER-DENTRY-LOOKUP-33-review.md及CORE-USER-LOOKUP-CALLBACK-34-review.md；旧名字匹配/回调契约问题沿旧ID，不重编号。涉及path_init/last类型旧结论再定点读31/32回执的相应限定。

## 5. G02／B：create/mkdir的条件、模式和清理

主干filename_create、do_creatat、do_mkdirat、vfs_mkdir、done_path_create及creat/mkdir的MYOS_SYSCALL_DEFINE包装；init.c::init_mkdir仅作内核调用对照。新增是进入FAT回调之前/之后的门，不重新分析FAT槽生成或mkdir分配。
追LOOKUP_DIRECTORY在is_dir保存与lookup_flags裁剪中的区别、LOOKUP_CREATE/EXCL如何传递/消费、LAST非NORM、正/负dentry和尾部字符路径。flag存在不证明callee执行排他检查，变量初值-EEXIST也不等于查到已存在时必拒绝。
对create/mkdir回调缺席、存在、返回0/负值建立路径与资源表；mode掩码、注释掉的检查与实际操作分开。do_mkdirat直接调i_op->mkdir，与vfs_mkdir包装路径分开，不画成必经同一个函数。
按具体退出点追filename/name、parent path、dentry三类对象的获得、移交、done_path_create/path_put/putname调用；直接return是否绕过共同清理由源证，不因出现清理函数名推资源已经真正释放。NULL/ERR_PTR/普通指针不能混为失败同一类。
不把creat返回值自动等同文件描述符，也不将此独立creat路径等同既有open(O_CREAT)路径；未核fd安装/系统调用表不作端到端可用性结论。第39批open完成链不重审。

## 6. G03／C：rmdir/unlink的类型门、回调和引用消费

主干do_rmdir/vfs_rmdir、do_unlinkat/vfs_unlink及rmdir/unlink包装。比较LAST_NORM/DOT/DOTDOT/ROOT、错误dentry、负dentry、目录/非目录、回调缺席/负返回/成功的实际分支；只按可达有效前缀继续。
对照rmdir的d_inode/type门与unlink的实际首次inode读取，区分没有检查、callee返回错误和先行无效解引用。末斜线注释不是已运行的拒绝门，注释中的锁/延迟截断也不是已实现协议。
列dget/dput、path_put、putname、iput的调用顺序和条件、成功S_DEAD与失败分支；读取当前实际helper定义，若实现为空、缺返回或只做部分工作就如实说明。追借用inode指针是否有对应引用增加，不能仅因看到iput推已释放，也不能在引用语义未知时签双重释放。
保留57底层FAT释放与父项标记的限定；这里新增上层门/对象消费，不签打开文件引用寿命、SMP竞态、完整挂载或POSIX正确性。不展开完整inode回收/内存分配器；无法在预算内闭合时留NG58-Uxx。

## 7. 输入预算、交付和验收

主文件namei.c只完整读取上述选定函数及其直接状态生产段；dcache.c读取__myos_d_alloc、__d_lookup、dput及必要初始化尾部。其余至多8个直接依赖文件：实际dget/path_put/putname/iput、qstr/dentry/inode、LOOKUP/LAST/模式/错误/类型及字符串或列表定义；先定位真实路径和定义，不凭Linux记忆补。宏展开/预处理未执行时标源级条件。软预算不允许截断核心定义；外围依赖不足列缺口。
六件为MANIFEST.md、vfs-mutation-gates.md、facts-and-boundaries.yaml、vfs-mutation-gates-paths.dot、deferred-findings.yaml、evidence.md。D00与A/B/C分区；facts至少含source_ledger、prior57_disposition、name_provenance_matrix、create_gate_cleanup_matrix、delete_gate_reference_matrix、finite_model_cases、prior_findings_links。新发现NG58-Fxx、缺口NG58-Uxx，无数量配额。
协议v2 P1–P13全部适用：每条VERIFIED附真实path::symbol、定义内原样连续1–5行，保留tab/空行/CRLF。完整定义支持命题，不用外围注释/声明当定义；短引文不够时分段共同证明而非堆积碎片。图区分CALL/RETURN/DATA/LOCAL_CONTROL/ANALYSIS_STOP及真实出口，不冒称完整CFG。
强制锚点组：D00的A32替代与真实旧条目；A的parentat/create/hash/名字复制；B的两个do入口、vfs_mkdir、done清理及实际模式/flag门；C的四个删除函数和实际引用helper。未能核实必需符号/依赖时相应结论标blocked或needs_evidence，不以INFERRED顶替强制证据。
MANIFEST写P6自检引文、双金丝雀、三个计数、D00与58计数分开、实际规则复用和新读、模型来源及NOT_RUN。允许自建纯文本/结构/有限数学模型；不执行仓库C/ASM/脚本/旧夹具、编译预处理、QEMU/硬件。完整本地P9、作者自查、主线审核和实机结果分层。

## 8. 发布与停止

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。
默认10/20/40秒或安全只读间隔，遵守Retry-After。复用已创建blob/tree/commit；ref结果不明查HEAD，评论先按任务marker查重。工具发现轮数与真实写请求分开，真实认证/策略限制不绕过。
写后固定新提交与活动分支六件全文/blob回读，compare自776297f1904b只允许新六件、旧件零净改删。唯一正式评论返回D00申请及A/B/C状态、实际提交/检查/重试，随后STOP_WRITING交C05独立验收，不自启59、不自签57闭合或58PASS。
不修内核、不出补丁、不安装工具、不访问Owner本机/凭据/外部镜像、不自动使用额外付费API/子代理、不跨仓维护、直写master/time、建分支/PR、合并关闭或强推删分支；本包不变更阶段3门。
