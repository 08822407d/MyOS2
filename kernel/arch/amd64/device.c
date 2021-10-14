#include "include/device.h"

void init_intr_dev(void);

void enum_char_dev(void);
void enum_block_dev(void);

void init_char_dev(void);
void init_block_dev(void);

void devices_init()
{
	init_intr_dev();

	enum_char_dev();
	enum_block_dev();

	init_char_dev();
	init_block_dev();
}

void creat_dev_file()
{

}

void init_intr_dev()
{
	#ifdef USE_APIC
		HPET_init();
		LVT_timer_init();
	#endif
}

void init_char_dev()
{
	init_keyboard();
}

void init_block_dev()
{
	init_disk();
}

void enum_char_dev()
{

}

void enum_block_dev()
{

}