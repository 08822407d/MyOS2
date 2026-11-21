---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-FS-CONTEXT-CWD-60-REVIEW-001
record_type: independent_bounded_static_acceptance_with_mandatory_supplements
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: unknown
model_per_owner: gpt pro（沿用Owner最近明确报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-08
base_snapshot: time
read_channel: GitHub_connector
source_pin_short12: a039d9803ade
contract90_short12: 94777b770c43
reviewed_base_short12: 2b3aec6f71e1
reviewed_head_short12: 1a6ef1c7a5aa
reviewed_delivery_comment: 6721849539
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
mandatory_supplements: [R60-S01, R60-S02]
subjects: {A: ACCEPTED_IN_SCOPE_WITH_LIMITS, B: ACCEPTED_IN_SCOPE_WITH_LIMITS, C: ACCEPTED_IN_SCOPE_WITH_LIMITS}
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original104_automatic_byte_scan: NOT_RUN
original_JSON_parse_replay: NOT_RUN
reviewer_finite_model_cases: 50
graph_blob_short12: f82e90cc2870
graph_bytes: 6700
graph_nodes: 66
graph_edges: 87
graph_undefined_endpoints: 0
graph_text_blob_match: true
self_check: {verified_claims: 3, quotes_reconfirmed: 3, downgraded_to_inferred: 0, scope: 本回执三段定义内短引文人工回源，非原104条全量自动闸门}
next_followup: CORE-USER-FD-CONTROL-61
next_task_ref: ../91-user-fd-control-contract.md
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
phase: 2
owner_decisions_requested_now: []
---

# 第60批独立验收：fs更新、cwd输出和taskfs持有关系

## 1. 裁定与远端事实

依据90号及六件固定原件，裁定PASS_PENDING_LOCAL，无新增RETURN。A/B/C按R60-S01/S02组合消费；不认证真实系统调用、完整P9、用户ABI或内核运行。C部分与45已审taskfs主干有明显重叠，属于在cwd消费者语境下复用和补充，不能把旧根因算成全新发现。
执行compare 2b3aec6f71e1→1a6ef1c7a5aa为ahead1/behind0、一个提交、仅user-fs-context-cwd-60/六件A新增3190行：MANIFEST78、正文96、facts1419、evidence1138、DOT167、deferred292。旧件零净改删，未出现内核或写区外文件。
PR16/直接lead=94777b770c43；PR17/直接执行=1a6ef1c7a5aa。两PR均Open/unmerged，17保持Draft；direct time=a039d9803ade、master=e8e60cd19540，开放PR第三项为空。评论6721849539申报READY_FOR_MAINLINE_REVIEW、PENDING及STOP_WRITING；这不是主线认证另一聊天全部内部动作。
58 B25本来有效、错误指控已由C05撤回；60已正确消费R58C-S01，没有重开历史项。56、57、58、59及52/53既有裁定不变。

## 2. 核心源码与条件证成

A：用户chdir在路径调用正常成功后直接替换pwd，没有调用set_fs_pwd：
[VERIFIED mykernel/fs/vfs/open.c::MYOS_SYSCALL_DEFINE1(chdir)]
```c
	curr->fs->pwd = path;
```
init_chdir/init_chroot使用setter；setter按path_get、锁、保存旧值、复制新path、解锁、按旧dentry非NULL请求path_put的顺序执行。path_get/put所选dget/dput活动体不产生实际目录项计数增减，挂载计数语句为注释。结构赋值、请求引用、对象有效和真正持有关系必须分开。

B：getcwd从pwd/root逆向拼接；固定PATH_MAX缓冲分配后没有本地NULL门便参与指针算术。myos_prepend可能令len变负但仍return true，不能由布尔值断言完成复制；外层另有长度与输出size门。
[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		else
			error = (long)buf;
```
所读error是int，成功表达式来自用户指针转换而非len变量；这是源码返回值来源差异，不是已测用户ABI。挂载根回溯、detached=2、escaped=3、unreachable前缀、早NUL及容量边界均须合读完整函数及对象前提；restart标签不构成实际重试。

C：copy_fs共享分支先核in_exec，再增加users，不重新赋tsk->fs；共享指针依赖前序*tsk=*orig以及中间没有改写。非共享分支在copy_fs_struct正常返回后先赋新指针再判NULL。后者新对象users=1、in_exec=0、先读old.umask再加旧锁复制root/pwd，不能当深拷贝。
[VERIFIED mykernel/sched/misc/fs_struct.c::exit_fs]
```c
		tsk->fs = NULL;
		kill = !--fs->users;
```
exit_fs解锁后才按kill请求free_fs_struct，后者请求两次path_put及cache释放。copy_fs失败直接到cleanup_files，未取得的taskfs持有不经exit_fs归还；较晚失败经cleanup_fs先exit_fs再fallthrough文件清理。do_exit的exit_files→exit_fs仅是所读局部顺序，前者及相关对象必须正常完成/仍有效。

## 3. R60-S01：后续强制技术限定

1. chdir的错误出口只证明本函数没有执行后置pwd赋值，不证明被调路径查找无任何其它副作用。LOOKUP_DIRECTORY位、名字或注释不能代替类型/权限检查；空dentry成员、NULL path对象与失效fs对象分别处理。旧PI31、FS45、46闭合及47限定保持。
2. 同一fs指针的共享与复制后相同root/pwd值不是同一事情；锁和path_get/put名称不认证SMP一致性、真实引用、永久泄漏或双重释放。静态init_fs来源与fs_cachep释放边界沿FS45-F01；mnt引用缺口沿FS45-F02，不重编号。原45的R45-S01/S02继续约束C部分。
3. getcwd的成功返回“不来自len”不等于数值在所有输入下都不可能恰好等于长度。指针→long及long→int结果依目标实现；本轮32位截断例仅为显式模型，不能把负数/零实例当已经测得errno。成功copy与正确字符串/完整初始化也不是同一保证。
4. prepend_name同时含size_t→uint32_t窄化、dlen+1和int剩余量更新。按显式uint32/int32模型，减法含无符号运算与转回int，不能写成普适的有符号数学减法；本轮未运行预处理或编译。
5. UINT32_MAX的dlen+1回绕为0本身不是有符号溢出，也不自动证明第一次slash写越界。游标处于末尾NUL位置时，slash仍可在界内覆盖NUL；之后首个非零名字字节才可能越界。若名字首字节为NUL，后续写不发生，仍可能丢失原终止符。必须依当前游标和真实可读名字找最早失效，不能把假设巨长qstr当真实用户生产者。
6. 正常单分量长度4094可占4096字节（含slash/NUL），4095在相同模型需4097并进入外层过长门；名字声明长度与首NUL不符时，已保留空间不等于实际复制字符数。父链/挂载对必须有限、有效，无证不能保证循环终止。
7. users的0→-1是可表示整数变化，但不是合法正持有状态；INT_MIN递减、INT_MAX递增不能越过溢出继续推结果。in_exec拒绝早于增加users，不需假设实际增加。共享配平要求同一对象、无其它修改、清理正常到达。
8. copy_fs_struct分配先失败时不读取old成员；分配成功后才可能在old.umask处失效。正常NULL返回才允许caller存NULL及返回ENOMEM，不能替无效对象补安全返回。dup_task_struct的node前缀、复制来源和先行失败仍受旧R43/R44限制。
9. 六项FC60是条件记录，不是六个全新根因或已复现漏洞；八项缺口未因本验收消失。本轮没有完整全历史去重或实际fork/目录/用户缓冲测试。

## 4. R60-S02：图与证据使用范围

DOT全文从连接器文本重建6700字节，完整Git blob匹配远端f82e90cc2870；66唯一节点、87边、零未定义端点，Graphviz未运行。其边为有界摘要：len_gate→usercopy的调用实际属于getcwd；newfs→copyfs表示返回数据而非对象调用；detached/escaped是函数返回状态；old_cond与usersdec的后继须分别受旧dentry/kill条件约束。错误和ANALYSIS_STOP节点不是CPU指令或源码错误处理。
C30的fs_cachep片段只证明全局变量定义，不能据此认证cache已初始化或对象来自该cache；相关C25初始化及真实对象来源另核。get_fs_root/pwd的定义受所在头的条件分支约束，不将其存在当作getcwd调用了它们或所有构建已启用。
原104标签已全文阅读，强制组及核心结论按完整源码核查；作者104/104、21源项、63个JS模型仍分别按作者申报使用，不追认为主线全部自动字节/定义扫描。原facts/deferred未在主线机械重解析。
主线实际运行50个自建有限Python模型：A更新3；B拓扑8、输出size8、容量4、helper9、copy1、返回转换4；C共享5、退出5、失败标签3。全部通过，包含跨挂载及早NUL反例，不是作者63例重放。完整P9、原104自动扫描、Graphviz、仓库C/ASM/脚本/旧夹具、预处理/编译/链接、QEMU/硬件均NOT_RUN。

## 5. 本轮实际读取、复用及后继

60六件及正式评论全文：正文1–75/76–96；facts1–350/351–700/701–1060/1061–1419；deferred1–170/171–292；evidence1–290/291–580/581–870/871–1138；MANIFEST和DOT全文。
新回源d_path.c、fs_struct.c/h、fs_struct_types.h全文，fork.c273–361（完整dup_task_struct）、1110–1145及1385–1420失败分派；exit.c1–195含do_exit。45回执1–100/101至末尾完整回读，保留其源级补充和旧问题身份。
open.c chdir、init.c、myos_vfs.c初始赋值、namei.c路径与引用helper、fork.c copy_fs、dcache helper、类型及两金丝雀复用本同一有效C05对话此前实际取得的相同固定源码，不用执行者引文代替源文件。90、28/63/79、协议v2及九项目规则和适用Mnemosyne指导沿同会话实际已读范围使用，不宣称本轮全量刷新。
后继91号为CORE-USER-FD-CONTROL-61：fcntl分派、描述符复制/引用交接、lseek位置控制三个相关专题，继续现有C05a。仅设计发布，不代执行61，不自动向另一聊天投递。
