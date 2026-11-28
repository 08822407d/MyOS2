---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-SHELL-INPUT-DISPATCH-119"
record_type: "phase2_independent_mainline_review"
status: "RETURN"
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
author_delivery: "CONFIRMED_STOP_WRITING"
historical_D00: "ONE_CLOSED_TWO_OPEN"
new_return: "R119-01"
source_text_windows: 46
eligible_after_table_reclassification: 43
prepublication_comments: 225
full_local_P9: "NOT_RUN"
build_QEMU_hardware: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/169-user-shell-input-dispatch-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CWD-DISPLAY-118-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CWD-DISPLAY-118-D00-115-117-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-21-shell-cwd118-reviewed-input119.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/evidence.md","myinitramfs/myshell/myshell.c","myinitramfs/mylib/myglibc/string/strcmp.c","mykernel/scripts/options_flags.cmake","mykernel/fs/vfs/d_path.c","mykernel/fs/vfs/open.c"]
---

# 第119批独立验收 · RETURN

第119批作者交付完整。读行和分词的有界模型可以消费；命令派发遇到负索引时须按本回执纠正机器表。历史三项中关闭R118-01，R117-02及R118-02继续开放，本批新增R119-01。[169号合同](https://github.com/08822407d/MyOS2/blob/009d3ffdaa7d/agent-workspace/lead/MYOS2-LEAD-002/169-user-shell-input-dispatch-contract.md) · [119作者正式交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7062147403) · [119固定六件](https://github.com/08822407d/MyOS2/tree/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119) · [历史裁定](CORE-USER-SHELL-INPUT-DISPATCH-119-D00-117-118-review.md)

## 交付物及作者身份

直接读四分支：lead `009d3ffdaa7d`、exec `9957ff1d927f`、time `a039d9803ade`、master `e8e60cd19540`。原PR16 Open/Ready、PR17原题Open/Draft，均向master、未合并。唯一执行父 `cb96288abb8b`，对比恰1提交、6新增、2146新增行，0修改/删除旧件。逐文件比较fixed/active正文，再按UTF-8计算带对象头的Git Blob SHA-1，与树、compare、两次文件读取的Blob一致。

| 成果 | 行 | 字节 | 重算Blob短12 |
|---|---:|---:|---|
| [MANIFEST.md](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/MANIFEST.md) | 162 | 11264 | `7fbfc4a08a42` |
| [shell-input-dispatch.md](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch.md) | 122 | 15001 | `d01a010e2c4e` |
| [facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/facts-and-boundaries.yaml) | 763 | 19409 | `1d3e4cc06c8c` |
| [shell-input-dispatch-paths.dot](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch-paths.dot) | 282 | 17727 | `aa2d14324cc5` |
| [deferred-findings.yaml](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/deferred-findings.yaml) | 210 | 9868 | `fff0742e440a` |
| [evidence.md](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/evidence.md) | 607 | 17999 | `eab92a98e260` |

总计91268字节。119正式作者评论按ID独立双读并比对顶层分页正文，100/100/25/0，225条中本人标记一次；核上轮正式发布及执行身份C08a/CHAT-EXEC-18。MANIFEST有168接收及指引刷新记录；四条金丝雀与direct master公约原句均回源命中。所列项目输入路径有对应Git对象，六件无40位提交串。上述结论只覆盖可独立读取的对象。[MANIFEST](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/MANIFEST.md) · [C08发布169](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7061777812)

## A · 逐键读行

从固定main和read_line重建11个事件，逐格核read结果、count前后、改写位置和值、请求字节序、缓冲C串及显式返回。`p x BS w d LF` 的缓冲依次为 p、px、p、pw、pwd、pwd；请求字节数依次3/3/3/3/3/1，LF返回3。空LF返回0且发BS；计数0时BS发BS与下划线，NUL仅发BS；已录pwd后EOT或read=0不进入循环体。main在调用后另请求LF。函数尾无显式返回，此处main未取用返回值，不据此推定整轮发生未定义行为或给EOT捏造返回数。A表与机器数据11/11相符。[正文按键表](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch.md#L69-L86) · [read_line完整体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L106-L137)

[VERIFIED myinitramfs/myshell/myshell.c::read_line]
```c
		case '\b':
			if (count > 0)
			{
				buf[--count] = 0;
				write(console_fd, "\b", 1);
```
[固定源码 122–126](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L122-L126)

## B · 分词与借用的缓冲区

用4096个预清零字节、ASCII空格、短输入、成功分配以及8字节指针和零值表示NULL的模型，独立按扫描、写NUL、跳过空格、取词首和查表复算六组。机器表的词尾、词首、分配量、覆盖下标、残余空格、游标和索引均相符。`  cd  docs  ` 的词尾为3/9，词首2/6，覆写4/10，保留空格0/1/5/11，申请24字节。`pwd`与`pwd   `均申请16字节，分别覆写原终止符与首个尾空格。全空格和空行均未分配。词直接借用main的buf，本文只在所读函数内观察向量的生命周期。[分词实表](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch.md#L88-L102) · [parse_command完整体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L154-L186)

[VERIFIED myinitramfs/myshell/myshell.c::parse_command]
```c
		while(buf[j] && buf[j] != ' ')
			j++;
		buf[j++] = '\0';
		while(buf[j] == ' ')
			j++;
```
[固定源码 178–182](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L178-L182)

## C · 负索引错误分支与 R119-01

源码十项表、strcmp循环顺序及run_cmd的间接调用成立。五组的argc、比较次数、解析索引、间接目标、独立LF及错误格式均与固定源相符，三组的机器字段 `direct_call` 错。下表的调用列只指主循环在解析后的本次分支。

| 输入/模型 | 比较数 | find_cmd是否调用及返回 | parse返回 | main在分支内调用 |
|---|---:|---|---:|---|
| cd docs（含多空格等价输入） | 1 | 是，0 | 0 | run_cmd，间接调cd_command |
| pwd | 3 | 是，2 | 2 | run_cmd，间接调pwd_command |
| nope | 10 | 是，−1 | −1 | printf错误消息 |
| 空行 | 0 | 否 | −1 | printf错误消息 |
| 全空格 | 0 | 否 | −1 | printf错误消息 |

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
		index = parse_command(buf, &__argc, &__argv);
		if(index < 0)
			printf("Input Error, No Command Found!\n");
		else
			run_cmd(index, __argc, __argv);	//argc,argv
```
[固定源码 98–102](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L98-L102)

[VERIFIED myinitramfs/myshell/myshell.c::parse_command]
```c
		if(buf[i] != ' ' && (buf[i+1] == ' ' || buf[i+1] == '\0'))
			(*argc)++;
	}
	if(!*argc)
		return -1;
```
[固定源码 166–170](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L166-L170)

**R119-01 OPEN。** 空格行、nope、空行的机器字段（708/719/730行）均填 `main -> run_cmd`；与真实分支以及同一行对象里的空callee和错误输出相冲突。空行/全空格的`find_cmd_index=-1`还混淆了提前退出与真实查找失败。正文这两行也应将其列表头区分为parse结果、是否调用find和实际find返回。`case_ids`以 `cd docs` 为键，而第一条机器案例保留首尾多空格，按原串无法连接该ID。请119原件与本表合读；120追加五行显式case_id和完整字段，正负分支交叉复算。[机器表关键五行](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/facts-and-boundaries.yaml#L672-L735) · [原报告C表](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch.md#L105-L116) · [查找和间接调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L139-L152)

## 图、引文与未做事项

独立解析两件JSON兼容YAML，DOT有D00A/D00B/D00C及A/B/C六簇，共102个唯一节点、154条具类型边、零悬空端点。独立选取21组有向必经点均可达；进一步指定实际调用位置、失败条件后出现断路，详见历史裁定。对当前A图，i_bs_case→i_uscore裸分支须限定count=0，count>0经过清字节及第二次BS。若比较真正直接调用，cat→“文件名构造”是多个表达式/函数的摘要，单个可调用符号需另标。[实际替代与A图](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch-paths.dot#L163-L221)

<details><summary>独立复查的21组有向必经点（按DOT节点名）</summary>

```
1. a_main a_mzero a_mgetcwd a_copied a_cache a_prompt
2. a_main a_mgetcwd a_untouched a_cache a_pwd c_pwdprintf
3. a_main a_loop a_lf a_prompt a_fflush a_read
4. b_cd b_cmp b_dot b_return
5. b_cd b_chdir b_kernel b_lookup b_pwdset b_retstatus b_success b_cachezero b_getcwd b_return
6. b_cd b_chdir b_kernel b_lookup b_retstatus b_fail b_errprintf b_return
7. c_maincall c_kernelgetcwd c_alloc c_prepend c_len c_usercopy c_succret c_kfree c_return
8. c_cdcall c_kernelgetcwd c_pwd c_prepend c_len c_err c_kfree c_return
9. c_cdcall c_kernelgetcwd c_len c_usercopy c_err c_kfree c_return
10. c_usercopy c_valid c_cache c_pwdprintf
11. c_usercopy c_nowrite c_cache c_promptprintf
12. c_usercopy c_nowrite c_cache c_catpath
13. i_readline i_read i_condition i_bs1 i_switch i_default i_echo i_uscore i_read
14. i_bs_case i_decrement i_bs2 i_uscore
15. i_switch i_nul i_read
16. i_switch i_lf i_mainlf i_parse
17. i_condition i_stop i_mainlf
18. p_parse p_jskip p_scan p_zero_words p_retneg d_negative d_error
19. p_parse p_alloc p_zeroargv p_ptr p_walk p_terminate p_skiprest p_find d_find d_compare
20. d_compare d_hit d_positive d_run d_target d_pwd d_pwdprintf
21. d_more d_miss d_negative d_error d_console
```
</details>

46段、5份固定源码逐字比对成功，正式前缀亦为46。三段全局命令数组初始化不是协议定义体，详情见D00。deferred九行涵盖三项待审及六项限定，真实控制台、完整本地P9、编译、链接、QEMU及实机均 `NOT_RUN`。[第120批任务](../170-user-shell-file-commands-contract.md)
