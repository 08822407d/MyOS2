/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_DIRENT_H
#define _LINUX_DIRENT_H

#include <stdint.h>

	typedef struct linux_dirent64 {
		uint64_t	d_ino;
		int64_t		d_off;
		unsigned short	d_reclen;
		unsigned char	d_type;
		char		d_name[];
	} linux_dirent64_s;

#endif