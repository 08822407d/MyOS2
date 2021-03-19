OUTPUT_FORMAT("elf64-x86-64","elf64-x86-64","elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(start)

_k_phy_start	= 0x400000;
_k_vir_start	= 0xffff800000400000;
_k_offset		= _k_vir_start - _k_phy_start;

__k_unpaged__k_offset		= _k_offset;
__k_unpaged__k_vir_start	= _k_vir_start;
__k_unpaged__k_phy_start	= _k_phy_start;

SECTIONS
{
	. = _k_phy_start;

	__k_unpaged__kern_unpaged_start = .;
	unpaged_text :
	{
		*/head.S.o(.text);
	}
	.unpaged_data ALIGN(0x1000) :
	{
		*/head.S.o(.data .rodata*);
	}
	.unpaged_bss  ALIGN(0x1000) :
	{
		*/head.S.o(.bss COMMON);
	}
	.unpaged_eh_frame ALIGN(0x1000) :
	{
		*/head.S.o(.eh_frame);
	}
	__k_unpaged__kern_unpaged_end = .;


	. += _k_offset;
	.text ALIGN(0x1000) : AT(ADDR(.text) - _k_offset)
	{
		_text = .;
		*.o(.text)
		_etext = .;
	}

	.data ALIGN(0x1000) : AT(ADDR(.data) - _k_offset)
	{
		_data = .;
		*.o(.data .rodata*)
		_edata = .;
	}
	.bss ALIGN(0x8000) : AT(ADDR(.bss) - _k_offset)
	{
		_bss = .;
		*.o(.bss COMMON)
		_ebss = .;
	}

	.eh_frame ALIGN(0x1000) : AT(ADDR(.eh_frame) - _k_offset)
	{
		_eh_frame = .;
		*.o(.eh_frame)
		_eeh_frame = .;
	}

	_end = .;
}