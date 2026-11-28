**范围恢复声明（执行优先级最高）**：原主线36f7c342dc88已发布的136/00、136/01与137-user-boot-info-transfer-contract.md属于一次非保真恢复，错误地将第97批改为MBI复制与BSS交接。该版97执行范围已由本138三件和139原定合同追加取代；原第95、96批审核裁定、证据与执行提交不变。本文件仅恢复上一轮Pro已准备的第97批技术范围，未重新进行Pro审核。实际发布提交由Owner提供的固定GitHub链接决定，不用旧头拼接。**

接收与执行模型：次一档（默认），沿用GPT-6＋Extra。新对话：MYOS2-A-C06e 代码分析执行；身份MYOS2-LEAD-002-CHAT-EXEC-14。必须通过138新接收入口，不再向旧C06d/13派发。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-BOOTINFO-TAG-BUILD-97
record_type: bounded_bootinfo_tag_build_contract
produced_by: GPT-6 Astra Pro（模型自报；后台UNATTESTED）
model_per_owner: Pro（主线独立验收与设计）
date: 2026-11-17
base_snapshot: time
source_pin_short12: a039d9803ade
read_channel: connector
result_base_short12: 6a1fa78d0356
lead_parent_short12: fda0f3d1cb60
fixed_contract_commit: ACTUAL_138_139_PUBLISHED_COMMIT_FROM_OWNER_FIXED_URL
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
executor_display_name: MYOS2-A-C06e 代码分析执行
executor_rotation: OWNER_REQUESTED_NEW_CONVERSATION
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-bootinfo-tag-build-97/
allowed_files: [MANIFEST.md, bootinfo-tag-build.md, facts-and-boundaries.yaml, bootinfo-tag-build-paths.dot, deferred-findings.yaml, evidence.md]
D00: REQUIRED_R96_01_R96_02_APPEND_ONLY
status_at_composition: PREPARED_NOT_PUBLISHED_NOT_STARTED
activation_gate: CONFIRMED_GITHUB_PUBLICATION_THEN_OWNER_FORWARD_138_AND_FRESH_RECEIVE_G00_PASS
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第97批：启动信息标签的构造、记录发布与交接边界

本合同不因在本地附件中出现而生效；须先由C06在原lead/PR16发布并完成导航、正式PR17评论和远端回读。缺少真实发布提交/评论时为DO_NOT_RUN，不得把父fda0f3d1cb60拼成新文件URL。正式发布后Owner把138/01固定入口发给新C06e，即要求通过接收门后连续执行，不再逐专题等待批准。

## 1. 已裁定状态与技术增量

95的R95-01/02由本轮独立闭合回执关闭，组合PASS_PENDING_LOCAL；不重做95，不重开旧70/70/0或文档误判。96仅R96-01/02定点RETURN，其余主体按R96-S01/S02/S03保留；先D00追加这两项，不重做整个96。

三个新专题：A共享MBI缓冲、标签指针推进与头尾发布；B EFI内存描述符到MMAP记录的有界转换、合并/排序及终止/版本/容量契约；C后续ACPI与CPU拓扑标签、服务调用阶段及实际直接消费者。补BM80和FM96输入来源缺口，不重复96帧缓冲算式、80完整e820/parse_tag审计、84根页表或完整UEFI启动。

普通问题分类入库，保留旧qualified_id，不要求Owner决定修复。预读只用于选择调查范围，不预定必须发现的缺陷或数量。

## 2. 固定材料与独立接收

L=agent-workspace/lead/MYOS2-LEAD-002/；R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。

新138三件、本139、96-review、95-DOC-CLOSURE、2026-11-17-framebuffer96-reviewed-bootinfo97-new-executor检查点，统一取正式发布后Owner固定URL的真实提交。110、124及九规则也取同一发布提交，不依活动分支漂移。

独立全文读取96固定6a1fa78d0356的R/user-framebuffer-mapping-96/严格六件和正式评论6941872312；读取本次主线正式发布评论（编号只从真实发布取得）。D00重开相关原条目和固定源码，不用旧执行者自检代替。

全文读取L/reviews/CORE-USER-BOOT-MEMORY-MAP-80-review.md，合读R80-S01/S02/S03及其适用旧限定；原80的MANIFEST.md、deferred-findings.yaml固定0e9e9e89c4be，位于R/user-boot-memory-map-80/。facts/evidence按BM80-F01/F02/F03/F04、U01–U07及真实依赖定点读取，不重做其全部模型。96接用的81–83回执不必为本97重新展开全套页表；凡实际消费其中结论则定位并合读相关正式限定，不把未读写成已读。

旧95六件be83c6095eb1、原95-review@fda0f3d1cb60，仅用于独立闭合回执指明的历史关系；95闭合回执必须全文读。91–94等停止门只按实际依赖合读，既有53撤回误判、90闭合均保持。旧134/135是13的历史接收/96合同，不是14的激活入口，不启动旧95/96任务。

按138先真实Mnemosyne接收报告，再独立全文九规则和110/124，再单独加载适用Mnemosyne指导，再新G00。九规则包含协议v2全部P1–P13、63全文、79全文。任何前任已读资格不转移；长响应补齐，本人已读同对象可明确复用。接收/刷新记录先真实临时保全，门后并入本六件，无第七件。

G00核四direct refs、PR16/17、开放PR、97全部目标及所有97正式marker。exec预期6a1fa78d0356，master e8e60cd19540，time a039d9803ade；lead是新138/139真实发布头。兼查退休13和新14及任意作者，未知推进先核归属，停相关写入、不覆盖、不自动变基。核公约整句和time/master双canary；配置与否定判断前全文读time options_flags.cmake。没有发布门就不准执行写入。

## 3. D00：只追加R96两项纠正

R96-01：完整重读multiboot_tag_framebuffer_common定义与紧随其后的独立typedef。将旧FM96-E036结构锚限制在定义内，别名另证；为source_quote_index、source_quote_recheck、正文、A-P06、M11、F04、图及其它真实消费者建旧新映射。保留原110/110/0，不倒造原全定义通过；不把独立typedef、尾部空白或合法嵌套结构都误判成错误。

R96-02：独立核FM96-F05原字段。把distinct_root文字说明、实际evidence_ids、prior_related_qualified_ids分开，确认E035/E041/E044对各事实的支持，不凭字段位置猜作者意图。撤回无保留索引全通过总括，对新六件作真实引用检查；每个证据ID指向实际短锚，每个旧qualified_id指向实际旧项。保留候选原身份，不把记录修复当源码修复。

本D00同时消费R96-S01/S02/S03，不重新签发95或96；只报CORRECTION_SUBMITTED_PENDING_C06_REVIEW。新97六件包含纠正对应表、真实短锚、消费者和检查范围，不额外交errata第七件、不修改96原件或旧评论。

## 4. A：共享存储、标签步进与头尾发布

完整读取myloader/uefi/bootloader.c::UefiMain和myloader/uefi/multiboot2_utils.c中的init_info_tag_addr、next_info_tag_addr、fill_info_tag_end、fill_info_tag_header；声明与条件include见bootloader.h。multiboot2.h读取相关整数别名、MBI头、普通tag、END/ALIGN常量及本包各载荷结构，不将结构后typedef夹进结构锚。

区分AllocatePages请求的page_count、pages输出、SetMem请求范围、初始tag地址、每次next返回、包含END的总长、total_size目标宽度和真实背后存储。不得假定外部服务成功或ABI sizeof；声明的局部数学假设与实际构建分账。定位实际PAGE_SIZE/PAGE_SHIFT来源，不因内核页宏值4096便默认loader所选头也已证。

按实际UINT32 size加7、按8对齐和指针推进顺序核正常大小、零/非进展、加法回绕、边界余量；不要用无限精度覆盖实际C类型。容量预算必须包含所有标签、填充与END，不以total_size最后赋值当事前容量校验。宏/函数未传容量与全库不存在保护是不同断言，限定所读范围。

交arena_and_tag_cursor_matrix、header_end_publication_matrix及必要有限模型。parse_mb2_header、ELF加载仅作为相邻未审代码，不展开完整镜像加载。

## 5. B：内存图记录转换、合并、排序与终止

完整读取fill_mmap_info、mbi_mmap_s/mbi_mmap_ent_s定义和相关标签步进；必要直接消费只追pre_init.c的ram_map赋值与e820__memory_setup入口对类型/长度/步长的使用。80原根因保留，新增生产者事实应明确收窄哪一子项，不能把整个BM80未知状态一次性关闭。

分开首次GetMemoryMap询长、容量增量、AllocatePool/SetMem、再次查询、错误分支和后续处理。DescriptorSize、MemMapSize、实际可读存储、记录数量、C索引类型及指针形成分别建前提。外部EFI服务/枚举语义无固定仓库定义时保持未证，不凭Linux或UEFI印象补结论。

按真实switch分类、NumberOfPages与字节换算、last_end、相邻同类型合并、不同记录追加和后续排序核查。特别区分输入相邻性与排序后相邻性、对象长度与结束地址、record count与字节容量；不预设输入有序、无重叠、可合并或某内存类型必须视为普通RAM。

检查size中e820_nr+1、entry_size、entry_version/zero字段的实际赋值或初始零来源、额外槽是否形成消费者可用哨兵；初始整区清零只有在服务成功/未越界/后续未覆盖前提下可消费。终止槽、128条内核目标容量和MBI总缓冲是不同界。交descriptor_conversion_and_coalescing_matrix、mmap_record_publication_matrix。

## 6. C：后续标签、调用阶段与交接消费

完整读取fill_acpi_RSDT_info、fill_cputopo_info及对应mbi_acpi_new_s/myos2_tag_smpinfo_s/myos2_cpudesc_s定义。追踪ACPI找到/未找到候选、匹配接口返回值、复制长度、零长尾数组的真实背后存储；CompareGuid/CopyMem/ZeroMem的外部定义未取得时不得凭名称裁定真假或数据合法。

CPU标签区分nCores/nRunning、core_num/core_available目标宽度、每条GetProcessorInfo调用/返回、cpus[i]写入、总size窄化及后续next；不扩成完整AP启动、拓扑语义或调度器。必要直接kernel消费者仅parse_tag/get_smpinfo/相关switch子句，不把局部读取提升为全系统消费认证。

建立UefiMain各fill调用以及fill_mmap_info内FreePool/ExitBootServices、返回后ACPI/CPU构造、尾标记/总头、后续打印/CloseProtocol/跳转的源码事件序列。服务名称、调用发生、状态成功及退出后的可用性是不同证明；所读源码可证明先后，外部规范需要另有精确一手来源才可作规范结论。不得运行任何服务、尝试实际退出固件，或扩大成全UEFI合规审计。

区分固定tag生产路径与GRUB等其它启动路径；不把未读路径当相同生产者。交auxiliary_tag_extent_matrix、service_stage_and_handoff_matrix及A/B/C接合表，给出最早未证门与可继续的独立子项。

## 7. 六件、证据和质量门

唯一六件为本头allowed_files。正文先说结论/限制/D00待复核；facts含旧新对应、上述矩阵、有限模型、真实输入账和证据/图索引；deferred只新增BT97-Fxx/Uxx和旧项有证增量。旧BM80/FM96/CN95按完整qualified_id保留，不把每个新消费者都算独立根因。

强制短锚覆盖D00两项、共享存储及步进/头尾、mmap获取/转换/合并/排序/发布、ACPI/CPU实际写入与服务阶段、必要消费者。宏单独取锚；字面、符号、单一定义、证据类别、语义支持分别核。声明可作声明证据，不能冒充对象/函数实现。

软限24份源码、8组定点检索、4次相关目录浏览；新短锚含D00上限110、有限模型含D00上限24、新候选/缺口各最多10。为上限不是产量指标；确需直接依赖超限时解释，不全树漫游、不以长文或模型数量冒充工作量。

MANIFEST十四字段直接齐备；self_check必须直接包含verified_claims、quotes_reconfirmed、downgraded_to_inferred三个真实整数。按六件全部原始标签计数（含D00、说明和字符串），不以构造器预期数替代。证据ID格式正确还不够，必须解析到实际锚且真正支持对应命题；distinct_root是有意义的文字，历史链接不塞入evidence_ids。

适用协议v2全部P1–P13；按P9逐项记账。完整本地P9/运行没做就NOT_RUN，不把独立文本/数学检查当执行了仓库实现。已知错误须发布前纠正，不能靠主线事后兜底。

## 8. 写权、回读与停点

14在正式发布及接收/G00全部通过后，仅在原执行分支新增本97六件及本人一次PR17顶层评论。marker：MYOS2-LEAD-002-CHAT-EXEC-14 / CORE-USER-BOOTINFO-TAG-BUILD-97 / FORMAL_DELIVERY。C06不并写results，旧13及更早执行者停用。

写前重核完整exec HEAD、六件目标和全部97 marker，单父提交、完整expected_sha、force=false；未知推进不覆盖、不自动变基。暂态按63先查远端效果再间隔安全重试，复用成功对象，恢复只补缺项，不重复评论。

发布后compare严格六件added、旧件零净改删；固定全文/活动内容或blob身份、PR状态、四refs、评论实际编号/全文/唯一性全部回读。完成后READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、R96纠正待C06复核，STOP_WRITING；交原C06，提醒Owner正式验收消息前选Pro，同轮审核补正、97及后继。不自签96闭合/97通过，不启动98。

禁止修改旧成果/回执/评论、PR元数据、lead/PR16、master/time、内核/loader/initramfs/Owner脚本；禁止补丁、配置/预处理/汇编/编译/链接、仓库程序/脚本/夹具、Graphviz/QEMU/实机/真实并发、外部libc镜像、本机凭据、额外模型/子代理/付费API、新分支PR、合并关闭强推删除、阶段3及跨项目维护。110与124继续进入以后主线交接。
