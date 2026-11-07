---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PID-LIFETIME-11
record_type: pid_lifetime_static_evidence_and_execution_record
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: MYOS2-A-C03a 代码分析执行
produced_by: "ChatGPT普通对话；Owner指定GPT-6 / Extra；backend身份不可独立认证"
model_per_owner: GPT-6
effort_per_owner: Extra
date: "2026-10-31"
read_channel: connector
base_snapshot: "source=time @ a039d9803ade；original_execution=848e2acdf7e1；instruction=69523af0507e"
task_contract: "lead/MYOS2-LEAD-002/29-user-pid-lifetime-contract.md @ 04fc4f26ba3d"
target_prior_review: CORE-USER-SIGNAL-SEND-WAKE-10-REVIEW-001
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
prior_sr10_01_mainline_status: RETURN
runtime_evidence: NOT_RUN
kernel_change_made: false
owner_decisions_requested_now: []
open_questions:
  - "用户fork条件与所有内核调用者不等价；非初始namespace和全并发时序未穷尽"
  - "退出参数未赋值、错误namespace实参及IDR空指针预留的运行后果均未测试"
  - "SR10-01仅P00执行方accept，是否关闭由C03主线验收决定"
---

# 第11批证据链、方法、自查与交付边界

## 一、对象基线和已读取的实际范围

所有PID与信号图更正的技术引文均按 **time @ a039d9803ade** 固定GitHub对象读取；原第10批图与facts只读 **848e2acdf7e1**；29号任务书固定 **04fc4f26ba3d**，10主线回执按指令基线 **69523af0507e** 读取。主线合法续写其lead不视为内核/结果源变化，执行方不写lead。

本批已实际读取/定点查验的直接源范围：`mykernel/scripts/options_flags.cmake`；`mykernel/sched/syscall.c`；`mykernel/sched/forkexec/fork.c` 的 `copy_process`/ `kernel_clone`/PID挂接和错误标签，`exit.c` 的 `exit_notify`/ `do_exit`/ `do_group_exit`；`mykernel/sched/task/pid.c`、`pid.h`、`pid_api.h`、`pid_const.h`、`pid_types.h`、`pid_namespace.h`、`pid_namespace.c`；`mykernel/lib/idr/idr.c`、`idr.h`、`idr_const.h`；`mykernel/init/init_task.c`、`main.c`；`mykernel/sched/scheduler/scheduler_core.c` 的 `wake_up_new_task` 和 `do_task_dead`、`misc/sched_misc_api.h`；`mykernel/lock_IPC/signal/signal.c` 与 `signal_macro.h` 的当前kill查询及P00定点调用，必要时读了API/类型定义。

历史消费仅定点读取05的C2/C6、UV-14、UV-15及05 MANIFEST；第10批的facts、DOT、报告身份和主线回执。**未重新执行08/09/10的批量查询、检查器或其审计全流程**。GitHub默认分支的搜索仅用于找到源码真实路径，所有实体技术判断回固定time对象；全树搜索命中只作为导航，不作全局不存在性证明。

配置前置阅读：`mykernel/scripts/options_flags.cmake` 的编译选项文本。该文件引用已有的`CMAKE_C_FLAGS`，本轮未进行预处理或验证外部启动参数，不能仅从缺少显式`-D`推断所有实际构建情况。

## 二、P00：SR10-01回源和26条旧图边逐项校验

先新增的 `signal10-graph-disposition.md` 对原10号图/引用作**执行方accept，主线RETURN仍待复验**：

| 旧边/位置 | 回证处理 | 不应作出的推断 |
|---|---|---|
| SW10-E01 | 原A03只覆盖入口；补实际`sys_kill→prepare_kill_siginfo`调用A03b | 附近存在函数名就表示引用了调用行 |
| SW10-E12 | A18支持SIGKILL/PF_KTHREAD分流，A19才给实际`__sigqueue_alloc`调用，A20界定未初始化实参 | 跳过条件本身就是调用证据或分配必成功 |
| SW10-E16 | A34是`signal_wake_up`宏调用，必须并读A35宏映射到`signal_wake_up_state` | 当前kill已接入上游`complete_signal`（实际调用被注释） |
| SW10-E21 | 以本批限定中间节点SW11-GN01和边SW11-GE01/GE02，将`dequeue_signal→__dequeue_signal→collect_signal`拆开；后者受`sig!=0`门限 | 原两端可当直接调用 |
| SW10-E24/头 | data恢复为**消费方→提供方**；链表插入行为进入effects而不是自造反向data | 数据依赖可随写操作反向定义 |

P00附八段源码逐字短引文（`lock_IPC/syscall.c`、`signal/signal.c`、`signal_macro.h`）与局部YAML/DOT覆盖片段。对旧图**SW10-E01～E26全部26条**按真实调用、条件、宏、数据依赖与注释边进行局部同类消费审查。未重新证实旧52条锚点的所有原技术推论，亦不更改旧文件与旧编号。新图`pid-lifecycle.dot`完全独立用于本批PID链，不将尚未被主线核收的旧E21作为直接证据。

## 三、P01–P03可复核范围和本轮真实核验

| 项目 | 实际方法及结果 | 明确保留的局限 |
|---|---|---|
| 固定源码强制引文 | facts的**125段引文/17个来源文件**，通过GitHub连接器按固定`time`回读全文后依行范围逐字切片比较：第一次8文件/80段、第二次9文件/45段，**125/125文本匹配**，每段1～5行 | 比较确证路径与行文本；符号边界仍需人工读函数上下文，非运行语义证明 |
| 叙述引文 | `pid-lifetime.md` 的**43段[VERIFIED]**逐项将所称路径、符号及引文与facts的锚点精确比较，43/43匹配 | 引用相同不意味着推断必然成立；反向测试另列 |
| 本批结构索引 | facts中的**47个节点、54条关系、16种生命周期状态、11类所有权接口、7条effects**；核所有54边端点及锚点存在 | 计数不是54条独立语义证明；分支状态`conditional/latent/commented`各有区别 |
| DOT关系 | `pid-lifecycle.dot` 的47节点/54边ID集合与facts精确相同；图中未连通的`detach_pid/free_pid`以潜伏/条件边标注 | 未运行Graphviz渲染器；仅做ID/端点/状态类型审查 |
| 延期问题 | 六项本批新问题`PID11-F01～F06`，三项针对05::UV-14、05::UV-15和10::SW10-F03的证据追加，另有一项05 C6措辞`PID11-ER01`候选 | 旧问题状态原样保留，候选尚未变更05主线结论 |
| 旧10图 | 定点5处修正和全部26条边局部检查，8条P00源码短引文 | SR10-01原主线RETURN不自动关闭 |

**调用方向约束。** call边只认真实函数调用及另列清楚的宏展开；经中间层不能“压成直接call”；data始终是使用方→数据提供方，赋值/删除等改变另归effects；init_order是后使用者→前提初始化。参照固定代码再次检查`alloc_pid`预留、`idr_replace`发布、`attach_pid`挂链分别属三种不同状态，未将`wake_up_new_task`存在等价于真实调度成功。文档中引用`get_pid`时区分`pid_s`引用与`task_s`引用。

**实际文本核对工具：** GitHub连接器`fetch_file`、`get_pr_info`、`compare_commits`和`functions.exec`内有限字符串/正则与集合检查。一次试图并行读取17个源码文件、全文核验时触及代码编排单次工具调用数量限制，未写出新的结果；随后在**两个独立有界批次**中完成80+45段逐字源码核对，成功结果上表可回证。没有借中断冒称先前核对成功。Graphviz/YAML完整语法解析、仓库专属验证器均NOT_RUN。

## 四、主动反证与源码情形

1. **“数字预留＝PID对象可见”：不成立。** `alloc_pid`调用`idr_alloc_cyclic(...,NULL,...)`；本树`idr_alloc_u32`仅判断`NULL`后写入相同`NULL`。`idr_replace`才把对象指针放进IDR。参见PID11-A13/A66/A67/A17/A70。
2. **“PID可见＝已有task”：不成立。** `alloc_pid`将`pid->tasks[type]`初始化为空表，后来`copy_process`在条件门后才调用`attach_pid`。PID11-A15/A17/A56/A58/A59。
3. **“find/get一律持有task引用”：不成立。** `find_vpid`、`pid_task`只获取裸指针；`get_pid`对pid计数，`get_pid_task`及`find_get_task_by_vpid`的`get_task_struct`为注释。PID11-A25/A32/A43/A36/A34。
4. **“exit执行就从IDR删除”：不能成立为当前活动链。** `do_exit`及`exit_notify`源码没有活动`detach_pid/free_pid`连接；这些函数定义存在及`release_task`声明，不能填补缺失调用。PID11-A83/A84/A30/A22/A88。
5. **“PID永不重用”：未证。** 假设有别的合法触发`free_pid`，`idr_remove`仍可使槽重新为NULL；本批只核当前exit直接链，未穷尽所有函数指针/内部回收路径，不能做全局不存在性证明。PID11-A22/A71。
6. **“退出/重挂必完成且结果固定”：不能成立。** `group_dead`未赋值即传参；有孩子时`exit_notify`把值为0的`PIDTYPE_PID`用作`find_task_by_pid_ns`的`pid_ns_s*`实参；`sys_wait4`函数体为空，不可凭名字推定回收完成。PID11-A79/A80/A83/A76/A33/A05。
7. **“缓存/IDR方法都与标准Linux一致”：不成立。** 本树`idr_alloc_u32`固定扫描32768个槽且未按传入max限制，`idr_find`以数字直接访问数组；超范围输入/并发/错误指针的语言层结果必须保持有前提，不能假定Linux标准的安全边界。PID11-A66/A69/A72/A73/A74。

上述源文事实与条件推断均未经过预处理/编译/运行。禁止把`ERR_PTR`返回、`NULL`返回与已经发生的硬件异常互换；也不将潜伏非初始namespace问题套到flags为0的普通用户fork必经路径。

## 五、写前与交付回读记录分层

- **首次写前：** PR17仍open/Draft、执行头`848e2acdf7e1`、time`a039d9803ade`、master`e8e60cd19540`，只有PR16/17开放。PR16与本任务七件新目录无路径冲突，执行方仅获新目录写权。
- **P00最先写入：**`signal10-graph-disposition.md`。其后按有界取证顺序新增facts、DOT、可读说明、延期增量；facts在本次交付前仅在同一授权新目录做一次源边引文校正，固定旧件全留存。
- **本证据文件写入前已核：** 比较`848e2acdf7e1`到`3dbc70171311`为6个后继提交，只有上述**5件新目录A新增**，旧件净修改/删除为0；PR17仍open/Draft；time/master短号未变化，lead正常前进与执行方写区无关。
- **最终七件的远端比较/固定提交全文/当前分支blob回读以及PR评论：** 必须在MANIFEST发布后执行，**只在PR17新增的交付评论中记录最终提交、七件结果和核对结果**，不把未来回读当成本文件写作时已成功。

GitHub连接器创建文件接口没有本批持有的原子分支锁；每次以最近已读远端头为期望做写前检查，仍保留读/写之间并行变化的竞态限制。发现不明变化不得覆盖/回退/force修复。

## 六、严格未执行内容与交回条件

本批未运行内核、C/ASM、QEMU、预处理、构建、宿主夹具、外部libc/镜像、完整YAML/Graphviz解析、原08/09/10检查器、全PID并发/调度时序、硬件/ISA实验，也没有产生内核补丁、重启调度器、修复任何普通问题或新增模型/API调用。用户计算机与旧本地CC02进程状态并未核实。

P00执行方`accept`和P01–03文档`READY_FOR_MAINLINE_REVIEW`均不是C03的主线最终裁定：**SR10-01在C03未再次验收前仍维持RETURN**，05的文档勘误仍是候选。阶段2接收最高`PASS_PENDING_LOCAL`；10以往被保留的静态证据与09此前已验的界限继续有效。后续必须由C03验收与另行派工，C03a正式提交后停写，不启动第12批或变更已有内核/用户程序。
