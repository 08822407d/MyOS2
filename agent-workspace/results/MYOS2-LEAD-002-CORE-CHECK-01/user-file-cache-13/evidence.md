---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FILE-CACHE-13
record_type: file_cache_evidence_method_and_check_record
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: MYOS2-A-C03a 代码分析执行
produced_by: "ChatGPT普通对话；Owner选择GPT-6 / Extra；实际后端不可独立认证"
model_per_owner: GPT-6
effort_per_owner: Extra
date: "2026-10-31"
read_channel: connector
source_pin: "time @ a039d9803ade"
old_execution_base: "16b77f23003f"
instruction_ref: "agent-workspace/lead/MYOS2-LEAD-002/31-user-file-cache-contract.md @ 58b0bf268726"
required_review: CORE-USER-MM-FAULT-12-REVIEW-001
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
kernel_change_made: false
owner_decisions_requested_now: []
final_remote_readback: "After MANIFEST commit, compare six added files and read back both fixed commit and live branch; final actual SHA and checks belong to PR17 delivery comment"
---

# 第13批：固定源、证据强度与自查记录

## 一、身份、基线和实际读取

**31号固定任务书（58b0bf268726）全文已读，12主线回执 CORE-USER-MM-FAULT-12-REVIEW-001 全文已读。** 依其裁定，SR11-01在限定审查中关闭，第12批为PASS_PENDING_LOCAL且须合读R12-S01/S02；没有为了两项既成主线消费纠正重做第12批。18号普通问题延期、28号同轮派发要求也已读取。

首次执行前确认：GitHub中PR17仍Open/Draft、head为`16b77f23003f`；固定`time=a039d9803ade`、`master=e8e60cd19540`；只见开放PR16/17，两个PR既有文件与本批唯一新目录没有交集，且该目录原先不存在。lead分支由C03正常独立推进，本执行方未写lead；不从master读取源码替代time。先读`mykernel/scripts/options_flags.cmake`，保留外部编译选项及未经预处理的未知，不冒称实际构建配置或内核二进制已经核实。

**固定源码阅读及定点检索范围：** `mykernel/mm/vm_map/filemap.c`（simple_filemap_read, simple_filemap_fault, simple_filemap_map_page, generic_file_read_iter, generic_perform_write, generic_file_mmap及回调绑定）；`fs/vfs/namei.c`（__lookup_slow, __lookup_hash, open_last_lookups及open边界）；`include/linux/fs/fs.h`（inode, address_space, file, fops, get_file, call_mmap）；`fs/vfs/inode.c`（inode_init_always, iput）、`fs/fat/inode.c`（fat_alloc_inode, fat_fill_inode）、`fs/vfs/open.c`与`file_table.c`（打开绑定、filp_close/fput）、`fs/fat/myos_fat32.c`（FAT32_file_ops, FAT32_read）；`mm/fault/fault.c`（do_read_fault/do_cow_fault、vm_ops回调调用、finish_fault、vmf.pgoff）；`mm/vm_map/mmap.c`（get_file/call_mmap及错误释放），`mm/page_alloc/buddy.h`与`buddy.c`（引用API与__folio_put正文）。还定点读`fs/vfs/dcache.c`、`fs/vfs/file.c`、`fs/vfs/read_write.c`、`include/linux/kernel/uio.h`以确定替代入口与现有引用边界；这些额外文件不增加本批facts的12文件引文计数。

固定历史资料只限05的主说明C4/C5、UV-06/07/08/10，12的文件VM回调端点和主线R12-S01/S02；08不需要另开批量检查。GitHub默认分支代码搜索仅作函数路径导航，实际技术断言一律回`time`固定文件。没有全树穷尽页缓存其它创建者、LRU/回收与所有并发调用者。

## 二、逐字回源、图与结构

| 校验对象 | 实际执行方法 | 结果与消费限定 |
|---|---|---|
| `facts-and-boundaries.yaml`固定源锚点 | 对12个固定`time`源文件按路径读取；将全部88条记录的1–5行引文与原行号切片比较，分两批7文件/40段和5文件/48段 | **88/88逐字和行范围匹配。** 三行片段为精准定位，并非88项独立语义证明 |
| `file-cache.md`引文 | 从facts读取路径、符号、起止行与文本，逐一对比正文中的70个`[VERIFIED]`代码块 | **70/70全文、路径、符号、行号一致。** 引用存在不能单独证实每个条件推论 |
| 事实/图结构 | 从facts抽取节点/边ID、各端点和锚点，再读DOT节点/边集合 | **42节点/46边**ID一一相同；每条边有存在的from/to及源码锚点 |
| 条件/所有权矩阵 | 对facts 17种状态、10条所有权关系、16类代表输入及11个effects引用作存在性检查 | 源码锚点全部可解析；不把表格ID数当作动态测试次数 |
| 问题增量 | 检查本批六条`FC13-F01～F06`、四条旧05证据追加、两条旧文字候选的证据ID均存在 | 旧05/12编号和状态保留，候选待C03裁决 |
| 方法边界 | GitHub连接器`fetch_file`、PR/refs/compare读取；有界JavaScript字符串、正则、行切片、集合对比 | **未运行YAML/Graphviz正式解析、仓库旧检查器、源码/内核编译或任何运行** |

**语义额外检查**：首次组织事实片段时超过5行的候选被构建器拒绝，未产生该次远端写入；随后改为每项3行的真实来源引文。第一次DOT读取器因`binding_FAT32`含数字而少匹配3条边，是读取器字符集过窄，不是事实/DOT缺边；更正状态字符集后确认完整46条边。交付前在**本批新建**的facts中修正了`do_read_fault→finish_fault`的调用锚点、将`do_cow_fault→folio_put`限定为自身错误清理而非cache槽释放、增加有真实调用点的条件COW→finish_fault边，并同步更新本批DOT。**旧文件从未被更新。** 不以初期失败的自查冒充已通过。

## 三、调用图类型和反证

- **调用边**只表示所读源码中的活动调用；当入口受`slot==NULL`、`file`有效、`vm_ops`选择或其上游条件限制时标`conditional`，而不是声称全输入无条件执行。通过`file.f_op.read`和`vm_ops`的边分为调用点与明确绑定，不能凭目标名字画成直接call。
- **data边**始终为消费方→提供方；页指针写槽、缓存读填充、PTE建立、file计数增减与释放作为单独effects。缓存槽被赋指针不等于完成数据读取。
- **反证一：** `page_array[i]!=NULL`可以在后端read完成前成立，且文件缺页消费者只判断非NULL；不能断言已可用文件内容。
- **反证二：** `generic_file_read_iter`的count零返回0并没有创建/验证缓存；普通读/缺页快速hit也不比较数组索引与`nrpages`。
- **反证三：** 私有写fault可不经快速缓存命中判断直达`simple_filemap_fault`中`BUG_ON(slot!=NULL)`；它是条件可达路径，不能直接写成已复现的崩溃。
- **反证四：** `get_file`只增file.f_count，`simple_filemap_map_page`只写PTE；二者都不是cache page_s的`get_page`。
- **反证五：** `filp_close/fput`与`iput/folio_put`的API存在，不证明本批所读关闭/映射清理链已遍历cache数组并释放各页。
- **反证六：** `__lookup_slow`在nrpages==0时未分配array，不等于page_array初始化为NULL；需要结合FAT inode的非清零分配及inode_init_always缺赋值前提。

所有负向结论都限明文所读函数体、配置未预处理及实际可见调用；不会被解释为全仓不存在缓存回收、锁或其它失效实现。对溢出/越界、未初始化、页分配失败时的语言层后果不预测固定errno、硬件崩溃或文件数据值。

## 四、写前差异与后置交付

本`evidence.md`发布前已实际核对：PR17头`30fcb58d2812`仍Open/Draft，`time=a039d9803ade`、`master=e8e60cd19540`未变。从旧基线`16b77f23003f`到该头为**7个连续后继提交，净变化仅本批目录4个文件A新增、旧件0修改/删除**。事实和DOT的本批校正发生在完成正式交付前，同路径按当时blob SHA更新；没有追加第七份文件。

第六件MANIFEST之后，必须分别以最终提交和PR17当前执行分支完整回读六件、核blob和Git compare只含六新增路径A、PR17仍Open/Draft；**这些尚未发生的结果只记录在PR17后置交付评论**，本文件不预测最终Git对象编号。Contents API不是原子全分支锁，预读头到写入仍有小的并行竞态限制，不宣称已锁定分支。

## 五、NOT_RUN与交回权限

**NOT_RUN**：完整YAML/Graphviz解析/渲染、旧05/11/12脚本、预处理、编译/运行C或ASM、内核、QEMU、硬件、真实磁盘/块设备/用户缓冲区测试、完整文件系统、内存页回收和全并发操作。未安装或调用额外API/模型，也未读取仓库外libc/镜像。普通问题只记录到本批deferred，保留Owner“以后再处理”的原规则。

本执行方交付仅为`READY_FOR_MAINLINE_REVIEW`，主线验收`PENDING`，最高`PASS_PENDING_LOCAL`。不重开12已验收事项，不修内核或缓存，不启动第14批；最终写入后停写交C03。
