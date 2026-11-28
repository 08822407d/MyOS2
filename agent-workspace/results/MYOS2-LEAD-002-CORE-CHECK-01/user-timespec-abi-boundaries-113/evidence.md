# 第113批固定 time 源码引文账

本批只登记实际连接器回读、属于一个真实定义的连续1–5行；任何函数签名、typedef/字段布局、完整体否定观察只列POSITION_ONLY。历史110/111/112固定引文原件不改。

### TS113-E001 [CFG]

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```c
	-DCONFIG_BUG \
```

固定 time@a039d9803ade；普通定位 mykernel/scripts/options_flags.cmake#L66–66；fixed CONFIG_BUG C flag。源码文字，不是编译或真实执行证明。

### TS113-E002 [LVL_BITS]

[VERIFIED mykernel/time/timer/timer_const.h::LVL_BITS]
```c
	#define LVL_BITS		6
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/timer_const.h#L108–108；single macro, formerly combined KD112-E026。源码文字，不是编译或真实执行证明。

### TS113-E003 [LVL_SIZE]

[VERIFIED mykernel/time/timer/timer_const.h::LVL_SIZE]
```c
	#define LVL_SIZE		(1UL << LVL_BITS)
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/timer_const.h#L109–109；single macro size。源码文字，不是编译或真实执行证明。

### TS113-E004 [LVL_MASK]

[VERIFIED mykernel/time/timer/timer_const.h::LVL_MASK]
```c
	#define LVL_MASK		(LVL_SIZE - 1)
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/timer_const.h#L110–110；single macro mask。源码文字，不是编译或真实执行证明。

### TS113-E005 [LVL_OFFS]

[VERIFIED mykernel/time/timer/timer_const.h::LVL_OFFS]
```c
	#define LVL_OFFS(n)		((n) * LVL_SIZE)
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/timer_const.h#L111–111；single macro level offset。源码文字，不是编译或真实执行证明。

### TS113-E006 [ARR_SHIFT]

[VERIFIED mykernel/time/timer/timer_const.h::TIMER_ARRAYSHIFT]
```c
	#define TIMER_ARRAYSHIFT		22
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/timer_const.h#L195–195；single macro, formerly KD112-E027。源码文字，不是编译或真实执行证明。

### TS113-E007 [ARR_MASK]

[VERIFIED mykernel/time/timer/timer_const.h::TIMER_ARRAYMASK]
```c
	#define TIMER_ARRAYMASK			0xFFC00000
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/timer_const.h#L196–196；single macro, formerly KD112-E027。源码文字，不是编译或真实执行证明。

### TS113-E008 [KT_MAX]

[VERIFIED mykernel/time/misc/time_misc_const.h::KTIME_MAX]
```c
	#define KTIME_MAX				((s64)~((u64)1 << 63))
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc_const.h#L44–44；single macro, formerly KD112-E034。源码文字，不是编译或真实执行证明。

### TS113-E009 [KT_MIN]

[VERIFIED mykernel/time/misc/time_misc_const.h::KTIME_MIN]
```c
	#define KTIME_MIN				(-KTIME_MAX - 1)
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc_const.h#L45–45；single macro。源码文字，不是编译或真实执行证明。

### TS113-E010 [SEC_MAX]

[VERIFIED mykernel/time/misc/time_misc_const.h::KTIME_SEC_MAX]
```c
	#define KTIME_SEC_MAX			(KTIME_MAX / NSEC_PER_SEC)
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc_const.h#L46–46；single macro。源码文字，不是编译或真实执行证明。

### TS113-E011 [SEC_MIN]

[VERIFIED mykernel/time/misc/time_misc_const.h::KTIME_SEC_MIN]
```c
	#define KTIME_SEC_MIN			(KTIME_MIN / NSEC_PER_SEC)
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc_const.h#L47–47；single macro。源码文字，不是编译或真实执行证明。

### TS113-E012 [LVL_CLK_SHIFT]

[VERIFIED mykernel/time/timer/timer_const.h::LVL_CLK_SHIFT]
```c
	#define LVL_CLK_SHIFT	3
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/timer_const.h#L94–94；independent level bit shift。源码文字，不是编译或真实执行证明。

### TS113-E013 [LVL_SHIFT]

[VERIFIED mykernel/time/timer/timer_const.h::LVL_SHIFT]
```c
	#define LVL_SHIFT(n)	((n) * LVL_CLK_SHIFT)
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/timer_const.h#L97–97；independent level shift。源码文字，不是编译或真实执行证明。

### TS113-E014 [LVL_START]

[VERIFIED mykernel/time/timer/timer_const.h::LVL_START]
```c
	#define LVL_START(n)	((LVL_SIZE - 1) << (((n) - 1) * LVL_CLK_SHIFT))
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/timer_const.h#L105–105；independent cutoff formula。源码文字，不是编译或真实执行证明。

### TS113-E015 [SELECT_LEVEL]

[VERIFIED mykernel/time/timer/timer.c::calc_wheel_index]
```c
	if (delta < LVL_START(1)) {
		idx = calc_index(expires, 0, bucket_expiry);
	} else if (delta < LVL_START(2)) {
		idx = calc_index(expires, 1, bucket_expiry);
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/timer.c#L54–57；two explicit selection gates。源码文字，不是编译或真实执行证明。

### TS113-E016 [INDEX_CALC]

[VERIFIED mykernel/time/timer/timer.h::calc_index]
```c
			expires = (expires >> LVL_SHIFT(lvl)) + 1;
			*bucket_expiry = expires << LVL_SHIFT(lvl);
			return LVL_OFFS(lvl) + (expires & LVL_MASK);
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/timer.h#L109–111；bucket index and expiry writes。源码文字，不是编译或真实执行证明。

### TS113-E017 [WRITE_INDEX]

[VERIFIED mykernel/time/timer/timer.h::timer_set_idx]
```c
			timer->flags = (timer->flags & ~TIMER_ARRAYMASK) |
								idx << TIMER_ARRAYSHIFT;
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/timer.h#L90–91；index bits write。源码文字，不是编译或真实执行证明。

### TS113-E018 [WRITE_CPU]

[VERIFIED mykernel/time/timer/timer.c::add_timer_on]
```c
		WRITE_ONCE(timer->flags,
			   (timer->flags & ~TIMER_BASEMASK) | cpu);
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/timer.c#L529–530；explicit CPU write。源码文字，不是编译或真实执行证明。

### TS113-E019 [ENQUEUE]

[VERIFIED mykernel/time/timer/timer.c::enqueue_timer]
```c
	hlist_add_head(&timer->entry, base->vectors + idx);
	// __set_bit(idx, base->pending_map);
	timer_set_idx(timer, idx);
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/timer.c#L123–125；downstream bucket write call。源码文字，不是编译或真实执行证明。

### TS113-E020 [ON_CPU_CALL]

[VERIFIED mykernel/time/timer/timer.c::add_timer_on]
```c
	new_base = get_timer_cpu_base(timer->flags, cpu);
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/timer.c#L508–508；caller of explicit selector。源码文字，不是编译或真实执行证明。

### TS113-E021 [PPREV_INIT]

[VERIFIED mykernel/time/timer/timer.c::simple_init_timer_key]
```c
	timer->entry.pprev = NULL;
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/timer.c#L716–716；actual writer of pprev。源码文字，不是编译或真实执行证明。

### TS113-E022 [A_COPY]

[VERIFIED mykernel/time/misc/time_misc.c::get_timespec64]
```c
	ret = copy_from_user(&kts, uts, sizeof(kts));
	if (ret)
		return -EFAULT;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.c#L279–281；single input copy and error gate。源码文字，不是编译或真实执行证明。

### TS113-E023 [A_SEC]

[VERIFIED mykernel/time/misc/time_misc.c::get_timespec64]
```c
	ts->tv_sec = kts.tv_sec;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.c#L283–283；first destination field assignment。源码文字，不是编译或真实执行证明。

### TS113-E024 [A_NSEC]

[VERIFIED mykernel/time/misc/time_misc.c::get_timespec64]
```c
	ts->tv_nsec = kts.tv_nsec;

	return 0;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.c#L290–292；second field assignment then return。源码文字，不是编译或真实执行证明。

### TS113-E025 [A_PUT_INIT]

[VERIFIED mykernel/time/misc/time_misc.c::put_timespec64]
```c
	__kernel_timespec_s kts = {
		.tv_sec = ts->tv_sec,
		.tv_nsec = ts->tv_nsec
	};
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.c#L307–310；two local kernel-timespec fields initialized。源码文字，不是编译或真实执行证明。

### TS113-E026 [A_PUT_COPY]

[VERIFIED mykernel/time/misc/time_misc.c::put_timespec64]
```c
	return copy_to_user(uts, &kts, sizeof(kts)) ? -EFAULT : 0;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.c#L312–312；copy-to-user result branch。源码文字，不是编译或真实执行证明。

### TS113-E027 [A_CLOCK_NULL]

[VERIFIED mykernel/time/syscall.c::clock_gettime]
```c
	if (!kc)
		return -EINVAL;
```

固定 time@a039d9803ade；普通定位 mykernel/time/syscall.c#L41–42；clock selector early failure。源码文字，不是编译或真实执行证明。

### TS113-E028 [A_CLOCK_GET]

[VERIFIED mykernel/time/syscall.c::clock_gettime]
```c
	error = kc->clock_get_timespec(which_clock, &kernel_tp);
```

固定 time@a039d9803ade；普通定位 mykernel/time/syscall.c#L44–44；clock callback call result。源码文字，不是编译或真实执行证明。

### TS113-E029 [A_CLOCK_PUT]

[VERIFIED mykernel/time/syscall.c::clock_gettime]
```c
	if (!error && put_timespec64(&kernel_tp, tp))
		error = -EFAULT;

	return error;
```

固定 time@a039d9803ade；普通定位 mykernel/time/syscall.c#L46–49；conditional output request and return。源码文字，不是编译或真实执行证明。

### TS113-E030 [B_VALID_NEG]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_valid]
```c
			if (ts->tv_sec < 0)
				return false;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.h#L93–94；negative seconds early rejection。源码文字，不是编译或真实执行证明。

### TS113-E031 [B_VALID_NSEC]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_valid]
```c
			if ((ulong)ts->tv_nsec >= NSEC_PER_SEC)
				return false;
			return true;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.h#L96–98；unsigned-cast nsec bound and true。源码文字，不是编译或真实执行证明。

### TS113-E032 [B_STRICT_VALID]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_valid_strict]
```c
			if (!timespec64_valid(ts))
				return false;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.h#L104–105；strict calls basic validator。源码文字，不是编译或真实执行证明。

### TS113-E033 [B_STRICT_UPPER]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_valid_strict]
```c
			if ((ulonglong)ts->tv_sec >= KTIME_SEC_MAX)
				return false;
			return true;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.h#L107–109；strict equality-inclusive upper threshold。源码文字，不是编译或真实执行证明。

### TS113-E034 [B_SETTOD_VALID]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_valid_settod]
```c
			if (!timespec64_valid(ts))
				return false;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.h#L115–116；settod calls basic validator。源码文字，不是编译或真实执行证明。

### TS113-E035 [B_SETTOD_UPPER]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_valid_settod]
```c
			if ((ulonglong)ts->tv_sec >= TIME_SETTOD_SEC_MAX)
				return false;
			return true;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.h#L118–120；settod equality-inclusive threshold。源码文字，不是编译或真实执行证明。

### TS113-E036 [B_NSEC_UNIT]

[VERIFIED mykernel/time/misc/time_misc_const.h::NSEC_PER_SEC]
```c
	#define NSEC_PER_SEC			1000000000L
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc_const.h#L32–32；nanoseconds unit macro。源码文字，不是编译或真实执行证明。

### TS113-E037 [B_UPTIME]

[VERIFIED mykernel/time/misc/time_misc_const.h::TIME_UPTIME_SEC_MAX]
```c
	#define TIME_UPTIME_SEC_MAX		(30LL * 365 * 24 *3600)
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc_const.h#L57–57；reserved uptime in seconds。源码文字，不是编译或真实执行证明。

### TS113-E038 [B_SETTOD_LIMIT]

[VERIFIED mykernel/time/misc/time_misc_const.h::TIME_SETTOD_SEC_MAX]
```c
	#define TIME_SETTOD_SEC_MAX		(KTIME_SEC_MAX - TIME_UPTIME_SEC_MAX)
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc_const.h#L58–58；set-time ceiling formula。源码文字，不是编译或真实执行证明。

### TS113-E039 [C_EQUAL]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_equal]
```c
			return (a->tv_sec == b->tv_sec) && (a->tv_nsec == b->tv_nsec);
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.h#L47–47；two-field short circuit equality。源码文字，不是编译或真实执行证明。

### TS113-E040 [C_CMP_LESS]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_compare]
```c
			if (lhs->tv_sec < rhs->tv_sec)
				return -1;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.h#L58–59；first seconds-order gate。源码文字，不是编译或真实执行证明。

### TS113-E041 [C_CMP_GREATER]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_compare]
```c
			if (lhs->tv_sec > rhs->tv_sec)
				return 1;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.h#L60–61；second seconds-order gate。源码文字，不是编译或真实执行证明。

### TS113-E042 [C_CMP_NSEC]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_compare]
```c
			return lhs->tv_nsec - rhs->tv_nsec;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.h#L62–62；same-second signed nsec difference。源码文字，不是编译或真实执行证明。

### TS113-E043 [C_CARRY_GATE]

[VERIFIED mykernel/time/misc/time_misc.c::set_normalized_timespec64]
```c
	while (nsec >= NSEC_PER_SEC) {
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.c#L24–24；carry loop predicate。源码文字，不是编译或真实执行证明。

### TS113-E044 [C_CARRY_ACT]

[VERIFIED mykernel/time/misc/time_misc.c::set_normalized_timespec64]
```c
		asm("" : "+rm"(nsec));
		nsec -= NSEC_PER_SEC;
		++sec;
	}
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.c#L30–33；carry subtract and increment。源码文字，不是编译或真实执行证明。

### TS113-E045 [C_BORROW]

[VERIFIED mykernel/time/misc/time_misc.c::set_normalized_timespec64]
```c
	while (nsec < 0) {
		asm("" : "+rm"(nsec));
		nsec += NSEC_PER_SEC;
		--sec;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.c#L34–37；borrow loop body。源码文字，不是编译或真实执行证明。

### TS113-E046 [C_ASSIGN]

[VERIFIED mykernel/time/misc/time_misc.c::set_normalized_timespec64]
```c
	ts->tv_sec = sec;
	ts->tv_nsec = nsec;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.c#L39–40；normalized writes in order。源码文字，不是编译或真实执行证明。

### TS113-E047 [C_ADD_CALL]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_add]
```c
			set_normalized_timespec64(&ts_delta, lhs.tv_sec + rhs.tv_sec,
					lhs.tv_nsec + rhs.tv_nsec);
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.h#L69–70；add calls normalization helper。源码文字，不是编译或真实执行证明。

### TS113-E048 [C_SUB_CALL]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_sub]
```c
			set_normalized_timespec64(&ts_delta, lhs.tv_sec - rhs.tv_sec,
					lhs.tv_nsec - rhs.tv_nsec);
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.h#L81–82；sub calls normalization helper。源码文字，不是编译或真实执行证明。

### TS113-E049 [D_NS_INIT]

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	timespec64_s ts = { 0, 0 };
	s32 rem;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.c#L52–53；zero initialized struct and signed remainder。源码文字，不是编译或真实执行证明。

### TS113-E050 [D_NS_POS]

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	if (likely(nsec > 0)) {
		ts.tv_sec = div_u64_rem(nsec, NSEC_PER_SEC, &rem);
		ts.tv_nsec = rem;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.c#L55–57；positive direct helper call and caller writes。源码文字，不是编译或真实执行证明。

### TS113-E051 [D_NS_NEG]

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
		ts.tv_sec = -div_u64_rem(-nsec - 1, NSEC_PER_SEC, &rem) - 1;
		ts.tv_nsec = NSEC_PER_SEC - rem - 1;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.c#L64–65；negative direct helper call and writes。源码文字，不是编译或真实执行证明。

### TS113-E052 [D_NS_RETURN]

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	return ts;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.c#L68–68；zero and nonzero shared return。源码文字，不是编译或真实执行证明。

### TS113-E053 [D_DIV]

[VERIFIED mykernel/include/linux/kernel/math64.h::div_u64_rem]
```c
		*remainder = dividend % divisor;
		return dividend / divisor;
```

固定 time@a039d9803ade；普通定位 mykernel/include/linux/kernel/math64.h#L43–44；helper remainder write and unsigned quotient return。源码文字，不是编译或真实执行证明。

### TS113-E054 [D_A_WRAP]

[VERIFIED mykernel/time/ktime/ktime.h::timespec64_to_ktime]
```c
			return ktime_set(ts.tv_sec, ts.tv_nsec);
```

固定 time@a039d9803ade；普通定位 mykernel/time/ktime/ktime.h#L64–64；structure fields forwarded as two scalar args。源码文字，不是编译或真实执行证明。

### TS113-E055 [D_A_KSET]

[VERIFIED mykernel/time/ktime/ktime.h::ktime_set]
```c
			if (unlikely(secs >= KTIME_SEC_MAX))
				return KTIME_MAX;

			return secs * NSEC_PER_SEC + (s64)nsecs;
```

固定 time@a039d9803ade；普通定位 mykernel/time/ktime/ktime.h#L54–57；ktime multiplication and positive ceiling。源码文字，不是编译或真实执行证明。

### TS113-E056 [D_A_NS_HI]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_to_ns]
```c
			if (ts->tv_sec >= KTIME_SEC_MAX)
				return KTIME_MAX;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.h#L134–135；upper guard before arithmetic。源码文字，不是编译或真实执行证明。

### TS113-E057 [D_A_NS_LO]

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_to_ns]
```c
			if (ts->tv_sec <= KTIME_SEC_MIN)
				return KTIME_MIN;

			return ((s64) ts->tv_sec * NSEC_PER_SEC) + ts->tv_nsec;
```

固定 time@a039d9803ade；普通定位 mykernel/time/misc/time_misc.h#L137–140；lower guard and arithmetic。源码文字，不是编译或真实执行证明。

### TS113-E058 [D_SAFE]

[VERIFIED mykernel/time/timer/hrtimer.c::ktime_add_safe]
```c
	if (res < 0 || res < lhs || res < rhs)
		res = ktime_set(KTIME_SEC_MAX, 0);

	return res;
```

固定 time@a039d9803ade；普通定位 mykernel/time/timer/hrtimer.c#L15–18；safe-add true condition calls ktime_set。源码文字，不是编译或真实执行证明。

### TS113-E059 [D_KT_MACRO]

[VERIFIED mykernel/time/ktime/ktime_macro.h::ktime_to_timespec64]
```c
	#define ktime_to_timespec64(kt)		ns_to_timespec64((kt))
```

固定 time@a039d9803ade；普通定位 mykernel/time/ktime/ktime_macro.h#L31–31；macro expansion to ns_to_timespec64。源码文字，不是编译或真实执行证明。

### TS113-E060 [D_UNHASH]

[VERIFIED mykernel/lib/list/hash_list.h::hlist_unhashed_lockless]
```c
			return !READ_ONCE(h->pprev);
```

固定 time@a039d9803ade；普通定位 mykernel/lib/list/hash_list.h#L110–110；source-only read of pprev。源码文字，不是编译或真实执行证明。

## 普通位置 POSITION_ONLY（不计新的P2）

- **TS113-P01** `mykernel/time/misc/time_misc.c#L273–293` — get_timespec64含局部kts、ret、两个返回和注释状态的完整体。
- **TS113-P02** `mykernel/time/misc/time_misc.c#L285–287` — compat掩码三行均为注释，不是可执行语句。
- **TS113-P03** `mykernel/time/misc/time_misc.c#L304–313` — put_timespec64的签名及完整体。
- **TS113-P04** `mykernel/time/syscall.c#L33–50` — clock_gettime完整体，输出条件与错误分支范围。
- **TS113-P05** `mykernel/include/uapi/linux/time_types.h#L7–10` — __kernel_timespec_s用户ABI两个字段类型；typedef而非函数体。
- **TS113-P06** `mykernel/include/uapi/asm-generic/posix_types.h#L94` — __kernel_time64_t = long long类型别名。
- **TS113-P07** `mykernel/time/misc/time_misc_types.h#L7–10` — 内核timespec64的time64_t秒与long纳秒。
- **TS113-P08** `mykernel/time/time_type_declaration.h#L9–15` — ktime_t、time64_t别名与timespec64_s转发声明；第10行time64_t=__s64。
- **TS113-P09** `mykernel/time/misc/time_misc.h#L89–121` — 三个校验器完整体：分别归属、只在strict和settod直接调用valid。
- **TS113-P10** `mykernel/time/misc/time_misc.h#L44–63` — 相等、比较的签名及完整体。
- **TS113-P11** `mykernel/time/misc/time_misc.c#L22–41` — 完整规范化函数/循环体；边界差异与最终赋值顺序。
- **TS113-P12** `mykernel/time/misc/time_misc.h#L65–84` — timespec64_add/sub完整函数。
- **TS113-P13** `mykernel/time/misc/time_misc.c#L50–69` — ns_to_timespec64完整体、零路径、负号求值及赋值者。
- **TS113-P14** `mykernel/include/linux/kernel/math64.h#L41–45` — div_u64_rem的u64返回、u32*余数参数与体内语句。
- **TS113-P15** `mykernel/time/ktime/ktime.h#L51–65` — timespec64_to_ktime按值形参与ktime_set两个标量形参。
- **TS113-P16** `mykernel/time/misc/time_misc.h#L130–141` — timespec64_to_ns指针形参与双门。
- **TS113-P17** `mykernel/time/timer/timer.c#L441–480` — 三个void包装完整体，含结束括号。
- **TS113-P18** `mykernel/time/timer/timer.c#L495–539` — add_timer_on的void签名与完整体。
- **TS113-P19** `mykernel/time/timer/timer.h#L114–126` — 显式CPU selector 完整体。
- **TS113-P20** `mykernel/time/timer/timer_const.h#L94–111` — 选层阈值与位移宏各自定义的物理位置。
- **TS113-P21** `mykernel/time/timer/timer_const.h#L195–196` — 下标位域两独立宏位置。
- **TS113-P22** `mykernel/time/misc/time_misc_const.h#L44–47` — KTIME四宏单独定义位置。
- **TS113-P23** `mykernel/time/time_type_declaration.h#L10` — 第112批所遗漏的time64_t类型声明。
- **TS113-P24** `mykernel/time/misc/time_misc.c#L273–314` — 用户入/出复制两个完整体及注释，未见本体调用valid。
- **TS113-P25** `mykernel/time/misc/time_misc.c#L264–294` — get_timespec64复制体内没有逐字节结果保证。
- **TS113-P26** `mykernel/time/misc/time_misc.h#L55–63` — timespec64_compare同秒纳秒差值返回int的签名。

P1/P2计数：本批新标签 60；逐字回源 60；每个窗口单一函数体/宏/CMake归属 60；降级0；普通位置 26。历史第112批53个字面命中/50个单一定义合格的计数不因新增宏锚而改写。
