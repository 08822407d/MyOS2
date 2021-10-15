#include <lib/string.h>

#include "include/device.h"
#include "include/tty.h"

void init_intr_dev(void);

void enum_char_dev(void);
void enum_block_dev(void);

void init_char_dev(void);
void init_block_dev(void);

List_hdr_s cdev_lhdr;
List_hdr_s bdev_lhdr;

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
dirent_s * creat_append_devdirent()
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
	list_hdr_init(&cdev_lhdr);
	list_hdr_append(&cdev_lhdr, &find_tty()->cdev_list);
}

void enum_block_dev()
{
	list_hdr_init(&bdev_lhdr);
}