---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
conversation_display_name: "MYOS2-A-C12 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（本轮系统身份；实际后台和effort未独立认证）"
actual_backend_model: "UNATTESTED"
actual_reasoning_effort: "UNATTESTED"
date: "2026-11-28"
date_timezone: "Asia/Shanghai"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "12535d98d47d"
execution_pin_short12: "ff7c39dea3dd"
read_channel: "GitHub connector；自写有限文本/字节/整数核验"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\", "\tthis_cpu = smp_processor_id();"]
self_check: {"verified_claims": 0, "quotes_reconfirmed": 0, "downgraded_to_inferred": 0}
self_check_scope: "本轮三份主线材料不新增正式源码引文；159原85窗回核、160设计就绪与未来作者P6分账。"
full_local_P9: "NOT_RUN"
actual_G04: "PENDING_POST_PUBLICATION"
R159_01: "OPEN_DOCUMENTATION"
R158_01: "CLOSED_BY_MAINLINE_GRAPH_CONSUMPTION_ADDENDUM_RETAINED"
owner_decisions_requested_now: []
record_type: "independent_bounded_batch_review"
followup_id: "CORE-USER-LIBC-FORMAT-ARG-CONSUMPTION-159"
status: "RETURN_DOCUMENTATION"
technical_result: "TEN_ROWS_SOURCE_MODEL_CONSISTENT"
author_identity: "MYOS2-A-C11c / MYOS2-LEAD-002-CHAT-EXEC-27"
author_original_P6: [85, 85, 0]
source_raw_windows_reconfirmed: 85
D00_for_160: ["R159-01"]
formal_acceptance: "PENDING_ADDITIVE_DOCUMENTATION_CORRECTION_AND_MAINLINE_REVIEW"
---

# 第159批独立验收：技术十行成立，证据分类退回补正

**裁定 `RETURN_DOCUMENTATION`；R159-01开放，交第160批D00处理。** 固定执行提交 `ff7c39dea3dd` 的三个场景十行，参数消费、源读、目标写入、两个首次停止点及三条printf请求均与本主线源级重建一致。原85个引文窗逐字命中并不等于85个都满足定义体资格；因此本次不将原六件整体签为PASS。作者提交、主线技术判断、文档退回和发布后G04是分别记账的状态。[作者回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7212991140) · [固定六件](https://github.com/08822407d/MyOS2/blob/ff7c39dea3dd/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-format-arg-consumption-159/MANIFEST.md) · [222合同](../222-user-libc-format-arg-consumption-contract.md)

## 1. 独立重建及其真实限界

C12已读222合同及作者简报，从固定源 `vsnprintf`、`printf`、`strlen` 定义和明确GIVEN重建十行，封存有限字节/整数模型后再对读作者完整案例账；合同中已有期望表，故不声称盲审。主线逐行投影95项全部吻合；另一只读辅助任务独立回源重建全部读写、状态和图关系，亦未发现语义差异。自写Python只处理有限字节、整数、文本和散列，没有执行项目C/ASM或脚本。

共同前提限于合同所给ASCII、int32、64位long/size_t/指针、小端、有效且类型匹配的va_list、独立存活对象及给定整字读取语义。C的write正常返回0只是到达printf返回点的GIVEN。实际ABI、有效头文件、链接、真实调用者和真实输出均未认证。

| 案例 | 主线重建结果 | 边界与消费 |
|---|---|---|
| A159_01 | 写 `5b 5d 00`，q/o/return=2 | 精度0仍先完成strlen的整字读及0–3确认，再裁剪；取2参 |
| A159_02 | 写 `5b 61 62 5d 00`，q/o/return=4 | 先strlen，再额外读a、b；取2参 |
| A159_03 | 写 `5b 61 62 63 5d 00`，q/o/return=5 | 精度-1规范化为无限制，额外读a、b、c；取2参 |
| A159_04 | 停止前仅D[0]=5b，q/o=1，无返回 | S[0]已读，S+1合法形成；在strlen L40下一次解引用前停止，未到精度裁剪 |
| B159_01 | 写 `5b 20 20 20 61 62 5d 7c 5a 00`，return=9 | width5，先补3空格；按序取宽度、精度、两指针 |
| B159_02 | 写 `5b 61 62 20 20 20 5d 7c 5a 00`，return=9 | width-5先取反再置FL_MINUS，后补3空格；第二转换重置状态 |
| B159_03 | 停止前仅D[0]=5b，q/o=1，无返回 | 仅取INT_MIN；在L614不可表示的取反前停止，flags仍0，后3参未消费 |
| C159_01 | 请求 `41 00 42`，count=3，printf return=3 | 数据NUL在1，收尾NUL在3且不在请求内 |
| C159_02 | 请求 `41 42`，count=2，printf return=2 | 精度0仍取int、建立carg，随后零次字符读取；无数据NUL |
| C159_03 | 请求 `41 20 20 00 42`，count=5，printf return=5 | 先两空格，数据NUL在3，收尾NUL在5 |

[固定格式定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c) · [printf定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/printf.c) · [strlen定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strlen.c) · [作者案例账](https://github.com/08822407d/MyOS2/blob/ff7c39dea3dd/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-format-arg-consumption-159/facts-and-boundaries.yaml)

图有471节点（445事件、7返回端口、19总览）、476边（435条相邻LOCAL_CONTROL、23数据边、18总览边）。相邻seq均为控制顺序，节点种类单列；两个STOP没有正常出边；三条write请求到printf返回的边有正常返回guard，rv来源另以数据边表达。159遵守了158图义补注，R158-01既有闭合保持；本轮不重启158。

## 2. R159-01：逐字命中与定义体资格应分账

85原窗逐字、行界、1–5物理行及原行尾均吻合：源物理行267=156 CRLF+109 LF+2 NONE；23原普通定位另有53 LF行。原六件正式标签字面计数为0/0/0/0/0/85，原MANIFEST 85/85/0的数字一致；P1无禁止长哈希。但其中5条extern显然位于所标函数定义体外；4条独立类型声明也应按普通定位消费，16条用户CMake不能作C定义体正式锚。保留2个内核options配置窗是协议P4的明示要求，与用户侧构建定位分开。

依据：[协议v2 P2/P4/P9](../../../tasks/00-gpt-task-protocol-v2.md)；[126 D126-R125-01既有构建分类补正](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill.md#L40-L74)；[126主审对R125-01的文档闭合](CORE-USER-LIBC-MEMORY-COPY-FILL-126-review.md)。222§7将强制覆盖的声明/用户构建与正式锚放在同一长句，措辞存在混杂；主线在此澄清：这些链路仍必须取证，取证覆盖不等于全部使用正式定义体标签。此澄清落实原协议和既有补正，不新增通用规范，也不将合同表达问题全归责作者。

| 原ID | 固定time位置 | 更正后的证据类别 |
|---|---|---|
| FA159-E055 | `myinitramfs/mylib/myglibc/string/string.h::strlen` L405–405 | 普通定位：活动extern声明 |
| FA159-E056 | `myinitramfs/mylib/myglibc/include/stdio.h::printf` L70–70 | 普通定位：活动extern声明 |
| FA159-E057 | `myinitramfs/mylib/myglibc/libio/stdio.h::stdout` L131–131 | 普通定位：活动extern声明 |
| FA159-E058 | `myinitramfs/mylib/myglibc/libio/stdio.h::vsnprintf` L367–367 | 普通定位：活动extern声明 |
| FA159-E059 | `myinitramfs/mylib/myglibc/bits/types/FILE.h::FILE` L8–8 | 普通定位：类型别名/条件类型声明 |
| FA159-E061 | `myinitramfs/mylib/myglibc/posix/unistd.h::ssize_t` L216–219 | 普通定位：类型别名/条件类型声明 |
| FA159-E062 | `myinitramfs/mylib/myglibc/posix/unistd.h::write` L377–377 | 普通定位：活动extern声明 |
| FA159-E064 | `myinitramfs/mylib/myglibc/posix/bits/types.h::__ssize_t` L193–193 | 普通定位：类型别名/条件类型声明 |
| FA159-E067 | `myinitramfs/mylib/myglibc/posix/sys/types.h::ssize_t` L105–108 | 普通定位：类型别名/条件类型声明 |
| FA159-E068 | `myinitramfs/CMakeLists.txt::set(CMAKE_C_FLAGS)` L26–29 | 普通定位：用户侧构建文本 |
| FA159-E069 | `myinitramfs/CMakeLists.txt::if(LINK_TYPE)` L31–33 | 普通定位：用户侧构建文本 |
| FA159-E070 | `myinitramfs/CMakeLists.txt::set(MUSL_PATH)` L40–40 | 普通定位：用户侧构建文本 |
| FA159-E071 | `myinitramfs/CMakeLists.txt::set(CMAKE_C_COMPILER)` L41–41 | 普通定位：用户侧构建文本 |
| FA159-E072 | `myinitramfs/CMakeLists.txt::set(MUSL_INC_PATH)` L43–43 | 普通定位：用户侧构建文本 |
| FA159-E073 | `myinitramfs/CMakeLists.txt::include_directories(MUSL_INC_PATH)` L46–46 | 普通定位：用户侧构建文本 |
| FA159-E074 | `myinitramfs/CMakeLists.txt::add_subdirectory(myinit)` L47–47 | 普通定位：用户侧构建文本 |
| FA159-E075 | `myinitramfs/CMakeLists.txt::add_subdirectory(myshell)` L48–48 | 普通定位：用户侧构建文本 |
| FA159-E076 | `myinitramfs/myshell/CMakeLists.txt::add_executable(sh)` L4–4 | 普通定位：用户侧构建文本 |
| FA159-E077 | `myinitramfs/mylib/CMakeLists.txt::add_subdirectory(myglibc)` L1–1 | 普通定位：用户侧构建文本 |
| FA159-E078 | `myinitramfs/mylib/myglibc/CMakeLists.txt::add_library(mylibc)` L1–1 | 普通定位：用户侧构建文本 |
| FA159-E079 | `myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories(mylibc)` L3–7 | 普通定位：用户侧构建文本 |
| FA159-E080 | `myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory(stdio)` L18–18 | 普通定位：用户侧构建文本 |
| FA159-E081 | `myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory(string)` L20–20 | 普通定位：用户侧构建文本 |
| FA159-E082 | `myinitramfs/mylib/myglibc/stdio/CMakeLists.txt::target_sources(mylibc)` L3–3 | 普通定位：用户侧构建文本 |
| FA159-E083 | `myinitramfs/mylib/myglibc/string/CMakeLists.txt::target_sources(mylibc)` L3–3 | 普通定位：用户侧构建文本 |


组合阅读视图的目标为：**60正式=E001–E054、E060、E063、E065、E066、E084–E085；48普通=原23+上述25**。60包括58个定义窗及2个P4活动配置窗。历史原件及作者回执中的85/85/0保留为当时申报；不得写成“原件实际只有60”，也不得写85/60/25，因为这25项转普通定位而非INFERRED。

D00需要可重建的逐项替代阅读视图：保存25个旧ID/原窗/定位，变更分类；明确旧MANIFEST计数字段、coverage/P9声明、正文末段、evidence总结、facts证据索引及所有引用这些ID的用途。保留类型/声明/构建本身的源码事实和UNKNOWN，不把声明支持的签名升级成函数实现或ABI证明。其余60窗、十行案例、字节账、图控制关系和旧deferred技术边界保留。原六件/旧评论一字节不改；160的新P6只统计160实际新增正式引文，旧60更正视图单列。细化执行见[224合同](../224-user-libc-bounded-string-object-gates-contract.md)§2。

## 3. 实物、冻结及可达核验

执行提交唯一父 `bf0a7ea26dcc`，compare前进1、落后0，仅六件新增。原父2277个非tree对象的mode/type/Blob全保持；当前执行树2681项/2283非tree。六件固定提交、活动分支和API Blob全文三渠道一致，独算Git Blob亦同。

| 文件 | UTF-8字节 | LF | Git Blob短12 |
|---|---:|---:|---|
| MANIFEST.md | 15931 | 110 | 1ede3ffb4890 |
| libc-format-arg-consumption.md | 5737 | 44 | 25cbdf03de7b |
| facts-and-boundaries.yaml | 412060 | 17510 | 687fcb5f5aeb |
| libc-format-arg-consumption-paths.dot | 69008 | 973 | 006622eaa67b |
| deferred-findings.yaml | 3208 | 86 | 4dbad4667bac |
| evidence.md | 29736 | 1170 | 0174a1bcc889 |

合计535680字节、19893 LF、156 CRLF，六件末尾均LF。当前lead `12535d98d47d`、exec `ff7c39dea3dd`、time `a039d9803ade`、直接master `e8e60cd19540`；lead2081项/1846非tree。PR16 Open/Ready、PR17 Open/Draft，均未合并。PR17无since分页100/100/100/14/0，共314唯一评论；76既有精确标记各1且ID吻合，19负标记0。159作者及159发令按ID双读与分页同文同时间。159前313条评论精确指纹900a961fc8b1f6da与作者入场账一致，旧正文和两时间没有变化。两条time金丝雀命中、直接master不命中，公约原句一致。

以上重核支持作者远端交付完整及旧对象冻结；不能用作者自报G04替代本次核验，更不能用远端G04替代P2资格判定。完整本地P9仍NOT_RUN；可达核验中P2资格失败已明确导致本次RETURN。

## 4. 当前终点与接续

上轮中断后，实际remote只到C12恢复提交12535d98d47d；没有159主线评论或160发令。2026-11-28续办本次独审和160设计，既有模型缓存经固定身份核对后继续消费。Owner提供的界面提示只表明回复显示被风险提示拦截，没有可见日志定位到159或160，也未提供可据以归因的具体片段；本P2发现不能解释该提示。

本次正式评论发表时G04仍待发表后实核。唯一后继是224／160，由原C11c／EXEC-27完成D00及三个相关场景十一行六件。159数值无需重做；R159-01只在后续主线实际审过补正后可关闭。R152/R155/R158既有闭合和旧RETURN历史不倒签。主线新P6=0/0/0；真实后台/推理档、完整本地P9、预处理、编译链接、Graphviz、QEMU、实机及真实I/O均未认证或NOT_RUN。
