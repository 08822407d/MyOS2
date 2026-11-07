执行模型：次一档，沿用现有「MYOS2-A-C04b 代码分析执行」的GPT-6＋Extra。
不新建对话、分支或PR，不上传附件；独立验收仍交MYOS2-A-C04。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FS-PATH-ACCESS-46
record_type: bounded_fs_root_pwd_access_and_direct_path_reference_contract
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-06
produced_by: ChatGPT主线规划
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（沿用Owner报告）
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
read_channel: GitHub连接器
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
mainline_owner: MYOS2-A-C04
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
executor_display_name: MYOS2-A-C04b 代码分析执行
source_pin_short12: a039d9803ade
result_base_short12: 207c87cda0e6
lead_before_publication_short12: 1a33d35f634c
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
required_review: CORE-USER-FS-CONTEXT-45-REVIEW-001
mandatory_supplements: [R45-S01, R45-S02]
required_write_guidance: 63-github-write-retry-and-readback.md
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fs-path-access-46/
allowed_files: [MANIFEST.md, fs-path-access.md, facts-and-boundaries.yaml, fs-path-access-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 207c87cda0e6及此前全部文件
executor_rotation: false
external_execution_started: false
activation_gate: Owner转发固定72号且P00预检通过
phase: 2
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
---

# 第46批：root/pwd怎样被读取和替换，路径引用辅助体实际做了什么

## 1. 增量目标与边界

45已覆盖taskfs_s的构造/共享/退出。本批打开它尚未主审的四个访问接口：get_fs_root、get_fs_pwd、set_fs_root、set_fs_pwd。
同时独立回源path_get/path_put及其直接dget/dput定义，形成从路径值、调用请求到实际活动效果的闭合局部说明；不以函数名预设引用增减。
45回执的主线预读是定位与消费限定，不代替本执行者独立取证。没有代码修复授权。

对象至少分为fs上下文、root/pwd两个内嵌path_s、调用者输入/输出path对象、path中的mnt/dentry指针值及其所指对象。
不重做45的copy_fs/exit_fs或31完整路径解析，不审完整chdir/chroot、启动、挂载/命名空间、目录项销毁、inode、锁/RCU、slab、fork/exec/exit或权限。
不展开mntget/mntput的全实现和全仓挂载所有权。需要的直接调用者只作参数来源与局部返回消费，不沿其余调用链递归。
普通问题依18号延期；不修内核、不生成补丁、不预处理/编译/运行，不进入阶段3。

## 2. P00：固定输入与现场

完整读取本72号、同固定提交45独立回执全部R45-S01/S02及63号重试规则。
沿用C04b已完成的67接收、MyOS2工作令/公约/P1–P13、18/24/28及适用Mnemosyne指导；实际新读/继承分栏，不导入其它项目维护。
固定time@a039d9803ade；重新核options_flags.cmake和panic.c金丝雀、direct master/time/lead/执行头、PR16/17、开放PR及写区交集。
首次执行写前预期PR17 207c87cda0e6；读45交付评论6675892248及本次主线裁定。46目录应不存在，未知后继或同名成果先核归属，主线正常前进不算执行冲突。

定点读45的path事件/FS45-F01/F02、31::PI31-F05及其主线限定、05::UV-10的path引用附带表述。
必须消费R45-S01对UV-10的收窄：源码调用path_get不等于真实增加dentry/mount计数；不撤销UV-10另一个file.f_count问题，不改其旧状态。
原qualified_id保留，不能把两端都未实现的引用操作直接说成已经一增一减或只增不减。

## 3. P01：直接辅助体、类型与真实活动效果

完整读include/linux/fs/path.h::path_s、namei.c::path_get/path_put、include/linux/fs/dcache.h::dget与相关dentry实际结构、fs/vfs/dcache.c::dput。
不能用extern、注释、默认分支搜索片段或API名称代替定义。若一个体只有return或完全没有活动语句，按实际活动体登记。
区分路径参数本体NULL/不可读、有效path对象中的dentry=NULL、有效目录项指针、确定的指针值与未初始化值；不要把“没有解引用”泛化成任意非法参数求值均安全。
分别列dget返回值是否被上层使用、dput返回类型、局部计数/字段写入、销毁调用，以及mnt字段是否被实际读取或调用引用操作。
空实现没有正常失败errno可供上层检查；未知编译绑定、非法求值、机器异常与源码正常返回分开。

只在实际结构定义内确认相关计数字段是否活动，不从某个字段被注释推断全仓完全没有替代生命周期管理。
若需要解释宏选择，仅定点读fs_struct.h中的DEBUG/FS_STRUCT_DEFINATION与相关PREFIX宏上下文，不运行预处理、不作链接绑定认证。

## 4. P02：get_fs_root/get_fs_pwd的输出时点

完整读sched/misc/fs_struct.h两个真实getter定义及必要选择上下文；列fs锁取得、输出path值复制、path_get调用、解锁的顺序。
区分“写出了地址对”“请求取得引用”“直接helper实际效果”“之后所指对象仍存活”四种命题。
输出参数必须指向有效可写path_s；没有判空不得补成源码验证。若输出原本持有其它路径，函数是否负责归还须靠调用者约定，不凭覆盖就宣判泄漏。
有效源/目标、相同或不同mnt/dentry、dentry为NULL、锁不正常返回、参数失效等分别给最早可证前缀。
对输出与fs内部root/pwd别名的情形仅作helper条件模型；没有真实调用者证据不得当作实际运行输入。

## 5. P03：set_fs_root/set_fs_pwd的替换与旧值归还

完整读sched/misc/fs_struct.c两个setter。逐语句核path_get(input)先于fs锁、旧值保存、目标path赋值、解锁、if(old.dentry)及path_put(old)。
get请求发生、字段发布、旧路径归还和真实引用效果是不同事件；锁名称不证明整个前后阶段原子或全部写者遵循协议。
旧dentry为NULL时的测试并非检查旧mnt；按当前包装体真实操作解释，不引入未活动的mntput。
同一输入与旧路径、root/pwd相同指针、不同mnt但相同dentry、初始空路径、输入path与目标字段别名等均分清值对象与所指对象。
两次get/put请求不自动成为两次计数增减或两次释放；当前dput空体不能画成实际析构成功。
getter/setter均按实际返回类型处理，不能添加-EFAULT/失败回滚或假造错误检查。

定点定位至少一个真实setter调用者的参数来源和调用后消费，可从fs/vfs/namespace.c或fs/vfs/init.c搜索定位后回固定time；只读相关函数的必要周边，不扩整个挂载/chroot。
对getter也定点寻找实际调用点；未找到时说明搜索范围和证据不足，不由搜索缺席宣称全仓无人调用。
每类至多选择一个直接调用场景即可，不用穷尽所有调用者。若调用点直接赋值而绕过helper，应如实区分，不虚构helper调用。

形成“输入/输出path身份→真实门/锁→值复制与请求→直接辅助体效果→发布与返回→尚未证明的生命周期”矩阵。
新增真正非同义候选用FP46-Fxx，无数量配额；FS45-F01/F02、PI31-F05和UV-10原状态保留，追加证据不自动关闭或实施修复。

## 6. P04：六件、自查与验收

仅新增allowed_files六件；MANIFEST记录身份、Owner模型选择与后台未知、read_channel、source_pin/base_snapshot、新读/继承、金丝雀、公约句、自查三数和实际write_retry_log。
facts分离值复制、调用请求、实际字段/计数操作及未知寿命；DOT明确CALL/RETURN、锁区、真实条件、数据关联和分析STOP，不能在成功操作后才分流更早失败。
正文保留空辅助体与假设完整引用实现的区别，不把理论+1/-1模型写成当前源码真实效果。

VERIFIED每段仅固定源一个函数、结构、对象、单宏或单独typedef的1–5行，关键语句须支持标题。
前置struct声明与typedef分开；定义外注释不计变量定义体；#if/extern/EXPORT另列上下文。不要重复45的A004/A042混合片段问题。
否定断言合读完整活动体；字节匹配、符号身份、支持强度、原blob身份、结构解析、图语义和有限模型各自记录，未执行写NOT_RUN。
仅用现有临时环境做纯文本/有限状态模型，不安装、不运行仓库C/ASM/脚本、预处理/编译/QEMU/硬件或额外付费模型/API。

## 7. 安全写入与停止线

完整遵守63号：暂态异常不得因一次失败直接停工；先核远端是否已写成，再通常首次加三次、约10/20/40秒或穿插安全只读工作间隔重试，遵守Retry-After。
写动作暂缺也多轮正规发现，不据此判定Owner无权限；发现次数与实际写请求次数分别记录，不编造操作或后端拒绝。
保持expected-head、force=false，复用已得blob/tree/commit；评论响应不明先查重，不覆盖未知并行写入，不绕过真实认证/策略/安全限制。
多轮仍不可用才准确报告已完成与待补部分，不默认让Owner下载上传长附件。

唯一执行者仍C04b/CHAT-EXEC-05，仅原claude/dazzling-cori-q0dnyt/PR17的user-fs-path-access-46/六件及本人新评论可写。
旧件/他人评论、lead/PR16、master/time及PR标题/body/Draft冻结；不新建分支/PR，不合并/关闭/强推/删除。
首次写前核207c87cda0e6；写后固定新提交和活动分支六件全文回读并核blob/head，compare只本六件A新增，旧件零净改删。
正式交付只申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING及真实回读/重试；随后STOP_WRITING交C04，不自行开始47。
Owner转发且P00通过后P00–P04同轮连续完成，不为普通内部步骤另等确认；任务发布不等于自动投递或后台执行。
