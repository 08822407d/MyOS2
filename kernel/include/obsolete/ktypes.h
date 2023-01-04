#ifndef _K_TYPES_H_
#define _K_TYPES_H_

#include <linux/sched/myos_percpu.h>
#include <linux/kernel/types.h>
#include <linux/kernel/stddef.h>
#include <linux/mm/page.h>

#include <obsolete/archconst.h>
#include <obsolete/keyboard.h>

#define NR_BOOT_PROCS   (NR_TASKS + LAST_SPECIAL_PROC_NR + 1)

/*==============================================================================================*
 *									data structures for devices									*
 *==============================================================================================*/
	typedef struct keyboard_inputbuffer
	{
		unsigned char * p_head;
		unsigned char * p_tail;
		int count;
		unsigned char buf[KB_BUF_SIZE];
	} kbd_inbuf_s;

/* Number of processes contained in the system image. */
	typedef struct
	{
		// mm
		int		slab;
		// task
		int		idle_task;
		// vfs
		int		vfs;
	} init_flags_s;
	
/* This is used to obtain system information through SYS_GETINFO. */

	typedef struct kinfo {
		unsigned		nr_lcpu;
		init_flags_s	init_flags;
	} kinfo_s;

	typedef struct framebuffer
	{
		phys_addr_t	FB_phybase;
		virt_addr_t	FB_virbase;
		uint64_t	FB_size;

		uint32_t	PixperScanline;
		uint32_t	X_Resolution;
		uint32_t	Y_Resolution;
	} framebuffer_s;

	typedef struct time
	{
		int second;	//00
		int minute;	//02
		int hour;	//04
		int day;	//07
		int month;	//08
		int year;	//09+32
	} time_s;

#endif /* _K_TYPES_H_ */