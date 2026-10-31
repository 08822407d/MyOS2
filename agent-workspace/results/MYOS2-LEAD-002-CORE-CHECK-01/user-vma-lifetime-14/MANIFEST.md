---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-VMA-LIFETIME-14
record_type: bounded_vma_lifetime_delivery_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: "MYOS2-A-C03a 代码分析执行"
produced_by: "ChatGPT普通对话；Owner选用GPT-6/Extra，实际后端身份不可独立认证"
model_per_owner: GPT-6
effort_per_owner: Extra
date: "2026-10-31"
read_channel: connector
source_pin: "time @ a039d9803ade"
result_base: "2d67bea2fe59"
task_contract: "agent-workspace/lead/MYOS2-LEAD-002/32-user-vma-lifetime-contract.md @ b8ae7762a96e"
instruction_base: "bd25e19fd944"
required_review_record: CORE-USER-FILE-CACHE-13-REVIEW-001
prior_status: {file_cache13: PASS_PENDING_LOCAL_WITH_R13_SUPPLEMENTS, FC13_ER01_ER02: ACCEPTED_BOUNDED_QUALIFICATION}
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_correctness_verdict: NOT_ISSUED
runtime_evidence: NOT_RUN
kernel_change_made: false
old_results_changed: false
branch_or_pr_created: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
allowed_write_prefix: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-vma-lifetime-14/"
deliverables: [MANIFEST.md, vma-lifetime.md, facts-and-boundaries.yaml, vma-lifecycle.dot, deferred-findings.yaml, evidence.md]
open_questions:
  - "首VMA/NULL迭代器、临界地址与分割错误可能阻断本树预期区间行为"
  - "C/ASM实际入口、TLB/PTE失效、页和file物理回收、并发与硬件行为未验证"
---

# CORE-USER-VMA-LIFETIME-14 · 六件交付总入口

**本批L00–L04有界静态分析与源码取证已完成，状态仅为READY_FOR_MAINLINE_REVIEW。** 最终验收由MYOS2-A-C03独立执行；没有内核运行正确性结论或内核修复授权。之前的第13批和所有更早成果按固定执行基线全部冻结。

## 1. 六件文件（建议阅读顺序）

| 文件 | 内容 |
|---|---|
| `MANIFEST.md` | 身份、固定输入、L00–L04结果、自查及主线接收边界 |
| `vma-lifetime.md` | **先读**：真实sys_mmap/sys_munmap入口；VMA创建、文件引用；find/split/unlink与首VMA/边界情况；失败偏序；file/PTE/page/exit清理分层；主动反证 |
| `facts-and-boundaries.yaml` | **精确源证**：固定time 15个文件107条1–5行引文、45节点、58边、23类代表区间输入、13种生命周期状态、10项所有权、12条effects |
| `vma-lifecycle.dot` | 与facts的45节点、58边ID完全对应；call/data区别、活动/条件/注释或潜伏边显式区分，保留do_mmap和simple_vma_link中间层 |
| `deferred-findings.yaml` | 新增6项有条件问题，向旧FC13-F05、05::UV-15及05::UV-10追加3项证据；旧状态/编号不变 |
| `evidence.md` | 真实读取范围、107/107固定源引用和73/73正文引文比对、图与引用完整性、反证、NOT_RUN及最终回读方法 |

所有文件仅在`allowed_write_prefix`下新建；没有为旧13修订重新生成文件，也没有增加第七份工具/日志/勘误文件。

## 2. L00–L04已完成与未完成的边界

| 阶段 | 本次实质完成 | 限制 |
|---|---|---|
| L00 | 全读32任务书及13主线回执R13-S01/S02/S03；原18/28延期与分工持续适用；核PR16/17、master/time/lead/执行头及目录冲突，先读options_flags.cmake | 未重做25接收/12/13分析，也不将旧机械检查说成主线已独立重跑 |
| L01 | 固定编号11与sys_munmap→__vm_munmap实际入口；mmap调用中的vm_mmap_pgoff→do_mmap→simple_mmap_region完整中间层；新VMA创建、get_file/call_mmap/入链及专用创建失败释放 | 非完整syscall汇编、VMA地址分配策略或真实映射运行认证 |
| L02 | 查找严格前驱及首/相邻边界；__split_vma浅拷贝、条件get_file和vm_ops.open、两分割和失败副作用；__simple_vma_adjust对vm_pgoff的维度前提；摘链和map_count变化；23例输入矩阵 | 迭代器初始NULL的C前提、指针/列表稳定性、越界/溢出等都未运行，不把正常分支说成必达 |
| L03 | __vma_unlink_from_list与vm_area_free/fput/close、PTE/TLB/page/cache slot逐项区分；simple_unmap_region注释主体与限定调用者；do_exit的exit_mm注释，exec_mmap→mmput但__mmput的exit_mmap注释；dup_mmap的file get | 没有证明页表/TLB真实撤销、页面释放/内存泄漏率、全树所有资源回收路径 |
| L04 | 六件按授权写区提交；完成事实/图/引用机械核对、条件反证及普通问题归档 | 最终提交后GitHub compare与固定提交/分支双重回读以及PR17交付评论必须在本文件之后真实核验 |

## 3. 可验证的本轮校验事实

- **固定源码：** `facts-and-boundaries.yaml`107条行范围准确的短引文，来自15个`time @ a039d9803ade`文件；两批分别取8文件94段、7文件13段，**107/107逐字与范围匹配**。引文数不是独立可达性/运行通过数。
- **正文：** `vma-lifetime.md`的73个`[VERIFIED]`块，按路径、符号、行号及整个引用原文对照facts，**73/73一致**；不因此宣称运行语义认证。
- **图/状态：** facts/DOT的45节点、58边ID集合一致，全部边的from/to与锚点存在；23种输入、13状态、10所有权、12 effects的引用也存在。图中修复了最初的两个中间调用链误连，仅修本批facts/DOT。
- **问题：** 本批6项新发现及3项旧问题证据追加，均保留条件/反证/未知；旧第13批主线给定的R13-S01/S02/S03和ER01/ER02限定继续有效。
- **主动反证：** 摘链不等于vm_area_free；复制VMA不等于所有引用自动增加；0返回不认证PTE/TLB撤销；第二split失败未回滚第一split；首VMA不保证严格前驱；声明/注释释放接口不等于进程退出时真的调用。

## 4. 写入历史与最终远端回读分界

首次写前原PR17为open/Draft，执行头`2d67bea2fe59`、master=`e8e60cd19540`、time=`a039d9803ade`，开放PR仅16/17，新目录原先不存在且PR16无路径交集。先提交facts，再只修订本批facts以补齐do_mmap和simple_vma_link中间节点及vm_munmap宏别名，随后提交DOT、正文和延期发现、evidence。没有改动`2d67bea2fe59`及此前任何文件。

**MANIFEST发布前GitHub compare**从旧头至`6f6f6f0e3981`为7个连续后继提交、仅本批五件A新增，旧件零净修改/删除，PR17仍open/Draft。第六件本MANIFEST尚需实际提交；后续的**最终固定提交SHA、六件完整回读/Blob一致性、diff及PR交付评论只在实际操作完成后记入PR17评论**，不在本文件预测未来Git对象。

## 5. 禁止动作与交接

严格保持静态阶段2：未运行正式YAML/Graphviz解析、仓库旧检查器、预处理/编译C/ASM、内核/QEMU/硬件、外部libc/文件镜像、CPU地址转换实验或真实并发；无patch、内核/用户程序修改、额外模型/API、阶段3、合并/关闭PR、force-push和新分支。本执行方只写PR17指定六件和自己的交付评论；GitHub Contents API不是全分支原子锁，保留预读至写入的并行竞态限制。

C03请先读`vma-lifetime.md`的区间矩阵、失败副作用与三层结果区分，再用facts/DOT证据核交叠选择、真实split和unlink、file引用、页表/退出清理边界，最后独立compare六件新路径。**最高静态验收上限PASS_PENDING_LOCAL，不是内核正确性通过。** 本执行正式交付后停写，不自行开始第15批或实施任何修复。
