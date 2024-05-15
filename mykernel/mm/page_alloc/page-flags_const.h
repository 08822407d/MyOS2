/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PAGE_FLAGS_CONST_H_
#define _LINUX_PAGE_FLAGS_CONST_H_

	/* Which page is the flag stored in */
	#define FOLIO_PF_ANY			0
	#define FOLIO_PF_HEAD			0
	#define FOLIO_PF_ONLY_HEAD		0
	#define FOLIO_PF_NO_TAIL		0
	#define FOLIO_PF_NO_COMPOUND	0
	#define FOLIO_PF_SECOND			1


	#define PAGE_TYPE_BASE			0xf0000000
	/* Reserve		0x0000007f to catch underflows of page_mapcount */
	#define PAGE_MAPCOUNT_RESERVE	-128
	#define PG_buddy				0x00000080
	#define PG_offline				0x00000100
	#define PG_table				0x00000200
	#define PG_guard				0x00000400

#endif /* _LINUX_PAGE_FLAGS_CONST_H_ */
