#ifndef _LINUX_HIGHMEM_H_
#define _LINUX_HIGHMEM_H_

	#include <linux/compiler/myos_optimize_option.h>
	#include <linux/kernel/mm.h>


	#ifdef DEBUG

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
	
	#if defined(HIGHMEM_DEFINATION) || !(DEBUG)

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