#ifndef _TTY_H_
#define _TTY_H_

#include "device.h"

	cdev_s * find_tty(void);
	void kputchar(unsigned int *fb, int Xresol, int x, int y, unsigned int FRcolor, unsigned int BKcolor, unsigned char font);
	void tty_write_color(const char * buf, size_t length, unsigned int FRcolor, unsigned int BKcolor);

#endif /* _TTY_H_ */