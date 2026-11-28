---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "independent_historical_errata_adjudication"
status: "CLOSED_DOCUMENTATION_ONLY"
phase: 2
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话自报）"
model_per_owner: "Pro，后台与effort未独立认证"
date: "2026-11-22"
date_timezone: "Asia/Shanghai"
read_channel: "connector"
base_snapshot: "time"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
execution_pin_short12: "16398b8b96c6"
lead_before_publication_short12: "561261755177"
batch122_composite: "PASS_PENDING_LOCAL"
batch123: "RETURN"
closed_documentation_only_this_round: ["R122-01"]
open_returns_this_round: ["R123-01"]
followup_id: "CORE-USER-LIBC-STRING-COMPARE-123"
adjudicated: "R122-01"
supersedes_scope: "122 typed calls and 123 D00 instance edges/argument numbering only"
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":2,"quotes_reconfirmed":2,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-OPEN-FLAG-ABI-122-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-OPEN-FLAG-ABI-122-D00-120-121-review.md","agent-workspace/lead/MYOS2-LEAD-002/173-user-libc-string-compare-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/stdio-open-flag-abi-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/facts-and-boundaries.yaml","myinitramfs/myshell/myshell.c","mykernel/fs/vfs/open.c","mykernel/fs/vfs/namei.c","myinitramfs/mylib/myglibc/libio/fileops.c","myinitramfs/mylib/myglibc/libio/iofopen.c","mykernel/scripts/options_flags.cmake"]
---

# D00 · R122-01 调用关系独立裁定

**R122-01 文档关闭。** 第122批原件、122审查中的访问位补记、123的D00来源表和本回执中的实例边与参数补记，组合为 `PASS_PENDING_LOCAL`。此次关闭覆盖122的指定退回项。123新专题中 cd、ls、find_cmd 的实例边另按 R123-01 退回。[122原裁定](https://github.com/08822407d/MyOS2/blob/561261755177/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-OPEN-FLAG-ABI-122-review.md) · [前轮历史裁定](https://github.com/08822407d/MyOS2/blob/561261755177/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-OPEN-FLAG-ABI-122-D00-120-121-review.md) · [122原图](https://github.com/08822407d/MyOS2/blob/7d0e810ec4ff/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/stdio-open-flag-abi-paths.dot) · [123追加图](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare-paths.dot#L273-L343)

## 源码直接调用与同函数内顺序

已从固定定义回核机器表17个调用现场，含15项直接调用与2项 i_op 间接调用；逐对检查DOT端点和边类型。另核四个字符串比较调用现场，静态图因此共有19项直接调用及2项间接调用。将两个方法、两种清理所属函数及 rm/rmdir 的同名操作分别展开，核11个特定端点组合均无误标的直接调用边。案例起点及条件节点的实例边按下节勘误。[17个来源和守卫](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/facts-and-boundaries.yaml) · [静态边](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare-paths.dot#L273-L305)

| 实际拥有者 | 调用与守卫 | 顺序或数据 |
|---|---|---|
| do_sys_open | 分别调用 build_open_how、do_sys_openat2 | 第一项返回的 how 是局部对象，随后传 &how。 |
| do_creatat / do_mkdirat | 方法表可访问。间接调用 create / mkdir；回调正常返回后由本函数直接调用 done_path_create 和 putname。 | D121-A02/B02的回调返回0。缺相应方法的A01/B01则早返 −EPERM，两项清理及回调不可达。用户包装的整数取决于选用的库及模型是否给定透传；touch/mkdir 在正常返回的 argc=2 路径最终返回0。 |
| rm_command / rmdir_command | argc=2 时分别直接调用 unlink / rmdir。随后 err != 0 才由各自本体直接调用 strerror 与 printf。 | 只有给定案例的值为−5。传给 printf 的第1个 C 实参是固定格式串，第2个是 strerror 的返回值（第1个 %s），第3个是路径（第2个 %s）。两个内建均返回各自 err。 |
| _IO_new_fopen / _IO_new_file_fopen / _IO_file_open | 包装器调用解析函数，合格模式可由解析函数调用五参 helper，helper 调用 open。 | C121-01 给定合法 FILE 分配及 fd=−1，helper 返回NULL，解析函数在339行返回此 result，包装器在87行转交。C121-04 的 q 在解析层返回NULL，helper/open未调用。C121-05和A121-03是同一类直接解析输入，初始fd=7，解析函数返回空指针，外层/helper/open均未调用。 |

[VERIFIED mykernel/fs/vfs/open.c::do_sys_open]
```c
long do_sys_open(int dfd, const char * filename, int flags, umode_t mode)
{
	open_how_s how = build_open_how(flags, mode);
	return do_sys_openat2(dfd, filename, &how);
}
```

[固定源码 266–270](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/open.c#L266-L270)

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
	char *filename = argv[1];
	err = unlink(filename);
	if (err != 0)
		printf("rm: %s: %s\n", strerror(errno), filename);
```

[固定源码 369–372](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L369-L372)

## C08 对追加实例边及数据栏的定点替代

1. 123追加图317、323行的 valid 节点表示“方法存在”的条件。各自到回调应为 CONTROL，真正的 INDIRECT_CALL_INSTANCE 从 do_creatat/do_mkdirat 的该次函数实例发出。回调后 done、putname 的顺序和各自实际调用者按上表。331行同理，mode 被接受是条件；拥有五参 helper 调用点的是解析函数实例。两者都应保留。[三条实例边](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare-paths.dot#L315-L334) · [create](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/namei.c#L916-L929) · [mkdir](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/namei.c#L983-L996) · [五个实参](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/libio/fileops.c#L264-L267)
2. D00 表中的 printf_arg1/arg2 与部分图节点标签，统一读为第1/2个格式占位参数。按 C 函数全部实参计，字符串结果和路径是 arg2/arg3。正文第54行“负值条件”以源码的 err != 0 为准。第56行起的错误边表应按“原边起点、原边终点、处置”三列读取。[原说明及表](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare.md#L54-L65) · [rmdir 的实参](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L355-L360) · [rm 的实参](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L369-L374)
3. D121-R03/D03为给定错误例。保留每例的 argc=2、返回值−5和本命令自己的诊断输出，把旧图中 strerror 到 printf 的直接调用改为返回值供给。实际 errno 内容未知。C121-01 的 open 返回的是整数−1，helper 判断后才生成空指针；C121-04/05 和 A121-03 按上表区分未参与层。[open结果](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/libio/fileops.c#L178-L181) · [解析层两个早退](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/libio/fileops.c#L209-L230) · [解析返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/libio/fileops.c#L337-L340) · [包装器返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/libio/iofopen.c#L83-L88)

以下为可由后继直接消费的增量关系。使用调用边时同时读取实例拥有者和守卫。

```json
{
  "retype_as_control": [
    ["D121_A02_valid","D121_A02_cb"],
    ["D121_B02_valid","D121_B02_cb"],
    ["C121_01_mode","C121_01_helper"]
  ],
  "instance_calls": [
    ["D121-A02","do_creatat","dir->i_op->create","INDIRECT_CALL_INSTANCE","namei.c:921"],
    ["D121-B02","do_mkdirat","dir->i_op->mkdir","INDIRECT_CALL_INSTANCE","namei.c:988"],
    ["C121-01","_IO_new_file_fopen","_IO_file_open","DIRECT_CALL_INSTANCE","fileops.c:267"]
  ],
  "diagnostics": {
    "guard":"err != 0",
    "D121-R03":{"caller":"rm_command","err":-5,"format_owner":"rm","calls":["strerror","printf"],"printf_args":["format","strerror_result","filename"],"builtin_return":-5},
    "D121-D03":{"caller":"rmdir_command","err":-5,"format_owner":"rmdir","calls":["strerror","printf"],"printf_args":["format","strerror_result","dirname"],"builtin_return":-5}
  },
  "direct_inner_only":["C121-05","A121-03"],
  "wrapper_helper_open_called_in_direct_inner_only":[false,false,false]
}
``

122的四个模式、八组 how 值及低两位访问模式按原审查限定沿用。本回执只裁定源码文档。完整本地P9、编译链接及运行仍待有相应执行条件时核验。[固定数值及访问模式补记](https://github.com/08822407d/MyOS2/blob/561261755177/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-OPEN-FLAG-ABI-122-review.md)
