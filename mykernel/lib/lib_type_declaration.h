#ifndef _LIB_TYPE_DECLARATION_H_
#define _LIB_TYPE_DECLARATION_H_

	#include <linux/kernel/types.h>

    #include "lib_const.h"


    /* double_list.c */
	struct list_head;
	typedef struct list_head List_s;

    /* hash_list.c */
    struct hlist_node;
    typedef struct hlist_node hlist_s;

#endif /* _LIB_TYPE_DECLARATION_H_ */