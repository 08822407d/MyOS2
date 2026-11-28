---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
conversation_display_name: "MYOS2-A-C12 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（系统身份；实际后台与effort未独立认证）"
actual_backend_model: "UNATTESTED"
actual_reasoning_effort: "UNATTESTED"
date: "2026-11-29"
date_timezone: "Asia/Shanghai"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "6f860774cf10"
execution_pin_short12: "80382a714198"
read_channel: "GitHub connector；自写有限文本/字节/整数核验"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\", "\tthis_cpu = smp_processor_id();"]
self_check: {"verified_claims": 0, "quotes_reconfirmed": 0, "downgraded_to_inferred": 0}
self_check_scope: "五份新增主线文件不新增正式源码窗；00保留旧前缀，新追加部分无正式源码窗；作者41、旧组合60和设计READY分账。"
full_local_P9: "NOT_RUN"
actual_G04: "PENDING_POST_PUBLICATION"
R159_01: "CLOSED_DOCUMENTATION_ONLY_WITH_C12_RECONSTRUCTION_NOTE"
D00_for_161: []
owner_decisions_requested_now: []
record_type: "independent_bounded_batch_review_and_historical_documentation_closure"
followup_id: "CORE-USER-LIBC-BOUNDED-STRING-OBJECT-GATES-160"
status: "PASS_PENDING_LOCAL"
batch159_composite: "PASS_PENDING_LOCAL_WITH_160_CORRECTION_AND_C12_RECONSTRUCTION_NOTE"
author_identity: "MYOS2-A-C11c / MYOS2-LEAD-002-CHAT-EXEC-27"
new_open_returns: []
author_new_P6: [41, 41, 0]
combined_159_P6: [60, 60, 0]
acceptance_basis: "160原六件；159原件加160 D00与本审§2确定性重建说明合读"
---

# 第160批独立验收及R159-01文档闭合

**160裁定PASS_PENDING_LOCAL；R159-01作限定文档闭合，161 D00=[]。** 固定作者提交80382a714198的三场景十一行与独立源码模型一致。159原RETURN和原报85/85/0保留；159原件与160 D00、本审§2合读，当前组合裁定PASS_PENDING_LOCAL。闭合只涉及证据分类/相应描述，不认证真实运行、链接或完整本地P9。[160作者](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7241284777) · [224合同](../224-user-libc-bounded-string-object-gates-contract.md) · [原159退回](CORE-USER-LIBC-FORMAT-ARG-CONSUMPTION-159-review.md)

## 1. 十一行独立重建

主线重新读固定__strnlen、STRNCPY、memset、MEMCPY及BYTE_COPY_FWD定义，按物理容量、地址余数和预算重建事件，后对完整作者案例账作160项投影，0失败。另一路只读辅助在读取作者案例账前封存源模型，逐行读写、状态、返回与DOT共1700项检查0失败；数量含结构/节点检查，不是1700个技术结论。合同和作者简报本来含结果，不声称主线盲审。

| 案例 | 主线独算的已发生操作与终点 |
|---|---|
| A160_01 | 先形成S+0，零预算返回0，无源读 |
| A160_02 | 读S[0]，形成S+1 one-past；下一L49读取前STOP，无返回 |
| A160_03 | 读S[0]/S[1]，后者预算外而对象内；char_ptr=end，返回1 |
| A160_04 | L35形成S+2超one-past，在形成前STOP，end未建立且无读取 |
| B160_01 | 起点0<end1成立，整窗[0,8)超cap7；L116前STOP，无载入/后增/mask |
| B160_02 | word0047464544434241、mask8000000000000000；整字8字节另确认0–7，夹到end1，返回1 |
| B160_03 | 首词4141414141414141、mask0；char_ptr=9、word_ptr=8；第二整窗[8,16)前STOP，无返回9 |
| C160_01 | 源扫描size3已得；D+3超cap2的one-past，形成前STOP，memset未调用 |
| C160_02 | D+3可形成cap3的one-past，进入memset，首个D[3]写前STOP，目标不变 |
| C160_03 | 仅补零D[3]，游标4/len1；下一写前STOP，目标7e 7e 7e 00，MEMCPY未到 |
| C160_04 | 依次写3、4、0、1、2，目标61 62 63 00 00；memset返回原实参D+3，MEMCPY/STRNCPY返回D+0 |

C四行同一源词7e7e7e7e00636261、mask0000000080000000，确认0–3；memset长度2<8、MEMCPY长度3<16，短字节路径范围明确。四个正常出口、七个对象门前停止。以上只在224明确的有限地址模型内成立；指针转换/整数游标、有效类型、实际编译器/ABI与真实内存行为未认证。[长度定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strnlen.c) · [STRNCPY](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strncpy.c) · [短复制宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h) · [作者账](https://github.com/08822407d/MyOS2/blob/80382a714198/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-bounded-string-object-gates-160/facts-and-boundaries.yaml)

DOT实测279节点=265事件+5返回数据端口+9总览；274边=254相邻LOCAL_CONTROL+10案例数据+10总览关系。七个STOP均无出边，无跨案例串接。STRNCPY拥有三个helper调用，MEMCPY拥有BYTE_COPY_FWD宏展开；C04正常返回guard和返回数据来源各自有证。不将函数间时序画成相互直接调用。

## 2. R159-01：实重建及限定闭合

主线从冻结159六件独立应用160的D00_correction，不只读作者总结。85个旧ID映射唯一；25转普通集合与224相同，余60正式。11个文本行选择器逐个核旧值（MANIFEST8、正文2、evidence总结1）；13个source_index文件选择器旧值及替代集合吻合；8处facts引用与13处正文消费定位完整。所有85原代码窗保留原字节，旧技术rows/source_graph及其余未列字段保持，DOT/deferred完全保持。

**组合视图为正式60/60/0、普通48；原件仍85/85/0、普通23。** 正式60窗227源物理行=156 CRLF+71 LF；25转普通40行=38 LF+2 NONE，连旧普通53 LF成为48窗93行（91 LF+2 NONE）。25项转普通不是降为INFERRED；160本批新41窗与旧60严格分账。

### 确定性重建补注（与作者D00合读）

facts_projection说明保留被移动记录原字段并追加classification/reason。为使他席按相同文本字节重建，本主线明确采用以下序列：按原evidence_index顺序移出25项、原ordinary_location_index的23项原序保留；每个新普通对象先保留原键序和值，再追加classification=`ORDINARY_LOCATION`；reason使用类别短语 `extern declaration`、`type declaration` 或 `user CMake text`，完整原因仍在classification_map.reason中；应用13个source_index替换，其余字段不改；JSON两空格缩进、非ASCII保持原字符、末尾一个LF。

这是重建字段表示的明确化，未改变分类或技术事实。辅助首次把完整原因而非类别短语放入reason，只有facts文本指纹不同；定位后按上述约定全部六件精确重现。主线用自己的分类集合和重建助手也复现六件，不以该本地假设差异制造新退回项。旧文件不改、新作者不补签历史。

| 组合阅读文件 | FNV1a64 UTF16（重建文本，非Git Blob） |
|---|---|
| MANIFEST.md | 5ede7df9c36604a3 |
| libc-format-arg-consumption.md | 74f034274666a4c2 |
| facts-and-boundaries.yaml | c830194cd0d9aae6 |
| libc-format-arg-consumption-paths.dot | c7cd4f01a8ed88e0 |
| deferred-findings.yaml | eaed2372a1377b49 |
| evidence.md | 9a38912d19f98068 |

E057/E062仅支持声明，printf行为由既有函数体及_IO_FILE结构成员支撑，用户构建定位不证明实际链接。**R159-01 CLOSED_DOCUMENTATION_ONLY_WITH_C12_RECONSTRUCTION_NOTE**；159当前合读通过，原159 RETURN、85原窗/历史自报及所有原件不倒改。R152/R155/R158既有闭合保持。

## 3. 160新P2/P6与远端实物

新P6字面分布0/0/0/0/0/41，与41/41/0一致；41窗=30函数定义+9宏定义+2个P4配置，107源物理LF，均1–5行且原字节/实际归属吻合。19普通定位45物理行=43 LF+2 NONE，声明、typedef、用户CMake没有混作正式锚；无LF源尾与Markdown围栏分隔LF分别核。可达P1/P2/P6和强制锚成立，完整本地P9仍NOT_RUN。

| 固定六件 | 字节 | LF | Git Blob短12 |
|---|---:|---:|---|
| MANIFEST.md | 14768 | 115 | 6f4fe453b1b7 |
| libc-bounded-string-object-gates.md | 5236 | 49 | d38a14eb047f |
| facts-and-boundaries.yaml | 255127 | 9459 | 00ef23a810c2 |
| libc-bounded-string-object-gates-paths.dot | 41226 | 581 | 7dcc8b17b3d0 |
| deferred-findings.yaml | 3287 | 97 | 508b7da5e9fd |
| evidence.md | 19722 | 654 | 7c1cf129569b |

合计339366字节、10955 LF、0 CRLF，均末LF。固定80382a714198、活动exec及API Blob全文三渠道相同，独算Git Blob一致；唯一父ff7c39dea3dd，compare一提交六新增零其它改删。全部2283旧执行非tree及1849主线非tree冻结，当前执行2688项/2289非tree；当前lead2084项/1849非tree。

fresh四refs：lead6f860774cf10、exec80382a714198、timea039d9803ade、直接mastere8e60cd19540。PR16原Open/Ready、PR17原标题Open/Draft，未合并，开放页2/0。评论无since100/100/100/16/0，316唯一，79既有正标记各一次且真实ID正确，22负标记0；160作者与发令各直接双读/分页同文同时间。旧315评论与主线保全原记录吻合，FNV bf526261e0d42f02也与作者基线一致。两time金丝雀命中、master不命中、公约原句成立。作者G04回传与本次独立重核分别记账。

## 4. 接续与限制

Owner报告C11c耗尽上下文，下一批新开对话。C12准备[225交接包](../225-executor-continuation/00-C11c-to-C12a-handoff-package.md)和[226／161](../226-user-libc-compare-read-order-contract.md)，新席MYOS2-A-C12a／EXEC-28；160作者仍属C11c／EXEC-27，未公开工作UNKNOWN。新方接收/加载/启动尚未观察，不让旧方补签，不跨席继承已读。

本轮主线新P6=0/0/0，161 D00=[]。普通历史UNKNOWN不因小模型闭合；完整本地P9、项目执行、预处理、C/ASM、编译链接、Graphviz、QEMU/实机及真实I/O均NOT_RUN，后台/effort未认证。本文件发表时实际G04待随后终检，最终状态由真实回读决定。
