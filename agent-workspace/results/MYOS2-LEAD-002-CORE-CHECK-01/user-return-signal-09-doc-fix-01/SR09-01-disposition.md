---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-RETURN-SIGNAL-09-DOC-FIX-01
record_type: scoped_documentation_erratum
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: MYOS2-A-C03a 代码分析执行
produced_by: "ChatGPT普通对话（Owner选择GPT-6；后台身份未独立认证）"
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: "2026-10-31"
read_channel: connector
base_snapshot: "source=time @ a039d9803ade；old_result=claude/dazzling-cori-q0dnyt @ 67b2458b4a6a；instruction=agent/MYOS2-LEAD-002 @ 5c5773bdd7de（均为工具原样复制的短提交）"
task_contract: "agent-workspace/lead/MYOS2-LEAD-002/26-chat-executor-sr09-disposition-contract.md @ 9d1d90b1322d"
target_review_record: CORE-USER-RETURN-SIGNAL-09-REVIEW-001
target_item: SR09-01
target_review_disposition: RETURN
disposition: accept
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_change_made: false
original_results_changed: false
scope: "仅追加09的SR09-01勘误；不重做08/09、不穷尽系统调用返回值"
supersedes_locations:
  - "user-return-signal-09/user-return-signal.md::L1（08 `3.2各行接上③之后，表第一行的全部四列）"
affected_ids:
  - CORE-USER-RETURN-SIGNAL-09::ipc.signal.syscall_restart
  - CORE-USER-RETURN-SIGNAL-09::RR-01
  - CORE-USER-RETURN-SIGNAL-09::RR-02
  - CORE-USER-RETURN-SIGNAL-09::RR-03
  - CORE-USER-RETURN-SIGNAL-09::RR-04
  - CORE-USER-RETURN-SIGNAL-09::RR-05
  - CORE-USER-RETURN-SIGNAL-09::RR-06
  - CORE-USER-RETURN-SIGNAL-09::RR-07
  - CORE-USER-RETURN-SIGNAL-09::RR-08
  - CORE-USER-RETURN-SIGNAL-09::RR-09
  - CORE-USER-RETURN-SIGNAL-09::RR-10
  - CORE-USER-RETURN-SIGNAL-09::RR-11
  - CORE-USER-RETURN-SIGNAL-09::RR-12
  - CORE-USER-RETURN-SIGNAL-09::RR-13
  - CORE-USER-RETURN-SIGNAL-09::RR-14
  - CORE-USER-RETURN-SIGNAL-09::ER-02
affected_ids_note: "受影响的是其被L1引用的消费口径；既有RR/ER-02原记录不修改、不断言其自身错误"
inputs_read:
  - "26号固定任务书9d1d90b1322d全文；沿用已完成的25号接收"
  - "主线09回执 CORE-USER-RETURN-SIGNAL-09-REVIEW-001（指令基线5c5773bdd7de，全文）"
  - "旧结果固定67b2458b4a6a：user-return-signal.md `0、D03.1～D03.5、L1/L2/L4/L5/L6/L7/L8相关段落及条件表"
  - "旧结果固定67b2458b4a6a：stdio-08-disposition.md `0、`3.1～3.6、`5；MANIFEST.md交付身份"
  - "旧结果固定67b2458b4a6a：facts-and-boundaries.yaml中ipc.signal.syscall_restart与RR-01～RR-14；deferred-findings.yaml中ER-02、RS-01的定点段落"
  - "time @ a039d9803ade：mykernel/scripts/options_flags.cmake"
  - "time @ a039d9803ade：mykernel/arch/x86_64/include/asm/syscall.h、mykernel/arch/x86_64/lock_IPC/signal/signal.c、signal_64.c"
  - "time @ a039d9803ade：mykernel/entry/common.c、mykernel/arch/x86_64/entry/common.c"
  - "time @ a039d9803ade：mykernel/include/linux/lib/errno.h、mykernel/include/linux/kernel/err.h、mykernel/arch/x86_64/include/asm/unistd_64.h、mykernel/include/uapi/asm-generic/errno-base.h"
open_questions:
  - "按计算得到重启码的全部实际返回值生产者未穷尽；并未发现新的实际生产者"
  - "没有活动return的若干处理函数，假设其返回时ax的值静态无法确定"
  - "RS-01保持needs_evidence；GAP-U04硬件/运行部分及GAP-U10继续开放"
---

# SR09-01：有限检索不能代替返回值分支证明

## 1. 位置、冲突与处置

**accept（文档勘误）。** 主线 `5 指出的 L1 第一行过度概括确实存在：负向的“按名字/字面量检索”不能推出一切表内已返回调用的 `ax` 必不等于四个重启值，因而不能预设只走 RR-07 或 RR-14。同一行的“没有投递handler时是分派值”也漏掉了无handler但命中重启 `switch` 的改写与控制不返回情形。本批纠正的是报告消费口径，**不是证明新的内核bug或新的生产者存在**。

原位置：旧件 `user-return-signal.md` 的“衔接”→ L1“08 `3.2各行接上③之后”，第一行全文（原件冻结）：

```text
| 在表中，处理函数返回 | 处理函数的返回值 | 按名字和字面量检索，没有活动处理函数返回重启码，落在 RR-07 或 RR-14；15 号 rt_sigreturn 成功恢复时 orig_ax 为 -1，落在 RR-01；没有活动 return 的处理函数除外（D03.5） | 没有投递 handler 时是分派值；投递成功时是 0（交给 handler）；handler 经 rt_sigreturn 返回后，是从用户帧恢复出的值 |
```

同一旧文件 D03.5 的明确限定（逐字）：

```text
| 按计算得出的返回值 | 没有穷尽。上面的检索只按名字和字面量。 | — |
```

L1 的确定分类与 D03.5 的未穷尽声明不相容；本处置以实际 `orig_ax`/`syscall_get_error(ax)` 和信号分支分类，取代 L1 首行读法，其他原件不改。

## 2. 直接可采用的 L1 替代整行

下列四列表头与旧 L1 相同，**整行替代**，并读随后的条件附注（不是只替换第三列）：

| 08 `3.2 中号的情况 | 分派后的 ax | ③ 可能命中的行 | 返回段装入 rax 的值（源码层面） |
|---|---|---|---|
| 在表中，处理函数返回；没有活动 `return` 的函数仅按“若返回、值未定”读 | 函数确实返回时写入实际返回值；缺少活动 `return` 的若返回则值不能静态确定。15号 `rt_sigreturn` 成功恢复时还会按用户帧改写寄存器并置 `orig_ax=-1` | `(int)orig_ax==-1` → RR-01（仅跳过重启switch）；否则按 `syscall_get_error(regs->ax)` 的实际结果：选到handler时四值→RR-02～06、其余→RR-07，后续建帧成功/失败→RR-08/09；没有handler时四值→RR-10～13、其余→RR-14。**名字/字面量检索未命中不等于排除四值** | 到达返回段才装入③之后的 `regs->ax`：没有handler且跳过重启门或未命中时可保持分派值；命中则按分支写 `orig_ax` 或219（即使值相等也可能改了IP）。有handler则先按重启条件改写或保持ax，建帧成功时ax变0；失败时保留建帧前ax（可能是-EINTR、orig_ax或原值），handler未运行。后来若经 `rt_sigreturn` 成功恢复，还要再经过一次③，没有成功建立新信号帧且未再次改写时才可说装入的是用户帧恢复值 |

**条件附注（也是本勘误的消费限制）：**

1. 门限只取 `syscall_get_nr` 返回的 `int`。当 `(int)orig_ax==-1` 时 RR-01 意味着**重启switch跳过**，不意味着跳过 `get_signal` 或其后可发生的信号建帧；`rt_sigreturn` 成功恢复是这个门限的一种来源。门限允许时 `syscall_get_error` 从完整64位 `regs->ax` 取得值，再经 `IS_ERR_VALUE` 过滤；低32位看似 -512 但高位不匹配的值不能直接当四种64位重启码。
2. 两个switch对应关系：有handler：-516/-514→-EINTR（RR-02/03）；-512无 `SA_RESTART` →-EINTR（RR-04），有 `SA_RESTART` →`ax=orig_ax`、`ip-=2`（RR-05）；-513→`ax=orig_ax`、`ip-=2`（RR-06）；其余→RR-07。无handler：-514/-512/-513→`ax=orig_ax`、`ip-=2`（RR-10/11/12）；-516→219、`ip-=2`（RR-13）；其余→RR-14。无handler也可能命中重启分支；两边的 `switch` 都没有default。
3. 有handler时，重启改写先于建帧。建帧成功（RR-08）会在保存当时寄存器后把 `ax` 置0并改IP/SP等；建帧失败（RR-09）不回滚先前重启改写。这里不宣称“信号帧内存从未写入”：某些失败之前可能已向用户帧写数据，原报告 L2/RS-08 另保留地址重叠例外。`ax` 数值相等也不等于帧未改写，例如 `ax=orig_ax` 恰为同值但 `ip-=2`。
4. 两类必须留作未知：**无活动return** 的 `dup`、`dup2`、`wait4` 和部分 `llseek` 路径若假设返回，其寄存器结果未确定；**计算形成的返回值** 未被名字/字面量检索穷尽，仍可能数值恰好命中四种重启码。不能预填 RR-07/14，也不能据此反推确有新生产者。表内缺项进入 `while(1)` 或其他不返回路径时，根本没有③及返回段装入值。
5. 对 `rt_sigreturn` 仅承认成功恢复会把 `orig_ax` 置 -1；这不阻断再次 `get_signal` 选中新handler并改写帧。本文所有“返回段装入”是源码级阶段描述；运行/硬件/用户态最终行为均 NOT_RUN。

## 3. 本轮固定源码定点引文

实际回读 `time @ a039d9803ade` 的源码文本。每条 [VERIFIED] 都取自指定路径和符号定义体内的1～5行逐字引文；不借旧报告引文冒充本轮已核。

**分派写回条件。** [VERIFIED mykernel/arch/x86_64/entry/common.c::do_syscall_64]

```c
	uint unr = nr;
	if (unr < __NR_syscalls) {
		// unr = array_index_nospec(unr, NR_syscalls);
		regs->ax = x64_sys_call(regs, unr);
		// return true;
```

**`orig_ax` 的int门限。** [VERIFIED mykernel/arch/x86_64/include/asm/syscall.h::syscall_get_nr]

```c
	static inline int
	syscall_get_nr(task_s *task, pt_regs_s *regs) {
		return regs->orig_ax;
	}
```

**`ax` 的源与错误码过滤。** [VERIFIED mykernel/arch/x86_64/include/asm/syscall.h::syscall_get_error]

```c
	static inline long
	syscall_get_error(task_s *task, pt_regs_s *regs) {
		unsigned long error = regs->ax;
```

[VERIFIED mykernel/arch/x86_64/include/asm/syscall.h::syscall_get_error]

```c
		return IS_ERR_VALUE(error) ? error : 0;
```

**有handler路径的门限。** [VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal.c::handle_signal]

```c
	if (syscall_get_nr(current, regs) != -1) {
		/* If so, check system call restarting.. */
		switch (syscall_get_error(current, regs)) {
```

**-516/-514 → -EINTR。** [VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal.c::handle_signal]

```c
		case -ERESTART_RESTARTBLOCK:
		case -ERESTARTNOHAND:
			regs->ax = -EINTR;
			break;
```

**-512 的 `SA_RESTART` 选择。** [VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal.c::handle_signal]

```c
		case -ERESTARTSYS:
			if (!(ksig->ka.sa.sa_flags & SA_RESTART)) {
				regs->ax = -EINTR;
				break;
			}
```

**-513及SA_RESTART-512的重试改写。** [VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal.c::handle_signal]

```c
			fallthrough;
		case -ERESTARTNOINTR:
			regs->ax = regs->orig_ax;
			regs->ip -= 2;
			break;
```

**`get_signal` 选路。** [VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal.c::arch_do_signal_or_restart]

```c
	if (get_signal(&ksig)) {
		/* Whee! Actually deliver the signal.  */
		handle_signal(&ksig, regs);
		return;
	}
```

**无handler的重启门限。** [VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal.c::arch_do_signal_or_restart]

```c
	if (syscall_get_nr(current, regs) != -1) {
		/* Restart the system call - no handlers present */
		switch (syscall_get_error(current, regs)) {
```

**无handler的三种原号改写。** [VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal.c::arch_do_signal_or_restart]

```c
		case -ERESTARTNOINTR:
			regs->ax = regs->orig_ax;
			regs->ip -= 2;
			break;
```

**无handler的-516改写。** [VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal.c::arch_do_signal_or_restart]

```c
		case -ERESTART_RESTARTBLOCK:
			regs->ax = get_nr_restart_syscall(regs);
			regs->ip -= 2;
			break;
```

**建帧的SA_RESTORER失败门。** [VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal_64.c::x64_setup_rt_frame]

```c
	if (!(ksig->ka.sa.sa_flags & SA_RESTORER))
		return -EFAULT;
```

**成功建立信号帧时改ax。** [VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal_64.c::x64_setup_rt_frame]

```c
	regs->di = ksig->sig;
	/* In case the signal handler was declared without prototypes */
	regs->ax = 0;
```

**失败状态不进入成功分支。** [VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal.c::handle_signal]

```c
	failed = (setup_rt_frame(ksig, regs) < 0);
	if (!failed) {
```

**`rt_sigreturn` 成功恢复后 orig_ax = -1。** [VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal_64.c::restore_sigcontext]

```c
	/* disable syscall checks */
	regs->orig_ax = -1;
```

配置先读 `mykernel/scripts/options_flags.cmake`（13～70行）。本文件所列 `CMAKE_C_FLAGS` 无 `-DCONFIG_IA32_EMULATION` 与 `-DCONFIG_X86_X32_ABI`；但 `CMAKE_C_FLAGS` 引用已有变量，未进行编译/预处理，不能推出所有外部构建环境均无这两个宏。常量从各自定义回读：`mykernel/include/linux/lib/errno.h` 的 ERESTARTSYS/NOINTR/NOHAND/RESTARTBLOCK 分别为512/513/514/516，`include/linux/kernel/err.h` 的 MAX_ERRNO=4095及 IS_ERR_VALUE 过滤，`include/uapi/asm-generic/errno-base.h` 的 EINTR=4，`arch/x86_64/include/asm/unistd_64.h` 的 __NR_restart_syscall=219。常量与条件源码只说明静态分支，不证明实际生产者或运行结果。

## 4. 同义摘要与结构化材料逐处核查

| 原位置（均来自固定09旧件） | 结果 | 本文件承接的消费限制 |
|---|---|---|
| `user-return-signal.md` L1第一行（四列） | **必须替代** | 以本文件`2整行取代过度确定的③和rax概括；保留冻结原件 |
| `user-return-signal.md` `0第5条 | **追加限定** | “按名字与字面量检索，没有活动处理函数返回这些重启码”只能理解为本次检索范围内未命中；原句还提无return函数，但未单独提计算返回值，须并读D03.5 |
| `user-return-signal.md` D03.5 | **无须更正** | “按计算得出的返回值：没有穷尽”已经明确；“目前已知可确定构造的唯一途径”只限已识别越界号途径，不是不存在性证明 |
| `user-return-signal.md` D03.3，RR-01～RR-14 | **无须更正** | 条件表按orig_ax、实际ax、handler与建帧判定；错误出在L1的概括，不在RR本体 |
| `user-return-signal.md` L2/L4/L5/L6/L8 | **无须更正** | 已区分数值相同与未写帧、失败不回滚、rt_sigreturn后的再次处理、不返回情形及NOT_RUN；原L2关于RS-08地址重叠的边界继续有效 |
| `stdio-08-disposition.md` `0、`3.1、`3.2 | **无须更正** | 对SR08-02已按四段限定，不构成所有返回值非重启码的证明 |
| `stdio-08-disposition.md` `3.3、`3.4、`3.6 | **无须更正** | 充分/必要条件与“数值相等仍写帧”的区别已陈述；ER-02的核心处置继续成立 |
| `stdio-08-disposition.md` `3.5第1行 | **追加限定** | “没有以这四个名字返回的处理函数”仅为名字检索；该行已提无活动return，仍须补计算返回值未穷尽的限定 |
| `stdio-08-disposition.md` `5（ERESTART四个名字的全部出现） | **追加限定** | “活动的产生点只有两处”仅为检索四个标识符的命中归纳，不穷尽表达式、间接返回或数值等价 |
| `facts-and-boundaries.yaml` `capabilities[ipc.signal.syscall_restart]` | **原有限定充分** | `failure_path` “已知只有越界号”须与同项 `not_covered` 的“按计算得出-512…-516的返回值”同读 |
| `facts-and-boundaries.yaml` `restart_conditions（RR-01～14）` | **无须更正** | 条件记录独立保持，引用不等于修改或逐条重验 |
| `deferred-findings.yaml` `CORE-USER-RETURN-SIGNAL-09::ER-02` 与 `RS-01` | **无须更正** | ER-02继续限定08的分派局部/返回段边界；RS-01仍needs_evidence，不因本次勘误转为已证 |

本表检查范围仅为列明的固定旧文档、相关能力/RR/ER条目与局部摘要；没有全量重读8229行facts、1101行deferred、10661行queries，也没有重跑09检查器。三处“追加限定”由此文件承担，不冒称原件已经更新。

## 5. 自查、保留结论及消费边界

- 逐字源码引文已在本轮固定 `time` 对应文件回读核对：**16 段**，每段1～5行并命中指定源码文件；这是连接器回读与当前任务的有界文本比对，不是原09检查器的重跑，也不认证所有路径语义。
- 26号合同的四要件已覆盖：原L1首行/D03.5冲突；`orig_ax` 与完整 `ax` 门限；有/无handler的两个switch及建帧成功/失败；同义摘要位置和RR/ER受影响ID。对未穷尽的计算值和无活动return值不作确定分类。
- **保留**09其他代表静态材料、08两项退回在主线中的限定关闭、`CORE-USER-RETURN-SIGNAL-09::RS-01` 的needs_evidence、`CORE-USER-VFS-BASELINE-05::GAP-U04` 的运行/硬件未核部分和 `CORE-USER-VFS-BASELINE-05::GAP-U10` 的open；不重开主线已裁定的五个组织问题，不自行创建新缺陷/修复事项。
- **未做：** 原批次262次检索、75锚点重跑、原检查器、内核编译/预处理、C/ASM执行、构建/QEMU、宿主运行、外部libc/镜像读取、用户程序修改、内核补丁及额外模型/API均NOT_RUN。未修改任何67b2458b4a6a旧件，未新建分支/PR。
- **请求主线核收：** 本文件仅 `READY_FOR_MAINLINE_REVIEW`，`mainline_acceptance=PENDING`。请C03只针对SR09-01与本文件及远端diff做有限验收；当前RETURN不因本文件的发布或自查自动改为PASS。交付后C03a停止写入，等待新的绑定任务。
