---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-RETURN-SIGNAL-09
record_type: stdio08_disposition
produced_by: "Claude Code desktop app (Code tab), local session f464c1b9-9b25-4af2-8f2d-d419b3b601fd (MYOS2-A-CC02 内核分析本地执行)"
model: unknown_or_not_attestable
model_declared_by_runtime: claude-opus-5-5
date: "2026-10-29"
base_snapshot: "kernel=time a039d9803ade；执行分支起点 8983a9235c8a；主线 agent/MYOS2-LEAD-002 14ecef9fc596；master e8e60cd19540（均为短 SHA）"
inputs_read: "见 evidence.md §2"
status: final_for_user_return_signal_09
runtime_evidence: NOT_RUN
owner_decisions_requested_now: []
---


# 08 回传项处置：SR08-01 与 SR08-02

本文处置主线对 CORE-USER-SYSCALL-STDIO-08（下称 08）提出的两项回传：SR08-01（fdput 引用计数措辞的条件）与 SR08-02（分派局部的 ax 与最终返回值）。源码以 `time a039d9803ade` 为准，08、07 包按 `8983a9235c8a` 读取。05、07、08 的原文件一律不改；需要更正的表述登记为本包勘误：SR08-01 对应 ER-01，SR08-02 对应 ER-02，二者都是 documentation_erratum，状态 superseded_interpretation。07 的同类表述由 ER-03 处理。全部结论是源码层面的静态结论，没有构建、预处理或运行任何东西；运行层一律 NOT_RUN。

**编号约定。** 不带前缀的 RS、ER、NVR、ED、AD、QD、SD 编号指本包（CORE-USER-RETURN-SIGNAL-09）；AS、QS、SS、FL、PU、US、ES、VT-FDLIMITS、VT-SYSTAB 指 08；UB、AB、QB、SB、FP、NVB、VT-PTREGS、VT-FLAGS 指 07；UV、A-C4-xx、GAP-Uxx 指 05。§2.6、§3.8 与结构化字段一律写全名。位置编号 L1-xx（SR08-01）与 L2-xx（SR08-02）在本文 §2.5、§3.7 中编排，sr08_dispositions 的 locations 与勘误 ER-02 引用的也是这套编号；它们与 user-return-signal.md“衔接”一节的小节名 L1–L8 无关，与未交付的 D00 草稿中的 Lxx、Mxx 同序。

## 0. 结论

| 回传项 | 结论 | 要点 |
|---|---|---|
| SR08-01 | 接受（accept） | US-01 的结论不变，但 08 各处的“每次”“返回前”“无条件”都要带条件。下标在 [0,256)、槽内是可用的非空 file、控制到达清理时，write、writev 净减一；成功的 F_DUPFD 与成功的 F_DUPFD_CLOEXEC 净值为零；空槽不进入 fdput。复核新增：US-01.claim 的“在返回前无条件 fdput_pos”与源码不符。共 35 处位置。 |
| SR08-02 | 接受（accept） | 08 写的“用户看到/拿回的 rax”只是分派后 regs->ax 的局部结果。返回尾部无条件运行重启与信号逻辑，可能改写 ax、ip、sp 等；返回段装入 rax 的是改写之后帧中的 ax。“rax 等于分派值”的四条前提合起来只是充分条件，其中“没有信号递送”可以放宽为“没有成功建立信号帧”（§3.3）。越界号的 64 位 rax 恰为 -512、-513、-514、-516 时，没有信号也会改写帧：四种都令 ip 减 2，-516 另把 ax 改为 219（-512、-513、-514 的 ax 写回同值）。08 中 20 处位置，另有 07 一处。 |

- 两项更正都以附注形式登记在本包，08 原文件不改：SR08-01 见 §2.4 与 ER-01（目标 CORE-USER-SYSCALL-STDIO-08::US-01）；SR08-02 见 §3.6 与 ER-02（目标 CORE-USER-SYSCALL-STDIO-08::entry.syscall_dispatch）。
- 07 的 CORE-USER-BOUNDARIES-07::UB-09 有同一说法，由 ER-03 处理（§4）。
- GAP-U04、GAP-U10 都不因本处置关闭。

## 1. 依据与复核

**只引用、不重述的既有事实。**
- 08：__to_fd 与 fdget 系列、fdput、fdput_pos（AS3-13 至 AS3-17、SS-07、VT-FDLIMITS）；write、writev、fcntl 的取槽与清理调用（AS3-20 至 AS3-23、AS3-25、AS3-26、SS-03 至 SS-06、SS-09、SS-10）；fcntl 的分支（AS3-27、AS3-39、SS-17、SS-18、QS-26）；f_dupfd（AS3-28、AS3-37、SS-21、SS-22）；fput 归零释放（AS3-11、SS-11）；入口建帧与分派（AS2-02 至 AS2-06、AS2-13、QS-18、SS-15、VT-SYSTAB）。
- 07：建帧顺序与返回段（SB-01、SB-02、AB2-13、AB2-16、VT-PTREGS、FP-3）；分派写 ax（AB2-14）；exit_to_user_mode_loop 无条件调用 arch_do_signal_or_restart（AB2-12、SB-14、SB-15、QB-43）；SYSRET 判断不被使用（SB-16、UB-09）。
- 05：fd 下标不查上界、表满时 alloc_fd 返回未赋值的 fd（UV-11）；两槽共享后 close 直接 kfree（UV-10）；fput 的释放行（A-C4-28）。

下文新增的源码依据只列上述条目没有引过的行。

**复核。** D00 草稿经过一次怀疑式复核，得到 C1 至 C9 九项更正，本文全部采纳；草稿与复核记录都不随本包交付，各项更正的内容与落实位置见 §6。其中 C1 是源码层面的更正：fd_install 的 BUG_ON 不会让控制停下（§2.2.4）。

## 2. SR08-01：fdput 引用计数措辞的条件

### 2.1 回传要求与结论

主线要求写清 08 中“write、writev 每次、fcntl 除成功的 F_DUPFD 外每次都让 f_count 少一”这一类说法的成立条件。

**结论：接受。** US-01 的核心事实不变：fdget、fdget_pos、fdget_raw 都是 __to_fd，不取引用却返回 flags=3；fdput 按 FDPUT_FPUT 调用 fput，本身不判空（AS3-14）。要补的是条件：

1. 三处调用者都先判空。空槽（NULL）返回 -EBADF，不执行 fdput 或 fdput_pos。
2. 只有下标在 [0,256)、槽内是可用的非空 file、控制到达清理时，才有确定的净值。
3. 成功的 F_DUPFD 与成功的 F_DUPFD_CLOEXEC 共用 f_dupfd 里的 get_file，净值都为零。“成功”指 alloc_fd 在 [from,256) 找到空槽。
4. 失败的 F_DUPFD 要拆成两种：from ≥ RLIMIT_NOFILE 时净减一；alloc_fd 找不到空槽时不给净值。
5. fcntl 还有一条 08 没有列出的净减一路径：FMODE_PATH 下被白名单拒绝的命令（推断走不到，§2.2.5）。
6. 复核新增：US-01.claim 的“在返回前无条件 fdput_pos”与源码不符。“无条件”只在 vfs_write、vfs_writev 返回之后成立。

### 2.2 源码依据

#### 2.2.1 write 与 writev：清理只在 file 非空的分支里

ksys_write 取槽后先判空 read_write.c:325（QD-L21）。fdput_pos 在这个 if 块里 read_write.c:334（QD-L21）；空槽时直接 return ret，ret 的初值是 -EBADF（AS3-20）：

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]
```c
			filp->f_pos = pos;
		fdput_pos(f);
	}

	return ret;
```

vfs_write 的几个早退检查都以 return 回到 ksys_write read_write.c:293（QD-L21）。所以对非空槽，vfs_write 不论成败，只要返回，接着就执行 fdput_pos。do_writev 同样先判空 read_write.c:391（QD-L21），fdput_pos 同样在块内 read_write.c:400（QD-L21）。

#### 2.2.2 fcntl：空槽跳过 fdput，FMODE_PATH 走 out1

sys_fcntl 在空槽时 goto out（AS3-25）。out 标签在 fdput 之后，只有空槽路径会跳到这里：

[VERIFIED mykernel/fs/syscall.c::sys_fcntl]
```c
out1:
 	fdput(f);
out:
	return err;
```

f_mode 含 FMODE_PATH、且命令不在 check_fcntl_cmd 白名单时 goto out1：不调用 do_fcntl，但仍执行 fdput，返回 -EBADF syscall.c:149（QD-L18）。白名单是 F_DUPFD、F_DUPFD_CLOEXEC、F_GETFD、F_SETFD、F_GETFL（QS-26）。

do_fcntl 只有四个活动分支：F_DUPFD 与 F_DUPFD_CLOEXEC（AS3-27、SS-18），F_GETFL fcntl.c:29（QD-L19），以及 default（AS3-39）。F_GETFD、F_SETFD、F_SETFL 的 case 被注释，落到 default，err 保持初值 -EINVAL（SS-17）。F_GETFL 与 default 自己不取也不放引用，随后的 fdput 使它们净减一。

#### 2.2.3 F_DUPFD 与 F_DUPFD_CLOEXEC：成功与两种失败

两者都调用 f_dupfd，只差 flags 参数（AS3-27），而 alloc_fd 不使用 flags（SS-22）。f_dupfd 在 from ≥ RLIMIT_NOFILE 时直接返回 -EINVAL，不 get_file（AS3-28）；否则 alloc_fd 的返回值 ≥ 0 时先 get_file 再 fd_install（AS3-37）。get_file 把 f_count 加一 fs.h:906（QD-L23）。分开看：

- **成功**（alloc_fd 在 [from,256) 找到空槽）：get_file 加一，sys_fcntl 的 fdput 减一，净值为零。新旧两个槽指向同一个 file。
- **from ≥ RLIMIT_NOFILE**：只剩 fdput，净减一。argi 为负时转成很大的 uint，也属此类；前提是 RLIMIT_NOFILE 仍是初值 1024（VT-FDLIMITS 的假设）。
- **alloc_fd 找不到空槽**：循环结束后直接返回 fd file.c:144（QD-L20），而 fd 只在遇到 NULL 槽时才被赋值（05 UV-11）。f_dupfd 中 err ≥ 0 是否成立不确定，不给净值。
- **扫描越过 256 项**：end 为 nofile（1024）时，若 [from,256) 中没有空槽，循环可能读到 256 项之外（US-02）；[from,256) 中有空槽时，循环在表内就 break。越界找到的“空槽”同样走 get_file，也不给净值。

调用点的更正（C7）：ksys_write、do_writev、do_fcntl 各有一个活动调用点；f_dupfd 有两个，即 fcntl.c:16 与 19，都在 do_fcntl 内 QD-02。带 FDPUT_FPUT 的取法只有 write、writev、fcntl 三处活动调用 QD-01。

#### 2.2.4 fd_install 的 BUG_ON 不会让控制停下（C1，源码更正）

fd_install 不查下标范围。目标槽非空时触发 BUG_ON，然后无条件写槽 file.c:191（QD-L20）。BUG_ON 在条件为真时调用 BUG() bug.h:66（QD-L24）。BUG() 只打印并调用 panic，之后没有 unreachable：

[VERIFIED mykernel/include/linux/kernel/asm-generic/bug.h::BUG]
```c
	#			define BUG() do {									\
							printk("BUG: failure at %s:%d/%s()!\n",	\
								__FILE__, __LINE__, __func__);		\
							barrier_before_unreachable();			\
							panic("BUG!");							\
```

这两个宏位于 bug.h 第 20 行的 #ifndef __ASSEMBLY__、第 24 行的 #ifdef CONFIG_BUG、第 56 行与第 65 行的 #ifndef HAVE_ARCH_BUG(_ON) 之内。CONFIG_BUG 由配置面定义 options_flags.cmake:66（QD-L41）；HAVE_ARCH_BUG 与 HAVE_ARCH_BUG_ON 都没有定义：mykernel 与全仓构建文本中没有它们的 #define 或 -D 写法 QD-04；在同一范围内，这两个名字只出现在 bug.h 第 56、65 行的 #ifndef 与第 162、166 行的注释里 QD-233。barrier_before_unreachable 是空的内联汇编 compiler-gcc.h:55（QD-L22），compiler.h 第 107 行的后备定义也是空的 do-while。

panic 在源码层面会返回：
- 带 __noreturn 的声明在 panic.h 的注释里，生效的是普通声明 QD-05。
- panic.c 中的循环都在注释里 QD-06；panic 函数体的活动文本中没有循环 SD-01。
- 打印最后一条信息之后，开中断与停机循环都是注释，函数在第 214 行正常结束 panic.c:201（QD-L16）。
- 函数开头关中断 panic.c:61（QD-L16）、关抢占 panic.c:62（QD-L16），返回时没有恢复（后果未追）。

所以 BUG_ON 触发后，fd_install 仍执行第 192 行：覆盖原来的非空槽，丢掉原 file 的槽引用；然后经 f_dupfd、do_fcntl 回到 sys_fcntl 的 out1: fdput。D00 草稿把它算作“控制没有到达清理”，这不对，应另列为“fd_install 覆盖非空槽”。这种情形只出现在 alloc_fd 返回未赋值的 fd、越界或并发安装时；它仍不给净值，理由是原槽被覆盖。运行时后果 NOT_RUN。

#### 2.2.5 FMODE_PATH 路径能否走到（C9，推断）

FMODE_PATH（0x4000）只出现在四处：fs.h:99 的定义、sys_fcntl 第 149 行的测试、fget 传给 __simple_fget 的 mask 实参（file.c:220，函数体不使用它）、open.c:326 的注释 QD-07。对 f_mode 的活动赋值只有 file_table.c:60 的 OPEN_FMODE(flags) 与 block/fops.c 第 68 至 75 行的几处 |= QD-08。OPEN_FMODE 只产生访问位与 __FMODE_NONOTIFY（07 VT-FLAGS）；block/fops.c 置的 FMODE_NOWAIT、FMODE_BUF_RASYNC、FMODE_NDELAY、FMODE_EXCL、FMODE_WRITE_IOCTL 都不是 0x4000 QD-09。推断：活动写法不会置上 FMODE_PATH。(e4) 只可能出现在内容不由这些写法得到的 file 上，而那已属 (x)。

#### 2.2.6 计数归零后槽不被清空

fput 在计数归零时调用 kmem_cache_free（AS3-11、05 A-C4-28）。fput 所在的 file_table.c 不访问 fd 槽 QD-03，所以槽仍指向已释放的对象，之后该槽悬空。

### 2.3 条件表

读 08 各处的“每次”，以本表为准。

| 编号 | 情形 | write（ksys_write） | writev（do_writev） | fcntl（sys_fcntl） | 到达 fdput？ | 该 file 的 f_count 净变化 |
|---|---|---|---|---|---|---|
| (a) | 下标在 [0,256)，槽内是可用的非空 file，控制到达清理 | filp != NULL → vfs_write 返回（含 -EBADF、-EINVAL、-EFAULT 早退）→ fdput_pos | f.file 非空 → vfs_writev 返回 → fdput_pos | 见 (c) 至 (e4) | 是 | write、writev：−1。f_count 为 1 时 fput 进入 kmem_cache_free，槽仍指向它，此后该槽悬空，归入 (x) |
| (b) | 槽为空（NULL） | ret 保持 -EBADF，不进 if，不 fdput_pos | 同左 | !f.file → goto out，跳过 out1: fdput，返回 -EBADF | 否 | 0 |
| (c) | F_DUPFD 成功：from < RLIMIT_NOFILE，alloc_fd 在 [from,256) 找到 NULL 槽 | — | — | get_file（+1）、fd_install，返回新 fd；out1: fdput（−1） | 是 | 0。两槽共享同一个 file，计数不变（此后计数偏少：US-01 counter_evidence、05 UV-10） |
| (d) | F_DUPFD_CLOEXEC 成功（条件同 c） | — | — | 与 (c) 走同一段 f_dupfd，只多传 O_CLOEXEC；alloc_fd 不使用 flags（SS-22、US-03） | 是 | 0 |
| (e1) | F_DUPFD 或 F_DUPFD_CLOEXEC 因 from ≥ RLIMIT_NOFILE 失败（argi 为负也属此类，前提是 RLIMIT_NOFILE 仍为 1024） | — | — | f_dupfd 返回 -EINVAL，不 get_file；out1: fdput | 是 | −1 |
| (e2) | F_GETFL | — | — | err = filp->f_flags；fdput | 是 | −1 |
| (e3) | default（不支持的命令；case 被注释的 F_GETFD、F_SETFD、F_SETFL 也落到这里） | — | — | pr_alert，err 保持 -EINVAL；fdput | 是 | −1 |
| (e4) | f_mode 含 FMODE_PATH，且命令不在 check_fcntl_cmd 白名单 | — | — | goto out1，err 为 -EBADF；fdput | 是 | −1（推断走不到，实际已属 (x)，见 §2.2.5） |
| (x) | 不给净值的情形：①下标 ≥ 256，或经形参类型转换后越界（__to_fd 不查上界，05 UV-11）；②槽内是已释放（悬空）或未写入（PU-01）的指针；③alloc_fd 找不到空槽，返回未赋值的 fd（05 UV-11）；④F_DUPFD 或 F_DUPFD_CLOEXEC 的扫描越过 256 项（US-02，同样走 get_file）；⑤fd_install 覆盖非空槽（panic 返回后第 192 行照写，再回到 fdput，§2.2.4）；⑥控制没有到达清理（vfs_write、vfs_writev 或 ->write 不返回） | 适用 | 适用 | 适用 | 不确定 | 不能写成确定的净引用公式 |

**口径。** 净变化只算 fdget\*/fdput\* 这一对与 f_dupfd 中 get_file 的贡献；被调用的 ->write 等若自己改动 f_count，不计在内（未追）。ksys_write 的 unsigned int fd 与 do_writev 的 ulong fd 先变成 int（fdget_pos 的形参），再作为 ulong 下标；fcntl 的 uint fd 直接作 ulong 下标。这些转换按 GCC 的实现定义理解，属推断。

### 2.4 附注全文（供读 08 时对照；只登记在本包，08 原文件不改）

**附注 1-1（总述）。** US-01 的结论不变：fdget、fdget_pos、fdget_raw 都是 __to_fd，不取引用却返回 flags=3，到达 fdput 时做一次不配对的 fput。08 中的“每次”“返回前”“无条件”都按附注 1-2 至 1-6 的条件读。

**附注 1-2（write、writev）。** fd 下标在 [0,256)、槽内是可用的非空 file，并且 vfs_write 或 vfs_writev 返回时，write(2)、writev(2) 各做一次不配对的 fput，净减一；vfs_write 早退返回 -EBADF、-EINVAL、-EFAULT 时也一样。fdput_pos 位于 file 非空的分支内；槽为空时 ksys_write、do_writev 直接返回 -EBADF，不执行 fdput_pos。

**附注 1-3（fcntl）。** fd 下标在 [0,256)、槽内是可用的非空 file，并且控制经 out1 到达 fdput 时：成功的 F_DUPFD 与成功的 F_DUPFD_CLOEXEC（alloc_fd 在 [from,256) 找到空槽）由 f_dupfd 中的 get_file 抵消 fdput，净值为零，两槽共享同一个 file；F_GETFL、default（含 case 被注释的 F_GETFD、F_SETFD、F_SETFL）、FMODE_PATH 下被白名单拒绝的命令（推断走不到）、from ≥ RLIMIT_NOFILE 而失败的 F_DUPFD 与 F_DUPFD_CLOEXEC，净减一。槽为空时 sys_fcntl goto out，跳过 fdput，返回 -EBADF。

**附注 1-4（计数归零）。** f_count 为 1 时，这次 fput 进入 kmem_cache_free(filp_cachep, …)，槽仍指向它；之后该槽悬空，后续调用按附注 1-5 处理。

**附注 1-5（不给净值）。** 以下情形不能写成确定的净引用公式：下标 ≥ 256，或经形参类型转换后越界；槽内是已释放或未写入的指针；alloc_fd 找不到空槽、返回未赋值的 fd；F_DUPFD 或 F_DUPFD_CLOEXEC 的扫描越过 256 项（同样走 get_file）；fd_install 覆盖非空槽（BUG_ON 之后 panic 在源码层面会返回，第 192 行照写，再回到 fdput）；控制没有到达清理（vfs_write、vfs_writev 或 ->write 不返回）。

**附注 1-6（口径与另行登记）。** 净变化只算 fdget\*/fdput\* 这一对与 get_file 的贡献，->write 等自己改动 f_count 的情形未追。两槽共享后计数偏少（US-01 counter_evidence、05 UV-10）与 close-on-exec 没有实现（US-03）仍按原条目登记。

### 2.5 受影响位置（08 中 35 处）

文件简称：md = user-syscall-stdio.md，facts = facts-and-boundaries.yaml，deferred = deferred-findings.yaml，evidence = evidence.md，都在 08 包内；现文都按 8983a9235c8a 逐行核对过。

**结论列口径。** 接受：原句须按“追加的附注”改读；部分接受：原句的主体成立，只需补条件或改措辞（SR08-02 中多指分派局部的说法成立）；只补注：原句不变（锚点的 quote 与行号也不变），只追加说明；不受影响：不需要附注；关联：位置在 08 之外，另由本包勘误处理。§3.7 同此口径。

| # | 文件 | 位置 | 现文（短引） | 结论 | 追加的附注 | 涉及的结构化 ID |
|---|---|---|---|---|---|---|
| L1-01 | md | §0 第 3 条（33 行） | “write、writev 每次、fcntl 除成功的 F_DUPFD 外每次都让 f_count 少一（US-01）”；“唯一能复制 fd 的是 fcntl(F_DUPFD)” | 接受 | 按附注 1-2、1-3、1-5 改读：下标在 [0,256)、槽内是可用的非空 file、控制到达清理时，write、writev 净减一；fcntl 中成功的 F_DUPFD 与成功的 F_DUPFD_CLOEXEC 净值为零，F_GETFL、default、FMODE_PATH 白名单拒绝（推断走不到）与 from ≥ RLIMIT_NOFILE 而失败的 F_DUPFD、F_DUPFD_CLOEXEC 净减一，alloc_fd 找不到空槽、扫描越过 256 项或 fd_install 覆盖非空槽时虽到达 fdput 也不给净值；空槽时三者都返回 -EBADF，不 fdput；其余情形不给净值。同条的“在返回前 fput”同样按此读；“唯一能复制 fd 的是 fcntl(F_DUPFD)”读作 F_DUPFD 与 F_DUPFD_CLOEXEC（两者都经 f_dupfd，AS3-27）。 | US-01、FL-04、FL-05 |
| L1-02 | md | §0 第 4 条（34 行） | “write 与 writev 都在返回前多做一次 fput” | 部分接受（打开结果是条件） | 下标在 [0,256)、槽内是可用的非空 file 时，write 与 writev 在 vfs_write、vfs_writev 返回后各做一次不配对的 fput；空槽返回 -EBADF，不 fput。console 的打开结果取决于 05 UV-04、07 UB-05 中缺 return 的上层，只能写成条件。 | US-01、UB-01 |
| L1-03 | md | §4.2 FL-04 行“引用计数”列（338 行） | “每次减一而不先加一；1 → 0 时释放，槽仍指着它” | 接受 | 按附注 1-2、1-4：满足条件的每次调用净减一；1 → 0 时 kmem_cache_free，槽仍指向它，此后该槽悬空，后续调用不给净值；空槽返回 -EBADF，计数不变。 | FL-04、US-01 |
| L1-04 | md | §4.2 FL-05 行“引用计数”列（339 行） | “F_GETFL、失败的 F_DUPFD 与不支持的命令只减一” | 接受 | 按附注 1-3：成功的 F_DUPFD 与 F_DUPFD_CLOEXEC（在 [from,256) 找到空槽）净值为零，两槽共享一个 file；F_GETFL、default（含 case 被注释的 F_GETFD、F_SETFD、F_SETFL）、FMODE_PATH 白名单拒绝、from ≥ RLIMIT_NOFILE 而失败的 F_DUPFD 与 F_DUPFD_CLOEXEC 净减一；空槽 goto out，不 fdput；alloc_fd 找不到空槽、扫描越过 256 项或 fd_install 覆盖非空槽时不给净值（05 UV-11）。 | FL-05、US-01、US-02、US-03 |
| L1-05 | md | §4.3 第 3 段引导句（370 行） | “于是 write 不论 `vfs_write` 成败，返回前都会 fput 一次” | 部分接受 | 于是只要下标在 [0,256)、槽内是可用的非空 file，并且 vfs_write 返回，write 不论成败都会 fput 一次；槽为空时 ksys_write 直接返回 -EBADF，不 fput。 | US-01 |
| L1-06 | md | §4.3 末段（396 行） | “或第一次除成功的 F_DUPFD 之外的 fcntl(2) 之后，file 就被释放” | 接受 | 第一次 write(2)、writev(2)，或第一次净减一的 fcntl(2)（成功的 F_DUPFD、F_DUPFD_CLOEXEC 不在其内）之后，file 被释放而槽仍指向它。成功的 F_DUPFD 与 F_DUPFD_CLOEXEC（在 [from,256) 找到空槽）都先 get_file，与 fdput 相抵；F_GETFL、default、FMODE_PATH 拒绝与 from 越限而失败的 F_DUPFD、F_DUPFD_CLOEXEC 只剩这次 fput；空槽上三种调用都返回 -EBADF，不 fput。 | US-01 |
| L1-07 | md | §4.6 表 write(2) 行（459 行） | “以写方式打开时返回 -EINVAL（UB-01），并多 fput 一次（US-01）” | 部分接受 | 对下标在 [0,256)、槽内是可用的非空 file 的 fd：以写方式打开时返回 -EINVAL（UB-01），并多 fput 一次（US-01）；f_count 为 1 时首次之后槽悬空。空槽返回 -EBADF，不 fput。 | US-01、UB-01 |
| L1-08 | md | §4.6 表 writev(2) 行（460 行） | “直接用用户指针（UB-07）；多 fput 一次（US-01）” | 部分接受 | 下标在 [0,256)、槽内是可用的非空 file 且 vfs_writev 返回时多 fput 一次（US-01）；空槽返回 -EBADF，不 fput。 | US-01 |
| L1-09 | md | §4.6 表 fcntl(2) 行（463 行） | “除成功的 F_DUPFD 外多 fput 一次（US-01）” | 接受 | 成功的 F_DUPFD 与 F_DUPFD_CLOEXEC（在 [from,256) 找到空槽）净值为零；下标在 [0,256)、槽内是可用的非空 file 时，其余到达 fdput 的命令（含 from 越限而失败的 F_DUPFD、F_DUPFD_CLOEXEC）多 fput 一次（US-01）；空槽返回 -EBADF，不 fput；alloc_fd 找不到空槽、扫描越过 256 项或 fd_install 覆盖非空槽时不给净值。 | US-01 |
| L1-10 | md | §5.1 图（484 行） | “fdput_pos（多一次 fput）” | 部分接受 | 图注补：（下标在 [0,256)、槽内是可用的非空 file 且 vfs_writev 返回时；空槽在 if (f.file) 处返回 -EBADF） | US-01 |
| L1-11 | md | §5.2 图（493 行） | “fdget 不取引用 + fdput 必 fput” | 接受 | write/writev，以及 fcntl 的 F_GETFL、default、FMODE_PATH 白名单拒绝、from ≥ RLIMIT_NOFILE 而失败的 F_DUPFD/F_DUPFD_CLOEXEC（下标在 [0,256)、槽内是可用的非空 file 且到达清理）─▶ fdget 不取引用，到达 fdput 时 fput ─▶ f_count 为 1 时首次后释放；成功的 F_DUPFD、F_DUPFD_CLOEXEC 由 get_file 抵消；alloc_fd 找不到空槽、扫描越过 256 项或 fd_install 覆盖非空槽时不给净值；空槽 -EBADF，不 fput ……… US-01 | US-01 |
| L1-12 | md | §7 表 07 UB-01 行（512 行） | “write 与 writev 返回前都多做一次 fput” | 部分接受 | 下标在 [0,256)、槽内是可用的非空 file 且 vfs_write、vfs_writev 返回时，write 与 writev 都多做一次 fput。 | UB-01、US-01 |
| L1-13 | md | §9（529 行） | “write、writev 与除成功 F_DUPFD 外的 fcntl 每次多 fput” | 接受 | US-01 fdget 系列不取引用却带 FDPUT_FPUT：下标在 [0,256)、槽内是可用的非空 file 且到达清理时，write、writev，以及 fcntl 的 F_GETFL、default、FMODE_PATH 白名单拒绝、from ≥ RLIMIT_NOFILE 而失败的 F_DUPFD/F_DUPFD_CLOEXEC，都多 fput 一次；成功的 F_DUPFD、F_DUPFD_CLOEXEC 净值为零；alloc_fd 找不到空槽、扫描越过 256 项或 fd_install 覆盖非空槽时不给净值；空槽不 fput（条件见本文 §2.3）。 | US-01 |
| L1-14 | deferred | US-01.title（31 行） | “write、writev 每次调用，以及除成功的 F_DUPFD 之外的每次 fcntl，都多做一次 fput” | 接受 | fdget/fdget_pos/fdget_raw 不取引用却带 FDPUT_FPUT：下标在 [0,256)、槽内是可用的非空 file 且控制到达清理时，write、writev，以及 fcntl 的 F_GETFL、default、FMODE_PATH 白名单拒绝、from ≥ RLIMIT_NOFILE 而失败的 F_DUPFD/F_DUPFD_CLOEXEC，都多做一次 fput；alloc_fd 找不到空槽、扫描越过 256 项或 fd_install 覆盖非空槽时不给净值 | US-01 |
| L1-15 | deferred | US-01.claim（37 行） | “并在返回前无条件 fdput_pos”；“sys_fcntl 用 fdget_raw 并在返回前 fdput”；“F_GETFL、失败的 F_DUPFD 与不支持的命令（default 分支，AS3-39）则只剩这次 fput” | 部分接受（复核新增：“无条件”与源码不符） | ksys_write 与 do_writev 只在 file 非空的分支里、vfs_write 或 vfs_writev 返回后执行 fdput_pos，空槽直接返回 -EBADF；sys_fcntl 在 file 非空时经 out1 执行 fdput，空槽 goto out 跳过 fdput。成功的 F_DUPFD、F_DUPFD_CLOEXEC（在 [from,256) 找到空槽）由 get_file 抵消；F_GETFL、default、FMODE_PATH 白名单拒绝、from 越限而失败的 F_DUPFD、F_DUPFD_CLOEXEC 只剩这次 fput；alloc_fd 找不到空槽、扫描越过 256 项或 fd_install 覆盖非空槽时不给净值。claim 其余部分不变。 | US-01 |
| L1-16 | deferred | US-01.premises（38 行） | “fcntl 的情形限于成功的 F_DUPFD/F_DUPFD_CLOEXEC 以外的命令” | 部分接受 | 追加前提：fd 下标在 [0,256)；槽内是可用的非空 file（不是 NULL，也不是已释放或未写入的指针）；控制到达 fdput 或 fdput_pos；“成功的 F_DUPFD、F_DUPFD_CLOEXEC”指在 [from,256) 找到空槽。 | US-01 |
| L1-17 | deferred | US-01.impact（39 行） | “或第一次除成功的 F_DUPFD 之外的 fcntl(2) 后，file 对象被释放”；“旧 fd 上的下一次 write、writev 或 fcntl 会把新 file 的 f_count 减一，若它原为 1 就释放它” | 接受 | 前一句改为：“……或第一次净减一的 fcntl(2)（成功的 F_DUPFD、F_DUPFD_CLOEXEC 不在其内）后……；之后该槽悬空”。后一句对悬空或复用的槽给出了确定净值，而且 fcntl 在这种槽上也可能走成功的 F_DUPFD、F_DUPFD_CLOEXEC，改按附注 1-5：不给确定净值。 | US-01 |
| L1-18 | deferred | US-01.counter_evidence 第 2 项（40 行） | “成功的 F_DUPFD 先 get_file 再 fdput，净值不变（AS3-37）” | 部分接受 | 成功的 F_DUPFD 与 F_DUPFD_CLOEXEC（在 [from,256) 找到空槽）都先 get_file 再 fdput，净值不变，但此后两槽共享的 file 的 f_count 仍为 1。另：空槽时 write、writev、fcntl 都返回 -EBADF，不执行 fdput。 | US-01 |
| L1-19 | facts | user.initramfs.shell_console_io.failure_path（151 行） | “内核侧每次都在 vfs_write 返回 -EINVAL，并在返回前对该 file 做一次不配对的 fput” | 部分接受 | 若 libc 的 write() 发出 write(2)，并且 console_fd 的下标在 [0,256)、槽内是可用的非空 file，内核侧在 vfs_write 返回 -EINVAL 后对该 file 做一次不配对的 fput（US-01，CORE-USER-BOUNDARIES-07::UB-01）；f_count 为 1 时首次之后槽悬空，后续调用的返回值与计数不再这样推；是否如此取决于 GAP-U10。 | user.initramfs.shell_console_io、US-01、UB-01 |
| L1-20 | facts | fs.vfs.rw_syscalls.failure_path（421 行） | “write 与 writev 每次调用都以一次不配对的 fput 结束” | 接受 | 下标在 [0,256)、槽内是可用的非空 file 且 vfs_write、vfs_writev 返回时，write 与 writev 以一次不配对的 fput 结束；空槽返回 -EBADF，不 fput；计数归零后槽悬空，不给净值。 | fs.vfs.rw_syscalls、US-01 |
| L1-21 | facts | fs.vfs.fd_lookup_ref.failure_path（479 行） | “每对 fdget\*/fdput\* 都会去掉一个从未取得的引用” | 部分接受（对实际执行到 fdput 的一对成立） | 补：只对实际执行到 fdput 的一对成立。三处活动调用者在 file 为 NULL 时都不调用 fdput、fdput_pos，而 fdput 本身不判空。 | fs.vfs.fd_lookup_ref、US-01 |
| L1-22 | facts | fs.vfs.fd_dup_fcntl.normal_path（536–537 行） | “成功的 F_DUPFD 中 get_file 与 fdput 相抵” | 接受 | 同 L1-04；同段的“返回前 fdput”读作“file 非空时经 out1 执行 fdput”。 | fs.vfs.fd_dup_fcntl、US-01 |
| L1-23 | facts | fs.vfs.fd_dup_fcntl.failure_path（539 行） | “除成功的 F_DUPFD 外，fcntl 也以一次不配对的 fput 结束（US-01）” | 接受 | 下标在 [0,256)、槽内是可用的非空 file 且到达 fdput 时，F_GETFL、default、FMODE_PATH 白名单拒绝（推断走不到）与 from ≥ RLIMIT_NOFILE 而失败的 F_DUPFD、F_DUPFD_CLOEXEC 以一次不配对的 fput 结束；成功的 F_DUPFD、F_DUPFD_CLOEXEC 由 get_file 抵消；空槽不 fput；alloc_fd 找不到空槽、扫描越过 256 项或 fd_install 覆盖非空槽时不给净值。 | fs.vfs.fd_dup_fcntl、US-01 |
| L1-24 | facts | FL-04.refcount_effect（974 行） | “每次调用 f_count 减一而不先加一” | 接受 | 同 L1-03。 | FL-04 |
| L1-25 | facts | FL-05.code_path 与 refcount_effect（992、994 行） | “sys_fcntl → fdget_raw → do_fcntl(F_DUPFD) → f_dupfd → fdput”；“F_GETFL、失败的 F_DUPFD 与不支持的命令只有 fdput 的一次减一” | 接受 | code_path 补：F_DUPFD_CLOEXEC 同样走 f_dupfd；空槽 goto out 跳过 fdput；FMODE_PATH 白名单拒绝时 goto out1。refcount_effect 同 L1-04。 | FL-05 |
| L1-26 | facts | AS3-21.role、AS3-23.role（1758、1780 行） | “vfs_write 返回后（包括 -EINVAL）无条件 fdput_pos”；“vfs_writev 返回后无条件 fdput_pos” | 只补注 | “无条件”只指 vfs_write、vfs_writev 返回之后；整段位于 if (filp != NULL) 或 if (f.file) 块内。quote 与行号不变。 | AS3-21、AS3-23 |
| L1-27 | evidence | §5 第 5 项第 1 点（90 行） | “标题、前提、影响与正文都已收窄” | 部分接受 | 当时的收窄只在 US-01 的 claim 与 premises 中写到 F_DUPFD_CLOEXEC；标题、影响、counter_evidence、正文摘要、FL-05 与能力摘要仍只写 F_DUPFD，而且都没有写空槽、越界与未到达清理的条件。以本文 §2.3 为准。 | US-01 |
| L1-28 | deferred | evidence_appends 中 UB-01 的 added_facts 第 1 项（143 行） | “write(2) 在 vfs_write 返回 -EINVAL 之后仍经 fdput_pos 对该 file 做一次不配对的 fput（见 US-01）” | 不受影响 | 这句以到达 vfs_write 为前提，已经隐含槽内是非空 file，无需附注。 | UB-01 |
| L1-29 | facts | AS3-26.role（1813 行） | “role: fcntl 返回前 fdput” | 只补注（C2） | 该 fdput 只在 file 非空的路径上执行：sys_fcntl 第 146–147 行空槽 goto out，跳过第 158–159 行。quote 与行号不变。 | AS3-26 |
| L1-30 | facts | SS-04.purpose（2446 行） | “purpose: write 返回前 fdput_pos” | 只补注（C2） | 正则计数只证明函数体内有一处调用；该调用在 file 非空的分支内（ksys_write 第 325、334 行）。 | SS-04 |
| L1-31 | facts | SS-06.purpose（2460 行） | “purpose: writev 返回前 fdput_pos” | 只补注（C2） | 同 L1-30（do_writev 第 391、400 行）。 | SS-06 |
| L1-32 | facts | SS-10.purpose（2488 行） | “purpose: fcntl 返回前 fdput” | 只补注（C2） | 同 L1-30（sys_fcntl 第 146–147、158–159 行）。 | SS-10 |
| L1-33 | facts | FL-04.code_path（972 行） | “都经 fdget_pos … fdput_pos” | 只补注（C2 次要） | 补：空槽时不经 fdput_pos。 | FL-04 |
| L1-34 | md | §4.2 FL-04 行“代码路径”列（338 行） | “`ksys_write`、`do_writev` → `fdget_pos` … `fdput_pos`” | 只补注（C2 次要） | 补：空槽时不经 fdput_pos。 | FL-04 |
| L1-35 | md | §4.2 FL-05 行“代码路径”列（339 行） | “fcntl：`fdget_raw` → `do_fcntl` → `f_dupfd` → `fdput`” | 只补注（C2 次要） | 补：空槽时 goto out，不经 fdput；FMODE_PATH 白名单拒绝时 goto out1。 | FL-05 |

合计 35 处：需要替换措辞 26 处（接受 14、部分接受 12），只补注 8 处（L1-26、L1-29 至 L1-35），不受影响 1 处（L1-28）。D00 草稿列了 28 处；C2 补了 L1-29 至 L1-32 四处主要项与 L1-33 至 L1-35 三处次要项，并把 L1-17 扩到 US-01.impact 的后一句。末列只列该位置引用或对应的条目，不表示该条目需要附注；各 ID 的归类以 §2.6 与 facts-and-boundaries.yaml 的 sr08_dispositions[SR08-01].affected_ids 为准。另：08 §7 表 05 UV-10 行（517 行）的“f_dupfd 的调用者是 fcntl(F_DUPFD)”不是引用计数的措辞，不计入上面 35 处；读法同 L1-01 的末句：F_DUPFD 与 F_DUPFD_CLOEXEC 都经 f_dupfd（AS3-27），两槽共享 file 的前提照样可达。

### 2.6 受影响的结构化 ID

- 需要附注：CORE-USER-SYSCALL-STDIO-08::US-01（title、claim、premises、impact、counter_evidence）；CORE-USER-SYSCALL-STDIO-08::FL-04（refcount_effect；其 code_path 只补注，见 L1-33）；CORE-USER-SYSCALL-STDIO-08::FL-05（code_path、refcount_effect）；CORE-USER-SYSCALL-STDIO-08::fs.vfs.rw_syscalls（failure_path）；CORE-USER-SYSCALL-STDIO-08::fs.vfs.fd_dup_fcntl（normal_path、failure_path）；CORE-USER-SYSCALL-STDIO-08::fs.vfs.fd_lookup_ref（failure_path）；CORE-USER-SYSCALL-STDIO-08::user.initramfs.shell_console_io（failure_path）。
- 只补注：CORE-USER-SYSCALL-STDIO-08::AS3-21、CORE-USER-SYSCALL-STDIO-08::AS3-23、CORE-USER-SYSCALL-STDIO-08::AS3-26（role）；CORE-USER-SYSCALL-STDIO-08::SS-04、CORE-USER-SYSCALL-STDIO-08::SS-06、CORE-USER-SYSCALL-STDIO-08::SS-10（purpose）。
- 相关但保持原状：CORE-USER-SYSCALL-STDIO-08::US-02、CORE-USER-SYSCALL-STDIO-08::US-03、CORE-USER-SYSCALL-STDIO-08::PU-01；CORE-USER-VFS-BASELINE-05::UV-09、CORE-USER-VFS-BASELINE-05::UV-10、CORE-USER-VFS-BASELINE-05::UV-11；CORE-USER-BOUNDARIES-07::UB-01（含 08 对它的证据追加）。
- 勘误：CORE-USER-RETURN-SIGNAL-09::ER-01。

## 3. SR08-02：分派局部 ax 与最终返回值

### 3.1 回传要求与结论

主线要求把“分派写进 regs->ax 的值”与“返回用户态时 rax 里的值”分开。

**结论：接受。** 08 写的“用户看到/拿回的 rax”只是分派后 regs->ax 的局部结果。分派之后，返回尾部无条件运行重启与信号逻辑，可能改写 ax、ip、sp 等；返回段装入 rax 的是改写之后帧中的 ax。“rax 等于分派值”的四条前提（§3.3）合起来是充分条件，不是必要条件：其中第 1、3、4 条必要，第 2 条“没有信号递送”可以放宽为“没有成功建立信号帧”。越界号的“ax 不被写”也只是分派局部事实，源码中有反例（§3.4）。

本节对应本包的 ED-01（08 分派 → entry.syscall_return_work）、ED-02、ED-03、ED-06、ED-17、ED-18、ED-19。

### 3.2 四段区分（源码层面）

| 段 | 帧中相关字段 | 依据 | 说明 |
|---|---|---|---|
| ① 分派前 | orig_ax = ax = 入口的完整 64 位 rax；ip = rcx；flags = r11；sp = 入口 rsp（经 TSS sp2 暂存）；cs、ss 为 __USER_CS、__USER_DS | 08 AS2-02；07 SB-01、AB2-13、VT-PTREGS | -ENOSYS 预置被注释（08 AS2-03、QS-18） |
| ② 分派后 | unr < 453 且处理函数返回：ax = 返回值。unr ≥ 453：ax 不写，仍是入口 rax。表内缺项：while (1)，没有 ③④ | 08 AS2-04、AS2-05、AS2-06、SS-15；07 AB2-14 | 号 = movslq(eax) → int nr → uint unr。这是“分派局部结果” |
| ③ 返回尾部之后 | 见下表 | 07 AB2-12、SB-14、SB-15；本节引文 | 每次返回都调用 get_signal（RS-04） |
| ④ 返回段装入 | rax ← 帧 ax；rcx ← 帧 ip；r11 ← 帧 flags；rsp ← 帧 sp；cs、ss 槽不读 | 07 SB-02、AB2-16、VT-PTREGS、FP-3 | ip、flags 经 entry_64.S 第 187–192 行的 xchg 落到 RCX、R11 槽，再由 POP_REGS 的 popq %rcx、popq %r11（第 77、72 行）装入 |

第③段的情形：

| 情形 | 条件 | 帧的改写 |
|---|---|---|
| 无信号，重启 | get_signal 为假；(int)orig_ax ≠ -1；ax 为 -514、-512 或 -513 | ax = orig_ax；ip −= 2 |
| 无信号，重启块 | 同上，ax 为 -516 | ax = 219；ip −= 2 |
| 无信号，其余 | 其余情形 | 不改 |
| 有信号，重启表 | get_signal 为真；(int)orig_ax ≠ -1 | -516、-514 → ax = -EINTR；-512 且无 SA_RESTART → ax = -EINTR；-513，或 -512 且有 SA_RESTART → ax = orig_ax、ip −= 2；其余不改 |
| 有信号，帧建立失败 | 没有 SA_RESTORER；或 SA_SIGINFO 时 copy_siginfo_to_user 失败 | 只剩上一行的改写；信号已出队而丢失（RS-02）。若 RS-01 成立，经 rt_sigaction 注册的处理函数都落在这一行 |
| 有信号，帧建立成功 | 上述两关都通过 | 先把当时的寄存器（含 ax、ip）存进用户信号帧；再令 di = 信号号、ax = 0、si 与 dx = 帧内地址、ip = 处理函数、sp = 信号帧、cs = __USER_CS；ss ≠ __USER_DS 时 force_valid_ss；清 flags 的 DF、RF、TF |
| 本次是 rt_sigreturn | 分派到 15 号，restore_sigcontext 成功 | ax、ip、sp、cx、r11 等取自用户帧，cs、ss 或上 3，flags 只取 FIX_EFLAGS 所列的位，orig_ax = -1；分派把返回的 regs->ax 写回 ax；两处重启 switch 都不进入 |

**① 与 ②。** 08 已经确立：入口把同一个 rax 压为 orig_ax 与 ax（AS2-02），-ENOSYS 预置被注释（AS2-03），rsi 是 movslq 之后的 eax（AS2-04），unr < 453 才写 ax（AS2-05、SS-15）。补一点：do_syscall_64 以 int nr 接收号 QD-10，再 uint unr = nr。所以号检查只看 eax；rax 的高 32 位不参与（推断：高 32 位非零、低 32 位在表内时，按低 32 位分派）。ax 与 orig_ax 都是 64 位的 reg_t（07 VT-PTREGS）。

**③ 返回尾部。** do_syscall_64 分派后无条件调用 syscall_exit_to_user_mode（07 QB-43），一路到 exit_to_user_mode_loop，后者无条件调用 arch_do_signal_or_restart（07 AB2-12、SB-14、SB-15；本包 entry.syscall_return_work）。get_signal 不带 regs 参数，它所在的 signal.c 里根本不出现 regs QD-11。所以从分派写 ax 到返回段之间，帧只会被 arch_do_signal_or_restart 这一支改写。

有信号递送（get_signal 为真）时，调用 handle_signal 后直接返回 AD0-08。只有默认动作或忽略动作的信号时，get_signal 同样返回假（RS-03）。

没有信号递送时，只在 (int)orig_ax ≠ -1 时按 syscall_get_error 进入重启 switch AD0-09。syscall_get_nr 返回 orig_ax 的 int 值 AD0-01。syscall_get_error 取 regs->ax AD0-02，只在 IS_ERR_VALUE 为真时返回它，否则返回 0 AD0-03；IS_ERR_VALUE 判的是 64 位无符号值不小于 (size_t)-4095 AD0-18。四个码的值是 512 AD0-19、513 AD0-20、514 AD0-21 与 516 AD0-22。

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal.c::arch_do_signal_or_restart]
```c
		case -ERESTARTNOHAND:
		case -ERESTARTSYS:
		case -ERESTARTNOINTR:
			regs->ax = regs->orig_ax;
			regs->ip -= 2;
```

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal.c::arch_do_signal_or_restart]
```c
		case -ERESTART_RESTARTBLOCK:
			regs->ax = get_nr_restart_syscall(regs);
			regs->ip -= 2;
```

get_nr_restart_syscall 走 #else 分支 signal.c:234（QD-L09），返回 __NR_restart_syscall，即 219 AD0-04。CONFIG_X86_X32_ABI 与 CONFIG_IA32_EMULATION 在源码与构建文本中都没有定义 QD-12 QD-13 QD-34 QD-49。219 在活动分派表中没有表项 QD-14（RS-05）。无处理函数分支对帧的活动写入只有这四处 SD-02。

有信号递送时，handle_signal 先在 (int)orig_ax ≠ -1 时按同一个错误码改写：-516、-514 改为 -EINTR AD0-05；-512 且没有 SA_RESTART 改为 -EINTR AD0-06；-513，或 -512 且有 SA_RESTART，改为 ax = orig_ax、ip −= 2 signal.c:189（QD-L09）。然后调用 setup_rt_frame，返回值不小于 0 才进入成功分支 AD0-07。失败时没有任何处理，对失败的处理在注释里 QD-15：重启表的改写保留，信号已出队而丢失（RS-02）。

x64_setup_rt_frame 在 sa_flags 没有 SA_RESTORER 时，在写任何寄存器之前就返回：

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal_64.c::x64_setup_rt_frame]
```c
	/* x86-64 should always use SA_RESTORER. */
	if (!(ksig->ka.sa.sa_flags & SA_RESTORER))
		return -EFAULT;
```

SA_SIGINFO 时 copy_siginfo_to_user 失败，也直接返回 -EFAULT AD0-14。帧的写入不等这一关：SA_RESTORER 检查通过后，就先把当时的寄存器写入用户信号帧，其中包括 ax AD0-12 与 ip（同函数第 120 行），复制 siginfo 在其后，所以复制失败时用户栈已经写入、寄存器还没改。两关都通过才改写寄存器：

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal_64.c::x64_setup_rt_frame]
```c
	regs->di = ksig->sig;
	/* In case the signal handler was declared without prototypes */
	regs->ax = 0;
```

同一段还令 si、dx 为帧内地址，ip = 处理函数，sp = 信号帧 AD0-16；cs = __USER_CS，ss 不等于 __USER_DS 时调用 force_valid_ss AD0-17。handle_signal 的成功分支再清 flags 的 DF、RF、TF（signal.c 第 216 行）。若 RS-01 成立（needs_evidence，待 NVR-2），经 rt_sigaction 注册的处理函数都会在第 168 行失败，这一段对它们走不到。

本次系统调用是 rt_sigreturn（15 号）时，restore_sigcontext 用用户帧覆盖 ax、ip、sp 等，最后把 orig_ax 置为 -1 AD0-11；rt_sigreturn 返回 regs->ax signal_64.c:272（QD-L10），分派又把同一个值写回 ax。orig_ax 为 -1，两处重启 switch 都不进入。坏帧时返回 0（RS-07）；读用户帧的容错问题见 RS-06。

**④ 返回段。** 07 已经确立：六次 xchg 把帧中的 ip、flags 换到 RCX、R11 槽；POP_REGS 装入 15 个寄存器，其中 rax 来自 ax 槽，rcx、r11 来自 RCX、R11 槽；addq $0x28 跳过 irq_nr、orig_ax、ip、cs、flags 五格；popq %rsp 取 sp；然后 swapgs、sti、sysretq（07 SB-02、AB2-16、VT-PTREGS、FP-3）。所以 rax 取 ③ 之后帧中的 ax，rcx 取帧 ip，r11 取帧 flags，rsp 取帧 sp；cs、ss 槽不被读取。do_syscall_64 返回的 bool 不被检查（07 SB-16、UB-09）。sysretq 之后用户态实际看到什么属 NOT_RUN（GAP-U04 剩余部分，07 NVB-4，本包 NVR-3）。

### 3.3 “rax 等于分派值”的充分条件与必要前提

在 a039d9803ade 的源码层面，下列四条同时成立时，返回段装入 rax 的就是分派写入 regs->ax 的值：

1. 号在表内，处理函数返回（分派确实写了 ax）。
2. 这次返回没有信号递送：get_signal 返回假。只有默认动作或忽略动作的信号时也是如此（RS-03）。
3. 没有重启改写：(int)orig_ax 为 -1（例如 rt_sigreturn 之后），或者分派写入的 64 位值不是 -512、-513、-514、-516。
4. 返回尾部返回，执行按位置落入 syscall_return_via_sysret（07 SB-01）。

这是充分条件，不是必要条件。源码中有反例：有信号递送，但 x64_setup_rt_frame 在 SA_RESTORER 检查处返回，而分派值又不是 ERESTART 码；这时 handle_signal 的重启 switch 不动 ax，帧建立也没有改寄存器，ax 仍是分派值。一般地说，有信号递送时 ax 可能保持不变，也可能被改为 -EINTR、orig_ax 或 0。帧建立成功时，分派值（或重启表改写后的值）先存进用户信号帧；它要等处理函数经 rt_sigreturn 返回才会再回到 ax（推断，取决于用户态的 restorer，NOT_RUN）。

**必要前提与充要条件。** 以“返回尾部不写帧的任何字段、并到达返回段”为准（全文与逐项依据见 user-return-signal.md“衔接”一节 L2 末段，编号与本节相同）：第 1、3、4 条是必要的。第 1 条（越界号按下段替换）或第 4 条不成立，就没有这次返回。第 3 条不成立时，不论走哪一处重启 switch，命中的 case 都写帧：有信号递送时 ax 改为 -EINTR，或 ax = orig_ax 且 ip 减 2（SD-14）；没有信号递送时 ip 都减 2（SD-02）；建帧失败也不回滚（RS-02）。第 2 条不是必要的，可以放宽为“没有成功建立信号帧”：get_signal 为假，或建帧在改写寄存器之前失败，即 x64_setup_rt_frame 在第 169 行（缺 SA_RESTORER，AD0-13）或第 191 行（SA_SIGINFO 时复制 siginfo 失败，AD0-14）返回 -EFAULT，两处都在第 198 行起的寄存器改写之前（SD-37）；第三处 return -EFAULT（第 236 行，Efault 标号下）只能由第 178–186 行写用户帧的语句跳入，同样不经过寄存器改写，而且按 RS-08 不可达（SD-69）；失败时 handle_signal 也不清 flags，清除只在成功分支内（AD0-07、SD-39）。第 2 条这样放宽之后，四条合起来就是上述充要条件（L2 末段另记一处 RS-08 地址重叠的例外，推断）。只比 rax 的数值时，还有三种数值相等而帧已被改写的情形，不算未改写返回：建帧成功而分派值恰为 0；越界号的 64 位 rax 恰为 -512、-513、-514 且 get_signal 为假；越界号的 64 位 rax 恰为 -513，或带 SA_RESTART 时恰为 -512，get_signal 为真而建帧失败。后两种都是 ax 写回同值、ip 减 2。

越界号时，把第 1 条换成“(uint)(int)eax ≥ 453，分派不写 ax，控制到达返回尾部（第③段）”，把第 3 条中的“分派写入的 64 位值”换成入口的 64 位 rax；上面的充分、必要与充要说法照样成立，返回段装入的就是入口 rax。

08 L2-01 的“号不小于 453 时用户拿回的就是原号”、L2-03 的“用户态在 rax 中看到返回值”一类说法越过了 NOT_RUN 边界，统一改为“返回段装入 rax 的值”。

### 3.4 源码反例：越界号的 rax 恰为 ERESTART 码

(uint)(int)eax ≥ 453 时，帧 ax 保留入口的完整 64 位 rax。它若恰为 -512、-513、-514 或 -516 的 64 位补码（高 32 位全 1），(int)orig_ax 也不等于 -1，于是没有信号递送时进入重启 switch：

- **-516**：帧 ax 被改为 219，ip 减 2。这是源码层面的事实。按 ISA 理解，ip 减 2 回到同一条 syscall 指令（推断，NOT_RUN，NVR-3）；以 219 再次进入时，219 小于 453 却不在表中，会落到 __x64_sys_ni_syscall 的 while (1)（08 AS2-06、RS-05）。
- **-512、-513、-514**：帧 ax 写回同一个值，只有 ip 减 2。“回到同一条 syscall、在内核与用户之间循环”是按 ISA 作的推断（NOT_RUN），还要求每次重入时 get_signal 仍为假。
- **有信号递送时**：-516、-514 与不带 SA_RESTART 的 -512 改为 -EINTR；-513 与带 SA_RESTART 的 -512 改为 orig_ax，并令 ip 减 2。

这正是 SR08-02 所说“越界号 ax 不被写只是分派局部事实”的源码反例。它的正式登记是本包对 CORE-USER-BOUNDARIES-07::UB-09 的证据追加；08 的同义表述由 ER-02 更正，07 的由 ER-03 更正。init 与 sh 是否会发出这样的号，取决于仓库外的 libc（GAP-U10、NVR-4）。

### 3.5 取代 08 §3.2 的表

| 号的情况（按 (uint)(int)eax 计） | 分派后 regs->ax（局部结果） | 到达返回段的前提与可能的改写 |
|---|---|---|
| 在表中（41 项之一） | 处理函数返回时为它的返回值。没有活动 return 的处理函数（dup、dup2、wait4；lseek 经 blkdev_llseek 或 dcache_dir_lseek 时）若返回，值未定义；exit 一类沿用 08 的“若返回”写法 | §3.3 四条同时成立时，返回段装入 rax 的就是这个值（充分条件）。没有信号递送且 (int)orig_ax ≠ -1 时：返回值为 -512、-513、-514 → ax = orig_ax（入口 rax）、ip −= 2；为 -516 → ax = 219、ip −= 2。15 号 rt_sigreturn 成功恢复时 orig_ax 已置为 -1，恢复出的值即使是这四个之一，两处重启 switch 也都不进入（§3.2 第③段末行）。活动代码中没有以这四个名字返回的处理函数（§5）；值未定义的那几个，静态阅读不能排除恰为这四个值（推断）。有信号递送时按 §3.2 第③段改写 |
| 小于 453 但不在表中 | 不返回：__x64_sys_ni_syscall 打印 orig_ax 后 while (1)（AS2-06） | 没有返回尾部，也没有返回段装入的值 |
| 452（__NR_syscalls − 1） | -ENOSYS（AS2-13） | -ENOSYS 不是重启码，没有信号递送时保留。有信号递送时：帧建立成功则 ax = 0，-ENOSYS 存进信号帧；帧建立失败则仍是 -ENOSYS |
| 不小于 453（含低 32 位按 int 为负） | 不写。帧 ax 仍是入口的完整 64 位 rax；它不一定等于 (int)eax 意义上的号，只有高 32 位不是低 32 位的符号扩展时才不同 | 没有信号递送时：64 位 rax 恰为 -514、-512、-513 → ax = orig_ax（同值）且 ip −= 2；恰为 -516 → ax = 219（不在表中，RS-05）且 ip −= 2；低 32 位为 0xFFFFFFFF 时 (int)orig_ax 为 -1，两处重启 switch 都跳过；其余值保留。有信号递送时：可能改为 -EINTR、orig_ax（ip −= 2）或 0，并改 ip、sp 等（§3.2 第③段） |

### 3.6 附注全文（供读 08 时对照；只登记在本包，08 原文件不改）

**附注 2-1（读法）。** 08 中凡写“用户看到的 rax”“用户拿回”“用户态 rax = 返回值”之处，都读作“分派后 regs->ax（分派局部结果）”。

**附注 2-2（号检查）。** 入口用 movslq 把 eax 符号扩展进 rsi，do_syscall_64 以 int nr 接收，再 uint unr = nr，与 __NR_syscalls（453）比较。号检查只覆盖这样得到的值，不能推广到任意 64 位 rax；rax 的高 32 位不参与（推断）。

**附注 2-3（四段）。** ① 分派前，帧中 orig_ax 与 ax 都是入口的完整 64 位 rax。② 分派只在 unr < 453 且处理函数返回时写 ax；unr ≥ 453 时不写。③ 返回尾部无条件调用 arch_do_signal_or_restart：没有信号递送且 (int)orig_ax ≠ -1 时，ax 为 -512、-513、-514 改为 orig_ax 并令 ip 减 2，为 -516 改为 219 并令 ip 减 2；有信号递送时，handle_signal 先按同一张重启表改写（-EINTR，或 orig_ax 与 ip 减 2），帧建立成功再改 di、ax（为 0）、si、dx、ip、sp、cs，必要时 ss，并清 flags 的 DF、RF、TF，帧建立失败则只保留重启表的改写；本次是 rt_sigreturn 且 restore_sigcontext 成功时，② 分派阶段已由用户信号帧恢复 15 个通用寄存器与 sp、ip（cs、ss 取用户值或上 3，flags 只取 FIX_EFLAGS 所列的位）并把 orig_ax 置为 -1，③ 的两处重启 switch 都不进入。④ 返回段把这之后帧中的 ax、ip、flags、sp 装入 rax、rcx、r11、rsp，然后 swapgs、sti、sysretq。

**附注 2-4（充分条件）。** 没有信号递送（get_signal 为假），没有重启改写（(int)orig_ax 为 -1，或分派写入的 64 位值不是 -512、-513、-514、-516），并且正常到达 syscall_return_via_sysret 时，返回段装入 rax 的就是分派写入的值。这是充分条件，不是必要条件：有信号递送时，ax 可能保持不变，也可能被改为 -EINTR、orig_ax 或 0；哪几条是必要前提见本文 §3.3。sysretq 之后用户态实际看到什么属 NOT_RUN。

**附注 2-5（越界号与边界）。** 越界号“ax 不被写”只是分派局部事实：64 位 rax 恰为 -512、-513、-514、-516 时，没有信号也会由重启分支改写帧：四种都令 ip 减 2，-516 另把 ax 改为 219，-512、-513、-514 的 ax 写回同值（§3.4）。处理函数不返回时（例如表内缺项的 while (1)）没有返回尾部。信号链内部见本包 RS-01 至 RS-07 与返回尾部、信号、重启各节；GAP-U04、GAP-U10 不因本附注关闭。

### 3.7 受影响位置（08 中 20 处，另有 07 一处）

| # | 文件 | 位置 | 现文（短引） | 结论 | 追加的附注 | 涉及的结构化 ID |
|---|---|---|---|---|---|---|
| L2-01 | md | §0 第 2 条（32 行） | “`do_syscall_64` 把号转为无符号”；“号不小于 453 时用户拿回的就是原号” | 接受 | 按附注 2-1 至 2-5 改读：处理函数返回时，返回值写进 regs->ax（分派局部结果）；随后 syscall_exit_to_user_mode 无条件调用 arch_do_signal_or_restart，可能改写 ax、ip、sp 等，再落入 syscall_return_via_sysret。号检查只看 eax（movslq 后以 int 接收，再转 uint）。转换后不小于 453 时，分派不写 ax，帧中 ax 仍是入口的完整 64 位 rax；没有信号递送、没有重启改写、并正常到达返回段时，返回段装入 rax 的就是这个值（充分条件，§3.3）；它恰为 -512、-513、-514、-516 时，没有信号也会由重启分支改写帧（四种都令 ip 减 2，-516 另把 ax 改为 219，§3.4）。 | entry.syscall_dispatch、UB-09 |
| L2-02 | md | §3.1“检查与分派”（177 行） | “号转成无符号后与 `__NR_syscalls`（453，只在 `__KERNEL__` 下定义，AS2-12、AS2-20）比较” | 部分接受（措辞） | 入口用 movslq 把 eax 符号扩展进 rsi，do_syscall_64 以 int nr 接收，再转成 uint 与 __NR_syscalls（453）比较。rax 的高 32 位不参与检查（推断：高 32 位非零、低 32 位在表内时，按低 32 位的号分派）。 | entry.syscall_dispatch、AS2-04、AS2-05 |
| L2-03 | md | §3.1“返回”（219 行） | “返回段把帧中的 ax 恢复到 rax，用户态在 rax 中看到返回值” | 接受 | x64_sys_call 写入 regs->ax 的是分派局部结果（SS-15 只证明 do_syscall_64 内写一次）。随后 syscall_exit_to_user_mode → exit_to_user_mode_loop 无条件调用 arch_do_signal_or_restart(regs)，可按 §3.2 第③段改写 ax、ip、sp 等；返回段 POP_REGS 装入 rax 的是这一步之后帧中的 ax。没有信号递送、没有重启改写，并且正常到达 syscall_return_via_sysret 时，返回段装入 rax 的就是分派写入的值；这是充分条件，不是必要条件。sysretq 之后用户态实际看到什么属 NOT_RUN（GAP-U04 剩余部分）。本段所引的 07 FP-3 本来就写有“syscall_exit_to_user_mode 可能改帧”。 | entry.syscall_dispatch、ES-12 |
| L2-04 | md | §3.2 表头第 3 列（223 行） | “用户看到的 rax” | 接受 | 改为“分派后 regs->ax（局部结果）”，另加一列“到达返回段的前提与可能的改写”；整表以本文 §3.5 取代。 | entry.syscall_dispatch |
| L2-05 | md | §3.2 第 1 行（225 行） | “处理函数的返回值；没有活动 return 的处理函数（dup、dup2、wait4；exit 在 do_exit 之后也没有 return）若返回，返回值没有定义” | 部分接受（分派局部的说法正确） | 见本文 §3.5 第 1 行。 | entry.syscall_dispatch、VT-SYSTAB |
| L2-06 | md | §3.2 第 2 行（226 行） | “不返回（UB-09）” | 接受（分派阶段的事实） | 分派阶段就不返回（while (1)），没有返回尾部，也没有返回段装入的值。 | UB-09 |
| L2-07 | md | §3.2 第 3 行第 3 列（227 行） | “-ENOSYS” | 部分接受 | 见本文 §3.5 第 3 行。 | entry.myos_special_syscalls、entry.syscall_dispatch |
| L2-08 | md | §3.2 第 4 行（228 行） | “原 rax，也就是号本身（UB-09）” | 接受（附源码反例） | 见本文 §3.5 第 4 行与 §3.4。 | entry.syscall_dispatch、UB-09 |
| L2-09 | md | §4.6 表头第 4 列（455 行） | “结果（源码层面）” | 部分接受（措辞） | 表注：本列的返回值是分派局部结果，返回段装入 rax 之前还要经过本文 §3.2 第③段。 | — |
| L2-10 | md | §5.1 图（481 行） | “用户态 rax = 返回值” | 接受 | └─▶ syscall_exit_to_user_mode ─▶ exit_to_user_mode_loop ─▶ arch_do_signal_or_restart（无条件；可能改 ax、ip、sp 等）─▶ syscall_return_via_sysret（07 FP-3）─▶ rax = 返回尾部之后帧中的 ax（前提见本文 §3.3） | entry.syscall_dispatch、ES-12 |
| L2-11 | md | §5.2 图（498 行） | “号 ≥ 453 ─▶ 返回原号” | 接受 | (uint)(int)eax ≥ 453 ─▶ 分派不写 ax（帧 ax 仍是入口 rax）；返回尾部仍可能改写 ……… UB-09、ER-02 | UB-09 |
| L2-12 | md | §8 GAP-U10 行（525 行） | “内核侧对每个候选系统调用的结果已列出” | 部分接受（措辞） | 补“（指分派局部结果与源码层面的内核侧处理）”；状态 open 不变。 | GAP-U10 |
| L2-13 | md | §10“未覆盖”（534 行） | “信号与 `syscall_exit_to_user_mode` 内部” | 不受影响 | 与本附注一致：§3.2 中的最终值说法越过了这条覆盖边界。 | — |
| L2-14 | facts | entry.syscall_dispatch.normal_path（226–227 行） | “do_syscall_64 把号转为无符号，小于 __NR_syscalls（453）才 regs->ax = x64_sys_call(regs, unr)”；“之后 syscall_exit_to_user_mode，并落入 syscall_return_via_sysret” | 部分接受 | 补：号指 movslq 后以 int 接收的 eax；syscall_exit_to_user_mode 无条件调用 arch_do_signal_or_restart，可能改帧（见本包 entry.syscall_return_work）。 | entry.syscall_dispatch |
| L2-15 | facts | entry.syscall_dispatch.failure_path（229 行） | “号不小于 453（包括转成无符号后的负号）时 ax 不被写，用户拿回原 rax，也就是系统调用号” | 接受 | (uint)(int)eax 不小于 453 时，分派不写 ax，帧 ax 仍是入口的完整 64 位 rax；返回段最终装入什么取决于返回尾部（本文 §3.5 第 4 行）。 | entry.syscall_dispatch、UB-09 |
| L2-16 | facts | entry.syscall_dispatch.correctness_evidence.reason（270 行） | “越界号返回原值、缺项原地循环（UB-09）” | 部分接受 | 越界号分派不写 ax（最终值取决于返回尾部）、缺项原地循环（UB-09）。 | entry.syscall_dispatch |
| L2-17 | facts | AS2-05.role（1358 行） | “号转成无符号后与 __NR_syscalls 比较，范围内才调 x64_sys_call 并把返回值写入 regs->ax” | 部分接受（措辞） | 以 int 接收的 eax 转成 uint 后与 __NR_syscalls 比较，范围内才调 x64_sys_call 并把返回值写入 regs->ax（分派局部写入）。quote 与行号不变。 | AS2-05 |
| L2-18 | facts | ES-12.note（819 行） | “写回 regs->ax 后经 syscall_exit_to_user_mode，再落入 syscall_return_via_sysret” | 不受影响 | 这句没有断言最终值，无需附注。 | ES-12 |
| L2-19 | facts | PU-02.now（1073 行） | “内核侧对每个候选系统调用的结果已列出（VT-SYSTAB 与 user-syscall-stdio.md §4）” | 部分接受（措辞） | 补“（指分派局部结果；返回段装入前的改写见本文 §3.2）”；status_to 仍为 open。 | PU-02 |
| L2-20 | facts | gap_status 中 GAP-U10 的 coverage（1102 行） | “内核侧每个可能被调用的系统调用的结果已逐项列出” | 部分接受（措辞） | 同 L2-19；GAP-U10 保持 open。 | GAP-U10 |
| L2-21 | 07 deferred（08 包外） | UB-09 的 title、claim、impact（212、218、220 行） | “越界号返回原值”；“号不小于 453 时 regs->ax 不被写，返回的是原 rax（即系统调用号）” | 关联（同一问题） | 不在 08 附注范围，由本包 ER-03 处理（§4）。 | CORE-USER-BOUNDARIES-07::UB-09 |

合计：08 中 20 处，需要附注 18 处（接受 8、部分接受 10），不受影响 2 处（L2-13、L2-18）；另有 07 一处（L2-21），由 ER-03 处理。D00 草稿把总数写成 21 处、把 07 一处算进 08，已按 C6 更正。末列只列该位置引用或对应的条目，不表示该条目需要附注；各 ID 的归类以 §3.8 与 facts-and-boundaries.yaml 的 sr08_dispositions[SR08-02].affected_ids 为准。

### 3.8 受影响的结构化 ID

- 需要附注：CORE-USER-SYSCALL-STDIO-08::entry.syscall_dispatch（normal_path、failure_path、correctness_evidence.reason）。
- 只补措辞：CORE-USER-SYSCALL-STDIO-08::AS2-05（role）；CORE-USER-SYSCALL-STDIO-08::PU-02（now）；CORE-USER-SYSCALL-STDIO-08::GAP-U10（08 gap_status 中的记录）的 coverage（状态 open 不变）。
- 不受影响：CORE-USER-SYSCALL-STDIO-08::ES-12。
- 相关但保持原状：CORE-USER-SYSCALL-STDIO-08::AS2-04（role 原样成立）、CORE-USER-SYSCALL-STDIO-08::VT-SYSTAB、CORE-USER-SYSCALL-STDIO-08::entry.myos_special_syscalls（452 号返回 -ENOSYS、缺项 while (1) 都是分派阶段结论，见 ER-02.unchanged_parts）；CORE-USER-BOUNDARIES-07::FP-3（本来已写“可能改帧”）、CORE-USER-BOUNDARIES-07::SB-14、CORE-USER-BOUNDARIES-07::SB-15、CORE-USER-BOUNDARIES-07::SB-16、CORE-USER-BOUNDARIES-07::AB2-12、CORE-USER-BOUNDARIES-07::AB2-14；CORE-USER-VFS-BASELINE-05::GAP-U04 与 CORE-USER-VFS-BASELINE-05::GAP-U10 都不关闭。
- 本包关联：CORE-USER-RETURN-SIGNAL-09::entry.syscall_return_work、CORE-USER-RETURN-SIGNAL-09::ipc.signal.delivery_dispatch、CORE-USER-RETURN-SIGNAL-09::ipc.signal.syscall_restart、CORE-USER-RETURN-SIGNAL-09::ipc.signal.sigreturn_restore；勘误 CORE-USER-RETURN-SIGNAL-09::ER-02。
- 07 对应项 CORE-USER-BOUNDARIES-07::UB-09 由 CORE-USER-RETURN-SIGNAL-09::ER-03 处理。

## 4. 07 的对应项

07 的 CORE-USER-BOUNDARIES-07::UB-09 在三处写了同一说法：title 的“越界号返回原值”、claim 的“号不小于 453 时 regs->ax 不被写，返回的是原 rax（即系统调用号）”、impact 的“号不小于 453 时不进循环，用户得到原系统调用号作为返回值”（07 deferred-findings.yaml 第 212、218、220 行）。它在 08 包外，本文不对它出附注，标记为**由本包 ER-03 处理**：按 §3.4 的重启逻辑，把 -512、-513、-514、-516 四个 64 位值排除在“返回原值”之外。

- 07 entry.syscall_frame_return.failure_path 的“号不小于 __NR_syscalls 时 ax 保持原值（即系统调用号）”（07 facts-and-boundaries.yaml 第 416 行）在 do_syscall_64 这一点仍成立，读作分派局部结果；ER-03 没有把它列入 original_locations，只在其 unchanged_parts 中写明它在这一点仍成立。
- 07 FP-3 已写“syscall_exit_to_user_mode 可能改帧”，与本文一致，不需要更正。

## 5. 检索补正与剩余边界

**ERESTART 四个名字的全部出现**（按 C7 改为不带管道的单条检索）QD-16：
- 活动 case 标签：arch signal.c 第 177、178、182、188、258、259、260、265 行。
- 定义：include/linux/lib/errno.h 第 13–15、17 行；名字表：lib/errname.c 第 185–187、189 行。
- 活动的产生点只有两处：kactive/completion/completion.h:52 的 timeout = -ERESTARTSYS；lock_IPC/signal/signal.h:464 的 return -ERESTARTNOINTR，所在的 restart_syscall() 没有调用者（RS-04）。前者是否流到系统调用返回值，见本包重启一节。
- 注释中的命中：fs/syscall.c 第 64–67 行；sched/forkexec/exec.c 第 219、237、340、445 行；sched/forkexec/fork.c 第 954 行。

**仍然开放的边界。**
- sysretq、swapgs 的语义，以及“ip 减 2 回到 syscall 指令”，属 ISA 语义：GAP-U04 剩余部分（07 NVB-4），候选 NVR-3，NOT_RUN。
- RS-01（UAPI_SA_FLAGS 是否掩掉 SA_RESTORER）决定“帧建立成功”一支能否走到：needs_evidence，候选 NVR-2。
- 返回尾部、信号递送与重启的实际效果：NOT_RUN，候选 NVR-1。
- write、writev、fcntl 前后 f_count 的实际变化：NOT_RUN，沿用 08 的候选 CORE-USER-SYSCALL-STDIO-08::NVS-1，观察时按 §2.3 的条件分组。
- libc 实际发出的系统调用号、fd 与 SA_RESTORER 用法：GAP-U10，候选 NVR-4。
- slab 释放后对象的内容与重新分配：NOT_RUN。C 整数转换（unsigned int、ulong 到 int 再到 ulong，u64 到 int）按 GCC 的实现定义理解，属推断。

## 6. 复核更正的落实（C1 至 C9）

| 更正 | 内容 | 落实位置 |
|---|---|---|
| C1 | fd_install 的 BUG_ON 不会让控制停下：panic 的 __noreturn 声明被注释，停机循环被注释，panic 会返回；BUG_ON 之后第 192 行覆盖原槽，再回到 fdput | §2.2.4；§2.3 (x) 第⑤项；附注 1-5 |
| C2 | 补 AS3-26.role 与 SS-04、SS-06、SS-10 的 purpose；L1-17 扩到 US-01.impact 的后一句；次要项 FL-04.code_path 与 md 第 338、339 行“代码路径”列 | §2.5 的 L1-17、L1-29 至 L1-35；位置数 35 |
| C3 | “只有……才”改为充分条件，并给出反例；“用户才拿回”不越过 NOT_RUN | §3.3；附注 2-4；§3.7 的 L2-01、L2-03 |
| C4 | (a) 补 (int)orig_ax ≠ -1；(b) 第 4 行“号本身”的措辞；(c) 反例中 -516 与 -512、-513、-514 分开写；(d) 有信号时加 ss，并引 si、dx；(e) flags 进 r11 的路径；(f) 补 SA_SIGINFO 复制失败 | (a) §3.2、§3.3、附注 2-3；(b) §3.5 第 4 行；(c) §3.4；(d) §3.2 第③段表；(e) §3.2 表第④段；(f) §3.2 第③段表 |
| C5 | 统一写成“下标在 [0,256)、槽内是可用的非空 file、控制到达清理”；“成功的 F_DUPFD、F_DUPFD_CLOEXEC”注明在 [from,256) 找到空槽；L1-02 写成条件；L1-19 保留首尾条件 | §2.5 各行；附注 1-2、1-3 |
| C6 | SR08-02 的位置为 08 中 20 处，另有 07 一处（UB-09.title 也列入） | §0；§3.7；§4 |
| C7 | f_dupfd 有两个活动调用点；ERESTART 检索改为单条 git grep 并列出注释命中 | §2.2.3；§5 |
| C8 | alloc_fd 越界改为“可能读到”；(e1) 注明 RLIMIT_NOFILE 前提；BUG 宏补 #ifndef __ASSEMBLY__；exit 沿用 08 的“若返回”写法；净变化的口径 | §2.2.3；§2.3；§2.2.4；§3.5 第 1 行；附注 1-6 |
| C9 | FMODE_PATH 路径收窄为推断走不到 | §2.2.5；§2.3 (e4) |
