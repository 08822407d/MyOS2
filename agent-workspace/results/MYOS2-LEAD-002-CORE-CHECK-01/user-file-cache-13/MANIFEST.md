---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FILE-CACHE-13
record_type: bounded_file_cache_delivery_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: "MYOS2-A-C03a 代码分析执行"
produced_by: "ChatGPT普通对话，Owner指定GPT-6 / Extra；实际后端身份不可独立认证"
model_per_owner: GPT-6
effort_per_owner: Extra
date: "2026-10-31"
read_channel: connector
source_pin: "time @ a039d9803ade"
result_base: "16b77f23003f"
instruction_ref: "agent-workspace/lead/MYOS2-LEAD-002/31-user-file-cache-contract.md @ 58b0bf268726"
instruction_base: "64c80dc4912b"
required_review: CORE-USER-MM-FAULT-12-REVIEW-001
prior_status: {SR11-01: CLOSED_IN_BOUNDED_REVIEW, fault12: PASS_PENDING_LOCAL_WITH_REVIEWER_SUPPLEMENTS}
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
kernel_change_made: false
old_results_changed: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
allowed_write_prefix: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-file-cache-13/"
deliverables: [MANIFEST.md, file-cache.md, facts-and-boundaries.yaml, cache-paths.dot, deferred-findings.yaml, evidence.md]
open_questions:
  - "全部inode/map/page回收、初始化分支与并发竞争未穷尽"
  - "真实文件数据/页缓存就绪、硬件缺页和读错误后果均未经运行测试"
  - "本批文档候选FC13-ER01/ER02和新发现交C03裁定，不自行改旧05条目"
---

# CORE-USER-FILE-CACHE-13 · 六件交付总入口

第13批**C00–C04已完成有界静态分析并交付**。证据全部来自固定 `time @ a039d9803ade`；本包仅申请`READY_FOR_MAINLINE_REVIEW`，不是内核运行正确性PASS。请C03按31号合同、12主线回执、此总入口及下表审查。

## 一、六件新文件及建议消费顺序

| 文件 | 核查内容 |
|---|---|
| `MANIFEST.md` | 任务身份、授权写区、C00–C04、成果指标、验收和停写边界 |
| `file-cache.md` | **先读：**缓存容器创建/容量/索引、指针发布先于读、命中/回退/私有写区别、FAT32实际读绑定、对象引用与释放、反证及代表输入 |
| `facts-and-boundaries.yaml` | 88段固定源码短引文（12个来源文件）、42节点、46边、17状态、10所有权接口、16类输入和11类effects，所有不确定前提显式限界 |
| `cache-paths.dot` | 与facts的42节点/46边ID一致；区分实际调用、条件、FOP/VM回调绑定及数据对象引用，改写副作用单独入effects |
| `deferred-findings.yaml` | 新增6个有条件发现、向05::UV-06/07/08/10追加4组证据、两项旧05表述的文档勘误候选，不改旧项状态 |
| `evidence.md` | 实际读取范围、按固定源码逐字回读、图与正文引用自查、前后置GitHub核验分界、运行NOT_RUN |

严格只在本包`allowed_write_prefix`下写上述六件，不恢复CC02权限，不写PR16/lead/master/time或第12批及以前旧文件。

## 二、C00–C04执行结果与限界

| 阶段 | 已有证据 | 不应推断 |
|---|---|---|
| C00 固定输入 | 完整读取31号任务书、12主线回执与R12-S01/S02、18/28约束、05 C4/C5和相关旧项；检查PR17、master/time/执行head及PR16路径不冲突；先读options_flags.cmake | 不重做旧11/12，不用master源码或未实际执行CMake变量推定编译配置 |
| C01 容量/索引 | `inode.i_data→i_mapping→file.f_mapping`真实指针来源；`__lookup_slow`按`ALIGN(i_size,PAGE_SIZE)>>PAGE_SHIFT`设置nrpages，正页数才kzalloc数组；普通read、map_page、fault索引路径和代表越界前提 | nrpages不是已有效缓存页数量；dentry命中、零长度、kzalloc失败、i_size变化不能默认初始化/修复成功 |
| C02 填页/错误 | 普通read和simple_filemap_fault均先写槽指针后调用`f_op->read`，丢弃读结果；缓存hit可直接memcpy或写PTE；私有文件写fault可以直接`__do_fault`遇已占槽BUG_ON；FAT32 fops/read签名和被核绑定齐全 | 非NULL不等于读完；零/负/短read不等于文件数据完整；BUG_ON条件不等于硬件已崩溃，返回0不等于CPU已恢复 |
| C03 引用/释放 | `get_file/fput`、`iput`、`get_page/put_page/folio_put`源码对照，区分file、inode、mapping、slot页、PTE、cow页；所读fput只释放file且未遍历cache数组 | 不能由对象指针/页表映射推定持有page引用，也不能由本链缺释放断言全系统永不回收 |
| C04 校验/交付 | facts 88短引文由12固定time文件分两次40+48回读全部逐字匹配；正文70段[VERIFIED]均匹配路径、符号、行号和原引文；42节点/46边集合一致，引用均可解析；问题入库 | 正式YAML/Graphviz解析、编译、运行、内核/QEMU与真实读写并发均NOT_RUN |

## 三、实际执行的机械核对与主动反证

- **引文精度**：facts每条1–5行，固定time逐字行范围比较**88/88一致**；正文70段[VERIFIED]均同facts原文、路径和行号一致；“88段”是证据切片数，不是88项独立技术正确性证明。
- **图与结构**：facts与DOT都列42节点、46边，ID集合完全一致；全部边的from/to和锚点可解析，17个状态、10条所有权、16个代表输入及11条effects的证据引用均存在。
- **审阅过程修正**：一次源引用候选因超过5行先行拒绝写入；一次图ID检查因status内`FAT32`数字而漏匹配，修正匹配器后消除误报。之后仅在本批新文件内补正了`do_read_fault→finish_fault`的直接证据、COW错误归还folio的条件边，并给COW→finish_fault追加独立条件边；同步DOT，**没有改旧件**。
- **反证与未知**：缓存槽非NULL不证数据已填好；后端读返回0不代表完整页；cache-hit不保证下标有效；私有文件写fault不总先经过map_single_page；file对象的引用不等于page引用；资源释放函数存在不等于从本路径实际调用。所有结论按函数体和必要前提，未知与条件不得升级为已复现故障。

## 四、远端差异及停写约束

首次写前PR17为Open/Draft且执行head=`16b77f23003f`，master=`e8e60cd19540`、time=`a039d9803ade`，仅PR16/17开放且本新目录不存在、路径无重叠。截止本清单发布前，执行head已到`b33c4bd87a70`，净变化仅本目录**5个新增文件，旧件0修改/删除**；其中第五件`evidence.md`在工具输出中断前实际成功提交，后经GitHub重新回读其提交与完整内容，未盲目重写。

**最终提交和六件回读、compare、PR17交付评论尚需在本MANIFEST提交后真实执行，后置结果只记录PR17交付评论。** GitHub Contents API不能保证读/写之间的全局原子锁，遇不明头变化不能直接覆盖或force。正式交付后本包也冻结，执行方停写、C03主线独立验收后派下一批。

## 五、禁止动作与交回

本轮没有预处理/编译/运行C/ASM、内核、QEMU、外部libc、硬件、挂载镜像、仓库旧检查器、正式YAML/Graphviz解析/渲染；没有内核补丁、修复、删除或合并PR、新建分支或阶段3行动。普通发现先入库不要求Owner现在处理。只交C03按有限静态范围审核，**最高PASS_PENDING_LOCAL**；并保留旧RS-01、GAP-U04硬件/运行部分、GAP-U10原有缺口。
