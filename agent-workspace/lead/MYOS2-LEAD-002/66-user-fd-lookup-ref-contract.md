接收模型：次一档（默认），建议GPT-6＋Extra。
执行位置：本Project新普通对话「MYOS2-A-C04b 代码分析执行」，不再派给C04a。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FD-LOOKUP-REF-41
record_type: bounded_fd_lookup_validity_and_reference_contract
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-05
produced_by: unknown
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（主线沿用Owner报告）
execution_model_per_owner_suggested: GPT-6
execution_effort_per_owner_suggested: Extra
backend_identity: unknown_or_not_attestable
read_channel: connector
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
mainline_owner: MYOS2-A-C04
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
executor_display_name: MYOS2-A-C04b 代码分析执行
previous_executor: MYOS2-LEAD-002-CHAT-EXEC-04
previous_executor_future_delegation: NO_BATCH41
receiving_model_tier: next_tier_default
source_pin_short12: a039d9803ade
result_base_short12: 46f5327a8cdf
lead_before_publication_short12: a70d22015ae5
fixed_contract_commit: CONTAINING_COMMIT_PINNED_BY_STARTUP_URL
required_review: CORE-USER-FD-ALLOCATION-INSTALL-40-REVIEW-001
mandatory_supplements: [R40-S01, R40-S02]
required_closure: CORE-USER-OPEN-COMPLETION-39-DOC-CLOSURE-REVIEW-001
required_write_guidance: 63-github-write-retry-and-readback.md
required_handoff: 67-executor-continuation/00-C04a-to-C04b-handoff-package.md
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fd-lookup-ref-41/
allowed_files: [MANIFEST.md, fd-lookup-ref.md, facts-and-boundaries.yaml, fd-lookup-ref-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 46f5327a8cdf及此前全部文件
executor_rotation: true
activation_gate: Owner转发固定67启动入口，新执行者接收/规则/现场预检通过
external_execution_started: false
phase: 2
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
---

# 第41批：从fd取回file的有效性、返回标签及引用配对

## 1. 增量目标与禁止扩展

40核的是选择和安装，本批只核安装之后几类取回接口的局部契约。
重点新增fget/__simple_fget的范围、空槽、mask和引用取得条件，以及真实mmap包装层如何消费。
将myos_fdget_pos和__to_fd/fdget系列作为对照，连接fdput/fdput_pos的标签语义。
08::US-01已记录轻量取回未增引用却携带FDPUT_FPUT；不能把它重新包装为新发现，或重做整个writev/fcntl、fork/close/exec轨道。

只分析所选函数及至多一层调用者。vm_mmap_pgoff、vfs_read/vfs_write的数据/映射实现保持opaque；只记录调用前后fd/file/引用相关动作。
不认证完整引用生命周期、VMA或页缓存回收、锁/RCU/并发、权限或磁盘行为。普通问题按18号登记，不修内核、不生成补丁、不编译运行。
UV-04、SB38及前序路径来源缺口仍有效；辅助有效file模型不等于实际用户open已经成功。

## 2. G00：新对话接收与规则加载

完整读取同一固定发布提交的67/00交接、67/01启动、66号、本次39闭合和40验收回执、63号以及最新检查点。
先形成Mnemosyne接收报告，再完整加载MyOS2规则，随后单独按Mnemosyne load命令刷新适用指导；具体入口见67号。
不能继承旧C04a已加载状态来替代新会话实际读取；不能导入Mnemosyne/Alaya/Meta-Agent维护任务。
接收和规则预检通过后同轮执行G01–G04，不要求Owner再发第二条“开始”。

固定源码time@a039d9803ade，首先核mykernel/scripts/options_flags.cmake和mykernel/debug/panic.c金丝雀。
核direct master/time/lead/执行头、PR16/17、开放PR、写区交集及目标目录缺席；读取40交付评论6649633435及后续主线裁定。
首次执行写前预期46f5327a8cdf；未知执行后继、同名成果或竞争写入先核归属。主线合法推进不算执行冲突，不能静默更换固定源码。

定点消费40中fd_array/fd_count、合法索引与安装前提；39组合回执R39-S01/S02和38根/子对象限制。
从固定执行46f5327a8cdf读取user-syscall-stdio-08/deferred-findings.yaml的US-01/02/03及user-vfs-baseline-05/deferred-findings.yaml的UV-09/10/11，按需读各自主线回执。
定位既有13/14对file/VMA引用的限定，仅用于区分包装层临时引用与被调函数的独立持有，不重做旧包；记录实际路径与范围，不编造历史编号。

## 3. G01：三个取回家族的真实输入和出口

完整读mykernel/fs/vfs/file.c的__simple_fget、fget、myos_fdget_pos；mykernel/include/linux/fs/file.h的fd_s、__to_fd、fdget、fdget_raw、fdget_pos。
分别列参数宽度/有符号性、fd_count逻辑上界、固定数组容量、实际下标、槽值读取、引用操作、返回类型和flags。
fget与fdget不是同一函数；普通file指针与fd_s结构、flags的位标记不是同一个对象。

__simple_fget逐语句核：范围比较、读取槽值、是否先判NULL、atomic_long_inc的对象、mask是否被活动体使用、返回NULL/普通指针的生产条件。
对空槽、合法对象、范围外、fd_count超过容量、值为错误编码或未证有效指针分别列最早停止点。
不可把调用者稍后的if(!file)解释成保护了callee更早的对象访问；也不可把任何非NULL槽位当作合法file。
不展开原子指令实现或分配器；仅核必要类型/宏及局部调用。

myos_fdget_pos与__to_fd系列另列有符号到无符号转换、实际数组索引及返回flags。
对负数或大整数只在明确的类型模型与源码转换依据下推演；不能跨越无效下标后再声称返回NULL或-EBADF。
默认分支搜索只定位，所有结论回固定time。源码没有的边界/空槽检查不得在图中补成真实门。

## 4. G02：借用、取得引用与归还标签

完整读file.h::fdput/fdput_pos/fput_light及FDPUT_FPUT/FDPUT_POS_UNLOCK单宏；必要时读fs.h::get_file和file_table.c::fput。
以“实际是否增加f_count”“返回标签是否要求fput”“位置解锁是否活动”“谁持有原槽引用”分别建表。
flags=0或3只按实际赋值和宏解释，不套Linux惯例。评论中的引用约定与实际计数调用分开。

US-01的既有观察作为基线，本批新增有界契约矩阵及fget对照，不重报同义问题。
有限计数模型只在有效对象、正初值、原子操作正常及无其它写者条件下讨论局部变化；到缓存释放调用不等于已证物理回收或实际UAF。
fd_install不增减计数不等于不允许调用者转移已有引用；沿用R40-S01，不自行要求所有安装必须get_file。

## 5. G03：三个局部消费者，深层实现不扩展

A. mykernel/mm/vm_map/mmap.c中MYOS_SYSCALL_DEFINE6(mmap)的活动函数体：定位非匿名门、fget(fd)、随后if(!file)、vm_mmap_pgoff调用、out_fput和返回。
   注释里的ksys_mmap_pgoff不是活动调用。核unsigned long fd到fget(uint)的转换及其与实际下标的关系，不能假定传入的大值已被拒绝。
   原始槽空、逻辑范围外、有效file、匿名路径分别建模；只能对fget确实正常返回的路径解释调用者判空。
   vm_mmap_pgoff保持opaque，只核包装层临时引用及正常返回后的fput；不把它的内部持有/回收计成已审，不展开VMA、页表、缺页或munmap。

B. mykernel/fs/vfs/read_write.c::ksys_read及file_ppos：核myos_fdget_pos返回后的判空、fdput_pos所在真实控制块。
C. 同文件ksys_write：核fdget_pos、判空、vfs_write正常返回后与返回值符号无关/相关的实际清理门。
   B/C只比较取回/归还及前提；不重做读写实现、用户缓冲、IOV、f_pos完整语义、writev或fcntl。

至少区分：callee内失效而无法到达caller判空、正常NULL与-EBADF、正常取回引用、借用却带归还标签、正常数据调用返回、数据callee不返回。
形成“索引来源/转换→实际检查→槽/对象访问→引用取得→返回标签→调用者门→局部归还→最后可证前缀”矩阵。
新候选用FG41-Fxx，不设数量配额；US-01、UV/OF/OC/SB/FD40保持原qualified_id与状态，不能自行关闭。

## 6. G04：规定六件、自查与交付

仅新增allowed_files六件。MANIFEST记录新执行身份、接收报告摘要、MyOS2加载和Mnemosyne独立刷新范围、backend未知、固定输入、金丝雀、公约句、self_check三数及write_retry_log。
接收/刷新记录可写在本MANIFEST或本地临时笔记，不增加第七件，不向Mnemosyne仓库登记维护任务。
正文讲增量，facts含类型/检查/引用矩阵，DOT区分调用、返回、数据与分析前提，deferred记录条件、反证和恢复门，evidence提供固定源单定义1–5行。
每条引文分别检查文本、符号身份与支持强度；#if选择上下文、extern、EXPORT另记，不算定义体；重复/重叠片段不算独立事实。
不把自动计数/字节存在说成全语义认证；转录副本、完整blob身份、结构解析和模型运行分栏，未执行写NOT_RUN。
只使用现有临时环境的纯文本/有限整数模型，不安装，不运行仓库C/ASM/脚本、预处理/编译/QEMU/硬件或额外付费模型/API。

## 7. 63号重试、唯一写区和停止线

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔安全重试，通常首次加三次，默认约10/20/40秒或穿插独立只读工作，遵守Retry-After。
写动作暂时缺席也多轮重新发现；发现次数与实际写请求分开。保持expected-head、force=false，复用已取得对象身份，提交/评论响应不明先查重，未知并行写入不覆盖。
真正认证/策略/写区安全拒绝不绕过；多轮仍不可用才准确报告剩余阻断，并保全可用材料，不直接让Owner搬运长附件。

新CHAT-EXEC-05仅写原claude/dazzling-cori-q0dnyt/PR17的user-fd-lookup-ref-41/六件及本人评论；全部旧件冻结。
不写lead/PR16/master/time，不改PR标题/body/Draft或他人评论，不新建分支PR、不合并/关闭/强推/删分支，不访问Owner本机、仓库外libc/镜像/凭据。
新固定提交和活动分支各全文回读六件、核blob/head；compare 46f5327a8cdf至新头只能本目录六件A新增，旧件零净改删。
交付评论申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING及真实重试/回读结果，随后STOP_WRITING交本C04，不自行开始42。
