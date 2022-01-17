#ifndef _K_TYPES_H_
#define _K_TYPES_H_

#include <sys/types.h>

#include <sys/cdefs.h>
#include <lib/utils.h>

#include <include/task.h>
#include <include/page.h>

#include <arch/amd64/include/archconst.h>
#include <arch/amd64/include/keyboard.h>
#include <arch/amd64/include/multiboot2.h>

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


#endif /* _K_TYPES_H_ */