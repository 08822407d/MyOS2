#include <lib/string.h>
#include <lib/assert.h>

#include "include/glo.h"
#include "include/proto.h"
#include "include/proc.h"
#include "include/printk.h"

#include "arch/amd64/include/arch_proto.h"

void kmain(void)
{
    prot_init();

	video_init();
	// clear screen
	memset(framebuffer.FB_virbase, 0, framebuffer.FB_size);

	LAPIC_IOAPIC_init();
	
	// init_proc0();
	// int i = 1 / 0;

	// void * testp = kmalloc(60);
	// unsigned long * atestp = (unsigned long *)testp;
	// *atestp = 0x123456789abcdef;
	// *(atestp + 2) = 0xfedcba987654321;
	// kfree(testp);

    while(1){};
}