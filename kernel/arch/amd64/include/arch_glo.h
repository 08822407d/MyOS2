#ifndef _AMD64_GLO_H_
#define _AMD64_GLO_H_

#include "archtypes.h"

	extern desctblptr64_s	gdt_ptr;
	extern desctblptr64_s	idt_ptr;
	extern tss64_s **		tss_ptr_arr;

#endif /* _AMD64_GLO_H_ */