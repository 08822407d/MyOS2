#include "include/device.h"

void devices_init()
{
	keyboard_init();
	HPET_init();
}