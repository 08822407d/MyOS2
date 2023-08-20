#include <asm/processor.h>
#include <asm/desc.h>

/*
 * per-CPU TSS segments. Threads are completely 'soft' on Linux,
 * no more per-task TSS's. The TSS size is kept cacheline-aligned
 * so they are allowed to end up in the .data..cacheline_aligned
 * section. Since TSS's are completely CPU-local, we want them
 * on exact cacheline boundaries, to eliminate cacheline ping-pong.
 */
__visible DEFINE_PER_CPU_PAGE_ALIGNED(struct tss_struct, cpu_tss_rw) = {
// struct tss_struct cpu_tss_rw = {
	.x86_tss = {
		/*
		 * .sp0 is only used when entering ring 0 from a lower
		 * privilege level.  Since the init task never runs anything
		 * but ring 0 code, there is no need for a valid value here.
		 * Poison it.
		 */
		.sp0 = (1UL << (BITS_PER_LONG-1)) + 1,
		.io_bitmap_base	= IO_BITMAP_OFFSET_INVALID,
	 },
};

DEFINE_PER_CPU_PAGE_ALIGNED(struct gdt_page, gdt_page) = { .gdt = {
// struct gdt_page gdt_page = { .gdt = {
	[GDT_ENTRY_INVALID_SEG]			= GDT_ENTRY_INIT(0, 0, 0),
#if defined(CONFIG_INTER_X64_GDT_LAYOUT)
	[GDT_ENTRY_KERNEL_CS]			= GDT_ENTRY_INIT(0xa09b, 0, 0xfffff),
	[GDT_ENTRY_KERNEL_DS]			= GDT_ENTRY_INIT(0xc093, 0, 0xfffff),
	[GDT_ENTRY_DEFAULT_USER_CS]		= GDT_ENTRY_INIT(0xa0fb, 0, 0xfffff),
	[GDT_ENTRY_DEFAULT_USER_DS]		= GDT_ENTRY_INIT(0xc0f3, 0, 0xfffff),
	[GDT_ENTRY_DEFAULT_USER_CS_DUP]	= GDT_ENTRY_INIT(0xa0fb, 0, 0xfffff),
	[GDT_ENTRY_DEFAULT_USER_DS_DUP]	= GDT_ENTRY_INIT(0xc0f3, 0, 0xfffff),
#else
	/*
	 * We need valid kernel segments for data and code in long mode too
	 * IRET will check the segment types  kkeil 2000/10/28
	 * Also sysret mandates a special GDT layout
	 *
	 * TLS descriptors are currently at a different place compared to i386.
	 * Hopefully nobody expects them at a fixed place (Wine?)
	 */
	[GDT_ENTRY_KERNEL32_CS]			= GDT_ENTRY_INIT(0xc09b, 0, 0xfffff),
	[GDT_ENTRY_KERNEL_CS]			= GDT_ENTRY_INIT(0xa09b, 0, 0xfffff),
	[GDT_ENTRY_KERNEL_DS]			= GDT_ENTRY_INIT(0xc093, 0, 0xfffff),
	[GDT_ENTRY_DEFAULT_USER32_CS]	= GDT_ENTRY_INIT(0xc0fb, 0, 0xfffff),
	[GDT_ENTRY_DEFAULT_USER_DS]		= GDT_ENTRY_INIT(0xc0f3, 0, 0xfffff),
	[GDT_ENTRY_DEFAULT_USER_CS]		= GDT_ENTRY_INIT(0xa0fb, 0, 0xfffff),
#endif
}};