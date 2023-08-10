OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(startup_64)

_k_phys_start	= 0x1000000;
_k_virt_start	= 0xffff800001000000;
_k_offset		= _k_virt_start - _k_phys_start;
_k_phys_edata	= _edata - _k_offset;
_k_phys_ebss	= __bss_stop - _k_offset;

_APboot_phys_start			= 0x4000;

SECTIONS
{
	/* the head of kernel running in low-half mem */
	. = _k_phys_start;
	.head.text :
	{
		*(.unpaged.text);
	}
	.head.data ALIGN(0x1000) :
	{
		*(.unpaged.data);
	}

	/* AP boot from real-mode, related code and data will be copied to */
	/* begin 1MB of memory, so the symbols need relocated */
	apb_start_viraddr = ALIGN(0x1000);
	. = _APboot_phys_start;
	.APboot.text ALIGN(0x1000) : AT(apb_start_viraddr)
	{
		_APboot_text = apb_start_viraddr - _APboot_phys_start;
		KEEP(*(.AP_boot.text));
		_APboot_etext = . + apb_start_viraddr - _APboot_phys_start;
	}
	. = _APboot_etext;


	/* here is the kernel in high-half meme */
	. += _k_offset;
	.text ALIGN(0x1000) : AT(ADDR(.text) - _k_offset)
	{
		_text = .;
		*(.text*)
		_etext = .;
	}
	.rodata ALIGN(0x1000) : AT(ADDR(.rodata) - _k_offset)
	{
		__start_rodata = .;
		*(.rodata*)
		__end_rodata = .;
	}
	.data ALIGN(0x1000) : AT(ADDR(.data) - _k_offset)
	{
		_sdata = .;
		*(.data*)
		_edata = .;
	}
	.bss ALIGN(0x1000) : AT(ADDR(.bss) - _k_offset)
	{
		__bss_start = .;
		*(.bss* COMMON)
		__bss_stop = .;
	}
	.brk ALIGN(0x1000) : AT(ADDR(.brk) - _k_offset)
	{
		__brk_base = .;
		*(.brk*)
		__brk_limit = .;
	}
	.eh_frame ALIGN(0x1000) : AT(ADDR(.eh_frame) - _k_offset)
	{
		_eh_frame = .;
		*(.eh_frame)
		_eeh_frame = .;
	}

	. = ALIGN((0x1000));
	_end = .;
}