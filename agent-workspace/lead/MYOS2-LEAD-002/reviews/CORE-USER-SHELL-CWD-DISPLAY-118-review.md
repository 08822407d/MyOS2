---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-SHELL-CWD-DISPLAY-118"
record_type: "mainline_independent_static_review"
status: "RETURN"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro（Owner选择）"
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
date: "2026-11-21"
date_timezone: "Asia/Shanghai"
phase: 2
read_channel: "connector"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "2b170b8072ed"
execution_pin_short12: "cb96288abb8b"
batch115: "PASS_PENDING_LOCAL"
batch116: "PASS_PENDING_LOCAL"
batch117: "RETURN"
batch118: "RETURN"
R115_03: "CLOSED_DOCUMENTATION_ONLY"
R116_01: "CLOSED_DOCUMENTATION_ONLY"
R117_01: "CLOSED_DOCUMENTATION_ONLY"
R117_02: "OPEN"
R117_03: "CLOSED_DOCUMENTATION_ONLY"
R118_01: "OPEN"
R118_02: "OPEN"
author_delivery: "CONFIRMED_IN_GIT"
author_final_read_sequence: "NOT_OBSERVABLE"
mainline_acceptance: "RETURN"
reviewer_new_findings: ["R118-01","R118-02"]
carried_count_gate: "R117-02"
git_diff: [1,6,3762]
fixed_active_blob_matches: 6
exact_body_windows: 91
formal_prefix_total: 92
waypoint_checks: 27
full_local_P9: "NOT_RUN"
build_and_runtime: "NOT_RUN"
open_questions: []
owner_decisions_requested_now: []
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/167-user-shell-cwd-display-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CAT-FILE-DISPLAY-117-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CAT-FILE-DISPLAY-117-D00-115-116-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FS-CONTEXT-CWD-60-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/evidence.md","myinitramfs/myshell/myshell.c","mykernel/fs/vfs/d_path.c","mykernel/fs/vfs/open.c","mykernel/scripts/options_flags.cmake","agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/README.md","agent-workspace/conventions.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md"]
---

# 第118批独立验收 · RETURN

六件作者成果和正式评论存在，足以独立审查。主线接受下表中的有界模型；R117-02 与两项新增文档退回阻止整包通过。旧 C07c 到此停写，第119批只向新接收者追加勘误。[118固定六件](https://github.com/08822407d/MyOS2/tree/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118) · [118作者交付 #7057835701](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7057835701) · [167号验收条件](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/167-user-shell-cwd-display-contract.md) · [五项历史裁定](CORE-USER-SHELL-CWD-DISPLAY-118-D00-115-117-review.md)

## 提交是否完整

从四条直接 ref、两个 open PR、固定提交及其单父差异、固定树、各文件固定/分支全文和 PR17 全量评论独立核对。执行父 `52c51a3752d3` → `cb96288abb8b`，单提交六件全部新增，合计3762行；父树没有这六件。文件逐字对齐后另用 UTF-8 字节重算 Git SHA-1（对象头为 blob、空格、字节数、NUL）。每项均与提交树及分支返回的 Blob 相同。

| 固定件 | 行数 | UTF-8 字节 | 重算 Blob（短12） |
|---|---:|---:|---|
| [MANIFEST.md](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/MANIFEST.md) | 118 | 8870 | `51b071d46eee` |
| [shell-cwd-display.md](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display.md) | 108 | 10378 | `c201f5540289` |
| [facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/facts-and-boundaries.yaml) | 1797 | 52767 | `58bd01dfbbb9` |
| [shell-cwd-display-paths.dot](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display-paths.dot) | 407 | 25578 | `ba07bd3599f3` |
| [deferred-findings.yaml](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/deferred-findings.yaml) | 225 | 8693 | `204d4673871b` |
| [evidence.md](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/evidence.md) | 1107 | 34518 | `3e828fec6c30` |

审查时 lead `2b170b8072ed`、执行 `cb96288abb8b`、time `a039d9803ade`、直接 master `e8e60cd19540`。仅 PR16 Open/Ready、PR17 原题 Open/Draft，base 均为 master，均未合并。按ID两次取118正式评论并与分页项比正文、作者及标记；无 since 分页100/100/23/0，作者标记唯一。评论中用将来时描述的终检不能证明作者后来执行了哪些动作。Owner报告上下文耗尽；本回执只认证能独立复核的远端结果。[118作者交付 #7057835701](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7057835701)

## 报告、机器表及图

解析两件机器文件，核七张表列宽6/5/8/7/9/7/5，复算 A 启动两例、B 的五个 cd 情形、C 的三个缓存输出及 C04 分离模型，另核 CAT 三个文件名与旧七行八列。八项候选和八项证据缺口仍作延期材料。

| 给定 shell 缓存 | pwd 请求字节数（含换行） | 提示符第二次请求字节数 | cat note 所构文件名（分配字节） |
|---|---:|---:|---|
| `/` | 2 | 11 | `/note`（7） |
| `/old/docs` | 10 | 19 | `/old/docs/note`（15） |
| 空串 | 1 | 10 | `note`（6） |

提示符先另发 LF。给定内核已更新为 `/new`、shell 已清零且后续 getcwd 未写任何字节，缓存为空；仅知道函数返回值不足以确定缓存。`cd .` 的字符串比较命中直接返回，原缓存可能已陈旧。printf/write 的请求与实机实际显示分别记录。[B/C实表](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display.md#L61-L103) · [shell 消费与刷新](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L194-L211) · [内核处理](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/d_path.c#L213-L230)

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
	memset(current_dir, 0, PATH_MAX);
	getcwd(current_dir, PATH_MAX);
```
[myshell.c L75–76](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L75-L76)

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	i = chdir(path);
	if(!i)
	{
		memset(current_dir, 0, PATH_MAX);
		getcwd(current_dir, PATH_MAX);
```
[myshell.c L203–207](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L203-L207)

[VERIFIED myinitramfs/myshell/myshell.c::pwd_command]
```c
int pwd_command(int argc, char **argv)
{
	if(current_dir)
		printf("%s\n", current_dir);
	return 1;
```
[myshell.c L245–249](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L245-L249)

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		else if (copy_to_user(buf, b.buf, len))
			error = -EFAULT;
		else
			error = (long)buf;
```
[d_path.c L223–226](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/d_path.c#L223-L226)

从实际 DOT 提取五个具名簇 R115/CAT117/A/B/C，163个唯一声明节点，220条带标签的边，218对不同端点，无悬空端点。按有向边独立检查27组依次必经的节点，可达27/27。清单保留节点名供复核；路径存在与边标签正确性分别判定。[固定DOT](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display-paths.dot)

<details><summary>27组依次验证的节点</summary>

```
1. r_mapread r_stubread r_doread r_ksread r_fdread r_pposread r_localread r_vfsread r_readzero r_zero0 r_case0 r_fpos9
2. r_mapread r_doread r_vfsread r_zero_loop r_chunk r_break r_return r_caseP r_fpos9
3. r_mapread r_zero_loop r_chunk r_accumulate r_zero_loop r_chunk r_break r_caseP1 r_fpos9
4. r_mapwrite r_stubwrite r_dowrite r_kswrite r_fdwrite r_pposwrite r_localwrite r_vfswrite r_writecb r_writecommit
5. r_vfsread r_readiter r_readbug r_readqueued
6. r_vfswrite r_writeiter r_writebug r_writequeued
7. r_vfsread r_readbug r_readpos r_commit
8. r_vfswrite r_writebug r_writepos r_writecommit
9. t_table t_run t_cat t_open t_failprintf t_retminus
10. t_cat t_readcall t_switch t_x64stub t_do t_ksys t_vfs t_newsync t_generic t_positive t_simple t_full t_printfcall t_ret1
11. t_cat t_seekend t_seekset t_clearbuf t_readcall t_newsync t_generic t_positive t_simple t_short t_close t_ret1
12. t_cat t_readcall t_generic t_zero t_readresult t_zeroexample t_close t_ret1
13. t_fatbind t_methodptr t_generic
14. t_cat t_mallname t_clearname t_strcpy t_separator t_open
15. a_main a_mzero a_mgetcwd a_copied a_current a_prompt
16. a_main a_mgetcwd a_untouched a_current a_prompt
17. a_main a_loop a_lf a_prompt a_fflush a_input a_run a_cd
18. a_cmdtable a_run a_pwd c_pwdcall c_pwdprintf c_pwdret
19. b_cd b_arggate b_home b_strcmp b_chdir b_kernelchdir b_lookup b_pwdassign b_success b_getcwd b_ret
20. b_cd b_arggate b_explicit b_dot b_ret
21. b_cd b_explicit b_dot b_stale
22. b_cd b_chdir b_kernelchdir b_error b_fail b_errprintf b_ret
23. b_cachezero b_getcwd c_kernelgetcwd c_alloc c_prepend c_lencheck c_usercopy c_succret c_intret
24. b_getcwd c_getcwd c_nowrite c_cache c_pwdprintf c_printf c_write
25. b_getcwd c_getcwd c_nowrite c_cache c_promptprintf c_printf c_write
26. c_kernelgetcwd c_lencheck c_err c_intret
27. c_cache c_shellcat c_catpath
```
</details>

## 必须追加的三项差量

**R117-02 · P6 计数继续开放。** 对最终六件搜索协议正式前缀，依次0/0/0/0/0/92；其中91个1–5行短窗与19份固定源码逐字匹配，第92处在 evidence 第3行的说明文字。32处普通位置均在原文件行界内。行文相等、P2资格、P6总数分别核。旧117的88窗/89前缀另列。[说明行](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/evidence.md#L1-L8) · [MANIFEST自检](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/MANIFEST.md#L32-L41)

**R118-01 · 对证据重新分类并纠正消费归属。** CW118-E051 是文件作用域的指针对象定义，E052 是数组的暂定定义。两处适合普通位置，不能计作定义体内短窗；E050 宏定义独立保留。用 main、cd、pwd 活动体证明初始化和消费。追加区须逐条核其它消费：chdir 的 lookup_flags 单独锚到 open.c 第277行；getcwd 读 pwd/root 对应 E085/E086，E087/E088 支撑内部缓冲及拼接。报告与 facts 将指针→long→int、复制效果独立性归到 R60-S02，应归 R60-S01 第3项；R60-S02 对应调用图。补 time 活动配置正文锚，条件编译和否定命题按 P4 检查。[两个对象与主函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L59-L76) · [lookup_flags](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/open.c#L277) · [正文历史归属](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display.md#L92-L94) · [facts 历史归属](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/facts-and-boundaries.yaml#L957-L966) · [第60批原裁定](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FS-CONTEXT-CWD-60-review.md)

**R118-02 · 图上调用者、顺序和参数来源。** 修 A 中 a_loop→a_lf 的 DIRECT_PRINTF_FIRST，由 main 拥有对 printf 的调用，循环位置只标顺序；B 中 memset 后 getcwd 是 cd_command 内的次序，两者的直接调用者均为 cd_command；C 中内部 kzalloc 和 prepend 是内核 getcwd 内的操作，kzalloc 不调用 prepend，pwd/root 进入后续路径拼接，分配大小来自 PATH_MAX。保持原27组相关路径及关键失败支路，并让真实节点和类型可由最终文本重新提取。[A/B调用边](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display-paths.dot#L310-L358) · [C分配与拼接边](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display-paths.dot#L360-L383) · [shell主循环](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L83-L102) · [getcwd定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/d_path.c#L190-L230)

**状态。** 115、116组合 `PASS_PENDING_LOCAL`；117 `RETURN`，余 R117-02；118 `RETURN`，新增 R118-01、R118-02。完整本地P9、构建、运行 `NOT_RUN`。119接收人只提交纠正，关闭权归C08。[119任务书](../169-user-shell-input-dispatch-contract.md)
