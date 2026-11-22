---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "independent_historical_errata_adjudication"
status: "CLOSED_DOCUMENTATION_ONLY"
phase: 2
date: "2026-11-22"
date_timezone: "Asia/Shanghai"
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话自报）"
model_per_owner: "Pro（后台和effort未独立认证）"
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "88968361c2fa"
execution_pin_short12: "00c0228c52b8"
batch123_composite: "PASS_PENDING_LOCAL"
batch124_with_C08_supplement: "PASS_PENDING_LOCAL"
closed_documentation_only_this_round: ["R123-01"]
new_open_returns_this_round: []
adjudicated: "R123-01"
followup_id: "CORE-USER-LIBC-STRING-LENGTH-COPY-124"
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":2,"quotes_reconfirmed":2,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/libc-string-length-copy.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/libc-string-length-copy-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COMPARE-123-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COMPARE-123-D00-122-review.md","agent-workspace/lead/MYOS2-LEAD-002/175-user-libc-string-length-copy-restart-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare-paths.dot","myinitramfs/myshell/myshell.c","myinitramfs/mylib/myglibc/string/strcmp.c"]
---

# R123-01 · 调用者、比较返回与目录循环退出

**R123-01 文档关闭。** 第123批原件、123主线回执、124的D00追加材料及下列C08补记合读，裁定 `PASS_PENDING_LOCAL`。原图的相邻操作须依改正后的边类型使用。[123退回范围](https://github.com/08822407d/MyOS2/blob/88968361c2fa/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COMPARE-123-review.md) · [124替代实例图](https://github.com/08822407d/MyOS2/blob/00c0228c52b8/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/libc-string-length-copy-paths.dot) · [124机器表](https://github.com/08822407d/MyOS2/blob/00c0228c52b8/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/facts-and-boundaries.yaml)

C08从固定命令表重新遍历，逐次重算选用仓内strcmp时的差值，核六例合计38次调用；同时核对原123比较表和新图中实例、返回数据、条件与函数自身返回。find_cmd在比较结果为零时返回当前索引，遍历完成时返回−1。原图末次比较到out的含义由下表及新图替代。[命令表](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L44-L56) · [strcmp 的退出值](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strcmp.c#L29-L44) · [前两例的具体边](https://github.com/08822407d/MyOS2/blob/00c0228c52b8/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/libc-string-length-copy-paths.dot#L221-L274)

| 输入 | 调用次数 | 最后一次strcmp | find_cmd自身返回 |
|---|---:|---:|---:|
| pwd | 3 | 0 | 2 |
| pw | 10 | −2 | −1 |
| rm | 6 | 0 | 5 |
| rmdir | 8 | 0 | 7 |
| rmd | 10 | 8 | −1 |
| cd | 1 | 0 | 0 |

[VERIFIED myinitramfs/myshell/myshell.c::find_cmd]
```c
	for(i = 0;i<sizeof(shell_internal_cmd)/sizeof(builtincmd_s);i++)
		if(!strcmp(cmd,shell_internal_cmd[i].name))
			return i;
	return -1;
```

[固定源码 148–151](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L148-L151)

**cd 与 ls。** D01/D02 均由 cd_command 直接请求 strcmp，第2例才请求 chdir。L01/L02/L03 的第一次比较均由 ls_command 发出；第二次比较只有L02/L03执行，名称输出只有L03执行。三例均另给“下一次 readdir 返回空”的输入条件，然后才到240行的换行输出。[cd调用点](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L199-L203) · [ls判断与输出](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L231-L242)

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
	while(1)
	{
		buf = readdir(dir);
		if(buf == NULL)
			break;
```

[固定源码 231–235](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L231-L235)

原追加图393、403行把“略过名称”直接连到第二次 readdir 为空。对这两条边按 **SEQUENCE（本例给定的下一次读取为空）** 解读；比较结果只决定本条目录项的名称输出。循环退出条件来自下一次 readdir 的返回，和前一个名字比较的真假独立。395、405、418行 loop_exit 到 LF 的 CONTROL 有源码依据。四个直接printf/比较调用点仍按 ls_command 拥有的源级位置解释。[ls 三例](https://github.com/08822407d/MyOS2/blob/00c0228c52b8/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/libc-string-length-copy-paths.dot#L390-L418) · [完整循环](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L231-L242)

**正反核查。** 205个唯一节点、262条有类型边均无悬空端点。在62个直接调用实例中，D00占50（find 38、cd 3、ls 9），新strncpy案例占12。逐条核对拥有者及实际端点；cd的比较器到chdir、ls的两次比较之间、比较器或名称输出到另一printf均无直接调用。原文“5项未调用”中D02的 early 是一个未走的返回分支，另四项才是调用点。下面是从这五个案例入口实测的可达集合，按原图共同的case前缀省略节点前缀。ENTRY为本例入口；集合顺序不表达执行时序。

| 原入口 | 可达节点后缀 | 原图存在且从该入口不可达 |
|---|---|---|
| C123-D01 | ENTRY, owner, cmp, cond, early | chdir |
| C123-D02 | ENTRY, owner, cmp, chdir, cond, unknown | early |
| C123-L01 | ENTRY, owner, cmp1, LF, cond1, loop_exit | cmp2, name |
| C123-L02 | ENTRY, owner, cmp1, cmp2, LF, cond1, cond2, loop_exit | name |
| C123-L03 | ENTRY, owner, cmp1, cmp2, name, LF, cond1, cond2, loop_exit | 合同所列调用点全部可达 |

稿件实际把38次查找比较都画了出来，超过175要求的末次比较范围；已一次核完，后继无须再画。直接调用边只表示调用归属；判断先后与是否满足守卫分别读取CONTROL、SEQUENCE和给定输入。完整本地P9和实际命令运行仍为 `NOT_RUN`。
