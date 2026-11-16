---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PRINTF-STRING-POINTER-93
record_type: static_source_anchors
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-12
produced_by: GPT-6 (visible family; backend UNATTESTED)
model_per_owner: GPT-6 + Extra
date: 2026-11-16
base_snapshot: time（分支名）
read_channel: connector
status: STATIC_AUTHOR_EVIDENCE_PENDING_C06_PRO
runtime_evidence: NOT_RUN
---

# 固定time源码证据
每条短锚对应一个真实定义内的连续1–5行。注释的代码仅用于证明原作者意图与活动源码之间的区别，不能当活动调用。条件分析还需结合facts中的函数全体、对象和类型门。
### SP93-E001 | NULL参数的替代字面

[VERIFIED mykernel/lib/printf/vsprintf.c::check_pointer_msg]

```c
	if (!ptr)
		return "(null)";

```

### SP93-E002 | 低地址和ERR_PTR类替代

[VERIFIED mykernel/lib/printf/vsprintf.c::check_pointer_msg]

```c
	if ((ulong)ptr < PAGE_SIZE || IS_ERR_VALUE(ptr))
		return "(efault)";
```

### SP93-E003 | 未命中形状筛查的返回

[VERIFIED mykernel/lib/printf/vsprintf.c::check_pointer_msg]

```c
	return NULL;
```

### SP93-E004 | 错误占位后负内部状态

[VERIFIED mykernel/lib/printf/vsprintf.c::check_pointer]

```c
	err_msg = check_pointer_msg(ptr);
	if (err_msg) {
		*buf = error_string(*buf, end, err_msg, spec);
		return -EFAULT;
	}
```

### SP93-E005 | 没有本地替代时的状态

[VERIFIED mykernel/lib/printf/vsprintf.c::check_pointer]

```c
	return 0;
```

### SP93-E006 | 占位字符串默认精度限制

[VERIFIED mykernel/lib/printf/vsprintf.c::error_string]

```c
	if (spec.precision == -1)
		spec.precision = 2 * sizeof(void *);

	return string_nocheck(buf, end, s, spec);
```

### SP93-E007 | 占位后返回更新的输出指针

[VERIFIED mykernel/lib/printf/vsprintf.c::string]

```c
	if (check_pointer(&buf, end, s, spec))
		return buf;

	return string_nocheck(buf, end, s, spec);
```

### SP93-E008 | 字符串字符和精度初值

[VERIFIED mykernel/lib/printf/vsprintf.c::string_nocheck]

```c
	int len = 0;
	int lim = spec.precision;
```

### SP93-E009 | NUL与精度的读取循环

[VERIFIED mykernel/lib/printf/vsprintf.c::string_nocheck]

```c
	while (lim--) {
		char c = *s++;
		if (!c)
			break;
```

### SP93-E010 | 目标物理写和逻辑游标推进

[VERIFIED mykernel/lib/printf/vsprintf.c::string_nocheck]

```c
		if (buf < end)
			*buf = c;
		++buf;
		++len;
```

### SP93-E011 | 读取后宽度补齐调用

[VERIFIED mykernel/lib/printf/vsprintf.c::string_nocheck]

```c
	return widen_string(buf, len, end, spec);
```

### SP93-E012 | PAGE_SIZE单独宏

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]

```c
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```

### SP93-E013 | PAGE_SHIFT单独宏

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT]

```c
	#define PAGE_SHIFT				12
```

### SP93-E014 | MAX_ERRNO单独宏

[VERIFIED mykernel/include/linux/kernel/err.h::MAX_ERRNO]

```c
	#define MAX_ERRNO	4095
```

### SP93-E015 | IS_ERR_VALUE单宏多行定义

[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR_VALUE]

```c
	#	define IS_ERR_VALUE(x) (								\
					(size_t)(void *)(x) >= (size_t)-MAX_ERRNO	\
				)
```

### SP93-E016 | 目标已满直接返回

[VERIFIED mykernel/lib/printf/vsprintf.c::move_right]

```c
	if (buf >= end)	/* nowhere to put anything */
		return;
	size = end - buf;
```

### SP93-E017 | 空格覆盖整个剩余容量

[VERIFIED mykernel/lib/printf/vsprintf.c::move_right]

```c
	if (size <= spaces) {
		memset(buf, ' ', size);
		return;
	}
```

### SP93-E018 | 搬移长度裁剪

[VERIFIED mykernel/lib/printf/vsprintf.c::move_right]

```c
	if (len) {
		if (len > size - spaces)
			len = size - spaces;
		memmove(buf + spaces, buf, len);
```

### SP93-E019 | 先搬移后填空格

[VERIFIED mykernel/lib/printf/vsprintf.c::move_right]

```c
		memmove(buf + spaces, buf, len);
	}
	memset(buf, ' ', spaces);
```

### SP93-E020 | 宽度已满足无动作

[VERIFIED mykernel/lib/printf/vsprintf.c::widen_string]

```c
	if (likely(n >= spec.field_width))
		return buf;
```

### SP93-E021 | 非LEFT右移的起点和返回

[VERIFIED mykernel/lib/printf/vsprintf.c::widen_string]

```c
	spaces = spec.field_width - n;
	if (!(spec.flags & LEFT)) {
		move_right(buf - n, end, n, spaces);
		return buf + spaces;
```

### SP93-E022 | LEFT尾部空格及逻辑推进

[VERIFIED mykernel/lib/printf/vsprintf.c::widen_string]

```c
	while (spaces--) {
		if (buf < end)
			*buf = ' ';
		++buf;
	}
```

### SP93-E023 | 通用memmove正向分支

[VERIFIED mykernel/lib/string/string.c::memmove]

```c
		if (dest <= src) {
			tmp = dest;
			s = src;
			while (count--)
				*tmp++ = *s++;
```

### SP93-E024 | 重叠目的在后时反向搬移

[VERIFIED mykernel/lib/string/string.c::memmove]

```c
		} else {
			tmp = dest;
			tmp += count;
			s = src;
			s += count;
```

### SP93-E025 | 反向真实读写语句

[VERIFIED mykernel/lib/string/string.c::memmove]

```c
				*--tmp = *--s;
		}
		return dest;
```

### SP93-E026 | 通用memset逐字节操作

[VERIFIED mykernel/lib/string/string.c::memset]

```c
		while (count--)
			*xs++ = c;
		return s;
```

### SP93-E027 | 普通字面直接返回长度

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	if (fmt != start || !*fmt)
		return fmt - start;
```

### SP93-E028 | 默认精度与点号分支

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	spec->precision = -1;
	if (*fmt == '.') {
		++fmt;
		if (isdigit(*fmt)) {
			spec->precision = skip_atoi(&fmt);
```

### SP93-E029 | 动态精度提前返回

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
			spec->type = FORMAT_TYPE_PRECISION;
			return ++fmt - start;
```

### SP93-E030 | 负动态精度及恢复

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	if (spec->type == FORMAT_TYPE_PRECISION) {
		if (spec->precision < 0)
			spec->precision = 0;

		spec->type = FORMAT_TYPE_NONE;
```

### SP93-E031 | 解析单独的c格式

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	case 'c':
		spec->type = FORMAT_TYPE_CHAR;
		return ++fmt - start;
```

### SP93-E032 | 解析单独的s格式

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	case 's':
		spec->type = FORMAT_TYPE_STR;
		return ++fmt - start;
```

### SP93-E033 | 解析单独的p格式

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	case 'p':
		spec->type = FORMAT_TYPE_PTR;
		return ++fmt - start;
```

### SP93-E034 | 解析单独的%格式

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	case '%':
		spec->type = FORMAT_TYPE_PERCENT_CHAR;
		return ++fmt - start;
```

### SP93-E035 | 容量拒绝直接返回0

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
	if (WARN_ON_ONCE(size > INT_MAX))
		return 0;
```

### SP93-E036 | 输出缓冲指针初始化

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
	str = buf;
	end = buf + size;
```

### SP93-E037 | 外层解析器调用和状态

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
		int read = format_decode(fmt, &spec);

		fmt += read;

		switch (spec.type) {
```

### SP93-E038 | 动态宽度取int

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
		case FORMAT_TYPE_WIDTH:
			set_field_width(&spec, va_arg(args, int));
			break;
```

### SP93-E039 | 动态精度取int

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
		case FORMAT_TYPE_PRECISION:
			set_precision(&spec, va_arg(args, int));
			break;
```

### SP93-E040 | 字符串分支char*实参

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
		case FORMAT_TYPE_STR:
			str = string(str, end, va_arg(args, char *), spec);
			break;
```

### SP93-E041 | 字符分支前导空格

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
		case FORMAT_TYPE_CHAR: {
			char c;

			if (!(spec.flags & LEFT)) {
				while (--spec.field_width > 0) {
```

### SP93-E042 | 字符int取参后unsigned char窄化

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
			c = (unsigned char) va_arg(args, int);
			if (str < end)
				*str = c;
			++str;
```

### SP93-E043 | 字符写入之后的尾随补齐循环

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
			while (--spec.field_width > 0) {
				if (str < end)
					*str = ' ';
				++str;
			}
```

### SP93-E044 | 百分号分支

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
		case FORMAT_TYPE_PERCENT_CHAR:
			if (str < end)
				*str = '%';
			++str;
			break;
```

### SP93-E045 | 指针返回赋给str的首次消费

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
		case FORMAT_TYPE_PTR:
			str = pointer(fmt, str, end, va_arg(args, void *),
				      spec);
			while (isalnum(*fmt))
				fmt++;
```

### SP93-E046 | 计划于pointer返回后扫描字母数字

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
			while (isalnum(*fmt))
				fmt++;
			break;
```

### SP93-E047 | 末尾NUL处理

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
	if (size > 0) {
		if (str < end)
			*str = '\0';
		else
			end[-1] = '\0';
```

### SP93-E048 | 最终逻辑差值作为返回

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
	return str-buf;
```

### SP93-E049 | pointer非void定义及注释体入口

[VERIFIED mykernel/lib/printf/vsprintf.c::pointer]

```c
char *pointer(const char *fmt, char *buf, char *end, void *ptr,
	      struct printf_spec spec)
{
```

### SP93-E050 | 原格式分派已注释

[VERIFIED mykernel/lib/printf/vsprintf.c::pointer]

```c
// 	switch (*fmt) {
// 	case 'S':
// 	case 's':
// 		ptr = dereference_symbol_descriptor(ptr);
```

### SP93-E051 | 被注释的默认返回与活动闭合

[VERIFIED mykernel/lib/printf/vsprintf.c::pointer]

```c
// 	default:
// 		return default_pointer(buf, end, ptr, spec);
// 	}
}
```

### SP93-E052 | ctype类别_U

[VERIFIED mykernel/include/linux/kernel/ctype.h::_U]

```c
	#define _U	0x01	/* upper */
```

### SP93-E053 | ctype类别_L

[VERIFIED mykernel/include/linux/kernel/ctype.h::_L]

```c
	#define _L	0x02	/* lower */
```

### SP93-E054 | ctype类别_D

[VERIFIED mykernel/include/linux/kernel/ctype.h::_D]

```c
	#define _D	0x04	/* digit */
```

### SP93-E055 | ctype类别_P

[VERIFIED mykernel/include/linux/kernel/ctype.h::_P]

```c
	#define _P	0x10	/* punct */
```

### SP93-E056 | ctype真实后缀处理宏__ismask

[VERIFIED mykernel/include/linux/kernel/ctype.h::__ismask]

```c
	#define __ismask(x) (_ctype[(int)(unsigned char)(x)])
```

### SP93-E057 | ctype真实后缀处理宏isalnum

[VERIFIED mykernel/include/linux/kernel/ctype.h::isalnum]

```c
	#define isalnum(c)	((__ismask(c)&(_U|_L|_D)) != 0)
```

### SP93-E058 | ASCII数码对应的ctype类别

[VERIFIED mykernel/lib/ctype.c::_ctype]

```c
_D,_D,_D,_D,_D,_D,_D,_D,				/* 48-55 */
_D,_D,_P,_P,_P,_P,_P,_P,				/* 56-63 */
```

### SP93-E059 | ASCII 96..103反引号与小写字母类别

[VERIFIED mykernel/lib/ctype.c::_ctype]

```c
_P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,		/* 96-103 */
_L,_L,_L,_L,_L,_L,_L,_L,				/* 104-111 */
```

### SP93-E060 | ASCII 88..95范围的下划线类别

[VERIFIED mykernel/lib/ctype.c::_ctype]

```c
_U,_U,_U,_P,_P,_P,_P,_P,				/* 88-95 */
```

### SP93-E061 | 剩余格式字面在外层的正常分支

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
		case FORMAT_TYPE_NONE: {
			int copy = read;
			if (str < end) {
				if (copy > end - str)
					copy = end - str;
```

### SP93-E062 | 不支持格式的本地分支

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
		case FORMAT_TYPE_INVALID:
			/*
			 * Presumably the arguments passed gcc's type
```

### SP93-E063 | worker错误日志%pe导航

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
		pr_err_once("workqueue: Failed to allocate a worker ID: %pe\n",
			    ERR_PTR(id));
```

### SP93-E064 | worker线程错误日志%pe导航

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
					"worker thread: %pe", worker->task);
```

所有仓库代码和脚本、编译、Graphviz、QEMU、实机、真实并发及完整本地P9均NOT_RUN。
