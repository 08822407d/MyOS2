OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(code_64_start)

_k_phys_start	= 0x1000000;
_k_virt_start	= 0xffff800001000000;
_k_offset		= _k_virt_start - _k_phys_start;
_k_phys_edata	= _edata - _k_offset;
_k_phys_loadend	= __bss_start - _k_offset;
_k_phys_ebss	= __bss_stop - _k_offset;

_APboot_phys_start		= 0xE000;
_PCPU_BASE		= 0x0;

_Page_Size		= 0x1000;

/* PHDRS {
 text PT_LOAD FLAGS(5);
 data PT_LOAD FLAGS(6);
 percpu PT_LOAD FLAGS(6);
 init PT_LOAD FLAGS(7);
 APboot PT_LOAD FLAGS(8);
 note PT_NOTE FLAGS(0);
} */
SECTIONS
{
	/* the head of kernel running in low-half mem */
	. = _k_phys_start;
	.head.text :
	{
		*(.unpaged.text);
	}
	.head.data ALIGN(_Page_Size) :
	{
		*(.unpaged.data);
	}

	/* AP boot from real-mode, related code and data will be copied to */
	/* start 1MB of memory, so the symbols need relocated */
	apb_start_viraddr = ALIGN(_Page_Size);
	.APboot.text _APboot_phys_start : AT(apb_start_viraddr)
	{
		_APboot_text = apb_start_viraddr - _APboot_phys_start;
		KEEP(*(.AP_boot.text));
		_APboot_etext = . + apb_start_viraddr - _APboot_phys_start;
	} . = apb_start_viraddr + SIZEOF(.APboot.text);


	/* here is the kernel in high-half meme */
	. += _k_offset;
	.text ALIGN(_Page_Size) : AT(ADDR(.text) - _k_offset)
	{
		_text = .;
		*(.text*)
		_etext = .;
	}
	.rodata ALIGN(_Page_Size) : AT(ADDR(.rodata) - _k_offset)
	{
		__start_rodata = .;
		*(.rodata*)
		__end_rodata = .;
	}
	.data ALIGN(_Page_Size) : AT(ADDR(.data) - _k_offset)
	{
		_sdata = .;
		*(.data)
		*(.data.rel*)
		*(.data..page_aligned*)
		_edata = .;
	}
	.data.sched_class ALIGN(8) : AT(ADDR(.data.sched_class) - _k_offset)
	{
		__sched_class_highest = .;
		KEEP(*(.data.sched_class))
		__sched_class_lowest = .;
	}
	. = ALIGN(_Page_Size);
	__per_cpu_load = .;
	.data..percpu _PCPU_BASE : AT(__per_cpu_load - _k_offset)
	{
		__per_cpu_start = .;
		*(.data..percpu..page_aligned) . = ALIGN((1 << 6));
		*(.data..percpu..read_mostly) . = ALIGN((1 << 6));
		*(.data..percpu) *(.data..percpu..shared_aligned) . = ALIGN(_Page_Size);
		__per_cpu_end = .;
	/* } :percpu . = __per_cpu_load + SIZEOF(.data..percpu); */
	} . = __per_cpu_load + SIZEOF(.data..percpu);
	ASSERT(SIZEOF(.data..percpu) < 0x1000000,
		"per-CPU data too large - increase CONFIG_PHYSICAL_START")
	. = ALIGN(_Page_Size);
	.init.text : AT(ADDR(.init.text) - _k_offset)
	{
		_sinittext = .;
		*(.init.text .init.text.*)
		*(.text.startup) _einittext = .;
	/* } :init */
	}
	.init.data : AT(ADDR(.init.data) - _k_offset)
	{
		*(.init.data init.data.*) . = ALIGN(8);
		/* KEEP(*(SORT(___kentry+*))) *(.init.data init.data.*) . = ALIGN(8);
		__start_mcount_loc = .;
		KEEP(*(__mcount_loc)) __stop_mcount_loc = .;
		ftrace_ops_list_func = arch_ftrace_ops_list_func; */
		*(.init.rodata .init.rodata.*) . = ALIGN(8);
		/* __start_ftrace_events = .;
		KEEP(*(_ftrace_events)) __stop_ftrace_events = .;
		__start_ftrace_eval_maps = .;
		KEEP(*(_ftrace_eval_map)) __stop_ftrace_eval_maps = .;
		. = ALIGN(8);
		__start_syscalls_metadata = .;
		KEEP(*(__syscalls_metadata)) __stop_syscalls_metadata = .;
		. = ALIGN(8); __start_kprobe_blacklist = .;
		KEEP(*(_kprobe_blacklist)) __stop_kprobe_blacklist = .;
		. = ALIGN(32);
		__start_error_injection_whitelist = .;
		KEEP(*(_error_injection_whitelist)) __stop_error_injection_whitelist = .;
		. = ALIGN(8);
		__clk_of_table = .;
		KEEP(*(__clk_of_table)) KEEP(*(__clk_of_table_end)) . = ALIGN(8);
		__cpu_method_of_table = .;
		KEEP(*(__cpu_method_of_table)) KEEP(*(__cpu_method_of_table_end)) . = ALIGN(8);
		__cpuidle_method_of_table = .;
		KEEP(*(__cpuidle_method_of_table)) KEEP(*(__cpuidle_method_of_table_end)) . = ALIGN(32);
		__dtb_start = .;
		KEEP(*(.dtb.init.rodata)) __dtb_end = .;
		. = ALIGN(8);
		__irqchip_acpi_probe_table = .;
		KEEP(*(__irqchip_acpi_probe_table)) __irqchip_acpi_probe_table_end = .;
		. = ALIGN(8);
		__timer_acpi_probe_table = .;
		KEEP(*(__timer_acpi_probe_table)) __timer_acpi_probe_table_end = .;
		. = ALIGN(8);
		__governor_thermal_table = .;
		KEEP(*(__governor_thermal_table)) __governor_thermal_table_end = .;
		. = ALIGN(8);
		__earlycon_table = .;
		KEEP(*(__earlycon_table)) __earlycon_table_end = .;
		. = ALIGN(8);
		__start_lsm_info = .;
		KEEP(*(.lsm_info.init)) __end_lsm_info = .;
		. = ALIGN(8);
		__start_early_lsm_info = .;
		KEEP(*(.early_lsm_info.init)) __end_early_lsm_info = .;
		. = ALIGN(8);
		__kunit_suites_start = .;
		KEEP(*(.kunit_test_suites)) __kunit_suites_end = .;
		. = ALIGN(16); __setup_start = .;
		KEEP(*(.init.setup)) __setup_end = .;
		__initcall_start = .;
		KEEP(*(.initcallearly.init)) __initcall0_start = .;
		KEEP(*(.initcall0.init)) KEEP(*(.initcall0s.init)) __initcall1_start = .;
		KEEP(*(.initcall1.init)) KEEP(*(.initcall1s.init)) __initcall2_start = .;
		KEEP(*(.initcall2.init)) KEEP(*(.initcall2s.init)) __initcall3_start = .;
		KEEP(*(.initcall3.init)) KEEP(*(.initcall3s.init)) __initcall4_start = .;
		KEEP(*(.initcall4.init)) KEEP(*(.initcall4s.init)) __initcall5_start = .;
		KEEP(*(.initcall5.init)) KEEP(*(.initcall5s.init)) __initcallrootfs_start = .;
		KEEP(*(.initcallrootfs.init)) KEEP(*(.initcallrootfss.init)) __initcall6_start = .;
		KEEP(*(.initcall6.init)) KEEP(*(.initcall6s.init)) __initcall7_start = .;
		KEEP(*(.initcall7.init)) KEEP(*(.initcall7s.init)) __initcall_end = .;
		__con_initcall_start = .;
		KEEP(*(.con_initcall.init)) __con_initcall_end = .;
		. = ALIGN(4); __initramfs_start = .;
		KEEP(*(.init.ramfs)) . = ALIGN(8);
		KEEP(*(.init.ramfs.info)) */
	}
	.bss ALIGN(_Page_Size) : AT(ADDR(.bss) - _k_offset)
	{
		__bss_start = .;
		*(.bss* COMMON)
		__bss_stop = .;
	}
	.brk ALIGN(_Page_Size) : AT(ADDR(.brk) - _k_offset)
	{
		__brk_base = .;
		*(.brk*)
		__brk_limit = .;
	}
	.eh_frame ALIGN(_Page_Size) : AT(ADDR(.eh_frame) - _k_offset)
	{
		_eh_frame = .;
		*(.eh_frame)
		_eeh_frame = .;
	}

	. = ALIGN((_Page_Size));
	_end = .;
}