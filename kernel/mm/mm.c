#include <obsolete/proto.h>
#include <linux/mm/page.h>
#include <linux/mm/slab.h>

void early_init_mm()
{
	preinit_page();
	preinit_slab();
}

void init_mm()
{
	init_page();
	init_slab();
}