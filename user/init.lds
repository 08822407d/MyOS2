/***************************************************
*		 Copyright (c) 2018 MINE 田宇
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of version 2 of the GNU General Public
* License as published by the Free Software Foundation.
*
***************************************************/

OUTPUT_FORMAT("elf64-x86-64","elf64-x86-64","elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(main)
SECTIONS
{
	. = 0x6000000;
	.text ALIGN(0x200000) :
	{
		_text = .;
		*/init.c.o(.text);
		*(.text)
		_etext = .;
	}

	.rodata ALIGN(0x200000) : 
	{
		_rodata = .;	
		*(.rodata)
		_erodata = .;
	}
	
	. = ALIGN(8);
	.data ALIGN(0x200000) :
	{
		_data = .;
		*(.data)		
		_edata = .;
	}
	.bss :
	{
		_bss = .;
		*(.bss)
		_ebss = .;
	}

	_end = .;
}