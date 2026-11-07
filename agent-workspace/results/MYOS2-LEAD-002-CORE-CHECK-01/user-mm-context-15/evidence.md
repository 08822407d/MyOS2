---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-MM-CONTEXT-15
record_type: bounded_mm_context_evidence_and_delivery_check
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: "2026-10-31"
read_channel: connector
source_pin: "time @ a039d9803ade"
results_base: "dd8e81ba6fc7"
task_contract: "33-user-mm-context-contract.md @ 9a423f5b45b1"
instruction_base: "18ee22673d83"
required_review: CORE-USER-VMA-LIFETIME-14-REVIEW-001
prior_status: {vma14: PASS_PENDING_LOCAL_WITH_R14_S01_S02}
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
source_quote_verification: "110/110 fixed-time direct line slices matched, 64+46 in two bounded batches"
readable_quote_verification: "88/88 [VERIFIED] blocks exact path, symbol, line bounds and quote compared to facts"
final_remote_readback: "Must occur after MANIFEST commit; record results in PR17 delivery comment only"
owner_decisions_requested_now: []
---

# A04：源码证据、自查、主动反证及禁止范围

## 一、固定版本、任务接受与实际读取

完整读取33号固定任务书及其指定的`CORE-USER-VMA-LIFETIME-14-REVIEW-001`主线回执；13旧缓存页所有权仅按R13-S03与14的明确引用限制消费。第14批以`PASS_PENDING_LOCAL`有限静态验收，**R14-S01必须替代旧报告“brk错误不恢复字段”断言**：当unmap正常非零返回而走sys_brk::out时，`mm->brk=origbrk`活动地恢复；VMA/文件引用先前副作用未证明回滚。R14-S02规定补真实调用点并保留帮助函数中间层。本批不重做14的107/73证据、split/unlink算法或13缓存读，不修改旧件。

本轮技术事实全部基于`time @ a039d9803ade`；结果旧基线`dd8e81ba6fc7`，指令固定`18ee22673d83`；三者不混淆。首先全文读取`mykernel/scripts/options_flags.cmake`：项目设置x86-64等选项，但CMAKE变量和`DEBUG`等实效开关无法凭配置文本认证全部编译条件，**未进行实际预处理/编译**。

写前查看了PR16/PR17、两页开放PR、双方变更路径、直接master/time/lead/执行分支头：PR17原head=`dd8e81ba6fc7`且open/Draft，仅PR16/17开放、PR16不含目标目录，master=`e8e60cd19540`、time=`a039d9803ade`，目标目录当时不存在。C03的lead合法前进不替换固定time来源或改变本执行写区。

## 二、真正读到的源码（15个锚点文件）

- `mykernel/sched/forkexec/fork.c`：完整读取，重点mm_init/mm_alloc、mm_free_pgd、mmput/__mmput/mmdrop/get_task_mm、mm_release/exec_mm_release、dup_mm/dup_mmap/copy_mm、copy_process的正常与bad_fork_cleanup_mm链，区分返回失败的前后副作用。
- `mykernel/sched/forkexec/exec.c`：bprm_mm_init及其失败mmdrop、__bprm_mm_init/临时VMA、begin_new_exec调用exec_mmap、exec_mmap的old_mm非NULL/NULL与task mm/active_mm安装、free_bprm的条件mmput。
- `mykernel/sched/forkexec/exit.c`：do_exit里exit_mm注释；不重做退出的信号/PID链。
- `mykernel/sched/scheduler/scheduler.h`、`scheduler_core.c`：由后者第1行的SCHEDULER_DEFINATION确认该C文件包含inline context_switch定义；核__schedule prev!=next、context_switch的四类mm切换、finish_task_switch对rq.prev_mm的清除和被注释的延迟归还。
- `mykernel/arch/x86_64/mm/mmu/mmu_arch.c`、`mmu_macro_arch.h`、`arch/x86_64/sched/context/switch_to_macro_arch.h`、`arch/x86_64/insns/special_insns_arch.h`：activate_mm宏、switch_mm/switch_mm_irqs_off同指针门、load_new_mm_cr3、write_cr3内联asm、switch_to→__switch_to_asm包装文本，均只读不执行。
- `mykernel/arch/x86_64/mm/pgtable/pgtable_arch.c`：pgd_alloc/pgd_free真实页表根分配、内核PGD部分复制和释放端点。
- `mykernel/mm/misc/mm_misc_types.h`、`mm_misc.h`、`init-mm.c`及`mykernel/sched/task/task_types.h`、`arch/x86_64/kernel/cpu/common.c`：字段mm_users、mm_refcount、task.mm/active_mm、mmget、init_mm特殊静态计数及CPU active_mm初始化。
- 另对`mykernel/mm/misc/mm_misc_api.h`与`arch/x86_64/mm/mmu/mmu_api_arch.h`接口声明作定点查看；未将纯声明当作活动mmgrab或释放实现的证明。

在默认分支GitHub代码搜索中使用`mmgrab/mmdrop/mm_refcount/context_switch/switch_mm_irqs_off/write_cr3/pgd_alloc`等检索**只作定位**。真正的正向调用/字段变化及有限否定结论回固定time全文，不能把搜索未命中扩展为全仓不存在。对于`mmgrab`，检查到注释及字段文档，却**未在本批选定mmget/mmput/mmdrop与scheduler借用链里找到活动mmgrab引用计数操作**；不补造标准Linux mm_count语义，不做全库不存在性声明。

## 三、真实核验方法与计数

| 检查 | 已运行及结果 | 边界 |
|---|---|---|
| 固定源码引文 | facts中**110段**路径/符号/1–3行短引文（含文件首行的2行锚点），分两批从15个固定time文件全文回读：前7文件64段、后8文件46段；**110/110逐字及行范围匹配** | 文本匹配不是编译有效性/调用可达性的证明 |
| 可读正文 | `mm-context.md`的**88段[VERIFIED]**路径、符号、行号和整个引文对照facts，**88/88相同** | 模型静态解释仍须按条件、真实返回和反证审 |
| 图与结构 | facts和DOT**46节点、60条边**ID集合相等，全部边端点和所用来源ID有效；19个输入/结果、9状态、10所有权和10个effects的锚点均存在 | 未执行Graphviz渲染，且静态集合核验不等于全边语义认证 |
| 增量问题 | 新`MC15-F01～F06`六项条件风险，向既有`VL14-F04/F03`、`FC13-F05`、`UV-15`追加四组证据；旧状态不变 | 不签内核运行修复或主线问题关闭 |
| 代码/运行 | **NOT_RUN**：仓库脚本、完整YAML解析、Graphviz渲染、预处理/构建、C/ASM、内核、QEMU、外部libc、真实任务切换与CR3/TLB实验 | 不认证真实页表/内存正确性 |

实际使用GitHub连接器固定源fetch、PR/branch/commit compare及有界JavaScript字符串、正则、集合检查。组装第一份facts时，`scheduler_core.c`首行锚点触发了错误的“必须有前一行”检查，**在GitHub写入前失败**，之后按合法1–5行裁剪、重试并完成。没有盲目写出错误文件，亦没有更动旧件。

## 四、主动反证与停边界

1. **mm_users归零≠mm被free。** `mmput→__mmput`活动调用成立，但exit_mmap/mmdrop在__mmput内是注释；另有直接mmdrop路径能触发pgd_free/free_mm，却没有先根据mm_refcount判零。
2. **两个task mm字段指向同一mm≠两份用户引用。** CLONE_VM只调用一次mmget，随后给task.mm和active_mm同时赋同一指针；新复制mm则要经历allocate/memcpy/mm_init/dup_mmap。
3. **结构memcpy≠复制后的VMA/PTE和引用均独立正确。** dup_mmap错误前可能已get_file/入链/调用copy_page_range，失败清理mmput不能替代注释中的exit_mmap。
4. **安装task.mm指针≠实际硬件切换。** exec_mmap按顺序安装指针并调用activate_mm；context_switch只在prev!=next实际进入。arch同指针可直接返回，不检测CPU当前CR3。
5. **write_cr3的代码路径≠全局TLB正确性。** 已追到虚拟页表根转物理地址与内联asm，但没有执行或检查PCID/跨CPU shootdown、同步屏障及实际CPU寄存器。
6. **错误返回≠撤销已有副作用。** dup_mm后期失败使用mmput，但无活动VMA/pgd全回滚；exec old_mm NULL可能先受mm_release的mm指针前提、后来受mmdrop(init_mm)守卫约束；不预测实际崩溃。
7. **释放接口存在≠当前do_exit调用。** do_exit的exit_mm仍注释，不能用exec_mmap条件mmput或mmdrop去填补所有退出路径。

## 五、GitHub交付前后保全

在准备本证据文件之前，GitHub compare`dd8e81ba6fc7`至`7603df109c46`为**4个连续后继提交、仅本批目录4件A新增、旧件零修改/删除**；PR17保持open/Draft、执行头一致，master/time不变。第五件evidence、第六件MANIFEST须按授权同目录顺序提交。**所有六件最终固定提交/分支全文、blob、diff和交付评论须在MANIFEST后真正执行并只记于PR17评论**；本文件不预言未产生的提交或结果。

GitHub contents写接口不提供全局原子head锁，读取与写入间仍有竞态可能；如果不明变动或写调用状态不确定，先回源，不覆盖旧文件。正式提交后停写交C03，最高只申请`PASS_PENDING_LOCAL`静态有限验收，不进入阶段3。
