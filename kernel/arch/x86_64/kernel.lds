OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(startup_64)

_k_phys_start	= 0x1000000;
_k_virt_start	= 0xffff800001000000;
_k_offset		= _k_virt_start - _k_phys_start;
_k_phys_edata	= _edata - _k_offset;
_k_phys_ebss	= _ebss - _k_offset;

__k_unpaged__k_offset		= _k_offset;
__k_unpaged__k_virt_start	= _k_virt_start;
__k_unpaged__k_phys_start	= _k_phys_start;

_APboot_phys_start			= 0x4000;

SECTIONS
{
	. = _k_phys_start;
	__k_unpaged__kern_unpaged_start = .;
	.head.text :
	{
		*(.unpaged.text);
	}
	.head.data ALIGN(0x1000) :
	{
		*(.unpaged.data);
	}
	__k_unpaged__kern_unpaged_end = .;

	apb_start_viraddr = ALIGN(0x1000);
	. = _APboot_phys_start;
	.APboot.text ALIGN(0x1000) : AT(apb_start_viraddr)
	{
		_APboot_text = apb_start_viraddr - _APboot_phys_start;
		KEEP(*(.AP_boot.text));
		_APboot_etext = . + apb_start_viraddr - _APboot_phys_start;
	}
	. = _APboot_etext;


	. += _k_offset;
	.text ALIGN(0x1000) : AT(ADDR(.text) - _k_offset)
	{
		_text = .;
		*(.text)
		_etext = .;
	}
	.rodata ALIGN(0x1000) : AT(ADDR(.rodata) - _k_offset)
	{
		_rodata = .;
		*(.rodata*)
		_erodata = .;
	}
	.data ALIGN(0x1000) : AT(ADDR(.data) - _k_offset)
	{
		_data = .;
		*(.data)
		_edata = .;
	}
	.bss ALIGN(0x8000) : AT(ADDR(.bss) - _k_offset)
	{
		_bss = .;
		*(.bss COMMON)
		_ebss = .;
	}
	.eh_frame ALIGN(0x1000) : AT(ADDR(.eh_frame) - _k_offset)
	{
		_eh_frame = .;
		*(.eh_frame)
		_eeh_frame = .;
	}

	_end = .;
}