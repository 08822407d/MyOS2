---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_type: post_publication_successor_scope_restoration
conversation_display_name: MYOS2-A-C06 内核分析主线续建
date: 2026-11-17
read_channel: GitHub_connector_and_original_Pro_prepared_local_package
source_pin_short12: a039d9803ade
mainline_before_restoration: 36f7c342dc88
execution_head: 6a1fa78d0356
master_direct: e8e60cd19540
time_direct: a039d9803ade
prior_mainline_comment: 6943032927
release_commit: OWNER_FORWARDED_ACTUAL_RESTORATION_COMMIT
formal_restoration_comment: RECORD_AFTER_GITHUB_PUBLICATION
batch95: PASS_PENDING_LOCAL_R95_01_02_CLOSED
batch96: RETURN_R96_01_02_ONLY
batch97: CORRECTED_SCOPE_PREPARED_NOT_STARTED
executor: MYOS2-A-C06e / MYOS2-LEAD-002-CHAT-EXEC-14
prior_executor: MYOS2-A-C06d / MYOS2-LEAD-002-CHAT-EXEC-13_STOP_WRITING
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
---

# 97任务范围恢复：新增发布，不改旧成果

## 出错的恢复与此修正的边界
Owner误在另一临时次一档聊天向C06发布恢复指令。该次GitHub恢复实际成功：lead 36f7c342dc88，PR17主线正式评论6943032927；其95两项文档闭合及96仅R96-01/02退回的结论与上一轮Pro已完成的独立审核相符，继续有效；不需要再消耗一次Pro审核。

但是该次发布未忠实恢复上一轮Pro准备的第97批合同：原定CORE-USER-BOOTINFO-TAG-BUILD-97及其A共享MBI缓冲/标签头尾、B EFI MMAP描述符转换合并排序、C ACPI与CPU拓扑标签，被更换成了CORE-USER-BOOT-INFO-TRANSFER-97及A容器、B寄存器与复制、C BSS/早期交接。原准备包列七份，实际恢复仅发布六份，也未保留原136/02授权/检查点。该差异并非普通排版或路径更名，不能把旧136/01当成原任务的执行入口。

本次只追加恢复材料，不删除、不重写或强推已发布的六份；旧136/00、136/01及137-user-boot-info-transfer-contract.md保留作历史非保真恢复证据，**不再是第97批执行依据**。本138/00、138/01、138/02及139-user-bootinfo-tag-build-contract.md是原Pro冻结技术范围的恢复本；文本仅正规化内部136→138、137→139路径和增加这份明确的优先级警示。旧95/96六件、回执、评论、固定源码一概不改。

## 唯一有效的第97批执行安排
- 新会话：MYOS2-A-C06e 代码分析执行；新身份MYOS2-LEAD-002-CHAT-EXEC-14；次一档GPT-6＋Extra。
- 新138/01是唯一启动入口，必须完整合读138/00、138/02、139原定技术合同、本检查点、当前96回执与95闭合回执、110/124、63/79、协议v2 P1–P13、九份规则和其它明确固定证据。
- 先真实Mnemosyne接收报告、独立九规则与110/124、单独适用指导，再新G00；全部门通过后连续D00、G01–G04，不另等指令。
- D00仅R96-01 E036跨定义和R96-02 FM96-F05字段/引用错位；不重做96或重开95。
- 三新专题只以139为准：A缓冲与tag步进/头尾；B EFI内存描述符至MMAP合并/排序及终止/容量；C ACPI/CPU标签及服务调用阶段、必要消费者。
- 执行HEAD预期6a1fa78d0356；仅在claude/dazzling-cori-q0dnyt新增agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-bootinfo-tag-build-97/严格六件及14本人唯一PR17正式评论，marker = MYOS2-LEAD-002-CHAT-EXEC-14 / CORE-USER-BOOTINFO-TAG-BUILD-97 / FORMAL_DELIVERY。
- G00兼查两个97任务名的目录和marker及旧执行者，不覆盖未知推进。master/time直接头不变、PR16/17保持Open，PR17 Draft。不创建分支、改内核、运行脚本/构建/QEMU/实机，不改旧文档和PR元数据。GitHub暂态按63先回读再安全重试，expected-head、force=false。
- 第97批此前仅发布了错误范围的任务，**并无作者执行/交付**；本恢复发布与Owner转发后方可启动新138/01。完成正式交付与全部回读后STOP_WRITING，交原C06主线Pro独立审核，不自签96闭合/97通过、不启动98。

本轮是次一档的发布内容一致性修复，不构成对95或96技术结论的重新审查。原Pro准备文本的具体A/B/C、静态证据门及跨项目禁令保留在139合同；本轮非内核或loader修复，完整本地P9和真实运行仍NOT_RUN。
