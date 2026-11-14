---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-VMA-LIFETIME-14
record_type: bounded_vma_static_evidence_and_delivery_check
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
produced_by: "ChatGPT普通对话；Owner指定GPT-6/Extra，实际backend未独立认证"
date: "2026-10-31"
read_channel: connector
source_pin: "time @ a039d9803ade"
results_base: "2d67bea2fe59"
instruction_ref: "agent-workspace/lead/MYOS2-LEAD-002/32-user-vma-lifetime-contract.md @ b8ae7762a96e"
required_review_record: CORE-USER-FILE-CACHE-13-REVIEW-001
prior_acceptance: "file_cache13 PASS_PENDING_LOCAL, must consume R13-S01/S02/S03 and ER01/ER02 bounded qualifications"
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
kernel_change_made: false
owner_decisions_requested_now: []
final_remote_readback: "After MANIFEST commit, actual six-file fixed-commit/branch comparison and PR17 comment must be checked."
---

# L04：实际源文、结构化证据、反证与可复核边界

## 一、固定输入与有界阅读

本任务完整读取**32号固定任务书**（`b8ae7762a96e`）及13主线回执`CORE-USER-FILE-CACHE-13-REVIEW-001`（指令基线`bd25e19fd944`）。固定源码仅以`time @ a039d9803ade`为事实来源；第13批与历史成果固定在`2d67bea2fe59`。依主线回执，13已在受限范围内通过`PASS_PENDING_LOCAL`，R13-S01补O_TRUNC的真实f_mode门、S02对FC13-E38的绑定方向作替代消费、S03区分源缓存页put与private cow页put；本轮不重做旧13，不改旧件或重新认证其88/70引文。

首先阅读`mykernel/scripts/options_flags.cmake`配置文本：x86-64标志与已有CMAKE_C_FLAGS的拼接不等于预处理后全部定义已确认。执行写前读取PR16/17、直接master/time/lead/执行head、两页开放PR与PR16/17变化路径；PR17当时open/Draft，执行head=`2d67bea2fe59`，master=`e8e60cd19540`，time=`a039d9803ade`，仅PR16/17开放，本批新目录不存在、无写区重叠。lead按C03独立合法更新不等于执行源漂移。

### 此批107条引文来自15个确实读取的固定time文件

`mykernel/mm/vm_map/mmap.c`的sys_mmap/sys_munmap、`__vm_munmap`、`simple_do_vma_munmap`、`__split_vma`、`vm_area_creat_dup`、`__simple_vma_adjust`、`simple_munmap_vma_range`、`simple_unmap_region`、`simple_mmap_region`、`simple_vma_merge`、`vm_area_alloc/free`和前后VMA helper；`mm/vm_map/{vm_map.h,vm_map_macro.h,vm_map_api.h,vm_map_types.h,util.c}`的区间查找/列表宏、VMA结构/ops/链操作和`vm_mmap_pgoff`；`arch/x86_64/include/asm/{syscalls_64.h,unistd_64.h}`的入口表项及编号；`arch/x86_64/myos/syscall.c`的sys_brk收缩；`sched/forkexec/{fork.c,exec.c,exit.c}`的dup_mmap/mmput/__mmput、exec旧mm归还、do_exit中exit_mm注释；`include/linux/fs/fs.h`的get_file；`fs/vfs/file_table.c`的fput；`lib/list/double_list.h`的链删除接口。另按需要读取`arch/x86_64/include/asm/syscall_wrapper.h`与`include/linux/kernel/mmap_lock.h`等直接上下文但未将其额外计入这15个锚点来源。

GitHub默认分支代码搜索只用于导航；**全部事实锚点**已回固定time实际读取的正文。没有使用默认master片段替代固定time；未使用仓库外libc、磁盘镜像或真实进程运行结果。否定结论限定所读函数主体、具体调用路径与注释状态，不扩大为仓库全局不存在其它资源释放者。

## 二、实际执行的机械检查

| 检查 | 操作与结果 | 限制 |
|---|---|---|
| facts短引文 | 对`facts-and-boundaries.yaml`的107个来源/行号/1–5行引文重新取固定time全源，分两批核对：前8文件94段，后7文件13段，**107/107逐字匹配** | 只是可追溯源码证据；并非107个已运行技术断言 |
| 正文引用 | 对`vma-lifetime.md`73个`[VERIFIED]`块逐项核对facts内路径、符号、行号及整个原文，**73/73完全相等** | 引文正确不等于条件推论必然可达 |
| 图与事实 | facts **45节点、58边**，DOT的45/58 ID集合完全对应；全部58条边的from/to及证据锚点有效，所有其它结构引文ID均存在 | call/data语义需合读活动/conditional/commented与实际源码，未运行Graphviz |
| 过程矩阵 | facts保存23个代表区间输入、13个生命周期状态、10条资源/所有权接口、12条effects；逐项有可解析的证据ID | 不对未定义输入设定固定运行后果或错误码 |
| 延期增量 | 本批6条新发现`VL14-F01～F06`，向`FC13-F05`、`UV-15`、`UV-10`追加3项证据；旧状态不改 | 不自行关闭或重新判定旧问题 |
| C/内核运行 | **NOT_RUN**：仓库脚本、完整YAML/Graphviz解析或渲染、预处理/编译、C/ASM、内核/QEMU/硬件、CPU TLB、真实用户munmap和多线程 | 无动态正确性认证 |

**工具实际使用：** 已授权GitHub连接器的`fetch_file`、PR refs/compare及有界JavaScript正则、字符串和集合核对。报告组装时第一次`unistd_64`正文引用行号偏1导致锚点检索失败，**报告未被写入**；改用精确锚点ID重新生成后才成功提交，并核73/73。编图前还在本包facts中把`vm_mmap_pgoff→do_mmap→simple_mmap_region`和`simple_mmap_region→simple_vma_link→__vma_link_to_list`还原为中间节点/真实调用，另补宏别名源码锚点；只修本批新件，无历史文件更动。

## 三、与旧审查及语义的必要限定

### 用户入口≠运行实测

编号11/系统调用表、`sys_munmap`活动`return __vm_munmap`成立于固定C源码；它不等于用户libc实际调用或完整08汇编入口重新验收。`vm_munmap`在vm_map_api.h是`__vm_munmap`的宏别名，不是额外实现。

### 首VMA及精确交界

`find_vma_prev_to_addr`比较`vm_end < start`；`find_vma_intersection`严格要求先得到前驱及其后继，不等于完整的“找所有交叠”的Linux算法。并且从NULL迭代器进入`mm_prev_vma/mm_next_vma`的C成员地址表达式存在无效对象前提。因此23类区间是**带有有效迭代器、指针和命中前提的源码分支矩阵**；不将没有得到目标VMA时返回的0冒充已卸载。

### call/data/init_order语义

图中call只表示**真实直接调用者→被调用者**；构建时发现`vm_mmap_pgoff`到`simple_mmap_region`中间实际存在`do_mmap`、`simple_mmap_region`到`__vma_link_to_list`中间存在`simple_vma_link`，已补足两个明确节点/边。data为消费方→对象/字段提供方，列表节点变更、`get_file/fput`、`vm_area_free`、页表操作归`effects`而不是反向data。无实际的完整图初始化前提时未生成虚假init_order。

### 失败、释放与退出

第一分割成功、第二分割失败时，前者的VMA拷贝、条件`get_file/open`、区间边界/挂链已可能发生，失败标签未回滚；mmap覆盖旧区间同样先做卸载再创建。摘链`__vma_unlink_from_list`是list与map_count操作，不是`vm_area_free/fput/close`。真正`simple_unmap_region`主体的`unmap_vmas/free_pgtables/tlb_finish_mmu`是注释，并且此函数在所读`simple_mmap_region`错误清理使用，不在当前munmap unlink环中调用。exec旧mm确会条件mmput，`__mmput`的exit_mmap仍注释；do_exit里的exit_mm也注释。上述是**限定的活动边断点**，不推定全树永远不会回收或一定硬件仍能访问。

主动反证六项：“删链=释放VMA”、 “浅拷贝自动加全部引用”、“返回0代表PTE/TLB撤销”、“第二次分割错误全量回滚”、“首VMA必有前驱”、“存在释放函数等于当前退出调用”，均不能由实际源码支持。前提/反例/unknown集中于本包facts与deferred；不因旧PASS_PENDING_LOCAL抹去新发现。

## 四、交付前Git状态与后置回读要求

本文件写前实际GitHub compare `2d67bea2fe59`至`c766868f7b86`为**6个后继提交，净只有本批目录四件A新增**、0旧件净修改/删除；PR17仍open/Draft，master/time未变，执行头与PR17一致。前两件facts/DOT曾在本批交付前按自身blob SHA合法调整调用图；全部旧件一直冻结。

写入本件evidence后，仍需新增第六件MANIFEST；**最终固定提交和当前分支的六文件全文与blob比较、PR17状态、基线diff及交付评论，必须在MANIFEST之后真实执行，记录在PR17交付评论**，不能将未来回读写成此刻已通过。GitHub Contents接口没有全局原子分支锁，保留从写前读取至写入间的并行竞态限制。

本包保持阶段2，仅`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`。结束后停写交C03，不自行启动第15批或补丁。
