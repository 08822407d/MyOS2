/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_ELF_H
#define _LINUX_ELF_H

	#include <linux/kernel/types.h>
	#include <asm/elf.h>
	#include <uapi/linux/elf.h>

	#ifndef START_THREAD
	#	define START_THREAD(elf_ex, regs, elf_entry, start_stack) \
					start_thread(regs, elf_entry, start_stack)
	#endif
		typedef Elf64_Ehdr	elfhdr_t;
		typedef Elf64_Phdr	elf_phdr_t;
		typedef Elf64_Shdr	elf_shdr_t;
		typedef Elf64_Nhdr	elf_note_t;
		typedef Elf64_Off	elf_addr_t;
		typedef Elf64_Half	Elf_Half_t;
		typedef Elf64_Word	Elf_Word_t;
	#	define ELF_GNU_PROPERTY_ALIGN	ELF64_GNU_PROPERTY_ALIGN

#endif /* _LINUX_ELF_H */
