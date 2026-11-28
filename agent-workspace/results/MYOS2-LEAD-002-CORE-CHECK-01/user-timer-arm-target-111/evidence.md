# 第111批固定time源码证据

全部新锚均取time@a039d9803ade真实定义体或单独宏/结构体中的连续1–5行。函数签名、extern、include、预处理条件及仅注释定位与源码短引文分账，未进行编译或内核执行。


### AT111-E001

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```c
	-DCONFIG_BUG \
```

固定time辅助行66–66；支持：fixed CMake CONFIG_BUG。仅源级，不认证选编或运行。


### AT111-E002

[VERIFIED mykernel/include/linux/kernel/asm-generic/bug.h::WARN_ON_ONCE]
```c
	#		define WARN_ON_ONCE(condition) (condition)
```

固定time辅助行113–113；支持：active macro pass-through under CONFIG_BUG。仅源级，不认证选编或运行。


### AT111-E003

[VERIFIED mykernel/time/timer/timer_const.h::TIMER_INIT_FLAGS]
```c
	#define TIMER_INIT_FLAGS		(				\
				TIMER_DEFERRABLE | TIMER_PINNED |	\
				TIMER_IRQSAFE						\
			)
```

固定time辅助行191–194；支持：init flags allowed mask。仅源级，不认证选编或运行。


### AT111-E004

[VERIFIED mykernel/time/timer/timer_const.h::TIMER_DEFERRABLE]
```c
	#define TIMER_DEFERRABLE		0x00080000
```

固定time辅助行188–188；支持：separate allowed flag。仅源级，不认证选编或运行。


### AT111-E005

[VERIFIED mykernel/time/timer/timer_const.h::TIMER_PINNED]
```c
	#define TIMER_PINNED			0x00100000
```

固定time辅助行189–189；支持：separate pinned flag。仅源级，不认证选编或运行。


### AT111-E006

[VERIFIED mykernel/time/timer/timer_const.h::TIMER_IRQSAFE]
```c
	#define TIMER_IRQSAFE			0x00200000
```

固定time辅助行190–190；支持：separate irqsafe flag。仅源级，不认证选编或运行。


### AT111-E007

[VERIFIED mykernel/time/timer/timer_const.h::TIMER_MIGRATING]
```c
	#define TIMER_MIGRATING			0x00040000
```

固定time辅助行184–184；支持：disallowed init migration flag。仅源级，不认证选编或运行。


### AT111-E008

[VERIFIED mykernel/time/timer/timer_const.h::TIMER_CPUMASK]
```c
	#define TIMER_CPUMASK			0x0003FFFF
```

固定time辅助行183–183；支持：cpu mask lower18。仅源级，不认证选编或运行。


### AT111-E009

[VERIFIED mykernel/time/timer/timer_const.h::TIMER_BASEMASK]
```c
	#define TIMER_BASEMASK			(				\
				TIMER_CPUMASK | TIMER_MIGRATING		\
			)
```

固定time辅助行185–187；支持：mask for migration flag plus CPU bits。仅源级，不认证选编或运行。


### AT111-E010

[VERIFIED mykernel/time/timer/timer_macro.h::timer_setup]
```c
	#define timer_setup(timer, callback, flags)		\
				simple_init_timer_key((timer), (callback), (flags))
```

固定time辅助行19–20；支持：regular timer setup macro。仅源级，不认证选编或运行。


### AT111-E011

[VERIFIED mykernel/time/timer/timer_macro.h::timer_setup_on_stack]
```c
	#define timer_setup_on_stack(timer, callback, flags)	\
				simple_init_timer_key((timer), (callback), (flags))
```

固定time辅助行21–22；支持：stack timer setup macro。仅源级，不认证选编或运行。


### AT111-E012

[VERIFIED mykernel/time/timer/timer.c::simple_init_timer_key]
```c
	timer->entry.pprev = NULL;
```

固定time辅助行716–716；支持：entry.pprev initialized NULL。仅源级，不认证选编或运行。


### AT111-E013

[VERIFIED mykernel/time/timer/timer.c::simple_init_timer_key]
```c
	timer->function = func;
```

固定time辅助行717–717；支持：callback function assignment。仅源级，不认证选编或运行。


### AT111-E014

[VERIFIED mykernel/time/timer/timer.c::simple_init_timer_key]
```c
	if (WARN_ON_ONCE(flags & ~TIMER_INIT_FLAGS))
		flags &= TIMER_INIT_FLAGS;
```

固定time辅助行718–719；支持：WARN gate and mask of input flags。仅源级，不认证选编或运行。


### AT111-E015

[VERIFIED mykernel/time/timer/timer.c::simple_init_timer_key]
```c
	timer->flags = flags | smp_processor_id();
```

固定time辅助行720–720；支持：final stored flags combines CPU id。仅源级，不认证选编或运行。


### AT111-E016

[VERIFIED mykernel/time/timer/timer_types.h::timer_list]
```c
		HList_s		entry;
		ulong		expires;
		void		(*function)(timer_list_s *);
		u32			flags;
```

固定time辅助行75–78；支持：timer entry expires callback flags four fields。仅源级，不认证选编或运行。


### AT111-E017

[VERIFIED mykernel/lib/list/hash_list.h::hlist_unhashed_lockless]
```c
		hlist_unhashed_lockless(const HList_s *h) {
			return !READ_ONCE(h->pprev);
```

固定time辅助行109–110；支持：pprev null means unhashed。仅源级，不认证选编或运行。


### AT111-E018

[VERIFIED mykernel/time/timer/timer.h::timer_pending]
```c
			return !hlist_unhashed_lockless(&timer->entry);
```

固定time辅助行63–63；支持：invert hlist_unhashed_lockless。仅源级，不认证选编或运行。


### AT111-E019

[VERIFIED mykernel/time/timer/timer.c::add_timer]
```c
	if (WARN_ON_ONCE(timer_pending(timer)))
		return;
```

固定time辅助行443–444；支持：normal wrapper pending early gate。仅源级，不认证选编或运行。


### AT111-E020

[VERIFIED mykernel/time/timer/timer.c::add_timer]
```c
	__mod_timer(timer, timer->expires, MOD_TIMER_NOTPENDING);
```

固定time辅助行445–445；支持：normal wrapper passes old expires and NOTPENDING。仅源级，不认证选编或运行。


### AT111-E021

[VERIFIED mykernel/time/timer/timer.c::add_timer_local]
```c
	if (WARN_ON_ONCE(timer_pending(timer)))
		return;
```

固定time辅助行459–460；支持：local wrapper pending early gate。仅源级，不认证选编或运行。


### AT111-E022

[VERIFIED mykernel/time/timer/timer.c::add_timer_local]
```c
	timer->flags |= TIMER_PINNED;
```

固定time辅助行461–461；支持：local wrapper sets pinned flag。仅源级，不认证选编或运行。


### AT111-E023

[VERIFIED mykernel/time/timer/timer.c::add_timer_local]
```c
	__mod_timer(timer, timer->expires, MOD_TIMER_NOTPENDING);
```

固定time辅助行462–462；支持：local wrapper passes expires NOTPENDING。仅源级，不认证选编或运行。


### AT111-E024

[VERIFIED mykernel/time/timer/timer.c::add_timer_global]
```c
	if (WARN_ON_ONCE(timer_pending(timer)))
		return;
```

固定time辅助行476–477；支持：global wrapper pending early gate。仅源级，不认证选编或运行。


### AT111-E025

[VERIFIED mykernel/time/timer/timer.c::add_timer_global]
```c
	timer->flags &= ~TIMER_PINNED;
```

固定time辅助行478–478；支持：global wrapper clears pinned。仅源级，不认证选编或运行。


### AT111-E026

[VERIFIED mykernel/time/timer/timer.c::add_timer_global]
```c
	__mod_timer(timer, timer->expires, MOD_TIMER_NOTPENDING);
```

固定time辅助行479–479；支持：global wrapper passes expires NOTPENDING。仅源级，不认证选编或运行。


### AT111-E027

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	ulong clk = 0, flags, bucket_expiry;
	timer_base_s *base, *new_base;
	uint idx = UINT_MAX;
	int ret = 0;
```

固定time辅助行215–218；支持：normal arm initialization ret and idx。仅源级，不认证选编或运行。


### AT111-E028

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	if (!(options & MOD_TIMER_NOTPENDING) && timer_pending(timer)) {
```

固定time辅助行227–227；支持：NOTPENDING bypass fast optimization。仅源级，不认证选编或运行。


### AT111-E029

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		if (!timer->function)
			goto out_unlock;

		forward_timer_base(base);
```

固定time辅助行286–289；支持：nonNULL function gate then forwarding。仅源级，不认证选编或运行。


### AT111-E030

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	timer->expires = expires;
```

固定time辅助行321–321；支持：timer expires assignment in downstream modifier。仅源级，不认证选编或运行。


### AT111-E031

[VERIFIED mykernel/time/timer/timer.c::internal_add_timer]
```c
	uint idx = calc_wheel_index(timer->expires,
					base->clk, &bucket_expiry);
	enqueue_timer(base, timer, idx, bucket_expiry);
```

固定time辅助行149–151；支持：eventual path consumes timer expires and base clk。仅源级，不认证选编或运行。


### AT111-E032

[VERIFIED mykernel/time/timer/timer.h::get_timer_cpu_base]
```c
			int index = tflags & TIMER_PINNED ? BASE_LOCAL : BASE_GLOBAL;
			timer_base_s *base = per_cpu_ptr(&timer_bases[index], cpu);
```

固定time辅助行117–118；支持：pinned class and explicit CPU pointer expression。仅源级，不认证选编或运行。


### AT111-E033

[VERIFIED mykernel/time/timer/timer.h::get_timer_this_cpu_base]
```c
			int index = tflags & TIMER_PINNED ? BASE_LOCAL : BASE_GLOBAL;
			timer_base_s *base = this_cpu_ptr(&timer_bases[index]);
```

固定time辅助行131–132；支持：current CPU and pinned class selection。仅源级，不认证选编或运行。


### AT111-E034

[VERIFIED mykernel/time/timer/timer.h::get_timer_base]
```c
			return get_timer_cpu_base(tflags, tflags & TIMER_CPUMASK);
```

固定time辅助行145–145；支持：masked old CPU chosen from flags。仅源级，不认证选编或运行。


### AT111-E035

[VERIFIED mykernel/time/timer/timer.c::lock_timer_base]
```c
		tf = READ_ONCE(timer->flags);

		if (!(tf & TIMER_MIGRATING)) {
			base = get_timer_base(tf);
```

固定time辅助行197–200；支持：old flags snapshot and migrating filter。仅源级，不认证选编或运行。


### AT111-E036

[VERIFIED mykernel/time/timer/timer.c::lock_timer_base]
```c
			// raw_spin_lock_irqsave(&base->lock, *flags);
			spin_lock_irqsave(&base->lock, *flags);
			if (timer->flags == tf)
				return base;
```

固定time辅助行201–204；支持：locked full flags recheck then return。仅源级，不认证选编或运行。


### AT111-E037

[VERIFIED mykernel/time/timer/timer.c::add_timer_on]
```c
	if (WARN_ON_ONCE(timer_pending(timer)))
		return;
```

固定time辅助行502–503；支持：pending early return before any flags write。仅源级，不认证选编或运行。


### AT111-E038

[VERIFIED mykernel/time/timer/timer.c::add_timer_on]
```c
	timer->flags |= TIMER_PINNED;
```

固定time辅助行506–506；支持：set PINNED before choosing new base。仅源级，不认证选编或运行。


### AT111-E039

[VERIFIED mykernel/time/timer/timer.c::add_timer_on]
```c
	new_base = get_timer_cpu_base(timer->flags, cpu);
```

固定time辅助行508–508；支持：explicit requested cpu chooses new base。仅源级，不认证选编或运行。


### AT111-E040

[VERIFIED mykernel/time/timer/timer.c::add_timer_on]
```c
	base = lock_timer_base(timer, &flags);
```

固定time辅助行515–515；支持：lock old base after new candidate。仅源级，不认证选编或运行。


### AT111-E041

[VERIFIED mykernel/time/timer/timer.c::add_timer_on]
```c
	if (!timer->function)
		goto out_unlock;
```

固定time辅助行520–521；支持：callback NULL exit。仅源级，不认证选编或运行。


### AT111-E042

[VERIFIED mykernel/time/timer/timer.c::add_timer_on]
```c
	if (base != new_base) {
		timer->flags |= TIMER_MIGRATING;
```

固定time辅助行523–524；支持：compare selected base pointers then migrating write。仅源级，不认证选编或运行。


### AT111-E043

[VERIFIED mykernel/time/timer/timer.c::add_timer_on]
```c
		spin_unlock(&base->lock);
		base = new_base;
		spin_lock(&base->lock);
```

固定time辅助行526–528；支持：unlock old and switch pointer lock new base。仅源级，不认证选编或运行。


### AT111-E044

[VERIFIED mykernel/time/timer/timer.c::add_timer_on]
```c
		WRITE_ONCE(timer->flags,
			   (timer->flags & ~TIMER_BASEMASK) | cpu);
```

固定time辅助行529–530；支持：mask flags and OR explicit cpu。仅源级，不认证选编或运行。


### AT111-E045

[VERIFIED mykernel/time/timer/timer.c::add_timer_on]
```c
	forward_timer_base(base);
```

固定time辅助行532–532；支持：forward selected base。仅源级，不认证选编或运行。


### AT111-E046

[VERIFIED mykernel/time/timer/timer.c::add_timer_on]
```c
	internal_add_timer(base, timer);
```

固定time辅助行535–535；支持：internal_add_timer registers existing expiry。仅源级，不认证选编或运行。


### AT111-E047

[VERIFIED mykernel/time/timer/timer.c::add_timer_on]
```c
	spin_unlock_irqrestore(&base->lock, flags);
```

固定time辅助行538–538；支持：common spin_unlock_irqrestore。仅源级，不认证选编或运行。


### AT111-E048

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
			WRITE_ONCE(timer->flags,
				(timer->flags & ~TIMER_BASEMASK) | base->cpu);
			forward_timer_base(base);
```

固定time辅助行313–315；支持：contrast __mod_timer writes base->cpu not explicit cpu。仅源级，不认证选编或运行。


### AT111-E049

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	new_base = get_timer_this_cpu_base(timer->flags);
```

固定time辅助行296–296；支持：normal modifier chooses this CPU candidate。仅源级，不认证选编或运行。


### AT111-E050

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::per_cpu_ptr]
```c
	#define per_cpu_ptr(ptr, cpu)	({		\
				__verify_pcpu_ptr(ptr);		\
				SHIFT_PERCPU_PTR((ptr),		\
					per_cpu_offset((cpu)));	\
			})
```

固定time辅助行244–248；支持：per_cpu_ptr macro not direct function。仅源级，不认证选编或运行。


### AT111-E051

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::this_cpu_ptr]
```c
	#define this_cpu_ptr(ptr)	arch_raw_cpu_ptr(ptr)
```

固定time辅助行250–250；支持：this_cpu_ptr macro not direct function。仅源级，不认证选编或运行。


### AT111-E052

[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
	timer.task = current;
	timer_setup_on_stack(&timer.timer, process_timeout, 0);
	__mod_timer(&timer.timer, expire, MOD_TIMER_NOTPENDING);
```

固定time辅助行795–797；支持：source example stack init followed by __mod。仅源级，不认证选编或运行。


### AT111-E053

[VERIFIED mykernel/time/timer/timer.h::timer_set_idx]
```c
			timer->flags = (timer->flags & ~TIMER_ARRAYMASK) |
								idx << TIMER_ARRAYSHIFT;
```

固定time辅助行90–91；支持：bucket index written by downstream modifier。仅源级，不认证选编或运行。


### AT111-E054

[VERIFIED mykernel/time/timer/timer_const.h::MOD_TIMER_NOTPENDING]
```c
	#define MOD_TIMER_NOTPENDING	0x04
```

固定time辅助行206–206；支持：normal nonpending path option constant。仅源级，不认证选编或运行。


## POSITION_ONLY：函数签名、extern、头文件/配置门、注释和整体范围

定位值属于固定time源码；不计入定义内P2短锚或源码VERIFIED词头。带离散行号的旧P位置须逐行独立核查。

- **AT111-P01**：`mykernel/time/timer/timer.c#L713-715`；simple_init_timer_key函数签名、func与flags参数；E012–E015为真实体内写。
- **AT111-P02**：`mykernel/time/timer/timer.c#L441-442`；add_timer返回void、唯一timer参数；E019–E020是体内门与请求。
- **AT111-P03**：`mykernel/time/timer/timer.c#L457-458`；add_timer_local返回void及timer参数。
- **AT111-P04**：`mykernel/time/timer/timer.c#L474-475`；add_timer_global返回void及timer参数。
- **AT111-P05**：`mykernel/time/timer/timer.c#L495-498`；add_timer_on返回void，显式int cpu参数与局部base/flags。
- **AT111-P06**：`mykernel/time/timer/timer_api.h#L9`；timer_bases[NR_BASES]仅extern声明，未验证定义、每CPU地址或BASE enum数值。
- **AT111-P07**：`mykernel/include/linux/kernel/asm-generic/bug.h#L24`；CONFIG_BUG预处理包围宏，活动WARN_ON_ONCE体见E002。
- **AT111-P08**：`mykernel/time/timer/timer.h#L48-49`；DEBUG/TIMER_DEFINATION门位置，具体函数体E018与E032等。
- **AT111-P09**：`mykernel/time/timer/timer.c#L713-722`；整个初始化体的否定检索区：仅写pprev、function和flags；未看到写entry.next或expires。
- **AT111-P10**：`mykernel/time/timer/timer.c#L441-479`；当前CPU三登记入口的完整函数区，早退门在所有修改之前。
- **AT111-P11**：`mykernel/time/timer/timer.c#L495-539`；显式CPU登记全函数区，供检查未写expires及各调用顺序。
- **AT111-P12**：`mykernel/time/timer/timer.c#L518-521`；显式CPU回调NULL门（真实判断见E041），置PINNED和选new_base已发生。
- **AT111-P13**：`mykernel/time/timer/timer.c#L523-530`；分支范围包括迁移标记、换锁以及写CPU的不同对象。
- **AT111-P14**：`mykernel/time/timer/timer.c#L533-539`；显式CPU成功后的最终登记及公共解锁区。
- **AT111-P15**：`mykernel/time/timer/timer.h#L114-125`；get_timer_cpu_base完整体与DEF注释，E032抽出活动选择。
- **AT111-P16**：`mykernel/time/timer/timer.h#L128-139`；get_timer_this_cpu_base完整体与DEF注释。
- **AT111-P17**：`mykernel/time/timer/timer.c#L213-218`；__mod_timer签名与初始idx、ret；对比显式CPU入口的void返回。
- **AT111-P18**：`mykernel/time/timer/timer.c#L306-315`；__mod_timer迁移后WRITE_ONCE使用base->cpu，与add_timer_on的显式cpu不同。
- **AT111-P19**：`mykernel/time/timer/timer_macro.h#L19-22`；两个setup宏各有独立定义，分别E010和E011，不代表完整对象初始化。
- **AT111-P20**：`mykernel/time/timer/timer_types.h#L70-83`；timer_list全部字段区，结论仅到可见成员。
- **AT111-P21**：`mykernel/arch/x86_64/include/asm/percpu.h#L244-250`；per_cpu_ptr与this_cpu_ptr两个独立宏，各自E050/E051，非直接C函数。
- **AT111-P22**：`mykernel/time/timer/timer_const.h#L183-196`；初始化许可的三宏分别E004/E005/E006；CPUMASK、MIGRATING、BASEMASK另定义。
