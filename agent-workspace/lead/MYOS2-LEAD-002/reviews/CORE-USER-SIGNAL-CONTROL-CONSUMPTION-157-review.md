---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
conversation_display_name: MYOS2-A-C11 内核分析主线续建
produced_by: GPT-6 Astra Pro（本轮系统身份）
model_per_owner: Pro独立验收并同轮安排后继
actual_backend_model: UNATTESTED
actual_reasoning_effort: UNATTESTED
date: '2026-11-27'
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
  verified_claims: 3
  quotes_reconfirmed: 3
  downgraded_to_inferred: 0
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
previous_mainline_design_count:
  conversation: C10
  distinct_batches: 11
evidence_class: SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / OBSERVATION / UNKNOWN
lead_before_publication_short12: e9f4a788b1aa
execution_pin_short12: abbf15de6fbb
self_check_scope: 仅本独审新增R157-MAP-1/2/3三正式源窗，3/3/0；作者205/205/0和41普通位置另账。
mainline_design_count:
  conversation: C11
  before_release: 6
  after_successful_158_release: 7
  thresholds:
  - 8
  - 10
  reached: false
status: PREPARED_FOR_PUBLICATION_G04_PENDING
record_type: independent_mainline_review
reviewed_batch: CORE-USER-SIGNAL-CONTROL-CONSUMPTION-157
reviewed_author: MYOS2-LEAD-002-CHAT-EXEC-26
mainline_acceptance: PASS_PENDING_LOCAL
D00: []
next_batch_D00: []
R155_01: CLOSED_BY_156_CHECKPOINT_READ_SUPPLEMENT (retained)
open_questions:
- 历史不倒签；两项非阻断说明及主线三叶宏窗合读；完整本地P9、实际后台/effort/运行未认证。
---

# 第157批独立验收 · 通过，同轮接158格式容量消费

## 1. 裁定、独立性与固定依据

**157裁定PASS_PENDING_LOCAL；158 D00=[]。** R155-01既有CLOSED_BY_156_CHECKPOINT_READ_SUPPLEMENT保持；155原RETURN_DOCUMENTATION、历史全文完成未证和原range/true不改，当前仍与156主线闭合合读为PASS_PENDING_LOCAL。R152-01既有闭合和152历史未知保持。本轮新增三个叶宏窄窗作为主线显式补链，另说明两处非阻断措辞/行号记账；均不要求改旧交付或重做运行。

被审者MYOS2-A-C11b／MYOS2-LEAD-002-CHAT-EXEC-26。[作者7206572285](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7206572285)、[唯一提交abbf15de6fbb](https://github.com/08822407d/MyOS2/commit/abbf15de6fbb)、[219合同@e9f4a788b1aa](https://github.com/08822407d/MyOS2/blob/e9f4a788b1aa/agent-workspace/lead/MYOS2-LEAD-002/219-user-signal-control-consumption-contract.md)、[发令7197587783](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7197587783)及[固定六件](https://github.com/08822407d/MyOS2/tree/abbf15de6fbb/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-signal-control-consumption-157)为正式依据。

技术分工先只读219、fresh固定源并封存源/GIVEN模型，随后才打开作者六件；没有使用旧157设计模型或311 READY作为预期，也未参与157作者实施。源模型按实际语句细化到作者事件粒度后，再独立比较每步写集合/from/to及完整所选状态，不以作者delta/terminal反建预期。文证分工另核全部引文、图、公开接收及历史selector，C11合并裁定。源码获取路径数不等于整文件所有行都作了语义验收；reads按所选关键读取范围审，不冒称全部C读取穷尽。

Owner回传实际G04=PASS、READY_FOR_MAINLINE_REVIEW、STOP_WRITING，未进入158。主线核当前公开成果、元数据与冻结，不认证作者私有工具全过程；原文件/评论PENDING_POST_PUBLICATION保留发表时点。后继见[220合同](../220-user-libc-format-capacity-contract.md)，继续原C11b，无新恢复席。

## 2. 唯一提交、三渠道及冻结基线

唯一父2d134f987c21；compare ahead1/behind0、一个提交、恰六新增、零其它修改/删除。固定提交、活动exec、实际API Git Blob六件全文一致，独算Git Blob匹配：

| 文件 | Blob短12 | LF | UTF-8字节 |
|---|---|---:|---:|
| MANIFEST.md | 86e1cfad380e | 42 | 22565 |
| signal-control-consumption.md | 6fdd5188c25b | 40 | 10888 |
| facts-and-boundaries.yaml | 8c46d8b8c4dc | 18989 | 500070 |
| signal-control-consumption-paths.dot | 9c29cd112bea | 558 | 111503 |
| deferred-findings.yaml | ce920f00920a | 104 | 4414 |
| evidence.md | 7268578eeaae | 2855 | 96592 |
| 合计 | | 22588 | 746032 |

当前exec完整2667项、2271非tree；父树2660项，2265旧非tree的SHA/type/mode逐一保持。lead=e9f4a788b1aa，完整2070项、1836非tree；time=a039d9803ade，直接master=e8e60cd19540，不以PR.base_sha替代。两time canary物理41/79命中，直接master反验均不命中，公约原句匹配。

PR16 Open/Ready、PR17原标题Open/Draft，原head/base master，未合并；开放PR无since2/0到空。PR17评论完整100/100/100/9/0，共309唯一ID，68精确标记逐一字面一次/实际ID正确；作者7206572285及前次发令7197587783均直接ID双读并与分页同文。原308评论正文、创建/更新时间和正文SHA256与上一轮完整快照全部核同。未见158目录或任何158作者。

主线入场/冻结G00 **2421项、0失败**；当前309正文已保全用于发表后冻结核。细项与完整标记表见[检查点](../checkpoints/2026-11-27-signal157-reviewed-format158.md)。此门不代替下文技术和证据裁定。

## 3. 独立模型及十行结果

首轮源/GIVEN模型**130项通过、0失败**：65源语义锚、7整数、20动作、6列表、20行结构、6别名、6边界；十行按自行划分粒度为216事件。最终比较器**6622项通过、0失败**，包含作者232有序事件、202独立from/to差量、5032个逐事件所选字段快照、202终态字段及门/未达/图语义。不同表示可核同一语义，总数不是独立源码事实数或P6。自制检查器曾将不同帧同名n混淆，按源码对象身份修正后完整重跑；未改作者数据或源码来消除差异。

| 独立输入行 | 事件／差量 | 本体局部return | 关键已提交状态与输出 |
|---|---:|---:|---|
| A01_ACT_IGN_COPYOUT_REJECT | 54／48 | -14 | action IGN、mask0x200；shared0x200/qB/count1；qA一次free请求，qC摘出PREALLOC；输出拒绝残量32 |
| B01_CHLD | 40／34 | 0 | action DFL；shared0x200/qO/count1，qM一次free请求；无oact输出 |
| B01_TERM | 18／13 | 0 | action DFL；shared0x4200/qM,qO/count2；不调用flush，无free/输出 |
| B02_BITMAP_MISS_WITH_QUEUED_NODE | 24／18 | 0 | action IGN；交集m0即早退；shared0x200/qM,qO/count2保持，无节点遍历/删除/free |
| C01_BLOCK | 16／15 | 0 | blocked0xa02，U回填0xa02；相等门提前返回，setter内无锁/写 |
| C01_UNBLOCK | 21／20 | 0 | blocked0x802，U回填0xa02；给定正常锁/写/解锁 |
| C01_SETMASK | 21／20 | 0 | blocked0x200，U回填0xa02；给定正常锁/写/解锁 |
| C02_QUERY | 7／6 | 0 | blocked/U均0xa02；nset=NULL，how99未校验，core未调用 |
| C02_BAD_HOW | 10／10 | -22 | blocked0xa02；输入/过滤已完成，随后default早退，oset判断和地址门均未到达 |
| C02_COMMIT_THEN_REJECT | 21／18 | -14 | blocked0x200已提交；输出拒绝残量8，无raw输出，不回滚 |

五场景含十个独立重置输入行，不能接成一次真实程序执行。全部终点均为选定syscall本体局部return，不等于最后用户返回/handler执行。任一更早对象有效性、复制、锁、列表或free正常返回前提不成立，即在首次边界停止，不能保证后续错误值。

### 动作、队列与错误顺序

[syscall.c](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/syscall.c)给size、输入、core、core错误优先、旧值输出、局部return顺序；[signal.c](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c)给合法号/immutable/旧快照、独立flags及mask过滤、整项写表、读刚提交handler并判忽略、共享flush。新action.sa_mask=0x200、do_sigaction.mask=0x4000及flush.m是不同对象。

flush先算交集、空则返回；非空先清shared位图，再安全遍历。实际[List宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list_macro.h)先缓存后继，[List函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h)先contains命中，valid while在GIVEN下为假，更新邻接、被摘节点自环，再减count。A的qA/qC均摘链；PREALLOC只控制摘链之后是否发free请求，不能阻止既有删除。普通free正常返回为GIVEN，之后不再读qA；先前对象信息仅历史快照，不认证分配器回收/复用或资源所有权。私有flush循环是注释，qD不由本体清理。

SIG_DFL的17属于实际ignore集合，15不属于；B01_TERM不调用flush。B02明确给定位图/节点不一致，m=0使遍历及删除均未达，不是生产者或已复现故障证据。A核心返回0后才检查旧动作B输出，拒绝返回32，外层-14；已提交新动作/shared清理不回滚，不能说用户收到了old_sa。

### mask别名、输入与地址门

old_set、syscall.new_set、sigprocmask.newset三个局部对象分开；U仅使nset/oset同址，不与内核对象别名。C01先取得输入并形成操作结果，再用old_set回填U，不反向改变此前输入或blocked。相等门先于setter取锁；TIF_SIGPENDING=false使实际&&右项thread_group_empty短路，retarget/recalc注释不产生活动调用或唤醒证明。NULL nset跳过how校验；无效how先-EINVAL，不能虚构晚到的-EFAULT。

[uaccess_64.h](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/uaccess_64.h)的同名guard与实际include链选中x86门。B=0xffff800000001000在所选64位模型中起点为负；B+8/B+32不回绕且仍负，小常量和general两支均拒绝，general后半条件短路。无需猜constant_p；拒绝返回原n，不进instrument/raw。U数值门通过不证明映射；输入成功与正常输出均另有明确GIVEN。实际预处理/ABI/用户映射不因此认证。

## 4. 文证、图、规范与历史范围

文证/图/公开接收辅助检查**3217项、0失败**。作者205正式窗P6=205/205/0：494 LF加1 NONE，共495物理行；41普通位置另89 LF，不并入P6。45实际源Git Blob全部独算一致，246项范围、原字节、1–5物理行、行尾、真实定义/命令实例、URL/blob、source_index及相邻断言核同。

唯一NONE为SG157-E192：[syscall_64.c::x64_sys_call物理L33](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/entry/syscall_64.c#L33-L33)，blob a21461322a9f，末行紧接EOF。L32有LF，围栏分隔换行未冒充源行尾。205中不含下一节主线新增的3窗。

DOT与facts逐条全部属性一致，共281节点/274边；ROW_TRACE为232/222，十条独立链的局部终点均无后继；SOURCE_OVERVIEW为49/52，46调用/宏关系与6消费者→提供者关系按实际体核同。总览CALL/CALL_CONDITIONAL只表静态依赖，不表示每行实际到达。注释的私有flush、retarget、recalc未画成活动调用；没有调用Graphviz。

历史检索固定1937b1b16805完整2067项树，限定标题模式所得26项与作者清单逐一一致；fresh16历史原件及63/64四件审查/闭合按具体selector核收。09/10基本信号语义、63地址门及64真实恢复未知均具名继承，不宣称全历史穷尽排重。63的旧空行误判撤回、64的arch_cpu_finalize_init归属更正未被本批错误消费。三个SG157-COMB保持CONDITIONAL_MODEL_ONLY，不重报旧根因或真实运行缺陷。

15项目规范和156同席前版blob全部核同；MA/Mne当前版本未变，同席本人完整读取复用有公开记录。R155引用156真实主线裁定和补记，155历史未倒签；不要求私有工具日志或重开闭合项。

## 5. 主线显式补齐__MAP递归叶窗

作者E174给__MAP4，E175给__MAP，E176给__SC_DECL；正文入口首段和facts.source_fact.entry_binding.source_text说明四实参声明绑定。完整固定syscalls.h已取得，技术分工实际读L114–128，绑定结果正确；原205清单未单列三个递归叶定义。本轮主线正式另列三窗：**本独审自身P6=3/3/0**，每窗单一宏定义、1物理行LF。它们与作者E174接成__MAP4→3→2→1→m(t,a)，无需__MAP0。不是声称作者原205已含叶窗，不改作者旧件，不增加158 D00。

### R157-MAP-1 · __MAP1

[VERIFIED mykernel/include/linux/kernel/syscalls.h::__MAP1]
```c
	#define __MAP1(m, t, a, ...)	m(t, a)
```

[固定源码L114](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L114-L114)；blob短12=6cc2f85fec90；1物理行，终止符LF，原TAB保留。

### R157-MAP-2 · __MAP2

[VERIFIED mykernel/include/linux/kernel/syscalls.h::__MAP2]
```c
	#define __MAP2(m, t, a, ...)	m(t, a), __MAP1(m, __VA_ARGS__)
```

[固定源码L115](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L115-L115)；blob短12=6cc2f85fec90；1物理行，终止符LF，原TAB保留。

### R157-MAP-3 · __MAP3

[VERIFIED mykernel/include/linux/kernel/syscalls.h::__MAP3]
```c
	#define __MAP3(m, t, a, ...)	m(t, a), __MAP2(m, __VA_ARGS__)
```

[固定源码L116](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L116-L116)；blob短12=6cc2f85fec90；1物理行，终止符LF，原TAB保留。

## 6. 两项非阻断合读说明

1. facts.input_rows[B01_TERM].ordered_events[key=core_ret].detail及B02相同key复用“内核动作/清理已成立”。这两行应明确为**新动作已成立、没有发生清理**：TERM不调用flush；B02在bitmap空交集门早退。正文、完整差量/状态/未达和图已经如此记录；该模板措辞不能反过来推导删除或free。旧件冻结，后续按本说明合读。
2. MANIFEST.guidance_refresh.mne.prior_full_reuse[3]沿写§7 L445–585。[Mne固定命名原件](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/notes/registries/project-research-display-name-registry-v0.1.md)blob2d930f07381e，实际584物理行/584 LF、CRLF0、末LF；真实§7为L445–584，共140行。585只是末换行拆分空段，实际§7已完整覆盖，没有漏读。158新记录使用真实范围，不修改157、不重开R155或新增D00。

这两处不改变技术状态、来源身份或219接收条件；本轮不将原文瑕疵隐去，也不将有限reads清单夸成所有C对象读取认证。R155历史缺口与其156实际补读闭合仍按原裁定保持。

## 7. 后继及运行边界

[220／158](../220-user-libc-format-capacity-contract.md)接仓内用户库格式容量：非整数EMIT/最终NUL、整数局部/外层指针及首次对象边界、实际shell提示符模板对printf 4096缓冲和返回长度的消费。旧09/10信号接收组合已覆盖，不重复；旧94是内核格式器，旧129/130与117/118的用户库见证处于充足容量域，158明确新增容量边界。四场景、十二个独立输入行、六件；真实链接、getcwd生产者、写入效果继续未知。158由设计分工提出，另一技术分工先读取27份固定源码和GIVEN-only输入、封存源码笔记及十二行有限模型，再首次读取技术合同作比较；没有使用设计预期生成模型。独算191项（27源Blob、62语义锚、102对象/整数/边界断言）与后读合同的100项比较分别0失败，技术READY。四场景十二行的容量/NUL、不同帧q/o、D[4]在q加5前首次停止、D[8]逆写时序及提示符4095/4096/4105请求核同；超对象9仅为请求整数范围，不认证实际读出。真实sh链接、getcwd生产者、ABI与IO保持未知；仅write正常返回时才可继续printf局部return。旧129容量边界项作限定证据追加，不关闭effective ABI或旧真实绑定/输出未知。该就绪账不是158未来作者P6、最终验收或实际G04。

[129原deferred](https://github.com/08822407d/MyOS2/blob/abbf15de6fbb/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/deferred-findings.yaml)的DT129-VSNPRINTF-BOUNDARY已将充足4096缓冲以外的edge cases留给后继；158对此qualified ID追加限定证据，不关闭effective ABI或真实链接/输出未知，也不重报忽略write返回。

原C11b／EXEC-26用GPT-6.2 Sol＋Extra／高推理接收执行，完成158实际G04后STOP_WRITING，回C11 Pro独审及同轮后继，不进入159。成功首次发158时C11计6→7，8/10均未达，C10历史11不变；验收/补链/说明不重复计数。

完整本地P9、项目程序/脚本、预处理、编译链接、C/ASM、Graphviz、宿主信号夹具、QEMU、实机/真实IO均NOT_RUN。实际配置/ABI、生产调用者/映射/运行效果及后台型号/effort未独立认证。作者准备窗口/调用上限/查询问题与正式写分列，公开结果一提交一作者无重复；不认证私有全过程。主线文件/评论发表时G04仍PENDING，发表后实核通过才向Owner报PASS及STOP_WRITING，不倒改发表时点。
