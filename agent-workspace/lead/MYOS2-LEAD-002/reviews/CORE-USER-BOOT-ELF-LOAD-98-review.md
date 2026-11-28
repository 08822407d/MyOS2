---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-BOOT-ELF-LOAD-98-REVIEW-001
record_type: independent_bounded_static_acceptance_with_consumption_supplements
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（正式独立验收及后继设计安排）
date: 2026-11-17
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract140_short12: 280b7b49a521
reviewed_base_short12: f2df1e04cea8
reviewed_head_short12: 39b28e36e8f6
reviewed_delivery_comment: 6951092059
executor: MYOS2-A-C06e / MYOS2-LEAD-002-CHAT-EXEC-14
status: PASS_PENDING_LOCAL
acceptance_verdict: PASS_PENDING_LOCAL
mandatory_supplements: [R98-S01, R98-S02, R98-S03, R97-S01, R97-S02, R97-S03]
returned_items: []
new_D00_required: false
original_author_self_check: 103/103/0
reviewer_readable_source_label_count: 103
self_check: {verified_claims: 7, quotes_reconfirmed: 7, downgraded_to_inferred: 0, scope: 本回执七段固定源码短锚；可读核对，与原103条分账}
original_automatic_JSON_DOT_regex_symbol_scan: NOT_RUN
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
next_followup: CORE-USER-UEFI-BUILD-PROVENANCE-99
next_contract: ../141-user-uefi-build-provenance-contract.md
executor_rotation: false
phase: 2
inputs_read: [98六件与正式评论全文, 140合同及97正式回执, 协议v2及79/110/18, 固定源码回源, 公约及双分支金丝雀, 本有效C06同对象历史规则明确复用]
owner_decisions_requested_now: []
open_questions: []
---

# 第98批限定接收；无新增返工

第98批裁定PASS_PENDING_LOCAL，必须合读R98-S01/S02/S03。主线直接追加可达性、数值前提与证据/图示限定，不修改98原件、不要求重做整包。96的R96-01/02已闭合，97限定接收继续有效；本次不重开旧文档事项。第99批无D00。

## 1. 交付和独立核查范围

固定执行提交39b28e36e8f6与作者评论6951092059对应。相对f2df1e04cea8为ahead1/behind0、一个提交，严格六件added4441行：MANIFEST138、正文116、facts2597、DOT174、deferred515、evidence901；旧件零净改删。PR16/17均Open/unmerged，17原title/base/Draft保持。master与time直接头分别e8e60cd19540、a039d9803ade，缓存PR base_sha不代替direct ref。

六件及评论阅读全文，正文、evidence及facts的长响应缺段重叠补齐至EOF。103个编号锚均已读，并对9份实际锚定源码的相关定义、宏和类型逐段回源；未发现需要另发D00的跨独立定义拼接或必备自检字段缺失。匿名typedef结构整体定义与独立typedef声明不同；CRLF、定义内空白和锚尾空行不作为失败理由。标签字面成立不等于其所有消费者自动成立，以下补充优先。

MANIFEST十四字段、103/103/0、公约完整原句、time/master配置与panic金丝雀可读核对。原103标签、7组58行、21模型、9候选9缺口、64节点92边分别是作者成果栏目，不相加为运行测试数。主线独立Python只计算4441、58、页数2、0x240、0x200400及下述整数界限；未执行、翻译或重放仓库程序、作者模型或检查脚本。原六件自动正则/JSON/DOT/全103符号解析及完整本地P9未运行，不能签无条件PASS，也不认证作者另一聊天的临时日志或后台档位。

## 2. R98-S01：未消费返回值与实际失败可达性分别成立

BE98-F05、B-C08与正文关于CalculateElfFileSize返回被丢弃的源码事实保留，但不能把该辅助函数的隔离失败直接说成当前Parse主路径可达的错误。原M07/M14已部分区分，所有下游须统一消费。

Q1，Parse的ELF64路径先检查节名表索引：

[VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
    Elf64Shdr = (Elf64_Shdr *)GetElf64SectionByIndex (ElfCt->FileBase, Elf64Hdr->e_shstrndx);
    if (Elf64Shdr == NULL) {
      return (ElfCt->ParseStatus = EFI_UNSUPPORTED);
    }
```

Q2，随后保存的计数来自同一头部：

[VERIFIED myloader/uefi/ElfLib/ElfLib.c::ParseElfImage]
```c
    ElfCt->EntryPoint = (UINTN)Elf64Hdr->e_entry;
    ElfCt->ShNum      = Elf64Hdr->e_shnum;
    ElfCt->PhNum      = Elf64Hdr->e_phnum;
    ElfCt->ShStrLen   = (UINT32)Elf64Shdr->sh_size;
    ElfCt->ShStrOff   = (UINT32)Elf64Shdr->sh_offset;
```

Q3，GetElfSectionPos在有效ELF64索引下确实返回成功：

[VERIFIED myloader/uefi/ElfLib/ElfLib.c::GetElfSectionPos]
```c
    if (Elf64Shdr != NULL) {
      *Offset = (UINTN)Elf64Shdr->sh_offset;
      *Size   = (UINTN)Elf64Shdr->sh_size;
      return EFI_SUCCESS;
    }
```

在明确假设头部/上下文稳定、源对象与指针合法、ZeroMem初始化正常、零状态被EFI_ERROR视为非错误、所需计算有定义的ELF64路径中，Q1通过意味着e_shnum大于零；Q2令ShNum等于它，所以ShNum-1仍在计数内。由原E051/E052和Q3，这种正常前提下不能用ShNum=0构造已到达Calculate的UNSUPPORTED。若主张可达失败，需另给对象突变、状态变化或其它实际前提；无效内存访问也不能自动当成函数返回错误码。

因此BE98-F05保留为“局部状态未消费及额外可达性待证”，不作已确认可触发故障，不改ID或原件。B-C07、B-C08、B-C11、ABC-03、M07/M14及相关正文/图的消费应合读本条。GetElfSegmentInfo的循环索引同样必须先区分稳定头部下的合法计数与隔离接口错误，不从ASSERT存在虚构真实失败轨迹。

FileSize仍只是最后一个节的末端与节头表末端的较大值，不是Read实测长度，也不是对所有节、段和原始文件范围的全称证明；该来源分离与BE98-F02/F04继续保留。魔数中漏检EI_MAG3的事实经回源成立；头文件中另有检查四字节的IS_ELF宏，不会自动修补未调用它的IsElfFormat。

## 3. R98-S02：中间表达式、隔离模型和最早停止点

Q4，节头表长度乘法的强制转换在乘积之后：

[VERIFIED myloader/uefi/ElfLib/ElfLib.c::CalculateElfFileSize]
```c
  } else if (ElfCt->EiClass == ELFCLASS64) {
    Elf64Hdr  = (Elf64_Ehdr *)ElfCt->FileBase;
    FileSize2 = ((UINTN)Elf64Hdr->e_shoff + (UINTN)(Elf64Hdr->e_shentsize * Elf64Hdr->e_shnum));
```

Q5，两个字段均沿Elf64_Half别名：

[VERIFIED myloader/uefi/ElfLib/Elf64.h::Elf64_Half]
```c
typedef UINT16  Elf64_Half;
```

在UINT16为16位无符号、int为32位的明示C类型模型里，Q4两操作数先提升为int；65535乘65535为4294836225，大于2147483647，不能描述成必然先按无符号32位回绕再转UINTN。GetElf64SectionByIndex中的Index是UINT32，Index乘entsize是另一运算域，不得混用。实际SDK别名、编译选项和对象先门未证；此处为条件推导，不是实际溢出观测。补充作用于B-V05/06、B-C07/09、BE98-U04/U05及有关长度说明，不另造新编号。

无PT_LOAD且先行对象合法时，Base保持MAX_UINT32、End保持0。假设MAX_UINT32为0xffffffff：UINTN32下End-Base+1为2，UINTN64下为0xffffffff00000002；源码“equals to 0”注释不构成计算结果。M10的64位隔离计算可保留，但不能延伸为真实申请成功。

M11和BE98-F06的大地址示例须再明确页掩码的类型：仅说EFI_PAGE_SIZE数值4096，不足以决定取反后掩码高位。例中“0x100000000仍对齐为自身”要求所选表达式保留高位；若SDK定义导致32位无符号掩码再零扩展，则不是同一模型。B-C05已经保留宏未证门，正文及M11必须一并消费，不以本仓loader的PAGE_SIZE替代外部EFI_PAGE_SIZE/ALIGN_VALUE。

Q6，段写请求在同一函数内依序发生：

[VERIFIED myloader/uefi/ElfLib/Elf64Lib.c::LoadElf64Image]
```c
    Delta = (UINTN)Phdr->p_paddr - (UINTN)ElfCt->PreferredImageAddress;
    CopyMem (ElfCt->ImageAddress + Delta, ElfCt->FileBase + (UINTN)Phdr->p_offset, (UINTN)Phdr->p_filesz);
    ZeroMem (ElfCt->ImageAddress + Delta + (UINTN)Phdr->p_filesz, (UINTN)(Phdr->p_memsz - Phdr->p_filesz));
```

M16的ZeroMem目的0x200400还须显式假设ImageAddress等于Preferred的0x200000、Delta为零、前一CopyMem正常返回；并非只列p_paddr和Preferred便唯一确定目的地址。M17和BE98-F08中filesz大于memsz只说明长度关系与减法风险，不自动证明CopyMem一定越过真实后备分配；如果源/目的对象或参数求值先不成立，在最早处停止；若这些前提仍合法，才讨论后继无符号减法及转换后的ZeroMem尺寸。合法尾后一指针与非法成员访问仍按R80-S01/R97-S01区分。

Parse对PT_LOAD的范围计算没有LoadElf64Image的memsz==0跳过门，不能用M18的装入阶段分支抹去此前解析阶段计算。M20讨论外层最终return时，除AllocatePages返回外还须假设中间LoadElfImage正常返回；只证明前一服务返回不够。

## 4. R98-S03：证据支持、实际调用者与历史记录

Q7补C-P06的reload日志支持；原E080本身没有包含这一行：

[VERIFIED myloader/uefi/bootloader.c::load_elf_kernel]
```c
	Print(L"Elf Image need to reload: %d\n", ElfCtx.ReloadRequired);
```

打印仍非装入成功证据。raw_img_addr由load_elf_kernel的局部常量生产，DOT的p_UefiMain到raw只能读作上游调用关联，不是该字段的赋值生产者。真正调用CopyMem和ZeroMem的都是LoadElf64Image；loadseg/copy/zeroreq是状态或调用请求节点，copy到zeroreq应解释为同一调用者内的SOURCE_ORDER，不能解释为CopyMem内部调用ZeroMem。

LoadElfImage返回分派取得的Status，LoadElf64Image在正常到达末尾时返回常量EFI_SUCCESS；共用loader_status图节点不合并两种语义。C-P05“只有其它调用者设置不同地址才进入”只针对经LoadElf64Image的分支；RelocateElf64Sections直接接口本身不含同样地址不等判断。当前load_elf_kernel先令ImageAddress等于Preferred，在上下文稳定、正常到达比较处时不选重定位；不外推全仓没有其它直接调用或根本无法使用重定位。重定位算法仍未审。

facts.read_account的98_checkpoint_full按140必读入口应读作2026-11-17-bootinfo97-reviewed-bootelf98-ready.md，不作为另有一份当时已发布98检查点的证据。作者本人复用资格、103条程序性自检和后台模型档位均为申报；主线独立依据本轮可读源证据，不认证另一聊天的工具历史。

9项候选是9条收存记录，不是9个相互独立、已触发的根因：F01/F02/F04有文件来源前提联系，F05要受S01可达性限定，F07/F08/F09共享装入对象及状态前提。保留qualified_id、needs_evidence和禁止自动修复；86等旧文档问题及96闭合不被重复打开。

## 5. 接续与明确未做

99按141调查UEFI模块输入/库依赖、现有EFI制品来源和安装交付链，继续C06e/14次一档；本批没有D00。不是重做85内核构建、98装入逻辑，也不是恢复错误136/137的MBI复制与BSS专题。模块清单、安装命令、二进制身份和实际固件启动四层分开，不能用阅读脚本当作部署证据。

主线七段短锚均来自固定time原文，独立于作者103条。完整本地P9、自动JSON/DOT/正则/符号扫描、预处理/配置/编译/链接、仓库程序/脚本、Graphviz/QEMU/实机、真实EFI服务/并发均NOT_RUN。无内核/loader/脚本修改、无补丁或合并。回执入库、99正式发布与99实际启动分别记账；后继交回原C06前Owner选择Pro，执行者不自签99通过、不启动100。
