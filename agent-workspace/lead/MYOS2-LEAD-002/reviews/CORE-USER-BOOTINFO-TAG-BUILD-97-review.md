---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-BOOTINFO-TAG-BUILD-97-REVIEW-001
record_type: independent_bounded_static_acceptance_with_consumption_supplements
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（正式独立验收及后继设计）
date: 2026-11-17
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract139_short12: b878cc3784da
reviewed_base_short12: 6a1fa78d0356
reviewed_head_short12: f2df1e04cea8
reviewed_delivery_comment: 6944267731
executor: MYOS2-A-C06e / MYOS2-LEAD-002-CHAT-EXEC-14
status: PASS_PENDING_LOCAL
acceptance_verdict: PASS_PENDING_LOCAL
mandatory_supplements: [R97-S01, R97-S02, R97-S03, R80-S01, R80-S02, R80-S03, R96-S01, R96-S02, R96-S03]
returned_items: []
new_D00_required: false
R96_01_02: CLOSED_DOCUMENTATION_ONLY_BY_SEPARATE_RECEIPT
batch96_composite: PASS_PENDING_LOCAL
original_author_self_check: 85/85/0
reviewer_readable_source_label_count: 85
self_check: {verified_claims: 6, quotes_reconfirmed: 6, downgraded_to_inferred: 0, scope: 本回执六段短锚，与原85条分账；可读核对不是自动符号扫描}
original_six_automatic_regex_JSON_DOT_scan: NOT_RUN
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
next_followup: CORE-USER-BOOT-ELF-LOAD-98
next_contract: ../140-user-boot-elf-load-contract.md
executor_rotation: false
phase: 2
inputs_read: [97六件及正式评论全文, 139合同及规则, 96回执与D00支持链, 80正式回执, 固定源码及有限独立整数核对]
owner_decisions_requested_now: []
open_questions: []
---

# 第97批按限定接收；没有新增RETURN

97按本件R97-S01/S02/S03限定接收为PASS_PENDING_LOCAL。主线直接追加局部边界、支持关系和图示消费纠正，不改97原件、不要求重做整包。96的两项文档关闭另见CORE-USER-FRAMEBUFFER-MAPPING-96-DOC-CLOSURE-review.md；关闭旧问题、接收97、发布98、启动98是四种不同状态。

## 1. 交付与检查范围

执行头及作者评论6944267731对应f2df1e04cea8。compare 6a1fa78d0356→f2df1e04cea8：ahead1/behind0、一个单父提交、严格六件added3920行，MANIFEST162、正文60、facts2247、DOT163、deferred547、evidence741，旧件零净改删。PR16/17仍Open/unmerged，PR17保持原title/base/Draft，master与time直接头分别e8e60cd19540、a039d9803ade。

六件分段阅读全文至EOF，长响应的正文、facts、evidence缺段另行补齐；全部85个短锚已读，结构与独立typedef分锚正确。D00的有效支持和失效历史展示分开；不能因为facts引用旧坏串用于说明错误，就再次当它是新增有效证据引用。未见新的定义外拼接或规范三整数缺失。MANIFEST十四字段、逐字公约、双分支canary与connector通道可读核对，P9仍只作明确的可读子集，不声称运行了完整机械P9。

8组51矩阵行、20隔离模型、8候选和9缺口、55节点87边是作者成果的不同栏目，不相加为测试数。主线独立算术核3920行、51行、85分类加总、131072字节、对齐16/零步长、320/112/88和u32窄化；没有运行或翻译仓库函数、作者模型或检查器。作者85/85逐字程序检查、接收/指导刷新临时文件是作者申报，主线不认证另一聊天后台。

97执行的是138/139恢复的BOOTINFO-TAG-BUILD范围，旧136/137替代的MBI寄存器复制/BSS任务没有恢复授权。后继98同样不借历史错误导航改回该范围。

## 2. R97-S01：第129项的停止点须消费R80-S01，不复活原误判

97 facts.B-PUB-05与BT97-M12把进入i=128、形成entries[128]地址本身描述为已非法。80原BM80-F03中该说法早已被R80-S01收窄，97必须消费正式回执而不是只复制原问题条目。

[VERIFIED mykernel/arch/x86_64/mm/early/e820.c::e820__memory_setup]
```c
		e820_entry_s *e820_entp = &(e820_table->entries[i]);
		e820_entp->addr = mmap_entp->addr;
		e820_entp->size = mmap_entp->len;
		e820_entp->type = mmap_entp->type;
```

正确限定：对合法128元素数组，i=128形成尾后一地址本身不等于对第129个元素读写；随后成员访问/写入缺少合法目标元素，正常执行证明在这里停止。假设第129条源记录及前128次复制都合法，仍支持BM80-F03的容量候选，但不能声称随后nr_entries正常变成129。N=128另有合法可读零哨兵时只写0..127，然后可以发布128。源记录是否可读、零哨兵、目标数组容量与真实运行各自分账。

替代消费者包括正文B的容量解释、B-PUB-05、BT97-M11/M12、ABC-02、BT97-U06及图e820/cap→old80。图的entries[128]只可读作容量128，不是最后合法下标128。BM80-F03原身份与needs_evidence保持，不能把主线的纠正另编一个新内核根因。

零步长M03/M04只是游标算式；实际builder是否能生成该size、当次tag是否可读与后续覆盖仍须先证。u32回绕本身不是CPU异常；M10的循环条件除零还以第二次GetMemoryMap未走错误停留分支及此前存储合法为前提。M17的core_num窄化发生于CPU循环之前；超大循环会碰到容量/对象门，不允许据此推导完整CPU tag已经发布。

## 3. R97-S02：原文支持链、记录发布与数值条件

B-MAP-06及正文RAM类别列表不能仅靠BT97-E033的末三项证明全部分支。须合读完整fill_mmap_info；缺少的前五个case为同一switch中的连续源码：

[VERIFIED myloader/uefi/bootloader.c::fill_mmap_info]
```c
			case EfiLoaderCode:
			case EfiLoaderData:
			case EfiBootServicesCode:
			case EfiBootServicesData:
			case EfiRuntimeServicesCode:
```

与原E033的EfiRuntimeServicesData/Conventional/Persistent及MemType=1合读，才支持完整类别列表。这里只陈述本loader的分类表达，不证明这些区域在任何真实服务阶段都可以回收。类别、策略正确性、实际固件值与内存重用不是同一结论。

B-MAP-09真实顺序为写addr/len/type→last_mb_mmap赋值→mb_mmap递增→last_end计算→e820_nr递增。矩阵的压缩叙述不能把计数移动到last_end之前。成员写、指针递增、端点计算和计数若有先行不成立之处，必须保留相应已执行前缀，不能用最终状态覆盖失败过程。

[VERIFIED myloader/uefi/multiboot2_utils.c::next_info_tag_addr]
```c
	return (mbi_tag_s *) ((UINT8 *) prev_info_tag +
				((prev_info_tag->size + 7) & ~7));
```

size+7按实际字段和整数提升计算；131072只是页申请预算，不是所有tag可写范围已证。M05的320字节是声明四个独立长度的算术示例，不是对当前ELF/固件实际sizeof的测量。u32 total_size的大跨度窄化也不能越过更早的32页容量门当成同次完整成功样例。

[VERIFIED myloader/uefi/bootloader.c::fill_mmap_info]
```c
	memmap_info->type = MULTIBOOT_TAG_TYPE_MMAP;
	memmap_info->size = sizeof(mbi_mmap_s) + sizeof(mbi_mmap_ent_s) * (e820_nr + 1);
	memmap_info->entry_size = sizeof(mbi_mmap_ent_s);
	return next_info_tag_addr(mb2_infotag_ptr);
```

e820_nr+1是int子表达式，外面的sizeof乘法不自动把此前的加法变宽；不过极大条目模型还须隔离更早的arena容量。额外一槽与entry_version/zero字段在初始清零成功、布局正确、无覆盖时可以保持零；没有在本函数重复写零不自动构成已确认bug。BT97-F04收存的是对初始化前提的依赖。输入原序合并、排序后未二次合并可保留；F03不能仅由“仍有相邻记录”推成协议违规或已发生故障。

## 4. R97-S03：服务阶段、图的真实调用者和问题身份

[VERIFIED myloader/uefi/bootloader.c::fill_mmap_info]
```c
	gBS->FreePool(MemMap);
	gBS->ExitBootServices(ImageHandle, MapKey);
```

固定文本确有在已取得MapKey之后调用FreePool，再用此前MapKey请求退出；后续ACPI/CPU、Print/Close位于该调用返回之后。这里只证明代码次序。没有读取适用外部EFI规范与运行状态，不能据此认证MapKey必失效、退出必成功/失败或所有后续函数都具有同一种返回类型。对SetMem等说“本地没有结果检查”不应补造成其返回一定为EFI_STATUS；不同服务接口要各自取证。

ACPI所谓“命中”仅指代码中的CompareGuid(...)==0及签名/Revision谓词满足并正常到达复制；本批未证CompareGuid的外部返回约定，不能把它直接等同GUID相等。无匹配后仍发ACPI_NEW头的结论要求函数正常走到末尾，不涵盖更早无效Root或服务调用停住的情形。

rsdp[0]、entries[0]、cpus[0]应称源码零长度数组尾部布局，不据此认证ISO C柔性数组[]语义或外部ABI。字段类型、实际分配的背后对象与相应字节长度仍需分别证明。

[VERIFIED myloader/uefi/bootloader.c::fill_cputopo_info]
```c
	gBS->LocateProtocol(&gEfiMpServiceProtocolGuid, NULL, (VOID **)&mpp);
	mpp->GetNumberOfProcessors(mpp, &nCores, &nRunning);
	smp_info->core_num = nCores;
	smp_info->core_available = nRunning;
```

图mp→count不是LocateProtocol这个被调用者又去调用GetNumberOfProcessors；两者由fill_cputopo_info顺序发起，后者使用取得的协议对象。mmap_tag/acpi_tag/cpu_tag→next是包含这些状态的实际fill函数发出的调用，不是结构体状态节点作为C函数。cpu_cons→nr应分parse_tag调用get_smpinfo与其内部nr_cpu_ids赋值。保留SOURCE_ORDER、INDIRECT_CALL和DATA_HANDOFF类别，图不认证全CFG或真实执行。

8个候选是8条收存记录，不等于8个互不重叠且已证的根因；F01和F07含共同arena容量依赖，F05与F08又有返回值和阶段依赖。保留原qualified_id并说明关系，不重新累计同根因或由文档接收自动关闭源码问题。内核ACPI本case局部指针及CPU本函数只读core_num的结论限于所读路径，不推广全仓无其它消费者。

## 5. 接收、后继和禁止事项

P9可读子集按上述限定消费；旧96两项闭合由单独回执确认。97无新增RETURN，98无D00，不让Owner核数、审代码或逐项决定修复。六段主线锚来自固定time原文，与作者85条分账；完整自动JSON/DOT/全85符号及引文扫描、完整本地P9、预处理/编译/链接、仓库脚本/程序、Graphviz/QEMU/实机、硬件与真实并发仍NOT_RUN。

98按140合同调查UEFI路径的文件读取与原始镜像范围、ELF64解析上下文、分配/段装入与入口发布，非用户态exec/解释器重审，非全重定位器审计；主线预读仅用于划界，不作为预定缺陷清单。继续现C06e/14次一档执行，完成后交原C06 Pro独立核收；不新开、不轮换，不联系退休执行者。发布98不等于执行者已收到或启动。