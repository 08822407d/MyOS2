/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_HASH_LIST_TYPES_H_
#define _LINUX_HASH_LIST_TYPES_H_

		struct hlist_node;
		typedef struct hlist_node hlist_s;
		typedef struct hlist_head {
			hlist_s *first;
		} hlist_hdr_s;

		typedef struct hlist_node {
			hlist_s *next, **pprev;
		} hlist_s;

#endif /* _LINUX_HASH_LIST_TYPES_H_ */