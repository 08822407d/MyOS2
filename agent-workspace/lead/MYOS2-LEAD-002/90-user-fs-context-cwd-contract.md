执行模型：次一档，继续现有「MYOS2-A-C05a 代码分析执行」，沿用Owner指定GPT-6＋Extra。
58文档项已闭合，59已限定接收；先读主线B25误判撤回，再连续完成三个新专题，共六件一次交付。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FS-CONTEXT-CWD-60
record_type: bounded_combined_fs_context_cwd_contract
produced_by: unknown
model_per_owner: gpt pro（沿用Owner最近明确报告）
backend_identity: unknown_or_not_attestable
conversation_display_name: MYOS2-A-C05 内核分析主线续建
date: 2026-11-08
base_snapshot: time
source_pin_short12: a039d9803ade
result_base_short12: 2b3aec6f71e1
lead_before_publication_short12: 669231eb8e5e
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
executor_display_name: MYOS2-A-C05a 代码分析执行
executor_rotation: false
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fs-context-cwd-60/
allowed_files: [MANIFEST.md, fs-context-cwd.md, facts-and-boundaries.yaml, fs-context-cwd-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: []
mandatory_supplements: [R58C-S01, R59-S01, R59-S02]
activation_gate: Owner向现有C05a转发本固定任务并要求执行且G00通过
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
---

# 第60批：工作目录、根目录、路径输出与taskfs复制共享

## 1. 业务目标及范围

A核chdir和根/pwd设置接口的真实更新与引用请求；B核getcwd逆向拼接、缓冲/返回；C核taskfs的复制、CLONE_FS共享及退出。三专题以同一个taskfs/root/pwd对象为联系，共用一次预检、六件成果和一次主线验收。不是完整路径解析、挂载命名空间、fork/exec、全引用计数或POSIX符合性审计。
权威是Owner持续主线指令、00工作令及18/24/28/63/79；Mnemosyne只作适用行为指导，不继承其维护任务。本包不新建会话/分支/PR，不自行调用额外付费模型。主线发布不等于跨聊天自动投递或执行开始。

## 2. G00：实际历史、输入与单写者

完整读本90号、同发布提交reviews/CORE-USER-VFS-MUTATION-GATES-58-DOC-CLOSURE-review.md和CORE-USER-DIRECTORY-ENUMERATION-59-review.md，以及63/79号。58的R58-01已闭合；B24确实纠正，B25原件本来正确，旧主线指控已撤回。59的两条替代/85条历史表述按R58C-S01重新消费，不再传播B25真实缺陷之说，也不为此再交微型返工包。
59最高PASS_PENDING_LOCAL，R59-S01/S02保持；57/56及52/53既有裁定不变。本包没有D00返工项，MANIFEST仅登记已读闭合/撤回及准确历史计数，不覆盖旧文件、旧评论或自签新的历史验收。
同一C05a可复用87接收、九份MyOS2规则和适用Mnemosyne指导的真实已读固定原文，分别记新读与复用；换新会话则先由主线提供独立接收，不把本合同作为其它身份写权。
核direct time/master/lead/执行refs、PR16/17、开放PR、新60目录与写区交集；首次写前执行头预期2b3aec6f71e1，新目录应未存在。固定源码time@a039d9803ade，核options_flags.cmake和panic.c双金丝雀。未知HEAD/竞争成果/写区交集先停相关写动作核归属，不强推、不静默换pin。

## 3. G01／A：chdir、root与pwd的生产和更新

主干：mykernel/fs/vfs/open.c的MYOS_SYSCALL_DEFINE1(chdir)完整调用体；init.c::init_chdir/init_chroot；mykernel/sched/misc/fs_struct.c::set_fs_pwd/set_fs_root；myos_vfs.c::set_init_taskfs只作初始对象赋值对照，不展开磁盘/GPT/完整挂载。
实际追user_path_at或kern_path成功/失败结果怎样影响pwd/root；比较用户chdir的直接结构赋值与setter的path_get、锁、保存旧path、替换、path_put请求。不要凭LOOKUP_DIRECTORY位、函数注释或接口名证明目录类型验证、权限许可或实际引用增减。
只定点读取直接path helper与path_init/filename_lookup/parent输出契约；完整路径解析作为已有条件边界，不重跑31/32全部链。区分pwd/root作为一对指针字段的更新、对象存在性、是否保留旧值和真正持有引用。读取taskfs真实定义及path_get/put、dget/put实际活动体；与58已审无操作限制合读，不按名称补出Linux完整协议。
建立有效path、lookup负错误、缺失对象、相同/不同root或pwd的有限矩阵；锁调用顺序和注释中的seqcount分开。没有证明所有读者遵守同锁时，不签全系统SMP一致性或具体竞态已发生。

## 4. G02／B：getcwd逆向拼接、容量及返回契约

主文件mykernel/fs/vfs/d_path.c完整读取。核心prpndbuf_s、DECLARE_BUFFER（仅实际使用时）、myos_prepend、prepend_name、__prepend_path、myos_prepend_path与getcwd宏调用体；extract_string/prepend_char只在确有调用或必要对照时纳入，不因同文件存在就画成必经。
追起始pwd/root读取、NUL写入、从dentry/mount向父级回溯、到root条件、挂载根/父关系、detached/escaped返回及unreachable前缀。源码restart标签不等于存在跳回；注释中RCU/重命名重试不能算活动同步。循环终止以合法有限父链为前提，不能由模型认证任意拓扑。
按真实int/uint32/size_t类型追剩余len、dlen窄化、dlen+1、buf向前移动、内嵌NUL、name实际长度与声明长度差异；只在算术和对象有效范围内推进。分别记录helper布尔返回、len变负、是否已memcpy和最终errno，不能把返回true自动等同写入完成。
核PATH_MAX申请及失败门、len>PATH_MAX/len>size、copy_to_user返回消费、bufhead清理及实际成功返回表达式。源码注释的“长度”或libc约定不能替代活动return；涉及指针转long再转int时仅给显式目标模型，不预设编译结果/用户ABI或安全影响。
有限例覆盖root本身、普通多级路径、跨挂载关系、恰好容量/差1、过长、输出size0/不足、分配失败及不可达前缀。用户copy或更早无效指针处停止，不推后续kfree/返回。禁止读取外部libc镜像、运行用户程序或修复getcwd。

## 5. G03／C：taskfs复制、CLONE_FS共享和退出

主干fs_struct.c::copy_fs_struct/exit_fs/free_fs_struct、forkexec/fork.c::copy_fs及其直接调用前后的task复制/失败释放片段；必要时定位实际exit_fs直接调用。init_fs为起始字段来源；不得把taskfs_s与files_struct_s、CLONE_FS与CLONE_FILES或原43文件描述符表共享混同。
CLONE_FS分支要说明tsk->fs“已是目标值”的实际生产者，in_exec拒绝前是否加users、成功是否加计数、当前锁是否注释；非共享分支核分配NULL、初始化users/in_exec/lock/umask、复制root/pwd及path_get请求。不能只引用注释完成父对象来源证明，也不把结构复制说成深拷贝dentry/mount。
exit_fs追tsk->fs清空、users递减/零判断、锁解除和free_fs_struct，区分持有计数、对象指针断开、path_put请求和分配器释放请求。给单持有、多持有、共享拒绝、复制分配失败及有界失败清理模型；非法初始计数、未知taskfs来源另列缺口，不推任意下溢/双重释放或全fork正确性。
仅追必要taskfs生产消费。需要展开的直接函数必须读完整定义；copy_process/do_exit只读明确片段及相应前置来源，不扩整个函数所有子系统。旧主线若已审同根因，仅增consumer/反例证据，不重编号。

## 6. 输入预算、强制证据及六件

核心文件为d_path.c与fs_struct.c全文，open.c/init.c/myos_vfs.c/fork.c仅上述选定完整定义和直接taskfs片段。额外依赖软上限8个文件，优先taskfs/path/task定义、fs_struct头、path/dentry引用helper、实际CLONE_FS/类型/限长/usercopy宏及直接exit调用。宏展开/编译未做应标源级条件。不得为守软预算截断主干定义后签完成，外围缺口记FC60-Uxx。
消费旧结论前按真实文件名定位并完整读相关回执限定：58及59本提交回执强制；依赖31/32路径结果、29名字拷贝或43 CLONE_FILES对照时再定点读取对应旧回执/原条目，保留qualified_id及真实source_field/source_value。不能假定本批已穷尽历史去重。
六件：MANIFEST.md、fs-context-cwd.md、facts-and-boundaries.yaml、fs-context-cwd-paths.dot、deferred-findings.yaml、evidence.md。facts至少含source_ledger、prior_review_disposition、fs_update_matrix、cwd_buffer_return_traces、fs_copy_share_exit_matrix、finite_model_cases、prior_findings_links。新候选FC60-Fxx/缺口FC60-Uxx，无行数、引文数、模型数或发现数配额。
适用协议v2全部P1–P13。强制源码锚组：A真实chdir宏调用体/setter/直接引用helper，B拼接主函数/getcwd及结构类型，C copy_fs/copy_fs_struct/exit_fs/free_fs_struct及taskfs来源片段。真实唯一宏调用体可定位，但不得冒充预处理后的函数。关键未证项标blocked/needs_evidence，不用INFERRED替代必需证据。
每条VERIFIED取单一定义内原样连续1–5物理行，片段内空行计入；跨宏拆分。分别检查字面连续、行数、定义归属、语义支持。审查如怀疑差异，先展示同pin原工件和源片段比较，不能只相信前次AI判定；B25误判不降低真实P2门，也不能成为一律退回的借口。
MANIFEST须有P6原句、双金丝雀、三个自查计数、历史与新证据分账、实际模型/规则复用/新读及NOT_RUN；图分CALL/RETURN/DATA/LOCAL_CONTROL/ANALYSIS_STOP，不冒称完整CFG。允许临时自写纯文本/结构与有限数学模型，不运行仓库脚本/旧夹具或内核代码。

## 7. G04：发布、重试和停止

仅新增原PR17的user-fs-context-cwd-60六件和本人唯一正式交付评论；所有旧件/旧评论、lead/PR16、PR元数据和master/time对执行者冻结。C05不并行写执行成果。作者只能READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，最高主线接收PASS_PENDING_LOCAL。
GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。
默认10/20/40秒或安全只读间隔，遵守Retry-After；复用已建对象，ref结果未知先查HEAD，评论marker查重；认证/策略限制不绕过。写后固定新提交与活动分支六件全文/blob/HEAD、compare从2b3aec6f71e1严格六件新增/旧件零净改删，真实结果后置记入唯一正式评论，不预造自身SHA或成功。
完成后STOP_WRITING，将真实短提交、正式评论与A/B/C状态交C05独立验收，不自启61。不得修改内核、出修复补丁、预处理/编译/链接、运行QEMU/硬件、安装工具、访问Owner本机/凭据/外部镜像、自动付费API/子代理、跨仓维护、建分支/PR、合并关闭或强推删分支；阶段3门不变。
