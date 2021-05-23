#include "include/device.h"

void devices_init()
{
	init_keyboard();
	HPET_init();
	init_disk();
}