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
ENTRY(_start)
SECTIONS
{
	. = 0x400000;
	.text ALIGN(0x1000) :
	{
		_text = .;
		*(.crt1.text)
		*(.text*)
		*(.rodata*)
		_etext = .;
	}

	.data ALIGN(0x1000) :
	{
		_data = .;
		*(.data*)
		*(.bss)
		_edata = .;
	}

	_end = .;
}