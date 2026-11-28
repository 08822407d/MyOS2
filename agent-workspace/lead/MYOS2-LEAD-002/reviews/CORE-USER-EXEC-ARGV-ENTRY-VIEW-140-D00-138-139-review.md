---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: phase2_historical_documentation_correction_review
followup_id: CORE-USER-EXEC-ARGV-ENTRY-VIEW-140
evidence_class: "冻结138/139实物、140增量及固定源码逐项独立复核"
conversation_display_name: "MYOS2-A-C10 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前系统标识，实际后台与effort未独立认证）"
model_per_owner: "沿用主线独审Pro约定"
date: 2026-11-24
date_timezone: Asia/Singapore
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
lead_before_publication_short12: f897f5ad5abc
correction_pin_short12: cde2d7f994b5
old_139_pin_short12: 0a3075f3f861
old_138_pin_short12: 38a5184142d7
read_channel: connector
status: ALL_FIVE_CLOSED_DOCUMENTATION_ONLY
closed_items: [R138-03, R139-01, R139-02, R139-03, R139-04]
open_items_in_this_D00: []
combined_138: PASS_PENDING_LOCAL
combined_139: PASS_PENDING_LOCAL
original_138_and_139_records_frozen: true
kernel_defects_closed: false
self_check: {verified_claims: 3, quotes_reconfirmed: 3, downgraded_to_inferred: 0}
self_check_scope: "本记录三条新引文；历史作者P6与140作者P6分别保留"
full_local_P9: NOT_RUN
inputs_read:
  - "192合同D00；139独审与139-D00-138原裁定；140报告、facts、evidence、DOT及deferred"
  - "138固定报告/evidence/facts与139冻结同件全文核同；139报告/facts/evidence/DOT的D00涉及部分及92窗索引"
  - "固定exec.c计数、预算、复制、清理、用户及内核入口；syscall.c fork完整宏式函数；auxvec宏和binfmt_elf使用点"
  - "本轮D00所用38个新窗逐字命中；冻结对象/原行/图选择器和六个C样本逐对象比对"
owner_decisions_requested_now: []
open_questions: ["文档纠正不关闭既有源码和运行延期项"]
---

# 第140批D00：R138-03及R139四项全部关闭

依据[192合同](https://github.com/08822407d/MyOS2/blob/f897f5ad5abc/agent-workspace/lead/MYOS2-LEAD-002/192-user-exec-argv-entry-view-contract.md)、[139原裁定](https://github.com/08822407d/MyOS2/blob/f897f5ad5abc/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-ARGUMENT-VECTORS-139-review.md)及[138余项裁定](https://github.com/08822407d/MyOS2/blob/f897f5ad5abc/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-ARGUMENT-VECTORS-139-D00-138-review.md)，本轮逐项裁定五项CLOSED_DOCUMENTATION_ONLY。原作者均为C09a／CHAT-EXEC-21；C10按194承接独立判闭权，不冒称旧C09签署本回执。短码均从连接器输出复制前十二位。

## R138-03：原键转录与系统调用身份已补齐

[140报告19–31行](https://github.com/08822407d/MyOS2/blob/cde2d7f994b5/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argv-entry-view-140/exec-argv-entry-view.md#L19)和[facts四个exact字段](https://github.com/08822407d/MyOS2/blob/cde2d7f994b5/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argv-entry-view-140/facts-and-boundaries.yaml#L15)分别保存完整冻结原行，逐字比对4/4相等：

| 原件位置 | 原字面值 | 实际定位 |
|---|---|---|
| 138报告41 | D00.R137-02.old_window_crosswalk | D00["R137-02"].all_73_old_windows |
| 138 evidence17 | facts.D00.R137_02 | 同上 |
| 138 evidence552 | old_window_crosswalk | 同上 |
| 139 facts443 | D00.R137_02.old_window_crosswalk | 这是错误转录，须与138原连字符拼写分列 |

实际旧138键含73项，new_47_window_index同级含47项。完整execve源宏调用身份及其活动return见[E140-001](https://github.com/08822407d/MyOS2/blob/cde2d7f994b5/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argv-entry-view-140/evidence.md#L16)，与exec.c1460–1464五行相符；fork完整MYOS_SYSCALL_DEFINE0(fork)头及SIGCHLD/kernel_clone活动语句与syscall.c51–52、62–64相符。作者使用源宏式身份，不声称已预处理得到生成函数名，因而无须为不存在的生成名断言另造证明。

原47/46计量和活动行拆分已在139裁定核收，本轮不重开；R138-01/02/04原文档关闭状态保持。**R138-03=CLOSED_DOCUMENTATION_ONLY；138原件及全部已核补正合读，组合PASS_PENDING_LOCAL。**

## R139-01：五次复制与完整30字节已修正

[140时序及地址表](https://github.com/08822407d/MyOS2/blob/cde2d7f994b5/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argv-entry-view-140/exec-argv-entry-view.md#L35)、[机器逐段记录](https://github.com/08822407d/MyOS2/blob/cde2d7f994b5/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argv-entry-view-140/facts-and-boundaries.yaml#L92)与实际DOT相符。独立从给定ASCII与NUL复算长度10/5/4/3/8，累积p为T−10/−15/−19/−22/−30；完整低址30字节与冻结139 B.complete_relative_bytes.bytes_hex逐项相同。

旧五个undefined与T0原行作为冻结证据保留，新输出明确半开区间[T−10,T)等；保存exec发生于文件名复制后、环境复制前，exec=T−10，argv[0]=T−30。五段所有字节、顺序和端点均可从新件独立重建。**R139-01=CLOSED_DOCUMENTATION_ONLY。**

## R139-02：无槽读取与真正终止槽、实边计量已分清

[140表58–72行](https://github.com/08822407d/MyOS2/blob/cde2d7f994b5/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argv-entry-view-140/exec-argv-entry-view.md#L58)明确A0两路径未读槽；用户外门为假，内核空向量直接返回。A1/A2/A3真正读槽为[0]、[0,1]、[0,1,2]，终止槽0/1/2；复制阶段再取[]、[0]、[1,0]。

[VERIFIED mykernel/sched/forkexec/exec.c::count_strings_kernel]
```c
	if (!argv)
		return 0;

	for (i = 0; argv[i]; ++i) {
		if (i >= MAX_ARG_STRINGS)
```

[facts真实选择器与别名](https://github.com/08822407d/MyOS2/blob/cde2d7f994b5/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argv-entry-view-140/facts-and-boundaries.yaml#L445)及DOT237–239逐字段对上冻结139：

| from | to | kind | label |
|---|---|---|---|
| D139_P | D139_K | INDEPENDENT_P_TO_K_STRUCT_COPY | 空串 |
| D139_P | D139_J | INDEPENDENT_P_TO_J_STRUCT_COPY | 空串 |
| D139_K | D139_KGET | GETTER_DATA_SOURCE_K_ONLY | 空串 |

冻结139实际120节点/103边，与其机器实物一致；140按节点ID、边四元组计量，未虚构边ID。A0新图边分别标OUTER_GATE_FALSE_NO_SLOT_READ和NULL_VECTOR_EARLY_RETURN_NO_SLOT_READ；计数到复制明确是输入关系，不是完整执行证明。**R139-02=CLOSED_DOCUMENTATION_ONLY。**

## R139-03：地址基准、空串来源和清理条件已补齐

[140六例](https://github.com/08822407d/MyOS2/blob/cde2d7f994b5/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argv-entry-view-140/exec-argv-entry-view.md#L74)及[facts逐例原值与改正值](https://github.com/08822407d/MyOS2/blob/cde2d7f994b5/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argv-entry-view-140/facts-and-boundaries.yaml#L520)分别对照冻结139原模型及旧节点。四个成功模型均预算24，有效限额6,291,456减24为6,291,432；argmin地址T−6,291,432与相对偏移−6,291,432分列。

U0/U1为用户入口合成单NUL，U2为调用者现成用户空串且随后合成条件为假，K2为内核数组普通复制现成空串。四例p=T−20、exec=T−10、argc=1、envc=2，止于bprm_execve调用，调用结果未知。

K0/K1在argc显式赋值前置错并跳清理：

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = count_strings_kernel(argv);
	if (retval == 0)
		retval = -EINVAL;
	if (retval < 0)
		goto out_free;
```

free_bprm在exec.c1094释放bprm；140机器明确post_free_bprm_read_allowed=false，T只作释放前最后可读p。图将错误赋值、goto、free_bprm、putname及正常清理条件下的返回分开，返回边标NORMAL_CLEANUP_RETURN_CONDITION。没有把释放后的字段读取或实际exec完成混入模型。**R139-03=CLOSED_DOCUMENTATION_ONLY。**

## R139-04：92旧ID和三组证据归属已纠正

[facts旧窗1080行起](https://github.com/08822407d/MyOS2/blob/cde2d7f994b5/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argv-entry-view-140/facts-and-boundaries.yaml#L1080)的92个ID、原路径、范围、标题及旧symbol，逐项与冻结139索引及evidence标题/定位一致，92个唯一ID对应91个不同区间；E139-082/E139-090共同为exec.c820–822。旧窗不算140新P6。

| 原问题 | 140证据 | 裁定 |
|---|---|---|
| E139-057使用点冒当宏定义 | E140-039/040，evidence447–464 | 使用点属于create_elf_tables；数值定义在auxvec.h38，已分窗。 |
| E139-043赋ret=0与返回混用 | E140-023/024，evidence266–284 | exec.c386赋值，393实际return；共同支持给定成功路径。 |
| E139-067单独承担正数条件 | E140-029/030，evidence333–353 | 1399–1403前置门与1404赋值分别引用，联合判断明示。 |

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_EXECFN, bprm->exec);
```

E023/E089完整身份另归R138-03，未遗漏也未重复冒计。**R139-04=CLOSED_DOCUMENTATION_ONLY；139原件合读本轮四项补正，组合PASS_PENDING_LOCAL。**

## 收口范围

五项均为文档表述、原值保存、图选择器和证据范围的补正。旧138/139原报告、旧独审及正式评论不改；没有把旧RETURN原件覆盖为新PASS。140的整体接收另见[本轮独审](CORE-USER-EXEC-ARGV-ENTRY-VIEW-140-review.md)。第141批无遗留D00补正包，不为已经关闭的五项另派小回合。

本轮D00相关38个新增源窗逐字核对，96个D节点和74条D边与DOT匹配；这与140全包98窗/193节点/165边不是同一计数集合。完整本地P9和所有构建、MyOS2程序/脚本、Graphviz、QEMU、实机及真实操作仍NOT_RUN；既有源码与运行延期项保留。
