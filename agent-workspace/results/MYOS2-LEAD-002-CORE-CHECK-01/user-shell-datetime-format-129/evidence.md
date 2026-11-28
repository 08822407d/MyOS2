---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SHELL-DATETIME-FORMAT-129
record_type: pinned_source_short_quotes
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-19
produced_by: "GPT-6（会话自报，实际后台和effort UNATTESTED）"
model_per_owner: "GPT-6＋Extra（180号固定合同指定）"
date: 2026-11-22
date_timezone: Asia/Shanghai
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
inputs_read:
  - "myinitramfs/myshell/myshell.c"
  - "myinitramfs/myshell/tests.c"
  - "myinitramfs/mylib/myglibc/stdio/printf.c"
  - "myinitramfs/mylib/myglibc/stdio/vsnprintf.c"
  - "myinitramfs/mylib/myglibc/string/wordcopy.c"
  - "myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h"
  - "myinitramfs/mylib/myglibc/string/memcpy.c"
  - "mykernel/scripts/options_flags.cmake"
  - "mykernel/debug/panic.c"
---

# 第129批新增源码短窗

所有定义窗口直接从固定 time@a039d9803ade（短12）通过GitHub连接器按原物理行读取，各单个窗口严格连续1–5行且位于所标符号定义内。特别是stdio/vsnprintf.c采用原仓库CRLF物理行，不剥除每行尾部CR，原tab/空格一并保留。源码短窗是读取证明，不等于构建、链接或运行。历史R127/R128文档补正不新增历史源码窗口。

## DT129-E001 · main首先调用时间测试且不接收返回

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
int main(int argc, const char *argv[])
{
	test_PrintDateTime();

	printf("This is MyShell v0.01.\n");
```

普通行界导航：myinitramfs/myshell/myshell.c:66-70，逐字引文按固定源码定义块回核。

## DT129-E002 · main接续问候和控制台设置

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
	printf("This is MyShell v0.01.\n");
	printf("You are login as root.\n");

	console_fd = open("/dev/console", O_RDWR);

```

普通行界导航：myinitramfs/myshell/myshell.c:70-74，逐字引文按固定源码定义块回核。

## DT129-E003 · 测试函数对象与64字节日期缓冲

[VERIFIED myinitramfs/myshell/tests.c::test_PrintDateTime]
```c
int test_PrintDateTime() {
	struct timeval tv;
	struct tm *tm_info;
	char buffer[64];

```

普通行界导航：myinitramfs/myshell/tests.c:104-108，逐字引文按固定源码定义块回核。

## DT129-E004 · gettimeofday失败调用perror并返回1

[VERIFIED myinitramfs/myshell/tests.c::test_PrintDateTime]
```c
	// 获取当前时间
	if (gettimeofday(&tv, NULL) == -1) {
		perror("gettimeofday failed");
		return 1;
	}
```

普通行界导航：myinitramfs/myshell/tests.c:109-113，逐字引文按固定源码定义块回核。

## DT129-E005 · localtime与strftime日期字符串格式

[VERIFIED myinitramfs/myshell/tests.c::test_PrintDateTime]
```c
	// 将秒数转换为本地时间
	tm_info = localtime(&tv.tv_sec);

	// 格式化时间为字符串
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
```

普通行界导航：myinitramfs/myshell/tests.c:117-121，逐字引文按固定源码定义块回核。

## DT129-E006 · 日期格式与完整printf参数

[VERIFIED myinitramfs/myshell/tests.c::test_PrintDateTime]
```c
	// 格式化时间为字符串
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

	// 打印日期和微秒
	printf("DateTime Now: %s.%06ld\n", buffer, tv.tv_usec);
```

普通行界导航：myinitramfs/myshell/tests.c:120-124，逐字引文按固定源码定义块回核。

## DT129-E007 · printf与正常返回0

[VERIFIED myinitramfs/myshell/tests.c::test_PrintDateTime]
```c
	// 打印日期和微秒
	printf("DateTime Now: %s.%06ld\n", buffer, tv.tv_usec);

	return 0;
}
```

普通行界导航：myinitramfs/myshell/tests.c:123-127，逐字引文按固定源码定义块回核。

## DT129-E008 · 4096字节缓冲宏

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::SZ_4K]
```c
	#define SZ_4K 4096
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/printf.c:9-9，逐字引文按固定源码定义块回核。

## DT129-E009 · printf内部char buf及rv

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
int printf(const char * fmt, ...)
{
	va_list ap;
	char buf[SZ_4K];
	int rv;
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/printf.c:11-15，逐字引文按固定源码定义块回核。

## DT129-E010 · 启动变参并格式化为rv

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	va_start(ap, fmt);
	rv = vsnprintf(buf, SZ_4K, fmt, ap);
	va_end(ap);
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/printf.c:17-19，逐字引文按固定源码定义块回核。

## DT129-E011 · 向stdout文件号调用write，忽略write返回

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	// buffered IO has unsolved bug, so derectly use the write() function
	write(stdout->_fileno, buf, rv);
	// rv = (fputs(buf, stdout) < 0) ? 0 : rv;
	// fflush(stdout);
	return rv;
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/printf.c:21-25，逐字引文按固定源码定义块回核。

## DT129-E012 · 格式标志包含FL_ZERO

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::flags]
```c
enum flags {
	FL_ZERO				= 0x01,		/* Zero modifier */
	FL_MINUS			= 0x02,		/* Minus modifier */
	FL_PLUS				= 0x04,		/* Plus modifier */
	FL_TICK				= 0x08,		/* ' modifier */
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:11-15，逐字引文按固定源码定义块回核。

## DT129-E013 · 格式标志包含FL_SIGNED

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::flags]
```c
	FL_SPACE			= 0x10,		/* Space modifier */
	FL_HASH				= 0x20,		/* # modifier */
	FL_SIGNED			= 0x40,		/* Number is signed */
	FL_UPPER			= 0x80,		/* Upper case digits */
};
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:16-20，逐字引文按固定源码定义块回核。

## DT129-E014 · rank_long和类型等级定义

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::ranks]
```c
enum ranks {
	rank_char			= -2,
	rank_short			= -1,
	rank_int			=  0,
	rank_long			=  1,
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:25-29，逐字引文按固定源码定义块回核。

## DT129-E015 · EMIT宏按q写字节且o自增

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::EMIT]
```c
#define EMIT(x)			({ if(o<n) {*q++ = (x);} o++; })
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:39-39，逐字引文按固定源码定义块回核。

## DT129-E016 · format_int宽度和缓冲参数

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
static size_t format_int(char * q, size_t n, size_t val,
		enum flags flags, int base, int width, int prec)
{
	char * qq;
	size_t o = 0, oo;
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:41-45，逐字引文按固定源码定义块回核。

## DT129-E017 · 十进制字符表与digits计数

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	static const char lcdigits[] = "0123456789abcdef";
	static const char ucdigits[] = "0123456789ABCDEF";
	const char * digits;
	size_t tmpval;
	int minus = 0;
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:46-50，逐字引文按固定源码定义块回核。

## DT129-E018 · 有符号负号判定逻辑

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	 * If signed, separate out the minus
	 */
	if ((flags & FL_SIGNED) && ((ssize_t)val < 0))
	{
		minus = 1;
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:60-64，逐字引文按固定源码定义块回核。

## DT129-E019 · 十进制初始数字位数

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	/*
	 * Count the number of digits needed.  This returns zero for 0
	 */
	tmpval = val;
	while (tmpval)
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:68-72，逐字引文按固定源码定义块回核。

## DT129-E020 · 数字数与零值特别处理

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	if (ndigits < prec)
	{
		ndigits = prec;		/* Mandatory number padding */
	}
	else if (val == 0)
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:87-91，逐字引文按固定源码定义块回核。

## DT129-E021 · 无需分组时tickskip等于ndigits

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
		tickskip = (base == 16) ? 4 : 3;
	}
	else
	{
		tickskip = ndigits;	/* No tick marks */
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:101-105，逐字引文按固定源码定义块回核。

## DT129-E022 · 数字组长度和nchars

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	 * Tick marks aren't digits, but generated by the number converter
	 */
	ndigits += (ndigits - 1) / tickskip;

	/*
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:109-113，逐字引文按固定源码定义块回核。

## DT129-E023 · 空格早期填充条件与循环

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	/*
	 * Emit early space padding
	 */
	if (!(flags & (FL_MINUS | FL_ZERO)) && (width > nchars))
	{
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:125-129，逐字引文按固定源码定义块回核。

## DT129-E024 · FL_ZERO的宽度填充循环

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	/*
	 * Emit zero padding
	 */
	if (((flags & (FL_MINUS | FL_ZERO)) == FL_ZERO) && (width > ndigits))
	{
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:153-157，逐字引文按固定源码定义块回核。

## DT129-E025 · 向数字区末端推进q/o再倒写

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	 * Generate the number.  This is done from right to left
	 */
	q += ndigits;			/* Advance the pointer to end of number */
	o += ndigits;
	qq = q;
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:165-169，逐字引文按固定源码定义块回核。

## DT129-E026 · 右向左数字循环入口

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	b4tick = tickskip;
	while (ndigits > 0)
	{
		if (!b4tick--)
		{
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:172-176，逐字引文按固定源码定义块回核。

## DT129-E027 · 右向左写digits并除以base

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
		qq--;
		oo--;
		ndigits--;
		if (oo < n)
			*qq = digits[val % base];
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:184-188，逐字引文按固定源码定义块回核。

## DT129-E028 · 尾空格与return o

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	while ((flags & FL_MINUS) && (width > nchars))
	{
		EMIT(' ');
		width--;
	}
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:195-199，逐字引文按固定源码定义块回核。

## DT129-E029 · vsnprintf格式游标输出游标与计数

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
int vsnprintf(char * buf, size_t n, const char * fmt, va_list ap)
{
	const char * p = fmt;
	char ch;
	char *q = buf;
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:533-537，逐字引文按固定源码定义块回核。

## DT129-E030 · rank/width/prec/flags本地状态

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	size_t val = 0;
	int rank = rank_int;	/* Default rank */
	int width = 0;
	int prec = -1;
	int base;
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:539-543，逐字引文按固定源码定义块回核。

## DT129-E031 · 格式状态枚举

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	enum {
		st_normal,		/* Ground state */
		st_flags,		/* Special flags */
		st_width,		/* Field width */
		st_prec,		/* Field precision */
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:546-550，逐字引文按固定源码定义块回核。

## DT129-E032 · 字符串参数和格式迭代入口

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	const char * sarg;	/* %s string argument */
	char carg;			/* %c char argument */
	int slen;			/* String length */

	while ((ch = *p++))
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:553-557，逐字引文按固定源码定义块回核。

## DT129-E033 · 正常态遇百分号进入flags并清空状态

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
		switch (state)
		{
		case st_normal:
			if (ch == '%')
			{
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:559-563，逐字引文按固定源码定义块回核。

## DT129-E034 · 每次新转换清零rank宽度和precision

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				flags = 0;
				rank = rank_int;
				width = 0;
				prec = -1;
			}
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:565-569，逐字引文按固定源码定义块回核。

## DT129-E035 · 普通字符经EMIT输出

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			else
			{
				EMIT(ch);
			}
			break;
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:570-574，逐字引文按固定源码定义块回核。

## DT129-E036 · flags状态开始

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
		case st_flags:
			switch (ch)
			{
			case '-':
				flags |= FL_MINUS;
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:576-580，逐字引文按固定源码定义块回核。

## DT129-E037 · 字符0置FL_ZERO，其他字符p回退

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			case '0':
				flags |= FL_ZERO;
				break;
			default:
				state = st_width;
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:594-598，逐字引文按固定源码定义块回核。

## DT129-E038 · width累加十进制数字

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
		case st_width:
			if (ch >= '0' && ch <= '9')
			{
				width = width * 10 + (ch - '0');
			}
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:604-608，逐字引文按固定源码定义块回核。

## DT129-E039 · 非宽度字符切入modifiers并回退

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			else
			{
				state = st_modifiers;
				p--;		/* Process this character again */
			}
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:623-627，逐字引文按固定源码定义块回核。

## DT129-E040 · 长度修饰l增加rank

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
		case st_modifiers:
			switch (ch)
			{
			/*
			 * Length modifiers - nonterminal sequences
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:648-652，逐字引文按固定源码定义块回核。

## DT129-E041 · modifiers默认转回正常态

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			default:
				/*
				 * Next state will be normal
				 */
				state = st_normal;
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:673-677，逐字引文按固定源码定义块回核。

## DT129-E042 · 修饰级别范围裁剪

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				 * Canonicalize rank
				 */
				if (rank < MIN_RANK)
					rank = MIN_RANK;
				else if (rank > MAX_RANK)
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:680-684，逐字引文按固定源码定义块回核。

## DT129-E043 · 带符号d选择10进制及FL_SIGNED

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				case 'd':		/* Signed decimal output */
				case 'i':
					base = 10;
					flags |= FL_SIGNED;
					switch (rank)
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:700-704，逐字引文按固定源码定义块回核。

## DT129-E044 · rank_long使用va_arg signed long

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
					case rank_int:
						val = (size_t)(ssize_t)
						va_arg(ap, signed int);
						break;
					case rank_long:
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:717-721，逐字引文按固定源码定义块回核。

## DT129-E045 · 调用format_int并增加外层q/o

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
					is_integer: sz = format_int(q, (o < n) ? n - o : 0, val,
							flags, base, width, prec);
					q += sz;
					o += sz;
					break;
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:772-776，逐字引文按固定源码定义块回核。

## DT129-E046 · 百分号s的va_arg与strlen

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				case 's':		/* String */
					sarg = va_arg(ap, const char *);
					sarg = sarg ? sarg : "(null)";
					slen = strlen(sarg);
					goto is_string;
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:783-787，逐字引文按固定源码定义块回核。

## DT129-E047 · 字符串precision前置条件

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
					is_string: {
						char sch;
						int i;

						if (prec != -1 && slen > prec)
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:789-793，逐字引文按固定源码定义块回核。

## DT129-E048 · 字符串每字节EMIT

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
						for (i = slen; i; i--)
						{
							sch = *sarg++;
							EMIT(sch);
						}
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:805-809，逐字引文按固定源码定义块回核。

## DT129-E049 · 默认格式字符处理

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				default:		/* Anything else, including % */
					EMIT(ch);
					break;
				}
				break;
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:858-862，逐字引文按固定源码定义块回核。

## DT129-E050 · vsnprintf尾部NUL收尾

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	/*
	 * Null-terminate the string
	 */
	if (o < n)
		*q = '\0';				/* No overflow */
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:868-872，逐字引文按固定源码定义块回核。

## DT129-E051 · 返回格式化输出o

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
		buf[n - 1] = '\0';		/* Overflow - terminate at end of buffer */

	return o;
}
```

普通行界导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:874-877，逐字引文按固定源码定义块回核。

## DT129-E052 · R127历史源码中先读再写的寄存暂存关系

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	do8:
		a0 = ((op_t *)srcp)[0];
		((op_t *)dstp)[0] = a1;
	do7:
		a1 = ((op_t *)srcp)[1];
```

普通行界导航：myinitramfs/mylib/myglibc/string/wordcopy.c:94-98，逐字引文按固定源码定义块回核。

## DT129-E053 · R127历史最后一对读写

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	do2:
		a0 = ((op_t *)srcp)[6];
		((op_t *)dstp)[6] = a1;
	do1:
		a1 = ((op_t *)srcp)[7];
```

普通行界导航：myinitramfs/mylib/myglibc/string/wordcopy.c:112-116，逐字引文按固定源码定义块回核。

## DT129-E054 · R127历史游标更新及do while回边

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
		srcp += 8 * OPSIZ;
		dstp += 8 * OPSIZ;
		len -= 8;
	} while (len != 0);
```

普通行界导航：myinitramfs/mylib/myglibc/string/wordcopy.c:119-122，逐字引文按固定源码定义块回核。

## DT129-E055 · R127历史循环外do0

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	/* This is the right position for do0.  Please don't move
	   it into the loop.  */
do0:
	((op_t *)dstp)[0] = a1;
}
```

普通行界导航：myinitramfs/mylib/myglibc/string/wordcopy.c:124-128，逐字引文按固定源码定义块回核。

## DT129-E056 · R128历史WORD宏根据源是否对齐选择helper

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::WORD_COPY_FWD]
```c
	#define WORD_COPY_FWD(dst_bp, src_bp, nbytes_left, nbytes)                \
		do                                                                    \
		{                                                                     \
			if (src_bp % OPSIZ == 0)                                          \
				_wordcopy_fwd_aligned(dst_bp, src_bp, (nbytes) / OPSIZ);      \
```

普通行界导航：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:121-125，逐字引文按固定源码定义块回核。

## DT129-E057 · R128历史源未对齐helper和外层游标更新

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::WORD_COPY_FWD]
```c
			else                                                              \
				_wordcopy_fwd_dest_aligned(dst_bp, src_bp, (nbytes) / OPSIZ); \
			src_bp += (nbytes) & -OPSIZ;                                      \
			dst_bp += (nbytes) & -OPSIZ;                                      \
			(nbytes_left) = (nbytes) % OPSIZ;                                 \
```

普通行界导航：myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:126-130，逐字引文按固定源码定义块回核。

## DT129-E058 · R128历史前导BYTE前需要len和目的对齐

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	/* If there not too few bytes to copy, use word copy.  */
	if (len >= OP_T_THRES)
	{
		/* Copy just a few bytes to make DSTP aligned.  */
		len -= (-dstp) % OPSIZ;
```

普通行界导航：myinitramfs/mylib/myglibc/string/memcpy.c:35-39，逐字引文按固定源码定义块回核。

## DT129-E059 · R128 PAGE和WORD位置

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
		PAGE_COPY_FWD_MAYBE(dstp, srcp, len, len);

		/* Copy from SRCP to DSTP taking advantage of the known alignment of
	   DSTP.  Number of bytes remaining is put in the third argument,
	   i.e. in LEN.  This number may vary from machine to machine.  */
```

普通行界导航：myinitramfs/mylib/myglibc/string/memcpy.c:45-49，逐字引文按固定源码定义块回核。

## DT129-E060 · R128尾BYTE之后返回原dstpp

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	/* There are just a few bytes to copy.  Use byte memory operations.  */
	BYTE_COPY_FWD(dstp, srcp, len);

	return dstpp;
```

普通行界导航：myinitramfs/mylib/myglibc/string/memcpy.c:56-59，逐字引文按固定源码定义块回核。

## DT129-E061 · time内核C配置分支金丝雀

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

普通行界导航：mykernel/scripts/options_flags.cmake:39-43，逐字引文按固定源码定义块回核。

## DT129-E062 · time CONFIG活动宏

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

普通行界导航：mykernel/scripts/options_flags.cmake:58-62，逐字引文按固定源码定义块回核。

## DT129-E063 · time panic分支金丝雀

[VERIFIED mykernel/debug/panic.c::panic]
```c
	this_cpu = smp_processor_id();
```

普通行界导航：mykernel/debug/panic.c:79-79，逐字引文按固定源码定义块回核。

## DT129-E064 · 零值数字必须输出一个0

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	}
	else if (val == 0)
	{
		ndigits = 1;		/* Zero still requires space */
	}
```

普通导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:90-94。

## DT129-E065 · FL_ZERO按nchars补零循环

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	if (((flags & (FL_MINUS | FL_ZERO)) == FL_ZERO) && (width > ndigits))
	{
		while (width > nchars) {
			EMIT('0');
			width--;
```

普通导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:156-160。

## DT129-E066 · 逆向数字区提前推进q

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	 * Generate the number.  This is done from right to left
	 */
	q += ndigits;			/* Advance the pointer to end of number */
	o += ndigits;
	qq = q;
```

普通导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:165-169。

## DT129-E067 · 从右至左写digits并整除base

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
		oo--;
		ndigits--;
		if (oo < n)
			*qq = digits[val % base];
		val /= base;
```

普通导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:185-189。

## DT129-E068 · format_int返回局部o

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	}

	return o;
}
```

普通导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:199-202。

## DT129-E069 · flags不识别转换字符则p回退

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				flags |= FL_ZERO;
				break;
			default:
				state = st_width;
				p--;	/* Process this character again */
```

普通导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:595-599。

## DT129-E070 · 小写l修饰提升到rank_long

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			case 'l':
				rank++;		/* Longer rank */
				break;
```

普通导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:657-659。

## DT129-E071 · rank_long从变参取signed long

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
					case rank_long:
						val = (size_t)(ssize_t)
						va_arg(ap, signed long);
						break;
```

普通导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:721-724。

## DT129-E072 · NUL收尾及返回o

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	if (o < n)
		*q = '\0';				/* No overflow */
	else if (n > 0)
		buf[n - 1] = '\0';		/* Overflow - terminate at end of buffer */

```

普通导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:871-875。

## DT129-E073 · 宽度状态未识别l回退进入modifiers

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			else
			{
				state = st_modifiers;
				p--;		/* Process this character again */
			}
```

普通导航：myinitramfs/mylib/myglibc/stdio/vsnprintf.c:623-627。

## DT129-E074 · R128定点前导字节调用位置

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
		len -= (-dstp) % OPSIZ;
		BYTE_COPY_FWD(dstp, srcp, (-dstp) % OPSIZ);
```

普通导航：myinitramfs/mylib/myglibc/string/memcpy.c:39-40。

## DT129-E075 · R128定点WORD复制调用位置

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c

		WORD_COPY_FWD(dstp, srcp, len, len);
```

普通导航：myinitramfs/mylib/myglibc/string/memcpy.c:50-51。

## DT129-E076 · shell启动先测试再问候

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
int main(int argc, const char *argv[])
{
	test_PrintDateTime();

	printf("This is MyShell v0.01.\n");
```

普通导航：myinitramfs/myshell/myshell.c:66-70。

## 有关输入来源和边界的普通定位

- `myinitramfs/myshell/tests.c:104-127`提供独立测试函数的所有调用；`myshell.c:66-76`是启动处。在正常例中把localtime成功与strftime产生19字节日期文本作为明确给定前提，不宣称本仓真实时区数据或实际日期。
- 字符串的`strlen`调用和`write`提交都仅来源于选定用户程序及其库定义文本；真实shell二进制是否链接此库、stdout->_fileno值、用户输出的屏显与syscall返回仍属未知。
- 日期格式和混合格式属于不同函数的常量：strftime负责`%Y-%m-%d %H:%M:%S`，vsnprintf负责`DateTime Now: %s.%06ld\\n`。数字样例是模型输入，不是实机实际时间。
