---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PRINTF-INTEGER-FORMAT-92
record_type: fixed_time_integer_formatting_source_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-12
produced_by: GPT-6 (visible family; backend UNATTESTED)
model_per_owner: GPT-6 + Extra
date: 2026-11-16
base_snapshot: "time（分支名）"
read_channel: connector
status: STATIC_EVIDENCE_PENDING_C06_REVIEW
runtime_evidence: NOT_RUN
---

# 第92批源证据短锚

所有新引文均为固定 time@a039d9803ade 源文件中一个真实宏、枚举、结构、数组对象或函数定义内部的连续1–5行原文。每条短锚仅对指定局部事实有效；完整结构化模型保留类型、临时存储、指针算术及失败止证门。一个宏单列一锚，缩写或注释说明不冒充外部标准/运行检验。源码未编译、执行或实测。

### PF92-E001 | 数字格式标志SIGN

[VERIFIED mykernel/lib/printf/vsprintf.c::SIGN]

```c
#define SIGN		1		/* unsigned/signed, must be 1 */
```

### PF92-E002 | 数字格式标志LEFT

[VERIFIED mykernel/lib/printf/vsprintf.c::LEFT]

```c
#define LEFT		2		/* left justified */
```

### PF92-E003 | 数字格式标志PLUS

[VERIFIED mykernel/lib/printf/vsprintf.c::PLUS]

```c
#define PLUS		4		/* show plus */
```

### PF92-E004 | 数字格式标志SPACE

[VERIFIED mykernel/lib/printf/vsprintf.c::SPACE]

```c
#define SPACE		8		/* space if plus */
```

### PF92-E005 | 数字格式标志ZEROPAD

[VERIFIED mykernel/lib/printf/vsprintf.c::ZEROPAD]

```c
#define ZEROPAD		16		/* pad with zero, must be 16 == '0' - ' ' */
```

### PF92-E006 | 数字格式标志SMALL

[VERIFIED mykernel/lib/printf/vsprintf.c::SMALL]

```c
#define SMALL		32		/* use lowercase in hex (must be 32 == 0x20) */
```

### PF92-E007 | 数字格式标志SPECIAL

[VERIFIED mykernel/lib/printf/vsprintf.c::SPECIAL]

```c
#define SPECIAL		64		/* prefix hex with "0x", octal with "0" */
```

### PF92-E008 | 格式类型枚举前部

[VERIFIED mykernel/lib/printf/vsprintf.c::format_type]

```c
	FORMAT_TYPE_NONE, /* Just a string part */
	FORMAT_TYPE_WIDTH,
	FORMAT_TYPE_PRECISION,
	FORMAT_TYPE_CHAR,
	FORMAT_TYPE_STR,
```

### PF92-E009 | 数字资格类型枚举

[VERIFIED mykernel/lib/printf/vsprintf.c::format_type]

```c
	FORMAT_TYPE_LONG_LONG,
	FORMAT_TYPE_ULONG,
	FORMAT_TYPE_LONG,
	FORMAT_TYPE_UBYTE,
	FORMAT_TYPE_BYTE,
```

### PF92-E010 | type/field_width/flags/base位域

[VERIFIED mykernel/lib/printf/vsprintf.c::printf_spec]

```c
struct printf_spec {
	unsigned int	type:8;		/* format_type enum */
	signed int	field_width:24;	/* width of output field */
	unsigned int	flags:8;	/* flags to number() */
	unsigned int	base:8;		/* number base, 8, 10 or 16 only */
```

### PF92-E011 | precision为signed16位域

[VERIFIED mykernel/lib/printf/vsprintf.c::printf_spec]

```c
	signed int	precision:16;	/* # of digits/chars */
```

### PF92-E012 | 24位宽度上限

[VERIFIED mykernel/lib/printf/vsprintf.c::FIELD_WIDTH_MAX]

```c
#define FIELD_WIDTH_MAX	((1 << 23) - 1)
```

### PF92-E013 | 16位精度上限

[VERIFIED mykernel/lib/printf/vsprintf.c::PRECISION_MAX]

```c
#define PRECISION_MAX	((1 << 15) - 1)
```

### PF92-E014 | 数位十进制累加与下个digit判断

[VERIFIED mykernel/lib/printf/vsprintf.c::skip_atoi]

```c
		i = i*10 + *((*s)++) - '0';
	} while (isdigit(**s));

```

### PF92-E015 | 实际isdigit范围

[VERIFIED mykernel/include/linux/kernel/ctype.h::isdigit]

```c
isdigit(int c) {
			return '0' <= c && c <= '9';
		}
```

### PF92-E016 | 资格l/L判定助手

[VERIFIED mykernel/include/linux/kernel/ctype.h::_tolower]

```c
_tolower(const char c) {
		return c | 0x20;
	}
```

### PF92-E017 | WIDTH状态负数宽度处理

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	if (spec->type == FORMAT_TYPE_WIDTH) {
		if (spec->field_width < 0) {
			spec->field_width = -spec->field_width;
			spec->flags |= LEFT;
		}
```

### PF92-E018 | WIDTH恢复后跳precision

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
		spec->type = FORMAT_TYPE_NONE;
		goto precision;
```

### PF92-E019 | PRECISION状态负数改零

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	if (spec->type == FORMAT_TYPE_PRECISION) {
		if (spec->precision < 0)
			spec->precision = 0;

		spec->type = FORMAT_TYPE_NONE;
```

### PF92-E020 | 普通状态归NONE后字面扫描

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	/* By default */
	spec->type = FORMAT_TYPE_NONE;

	for (; *fmt ; ++fmt) {
		if (*fmt == '%')
```

### PF92-E021 | 字面扫描直接返回read

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	if (fmt != start || !*fmt)
		return fmt - start;
```

### PF92-E022 | 标志重置与循环扫描

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	spec->flags = 0;

	while (1) { /* this also skips first '%' */
		bool found = true;

```

### PF92-E023 | LEFT/PLUS/SPACE/SPECIAL标志位

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
		case '-': spec->flags |= LEFT;    break;
		case '+': spec->flags |= PLUS;    break;
		case ' ': spec->flags |= SPACE;   break;
		case '#': spec->flags |= SPECIAL; break;
```

### PF92-E024 | ZEROPAD标志位

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
		case '0': spec->flags |= ZEROPAD; break;
		default:  found = false;
```

### PF92-E025 | 宽度字面或星号分支前置

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	spec->field_width = -1;

	if (isdigit(*fmt))
		spec->field_width = skip_atoi(&fmt);
	else if (*fmt == '*') {
```

### PF92-E026 | WIDTH解析提前返回

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
		spec->type = FORMAT_TYPE_WIDTH;
		return ++fmt - start;
```

### PF92-E027 | precision字面和星号门

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	spec->precision = -1;
	if (*fmt == '.') {
		++fmt;
		if (isdigit(*fmt)) {
			spec->precision = skip_atoi(&fmt);
```

### PF92-E028 | PRECISION解析提前返回

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
			spec->type = FORMAT_TYPE_PRECISION;
			return ++fmt - start;
```

### PF92-E029 | 整数资格字母读取和重复字母

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	if (*fmt == 'h' || _tolower(*fmt) == 'l' ||
	    *fmt == 'z' || *fmt == 't') {
		qualifier = *fmt++;
		if (unlikely(qualifier == *fmt)) {
			if (qualifier == 'l') {
```

### PF92-E030 | ll重复资格约定

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
				qualifier = 'L';
				++fmt;
			} else if (qualifier == 'h') {
				qualifier = 'H';
```

### PF92-E031 | 转换前默认base、字符及字符串分支

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	spec->base = 10;
	switch (*fmt) {
	case 'c':
		spec->type = FORMAT_TYPE_CHAR;
		return ++fmt - start;
```

### PF92-E032 | 八进制base8

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	case 'o':
		spec->base = 8;
		break;
```

### PF92-E033 | 小写x与大写X的SMALL/base16

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	case 'x':
		spec->flags |= SMALL;
		fallthrough;

	case 'X':
```

### PF92-E034 | d/i有符号和u路径

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	case 'd':
	case 'i':
		spec->flags |= SIGN;
		break;
	case 'u':
```

### PF92-E035 | 不支持格式invalid停点

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
		spec->type = FORMAT_TYPE_INVALID;
		return fmt - start;
```

### PF92-E036 | 长整型资格映射

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	if (qualifier == 'L')
		spec->type = FORMAT_TYPE_LONG_LONG;
	else if (qualifier == 'l') {
		BUILD_BUG_ON(FORMAT_TYPE_ULONG + SIGN != FORMAT_TYPE_LONG);
		spec->type = FORMAT_TYPE_ULONG + (spec->flags & SIGN);
```

### PF92-E037 | z和t资格映射

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	} else if (qualifier == 'z') {
		spec->type = FORMAT_TYPE_SIZE_T;
	} else if (qualifier == 't') {
		spec->type = FORMAT_TYPE_PTRDIFF;
	} else if (qualifier == 'H') {
```

### PF92-E038 | hh/h资格映射

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
	} else if (qualifier == 'H') {
		BUILD_BUG_ON(FORMAT_TYPE_UBYTE + SIGN != FORMAT_TYPE_BYTE);
		spec->type = FORMAT_TYPE_UBYTE + (spec->flags & SIGN);
	} else if (qualifier == 'h') {
		BUILD_BUG_ON(FORMAT_TYPE_USHORT + SIGN != FORMAT_TYPE_SHORT);
```

### PF92-E039 | 普通uint/int类型及完成消费

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]

```c
		spec->type = FORMAT_TYPE_UINT + (spec->flags & SIGN);
	}

```

### PF92-E040 | 动态宽度位域截断及clamp

[VERIFIED mykernel/lib/printf/vsprintf.c::set_field_width]

```c
	spec->field_width = width;
	if (WARN_ONCE(spec->field_width != width, "field width %d too large", width)) {
		spec->field_width = clamp(width, -FIELD_WIDTH_MAX, FIELD_WIDTH_MAX);
	}
}
```

### PF92-E041 | 动态精度位域截断及clamp

[VERIFIED mykernel/lib/printf/vsprintf.c::set_precision]

```c
	spec->precision = prec;
	if (WARN_ONCE(spec->precision != prec, "precision %d too large", prec)) {
		spec->precision = clamp(prec, 0, PRECISION_MAX);
	}
}
```

### PF92-E042 | 当前可用WARN_ONCE返回条件原值

[VERIFIED mykernel/include/linux/kernel/asm-generic/bug.h::WARN_ONCE]

```c
	#		define WARN_ONCE(condition, format...)	(condition)
```

### PF92-E043 | clamp宏范围动作

[VERIFIED mykernel/include/linux/kernel/minmax.h::clamp]

```c
	#define clamp(val, lo, hi)	min((typeof(val))max(val, lo), hi)
```

### PF92-E044 | 变参实际builtin读取宏

[VERIFIED mykernel/include/linux/lib/stdarg.h::va_arg]

```c
	#define va_arg(v, T)	__builtin_va_arg(v, T)
```

### PF92-E045 | 每次新调用spec初值和size拒绝门

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
	struct printf_spec spec = {0};

	/* Reject out-of-range values early.  Large positive sizes are
	   used for unknown buffer sizes. */
	if (WARN_ON_ONCE(size > INT_MAX))
```

### PF92-E046 | buf与end直接指针运算

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
	str = buf;
	end = buf + size;
```

### PF92-E047 | 解析器返回read及状态分派

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
		int read = format_decode(fmt, &spec);

		fmt += read;

		switch (spec.type) {
```

### PF92-E048 | 字面条件写入copy限制

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
		case FORMAT_TYPE_NONE: {
			int copy = read;
			if (str < end) {
				if (copy > end - str)
					copy = end - str;
```

### PF92-E049 | 逻辑指针即便截断也推进

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
			str += read;
			break;
```

### PF92-E050 | 取动态宽度int

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
		case FORMAT_TYPE_WIDTH:
			set_field_width(&spec, va_arg(args, int));
			break;
```

### PF92-E051 | 取动态精度int

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
		case FORMAT_TYPE_PRECISION:
			set_precision(&spec, va_arg(args, int));
			break;
```

### PF92-E052 | 百分号单字节输出

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
		case FORMAT_TYPE_PERCENT_CHAR:
			if (str < end)
				*str = '%';
			++str;
```

### PF92-E053 | invalid格式的goto out前置

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
		case FORMAT_TYPE_INVALID:
			/*
			 * Presumably the arguments passed gcc's type
			 * checking, but there is no safe or sane way
			 * for us to continue parsing the format and
```

### PF92-E054 | invalid中断解析

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
			goto out;
```

### PF92-E055 | ll资格按signed long long取参

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
			case FORMAT_TYPE_LONG_LONG:
				num = va_arg(args, long long);
				break;
```

### PF92-E056 | long unsigned和signed分别取参

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
			case FORMAT_TYPE_ULONG:
				num = va_arg(args, unsigned long);
				break;
			case FORMAT_TYPE_LONG:
				num = va_arg(args, long);
```

### PF92-E057 | z资格按SIGN分size_t/ssize_t

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
			case FORMAT_TYPE_SIZE_T:
				if (spec.flags & SIGN)
					num = va_arg(args, ssize_t);
				else
					num = va_arg(args, size_t);
```

### PF92-E058 | t资格一律ptrdiff_t

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
			case FORMAT_TYPE_PTRDIFF:
				num = va_arg(args, ptrdiff_t);
				break;
```

### PF92-E059 | hh unsigned/signed均va_arg int再窄化

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
			case FORMAT_TYPE_UBYTE:
				num = (unsigned char) va_arg(args, int);
				break;
			case FORMAT_TYPE_BYTE:
				num = (signed char) va_arg(args, int);
```

### PF92-E060 | h unsigned/signed窄化

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
			case FORMAT_TYPE_USHORT:
				num = (unsigned short) va_arg(args, int);
				break;
			case FORMAT_TYPE_SHORT:
				num = (short) va_arg(args, int);
```

### PF92-E061 | int/default unsigned参数读取

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
			case FORMAT_TYPE_INT:
				num = (int) va_arg(args, int);
				break;
			default:
				num = va_arg(args, unsigned int);
```

### PF92-E062 | 数字生成函数调用

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
			str = number(str, end, num, spec);
		}
```

### PF92-E063 | size非零的NUL位置分派

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
	if (size > 0) {
		if (str < end)
			*str = '\0';
		else
			end[-1] = '\0';
```

### PF92-E064 | 需求长度返回表达式

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]

```c
	return str-buf;
```

### PF92-E065 | decpair数组对象自身定义

[VERIFIED mykernel/lib/printf/vsprintf.c::decpair]

```c
static const u16 decpair[100] = {
```

### PF92-E066 | 数字对的单行生成宏

[VERIFIED mykernel/lib/printf/vsprintf.c::_]

```c
#define _(x) (__force u16) cpu_to_le16(((x % 10) | ((x / 10) << 8)) + 0x3030)
```

### PF92-E067 | decpair数组最后十个有效元素

[VERIFIED mykernel/lib/printf/vsprintf.c::decpair]

```c
	_(90), _(91), _(92), _(93), _(94), _(95), _(96), _(97), _(98), _(99),
```

### PF92-E068 | cpu_to_le16真实别名

[VERIFIED mykernel/include/linux/kernel/byteorder/generic.h::cpu_to_le16]

```c
	#define cpu_to_le16 __cpu_to_le16
```

### PF92-E069 | little_endian活动宏

[VERIFIED mykernel/include/uapi/linux/byteorder/little_endian.h::__cpu_to_le16]

```c
	#define __cpu_to_le16(x)			((__force __le16)(__u16)(x))
```

### PF92-E070 | 16字符大写HEX表

[VERIFIED mykernel/lib/digit/hexdump.c::hex_asc_upper]

```c
const char hex_asc_upper[] = "0123456789ABCDEF";
```

### PF92-E071 | trunc8快速分支

[VERIFIED mykernel/lib/printf/vsprintf.c::put_dec_trunc8]

```c
	if (r < 100)
		goto out_r;

	/* 100 <= r < 10^8 */
```

### PF92-E072 | trunc8首100商及两字节存储

[VERIFIED mykernel/lib/printf/vsprintf.c::put_dec_trunc8]

```c
	q = (r * (u64)0x28f5c29) >> 32;
	*((u16 *)buf) = decpair[r - 100*q];
	buf += 2;
```

### PF92-E073 | trunc8第二级100商

[VERIFIED mykernel/lib/printf/vsprintf.c::put_dec_trunc8]

```c
	r = (q * (u64)0x28f5c29) >> 32;
	*((u16 *)buf) = decpair[q - 100*r];
	buf += 2;
```

### PF92-E074 | trunc8较小商固定乘法

[VERIFIED mykernel/lib/printf/vsprintf.c::put_dec_trunc8]

```c
	q = (r * 0x147b) >> 19;
	*((u16 *)buf) = decpair[r - 100*q];
	buf += 2;
```

### PF92-E075 | trunc8最后一对写2字节但逻辑可只加1

[VERIFIED mykernel/lib/printf/vsprintf.c::put_dec_trunc8]

```c
	*((u16 *)buf) = decpair[r];
	buf += r < 10 ? 1 : 2;
	return buf;
```

### PF92-E076 | full8低两位对写入

[VERIFIED mykernel/lib/printf/vsprintf.c::put_dec_full8]

```c
	q = (r * (u64)0x28f5c29) >> 32;
	*((u16 *)buf) = decpair[r - 100*q];
	buf += 2;
```

### PF92-E077 | full8中两位对写入

[VERIFIED mykernel/lib/printf/vsprintf.c::put_dec_full8]

```c
	r = (q * (u64)0x28f5c29) >> 32;
	*((u16 *)buf) = decpair[q - 100*r];
	buf += 2;
```

### PF92-E078 | full8最后pair及8字节总游标

[VERIFIED mykernel/lib/printf/vsprintf.c::put_dec_full8]

```c
	*((u16 *)buf) = decpair[q];
	buf += 2;
	return buf;
```

### PF92-E079 | put_dec第一次1e8分段

[VERIFIED mykernel/lib/printf/vsprintf.c::put_dec]

```c
	if (n >= 100*1000*1000)
		buf = put_dec_full8(buf, do_div(n, 100*1000*1000));
	/* 1 <= n <= 1.6e11 */
```

### PF92-E080 | put_dec第二次1e8分段

[VERIFIED mykernel/lib/printf/vsprintf.c::put_dec]

```c
	/* 1 <= n <= 1.6e11 */
	if (n >= 100*1000*1000)
		buf = put_dec_full8(buf, do_div(n, 100*1000*1000));
```

### PF92-E081 | put_dec残值最后一段

[VERIFIED mykernel/lib/printf/vsprintf.c::put_dec]

```c
	return put_dec_trunc8(buf, n);
```

### PF92-E082 | do_div余数并更新被除数

[VERIFIED mykernel/include/linux/kernel/asm-generic/div64.h::do_div]

```c
				__rem = ((uint64_t)(n)) % __base;	\
				(n) = ((uint64_t)(n)) / __base;		\
				__rem;								\
```

### PF92-E083 | do_div宏起始和base局部类型

[VERIFIED mykernel/include/linux/kernel/asm-generic/div64.h::do_div]

```c
	#define do_div(n,base) ({						\
				uint32_t __base = (base);			\
				uint32_t __rem;						\
```

### PF92-E084 | number临时空间和前缀状态

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
	char tmp[3 * sizeof(num)] __aligned(2);
	char sign;
	char locase;
	int need_pfx = ((spec.flags & SPECIAL) && spec.base != 10);
```

### PF92-E085 | LEFT清除ZEROPAD与初始sign

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
	if (spec.flags & LEFT)
		spec.flags &= ~ZEROPAD;
	sign = 0;
	if (spec.flags & SIGN) {
```

### PF92-E086 | signed long long取负风险

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
			num = -(signed long long)num;
			field_width--;
```

### PF92-E087 | 正数PLUS/SPACE次序

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
		} else if (spec.flags & PLUS) {
			sign = '+';
			field_width--;
		} else if (spec.flags & SPACE) {
			sign = ' ';
```

### PF92-E088 | 前缀保留2列或oct1列

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
		if (spec.base == 16)
			field_width -= 2;
		else if (!is_zero)
			field_width--;
```

### PF92-E089 | 单个HEX digit源

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
	if (num < spec.base)
		tmp[i++] = hex_asc_upper[num] | locase;
	else if (spec.base != 10) { /* 8 or 16 */
		int mask = spec.base - 1;
```

### PF92-E090 | hex/oct位移与tmp逐字节倒序

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
			tmp[i++] = (hex_asc_upper[((unsigned char)num) & mask] | locase);
			num >>= shift;
		} while (num);
```

### PF92-E091 | decimal调用put_dec

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
		i = put_dec(tmp, num) - tmp;
	}
```

### PF92-E092 | width减已知数字长度与前导空格

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
	field_width -= precision;
	if (!(spec.flags & (ZEROPAD | LEFT))) {
		while (--field_width >= 0) {
			if (buf < end)
				*buf = ' ';
```

### PF92-E093 | 符号真实输出位置

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
	if (sign) {
		if (buf < end)
			*buf = sign;
		++buf;
	}
```

### PF92-E094 | 前缀的实际字符发射（非前部宽度预留）

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
	/* "0x" / "0" prefix */
	if (need_pfx) {
		if (spec.base == 16 || !is_zero) {
			if (buf < end)
				*buf = '0';
```

### PF92-E095 | 十六进制前缀大小写

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
				*buf = ('X' | locase);
			++buf;
		}
```

### PF92-E096 | ZEROPAD与空格的输出前序

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
		char c = ' ' + (spec.flags & ZEROPAD);

		while (--field_width >= 0) {
			if (buf < end)
				*buf = c;
```

### PF92-E097 | 零精度补齐在数字之前

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
	while (i <= --precision) {
		if (buf < end)
			*buf = '0';
		++buf;
	}
```

### PF92-E098 | 反向消费tmp并检查end

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
	while (--i >= 0) {
		if (buf < end)
			*buf = tmp[i];
		++buf;
	}
```

### PF92-E099 | 尾随空格发射的独立循环

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
	/* trailing space padding */
	while (--field_width >= 0) {
		if (buf < end)
			*buf = ' ';
		++buf;
```

### PF92-E100 | number返回逻辑需求末尾指针

[VERIFIED mykernel/lib/printf/vsprintf.c::number]

```c
	return buf;
```

### PF92-E101 | num_to_str局部24字节tmp

[VERIFIED mykernel/lib/printf/vsprintf.c::num_to_str]

```c
	char tmp[sizeof(num) * 3] __aligned(2);
	int idx, len;

	/* put_dec() may work incorrectly for num = 0 (generate "", not "0") */
```

### PF92-E102 | 0..9特例

[VERIFIED mykernel/lib/printf/vsprintf.c::num_to_str]

```c
	if (num <= 9) {
		tmp[0] = '0' + num;
		len = 1;
	} else {
		len = put_dec(tmp, num) - tmp;
```

### PF92-E103 | 缓冲和宽度检查

[VERIFIED mykernel/lib/printf/vsprintf.c::num_to_str]

```c
	if (len > size || width > size)
		return 0;
```

### PF92-E104 | num_to_str前导空格写入

[VERIFIED mykernel/lib/printf/vsprintf.c::num_to_str]

```c
	if (width > len) {
		width = width - len;
		for (idx = 0; idx < width; idx++)
			buf[idx] = ' ';
```

### PF92-E105 | num_to_str复制倒序且返回写入字符数

[VERIFIED mykernel/lib/printf/vsprintf.c::num_to_str]

```c
	for (idx = 0; idx < len; ++idx)
		buf[idx + width] = tmp[len - idx - 1];

	return len + width;
```

### PF92-E106 | vscnprintf截断返回逻辑

[VERIFIED mykernel/lib/printf/vsprintf.c::vscnprintf]

```c
	i = vsnprintf(buf, size, fmt, args);

	if (likely(i < size))
		return i;
	if (size != 0)
```

### PF92-E107 | snprintf返回需求长度

[VERIFIED mykernel/lib/printf/vsprintf.c::snprintf]

```c
	i = vsnprintf(buf, size, fmt, args);
	va_end(args);

	return i;
```

### PF92-E108 | scnprintf返回写入长度

[VERIFIED mykernel/lib/printf/vsprintf.c::scnprintf]

```c
	i = vscnprintf(buf, size, fmt, args);
	va_end(args);

	return i;
```

### PF92-E109 | worker实际十进制消费者

[VERIFIED mykernel/kactive/workqueue/workqueue.c::format_worker_id]

```c
			return scnprintf(buf, size, "kworker/%d:%d%s",
					pool->cpu, worker->id,
					pool->attrs->nice < 0  ? "H" : "");
```

### PF92-E110 | worker调用未消费返回值

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
		format_worker_id(id_buf, sizeof(id_buf), worker, pool);
		worker->task = kthread_create_on_node(simple_worker_thread,
							worker, pool->node, "%s", id_buf);
```


本批所有条件性输出、数值和偏序均以 facts 的前置条件及最早未证边界为准；不代表完整C ABI证明或外部printf标准兼容认证。
