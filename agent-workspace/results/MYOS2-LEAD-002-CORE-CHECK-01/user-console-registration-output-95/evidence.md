---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-CONSOLE-REGISTRATION-OUTPUT-95
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-13
record_type: fixed_time_single_definition_short_anchors
produced_by: GPT-6（会话模型家族，后台UNATTESTED）
model_per_owner: GPT-6＋Extra（Owner指定）
date: 2026-11-17
base_snapshot: time
source_pin_short12: a039d9803ade
read_channel: connector
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第95批固定time源码短锚

每段为`time@a039d9803ade`真实宏、对象、函数或类型定义内连续1–5行；标注路径和行号供复查。每段静态文本已独立用GitHub连接器按固定time原文回读核对，**不**认证宏最终编译选择、硬件运行、C指针或帧缓冲有效性。注释仅证明代码处于注释状态，绝非活动调用。

### CN95-E001 | [VERIFIED mykernel/printk/printk.c::console_list_hdr]
固定源：`mykernel/printk/printk.c:78-78`。
```c
DECLARE_LIST_HDR_S(console_list_hdr);
```

### CN95-E002 | [VERIFIED mykernel/lib/list/double_list_macro.h::LIST_HEADER_INIT]
固定源：`mykernel/lib/list/double_list_macro.h:9-12`。
```c
	#define LIST_HEADER_INIT(name) {				\
				.anchor	= LIST_INIT(name.anchor),	\
				.count	= 0,						\
			}
```

### CN95-E003 | [VERIFIED mykernel/lib/list/double_list_macro.h::DECLARE_LIST_HDR_S]
固定源：`mykernel/lib/list/double_list_macro.h:16-17`。
```c
	#define DECLARE_LIST_HDR_S(name) \
				List_hdr_s name = LIST_HEADER_INIT(name)
```

### CN95-E004 | [VERIFIED mykernel/lib/list/double_list.h::list_header_contains]
固定源：`mykernel/lib/list/double_list.h:635-639`。
```c
		list_header_contains(List_hdr_s *lhdr_p, List_s *l_p) {
			List_s *tmp = NULL;
			list_header_foreach(tmp, lhdr_p) {
				if (tmp == l_p)
					return true;
```

### CN95-E005 | [VERIFIED mykernel/lib/list/double_list.h::list_header_add_to_tail]
固定源：`mykernel/lib/list/double_list.h:663-667`。
```c
		void
		list_header_add_to_tail(List_hdr_s *lhdr_p, List_s *l_p) {
			list_add_to_prev(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
		}
```

### CN95-E006 | [VERIFIED mykernel/lib/list/double_list.h::list_add_to_prev]
固定源：`mykernel/lib/list/double_list.h:220-223`。
```c
		void
		list_add_to_prev(List_s *new, List_s *head) {
			__list_add_between(new, head->prev, head);
		}
```

### CN95-E007 | [VERIFIED mykernel/lib/list/double_list.h::__list_add_between]
固定源：`mykernel/lib/list/double_list.h:189-193`。
```c
		__list_add_between(List_s *new, List_s *prev, List_s *next) {
			if (!__list_add_valid(new, prev, next))
				return;

			next->prev = new;
```

### CN95-E008 | [VERIFIED mykernel/lib/list/double_list.h::INIT_LIST_S]
固定源：`mykernel/lib/list/double_list.h:148-152`。
```c
		void
		INIT_LIST_S(List_s *list) {
			WRITE_ONCE(list->next, list);
			WRITE_ONCE(list->prev, list);
		}
```

### CN95-E009 | [VERIFIED mykernel/printk/printk.c::register_console]
固定源：`mykernel/printk/printk.c:601-605`。
```c
void register_console(console_s *newcon)
{
	int i;
	ulong flags;
	console_s *bcon = NULL;
```

### CN95-E010 | [VERIFIED mykernel/printk/printk.c::register_console]
固定源：`mykernel/printk/printk.c:607-610`。
```c
	if (list_header_contains(&console_list_hdr, &newcon->node))
		return;
	else
		list_header_add_to_tail(&console_list_hdr, &newcon->node);
```

### CN95-E011 | [VERIFIED mykernel/printk/printk.c::register_console]
固定源：`mykernel/printk/printk.c:615-619`。
```c
	//  */
	// if (console_drivers && newcon->flags & CON_BOOT) {
	// 	/* find the last or real console */
	// 	for_each_console(bcon) {
	// 		if (!(bcon->flags & CON_BOOT)) {
```

### CN95-E012 | [VERIFIED mykernel/printk/printk.c::register_console]
固定源：`mykernel/printk/printk.c:639-643`。
```c
	//  *	that registers here.
	//  */
	// if (preferred_console < 0) {
	// 	if (newcon->index < 0)
	// 		newcon->index = 0;
```

### CN95-E013 | [VERIFIED mykernel/printk/printk.c::unregister_console]
固定源：`mykernel/printk/printk.c:749-753`。
```c
int unregister_console(console_s *console)
{
	console_s *a, *b;
	int res = 1;

```

### CN95-E014 | [VERIFIED mykernel/printk/printk.c::unregister_console]
固定源：`mykernel/printk/printk.c:774-778`。
```c
	// 	console_drivers->flags |= CON_CONSDEV;

	// console_unlock();
	// console_sysfs_notify();
	// return res;
```

### CN95-E015 | [VERIFIED mykernel/printk/printk.c::console_init]
固定源：`mykernel/printk/printk.c:790-794`。
```c
void __init console_init(void)
{
extern console_s myos_tempcon;
extern void myos_init_video();
	myos_init_video();
```

### CN95-E016 | [VERIFIED mykernel/printk/printk.c::console_init]
固定源：`mykernel/printk/printk.c:817-821`。
```c
	// 	ce++;
	// }

	pr_info("Printk available now.\n");
}
```

### CN95-E017 | [VERIFIED mykernel/printk/printk.c::myos_tempcon_write]
固定源：`mykernel/printk/printk.c:825-828`。
```c
void myos_tempcon_write(console_s *co, const char *s, unsigned int count)
{
	myos_tty_write_color(s, count, WHITE, BLACK);
}
```

### CN95-E018 | [VERIFIED mykernel/printk/printk.c::myos_tempcon_read]
固定源：`mykernel/printk/printk.c:830-833`。
```c
int myos_tempcon_read(console_s *co, char *s, unsigned int count)
{

}
```

### CN95-E019 | [VERIFIED mykernel/printk/printk.c::myos_tempcon]
固定源：`mykernel/printk/printk.c:835-839`。
```c
console_s myos_tempcon = {
	.name	= "Myos_Temp_Console",
	.write	= myos_tempcon_write,
	.read	= myos_tempcon_read,
};
```

### CN95-E020 | [VERIFIED mykernel/include/linux/device/console.h::console_s]
固定源：`mykernel/include/linux/device/console.h:194-198`。
```c
	typedef struct console {
		char			name[32];
		void			(*write)(console_s *co, const char *s, unsigned int count);
		int				(*read)(console_s *co, char *s, unsigned int count);
		// struct tty_driver	*(*device)(console_s *co, int *index);
```

### CN95-E021 | [VERIFIED mykernel/include/linux/device/console.h::console_s]
固定源：`mykernel/include/linux/device/console.h:207-211`。
```c
		// uint			ospeed;
		// u64				seq;
		// unsigned long	dropped;
		// void			*data;
		List_s			node;
```

### CN95-E022 | [VERIFIED mykernel/include/linux/device/console.h::for_each_console]
固定源：`mykernel/include/linux/device/console.h:337-340`。
```c
	#define for_each_console(con)						\
			list_header_for_each_container(						\
				con, (&console_list_hdr), node		\
			)
```

### CN95-E023 | [VERIFIED mykernel/lib/list/double_list_macro.h::list_header_foreach]
固定源：`mykernel/lib/list/double_list_macro.h:340-342`。
```c
	#define list_header_foreach(pos, lhdr)	\
				list_for_each(pos, &((lhdr)->anchor))

```

### CN95-E024 | [VERIFIED mykernel/printk/printk.c::can_use_console]
固定源：`mykernel/printk/printk.c:323-326`。
```c
static inline int can_use_console(unsigned int cpu) {
	// return cpu_online(cpu) || have_callable_console();
	return (console_list_hdr.count > 0);
}
```

### CN95-E025 | [VERIFIED mykernel/printk/printk.c::__call_console_drivers]
固定源：`mykernel/printk/printk.c:124-128`。
```c

	for_each_console(con) {
		// if (exclusive_console && con != exclusive_console)
		// 	continue;
		// if ((con->flags & CON_ENABLED) && con->write &&
```

### CN95-E026 | [VERIFIED mykernel/printk/printk.c::console_unlock]
固定源：`mykernel/printk/printk.c:554-558`。
```c
		// spin_lock_irqsave(&logbuf_lock, flags);
		wake_klogd |= log_start - log_end;
		if (con_start == log_end)
			break;			/* Nothing to print */
		_con_start = con_start;
```

### CN95-E027 | [VERIFIED mykernel/drivers/char/myos_tty.c::Pos]
固定源：`mykernel/drivers/char/myos_tty.c:15-15`。
```c
position_t Pos;
```

### CN95-E028 | [VERIFIED mykernel/drivers/char/myos_tty.c::myos_tty_write_color]
固定源：`mykernel/drivers/char/myos_tty.c:112-116`。
```c
void myos_tty_write_color(const char *buf, size_t length, unsigned int FRcolor, unsigned int BKcolor)
{
	int count = 0;
	int line = 0;

```

### CN95-E029 | [VERIFIED mykernel/drivers/char/myos_tty.c::myos_tty_write_color]
固定源：`mykernel/drivers/char/myos_tty.c:117-120`。
```c
	spin_lock(&Pos.lock);
	for (count = 0; count < length || line; count++)
	{
		////	add \n \b \t
```

### CN95-E030 | [VERIFIED mykernel/drivers/char/myos_tty.c::myos_tty_write_color]
固定源：`mykernel/drivers/char/myos_tty.c:121-125`。
```c
		if (line > 0)
		{
			count--;
			goto Label_tab;
		}
```

### CN95-E031 | [VERIFIED mykernel/drivers/char/myos_tty.c::myos_tty_write_color]
固定源：`mykernel/drivers/char/myos_tty.c:126-130`。
```c
		if ((unsigned char)*(buf + count) == '\n')
		{
			Pos.YPosition++;
			Pos.XPosition = 0;
		}
```

### CN95-E032 | [VERIFIED mykernel/drivers/char/myos_tty.c::myos_tty_write_color]
固定源：`mykernel/drivers/char/myos_tty.c:132-135`。
```c
		{
			Pos.XPosition--;
			if (Pos.XPosition < 0)
			{
```

### CN95-E033 | [VERIFIED mykernel/drivers/char/myos_tty.c::myos_tty_write_color]
固定源：`mykernel/drivers/char/myos_tty.c:136-139`。
```c
				Pos.XPosition = Pos.XResolution / Pos.XCharSize - 1;
				Pos.YPosition--;
				if (Pos.YPosition < 0)
					Pos.YPosition = Pos.YResolution / Pos.YCharSize - 1;
```

### CN95-E034 | [VERIFIED mykernel/drivers/char/myos_tty.c::myos_tty_write_color]
固定源：`mykernel/drivers/char/myos_tty.c:141-142`。
```c
			kputchar(Pos.FB_addr, Pos.XResolution, Pos.XPosition * Pos.XCharSize,
					Pos.YPosition * Pos.YCharSize, FRcolor, BKcolor, ' ');
```

### CN95-E035 | [VERIFIED mykernel/drivers/char/myos_tty.c::myos_tty_write_color]
固定源：`mykernel/drivers/char/myos_tty.c:144-148`。
```c
		else if ((unsigned char)*(buf + count) == '\t')
		{
			line = ((Pos.XPosition + 8) & ~(8 - 1)) - Pos.XPosition;

		Label_tab:
```

### CN95-E036 | [VERIFIED mykernel/drivers/char/myos_tty.c::myos_tty_write_color]
固定源：`mykernel/drivers/char/myos_tty.c:148-152`。
```c
		Label_tab:
			line--;
			kputchar(Pos.FB_addr, Pos.XResolution, Pos.XPosition * Pos.XCharSize,
					Pos.YPosition * Pos.YCharSize, FRcolor, BKcolor, ' ');
			Pos.XPosition++;
```

### CN95-E037 | [VERIFIED mykernel/drivers/char/myos_tty.c::myos_tty_write_color]
固定源：`mykernel/drivers/char/myos_tty.c:155-158`。
```c
		{
			kputchar(Pos.FB_addr, Pos.XResolution, Pos.XPosition * Pos.XCharSize,
					Pos.YPosition * Pos.YCharSize, FRcolor, BKcolor, (unsigned char)*(buf + count));
			Pos.XPosition++;
```

### CN95-E038 | [VERIFIED mykernel/drivers/char/myos_tty.c::myos_tty_write_color]
固定源：`mykernel/drivers/char/myos_tty.c:161-165`。
```c
		if (Pos.XPosition >= (Pos.XResolution / Pos.XCharSize))
		{
			Pos.YPosition++;
			Pos.XPosition = 0;
		}
```

### CN95-E039 | [VERIFIED mykernel/drivers/char/myos_tty.c::myos_tty_write_color]
固定源：`mykernel/drivers/char/myos_tty.c:166-170`。
```c
		if (Pos.YPosition >= (Pos.YResolution / Pos.YCharSize))
		{
			Pos.YPosition = 0;
		}
	}
```

### CN95-E040 | [VERIFIED mykernel/include/obsolete/printk.h::position_t]
固定源：`mykernel/include/obsolete/printk.h:49-53`。
```c
	unsigned int	XResolution;
	unsigned int	YResolution;
	
	unsigned int	XPosition;
	unsigned int	YPosition;
```

### CN95-E041 | [VERIFIED mykernel/include/obsolete/printk.h::position_t]
固定源：`mykernel/include/obsolete/printk.h:55-59`。
```c
	unsigned int	XCharSize;
	unsigned int	YCharSize;

	unsigned int *	FB_addr;
	unsigned long	FB_length;
```

### CN95-E042 | [VERIFIED mykernel/klib/printk.c::color_printk]
固定源：`mykernel/klib/printk.c:28-32`。
```c
int color_printk(unsigned int FRcolor, unsigned int BKcolor, const char *fmt, ...)
{
	static char buf[4096] = {0};
	int i = 0;
	int count = 0;
```

### CN95-E043 | [VERIFIED mykernel/klib/printk.c::color_printk]
固定源：`mykernel/klib/printk.c:34-38`。
```c
	va_list args;
	va_start(args, fmt);

	i = vsnprintf(buf, PAGE_SIZE, fmt, args);

```

### CN95-E044 | [VERIFIED mykernel/klib/printk.c::color_printk]
固定源：`mykernel/klib/printk.c:39-43`。
```c
	va_end(args);

	myos_tty_write_color(buf, i, FRcolor, BKcolor);

	return i;
```

### CN95-E045 | [VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
固定源：`mykernel/arch/x86_64/mm/page/page_const_arch.h:8-11`。
```c
	/* PAGE_SHIFT determines the page size */
	#define PAGE_SHIFT				12
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
	#define PAGE_MASK				(~(PAGE_SIZE-1))
```

### CN95-E046 | [VERIFIED mykernel/drivers/char/myos_tty.c::kputchar]
固定源：`mykernel/drivers/char/myos_tty.c:86-90`。
```c
static void kputchar(unsigned int *fb, int Xresol, int x, int y,
		unsigned int FRcolor, unsigned int BKcolor, unsigned char font)
{
	int i = 0, j = 0;
	unsigned int *addr = NULL;
```

### CN95-E047 | [VERIFIED mykernel/drivers/char/myos_tty.c::kputchar]
固定源：`mykernel/drivers/char/myos_tty.c:93-97`。
```c
	fontp = font_ascii[font];

	for (i = 0; i < 16; i++)
	{
		addr = fb + Xresol * (y + i) + x;
```

### CN95-E048 | [VERIFIED mykernel/drivers/char/myos_tty.c::kputchar]
固定源：`mykernel/drivers/char/myos_tty.c:98-102`。
```c
		testval = 0x100;
		for (j = 0; j < 8; j++)
		{
			testval = testval >> 1;
			if (*fontp & testval)
```

### CN95-E049 | [VERIFIED mykernel/drivers/char/myos_tty.c::kputchar]
固定源：`mykernel/drivers/char/myos_tty.c:103-107`。
```c
				*addr = FRcolor;
			else
				*addr = BKcolor;
			addr++;
		}
```

### CN95-E050 | [VERIFIED mykernel/include/klib/font.h::font_ascii]
固定源：`mykernel/include/klib/font.h:20-20`。
```c
extern unsigned char font_ascii[256][16];
```

### CN95-E051 | [VERIFIED mykernel/klib/font.c::font_ascii]
固定源：`mykernel/klib/font.c:3-6`。
```c
unsigned char font_ascii[256][16]=
{
	/*	0000	*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
```

### CN95-E052 | [VERIFIED mykernel/include/obsolete/ktypes.h::framebuffer_s]
固定源：`mykernel/include/obsolete/ktypes.h:22-26`。
```c
	typedef struct framebuffer
	{
		phys_addr_t	FB_phybase;
		virt_addr_t	FB_virbase;
		uint64_t	FB_size;
```

### CN95-E053 | [VERIFIED mykernel/init/main.c::start_kernel]
固定源：`mykernel/init/main.c:132-136`。
```c
asmlinkage void __init start_kernel(void)
{
	char *command_line;
	char *after_dashes;

```

### CN95-E054 | [VERIFIED mykernel/init/main.c::start_kernel]
固定源：`mykernel/init/main.c:172-175`。
```c
	 * this. But we do want output early, in case something goes wrong.
	 */
	console_init();

```

### CN95-E055 | [VERIFIED mykernel/drivers/char/tty_io.c::myos_init_video]
固定源：`mykernel/drivers/char/tty_io.c:193-197`。
```c
void myos_init_video()
{	
	Pos.FB_addr = (unsigned int *)framebuffer.FB_virbase;
	Pos.FB_length = framebuffer.FB_size;
	Pos.XResolution = framebuffer.X_Resolution;
```

### CN95-E056 | [VERIFIED mykernel/drivers/char/tty_io.c::myos_init_video]
固定源：`mykernel/drivers/char/tty_io.c:200-204`。
```c
	Pos.XPosition = 0;
	Pos.YPosition = 0;
	Pos.XCharSize = 8;
	Pos.YCharSize = 16;

```

### CN95-E057 | [VERIFIED mykernel/drivers/char/tty_io.c::myos_init_video]
固定源：`mykernel/drivers/char/tty_io.c:208-211`。
```c
	char linebuf[4096] = {0};
	int i;
	for (i = 0; i < Pos.XResolution / Pos.XCharSize ; i++)
		linebuf[i] = ' ';
```

### CN95-E058 | [VERIFIED mykernel/drivers/char/tty_io.c::myos_init_video]
固定源：`mykernel/drivers/char/tty_io.c:212-215`。
```c
	color_printk(BLACK, GREEN, "%s", linebuf);
	color_printk(BLACK, GREEN, "\n");

	pr_info("MyOS Templete Console Initiated.\n");
```

### CN95-E059 | [VERIFIED mykernel/drivers/char/tty_io.c::myos_init_VBE_mapping]
固定源：`mykernel/drivers/char/tty_io.c:221-225`。
```c
void myos_init_VBE_mapping()
{
	u64 start, end;
	start = PFN_PHYS(PFN_DOWN(framebuffer.FB_phybase));
	myos_ioremap(start, PFN_PHYS(PFN_UP(framebuffer.FB_size)));
```

### CN95-E060 | [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::framebuffer]
固定源：`mykernel/arch/x86_64/myos/pre_init.c:18-18`。
```c
framebuffer_s		framebuffer;
```

### CN95-E061 | [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::get_framebuffer_info]
固定源：`mykernel/arch/x86_64/myos/pre_init.c:29-33`。
```c
static void get_framebuffer_info(mbi_framebuffer_s *framebuffer_info)
{
	mbi_framebuffer_common_s *common = &framebuffer_info->common;
	framebuffer.FB_phybase = common->framebuffer_addr;
	framebuffer.FB_virbase = (virt_addr_t)phys_to_virt(framebuffer.FB_phybase);
```

### CN95-E062 | [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::get_framebuffer_info]
固定源：`mykernel/arch/x86_64/myos/pre_init.c:34-37`。
```c
	framebuffer.FB_size = common->framebuffer_width * common->framebuffer_height * common->framebuffer_bpp / 8;
	framebuffer.X_Resolution = common->framebuffer_width;
	framebuffer.Y_Resolution = common->framebuffer_height;
	framebuffer.PixperScanline = common->framebuffer_pitch / (common->framebuffer_bpp / 8);
```

### CN95-E063 | [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::parse_tag]
固定源：`mykernel/arch/x86_64/myos/pre_init.c:95-99`。
```c
			break;
		case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
			mbi_framebuffer_s *mbi_fb = (mbi_framebuffer_s *)tag;
			get_framebuffer_info(mbi_fb);
			break;
```

### CN95-E064 | [VERIFIED mykernel/include/linux/device/console.h::for_each_console]
固定源：`mykernel/include/linux/device/console.h:327-331`。
```c
	 * for_each_console() - Iterator over registered consoles
	 * @con:	console_s pointer used as loop cursor
	 *
	 * The console list and the console->flags are immutable while iterating.
	 *
```

### CN95-E065 | [VERIFIED mykernel/printk/printk.c::console_init]
固定源：`mykernel/printk/printk.c:793-797`。
```c
extern void myos_init_video();
	myos_init_video();
	INIT_LIST_S(&myos_tempcon.node);
	register_console(&myos_tempcon);

```

### CN95-E066 | [VERIFIED mykernel/printk/printk.c::__call_console_drivers]
固定源：`mykernel/printk/printk.c:129-133`。
```c
		// 		(cpu_online(smp_processor_id()) ||
		// 		(con->flags & CON_ANYTIME)))
			con->write(con, &LOG_BUF(start), end - start);
	}
}
```

### CN95-E067 | [VERIFIED mykernel/lib/list/double_list.h::__list_add_valid]
固定源：`mykernel/lib/list/double_list.h:162-165`。
```c
		__list_add_valid(List_s *new, List_s *prev, List_s *next) {
			while ((prev == NULL) || (next == NULL) ||
					(next->prev != prev) || (prev->next != next) ||
					(new == prev || new == next));
```

### CN95-E068 | [VERIFIED mykernel/lib/list/double_list.h::list_header_contains]
固定源：`mykernel/lib/list/double_list.h:640-642`。
```c
			}
			return false;
		}
```

### CN95-E069 | [VERIFIED mykernel/lib/list/double_list.h::__list_add_between]
固定源：`mykernel/lib/list/double_list.h:193-197`。
```c
			next->prev = new;
			new->next = next;
			new->prev = prev;
			WRITE_ONCE(prev->next, new);
		}
```

### CN95-E070 | [VERIFIED mykernel/printk/printk.c::console_unlock]
固定源：`mykernel/printk/printk.c:558-562`。
```c
		_con_start = con_start;
		_log_end = log_end;
		con_start = log_end;		/* Flush */
		// spin_unlock(&logbuf_lock);
		// stop_critical_timings();	/* don't trace print latency */
```
