/* Original MyOS2 list types, macros and primitives, copied verbatim from time by the builder.
 * Hand-written here: the two typedef lines (same text as lib/lib_type_declaration.h). */
typedef struct list_head List_s;
typedef struct list_hdr List_hdr_s;
//@@ORIG time mykernel/lib/list/double_list_types.h struct list_head
//@@ORIG time mykernel/lib/list/double_list_types.h struct list_hdr
//@@ORIG time mykernel/lib/list/double_list_const.h macro LIST_POISON1
//@@ORIG time mykernel/lib/list/double_list_const.h macro LIST_POISON2
//@@ORIG time mykernel/include/uapi/linux/stddef.h macro container_of
//@@ORIG time mykernel/lib/list/double_list.h macro INIT_LIST_HEAD
//@@ORIG time mykernel/lib/list/double_list.h macro list_is_head
//@@ORIG time mykernel/lib/list/double_list_macro.h macro LIST_INIT
//@@ORIG time mykernel/lib/list/double_list_macro.h macro LIST_HEADER_INIT
//@@ORIG time mykernel/lib/list/double_list_macro.h macro list_container
//@@ORIG time mykernel/lib/list/double_list_macro.h macro list_entry
//@@ORIG time mykernel/lib/list/double_list_macro.h macro list_first_entry
//@@ORIG time mykernel/lib/list/double_list_macro.h macro list_for_each
//@@ORIG time mykernel/lib/list/double_list_macro.h macro list_headr_first_container
//@@ORIG time mykernel/lib/list/double_list_macro.h macro list_header_foreach
//@@ORIG time mykernel/lib/list/double_list.h func INIT_LIST_S
//@@ORIG time mykernel/lib/list/double_list.h func INIT_LIST_HEADER_S
//@@ORIG time mykernel/lib/list/double_list.h func __list_add_valid
//@@ORIG time mykernel/lib/list/double_list.h func __list_del_entry_valid
//@@ORIG time mykernel/lib/list/double_list.h func __list_add_between
//@@ORIG time mykernel/lib/list/double_list.h func list_add_to_next
//@@ORIG time mykernel/lib/list/double_list.h func list_add_to_prev
//@@ORIG time mykernel/lib/list/double_list.h func __list_del
//@@ORIG time mykernel/lib/list/double_list.h func __list_del_entry
//@@ORIG time mykernel/lib/list/double_list.h func list_del_init
//@@ORIG time mykernel/lib/list/double_list.h func list_is_head_anchor
//@@ORIG time mykernel/lib/list/double_list.h func list_is_empty_entry
//@@ORIG time mykernel/lib/list/double_list.h func list_header_is_empty
//@@ORIG time mykernel/lib/list/double_list.h func list_header_contains
//@@ORIG time mykernel/lib/list/double_list.h func list_header_add_to_head
//@@ORIG time mykernel/lib/list/double_list.h func list_header_remove_head
//@@ORIG time mykernel/lib/list/double_list.h func list_header_add_to_tail
//@@ORIG time mykernel/lib/list/double_list.h func list_header_delete_node

/* fixture-side observers (hand-written): bounded walk, never follows more than 16 links */
static int fx_walk_len(List_hdr_s *h)
{
	int n = 0;
	List_s *p = h->anchor.next;
	while (p != &h->anchor && n < 16) { p = p->next; n++; }
	return p == &h->anchor ? n : -1;
}
static int fx_occurrences(List_hdr_s *h, List_s *node)
{
	int n = 0, k = 0;
	List_s *p = h->anchor.next;
	while (p != &h->anchor && k < 16) { if (p == node) n++; p = p->next; k++; }
	return n;
}
