---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-EXEC-STACK-PLACEMENT-142"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-22"
conversation_display_name: "MYOS2-A-C10a 代码分析执行"
produced_by: "GPT-6.2（系统模型族标识；界面实际型号不可独立核实）"
model_per_owner: "次一档GPT-6.2 Sol；Extra/高档偏好"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
date: "2026-11-24"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
read_channel: "connector"
status: "AUTHOR_CONTENT_PREPARED_PENDING_PUBLICATION_AND_G04"
mainline_acceptance: "PENDING"
full_local_P9: "NOT_RUN"
open_questions: ["实际配置、C指针/对象合法性、vma前置、映射和真实运行仍未证"]
inputs_read: [{"path":"mykernel/scripts/options_flags.cmake","ref":"a039d9803ade","blob":"6019436417ad"},{"path":"mykernel/fs/vfs/binfmt_elf.c","ref":"a039d9803ade","blob":"579fb4e9fadd"},{"path":"mykernel/arch/x86_64/include/asm/elf.h","ref":"a039d9803ade","blob":"421649d6f3bc"},{"path":"mykernel/include/linux/kernel/elf.h","ref":"a039d9803ade","blob":"dd427f17f005"},{"path":"mykernel/include/uapi/linux/elf.h","ref":"a039d9803ade","blob":"eff5e1ff56b8"},{"path":"mykernel/include/uapi/linux/auxvec.h","ref":"a039d9803ade","blob":"166fa1066af1"},{"path":"mykernel/include/linux/fs/binfmts.h","ref":"a039d9803ade","blob":"002c2aec4aa8"},{"path":"mykernel/include/uapi/linux/binfmts.h","ref":"a039d9803ade","blob":"a60f4e7c53b0"},{"path":"mykernel/mm/misc/mm_misc_const.h","ref":"a039d9803ade","blob":"ec983b79166f"},{"path":"mykernel/include/linux/kernel/asm-generic/param.h","ref":"a039d9803ade","blob":"16bba89f98f5"},{"path":"mykernel/mm/misc/mm_misc_types.h","ref":"a039d9803ade","blob":"b9d93f504756"},{"path":"mykernel/arch/x86_64/sched/context/process_arch.c","ref":"a039d9803ade","blob":"4431545af5fb"},{"path":"mykernel/sched/forkexec/exec.c","ref":"a039d9803ade","blob":"0861ebae6fc2"},{"path":"mykernel/include/linux/kernel/types.h","ref":"a039d9803ade","blob":"ef531baa8317"}]
record_type: "phase2_stack_placement_analysis"
---

# 142 · 具体栈位置、区间、字段时点

## D00 R141-01（仅提交文档补正）

141独审RETURN_DOCUMENTATION，A/B/C限定技术内容无新增退回。原141的六件和作者评论冻结；当前仅在142 MANIFEST及facts.D00.R141-01补齐规范键、逐文件消费索引和实际重新核对结果。冻结原件129个正式标记全部在evidence.md，本轮重新打开11份固定源按原source_index逐项核得129/129/0（129+0=129），范围RECONFIRMATION_DURING_BATCH_142_ONLY，不回填141发布前时序。旧与新P6分计。判闭权C10；140及五项历史文档判闭不变。

## 共同题设及两道片段边界

U=0x70000000是给定的既有字符串区上界，不是已观测栈顶；p0=bprm.p=mm.arg_start、F=bprm.exec及三个字符串对象按140及197给定。141 A0/Afd所选载荷复用，不把其C1/C2装载算例自动代入。宏模型排除ARCH_DLINFO、ELF_HWCAP2、CONFIG_RSEQ，ELF_BASE_PLATFORM=NULL，ELF_PLATFORM=x86_64、AT_VECTOR_SIZE_ARCH=0；options_flags先读，显式-m64等不穷尽实际构建。E142-071,E142-072,E142-073,E142-042,E142-004；H=0x1234为题设而非测量。elf_addr_t实际为Elf64_Off（__u64）而非Elf64_Addr；ulong为unsigned long，二者及槽宽8字节是采用模型；elf_phdr_s对应elf64_phdr的2个4字节与6个8字节字段，所选56字节。E142-045,E142-049,E142-050,E142-055,E142-046,E142-047,E142-053,E142-054,E142-051,E142-052。没有编译或实际布局测量。

上游setup_arg_pages计算stack_shift=vma.vm_end−stack_top，bprm.p减该值后赋mm.arg_start，bprm.exec也减shift；随后仍有失败门，23/24的页、引用及返回缺口不变。E142-039,E142-040,E142-041。本例直接给定入口，不据此证明实际平移。门前和门后是独立片段：vma局部无初始化，查找赋值被注释，锁/!vma门保持；不得把前缀模型串成生产路径。E142-007,E142-021,E142-022。门后给定到达、对象有效、用户槽可写/串可读、长度及辅助写复制正常完成。caller再给定表正常返回及后续条件。

## A · 计算两次对齐及表基址

以下为[INFERRED]整数地址投影，不认证typed pointer是否合法。p1=floor16(p0)，固定arch_align_stack活动返回sp & ~0xf，随机化为注释；P=p1−7（x86_64+NUL），local p=P，u_platform=P。ei_index=26/28/26；sp_aux=P−8*ei_index；items=(argc+1)+(envc+1)+1；raw_bottom=sp_aux−8*items；S=floor16(raw_bottom)，Q=S+8*items，E=Q+8*ei_index。源公式与成员：E142-036,E142-037,E142-038,E142-008,E142-003,E142-009,E142-010,E142-001,E142-002,E142-018,E142-019,E142-020,E142-030。题设所有整数地址可表示且无回绕。

### R1（140 A1，141 A0）

argc=2、envc=2，items=7字，ei_index=26字；F=0x6ffffff6。载荷：{"prior_ref":"08f6fee103dd","selector":"A.A0","common":{"H":"0x1234","PHDR":"0x400040","PHNUM":3,"PHENT":56,"BASE":0,"ENTRY":"0x401020","AT_PAGESZ":4096,"AT_CLKTCK":250},"interp_flags":0,"AT_FLAGS":0,"secureexec":0,"have_execfd":false,"execfd":"NOT_READ","AT_EXECFN":"0x6ffffff6","AT_PLATFORM":"0x6fffffd9","ei_index":26,"not_using_C1_C2":true}。

| 量 | 十六进制 | 相对U字节 | mod8 | mod16 | 类型/单位 | 公式/来源 | 新证据 |
|---|---|---|---|---|---|---|---|
| U | 0x70000000 | 0 | 0 | 0 | given integer address / byte | given; 197 common input |  |
| p0 | 0x6fffffe2 | -30 | 2 | 2 | ulong local p / bprm.p / byte | U-30; 140 string length + new given U | E142-005,E142-056 |
| p1 | 0x6fffffe0 | -32 | 0 | 0 | ulong local p / byte | floor16(p0); arch_align_stack | E142-008,E142-038,E142-037 |
| P | 0x6fffffd9 | -39 | 1 | 9 | ulong p / u_platform pointer numeric projection / byte | p1-7; strlen(x86_64)+1, STACK_ALLOC | E142-042,E142-009,E142-003,E142-010 |
| sp_aux | 0x6fffff09 | -247 | 1 | 9 | elf_addr_t user pointer numeric projection / byte (8-byte word step) | P-8*ei_index; ei_index and STACK_ADD | E142-018,E142-001,E142-045,E142-049 |
| raw_bottom | 0x6ffffed1 | -303 | 1 | 1 | intermediate integer projection of sp-items / byte (8-byte word step) | sp_aux-8*items; STACK_ROUND before clearing low bits | E142-019,E142-002 |
| S | 0x6ffffed0 | -304 | 0 | 0 | ulong bprm.p / sp reset projection / byte | floor16(raw_bottom); STACK_ROUND | E142-019,E142-002,E142-020 |
| Q | 0x6fffff08 | -248 | 0 | 8 | final table sp projection / byte | S+8*items; items slots after prefix | E142-023,E142-024,E142-026,E142-027,E142-029 |
| E | 0x6fffffd8 | -40 | 0 | 8 | exclusive copy end, not sp value / byte | Q+8*ei_index; copy length | E142-030 |
| F | 0x6ffffff6 | -10 | 6 | 6 | ulong bprm.exec value / byte | U-10; 140 filename + given U | E142-058,E142-014 |
| arg_scan_end | 0x6fffffed | -19 | 5 | 13 | local p at end argv scan / byte | p0+sum(argv lengths); 140 argv lengths | E142-025,E142-026 |

### R2（140 A2，141 Afd）

argc=1、envc=2，items=6字，ei_index=28字；F=0x6ffffff6。载荷：{"prior_ref":"08f6fee103dd","selector":"A.Afd","common":{"H":"0x1234","PHDR":"0x400040","PHNUM":3,"PHENT":56,"BASE":0,"ENTRY":"0x401020","AT_PAGESZ":4096,"AT_CLKTCK":250},"interp_flags":8,"AT_FLAGS":1,"secureexec":1,"have_execfd":true,"execfd":7,"AT_EXECFN":"0x6ffffff6","AT_PLATFORM":"0x6fffffd9","ei_index":28,"not_using_C1_C2":true}。

| 量 | 十六进制 | 相对U字节 | mod8 | mod16 | 类型/单位 | 公式/来源 | 新证据 |
|---|---|---|---|---|---|---|---|
| U | 0x70000000 | 0 | 0 | 0 | given integer address / byte | given; 197 common input |  |
| p0 | 0x6fffffec | -20 | 4 | 12 | ulong local p / bprm.p / byte | U-20; 140 string length + new given U | E142-005,E142-056 |
| p1 | 0x6fffffe0 | -32 | 0 | 0 | ulong local p / byte | floor16(p0); arch_align_stack | E142-008,E142-038,E142-037 |
| P | 0x6fffffd9 | -39 | 1 | 9 | ulong p / u_platform pointer numeric projection / byte | p1-7; strlen(x86_64)+1, STACK_ALLOC | E142-042,E142-009,E142-003,E142-010 |
| sp_aux | 0x6ffffef9 | -263 | 1 | 9 | elf_addr_t user pointer numeric projection / byte (8-byte word step) | P-8*ei_index; ei_index and STACK_ADD | E142-018,E142-001,E142-045,E142-049 |
| raw_bottom | 0x6ffffec9 | -311 | 1 | 9 | intermediate integer projection of sp-items / byte (8-byte word step) | sp_aux-8*items; STACK_ROUND before clearing low bits | E142-019,E142-002 |
| S | 0x6ffffec0 | -320 | 0 | 0 | ulong bprm.p / sp reset projection / byte | floor16(raw_bottom); STACK_ROUND | E142-019,E142-002,E142-020 |
| Q | 0x6ffffef0 | -272 | 0 | 0 | final table sp projection / byte | S+8*items; items slots after prefix | E142-023,E142-024,E142-026,E142-027,E142-029 |
| E | 0x6fffffd0 | -48 | 0 | 0 | exclusive copy end, not sp value / byte | Q+8*ei_index; copy length | E142-030 |
| F | 0x6ffffff6 | -10 | 6 | 6 | ulong bprm.exec value / byte | U-10; 140 filename + given U | E142-058,E142-014 |
| arg_scan_end | 0x6fffffed | -19 | 5 | 13 | local p at end argv scan / byte | p0+sum(argv lengths); 140 argv lengths | E142-025,E142-026 |

### R3（140 C_ENTRY，141 A0）

argc=3、envc=3，items=9字，ei_index=26字；F=0x6ffffff7。载荷：{"prior_ref":"08f6fee103dd","selector":"A.A0","common":{"H":"0x1234","PHDR":"0x400040","PHNUM":3,"PHENT":56,"BASE":0,"ENTRY":"0x401020","AT_PAGESZ":4096,"AT_CLKTCK":250},"interp_flags":0,"AT_FLAGS":0,"secureexec":0,"have_execfd":false,"execfd":"NOT_READ","AT_EXECFN":"0x6ffffff7","AT_PLATFORM":"0x6fffff99","ei_index":26,"not_using_C1_C2":true}。

| 量 | 十六进制 | 相对U字节 | mod8 | mod16 | 类型/单位 | 公式/来源 | 新证据 |
|---|---|---|---|---|---|---|---|
| U | 0x70000000 | 0 | 0 | 0 | given integer address / byte | given; 197 common input |  |
| p0 | 0x6fffffa3 | -93 | 3 | 3 | ulong local p / bprm.p / byte | U-93; 140 string length + new given U | E142-005,E142-056 |
| p1 | 0x6fffffa0 | -96 | 0 | 0 | ulong local p / byte | floor16(p0); arch_align_stack | E142-008,E142-038,E142-037 |
| P | 0x6fffff99 | -103 | 1 | 9 | ulong p / u_platform pointer numeric projection / byte | p1-7; strlen(x86_64)+1, STACK_ALLOC | E142-042,E142-009,E142-003,E142-010 |
| sp_aux | 0x6ffffec9 | -311 | 1 | 9 | elf_addr_t user pointer numeric projection / byte (8-byte word step) | P-8*ei_index; ei_index and STACK_ADD | E142-018,E142-001,E142-045,E142-049 |
| raw_bottom | 0x6ffffe81 | -383 | 1 | 1 | intermediate integer projection of sp-items / byte (8-byte word step) | sp_aux-8*items; STACK_ROUND before clearing low bits | E142-019,E142-002 |
| S | 0x6ffffe80 | -384 | 0 | 0 | ulong bprm.p / sp reset projection / byte | floor16(raw_bottom); STACK_ROUND | E142-019,E142-002,E142-020 |
| Q | 0x6ffffec8 | -312 | 0 | 8 | final table sp projection / byte | S+8*items; items slots after prefix | E142-023,E142-024,E142-026,E142-027,E142-029 |
| E | 0x6fffff98 | -104 | 0 | 8 | exclusive copy end, not sp value / byte | Q+8*ei_index; copy length | E142-030 |
| F | 0x6ffffff7 | -9 | 7 | 7 | ulong bprm.exec value / byte | U-9; 140 filename + given U | E142-058,E142-014 |
| arg_scan_end | 0x6fffffca | -54 | 2 | 10 | local p at end argv scan / byte | p0+sum(argv lengths); 140 argv lengths | E142-025,E142-026 |

三例S均16对齐；前缀及auxv槽均8对齐；P、sp_aux、raw_bottom均mod8=1。初始p0也可能不对齐，C指针转换、合法对象范围及指针算术独立未证；最终对齐不补偿此前潜在不合法操作。25已知双间隙，不重复报缺陷。所选载荷复用141 A，AT_EXECFN/PLATFORM实际绑定F/P，终止对由内核清尾提供；容量46字（368字节），不把内核清尾当用户栈清零。E142-012,E142-013,E142-016,E142-017,E142-061,E142-062,E142-063,E142-064,E142-014,E142-015。

## B · 六个半开区间、作用及守恒

### R1

| 区间ID | [start,end) | 字节 | 写入者/来源 | 本函数新写（局部条件） | 清零依据 | 语义 |
|---|---|---|---|---|---|---|
| R1_prefix | [0x6ffffed0,0x6fffff08) | 56 | put_user in post-vma independent fragment | true | two explicit NULL pointer stores; not blanket clear | argc argv envp pointer slots and terminators |
| R1_auxv | [0x6fffff08,0x6fffffd8) | 208 | copy_to_user from mm.saved_auxv | true | copies initialized payload plus zero-valued words/AT_NULL pair; does not copy full cleared kernel tail | selected variant A0 |
| R1_final_gap | [0x6fffffd8,0x6fffffd9) | 1 | STACK_ROUND downward rounding | false | NO_DEDICATED_ZERO_OR_WRITE_IN_READ_FRAGMENT | gap between copy end and platform |
| R1_platform | [0x6fffffd9,0x6fffffe0) | 7 | platform copy_to_user | true | string terminating NUL only; no blanket clear | x86_64 + NUL; hex 78 38 36 5f 36 34 00 |
| R1_initial_gap | [0x6fffffe0,0x6fffffe2) | 2 | arch_align_stack | false | NO_DEDICATED_ZERO_OR_WRITE_IN_READ_FRAGMENT | gap after platform, before original strings |
| R1_strings | [0x6fffffe2,0x70000000) | 30 | given pre-existing string region; argv/envp scan reads lengths | false | given original NUL bytes; no new clear | argv,envp,filename; filename saved F is not scanned in this table fragment |

守恒：前缀56+auxv208+最终间隙1+平台7+初始间隙2+既有字符串30=304=U−S；连续不重叠无遗漏。既有字符串分组及全部前缀槽：

| 源 | 串 | [start,end) | 长度 |
|---|---|---|---|
| argv[0] | "display" | [0x6fffffe2,0x6fffffea) | 8 |
| argv[1] | "-q" | [0x6fffffea,0x6fffffed) | 3 |
| envp[0] | "E=1" | [0x6fffffed,0x6ffffff1) | 4 |
| envp[1] | "F=xy" | [0x6ffffff1,0x6ffffff6) | 5 |
| filename | "/bin/tool" | [0x6ffffff6,0x70000000) | 10 |

| 槽 | 地址 | 值 | 140原符号值 |
|---|---|---|---|
| argc_integer | 0x6ffffed0 | 2 | 2 |
| argv[0] | 0x6ffffed8 | 0x6fffffe2 | U-30 |
| argv[1] | 0x6ffffee0 | 0x6fffffea | U-22 |
| argv_terminator | 0x6ffffee8 | 0 | 0 |
| envp[0] | 0x6ffffef0 | 0x6fffffed | U-19 |
| envp[1] | 0x6ffffef8 | 0x6ffffff1 | U-15 |
| envp_terminator | 0x6fffff00 | 0 | 0 |

| 条目 | tag字序 | tag地址 | tag | value字序 | value地址 | value |
|---|---|---|---|---|---|---|
| AT_EXECFN | 20 | 0x6fffffa8 | 31 | 21 | 0x6fffffb0 | 0x6ffffff6 |
| AT_PLATFORM | 22 | 0x6fffffb8 | 15 | 23 | 0x6fffffc0 | 0x6fffffd9 |
| AT_NULL | 24 | 0x6fffffc8 | 0 | 25 | 0x6fffffd0 | 0 |

Q=0x6fffff08、E=0x6fffffd8；argv[0]=0x6fffffe2，F=0x6ffffff6，P=0x6fffffd9，S=0x6ffffed0，互不替代。F保存/bin/tool，argv[0]指向display。

### R2

| 区间ID | [start,end) | 字节 | 写入者/来源 | 本函数新写（局部条件） | 清零依据 | 语义 |
|---|---|---|---|---|---|---|
| R2_prefix | [0x6ffffec0,0x6ffffef0) | 48 | put_user in post-vma independent fragment | true | two explicit NULL pointer stores; not blanket clear | argc argv envp pointer slots and terminators |
| R2_auxv | [0x6ffffef0,0x6fffffd0) | 224 | copy_to_user from mm.saved_auxv | true | copies initialized payload plus zero-valued words/AT_NULL pair; does not copy full cleared kernel tail | selected variant Afd |
| R2_final_gap | [0x6fffffd0,0x6fffffd9) | 9 | STACK_ROUND downward rounding | false | NO_DEDICATED_ZERO_OR_WRITE_IN_READ_FRAGMENT | gap between copy end and platform |
| R2_platform | [0x6fffffd9,0x6fffffe0) | 7 | platform copy_to_user | true | string terminating NUL only; no blanket clear | x86_64 + NUL; hex 78 38 36 5f 36 34 00 |
| R2_initial_gap | [0x6fffffe0,0x6fffffec) | 12 | arch_align_stack | false | NO_DEDICATED_ZERO_OR_WRITE_IN_READ_FRAGMENT | gap after platform, before original strings |
| R2_strings | [0x6fffffec,0x70000000) | 20 | given pre-existing string region; argv/envp scan reads lengths | false | given original NUL bytes; no new clear | argv,envp,filename; filename saved F is not scanned in this table fragment |

守恒：前缀48+auxv224+最终间隙9+平台7+初始间隙12+既有字符串20=320=U−S；连续不重叠无遗漏。既有字符串分组及全部前缀槽：

| 源 | 串 | [start,end) | 长度 |
|---|---|---|---|
| argv[0] | "" | [0x6fffffec,0x6fffffed) | 1 |
| envp[0] | "E=1" | [0x6fffffed,0x6ffffff1) | 4 |
| envp[1] | "F=xy" | [0x6ffffff1,0x6ffffff6) | 5 |
| filename | "/bin/tool" | [0x6ffffff6,0x70000000) | 10 |

| 槽 | 地址 | 值 | 140原符号值 |
|---|---|---|---|
| argc_integer | 0x6ffffec0 | 1 | 1 |
| argv[0] | 0x6ffffec8 | 0x6fffffec | U-20 |
| argv_terminator | 0x6ffffed0 | 0 | 0 |
| envp[0] | 0x6ffffed8 | 0x6fffffed | U-19 |
| envp[1] | 0x6ffffee0 | 0x6ffffff1 | U-15 |
| envp_terminator | 0x6ffffee8 | 0 | 0 |

| 条目 | tag字序 | tag地址 | tag | value字序 | value地址 | value |
|---|---|---|---|---|---|---|
| AT_EXECFN | 20 | 0x6fffff90 | 31 | 21 | 0x6fffff98 | 0x6ffffff6 |
| AT_PLATFORM | 22 | 0x6fffffa0 | 15 | 23 | 0x6fffffa8 | 0x6fffffd9 |
| AT_NULL | 26 | 0x6fffffc0 | 0 | 27 | 0x6fffffc8 | 0 |

Q=0x6ffffef0、E=0x6fffffd0；argv[0]=0x6fffffec，F=0x6ffffff6，P=0x6fffffd9，S=0x6ffffec0，互不替代。argv[0]非空指针指向单个NUL，不是argv终止空指针。

### R3

| 区间ID | [start,end) | 字节 | 写入者/来源 | 本函数新写（局部条件） | 清零依据 | 语义 |
|---|---|---|---|---|---|---|
| R3_prefix | [0x6ffffe80,0x6ffffec8) | 72 | put_user in post-vma independent fragment | true | two explicit NULL pointer stores; not blanket clear | argc argv envp pointer slots and terminators |
| R3_auxv | [0x6ffffec8,0x6fffff98) | 208 | copy_to_user from mm.saved_auxv | true | copies initialized payload plus zero-valued words/AT_NULL pair; does not copy full cleared kernel tail | selected variant A0 |
| R3_final_gap | [0x6fffff98,0x6fffff99) | 1 | STACK_ROUND downward rounding | false | NO_DEDICATED_ZERO_OR_WRITE_IN_READ_FRAGMENT | gap between copy end and platform |
| R3_platform | [0x6fffff99,0x6fffffa0) | 7 | platform copy_to_user | true | string terminating NUL only; no blanket clear | x86_64 + NUL; hex 78 38 36 5f 36 34 00 |
| R3_initial_gap | [0x6fffffa0,0x6fffffa3) | 3 | arch_align_stack | false | NO_DEDICATED_ZERO_OR_WRITE_IN_READ_FRAGMENT | gap after platform, before original strings |
| R3_strings | [0x6fffffa3,0x70000000) | 93 | given pre-existing string region; argv/envp scan reads lengths | false | given original NUL bytes; no new clear | argv,envp,filename; filename saved F is not scanned in this table fragment |

守恒：前缀72+auxv208+最终间隙1+平台7+初始间隙3+既有字符串93=384=U−S；连续不重叠无遗漏。既有字符串分组及全部前缀槽：

| 源 | 串 | [start,end) | 长度 |
|---|---|---|---|
| argv[0] | "/boot/sh" | [0x6fffffa3,0x6fffffac) | 9 |
| argv[1] | "userarg_test_1" | [0x6fffffac,0x6fffffbb) | 15 |
| argv[2] | "userarg_test_2" | [0x6fffffbb,0x6fffffca) | 15 |
| envp[0] | "userenv_test_1" | [0x6fffffca,0x6fffffd9) | 15 |
| envp[1] | "userenv_test_2" | [0x6fffffd9,0x6fffffe8) | 15 |
| envp[2] | "userenv_test_3" | [0x6fffffe8,0x6ffffff7) | 15 |
| filename | "/boot/sh" | [0x6ffffff7,0x70000000) | 9 |

| 槽 | 地址 | 值 | 140原符号值 |
|---|---|---|---|
| argc_integer | 0x6ffffe80 | 3 | 3 |
| argv[0] | 0x6ffffe88 | 0x6fffffa3 | U-93 |
| argv[1] | 0x6ffffe90 | 0x6fffffac | U-84 |
| argv[2] | 0x6ffffe98 | 0x6fffffbb | U-69 |
| argv_terminator | 0x6ffffea0 | 0 | 0 |
| envp[0] | 0x6ffffea8 | 0x6fffffca | U-54 |
| envp[1] | 0x6ffffeb0 | 0x6fffffd9 | U-39 |
| envp[2] | 0x6ffffeb8 | 0x6fffffe8 | U-24 |
| envp_terminator | 0x6ffffec0 | 0 | 0 |

| 条目 | tag字序 | tag地址 | tag | value字序 | value地址 | value |
|---|---|---|---|---|---|---|
| AT_EXECFN | 20 | 0x6fffff68 | 31 | 21 | 0x6fffff70 | 0x6ffffff7 |
| AT_PLATFORM | 22 | 0x6fffff78 | 15 | 23 | 0x6fffff80 | 0x6fffff99 |
| AT_NULL | 24 | 0x6fffff88 | 0 | 25 | 0x6fffff90 | 0 |

Q=0x6ffffec8、E=0x6fffff98；argv[0]=0x6fffffa3，F=0x6ffffff7，P=0x6fffff99，S=0x6ffffe80，互不替代。两份/boot/sh内容相同但地址不同。

两个间隙在所读局部片段中未见专门写入或清零；平台复制仅7字节含NUL，前缀只逐槽写，auxv仅ei_index*8字节，内核saved_auxv未复制零尾不能推出用户padding为零。既有字符串区给定并被argv/envp扫描，保存文件名并不由这些循环重新复制或扫描。未断言旧数据泄露、真实残留或可利用性。

## C · 变量时点与caller栈实参

每行是前述语句相继完成后的观察点，不表示同一原子操作。T01–T06为门前整数模型；T07–T13为独立给定到达的门后片段；T14仅给定正常返回及caller后续到达。入口mm.arg_end/env_start/env_end与尚未初始化u_platform按NOT_STIPULATED记录；local p、sp不同于bprm.p。写argc时p仍P（对140 PRE_RESET_P_NOT_STIPULATED作新增题设具体化），随后278行重置p。最后一次后缀递减条件为假后local argc/envc=-1，bprm计数不写回。E142-005,E142-023,E142-024,E142-025,E142-026,E142-027,E142-028,E142-029,E142-030。

### R1

| ID / 阶段 | segment | local p | sp | bprm.p | u_platform | mm.arg_start | mm.arg_end | mm.env_start | mm.env_end | bprm.exec | local argc/envc | mm.start_stack |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| R1_T01 entry initialized | PRE_GATE | 0x6fffffe2 | 0x6fffffe2 | 0x6fffffe2 | NOT_STIPULATED | 0x6fffffe2 | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | 2/2 | NOT_STIPULATED |
| R1_T02 after arch_align_stack | PRE_GATE | 0x6fffffe0 | 0x6fffffe2 | 0x6fffffe2 | NOT_STIPULATED | 0x6fffffe2 | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | 2/2 | NOT_STIPULATED |
| R1_T03 platform successfully allocated/copied | PRE_GATE | 0x6fffffd9 | 0x6fffffe2 | 0x6fffffe2 | 0x6fffffd9 | 0x6fffffe2 | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | 2/2 | NOT_STIPULATED |
| R1_T04 after STACK_ADD | PRE_GATE | 0x6fffffd9 | 0x6fffff09 | 0x6fffffe2 | 0x6fffffd9 | 0x6fffffe2 | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | 2/2 | NOT_STIPULATED |
| R1_T05 after STACK_ROUND assignment | PRE_GATE | 0x6fffffd9 | 0x6fffff09 | 0x6ffffed0 | 0x6fffffd9 | 0x6fffffe2 | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | 2/2 | NOT_STIPULATED |
| R1_T06 after sp reset; before vma gate | PRE_GATE | 0x6fffffd9 | 0x6ffffed0 | 0x6ffffed0 | 0x6fffffd9 | 0x6fffffe2 | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | 2/2 | NOT_STIPULATED |
| R1_T07 after argc write | POST_GATE | 0x6fffffd9 | 0x6ffffed8 | 0x6ffffed0 | 0x6fffffd9 | 0x6fffffe2 | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | 2/2 | NOT_STIPULATED |
| R1_T08 after arg reset | POST_GATE | 0x6fffffe2 | 0x6ffffed8 | 0x6ffffed0 | 0x6fffffd9 | 0x6fffffe2 | 0x6fffffe2 | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | 2/2 | NOT_STIPULATED |
| R1_T09 after argv loop (failed final test) | POST_GATE | 0x6fffffed | 0x6ffffee8 | 0x6ffffed0 | 0x6fffffd9 | 0x6fffffe2 | 0x6fffffe2 | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | -1/2 | NOT_STIPULATED |
| R1_T10 after argv NULL, arg_end update and env boundary initialization | POST_GATE | 0x6fffffed | 0x6ffffef0 | 0x6ffffed0 | 0x6fffffd9 | 0x6fffffe2 | 0x6fffffed | 0x6fffffed | 0x6fffffed | 0x6ffffff6 | -1/2 | NOT_STIPULATED |
| R1_T11 after envp loop (before env NULL) | POST_GATE | 0x6ffffff6 | 0x6fffff00 | 0x6ffffed0 | 0x6fffffd9 | 0x6fffffe2 | 0x6fffffed | 0x6fffffed | 0x6fffffed | 0x6ffffff6 | -1/-1 | NOT_STIPULATED |
| R1_T12 after env NULL and env_end update | POST_GATE | 0x6ffffff6 | 0x6fffff08 | 0x6ffffed0 | 0x6fffffd9 | 0x6fffffe2 | 0x6fffffed | 0x6fffffed | 0x6ffffff6 | 0x6ffffff6 | -1/-1 | NOT_STIPULATED |
| R1_T13 auxv copy returns zero and local return 0 | POST_GATE | 0x6ffffff6 | 0x6fffff08 | 0x6ffffed0 | 0x6fffffd9 | 0x6fffffe2 | 0x6fffffed | 0x6fffffed | 0x6ffffff6 | 0x6ffffff6 | -1/-1 | NOT_STIPULATED |
| R1_T14 caller consumption after normal return and later gates | CALLER | OUT_OF_SCOPE (last 0x6ffffff6) | OUT_OF_SCOPE (last 0x6fffff08) | 0x6ffffed0 | OUT_OF_SCOPE (last 0x6fffffd9) | 0x6fffffe2 | 0x6fffffed | 0x6fffffed | 0x6ffffff6 | 0x6ffffff6 | OUT_OF_SCOPE (last -1)/OUT_OF_SCOPE (last -1) | 0x6ffffed0 |

T11：p=0x6ffffff6、sp=0x6fffff00=Q−8、mm.env_end仍0x6fffffed（env_start）；T12成功写终止零槽并更新env_end后，sp=0x6fffff08=Q、mm.env_end=0x6ffffff6；T13 copy返回0才局部return0，sp仍Q而非E=0x6fffffd8。T14 local p/sp/u_platform已出作用域；在caller表正常返回及后续条件下，mm.start_stack与START_THREAD栈实参均0x6ffffed0=S，非F/E；bprm计数保持2/2。E142-031,E142-032,E142-033,E142-034,E142-035,E142-048。实际CPU SP及CRT未证。

### R2

| ID / 阶段 | segment | local p | sp | bprm.p | u_platform | mm.arg_start | mm.arg_end | mm.env_start | mm.env_end | bprm.exec | local argc/envc | mm.start_stack |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| R2_T01 entry initialized | PRE_GATE | 0x6fffffec | 0x6fffffec | 0x6fffffec | NOT_STIPULATED | 0x6fffffec | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | 1/2 | NOT_STIPULATED |
| R2_T02 after arch_align_stack | PRE_GATE | 0x6fffffe0 | 0x6fffffec | 0x6fffffec | NOT_STIPULATED | 0x6fffffec | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | 1/2 | NOT_STIPULATED |
| R2_T03 platform successfully allocated/copied | PRE_GATE | 0x6fffffd9 | 0x6fffffec | 0x6fffffec | 0x6fffffd9 | 0x6fffffec | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | 1/2 | NOT_STIPULATED |
| R2_T04 after STACK_ADD | PRE_GATE | 0x6fffffd9 | 0x6ffffef9 | 0x6fffffec | 0x6fffffd9 | 0x6fffffec | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | 1/2 | NOT_STIPULATED |
| R2_T05 after STACK_ROUND assignment | PRE_GATE | 0x6fffffd9 | 0x6ffffef9 | 0x6ffffec0 | 0x6fffffd9 | 0x6fffffec | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | 1/2 | NOT_STIPULATED |
| R2_T06 after sp reset; before vma gate | PRE_GATE | 0x6fffffd9 | 0x6ffffec0 | 0x6ffffec0 | 0x6fffffd9 | 0x6fffffec | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | 1/2 | NOT_STIPULATED |
| R2_T07 after argc write | POST_GATE | 0x6fffffd9 | 0x6ffffec8 | 0x6ffffec0 | 0x6fffffd9 | 0x6fffffec | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | 1/2 | NOT_STIPULATED |
| R2_T08 after arg reset | POST_GATE | 0x6fffffec | 0x6ffffec8 | 0x6ffffec0 | 0x6fffffd9 | 0x6fffffec | 0x6fffffec | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | 1/2 | NOT_STIPULATED |
| R2_T09 after argv loop (failed final test) | POST_GATE | 0x6fffffed | 0x6ffffed0 | 0x6ffffec0 | 0x6fffffd9 | 0x6fffffec | 0x6fffffec | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff6 | -1/2 | NOT_STIPULATED |
| R2_T10 after argv NULL, arg_end update and env boundary initialization | POST_GATE | 0x6fffffed | 0x6ffffed8 | 0x6ffffec0 | 0x6fffffd9 | 0x6fffffec | 0x6fffffed | 0x6fffffed | 0x6fffffed | 0x6ffffff6 | -1/2 | NOT_STIPULATED |
| R2_T11 after envp loop (before env NULL) | POST_GATE | 0x6ffffff6 | 0x6ffffee8 | 0x6ffffec0 | 0x6fffffd9 | 0x6fffffec | 0x6fffffed | 0x6fffffed | 0x6fffffed | 0x6ffffff6 | -1/-1 | NOT_STIPULATED |
| R2_T12 after env NULL and env_end update | POST_GATE | 0x6ffffff6 | 0x6ffffef0 | 0x6ffffec0 | 0x6fffffd9 | 0x6fffffec | 0x6fffffed | 0x6fffffed | 0x6ffffff6 | 0x6ffffff6 | -1/-1 | NOT_STIPULATED |
| R2_T13 auxv copy returns zero and local return 0 | POST_GATE | 0x6ffffff6 | 0x6ffffef0 | 0x6ffffec0 | 0x6fffffd9 | 0x6fffffec | 0x6fffffed | 0x6fffffed | 0x6ffffff6 | 0x6ffffff6 | -1/-1 | NOT_STIPULATED |
| R2_T14 caller consumption after normal return and later gates | CALLER | OUT_OF_SCOPE (last 0x6ffffff6) | OUT_OF_SCOPE (last 0x6ffffef0) | 0x6ffffec0 | OUT_OF_SCOPE (last 0x6fffffd9) | 0x6fffffec | 0x6fffffed | 0x6fffffed | 0x6ffffff6 | 0x6ffffff6 | OUT_OF_SCOPE (last -1)/OUT_OF_SCOPE (last -1) | 0x6ffffec0 |

T11：p=0x6ffffff6、sp=0x6ffffee8=Q−8、mm.env_end仍0x6fffffed（env_start）；T12成功写终止零槽并更新env_end后，sp=0x6ffffef0=Q、mm.env_end=0x6ffffff6；T13 copy返回0才局部return0，sp仍Q而非E=0x6fffffd0。T14 local p/sp/u_platform已出作用域；在caller表正常返回及后续条件下，mm.start_stack与START_THREAD栈实参均0x6ffffec0=S，非F/E；bprm计数保持1/2。E142-031,E142-032,E142-033,E142-034,E142-035,E142-048。实际CPU SP及CRT未证。

### R3

| ID / 阶段 | segment | local p | sp | bprm.p | u_platform | mm.arg_start | mm.arg_end | mm.env_start | mm.env_end | bprm.exec | local argc/envc | mm.start_stack |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| R3_T01 entry initialized | PRE_GATE | 0x6fffffa3 | 0x6fffffa3 | 0x6fffffa3 | NOT_STIPULATED | 0x6fffffa3 | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff7 | 3/3 | NOT_STIPULATED |
| R3_T02 after arch_align_stack | PRE_GATE | 0x6fffffa0 | 0x6fffffa3 | 0x6fffffa3 | NOT_STIPULATED | 0x6fffffa3 | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff7 | 3/3 | NOT_STIPULATED |
| R3_T03 platform successfully allocated/copied | PRE_GATE | 0x6fffff99 | 0x6fffffa3 | 0x6fffffa3 | 0x6fffff99 | 0x6fffffa3 | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff7 | 3/3 | NOT_STIPULATED |
| R3_T04 after STACK_ADD | PRE_GATE | 0x6fffff99 | 0x6ffffec9 | 0x6fffffa3 | 0x6fffff99 | 0x6fffffa3 | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff7 | 3/3 | NOT_STIPULATED |
| R3_T05 after STACK_ROUND assignment | PRE_GATE | 0x6fffff99 | 0x6ffffec9 | 0x6ffffe80 | 0x6fffff99 | 0x6fffffa3 | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff7 | 3/3 | NOT_STIPULATED |
| R3_T06 after sp reset; before vma gate | PRE_GATE | 0x6fffff99 | 0x6ffffe80 | 0x6ffffe80 | 0x6fffff99 | 0x6fffffa3 | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff7 | 3/3 | NOT_STIPULATED |
| R3_T07 after argc write | POST_GATE | 0x6fffff99 | 0x6ffffe88 | 0x6ffffe80 | 0x6fffff99 | 0x6fffffa3 | NOT_STIPULATED | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff7 | 3/3 | NOT_STIPULATED |
| R3_T08 after arg reset | POST_GATE | 0x6fffffa3 | 0x6ffffe88 | 0x6ffffe80 | 0x6fffff99 | 0x6fffffa3 | 0x6fffffa3 | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff7 | 3/3 | NOT_STIPULATED |
| R3_T09 after argv loop (failed final test) | POST_GATE | 0x6fffffca | 0x6ffffea0 | 0x6ffffe80 | 0x6fffff99 | 0x6fffffa3 | 0x6fffffa3 | NOT_STIPULATED | NOT_STIPULATED | 0x6ffffff7 | -1/3 | NOT_STIPULATED |
| R3_T10 after argv NULL, arg_end update and env boundary initialization | POST_GATE | 0x6fffffca | 0x6ffffea8 | 0x6ffffe80 | 0x6fffff99 | 0x6fffffa3 | 0x6fffffca | 0x6fffffca | 0x6fffffca | 0x6ffffff7 | -1/3 | NOT_STIPULATED |
| R3_T11 after envp loop (before env NULL) | POST_GATE | 0x6ffffff7 | 0x6ffffec0 | 0x6ffffe80 | 0x6fffff99 | 0x6fffffa3 | 0x6fffffca | 0x6fffffca | 0x6fffffca | 0x6ffffff7 | -1/-1 | NOT_STIPULATED |
| R3_T12 after env NULL and env_end update | POST_GATE | 0x6ffffff7 | 0x6ffffec8 | 0x6ffffe80 | 0x6fffff99 | 0x6fffffa3 | 0x6fffffca | 0x6fffffca | 0x6ffffff7 | 0x6ffffff7 | -1/-1 | NOT_STIPULATED |
| R3_T13 auxv copy returns zero and local return 0 | POST_GATE | 0x6ffffff7 | 0x6ffffec8 | 0x6ffffe80 | 0x6fffff99 | 0x6fffffa3 | 0x6fffffca | 0x6fffffca | 0x6ffffff7 | 0x6ffffff7 | -1/-1 | NOT_STIPULATED |
| R3_T14 caller consumption after normal return and later gates | CALLER | OUT_OF_SCOPE (last 0x6ffffff7) | OUT_OF_SCOPE (last 0x6ffffec8) | 0x6ffffe80 | OUT_OF_SCOPE (last 0x6fffff99) | 0x6fffffa3 | 0x6fffffca | 0x6fffffca | 0x6ffffff7 | 0x6ffffff7 | OUT_OF_SCOPE (last -1)/OUT_OF_SCOPE (last -1) | 0x6ffffe80 |

T11：p=0x6ffffff7、sp=0x6ffffec0=Q−8、mm.env_end仍0x6fffffca（env_start）；T12成功写终止零槽并更新env_end后，sp=0x6ffffec8=Q、mm.env_end=0x6ffffff7；T13 copy返回0才局部return0，sp仍Q而非E=0x6fffff98。T14 local p/sp/u_platform已出作用域；在caller表正常返回及后续条件下，mm.start_stack与START_THREAD栈实参均0x6ffffe80=S，非F/E；bprm计数保持3/3。E142-031,E142-032,E142-033,E142-034,E142-035,E142-048。实际CPU SP及CRT未证。

## 边界与消费

本轮新正式源码窄窗 73，定义/使用/注释/配置分开，详见evidence与facts.source_index。旧141 129窗只计D00。图95节点/100条from/to/kind/label四元组，独立片段以不同关系标示；不构造真实执行轨迹。完整本地P9、预处理、编译链接、MyOS2程序／脚本、Graphviz、QEMU、实机及真实操作均NOT_RUN。C10独立验收及R141-01复判PENDING；不自行启动143。
