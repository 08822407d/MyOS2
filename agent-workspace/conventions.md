# 写入公约与共享词汇表（每个 agent 会话开工前必读，违反=产出作废）

## §1 写入规则（并行安全的根基）

1. **身份**：一个对话=一个任务号（Owner 启动提示词里给出）。你只为这一个任务号工作。
2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。
3. **分支与提交**：从 master 新建分支 `agent/<你的任务号>`，提交信息以 `<你的任务号>: ` 开头，完成后向 master 开 Pull Request（标题同前缀）。禁止 force-push、禁止直接推 master。
4. **降级路径**：若你无法创建分支/提交/开 PR（连接器只读等情况），则在对话中**逐文件完整输出**全部交付物，每个文件先写一行目标路径，由 Owner 手工入库。宁可降级，不可写错位置。
5. **必交 MANIFEST**：你的 results 目录里必须有 `MANIFEST.md`：逐文件一行说明＋覆盖状态（做完了什么、明确没做什么）＋后续 agent 该怎么消费。没做完不丢分，没写清楚才丢分。

## §2 产出格式

每个产出文件以 YAML 头开始：

```yaml
task_id: MYOS2-DR-0NN
produced_by: <模型/产品名>（如 ChatGPT Pro / GPT-5.x）
date: 2026-09-0X
base_snapshot: master @ 63f0785c
inputs_read: [实际读过的仓库路径列表]
status: draft | final
open_questions: [未解决的问题，可为空]
```

正文纪律：

- **三类标注**：源码里核实过的事实标 `[VERIFIED mykernel/xxx/yyy.c]`（尽量带文件路径）；从命名/结构推断的标 `[INFERRED]`；来自手册/外部资料的标 `[EXTERNAL 出处]`。三者不得混写。
- **图与矩阵优先机器可读**：依赖图交 DOT＋YAML 双格式；矩阵/清单交 YAML＋Markdown 表双格式。
- **语言**：说明文字用中文，标识符/代码/节点 ID 用英文。
- **修订不覆盖**：要改自己已提交的结论，新增 `-v2` 文件并在头部写 `supersedes: <旧文件名>`。

## §3 共享词汇表（节点 ID；002/003/007 号任务的 YAML 必须用它，其他任务引用时也用它）

子系统 ID（对应 mykernel/ 下目录，另加两个仓库级条目）：

```
arch.x86_64  arch.aarch64  boot.uefi(=myloader)  user.initramfs(=myinitramfs)
entry  cpu  init  namespace  printk  debug  klib
mm.early  mm.page_alloc  mm.kmalloc  mm.vm_map  mm.fault  mm.highmem  mm.misc
sched.task  sched.forkexec  sched.runqueue  sched.scheduler  sched.misc
lock.atomic  lock.spinlock  lock.semaphore  lock.futex  lock.lockdep  ipc.signal
kactive.softirq  kactive.workqueue  kactive.swait  kactive.completion
time.systick  time.ktime  time.clocksource  time.timekeeping  time.timer  time.adjtime
fs.vfs  fs.fat  block  device  drivers.ata  drivers.base  drivers.char  drivers.pci  drivers.rtc
lib.digit  lib.idr  lib.list  lib.printf  lib.string
```

能力节点 ID：`<子系统ID>.<能力短语>`，小写下划线，如 `mm.page_alloc.buddy_system`、`sched.scheduler.smp_load_balance`、`fs.vfs.mount_table`。新造节点自行命名即可，但前缀必须落在上表。

## §4 事实纪律

- 以 master @ 63f0785c 为分析基线；仓库另有多个主题分支（见 repo-map.md §4），你无需读它们，但下结论时不要断言"仓库中不存在 X"，只说"master 快照中未见 X"。
- `bugs_record.md`、`todo.txt`、`changelog.md`、`documents/` 是历史材料，可引用但不可当作现状依据。
- 不确定就写进 open_questions，禁止编造：拿不到的数据标 `not_measurable`，不要估一个数糊弄。
