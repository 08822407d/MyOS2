---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-UEFI-BUILD-PROVENANCE-99
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
record_type: fixed_time_uefi_module_external_dependency_artifact_install_provenance_report
produced_by: GPT-6（可见模型家族；后台UNATTESTED）
model_per_owner: GPT-6＋Extra（Owner指定次一档）
date: 2026-11-18
base_snapshot: time（分支名）
source_pin_short12: a039d9803ade
read_channel: connector
status: AUTHOR_STATIC_COMPLETE_PENDING_C06_PRO
mainline_acceptance: PENDING
D00: NOT_REQUIRED
runtime_evidence: NOT_RUN
---

# 第99批｜UEFI模块、外部依赖、现有制品与安装来源

**结果：** 固定time中的UEFI模块INF声明、CMake安装配方和独立EDK安装脚本属于不同的来源链。仓库确有跟踪的 `myloader/BOOTX64.EFI` 文件（GitHub元数据：19200字节、短Blob `6596c0462579`）；`myloader/CMakeLists.txt`的安装输入是该同名源文件，而 `myloader/uefi/install_loader.sh`的复制输入是外部相对路径 `Build/Shell/DEBUG_GCC5/X64/BootLoader/bootloader/DEBUG/bootloader.efi`。两条路线都可在文本上指向 `/mnt/EFI/BOOT/BOOTX64.EFI`，但**没有证据证明两个输入字节相同、任何一个安装曾成功或实际固件启动了哪一个EFI文件**。此报告只读来源，不运行脚本、不读取或反汇编二进制、不配置/编译/链接、不安装、不检查主机设备。

本轮固定对象：主线 `896f19a08088`、执行父 `39b28e36e8f6`、唯一源码 `time@a039d9803ade`、直接master `e8e60cd19540`，均为连接器返回短12。第98批为`PASS_PENDING_LOCAL`（必须合读R98-S01/S02/S03），没有RETURN；第99批`D00=NOT_REQUIRED`。原96文档闭合、97限定验收与85的R85-S01/S02/S03均不重开。

## A｜INF模块清单、实际源码与入口的三层区别

`myloader/uefi/bootloader.inf`的`[Defines]`声明`MODULE_TYPE=UEFI_APPLICATION`、`BASE_NAME=bootloader`、`ENTRY_POINT=UefiMain`，另列`INF_VERSION`和唯一模块GUID。它的`[Sources]`有**15个条目、14个不同相对路径**；`ElfLib/ElfLibInternal.h`在清单第17与21行重复列出一次。文件名已用time下`myloader/uefi`与`ElfLib`两次有限目录元数据核查；列表包括`bootloader.c`、`multiboot2_utils.c`、`utils.c`、`ElfLib.c`、`Elf32Lib.c`、`Elf64Lib.c`及相关头文件。把头文件列进Sources并不等于每个头都是编译出的独立目标对象；重复路径是否被EDK解析器自动去重，**未证**。（`UB99-E001…E007`）

四个`[Packages]`项分别指向MdePkg、MdeModulePkg、PcAtChipsetPkg、UefiPayloadPkg中的DEC；十个`[LibraryClasses]`含`UefiApplicationEntryPoint`、`UefiLib`、`UefiBootServicesTableLib`、`DebugLib`、`UefiRuntimeServicesTableLib`、`BaseMemoryLib`、`PcdLib`、`MemoryAllocationLib`、`HobLib`、`BaseLib`。独立的`[Protocols]`列五项、`[Ppis]`一项、`[Pcd]`两项、`[Guids]`两项。这些是INF声明类别，不是本次已取得的外部包源码、所选库实例或运行时调用事实。（`UB99-E008…E014`）

真实C源 `bootloader.c`中存在 `EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)` 函数定义及其`gBS->HandleProtocol`调用。须区分：

1. INF的 `ENTRY_POINT=UefiMain` 是模块入口声明；
2. `UefiApplicationEntryPoint` 是请求外部库类的名字；
3. `EFIAPI UefiMain` 是实际源码的函数定义。

三者不能自动合成为已链接的PE入口包装、某个调用约定或真实机器码。外部DSC、平台库类绑定、编译命令及PE入口内容均未取到（`UB99-U01/U02/U03`）。（`UB99-E002/E009/E015/E016`）

同时，time中的`myloader/CMakeLists.txt`只有其项目声明与 `install(PROGRAMS BOOTX64.EFI DESTINATION EFI/BOOT)` 等内容。**在所读这个CMake文件里**没有把上述INF的C源传给一个UEFI编译目标的语句；这不构成全仓或外部EDK工作区没有另一个编译入口的断言。它是从现有文件出发的安装路线，而INF描述的是另一个可能由EDK消费的源码模块清单。（`UB99-E017/E018`）

## B｜四组直接依赖和ABI/配置边界

| 依赖组 | 当前可证的仓内来源 | 缺失的关键证据 |
|---|---|---|
| 1. `UINTN/UINT16/UINT32`、`EFI_STATUS/RETURN_STATUS` | `utils.h`包含`Uefi.h`，`ElfLib.h`包含`PiPei.h`；`Elf64_Half`独立别名为`UINT16`；`multiboot2.h`在`__PI_UEFI_H__`条件分支内使用不同typedef | 实际SDK类型定义、条件宏是否选中、目标ABI与指针表示 |
| 2. `EFI_PAGE_SIZE/ALIGN_VALUE/MAX_UINT32` | `ElfLib/ElfLib.c::ParseElfImage`用这三个外部名字计算图像区间；`utils.h`只定义独立的`PAGE_SHIFT=12`、`PAGE_SIZE=(1<<PAGE_SHIFT)` | 外部宏实际展开的类型、位宽、取反掩码高位、计算中间域及实际编译 |
| 3. `ASSERT/ASSERT_EFI_ERROR`与`DebugLib` | `ElfLibInternal.h`包含`DebugLib.h`，ParseElfImage以`ASSERT_EFI_ERROR(Status)`消费段信息状态，INF声明`DebugLib`库类 | 真实选定DebugLib实现、DEBUG宏、断言生效与错误后可达性 |
| 4. `EFIAPI/UefiApplicationEntryPoint/BaseMemoryLib/服务表` | INF库类、C入口`EFIAPI`、`utils.h/ElfLibInternal.h`的库头及`gBS`调用文本 | PE入口包装/调用约定、BaseMemory真实实现、BootServices返回、服务阶段和真实模块链接 |

证据链：`UB99-E019…E046`及`UB99-E002/E009/E015`。R98-S02必须保留：**数值、实际C类型、表达式中间提升和目的字段宽度不是同一事物**。例如`Elf64_Half`别名`UINT16`，在假设其为16位无符号且`int`为32位的隔离模型里，`65535×65535=4294836225`超过有符号32位范围；不能把乘积后的`(UINTN)`强转倒算成乘法前已经宽化。相同地，单知`EFI_PAGE_SIZE`的可能数值4096不能证明`~(EFI_PAGE_SIZE-1)`掩码保留了何种高位：宏实际类型、整数提升和SDK版本未证。（R98-S02、`UB99-M04/M05/M06`）

**内核与UEFI配置不可混用。** `mykernel/scripts/options_flags.cmake`中`CMAKE_C_FLAGS`文字包含`-m64 -mcmodel=kernel -fno-pie -fno-pic`和`-mabi=sysv`；它属于内核CMake输入，不是UEFI INF所选编译器的命令或最终EFI ABI。另一方面，独立脚本的`DEBUG_GCC5`仅存在于外部Build路径字符串，也不能证明实际编译器版本、优化选项或PE机器码。没有运行配置/预处理、实际编译命令生成、链接或ELF/EFI反汇编；外部库类名字不代表已定位具体实现。（`UB99-E047/E048/E075`）

## C｜三条文件交付关系、条件安装路径及失败前缀

### 1. 制品身份与目标

| 来源/生产者 | 当前可见的源对象或请求 | 目标或消费位置 | 身份上限 |
|---|---|---|---|
| 仓库Git记录 | `myloader/BOOTX64.EFI`，time文件元数据19200字节、Git Blob短12 `6596c0462579` | CMake `install(PROGRAMS ... DESTINATION EFI/BOOT)` | 只有Git文件身份，二进制内容未读取 |
| 独立EDK脚本 | 相对路径`Build/Shell/DEBUG_GCC5/X64/BootLoader/bootloader/DEBUG/bootloader.efi` | `/mnt/EFI/BOOT/BOOTX64.EFI` | 外部编译产物内容、版本及存在性未证 |
| 同一独立脚本的第二份cp | 与上一行相同的EDK源路径 | `~/projs/MyOS2/elfbinary_loader.efi` | 另存副本请求，不代表cp已成功或两副本最终同字节 |
| kernel CMake目标 | `add_executable(kernel)`与`install(TARGETS kernel DESTINATION boot)` | prefix为/mnt条件下的`/mnt/boot/kernel` | 实际链接/安装字节属于历史`BB85-U08`的未证范围 |
| UEFI消费方 | `UefiMain`调用`load_elf_kernel(LoadedImage,L"\\boot\\kernel",...)` | 外部EFI文件协议读取的`\\boot\\kernel` | 与宿主`/mnt/boot/kernel`仅为字面路径关联，未证明同一卷或Read成功 |

CMake 3.16.9官方`install()`文档支持`PROGRAMS`相对输入文件由当前源目录解释、相对`DESTINATION`由安装前缀定位： [EXTERNAL https://cmake.org/cmake/help/v3.16/command/install.html]。这仅在实际配置/执行具有相应语义时用于条件模型；本次既未证明宿主CMake版本，也未运行安装。GitHub目录元数据是另一类来源，不标为源码VERIFIED；跟踪Blob值与外部EDK文件或ESP字节不存在比较数据。

### 2. 真实脚本次序与失败停止门

`make_install.sh`先用`PROJ_ROOT=$(pwd)`，声明Debug/x86_64/static参数，`source`四份函数脚本，可能创建build目录，然后出现`rm -rf ./build/*`文本，最后依次调用`make_install_kernel`、`make_install_initranfs`、`make_install_bootloader`。这三个**顶层函数调用不是互相以`&&`连接**；所读主脚本只`source`物理安装函数而没有有效调用该函数，不能据此声称任何磁盘清理/物理复制实际发生。源文本中的清理/挂载/复制命令均未执行。（`UB99-E049…E060/E083`）

- **kernel函数**：设`$1/build/mykernel`、`cd`，独立`mount`后`rm /mnt/boot/kernel`，再`cmake ... && make && make install`；后面仍有`sync/umount/objdump/cp`语句。若mount失败、shell仍继续的隔离条件成立，rm及后续命令不因mount状态而自动被源文本阻止。不能从该模型声称任何目标被实际删除、编译失败或已卸载。（`UB99-E065…E069`）
- **initranfs函数**：另设自己的`CMAKE_GEN_DIR`并`cd`，按文本挂载、删除init/sh、`cmake&&make`，随后另有单独`make install`、sync和umount；它位于kernel与bootloader两阶段之间。这里仅读其与工作目录/命令状态相关范围，不重审用户程序、musl或initramfs构建。（`UB99-E070…E073`）
- **bootloader函数**：设`$1/build/bootloader`并`cd`，mount后调用`cmake $1/myloader -DCMAKE_INSTALL_PREFIX=/mnt && make && make install`，随后独立sync和umount。若cmake返回非零，Bash的`&&`链不继续make/install；但不能因此认为后续sync、umount就证明EFI安装或失败恢复。**也不能把mount列入该`&&`链**。（`UB99-E061…E064`）
- **独立`install_loader.sh`**：其相对Build源路径依赖启动该脚本时的当前工作目录，先mount再执行两条cp、sync、umount；在已读主`make_install.sh`调用列表中没有调用这份独立脚本。两条cp未以`&&`关联；即使第一条成功也不能保证第二条复制成功或辅助副本相同。（`UB99-E074…E078`）

上述shell条件使用GNU Bash官方关于`&&`的规则：[EXTERNAL https://www.gnu.org/software/bash/manual/bash.html]；真实解释器环境、cwd、`$USER`和未加引号路径在特殊输入下的展开、mount状态、设备/卷身份、文件存在与命令退出状态，均不由文本自动证明。不能仅因存在未加引号变量就宣称所有正常路径必然失败，也不在本批运行任何破坏性命令。

### 3. 交付与启动的跨阶段边界

kernel目标`DESTINATION boot`、脚本`/mnt/boot/kernel`与UEFI代码请求`\\boot\\kernel`可以建立**文字路径上的关联**，但中间至少还需要：真实kernel构建产物、安装成功与卷身份、EFI应用实际执行及其打开文件结果、FileInfo/Read实际字节数、ELF解析与PT_LOAD目标及入口执行。这些属于`CORE-USER-BOOT-BUILD-CONTRACT-85::BB85-U08`、`CORE-USER-BOOT-ELF-LOAD-98::BE98-U02/U06/U09`与既有BT97阶段性未证前提。路径相似不能将其闭合，文档检查不能替代真实启动。

## 账目、图和交回

- `evidence.md`新增**87条**真实单定义、INF命名节/字段、CMake唯一命令或shell函数/顶层调用短锚。固定time正文经四组独立逐字回比，A18、B30、C39，全部连续1–5行。
- `facts-and-boundaries.yaml`包含**7组61行**条件矩阵、**18个**有限模型、逐条source_quote_index、Git跟踪制品元数据与四组外部依赖。矩阵分别为模块/入口、配方/制品、类型/库、配置/ABI、命令失败前缀、制品/boot路径和ABC接合。
- `deferred-findings.yaml`只新增**5项条件候选`UB99-F01–F05`、9项缺口`UB99-U01–U09`**，源级未证不等于真实故障。与旧BB85、BE98、BT97的同根因通过完整qualified_id连接，不重新编号历史问题。
- `uefi-build-provenance-paths.dot`是静态分类图，区分`INF_DECLARATION`、`DECLARED_INSTALL`、`SHELL_CP_REQUEST`、`PATH_TEXT_MATCH`、`DATA_HANDOFF`和`PROOF_BOUNDARY`，不是完成顺序或完整CFG，Graphviz仍为NOT_RUN。
- 完整本地P9、配置/预处理/汇编/编译/链接、仓库脚本、安装、挂载、清理、复制、objdump、模拟器/实机及二进制机器码检查均**NOT_RUN**，没有创建修复代码或外部构建；仅文本/有限纯数学核对。

本作者只向既有PR17执行分支新增严格六件及本人一次正式评论。发布与回读后状态应为`READY_FOR_MAINLINE_REVIEW`且`mainline_acceptance=PENDING`；第99批是否接收，由原“MYOS2-A-C06 内核分析主线续建”在Pro独立同轮验收，作者不能自行签PASS或启动第100批。
