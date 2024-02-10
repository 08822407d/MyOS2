// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel/bcd.h>
#include <linux/kernel/export.h>

unsigned _bcd2bin(unsigned char val) {
	return (val & 0x0f) + (val >> 4) * 10;
}

unsigned char _bin2bcd(unsigned val) {
	return ((val / 10) << 4) + val % 10;
}
