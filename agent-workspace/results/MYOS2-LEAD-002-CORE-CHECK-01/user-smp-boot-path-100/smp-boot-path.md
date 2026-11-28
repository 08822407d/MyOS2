---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SMP-BOOT-PATH-100
record_type: bounded_static_smp_boot_and_R99_01_documentation_correction
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
produced_by: GPT-6（当前可见模型家族；后台UNATTESTED）
model_per_owner: GPT-6＋Extra（Owner指定次一档）
date: 2026-11-18
base_snapshot: time（分支名）
read_channel: connector
source_pin_short12: a039d9803ade
lead_fixed_short12: 928c77c6bbb4
execution_parent_short12: 3df2ee58f100
mainline_acceptance: PENDING
batch99: RETURN_DOCUMENTATION_ONLY_R99_01
R99_01: CORRECTION_SUBMITTED_PENDING_C06_REVIEW
status: AUTHOR_STATIC_COMPLETE_PENDING_C06_PRO
runtime_evidence: NOT_RUN
---

# 第100批｜99文档类别补正与CPU拓扑至AP启动路径静态取证

**交付结论及范围。** 固定time源码能够连接UEFI CPU tag的计数产生、内核`nr_cpu_ids`与稠密perCPU循环，以及BSP主启动路径中的几个实际函数调用；能够独立读出另有AP复制/启动请求C函数和AP汇编入口、索引、栈及终止指令。但**这些分支尚未形成从BSP调用AP复制、发出启动请求、使AP安全查表/配置栈到宣布在线的完整已证执行链**。有些相邻源定义的代码存在，不等于它们之间已有活动调用；链接脚本和ASM标签不等于实际链接产物或硬件状态。本批没有编译、链接、运行、硬件实验或修复。

本次严格六件：`evidence.md`独立定义/ASM真实入口短锚**101**条；`facts-and-boundaries.yaml`含D00十四项、**8组52行矩阵、18个有界条件模型**；`deferred-findings.yaml`六项新条件候选与九项待证缺口，旧qualified_id不重复计根因；`smp-boot-paths.dot`为84节点94分类边的静态图（Graphviz NOT_RUN）。

## D00｜R99-01：十四条定义类别纠正（追加，不改原99）

原第99批可读证据`UB99-E019–E031`共十三条来自`utils.h`、`bootloader.h`、`ElfLib.h`和`ElfLibInternal.h`的独立`#include`文本，确有这些预处理包含指令，却并非对应外部头文件定义体或库函数实现。`UB99-E041`是`bootloader.h`里的`load_elf_kernel`函数**原型**，也并非函数体。前十三条保留为**DECLARATION_CONTEXT**而撤销定义锚资格；E041撤回定义资格并补以本100真正的`myloader/uefi/bootloader.c::load_elf_kernel`实现体短锚`SP100-E056–E058`支持必要的源码消费；包含声明不得凭空替换成外部库定义。原99的所有引文字面本身真实，但旧MANIFEST/facts关于“87条都属于合格定义体/声明式锚”的全称结论必须撤回。

`facts-and-boundaries.yaml::D00.corrections`逐条列旧ID、真实路径/选择符、修正类别、处置、受影响旧矩阵消费者及仍缺的外部类型/ABI/函数调用前提；包含B-T01、B-T03、B-T05、B-T07、B-T08、B-T09及B-C08。**第99批原六件、原评论、原87/87/0全部冻结；不能把扣去14条得73自动宣称整个原P9通过，也不能将旧13条include重新标成14条新定义。** R99-01状态严格为`CORRECTION_SUBMITTED_PENDING_C06_REVIEW`，第99批仍RETURN，是否关闭只由C06未来独立复核决定。

与补正一并消费主线三项限定：

- **R99-S01：** CMake相对目的地须有适用prefix、DESTDIR及实际命令环境；shell后缀能否继续、函数最终返回状态、两个copy目标同字节的条件均不能当实际安装。
- **R99-S02：** `CalculateElfFileSize`中`e_shentsize * e_shnum`发生在外层UINTN转换之前（本100`SP100-E060`真实函数体补锚），`LoadFile`真实字段为`FileInfo->FileSize`（`SP100-E059`），`ASM_FILE`及`__PI_UEFI_H__`双预处理条件不能只看后者。此前多义的`set(CMAKE_C_FLAGS)`名字不能代替具体命令位置。
- **R99-S03：** 旧图的“不是相同字节”仅可消费为“相同字节尚未证明”；kernel/UEFI配置来源不同不能证明最终flags必不相同；图中`libclass`、`runner`的压缩矩阵标签不得变为实际函数调用者。保留99原F/U身份，不改旧DOT。

## A｜CPU拓扑记录到内核计数和映射

`UefiMain`顺序调用framebuffer、mmap、ACPI、CPU四个producer；其中`fill_mmap_info`源内的`ExitBootServices`调用位于CPU producer之前，依R97-S03仅证明源码调用次序，不认证EFI退出结果或后续服务阶段合法性。`fill_cputopo_info`通过`LocateProtocol`获取MP协议对象、`GetNumberOfProcessors`获取`UINTN nCores/nRunning`，然后将它们赋给CPU标签的`core_num/core_available`，逐个以`GetProcessorInfo`取得`ProcessorId/StatusFlag/Package/Core/Thread`并写`cpus[i]`，再写tag type和nCores派生size，之后推进cursor。没有就地验证每次MP调用成功、实际arena容量和UINTN到u32/size的可表示性；不能以日志或标签头证明硬件给出合法N条记录。（`SP100-E001–E012`）

内核`parse_tag`在magic和8字节对齐前置条件通过、tag遍历正常且遇到CPU_TOPO type时调用`get_smpinfo`。当前**这个函数体**只把`smp_info->core_num`赋给全局`unsigned nr_cpu_ids`，未消费`core_available`或`cpus[]`中的实际ProcessorId字段，也未向APIC映射表写入。没有遍历原始tag外层对象的有效长度与核侧读取前提，不能把记录名或数值看作已完成注册/核上线。（`SP100-E013–E019`）

`for_each_cpu(cpu)`是`int cpu=0;cpu<nr_cpu_ids;cpu++`的稠密整数循环，`for_each_possible_cpu`与`for_each_online_cpu`在所读头文件都别名到它；“online”字样**不证明**CPU在线位图已真实维护。`setup_per_cpu_areas`调用首块分配后为每个迭代CPU写perCPU offset、this_cpu_off与cpu_number，且在cpu0分支发出load_direct_gdt及GS_BASE写请求。这不是AP通用栈/GS准备成功的证明。（`SP100-E020–E028`）

`apicid_to_cpunr`是大小`CONFIG_NR_CPUS`的u64数组，所读定义把所有元素初始化为-1；`NR_CPUS`别名CONFIG_NR_CPUS，内核options_flags字面要求`-DCONFIG_NR_CPUS=256`，但配置**文本**不是已预处理的真实构建结果。本轮所读CPU tag消费者没有填充APIC表；这不等于全仓不存在其它写者。严格分开`nr_cpu_ids`的CPU计数、静态目标数组容量、APIC标识与CPU逻辑编号、在线集合。N=257且容量按256的隔离模型中，idx256为首个超出目标数组有效元素的访问门，形成尾后一地址本身与对其成员访问须按R80-S01/R97-S01分开。（`SP100-E024/E029–E032`）

## B｜BSP准备、AP代码复制和启动请求的实际连接

`x86_64_start_kernel`所读定义在早期调用`myos_early_init_system`后调用`start_kernel`；`start_kernel`的活动函数体调用`setup_arch`、`setup_per_cpu_areas`，后面还调用`myos_init_percpu_intr`（其体调用`LVT_timer_init`）和`rest_init`。`setup_arch`请求尝试/检查x2APIC；它们的外部寄存器结果没有回读认证。`kernel_init_freeable`里的`smp_prepare_cpus`和`smp_init`处于**注释**文本，不能当作活动函数调用。（`SP100-E037–E055`）

同一固定源码下`myos_early_init_smp`和`myos_startup_smp`两函数真实存在，**但在已审的上述BSP调用者定义中没有发现连到这两个函数的活动调用边**。这只是所读函数的限定判断，不冒充全仓调用穷尽，更不能断言整个系统绝不可能执行它们。`myos_early_init_smp`以`&_APboot_etext - &_APboot_text`计算请求长度，尝试`memcpy`到`phys_to_virt(&_APboot_phys_start)`，再给`cr3_paddr`赋`kernel_cr3`。实际源/目标对象、拷贝成功、指针减法合法、链接器符号与地址翻译都仍是独立证明门。（`SP100-E033–E035/E101`）

固定linker文本`_APboot_phys_start=0xE000`只提供名字的数值声明，不是实际0xE000低地址页可写或已分配证明；ASM文件使用`.AP_boot.text`输入节，而本次定位的链接器`.text`输入通配是`.text*`，`_APboot_text/_APboot_etext`真实符号与输入节放置要由完整链接产物确认。CMake的`*.c/*.S`与`target_sources`只有源选择意图，不能认证编译器确把AP源/汇编编进当前kernel。（`SP100-E061–E070/E071`）

`myos_startup_smp`依次发出`wrmsrl(0x830,0xc4500)`和`wrmsrl(0x830,0xc4620)`源级请求；源注释称INIT和Start-up，但**没有外部版本化x86手册或真实运行证据**，本批不把它们签成合规IPI发送、固件接收或AP从指定入口执行。两函数定义、真实调用、服务寄存器行为、AP回执四个门必须分别取得证据。（`SP100-E036`）

## C｜AP汇编分阶段入口、查表、栈和终点

`myos_APboot.S`的`_APboot_start`为`.code16`下的ASM入口，含清中断/刷新缓存请求、临时栈以及GDT/IDT和模式转换指令；`_AP_code32`改写段选择符、请求CR4、以`cr3_paddr`符号形成地址读取后请求CR3与后续模式切换；`_AP_code64`再请求段寄存器、`kernel_cr3`地址/CR3、MSR和CR4、APIC值进入索引查找。代码存在不保证固件/硬件按这些步骤成功进入长模式；实物CPU特性、MSR语义、物理页表和值均未证。（`SP100-E071–E088/E097/E101`）

索引块`l1`先将`rbx`从0**递增到1**再读`apicid_to_cpunr[rbx]`，与候选`r8`比较，失败`jne l1`，所读循环中没有数组上界判定。若有效条目中只有索引0匹配，或者假设候选与初始化-1表的有效索引1..255都不相等，则后续读到索引256时没有有效数组元素；**证明停在首个无效读取，而不是源码自行设有停止分支、真实机器已死循环或必然发生异常**。源码注释“BSP APIC ID是0”也只是作者前提，不是实际硬件拓扑。（`SP100-E029/E088/E089`）

`AP_DEBUG`入口中的注释`idle_tasks`加载没有执行，活动语句却从**同一APIC映射数组位置**再次读值放入`rax`，随后加`THREAD_SIZE`并写`rsp`。这里的表值未被确认为idle task/PCB指针，也未证明有效内核栈对象。独立宏`THREAD_SIZE_ORDER=3`、`THREAD_SIZE=(PAGE_SIZE<<THREAD_SIZE_ORDER)`在显式`PAGE_SIZE=4096`模型下给出32768字节，这只是尺寸数学，不认证所形成指针有效。汇编接着`jmp .`，未见该局部块跳转到idle/scheduler或发布CPU online的活动语句；真实AP是否到达此处未证。临时GDT/IDT及`cr3_paddr`数据标签、`_APboot_tmp_Stack`亦依赖布局、复制和真实硬件门。（`SP100-E090–E100`）

## 跨批次证据与交付边界

R99-S01/S02/S03、R97-S03与R80-S01、R86-S01/S02/S03、R87-S01/S02/S03继续控制解释：EFI服务阶段、计数窄化、指针与先行有效对象、perCPU分配/GS、汇编链接布局与CPU上线分别是待证门。旧`CORE-USER-BOOTINFO-TAG-BUILD-97::BT97-F07/F08`、`CORE-USER-PERCPU-BOOTSTRAP-86::PC86-F07/U04/U05/U07/U08`及相关PA87候选在`deferred-findings.yaml`保留原身份，不能因本次模型重复登记成多个独立已发生bug。

`smp-boot-paths.dot`按`DIRECT_CALL`、`DATA_HANDOFF_NOT_DIRECT_CALL`、`SOURCE_ORDER_ONLY`、`WRMSR_REQUEST`、`UNPROVEN_JOIN_NOT_CALL`和`PROOF_BOUNDARY`分开画边。矩阵节点并非C函数或运行状态，引用无悬空只是文本一致，不是Graphviz运行或完整CFG认证。所有模型为独立整数/条件输入推导，未调用或翻译执行仓库函数。

本执行对话只获准在原执行分支新`user-smp-boot-path-100/`目录新增严格六件及本人一次PR17顶层正式评论。发布前核真实expected-head、六目标与100 marker；发布后以Git compare、固定/活动全部六件、四direct refs、PR16/17及评论编号/全文/唯一性为准。全部完成后`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`、`R99-01=CORRECTION_SUBMITTED_PENDING_C06_REVIEW`，然后`STOP_WRITING`。交原C06 Pro同轮独立闭合99纠正、验收100和安排后继；本执行者不自签99/100通过，不启动101。
