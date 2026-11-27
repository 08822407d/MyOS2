# 第158批：用户库格式容量、对象游标与长度消费

CORE-USER-LIBC-FORMAT-CAPACITY-158；MYOS2-LEAD-002-CHAT-EXEC-27／MYOS2-A-C11c。固定time@a039d9803ade，修订220及221@c2bb3f55604f。源级静态与GIVEN有限模型，不是运行结果。四场景十二行分别重置对象。

## A｜n限制写，不免除源读取

FC158-E001 的宏把写和q++放在o<n内，o++在门外。FC158-E055 的%s实际取const char*、非NULL后strlen，再在FC158-E059每次EMIT之前读*sarg++。n=0仍需读fmt、取参、strlen和两个源字符。D[8]初始0x7e；S[8]按8对齐，62 63 00 7e 7e 7e 7e 7e。strlen读[0,8)，候选从cp0确认至cp2，给定有效整字语义。四行逻辑输出AbcZ长4。FC158-E062 收尾o<n写*q，否则n>0覆盖D[n-1]；FC158-E063 返回o，选定int可表示。

| 行 | n | 收尾前(q,o) | 真EMIT存储 | NUL | 最终D(hex) | 返回 |
|---|---:|---|---|---|---|---:|
| A01_N0 | 0 | 0,4 | 无 | 无 | 7e 7e 7e 7e 7e 7e 7e 7e | 4 |
| A01_N1 | 1 | 1,4 | A | D0=0 | 00 7e 7e 7e 7e 7e 7e 7e | 4 |
| A01_N4 | 4 | 4,4 | AbcZ | D3=0 | 41 62 63 00 7e 7e 7e 7e | 4 |
| A01_N5 | 5 | 4,4 | AbcZ | D4=0 | 41 62 63 5a 00 7e 7e 7e | 4 |

N1覆盖A，N4覆盖Z，N5追加NUL。存储次数、不同目标地址数、末态非NUL和return不等同。A只走字面/%s，不概括数字writer。

## B｜同一n不是同一物理对象

B01的%06ld!：%重置、0置FL_ZERO=1，6在width重读，l在modifiers重读令rank_long=1，d置base10、FL_SIGNED=64，实际va_arg signed long 7（FC158-E049）。flags65、width6、prec-1。helper局部o=0，位数1，tickskip1、nchars1；无前空格、符号、hash，五次补零（FC158-E023）；n=4时第五次只增o。随后FC158-E024先q+=ndigits再o+=ndigits，qq/oo从此复制；FC158-E028先递减qq/oo/ndigits，再oo<n存储；FC158-E029无条件除base。helper返回size_t o=6（FC158-E031），外层自己q+=sz/o+=sz（FC158-E054），并非helper的q。

| 行 | D/n | 真补零偏移 | 数字写 | helper末q/o，qq/oo | 外层进入!前q/o | !与NUL | 最终前缀／return |
|---|---|---|---|---|---|---|---|
| B01_N4 | 16/4 | 0–3 | 跳过D4 | 5/6，4/5 | 6/6 | !跳过；D3覆0 | 000 NUL／7 |
| B01_N7 | 16/7 | 0–4 | D5=7 | 6/6，5/5 | 6/6 | D6=!再覆0 | 000007 NUL／7 |
| B01_N8 | 16/8 | 0–4 | D5=7 | 6/6，5/5 | 6/6 | D6=!、D7=0 | 000007! NUL／7 |

D[16]所有游标均合法；未写维持0x7e。特别N4 helper q=5与外层q=6分属两调用帧。

B02两行%u、unsigned int 12345、n=4，base10、rank_int、flags0、width0、prec-1，位数5，无前置写。B02_NARROW的D[4]在FC158-E024（L167）第一次试图形成D+5，超one-past D+4；**在运算之前停止**。仅记停止前未写，o+=5、逆写、return、外层更新、NUL均不认证，不给最终D。B02_WIDE的D[8]允许D+5；逆向第一轮oo=4门假，随后真写序D3=4、D2=3、D1=2、D0=1。helper末q/o=5/5、qq/oo=0/0；外层q/o=5/5，D3覆NUL，最终31 32 33 00 7e 7e 7e 7e，return5。时间序与最终地址表分开。两个小缓冲案例均非已证shell真实调用。

## C｜提示符请求长度与实际对象关系

FC158-E081/FC158-E082/FC158-E084 定位PATH_MAX=4096、全局current_dir和真实main模板。单独取printf("MyOS2:%s # _", current_dir)，之前独立LF及后续fflush不合计。FC158-E094/FC158-E096/FC158-E097/FC158-E100/FC158-E101/FC158-E104：顶层文本写外部musl路径并加入myinit/myshell，另有独立静态mylibc；CMAKE_C_COMPILER在project后，实际链接UNKNOWN。相同符号和声明不能认证实际绑定，图用CONDITIONAL_MODEL_BINDING。用户-m64及wordsize条件仅支持模型选取，外部stddef/stdarg、va_list宏、真实ABI未知。FC158-E113/FC158-E118/FC158-E116/FC158-E117定位活动声明及类型路由。

FC158-E083/FC158-E089/FC158-E090/FC158-E091：main清空并以4096调用getcwd；内核先为NUL预留1，再拼路径，算len，依次检查len>PATH_MAX、len>size，才copy_to_user；成功分支将(long)buf赋给int error。不能反推缓存内容、证明实际目录树可生产这些串。给定current_dir[4096]按8对齐，首/、其后x直到L处NUL，余0x7e；L=4085/4086末字[4080,4088)，L=4095末字[4088,4096)，给定整字访问语义，源范围均足够。

FC158-E065/FC158-E064/FC158-E066：printf局部buf[4096]、int rv=vsnprintf。前缀6、后缀4，逻辑o=L+10，纯字面/%s饱和q最多one-past。FC158-E067 直接将rv转换为write的size_t长度，没有夹取；FILE._fileno int（FC158-E112）给定F，不默认1。只有write正常返回时FC158-E068返回原rv，不用write的返回值。

| L | rv | 收尾前q | 最后buf | write整数请求 | 超对象字节 |
|---:|---:|---:|---|---|---:|
| 4085 | 4095 | 4095 | 全4095文本，D4095=NUL | [0,4095)，不含NUL | 0 |
| 4086 | 4096 | 4096 | _先存D4095再覆NUL，非NUL4095 | [0,4096)，含NUL | 0 |
| 4095 | 4105 | 4096 | 先存前缀6+cwd前4090，末字覆NUL；最终前缀6+cwd前4089+NUL，后缀全跳过 | [0,4105)，含NUL | 9 |

最长行读整个4095源字符（另有strlen整字和确认）；未在格式阶段写出目的对象。4105只描述请求整数偏移区间，4096..4104是对象外9字节；不形成或解引用buf+4105，不认证它们被读取、输出、内容或泄漏。实际write返回/内核/设备/屏显未知。

## 旧项与验收边界

08 GAP-U10、117/118短文件与缓存显示、129 41字节日期、130短诊断、124 strlen整字条件保留；94及130–132号合同内核printf/printk是另一实现。129的R129-01既有文档关闭、129组合PASS_PENDING_LOCAL保持，只向qualified容量项追加本次限定证据，不关闭effective ABI/绑定/真实写结果。R155既有闭合保持，D00=[]，不重算旧批。事实机表按case给输入、局部帧、时间序、最终RLE、返回及请求；图逐case按seq连接。122本席新窗、122回核、0降级；历史205、主线3、设计191/100均不计。完整本地P9与真实运行NOT_RUN；实际G04发表后实核，Pro独立验收PENDING。
