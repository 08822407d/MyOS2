#include "include/device.h"

void devices_init()
{
	init_keyboard();

	#ifdef USE_APIC
		HPET_init();
		LVT_timer_init();
	#endif

	init_disk();
}