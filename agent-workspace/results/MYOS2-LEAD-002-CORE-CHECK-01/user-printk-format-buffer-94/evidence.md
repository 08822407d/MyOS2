---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PRINTK-FORMAT-BUFFER-94
record_type: fixed_time_source_anchors
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-12
produced_by: GPT-6 (visible family; backend UNATTESTED)
model_per_owner: GPT-6 + Extra
date: 2026-11-16
base_snapshot: "time（分支名）"
read_channel: connector
status: AUTHOR_STATIC_SOURCE_PENDING_C06_PRO
runtime_evidence: NOT_RUN
---

# 第94批固定time定义内短锚

每条短引文都是固定 time@a039d9803ade 真实函数、单一宏或对象定义内部连续1–5行。短引文须与完整定义及facts矩阵合读，尤其注释掉的锁/输出筛选、宏的静态一次标记、log_prefix出参、LOG_BUF逻辑/物理单位以及控制台回调区间不能由单行直接证明。未运行任何仓库代码、构建、QEMU、实机或Graphviz。

### PK94-E001 | KERN_ERR："<3>"

[VERIFIED mykernel/include/linux/debug/printk.h::KERN_ERR]

```c
	#define KERN_ERR		"<3>"	/* error conditions			*/
```

### PK94-E002 | pr_fmt：pr_fmt(fmt) fmt

[VERIFIED mykernel/include/linux/debug/printk.h::pr_fmt]

```c
	#	define pr_fmt(fmt) fmt
```

### PK94-E003 | pr_err：printk(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)

[VERIFIED mykernel/include/linux/debug/printk.h::pr_err]

```c
		printk(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)
```

### PK94-E004 | pr_err_once：printk_once(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)

[VERIFIED mykernel/include/linux/debug/printk.h::pr_err_once]

```c
		printk_once(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)
```

### PK94-E005 | printk_once：static bool __print_once;

[VERIFIED mykernel/include/linux/debug/printk.h::printk_once]

```c
		static bool __print_once;		\
							\
```

### PK94-E006 | printk_once：if (!__print_once) {

[VERIFIED mykernel/include/linux/debug/printk.h::printk_once]

```c
		if (!__print_once) {			\
			__print_once = true;		\
			printk(fmt, ##__VA_ARGS__);	\
		}					\
```

### PK94-E007 | default_message_loglevel：(console_printk[1])

[VERIFIED mykernel/include/linux/debug/printk.h::default_message_loglevel]

```c
	#define default_message_loglevel	(console_printk[1])
```

### PK94-E008 | console_loglevel：(console_printk[0])

[VERIFIED mykernel/include/linux/debug/printk.h::console_loglevel]

```c
	#define console_loglevel			(console_printk[0])
```

### PK94-E009 | create_worker：pr_err_once("workqueue: Failed to allocate a worker ID: %p

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
		pr_err_once("workqueue: Failed to allocate a worker ID: %pe\n",
			    ERR_PTR(id));
```

### PK94-E010 | create_worker：pr_err_once("workqueue: Failed to allocate a worker\n");

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
		pr_err_once("workqueue: Failed to allocate a worker\n");
		goto fail;
```

### PK94-E011 | printk：va_start(args, fmt);

[VERIFIED mykernel/printk/printk.c::printk]

```c
	va_start(args, fmt);
	r = vprintk(fmt, args);
	va_end(args);

```

### PK94-E012 | printk：return r;

[VERIFIED mykernel/printk/printk.c::printk]

```c
	return r;
```

### PK94-E013 | vprintk：int printed_len = 0;

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
	int printed_len = 0;
	int current_log_level = default_message_loglevel;
	ulong flags;
	int this_cpu;
	char *p;
```

### PK94-E014 | vprintk：printk_delay();

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
	printk_delay();

	// preempt_disable();
	// /* This stops the holder of console_sem just where we want him */
```

### PK94-E015 | vprintk：// spin_lock(&logbuf_lock);

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
	// spin_lock(&logbuf_lock);
	// printk_cpu = this_cpu;

```

### PK94-E016 | vprintk：if (recursion_bug) {

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
	if (recursion_bug) {
		recursion_bug = 0;
		strcpy(printk_buf, recursion_bug_msg);
		printed_len = strlen(recursion_bug_msg);
	}
```

### PK94-E017 | vprintk：printed_len += vscnprintf(printk_buf + printed_len,

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
	printed_len += vscnprintf(printk_buf + printed_len,
				  sizeof(printk_buf) - printed_len, fmt, args);
```

### PK94-E018 | vprintk：plen = log_prefix(p, &current_log_level, &special);

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
	plen = log_prefix(p, &current_log_level, &special);
	if (plen) {
		p += plen;
```

### PK94-E019 | vprintk：case 'c':

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
		case 'c': /* Strip <c> KERN_CONT, continue line */
			plen = 0;
			break;
		case 'd': /* Strip <d> KERN_DEFAULT, start new line */
```

### PK94-E020 | vprintk：case 'd':

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
		case 'd': /* Strip <d> KERN_DEFAULT, start new line */
			plen = 0;
		default:
			if (!new_text_line) {
				emit_log_char('\n');
```

### PK94-E021 | vprintk：for (; *p; p++) {

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
	for (; *p; p++) {
		if (new_text_line) {
			new_text_line = 0;

```

### PK94-E022 | vprintk：for (i = 0; i < plen; i++)

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
				for (i = 0; i < plen; i++)
					emit_log_char(printk_buf[i]);
				printed_len += plen;
```

### PK94-E023 | vprintk：printed_len += plen;

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
				printed_len += plen;
			} else {
```

### PK94-E024 | vprintk：emit_log_char('<');

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
				emit_log_char('<');
				emit_log_char(current_log_level + '0');
				emit_log_char('>');
				printed_len += 3;
			}
```

### PK94-E025 | vprintk：char tbuf[50], *tp;

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
				char tbuf[50], *tp;
				unsigned tlen;
				unsigned long long t;
				unsigned long nanosec_rem;
```

### PK94-E026 | vprintk：t = local_clock();

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
				t = local_clock();
				nanosec_rem = do_div(t, 1000000000);
				tlen = sprintf(tbuf, "[%5lu.%06lu] ",
						(unsigned long) t,
						nanosec_rem / 1000);
```

### PK94-E027 | vprintk：for (tp = tbuf; tp < tbuf + tlen; tp++)

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
				for (tp = tbuf; tp < tbuf + tlen; tp++)
					emit_log_char(*tp);
				printed_len += tlen;
```

### PK94-E028 | vprintk：if (!*p)

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
			if (!*p)
				break;
```

### PK94-E029 | vprintk：emit_log_char(*p);

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
		emit_log_char(*p);
		if (*p == '\n')
			new_text_line = 1;
```

### PK94-E030 | vprintk：if (console_trylock_for_printk(this_cpu))

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
	if (console_trylock_for_printk(this_cpu))
		console_unlock();
```

### PK94-E031 | vprintk：return printed_len;

[VERIFIED mykernel/printk/printk.c::vprintk]

```c
	return printed_len;
```

### PK94-E032 | recursion_bug_msg：static const char	recursion_bug_msg [] =

[VERIFIED mykernel/printk/printk.c::recursion_bug_msg]

```c
static const char	recursion_bug_msg [] =
		KERN_CRIT "BUG: recent printk recursion!\n";
```

### PK94-E033 | recursion_bug：static int		recursion_bug;

[VERIFIED mykernel/printk/printk.c::recursion_bug]

```c
static int		recursion_bug;
```

### PK94-E034 | new_text_line：static int		new_text_line = 1;

[VERIFIED mykernel/printk/printk.c::new_text_line]

```c
static int		new_text_line = 1;
```

### PK94-E035 | printk_buf：static char		printk_buf[1024];

[VERIFIED mykernel/printk/printk.c::printk_buf]

```c
static char		printk_buf[1024];
```

### PK94-E036 | printk_delay_msec：int printk_delay_msec __read_mostly;

[VERIFIED mykernel/printk/printk.c::printk_delay_msec]

```c
int printk_delay_msec __read_mostly;
```

### PK94-E037 | printk_delay：if (unlikely(printk_delay_msec))

[VERIFIED mykernel/printk/printk.c::printk_delay]

```c
	if (unlikely(printk_delay_msec)) {
		int m = printk_delay_msec;

		while (m--) {
			mdelay(1);
```

### PK94-E038 | console_printk：int console_printk[4] = {

[VERIFIED mykernel/printk/printk.c::console_printk]

```c
int console_printk[4] = {
	DEFAULT_CONSOLE_LOGLEVEL,	/* console_loglevel */
	DEFAULT_MESSAGE_LOGLEVEL,	/* default_message_loglevel */
	MINIMUM_CONSOLE_LOGLEVEL,	/* minimum_console_loglevel */
	DEFAULT_CONSOLE_LOGLEVEL,	/* default_console_loglevel */
```

### PK94-E039 | CONFIG_LOG_BUF_SHIFT：CONFIG_LOG_BUF_SHIFT

[VERIFIED mykernel/include/generated/conf.h::CONFIG_LOG_BUF_SHIFT]

```c
#define CONFIG_LOG_BUF_SHIFT			23
```

### PK94-E040 | __LOG_BUF_LEN：(1 << CONFIG_LOG_BUF_SHIFT)

[VERIFIED mykernel/printk/printk.c::__LOG_BUF_LEN]

```c
#define __LOG_BUF_LEN				(1 << CONFIG_LOG_BUF_SHIFT)
```

### PK94-E041 | __log_buf：static char	__log_buf[__LOG_BUF_LEN];

[VERIFIED mykernel/printk/printk.c::__log_buf]

```c
static char	__log_buf[__LOG_BUF_LEN];
```

### PK94-E042 | log_buf：static char	*log_buf = __log_buf;

[VERIFIED mykernel/printk/printk.c::log_buf]

```c
static char	*log_buf = __log_buf;
```

### PK94-E043 | log_buf_len：static int	log_buf_len = __LOG_BUF_LEN;

[VERIFIED mykernel/printk/printk.c::log_buf_len]

```c
static int	log_buf_len = __LOG_BUF_LEN;
```

### PK94-E044 | log_start：static uint log_start;

[VERIFIED mykernel/printk/printk.c::log_start]

```c
static uint log_start;	/* Index into log_buf: next char to be read by syslog() */
```

### PK94-E045 | con_start：static uint con_start;

[VERIFIED mykernel/printk/printk.c::con_start]

```c
static uint con_start;	/* Index into log_buf: next char to be sent to consoles */
```

### PK94-E046 | log_end：static uint log_end;

[VERIFIED mykernel/printk/printk.c::log_end]

```c
static uint log_end;	/* Index into log_buf: most-recently-written-char + 1 */
```

### PK94-E047 | logged_chars：static uint	logged_chars;

[VERIFIED mykernel/printk/printk.c::logged_chars]

```c
static uint	logged_chars; /* Number of chars produced since last read+clear operation */
```

### PK94-E048 | LOG_BUF_MASK：(log_buf_len-1)

[VERIFIED mykernel/printk/printk.c::LOG_BUF_MASK]

```c
#define LOG_BUF_MASK	(log_buf_len-1)
```

### PK94-E049 | LOG_BUF：log_buf[(idx) & LOG_BUF_MASK]

[VERIFIED mykernel/printk/printk.c::LOG_BUF]

```c
#define LOG_BUF(idx)	(log_buf[(idx) & LOG_BUF_MASK])
```

### PK94-E050 | log_prefix：if (p[0] != '<' || !p[1])

[VERIFIED mykernel/printk/printk.c::log_prefix]

```c
	if (p[0] != '<' || !p[1])
		return 0;
```

### PK94-E051 | log_prefix：if (p[2] == '>')

[VERIFIED mykernel/printk/printk.c::log_prefix]

```c
	if (p[2] == '>') {
		/* usual single digit level number or special char */
		switch (p[1]) {
		case '0' ... '7':
			lev = p[1] - '0';
```

### PK94-E052 | log_prefix：case 'c': /* KERN_CONT */

[VERIFIED mykernel/printk/printk.c::log_prefix]

```c
		case 'c': /* KERN_CONT */
		case 'd': /* KERN_DEFAULT */
			sp = p[1];
			break;
```

### PK94-E053 | log_prefix：if (p[1] < '0' && p[1] > '9')

[VERIFIED mykernel/printk/printk.c::log_prefix]

```c
		if (p[1] < '0' && p[1] > '9')
			return 0;
```

### PK94-E054 | log_prefix：lev = (simple_strtoul(&p[1], &endp, 10) & 7);

[VERIFIED mykernel/printk/printk.c::log_prefix]

```c
		lev = (simple_strtoul(&p[1], &endp, 10) & 7);
		if (endp == NULL || endp[0] != '>')
			return 0;
		len = (endp + 1) - p;
```

### PK94-E055 | log_prefix：if (sp && !special)

[VERIFIED mykernel/printk/printk.c::log_prefix]

```c
	if (sp && !special)
		return 0;
```

### PK94-E056 | log_prefix：*special = sp;

[VERIFIED mykernel/printk/printk.c::log_prefix]

```c
		*special = sp;
		/* return special char, do not touch level */
		if (sp)
			return len;
```

### PK94-E057 | log_prefix：*level = lev;

[VERIFIED mykernel/printk/printk.c::log_prefix]

```c
		*level = lev;
	return len;
```

### PK94-E058 | simple_strntoull：cp = _parse_integer_fixup_radix(startp, &base);

[VERIFIED mykernel/lib/printf/vsprintf.c::simple_strntoull]

```c
	cp = _parse_integer_fixup_radix(startp, &base);
	prefix_chars = cp - startp;
	if (prefix_chars < max_chars) {
		rv = _parse_integer_limit(cp, base, &result, max_chars - prefix_chars);
		/* FIXME */
```

### PK94-E059 | simple_strntoull：cp += (rv & ~KSTRTOX_OVERFLOW);

[VERIFIED mykernel/lib/printf/vsprintf.c::simple_strntoull]

```c
		cp += (rv & ~KSTRTOX_OVERFLOW);
	} else {
		/* Field too short for prefix + digit, skip over without converting */
```

### PK94-E060 | simple_strntoull：*endp = (char *)cp;

[VERIFIED mykernel/lib/printf/vsprintf.c::simple_strntoull]

```c
		*endp = (char *)cp;

	return result;
```

### PK94-E061 | simple_strtoull：return simple_strntoull(cp, INT_MAX, endp, base);

[VERIFIED mykernel/lib/printf/vsprintf.c::simple_strtoull]

```c
	return simple_strntoull(cp, INT_MAX, endp, base);
```

### PK94-E062 | simple_strtoul：return simple_strtoull(cp, endp, base);

[VERIFIED mykernel/lib/printf/vsprintf.c::simple_strtoul]

```c
	return simple_strtoull(cp, endp, base);
```

### PK94-E063 | _parse_integer_fixup_radix：if (*base == 0)

[VERIFIED mykernel/lib/digit/kstrtox.c::_parse_integer_fixup_radix]

```c
	if (*base == 0) {
		if (s[0] == '0') {
			if (_tolower(s[1]) == 'x' && isxdigit(s[2]))
				*base = 16;
```

### PK94-E064 | _parse_integer_fixup_radix：return s;

[VERIFIED mykernel/lib/digit/kstrtox.c::_parse_integer_fixup_radix]

```c
	return s;
```

### PK94-E065 | _parse_integer_limit：rv = 0;

[VERIFIED mykernel/lib/digit/kstrtox.c::_parse_integer_limit]

```c
	rv = 0;
	while (max_chars--) {
		unsigned int c = *s;
		unsigned int lc = c | 0x20; /* don't tolower() this line */
		unsigned int val;
```

### PK94-E066 | _parse_integer_limit：if ('0' <= c && c <= '9')

[VERIFIED mykernel/lib/digit/kstrtox.c::_parse_integer_limit]

```c
		if ('0' <= c && c <= '9')
			val = c - '0';
		else if ('a' <= lc && lc <= 'f')
			val = lc - 'a' + 10;
		else
```

### PK94-E067 | _parse_integer_limit：if (val >= base)

[VERIFIED mykernel/lib/digit/kstrtox.c::_parse_integer_limit]

```c
		if (val >= base)
			break;
```

### PK94-E068 | _parse_integer_limit：*p = res;

[VERIFIED mykernel/lib/digit/kstrtox.c::_parse_integer_limit]

```c
	*p = res;
	return rv;
```

### PK94-E069 | KSTRTOX_OVERFLOW：(1U << 31)

[VERIFIED mykernel/lib/kstrtox.h::KSTRTOX_OVERFLOW]

```c
	#define KSTRTOX_OVERFLOW	(1U << 31)
```

### PK94-E070 | emit_log_char：LOG_BUF(log_end) = c;

[VERIFIED mykernel/printk/printk.c::emit_log_char]

```c
	LOG_BUF(log_end) = c;
	log_end++;
```

### PK94-E071 | emit_log_char：if (log_end - log_start > log_buf_len)

[VERIFIED mykernel/printk/printk.c::emit_log_char]

```c
	if (log_end - log_start > log_buf_len)
		log_start = log_end - log_buf_len;
	if (log_end - con_start > log_buf_len)
		con_start = log_end - log_buf_len;
```

### PK94-E072 | emit_log_char：if (logged_chars < log_buf_len)

[VERIFIED mykernel/printk/printk.c::emit_log_char]

```c
	if (logged_chars < log_buf_len)
		logged_chars++;
```

### PK94-E073 | console_list_hdr：DECLARE_LIST_HDR_S(console_list_hdr);

[VERIFIED mykernel/printk/printk.c::console_list_hdr]

```c
DECLARE_LIST_HDR_S(console_list_hdr);
```

### PK94-E074 | logbuf_lock：static DEFINE_SPINLOCK(logbuf_lock);

[VERIFIED mykernel/printk/printk.c::logbuf_lock]

```c
static DEFINE_SPINLOCK(logbuf_lock);
```

### PK94-E075 | console_locked：static int console_locked, console_suspended;

[VERIFIED mykernel/printk/printk.c::console_locked]

```c
static int console_locked, console_suspended;
```

### PK94-E076 | can_use_console：return (console_list_hdr.count > 0);

[VERIFIED mykernel/printk/printk.c::can_use_console]

```c
	return (console_list_hdr.count > 0);
```

### PK94-E077 | console_trylock_for_printk：// if (console_trylock()) {

[VERIFIED mykernel/printk/printk.c::console_trylock_for_printk]

```c
	// if (console_trylock()) {
		retval = 1;

		// /*
```

### PK94-E078 | console_trylock_for_printk：if (!can_use_console(cpu))

[VERIFIED mykernel/printk/printk.c::console_trylock_for_printk]

```c
		if (!can_use_console(cpu)) {
			console_locked = 0;
			// up(&console_sem);
			retval = 0;
```

### PK94-E079 | console_trylock_for_printk：return retval;

[VERIFIED mykernel/printk/printk.c::console_trylock_for_printk]

```c
	return retval;
```

### PK94-E080 | console_unlock：// spin_lock_irqsave(&logbuf_lock, flags);

[VERIFIED mykernel/printk/printk.c::console_unlock]

```c
		// spin_lock_irqsave(&logbuf_lock, flags);
		wake_klogd |= log_start - log_end;
		if (con_start == log_end)
			break;			/* Nothing to print */
```

### PK94-E081 | console_unlock：if (con_start == log_end)

[VERIFIED mykernel/printk/printk.c::console_unlock]

```c
		if (con_start == log_end)
			break;			/* Nothing to print */
		_con_start = con_start;
		_log_end = log_end;
		con_start = log_end;		/* Flush */
```

### PK94-E082 | console_unlock：con_start = log_end;

[VERIFIED mykernel/printk/printk.c::console_unlock]

```c
		con_start = log_end;		/* Flush */
		// spin_unlock(&logbuf_lock);
		// stop_critical_timings();	/* don't trace print latency */
		call_console_drivers(_con_start, _log_end);
```

### PK94-E083 | console_unlock：// up(&console_sem);

[VERIFIED mykernel/printk/printk.c::console_unlock]

```c
	// up(&console_sem);
	// spin_unlock_irqrestore(&logbuf_lock, flags);
	// if (wake_klogd)
	// 	wake_up_klogd();
```

### PK94-E084 | call_console_drivers：static int msg_level = -1;

[VERIFIED mykernel/printk/printk.c::call_console_drivers]

```c
	static int msg_level = -1;

	BUG_ON(((int)(start - end)) > 0);

```

### PK94-E085 | call_console_drivers：cur_index += log_prefix(&LOG_BUF(cur_index), &msg_level, N

[VERIFIED mykernel/printk/printk.c::call_console_drivers]

```c
			cur_index += log_prefix(&LOG_BUF(cur_index), &msg_level, NULL);
			start_print = cur_index;
		}
```

### PK94-E086 | call_console_drivers：char c = LOG_BUF(cur_index);

[VERIFIED mykernel/printk/printk.c::call_console_drivers]

```c
			char c = LOG_BUF(cur_index);

			cur_index++;
```

### PK94-E087 | call_console_drivers：_call_console_drivers(start_print, cur_index, msg_level);

[VERIFIED mykernel/printk/printk.c::call_console_drivers]

```c
				_call_console_drivers(start_print, cur_index, msg_level);
				msg_level = -1;
				start_print = cur_index;
				break;
```

### PK94-E088 | call_console_drivers：// _call_console_drivers(start_print, end, msg_level);

[VERIFIED mykernel/printk/printk.c::call_console_drivers]

```c
	// _call_console_drivers(start_print, end, msg_level);
```

### PK94-E089 | _call_console_drivers：if ((start & LOG_BUF_MASK) > (end & LOG_BUF_MASK))

[VERIFIED mykernel/printk/printk.c::_call_console_drivers]

```c
		if ((start & LOG_BUF_MASK) > (end & LOG_BUF_MASK)) {
			/* wrapped write */
			__call_console_drivers(start & LOG_BUF_MASK,
						log_buf_len);
```

### PK94-E090 | _call_console_drivers：__call_console_drivers(0, end & LOG_BUF_MASK);

[VERIFIED mykernel/printk/printk.c::_call_console_drivers]

```c
			__call_console_drivers(0, end & LOG_BUF_MASK);
		} else {
			__call_console_drivers(start, end);
```

### PK94-E091 | _call_console_drivers：__call_console_drivers(start, end);

[VERIFIED mykernel/printk/printk.c::_call_console_drivers]

```c
			__call_console_drivers(start, end);
		}
```

### PK94-E092 | __call_console_drivers：for_each_console(con)

[VERIFIED mykernel/printk/printk.c::__call_console_drivers]

```c
	for_each_console(con) {
		// if (exclusive_console && con != exclusive_console)
		// 	continue;
		// if ((con->flags & CON_ENABLED) && con->write &&
		// 		(cpu_online(smp_processor_id()) ||
```

### PK94-E093 | __call_console_drivers：con->write(con, &LOG_BUF(start), end - start);

[VERIFIED mykernel/printk/printk.c::__call_console_drivers]

```c
			con->write(con, &LOG_BUF(start), end - start);
	}
```

### PK94-E094 | for_each_console：#define for_each_console(con)

[VERIFIED mykernel/include/linux/device/console.h::for_each_console]

```c
	#define for_each_console(con)						\
			list_header_for_each_container(						\
```

### PK94-E095 | console：(*write)(console_s *co,

[VERIFIED mykernel/include/linux/device/console.h::console]

```c
		void			(*write)(console_s *co, const char *s, unsigned int count);
```

### PK94-E096 | local_clock：return native_sched_clock();

[VERIFIED mykernel/include/linux/sched/clock.h::local_clock]

```c
		return native_sched_clock();
```

### PK94-E097 | do_div：__rem = ((uint64_t)(n)) % __base;

[VERIFIED mykernel/include/linux/kernel/asm-generic/div64.h::do_div]

```c
				__rem = ((uint64_t)(n)) % __base;	\
				(n) = ((uint64_t)(n)) / __base;		\
				__rem;								\
```

### PK94-E098 | vscnprintf：i = vsnprintf(buf, size, fmt, args);

[VERIFIED mykernel/lib/printf/vsprintf.c::vscnprintf]

```c
	i = vsnprintf(buf, size, fmt, args);

	if (likely(i < size))
		return i;
	if (size != 0)
```

### PK94-E099 | vsnprintf：if (WARN_ON_ONCE(size > INT_MAX))

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
	if (WARN_ON_ONCE(size > INT_MAX))
		return 0;
```

### PK94-E100 | vsnprintf：if (size > 0) {

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
	if (size > 0) {
		if (str < end)
			*str = '\0';
		else
			end[-1] = '\0';
```

### PK94-E101 | sprintf：i = vsnprintf(buf, INT_MAX, fmt, args);

[VERIFIED mykernel/lib/printf/vsprintf.c::sprintf]

```c
	i = vsnprintf(buf, INT_MAX, fmt, args);
	va_end(args);

	return i;
```

完整本地机械P9 = NOT_RUN。没有将条件数学字节串转换成实际C编译或运行证明。
