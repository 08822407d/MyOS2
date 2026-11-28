---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-FAT-NAME-PARSE-36-DOC-CLOSURE-REVIEW-001
record_type: independent_prior_documentation_return_closure
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-04
produced_by: unknown
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（沿用Owner报告）
backend_identity: unknown_or_not_attestable
read_channel: connector
source_pin_short12: a039d9803ade
original36_head_short12: 3c02d6c020e3
correction37_head_short12: bcb7ecb66a06
reviewed_delivery_comment: 6632057217
fixed_contract61_short12: 345b997470f7
prior_return_record: CORE-USER-FAT-NAME-PARSE-36-REVIEW-001
prior_return_record_short12: 345b997470f7
R36_01: CLOSED_DOCUMENTATION_ONLY
R36_02: CLOSED_DOCUMENTATION_ONLY
remaining_return_items: []
combined36_acceptance: PASS_PENDING_LOCAL
original36_artifacts: PRESERVED_UNMODIFIED
mandatory_consumption_notes: [R36C-S01]
runtime_evidence: NOT_RUN
full_local_P9_gate: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
publication_status: PUBLISHED_IN_CONTAINING_COMMIT
repository_status_update: VERDICT_RECORDED_BY_THIS_COMMIT
mainline_head_observed_short12: 345b997470f7
execution_head_observed_short12: bcb7ecb66a06
owner_decisions_requested_now: []
---

# 第36批文档退回独立闭合裁定

> 本文件承接上一验收回合已完成并保全的独立裁定，本次恢复发布不重做验收、不改变结论。
> 技术章节中的“本轮”指原验收回合；本次发布状态与同提交的发布恢复检查点合读。
> 第38批启动只使用同一固定提交的62号任务书，旧附件中的发布暂停说明属于历史。

## 1. 独立裁定

R36-01、R36-02均可闭合，状态为CLOSED_DOCUMENTATION_ONLY，没有剩余文档退回项。
第36批按“原六件＋第37批N00追加勘误＋原36回执＋本闭合裁定”组合材料，以PASS_PENDING_LOCAL限定接收。
这不撤销原RETURN的历史事实，不追认原56标签全合格，不覆盖原图，不表示任何内核源码问题已经修复。

纠正材料位于PR17 bcb7ecb66a06的user-inode-init-37/：
- inode-init.md开头“第36批追加勘误”及完整替代DOT；
- facts-and-boundaries.yaml的原证据处置索引、原图节点/边映射及prior_return_items；
- evidence.md的N36C-A001至N36C-A021；
- MANIFEST.md和deferred-findings.yaml的范围、状态、计数与消费说明。

执行者只申报CORRECTION_SUBMITTED_PENDING_MAINLINE/PENDING_C04_REVIEW，没有自行关闭RETURN。
关闭是本轮C04独立复核的结论；与第37批新分析的裁定分别记录。

## 2. R36-01：证据分类、关联和计数闭合

原NP36-A001至A056均有处置条目：53条限于历史保留、不冒称本轮再次逐字验证；
两个原ID被替换，另一个降为条件编译上下文。两条被替换的原ID不是仅产生两个新标签：
A032拆成LOWERCASE_BASE和LOWERCASE_EXT两条单宏证据，A036只保留MSDOS_NAME自己的单行定义。
A043的#ifndef文字保留为选择上下文，不再假造“strncmp conditional compiler gate”定义符号。

N36C-A001/A002分别给出两个真实小写宏，A003只给MSDOS_NAME。
长槽相关N36C-A004至A007直接锚定msdos_dirslot_s，而非以短目录项lcase或size证明长槽布局。
F02/F07的引用范围得到重新约束：字段存在只证明布局；是否进行序号、校验或字符检查仍要合读完整活动函数。

MANIFEST、正文、evidence、facts、deferred以及旧交付评论的总括声称已给出追加替代口径。
旧评论没有被编辑；以后不能脱离追加勘误单独使用其中“56条全部单定义合格”的表述。

本次纠正来源的21个新直接标签与37新取证的59个标签分区计数。
原56个ID的处置数量、新标签数量、实际重新核查数量彼此分开；53条历史保留不升级为53条本轮机械复验。
因此R36-01的指定关闭条件得到满足。

## 3. R36-02：替代图及跨件语义闭合

完整替代图位于37正文专属勘误节，而不是新增第七件文件。其关键改动对应原退回要求：

1. 外层左条件、唯一getter调用和返回后的IS_ERR判断分开；左条件假直接到循环后，不调用getter或进入解析体。
2. 长名回退中的cursor减量和readent只对应一次实际读取，返回后重新求值第一遍while。
3. 单元筛选具有“过滤后不写输出”的路径；所有单元被过滤也能结束内层循环和本次槽处理。
4. 输出可写性标作实际存储的分析前提，而不是源码中的提前NULL测试。
   短名目录首字节为空格的零存储分支，以及长名零存储且其它操作正常完成的分支，保留正常NULL返回模型。
5. 每轮namelen=0的位置在辅助readent和off赋值之后、空项检查之前；
   长名选择及第一遍的属性、空项、cursor条件按实际短路顺序表示。
6. C18/C19负长度模型改为helper-only；F03的“失效存储前缀”和“零存储后正常返回NULL”分开，不补出失效后的调用。

facts中提供原45个节点和65条边的替代说明，原图保留历史而不再作为当前控制流依据。
作者申报替代图53节点/81边；本轮闭合根据逐项控制语义回核，不以作者计数代替语义证明，也不冒称原DOT全文字节机械复验。

### R36C-S01：替代图的必要阅读条件

- sbase的“space/end”必须包含真实x<8循环上界；sext的“no extension”严格指x==8，不能把已经写入扩展名后的空格误当成撤销点号。
- lcursor的负值分支属于源码表达式结构；在已核readent负offset返回ERR_PTR的路径上，早先attr消费已经失去有效对象前提，不能把它当作可正常到达的安全退出。
- zero、nullret、shared、cache和premise等解释/数据节点不新增源码控制门，也不保证深层调用没有副作用。
- 原图到新图的映射是替代消费说明，不要求每条旧边在新图中有同名的一条直接边。

这些限定与已读源码及替代正文一致，不需要再委派一轮文档纠正。

## 4. 长度、NUL和零存储补充的核查

长名对0/0xffff单元执行过滤而非终止：在明确小端字节模型中，A、0、B被投影为AB。
输出计数与首次NUL位置不同：0x0100可被接受后写入一个零字节，不能把首次NUL无条件等同namelen。
真实lookup初始化长度为0；非零初始值只按独立helper模型使用。

本轮独立有限模型复核了：
- i=1、初值0、13次存储：容量14，最后存储下标12，尾字节13仍在容量内；
- 初值1、13次存储：下标1至13均在容量内，但最终计数14处没有容量内尾字节；
- 初值2、13次存储：首次越出容量的存储下标为14；
- 过滤零并不停止后续字符，16位值的单字节投影及嵌入零；
- 先取max再转size_t，单个负数不等于最终负计数。

模型不执行仓库C，不验证目标对齐、别名、清零分配或真实目录生产者。
零存储只有在其它输入访问及额外readent正常完成后，才支持正常NULL返回。

## 5. 证据入口与范围

固定原RETURN：
https://github.com/08822407d/MyOS2/blob/345b997470f7/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FAT-NAME-PARSE-36-review.md

固定61号：
https://github.com/08822407d/MyOS2/blob/345b997470f7/agent-workspace/lead/MYOS2-LEAD-002/61-user-inode-init-contract.md

固定纠正正文：
https://github.com/08822407d/MyOS2/blob/bcb7ecb66a06/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-inode-init-37/inode-init.md

固定纠正证据：
https://github.com/08822407d/MyOS2/blob/bcb7ecb66a06/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-inode-init-37/evidence.md

源定义固定time@a039d9803ade：myos_dir.c两个解析器完整活动体、myos_fat32_namei.c::FAT32_lookup、
fat32.h小写宏、uapi/linux/msdos_fs.h长短槽定义、myos_fat32_iobuf.c::FAT32_iobuf_readent；
strncmp条件及活动定义沿用同一固定源已经读取的上下文。
本轮完整读取37六件及交付评论，原36按指定问题和此前独立回核记录消费，不重做全部原56项。

完整本地P9、原始交付的全量字节匹配、编译/预处理、QEMU/硬件、磁盘及真实路径均未执行。
本轮与执行者的模型分角色复核，不等于人类或异构模型技术验收。

## 6. 历史阻断与本次恢复发布

上一验收回合GitHub读取与仓库push许可正常，但当时可调用工具缺少创建提交、更新分支及写PR动作。
因此仅保全裁定，未声称GitHub已经同步。此事实保留，不追溯改写成上一回合已发布成功。

Owner随后明确要求继续至下一任务发布。本次已重新发现正规写入口，核主线仍345b997470f7、执行仍bcb7ecb66a06，
同名回执不存在后，以既有主线正常非强推快进追加本文件。原验收结论和技术章节未改，执行成果不改。
同提交62号及发布恢复检查点共同组成正式入口；提交身份以GitHub真实包含本文件的固定提交为准。
PR导航和追加评论的后置同步结果记录于实际远端，不在本文件预造自身SHA或评论号。
