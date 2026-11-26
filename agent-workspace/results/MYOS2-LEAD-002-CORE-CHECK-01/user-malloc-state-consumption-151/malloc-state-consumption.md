---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
batch_id: "CORE-USER-MALLOC-STATE-CONSUMPTION-151"
conversation_display_name: "MYOS2-A-C10c 代码分析执行"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-24"
produced_by: "GPT-6.2（系统标识）"
model_per_owner: "GPT-6.2 Sol＋Extra／高推理（Owner指定）"
actual_backend_model: "UNATTESTED"
actual_client_selection: "UNATTESTED"
actual_reasoning_effort: "UNATTESTED"
date: "2026-11-26"
date_timezone: "Asia/Shanghai"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_pin_short12: "974a51a9aa96"
execution_parent_short12: "cca59f60e032"
read_channel: "connector"
full_local_P9: "NOT_RUN"
status: "AUTHOR_PENDING_MAINLINE_REVIEW"
D00: []
record_type: "batch151_analysis"
---

# 第151批 · 分配与释放的跨调用状态、满位图后继及地址识别

**按209及211恢复既有151；D00=[]，7独立入口、10顶层调用。** A两有限序列5调用、B2/C3各独立一次。150 PASS_PENDING_LOCAL；149仅合读150 D00及C10独审§3消费，R149-01、D150-O04、R149-02有界闭合，旧RETURN、原件/评论/证据账保持。交C11 Pro独审，不重复补正。

## 1. 模型范围与比较谓词
H=0x200000来自malloc.c PAGE_SIZE→CONST_2M，不是内核页尺寸；GIVEN byte8位、int/unsigned32、pointer/size_t/unsigned long/unsigned long long/bitmap_t64位、BITMAP_UNITSIZE=64，自然8对齐。List_s两指针16，List_hdr_s anchor16+count8=24，uPage_s32，uslab_s64，uslab_cache_s128，三个用于container_of的链成员偏移0。成员具体偏移见facts.given_models。给定活、足够大、合法metadata/bitmap/header/list及转换，无未说明别名、回绕并发；initiated=true。地址、字节差及商余数均整数投影，不证明真实C指针运算、对象/生命周期、ABI/标准/DEBUG/编译器、链接、映射或真实运行，不访问payload。N1547独立外证UNKNOWN继承；assigned仅0/1/3低位。
V_BITMAP逐slab比较free与N减前N位popcount；V_CACHE逐cache比较CF/CU与Σfree/Σ(N−free)；V_LENGTH比较给定header.count和物理实长；V_CLASS比较free链free=N、used 0<free<N、full free=0。只观察命名时点，不是源码校验分支或每次单字段写后的不变量。其他未指定cache字段不补齐。证据 E151-001–032。

## 2. A · 跨调用状态
共同给合法16档升序32×2^i；size=0x4000小于上限，init门假。malloc从groups[0]开始，0…8九次size>obj_size为真并next/container_of推进，9处比较假，O16384、N128。C两slab S/R、total2/base S；S.V=0x60000000、R.V=0x60400000；合法P_S/P_R对应page，used[P_S,P_R]/2、free页[]/0；R始终free128两word0，非选取对象。入口均used[S]/1、free[R]/1、full[]/0，四谓词真。

A_LAST_SLOT_CYCLE：入口S.free1、word0=0xfffffffffffffff7、word1全1、CF/CU129/127。首次malloc used优先；摘首S，邻居摘链+self-loop，used.count1→0；275按旧free1判真，276插full（helper先对self-loop再次4次写，再插4指针、full.count0→1）。scan前S.free仍1，V_CLASS=false，其他真。295扫描0、1、2、3（word0共4次源码unit求值），第3空而break，返回idx3，ret_val仍NULL。296真，298/299 offset0xc000、p1=0x6000c000，301–304 free0、CF/CU128/128，再set 1ULL<<3，word0全1、word1全1保持。308 free.count1<1假；327返回p1，四谓词真。
直接free(p1)，DEBUG若含等待则条件假，非NULL。search按H向下取整V，首P_S匹配break，随后S/C两个owner读取。345 free0!=128；351差49152/16384=3余0；assigned低位mask8、unsigned返回8而非布尔1，352拒绝假。358 clear word0全1→0xfffffffffffffff7，再359–361 free1、CF/CU129/127；此刻仍full故V_CLASS暂假。364按更新后free==1真，366摘S，367 full.count1→0，368 used头插/used.count0→1；370未达。381 free.count1>2假，391正常到尾，四谓词真。第二次malloc直接消费该后态，重复旧free1预迁full、扫描idx3、扣计数/置位、308假、327返回p2=0x6000c000；与第一次终态相同。只证明数值相等，非真实对象同一/寿命。

A_LAST_OCCUPIED_REUSE：入口S.free127、map(1,0)、CF/CU255/1。free(V)首P_S，345假，idx0余0、assigned1；清bit0后map(0,0)，free128、CF/CU256/0。364更新后free==1假，370 free==0假，S仍used，381假、391正常到尾；V_BITMAP/CACHE/LENGTH真、V_CLASS假。随后malloc优先used仍取S、非R，旧free128==1假，摘首再头插used，count1→0→1，scan前V_CLASS仍假。idx0首空、word0一次求值，296真，offset0、ret=V、free127/CF255/CU1、set word0=1，308假、327返回V，四谓词恢复真。增量仅这一有限后继消费；147既有单次分类差异不重复登记新缺陷，不推所有差异无害。
A五次调用均不写R节点prev/next、R载荷或bitmap，仅判free header count；实际有序邻居/节点/header写见facts，页链保持。无uslab_alloc/uslab_free/upage_alloc/upage_free或递归。

## 3. B · 满bitmap且正free字段
两独立入口S两个word全1，R仍free128/空图。B_USED_ONE_FULLMAP入口used[S]/1、free[R]/1、full[]/0，S.free1、CF/CU129/127；B_FREE_FULLMAP入口used[]/0、free[S,R]/2、full[]/0，S.free128、CF/CU256/0。初态V_BITMAP=false、其他三项真；不一致显式GIVEN，来源未给。
B_USED：used摘S，旧free1迁full，未扣free故V_CLASS=false。B_FREE：used空、free.count2>0，282摘S时free.anchor.next=R、R.prev从S→free.anchor、S自环，free.count2→1；286头插used，free仍128，V_CLASS=false。R.prev实际写，R.next及其载荷/bitmap保持。
两例295循环idx0…127均占用，bm_unit源码求值128次，只涉及word0/1，非128次实测访存。i=128退出helper，返回limit128；ret_val仍NULL；296假，298–304不执行offset、地址赋值、三计数和set。已有迁链不回滚；308 free.count=1门假，不重选R，291/316显式循环不进入，最后327才正常返回NULL。B_USED终态used空/full[S]，B_FREE终态used[S]/full空，free均[R]/1；free字段、CF/CU、bitmap保持，V_BITMAP/V_CLASS假，V_CACHE/V_LENGTH真。未扩展第二次malloc/恢复实现。

## 4. C · 首匹配与槽内偏移
V=0x62000000，两合法互异metadata/bitmap/header，同V仅地址字段关系。Q32→S32→C32，O32/N65536、free65534/CF65534/CU2、word0=0x88余1023word0；Q64→T64→C64，O64/N32768、free32766/CF32766/CU2、word0=0x82余511word0。每套slab_total1、base自身、used唯一slab/count1，free/full空；各入口独立，单键只需32套，双键两套。四谓词初始真。
 
| ID | used_upage/count | obj_p | 首匹配 | 差/商/余 | assigned |
|---|---|---|---|---|---|
| C_INTERIOR_SINGLE | [Q32]/1 | 0x62000061 | Q32 | 97/3/1，÷32 | 8 |
| C_DUP32_FIRST | [Q32,Q64]/2 | 0x62000060 | Q32 | 96/3/0，÷32 | 8 |
| C_DUP64_FIRST | [Q64,Q32]/2 | 0x62000060 | Q64 | 96/1/32，÷64 | 2 |

154 PAGE_ROUND_DOWN→round_down→__round_mask，H−1=0x1fffff，三投影均对齐V。156起首used.anchor.next、160 container_of、161相等、163–164赋返回并break、168返回描述符；不检查第二候选（无第二vaddr/next求值），不主动交换链。341–343随后page→slab→cache，不以地址直接定cache。345非空；351整数差/商，余数为观察量，所读路径无另验余数为0的门。352 assigned源int字面1低位1/3，mask2/8、unsigned32返回2/8，拒绝条件假。358 clear用1ULL补码；前两0x88→0x80，第三0x82→0x80，余word不读写；359–361前两S32/CF65535、CU1，第三T64/CF32767、CU1。364/370假，381 free.count0>2假，391正常到尾；不迁链、不递归、未选cache及slab/bitmap不由源路径改写，页链保持。四谓词全真。非零余数不证真实非法free/UB；重复键不证真实双映射、双重分配或生产可达。

## 5. 每次调用终点及命名时点
V按BITMAP/CACHE/LENGTH/CLASS，T/F。U/F/L为used/free/full；C双键表只列选中cache，未选套完整在facts。free正常尾部为void，不与malloc NULL混同。
 
| 入口/调用 | idx | 返回 | S/T.free | CF/CU | bitmap前缀 | U/F/L成员/count | V |
|---|---:|---|---:|---|---|---|---|
| A_LAST_SLOT_CYCLE 1 malloc | 3 | 0x6000c000 | 0 | 128/128 | 0xffffffffffffffff/0xffffffffffffffff | []/0;[R]/1;[S]/1 | T/T/T/T |
| A_LAST_SLOT_CYCLE 2 free | 3 | VOID_FALLTHROUGH | 1 | 129/127 | 0xfffffffffffffff7/0xffffffffffffffff | [S]/1;[R]/1;[]/0 | T/T/T/T |
| A_LAST_SLOT_CYCLE 3 malloc | 3 | 0x6000c000 | 0 | 128/128 | 0xffffffffffffffff/0xffffffffffffffff | []/0;[R]/1;[S]/1 | T/T/T/T |
| A_LAST_OCCUPIED_REUSE 1 free | 0 | VOID_FALLTHROUGH | 128 | 256/0 | 0x0000000000000000/0x0000000000000000 | [S]/1;[R]/1;[]/0 | T/T/T/F |
| A_LAST_OCCUPIED_REUSE 2 malloc | 0 | 0x60000000 | 127 | 255/1 | 0x0000000000000001/0x0000000000000000 | [S]/1;[R]/1;[]/0 | T/T/T/T |
| B_USED_ONE_FULLMAP 1 malloc | 128 | NULL | 1 | 129/127 | 0xffffffffffffffff/0xffffffffffffffff | []/0;[R]/1;[S]/1 | F/T/T/F |
| B_FREE_FULLMAP 1 malloc | 128 | NULL | 128 | 256/0 | 0xffffffffffffffff/0xffffffffffffffff | [S]/1;[R]/1;[]/0 | F/T/T/F |
| C_INTERIOR_SINGLE 1 free | 3 | VOID_FALLTHROUGH | 65535 | 65535/1 | 0x0000000000000080 | [S32]/1;[]/0;[]/0 | T/T/T/T |
| C_DUP32_FIRST 1 free | 3 | VOID_FALLTHROUGH | 65535 | 65535/1 | 0x0000000000000080 | [S32]/1;[]/0;[]/0 | T/T/T/T |
| C_DUP64_FIRST 1 free | 1 | VOID_FALLTHROUGH | 32767 | 32767/1 | 0x0000000000000080 | [T64]/1;[]/0;[]/0 | T/T/T/T |
 
| 入口/调用 | 命名时点 | V |
|---|---|---|
| A_LAST_SLOT_CYCLE 1 | SCAN_BEFORE (269–286 complete, before295) | T/T/T/F |
| A_LAST_SLOT_CYCLE 1 | SCAN_RETURN (295) | T/T/T/F |
| A_LAST_SLOT_CYCLE 1 | OBJECT_UPDATE_DONE (298–304) | T/T/T/T |
| A_LAST_SLOT_CYCLE 1 | RETURN (327) | T/T/T/T |
| A_LAST_SLOT_CYCLE 2 | AFTER_CLEAR_COUNTS (358–361 before364) | T/T/T/F |
| A_LAST_SLOT_CYCLE 2 | CLASSIFICATION_DONE (364–379) | T/T/T/T |
| A_LAST_SLOT_CYCLE 2 | RETURN (391) | T/T/T/T |
| A_LAST_SLOT_CYCLE 3 | SCAN_BEFORE (269–286 complete, before295) | T/T/T/F |
| A_LAST_SLOT_CYCLE 3 | SCAN_RETURN (295) | T/T/T/F |
| A_LAST_SLOT_CYCLE 3 | OBJECT_UPDATE_DONE (298–304) | T/T/T/T |
| A_LAST_SLOT_CYCLE 3 | RETURN (327) | T/T/T/T |
| A_LAST_OCCUPIED_REUSE 1 | AFTER_CLEAR_COUNTS (358–361 before364) | T/T/T/F |
| A_LAST_OCCUPIED_REUSE 1 | CLASSIFICATION_DONE (364–379) | T/T/T/F |
| A_LAST_OCCUPIED_REUSE 1 | RETURN (391) | T/T/T/F |
| A_LAST_OCCUPIED_REUSE 2 | SCAN_BEFORE (269–286 complete, before295) | T/T/T/F |
| A_LAST_OCCUPIED_REUSE 2 | SCAN_RETURN (295) | T/T/T/F |
| A_LAST_OCCUPIED_REUSE 2 | OBJECT_UPDATE_DONE (298–304) | T/T/T/T |
| A_LAST_OCCUPIED_REUSE 2 | RETURN (327) | T/T/T/T |
| B_USED_ONE_FULLMAP 1 | SCAN_BEFORE (269–286 complete, before295) | F/T/T/F |
| B_USED_ONE_FULLMAP 1 | SCAN_RETURN (295) | F/T/T/F |
| B_USED_ONE_FULLMAP 1 | OBJECT_UPDATE_SKIPPED (296 false;298–304 not reached) | F/T/T/F |
| B_USED_ONE_FULLMAP 1 | RETURN (327) | F/T/T/F |
| B_FREE_FULLMAP 1 | SCAN_BEFORE (269–286 complete, before295) | F/T/T/F |
| B_FREE_FULLMAP 1 | SCAN_RETURN (295) | F/T/T/F |
| B_FREE_FULLMAP 1 | OBJECT_UPDATE_SKIPPED (296 false;298–304 not reached) | F/T/T/F |
| B_FREE_FULLMAP 1 | RETURN (327) | F/T/T/F |
| C_INTERIOR_SINGLE 1 | AFTER_CLEAR_COUNTS (358–361 before364) | T/T/T/T |
| C_INTERIOR_SINGLE 1 | CLASSIFICATION_DONE (364–379) | T/T/T/T |
| C_INTERIOR_SINGLE 1 | RETURN (391) | T/T/T/T |
| C_DUP32_FIRST 1 | AFTER_CLEAR_COUNTS (358–361 before364) | T/T/T/T |
| C_DUP32_FIRST 1 | CLASSIFICATION_DONE (364–379) | T/T/T/T |
| C_DUP32_FIRST 1 | RETURN (391) | T/T/T/T |
| C_DUP64_FIRST 1 | AFTER_CLEAR_COUNTS (358–361 before364) | T/T/T/T |
| C_DUP64_FIRST 1 | CLASSIFICATION_DONE (364–379) | T/T/T/T |
| C_DUP64_FIRST 1 | RETURN (391) | T/T/T/T |

实际链字段有序写、读取范围、locals、helper返回及前后态见facts对应call.stages/read_write_sets/call_return_boundaries。谓词只于命名时点评价；图的跨调用边是给定控制组合，非源码校验、回滚或重选。malloc E151-040–056；free E151-057–070；search及宏E151-002–006、071–074；实际列表助手075–086、四bitmap函数087–093；边界身份033–039。完整函数回源，不以局部标签推额外行为。

## 6. 构建与未证
五份CMake精确到命令实例：根project ASM C、Debug/Release条件及flags、musl-gcc配置路径、myinit/myshell、install init/sh，完整根文本无直接add_subdirectory(mylib)；mylib三独立子目录，myglibc add_library(mylibc STATIC)、set、target_include_directories、add_subdirectory(malloc)；malloc/util各两个独立file(GLOB)与target_sources。内核options活动CONFIG、m64/mcmodel/kernel/mabi=sysv只为内核文本，双canary定向；不认证用户ABI/DEBUG/标准/编译器、sh链接。仓外musl未读。E151-094–138。149闭合合读规则保持，所有新窗重新取源，不复制旧标签凑P6。
完整本地P9、预处理、编译链接、MyOS2程序/脚本、Graphviz、QEMU、实机、真实操作NOT_RUN。新增条件消费观察见deferred；不改源码或旧件、不自行进入152。
