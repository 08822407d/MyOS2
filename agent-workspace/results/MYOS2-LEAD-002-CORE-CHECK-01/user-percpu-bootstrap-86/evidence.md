---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PERCPU-BOOTSTRAP-86
record_type: fixed_source_definition_short_anchors
produced_by: GPT-6 (visible identity only)
model_per_owner: GPT-6 + Extra
date: 2026-11-14
base_snapshot: time
read_channel: connector
status: static_evidence_for_mainline
runtime_evidence: NOT_RUN
---

# 第86批源码引文

以下77项引文使用MyOS2固定time@a039d9803ade中各自独立的函数、数据对象、宏或链接节真实定义内连续1–5行。页面行号仅供导航；不把文件注释、宏外条件或调用者拼进另一个函数定义，不认证机器码、链接产物或CPU真实寄存器。

### PC86-E001 | linked template load marker, isolated from next section header

[VERIFIED mykernel/arch/x86_64/kernel.lds::__per_cpu_load]

```ld
	__per_cpu_load = .;
```

引文范围 70–70 为真实单一定义内连续1行；执行NOT_RUN。

### PC86-E002 | template start and alignment

[VERIFIED mykernel/arch/x86_64/kernel.lds::.data..percpu]

```ld
	{
		__per_cpu_start = .;
		. = ALIGN((1 << 12));
		*(.data..percpu..page_aligned) . = ALIGN((1 << (6)));
```

原文4行；静态定义内，运行NOT_RUN。

### PC86-E003 | hot/read-mostly input patterns

[VERIFIED mykernel/arch/x86_64/kernel.lds::.data..percpu]

```ld
		__per_cpu_hot_start = .;
		*(SORT_BY_ALIGNMENT(.data..percpu..hot.*)) __per_cpu_hot_end = .;
		. = ALIGN((1 << (6)));
		*(.data..percpu..read_mostly) . = ALIGN((1 << (6)));
```

原文4行；静态定义内，运行NOT_RUN。

### PC86-E004 | template end and input pattern

[VERIFIED mykernel/arch/x86_64/kernel.lds::.data..percpu]

```ld
		*(.data..percpu) *(.data..percpu..shared_aligned) . = ALIGN((1 << 12));
		*(.data..percpu..decrypted) . = ALIGN((1 << 12));
		__per_cpu_end = .;
	}
```

原文4行；静态定义内，运行NOT_RUN。

### PC86-E005 | top-level hot-section size ASSERT independent of output section

[VERIFIED mykernel/arch/x86_64/kernel.lds::ASSERT]

```c
	ASSERT(__per_cpu_hot_end - __per_cpu_hot_start <= 64, "percpu cache hot data too large")
```

引文范围 84–84 为真实单一定义内连续1行；执行NOT_RUN。

### PC86-E006 | chunk_size storage type

[VERIFIED mykernel/mm/misc/percpu.c::chunk_size]

```c
size_t chunk_size = 0;
```

原文1行；静态定义内，运行NOT_RUN。

### PC86-E007 | base pointer definition

[VERIFIED mykernel/mm/misc/percpu.c::pcpu_base_addr]

```c
void *pcpu_base_addr __ro_after_init;
```

原文1行；静态定义内，运行NOT_RUN。

### PC86-E008 | offset array pointer definition

[VERIFIED mykernel/mm/misc/percpu.c::pcpu_unit_offsets]

```c
const ulong *pcpu_unit_offsets __ro_after_init;	/* cpu -> unit offset */
```

原文1行；静态定义内，运行NOT_RUN。

### PC86-E009 | first chunk pointer

[VERIFIED mykernel/mm/misc/percpu.c::pcpu_first_chunk]

```c
pcpu_chunk_s *pcpu_first_chunk __ro_after_init;
```

原文1行；静态定义内，运行NOT_RUN。

### PC86-E010 | reserved chunk pointer

[VERIFIED mykernel/mm/misc/percpu.c::pcpu_reserved_chunk]

```c
pcpu_chunk_s *pcpu_reserved_chunk __ro_after_init;
```

原文1行；静态定义内，运行NOT_RUN。

### PC86-E011 | free metadata int type

[VERIFIED mykernel/include/linux/smp/percpu_chunk.h::simple_pcpu_chunk]

```c
	struct simple_pcpu_chunk {
		List_s		list;			/* linked to pcpu_slot lists */
		int			free_bytes;		/* free bytes in the chunk */
		int			free_offset;
```

原文4行；静态定义内，运行NOT_RUN。

### PC86-E012 | chunk pointers

[VERIFIED mykernel/include/linux/smp/percpu_chunk.h::simple_pcpu_chunk]

```c
		void		*base_addr;
		// 通过memblock或者slub-buddy申请到的内存块的虚拟地址
		void		*data;			/* chunk data */
	};
```

原文4行；静态定义内，运行NOT_RUN。

### PC86-E013 | metadata alloc and null branch

[VERIFIED mykernel/mm/misc/percpu.c::early_pcpu_create_chunk]

```c
	pcpu_chunk_s *chunk =
		myos_memblock_alloc_DMA32(sizeof(*chunk), sizeof(*chunk));
	if (!chunk)
		return NULL;
```

原文4行；静态定义内，运行NOT_RUN。

### PC86-E014 | init and data allocation

[VERIFIED mykernel/mm/misc/percpu.c::early_pcpu_create_chunk]

```c
	INIT_LIST_HEAD(&chunk->list);
	chunk->free_offset = 0;
	chunk->free_bytes = chunk_size;
	chunk->data =
		myos_memblock_alloc_DMA32(nr_cpu_ids * chunk_size, chunk_size);
```

原文5行；静态定义内，运行NOT_RUN。

### PC86-E015 | data allocation then list insertion

[VERIFIED mykernel/mm/misc/percpu.c::early_pcpu_create_chunk]

```c
	chunk->data =
		myos_memblock_alloc_DMA32(nr_cpu_ids * chunk_size, chunk_size);
	list_header_add_to_tail(&pcpu_chunk_lists, &chunk->list);

	return chunk;
```

原文5行；静态定义内，运行NOT_RUN。

### PC86-E016 | static size and reserved size

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]

```c
void simple_pcpu_setup_first_chunk()
{
	// 计算关键全局数据
	const size_t static_size = __per_cpu_end - __per_cpu_start;
	size_t reserved_size = 2 * PAGE_SIZE;
```

原文5行；静态定义内，运行NOT_RUN。

### PC86-E017 | size sum and power-of-two chunk

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]

```c
	size_t dyn_size = 0;
	size_t size_sum = static_size + reserved_size + dyn_size;
	chunk_size = roundup_pow_of_two(size_sum);
```

原文3行；静态定义内，运行NOT_RUN。

### PC86-E018 | unchecked first chunk pointer deref

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]

```c
	pcpu_first_chunk = early_pcpu_create_chunk();
	pcpu_base_addr = pcpu_first_chunk->data;
	// 手工初始化 pcpu_first_chunk
```

原文3行；静态定义内，运行NOT_RUN。

### PC86-E019 | free metadata conversions

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]

```c
	pcpu_first_chunk->free_bytes = chunk_size - size_sum;
	pcpu_first_chunk->free_offset = size_sum;
	pcpu_first_chunk->base_addr = 0;
```

原文3行；静态定义内，运行NOT_RUN。

### PC86-E020 | single template memcpy

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]

```c
	// 复制“模板”percpu段到运行时cpu0的percpu段
	// 主要是将静态初始化的一些percpu变量的值复制过来
	memcpy(pcpu_first_chunk->data, &__per_cpu_load, static_size);
```

原文3行；静态定义内，运行NOT_RUN。

### PC86-E021 | offset allocation then static-size stride

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]

```c
	pcpu_unit_offsets =
		myos_memblock_alloc_normal(nr_cpu_ids * sizeof(void *), 8);
	for (int i = 0; i < nr_cpu_ids; i++)
		((ulong *)pcpu_unit_offsets)[i] = i * static_size;
```

原文4行；静态定义内，运行NOT_RUN。

### PC86-E022 | reserved chunk create and base difference

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]

```c
	// 创建pcpu_reserved_chunk
	pcpu_reserved_chunk = early_pcpu_create_chunk();
	pcpu_reserved_chunk->base_addr =
		pcpu_reserved_chunk->data - (ulong)pcpu_base_addr;
```

原文4行；静态定义内，运行NOT_RUN。

### PC86-E023 | alloc NULL and phys_to_virt

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_internal]

```c

	if (!alloc)
		return NULL;

	return (void *)phys_to_virt(alloc);
```

原文5行；静态定义内，运行NOT_RUN。

### PC86-E024 | DMA32 wrapper conditional memset

[VERIFIED mykernel/mm/early/memblock.c::myos_memblock_alloc_DMA32]

```c
myos_memblock_alloc_DMA32(size_t size, size_t align) {
	void *ptr = memblock_alloc_internal(size, align,
			MAX_DMA_PFN << PAGE_SHIFT, MAX_DMA32_PFN << PAGE_SHIFT);
	if (ptr)
		memset(ptr, 0, size);
```

原文5行；静态定义内，运行NOT_RUN。

### PC86-E025 | normal wrapper conditional memset

[VERIFIED mykernel/mm/early/memblock.c::myos_memblock_alloc_normal]

```c
myos_memblock_alloc_normal(size_t size, size_t align) {
	void *ptr = memblock_alloc_internal(size, align,
			MAX_DMA_PFN << PAGE_SHIFT, MEMBLOCK_ALLOC_ACCESSIBLE);
	if (ptr)
		memset(ptr, 0, size);
```

原文5行；静态定义内，运行NOT_RUN。

### PC86-E026 | roundup constant macro branch

[VERIFIED mykernel/include/linux/kernel/log2.h::roundup_pow_of_two]

```c
	#define roundup_pow_of_two(n)	(					\
				__builtin_constant_p(n) ? (				\
					((n) == 1) ? 1 :					\
						(1UL << (ilog2((n) - 1) + 1))	\
				) :										\
```

原文5行；静态定义内，运行NOT_RUN。

### PC86-E027 | roundup general macro branch

[VERIFIED mykernel/include/linux/kernel/log2.h::roundup_pow_of_two]

```c
					__roundup_pow_of_two(n)				\
			)
```

原文2行；静态定义内，运行NOT_RUN。

### PC86-E028 | roundup arithmetic helper

[VERIFIED mykernel/include/linux/kernel/log2.h::__roundup_pow_of_two]

```c
	__roundup_pow_of_two(ulong n) {
		return 1UL << fls_long(n - 1);
	}
```

原文3行；静态定义内，运行NOT_RUN。

### PC86-E029 | PAGE_SHIFT definition

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT]

```c
	#define PAGE_SHIFT				12
```

原文1行；静态定义内，运行NOT_RUN。

### PC86-E030 | PAGE_SIZE definition

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]

```c
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```

原文1行；静态定义内，运行NOT_RUN。

### PC86-E031 | reserved allocation macro boundary only; dynamic allocator not audited

[VERIFIED mykernel/include/linux/smp/percpu.h::__alloc_reserved_percpu]

```c
	#define __alloc_reserved_percpu(_size, _align)	\
				pcpu_alloc_noprof(_size, _align,	\
					true, GFP_KERNEL)
```

引文范围 34–36 为真实单一定义内连续3行；执行NOT_RUN。

### PC86-E032 | CPU count global

[VERIFIED mykernel/arch/x86_64/myos/pre_init.c::nr_cpu_ids]

```c
unsigned			nr_cpu_ids;
```

固定time实际单一定义连续1行，未执行构建/CPU。

### PC86-E033 | CPU count assigned from core_num

[VERIFIED mykernel/arch/x86_64/myos/pre_init.c::get_smpinfo]

```c
static void get_smpinfo(myos2_tag_smpinfo_s *smp_info)
{
	nr_cpu_ids = smp_info->core_num;
}
```

固定time实际单一定义连续4行，未执行构建/CPU。

### PC86-E034 | CPU topo tag dispatch

[VERIFIED mykernel/arch/x86_64/myos/pre_init.c::parse_tag]

```c
		case MYOS2_TAG_TYPE_CPU_TOPO:
			myos2_tag_smpinfo_s *smpinfo = (myos2_tag_smpinfo_s *)tag;
			get_smpinfo(smpinfo);
			break;
```

固定time实际单一定义连续4行，未执行构建/CPU。

### PC86-E035 | contiguous loop macro excluding prior extern declaration

[VERIFIED mykernel/include/linux/kernel/cpumask.h::for_each_cpu]

```c
	#define for_each_cpu(cpu) \
				for (int cpu = 0; cpu < nr_cpu_ids; cpu++)
```

引文范围 19–20 为真实单一定义内连续2行；执行NOT_RUN。

### PC86-E036 | possible CPU macro alias

[VERIFIED mykernel/include/linux/kernel/cpumask.h::for_each_possible_cpu]

```c
	#define for_each_possible_cpu(cpu)	for_each_cpu((cpu))
```

固定time实际单一定义连续1行，未执行构建/CPU。

### PC86-E037 | array bound from CONFIG_NR_CPUS

[VERIFIED mykernel/include/linux/kernel/threads.h::NR_CPUS]

```c
	#define NR_CPUS				CONFIG_NR_CPUS
```

固定time实际单一定义连续1行，未执行构建/CPU。

### PC86-E038 | CONFIG_NR_CPUS flag string

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]

```cmake
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

固定time实际单一定义连续4行，未执行构建/CPU。

### PC86-E039 | boot offset perCPU definition

[VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::this_cpu_off]

```c
DEFINE_PER_CPU_READ_MOSTLY(ulong, this_cpu_off) = BOOT_PERCPU_OFFSET;
```

固定time实际单一定义连续1行，未执行构建/CPU。

### PC86-E040 | fixed-size offset array zero storage

[VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::__per_cpu_offset]

```c
unsigned long __per_cpu_offset[NR_CPUS] __ro_after_init = {
	// [0 ... NR_CPUS-1] = BOOT_PERCPU_OFFSET,
	[0 ... NR_CPUS-1] = 0,
};
```

固定time实际单一定义连续4行，未执行构建/CPU。

### PC86-E041 | simple setup then delta and loop

[VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]

```c
	simple_pcpu_setup_first_chunk();
	/* alrighty, percpu areas up and running */
	delta = (ulong)pcpu_base_addr - (ulong)__per_cpu_start;
	for_each_possible_cpu(cpu) {
		per_cpu_offset(cpu) = delta + pcpu_unit_offsets[cpu];
```

固定time实际单一定义连续5行，未执行构建/CPU。

### PC86-E042 | perCPU offset/this_cpu_off/cpu_number assignments

[VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]

```c
		per_cpu_offset(cpu) = delta + pcpu_unit_offsets[cpu];
		per_cpu(this_cpu_off, cpu) = per_cpu_offset(cpu);
		per_cpu(pcpu_hot.cpu_number, cpu) = cpu;
```

固定time实际单一定义连续3行，未执行构建/CPU。

### PC86-E043 | per_cpu_offset macro only, not extern or conditional guard

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::per_cpu_offset]

```c
		#	define per_cpu_offset(x) (__per_cpu_offset[x])
```

引文范围 34–34 为真实单一定义内连续1行；执行NOT_RUN。

### PC86-E044 | perCPU section basename macro alone

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::PER_CPU_BASE_SECTION]

```c
		#	define PER_CPU_BASE_SECTION ".data..percpu"
```

引文范围 38–38 为真实单一定义内连续1行；执行NOT_RUN。

### PC86-E045 | perCPU variable section attribute

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::__PCPU_ATTRS]

```c
	#define __PCPU_ATTRS(sec)	__percpu __attribute__(	\
				(section(PER_CPU_BASE_SECTION sec))		\
			)
```

固定time实际单一定义连续3行，未执行构建/CPU。

### PC86-E046 | read-mostly perCPU definition macro

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::DEFINE_PER_CPU_READ_MOSTLY]

```c
	#define DEFINE_PER_CPU_READ_MOSTLY(type, name)		\
				DEFINE_PER_CPU_SECTION(type, name,		\
					"..read_mostly")
```

固定time实际单一定义连续3行，未执行构建/CPU。

### PC86-E047 | perCPU pointer shift using RELOC_HIDE

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::SHIFT_PERCPU_PTR]

```c
	#define SHIFT_PERCPU_PTR(__p, __offset)								\
			RELOC_HIDE(													\
				(typeof(*(__p)) __kernel __force *)(__p), (__offset)	\
			)
```

固定time实际单一定义连续4行，未执行构建/CPU。

### PC86-E048 | CPU-specific variable address formula

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::per_cpu_ptr]

```c
	#define per_cpu_ptr(ptr, cpu)	({		\
				__verify_pcpu_ptr(ptr);		\
				SHIFT_PERCPU_PTR((ptr),		\
					per_cpu_offset((cpu)));	\
			})
```

固定time实际单一定义连续5行，未执行构建/CPU。

### PC86-E049 | per_cpu macro without adjacent this_cpu_ptr macro

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::per_cpu]

```c
	#define per_cpu(var, cpu)	(*per_cpu_ptr(&(var), cpu))
```

引文范围 251–251 为真实单一定义内连续1行；执行NOT_RUN。

### PC86-E050 | RELOC_HIDE macro excludes #ifndef

[VERIFIED mykernel/include/linux/compiler/compiler.h::RELOC_HIDE]

```c
# define RELOC_HIDE(ptr, off)					\
  ({ unsigned long __ptr;					\
	 __ptr = (unsigned long) (ptr);				\
	(typeof(ptr)) (__ptr + (off)); })
```

引文范围 166–169 为真实单一定义内连续4行；执行NOT_RUN。

### PC86-E051 | BOOT_PERCPU_OFFSET macro excludes prior include

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::BOOT_PERCPU_OFFSET]

```c
	#define BOOT_PERCPU_OFFSET ((unsigned long)__per_cpu_load)
```

引文范围 255–255 为真实单一定义内连续1行；执行NOT_RUN。

### PC86-E052 | rdgsbase read wrapper excludes prior extern declaration

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::raw_cpu_read_long]

```c
	#define raw_cpu_read_long(pcp)	((ulong)rdgsbase())
```

引文范围 208–208 为真实单一定义内连续1行；执行NOT_RUN。

### PC86-E053 | read GS base and add perCPU symbol address

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::arch_raw_cpu_ptr]

```c
	#define arch_raw_cpu_ptr(_ptr)	({							\
				ulong tcp_ptr__ =								\
						raw_cpu_read_long(this_cpu_off);		\
				tcp_ptr__ += (__force ulong)(_ptr);				\
				(typeof(*(_ptr)) __kernel __force *)tcp_ptr__;	\
```

固定time实际单一定义连续5行，未执行构建/CPU。

### PC86-E054 | hot structure fields current_task and cpu_number

[VERIFIED mykernel/arch/x86_64/include/asm/current.h::pcpu_hot]

```c
			struct {
				task_s		*current_task;
				int			preempt_count;
				int			cpu_number;
				u64			call_depth;
```

固定time实际单一定义连续5行，未执行构建/CPU。

### PC86-E055 | hot template static values

[VERIFIED mykernel/arch/x86_64/smp/percpu_area.c::pcpu_hot]

```c
DEFINE_PER_CPU_CACHE_ALIGNED(pcpu_hot_s, pcpu_hot) = {
	.current_task	= &init_task,
	.preempt_count	= INIT_PREEMPT_COUNT,
	.top_of_stack	= TOP_OF_INIT_STACK,
};
```

固定time实际单一定义连续5行，未执行构建/CPU。

### PC86-E056 | standalone current_task separate template

[VERIFIED mykernel/arch/x86_64/smp/percpu_area.c::current_task]

```c
DEFINE_PER_CPU_CACHE_HOT(task_s *, current_task) = &init_task;
```

固定time实际单一定义连续1行，未执行构建/CPU。

### PC86-E057 | current consumer returns pointer from hot area

[VERIFIED mykernel/arch/x86_64/include/asm/current.h::get_current]

```c
	static __always_inline task_s *get_current(void) {
		return *this_cpu_ptr(&pcpu_hot.current_task);
	}
```

固定time实际单一定义连续3行，未执行构建/CPU。

### PC86-E058 | start_kernel calls setup_arch then setup_per_cpu_areas

[VERIFIED mykernel/init/main.c::start_kernel]

```c
	setup_arch(NULL);

	setup_per_cpu_areas();
```

固定time单一定义连续3行；硬件指令完成NOT_RUN。

### PC86-E059 | CPU0 conditional GDT block entry

[VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]

```c
		if (cpu == 0)
		{
			/**
			 * switch_gdt_and_percpu_base - Switch to direct GDT and runtime per CPU base
```

固定time单一定义连续4行；硬件指令完成NOT_RUN。

### PC86-E060 | CPU0 load_direct_gdt actual call

[VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]

```c
			// void __init switch_gdt_and_percpu_base(int cpu)
			// {
				load_direct_gdt(cpu);
```

固定time单一定义连续3行；硬件指令完成NOT_RUN。

### PC86-E061 | CPU0 requested wrmsrl GS_BASE and loop end

[VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]

```c
				// wrmsrl(MSR_GS_BASE, cpu_kernelmode_gs_base(cpu));
				wrmsrl(MSR_GS_BASE, per_cpu_offset(cpu));
			// }
		}
	}
```

固定time单一定义连续5行；硬件指令完成NOT_RUN。

### PC86-E062 | GDT descriptor address and size

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::load_direct_gdt]

```c
{
	struct desc_ptr gdt_descr;

	gdt_descr.address = (ulong)get_cpu_gdt_rw(cpu);
	gdt_descr.size = GDT_SIZE - 1;
```

固定time单一定义连续5行；硬件指令完成NOT_RUN。

### PC86-E063 | load_gdt then segment refresh call

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::load_direct_gdt]

```c
	load_gdt(&gdt_descr);

	refresh_segment_registers();
}
```

固定time单一定义连续4行；硬件指令完成NOT_RUN。

### PC86-E064 | GDT lookup function excludes preceding specifier

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::get_cpu_gdt_rw]

```c
		*get_cpu_gdt_rw(uint cpu) {
			return per_cpu(gdt_page, cpu).gdt;
		}
```

引文范围 85–87 为真实单一定义内连续3行；执行NOT_RUN。

### PC86-E065 | lgdt instruction wrapper

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::load_gdt]

```c
		load_gdt(const struct desc_ptr *dtr) {
			asm volatile(	"lgdt	%0		\t\n"
						:
						:	"m" (*dtr)
						);
```

固定time单一定义连续5行；硬件指令完成NOT_RUN。

### PC86-E066 | save rsp and request ss refresh

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::refresh_segment_registers]

```c
		refresh_segment_registers() {

			asm volatile(	"movq	%%rsp,		%%rax		\n\t"
							"mov 	%0,			%%ss		\n\t"
							"movq	%%rax,		%%rsp		\n\t"
```

固定time单一定义连续5行；硬件指令完成NOT_RUN。

### PC86-E067 | near label and lretq requests

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h::refresh_segment_registers]

```c
							"xor	%%rax,		%%rax		\n\t"
							"leaq	1f(%%rip),	%%rax		\n\t"
							"pushq	%1						\n\t"
							"pushq	%%rax					\n\t"
							"lretq							\n\t"
```

固定time单一定义连续5行；硬件指令完成NOT_RUN。

### PC86-E068 | GDT 16 entries macro

[VERIFIED mykernel/arch/x86_64/processor/desc_const_arch.h::GDT_ENTRIES]

```c
	#define GDT_ENTRIES				16
```

固定time单一定义连续1行；硬件指令完成NOT_RUN。

### PC86-E069 | GDT size macro

[VERIFIED mykernel/arch/x86_64/processor/desc_const_arch.h::GDT_SIZE]

```c
	#define GDT_SIZE				(GDT_ENTRIES * 8)
```

固定time单一定义连续1行；硬件指令完成NOT_RUN。

### PC86-E070 | page-aligned source GDT template initializer

[VERIFIED mykernel/arch/x86_64/smp/percpu_area.c::gdt_page]

```c
DEFINE_PER_CPU_PAGE_ALIGNED(struct gdt_page, gdt_page) = { .gdt = {
	[GDT_ENTRY_INVALID_SEG]			= GDT_ENTRY_INIT(0, 0, 0),
```

固定time单一定义连续2行；硬件指令完成NOT_RUN。

### PC86-E071 | GS base MSR selector macro

[VERIFIED mykernel/arch/x86_64/insns/x86msr_const.h::MSR_GS_BASE]

```c
	#define MSR_GS_BASE			0xc0000101 /* 64bit GS base */
```

固定time单一定义连续1行；硬件指令完成NOT_RUN。

### PC86-E072 | MSR wrapper separates low/high pieces

[VERIFIED mykernel/arch/x86_64/insns/x86msr.h::wrmsrl]

```c
		void
		wrmsrl(unsigned int msr, u64 val) {
			__wrmsr(msr, (u32)(val & 0xffffffffULL),
					(u32)(val >> 32));
		}
```

固定time单一定义连续5行；硬件指令完成NOT_RUN。

### PC86-E073 | MSR write function body excludes preceding specifier

[VERIFIED mykernel/arch/x86_64/insns/x86msr.h::__wrmsr]

```c
		__wrmsr(unsigned int msr, u32 low, u32 high) {
			asm volatile(	"1: wrmsr			\n"
							"2:					\n"
```

引文范围 96–98 为真实单一定义内连续3行；执行NOT_RUN。

### PC86-E074 | GSBASE read function excludes preceding specifier

[VERIFIED mykernel/arch/x86_64/insns/fsgsbase.h::rdgsbase]

```c
		rdgsbase(void) {
			unsigned long gsbase;
			asm volatile(	"rdgsbase	%0		\t\n"
```

引文范围 55–57 为真实单一定义内连续3行；执行NOT_RUN。

### PC86-E075 | rdgsbase output and return

[VERIFIED mykernel/arch/x86_64/insns/fsgsbase.h::rdgsbase]

```c
						:	"=r" (gsbase)
						:
						:	"memory");
			return gsbase;
		}
```

固定time单一定义连续5行；硬件指令完成NOT_RUN。

### PC86-E076 | early assembly per CPU offset indexing

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::secondary_startup_64]

```asm
.Lsetup_cpu:
	/* Get the per cpu offset for the given CPU# which is in ECX */
	movq	__per_cpu_offset(,%rcx,8),	%rdx
```

固定time单一定义连续3行；硬件指令完成NOT_RUN。

### PC86-E077 | early current_task and stack offset consumer

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::secondary_startup_64]

```asm
	movq	current_task(%rdx),			%rax
	movq	TASK_threadsp(%rax),		%rsp
```

固定time单一定义连续2行；硬件指令完成NOT_RUN。

