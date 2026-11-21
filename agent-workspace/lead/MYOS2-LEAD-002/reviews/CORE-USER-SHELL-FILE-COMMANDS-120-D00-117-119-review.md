---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-SHELL-FILE-COMMANDS-120"
record_type: "independent_historical_correction_review"
status: "TWO_CLOSED_ONE_OPEN"
phase: 2
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话自报）"
model_per_owner: "Pro，后台与effort未独立认证"
date: "2026-11-21"
date_timezone: "Asia/Shanghai"
base_snapshot: "time"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "9df81502ae54"
execution_pin_short12: "9ea23faa4ab7"
batch115: "PASS_PENDING_LOCAL"
batch116: "PASS_PENDING_LOCAL"
batch117: "RETURN"
batch118_composite: "PASS_PENDING_LOCAL"
batch119_composite: "PASS_PENDING_LOCAL"
batch120: "RETURN"
R117_02: "OPEN"
R118_01: "CLOSED_DOCUMENTATION_ONLY"
R118_02: "CLOSED_DOCUMENTATION_ONLY"
R119_01: "CLOSED_DOCUMENTATION_ONLY"
R120_01: "OPEN"
R120_02: "OPEN"
D00_closed: ["R118-02","R119-01"]
D00_still_open: ["R117-02"]
historical_count_basis: "byte_occurrences_and_qualified_windows_separate"
full_local_P9: "NOT_RUN"
build_link_QEMU_hardware_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":3,"quotes_reconfirmed":3,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/170-user-shell-file-commands-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-INPUT-DISPATCH-119-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-INPUT-DISPATCH-119-D00-117-118-review.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/shell-file-commands.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/shell-file-commands-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/evidence.md","myinitramfs/myshell/myshell.c","myinitramfs/mylib/myglibc/syscall/syscall.c","mykernel/fs/vfs/d_path.c","mykernel/scripts/options_flags.cmake"]
---

# 第120批三项历史补正的独立裁定

关闭R118-02和R119-01的文档问题。R117-02的117–119历史分账已齐，新120的一个引文跨了符号边界，继续OPEN。旧文件按本回执指定的范围和勘误组合使用。[170号关闭条件](https://github.com/08822407d/MyOS2/blob/9df81502ae54/agent-workspace/lead/MYOS2-LEAD-002/170-user-shell-file-commands-contract.md) · [119独立验收](https://github.com/08822407d/MyOS2/blob/9df81502ae54/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-INPUT-DISPATCH-119-review.md) · [上轮历史裁定](https://github.com/08822407d/MyOS2/blob/9df81502ae54/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-INPUT-DISPATCH-119-D00-117-118-review.md) · [120作者正式交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7062612922)

## R117-02 · 字面计数正确，有一窗须拆分

| 快照 | 六件正式前缀（按MANIFEST/正文/facts/DOT/deferred/evidence） | 原文列出的短窗 | 当前消费 |
|---|---|---:|---|
| 117 | 1/0/0/0/0/88 | 88 | 89次前缀；额外处为MANIFEST第113行。 |
| 118 | 0/0/0/0/0/92 | 91 | evidence第3行另有说明；去除两处文件作用域对象后89个候选。 |
| 119 | 0/0/0/0/0/46 | 46 | 三段全局数组初值改普通位置，43个候选；结构体窗保留。 |
| 120 | 0/0/0/0/0/81 | 81 | 逐字匹配81；以当前声明的符号为界，80个合格，E072一窗越界。 |

固定119、118、117和当前六件的原文分文件字面次数已分别重算。120 E072署名LABEL_SYSCALL，取71–75行，其中74行已经定义另一个符号 sysexit_ret_addr，75行的比较属于该处。只能将71–73、74–76按两个标签分别锚定或作普通位置，79–81是第三个返回标签，77–78的errno赋值和返回归一化是注释。不能保留原来跨两标签的合格状态。其余80窗核对源位置、单一定义及字面相等。历史表修正有效，121只需追加这一行及最终121本身的自检，原编号沿用。[E072原窗](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/evidence.md#L873-L885) · [三个汇编标签及注释](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/syscall/syscall.c#L70-L81) · [P2/P6协议](https://github.com/08822407d/MyOS2/blob/9df81502ae54/agent-workspace/tasks/00-gpt-task-protocol-v2.md)

## R118-02 · 实际条件路径关闭，C08补正机器节点名

在实际DOT上从`d_getcwd`可直接到同一内核getcwd定义。从cd成功清零出发，先经prepend再判长度；长度门失败绕过copy，在无其他写入的前提下为空缓存，分别能到pwd和提示符。给定正常完整写入且有NUL时可到有效缓存；copy失败沿d_partial_unknown走，不能到d_empty_cache。计数0/正数的退格请求及cat具体strcpy/strcat两条直接调用也已验。main启动及cd刷新是两个独立调用点。[实际D00图](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/shell-file-commands-paths.dot#L153-L198) · [cd清零与刷新](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L203-L207) · [长度与复制分支](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/d_path.c#L218-L231)

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	i = chdir(path);
	if(!i)
	{
		memset(current_dir, 0, PATH_MAX);
		getcwd(current_dir, PATH_MAX);
```
[固定源码 203–207](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L203-L207)

[VERIFIED mykernel/fs/vfs/d_path.c::MYOS_SYSCALL_DEFINE2(getcwd)]
```c
		else if (len > size)
			error = -ERANGE;
		else if (copy_to_user(buf, b.buf, len))
			error = -EFAULT;
		else
```
[固定源码 221–225](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/d_path.c#L221-L225)

报告第47、50行及facts字段中，`d_pwd_printf`、`d_prompt_printf`与文字`d_copyfail_unknown`并非实际DOT节点。规范名为`d_pwdprintf`、`d_promptprintf`以及两步`d_copyfail→d_partial_unknown`。facts的mandatory_path给的是必经点，作为实际逐边路径时须补`d_prepend`。119旧图 `a_loop→a_lf` 的类型为SEQUENCE；120另画main的cwd刷新，不把该旧边重命名成CONTROL。以下为C08可直接供机器消费的替代表，取代字段中的旧拼写和不相邻的跳步。按170规定的实际路径关闭历史问题。[原机器字段](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/facts-and-boundaries.yaml#L100-L128) · [119的SEQUENCE实边](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch-paths.dot#L132-L136)

```json
{
 "supersedes": [
  "D00.R118_02.mandatory_path",
  "D00.R118_02.mandatory_path_prompt_last",
  "shell-file-commands.md D00 getcwd path aliases"
 ],
 "previous_119_loop_to_lf_kind": "SEQUENCE",
 "observed_120_paths": [
  {
   "nodes": [
    "d_cd",
    "d_success",
    "d_cachezero",
    "d_getcwd",
    "d_kernelgetcwd",
    "d_prepend",
    "d_lencheck",
    "d_sizefail",
    "d_no_usercopy",
    "d_empty_cache",
    "d_pwdprintf"
   ],
   "edge_types": [
    "CONTROL",
    "CONTROL",
    "SEQUENCE",
    "BOUNDARY",
    "DIRECT_CALL",
    "SEQUENCE",
    "CONTROL",
    "CONTROL",
    "MODEL",
    "DATA"
   ]
  },
  {
   "nodes": [
    "d_cd",
    "d_success",
    "d_cachezero",
    "d_getcwd",
    "d_kernelgetcwd",
    "d_prepend",
    "d_lencheck",
    "d_sizefail",
    "d_no_usercopy",
    "d_empty_cache",
    "d_promptprintf"
   ],
   "edge_types": [
    "CONTROL",
    "CONTROL",
    "SEQUENCE",
    "BOUNDARY",
    "DIRECT_CALL",
    "SEQUENCE",
    "CONTROL",
    "CONTROL",
    "MODEL",
    "DATA"
   ]
  },
  {
   "nodes": [
    "d_cd",
    "d_success",
    "d_cachezero",
    "d_getcwd",
    "d_kernelgetcwd",
    "d_prepend",
    "d_lencheck",
    "d_usercopy",
    "d_fullcopy",
    "d_valid_cache",
    "d_pwdprintf"
   ],
   "edge_types": [
    "CONTROL",
    "CONTROL",
    "SEQUENCE",
    "BOUNDARY",
    "DIRECT_CALL",
    "SEQUENCE",
    "CONTROL",
    "MODEL",
    "DATA",
    "DATA"
   ]
  },
  {
   "nodes": [
    "d_cd",
    "d_success",
    "d_cachezero",
    "d_getcwd",
    "d_kernelgetcwd",
    "d_prepend",
    "d_lencheck",
    "d_usercopy",
    "d_copyfail",
    "d_partial_unknown",
    "d_promptprintf"
   ],
   "edge_types": [
    "CONTROL",
    "CONTROL",
    "SEQUENCE",
    "BOUNDARY",
    "DIRECT_CALL",
    "SEQUENCE",
    "CONTROL",
    "CONTROL",
    "MODEL",
    "MODEL"
   ]
  },
  {
   "nodes": [
    "d_main",
    "d_mainzero",
    "d_maingetcwd",
    "d_kernelgetcwd",
    "d_prepend",
    "d_lencheck",
    "d_sizefail",
    "d_no_usercopy",
    "d_empty_cache",
    "d_promptprintf"
   ],
   "edge_types": [
    "DIRECT_CALL",
    "SEQUENCE",
    "BOUNDARY",
    "DIRECT_CALL",
    "SEQUENCE",
    "CONTROL",
    "CONTROL",
    "MODEL",
    "DATA"
   ]
  },
  {
   "nodes": [
    "i_bs",
    "i_count0",
    "i_underscore"
   ],
   "edge_types": [
    "CONTROL",
    "CONTROL"
   ]
  },
  {
   "nodes": [
    "i_bs",
    "i_countpos",
    "i_decrement",
    "i_secondbs",
    "i_underscore"
   ],
   "edge_types": [
    "CONTROL",
    "CONTROL",
    "SEQUENCE",
    "SEQUENCE"
   ]
  },
  {
   "nodes": [
    "d_cat",
    "d_strcpy"
   ],
   "edge_types": [
    "DIRECT_CALL"
   ]
  },
  {
   "nodes": [
    "d_cat",
    "d_strcat"
   ],
   "edge_types": [
    "DIRECT_CALL"
   ]
  }
 ],
 "copy_failure_reaches_empty_cache": false
}
```

## R119-01 · 五种输入的实际分支均已对齐

原始输入→按空格分词→按十项表线性比较→parse返回→main分支逐格复算，用D120-C01～C05将报告、JSON行和实际图节点对应。C01保留首尾空格，2个词、比较1次、索引0；C02为pwd、比较3次、索引2；C03为nope、比较10次、查找返−1；C04为空、C05为全空格，均0次比较、find_called=false、find_return=null、parse_return=−1。三个负解析结果（C03至C05）只请求main错误行。全五例的先行LF与分支输出分开。[五行正文](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/shell-file-commands.md#L54-L67) · [五个机器行](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/facts-and-boundaries.yaml#L129-L246) · [main正负分支](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L94-L102)

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
		index = parse_command(buf, &__argc, &__argv);
		if(index < 0)
			printf("Input Error, No Command Found!\n");
		else
			run_cmd(index, __argc, __argv);	//argc,argv
```
[固定源码 98–102](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L98-L102)

DOT的`p_good`标签“positive index”须读作`index >= 0`，包含0；表绑定以case_id和index共同限制。`main_indirect_target`表示run_cmd内发生的间接调用最终目标，拥有间接调用点的是run_cmd。C01/C02可分别到cd/pwd，C03–C05均走错误printf；与正文和facts的条件一致。五例按此补记恢复使用。[run_cmd与查找](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L139-L151) · [五个案例图边](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/shell-file-commands-paths.dot#L198-L220)

115/116既有裁定保留，118/119各自已审专题与指定勘误组合可按 `PASS_PENDING_LOCAL` 消费。117仍受R117-02，120新增R120-01/02并保留同项计数门，两批为 `RETURN`。此次只关闭文档问题。完整本地P9和运行均未执行。
