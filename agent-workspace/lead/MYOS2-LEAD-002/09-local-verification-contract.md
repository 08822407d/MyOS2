---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
verification_packet_id: MYOS2-LEAD-002-CORE-CHECK-01
record_type: bounded_local_verification_contract
evidence_class: "基于已读源码的验证设计；未执行"
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-09-24
base_snapshot: "核查源码=time（分支名）；本包=agent/MYOS2-LEAD-002（分支名）"
inputs_read:
  - agent-workspace/lead/MYOS2-LEAD-002/07-scheduler-wakeup-timer-audit.md
  - agent-workspace/lead/MYOS2-LEAD-002/07-core-audit-map.yaml
  - "上述核查件实际读取的 time 源码"
  - "Mnemosyne 的跨对话执行意图、文件交付与权限边界指导"
status: READY_NOT_EXECUTED
execute_in: "本地 Claude Code 或等价本地编程执行面；优先现有 MYOS2-LEAD-001 会话，不是十八个旧研究对话"
execution_disposition: RUN_NOW_OPTIONAL_BY_OWNER_LAUNCH
actual_execution_started: false
remote_write_authorized: false
kernel_modification_authorized: false
vm_execution_authorized: false
readme_merge_prerequisite: false
acceptance_ceiling: PASS_PENDING_LOCAL
local_validation: "全部待本地；主线没有运行本包中的任何步骤。"
open_questions:
  - "本地可用工具与实际构建产物未知；缺工具就记录，不自动安装。"
  - "宿主机夹具不能替代真实内核调度、IRQ、TLB 或 SMP 的验证。"
---

# 一次完成源码证据、局部反例与交付一致性核验

**目标：给已经查清的核心问题补上独立机械见证，而不是再做一轮大规模研究或直接修内核。** 本包不要求先合并新的 PR；直接读取 `agent/MYOS2-LEAD-002` 上的包与分析件即可。Owner 向本地执行面发送下面的启动指令才构成本次本地执行授权；主线创建文件本身不表示已启动、已消耗外部额度或获得修复权限。

## 1. 操作流程（现在可选执行）

在原来处理 MyOS2 的本地 Claude Code 会话中发送下面一段；没有该会话时，可用能够读取该 public 仓库的本地编程会话。无需新开 ChatGPT 深度研究，也不需要把十八份聊天逐个附上。

```text
执行 MYOS2-LEAD-002-CORE-CHECK-01 的只读核验。
仓库：08822407d/MyOS2。
先读取分支 agent/MYOS2-LEAD-002 的：
agent-workspace/lead/MYOS2-LEAD-002/09-local-verification-contract.md
再按该文件读取 07 报告和 07 YAML；源码只以 time 分支为准。
本次允许在独立临时目录做源码/引文/结构校验，以及使用现有工具编译运行隔离的宿主机最小夹具；不得修改任何现有 MyOS2 工作树、分支或 GitHub 状态，不运行内核、QEMU、安装/分区/写盘脚本，不安装工具，不修复业务代码。
按 V00-V14 一次完成可做部分。工具缺失或夹具无法保真时记录未完成，不以手写模型冒充原函数运行。发现与主线不同的结果，保留双方证据，不迎合预期。
最后只交一份 MYOS2-LEAD-002-CORE-CHECK-01-result.md，包含逐例结果、执行命令和必要输出、保真限制、失败/阻断项及复核建议。给出该真实文件的位置，供我上传回 MYOS2-A-C02 内核分析主线。不要提交、开PR或合并。
```

完整任务入口：
`https://github.com/08822407d/MyOS2/blob/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/09-local-verification-contract.md`

完成后，Owner 只需把结果 Markdown 附件带回本主线；无需手动摘要、拼接日志或逐条回答。正文应收齐本次判断必需的日志片段；额外原始日志可留本地并在报告中给准确路径。不要另外再制造一份仅用于搬运相同回复的重复研究报告。本任务需要的是这一份指定结果文件，不要求整个本地会话转录。

## 2. 执行安全与输入一致性

[INFERRED] 下列为本次验证设计，不改变 MyOS2 的执行源或既有架构：

- 先读取本包、`07-scheduler-wakeup-timer-audit.md`、`07-core-audit-map.yaml`；相同分支中的 `05/06` 不是源码事实替代物，不要求全文重读。
- 源码读取 time。可以在新建临时目录获取 public 仓库的独立只读研究副本；不要 checkout、reset、clean、stash 或修改用户已有工作树，也不修改已有仓库 `.git`。记录实际分支与从工具复制的短标识，不写完整提交标识。
- 临时文件只能落在本次新建的隔离目录；先记录目录绝对路径，最后不自动删除该目录。已有文件、用户测试盘、固件变量文件和运行进程都不作为可覆盖目标。
- 不执行原 `.vscode-kdbg/run-qemu-gdb-myos2.sh`、make_install、part_vdisk、phys_nvme_install 或同类脚本；不启动 QEMU、VMware 或内核。不得为了复现超时自行占用用户真实磁盘镜像。
- 只使用已安装工具。没有 YAML 库、编译器、ELF 工具等则记录相应条目 BLOCKED/NOT_RUN，不安装、不修改系统、不凭名称猜命令存在。
- 对可能循环或崩溃的宿主机夹具使用独立进程、明确步骤上限和现有超时控制；无可用隔离/限制时不运行该项。
- 如果 time 在取数过程中发生变化，重取受影响输入并说明；不能把两个版本片段拼成一个见证。

## 3. 结果类型必须分开

每项使用以下证据类型之一或多个，分别记录：

| 类型 | 含义 | 不证明什么 |
|---|---|---|
| SOURCE_MATCH | 对应版本原文、符号体和引文已由机械/独立读取比对 | 不证明行为正确 |
| STATIC_COUNTEREXAMPLE | 明确前提下，逐步执行语义推导成立 | 不等于运行复现 |
| HOST_ORIGINAL_SLICE | 保留原函数/宏的隔离宿主机片段实际执行，所有替换依赖已列出 | 不等于真实内核的 IRQ/抢占/SMP 行为 |
| MODEL_ONLY | 为说明问题建立的另写模型 | 不能标作原函数测试通过或原内核 bug 复现 |
| EXISTING_ELF_EVIDENCE | 已有产物的身份、符号等可核对 | 不证明这个产物运行过，且必须说明与所读源码的关联是否成立 |
| NOT_RUN / BLOCKED | 本轮无法可靠完成 | 不填成 PASS，也不补猜数值 |

测试结果采用 OBSERVED_AS_PREDICTED、COUNTEREVIDENCE、NO_FAILURE_IN_SCOPE、BLOCKED、NOT_RUN。它们不是整个 MyOS2 的 PASS。对每个结果同时给出范围、前提、实际值和不能外推的部分。

## 4. 十五个条目：先独立比对，再运行可保真的局部夹具

### V00｜输入、保护边界与引文

核对 07 报告 A01-A47：路径存在、逐字引文连续、长度 1-5 行、属于标记符号定义体或明确的宏/配置定义；对照 time/master 两条金丝雀。特别检查 asm 引文中的真实制表符与反斜杠转义，不能仅靠屏幕相似判等。

确认本次只读既有工作树，写入仅发生在本次临时目录。记录实际命令、退出状态和比对范围。不能因为报告写了“47”就直接填 47 个通过。

### V01｜结构与引用完整性

用现有解析器读取 07 YAML 与本轮 MANIFEST，核对 CA-01 至 CA-07、A01 至 A47、V00 至 V14 的引用，无重复/悬空 ID；本包引用文件确实存在。其内容是限定核查，不应含全量 002R/003R/007R 已完成的状态。

### V02｜swait 单等待者的链接与 count

对应 A21-A26。空头 count=0；准备一个合法 waiter 后记录链接和 count；在不并发、唤醒依赖可控正常返回的夹具中执行原摘链路径；再执行原 finish 路径。

主线预测：摘链后链接为空而 count 保持 1，finish 因节点已自指不补减。记录每步真实值。替换的 current、锁、try_to_wake_up 必须列出；若只能模型演示，标 MODEL_ONLY。

### V03｜swait 二次通知与全唤醒

承接 V02，验证空链接但 count 非零时的第二次唤醒，以及两个 waiter 的 `swake_up_all_locked`。不要故意无限运行或任由 anchor 被当作有效对象继续访问；用步骤上限、地址合法性检查和隔离进程捕捉到违背不变量即停止。

目标不变量：节点数与 header count 一致，空表不产生 waiter 容器。报告实际最早偏离处；不得用插入修复后的函数代替待测原函数。

### V04｜非 current 唤醒：副作用与返回值

对应 A02-A08。目标任务初始为 TASK_NORMAL 中可匹配的睡眠状态，尚不在自有队列；核对活动 set_task_cpu 路径、最终状态/成员关系/计数与正常返回值。

主线预测：可产生入队及 TASK_RUNNING 副作用而返回 0。核验必须保留自有 `running_lhdr`，不能仅测试上游空 enqueue_task。

### V05｜不匹配状态掩码

给一个不属于传入 state mask 的合法任务状态，观察函数是否仍变更状态/队列。先抄录现有函数注释的状态筛选合同，再对照实际代码；若认为该合同不适用于 MyOS2，要单列为设计冲突，不静默把预期改成现状。

### V06｜重复唤醒与重复队列成员

同一个未运行目标任务连续唤醒两次。在串行夹具中记录 `list_header_contains` 的防重结果，期望不出现重复链表节点或计数增长两次。没有失败也需如实记录，不能为了证实报告把全部条目填失败。并发情形单列未测。

### V07｜新任务与 CPU 选择

核对 wake_up_new_task 的 CPU 0 选择、set_task_cpu 的目标队列与 task CPU 元数据是否一起维护。存在 init 中的赋值不等于后续迁移已经同步。当前只做读码/隔离值检查，不要求启动 AP；记录哪里有赋值、哪里只返回输入 CPU。

### V08｜非 runnable current 与空队列

对应 A33/A34。分别检查：current 可运行且空队列；current 阻塞且空队列；current 阻塞且有一个可运行替代项。记录默认返回对象、rq->curr、链表状态。

主线预测的反例前提是第二种组合。独立检查 init_idle 和重新入队约定是否保证该组合不可达；若有证明，报告 COUNTEREVIDENCE，而不是只跑预先构造的不合法状态。

### V09｜有限 timeout 的循环进展

对应 A15-A18。分别选 0、有限正值 1/5、负值；核对 schedule 调用次数、返回量与任务状态。对 msleep 只做有界迭代见证，不调用会无限循环的原函数而不设终止措施。

有限正值的预测是每次正常返回原 T；不要把 `__mod_timer` 或删除函数未能保真时的模型输出称作原内核运行。记录导致有限等待无法完成的独立条件，不只看 schedule 一行。

### V10｜无限 completion 与先通知后等待

对应 A19/A20/A40/A41。区分 MAX_SCHEDULE_TIMEOUT 的 schedule 分支与普通有限路径；done 预先为 1 时检查等待快路径是否消费一次通知而不必睡眠。该例用于避免错误结论“所有 completion 都不工作”。

### V11｜先等待后通知与对象生命周期

对应 A21-A26/A42。验证等待项先入队的时序，结合 V02 的 count；记录任务状态、waiter 链接和通知消费。需要真实上下文切换而夹具无法表达时，标运行未完成，保留静态序列。

不得将“等待函数返回”解释成异步生产者已经停止；本包不测试或修改完整取消协议。

### V12｜加法判负原语

对应 A38。避免有符号溢出，至少测试 `(初值, 增量)=(-1,1)、(1,2)、(2,-1)`，同时观测结果值与返回符号位。主线预测实际 sub 与注释 add 不同。若运行原 asm 片段，保留约束/类型、记录编译命令；若只做算术模型则不能冒称 asm 执行。

### V13｜trylock 成功后的所有权

对应 A39。初始化未锁对象，执行一次原 trylock，记录返回值与票据/val 是否改变；未释放就再次尝试，验证非递归获取合同。无需 SMP 就能检查“成功未改变所有权”的局部现象；这不替代并发压力测试，也不证明阻塞 arch_spin_lock 同样错误。

### V14｜jiffies 别名与单事件增量

对应 A28-A31。优先独立读源码确认两次增量与链接赋值。若已有、身份可说明的内核 ELF，可用现有工具检查 jiffies/jiffies_64 的地址与大小；无法说明产物来源则只报其观察，不用于认证所读 time 版本。

可以做隔离宿主模型解释两次别名增量，但标 MODEL_ONLY。**本包不授权构建整个内核、不启动 HPET/QEMU，不测墙钟倍率。** 没有安全真实事件证据时，保留真实时基为 pending_local。

## 5. 固定返回内容

文件名：`MYOS2-LEAD-002-CORE-CHECK-01-result.md`。文件顶部包含执行者实际显示/报告的模型选择，不冒用本包作者；`model_per_owner: gpt6` 仅保留为本主线历史归属时，必须注明它不是此次本地模型的认证。

至少包含：

1. 实际输入分支、读取时间、从工具复制的短标识；与主线取样不一致的文件；使用/缺失工具。
2. V00-V14 每项的证据类型、结果、初始条件、实际值、命令/关键输出及不适用范围；未跑项有原因。
3. 每个夹具的原函数保留范围、所有 stub/抽取改动、是否仅模型；不能以“编译通过”替代核心断言结果。
4. 对 CA-01 至 CA-07 的支持、反证或无法判断；特别给出是否存在遗漏的活动入队/计数维护/到期路径。
5. 现有仓库与原测试盘未修改的核查范围；临时文件真实位置；没有远程提交/PR/合并的动作记录。
6. 下一步建议：只核验成功且目标明确的部分才可进入后续修复设计；没有自动修改权限。

没有全部跑完也直接交出已取得证据，禁止把缺少工具、未确认产物或未闭合符号掩盖为 PASS。报告本身必须是实际存在的文件，不返回虚构 sandbox 链接。

## 6. 为什么本地是下一道真实门

当前主线已从原文核查推进到静态反例和完整验证规格。仅继续由同一对话重述这些源码，不能增加机械执行、编译约束或运行事实的证据等级。这个门与 PR 是否合并无关，也与十八个旧对话是否保持活动无关。

其余研究资料仍有待办；不声称整个阶段 2 已完成。本包只把当前最有价值的调度/等待链推进到可独立验证的位置。模型能力估计为 NEXT_TIER_SUFFICIENT_CANDIDATE：有冻结输入、具体条目、明确失败与返回标准；遇到新架构/权限取舍或相反证据，交回主线，不自行扩写方案。
