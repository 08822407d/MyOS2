---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PID-LIFETIME-11
record_type: signal10_graph_evidence_link_disposition
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
produced_by: "ChatGPT普通对话，Owner选择GPT-6 / Extra；backend未独立认证"
date: "2026-10-31"
read_channel: connector
base_snapshot: "source=time @ a039d9803ade；old_results=848e2acdf7e1；instruction=69523af0507e"
target_review_record: CORE-USER-SIGNAL-SEND-WAKE-10-REVIEW-001
target_item: SR10-01
disposition: accept
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
prior_verdict: RETURN
runtime_evidence: NOT_RUN
supersedes_locations:
  - "user-signal-send-wake-10/facts-and-boundaries.yaml::edge_direction,SW10-E01,SW10-E12,SW10-E16,SW10-E21,SW10-E24"
  - "user-signal-send-wake-10/send-paths.dot::header,edges SW10-E01/E12/E16/E21/E24"
new_local_ids: [SW11-GN01, SW11-GE01, SW11-GE02]
inputs_read:
  - "29号固定任务书、主线10回执及28号"
  - "旧10固定facts/DOT全部26条边与相关A03/A03b/A18/A19/A23/A29/A34/A35锚点"
  - "time固定源码lock_IPC/syscall.c、signal/signal.c、signal_macro.h的实际调用与宏"
open_questions:
  - "SR10-01主线是否限定关闭待C03；本文的accept不是主线验收"
  - "跨全仓间接调用/静态单个源段以外的全局调用覆盖未核"
---

# P00：SR10-01图语义与证据关联定点处置

**裁定：accept。** 第10批的52条源码引文本身可复用，但“引文存在”不等于支持它旁边的调用边。此文件给出追加纠正读法，不改变旧10的任何文件、SW10编号、内核源码或既有10主线RETURN；待C03审后才有可能关闭SR10-01。

## 一、五处原问题与替代关联

| 固定旧位置 | 问题 | 追加读法及ID |
|---|---|---|
| `facts::SW10-E01` 和DOT同边 | `SW10-N01 sys_kill → N02 prepare_kill_siginfo` 标活动call但仅引用SW10-A03（函数开头/调试宏/局部变量），证据不包含实际调用 | 保留真实活动call，**补SW10-A03b**（源中第97行），A03仅定位入口；不重编号 |
| `SW10-E12` | N09→N10 的条件call只引用A18（SIGKILL/PF_KTHREAD跳过），没有实际调用行 | 引**A18**确定是否进入、引**A19**确定调用 `__sigqueue_alloc`；再引A20的未初始化实参前提，不宣告该调用结果确定 |
| `SW10-E16` | `complete_signal → signal_wake_up_state` 只引用A34（调用宏名字） | 与**A35** 宏 `signal_wake_up→signal_wake_up_state` 同时使用；应标“宏展开形成的潜伏调用”，不能标当前kill活动边（上游E15被注释） |
| `SW10-E21` / DOT | `dequeue_signal → collect_signal` 被画成直接call，却遗漏了实际 `__dequeue_signal` 中间层 | 原E21仅按**superseded_path_not_direct**保留历史导航；以**SW11-GN01** `__dequeue_signal`、**SW11-GE01** `dequeue_signal→__dequeue_signal`、**SW11-GE02** `__dequeue_signal→collect_signal` 代替，后者明确 `sig!=0` |
| `edge_direction`、DOT头及E24 | 把data写成“consumer→provider，except explicit sender writes”，方向例外无授权且把插入副作用当数据流 | **统一data：消费方→提供方。** N09→N21只表示N09使用shared_pending数据/地址；A23的链表插入另列 `effects`，不得创造反向data口径。E10与E24同端点表示“选容器”和“附条件入链”两个用途，不计为两项独立任务依赖 |

## 二、必要的当轮逐字源证

**[VERIFIED] `mykernel/lock_IPC/syscall.c::MYOS_SYSCALL_DEFINE2(kill)`，固定 `time` 第95–99行：**

```c
	kernel_siginfo_t info;

	prepare_kill_siginfo(sig, &info, PIDTYPE_TGID);

	return kill_something_info(sig, &info, pid);
```

**[VERIFIED] `mykernel/lock_IPC/signal/signal.c::__send_signal_locked`，固定 `time` 第509–512行：**

```c
	 * Skip useless siginfo allocation for SIGKILL and kernel threads.
	 */
	if ((sig == SIGKILL) || (t->flags & PF_KTHREAD))
		goto out_set;
```

**[VERIFIED] `mykernel/lock_IPC/signal/signal.c::__send_signal_locked`，固定 `time` 第528–528行：**

```c
	q = __sigqueue_alloc(sig, t, GFP_ATOMIC, override_rlimit, 0);
```

**[VERIFIED] `mykernel/lock_IPC/signal/signal.c::__send_signal_locked`，固定 `time` 第530–534行：**

```c
	if (q) {
		// list_add_tail(&q->list, &pending->list);
		list_header_add_to_tail(&pending->list, &q->list);
		switch ((ulong) info) {
		case (ulong) SEND_SIG_NOINFO:
```

**[VERIFIED] `mykernel/lock_IPC/signal/signal.c::dequeue_signal`，固定 `time` 第236–240行：**

```c
	*type = PIDTYPE_PID;
	signr = __dequeue_signal(&tsk->pending, mask, info, &resched_timer);
	if (!signr) {
		*type = PIDTYPE_TGID;
		signr = __dequeue_signal(&tsk->signal->shared_pending,
```

**[VERIFIED] `mykernel/lock_IPC/signal/signal.c::__dequeue_signal`，固定 `time` 第215–218行：**

```c
	int sig = next_signal(pending, mask);
	if (sig != 0)
		collect_signal(sig, pending, info, resched_timer);
	return sig;
```

**[VERIFIED] `mykernel/lock_IPC/signal/signal.c::complete_signal`，固定 `time` 第474–477行：**

```c
	 * The signal is already in the shared-pending queue.
	 * Tell the chosen thread to wake up and dequeue it.
	 */
	signal_wake_up(t, sig == SIGKILL);
```

**[VERIFIED] `mykernel/lock_IPC/signal/signal_macro.h::signal_wake_up`，固定 `time` 第197–198行：**

```c
	#define signal_wake_up(t, resume)	\
			signal_wake_up_state(t, resume ? TASK_WAKEKILL : 0)
```


**重要限定：** `dequeue_signal` 还在私有pending无结果时对共享pending第二次调用 `__dequeue_signal`（固定signal.c第239–241行），不是单次无条件调collect；`__dequeue_signal` 先调用 `next_signal`，只在结果非0时调用collect。A29的“调用到__dequeue_signal”只能支持第一段，不能单独证明collect的调用边。宏绑定同理不能只靠A34的字符串命中证明函数目标。

## 三、可直接消费的局部结构记录（只追加、不修改旧YAML）

以下为**覆盖读法**，`original_edge`仍保持旧ID以供溯源；完整图使用本文件后续DOT片段，非要求编辑旧图。

```yaml
edge_direction:
  call: "caller -> callee; only actual call or verified macro expansion"
  data: "consumer -> provider; NO writer-direction exception"
  init_order: "later consumer -> prior initialization"
corrections:
  - {original_edge: SW10-E01, action: supplement_anchor, anchors: [SW10-A03, SW10-A03b], kind: call, status: active}
  - {original_edge: SW10-E12, action: supplement_anchor, anchors: [SW10-A18, SW10-A19, SW10-A20], kind: call, status: conditional, condition: "not SIGKILL/PF_KTHREAD; override_rlimit undefined"}
  - {original_edge: SW10-E16, action: resolve_macro, anchors: [SW10-A34, SW10-A35], kind: call, status: latent_through_macro}
  - {original_edge: SW10-E21, action: supersede_direct_edge, replacement: [SW11-GE01, SW11-GE02], status: superseded_path_not_direct}
  - original_edge: SW10-E24
    action: normalize_data_direction
    kind: data
    from: SW10-N09
    to: SW10-N21
    direction: consumer_to_provider
    anchors: [SW10-A16, SW10-A23]
    effects: "when q!=NULL, list_header_add_to_tail(&pending->list,&q->list); this is a mutation, not a reverse data edge"
new_nodes:
  - {id: SW11-GN01, label: "__dequeue_signal (intermediate)", source: "mykernel/lock_IPC/signal/signal.c::__dequeue_signal"}
new_edges:
  - {id: SW11-GE01, from: SW10-N19, to: SW11-GN01, kind: call, status: conditional, anchors: [SW10-A29], condition: "for private, then shared if first misses"}
  - {id: SW11-GE02, from: SW11-GN01, to: SW10-N20, kind: call, status: conditional, condition: "next_signal returned sig!=0", source: "signal.c::__dequeue_signal:215-218"}
```

```dot
// Replaces only the erroneous direct SW10-E21 edge; DO NOT overlay both as active calls.
"SW11-GN01" [label="SW11-GN01\n__dequeue_signal"];
"SW10-N19" -> "SW11-GN01" [id="SW11-GE01",label="call, conditional (private/shared)"];
"SW11-GN01" -> "SW10-N20" [id="SW11-GE02",label="call if sig != 0"];
// SW10-E01: add anchors A03,A03b; E12: A18,A19,A20; E16: A34,A35.
// SW10-E24: consumer->provider; move q-list insertion to effects.
```

## 四、26条旧边的同类限定检查

| 旧ID | 调用/数据/初始化语义的检查结论与消费约束 |
|---|---|
| E01 | **需补A03b**；A03不是调用证据 |
| E02 | 直接call，A03b含kill_something_info |
| E03 | 条件call，pid>0才调kill_proc_info |
| E04 | 调用在kill_proc_info表达式中，find_vpid；A08支持 |
| E05 | 同一表达式调用pid_task，需区分它的PIDTYPE_PID |
| E06 | data依赖find_vpid对当前PID namespace的取得；符号展开应并读task_active_pid_ns/ns_of_pid，而非凭“current namespace”标签 |
| E07 | p非NULL才调用group_send_sig_info；A08支持 |
| E08 | group_send_sig_info→send_signal_locked真实call，权限语句注释不等于阻断 |
| E09 | send_signal_locked→__send_signal_locked真实call，force=false |
| E10 | N09消费/选择shared_pending的data依赖，限TGID kill；不凭它单独证明节点已插入 |
| E11 | N09→private_pending为其他type分支；不代表sys_kill当前写私有 |
| E12 | **补A19分配调用**，A18只支持跳过条件 |
| E13 | 由__sigqueue_alloc条件内调用kmem_cache_alloc；A21含调用，结果取决于分配前提 |
| E14 | N09→sigaddset在out_set条件内实际call；不能从函数名推定有效位号 |
| E15 | 调用为注释，不存在当前kill的活动通知边；点线只表示历史代码位置 |
| E16 | **补A35宏绑定**；潜伏macro-call路径，不将complete_signal视为已由E15接入 |
| E17 | signal_wake_up_state→wake_up_state真实函数内call，但属于上游未接入的潜伏子图 |
| E18 | 同一函数条件性→kick_process，仅在wake_up_state返回0后；仍属潜伏 |
| E19 | wake_up_state包装器→try_to_wake_up真实call；不能把wake成功或入队当已证明 |
| E20 | get_signal中条件调用dequeue_signal；在同步私有未选出时 |
| E21 | **必须拆解**经__dequeue_signal的两段调用及sig!=0门，不是直接call |
| E22 | dequeue_signal读私有pending，data消费方→提供方，先检查它 |
| E23 | 私有无结果后读共享pending，data消费方→提供方 |
| E24 | **移除writer例外**；N09依赖共享pending，入链是effects，不是反向data |
| E25 | init_order N10→signals_init只表示cache应预先创建；不因初始化函数存在就保证启动顺序已运行成功 |
| E26 | start_kernel中signals_init活动call；不是内核启动验证 |

逐边检查的是固定10的26条边以及这些边在本次目标源码中的直接证据。**其他未列的间接调用、全树数值/宏拼接或并发时序没有因此认证。** 当某旧边只有“附近文本”但缺完整调用目标证据时，以上读法限定其证明强度，未制造新的旧ID状态。

## 五、主线交接与未做事项

该处置接受SR10-01图/引文关联退回，但**10的主线verdict仍是RETURN**，直到C03回读并审核。09已关闭的SR09-01不因此重新开启。它不修改发送端逻辑、不再分析旧52锚点的全部技术断言，也不把complete_signal作为活动发送调用。接续11的PID源码取证只使用真实源文，不让未修的旧图充当PID生命周期的证据。

未执行原10检查器、Graphviz渲染、C/ASM、预处理、构建、QEMU或运行内核；没有写10旧YAML/DOT、改内核或复制旧成果。
