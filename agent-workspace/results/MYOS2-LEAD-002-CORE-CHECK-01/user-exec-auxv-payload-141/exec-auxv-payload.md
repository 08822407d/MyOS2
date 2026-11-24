---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
batch: "CORE-USER-EXEC-AUXV-PAYLOAD-141"
executor: "MYOS2-LEAD-002-CHAT-EXEC-22"
date: "2026-11-24"
base_snapshot: "time@a039d9803ade"
phase: 2
produced_by: "GPT-6.2 system family; backend/tier/effort unknown"
model_per_owner: "GPT-6.2 Sol Extra/high requested; actual unknown"
evidence_class: "pinned source/static model/prior accepted result"
status: "AUTHOR_STATIC_CONTENT_PREPARED_PENDING_PUBLICATION_AND_G04"
mainline_acceptance: "PENDING"
full_local_P9: "NOT_RUN"
record_type: "analysis"
---
# 141 auxv 有序载荷及地址来源

140 PASS_PENDING_LOCAL，R138-03及R139-01~04 CLOSED_DOCUMENTATION_ONLY；D00为空，不重做旧件。固定源码中 elf_addr_t=Elf64_Off，不是Elf64_Addr；二者均__u64。elf_phdr/elf_phdr_s对应elf64_phdr，两个4字节Word与六个8字节字段，所选布局56字节；elfhdr/elfhdr_s对应elf64_hdr。E141-070,E141-078,E141-076,E141-071,E141-073,E141-072,E141-074,E141-083,E141-084。构建声明-m64等不是预处理结果。E141-127,E141-128,E141-129。所选ARCH_DLINFO、HWCAP2、RSEQ不选，ELF_BASE_PLATFORM=NULL、ELF_PLATFORM=x86_64、ARCH大小0。H=0x1234为题设表达式值，非实际CPU。P为成功复制7字节x86_64及NUL后的用户地址，独立于S/U，不是内核常量地址。vma未初始化且赋值注释，以下只在所列条件下推演，不能推为实际可达。E141-010,E141-044,E141-043,E141-012,E141-013。

## A 有序tag/value
elf_info起于saved_auxv，NEW_AUX_ENT按id,val各写一字并前移，tag/value均8字节。输入位1<<3=8，输出位1<<0=1；不可复制8。HZ=1000与USER_HZ=250/CLOCKS_PER_SEC不同。非零tag的零value仍有效；AT_NULL两零由清零产生。
### A0
输入：{"H":"0x1234","phdr_addr":"0x400040","e_phnum":3,"interp_load_addr":0,"e_entry":"0x401020","interp_flags":0,"secureexec":0,"have_execfd":false,"execfd":"not_read","F":"bprm->exec","P":"copied platform user address"}

| 序号 | tag | 名称 | tag/value字序 | value | 类型 | 来源 | 条件 | 证据 |
|---|---|---|---|---|---|---|---|---|
| 1 | 16 | AT_HWCAP | 0,1 | 0x1234 | 位集 | ELF_HWCAP；题设H | always | E141-101,E141-020,E141-069 |
| 2 | 6 | AT_PAGESZ | 2,3 | 4096 | 字节数 | ELF_EXEC_PAGESIZE | always | E141-091,E141-021,E141-068 |
| 3 | 17 | AT_CLKTCK | 4,5 | 250 | 频率 | CLOCKS_PER_SEC=USER_HZ，非HZ | always | E141-102,E141-022,E141-125,E141-124,E141-123 |
| 4 | 3 | AT_PHDR | 6,7 | 0x400040 | 用户地址 | phdr_addr形参 | always | E141-088,E141-023,E141-006 |
| 5 | 4 | AT_PHENT | 8,9 | 56 | 字节数 | sizeof(struct elf_phdr)，非e_phentsize | always | E141-089,E141-024,E141-072,E141-083,E141-084,E141-079,E141-080 |
| 6 | 5 | AT_PHNUM | 10,11 | 3 | 数量 | exec->e_phnum | always | E141-090,E141-025,E141-082 |
| 7 | 7 | AT_BASE | 12,13 | 0 | 地址/零 | interp_load_addr | always | E141-092,E141-026,E141-006 |
| 8 | 8 | AT_FLAGS | 14,15 | 0 | 标志 | flags初始0；输入位8测试，输出位1 | always | E141-093,E141-009,E141-027,E141-028,E141-116,E141-117,E141-118,E141-119 |
| 9 | 9 | AT_ENTRY | 16,17 | 0x401020 | 用户地址 | e_entry主程序入口 | always | E141-094,E141-029,E141-006 |
| 10 | 23 | AT_SECURE | 18,19 | 0 | 布尔字 | bprm->secureexec | always | E141-103,E141-031,E141-114 |
| 11 | 31 | AT_EXECFN | 20,21 | F | 用户字符串地址 | bprm->exec保存文件名 | always | E141-111,E141-034,E141-115 |
| 12 | 15 | AT_PLATFORM | 22,23 | P | 用户字符串地址 | u_platform，成功复制x86_64及NUL | k_platform!=NULL | E141-100,E141-035,E141-012,E141-013,E141-067 |
| 13 | 0 | AT_NULL | 24,25 | 0 | 终止 | memset清零，不是宏写入 | always | E141-085,E141-039,E141-040,E141-041 |

### Afd
输入：{"H":"0x1234","phdr_addr":"0x400040","e_phnum":3,"interp_load_addr":0,"e_entry":"0x401020","interp_flags":8,"secureexec":1,"have_execfd":true,"execfd":7,"F":"bprm->exec","P":"copied platform user address"}

| 序号 | tag | 名称 | tag/value字序 | value | 类型 | 来源 | 条件 | 证据 |
|---|---|---|---|---|---|---|---|---|
| 1 | 16 | AT_HWCAP | 0,1 | 0x1234 | 位集 | ELF_HWCAP；题设H | always | E141-101,E141-020,E141-069 |
| 2 | 6 | AT_PAGESZ | 2,3 | 4096 | 字节数 | ELF_EXEC_PAGESIZE | always | E141-091,E141-021,E141-068 |
| 3 | 17 | AT_CLKTCK | 4,5 | 250 | 频率 | CLOCKS_PER_SEC=USER_HZ，非HZ | always | E141-102,E141-022,E141-125,E141-124,E141-123 |
| 4 | 3 | AT_PHDR | 6,7 | 0x400040 | 用户地址 | phdr_addr形参 | always | E141-088,E141-023,E141-006 |
| 5 | 4 | AT_PHENT | 8,9 | 56 | 字节数 | sizeof(struct elf_phdr)，非e_phentsize | always | E141-089,E141-024,E141-072,E141-083,E141-084,E141-079,E141-080 |
| 6 | 5 | AT_PHNUM | 10,11 | 3 | 数量 | exec->e_phnum | always | E141-090,E141-025,E141-082 |
| 7 | 7 | AT_BASE | 12,13 | 0 | 地址/零 | interp_load_addr | always | E141-092,E141-026,E141-006 |
| 8 | 8 | AT_FLAGS | 14,15 | 1 | 标志 | flags初始0；输入位8测试，输出位1 | always | E141-093,E141-009,E141-027,E141-028,E141-116,E141-117,E141-118,E141-119 |
| 9 | 9 | AT_ENTRY | 16,17 | 0x401020 | 用户地址 | e_entry主程序入口 | always | E141-094,E141-029,E141-006 |
| 10 | 23 | AT_SECURE | 18,19 | 1 | 布尔字 | bprm->secureexec | always | E141-103,E141-031,E141-114 |
| 11 | 31 | AT_EXECFN | 20,21 | F | 用户字符串地址 | bprm->exec保存文件名 | always | E141-111,E141-034,E141-115 |
| 12 | 15 | AT_PLATFORM | 22,23 | P | 用户字符串地址 | u_platform，成功复制x86_64及NUL | k_platform!=NULL | E141-100,E141-035,E141-012,E141-013,E141-067 |
| 13 | 2 | AT_EXECFD | 24,25 | 7 | 整数fd | bprm->execfd | bprm->have_execfd | E141-087,E141-037,E141-113,E141-115 |
| 14 | 0 | AT_NULL | 26,27 | 0 | 终止 | memset清零，不是宏写入 | always | E141-085,E141-039,E141-040,E141-041 |

### 不加入项
| 项 | 区分定义/注释/条件 | 证据 |
|---|---|---|
| AT_UID,AT_EUID,AT_GID,AT_EGID | 定义存在；加入处为注释 | E141-096,E141-097,E141-098,E141-099,E141-030 |
| AT_RANDOM | 定义存在；随机生成及加入注释 | E141-105,E141-016,E141-032 |
| AT_HWCAP2 | 条件加入；所选宏不存在；架构定义注释 | E141-106,E141-033 |
| ARCH_DLINFO | 所选宏不存在；架构候选注释 | E141-018,E141-019 |
| AT_BASE_PLATFORM | 定义存在，base platform=NULL使条件假 | E141-104,E141-004,E141-014,E141-036 |
| AT_RSEQ_FEATURE_SIZE,AT_RSEQ_ALIGN | 条件加入；所选CONFIG_RSEQ不存在 | E141-107,E141-108,E141-038 |
| AT_IGNORE,AT_NOTELF,AT_HWCAP3,AT_HWCAP4,AT_MINSIGSTKSZ | 有定义不等于本函数加入 | E141-086,E141-095,E141-109,E141-110,E141-112 |

A0不读取execfd；Afd在平台项后、NULL前追加2/7。未选分支不等于全部实际构建配置不存在。

## B 三个140表尾、逐字地址
S为表底、U为先前字符串符号锚、Q为auxv首字。承继140的argc/argv/null/envp/null，不重验140。F为bprm->exec保存的文件名地址，不是argv0；B2 argv0即使为空字符串仍为非NULL。每行列两字，无省略。
### B1（A0）
前缀：[2,"U-30","U-22",0,"U-19","U-15",0]；Q=S+56，F=U-10（/bin/tool），argv0=U-30；复制区间[S+56,S+264) 208字节。NULL字地址S+248 / S+256，Q偏移192 / 200。末地址不是sp更新。

| pair | 名称 | tag字序 | tag地址 | tag值 | value字序 | value地址 | value |
|---|---|---|---|---|---|---|---|
| 1 | AT_HWCAP | 0 | S+56 | 16 | 1 | S+64 | 0x1234 |
| 2 | AT_PAGESZ | 2 | S+72 | 6 | 3 | S+80 | 4096 |
| 3 | AT_CLKTCK | 4 | S+88 | 17 | 5 | S+96 | 250 |
| 4 | AT_PHDR | 6 | S+104 | 3 | 7 | S+112 | 0x400040 |
| 5 | AT_PHENT | 8 | S+120 | 4 | 9 | S+128 | 56 |
| 6 | AT_PHNUM | 10 | S+136 | 5 | 11 | S+144 | 3 |
| 7 | AT_BASE | 12 | S+152 | 7 | 13 | S+160 | 0 |
| 8 | AT_FLAGS | 14 | S+168 | 8 | 15 | S+176 | 0 |
| 9 | AT_ENTRY | 16 | S+184 | 9 | 17 | S+192 | 0x401020 |
| 10 | AT_SECURE | 18 | S+200 | 23 | 19 | S+208 | 0 |
| 11 | AT_EXECFN | 20 | S+216 | 31 | 21 | S+224 | U-10 |
| 12 | AT_PLATFORM | 22 | S+232 | 15 | 23 | S+240 | P |
| 13 | AT_NULL | 24 | S+248 | 0 | 25 | S+256 | 0 |

### B2（Afd）
前缀：[1,"U-20",0,"U-19","U-15",0]；Q=S+48，F=U-10（/bin/tool），argv0=U-20；复制区间[S+48,S+272) 224字节。NULL字地址S+256 / S+264，Q偏移208 / 216。末地址不是sp更新。

| pair | 名称 | tag字序 | tag地址 | tag值 | value字序 | value地址 | value |
|---|---|---|---|---|---|---|---|
| 1 | AT_HWCAP | 0 | S+48 | 16 | 1 | S+56 | 0x1234 |
| 2 | AT_PAGESZ | 2 | S+64 | 6 | 3 | S+72 | 4096 |
| 3 | AT_CLKTCK | 4 | S+80 | 17 | 5 | S+88 | 250 |
| 4 | AT_PHDR | 6 | S+96 | 3 | 7 | S+104 | 0x400040 |
| 5 | AT_PHENT | 8 | S+112 | 4 | 9 | S+120 | 56 |
| 6 | AT_PHNUM | 10 | S+128 | 5 | 11 | S+136 | 3 |
| 7 | AT_BASE | 12 | S+144 | 7 | 13 | S+152 | 0 |
| 8 | AT_FLAGS | 14 | S+160 | 8 | 15 | S+168 | 1 |
| 9 | AT_ENTRY | 16 | S+176 | 9 | 17 | S+184 | 0x401020 |
| 10 | AT_SECURE | 18 | S+192 | 23 | 19 | S+200 | 1 |
| 11 | AT_EXECFN | 20 | S+208 | 31 | 21 | S+216 | U-10 |
| 12 | AT_PLATFORM | 22 | S+224 | 15 | 23 | S+232 | P |
| 13 | AT_EXECFD | 24 | S+240 | 2 | 25 | S+248 | 7 |
| 14 | AT_NULL | 26 | S+256 | 0 | 27 | S+264 | 0 |

### B3（A0）
前缀：[3,"U-93","U-84","U-69",0,"U-54","U-39","U-24",0]；Q=S+72，F=U-9（/boot/sh），argv0=U-93；复制区间[S+72,S+280) 208字节。NULL字地址S+264 / S+272，Q偏移192 / 200。末地址不是sp更新。

| pair | 名称 | tag字序 | tag地址 | tag值 | value字序 | value地址 | value |
|---|---|---|---|---|---|---|---|
| 1 | AT_HWCAP | 0 | S+72 | 16 | 1 | S+80 | 0x1234 |
| 2 | AT_PAGESZ | 2 | S+88 | 6 | 3 | S+96 | 4096 |
| 3 | AT_CLKTCK | 4 | S+104 | 17 | 5 | S+112 | 250 |
| 4 | AT_PHDR | 6 | S+120 | 3 | 7 | S+128 | 0x400040 |
| 5 | AT_PHENT | 8 | S+136 | 4 | 9 | S+144 | 56 |
| 6 | AT_PHNUM | 10 | S+152 | 5 | 11 | S+160 | 3 |
| 7 | AT_BASE | 12 | S+168 | 7 | 13 | S+176 | 0 |
| 8 | AT_FLAGS | 14 | S+184 | 8 | 15 | S+192 | 0 |
| 9 | AT_ENTRY | 16 | S+200 | 9 | 17 | S+208 | 0x401020 |
| 10 | AT_SECURE | 18 | S+216 | 23 | 19 | S+224 | 0 |
| 11 | AT_EXECFN | 20 | S+232 | 31 | 21 | S+240 | U-9 |
| 12 | AT_PLATFORM | 22 | S+248 | 15 | 23 | S+256 | P |
| 13 | AT_NULL | 24 | S+264 | 0 | 25 | S+272 | 0 |

### 内核缓冲范围（全部半开）
AT_VECTOR_SIZE=2*(0+22+1)=46，ulong[46]为368字节。memset不移动elf_info；随后加2才纳入终止对。E141-126,E141-122,E141-121,E141-120,E141-017,E141-005,E141-039,E141-040,E141-041,E141-050。
| 模型 | 宏写word | 清零word | 清零byte | 清零数 | ei_index | 复制byte | 复制数 | 清零未复制byte | 未复制数 |
|---|---|---|---|---|---|---|---|---|---|
| A0 | 0,24 | 24,46 | 192,368 | 176 | 26 | 0,208 | 208 | 208,368 | 160 |
| Afd | 0,26 | 26,46 | 208,368 | 160 | 28 | 0,224 | 224 | 224,368 | 144 |

源端清零不等于复制整个368字节；copy_to_user长度ei_index*8。返回非零时-EFAULT；返回0且先前条件成立才局部返回0。不推出实际映射/可写/部分失败字节。B共80字。

## C PHDR与入口
phdr_addr初始0；PT_LOAD条件p_offset<=e_phoff< p_offset+p_filesz，仅测起点。命中赋e_phoff-p_offset+p_vaddr，多次命中可覆盖；最终无条件加load_bias L。主e_entry=主头e_entry+L；有解释器时，非错误R保存为interp_load_addr，再加解释器头e_entry得elf_entry，BAD_ADDR检查最终elf_entry；无解释器时BASE=0，elf_entry=e_entry。create_elf_tables(bprm,elf_ex,interp_load_addr,e_entry,phdr_addr)，START_THREAD(elf_ex,regs,elf_entry,bprm->p)。均有前置门，实际寄存器/CRT未证。C与A/B相互独立。E141-052,E141-055,E141-057,E141-058,E141-059,E141-060,E141-061,E141-062,E141-063,E141-065,E141-075。
| 例 | off,size,vaddr,phoff,L | 命中 | 加L前PHDR | PHDR | 表文件区间 | 段文件区间 | 整表包含 | AT_ENTRY | AT_BASE | elf_entry |
|---|---|---|---|---|---|---|---|---|---|---|
| C1 | 0x0,0x2000,0x400000,0x40,0x0 | true | 0x400040 | 0x400040 | 0x40,0xe8 | 0x0,0x2000 | true | 0x401020 | 0 | 0x401020 |
| C2 | 0x0,0x2000,0x0,0x40,0x600000 | true | 0x40 | 0x600040 | 0x40,0xe8 | 0x0,0x2000 | true | 0x600120 | 0x70000000 | 0x70002340 |
| C3a | 0x1000,0x1000,0x400000,0x2000,0x200000 | false | 0x0 | 0x200000 | 0x2000,0x20a8 | 0x1000,0x2000 | false | 未推 | 未推 | 未推 |
| C3b | 0x1000,0x1000,0x400000,0x1ff8,0x200000 | true | 0x400ff8 | 0x600ff8 | 0x1ff8,0x20a0 | 0x1000,0x2000 | false | 未推 | 未推 | 未推 |

C1: 0x40-0+0x400000+0=0x400040；主入口0x401020，BASE=0，最终0x401020。C2: 0x40+0x600000=0x600040；主0x120+0x600000=0x600120；R=0x70000000；解释器头0x2340，最终R+0x2340=0x70002340。C3a: 0x2000为排他的段末，未命中；0+0x200000=0x200000，不保证指向PHDR。C3b: 0x1ff8起点命中，0x1ff8-0x1000+0x400000+0x200000=0x600ff8；3*56=168=0xa8，整表[0x1ff8,0x20a0)不全在[0x1000,0x2000)。C3只算PHDR，不推入口，不宣称真实非法映射、漏洞或崩溃。

## 边界
历史25/26/28/66/69限制不变。完整本地P9、预处理、编译链接、程序/脚本、Graphviz、QEMU、实机、真实操作NOT_RUN。图和机器数据同源；图104节点/103边，80字；源码正式窄窗129。主线独审待定。
