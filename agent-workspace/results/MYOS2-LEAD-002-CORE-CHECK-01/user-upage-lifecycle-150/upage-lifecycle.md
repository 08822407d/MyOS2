---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
batch_id: "CORE-USER-UPAGE-LIFECYCLE-150"
conversation_display_name: "MYOS2-A-C10c 代码分析执行"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-24"
produced_by: "GPT-6.2"
model_per_owner: "GPT-6.2 Sol；Extra／高推理强度（Owner指定）"
actual_model_identifier_basis: "系统标识GPT-6.2；客户端具体选择不可见"
actual_backend_model: "UNATTESTED"
actual_reasoning_effort: "UNATTESTED"
date: "2026-11-26"
date_timezone: "Asia/Shanghai"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_pin_short12: "dc12f97a7a91"
execution_parent_short12: "9aa7de497eb0"
read_channel: "connector"
status: "AUTHOR_PENDING_MAINLINE_REVIEW"
full_local_P9: "NOT_RUN"
evidence_class: "SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / CONDITIONAL_FRAGMENT / OBSERVATION / UNKNOWN"
open_questions: ["R149-01作者补正待主线判闭；真实NULL brk可达性、ABI/标准/编译器/DEBUG、实际对象/映射/生命周期、链接及完整递归未知；N1547独立外证UNKNOWN继承。"]
record_type: "batch150_analysis"
---

# 第150批 · 用户页申请结果、失败清理与页描述符复用归属
作者交付，主线独立验收待做。第149批RETURN_DOCUMENTATION保留；R149-01本轮提交定点补正、待C10判闭。148及更早有效裁定/合读补注保持。A三例、B两个独立细化，C一条控制序列三个快照（共8观察记录，不是8个独立运行）。D00另计。
 
## D00 · R149-01
原固定149@9aa7de497eb0，主线独审@dc12f97a7a91，作者#7167216690。原引文真实但符号范围不合P2；不能用字节命中抵消错误。逐命令补正如下（证据中的实例范围及行尾可逆信息为准）：
 
| 原ID | 原符号/物理行 | 问题 | 新证据/实际命令 |
|---|---|---|---|
| E149-087 | add_subdirectory(myinit) 46–50 | MIXED_DISTINCT_COMMANDS | E150-001 include_directories(MUSL_INC_PATH)；E150-002 add_subdirectory(myinit)；E150-003 add_subdirectory(myshell)；E150-004 install(TARGETS init DESTINATION boot) |
| E149-088 | add_subdirectory(myinit) 51–51 | WRONG_COMMAND_P2 | E150-005 install(TARGETS sh DESTINATION boot) |
| E149-089 | add_subdirectory(myglibc) 1–3 | OVER_NARROW_INSTANCE_LABEL | E150-006 add_subdirectory(myglibc)；E150-007 add_subdirectory(csu)；E150-008 add_subdirectory(util) |
| E149-094 | add_subdirectory(malloc) 18–21 | OVER_NARROW_INSTANCE_LABEL | E150-009 add_subdirectory(stdio)；E150-010 add_subdirectory(malloc)；E150-011 add_subdirectory(string)；E150-012 add_subdirectory(exit) |
 
四个旧ID映射12个新窗，E149-088源末行NONE仍只在闭围栏前加一个排版LF。原149 evidence/facts.source_index/build_scope/required_anchor_coverage、正文§5、MANIFEST及作者P2概括合读此表；旧P6=96/96/0不改，新窗计150 P6；旧原件/评论冻结。修正后只证明根CMake的include、myinit/myshell、install init/sh及两子树的具体add_subdirectory调用；不能据此推定真实sh链接。E149-090邻接旧窗混合命令另登记D150-O04，非208四ID的扩大判闭；150当前构建全部使用准确新窗。documentation_disposition_submitted；mainline_closure=PENDING。
 
## 1. 共同模型
H=0x200000源于malloc.c PAGE_SIZE→CONST_2M；非内核页尺寸。GIVEN 8位byte、32位int/unsigned、64位pointer/size_t/unsigned long/unsigned long long/bitmap_t，BITMAP_UNITSIZE=64；自然8对齐。实际List两指针16字节；uPage_s的upage_list 0/16、vaddr 16/8、uslab_p 24/8，总32；uslab_s的uslab_list0/16，owner16、total24、free32、upage_p40、virt_addr48、colormap56各8，总64。布局算术而非实际ABI。地址加减/商及对齐仅整数投影，合法对象和转换按例独立给定，无回绕并发。每一未展开malloc/brk分别GIVEN正常返回指定值；从调用前至返回，除指定返回外，对调用前已确定所跟踪字段无其他净效应；本层显式写另计；有效对象存活、类型、大小及无未说明别名跨调用保持；未跟踪全局/递归及中间瞬态UNKNOWN。 A/B brk NULL是独立给定，非144的非零旧break拒绝后继；不证生产可达。144的C_REUSE/C_GROW、147低位free/lookup、148 upage_free、146 caller发布只作排重/边界；149原模型不重做。N1547独立外证UNKNOWN继承。
 
## 2. A3 · metadata/brk响应组合
入口独立：initiated=true、free_upage空/count0、used=[Q]/count1、Q合法vaddr未指定；brk_end=0x70000000，ret_val初始NULL。124 malloc(sizeof(uPage_s))=32，125 brk实参=0x70200000；逐调用frame。M若给定为活、足够大合法uPage，投影0x62000060，未入页队列，不与Q/header别名；不自动给free所需lookup/owner。
 
| ID | upgp | pg_vaddr | 126门 | brk_end于停点 | free及返回 |
|---|---|---|---|---|---|
| A_META_NULL_NONNULL | NULL | 0x70200000 | 真 | 0x70200000（128已写） | 129前缺对象；136/137/140未达 |
| A_META_NULL_NULL | NULL | NULL | 假 | 0x70000000 | 136 free(NULL)正常早返；137赋NULL、140条件返回NULL |
| A_META_VALID_NULL | M | NULL | 假 | 0x70000000 | 136 free(M)调用边界；返回未推导，137/140未达 |
 
A1：124给NULL也照样调用125，无metadata检查；126真，128先独立+=H。129要写upgp->vaddr缺对象，分析停点非源码if、非实际异常；保留已完成的brk_end更新，130–132未达。A2：126假、136进入free(NULL)，332–335若DEBUG包含，initiated=true使while假；非DEBUG则没有该循环；337真，338正常早返，不读lookup/owner/bitmap。137显式赋NULL（区别初值），140返回。A3：136调用free(M)而止；不能以初始ret_val=NULL当已返回。三例停点前页队列仍used[Q]/1、free[]/0；A3不推未展开清理后态。源读写集合、局部量与时序逐例见facts.allocation_result_cases。证据 E150-063、E150-064、E150-065、E150-066、E150-067、E150-068、E150-069、E150-084、E150-085、E150-086、E150-087、E150-088、E150-089、E150-090、E150-091、E150-092、E150-093、E150-094、E150-095、E150-096、E150-037、E150-038、E150-039、E150-040、E150-041、E150-042、E150-032、E150-033、E150-034、E150-035、E150-036、E150-043、E150-044、E150-045、E150-046、E150-047、E150-048、E150-049、E150-050、E150-051、E150-052、E150-053、E150-054、E150-055、E150-056、E150-057、E150-058、E150-059、E150-060、E150-061、E150-062。
 
## 3. B2 · free(M)能否正常返回
两个独立细化，不连接A的真实生产链；入口M=0x62000060、ret_val初始NULL尚未执行137、pg_vaddr=NULL、brk_end=0x70000000、used[Q]/1、free[]/0；已给定malloc/brk响应并分别满足frame。free DEBUG等待（若编译）因initiated=true为假；337非NULL不早返。
 
### B_LOOKUP_MISS
Q.vaddr=0x66000000。upage_search在154以PAGE_ROUND_DOWN→round_down→__round_mask进行H对齐，mask=0x1fffff；M向下取整0x62000000。156–166从used.anchor.next=Q开始，Q.vaddr不匹配，next到anchor终止；168返回NULL。341保存upgp=NULL；342读取upgp->uslab_p需活page，缺对象停止；未读slab/cache/bitmap，未写对象/页链或brk_end，无正常free返回、137/140未达。活M不自动进入lookup索引；无源码NULL检查或真实故障结论。
 
### B_LOW_BIT_RETURN
Q.vaddr=V=0x62000000，Q.uslab_p=S，S.upage_p=Q、virt_addr=V、owner=C，32档，S.total=65536、free=65534；C.total=1、base=S、used[S]/1，free/full物理空/count0；C对象free/used=65534/2。1024合法bitmap word，word0=0x88(bits3,7)，其余全0。M为活合法uPage但不是Q、未登记页节点；与Q/S/C/bitmap/header不重叠；所有本例读写及转换合法GIVEN，地址投影不证明实际C指针操作。
 
search同键V，首Q匹配返回，342–343依次取得S/C；345 65534==65536假。351整数(M−V)=0x60=96，/32得3。352 assigned：unit_idx=0、shift=3、源字面1按GIVEN int32，mask=8、word0&8=8，unsigned32返回8（非布尔1），!8假，不入显式拒绝循环。358 clear：1ULL<<3=8，64位补码0xfffffffffffffff7，word0从0x0000000000000088变0x0000000000000080；其余1023 word不读写。359–361 S.free/C.free 65534→65535、C.used2→1；364/370更新后值既非1也非0，不迁S；381 free.count0>2假，未调用uslab_free/upage_free或递归free。S仍used[S]/1，free/full0，slab_total1、base=S；used页[Q]/1、free页[]/0、brk_end不变。M载荷没有本段直接写入，不由计数推真实生命周期。free正常走到391尾后，才回到137显式赋NULL、140条件返回NULL。两细化的读写、lookup扫描、门/返回及状态见facts.cleanup_cases；不扩展高位类型问题。证据 E150-072、E150-073、E150-074、E150-075、E150-084、E150-085、E150-086、E150-087、E150-088、E150-089、E150-090、E150-091、E150-092、E150-093、E150-094、E150-095、E150-096、E150-109、E150-110、E150-111、E150-032、E150-033、E150-034、E150-035、E150-036、E150-043、E150-044、E150-045、E150-046、E150-047、E150-048、E150-049、E150-050、E150-051、E150-052、E150-053、E150-054、E150-055、E150-056、E150-057、E150-058、E150-059、E150-060、E150-061、E150-062。
 
## 4. C · 单一受控释放/复用/重绑序列
入口used=[P,Q]/2、free=[R]/1；P.vaddr=W=0x64000000，Q=0x66000000，R=0x68000000；P.owner=D_old，D_old活且upage_p=P、virt_addr=W，全序列保持、不清理。合法活T两次total=128；uslab_alloc的225 malloc64给合法D_new，228 malloc24给合法Mmap，全部互异不别名，D_new未入slab列表；逐调用frame保持页队列/字段、旧D_old、T.total、已确定新D_new字段，特别第二次malloc保持226 self-loop。
 
1. 直接upage_free(P)：145 list_del_init先UA.next=Q、Q.prev=UA，P.prev/P.next=self；146 used.count2→1；147 list_insert_next再次对self-loop作四指针写但不再扣count，头插FA前R，P.next=R、P.prev=FA、R.prev=P、FA.next=P，free.count1→2；148 P.uslab_p=NULL。正常返回后C_RELEASED；D_new还未给定。
2. 一次uslab_alloc(T)：225 malloc64给D_new；226双指针self-loop；228读T.total=128，malloc(128/8+8)=24给Mmap、存colormap；229–230再读128，写free/total128。231进入唯一一次upage_alloc：free.count2>0，116 remove_head返回P.list，先FA.next=R、R.prev=FA、P.prev/P.next=self，free.count2→1；117等待假，119 container_of偏移0到给定P；120头插used，重复self-loop del_init四写、P.next=Q、P.prev=UA、Q.prev=P、UA.next=P，used.count1→2；140返回P。内部124/125未达，未调用内部malloc/brk。231存D_new.upage_p=P后C_REUSED；P.owner仍NULL，D_new.virt_addr入口未指定且尚未写。
3. 232读P.vaddr=W并赋D_new.virt_addr=W；233 P.uslab_p=D_new；235条件正常返回D_new，C_REBOUND。旧D_old.upage_p/virt_addr保持P/W；本层无直接写D_new.uslabcache_p，实际值UNKNOWN。未执行caller318–321发布、未加入cache、未补写owner。C中实际展开一次upage_free、一次uslab_alloc及内部一次upage_alloc，前两次metadata malloc另计，非全序列无malloc。指针/计数有序写入具体身份在facts.reuse_sequence及read_write_sets。
 
| 观察 | used/count | free/count | P.vaddr | P.uslab_p | D_old.upage_p/virt_addr | D_new.upage_p | D_new.virt_addr |
|---|---|---|---|---|---|---|---|
| C_RELEASED | [Q]/1 | [P,R]/2 | W | NULL | P/W | NOT_YET_GIVEN | NOT_YET_GIVEN |
| C_REUSED | [P,Q]/2 | [R]/1 | W | NULL | P/W | P | UNKNOWN_NOT_YET_SET |
| C_REBOUND | [P,Q]/2 | [R]/1 | W | D_new | P/W | P | W，232已写 |
 
三个独立只读探针均输入W+0x80=0x64000080→round_down W；RELEASED扫描Q不匹配→NULL，REUSED/REBOUND首P匹配→P；其owner作为快照字段分别NULL/D_new，search本身不读取owner。探针是观察关系，不插入源序列或推出并发可见性，不继续调用free。旧反向P/W保持不证明实际悬空、双重分配或故障。证据 E150-070、E150-071、E150-063、E150-064、E150-065、E150-066、E150-067、E150-068、E150-069、E150-081、E150-082、E150-083、E150-072、E150-073、E150-074、E150-075、E150-032、E150-033、E150-034、E150-035、E150-036、E150-097、E150-098、E150-099、E150-100、E150-101、E150-102、E150-103、E150-104、E150-105、E150-106、E150-107、E150-108、E150-043、E150-044、E150-045、E150-046、E150-047、E150-048、E150-049、E150-050、E150-051、E150-052、E150-053、E150-054、E150-055、E150-056、E150-057、E150-058、E150-059、E150-060、E150-061、E150-062、E150-076、E150-077、E150-078、E150-079、E150-080。
 
## 5. 构建与边界
五份CMake固定候选文本：根列myinit/myshell、安装init/sh、Debug/Release及C/ASM配置、musl-gcc路径（不读取外部musl）；mylib下myglibc/csu/util；myglibc add_library(mylibc STATIC)、add_subdirectory(malloc)；malloc/util file(GLOB C及ASM)两命令各自进target_sources。根完整所读文本未见直接add_subdirectory(mylib)，不证sh实际链接；活动内核options配置和两canary不证明用户ABI、标准、实际DEBUG或编译器。P2窗逐命令/实例，D00与其余新窗分别计数；证据 E150-001、E150-002、E150-003、E150-004、E150-005、E150-006、E150-007、E150-008、E150-009、E150-010、E150-011、E150-012、E150-112、E150-113、E150-114、E150-115、E150-116、E150-117、E150-118、E150-119、E150-120、E150-121、E150-122、E150-123、E150-124、E150-125、E150-126、E150-127、E150-128、E150-129、E150-130、E150-131、E150-132、E150-133、E150-134、E150-135、E150-136、E150-137、E150-013、E150-014、E150-015、E150-016、E150-017、E150-018、E150-019、E150-020、E150-021、E150-022、E150-023、E150-024、E150-025、E150-026、E150-027、E150-028、E150-029、E150-030。真实对象/映射、生产NULL、完整递归/恢复、实际运行均未证；完整本地P9、预处理、编译链接、MyOS2程序/脚本、Graphviz、QEMU、实机及真实操作NOT_RUN。不改旧件或源码。
