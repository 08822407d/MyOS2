---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-BRK-ALLOCATOR-144"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-22"
conversation_display_name: "MYOS2-A-C10a 代码分析执行"
produced_by: "GPT-6.2（系统模型族；界面具体型号及实际后台未独立核实）"
model_per_owner: "次一档GPT-6.2 Sol及高推理强度"
actual_effort: "UNATTESTED"
date: "2026-11-24"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
read_channel: "connector"
status: "AUTHOR_CONTENT_PREPARED_PENDING_PUBLICATION_AND_G04"
mainline_acceptance: "PENDING"
full_local_P9: "NOT_RUN"
inputs_read: [{"path":"mykernel/scripts/options_flags.cmake","ref":"a039d9803ade","blob":"6019436417ad","LF":84,"CRLF":0,"physical_lines":85},{"path":"mykernel/arch/x86_64/myos/syscall.c","ref":"a039d9803ade","blob":"8ed3c9a1e018","LF":223,"CRLF":0,"physical_lines":223},{"path":"mykernel/mm/vm_map/mmap.c","ref":"a039d9803ade","blob":"69000768d94b","LF":1356,"CRLF":0,"physical_lines":1357},{"path":"mykernel/mm/misc/mm_misc_types.h","ref":"a039d9803ade","blob":"b9d93f504756","LF":188,"CRLF":0,"physical_lines":188},{"path":"mykernel/mm/page_alloc/buddy_const.h","ref":"a039d9803ade","blob":"536bc906fc5e","LF":410,"CRLF":0,"physical_lines":411},{"path":"mykernel/include/linux/kernel/align.h","ref":"a039d9803ade","blob":"ddd63ec4b93f","LF":14,"CRLF":0,"physical_lines":15},{"path":"mykernel/include/uapi/linux/const.h","ref":"a039d9803ade","blob":"d8bcaf7e3151","LF":38,"CRLF":0,"physical_lines":39},{"path":"mykernel/arch/x86_64/mm/page/page_const_arch.h","ref":"a039d9803ade","blob":"b10e280f26f0","LF":104,"CRLF":0,"physical_lines":105},{"path":"mykernel/include/linux/kernel/types.h","ref":"a039d9803ade","blob":"ef531baa8317","LF":240,"CRLF":0,"physical_lines":241},{"path":"mykernel/arch/x86_64/include/asm/unistd_64.h","ref":"a039d9803ade","blob":"3b54c09e5d3a","LF":447,"CRLF":0,"physical_lines":447},{"path":"mykernel/arch/x86_64/include/asm/syscalls_64.h","ref":"a039d9803ade","blob":"544cfdb93d7e","LF":79,"CRLF":0,"physical_lines":80},{"path":"mykernel/arch/x86_64/entry/syscall_64.c","ref":"a039d9803ade","blob":"a21461322a9f","LF":32,"CRLF":0,"physical_lines":33},{"path":"mykernel/include/linux/kernel/syscalls.h","ref":"a039d9803ade","blob":"6cc2f85fec90","LF":1399,"CRLF":0,"physical_lines":1399},{"path":"mykernel/arch/x86_64/include/asm/syscall_wrapper.h","ref":"a039d9803ade","blob":"4224440c2c48","LF":254,"CRLF":0,"physical_lines":254},{"path":"myinitramfs/CMakeLists.txt","ref":"a039d9803ade","blob":"6757d21bc64c","LF":50,"CRLF":0,"physical_lines":51},{"path":"myinitramfs/mylib/CMakeLists.txt","ref":"a039d9803ade","blob":"8f6abc24095a","LF":2,"CRLF":0,"physical_lines":3},{"path":"myinitramfs/mylib/myglibc/CMakeLists.txt","ref":"a039d9803ade","blob":"b956957a602a","LF":29,"CRLF":0,"physical_lines":29},{"path":"myinitramfs/mylib/myglibc/malloc/CMakeLists.txt","ref":"a039d9803ade","blob":"3b9f22dd9ab9","LF":2,"CRLF":0,"physical_lines":3},{"path":"myinitramfs/mylib/myglibc/syscall/CMakeLists.txt","ref":"a039d9803ade","blob":"22503b9bff44","LF":6,"CRLF":0,"physical_lines":7},{"path":"myinitramfs/mylib/util/CMakeLists.txt","ref":"a039d9803ade","blob":"5fd782a35629","LF":2,"CRLF":0,"physical_lines":3},{"path":"myinitramfs/mylib/myglibc/include/unistd.h","ref":"a039d9803ade","blob":"bdb6ffa91612","LF":177,"CRLF":0,"physical_lines":177},{"path":"myinitramfs/mylib/myglibc/syscall/syscall.c","ref":"a039d9803ade","blob":"847cd015da2b","LF":89,"CRLF":0,"physical_lines":89},{"path":"myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/brk.c","ref":"a039d9803ade","blob":"6bd7a1079152","LF":48,"CRLF":0,"physical_lines":48},{"path":"myinitramfs/mylib/myglibc/malloc/malloc.c","ref":"a039d9803ade","blob":"44db9ea9d7f7","LF":390,"CRLF":390,"physical_lines":391},{"path":"myinitramfs/mylib/myglibc/malloc/_malloc.h","ref":"a039d9803ade","blob":"2dea1f8fc05c","LF":56,"CRLF":0,"physical_lines":57},{"path":"myinitramfs/mylib/util/list.c","ref":"a039d9803ade","blob":"24f7a214a288","LF":127,"CRLF":0,"physical_lines":127},{"path":"myinitramfs/mylib/include/utils.h","ref":"a039d9803ade","blob":"4e3e8910ac00","LF":62,"CRLF":0,"physical_lines":63}]
open_questions: ["实际sh链接、实际预处理/编译、C指针与对象、真实VMA/页和堆操作未证"]
record_type: "phase2_brk_allocator_analysis"
---

# 144 · brk返回值与仓内用户分配器消费

## 共同范围与证据

200合同11组 A6/B2/C3，D00空。143限定通过，R143-N01主线已补导航；141/142/140及历史判闭不重开。14 R14-S01保留字段恢复而非VMA事务恢复，26 R26-S02限制simple_vma_link正常只返回0，75为内核分配器；121 A有效对象/构建边界及122对121判闭保持。所有模型为[INFERRED]整数投影或给定条件片段，不是实际运行。

内核K=4096来自PAGE_SHIFT=12、PAGE_SIZE=_AC(1,UL)<<PAGE_SHIFT；PAGE_ALIGN→ALIGN→__ALIGN_KERNEL→__ALIGN_KERNEL_MASK，掩码为K−1，加掩码后清低位，题设无回绕。用户malloc.c本地H=0x200000=2MiB，CONST_2M，不能混为内核页。ulong别名unsigned long，mm.start_brk/brk为ulong；int/long/size_t/指针宽度按题设，不是编译测量。brk_end为void*，+H只作给定字节地址投影，不认证GNU扩展或C对象范围。依据E144-029,E144-030,E144-028,E144-024,E144-025,E144-026,E144-027,E144-031,E144-023,E144-075,E144-076,E144-077。

### 构建及候选返回绑定

先读内核options_flags及用户六份构建文件。顶层myinitramfs CMake设仓外musl路径与工具、仅进入myinit/myshell并安装init/sh；独立mylib下myglibc/csu/util，myglibc定义mylibc STATIC，malloc/syscall/sysdeps等子目录及glob/target_sources是候选构建文本。actual_linked_to_sh=UNKNOWN，actual_compilation/actual_preprocessing=NOT_RUN。不访问仓外musl，不以其行为补全本仓实现。依据E144-001,E144-002,E144-003,E144-004,E144-051,E144-052,E144-053,E144-054,E144-055,E144-056,E144-057,E144-058,E144-059,E144-060。

内核__NR_brk=12，syscalls_64的__SYSCALL(__NR_brk,sys_brk)是生成项，sys_call_table为数组映射，活动x64_sys_call经switch宏生成case 12返回__x64_sys_brk(regs)。MYOS_SYSCALL_DEFINE1→__MYOS_SYSCALL_DEFINEx→__SYS_STUBx生成__x64_sys_brk→__se_sys_brk→__do_sys_brk；__MAP/DECL给unsigned long brk签名，__ARCH_MAP/SC_CAST取源级regs->di并转目标类型，返回long。上述是C绑定文本，不证真实寄存器/CPU路径。

候选用户声明extern void *brk(void *__addr)，int版本仅注释。SYSFUNC_DEF(brk)→_SYSFUNC_DEF_→__SYSFUNC_DEF__，生成标签/设置编号/跳LABEL_SYSCALL；共用汇编文本含syscall和sysexit_ret_addr，cmp/jb至LABEL_SYSCALL_RET，errno/-1转换两行注释，非跳转亦落到同一返回标签，leave/ret没有活动结果改写。sysdeps的__brk整个转换主体及weak_alias均注释。不把宏实例当整个函数定义，不认证实际寄存器状态。给定正常返回传出的原始数值在所选包装中保持，不变成libc 0/-1；非NULL旧break不是分配成功。证据E144-032,E144-033,E144-034,E144-035,E144-036,E144-037,E144-038,E144-039,E144-040,E144-041,E144-042,E144-043,E144-044,E144-045,E144-046,E144-047,E144-048,E144-049,E144-050,E144-005,E144-062,E144-061,E144-063,E144-064,E144-065,E144-066,E144-067,E144-068,E144-069,E144-070,E144-071,E144-072,E144-073,E144-074。

## A · 内核局部返回及候选消费（6组）

每组重置mm.start_brk=0x400000，mm.brk=origbrk=0x401234。先比较min，未到对齐不虚构old/new；同页直接赋请求。缩小时先改mm.brk再调用munmap，非零out恢复；增大先调用do_brk_flags，负值out恢复，正常0后才改字段。rlimit检查为注释。munmap第三参为排他end，do_brk_flags第二参是len（源码vm_end=addr+len）。指定helper正常返回为输入，不推内部副作用为零或把失败归给simple_vma_link。依据E144-007,E144-008,E144-009,E144-010,E144-011,E144-012,E144-013,E144-014,E144-015,E144-016,E144-017,E144-018,E144-019,E144-020,E144-021,E144-022。

| ID | request | min | align | oldbrk | newbrk | branch | helper(args) | given ret | mm.brk before helper | after helper before caller update | final mm.brk | local return | out restore | candidate raw value |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| A_QUERY | 0x0 | REJECT_TO_OUT | false | NOT_REACHED | NOT_REACHED | OUT_BELOW_MIN | SKIPPED() | NOT_REACHED | NOT_REACHED | NOT_REACHED | 0x401234 | 0x401234 | true | 0x401234 |
| A_SAME | 0x401ff0 | PASS | true | 0x402000 | 0x402000 | SAME_ALIGNED_PAGE | SKIPPED() | NOT_REACHED | NOT_REACHED | NOT_REACHED | 0x401ff0 | 0x401ff0 | false | 0x401ff0 |
| A_SHRINK_OK | 0x400ff0 | PASS | true | 0x402000 | 0x401000 | SHRINK | simple_do_vma_munmap(mm,0x401000,0x402000) | 0 | 0x400ff0 | 0x400ff0 | 0x400ff0 | 0x400ff0 | false | 0x400ff0 |
| A_SHRINK_FAIL | 0x400ff0 | PASS | true | 0x402000 | 0x401000 | SHRINK | simple_do_vma_munmap(mm,0x401000,0x402000) | -ENOMEM | 0x400ff0 | 0x400ff0 | 0x401234 | 0x401234 | true | 0x401234 |
| A_GROW_OK | 0x403001 | PASS | true | 0x402000 | 0x404000 | GROW | do_brk_flags(0x402000,0x2000,0) | 0 | 0x401234 | 0x401234 | 0x403001 | 0x403001 | false | 0x403001 |
| A_GROW_FAIL | 0x403001 | PASS | true | 0x402000 | 0x404000 | GROW | do_brk_flags(0x402000,0x2000,0) | -ENOMEM | 0x401234 | 0x401234 | 0x401234 | 0x401234 | true | 0x401234 |

A_QUERY在min门走out；两个映射helper均未调用。A_SAME亦均未调用但赋请求。A_SHRINK_FAIL helper返回负值不外传该errno，恢复origbrk；A_GROW_FAIL原字段未改增长，out仍写回origbrk。所有六例给定局部正常返回的数值均非NULL；映射状态和真实用户返回仍未证。

## B · 完整16轮init_uslab（2组）

共同B0=0x40000000，mm.start_brk=mm.brk=B0，initiated=false；查询brk(0)条件返回B0，brk_end=B0，随后三个列表初始化count=0。USLAB_LEVEL=16，USLAB_SIZE_BASE=32，obj_size=32<<i，obj_nr=H/obj_size。每轮cache加入尾，normal三个头初始化；base slab入normal_free，normal_slab_total=1，free/total、nsobj_free_count=obj_nr，nsobj_used_count=0。base_upage先init节点，链式赋值upgp->vaddr=brk_end=brk(brk_end+H)，随后加free尾。free原为0，现1，upage_alloc非空分支移除唯一头；count--，container_of给定正常转换得到同一upgp，加入used头count++，返回同一节点。此轮不会进入malloc/brk动态分支；slab.virt_addr取返回节点vaddr并设置双向关联。所需有效对象/稳定列表及给定操作正常返回是条件。E144-077,E144-078,E144-079,E144-096,E144-097,E144-075,E144-076,E144-086,E144-087,E144-088,E144-089,E144-090,E144-091,E144-092,E144-093,E144-094,E144-095,E144-080,E144-081,E144-085,E144-110,E144-114,E144-112,E144-113,E144-111,E144-104,E144-109,E144-105,E144-106,E144-107,E144-108,E144-115,E144-116,E144-098,E144-100,E144-099,E144-101,E144-102,E144-103,E144-118,E144-117。不展开bitmap。

### B_OK

| i | obj_size | obj_nr | before kernel/user | request | given helper ret | raw return | after kernel/user | upage identity:vaddr | slab identity:virt_addr | cache count | free sequence | used count | normal free/used/full/total | selected branch |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 0 | 32 | 65536 | 0x40000000/0x40000000 | 0x40200000 | 0 | 0x40200000 | 0x40200000/0x40200000 | base_upages[0]:0x40200000 | base_uslabs[0]:0x40200000 | 0→1 | 0→1→0 | 0→1 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 1 | 64 | 32768 | 0x40200000/0x40200000 | 0x40400000 | 0 | 0x40400000 | 0x40400000/0x40400000 | base_upages[1]:0x40400000 | base_uslabs[1]:0x40400000 | 1→2 | 0→1→0 | 1→2 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 2 | 128 | 16384 | 0x40400000/0x40400000 | 0x40600000 | 0 | 0x40600000 | 0x40600000/0x40600000 | base_upages[2]:0x40600000 | base_uslabs[2]:0x40600000 | 2→3 | 0→1→0 | 2→3 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 3 | 256 | 8192 | 0x40600000/0x40600000 | 0x40800000 | 0 | 0x40800000 | 0x40800000/0x40800000 | base_upages[3]:0x40800000 | base_uslabs[3]:0x40800000 | 3→4 | 0→1→0 | 3→4 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 4 | 512 | 4096 | 0x40800000/0x40800000 | 0x40a00000 | 0 | 0x40a00000 | 0x40a00000/0x40a00000 | base_upages[4]:0x40a00000 | base_uslabs[4]:0x40a00000 | 4→5 | 0→1→0 | 4→5 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 5 | 1024 | 2048 | 0x40a00000/0x40a00000 | 0x40c00000 | 0 | 0x40c00000 | 0x40c00000/0x40c00000 | base_upages[5]:0x40c00000 | base_uslabs[5]:0x40c00000 | 5→6 | 0→1→0 | 5→6 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 6 | 2048 | 1024 | 0x40c00000/0x40c00000 | 0x40e00000 | 0 | 0x40e00000 | 0x40e00000/0x40e00000 | base_upages[6]:0x40e00000 | base_uslabs[6]:0x40e00000 | 6→7 | 0→1→0 | 6→7 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 7 | 4096 | 512 | 0x40e00000/0x40e00000 | 0x41000000 | 0 | 0x41000000 | 0x41000000/0x41000000 | base_upages[7]:0x41000000 | base_uslabs[7]:0x41000000 | 7→8 | 0→1→0 | 7→8 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 8 | 8192 | 256 | 0x41000000/0x41000000 | 0x41200000 | 0 | 0x41200000 | 0x41200000/0x41200000 | base_upages[8]:0x41200000 | base_uslabs[8]:0x41200000 | 8→9 | 0→1→0 | 8→9 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 9 | 16384 | 128 | 0x41200000/0x41200000 | 0x41400000 | 0 | 0x41400000 | 0x41400000/0x41400000 | base_upages[9]:0x41400000 | base_uslabs[9]:0x41400000 | 9→10 | 0→1→0 | 9→10 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 10 | 32768 | 64 | 0x41400000/0x41400000 | 0x41600000 | 0 | 0x41600000 | 0x41600000/0x41600000 | base_upages[10]:0x41600000 | base_uslabs[10]:0x41600000 | 10→11 | 0→1→0 | 10→11 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 11 | 65536 | 32 | 0x41600000/0x41600000 | 0x41800000 | 0 | 0x41800000 | 0x41800000/0x41800000 | base_upages[11]:0x41800000 | base_uslabs[11]:0x41800000 | 11→12 | 0→1→0 | 11→12 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 12 | 131072 | 16 | 0x41800000/0x41800000 | 0x41a00000 | 0 | 0x41a00000 | 0x41a00000/0x41a00000 | base_upages[12]:0x41a00000 | base_uslabs[12]:0x41a00000 | 12→13 | 0→1→0 | 12→13 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 13 | 262144 | 8 | 0x41a00000/0x41a00000 | 0x41c00000 | 0 | 0x41c00000 | 0x41c00000/0x41c00000 | base_upages[13]:0x41c00000 | base_uslabs[13]:0x41c00000 | 13→14 | 0→1→0 | 13→14 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 14 | 524288 | 4 | 0x41c00000/0x41c00000 | 0x41e00000 | 0 | 0x41e00000 | 0x41e00000/0x41e00000 | base_upages[14]:0x41e00000 | base_uslabs[14]:0x41e00000 | 14→15 | 0→1→0 | 14→15 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 15 | 1048576 | 2 | 0x41e00000/0x41e00000 | 0x42000000 | 0 | 0x42000000 | 0x42000000/0x42000000 | base_upages[15]:0x42000000 | base_uslabs[15]:0x42000000 | 15→16 | 0→1→0 | 15→16 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |

查询1＋扩展16=17个给定brk调用；upage_alloc 16次均非空，额外malloc/brk均0。成功请求16、拒绝0，元数据16个、不同保存地址16种。尝试长度和33554432字节，去重请求区间并集[0x40000000,0x42000000)，跨度33554432字节，字段增长33554432字节；实际映射字节未证。最终kernel=0x42000000，user=0x42000000，cache16/free0/used16，used头→尾15…0，每档normal_free=1、normal_used/full=0、normal_slab_total=1；全部给定调用及元数据操作完成才initiated=true。

每轮保存值均等于当次请求区间的排他上界，末值为总请求跨度排他上界；不推实际越界写或地址未映射。

### B_REFUSE_I1

| i | obj_size | obj_nr | before kernel/user | request | given helper ret | raw return | after kernel/user | upage identity:vaddr | slab identity:virt_addr | cache count | free sequence | used count | normal free/used/full/total | selected branch |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 0 | 32 | 65536 | 0x40000000/0x40000000 | 0x40200000 | 0 | 0x40200000 | 0x40200000/0x40200000 | base_upages[0]:0x40200000 | base_uslabs[0]:0x40200000 | 0→1 | 0→1→0 | 0→1 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 1 | 64 | 32768 | 0x40200000/0x40200000 | 0x40400000 | -ENOMEM | 0x40200000 | 0x40200000/0x40200000 | base_upages[1]:0x40200000 | base_uslabs[1]:0x40200000 | 1→2 | 0→1→0 | 1→2 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 2 | 128 | 16384 | 0x40200000/0x40200000 | 0x40400000 | 0 | 0x40400000 | 0x40400000/0x40400000 | base_upages[2]:0x40400000 | base_uslabs[2]:0x40400000 | 2→3 | 0→1→0 | 2→3 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 3 | 256 | 8192 | 0x40400000/0x40400000 | 0x40600000 | 0 | 0x40600000 | 0x40600000/0x40600000 | base_upages[3]:0x40600000 | base_uslabs[3]:0x40600000 | 3→4 | 0→1→0 | 3→4 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 4 | 512 | 4096 | 0x40600000/0x40600000 | 0x40800000 | 0 | 0x40800000 | 0x40800000/0x40800000 | base_upages[4]:0x40800000 | base_uslabs[4]:0x40800000 | 4→5 | 0→1→0 | 4→5 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 5 | 1024 | 2048 | 0x40800000/0x40800000 | 0x40a00000 | 0 | 0x40a00000 | 0x40a00000/0x40a00000 | base_upages[5]:0x40a00000 | base_uslabs[5]:0x40a00000 | 5→6 | 0→1→0 | 5→6 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 6 | 2048 | 1024 | 0x40a00000/0x40a00000 | 0x40c00000 | 0 | 0x40c00000 | 0x40c00000/0x40c00000 | base_upages[6]:0x40c00000 | base_uslabs[6]:0x40c00000 | 6→7 | 0→1→0 | 6→7 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 7 | 4096 | 512 | 0x40c00000/0x40c00000 | 0x40e00000 | 0 | 0x40e00000 | 0x40e00000/0x40e00000 | base_upages[7]:0x40e00000 | base_uslabs[7]:0x40e00000 | 7→8 | 0→1→0 | 7→8 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 8 | 8192 | 256 | 0x40e00000/0x40e00000 | 0x41000000 | 0 | 0x41000000 | 0x41000000/0x41000000 | base_upages[8]:0x41000000 | base_uslabs[8]:0x41000000 | 8→9 | 0→1→0 | 8→9 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 9 | 16384 | 128 | 0x41000000/0x41000000 | 0x41200000 | 0 | 0x41200000 | 0x41200000/0x41200000 | base_upages[9]:0x41200000 | base_uslabs[9]:0x41200000 | 9→10 | 0→1→0 | 9→10 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 10 | 32768 | 64 | 0x41200000/0x41200000 | 0x41400000 | 0 | 0x41400000 | 0x41400000/0x41400000 | base_upages[10]:0x41400000 | base_uslabs[10]:0x41400000 | 10→11 | 0→1→0 | 10→11 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 11 | 65536 | 32 | 0x41400000/0x41400000 | 0x41600000 | 0 | 0x41600000 | 0x41600000/0x41600000 | base_upages[11]:0x41600000 | base_uslabs[11]:0x41600000 | 11→12 | 0→1→0 | 11→12 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 12 | 131072 | 16 | 0x41600000/0x41600000 | 0x41800000 | 0 | 0x41800000 | 0x41800000/0x41800000 | base_upages[12]:0x41800000 | base_uslabs[12]:0x41800000 | 12→13 | 0→1→0 | 12→13 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 13 | 262144 | 8 | 0x41800000/0x41800000 | 0x41a00000 | 0 | 0x41a00000 | 0x41a00000/0x41a00000 | base_upages[13]:0x41a00000 | base_uslabs[13]:0x41a00000 | 13→14 | 0→1→0 | 13→14 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 14 | 524288 | 4 | 0x41a00000/0x41a00000 | 0x41c00000 | 0 | 0x41c00000 | 0x41c00000/0x41c00000 | base_upages[14]:0x41c00000 | base_uslabs[14]:0x41c00000 | 14→15 | 0→1→0 | 14→15 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |
| 15 | 1048576 | 2 | 0x41c00000/0x41c00000 | 0x41e00000 | 0 | 0x41e00000 | 0x41e00000/0x41e00000 | base_upages[15]:0x41e00000 | base_uslabs[15]:0x41e00000 | 15→16 | 0→1→0 | 15→16 | 1/0/0/1 | NONEMPTY_REMOVE_HEAD_ADD_USED_HEAD |

查询1＋扩展16=17个给定brk调用；upage_alloc 16次均非空，额外malloc/brk均0。成功请求15、拒绝1，元数据16个、不同保存地址15种。尝试长度和33554432字节，去重请求区间并集[0x40000000,0x41e00000)，跨度31457280字节，字段增长31457280字节；实际映射字节未证。最终kernel=0x41e00000，user=0x41e00000，cache16/free0/used16，used头→尾15…0，每档normal_free=1、normal_used/full=0、normal_slab_total=1；全部给定调用及元数据操作完成才initiated=true。

i=1请求0x40400000，拒绝返回非NULL旧break 0x40200000，链赋两个break维持旧值；base_upages[0]和[1]不同节点保存同地址。i=2重试0x40400000。其余成功轮保存值为当次排他上界；失败轮保存旧值是请求区间下界，不是该次排他上界。重复地址数值不等于已发生页重复占用/真实对象别名或破坏；失败helper副作用未知。

## C · free复用及动态消费（3组）

C_REUSE给Q为唯一free节点，另两个used节点；Q.uslab_p未指定。非空分支不调用malloc/brk，不读写Q.vaddr或uslab_p。动态两组直接从给定malloc正常返回有效M后、brk调用前起算，不能从递归malloc推导该入口。M尚未入列表，vaddr/uslab_p未指定。U0=0x50000000，请求U0+H=0x50200000；mm.start_brk<=U0，给定扩展helper正常返回0或-ENOMEM。非NULL门后独立brk_end += H，M.vaddr=pg_vaddr，init链后加used尾。两个分支均返回同一M，free(M)未到，M.uslab_p无赋值。E144-080,E144-081,E144-082,E144-083,E144-084,E144-085,E144-112,E144-111,E144-114,E144-104,E144-098。

| ID | raw/pg_vaddr | nonnull | kernel final | user final | node | vaddr | uslab_p | free/used | return | malloc | brk | free(M) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| C_REUSE | NOT_REACHED | NOT_REACHED | 0x50000000 | 0x50000000 | Q | 0x4fe00000 | NOT_STIPULATED | 0/3 | Q | SKIPPED | SKIPPED | SKIPPED |
| C_GROW_OK | 0x50200000 | true | 0x50200000 | 0x50200000 | M | 0x50200000 | NOT_STIPULATED | 0/3 | M | GIVEN_NORMAL_RETURN_VALID_M_BEFORE_ENTRY | CONDITIONAL_REACHED | SKIPPED |
| C_GROW_REFUSED | 0x50000000 | true | 0x50000000 | 0x50200000 | M | 0x50000000 | NOT_STIPULATED | 0/3 | M | GIVEN_NORMAL_RETURN_VALID_M_BEFORE_ENTRY | CONDITIONAL_REACHED | SKIPPED |

### C_REUSE

| phase | action | free | used | kernel | user | node | vaddr | uslab_p | pg_vaddr | nonnull |
|---|---|---|---|---|---|---|---|---|---|---|
| C_REUSE_T01 | entry, free count>0 | 1 | 2 | 0x50000000 | 0x50000000 | Q | 0x4fe00000 | NOT_STIPULATED | NOT_APPLICABLE | NOT_APPLICABLE |
| C_REUSE_T02 | remove same free head; return member node, count-- | 0 | 2 | 0x50000000 | 0x50000000 | Q | 0x4fe00000 | NOT_STIPULATED | NOT_APPLICABLE | NOT_APPLICABLE |
| C_REUSE_T03 | container_of and add Q to used head | 0 | 3 | 0x50000000 | 0x50000000 | Q | 0x4fe00000 | NOT_STIPULATED | NOT_APPLICABLE | NOT_APPLICABLE |
| C_REUSE_T04 | return same Q | 0 | 3 | 0x50000000 | 0x50000000 | Q | 0x4fe00000 | NOT_STIPULATED | NOT_APPLICABLE | NOT_APPLICABLE |

### C_GROW_OK

| phase | action | free | used | kernel | user | node | vaddr | uslab_p | pg_vaddr | nonnull |
|---|---|---|---|---|---|---|---|---|---|---|
| C_GROW_OK_T01 | given normal malloc return M; observation begins before brk | 0 | 2 | 0x50000000 | 0x50000000 | M | NOT_STIPULATED | NOT_STIPULATED | NOT_REACHED | NOT_REACHED |
| C_GROW_OK_T02 | request U0+H; given kernel helper return then raw brk return | 0 | 2 | 0x50200000 | 0x50000000 | M | NOT_STIPULATED | NOT_STIPULATED | 0x50200000 | NOT_YET_TESTED |
| C_GROW_OK_T03 | pg_vaddr != NULL true | 0 | 2 | 0x50200000 | 0x50000000 | M | NOT_STIPULATED | NOT_STIPULATED | 0x50200000 | true |
| C_GROW_OK_T04 | independent brk_end += H then M.vaddr=pg_vaddr | 0 | 2 | 0x50200000 | 0x50200000 | M | 0x50200000 | NOT_STIPULATED | 0x50200000 | true |
| C_GROW_OK_T05 | initialize M list node and insert used tail | 0 | 3 | 0x50200000 | 0x50200000 | M | 0x50200000 | NOT_STIPULATED | 0x50200000 | true |
| C_GROW_OK_T06 | return same M; free(M) skipped | 0 | 3 | 0x50200000 | 0x50200000 | M | 0x50200000 | NOT_STIPULATED | 0x50200000 | true |

### C_GROW_REFUSED

| phase | action | free | used | kernel | user | node | vaddr | uslab_p | pg_vaddr | nonnull |
|---|---|---|---|---|---|---|---|---|---|---|
| C_GROW_REFUSED_T01 | given normal malloc return M; observation begins before brk | 0 | 2 | 0x50000000 | 0x50000000 | M | NOT_STIPULATED | NOT_STIPULATED | NOT_REACHED | NOT_REACHED |
| C_GROW_REFUSED_T02 | request U0+H; given kernel helper return then raw brk return | 0 | 2 | 0x50000000 | 0x50000000 | M | NOT_STIPULATED | NOT_STIPULATED | 0x50000000 | NOT_YET_TESTED |
| C_GROW_REFUSED_T03 | pg_vaddr != NULL true | 0 | 2 | 0x50000000 | 0x50000000 | M | NOT_STIPULATED | NOT_STIPULATED | 0x50000000 | true |
| C_GROW_REFUSED_T04 | independent brk_end += H then M.vaddr=pg_vaddr | 0 | 2 | 0x50000000 | 0x50200000 | M | 0x50000000 | NOT_STIPULATED | 0x50000000 | true |
| C_GROW_REFUSED_T05 | initialize M list node and insert used tail | 0 | 3 | 0x50000000 | 0x50200000 | M | 0x50000000 | NOT_STIPULATED | 0x50000000 | true |
| C_GROW_REFUSED_T06 | return same M; free(M) skipped | 0 | 3 | 0x50000000 | 0x50200000 | M | 0x50000000 | NOT_STIPULATED | 0x50000000 | true |

C_REUSE：free1→0，used2→3，Q加入used头；两break及Q.vaddr保持。C_GROW_OK：用户/内核均0x50200000，M.vaddr等于请求排他上界。C_GROW_REFUSED：旧break 0x50000000非NULL仍通过门，独立递增使user=0x50200000而kernel=0x50000000，相差H；M.vaddr=0x50000000，used仍增至3且M在尾。此仅为条件数值状态与B链赋的不同消费；不签实际页、堆对象、越界写、破坏、真实递归调用或VMA副作用。没有额外NULL拒绝场景。

## 消费边界

11组模型、32轮B表、C各4/6/6阶段。事实与图节点ID/关系区分来源、条件调用、数值模型及未证边界；不画成已运行全链。新正式窄窗118，malloc.c固定Blob=44db9ea9d7f7实际390 CRLF/390 LF，展示仅可逆转为LF，逐窗line_endings记录，发布前重构对源字节。完整本地P9、预处理、编译链接、MyOS2程序／脚本、Graphviz、QEMU、实机和真实操作NOT_RUN。主线独审待定，不启动145。
