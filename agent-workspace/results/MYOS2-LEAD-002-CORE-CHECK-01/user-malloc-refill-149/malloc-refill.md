---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
batch_id: "CORE-USER-MALLOC-REFILL-149"
conversation_display_name: "MYOS2-A-C10c 代码分析执行"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-24"
produced_by: "GPT-6.2"
model_per_owner: "GPT-6.2 Sol；Extra／高推理强度（Owner指定）"
actual_model_identifier_basis: "系统标识GPT-6.2；客户端选择不可见"
actual_backend_model: "UNATTESTED"
actual_reasoning_effort: "UNATTESTED"
date: "2026-11-25"
date_timezone: "Asia/Shanghai"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_pin_short12: "95d47a823b80"
execution_parent_short12: "d2c99a1f285b"
read_channel: "connector"
status: "AUTHOR_PENDING_MAINLINE_REVIEW"
evidence_class: "SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / CONDITIONAL_FRAGMENT / UNKNOWN"
open_questions: ["完整补充递归及终止性、实际对象建立/映射、bitmap载荷、真实ABI/标准/编译器/DEBUG、sh实际链接与运行均未证；N1547独立外证沿旧UNKNOWN。"]
full_local_P9: "NOT_RUN"
record_type: "batch149_analysis"
---

# 第149批 · malloc补充预留、申请依赖与对象门
作者交付，待C10 Pro独立验收；只读静态分析。A4/B3/C3十独立记录、两辅助布局。修订206及148冻结六件/主线PASS_PENDING_LOCAL，D00=[]；147/146合读§4补注、combined145及R145-01/02/03关闭、144及更早裁定保持。N1547独立外证UNKNOWN。旧EXEC-23私下工作UNKNOWN，不继承其分析或已读声明。

## 1. 公共给定和两项布局
8位byte；pointer、size_t、unsigned long、unsigned long long、bitmap_t均8字节；BITMAP_UNITSIZE=64。自然8对齐、声明顺序，无pack。List_s为prev/next两个指针，16字节；List_hdr_s首成员anchor，后跟count，模型24字节。H=0x200000仅本malloc.c PAGE_SIZE→CONST_2M；上限CONST_1M=0x100000。无并发、算术无回绕；明确NULL及缺失对象例外。地址只作整数投影。证据 E149-014、E149-015、E149-013、E149-016、E149-040、E149-017、E149-018、E149-019、E149-038、E149-039、E149-020、E149-021、E149-022、E149-023、E149-024、E149-025、E149-026、E149-035、E149-036、E149-027、E149-028、E149-029、E149-030、E149-033、E149-034、E149-031、E149-032、E149-037。

### uslab_s
| 成员 | offset | bytes | align |
|---|---:|---:|---:|
| uslab_list | 0 | 16 | 8 |
| uslabcache_p | 16 | 8 | 8 |
| total | 24 | 8 | 8 |
| free | 32 | 8 | 8 |
| upage_p | 40 | 8 | 8 |
| virt_addr | 48 | 8 | 8 |
| colormap | 56 | 8 | 8 |

总尺寸=64，尾补齐至8倍数；List成员offset=0。GIVEN算术模型而非实际ABI。

### uPage_s
| 成员 | offset | bytes | align |
|---|---:|---:|---:|
| upage_list | 0 | 16 | 8 |
| vaddr | 16 | 8 | 8 |
| uslab_p | 24 | 8 | 8 |

总尺寸=32，尾补齐至8倍数；List成员offset=0。GIVEN算术模型而非实际ABI。

## 2. A4 · 申请依赖
固定146@42066de2e04f正文和facts消费A00/01/08/09、B00/01/08/09及主线§4；重新核init 181–195、malloc 251–264、uslab_alloc 225/228、upage_alloc 114–124，不重做16档。initiated=true，cache按32<<i合法升序；请求不超过CONST_1M时选首个obj_size>=size。N=H/O，原228表达式是N/sizeof(bitmap_t)+sizeof(bitmap_t)即整数N/8+8，不是通用word向上取整。

| ID | i / O | N | metadata 请求→档 | bitmap 请求→档 | 动态page 请求→档 |
|---|---|---:|---|---|---|
| A32 | 0/32 | 65536 | 64→64 i1 | 8200→16384 i9 | 32→32 i0 条件 |
| A64 | 1/64 | 32768 | 64→64 i1 | 4104→8192 i8 | 32→32 i0 条件 |
| A8K | 8/8192 | 256 | 64→64 i1 | 40→64 i1 | 32→32 i0 条件 |
| A16K | 9/16384 | 128 | 64→64 i1 | 24→32 i0 | 32→32 i0 条件 |

225先申请64；合法metadata对象才能进入226及228申请；231 upage_alloc若free_upage_lhdr.count>0走116–120复用，不到124；else才有sizeof(uPage_s)=32请求，合法count对应0。全部请求在上限内。64自依赖、32↔16K、64↔8K均是潜在申请类依赖，另有各档metadata到64及条件page到32；不推出实际递归、无限递归或结果。A不执行列表/计数/分配。证据 E149-014、E149-015、E149-013、E149-016、E149-040、E149-017、E149-018、E149-019、E149-038、E149-039、E149-051、E149-052、E149-053、E149-054、E149-048、E149-049、E149-050、E149-041、E149-042、E149-043、E149-044、E149-045、E149-046、E149-047。

## 3. B3 · outer预留和首次nested malloc64
各独立从outer308前进入，非init或145续段。outer局部uscgp=C（64档）、uslp=U、ret_val=R=0x61000080尚未返回；C.normal_slab_total=1，free.count=0/物理空，F=free.anchor，F.prev=F.next=F。U.total=32768，V=0x61000000，owner=C；唯一slab，512 bitmap word合法，word1…511全1，除给定关系外不别名；不另读owner/base。 

| ID | used/full | U.free | word0 | C.free/C.used |
|---|---|---:|---|---|
| B_MORE | ["U"] / [] | 2 | 0xfffffffffffffffc | 2/32766 |
| B_LAST | ["U"] / [] | 1 | 0xfffffffffffffffe | 1/32767 |
| B_NO_USED | [] / ["U"] | 0 | 0xffffffffffffffff | 0/32768 |

共同：308读0，条件真，310 count0→1而物理空；311调用uslab_alloc(U)，outer暂停，312及以后未达；callee225调用malloc64。nested独立ret=NULL/uslp=NULL，initiated=true跳过init，从i0跨一步仍选同C。R不是nested ret。

### B_MORE / B_LAST
269 used.count>0，271摘U.list；helper 87按count，89取anchor.next，90 list_del_init先写前邻next/后邻prev，再INIT_LIST_S设U.list.prev/next=self，91 used.count1→0，92返回U.list。272 NULL等待假，274 offset0投影合法U。275比较减之前U.free，MORE=2回used，LAST=1先入full。list_header_add_to_head→list_insert_next先对self-loop的U.list再次del_init（四指针写、不另扣header count），然后src.next=dst.next、src.prev=dst、src.next->prev=src、src.prev->next=src，目标count++；扫描前完成迁移。

| 场景 | 摘取前 free | 扫描前 used/full count | idx/offset | U.free | C.free/C.used | word0 |
|---|---:|---|---|---|---|---|
| B_MORE | 2 | 1/0 | 0/0 | 2→1 | 2→1 / 32766→32767 | fc→fd（高位全1） |
| B_LAST | 1 | 0/1 | 0/0 | 1→0 | 1→0 / 32767→32768 | fe→ff（高位全1） |

295 bm_get_freebit_idx从0到total，word0、shift0、1ULL掩码1、AND=0，break返回idx0；296 0<32768真，298 offset64*0=0、299 nested ret=V；301–303三计数更新，304 bm_set_bit对word0读改写OR1ULL。其他511word未直接读写（非机器访存次数认证）。308 nested门到达且条件假，读到预留count1/物理0，不代表存在备用节点；跳过主体，327返回V给callee225。slab_total=1；V处未给活且足够大、类型/成员合法的uslab对象，止于226之前，不作新metadata访问。outer仍暂停，R未返回、312未减、313未检查、318–321未发布。

### B_NO_USED
269 used.count0，280 free.count1>0，282调用remove_head；87 count>0，89 ret=F.next=F；90 list_del_init(F)：F.prev->next=F.next即F.next=F、F.next->prev=F.prev即F.prev=F，再INIT_LIST_S写F.prev=F/F.next=F；91 count1→0、92返回F而不是NULL。环仍物理空，full=[U]、used空、slab_total=1，U.free=0/C.free=0/C.used=32768，512word未读写。283等待条件假；285偏移0只投影F−0。F是合法header.anchor，不是活uslab的uslab_list，缺对象及转换前提；停止于286之前，**不执行286**。无扫描、对象计数更新、nested/outer返回、outer312；count回0来自helper。既未触发NULL等待也未走291显式循环。ANALYSIS_STOP_MISSING_SLAB_OBJECT_PREMISE不是源码分支或真实崩溃。B各字段及两帧见facts，证据 E149-051、E149-052、E149-053、E149-054、E149-055、E149-056、E149-057、E149-058、E149-059、E149-060、E149-061、E149-062、E149-063、E149-064、E149-065、E149-066、E149-067、E149-048、E149-049、E149-050、E149-074、E149-075、E149-076、E149-071、E149-072、E149-068、E149-073、E149-069、E149-070、E149-078、E149-079、E149-080、E149-077、E149-033、E149-034、E149-031、E149-032、E149-027、E149-028、E149-029、E149-030、E149-020、E149-021、E149-022、E149-023、E149-024、E149-025、E149-026、E149-035、E149-036、E149-037。

## 4. C3 · 独立GIVEN响应与对象门
合法活cslp，两次total读取均128；合法D/M/P足够大、活、成员访问合法，无未给别名，跨调用保持。每一次未展开调用分别GIVEN：从调用前到其正常返回，除指定返回值外，对调用前已确定的所跟踪字段无其他净效应；本层随后显式写另计。未给全局/递归效应和中间瞬态UNKNOWN。 不从净零推出内部无写。
- C_META_NULL：225 malloc64给NULL，nslp=NULL。226成员初始化需合法对象，分析止于226前，无cslp.total读取、后续调用或合法metadata写；不新增源码NULL检查，不推导真实return NULL或异常。
- C_MAP_NULL：225给D，226 INIT_LIST_S置D.list.prev/next=self；228读128、申请24，GIVEN NULL，合法D.colormap=NULL存储；229–230再读128，D.free=128、D.total=128；231给P，D.upage_p=P；232读P.vaddr=W=0x70000000、写D.virt_addr=W；233写P.uslab_p=D；235条件性返回D。self-loop由每调用frame保持。完整所读uslab_alloc及直接INIT_LIST_S无直接bitmap payload访问，不证后续可安全使用；D.uslabcache_p本层未直接写、实际值UNKNOWN，不说NULL或未初始化字节。
- C_PAGE_NULL：225给D，226 self-loop；228读128申请24给合法M，D.colormap=M；229–230赋free/total128；231给NULL，合法D.upage_p=NULL存储已完成。232读该指针后需要活page的vaddr，缺对象前提，止于该读，不写D.virt_addr/owner或推导返回；前一存储不是对NULL对象写。

caller312回减、313检查new_uslp只发生在callee返回之后，不能保护226/232内部对象门。C不执行caller发布，不重做146 C_PUBLISH；未推导真实allocator可达性、自然返回NULL、运行故障或补丁。证据 E149-048、E149-049、E149-050、E149-068、E149-063、E149-064、E149-065、E149-066、E149-067。直接写/调用完整范围见facts，其他调用按frame限定，未给副作用UNKNOWN。

## 5. 构建范围
固定time kernel options_flags的活动CONFIG、m64/mcmodel/kernel/mabi=sysv和panic双canary，直接master反验；不能推出用户ABI。五份用户CMake文本：initramfs根C/ASM/Debug/Release、仓外musl-gcc配置路径（未读仓外musl）、myinit/myshell及install init/sh；mylib子树myglibc/csu/util；myglibc定义mylibc STATIC及add_subdirectory(malloc)；malloc/util各自GLOB *.c/*.S到target_sources(mylibc PUBLIC)。所读根未见直接add_subdirectory(mylib)；候选目标不证明sh实际链接。actual_linked_to_sh、实际标准/编译器/DEBUG/ABI/映射/载荷/对象/完整递归/终止均UNKNOWN。证据 E149-001、E149-002、E149-003、E149-004、E149-005、E149-006、E149-007、E149-008、E149-009、E149-010、E149-011、E149-012、E149-081、E149-082、E149-083、E149-084、E149-085、E149-086、E149-087、E149-088、E149-089、E149-090、E149-091、E149-092、E149-093、E149-094、E149-095、E149-096。完整本地P9、预处理、编译链接、项目程序/脚本、Graphviz、QEMU、实机及真实操作NOT_RUN。新条件观察见deferred；不重报继承未知。
