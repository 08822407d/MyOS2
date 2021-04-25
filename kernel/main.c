#include <lib/string.h>
#include <lib/assert.h>

#include "include/glo.h"
#include "include/proto.h"
#include "include/proc.h"
#include "include/printk.h"

#include "arch/amd64/include/arch_proto.h"
#include "arch/amd64/include/device.h"

void kmain(void)
{
    prot_init();

	devices_init();
	// init_proc0();
	// int i = 1 / 0;

	// void * testp = kmalloc(60);
	// unsigned long * atestp = (unsigned long *)testp;
	// *atestp = 0x123456789abcdef;
	// *(atestp + 2) = 0xfedcba987654321;
	// kfree(testp);
	
	SMP_init();

    while(1){
		if(p_kb->count)
			analysis_keycode();
	};
}