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

	init_cpu();
	SMP_init();

    while(1){
		if(p_kb->count)
			analysis_keycode();
	};
}