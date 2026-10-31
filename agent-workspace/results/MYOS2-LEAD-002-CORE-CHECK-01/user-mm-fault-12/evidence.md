---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-MM-FAULT-12
record_type: bounded_fault_source_evidence_and_preflight
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
produced_by: "ChatGPT普通对话，Owner指定GPT-6/Extra；backend未独立认证"
date: "2026-10-31"
read_channel: connector
source_pin: "time @ a039d9803ade"
result_base: "2c410132e42d"
instruction_ref: "agent-workspace/lead/MYOS2-LEAD-002/30-user-mm-fault-contract.md @ ec5ab09e3c2d"
required_prior_review: CORE-USER-PID-LIFETIME-11-REVIEW-001
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
source_quote_verification: "facts 202/202 slices against fixed time (178+24 in two bounded batches); narrative 59/59 matched to facts"
method_limit: "Connector fixed-source fetch and bounded JS text/ID validation; no compilation, execution or full parser"
final_remote_readback: "Must follow final MANIFEST commit; report final commit and seven-file comparison in PR17 comment"
open_questions:
  - "IDT/exception assembly, full mmap locking, file cache allocation, actual hardware fault recovery all untested"
  - "SR11-01 remains in mainline RETURN until C03 independently reviews M00"
---

# M04：源证、主动反证与可复核边界

## 1. 固定对象及读写前提

本轮固定**三个不同对象**：`time @ a039d9803ade`是唯一技术源码快照；第11批及此前所有旧成果按`execution @ 2c410132e42d`冻结读取；第12批任务书按`ec5ab09e3c2d`读取，主线11回执按`e7ca3dbbcc06`读取。master与lead的同名源码和演进不混充time事实。

首先读取`mykernel/scripts/options_flags.cmake`全文，核编译文本声明的x86-64环境和CMAKE_C_FLAGS的已有变量拼接；**没有实际CMake预处理/宏求值**，不可否定外部变量注入。PR17首次写前实际为open/Draft，head=`2c410132e42d`；master=`e8e60cd19540`、time=`a039d9803ade`；开放PR只有16/17，主线PR16与本批新目录无路径交集。执行分支`claude/dazzling-cori-q0dnyt`沿用历史名称，不表示本轮启动了Claude Code或旧CC02写权。

## 2. 本轮实际读取的固定源码与查询范围

| 读取组 | 文件/符号范围 | 允许的结论，不作的外推 |
|---|---|---|
| 架构入口 | `mykernel/arch/x86_64/mm/fault.c`全文（465行），`exc_page_fault`、`handle_page_fault`、`do_kern_addr_fault`、`do_user_addr_fault`、`myos_bad_area`、`fault_in_kernel_space` | 只确认C入口和返回层，未验证IDT/ASM异常入口、硬件重试与实际SIGSEGV |
| VMA和栈 | `mykernel/mm/vm_map/vm_map.h`的`simple_find_vma`与`vma_is_anonymous`，`mmap.c`的`expand_stack`、`simple_mmap_region`，`vm_map_types.h`的`vm_ops`类型 | 核包含关系、函数指针定义；不做完整VMA/并发/mmap策略审计 |
| 中央fault | `mykernel/mm/fault/fault.c`全文（1486行）并定点`myos_handle_mm_fault`、`handle_pte_fault`、`do_pte_missing`、`do_anonymous_page`、`do_fault`、`do_read_fault`、`do_fault_around`、`__do_fault`、`finish_fault`、`do_cow_fault`、`do_wp_page`、`wp_page_copy` | 区分有条件call/函数指针、0/非0/不返回；页表/folio后端只追必要接口 |
| 页表帮助与状态码 | `mykernel/mm/fault/{pgalloc.h,pgalloc.c,pgalloc_macro.h,pgtable-generic.c,fault_types.h,fault_const.h,memfault_api.h}`及`mykernel/mm/mm_type_declaration.h`、`include/uapi/asm-generic/errno-base.h` | 回源`pte_alloc`宏、PUD/PMD/PTE分配和`vm_fault_t`位掩码 vs `ENOERR=0`，不认证完整页表分配器 |
| 文件页缓存 | `mykernel/mm/vm_map/filemap.c`的`generic_file_vm_ops`、`generic_file_mmap`、`simple_filemap_map_page`、`simple_filemap_fault`；`mmap.c`的`call_mmap`调用 | 固定通用文件VM函数指针绑定，未穷尽所有vm_ops赋值/函数指针目标，FAT/ATA读后端沿05已有入口停边界 |
| 历史记录 | 11的OWN11/E38/E39/ST14及其`pid-lifetime.md`P02摘要；10的P00和SW10-A15/A19/A20；05主说明C6原句与UV-15.claim；09的未覆盖描述、UV-08/UV-06/RS-08旧项 | 定点追加消费，旧文件、旧问题状态不重写；不是旧10/11全锚点检查器重跑 |

**否定结论的实际集合**：对`do_user_addr_fault`的guard/access_error/retry/SIGSEGV判断，只依据固定`arch/x86_64/mm/fault.c`可见函数体活动与注释行；对`simple_find_vma`的“无后继VMA”只依据`vm_map.h`的该函数定义；对`do_shared_fault`等“当前不返回”只依据`fault.c`该函数可见前部`while(1)`；对generic文件绑定只依据`filemap.c/mmap.c`具体目标。未完成全树负向搜索、预处理后的实际符号/间接调用穷尽，不能将这些限定断言扩大为整个仓库不存在其它保护/生产者。

## 3. 源码引文核对、图及矩阵

- **facts源文**：`facts-and-boundaries.yaml`共**202段**带文件路径、所属符号标签、固定源码行号和1–5行文本的锚点，来自11个固定time文件。机械引用核验分两次成功完成：第1批6文件178段，第2批5文件24段，**202/202逐字、行范围匹配**。数字表示细分的可追踪短片段，不是202项相互独立的语义证明；函数/宏所属符号与执行顺序按本轮源码上下文人工判断。
- **可读正文**：`user-mm-fault.md`的**59段[VERIFIED]**均再次以facts固定路径、符号、行范围、原文做**59/59完全匹配**。其它文字的“条件推导”“固定失效”仍受上下文及未运行限制，不以引用数量替代语义。
- **结构图**：facts有**41节点、49条边、25类输入/结果、9类effects**；`fault-paths.dot`的41节点ID、49边ID逐项集合一致，49边端点/锚点引用全部能解析。call只用实际C直接调用，动态`vm_ops`先到dispatch节点，并须连读generic_file_vm_ops的赋值绑定；`alloc_anon_folio`与`folio_prealloc`明确分为两层（MF12-N41/MF12-E34/E49），非虚构直接call。
- **勘误/问题**：`pid11-disposition.md`逐项覆盖SR11-01返回类型/E38证据，补ST14/A84b及SR10-01的SW10-A20消费；旧05 C6、UV-15.claim按准确原文分别限定。新`deferred-findings.yaml`登记7项本批有条件风险、3条旧问题证据追加，不修改原条目。
- **工具真实运行**：以上是GitHub`fetch_file`、`get_pr_info`、`compare_commits`及有界JavaScript字符串/正则/集合检查。未运行正式YAML/Graphviz解析器或渲染器，未执行仓库脚本、编译、预处理、C/ASM、内核、QEMU或宿主负例。

## 4. 主动反证本批易犯的六类夸大

| 先假设的过强说法 | 固定源码的反证或前提 | 结果 |
|---|---|---|
| “有VMA就有读/写/取指许可” | `simple_find_vma`只核包含地址；`access_error`整个分支注释 | **不能保证**；需独立权限/页表前提 |
| “分配器返回或调用过就有有效PTE” | `myos_handle`分配失败走VM_FAULT_OOM、pmd_none时自旋、pte_ptr NULL时return0 | **不成立**；PTE尚未写入也可0 |
| “返回0就证明异常已被修复” | `pte_offset_map_nolock`NULL早退0，匿名folio IS_ERR返回0，WP页复制失败可能0，既有PTE访问位不写回 | **不成立**；0只能解释源文字面返回 |
| “非零就是发SIGSEGV/OOM或重试成功” | `fault!=ENOERR`忙等不重新调用；后续VM_FAULT_RETRY/OOM/SIGBUS分派为注释 | **不成立**；非零停留该C调用栈 |
| “名字出现就是活动call” | `expand_stack`在正常stable包含VMA条件下未被选择；`do_shared_fault`开头死循环；文件`vm_ops`必须有实际绑定 | **不成立**；按条件/中间调用画图 |
| “异常C函数返回＝09的syscall返回尾” | 09此前只核特定syscall/ret_from_fork出口；本批未验证IDT/异常IRET、硬件重复取指 | **未知**；不跨入口外推 |

## 5. 写前历史与提交后安排

本证据文件准备发布前实际compare从`2c410132e42d`到`c13f7d51d985`为**7个新增提交、仅本目录5个A新增**，0旧件净修改/删除；PR17仍open/Draft且执行头`c13f7d51d985`，time/master分别为`a039d9803ade`/`e8e60cd19540`，lead合法独立前进。本轮两次只修正**本批新建**facts中的调用锚点与间接边，历史全部保持。

**最终七件的固定提交与分支完整回读、blob身份、diff及PR评论必须在MANIFEST发布后执行**；本文件不猜最终Git提交。GitHub contents逐次写入无可证明的全局原子分支锁，写前检查与写调用之间的竞态限制保留，若头不明变化先停写核源，不覆盖旧件。

## 6. 严格停止边界

本批M00执行者accept不是主线关闭：SR11-01仍待C03核收，历史05的UV-15状态也不由执行方改判。未实施内核修复、补丁、阶段3，未启动外部API/其它模型、安装工具、修改用户程序、合并/关闭PR或删除分支。阶段2上限`PASS_PENDING_LOCAL`仅指有限静态成果的可验收程度，**不是缺页恢复在硬件上通过验证**。
