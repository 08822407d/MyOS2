# 第158批固定源短窗

固定 time@a039d9803ade。每个短窗连续1–5物理行，保留源CRLF/LF/NONE和tab。NONE的围栏分隔LF不算源尾。行号仅作普通定位。源事实不等于构建或运行；独立重开各源逐窗核同后才计P6。普通include路由见facts源账，不计正式窗。

## FC158-E001 · emit

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::EMIT]
```c
#define EMIT(x)			({ if(o<n) {*q++ = (x);} o++; })
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:39-39；blob短12 ef7e61a278cb；行尾 CRLF。

## FC158-E002 · zeroFlag

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::flags]
```c
	FL_ZERO				= 0x01,		/* Zero modifier */
	FL_MINUS			= 0x02,		/* Minus modifier */
	FL_PLUS				= 0x04,		/* Plus modifier */
	FL_TICK				= 0x08,		/* ' modifier */
	FL_SPACE			= 0x10,		/* Space modifier */
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:12-16；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E003 · signedFlag

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::flags]
```c
	FL_HASH				= 0x20,		/* # modifier */
	FL_SIGNED			= 0x40,		/* Number is signed */
	FL_UPPER			= 0x80,		/* Upper case digits */
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:17-19；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF。

## FC158-E004 · rank

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::ranks]
```c
	rank_short			= -1,
	rank_int			=  0,
	rank_long			=  1,
	rank_longlong		=  2,
};
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:27-31；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E005 · helperInit

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
static size_t format_int(char * q, size_t n, size_t val,
		enum flags flags, int base, int width, int prec)
{
	char * qq;
	size_t o = 0, oo;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:41-45；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E006 · digits

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	static const char ucdigits[] = "0123456789ABCDEF";
	const char * digits;
	size_t tmpval;
	int minus = 0;
	int ndigits = 0, nchars;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:47-51；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E007 · selectDigits

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	digits = (flags & FL_UPPER) ? ucdigits : lcdigits;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:57-57；blob短12 ef7e61a278cb；行尾 CRLF。

## FC158-E008 · sign

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	if ((flags & FL_SIGNED) && ((ssize_t)val < 0))
	{
		minus = 1;
		val = (size_t)(-(ssize_t)val);
	}
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:62-66；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E009 · count

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	tmpval = val;
	while (tmpval)
	{
		tmpval /= base;
		ndigits++;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:71-75；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E010 · octal

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	if ((flags & FL_HASH) && (base == 8))
	{
		if (prec < ndigits + 1)
			prec = ndigits + 1;
	}
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:81-85；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E011 · prec

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	if (ndigits < prec)
	{
		ndigits = prec;		/* Mandatory number padding */
	}
	else if (val == 0)
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:87-91；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E012 · zeroDigit

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	else if (val == 0)
	{
		ndigits = 1;		/* Zero still requires space */
	}
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:91-94；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF。

## FC158-E013 · tick

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	if (flags & FL_TICK)
	{
		tickskip = (base == 16) ? 4 : 3;
	}
	else
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:99-103；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E014 · noTick

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	else
	{
		tickskip = ndigits;	/* No tick marks */
	}
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:103-106；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF。

## FC158-E015 · tickCount

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	ndigits += (ndigits - 1) / tickskip;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:111-111；blob短12 ef7e61a278cb；行尾 CRLF。

## FC158-E016 · chars

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	nchars = ndigits;

	if (minus || (flags & (FL_PLUS | FL_SPACE)))
		nchars++;			/* Need space for sign */
	if ((flags & FL_HASH) && (base == 16))
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:116-120；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E017 · hashLen

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	if ((flags & FL_HASH) && (base == 16))
	{
		nchars += 2;		/* Add 0x for hex */
	}
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:120-123；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF。

## FC158-E018 · early

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	if (!(flags & (FL_MINUS | FL_ZERO)) && (width > nchars))
	{
		while (width > nchars)
		{
			EMIT(' ');
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:128-132；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E019 · earlyDec

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
		while (width > nchars)
		{
			EMIT(' ');
			width--;
		}
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:130-134；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E020 · signEmit

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	if (minus)
		EMIT('-');
	else if (flags & FL_PLUS)
		EMIT('+');
	else if (flags & FL_SPACE)
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:140-144；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E021 · spaceSign

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	else if (flags & FL_PLUS)
		EMIT('+');
	else if (flags & FL_SPACE)
		EMIT(' ');
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:142-145；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF。

## FC158-E022 · prefix

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	if ((flags & FL_HASH) && (base == 16))
	{
		EMIT('0');
		EMIT((flags & FL_UPPER) ? 'X' : 'x');
	}
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:147-151；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E023 · zeroPad

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	if (((flags & (FL_MINUS | FL_ZERO)) == FL_ZERO) && (width > ndigits))
	{
		while (width > nchars) {
			EMIT('0');
			width--;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:156-160；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E024 · qAdvance

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	q += ndigits;			/* Advance the pointer to end of number */
	o += ndigits;
	qq = q;
	oo = o;					/* Temporary values */
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:167-170；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF。

## FC158-E025 · revTick

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	b4tick = tickskip;
	while (ndigits > 0)
	{
		if (!b4tick--)
		{
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:172-176；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E026 · revSep

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
			qq--;
			oo--;
			ndigits--;
			if (oo < n)
				*qq = '_';
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:177-181；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E027 · revSepReset

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
			b4tick = tickskip - 1;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:182-182；blob短12 ef7e61a278cb；行尾 CRLF。

## FC158-E028 · rev

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
		qq--;
		oo--;
		ndigits--;
		if (oo < n)
			*qq = digits[val % base];
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:184-188；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E029 · divide

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
		val /= base;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:189-189；blob短12 ef7e61a278cb；行尾 CRLF。

## FC158-E030 · late

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	while ((flags & FL_MINUS) && (width > nchars))
	{
		EMIT(' ');
		width--;
	}
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:195-199；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E031 · helperReturn

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	return o;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:201-201；blob短12 ef7e61a278cb；行尾 CRLF。

## FC158-E032 · outerInit

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	const char * p = fmt;
	char ch;
	char *q = buf;
	size_t o = 0;			/* Number of characters output */
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:535-538；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF。

## FC158-E033 · outerFlags

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	size_t val = 0;
	int rank = rank_int;	/* Default rank */
	int width = 0;
	int prec = -1;
	int base;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:539-543；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E034 · parserState

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	enum {
		st_normal,		/* Ground state */
		st_flags,		/* Special flags */
		st_width,		/* Field width */
		st_prec,		/* Field precision */
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:546-550；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E035 · parserState2

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
		st_modifiers,	/* Length or conversion modifiers */
	} state = st_normal;
	const char * sarg;	/* %s string argument */
	char carg;			/* %c char argument */
	int slen;			/* String length */
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:551-555；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E036 · readFmt

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	while ((ch = *p++))
	{
		switch (state)
		{
		case st_normal:
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:557-561；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E037 · percent

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			if (ch == '%')
			{
				state = st_flags;
				flags = 0;
				rank = rank_int;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:562-566；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E038 · reset

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				flags = 0;
				rank = rank_int;
				width = 0;
				prec = -1;
			}
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:565-569；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E039 · literal

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			else
			{
				EMIT(ch);
			}
			break;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:570-574；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E040 · parseZero

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			case '0':
				flags |= FL_ZERO;
				break;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:594-596；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF。

## FC158-E041 · flagsFallback

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			default:
				state = st_width;
				p--;	/* Process this character again */
				break;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:597-600；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF。

## FC158-E042 · width

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			if (ch >= '0' && ch <= '9')
			{
				width = width * 10 + (ch - '0');
			}
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:605-608；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF。

## FC158-E043 · widthPrec

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			else if (ch == '.')
			{
				prec = 0;	/* Precision given */
				state = st_prec;
			}
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:618-622；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E044 · widthFallback

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			else
			{
				state = st_modifiers;
				p--;		/* Process this character again */
			}
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:623-627；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E045 · long

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			case 'l':
				rank++;		/* Longer rank */
				break;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:657-659；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF。

## FC158-E046 · normal

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
			default:
				/*
				 * Next state will be normal
				 */
				state = st_normal;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:673-677；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E047 · clampRank

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				if (rank < MIN_RANK)
					rank = MIN_RANK;
				else if (rank > MAX_RANK)
					rank = MAX_RANK;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:682-685；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF。

## FC158-E048 · signed

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				case 'd':		/* Signed decimal output */
				case 'i':
					base = 10;
					flags |= FL_SIGNED;
					switch (rank)
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:700-704；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E049 · signedLong

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
					case rank_long:
						val = (size_t)(ssize_t)
						va_arg(ap, signed long);
						break;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:721-724；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF。

## FC158-E050 · signedGoto

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
					}
					goto is_integer;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:730-731；blob短12 ef7e61a278cb；行尾 CRLF,CRLF。

## FC158-E051 · unsigned

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				case 'u':		/* Unsigned decimal */
					base = 10;
					goto is_unsigned;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:735-737；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF。

## FC158-E052 · unsignedRank

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
					is_unsigned: switch (rank)
					{
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:746-747；blob短12 ef7e61a278cb；行尾 CRLF,CRLF。

## FC158-E053 · unsignedInt

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
					case rank_int:
						val = (size_t)
						va_arg(ap, unsigned	int);
						break;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:758-761；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF。

## FC158-E054 · outerHelper

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
					is_integer: sz = format_int(q, (o < n) ? n - o : 0, val,
							flags, base, width, prec);
					q += sz;
					o += sz;
					break;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:772-776；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E055 · stringArg

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				case 's':		/* String */
					sarg = va_arg(ap, const char *);
					sarg = sarg ? sarg : "(null)";
					slen = strlen(sarg);
					goto is_string;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:783-787；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E056 · strPrec

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
						if (prec != -1 && slen > prec)
							slen = prec;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:793-794；blob短12 ef7e61a278cb；行尾 CRLF,CRLF。

## FC158-E057 · strLeft

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
						if (width > slen && !(flags & FL_MINUS))
						{
							char pad = (flags & FL_ZERO) ? '0' : ' ';
							while (width > slen)
							{
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:796-800；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E058 · strLeftEmit

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
							while (width > slen)
							{
								EMIT(pad);
								width--;
							}
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:799-803；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E059 · strLoop

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
						for (i = slen; i; i--)
						{
							sch = *sarg++;
							EMIT(sch);
						}
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:805-809；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E060 · strRight

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
						if (width > slen && (flags & FL_MINUS))
						{
							while (width > slen) {
								EMIT(' ');
								width--;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:810-814；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E061 · strRightDec

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
							while (width > slen) {
								EMIT(' ');
								width--;
							}
						}
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:812-816；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF,CRLF。

## FC158-E062 · nul

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	if (o < n)
		*q = '\0';				/* No overflow */
	else if (n > 0)
		buf[n - 1] = '\0';		/* Overflow - terminate at end of buffer */
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:871-874；blob短12 ef7e61a278cb；行尾 CRLF,CRLF,CRLF,CRLF。

## FC158-E063 · return

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	return o;
```

myinitramfs/mylib/myglibc/stdio/vsnprintf.c:876-876；blob短12 ef7e61a278cb；行尾 CRLF。

## FC158-E064 · sz

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::SZ_4K]
```c
	#define SZ_4K 4096
```

myinitramfs/mylib/myglibc/stdio/printf.c:9-9；blob短12 e8c0430ad375；行尾 LF。

## FC158-E065 · printfBuf

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
int printf(const char * fmt, ...)
{
	va_list ap;
	char buf[SZ_4K];
	int rv;
```

myinitramfs/mylib/myglibc/stdio/printf.c:11-15；blob短12 e8c0430ad375；行尾 LF,LF,LF,LF,LF。

## FC158-E066 · printfFormat

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	va_start(ap, fmt);
	rv = vsnprintf(buf, SZ_4K, fmt, ap);
	va_end(ap);
```

myinitramfs/mylib/myglibc/stdio/printf.c:17-19；blob短12 e8c0430ad375；行尾 LF,LF,LF。

## FC158-E067 · write

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	write(stdout->_fileno, buf, rv);
```

myinitramfs/mylib/myglibc/stdio/printf.c:22-22；blob短12 e8c0430ad375；行尾 LF。

## FC158-E068 · printfReturn

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	return rv;
```

myinitramfs/mylib/myglibc/stdio/printf.c:25-25；blob短12 e8c0430ad375；行尾 LF。

## FC158-E069 · strlenMacro

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
#ifndef STRLEN
	#define STRLEN strlen
#endif
```

myinitramfs/mylib/myglibc/string/strlen.c:23-25；blob短12 7416e487fb71；行尾 LF,LF,LF。

## FC158-E070 · strlenInit

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
size_t
STRLEN(const char *str)
{
	const char *char_ptr;
	const unsigned long int *longword_ptr;
```

myinitramfs/mylib/myglibc/string/strlen.c:29-33；blob短12 7416e487fb71；行尾 LF,LF,LF,LF,LF。

## FC158-E071 · strlenAlign

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
	for (char_ptr = str; ((unsigned long int)char_ptr & (sizeof(longword) - 1)) != 0;
		 ++char_ptr)
		if (*char_ptr == '\0')
			return char_ptr - str;
```

myinitramfs/mylib/myglibc/string/strlen.c:38-41；blob短12 7416e487fb71；行尾 LF,LF,LF,LF。

## FC158-E072 · strlenPtr

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
	longword_ptr = (unsigned long int *)char_ptr;
```

myinitramfs/mylib/myglibc/string/strlen.c:46-46；blob短12 7416e487fb71；行尾 LF。

## FC158-E073 · strlenMagic

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
	himagic = 0x80808080L;
	lomagic = 0x01010101L;
	if (sizeof(longword) > 4)
	{
		/* 64-bit version of the magic.  */
```

myinitramfs/mylib/myglibc/string/strlen.c:57-61；blob短12 7416e487fb71；行尾 LF,LF,LF,LF,LF。

## FC158-E074 · strlenMagic64

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
		/* 64-bit version of the magic.  */
		/* Do the shift in two steps to avoid a warning if long has 32 bits.  */
		himagic = ((himagic << 16) << 16) | himagic;
		lomagic = ((lomagic << 16) << 16) | lomagic;
	}
```

myinitramfs/mylib/myglibc/string/strlen.c:61-65；blob短12 7416e487fb71；行尾 LF,LF,LF,LF,LF。

## FC158-E075 · strlenRead

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
	for (;;)
	{
		longword = *longword_ptr++;

		if (((longword - lomagic) & ~longword & himagic) != 0)
```

myinitramfs/mylib/myglibc/string/strlen.c:72-76；blob短12 7416e487fb71；行尾 LF,LF,LF,LF,LF。

## FC158-E076 · strlenConfirm0

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
			const char *cp = (const char *)(longword_ptr - 1);

			if (cp[0] == 0)
				return cp - str;
			if (cp[1] == 0)
```

myinitramfs/mylib/myglibc/string/strlen.c:81-85；blob短12 7416e487fb71；行尾 LF,LF,LF,LF,LF。

## FC158-E077 · strlenConfirm2

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
			if (cp[1] == 0)
				return cp - str + 1;
			if (cp[2] == 0)
				return cp - str + 2;
			if (cp[3] == 0)
```

myinitramfs/mylib/myglibc/string/strlen.c:85-89；blob短12 7416e487fb71；行尾 LF,LF,LF,LF,LF。

## FC158-E078 · strlenConfirm3

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
			if (cp[3] == 0)
				return cp - str + 3;
			if (sizeof(longword) > 4)
			{
				if (cp[4] == 0)
```

myinitramfs/mylib/myglibc/string/strlen.c:89-93；blob短12 7416e487fb71；行尾 LF,LF,LF,LF,LF。

## FC158-E079 · strlenConfirm5

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
				if (cp[4] == 0)
					return cp - str + 4;
				if (cp[5] == 0)
					return cp - str + 5;
				if (cp[6] == 0)
```

myinitramfs/mylib/myglibc/string/strlen.c:93-97；blob短12 7416e487fb71；行尾 LF,LF,LF,LF,LF。

## FC158-E080 · strlenConfirm7

[VERIFIED myinitramfs/mylib/myglibc/string/strlen.c::STRLEN]
```c
				if (cp[6] == 0)
					return cp - str + 6;
				if (cp[7] == 0)
					return cp - str + 7;
```

myinitramfs/mylib/myglibc/string/strlen.c:97-100；blob短12 7416e487fb71；行尾 LF,LF,LF,LF。

## FC158-E081 · pathMax

[VERIFIED myinitramfs/myshell/myshell.c::PATH_MAX]
```c
	#define PATH_MAX        4096	/* # chars in a path name including nul */
```

myinitramfs/myshell/myshell.c:15-15；blob短12 5e177066dc16；行尾 LF。

## FC158-E082 · currentDir

[VERIFIED myinitramfs/myshell/myshell.c::current_dir]
```c
char current_dir[PATH_MAX];
```

myinitramfs/myshell/myshell.c:60-60；blob短12 5e177066dc16；行尾 LF。

## FC158-E083 · shellGetcwd

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
	memset(current_dir, 0, PATH_MAX);
	getcwd(current_dir, PATH_MAX);
```

myinitramfs/myshell/myshell.c:75-76；blob短12 5e177066dc16；行尾 LF,LF。

## FC158-E084 · prompt

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
		printf("\n");
		printf("MyOS2:%s # _", current_dir);
		fflush(stdout);
```

myinitramfs/myshell/myshell.c:85-87；blob短12 5e177066dc16；行尾 LF,LF,LF。

## FC158-E085 · cd

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	if(!i)
	{
		memset(current_dir, 0, PATH_MAX);
		getcwd(current_dir, PATH_MAX);
	}
```

myinitramfs/myshell/myshell.c:204-208；blob短12 5e177066dc16；行尾 LF,LF,LF,LF,LF。

## FC158-E086 · oldTime

[VERIFIED myinitramfs/myshell/tests.c::test_PrintDateTime]
```c
	printf("DateTime Now: %s.%06ld\n", buffer, tv.tv_usec);

	return 0;
```

myinitramfs/myshell/tests.c:124-126；blob短12 71e7681d050b；行尾 LF,LF,LF。

## FC158-E087 · prepend

[VERIFIED mykernel/fs/vfs/d_path.c::myos_prepend]
```c
	p->len -= namelen;
	if (p->len >= 0) {
		p->buf -= namelen;
		memcpy(p->buf, str, namelen);
	}
```

mykernel/fs/vfs/d_path.c:51-55；blob短12 75aae754bb56；行尾 LF,LF,LF,LF,LF。

## FC158-E088 · cwdBuf

[VERIFIED mykernel/fs/vfs/d_path.c::getcwd]
```c
		unsigned len;
		b.len = PATH_MAX;
		bufhead = kzalloc(b.len, GFP_KERNEL);
		b.buf = bufhead + PATH_MAX;

```

mykernel/fs/vfs/d_path.c:209-213；blob短12 75aae754bb56；行尾 LF,LF,LF,LF,LF。

## FC158-E089 · cwdNul

[VERIFIED mykernel/fs/vfs/d_path.c::getcwd]
```c
		myos_prepend(&b, "", 1);
		if (myos_prepend_path(&pwd, &root, &b) > 0)
			myos_prepend(&b, "(unreachable)", 13);
```

mykernel/fs/vfs/d_path.c:214-216；blob短12 75aae754bb56；行尾 LF,LF,LF。

## FC158-E090 · cwdLen

[VERIFIED mykernel/fs/vfs/d_path.c::getcwd]
```c
		len = PATH_MAX - b.len;
		if (len > PATH_MAX)
			error = -ENAMETOOLONG;
		else if (len > size)
			error = -ERANGE;
```

mykernel/fs/vfs/d_path.c:218-222；blob短12 75aae754bb56；行尾 LF,LF,LF,LF,LF。

## FC158-E091 · cwdCopy

[VERIFIED mykernel/fs/vfs/d_path.c::getcwd]
```c
		else if (copy_to_user(buf, b.buf, len))
			error = -EFAULT;
		else
			error = (long)buf;
```

mykernel/fs/vfs/d_path.c:223-226；blob短12 75aae754bb56；行尾 LF,LF,LF,LF。

## FC158-E092 · cwdFree

[VERIFIED mykernel/fs/vfs/d_path.c::getcwd]
```c
	kfree(bufhead);
	return error;
```

mykernel/fs/vfs/d_path.c:229-230；blob短12 75aae754bb56；行尾 LF,LF。

## FC158-E093 · kernelPathMax

[VERIFIED mykernel/include/uapi/linux/limits.h::PATH_MAX]
```c
	#define PATH_MAX		4096	/* # chars in a path name including nul */
```

mykernel/include/uapi/linux/limits.h:13-13；blob短12 bddfd09e4be2；行尾 LF。

## FC158-E094 · userFlags

[VERIFIED myinitramfs/CMakeLists.txt::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 \
	-ggdb \
")
```

myinitramfs/CMakeLists.txt:26-29；blob短12 6757d21bc64c；行尾 LF,LF,LF,LF。

## FC158-E095 · staticFlag

[VERIFIED myinitramfs/CMakeLists.txt::if(LINK_TYPE)]
```cmake
if (${LINK_TYPE} STREQUAL "static")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
endif()
```

myinitramfs/CMakeLists.txt:31-33；blob短12 6757d21bc64c；行尾 LF,LF,LF。

## FC158-E096 · muslPath

[VERIFIED myinitramfs/CMakeLists.txt::set(MUSL_PATH)]
```cmake
set(MUSL_PATH /home/cheyh/projs/musl/build)
```

myinitramfs/CMakeLists.txt:40-40；blob短12 6757d21bc64c；行尾 LF。

## FC158-E097 · muslCompiler

[VERIFIED myinitramfs/CMakeLists.txt::set(CMAKE_C_COMPILER)]
```cmake
set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)
```

myinitramfs/CMakeLists.txt:41-41；blob短12 6757d21bc64c；行尾 LF。

## FC158-E098 · muslInc

[VERIFIED myinitramfs/CMakeLists.txt::set(MUSL_INC_PATH)]
```cmake
set(MUSL_INC_PATH ${MUSL_PATH}/include)
```

myinitramfs/CMakeLists.txt:43-43；blob短12 6757d21bc64c；行尾 LF。

## FC158-E099 · userInc

[VERIFIED myinitramfs/CMakeLists.txt::include_directories(MUSL_INC_PATH)]
```cmake
include_directories("${MUSL_INC_PATH}")
```

myinitramfs/CMakeLists.txt:46-46；blob短12 6757d21bc64c；行尾 LF。

## FC158-E100 · subInit

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory(myinit)]
```cmake
add_subdirectory(myinit)
```

myinitramfs/CMakeLists.txt:47-47；blob短12 6757d21bc64c；行尾 LF。

## FC158-E101 · subShell

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory(myshell)]
```cmake
add_subdirectory(myshell)
```

myinitramfs/CMakeLists.txt:48-48；blob短12 6757d21bc64c；行尾 LF。

## FC158-E102 · shellTarget

[VERIFIED myinitramfs/myshell/CMakeLists.txt::add_executable(sh)]
```cmake
add_executable(sh ${SHELL_C_SRCS} ${SHELL_ASM_SRCS})
```

myinitramfs/myshell/CMakeLists.txt:4-4；blob短12 dcdac0564c0e；行尾 LF。

## FC158-E103 · mylibSub

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory(myglibc)]
```cmake
add_subdirectory(myglibc)
```

myinitramfs/mylib/CMakeLists.txt:1-1；blob短12 8f6abc24095a；行尾 LF。

## FC158-E104 · mylibc

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_library(mylibc)]
```cmake
add_library(mylibc STATIC)
```

myinitramfs/mylib/myglibc/CMakeLists.txt:1-1；blob短12 b956957a602a；行尾 LF。

## FC158-E105 · mylibIncludes

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::target_include_directories(mylibc)]
```cmake
target_include_directories(mylibc PUBLIC
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/include
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sys
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/string
```

myinitramfs/mylib/myglibc/CMakeLists.txt:3-7；blob短12 b956957a602a；行尾 LF,LF,LF,LF,LF。

## FC158-E106 · mylibStdio

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory(stdio)]
```cmake
add_subdirectory(stdio)
```

myinitramfs/mylib/myglibc/CMakeLists.txt:18-18；blob短12 b956957a602a；行尾 LF。

## FC158-E107 · mylibString

[VERIFIED myinitramfs/mylib/myglibc/CMakeLists.txt::add_subdirectory(string)]
```cmake
add_subdirectory(string)
```

myinitramfs/mylib/myglibc/CMakeLists.txt:20-20；blob短12 b956957a602a；行尾 LF。

## FC158-E108 · stdioSource

[VERIFIED myinitramfs/mylib/myglibc/stdio/CMakeLists.txt::target_sources(mylibc)]
```cmake
target_sources(mylibc PUBLIC ${STDIO_C_SRCS} ${STDIO_ASM_SRCS})
```

myinitramfs/mylib/myglibc/stdio/CMakeLists.txt:3-3；blob短12 73ca342146a7；行尾 NONE。

## FC158-E109 · stdout

[VERIFIED myinitramfs/mylib/myglibc/libio/stdio.h::stdout]
```c
	extern FILE *stdout; /* Standard output stream.  */
```

myinitramfs/mylib/myglibc/libio/stdio.h:131-131；blob短12 396bfb0df778；行尾 LF。

## FC158-E110 · vsnprintfDecl

[VERIFIED myinitramfs/mylib/myglibc/libio/stdio.h::vsnprintf]
```c
	extern int vsnprintf(char *__s, size_t __maxlen, const char *__format, __gnuc_va_list __arg);
```

myinitramfs/mylib/myglibc/libio/stdio.h:367-367；blob短12 396bfb0df778；行尾 LF。

## FC158-E111 · fileType

[VERIFIED myinitramfs/mylib/myglibc/bits/types/FILE.h::FILE]
```c
	typedef struct _IO_FILE FILE;
```

myinitramfs/mylib/myglibc/bits/types/FILE.h:8-8；blob短12 74f62d695aa3；行尾 LF。

## FC158-E112 · fileno

[VERIFIED myinitramfs/mylib/myglibc/bits/types/struct_FILE.h::_IO_FILE]
```c
		int _fileno;
		int _flags2;
```

myinitramfs/mylib/myglibc/bits/types/struct_FILE.h:73-74；blob短12 77633f6b639b；行尾 LF,LF。

## FC158-E113 · writeDecl

[VERIFIED myinitramfs/mylib/myglibc/posix/unistd.h::write]
```c
	extern ssize_t write(int __fd, const void *__buf, size_t __n);
```

myinitramfs/mylib/myglibc/posix/unistd.h:377-377；blob短12 43f11162433e；行尾 LF。

## FC158-E114 · ssize

[VERIFIED myinitramfs/mylib/myglibc/posix/unistd.h::ssize_t]
```c

	#ifndef __ssize_t_defined
	typedef __ssize_t ssize_t;
	#define __ssize_t_defined
	#endif
```

myinitramfs/mylib/myglibc/posix/unistd.h:215-219；blob短12 43f11162433e；行尾 LF,LF,LF,LF,LF。

## FC158-E115 · wordSize

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/x86/bits/wordsize.h::__WORDSIZE]
```c
#if defined __x86_64__ && !defined __ILP32__
	#define __WORDSIZE	64
#else
	#define __WORDSIZE	32
```

myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/x86/bits/wordsize.h:3-6；blob短12 94ff7c423930；行尾 LF,LF,LF,LF。

## FC158-E116 · sword

[VERIFIED myinitramfs/mylib/myglibc/posix/bits/types.h::__SWORD_TYPE]
```c
		#define __SWORD_TYPE	long int
```

myinitramfs/mylib/myglibc/posix/bits/types.h:130-130；blob短12 7ac0a3f83b2a；行尾 LF。

## FC158-E117 · ssizeTypedef

[VERIFIED myinitramfs/mylib/myglibc/posix/bits/types.h::__ssize_t]
```c
	__STD_TYPE __SSIZE_T_TYPE		__ssize_t; /* Type of a byte count, or error.  */
```

myinitramfs/mylib/myglibc/posix/bits/types.h:193-193；blob短12 7ac0a3f83b2a；行尾 LF。

## FC158-E118 · ssizeType

[VERIFIED myinitramfs/mylib/myglibc/bits/typesizes.h::__SSIZE_T_TYPE]
```c
	#define __SSIZE_T_TYPE		__SWORD_TYPE
```

myinitramfs/mylib/myglibc/bits/typesizes.h:79-79；blob短12 d72a1e8c7b9e；行尾 LF。

## FC158-E119 · strlenDecl

[VERIFIED myinitramfs/mylib/myglibc/string/string.h::strlen]
```c
	extern size_t strlen(const char *__s);
```

myinitramfs/mylib/myglibc/string/string.h:405-405；blob短12 23305a96a57a；行尾 LF。

## FC158-E120 · kernelFlags

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

mykernel/scripts/options_flags.cmake:39-43；blob短12 6019436417ad；行尾 LF,LF,LF,LF,LF。

## FC158-E121 · printfDecl

[VERIFIED myinitramfs/mylib/myglibc/include/stdio.h::printf]
```c
	extern int printf(const char *, ...);
```

myinitramfs/mylib/myglibc/include/stdio.h:70-70；blob短12 76b24e00c502；行尾 LF。

## FC158-E122 · sysSsize

[VERIFIED myinitramfs/mylib/myglibc/posix/sys/types.h::ssize_t]
```c
	#ifndef __ssize_t_defined
		typedef __ssize_t	ssize_t;
		#define __ssize_t_defined
	#endif
```

myinitramfs/mylib/myglibc/posix/sys/types.h:105-108；blob短12 bfdcead553a9；行尾 LF,LF,LF,LF。

## 回核方法与普通定位

本席先生成暂存blob的原物理行窗，再另一次打开每一对应固定源码、核blob短12并逐窗比较原字节；三组92、20、10窗。第三组E116在草稿阶段发现邻行误标，已换成实际__SWORD_TYPE定义L130并当次回核；没有把错误窗放入最终成果。共122个正式窗，122个回核一致，0降级。CRLF不归一化。普通位置（不计正式窗）：myinitramfs/CMakeLists.txt L3 project、L40–48配置路径；mykernel/include/linux/kernel/limits.h L5转UAPI；myglibc/include/stdio.h L15、libio/stdio.h L30–42；include/unistd.h L3、posix/unistd.h L221–223；include/bits/types.h L1、posix/bits/types.h L28及141；sys/types.h L1、posix/sys/types.h L27及142–143；string/string.h L30–32；外部stddef/stdarg完整定义及实际include/链接不在本批。

有限模型只用自写整数/字节/对象边界复算。完整本地P9、预处理、编译链接、Graphviz与运行NOT_RUN。
