---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "independent_historical_correction_review"
status: "THREE_CLOSED_DOCUMENTATION_ONLY"
phase: 2
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话自报）"
model_per_owner: "Pro，后台与effort未独立认证"
date: "2026-11-22"
date_timezone: "Asia/Shanghai"
base_snapshot: "time"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "72abc20e7a70"
execution_pin_short12: "7d0e810ec4ff"
batch120_composite: "PASS_PENDING_LOCAL"
batch121_composite: "PASS_PENDING_LOCAL"
batch122: "RETURN"
closed_documentation_only_this_round: ["R120-02","R121-01","R121-02"]
open_returns_this_round: ["R122-01"]
followup_id: "CORE-USER-STDIO-OPEN-FLAG-ABI-122"
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":2,"quotes_reconfirmed":2,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/172-user-stdio-open-flag-abi-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-FILE-OPEN-121-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-FILE-OPEN-121-D00-117-120-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/stdio-open-flag-abi.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/stdio-open-flag-abi-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/evidence.md","myinitramfs/mylib/myglibc/libio/fileops.c","myinitramfs/mylib/myglibc/libio/iofopen.c","mykernel/fs/vfs/namei.c","myinitramfs/myshell/myshell.c","mykernel/scripts/options_flags.cmake"]
---

# 第122批D00独立裁定 · 三项历史纠正关闭

| 编号 | C08裁定 | 通过的条件 |
|---|---|---|
| R120-02 | `CLOSED_DOCUMENTATION_ONLY` | 13个命令案例与11列表格对齐，给定缺少方法的两个入口只到相应早返，rm/rmdir分支隔离。 |
| R121-01 | `CLOSED_DOCUMENTATION_ONLY` | 五参与三参归属、五个返回现场、直接内层fd=7和b的游标更新均可核。C121-04的显示栏用下表补记。 |
| R121-02 | `CLOSED_DOCUMENTATION_ONLY` | E084/E085现为正式的活动C配置短窗，85个引文的最终P6分账正确。 |

120、121原件连同各自历史回执、本轮122纠正和这里的裁定，可按 `PASS_PENDING_LOCAL` 组合使用。122自身的调用图类型另归R122-01。[172号合同](https://github.com/08822407d/MyOS2/blob/72abc20e7a70/agent-workspace/lead/MYOS2-LEAD-002/172-user-stdio-open-flag-abi-contract.md) · [121独立审查](https://github.com/08822407d/MyOS2/blob/72abc20e7a70/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-FILE-OPEN-121-review.md) · [121历史裁定](https://github.com/08822407d/MyOS2/blob/72abc20e7a70/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-FILE-OPEN-121-D00-117-120-review.md) · [122整包回执](CORE-USER-STDIO-OPEN-FLAG-ABI-122-review.md)

## R120-02 · 从真实case_id验证正反两路

C08从13个D121入口分别枚举最终DOT可达节点，并与facts逐行比较回调、done、putname、输出及内建结果。13个入口的可达节点均限于本案例，正文每一行有11列，内核返回、用户接口、局部err和内建返回各有独立栏。argc错误的四例均止于−1。rm/rmdir按给定用户返回0或−5分开，负例只到本命令的strerror、printf及其原返回；给定−5不能独立确定errno或显示文字。表中格式串末尾的转义按源码C字符串换行解释。[D00正文](https://github.com/08822407d/MyOS2/blob/7d0e810ec4ff/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/stdio-open-flag-abi.md#L27-L49) · [真实案例边](https://github.com/08822407d/MyOS2/blob/7d0e810ec4ff/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/stdio-open-flag-abi-paths.dot#L237-L314) · [shell命令](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L325-L375)

- D121-A01/B01的唯一前缀分别为本case → argc → valid → absent → eperm → user → err → builtin0。从同一个case核禁止路径，回调、done和putname均不可达。节点“i_op missing”精确指给定dir及其i_op可访问，分别缺create或mkdir方法；整张i_op指针为空不满足该模型。内核早返−EPERM，用户层值以所选库正常返回且无改写为条件，内建返回0。
- D121-A02/B02在给定回调为0时两项清理均可达；D121-B03给定有效name及filename_create的错误结果，只到putname。创建函数才是这几次调用的拥有者，图上执行顺序按本轮整包回执的R122-01补正。

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
		dir = path.dentry->d_inode;
		error = -ENOENT;
		if (!dir->i_op->create)
			return -EPERM;
```

[固定源码 916–919](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/namei.c#L916-L919)

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
		dir = path.dentry->d_inode;
		error = -ENOENT;
		if (!dir->i_op->mkdir)
			return -EPERM;
```

[固定源码 983–986](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/namei.c#L983-L986)

## R121-01 · 哪一层收到哪些参数，哪一层返回

已对照源码的两个调用点，并核机器件中按顺序存储的5个和3个实参。r模型的两组实参分别为(P, note, 0, 438, 8)与(note, 0, 438)。C121-01/02/03的图分别检验：移除解析、循环结束或五参helper中任一必经节点后，从本案例都到不了open。C121-04和C121-05、A121-03的路径均无open。[包装器](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/libio/iofopen.c#L82-L88) · [实际open三参](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/libio/fileops.c#L173-L181) · [helper五参](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/libio/fileops.c#L264-L267) · [原表与解释](https://github.com/08822407d/MyOS2/blob/7d0e810ec4ff/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/stdio-open-flag-abi.md#L53-L70)

| case_id | 包装器/解析函数/五参helper | open次数 | 给定fd → 最终_fileno | 解析函数返回 / 包装器返回 |
|---|---|---:|---|---|
| C121-01 | 调用 / 调用 / 调用 | 1 | −1 → −1 | NULL / NULL |
| C121-02 | 调用 / 调用 / 调用 | 1 | 0 → 0 | 同一P / 同一P |
| C121-03 | 调用 / 调用 / 调用 | 1 | 5 → 5 | 同一P / 同一P |
| C121-04 | 调用 / 调用 / **未调用** | 0 | 未给定 → −1 | NULL / NULL |
| C121-05、A121-03 | 未调用 / 直接调用 / 未调用 | 0 | 未给定 → 7（原值） | NULL / 未调用 |

122正文第64行第一栏误套“包装→模式→helper”；同一行后两栏、行后解释、facts和DOT均给出helper未调用。此回执以“包装器→解析函数，在q早返”替代该单元。wbx图上b节点只写last_recognized=1，随后扫描x，x才增加O_EXCL并记录下标2。实际sh所链库保留UNKNOWN。[坏首字符及已有fd分支](https://github.com/08822407d/MyOS2/blob/7d0e810ec4ff/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/stdio-open-flag-abi-paths.dot#L349-L360) · [入口和坏首字符](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/libio/fileops.c#L209-L230) · [x和b两个分支](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/libio/fileops.c#L243-L249)

## R121-02 · 活动内核C配置的证据

E084逐字命中39–43行，处在设置内核C编译选项的命令内；E085命中64–66行，处在追加活动CONFIG定义的另一条命令内。它们支持内核源配置；用户库目标的编译定义另从用户树CMake取证。全部85个正式前缀只出现在evidence，声明数、回读数、降级数为85、85、0。原121的P19继续作普通位置。[两处新短窗](https://github.com/08822407d/MyOS2/blob/7d0e810ec4ff/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/evidence.md#L905-L927) · [内核C配置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L39-L70) · [用户库目标配置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/CMakeLists.txt#L1-L16)

三项关闭限于静态文档，完整本地P9和程序运行均未执行。
