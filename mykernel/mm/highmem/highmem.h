#ifndef _LINUX_HIGHMEM_H_
#define _LINUX_HIGHMEM_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../mm_const.h"
	#include "../mm_types.h"
	#include "../mm_api.h"


	#ifdef DEBUG

		extern void
		copy_user_highpage(page_s *to, page_s *from,
				ulong vaddr, vma_s *vma);

		extern void
		copy_highpage(page_s *to, page_s *from);

		extern void
		memcpy_page(page_s *dst_page, size_t dst_off,
				page_s *src_page, size_t src_off, size_t len);

		extern void
		memset_page(page_s *page, size_t offset, int val, size_t len);

		extern void
		memcpy_from_page(char *to, page_s *page, size_t offset, size_t len);

		extern void
		memcpy_to_page(page_s *page, size_t offset,
				const char *from, size_t len);

		extern void
		memzero_page(page_s *page, size_t offset, size_t len);

	#endif

	#include "highmem_macro.h"
	
	#if defined(HIGHMEM_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		void
		copy_user_highpage(page_s *to, page_s *from,
				ulong vaddr, vma_s *vma) {

			// char *vfrom, *vto;

			// vfrom = kmap_local_page(from);
			// vto = kmap_local_page(to);
			// copy_user_page(vto, vfrom, vaddr, to);
			// kmsan_unpoison_memory(page_address(to), PAGE_SIZE);
			// kunmap_local(vto);
			// kunmap_local(vfrom);
			copy_user_page((void *)page_to_virt(to),
					(void *)page_to_virt(from), vaddr, to);
		}

		PREFIX_STATIC_INLINE
		void
		copy_highpage(page_s *to, page_s *from) {
			// char *vfrom, *vto;

			// vfrom = kmap_local_page(from);
			// vto = kmap_local_page(to);
			// kmsan_copy_page_meta(to, from);
			// kunmap_local(vto);
			// kunmap_local(vfrom);
			copy_page((void *)page_to_virt(to), (void *)page_to_virt(from));
		}

		PREFIX_STATIC_INLINE
		void
		memcpy_page(page_s *dst_page, size_t dst_off,
				page_s *src_page, size_t src_off, size_t len) {

			// char *dst = kmap_local_page(dst_page);
			// char *src = kmap_local_page(src_page);

			// VM_BUG_ON(dst_off + len > PAGE_SIZE || src_off + len > PAGE_SIZE);
			// memcpy(dst + dst_off, src + src_off, len);
			// kunmap_local(src);
			// kunmap_local(dst);
		}

		PREFIX_STATIC_INLINE
		void
		memset_page(page_s *page, size_t offset, int val, size_t len) {

			// char *addr = kmap_local_page(page);

			// VM_BUG_ON(offset + len > PAGE_SIZE);
			// memset(addr + offset, val, len);
			// kunmap_local(addr);
		}

		PREFIX_STATIC_INLINE
		void
		memcpy_from_page(char *to, page_s *page, size_t offset, size_t len) {
			// char *from = kmap_local_page(page);

			// VM_BUG_ON(offset + len > PAGE_SIZE);
			// memcpy(to, from + offset, len);
			// kunmap_local(from);

			virt_addr_t page_vaddr = page_to_virt(page);
			memcpy(to, (void *)(page_vaddr + offset), len);
		}

		PREFIX_STATIC_INLINE
		void
		memcpy_to_page(page_s *page, size_t offset,
				const char *from, size_t len) {

			// char *to = kmap_local_page(page);

			// VM_BUG_ON(offset + len > PAGE_SIZE);
			// memcpy(to + offset, from, len);
			// flush_dcache_page(page);
			// kunmap_local(to);

			virt_addr_t page_vaddr = page_to_virt(page);
			memcpy((void *)(page_vaddr + offset), from, len);
		}

		PREFIX_STATIC_INLINE
		void
		memzero_page(page_s *page, size_t offset, size_t len) {

			// char *addr = kmap_local_page(page);

			// VM_BUG_ON(offset + len > PAGE_SIZE);
			// memset(addr + offset, 0, len);
			// flush_dcache_page(page);
			// kunmap_local(addr);
		}

	#endif

#endif /* _LINUX_HIGHMEM_H_ */