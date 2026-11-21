---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-OPEN-FLAGS-30-DOC-FIX-01
record_type: additive_documentation_and_evidence_erratum
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
date: 2026-11-03
read_channel: connector
source_ref: time
source_pin_short12: a039d9803ade
task53_short12: 7fb723be2344
original_result_short12: da292d44b40c
canonical_branch: claude/dazzling-cori-q0dnyt
supersedes: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-open-flags-30/evidence.md
supersedes_scope: "仅原30批的证据标签资格、定义补证及原六件中受影响的同义描述和DOT边注；非原30技术成果或历史其他任务全量替换"
parent_review_record: CORE-USER-OPEN-FLAGS-30-REVIEW-001
parent_review_verdict: RETURN
returned_scope: [R30-01, R30-02]
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---
# 第30批定点勘误：80个旧证据ID资格与跨件表述

## 适用范围与结论位置

本文件是53号固定任务指定的**追加式消费勘误**，不会修改原 `user-open-flags-30/` 的六件、旧PR17评论或其中的历史自查数字。正式30验收结果仍为 **RETURN（R30-01、R30-02待C04独立复核闭合）**。本执行者只能提交READY_FOR_MAINLINE_REVIEW，不能自行将RETURN改为PASS_PENDING_LOCAL。

历史原80引文在固定time源码中**逐字都能匹配**，不意味着原80个单标签都符合协议P2的“单定义体、支持题目”要求。本轮依53号逐项重核，分类与新增直接证据严格分层：原旧ID处置为**47个保留（已重核）、31个修正、2个修正为声明/包含链导航**，保留（未重核）0、撤回0；新补短引文**75段**，其新标签在本文件另行计数，不把原80混算成75或宣称原80全部严格合格。表中的“保留”仅针对片段归属，相关内核后果依原报告及主线回执继续有条件。

## R30-01｜原80项逐一处置，标签资格与替代关系

旧ID对应原文件`user-open-flags-30/evidence.md`，来源分支`time@a039d9803ade`；表中每一个ID只出现一次。修正项的新直接短引文在本文后面的OF30-X001–X075定义体登记中。导航项不能再作为函数或宏定义体的VERIFIED证据消费，其实际调用语义另有本轮直接引文。

| 原证据ID | 处置 | 原path::symbol（原行段） | 精确原因／保留限定 | 替代证据或定位 |
|---|---|---|---|---|
| OF30-A001 | 修正 | `mykernel/fs/vfs/open.c::WILL_CREATE`（23–24） | WILL_CREATE与O_PATH_FLAGS为两个独立宏；原单标签一概覆盖不能通过定义归属 | OF30-X001、OF30-X002 |
| OF30-A002 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_how`（27–31） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A003 | 修正 | `mykernel/fs/vfs/open.c::build_open_how`（29–32） | “复合字面量”改为局部聚合变量的指定初始化；省略resolve按C聚合规则置零 | OF30-X003 |
| OF30-A004 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_how`（34–36） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A005 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_how`（37–40） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A006 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（68–72） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A007 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（79–79） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A008 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（86–89） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A009 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（91–93） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A010 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（95–99） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A011 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（99–103） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A012 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（112–116） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A013 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（118–122） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A014 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（131–134） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A015 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（136–138） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A016 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（140–143） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A017 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（145–147） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A018 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（149–153） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A019 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（151–155） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A020 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（157–160） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A021 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（162–165） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A022 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（166–169） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A023 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（170–171） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A024 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（172–176） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A025 | 保留（已重核） | `mykernel/fs/vfs/open.c::build_open_flags`（176–180） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A026 | 保留（已重核） | `mykernel/fs/vfs/open.c::file_open_name`（197–201） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A027 | 保留（已重核） | `mykernel/fs/vfs/open.c::file_open_name`（200–204） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A028 | 保留（已重核） | `mykernel/fs/vfs/open.c::filp_open`（220–224） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A029 | 保留（已重核） | `mykernel/fs/vfs/open.c::filp_open`（223–227） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A030 | 保留（已重核） | `mykernel/fs/vfs/open.c::do_sys_openat2`（241–245） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A031 | 保留（已重核） | `mykernel/fs/vfs/open.c::do_sys_openat2`（247–249） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A032 | 保留（已重核） | `mykernel/fs/vfs/open.c::do_sys_openat2`（251–254） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A033 | 保留（已重核） | `mykernel/fs/vfs/open.c::do_sys_openat2`（254–258） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A034 | 保留（已重核） | `mykernel/fs/vfs/open.c::do_sys_openat2`（258–262） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A035 | 保留（已重核） | `mykernel/fs/vfs/open.c::do_sys_open`（266–270） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A036 | 修正 | `mykernel/fs/syscall.c::sys_open`（37–41） | 原片段含函数定义外的__NR_open注释，改用真实MYOS_SYSCALL_DEFINE3(open)宏实例与return | OF30-X004 |
| OF30-A037 | 修正 | `mykernel/include/linux/fs/internal.h::open_flags_s`（91–95） | open_flags_s原片段只到intent，新增真正含lookup_flags的结构体续段 | OF30-X005、OF30-X006 |
| OF30-A038 | 修正 | `mykernel/include/linux/fs/internal.h::build_open_how`（98–102） | 原片段是internal.h的extern声明并混入其它声明，仅保留签名／导航用途；定义体改用open.c及namei.c真实函数锚点 | 导航，另合读OF30-X064–X075 |
| OF30-A039 | 修正 | `mykernel/include/linux/kernel/fcntl.h::VALID_OPEN_FLAGS`（8–12） | 原片段首行是定义外说明，改用VALID_OPEN_FLAGS同一长宏内部段 | OF30-X007 |
| OF30-A040 | 保留（已重核） | `mykernel/include/linux/kernel/fcntl.h::VALID_OPEN_FLAGS`（12–16） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A041 | 修正 | `mykernel/include/linux/kernel/fcntl.h::VALID_RESOLVE_FLAGS`（18–22） | 原片段首行是定义外说明，改用VALID_RESOLVE_FLAGS宏内部 | OF30-X008 |
| OF30-A042 | 修正 | `mykernel/include/uapi/asm-generic/fcntl.h::O_ACCMODE`（19–22） | O_ACCMODE/RDONLY/WRONLY/RDWR是四个不同宏，各自锚定 | OF30-X009、OF30-X010、OF30-X011、OF30-X012 |
| OF30-A043 | 修正 | `mykernel/include/uapi/asm-generic/fcntl.h::O_CREAT`（23–27） | 删除#ifndef等外围；O_CREAT和O_EXCL独立 | OF30-X013、OF30-X014 |
| OF30-A044 | 修正 | `mykernel/include/uapi/asm-generic/fcntl.h::O_TRUNC`（32–36） | 删除#ifndef等外围；O_TRUNC和O_APPEND独立 | OF30-X015、OF30-X016 |
| OF30-A045 | 修正 | `mykernel/include/uapi/asm-generic/fcntl.h::O_DSYNC`（41–42） | 剔除ifndef外围，保留O_DSYNC定义 | OF30-X017 |
| OF30-A046 | 修正 | `mykernel/include/uapi/asm-generic/fcntl.h::O_LARGEFILE`（50–54） | O_LARGEFILE和O_DIRECTORY分开标注，删除条件外围 | OF30-X018、OF30-X019 |
| OF30-A047 | 修正 | `mykernel/include/uapi/asm-generic/fcntl.h::O_NOFOLLOW`（56–57） | 只取O_NOFOLLOW真实定义行 | OF30-X020 |
| OF30-A048 | 修正 | `mykernel/include/uapi/asm-generic/fcntl.h::O_CLOEXEC`（62–63） | 只取O_CLOEXEC真实定义行 | OF30-X021 |
| OF30-A049 | 修正 | `mykernel/include/uapi/asm-generic/fcntl.h::O_SYNC`（79–82） | __O_SYNC和O_SYNC是不同宏定义，分别锚定 | OF30-X022、OF30-X023 |
| OF30-A050 | 修正 | `mykernel/include/uapi/asm-generic/fcntl.h::O_PATH`（84–85） | 只取O_PATH真实定义行 | OF30-X024 |
| OF30-A051 | 修正 | `mykernel/include/uapi/asm-generic/fcntl.h::O_TMPFILE`（90–94） | 去掉上一宏endif与说明，分别补__O_TMPFILE数值、O_TMPFILE组合及O_TMPFILE_MASK组合 | OF30-X025、OF30-X026、OF30-X027 |
| OF30-A052 | 修正 | `mykernel/include/uapi/asm-generic/fcntl.h::O_NDELAY`（96–98） | O_NDELAY指向O_NONBLOCK的同值别名，分别锚定 | OF30-X028、OF30-X029 |
| OF30-A053 | 保留（已重核） | `mykernel/include/uapi/linux/openat2.h::open_how_s`（19–23） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A054 | 修正 | `mykernel/include/uapi/linux/openat2.h::RESOLVE_NO_XDEV`（25–29） | RESOLVE_NO_XDEV和RESOLVE_NO_MAGICLINKS分开；去掉定义外标题 | OF30-X030、OF30-X031 |
| OF30-A055 | 修正 | `mykernel/include/uapi/linux/openat2.h::RESOLVE_NO_SYMLINKS`（30–34） | RESOLVE_NO_SYMLINKS和RESOLVE_BENEATH分开 | OF30-X032、OF30-X033 |
| OF30-A056 | 修正 | `mykernel/include/uapi/linux/openat2.h::RESOLVE_IN_ROOT`（35–38） | RESOLVE_IN_ROOT和RESOLVE_CACHED是两宏，分别锚定并补齐值 | OF30-X034、OF30-X035 |
| OF30-A057 | 保留（已重核） | `mykernel/include/uapi/linux/openat2.h::RESOLVE_CACHED`（38–41） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A058 | 修正 | `mykernel/include/uapi/linux/fcntl.h::fcntl`（5–6） | 原片段是uapi/linux/fcntl.h的两条#include，仅作包含链导航，不是名为fcntl的宏或函数定义 | 导航，另合读OF30-X064–X075 |
| OF30-A059 | 修正 | `mykernel/include/linux/kernel/stat.h::S_IALLUGO`（9–10） | S_IRWXUGO与S_IALLUGO各有独立宏；后者的组成另由特殊位和rwx位补证 | OF30-X036、OF30-X037 |
| OF30-A060 | 修正 | `mykernel/include/uapi/linux/stat.h::S_IFREG`（9–13） | S_IFREG从相邻文件类型宏中分离；补S_ISUID/S_ISGID/S_ISVTX | OF30-X038、OF30-X039、OF30-X040、OF30-X041 |
| OF30-A061 | 修正 | `mykernel/include/uapi/linux/stat.h::S_IRWXG`（29–33） | S_IRWXU与S_IRWXG拆分，避免用户位与组位同标签 | OF30-X042、OF30-X043 |
| OF30-A062 | 修正 | `mykernel/include/uapi/linux/stat.h::S_IRWXO`（36–39） | S_IRWXO定义单独定位，不带其它宏 | OF30-X044 |
| OF30-A063 | 修正 | `mykernel/include/linux/fs/fs.h::MAY_READ`（44–48） | MAY_EXEC/MAY_WRITE/MAY_READ/MAY_APPEND拆成四个定义 | OF30-X045、OF30-X046、OF30-X047、OF30-X048 |
| OF30-A064 | 修正 | `mykernel/include/linux/fs/fs.h::FMODE_NONOTIFY`（115–118） | FMODE_NONOTIFY保留单宏定义行，删除前置注释 | OF30-X049 |
| OF30-A065 | 保留（已重核） | `mykernel/include/linux/fs/fs.h::ACC_MODE`（3424–3424） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A066 | 修正 | `mykernel/include/linux/fs/namei.h::LOOKUP_FOLLOW`（38–42） | LOOKUP_FOLLOW/DIRECTORY/EMPTY各自定义，去掉函数外导航文字 | OF30-X050、OF30-X051、OF30-X052 |
| OF30-A067 | 修正 | `mykernel/include/linux/fs/namei.h::LOOKUP_OPEN`（49–52） | LOOKUP_OPEN/CREATE/EXCL独立定义，去掉前置说明 | OF30-X053、OF30-X054、OF30-X055 |
| OF30-A068 | 修正 | `mykernel/include/linux/fs/namei.h::LOOKUP_NO_SYMLINKS`（58–62） | LOOKUP_NO_SYMLINKS/MAGICLINKS/XDEV/BENEATH/IN_ROOT/CACHED逐个定义，后两项由新增证据补齐 | OF30-X056、OF30-X057、OF30-X058、OF30-X059、OF30-X060、OF30-X061 |
| OF30-A069 | 保留（已重核） | `mykernel/fs/vfs/namei.c::do_filp_open`（753–757） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A070 | 保留（已重核） | `mykernel/fs/vfs/namei.c::do_filp_open`（759–763） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A071 | 保留（已重核） | `mykernel/fs/vfs/namei.c::path_openat`（715–719） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A072 | 保留（已重核） | `mykernel/fs/vfs/namei.c::path_openat`（723–727） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A073 | 保留（已重核） | `mykernel/fs/vfs/namei.c::do_open`（703–707） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A074 | 保留（已重核） | `mykernel/sched/forkexec/exec.c::do_open_execat`（554–558） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A075 | 保留（已重核） | `mykernel/sched/forkexec/exec.c::do_open_execat`（559–563） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A076 | 修正 | `mykernel/sched/forkexec/exec.c::do_open_execat`（565–569） | 旧片段止于if(AT_EMPTY_PATH)，缺少真正LOOKUP_EMPTY赋值，补齐赋值行 | OF30-X062 |
| OF30-A077 | 保留（已重核） | `mykernel/sched/forkexec/exec.c::do_open_execat`（572–574） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A078 | 保留（已重核） | `mykernel/sched/forkexec/exec.c::open_exec`（606–610） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A079 | 保留（已重核） | `mykernel/sched/forkexec/exec.c::open_exec`（609–613） | 固定源码逐字重核；该行段属于所标同一函数／宏／结构体定义范围；维持原语义限定 | 原ID继续受主报告条件限制 |
| OF30-A080 | 修正 | `mykernel/include/linux/kernel/types.h::umode_t`（32–36） | umode_t是独立typedef，不把邻近dev_t/ino_t/mode_t/nlink_t称作同一符号证据 | OF30-X063 |

### 仍可按旧ID消费的代表范围

- `OF30-A002/A004–A035`中没有列入“修正”的项，均处于所标`open.c`函数定义的真实范围，语义仍限实际代码门；同一函数体里的普通注释不因是注释就整体判出定义外。
- `OF30-A040`是同一`VALID_OPEN_FLAGS`长宏的后半定义段；须与本次`OF30-X007`及宏完整定义合读，不能拆成新独立flag。`OF30-A053/A057/A065`分别是完整结构体、`RESOLVE_CACHED`宏和单行`ACC_MODE`定义证据。
- `OF30-A069–A075/A077–A079`中未标修正的项仍在实际选定函数体，支持调用及返回边界；未认证完整namei/文件系统成功或CPU状态。原`OF30-A076`仅验证条件前缀，`LOOKUP_EMPTY`的真实赋值须使用本次`OF30-X062`。

## R30-01｜替代和强制直接源码证据（75段）

以下每段都是从同一固定time的真实定义体截取，含原始缩进、空行与宏续行；每段1–5行。一个标签只指一项函数/宏/typedef/结构体；同一长宏可拆为多个连续片段。标记本身不表示整个C程序已运行或文件打开成功。**新片段75/75均按所选path、行段及实际定义区间二次回源核对**；该计数属于本勘误而非原30件当时的自查。

### OF30-X001 · 替代OF30-A001：WILL_CREATE

[VERIFIED mykernel/fs/vfs/open.c::WILL_CREATE]
```c
#define WILL_CREATE(flags)	(flags & (O_CREAT | __O_TMPFILE))
```
<small>固定 `time` 第23–23行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X002 · 替代OF30-A001：O_PATH_FLAGS

[VERIFIED mykernel/fs/vfs/open.c::O_PATH_FLAGS]
```c
#define O_PATH_FLAGS		(O_DIRECTORY | O_NOFOLLOW | O_PATH | O_CLOEXEC)
```
<small>固定 `time` 第24–24行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X003 · 替代OF30-A003：build_open_how

[VERIFIED mykernel/fs/vfs/open.c::build_open_how]
```c
	struct open_how how = {
		.flags = flags & VALID_OPEN_FLAGS,
		.mode = mode & S_IALLUGO,
	};
```
<small>固定 `time` 第29–32行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X004 · 替代OF30-A036：sys_open

[VERIFIED mykernel/fs/syscall.c::sys_open]
```c
MYOS_SYSCALL_DEFINE3(open, const char *, filename,
		int, flags, umode_t, mode)
{
	return do_sys_open(0, filename, flags, mode);
```
<small>固定 `time` 第38–41行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X005 · 替代OF30-A037：open_flags_s

[VERIFIED mykernel/include/linux/fs/internal.h::open_flags_s]
```c
typedef struct open_flags {
	int open_flag;
	umode_t mode;
	int acc_mode;
	int intent;
```
<small>固定 `time` 第91–95行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X006 · 替代OF30-A037：open_flags_s

[VERIFIED mykernel/include/linux/fs/internal.h::open_flags_s]
```c
	int lookup_flags;
} open_flags_s;
```
<small>固定 `time` 第96–97行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X007 · 替代OF30-A039：VALID_OPEN_FLAGS

[VERIFIED mykernel/include/linux/kernel/fcntl.h::VALID_OPEN_FLAGS]
```c
	#define VALID_OPEN_FLAGS (											\
				O_RDONLY | O_WRONLY | O_RDWR | O_CREAT |				\
				O_EXCL | O_NOCTTY | O_TRUNC |							\
				O_APPEND | O_NDELAY | O_NONBLOCK | __O_SYNC | O_DSYNC | \
				FASYNC	| O_DIRECT | O_LARGEFILE |						\
```
<small>固定 `time` 第9–13行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X008 · 替代OF30-A041：VALID_RESOLVE_FLAGS

[VERIFIED mykernel/include/linux/kernel/fcntl.h::VALID_RESOLVE_FLAGS]
```c
	#define VALID_RESOLVE_FLAGS (							\
				RESOLVE_NO_XDEV | RESOLVE_NO_MAGICLINKS |	\
				RESOLVE_NO_SYMLINKS | RESOLVE_BENEATH |		\
				RESOLVE_IN_ROOT | RESOLVE_CACHED			\
			)
```
<small>固定 `time` 第19–23行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X009 · 替代OF30-A042：O_ACCMODE

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_ACCMODE]
```c
	#define O_ACCMODE			00000003
```
<small>固定 `time` 第19–19行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X010 · 替代OF30-A042：O_RDONLY

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_RDONLY]
```c
	#define O_RDONLY			00000000
```
<small>固定 `time` 第20–20行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X011 · 替代OF30-A042：O_WRONLY

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_WRONLY]
```c
	#define O_WRONLY			00000001
```
<small>固定 `time` 第21–21行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X012 · 替代OF30-A042：O_RDWR

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_RDWR]
```c
	#define O_RDWR				00000002
```
<small>固定 `time` 第22–22行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X013 · 替代OF30-A043：O_CREAT

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_CREAT]
```c
	#	define O_CREAT			00000100	/* not fcntl */
```
<small>固定 `time` 第24–24行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X014 · 替代OF30-A043：O_EXCL

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_EXCL]
```c
	#	define O_EXCL			00000200	/* not fcntl */
```
<small>固定 `time` 第27–27行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X015 · 替代OF30-A044：O_TRUNC

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_TRUNC]
```c
	#	define O_TRUNC			00001000	/* not fcntl */
```
<small>固定 `time` 第33–33行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X016 · 替代OF30-A044：O_APPEND

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_APPEND]
```c
	#	define O_APPEND			00002000
```
<small>固定 `time` 第36–36行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X017 · 替代OF30-A045：O_DSYNC

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_DSYNC]
```c
	#	define O_DSYNC			00010000	/* used to be O_SYNC, see below */
```
<small>固定 `time` 第42–42行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X018 · 替代OF30-A046：O_LARGEFILE

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_LARGEFILE]
```c
	#	define O_LARGEFILE		00100000
```
<small>固定 `time` 第51–51行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X019 · 替代OF30-A046：O_DIRECTORY

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_DIRECTORY]
```c
	#	define O_DIRECTORY		00200000	/* must be a directory */
```
<small>固定 `time` 第54–54行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X020 · 替代OF30-A047：O_NOFOLLOW

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_NOFOLLOW]
```c
	#	define O_NOFOLLOW		00400000	/* don't follow links */
```
<small>固定 `time` 第57–57行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X021 · 替代OF30-A048：O_CLOEXEC

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_CLOEXEC]
```c
	#	define O_CLOEXEC		02000000	/* set close_on_exec */
```
<small>固定 `time` 第63–63行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X022 · 替代OF30-A049：__O_SYNC

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::__O_SYNC]
```c
	#	define __O_SYNC			04000000
```
<small>固定 `time` 第80–80行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X023 · 替代OF30-A049：O_SYNC

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_SYNC]
```c
	#	define O_SYNC			(__O_SYNC|O_DSYNC)
```
<small>固定 `time` 第81–81行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X024 · 替代OF30-A050：O_PATH

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_PATH]
```c
	#	define O_PATH			010000000
```
<small>固定 `time` 第85–85行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X025 · 替代OF30-A051：__O_TMPFILE

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::__O_TMPFILE]
```c
	#	define __O_TMPFILE		020000000
```
<small>固定 `time` 第89–89行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X026 · 替代OF30-A051：O_TMPFILE

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_TMPFILE]
```c
	#define O_TMPFILE			(__O_TMPFILE | O_DIRECTORY)
```
<small>固定 `time` 第93–93行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X027 · 替代OF30-A051：O_TMPFILE_MASK

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_TMPFILE_MASK]
```c
	#define O_TMPFILE_MASK		(__O_TMPFILE | O_DIRECTORY | O_CREAT)      
```
<small>固定 `time` 第94–94行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X028 · 替代OF30-A052：O_NDELAY

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_NDELAY]
```c
	#	define O_NDELAY			O_NONBLOCK
```
<small>固定 `time` 第97–97行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X029 · 替代OF30-A052：O_NONBLOCK

[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_NONBLOCK]
```c
	#	define O_NONBLOCK		00004000
```
<small>固定 `time` 第39–39行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X030 · 替代OF30-A054：RESOLVE_NO_XDEV

[VERIFIED mykernel/include/uapi/linux/openat2.h::RESOLVE_NO_XDEV]
```c
	#define RESOLVE_NO_XDEV			0x01 /* Block mount-point crossings
											(includes bind-mounts). */
```
<small>固定 `time` 第26–27行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X031 · 替代OF30-A054：RESOLVE_NO_MAGICLINKS

[VERIFIED mykernel/include/uapi/linux/openat2.h::RESOLVE_NO_MAGICLINKS]
```c
	#define RESOLVE_NO_MAGICLINKS	0x02 /* Block traversal through procfs-style
											"magic-links". */
```
<small>固定 `time` 第28–29行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X032 · 替代OF30-A055：RESOLVE_NO_SYMLINKS

[VERIFIED mykernel/include/uapi/linux/openat2.h::RESOLVE_NO_SYMLINKS]
```c
	#define RESOLVE_NO_SYMLINKS		0x04 /* Block traversal through all symlinks
											(implies OEXT_NO_MAGICLINKS) */
```
<small>固定 `time` 第30–31行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X033 · 替代OF30-A055：RESOLVE_BENEATH

[VERIFIED mykernel/include/uapi/linux/openat2.h::RESOLVE_BENEATH]
```c
	#define RESOLVE_BENEATH			0x08 /* Block "lexical" trickery like
											"..", symlinks, and absolute
											paths which escape the dirfd. */
```
<small>固定 `time` 第32–34行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X034 · 替代OF30-A056：RESOLVE_IN_ROOT

[VERIFIED mykernel/include/uapi/linux/openat2.h::RESOLVE_IN_ROOT]
```c
	#define RESOLVE_IN_ROOT			0x10 /* Make all jumps to "/" and ".."
											be scoped inside the dirfd
											(similar to chroot(2)). */
```
<small>固定 `time` 第35–37行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X035 · 替代OF30-A056：RESOLVE_CACHED

[VERIFIED mykernel/include/uapi/linux/openat2.h::RESOLVE_CACHED]
```c
	#define RESOLVE_CACHED			0x20 /* Only complete if resolution can be
											completed through cached lookup. May
											return -EAGAIN if that's not
											possible. */
```
<small>固定 `time` 第38–41行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X036 · 替代OF30-A059：S_IRWXUGO

[VERIFIED mykernel/include/linux/kernel/stat.h::S_IRWXUGO]
```c
	#define S_IRWXUGO	(S_IRWXU|S_IRWXG|S_IRWXO)
```
<small>固定 `time` 第9–9行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X037 · 替代OF30-A059：S_IALLUGO

[VERIFIED mykernel/include/linux/kernel/stat.h::S_IALLUGO]
```c
	#define S_IALLUGO	(S_ISUID|S_ISGID|S_ISVTX|S_IRWXUGO)
```
<small>固定 `time` 第10–10行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X038 · 替代OF30-A060：S_IFREG

[VERIFIED mykernel/include/uapi/linux/stat.h::S_IFREG]
```c
	#	define S_IFREG		0100000		// 1--- ---- ---- ---- b
```
<small>固定 `time` 第11–11行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X039 · 替代OF30-A060：S_ISUID

[VERIFIED mykernel/include/uapi/linux/stat.h::S_ISUID]
```c
	#	define S_ISUID		0004000		// ---- 1--- ---- ---- b
```
<small>固定 `time` 第16–16行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X040 · 替代OF30-A060：S_ISGID

[VERIFIED mykernel/include/uapi/linux/stat.h::S_ISGID]
```c
	#	define S_ISGID		0002000		// ---- -1-- ---- ---- b
```
<small>固定 `time` 第17–17行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X041 · 替代OF30-A060：S_ISVTX

[VERIFIED mykernel/include/uapi/linux/stat.h::S_ISVTX]
```c
	#	define S_ISVTX		0001000		// ---- --1- ---- ---- b
```
<small>固定 `time` 第18–18行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X042 · 替代OF30-A061：S_IRWXU

[VERIFIED mykernel/include/uapi/linux/stat.h::S_IRWXU]
```c
	#	define S_IRWXU		00700		// ---- ---1 11-- ---- b
```
<small>固定 `time` 第28–28行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X043 · 替代OF30-A061：S_IRWXG

[VERIFIED mykernel/include/uapi/linux/stat.h::S_IRWXG]
```c
	#	define S_IRWXG		00070		// ---- ---- --11 1--- b
```
<small>固定 `time` 第33–33行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X044 · 替代OF30-A062：S_IRWXO

[VERIFIED mykernel/include/uapi/linux/stat.h::S_IRWXO]
```c
	#	define S_IRWXO		00007		// ---- ---- ---- -111 b
```
<small>固定 `time` 第38–38行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X045 · 替代OF30-A063：MAY_EXEC

[VERIFIED mykernel/include/linux/fs/fs.h::MAY_EXEC]
```c
	#define MAY_EXEC		0x00000001
```
<small>固定 `time` 第45–45行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X046 · 替代OF30-A063：MAY_WRITE

[VERIFIED mykernel/include/linux/fs/fs.h::MAY_WRITE]
```c
	#define MAY_WRITE		0x00000002
```
<small>固定 `time` 第46–46行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X047 · 替代OF30-A063：MAY_READ

[VERIFIED mykernel/include/linux/fs/fs.h::MAY_READ]
```c
	#define MAY_READ		0x00000004
```
<small>固定 `time` 第47–47行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X048 · 替代OF30-A063：MAY_APPEND

[VERIFIED mykernel/include/linux/fs/fs.h::MAY_APPEND]
```c
	#define MAY_APPEND		0x00000008
```
<small>固定 `time` 第48–48行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X049 · 替代OF30-A064：FMODE_NONOTIFY

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_NONOTIFY]
```c
	#define FMODE_NONOTIFY			((__force fmode_t)0x4000000)
```
<small>固定 `time` 第117–117行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X050 · 替代OF30-A066：LOOKUP_FOLLOW

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_FOLLOW]
```c
	#define LOOKUP_FOLLOW			0x0001	/* follow links at the end */
```
<small>固定 `time` 第39–39行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X051 · 替代OF30-A066：LOOKUP_DIRECTORY

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_DIRECTORY]
```c
	#define LOOKUP_DIRECTORY		0x0002	/* require a directory */
```
<small>固定 `time` 第40–40行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X052 · 替代OF30-A066：LOOKUP_EMPTY

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_EMPTY]
```c
	#define LOOKUP_EMPTY			0x4000	/* accept empty path [user_... only] */
```
<small>固定 `time` 第42–42行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X053 · 替代OF30-A067：LOOKUP_OPEN

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_OPEN]
```c
	#define LOOKUP_OPEN				0x0100	/* ... in open */
```
<small>固定 `time` 第50–50行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X054 · 替代OF30-A067：LOOKUP_CREATE

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_CREATE]
```c
	#define LOOKUP_CREATE			0x0200	/* ... in object creation */
```
<small>固定 `time` 第51–51行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X055 · 替代OF30-A067：LOOKUP_EXCL

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_EXCL]
```c
	#define LOOKUP_EXCL				0x0400	/* ... in exclusive creation */
```
<small>固定 `time` 第52–52行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X056 · 替代OF30-A068：LOOKUP_NO_SYMLINKS

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_NO_SYMLINKS]
```c
	#define LOOKUP_NO_SYMLINKS		0x010000 /* No symlink crossing. */
```
<small>固定 `time` 第59–59行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X057 · 替代OF30-A068：LOOKUP_NO_MAGICLINKS

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_NO_MAGICLINKS]
```c
	#define LOOKUP_NO_MAGICLINKS	0x020000 /* No nd_jump_link() crossing. */
```
<small>固定 `time` 第60–60行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X058 · 替代OF30-A068：LOOKUP_NO_XDEV

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_NO_XDEV]
```c
	#define LOOKUP_NO_XDEV			0x040000 /* No mountpoint crossing. */
```
<small>固定 `time` 第61–61行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X059 · 替代OF30-A068：LOOKUP_BENEATH

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_BENEATH]
```c
	#define LOOKUP_BENEATH			0x080000 /* No escaping from starting point. */
```
<small>固定 `time` 第62–62行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X060 · 替代OF30-A068：LOOKUP_IN_ROOT

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_IN_ROOT]
```c
	#define LOOKUP_IN_ROOT			0x100000 /* Treat dirfd as fs root. */
```
<small>固定 `time` 第63–63行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X061 · 替代OF30-A068：LOOKUP_CACHED

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_CACHED]
```c
	#define LOOKUP_CACHED			0x200000 /* Only do cached lookup */
```
<small>固定 `time` 第64–64行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X062 · 替代OF30-A076：do_open_execat

[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	if (flags & AT_EMPTY_PATH)
		open_exec_flags.lookup_flags |= LOOKUP_EMPTY;
```
<small>固定 `time` 第569–570行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X063 · 替代OF30-A080：umode_t

[VERIFIED mykernel/include/linux/kernel/types.h::umode_t]
```c
		typedef unsigned int		umode_t;
```
<small>固定 `time` 第35–35行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X064 · 强制直接消费语句：do_sys_openat2

[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	open_flags_s op;
	int fd = build_open_flags(how, &op);
	filename_s *tmp;
```
<small>固定 `time` 第243–245行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X065 · 强制直接消费语句：do_sys_openat2

[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	tmp = getname(filename);
	if (IS_ERR(tmp))
		return PTR_ERR(tmp);
```
<small>固定 `time` 第247–249行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X066 · 强制直接消费语句：do_sys_openat2

[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	fd = get_unused_fd_flags(how->flags);
	if (fd >=0)
	{
		file_s * f = do_filp_open(dfd, tmp, &op);
```
<small>固定 `time` 第251–254行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X067 · 强制直接消费语句：build_open_flags

[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (flags & __O_TMPFILE) {
		if ((flags & O_TMPFILE_MASK) != O_TMPFILE)
			return -EINVAL;
		if (!(acc_mode & MAY_WRITE))
			return -EINVAL;
```
<small>固定 `time` 第112–116行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X068 · 强制直接消费语句：build_open_flags

[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (WILL_CREATE(flags)) {
		if (how->mode & ~S_IALLUGO)
			return -EINVAL;
		op->mode = how->mode | S_IFREG;
```
<small>固定 `time` 第96–99行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X069 · 强制直接消费语句：build_open_flags

[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	} else {
		if (how->mode != 0)
			return -EINVAL;
		op->mode = 0;
	}
```
<small>固定 `time` 第100–104行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X070 · 强制直接消费语句：build_open_flags

[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	op->open_flag = flags;
```
<small>固定 `time` 第134–134行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X071 · 强制直接消费语句：build_open_flags

[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	op->lookup_flags = lookup_flags;
	return 0;
```
<small>固定 `time` 第179–180行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X072 · 强制直接消费语句：file_open_name

[VERIFIED mykernel/fs/vfs/open.c::file_open_name]
```c
	open_how_s how = build_open_how(flags, mode);
	int err = build_open_flags(&how, &op);
	if (err)
		return ERR_PTR(err);
	return do_filp_open(AT_FDCWD, name, &op);
```
<small>固定 `time` 第200–204行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X073 · 强制直接消费语句：do_filp_open

[VERIFIED mykernel/fs/vfs/namei.c::do_filp_open]
```c
file_s *do_filp_open(int dfd, IN filename_s * name, const open_flags_s *op)
{
	nameidata_s nd;
	int flags = op->lookup_flags;
	file_s *filp;
```
<small>固定 `time` 第753–757行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X074 · 强制直接消费语句：build_open_how

[VERIFIED mykernel/fs/vfs/open.c::build_open_how]
```c
open_how_s build_open_how(int flags, umode_t mode)
{
	struct open_how how = {
		.flags = flags & VALID_OPEN_FLAGS,
		.mode = mode & S_IALLUGO,
```
<small>固定 `time` 第27–31行；本片段只指所列定义体，不由标题补出其它源代码。</small>

### OF30-X075 · 强制直接消费语句：open_how_s

[VERIFIED mykernel/include/uapi/linux/openat2.h::open_how_s]
```c
	typedef struct open_how {
		__u64 flags;
		__u64 mode;
		__u64 resolve;
	} open_how_s;
```
<small>固定 `time` 第19–23行；本片段只指所列定义体，不由标题补出其它源代码。</small>


### 强制证据闭合检查（不得以声明或include代替）

| 核查要求 | 可直接消费的新锚点 | 原证据补充 |
|---|---|---|
| `do_sys_openat2`第一次fd赋值、getname门、第二次fd赋值及条件下游 | OF30-X064/X065/X066 | 原A030–A034 |
| `build_open_flags` TMPFILE门、两种mode分支、open_flag写入、最后lookup_flags写入及return | OF30-X067/X068/X069/X070/X071 | 原A010–A014/A024–A025 |
| `file_open_name`检查err后立即返错或继续调用 | OF30-X072 | 原A027 |
| `do_filp_open`从op读取lookup_flags | OF30-X073 | 原A069 |
| `open_flags_s`五个字段，尤其末尾lookup_flags | OF30-X005/X006 | 原A037只到intent |
| `__O_TMPFILE`数值、`O_DIRECTORY`数值、`O_TMPFILE`和`O_TMPFILE_MASK`组成 | OF30-X025/X019/X026/X027 | 原A046/A051不够 |
| `S_IALLUGO`与S_ISUID/S_ISGID/S_ISVTX及S_IRWXU/G/O | OF30-X036/X037/X039–X044 | 原A059–A062不够 |
| `LOOKUP_IN_ROOT`、`LOOKUP_CACHED`定义及`LOOKUP_EMPTY`赋值 | OF30-X060/X061/X052/X062 | 原A056/A068/A076缺尾 |
| 旧入口局部聚合指定初始化与`open_how_s`三字段 | OF30-X003/X074/X075 | 原A003用词不准 |
| `O_PATH`、`O_CREAT`、`VALID_OPEN_FLAGS`实际位定义 | OF30-X024/X013/X007 | 原A039及多符号导航不够 |

## R30-02｜原六件逐件消费表与正文替代

| 原件（全部不改） | 消费裁定 | 必须合读／局部替代 |
|---|---|---|
| `user-open-flags-30/MANIFEST.md` | 保留原执行者申报；不再按严格P2全合格消费 | 原`80/80`只证明历史固定源文字匹配的作者检查；实际单标签定义资格本次47保留、31修正、2导航；新75另计，不追溯改动旧self_check |
| `user-open-flags-30/open-flags.md` | 技术前缀有界保留，词义/证据层需本勘误 | 将“复合字面量”替为“局部聚合变量的指定初始化”； `resolve`零值由C聚合规则成立；未知/非法raw-helper和legacy分别说明，`O_TMPFILE`是**复合掩码包含O_DIRECTORY位**不是同值别名；旧打开成功、权限后果依旧NOT_RUN |
| `user-open-flags-30/facts-and-boundaries.yaml` | 10个字段快照继续以源条件消费 | `op_uninitialized`对本批`open_flags_s op;`自动局部对象在调用前未赋值成立；对任意外部raw-helper调用者只能说“**helper本次未写**”，不能证明调用前未初始化。 `RESOLVE_CACHED`只在有非零resolve的helper条件中；补后述有限纸面模型 |
| `user-open-flags-30/open-flags-paths.dot` | 原图保留为历史关系草图，指定边标签须替换 | 下方精确列原图边／注释的新表达；不把修改注记视为已经重画99条边、完成全图独立语义核验 |
| `user-open-flags-30/deferred-findings.yaml` | 原OF30-F01..F04条件项及旧qualified_id保留 | F01需getname有效且第二次fd>=0才能到部分op消费；F02是字段快照分歧，不凭字段差异断定bug；F03是helper-only的复合位门；F04是下游覆盖缺口，非已证权限漏洞；与US-03等的链接不是自动同根证明 |
| `user-open-flags-30/evidence.md` | **supersedes所指的限定证据资格勘误对象** | 以上原80表＋75段替代引文为本批规范消费视图；原历史文件零修改，A038、A058为导航，不可当定义体标签 |

### 图示文字与节点/边精确修正（不生成独立DOT）

以下是**原`open-flags-paths.dot`内相应文字的替代解释**；原图仍按历史文件保持字节冻结。

| 原图位置/含义 | 改为本勘误消费文本 | 理由/来源 |
|---|---|---|
| `doint -> buildflags`的`initial int fd=build_open_flags(&how,&op)` | `initial int fd=build_open_flags(how, &op)` | `do_sys_openat2`参数`open_how_s *how`，不用多一层取址；OF30-X064 |
| 图首部`O_TMPFILE aliases O_DIRECTORY` | `O_TMPFILE is a composite bitmask containing the O_DIRECTORY bit; not an equal-value alias` | OF30-X025/X019/X026 |
| `tmpcomp -> cached`的`O_DIRECTORY alias contributes ...` | `O_DIRECTORY component of O_TMPFILE participates in the RESOLVE_CACHED bitwise mask` | `O_TMPFILE=(__O_TMPFILE|O_DIRECTORY)`；原A024和本次宏定义 |
| `getname -> validname`的“valid ptr when no ERR_PTR” | `conditional result: only when getname actually returns a valid filename_s pointer; error pointer is a separate, unexpanded branch` | 不由“没看到失败”断言任意指针有效；原A031、OF30-X065及R29-S02 |
| `doint -> nameerr`的`IS_ERR(tmp)`控制门 | `getname may instead return ERR_PTR; IS_ERR(tmp) then returns PTR_ERR(tmp), no fd reassignment on that branch` | 错误指针来源从getname的返回语义而非validname节点中推出，原A031及OF30-X065 |
| `buildflags -> op_lookup`的“only after cached refusal branch” | `op.lookup_flags is assigned only if all preceding returns, including -EAGAIN under RESOLVE_CACHED, were NOT taken` | 原A024/A025；绝不表示已经在- EAGAIN之后写op |
| `op`五字段与“未初始化”关系 | `not written by build_open_flags on this exit; uninitialized only if caller provided an uninitialized op, as do_sys_openat2 does` | 新OF30-X005/X006/X064、主报告fields表 |
| `buildhow -> bhow`及原A003术语 | `local open_how aggregate object with designated initializers for flags/mode; omitted resolve zero-initialized by C rule` | OF30-X003/X075 |

**原图的覆盖边界：** `do_filp_open`第一层确实读取`op->lookup_flags`，再将`op`转交`path_openat`；错误的`op`字段内容不能安全地推导具体文件打开结果。历史05::UV-04指向更深`do_open/myos_do_dentry_open`的缺return，非`do_filp_open`自身。以上更正不冒充全图99边逐项功能再审。

## R30-02｜有限纸面数值对照（非运行）

固定定义：`O_PATH=0x200000`、`O_CREAT=0x40`、`__O_TMPFILE=0x400000`、`O_DIRECTORY=0x10000`、`O_TMPFILE=0x410000`、`O_TMPFILE_MASK=0x410040`、`S_IALLUGO=0xFFF`、`S_IFREG=0x8000`、`FMODE_NONOTIFY=0x04000000`。各十六进制从所选八进制宏复算，未运行编译器/用户程序。

| 纸面输入与来源 | 经过旧`build_open_how` | 直接送`build_open_flags`（helper-only） |
|---|---|---|
| 未知位`flags=0x08000000`，不在VALID_OPEN_FLAGS也不被strip | 先`flags &= VALID_OPEN_FLAGS`，该位变0，`resolve=0`；不会因该位命中未知flags门 | strip不删除这位，unknown-flags门返回`-EINVAL`，此前没有写任何op字段 |
| `O_PATH|O_CREAT=0x200040`，mode=0 | O_PATH优先只留`0x200000`，WILL_CREATE为假，mode归零；该O_PATH非法组合门不被触发 | raw how.flags=0x200040先走create-like并写op.mode，后O_PATH不允许组合门返回`-EINVAL`，此时只写mode |
| `O_CREAT|mode=0x1000`（S_IALLUGO之外） | `mode & 0xFFF=0`，再依create-like进入，op.mode可为S_IFREG（0x8000） | raw how.mode=0x1000通过WILL_CREATE后在mode合法性门`-EINVAL`，没有op字段写入 |
| `FMODE_NONOTIFY=0x04000000`单独输入 | 旧normalizer遮掉这无效用户位 | raw helper先`flags &= ~strip`移除它，所以也不因这一位触发unknown-flags门；与上一行未知位不同 |
| helper输入`resolve=RESOLVE_CACHED`且`flags=O_DIRECTORY` | 旧入口不会产生该非零resolve（C指定初始化零） | CACHED门位掩码含`O_TMPFILE`的O_DIRECTORY分量，因此可返回`-EAGAIN`；四字段已写，`op.lookup_flags`尚未写 |

这些模型要求原输入位、宏条件、有效对象及到达相应门，不能推导真实权限/文件/CPU结果。原15个纸面模型经C04独立有限复算报告“关键数值匹配”，但本次不继承为全部15行或各字段真实执行的完整通过证明。

## 待验边界、RETURN归属和D03自查

- 旧`OF30-F01..F04`保持原qualified_id和普通延期状态；本次修的是证据资格/文字口径，不决定内核修复、不增加漏洞数量。保留旧`CORE-USER-SYSCALL-STDIO-08::US-03`与`CORE-USER-VFS-BASELINE-05::UV-04`作为条件边界，而非新的同根判决。
- 原80源码文字逐字匹配是本轮新独立回源复核；原作者此前的80次文本自检仍是其历史声明，本文件不伪称C04已经机械复跑80条。新75段直接证据由本次执行者单独准备、原14源文件核对，不代表真实编译/构建。
- 实际构建条件、gcc优化、完整openat2入口与路径限制、fd级close-on-exec生命周期、未初始化op被读取后的具体运行表现均未得到证明。图示替代为文本增量，不变更原DOT本体。
- **D01/D02文档纠正申请完成，主线`RETURN`不自动关闭。** 待C04按53号/R30-01/R30-02独立验收；若某个替代引文的符号归属或图代换仍不闭合，应由C04保留returned_scope，不由本执行者自行签PASS。
- 本次勘误只涉及原30六件，不重写旧文件；C/ASM、预处理、编译、仓库脚本、QEMU/硬件、真实用户样本、文件系统/权限、仓库外libc/镜像与其它付费模型/API全未执行。所有动态核对仍`NOT_RUN`。
