#include <include/proto.h>
#include <include/page.h>
#include <include/slab.h>

void pre_init_mm()
{
	pre_init_page();
	pre_init_slab();
}

void init_mm()
{
	init_page();
	init_slab();
}