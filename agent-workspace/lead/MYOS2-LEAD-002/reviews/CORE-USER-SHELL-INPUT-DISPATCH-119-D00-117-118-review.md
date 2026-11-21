---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-SHELL-INPUT-DISPATCH-119"
record_type: "independent_historical_correction_review"
status: "ONE_CLOSED_TWO_OPEN"
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话自报）"
model_per_owner: "Pro，后台身份与effort未独立认证"
date: "2026-11-21"
date_timezone: "Asia/Shanghai"
phase: 2
read_channel: "connector"
base_snapshot: "time"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "009d3ffdaa7d"
execution_pin_short12: "9957ff1d927f"
batch115: "PASS_PENDING_LOCAL"
batch116: "PASS_PENDING_LOCAL"
batch117: "RETURN"
batch118: "RETURN"
batch119: "RETURN"
R117_02: "OPEN"
R118_01: "CLOSED_DOCUMENTATION_ONLY"
R118_02: "OPEN"
R119_01: "OPEN"
review_set: ["R117-02","R118-01","R118-02"]
eligible_new_windows_after_reclassification: 43
full_local_P9: "NOT_RUN"
build_QEMU_hardware: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/169-user-shell-input-dispatch-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CWD-DISPLAY-118-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CWD-DISPLAY-118-D00-115-117-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FS-CONTEXT-CWD-60-review.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/evidence.md","myinitramfs/myshell/myshell.c","mykernel/fs/vfs/open.c","mykernel/fs/vfs/d_path.c","mykernel/scripts/options_flags.cmake"]
---

# 第119批 D00 · 117、118三项补正独立裁定

依据169号逐项复核，R118-01达到文档关闭条件；R117-02的字面计数、R118-02列举的原错误直接调用边分别得到修正，两项的其余关闭条件未满足。[合同D00](https://github.com/08822407d/MyOS2/blob/009d3ffdaa7d/agent-workspace/lead/MYOS2-LEAD-002/169-user-shell-input-dispatch-contract.md) · [118独立裁定](https://github.com/08822407d/MyOS2/blob/009d3ffdaa7d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CWD-DISPLAY-118-review.md) · [118历史裁定](https://github.com/08822407d/MyOS2/blob/009d3ffdaa7d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CWD-DISPLAY-118-D00-115-117-review.md) · [119作者正式交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7062147403)

| 待审编号 | C08裁定 | 可用范围与剩余差量 |
|---|---|---|
| R117-02 | OPEN | 119的字面计数已对齐；需纠正新P2资格和跨批合格数口径。 |
| R118-01 | CLOSED_DOCUMENTATION_ONLY | 旧对象定位、消费者、配置及R60归属均有回源依据，以下行界补记随件消费。 |
| R118-02 | OPEN | 原错误调用者已改正；跨簇同一次调用、长度门失败到未写缓存的条件路径仍待补。 |

## R117-02 · 对三个快照分别计数

| 批次 | 次序为MANIFEST/正文/facts/DOT/deferred/evidence | 原文中列出的引文窗口 | 字面前缀 | 主线读法 |
|---|---|---:|---:|---|
| 117 | 1/0/0/0/0/88 | 88 | 89 | 额外一次为旧MANIFEST 113行自述。 |
| 118 | 0/0/0/0/0/92 | 91 | 92 | 额外一次为旧evidence第3行；按118裁定，E051/E052须另归普通位置。 |
| 119 | 0/0/0/0/0/46 | 46 | 46 | 全文逐字命中46；三段全局数组初值应列入普通位置，按本次分类余43。 |

协议P2允许函数体、宏定义、结构体定义。IN119-E019是结构体定义；E020/E021/E022依次摘自全局 `shell_internal_cmd[]` 的44–48、49–53、54–56行。表项真实且可查，直接用P04及表内行界作绑定证据；三段不能进入正式P2合格数。E017、E018等实际函数体另证间接调用与检索。119原报告的“真正的定义体短引文”把118的91窗全部记合格，亦与118对E051/E052的既有裁定冲突。请保留历史原计数，118按已裁定的两处重分类为89个候选合格窗，119为43；不修改已提交的字面分布，不把历史数字混入120自检。169要求字面数及逐窗资格两者均能重算，故原编号继续OPEN。[P2原文](https://github.com/08822407d/MyOS2/blob/009d3ffdaa7d/agent-workspace/tasks/00-gpt-task-protocol-v2.md) · [表初始化三窗](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/evidence.md#L249-L298) · [旧新账并列表](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch.md#L27-L38) · [结构体定义与整张表](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L22-L56)

## R118-01 · 证据落实到实际消费者

原CW118-E051/E052已改为IN119-P01/P02（myshell.c59/60），标出存储位置。E001/E002及E026/E028/E030/E031/E032分别定位启动、提示符、cd、pwd与cat。E033为内核chdir第277行lookup_flags赋值，E034/035另证查找、pwd赋值和返回。getcwd的E036读pwd/root，E037分配，E038拼接，E039长度门，E040/E041复制、指针→long→int赋值、释放并返回。固定169视图中的第60批裁定§3 R60-S01第3项确管返回转换与复制独立性，§4 R60-S02管图和证据消费。正文、机器表、evidence及deferred现已写明。[消费者及R60映射](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch.md#L39-L53) · [R60原裁定](https://github.com/08822407d/MyOS2/blob/009d3ffdaa7d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FS-CONTEXT-CWD-60-review.md)

[VERIFIED mykernel/fs/vfs/open.c::MYOS_SYSCALL_DEFINE1(chdir)]
```c
	path_s path;
	task_s *curr = current;
	int error;
	unsigned int lookup_flags = LOOKUP_FOLLOW | LOOKUP_DIRECTORY;
```
[固定源码 274–277](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/open.c#L274-L277)

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
	int		error;
	task_s *curr = current;
	path_s	pwd = curr->fs->pwd,
			root = curr->fs->root;
```
[固定源码 198–201](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/d_path.c#L198-L201)

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```
[固定源码 64–66](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L64-L66)

C08行界补记：119的P06列196–230作getcwd“整段”，最后一行return已入，完整体的闭括号在231行；涉及完整体观察时固定引用196–231。条件复制和返回口径以E036–041及该整段为准。已读活动C配置，不能认证构建现场。此项文档关闭，不重开旧P2对象。[getcwd完整定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/d_path.c#L196-L231) · [P06原位置](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/evidence.md#L593-L603)

## R118-02 · 直接边类型与跨簇路径

对原四项主张查新DOT：main→LF printf、cd→memset/getcwd、kernel getcwd→kzalloc/prepend均有真实DIRECT_CALL；清零→getcwd与分配→prepend为SEQUENCE；pwd/root进入prepend，PATH_MAX提供分配大小。原错边已去。facts第160行却把 `a_loop→a_lf` 记作CONTROL，实际DOT第134行为SEQUENCE。[替代DOT](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch-paths.dot#L125-L183) · [机器替代表](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/facts-and-boundaries.yaml#L152-L182)

用有向搜索从具体刷新调用位置 `b_getcwd` 不能到 `c_kernelgetcwd`，启动位置 `a_mgetcwd` 亦不能到内核节点。C簇另造了 `c_cdcall`、`c_maincall`，其入边分别从函数总入口发出，绕过成功刷新或先清零的前提；原169要求的有序跨簇路径仍缺。长度门失败从`c_len→c_err→c_kfree→c_return`，没有在该路径上接回预清零缓存。唯一进入`c_nowrite`的边来自`c_usercopy`，而长度门失败时源码跳过复制调用。把这条路径与复制失败、未知部分写入及成功完整复制明确分开。必须按实际分支保留旁证，不能用无条件的通用错误边把每个失败都解释成零写入。[C簇复制与缓存图](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch-paths.dot#L171-L201) · [长度及复制分支](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/d_path.c#L218-L231)

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		if (len > PATH_MAX)
			error = -ENAMETOOLONG;
		else if (len > size)
			error = -ERANGE;
		else if (copy_to_user(buf, b.buf, len))
```
[固定源码 219–223](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/d_path.c#L219-L223)

120可合并调用位置别名或显式以同一次调用的连接边标记；核 `cd成功→清零→同一次getcwd→长度失败→未调用copy→原空缓存→pwd或提示符`，也核成功复制与失败后效果未知两个对照。报告中按端点、边类型和必经条件记录结果，facts逐边与最终DOT一致；原118/119图按当前补记有条件消费。[实际调用与别名](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch-paths.dot#L154-L178) · [本轮119新退回](CORE-USER-SHELL-INPUT-DISPATCH-119-review.md)

115/116组合仍为PASS_PENDING_LOCAL；117受R117-02、118受R118-02，均为RETURN。完整本地P9、编译及实机未运行。
