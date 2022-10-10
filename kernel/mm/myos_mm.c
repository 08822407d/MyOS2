#include <linux/mm/page.h>
#include <linux/mm/myos_slab.h>

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