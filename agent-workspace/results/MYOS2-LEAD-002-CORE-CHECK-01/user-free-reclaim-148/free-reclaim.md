---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: batch148_free_reclaim_analysis
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-23
produced_by: GPT-6.2
model_per_owner: "GPT-6.2 Sol Extra/high requested; backend and actual effort UNATTESTED"
date: 2026-11-25
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
read_channel: connector
status: AUTHOR_PENDING_MAINLINE_REVIEW
---

# 第148批 · free回收门、页描述符和调用边界

**范围：** A6/B4/C5共15独立记录，固定time候选用户库。147合读主线§4通过，D00空；147三个正常C门为假，不接入本批A真分支。C.clear字段只表示独立复算算术，实际给定free片段安全assigned后clear。N1547作者独立外证UNKNOWN继承，不重做。实际编译器/标准/DEBUG/ABI、链接、页映射、运行未证。给定计数可表示、无回绕并发；对象、字段和循环链合法且不别名，除明确未知或额外前提外。

## A · 六个回收门独立新入口

S在used，total/free=128/65，full空，其余free slab各128/128。初free.count=n=实长、used.count=1、full.count=0、total=n+1、CF=128n+65、CU=63，因此CF+CU=128(n+1)。E148-033实际是单次if，读count>2；真时读anchor.prev并container_of选尾节点，仅减caller count后调用uslab_free，未摘链。假时既不读tail也不写相关字段，止于条件性void尾部结束。

| ID | free链 | base | 初count | gate | tail read | 选择/是否base | 末count/实长 | total | CF/CU | CF+CU | 终点 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| A0 | [] | S | 0 | FALSE | false | NOT_REACHED/NOT_REACHED | 0/0 | 1 | 65/63 | 128 | CONDITIONAL_VOID_TAIL_COMPLETION |
| A1 | [B] | B | 1 | FALSE | false | NOT_REACHED/NOT_REACHED | 1/1 | 2 | 193/63 | 256 | CONDITIONAL_VOID_TAIL_COMPLETION |
| A2 | [B,R] | B | 2 | FALSE | false | NOT_REACHED/NOT_REACHED | 2/2 | 3 | 321/63 | 384 | CONDITIONAL_VOID_TAIL_COMPLETION |
| A3_BASE_HEAD | [B,R,T] | B | 3 | TRUE | true | T/false | 2/3 | 4 | 449/63 | 512 | BEFORE_USLAB_FREE_CALLEE_EFFECTS |
| A3_BASE_TAIL | [R,T,B] | B | 3 | TRUE | true | B/true | 2/3 | 4 | 449/63 | 512 | BEFORE_USLAB_FREE_CALLEE_EFFECTS |
| A4 | [B,R,R2,T] | B | 4 | TRUE | true | T/false | 3/4 | 5 | 577/63 | 640 | BEFORE_USLAB_FREE_CALLEE_EFFECTS |

真例调用前count=实长−1，total、CF/CU、列表成员和链接未被本片段改变。base尾例选B并传参，此处实际条件只有count>2，没有base身份检查；base指针字段与保护意图注释、base数组和初始化赋值分别由E148-023,E148-024,E148-027,E148-028,E148-045,E148-046支持，注释不构成保护分支。只证明给定入口选中参数；不推断真实base被释放、callee返回或后续计数。源级读写/调用集合逐ID见facts.reclaim_entries。

## B · 四个独立upage_free调用

E148-029/E148-030读完整143–149，直接调用list_del_init、list_header_add_to_head；后者展开list_insert_next，再调用list_del_init与INIT_LIST_S（E148-050,E148-051,E148-052,E148-053,E148-054,E148-055,E148-056,E148-057）。以下UA/FA分别为used/free的anchor；P0/P1/P2/Q0/Q1的vaddr=0x60000000/0x60200000/0x60400000/0x60600000/0x60800000，仅字段值。目标原owner=TargetSlab，其余Owner_节点标签不同。

| ID | 目标 | 初used/free | 末used/free | counts初→末 | 总数 | owner末 | vaddr |
| --- | --- | --- | --- | --- | --- | --- | --- |
| B_HEAD | P0 | [P0,P1,P2]/[Q0,Q1] | [P1,P2]/[P0,Q0,Q1] | 3/2→2/3 | 5 | NULL | 0x60000000 |
| B_MIDDLE | P1 | [P0,P1,P2]/[Q0,Q1] | [P0,P2]/[P1,Q0,Q1] | 3/2→2/3 | 5 | NULL | 0x60200000 |
| B_TAIL | P2 | [P0,P1,P2]/[Q0,Q1] | [P0,P1]/[P2,Q0,Q1] | 3/2→2/3 | 5 | NULL | 0x60400000 |
| B_SINGLE_EMPTY | P0 | [P0]/[] | []/[P0] | 1/0→0/1 | 1 | NULL | 0x60000000 |

### B_HEAD

| 阶段 | 写集合（按源码顺序，含同值写） | used成员/count | free成员/count | 目标owner |
| --- | --- | --- | --- | --- |
| 1 | UA.next:P0→P1；P1.prev:P0→UA；P0.prev:UA→P0；P0.next:P1→P0 | [P1,P2]/3 | [Q0,Q1]/2 | TargetSlab |
| 2 | used_upage_lhdr.count:3→2 | [P1,P2]/2 | [Q0,Q1]/2 | TargetSlab |
| 3 | P0.next:P0→P0；P0.prev:P0→P0；P0.prev:P0→P0；P0.next:P0→P0 | [P1,P2]/2 | [Q0,Q1]/2 | TargetSlab |
| 4 | P0.next:P0→Q0；P0.prev:P0→FA；Q0.prev:FA→P0；FA.next:Q0→P0 | [P1,P2]/2 | [P0,Q0,Q1]/2 | TargetSlab |
| 5 | free_upage_lhdr.count:2→3 | [P1,P2]/2 | [P0,Q0,Q1]/3 | TargetSlab |
| 6 | P0.uslab_p:TargetSlab→NULL | [P1,P2]/2 | [P0,Q0,Q1]/3 | NULL |

### B_MIDDLE

| 阶段 | 写集合（按源码顺序，含同值写） | used成员/count | free成员/count | 目标owner |
| --- | --- | --- | --- | --- |
| 1 | P0.next:P1→P2；P2.prev:P1→P0；P1.prev:P0→P1；P1.next:P2→P1 | [P0,P2]/3 | [Q0,Q1]/2 | TargetSlab |
| 2 | used_upage_lhdr.count:3→2 | [P0,P2]/2 | [Q0,Q1]/2 | TargetSlab |
| 3 | P1.next:P1→P1；P1.prev:P1→P1；P1.prev:P1→P1；P1.next:P1→P1 | [P0,P2]/2 | [Q0,Q1]/2 | TargetSlab |
| 4 | P1.next:P1→Q0；P1.prev:P1→FA；Q0.prev:FA→P1；FA.next:Q0→P1 | [P0,P2]/2 | [P1,Q0,Q1]/2 | TargetSlab |
| 5 | free_upage_lhdr.count:2→3 | [P0,P2]/2 | [P1,Q0,Q1]/3 | TargetSlab |
| 6 | P1.uslab_p:TargetSlab→NULL | [P0,P2]/2 | [P1,Q0,Q1]/3 | NULL |

### B_TAIL

| 阶段 | 写集合（按源码顺序，含同值写） | used成员/count | free成员/count | 目标owner |
| --- | --- | --- | --- | --- |
| 1 | P1.next:P2→UA；UA.prev:P2→P1；P2.prev:P1→P2；P2.next:UA→P2 | [P0,P1]/3 | [Q0,Q1]/2 | TargetSlab |
| 2 | used_upage_lhdr.count:3→2 | [P0,P1]/2 | [Q0,Q1]/2 | TargetSlab |
| 3 | P2.next:P2→P2；P2.prev:P2→P2；P2.prev:P2→P2；P2.next:P2→P2 | [P0,P1]/2 | [Q0,Q1]/2 | TargetSlab |
| 4 | P2.next:P2→Q0；P2.prev:P2→FA；Q0.prev:FA→P2；FA.next:Q0→P2 | [P0,P1]/2 | [P2,Q0,Q1]/2 | TargetSlab |
| 5 | free_upage_lhdr.count:2→3 | [P0,P1]/2 | [P2,Q0,Q1]/3 | TargetSlab |
| 6 | P2.uslab_p:TargetSlab→NULL | [P0,P1]/2 | [P2,Q0,Q1]/3 | NULL |

### B_SINGLE_EMPTY

| 阶段 | 写集合（按源码顺序，含同值写） | used成员/count | free成员/count | 目标owner |
| --- | --- | --- | --- | --- |
| 1 | UA.next:P0→UA；UA.prev:P0→UA；P0.prev:UA→P0；P0.next:UA→P0 | []/1 | []/0 | TargetSlab |
| 2 | used_upage_lhdr.count:1→0 | []/0 | []/0 | TargetSlab |
| 3 | P0.next:P0→P0；P0.prev:P0→P0；P0.prev:P0→P0；P0.next:P0→P0 | []/0 | []/0 | TargetSlab |
| 4 | P0.next:P0→FA；P0.prev:P0→FA；FA.prev:FA→P0；FA.next:FA→P0 | []/0 | [P0]/0 | TargetSlab |
| 5 | free_upage_lhdr.count:0→1 | []/0 | [P0]/1 | TargetSlab |
| 6 | P0.uslab_p:TargetSlab→NULL | []/0 | [P0]/1 | NULL |

每例先改邻接prev/next并self-init，再used.count--；insert_next再次unlink已self-loop的目标仅写自身、不重复摘旧成员或改变header count；写新头及邻接后才free.count++，最后目标page.uslab_p=NULL。每个阶段含两个anchor及所有节点链接快照在facts；描述符总数守恒，目标只在目的链出现一次，vaddr与其他owner不改。完整函数及直接helper定义无brk或free调用；本层显式写集合不含slab.upage_p反向字段、vaddr或payload，不证明真实缩堆、内核页归还、解除映射或后续分配成功。

## C · 五个独立片段

E148-031/E148-032的完整uslab_free为：先upage_free(slp->upage_p)，再free(slp->colormap)，再list_del_init，读当前owner并减其free.count，最后free(slp)。free调用均不展开。C1/C2/C3的“返回”是独立给定入口，不由前例推出。

### C0

A3_BASE_HEAD调用前状态作为本例给定：C.free=[B,R,T]，count=2、实长3，total=4、CF/CU=449/63；T.upage_p=P1、T.uslabcache_p=C、T.colormap=M、total/free128。展开本层upage_free(P1)，从B_MIDDLE页初态、owner=T出发：

| 阶段 | 有序字段写 | used/count | free/count | P1.owner |
| --- | --- | --- | --- | --- |
| 1 | P0.next:P1→P2；P2.prev:P1→P0；P1.prev:P0→P1；P1.next:P2→P1 | [P0,P2]/3 | [Q0,Q1]/2 | T |
| 2 | used_upage_lhdr.count:3→2 | [P0,P2]/2 | [Q0,Q1]/2 | T |
| 3 | P1.next:P1→P1；P1.prev:P1→P1；P1.prev:P1→P1；P1.next:P1→P1 | [P0,P2]/2 | [Q0,Q1]/2 | T |
| 4 | P1.next:P1→Q0；P1.prev:P1→FA；Q0.prev:FA→P1；FA.next:Q0→P1 | [P0,P2]/2 | [P1,Q0,Q1]/2 | T |
| 5 | free_upage_lhdr.count:2→3 | [P0,P2]/2 | [P1,Q0,Q1]/3 | T |
| 6 | P1.uslab_p:T→NULL | [P0,P2]/2 | [P1,Q0,Q1]/3 | NULL |

页链末used=[P0,P2] count2，free=[P1,Q0,Q1] count3；T.upage_p仍P1，T.colormap仍M、T.owner=C，cache链/字段保持；P1.owner=NULL且vaddr不改。随后读T.colormap并调用free(M)，止于该call边界，返回、副作用及后续对象状态UNKNOWN。

### C1

新GIVEN：free(colormap)正常返回、T仍存活且合法，当前owner=C_m（可与C不同），free链[B,R0,R1,R2,T]、count=5。243行unlink：R2.next T→anchor、anchor.prev T→R2、T.prev/next→T；链[B,R0,R1,R2]实长4、count仍5。244行通过**当前**T.uslabcache_p读C_m并count5→4。T为self-loop；246行调用free(T)即停，不推断返回、生存期或caller读取。

### C2 / C3

两个分别独立的uslab_free返回后入口：C仍有效，total4、free.count1、free链[B,R]实长2，used=[S]、full=[]，CF449/CU63；给定所列成员容量384、CF321、CU63，入口字段总容量512，入口已有count/长度、total/成员数、CF/所列空闲槽不一致。不是本片段制造。386行`uscgp->normal_slab_total;`是丢弃值的字段表达式，**无store和递减**，不声称实际机器load。387行复合赋值需要读取tmp_uslp->total与CF；不额外断言子求值次序。

| ID | T前提 | total前→后 | count/实长 | CF/CU前→后 | 末CF+CU | 终点 |
| --- | --- | --- | --- | --- | --- | --- |
| C2 | UNKNOWN | 4→4 | 1/2保持 | 449/63→NOT_DERIVED/63 | NOT_DERIVED | ANALYSIS_STOP_MISSING_OBJECT_ACCESS_PREMISE |
| C3 | GIVEN_ALIVE_TYPED_LEGAL_USLAB_ACCESS | 4→4 | 1/2保持 | 449/63→321/63 | 384 | CONDITIONAL_TAIL_COMPLETION |

C2只有指针值T，缺少存活/类型/合法访问及total证据，停在ANALYSIS_STOP_MISSING_OBJECT_ACCESS_PREMISE；CF不推导（表中63是本片段无CU写的源级记录，不是实际运行后状态保证）。C3**另给**T存活、类型合法且T.total=128，才得CF449−128=321，CU63，末CF+CU=384；total仍4、count仍1/实长2，原有不一致保留，不能说总体回收成功。C2不消费C3前提。

### C4 · 两写一次摘链的条件代数

E148-033的caller 384先减C.free.count；E148-032的callee 243摘T，244在递归free(colormap)已返回后通过当时T.owner读相应cache再减。额外GIVEN：同一C、两store均到达、F0=3、[B,R,T]唯一、合法对象/链接、无下溢，除此两减量和一次摘T之外，无其他净count/成员/长度/剩余顺序影响。观察终点是第二次count--完成之后、free(T)调用之前。故count=3−2=1、成员[B,R]实长2、count−实长=−1。若Δ是两写以外同一字段的有符号净变化，则count=1+Δ；仅在仍保持无其他净成员变化时实长2、差Δ−1。中间瞬态不能由净效应约束擅自推出；若owner变或某store未到达，不能合并为同一字段−2。此关系不推断free(T)或caller下一读取。

## 配置与继承范围

活动内核配置、panic canary及五份用户构建文本E148-001,E148-002,E148-003,E148-004,E148-005,E148-006,E148-007,E148-008,E148-009,E148-010,E148-011,E148-012,E148-013,E148-014,E148-015,E148-016,E148-017,E148-018,E148-019,E148-020,E148-021,E148-022；顶层外部musl、直接myinit/myshell，另一棵mylibc含malloc/util；实际sh链接UNKNOWN，用户DEBUG实际宏/编译标准未知。完整本地P9、预处理、编译链接、项目程序/脚本、Graphviz、QEMU、实机/真实操作NOT_RUN。只有局部条件观察，登记deferred；不修复，不将C0/C1/C2/C3串为实际递归。主线独审PENDING。
