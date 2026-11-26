---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
conversation_display_name: MYOS2-A-C11 内核分析主线续建
produced_by: GPT-6 Astra Pro（本轮系统身份）
model_per_owner: Pro独立验收并同轮安排后继
actual_backend_model: UNATTESTED
actual_reasoning_effort: UNATTESTED
date: 2026-11-26
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: time（分支名）
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
read_channel: GitHub connector；自制文本/整数/符号链核验
startup_selfcheck_quote: 2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。
branch_canary_quotes:
- "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
- "\tthis_cpu = smp_processor_id();"
self_check:
  verified_claims: 0
  quotes_reconfirmed: 0
  downgraded_to_inferred: 0
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
previous_mainline_design_count:
  conversation: C10
  distinct_batches: 11
evidence_class: SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / OBSERVATION / UNKNOWN
lead_before_publication_short12: 1937b1b16805
execution_pin_short12: 2d134f987c21
self_check_scope: 本文件新增正式源码窄窗0/0/0；156作者76正式窗、26普通位置及157合同READY的独立复核分别记账。
mainline_design_count:
  conversation: C11
  before_release: 5
  after_successful_157_release: 6
  thresholds:
  - 8
  - 10
  reached: false
status: PREPARED_FOR_PUBLICATION_G04_PENDING
record_type: phase2_executor_contract
followup_id: CORE-USER-SIGNAL-CONTROL-CONSUMPTION-157
executor_display_name: MYOS2-A-C11b 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-26
executor_model_per_owner: GPT-6.2 Sol＋Extra／高推理；接收执行无需Pro
executor_start_state: NOT_OBSERVED_FOR_157
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
allowed_directory: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-signal-control-consumption-157/
allowed_files:
- MANIFEST.md
- signal-control-consumption.md
- facts-and-boundaries.yaml
- signal-control-consumption-paths.dot
- deferred-findings.yaml
- evidence.md
D00: []
R155_01: CLOSED_BY_156_CHECKPOINT_READ_SUPPLEMENT
execution_parent_short12: 2d134f987c21
model_scenarios: 5
independent_input_rows: 10
open_questions:
- 仅源级条件模型；真实调用者、映射、返回尾、运行效果及后台档位未知。
---

# 219号合同 · 第157批 · 信号设置、共享队列与旧值回填消费

**继续原「MYOS2-A-C11b 代码分析执行」／EXEC-26，使用GPT-6.2 Sol＋Extra／高推理，接收与执行无需Pro。** 本合同授权规范刷新与fresh G00后连续完成三相关专题、五必要场景、恰六新增、自检、唯一作者回执和发表后实际G04；随后STOP_WRITING，回C11 Pro独审并同轮后继，不另等开工、不进入158。

[156主线独审](reviews/CORE-USER-KERNEL-WRITE-CONSUMPTION-156-review.md)为PASS_PENDING_LOCAL，并将R155-01判为CLOSED_BY_156_CHECKPOINT_READ_SUPPLEMENT。本批D00=[]；155原RETURN_DOCUMENTATION及历史全文完成未证保留，当前与本次闭合合读后为PASS_PENDING_LOCAL。R152-01既有闭合不变。冻结[156作者7196164949](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7196164949)及[2d134f987c21六件](https://github.com/08822407d/MyOS2/tree/2d134f987c21/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-kernel-write-consumption-156)。156三项非阻断说明按主线独审合读，不回改旧件。

固定源码time=a039d9803ade；执行唯一父2d134f987c21；最终发布lead以本轮真实主线评论为准。以下五场景中的十个输入行各自重置；按场景覆盖必要对照，不把行数变成新增场景配额。

## 1. 本批目标与增量

本批追踪两个实际存在的信号设置系统调用：`rt_sigaction`与`rt_sigprocmask`。回答三个相邻问题：状态何时已经写入、改成忽略动作如何消费共享pending位图与队列、回填旧值如何与输入/输出别名及错误出口相互作用。冻结五个必要场景A01、B01、B02、C01、C02；表中同源对照行仍各自独立重置，不能接成一次真实程序执行。

旧09已经说明两个接口保存什么，不能把基本入口、掩码、锁或copyout顺序重新包装成首次发现。157的新增交付是带对象身份、位图、队列计数和返回阶段的具体见证，尤其是`flush_sigqueue_mask`实际体与其调用前后的组合，不重复信号帧、重启、kill发送/唤醒或155/156文件位置消费。

### 1.1 必须保留的旧范围

旧结果均固定于`2d134f987c21`中的原件；下表是定点消费范围，不是全历史逐字验收声明。

| 旧包及原件selector | 已有结论 | 157增量及排除范围 |
|---|---|---|
| 09 `user-return-signal.md` D02-a（L245–261）；facts的`ipc.signal.action_mask_syscalls`（L417起）、AD2-16/19/20/21/22 | 大小门、旧动作快照、新旧flags过滤、不可屏蔽位过滤、动作写表、忽略动作只清共享pending、mask合成及旧值copyout顺序均已存在 | 以混合队列节点、bitmap早退、输出检查拒绝及输入输出同址的对象见证接上这些事实；不声称这些源语句是新发现 |
| 09 同正文D02-a用户访问段；deferred RS-01/RS-11；D02-b中RS-12/13/14 | flags/include构型仍有构建未知；from与to检查不对称、共享sighand、私有pending来源边界已登记 | 不补签SA_RESTORER配置、实际用户缓冲或任务生产者；若同根因仅作evidence_append；不自动关闭旧ID |
| 10 `signal-send-wake.md` S02及与09接收端衔接（尤其L197、L220、L290–294） | kill的共享pending选择、出队消费、`__sigqueue_free`连接与私有同步前提已有材料 | 本批入口是设置动作后`do_sigaction`的flush调用；不重做kill、分配、PID、出队或唤醒 |
| 63 `uaccess-copy.md` A/G01、B/G02（尤其to包装段）；64 `uaccess-recovery.md` A/B/C及其后继消费限定 | 地址真假不等于映射；to门拒绝返回原长度；raw/异常表/alternative真实恢复未证 | 只消费门拒绝和局部返回，增加信号状态已提交后的消费见证；不重新审计复制汇编或恢复能力 |
| 155 `rw-position-consumption.md` A/B/C；156 `kernel-write-consumption.md` A/B/C | 文件I/O位置对象、方法选择、正/零/负返回消费 | 本批不复用位置数值模型或把函数名替换成新课题 |

在完整lead树2067项的review/checkpoint/contract标题中检索`signal|sigmask|pending|queue`，除09/10及勘误、检查点外，命中88/89工作队列、133/136运行队列等。已定点读取88/89/133/136原MANIFEST的followup、inputs及覆盖栏：88清单的交付表写work提交、pool链、scheduled/回调；89正文G01/A、G02/B、G03/C分别为静态池、worker创建、manager进展；133清单A/B/C为rq clock/flags与picker。136虽读取signal.h，其正文D00 R135-02（L38–42）涉及`K.signal.pids[PIDTYPE_TGID]`写槽，A/B/C为sched_fork、CPU0链表及picker；这不等于本批动作表/信号队列清理已被建模。不能仅凭同含queue就判断重叠，也不据标题检索声称全历史无任何重叠。157证据中要保留本批实际检索集合及上述具体selector。

### 1.2 实际调用者与停止层级

固定`syscalls_64.h`有两个活动表项，`lock_IPC/syscall.c`有实际包装和核心调用；`linux/kernel/syscalls.h::MYOS_SYSCALL_DEFINE4`经`asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx`形成`__do_sys_rt_*`本体及相应包装。`do_sigaction`在忽略谓词为真后活动调用`flush_sigqueue_mask`。这些建立的是所选配置下的源内接口连接，不是预处理或实际调用已执行。

已读`myinitramfs/myshell/tests.c::test_all`含`signal(SIGTERM, sigaction_SIGKILL_test)`，而`myshell.c`中的对应`test_all()`行被注释。必须同时给两处证据。只据这两个已读文件不能称全仓绝无其它调用者；更不能把该测试当作当前shell已实际执行。外部libc的`signal`包装、真实程序是否发出这两个系统调用、实际任务/队列生产者、用户映射、最后用户返回和handler执行均未知。GIVEN对象不是实际用户进程或内核现场。

所有结果停在选定`__do_sys_rt_sigaction`／`__do_sys_rt_sigprocmask`本体的局部return或更早证据边界，不继续套用系统调用返回尾。09已证明返回工作还可能消费信号并改帧，所以本批末态不能称为用户最终观察状态。

## 2. 共同GIVEN、配置与对象有效性

1. 选定8位字节、32位int、64位long/ulong/指针及一字`sigset_t`的有限源级模型。`options_flags.cmake`给64位/x86_64/SysV文本；`asm/uapi_bitsperlong.h`在`__x86_64__ && !__ILP32__`时给64；generic signal头给`_NSIG=64`及`_NSIG_WORDS`。本模型选该条件，`sizeof(sigset_t)=8`。额外选`__ARCH_HAS_IRIX_SIGACTION`及`__ARCH_HAS_KA_RESTORER`未定义的类型分支，sigaction仅四个所读字段，名义复制尺寸32。编译、布局和宏覆盖没有独立认证；若执行方无法支持所选类型/定义条件，就给符号尺寸或登记PARTIAL，不能伪造预处理结果。
2. `current=T`，T、`T->sighand=H`、`T->signal=S`及被跟踪动作表、blocked、共享与私有pending均有效、可读写、存活；各对象互不意外别名。锁取得/释放与选定前缀正常完成；没有并发修改、异步投递或回调改动本批跟踪字段。它们是推导前提，不是共享sighand、锁、任务生命周期已认证。
3. 所有链表均按真实`List_hdr_s`/`List_s`表示：有效循环双链、anchor及next/prev互相一致、count与列出的成员数一致，每个节点只出现一次。next/prev非NULL且不等于LIST_POISON1/2，邻接互指一致，`__list_del_entry_valid` L174–176的while条件为假。队列`info.si_signo`在1..64内；其它被读取字段已初始化。只在明确的B02中允许pending位图与节点集合不一致；链表本身仍一致。`list_header_contains`、删链与安全迭代宏必须回源，不能以Linux链表常识替代。
4. 普通节点到`kmem_cache_free(sigqueue_cachep,q)`时，只认证源内请求及已发生的摘链/count变化。若要推后续迭代和局部return，额外GIVEN该调用正常返回且不改其它在链节点；分配器实际回收/复用未知。`SIGQUEUE_PREALLOC`分支只按实际早退解释，不据此新签泄漏或资源所有权。已交free的对象后续不再读取。
5. 所有input-copy成功行明确GIVEN复制源/目的对象合法、`copy_from_user`正常返回0且完整形成列出的输入值。不能因源码from方向缺门就保证复制成功。正常output-copy行另外GIVEN目标合法、instrument/raw正常完成并以0残量复制完整旧值；这一承诺不是从access_ok为真推出。
6. 输出拒绝地址使用纯数值`B=0xffff800000001000`，实际C指针及64位有符号转换采用所选模型；不建立映射。对n=8或32，小常量门判断起点为负；general门的无符号sum不回绕且仍为负，两个分支都拒绝。因此无需预设编译器的`__builtin_constant_p`取值。必须从实际include/guard连接到x86 `__access_ok`，沿`copy_to_user→_inline_copy_to_user`读出返回原n；不进入instrument/raw，更不模拟真实CPU访问。
7. 掩码场景另外GIVEN T的`TIF_SIGPENDING`测试为假，因而`__set_task_blocked`的前置if短路，不展开thread_group_empty或retarget；必须注明这是输入边界。必要时可用实际thread flag字段/测试定义支撑该条件，不把它说成所有真实任务恒假。本体中retarget/recalc注释状态仍按实际源记录。

数值以真实定义为准：`sigmask(s)=1UL<<(s-1)`；SIGKILL=9、SIGUSR1=10、SIGUSR2=12、SIGTERM=15、SIGCHLD=17、SIGSTOP=19；SIG_BLOCK=0、SIG_UNBLOCK=1、SIG_SETMASK=2；SIG_IGN/SIG_DFL为所读特殊动作；`SIGQUEUE_PREALLOC=1`、`EFAULT=14`、`EINVAL=22`。使用十六进制及信号集合双重表示，禁止把GIVEN位图当作真实发送历史。

## 3. A：动作提交后，旧动作输出门拒绝

### A01／ACT_IGN_COPYOUT_REJECT

从rt_sigaction本体入口开始。`sig=SIGTERM(15)`、`sigsetsize=8`、act非NULL且其32字节输入完整复制成功，oact=B。旧`H.action[14].sa={handler=SIG_DFL, flags=0, restorer=Rold, mask={12}=0x800}`；新输入`{handler=SIG_IGN, flags=0, restorer=Rnew, mask={9,10,19}=0x40300}`。Rold/Rnew为可保存的不透明字段值，不作为可执行代码证明。旧flags不含SA_IMMUTABLE；new/old flags都为0，避免把RS-01未知用作本例结论。

共享pending位图为`{10,15}=0x4200`，链表按顺序`qA(15,flags=0) → qB(10,flags=0) → qC(15,flags=SIGQUEUE_PREALLOC)`，count=3。私有pending独立含`qD(15,flags=0)`、位图`{15}=0x4000`、count=1。

必须逐事件说明：大小门；输入复制；号/只允许内核处理信号门；锁与immutable门；old_sa快照；flags与mask过滤；`*k=*act`；忽略谓词；flush位图交集/清位/遍历；普通与预分配节点的不同free请求；解锁；核心return；旧动作copyout门；syscall本体return。不能把最后-EFAULT提前到动作写入之前。

**预期核验项（执行者必须自行回源核算）：**新动作sa_mask去掉9/19后为`{10}=0x200`；flush另由sig=15形成其局部mask `{15}=0x4000`，两份mask不是同一对象或同一含义。shared位图清15后`0x200`，qA/qC均摘链，count依次3→2→1，最终只qB。qA有一次`kmem_cache_free`请求；qC在摘链后因PREALLOC早退，不向free传递；qD/私有位图/count在这段函数内保持原值，因为每线程flush循环为注释。核心返回0后，copy_to_user(B,old_sa,32)数值门拒绝、返回32，本体返回-14。此时新动作、shared清理已成立，不因后面的输出失败撤销。old_sa只是内核局部快照，未向B发起raw写；不能说用户得到了旧动作，也不能说真实进程观察到上述末态。

遇任一更早输入访问、锁、链表或free前提不成立，只保留此前事件并停在首次边界，不能跨过它保证后续-EFAULT。

## 4. B：清理谓词与位图门，不能只看同号节点

### B01／DEFAULT_ACTION_FLUSH_SELECTOR

两个独立重置的源级对象模型，均`sigsetsize=8`、act复制成功、oact=NULL、新handler=SIG_DFL、new flags/mask=0，旧对应表项flags=0且handler为H0（不等于SIG_DFL/SIG_IGN的不透明值）。每行shared链表为`qM(sig,flags=0) → qO(10,flags=0)`，count=2；私有同号位与节点单列保持有效。其余前提沿§2。

| 行 | sig | shared入口位图 | 必答的真实分支 | 正常完成时的局部末态 |
|---|---:|---:|---|---|
| B01_CHLD | 17 | `{10,17}=0x10200` | `sig_handler_ignored(SIG_DFL,17)`经`sig_kernel_ignore`为真，活动进入flush | 新动作已存；shared剩`0x200`及qO，count=1；qM摘链后交一次free请求；私有同号对象未由本体清理；局部return0 |
| B01_TERM | 15 | `{10,15}=0x4200` | 相同SIG_DFL字段经实际ignore集合得到假，不调用flush | 新动作已存；shared仍`0x4200`及qM/qO，count=2；本段无队列删链/free请求；局部return0 |

必须给`sig_handler`读刚写入表项、`sig_handler_ignored`、`sig_kernel_ignore`/`SIG_KERNEL_IGNORE_MASK`的真实定义，不能从POSIX/Linux预设默认动作。此处不展开后来get_signal对默认致命动作的处理；局部return0不证明任务终止或handler执行。

### B02／BITMAP_MISS_WITH_QUEUED_NODE

`sig=15`、新动作SIG_IGN、flags/mask=0、oact=NULL；其它门正常。shared位图只有`{10}=0x200`，链表却为`qM(15,flags=0) → qO(10,flags=0)`、count=2；这是明确GIVEN的位图/节点不一致见证，不声称当前kill、并发、fork或任何实际生产者已产生该状态。

`do_sigaction`仍写新动作并进入flush；flush的mask为`{15}=0x4000`，与shared位图交集为0，先行`return`阻止后面的清位与遍历。预期为qM/qO、count=2及位图0x200保持；没有`sigismember`逐节点判定、没有删链/free请求，最终核心及syscall本体均return0。不能按“队列里有15”绕过真实bitmap门，也不能把这个条件模型登记为已复现内核故障。图应画出早退门，不能把位图检查直接画成必经队列遍历。

## 5. C：mask的输入快照、同址输出与错误优先级

### C01／ALIASED_MASK_INPUT_OUTPUT

每行独立重置：`T.blocked={2,10,12}=0xA02`。用户符号对象U起点数值0x400100、名义容量8、输入值`{9,10,19}=0x40300`；nset=oset=U，sigsetsize=8，U不与T.blocked、局部old_set/new_set或任何内核对象别名。input和output完整复制成功条件按§2分别给定。

必须追踪`old_set`先快照、input复制到独立new_set、去9/19得0x200、按how形成另一局部newset、提交blocked，然后把old_set复制回同一个U。不能把晚发生的输出覆盖倒推成输入读取的新值；也不能将用户U和内核new_set当作同一对象。

| how | new_set过滤后 | 合成后blocked | `__set_current_blocked`门 | U最终值 / 局部return |
|---|---:|---:|---|---|
| SIG_BLOCK=0 | 0x200 | 0xA02 | 与旧blocked相等，直接return，不取该函数内的siglock | 0xA02 / 0 |
| SIG_UNBLOCK=1 | 0x200 | 0x802（{2,12}） | 不等，按GIVEN完成锁/写/解锁 | 0xA02 / 0 |
| SIG_SETMASK=2 | 0x200 | 0x200（{10}） | 不等，按GIVEN完成锁/写/解锁 | 0xA02 / 0 |

队列、action和TIF不因这段mask函数本体得到新的“已经清理/唤醒”结论。无需增加真实信号投递、返回尾或恢复帧场景。

### C02／NULL_SET_INVALID_HOW_AND_COPYOUT_ORDER

各行独立重置T.blocked=0xA02。V为独立合法8字节输入、内容0x40300，复制成功；U为独立合法8字节输出，正常复制条件另给。大小均8；how=99可表示但不属三个实际case。B仍为§2的拒绝地址。

| 行 | nset / how / oset | 必须认证的到达与未到达事件 | 局部return及blocked |
|---|---|---|---|
| C02_QUERY | NULL / 99 / U | 跳过整个nset块，不调用sigprocmask，不做how校验；用入口old_set正常写U | 0；blocked=0xA02，U=0xA02 |
| C02_BAD_HOW | V / 99 / B | 先完成输入复制及9/19过滤，再由sigprocmask default返回-EINVAL；外层立即return，不调用copy_to_user，连B的地址门都未执行 | -22；blocked=0xA02 |
| C02_COMMIT_THEN_REJECT | V / SIG_SETMASK / B | 合法how先提交blocked=0x200，再到copy_to_user；B门拒绝返回8，外层-EFAULT | -14；blocked仍0x200，无raw输出请求 |

给出同一表中errno优先级的源语句依据，而不是泛称“无效输出总是EFAULT”。本批不扩大到任意非零raw残量或真实缺页；这两种需要独立的底层返回与对象前提，不能拿本表证明。

## 6. 必要源门、锚与图要求

全部正式强断言按现行P2/P6给单一定义内1–5物理行连续原文，完整保留空白；定义或宏入口另给，不用行号猜归属。一个源窗可由多处引用，不按五场景复制五份相同锚。

至少闭合以下实际来源，缺关键体不得只列TODO就申报READY：

- [mykernel/lock_IPC/syscall.c](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/syscall.c)两入口的大小门、输入复制、do_sigaction/sigprocmask调用、错误优先返回、old_set快照、输出调用及局部返回。
- `mykernel/lock_IPC/signal/signal.c::do_sigaction`的合法号/内核独占号、immutable、old_sa、flags过滤、mask过滤和表项写入、忽略谓词、共享flush与注释私有循环、锁及return；`flush_sigqueue_mask`完整逻辑；`__sigqueue_free`的PREALLOC门和free调用。
- 同文件`sigprocmask`三case/default、`__set_current_blocked`相等早退/写入路径、`__set_task_blocked`实际字段写入与本批采用的短路前提。
- `signal.h`的sig_handler/sig_handler_ignored、sigismember/sigisemptyset/sigequalsets/sigdelsetmask、sigandsets/sigorsets/sigandnsets的真实宏实例；`signal_macro.h`的sigmask、ignore集合、siginmask和_BINOP；必要的const/types/generic信号值。
- `double_list_macro.h`的安全迭代到真实for宏；`double_list.h`的contains、delete、list_del_init及所需邻接更新；`double_list_types.h`的anchor/count。删链函数返回值不被flush检查是事实，但在有效列表GIVEN下先证明contains命中，不扩张到损坏链表后仍可安全free。
- uaccess实际include/guard选择、copy_to_user包装和两种__access_ok数值分支；默认构型、位宽、errno、表项；`linux/kernel/syscalls.h::MYOS_SYSCALL_DEFINE4`与`asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx`的命名/参数绑定；tests.c/myshell.c调用者限定。

图仅需覆盖本批实际消费：call为调用方→被调用方，data为消费方→提供方。flags/位图/计数写入写成effects；源中注释的每线程flush/retarget/recalc不得画为活动调用。应区分核心return0、最后copyout及syscall本体错误return，标清B02早退、C02不进入输出门。GIVEN节点不是新的真实helper，ANALYSIS_STOP不是CPU自旋；本批没有需要新增CPU无限循环结论的场景。

## 7. 恰六件交付

仅向`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-signal-control-consumption-157/`新增以下六件：

| 文件 | 内容 |
|---|---|
| MANIFEST.md | 接收/规范刷新和主线指定D00的实际记录；固定输入、五场景完成度、六件清单、自查、未知、STOP_WRITING及待主线验收 |
| signal-control-consumption.md | 三专题连续解释；五场景和必要对照表；实际调用者已证/未知；对象写入、返回阶段及停止边界 |
| facts-and-boundaries.yaml | SOURCE_FACT/GIVEN/ARITHMETIC_MODEL/UNKNOWN分栏；场景、对象、事件、数值、正式窄窗和精确历史selector |
| signal-control-consumption-paths.dot | 有界条件图；与facts同ID、真实调用层次、输出门和早退边一致，不运行Graphviz |
| deferred-findings.yaml | 新的有条件候选或显式空集；旧项evidence_append；不重复立RS-01/RS-11/RS-12/13/14及UC63根因，不自行改历史status |
| evidence.md | 实际读取/限定查询集合、方法、自制有限模型若有、P2/P6结果及首次停止、写前后核对入口；不伪称完整本地P9或真实程序运行 |

允许只对已取得的固定文本与上述有限整数/对象模型自行写少量核对；不执行项目脚本、预处理、编译、C/ASM、Graphviz、内核或QEMU，不构造宿主信号夹具，不读仓库外libc。不要为了窗口/场景数字追加同义工作。完成后只申报READY_FOR_MAINLINE_REVIEW并停写；不从开放项自行进入158。

## 8. 技术READY的可核标准

五场景都必须同时给：明确入口与GIVEN、实际有序门、正常/提前返回、跟踪对象的前后状态、未到达事件和首次未知。A01要区分摘链、count、free请求及最终输出拒绝；B01要从新动作和真实默认ignore集合解释两个结果；B02要保持“条件不一致状态”与“真实生产者未知”；C01要区分三个局部mask对象和别名的用户U；C02要以源顺序证实how和copyout谁实际执行。真实用户/映射/运行未知可以保留，但关键源语句、调用绑定或前置定义未取到不能由设计预期补齐。

设计中的数值是待作者独立核验的检查对象，不是结果证据。主线最终验收应另从固定源和GIVEN推导，再比较作者交付，不以本设计文本或作者自报计数代替独立判断。


## 9. 本轮入口、规范复用和权限

以本轮PR17唯一主线正式评论为真实入口，取实际ID及最终固定lead提交，完整读本合同、同提交[156独审](reviews/CORE-USER-KERNEL-WRITE-CONSUMPTION-156-review.md)及[检查点](checkpoints/2026-11-26-kernel-write156-reviewed-signal157.md)。216恢复包和完整启动词继续提供身份/读序依据。157不继承156调用控制栈；原C11b继续，无新对话/改名UI动作。

保持MYOS2-LEAD-002-CORE-CHECK-01、阶段2、C11b／EXEC-26、claude/dazzling-cori-q0dnyt及原Draft PR17；唯一写区为页首user-signal-control-consumption-157六件。Owner同轮后继授权与00§19、本合同共同成立，不重演早期建分支/阶段0等待。没有源码、旧件、主线、PR元数据或外仓写权。

按79只复用**本人同一C11b会话实际完整读过且版本未变**的十五项目规则、MA有效全文及45十五条、Mne任务局部接收/命名。R155补读于156完成并由本轮主线闭合；版本未变可复用当前本人2032行完整覆盖，不得把该记录倒签到155，也不得用主线/其他对话的已读代替自己。本轮fresh MA master b234c6c91a2e、建设06b783861368，入口881fd8c1076e、检查点54ac6e5fc66c；Mne master e581eb9df6c4。新适用差异实际读并留证；§322待议不是新采用。

保留18结果先行/问题先入库、24职责、28同轮后继、63先查效果再限次重试、124次一档执行/Pro验收、198正文入库和单一短入口、210的8/10提醒。实际胜任、容量、规范刷新和下一安全步如实记；未知后台型号不冒签，不因此重复索取Owner许可；真正权限/材料/容量阻断才保全并报告最小缺口。

## 10. fresh G00与冻结基线

1. 直接lead等于本轮评论真实发布提交，exec=2d134f987c21，time=a039d9803ade，直接master=e8e60cd19540；不以PR.base_sha代替master。原PR16 Open/Ready、PR17原标题Open/Draft，原head/base master且未合并；开放PR无since完整到空。
2. PR17评论无since全分页到空，满100继续。检查点64历史标记加本轮独审156/判闭R155/发令157三枚，共67，逐条字面一次/实际ID正确，不靠总数。当前入口、156作者7196164949、前次发令7195763761各直接ID双读并与分页同文。准备307评论，加本轮入口预计308，以fresh实际页数为准。
3. 检查点十二枚错误作者负标记均0：EXEC-23/149、EXEC-22/145、EXEC-24/152/153/154/155/156/157、EXEC-25/155/156/157、EXEC-26/154。正确EXEC-26/155及156各1，EXEC-25/152/153/154各1。本批EXEC-26/157入场0；核任何作者的157占用和整个结果目录，未知并行写不覆盖。
4. 156固定2d134f987c21、活动exec及API Git Blob六件全文一致，12325 LF/404944字节；唯一父def3b82d9191，一提交六新增零其它改删、原2259非tree保持。157写前完整exec2660项/2265非tree为冻结基线。只核冻结及主线裁定/闭合和三项说明，不重算156的126阶段或76窗。
5. 两time canary、直接master反验和公约原句实核，保留read_channel与self_check。新MANIFEST明确D00=[]、R155由本轮主线闭合的真实入口、旧历史保持，以及本次接收/权限/规范复用/胜任容量和下一安全步。只有本人实际完成的读取才记完成；通过G00后连续A/B/C。

## 11. 六件、自检、唯一发表与实际G04

文件名及内容见§7。接收、规范刷新和D00空集放MANIFEST；R155既有主线闭合用精确入口/selector索引，不重新补读/补写155历史，不追加第七件或接收评论。保留startup_selfcheck_quote、branch_canary_quotes、read_channel及self_check三整数。允许共享基态和有序差量，但须可重建关键状态/门/错误优先级；函数帧退出后局部对象只可为历史快照。指针同址、位图、链表count、摘链和free请求各按真实对象/事件记账。

P2/P6正式新证据每窗1–5连续物理行，固定源原字节、LF/CRLF/NONE、真实定义/命令实例及相邻断言重开核；P6 n=m+k，普通位置另账。旧156的76、155的101及主线0不并入新账。连接器可达P9第1–8、11、13实做，9/10无代码变更、12非降级可NA；完整本地P9未做即NOT_RUN。强制锚不得以降级逃避，不设字数/窗口/新问题配额。

写前fresh四refs、原PR状态、作者/目录未占用及未知并行写。以真实2d134f987c21为唯一父，expected_sha匹配、force=false，原exec一次提交六新增零其它改删。提交后先回读六件固定/活动/API Git Blob全文、LF/字节、唯一父、compare净差异及全部2265旧非tree对象，再发唯一作者正式评论。

唯一作者标记：

MYOS2-LEAD-002-CHAT-EXEC-26 / CORE-USER-SIGNAL-CONTROL-CONSUMPTION-157 / FORMAL_DELIVERY

作者评论给真实固定合同/提交/六件、D00=[]和R155既有主线闭合、五场景与关键停止边界、指定/可见/未知模型信息、实际P6及自检。发表时actual_G04=PENDING_POST_PUBLICATION、mainline_acceptance=PENDING；不可预签发表后终检或157主线验收。

发表后实际核新评论ID两次并与全分页同文；全评论到空，67原标记加唯一作者共68，逐条字面一次/实际ID正确，十二负标记0。四refs（exec为真实157提交）、开放PR到空、两原PR状态/原标题/元数据、六件三渠道全文、唯一父/一提交六新增零其它改删及全部旧对象/旧评论冻结均实核。通过后才报READY_FOR_MAINLINE_REVIEW、实际G04=PASS、STOP_WRITING，不启动158。

暂态遵63：先查远端效果，避免重复写入，再通常首次加三次、10/20/40秒或实质工作间隔限次重试。参数校验未发出、工具上限恢复、写重试与读回暂态分账；expected-head与force=false不放松，write_retry_log如实。未知并行写不覆盖，旧评论/文件不为补正而改。

按198最终只给一个通常3–6行原生短复制块和一个真实作者评论URL，报157五场景六件、唯一提交/P6、D00空集、实际G04/停写与必要未知，交C11 Pro独审并同轮后继。完整正文留GitHub，不要求Owner拼长报告或裁决普通技术问题。

## 12. 技术READY、计数与限制

157由设计分工提出，另一未参与设计的技术分工先读固定实际源、保存源语句推导笔记，再读具体GIVEN并独建五场景十个重置分支，未接收或执行设计模型。28个实际源Blob、153处语义源锚与有限对象/整数推导共311项，311通过、0失败，技术READY。实际MYOS_SYSCALL_DEFINE4到__do_sys的宏文本绑定、有效链表非NULL/非poison/双向一致且删除valid门while为假的GIVEN已明列；摘链/计数/free请求、bitmap早退、三mask对象、输入输出同址及错误优先级均核同。该计数只属合同就绪，不是未来作者P6或完整本地P9，不预签157交付与实际运行。

该READY只认证合同的源码/GIVEN/预期/边界自洽，不预签未来157成果、作者G04、实际用户/信号生产者或运行效果。旧09基本入口/保存/过滤/copyout事实不当首次发现；标题检索是限定查询，不能证明全历史无任何重叠。A/B仅局部信号设置及清理，C仅mask本体返回，不推完整用户返回尾和信号处理结果。

本次成功首次发布157时C11计5→6，8/10未达；156验收与R155判闭不另计，C10历史11不变。未来交接带十五项目规则、有效MA全文/同版共用全文、Mnemosyne命名依据、实际断点/身份/计数和口述约定。原C11b当前继续，不以一次交付保证未来容量。

阶段2，只允许自写有限纯文本/整数/对象辅助；无项目脚本/程序、预处理、编译链接、C/ASM执行、Graphviz、宿主信号夹具、内核/QEMU或实机/真实IO。完整本地P9 NOT_RUN，实际ABI/构型、用户映射/真实调用者/生命周期/运行效果及后台模型和Extra未认证。不得改源码/旧件/主线/PR元数据、新分支PR、合并、强推、删除或跨仓写；没有聊天UI或自动选模操作。
