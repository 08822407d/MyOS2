/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHEDULER_CONST_H_
#define _LINUX_SCHEDULER_CONST_H_


	/* Wake flags. The first three directly map to some SD flag value */
	#define WF_EXEC			0x02 /* Wakeup after exec; maps to SD_BALANCE_EXEC */
	#define WF_FORK			0x04 /* Wakeup after fork; maps to SD_BALANCE_FORK */
	#define WF_TTWU			0x08 /* Wakeup;            maps to SD_BALANCE_WAKE */

	#define WF_SYNC			0x10 /* Waker goes to sleep after wakeup */
	#define WF_MIGRATED		0x20 /* Internal use, task got migrated */

#endif /* _LINUX_SCHEDULER_CONST_H_ */