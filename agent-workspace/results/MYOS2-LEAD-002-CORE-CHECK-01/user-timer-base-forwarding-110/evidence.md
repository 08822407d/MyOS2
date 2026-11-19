# 第110批定点源码证据索引

全部短引文均源自固定 `time@a039d9803ade`。声明、函数签名、include、预处理条件和注释不冒作独立函数体引文；这些在文末单列POSITION_ONLY。以下仅源级文本，不证明编译、链接、执行或设备状态。

### TB110-E001

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```c
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
```

固定辅助行65–65；用途：D00 and P4 arch syscall wrapper configuration line。实际机器路径及对象状态未测。


### TB110-E002

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```c
	-DCONFIG_BUG \
```

固定辅助行66–66；用途：B CONFIG_BUG activation line。实际机器路径及对象状态未测。


### TB110-E003

[VERIFIED mykernel/include/linux/kernel/asm-generic/param.h::HZ]
```c
	#define HZ				1000		/* Internal kernel timer frequency */
```

固定辅助行9–9；用途：D00 wheel depth HZ source。实际机器路径及对象状态未测。


### TB110-E004

[VERIFIED mykernel/time/timer/timer_const.h::TIMER_CPUMASK]
```c
	#define TIMER_CPUMASK			0x0003FFFF
```

固定辅助行183–183；用途：A CPU identifier low bits。实际机器路径及对象状态未测。


### TB110-E005

[VERIFIED mykernel/time/timer/timer_const.h::TIMER_MIGRATING]
```c
	#define TIMER_MIGRATING			0x00040000
```

固定辅助行184–184；用途：A MIGRATING flag constant。实际机器路径及对象状态未测。


### TB110-E006

[VERIFIED mykernel/time/timer/timer_const.h::TIMER_BASEMASK]
```c
	#define TIMER_BASEMASK			(				\
				TIMER_CPUMASK | TIMER_MIGRATING		\
			)
```

固定辅助行185–187；用途：A complete base mask combines CPU and migration bits。实际机器路径及对象状态未测。


### TB110-E007

[VERIFIED mykernel/time/timer/timer_const.h::TIMER_PINNED]
```c
	#define TIMER_PINNED			0x00100000
```

固定辅助行189–189；用途：A local-vs-global selector flag。实际机器路径及对象状态未测。


### TB110-E008

[VERIFIED mykernel/time/timer/timer.h::get_timer_cpu_base]
```c
			int index = tflags & TIMER_PINNED ? BASE_LOCAL : BASE_GLOBAL;
			timer_base_s *base = per_cpu_ptr(&timer_bases[index], cpu);
```

固定辅助行117–118；用途：A explicit CPU plus pinned base index and per_cpu_ptr。实际机器路径及对象状态未测。


### TB110-E009

[VERIFIED mykernel/time/timer/timer.h::get_timer_cpu_base]
```c
			return base;
```

固定辅助行125–125；用途：A actual returned base pointer。实际机器路径及对象状态未测。


### TB110-E010

[VERIFIED mykernel/time/timer/timer.h::get_timer_this_cpu_base]
```c
			int index = tflags & TIMER_PINNED ? BASE_LOCAL : BASE_GLOBAL;
			timer_base_s *base = this_cpu_ptr(&timer_bases[index]);
```

固定辅助行131–132；用途：A current CPU base selection。实际机器路径及对象状态未测。


### TB110-E011

[VERIFIED mykernel/time/timer/timer.h::get_timer_this_cpu_base]
```c
			return base;
```

固定辅助行139–139；用途：A actual returned current CPU base。实际机器路径及对象状态未测。


### TB110-E012

[VERIFIED mykernel/time/timer/timer.h::get_timer_base]
```c
			return get_timer_cpu_base(tflags, tflags & TIMER_CPUMASK);
```

固定辅助行145–145；用途：A CPU mask and function call to explicit CPU picker。实际机器路径及对象状态未测。


### TB110-E013

[VERIFIED mykernel/time/timer/timer.c::lock_timer_base]
```c
		tf = READ_ONCE(timer->flags);
```

固定辅助行197–197；用途：A stable flags READ_ONCE sample。实际机器路径及对象状态未测。


### TB110-E014

[VERIFIED mykernel/time/timer/timer.c::lock_timer_base]
```c
		if (!(tf & TIMER_MIGRATING)) {
			base = get_timer_base(tf);
```

固定辅助行199–200；用途：A migration gate and selector call。实际机器路径及对象状态未测。


### TB110-E015

[VERIFIED mykernel/time/timer/timer.c::lock_timer_base]
```c
			// raw_spin_lock_irqsave(&base->lock, *flags);
			spin_lock_irqsave(&base->lock, *flags);
			if (timer->flags == tf)
				return base;
```

固定辅助行201–204；用途：A lock request and exact flags recheck。实际机器路径及对象状态未测。


### TB110-E016

[VERIFIED mykernel/time/timer/timer.c::lock_timer_base]
```c
			// raw_spin_unlock_irqrestore(&base->lock, *flags);
			spin_unlock_irqrestore(&base->lock, *flags);
		}
		cpu_relax();
```

固定辅助行205–208；用途：A retry unlock and cpu_relax。实际机器路径及对象状态未测。


### TB110-E017

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	new_base = get_timer_this_cpu_base(timer->flags);
```

固定辅助行296–296；用途：A new base selected using current CPU。实际机器路径及对象状态未测。


### TB110-E018

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		if (likely(base->running_timer != timer)) {
			/* See the comment in lock_timer_base() */
			timer->flags |= TIMER_MIGRATING;
```

固定辅助行306–308；用途：A migration safety conditional and flag write。实际机器路径及对象状态未测。


### TB110-E019

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
			spin_unlock(&base->lock);
			base = new_base;
			spin_lock(&base->lock);
```

固定辅助行310–312；用途：A unlock and base pointer substitution。实际机器路径及对象状态未测。


### TB110-E020

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
			WRITE_ONCE(timer->flags,
				(timer->flags & ~TIMER_BASEMASK) | base->cpu);
			forward_timer_base(base);
```

固定辅助行313–315；用途：A flags base CPU update and target clock forward。实际机器路径及对象状态未测。


### TB110-E021

[VERIFIED mykernel/time/timer/timer_types.h::timer_list]
```c
		HList_s		entry;
		ulong		expires;
		void		(*function)(timer_list_s *);
		u32			flags;
```

固定辅助行75–78；用途：A/C timer entry expires callback flags distinct members。实际机器路径及对象状态未测。


### TB110-E022

[VERIFIED mykernel/time/timer/timer_types.h::timer_base]
```c
		ulong			clk;
		ulong			next_expiry;
		uint			cpu;
		bool			next_expiry_recalc;
```

固定辅助行60–63；用途：A/B base clk next_expiry CPU and recalc。实际机器路径及对象状态未测。


### TB110-E023

[VERIFIED mykernel/time/systick/systick_macro.h::time_before]
```c
	#define time_before(a,b)	time_after(b,a)
```

固定辅助行39–39；用途：B before as after swapped arguments。实际机器路径及对象状态未测。


### TB110-E024

[VERIFIED mykernel/time/systick/systick_macro.h::time_after]
```c
	#define time_after(a,b)	(				\
				typecheck(ulong, a) &&		\
				typecheck(ulong, b) &&		\
				((long)((b) - (a)) < 0)		\
			)
```

固定辅助行27–31；用途：B after comparison signed long from unsigned diff。实际机器路径及对象状态未测。


### TB110-E025

[VERIFIED mykernel/time/systick/systick_macro.h::time_after_eq]
```c
	#define time_after_eq(a,b)	(			\
				typecheck(ulong, a) &&		\
				typecheck(ulong, b) &&		\
				((long)((a) - (b)) >= 0)	\
			)
```

固定辅助行48–52；用途：B after or equal signed comparison。实际机器路径及对象状态未测。


### TB110-E026

[VERIFIED mykernel/time/systick/systick_macro.h::time_before_eq]
```c
	#define time_before_eq(a,b)	time_after_eq(b,a)
```

固定辅助行60–60；用途：B before_eq delegates to after_eq。实际机器路径及对象状态未测。


### TB110-E027

[VERIFIED mykernel/time/timer/timer.h::__forward_timer_base]
```c
			if (time_before_eq(basej, base->clk))
				return;
```

固定辅助行155–156；用途：B basej less or equal early return。实际机器路径及对象状态未测。


### TB110-E028

[VERIFIED mykernel/time/timer/timer.h::__forward_timer_base]
```c
			if (time_after(base->next_expiry, basej)) {
				base->clk = basej;
			} else {
```

固定辅助行162–164；用途：B next_expiry later than basej direct basej write。实际机器路径及对象状态未测。


### TB110-E029

[VERIFIED mykernel/time/timer/timer.h::__forward_timer_base]
```c
				if (WARN_ON_ONCE(time_before(base->next_expiry, base->clk)))
					return;
```

固定辅助行165–166；用途：B warning branch may early return。实际机器路径及对象状态未测。


### TB110-E030

[VERIFIED mykernel/time/timer/timer.h::__forward_timer_base]
```c
				base->clk = base->next_expiry;
```

固定辅助行167–167；用途：B else writes next_expiry into clk。实际机器路径及对象状态未测。


### TB110-E031

[VERIFIED mykernel/time/timer/timer.h::forward_timer_base]
```c
			__forward_timer_base(base, READ_ONCE(jiffies));
```

固定辅助行174–174；用途：B direct READ_ONCE jiffies sampling call。实际机器路径及对象状态未测。


### TB110-E032

[VERIFIED mykernel/include/linux/kernel/asm-generic/bug.h::WARN_ON_ONCE]
```c
	#		define WARN_ON_ONCE(condition) (condition)
```

固定辅助行113–113；用途：B active macro returns raw condition under CONFIG_BUG。实际机器路径及对象状态未测。


### TB110-E033

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		forward_timer_base(base);
```

固定辅助行255–255；用途：B actual call from pending optimized path。实际机器路径及对象状态未测。


### TB110-E034

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		forward_timer_base(base);
```

固定辅助行289–289；用途：B actual call from normal path。实际机器路径及对象状态未测。


### TB110-E035

[VERIFIED mykernel/time/timer/timer_const.h::MOD_TIMER_PENDING_ONLY]
```c
	#define MOD_TIMER_PENDING_ONLY	0x01
```

固定辅助行204–204；用途：C public pending only option。实际机器路径及对象状态未测。


### TB110-E036

[VERIFIED mykernel/time/timer/timer_const.h::MOD_TIMER_REDUCE]
```c
	#define MOD_TIMER_REDUCE		0x02
```

固定辅助行205–205；用途：C public reduce option。实际机器路径及对象状态未测。


### TB110-E037

[VERIFIED mykernel/time/timer/timer_const.h::MOD_TIMER_NOTPENDING]
```c
	#define MOD_TIMER_NOTPENDING	0x04
```

固定辅助行206–206；用途：C creation path bypass option。实际机器路径及对象状态未测。


### TB110-E038

[VERIFIED mykernel/time/timer/timer.h::timer_pending]
```c
			return !hlist_unhashed_lockless(&timer->entry);
```

固定辅助行63–63；用途：C actual pending uses hashed node state。实际机器路径及对象状态未测。


### TB110-E039

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	if (!(options & MOD_TIMER_NOTPENDING) && timer_pending(timer)) {
```

固定辅助行227–227；用途：C pending fast path guard。实际机器路径及对象状态未测。


### TB110-E040

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		long diff = timer->expires - expires;

		if (!diff)
			return 1;
```

固定辅助行233–236；用途：C equality early return。实际机器路径及对象状态未测。


### TB110-E041

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		if (options & MOD_TIMER_REDUCE && diff <= 0)
			return 1;
```

固定辅助行237–238；用途：C reduce direction early return。实际机器路径及对象状态未测。


### TB110-E042

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		base = lock_timer_base(timer, &flags);
```

固定辅助行246–246；用途：C lock pending path。实际机器路径及对象状态未测。


### TB110-E043

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		if (!timer->function)
			goto out_unlock;

		forward_timer_base(base);
```

固定辅助行252–255；用途：C callback valid check and clock forward call。实际机器路径及对象状态未测。


### TB110-E044

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		if (timer_pending(timer) && (options & MOD_TIMER_REDUCE) &&
				time_before_eq(timer->expires, expires)) {
			ret = 1;
			goto out_unlock;
		}
```

固定辅助行257–261；用途：C postlock reduce direction check returns1。实际机器路径及对象状态未测。


### TB110-E045

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		clk = base->clk;
		idx = calc_wheel_index(expires, clk, &bucket_expiry);
```

固定辅助行263–264；用途：C precompute index from expires and clk。实际机器路径及对象状态未测。


### TB110-E046

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		if (idx == timer_get_idx(timer)) {
			if (!(options & MOD_TIMER_REDUCE))
				timer->expires = expires;
			else if (time_after(timer->expires, expires))
```

固定辅助行271–274；用途：C same bucket early return with ordinary write。实际机器路径及对象状态未测。


### TB110-E047

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
				timer->expires = expires;
			ret = 1;
			goto out_unlock;
```

固定辅助行275–277；用途：C reduce same bucket conditional write and ret。实际机器路径及对象状态未测。


### TB110-E048

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	} else {
		base = lock_timer_base(timer, &flags);
```

固定辅助行279–280；用途：C normal lock else。实际机器路径及对象状态未测。


### TB110-E049

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		if (!timer->function)
			goto out_unlock;

		forward_timer_base(base);
```

固定辅助行286–289；用途：C callback valid and forward in normal arm。实际机器路径及对象状态未测。


### TB110-E050

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	ret = detach_if_pending(timer, base, false);
	if (!ret && (options & MOD_TIMER_PENDING_ONLY))
		goto out_unlock;
```

固定辅助行292–294；用途：C detach if pending and PENDING_ONLY no-target exit。实际机器路径及对象状态未测。


### TB110-E051

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	new_base = get_timer_this_cpu_base(timer->flags);

	if (base != new_base) {
```

固定辅助行296–298；用途：C new this CPU base and cross-base gate。实际机器路径及对象状态未测。


### TB110-E052

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	timer->expires = expires;
```

固定辅助行321–321；用途：C expiry assigned to timer。实际机器路径及对象状态未测。


### TB110-E053

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	if (idx != UINT_MAX && clk == base->clk)
		enqueue_timer(base, timer, idx, bucket_expiry);
	else
		internal_add_timer(base, timer);
```

固定辅助行328–331；用途：C reuse idx and bucket_expiry vs recalc。实际机器路径及对象状态未测。


### TB110-E054

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	return ret;
```

固定辅助行337–337；用途：C return ret after unlock。实际机器路径及对象状态未测。


### TB110-E055

[VERIFIED mykernel/time/timer/timer.c::mod_timer_pending]
```c
	return __mod_timer(timer, expires, MOD_TIMER_PENDING_ONLY);
```

固定辅助行358–358；用途：C public pending only call。实际机器路径及对象状态未测。


### TB110-E056

[VERIFIED mykernel/time/timer/timer.c::mod_timer]
```c
	return __mod_timer(timer, expires, 0);
```

固定辅助行391–391；用途：C public unrestricted call。实际机器路径及对象状态未测。


### TB110-E057

[VERIFIED mykernel/time/timer/timer.c::timer_reduce]
```c
	return __mod_timer(timer, expires, MOD_TIMER_REDUCE);
```

固定辅助行417–417；用途：C public reduce call。实际机器路径及对象状态未测。


### TB110-E058

[VERIFIED mykernel/time/timer/timer.c::detach_if_pending]
```c
	uint idx = timer_get_idx(timer);

	if (!timer_pending(timer))
		return 0;
```

固定辅助行159–162；用途：C get index and inactive return 0。实际机器路径及对象状态未测。


### TB110-E059

[VERIFIED mykernel/time/timer/timer.c::detach_if_pending]
```c
	if (hlist_is_singular_node(&timer->entry, base->vectors + idx)) {
		// __clear_bit(idx, base->pending_map);
		base->next_expiry_recalc = true;
	}
```

固定辅助行164–167；用途：C singular node requires recalc。实际机器路径及对象状态未测。


### TB110-E060

[VERIFIED mykernel/time/timer/timer.c::detach_if_pending]
```c
	detach_timer(timer, clear_pending);
	return 1;
```

固定辅助行169–170；用途：C actual detach and return 1。实际机器路径及对象状态未测。


### TB110-E061

[VERIFIED mykernel/time/timer/timer.h::timer_get_idx]
```c
			return (timer->flags & TIMER_ARRAYMASK) >> TIMER_ARRAYSHIFT;
```

固定辅助行84–84；用途：C idx obtained from flags。实际机器路径及对象状态未测。


### TB110-E062

[VERIFIED mykernel/time/timer/timer.h::timer_set_idx]
```c
			timer->flags = (timer->flags & ~TIMER_ARRAYMASK) |
								idx << TIMER_ARRAYSHIFT;
```

固定辅助行90–91；用途：C flags updated with retained lower bits。实际机器路径及对象状态未测。


### TB110-E063

[VERIFIED mykernel/time/timer/timer.h::calc_index]
```c
			expires = (expires >> LVL_SHIFT(lvl)) + 1;
			*bucket_expiry = expires << LVL_SHIFT(lvl);
			return LVL_OFFS(lvl) + (expires & LVL_MASK);
```

固定辅助行109–111；用途：C bucket q shift plus one absolute expiry。实际机器路径及对象状态未测。


### TB110-E064

[VERIFIED mykernel/time/timer/timer.c::calc_wheel_index]
```c
	ulong delta = expires - clk;
	uint idx;

	if (delta < LVL_START(1)) {
		idx = calc_index(expires, 0, bucket_expiry);
```

固定辅助行51–55；用途：C delta and level0 choose。实际机器路径及对象状态未测。


### TB110-E065

[VERIFIED mykernel/time/timer/timer.c::calc_wheel_index]
```c
	} else if (delta < LVL_START(2)) {
		idx = calc_index(expires, 1, bucket_expiry);
	} else if (delta < LVL_START(3)) {
		idx = calc_index(expires, 2, bucket_expiry);
```

固定辅助行56–59；用途：C level1 and level2。实际机器路径及对象状态未测。


### TB110-E066

[VERIFIED mykernel/time/timer/timer_const.h::LVL_START]
```c
	#define LVL_START(n)	((LVL_SIZE - 1) << (((n) - 1) * LVL_CLK_SHIFT))
```

固定辅助行105–105；用途：C strict first threshold formula。实际机器路径及对象状态未测。


### TB110-E067

[VERIFIED mykernel/time/timer/timer_const.h::LVL_SHIFT]
```c
	#define LVL_SHIFT(n)	((n) * LVL_CLK_SHIFT)
```

固定辅助行97–97；用途：C level shift。实际机器路径及对象状态未测。


### TB110-E068

[VERIFIED mykernel/time/timer/timer_const.h::LVL_MASK]
```c
	#define LVL_MASK		(LVL_SIZE - 1)
```

固定辅助行110–110；用途：C slot mask。实际机器路径及对象状态未测。


### TB110-E069

[VERIFIED mykernel/time/timer/timer_const.h::LVL_OFFS]
```c
	#define LVL_OFFS(n)		((n) * LVL_SIZE)
```

固定辅助行111–111；用途：C global array offset。实际机器路径及对象状态未测。


### TB110-E070

[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
	timer_setup_on_stack(&timer.timer, process_timeout, 0);
	__mod_timer(&timer.timer, expire, MOD_TIMER_NOTPENDING);
	// schedule();
	del_timer_sync(&timer.timer);
```

固定辅助行796–799；用途：D00 A08/X08 direct request then commented schedule and delete call。实际机器路径及对象状态未测。


### TB110-E071

[VERIFIED mykernel/time/syscall.c::gettimeofday]
```c
		ktime_get_real_ts64(&ts);
		if (put_user(ts.tv_sec, &tv->tv_sec) ||
				put_user(ts.tv_nsec / 1000, &tv->tv_usec))
			return -EFAULT;
```

固定辅助行19–22；用途：D00 first successful put_user macro enables second under short circuit。实际机器路径及对象状态未测。


### TB110-E072

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess.h::put_user]
```c
	#define put_user(x, ptr) ({					\
				*(ptr) = (__typeof(*(ptr)))(x);	\
				-ENOERR;						\
			})
```

固定辅助行191–194；用途：D00 scalar store macro returns -ENOERR。实际机器路径及对象状态未测。


### TB110-E073

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOERR]
```c
	#define	ENOERR		0	/*  */
```

固定辅助行5–5；用途：D00 ENOERR equals 0。实际机器路径及对象状态未测。


### TB110-E074

[VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_cycles_to_ns]
```c
	/* Calculate the delta since the last update_wall_time() */
	u64 mask = tkr->mask, delta = (cycles - tkr->cycle_last) & mask;
```

固定辅助行199–200；用途：D00 independent clocksource masked delta, not wheel delta。实际机器路径及对象状态未测。


## POSITION_ONLY：不计入定义体短锚的签名、声明、预处理门和整段注释

以下均为固定time文件位置；仅单独索引，不作为P2函数体内的独立VERIFIED锚。跨批原件以固定提交中实际路径和编号定位。

- **TB110-P01**：`mykernel/time/timer/timer_api.h#L9`；extern timer_base_s timer_bases[NR_BASES] declaration only; not storage definition or perCPU allocator。
- **TB110-P02**：`mykernel/time/timer/timer.c#L184-187`；lock_timer_base signature, acquires annotation references timer->base->lock; source-only position, not ABI or successful typecheck。
- **TB110-P03**：`mykernel/time/timer/timer.c#L1-2`；TIMER_DEFINATION and timer.h include source position; debug selection not compiled。
- **TB110-P04**：`mykernel/time/timer/timer.h#L48-49`；conditional inline body compilation gate; distinct from function body。
- **TB110-P05**：`mykernel/time/timer/timer_const.h#L113-118`；HZ>100 preprocessor gate selects LVL_DEPTH=9 (otherwise 8), old TW109-P08。
- **TB110-P06**：`mykernel/include/linux/kernel/asm-generic/bug.h#L24`；outer CONFIG_BUG gate for active WARN_ON_ONCE macro。
- **TB110-P07**：`mykernel/include/linux/kernel/asm-generic/bug.h#L105-113`；old warning macro is commented; active line113 defines WARN_ON_ONCE(condition) (condition), E032。
- **TB110-P08**：`mykernel/include/linux/kernel/syscalls.h#L93-102`；positive wrapper guard and architecture header include; DT108-P08。
- **TB110-P09**：`mykernel/include/linux/kernel/syscalls.h#L314-317`；separate inverse guard for other prototypes; disproves 'only affects' claim。
- **TB110-P10**：`mykernel/arch/x86_64/entry/syscall_64.c#L26-31`；third __SYSCALL definition plus actual switch include; DT108-E004/E006。
- **TB110-P11**：`mykernel/arch/x86_64/include/asm/syscalls_64.h#L50,71`；gettimeofday/clock_gettime map instances; RT107-P01/P02。
- **TB110-P12**：`mykernel/time/timer/timer.c#L87-112`；trigger_dyntick_cpu definition and commented body, not DIRECT_CALL from itself; TW109-P06。
- **TB110-P13**：`mykernel/time/timer/timer.c#L119-143`；enqueue_timer original function order hlist/flags, strict compare, trigger call; TW109-E036/E037/E038。
- **TB110-P14**：`mykernel/time/timer/timer.c#L213-214`；__mod_timer signature, options typed as uint and expires as ulong。
- **TB110-P15**：`mykernel/time/timer/timer.c#L356-359`；mod_timer_pending signature and function body; new E055 is body-only。
- **TB110-P16**：`mykernel/time/timer/timer.c#L389-392`；mod_timer signature and body; E056 body-only。
- **TB110-P17**：`mykernel/time/timer/timer.c#L415-418`；timer_reduce signature and body; E057 body-only。
- **TB110-P18**：`mykernel/time/timer/timer.h#L114-116`；get_timer_cpu_base signature (tflags and explicit cpu)。
- **TB110-P19**：`mykernel/time/timer/timer.h#L128-130`；get_timer_this_cpu_base signature (only tflags)。
- **TB110-P20**：`mykernel/time/timer/timer.h#L142-145`；get_timer_base signature and delegating body, E012。
- **TB110-P21**：`mykernel/time/timer/timer.h#L148-150`；__forward_timer_base signature with explicit basej。
- **TB110-P22**：`mykernel/time/timer/timer.h#L171-174`；forward_timer_base signature and READ_ONCE(jiffies) call, E031。
- **TB110-P23**：`mykernel/time/timer/timer.h#L123-138`；DEF base selection code commented in two helper bodies; no active BASE_DEF selection proven。
- **TB110-P24**：`mykernel/time/timer/timer.c#L298-316`；migration condition, flags assignment, pointer change and new base clock request; not executed。
- **TB110-P25**：`mykernel/time/timer/timer.c#L334-337`；unlock and common return span; actual unlock and return distinct。
- **TB110-P26**：`mykernel/time/timer/timer.c#L227-278`；full pending fast branch for phase C decisions; individual body anchors E039–E047。
