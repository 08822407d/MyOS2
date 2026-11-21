---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FAT-NAME-PARSE-36
record_type: bounded_short_long_name_parser_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-04
date: 2026-11-04
read_channel: connector
base_snapshot: time
source_pin_short12: a039d9803ade
result_base_short12: a7cb8bf8d2e6
contract60_short12: d5b6bf140e03
prior_review_supplements: [R35-S01, R35-S02]
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第36批｜FAT32短名/长名解析：字节缓冲、长度输出与共享游标

## P00｜第35批的限定接收、工作身份与现场

Owner当前明确要求执行固定60号`CORE-USER-FAT-NAME-PARSE-36`，读取`agent/MYOS2-LEAD-002@d5b6bf140e03`的任务书与**同一提交**第35批主线回执，后者`PASS_PENDING_LOCAL`、`returned_scope=[]`，必须合读**R35-S01与R35-S02**。本对话仍是`MYOS2-A-C04a`／`MYOS2-LEAD-002-CHAT-EXEC-04`，只是执行者；独立验收与后继规划均归`MYOS2-A-C04`，不新建执行对话。

R35-S01明确区分外层lookup上界L、整个iobuf的B及单缓冲边界。getter可在**同一次**调用中因长名/空项重复执行`next`跨越L；只有下次外层while才重新检查L。R35-S02指出旧35图的data→call、bound-false→loop-body等边不可原样继承；本批图重画真实调用者、数据来源及条件门。35对`FAT32_get_full_ent`的attr错误指针前置消费与末项覆盖依旧有界成立；本批只展开名字解析器及其直接调用者，不重做35迭代器、33快查或34 inode构造。旧R34-S01第4/5条要求先计算`max(namelen,destlen)`再转换为size_t，且tmpde、de_start、off各有不同写入时点；旧R33/R32/R31、30-DOC-FIX、UV-04/OF30/LC32及DL33问题仍不覆盖。

写前GitHub核实：固定源码`time@a039d9803ade`，options_flags.cmake和panic.c两条金丝雀均在该分支逐字命中，master不含相同片段；`master@e8e60cd19540`，PR16/lead`d5b6bf140e03`，PR17/执行分支`a7cb8bf8d2e6`。全部开放PR只有16、17；PR17 open/Draft/unmerged、执行HEAD匹配首次写前预期。PR16当前150个变更文件与本批`user-fat-name-parse-36/`目录无交集，目录尚不存在。本人35交付评论6622471344已实际取得，明确上一批STOP。现由Owner在**同一聊天**重新授予本第36批专属六件写权，不覆盖旧件。工具可写性≠额外任务授权，提交前还需二次租约检查。

MyOS2工作令、公约、协议v2 P1–P13、18/24/28号与适用Mnemosyne通用guard沿用本对话先前已实际加载的范围；本轮未再次全部刷新，不把概括的`applicable guards`当精确新读取日志。当前实读与历史继承的清单分别登记在MANIFEST；MyOS2/Owner固定任务为权限来源，Mnemosyne自身维护任务不导入。Owner选择GPT-6＋Extra，后台精确标识及作者实际型号未知，证据按`produced_by=unknown`记载。

## P01｜FAT32_parse_short：15字节分配、两组循环和终止

所选完整`mykernel/fs/fat/myos_dir.c::FAT32_parse_short`入口为`char *FAT32_parse_short(int *namelen,const msdos_dirent_s *de)`，**首先`*namelen=0`**，随后`ret_val=kzalloc(15,GFP_KERNEL)`，在该函数内没有单独拒绝NULL的检查（NP36-A001）。`kzalloc`在固定`slub.h`包装上调用`kmalloc(size, flags|__GFP_ZERO)`；它传递清零flag，不是已经实际验证底层分配成功或15字节物理上全部为零（NP36-A047）。

基名循环`for(x=0;x<8;x++)`遇`de->name[x]==' '`就break，否则按`de->lcase & LOWERCASE_BASE`是否为真选择`de->name[x]+32`或源字节原样写入`ret_val[(*namelen)++]`。目录谓词是活动宏`ATTR_DIRECTORY=(1<<4)`；目录分支跳过点号及扩展名，长度即0至8。**非目录**不论扩展名是否存在都先写`'.'`并把长度加1，再按`x=8..10`扫描扩展名、遇首个空格break；小写位`LOWERCASE_EXT=16`时逐字节`+32`。若`x==8`说明没有写入扩展名字节，代码执行`ret_val[--(*namelen)]=0`，撤销刚写的点号并把长度还原到基名长度（NP36-A002–A008，宏NP36-A030/A032）。它不是所有名字都必带点的8.3字符串重建。

在来源数组为11个可读`__u8`、分配成功、访问/整数合法的模型中：目录0..8字节，非目录最大`8+1+3=12`字节；15字节容量在成功且下层确实清零前提下保证输出后面仍有NUL。无扩展名时NUL在撤回点号的位置**由源码显式写入**，不需仅依赖剩余全零。然而成功分配之外，没有NULL门：首个基名非空格时会尝试向NULL的ret_val写；非目录即使基名为空仍会尝试写点号；目录且首字节为空格时可能**没有写入就直接返回NULL**（仅在分配失败且所有所选输入操作定义良好的helper模型内）。这些情形必须分别停在首次非法写或正常返回，不宣称同一CPU终态（NP36-F03）。

lowercase是**数值加32**，不是先检验该字节为字母才做大小写映射。有限ASCII字节模型`'A'=65 -> 'a'=97`，但若源字节`'1'=49`且base小写位为真，就会得`81='Q'`；这只说明所选源码对**某些带位标记字节**作不同变换，不证明真实目录项包含这样组合，也不能签Linux/VFAT或Unicode规范符合性（NP36-F04）。带/无扩展名、目录、满基名和扩展名、空格开头等细分见facts`NP36-C01..C08`。

## P02｜FAT32_parse_long：反向第一遍、i*13+1和字节单元投影

长名定义入口为`char *FAT32_parse_long(int *namelen,FAT32_iobuf_s *iobuf)`。它定义`int i=0,x,y`、局部`char*ret_val`与`int entlen=sizeof(msdos_dirent_s)`，将**局部**`loff_t cursor=iobuf->iter_cursor-2*entlen`，执行`lde_tmp=(msdos_dirslot_s*)FAT32_iobuf_readent(iobuf,cursor)`（NP36-A009/A010）。与短名不同，完整长名定义**没有赋`*namelen=0`**；但真实`FAT32_lookup`调用前的`int namelen=0`是已证生产者（NP36-A024），所以不能从函数内部无复位直接声称该实际调用携带非零初值。

第一遍`while`的真实求值次序：
1. 先读`lde_tmp->attr==ATTR_LONG_NAME`。
2. 若为真，再执行`!FAT32_ent_empty((msdos_dirent_s*)lde_tmp)`，内部读cast后首字节，对msdos_dirslot结构而言对应`id`所在偏移。
3. 前两项为真才计算`cursor>=0`。
4. 条件成立后`i++`，如`lde_tmp->id&0x40`则break；否则`cursor-=entlen`再调用`readent`读取上一槽（NP36-A010–A013）。

**签名/错误指针最早停止点：**readent对负off返回`ERR_PTR(-ENOENT)`（NP36-A048），但第一遍`while`先访问`lde_tmp->attr`，不能用位于表达式末尾的`cursor>=0`补作先行检查。有效长名槽若一直向前退到offset0仍无`0x40`，下一次反向readent可在负位置产生错误指针；**再次评价while时attr消费已先于cursor门**。这是条件字节序列与函数有效性前提，不是已复现磁盘损坏或内核panic（NP36-F01/F07）。若前面有合法非长槽，比较可提前为假；初次已是非长槽时`i=0`，不能无证据声称真实lookup一定会选择长解析，因其先有长名辅助槽选择门。

第一遍之后请求`ret_val=kzalloc(i*13+1,GFP_KERNEL)`，把**局部cursor**重置为`iobuf->iter_cursor-2*entlen`并重新读取`lde`，然后实际for为：
`for(x=0; x<i; x++, cursor-=entlen, lde=(msdos_dirslot_s*)FAT32_iobuf_readent(iobuf,cursor))`（NP36-A014/A015）。这里`i*13+1`按int先算后才作为size_t分配参数；需有i非负、乘法和加法不溢出、大小有效的前提。kzalloc失败未被该函数明确检查，不能把“申请了”当作已经取得可写指针。

每个合法槽的三组循环按`y=0,2,...`访问`name0_4[10]`中5个16位单元、`name5_10[12]`中6个、`name11_12[4]`中2个；若`*(u16*)(&lde->nameX[y])`不等于`0xffff`且不等于`0x0000`，执行**一次**`ret_val[(*namelen)++]=lde->nameX[y]`（第三段显式cast为char）。也就是说它对每个符合条件的16位单元只复制**首地址处一个字节**，并不是复制UTF-16的两个字节或执行Unicode转换（NP36-A016–A018）。16位指针重解释需要所选编译器/布局的对齐、别名、字节序、字段可读与宽度等前提，不能只靠packed声明推断实际链接效果。

若真实调用的初始`*namelen=0`、每槽最多13个被接受单元、i/长度与分配有效，写入计数不超过`13*i`，输出的`13*i+1`容量保留1个末尾字节；但**末尾NUL没有显式写入**，只在底层满足kzalloc清零时条件成立。更严格地，即使末尾为零，若一个被接受的16位值高位非零而首地址字节为0，源码仍可能把字节0插入**逻辑长度范围内**；这样`namelen`与C字符串停止位置不一致。字节模型下`0x0041`与`0x0141`在小端假设中均向输出复制`0x41`；`0x0100`会输出零字节，但具体真实VFAT名称是否生产这些值未证、也不作全面标准校验（NP36-F02/F08）。

**第二遍for最后一次更新不是新一轮正文：**即使在x=i-1的循环体处理完全部数据，更新表达式仍依次执行`x++`、局部`cursor-=entlen`、再次`readent(iobuf,cursor)`。然后才测试新的`x<i`，若假就结束并`return ret_val`。因此这个末次readent若在负offset处**正常返回**错误编码指针，可能仅赋给局部lde、**不再解引用其字段**；不能把“出现ERR_PTR”自动等同于第一次扫描所见的attr错误解引用或函数返回ERR_PTR（NP36-F06）。若readent内部未能正常返回、分配/参数失败在更早处使状态失效，则不可继续签最后return。

字节源和游标身份：`cursor`是长名解析函数局部变量，`iobuf->iter_cursor`是共享迭代状态；选定两个解析函数没有对共享游标或`iter_reach_end`进行**显式赋值**。它们所选直接callee`FAT32_iobuf_readent`也没有对此二字段显式赋值，不过缓存缺失时可以分配并写`iobuf->buffers[bufidx]`；宏/深层副作用不在本批认证。因而**长名倒退读取不等于迭代器倒退**，但可能带来缓存/读取副作用。所有当前结论都以有效readent返回和实际字段可读为前提（NP36-A048–A052，NP36-F06）。

## P03｜FAT32_lookup：选择、max比较、释放和下一轮游标

在`FAT32_lookup`所选完整函数内：getter先取得tmpde；进入循环体后`tmplde=FAT32_iobuf_readent(iobuf,iobuf->iter_cursor-2*entlen)`（这个辅助readent调用**先于**空项continue或后续解析选择）。随后`char*name=NULL; int namelen=0`；当`FAT32_ent_empty(tmpde)`为真会continue，未进入解析（NP36-A020–A025）。否则只有在`!IS_ERR(tmplde)`、`tmplde->attr==ATTR_LONG_NAME`及`!FAT32_ent_empty((msdos_dirent_s*)tmplde)`联合成立时选`FAT32_parse_long(&namelen,iobuf)`，其它走`FAT32_parse_short(&namelen,tmpde)`（NP36-A025/A026）。这是当前所读唯一直接分派证明，不把初始tmplde是NULL或不完整内存当安全有效值；`!IS_ERR`不足以保证其非NULL、可读、可按slot解释。

两解析器正常返回后，代码使用`int len=namelen>destlen?namelen:destlen`，将这个**最终max值**作为`strncmp(name,destname,len)`的第三参数；其实际声明类型为`size_t`（NP36-A027/A044），之后**先`kfree(name)`，再`if(equal) break`**。如果kfree未正常返回，不能声称已进入equal判断。单个输入为负并不足以证明count变成巨大无符号值：`max(-1,3)=3`、`max(-1,0)=0`；只有最终max仍负，例如`max(-1,-2)=-1`，在所选64位unsigned模型中才会转成很大count。当前实际lookup把namelen置零，未证明正常解析器负数输出，bad count的具体生产者仍helper-only，不能用它宣称真实越界（R34-S01第4条）。

固定`string.c`的`strncmp`受外部`#ifndef __HAVE_ARCH_STRNCMP`条件编译门约束；本轮没有预处理/链接证据证实最终实现选择。所读体会在count尚有剩余时逐字节比较、遇不同返回±1；遇**相等零字节**便break并返回0，即使原max>该停止位置（NP36-A043–A046）。因此只有在双方可读、内部零合法性成立、实际采用该实现的前提下，才成立“长名逻辑长度>首个NUL，而strncmp可能把后续字节忽略”的有限比较情形；不能把它升格为已打开错误文件。

至于R35-S01的跨L状态：被getter连续跳过长名/空项造成的共享游标增加是在调用本解析器**之前**通过`iobuf->next`完成；本批所选解析器只做局部逆向cursor的readent，不在选定活动体内直接推进共享iter_cursor。下一次外层while仍重读原iobuf共享游标，故R35内部跨L案例和下一次左条件为假保留的旧tmpde案例都是**其它阶段真实控制门**，不是parser自身向后/向前移动共享游标的证据。外层左条件为假时直接离开while并进行循环后的`IS_ERR(tmpde)`检查，**绝不会凭旧tmpde重新进入本轮名字解析循环体**（R35-S02）。不能从解析器是否构造了字节名推导最终file/open成功、权限或inode正确。

有类型`facts-and-boundaries.yaml`列出20组有限字节/状态模型，区分：正常短名有无扩展名、目录、满8+3、ASCII lcase、空格开头、短名分配失败；正常受限长名1槽/2槽、筛选出的单字节、内嵌零、无0x40标志、回退负offset、零槽、最后for更新中未消费的ERR_PTR、长名缓冲失败与max有符号转换。所有helper-only输入均明示实际上游未证，字段数据不隐含真实磁盘或Unicode合法性。

## P04｜交付与证据级别

本批仅新增六件：`MANIFEST.md`、本文、`facts-and-boundaries.yaml`（JSON兼容YAML）、`fat-name-parse-paths.dot`、`deferred-findings.yaml`及`evidence.md`。56段独立固定源码短引文来自9份直接源码/定义文件；每段单定义体1–5行，且没有复用35证据数量。图将真正的lookup分派、两个解析器不同的长度初始化、两遍long循环、最后一次for更新、共享字段、额外前提和停止点分开，不把数据对象误写成调用方或把越界退出错误指向loop正文。NP36-F01至F08为**有前提的延期候选**而非8个已复现独立漏洞；其相互依赖、反证、旧ID限定与恢复条件见deferred。

**NOT_RUN：**仓库C/ASM、脚本、预处理/编译、QEMU/硬件、真实FAT磁盘或完整VFAT编码/Unicode规范校验、完整分配器/簇链/设备I/O、权限/挂载/锁/引用/物理回收、额外付费模型/API。不能从工具读源和有限字节模型宣布内核已通过运行、用户文件正确查找或实际故障。无内核/用户程序/旧成果修改、补丁、lead/PR16/master/time或PR17标题/body/Draft/他人评论改动，不创建分支/PR、不强推合并或进入阶段3。

只申请`READY_FOR_MAINLINE_REVIEW`，`mainline_acceptance=PENDING`；最终六件若能按预期HEAD非强推提交，将从新固定提交和活动PR17分支分别全文回读正文、blob及HEAD，并将**实际**GitHub提交号和核验结论记录在本执行者本人PR17评论。正式交付后`STOP_WRITING`，交C04独立验收，不自行开始37。
