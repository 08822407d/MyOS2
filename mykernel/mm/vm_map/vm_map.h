#ifndef _LINUX_VM_MAP_H_
#define _LINUX_VM_MAP_H_

	#include <linux/compiler/myos_debug_option.h>
	#include <uapi/linux/mman.h>

	#include "../mm_const.h"
	#include "../mm_types.h"
	#include "../mm_api.h"


	#ifdef DEBUG
	
		extern void
		vma_init(vma_s *vma, mm_s *mm);

		extern void
		vma_set_anonymous(vma_s *vma);

		extern bool
		vma_is_anonymous(vma_s *vma);
		extern bool
		vma_is_initial_heap(const vma_s *vma);
		extern bool
		vma_is_initial_stack(const vma_s *vma);
		extern bool
		vma_is_temporary_stack(vma_s *vma);
		extern bool
		vma_is_foreign(vma_s *vma);
		extern bool
		vma_is_accessible(vma_s *vma);
		extern bool
		is_shared_maywrite(vm_flags_t vm_flags);
		extern bool
		vma_is_shared_maywrite(vma_s *vma);

		extern vma_s
		*simple_find_vma(mm_s *mm, ulong addr);
		extern vma_s
		*simple_find_vma_topdown(mm_s *mm, ulong addr);

		extern vma_s
		*find_vma_prev(mm_s *mm, ulong addr, vma_s **pprev);

		extern vma_s
		*find_vma_prev_to_addr(mm_s *mm, ulong addr);
		extern vma_s
		*find_vma_next_to_addr(mm_s *mm, ulong addr);

		extern vma_s
		*find_vma_intersection(mm_s *mm, ulong start, ulong end);

		extern vma_s
		*vma_lookup(mm_s *mm, ulong addr);
		
		extern ulong
		vm_start_gap(vma_s *vma);

		extern ulong
		vm_end_gap(vma_s *vma);

		extern ulong
		vma_pages(vma_s *vma);

		extern vma_s
		*find_exact_vma(mm_s *mm, ulong vm_start, ulong vm_end);

		extern ulong
		calc_vm_prot_bits(ulong prot, ulong pkey);

		extern ulong
		calc_vm_flag_bits(ulong flags);

	#endif

	#include "vm_map_macro.h"
	
	#if defined(VM_MAP_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		void
		vma_init(vma_s *vma, mm_s *mm) {
			static const vm_ops_s dummy_vm_ops = {};
			memset(vma, 0, sizeof(*vma));
			INIT_LIST_S(&vma->list);
			// INIT_LIST_HEAD(&vma->anon_vma_chain);
			vma->vm_mm = mm;
			vma->vm_ops = &dummy_vm_ops;
		}

		PREFIX_STATIC_INLINE
		void
		vma_set_anonymous(vma_s *vma) {
			vma->vm_ops = NULL;
		}

		PREFIX_STATIC_INLINE
		bool
		vma_is_anonymous(vma_s *vma) {
			return !vma->vm_ops;
		}

		/*
		 * Indicate if the VMA is a heap for the given task; for
		 * /proc/PID/maps that is the heap of the main task.
		 */
		PREFIX_STATIC_INLINE
		bool
		vma_is_initial_heap(const vma_s *vma) {
			return vma->vm_start < vma->vm_mm->brk &&
						vma->vm_end > vma->vm_mm->start_brk;
		}

		/*
		 * Indicate if the VMA is a stack for the given task; for
		 * /proc/PID/maps that is the stack of the main task.
		 */
		PREFIX_STATIC_INLINE
		bool
		vma_is_initial_stack(const vma_s *vma) {
			/*
			 * We make no effort to guess what a given thread considers to be
			 * its "stack".  It's not even well-defined for programs written
			 * languages like Go.
			 */
			return vma->vm_start <= vma->vm_mm->start_stack &&
						vma->vm_end >= vma->vm_mm->start_stack;
		}

		PREFIX_STATIC_INLINE
		bool
		vma_is_temporary_stack(vma_s *vma) {
			int maybe_stack = vma->vm_flags & (VM_GROWSDOWN | VM_GROWSUP);

			if (!maybe_stack)
				return false;

			if ((vma->vm_flags & VM_STACK_INCOMPLETE_SETUP) ==
						VM_STACK_INCOMPLETE_SETUP)
				return true;

			return false;
		}

		PREFIX_STATIC_INLINE
		bool
		vma_is_foreign(vma_s *vma) {
			if (!current->mm)
				return true;

			if (current->mm != vma->vm_mm)
				return true;

			return false;
		}

		PREFIX_STATIC_INLINE
		bool
		vma_is_accessible(vma_s *vma) {
			return vma->vm_flags & VM_ACCESS_FLAGS;
		}

		PREFIX_STATIC_INLINE
		bool
		is_shared_maywrite(vm_flags_t vm_flags) {
			return (vm_flags & (VM_SHARED | VM_MAYWRITE)) ==
						(VM_SHARED | VM_MAYWRITE);
		}

		PREFIX_STATIC_INLINE
		bool
		vma_is_shared_maywrite(vma_s *vma) {
			return is_shared_maywrite(vma->vm_flags);
		}

		/**
		 * find_vma() - Find the VMA for a given address bottom-up.
		 * @mm: The mm_struct to check
		 * @addr: The address
		 *
		 * Returns: The VMA associated with addr.
		 * May return %NULL in the case of no VMA at addr.
		 */
		PREFIX_STATIC_INLINE
		vma_s
		*simple_find_vma(mm_s *mm, ulong addr) {
			vma_s *vma = NULL;
			for_each_vma(mm, vma) {
				if (addr >= vma->vm_start && addr < vma->vm_end)
					return vma;
			}
			return NULL;
		}
		/**
		 * find_vma() - Find the VMA for a given address top-down.
		 * @mm: The mm_struct to check
		 * @addr: The address
		 *
		 * Returns: The VMA associated with addr.
		 * May return %NULL in the case of no VMA at addr.
		 */
		PREFIX_STATIC_INLINE
		vma_s
		*simple_find_vma_topdown(mm_s *mm, ulong addr) {
			vma_s *vma = NULL;
			for_each_vma_topdown(mm, vma) {
				if (addr >= vma->vm_start && addr < vma->vm_end)
					return vma;
			}
			return NULL;
		}

		/*
		 * Same as find_vma, but also return a pointer to the previous VMA in *pprev.
		 */
		PREFIX_STATIC_INLINE
		vma_s
		*find_vma_prev(mm_s *mm, ulong addr, vma_s **pprev) {
			*pprev = NULL;
			vma_s *vma = simple_find_vma(mm, addr);
			if (vma)
				*pprev = vma_prev_vma(vma);
			return vma;
		}

		/*
		 * Find the vma(@retval) whose range is prev to @addr no intersection
		 */
		PREFIX_STATIC_INLINE
		vma_s
		*find_vma_prev_to_addr(mm_s *mm, ulong addr) {
			vma_s *vma = NULL;
			for_each_vma_topdown(mm, vma) {
				if (vma->vm_end < addr)
					return vma;
			}
			return NULL;
		}
		/*
		 * Find the vma(@retval) whose range is next to @addr no intersection
		 */
		PREFIX_STATIC_INLINE
		vma_s
		*find_vma_next_to_addr(mm_s *mm, ulong addr) {
			vma_s *vma = NULL;
			for_each_vma(mm, vma) {
				if (vma->vm_start > addr)
					return vma;
			}
			return NULL;
		}

		PREFIX_STATIC_INLINE
		bool
		__vma_intersect_with_range(vma_s *vma, ulong start, ulong end) {
			return !((vma->vm_end <= start) || (vma->vm_start >= end));
		}
		/**
		 * find_vma_intersection() - Look up the first VMA which intersects the interval
		 * @mm: The process address space.
		 * @start_addr: The inclusive start user address.
		 * @end_addr: The exclusive end user address.
		 *
		 * Returns: The first VMA within the provided range, %NULL otherwise.  Assumes
		 * start_addr < end_addr.
		 */
		PREFIX_STATIC_INLINE
		vma_s
		*find_vma_intersection(mm_s *mm, ulong start, ulong end) {
			// BUG_ON(start> end);
			while (start > end);

			vma_s *vma = find_vma_prev_to_addr(mm, start);

			if (vma != NULL && (vma = vma_next_vma(vma)) != NULL &&
					__vma_intersect_with_range(vma, start, end))
				return vma;
			else
				return NULL;
		}

		/**
		 * vma_lookup() - Find a VMA at a specific address
		 * @mm: The process address space.
		 * @addr: The user address.
		 *
		 * Return: The vm_area_struct at the given address, %NULL otherwise.
		 */
		PREFIX_STATIC_INLINE
		vma_s
		*vma_lookup(mm_s *mm, ulong addr) {
			vma_s *vma = simple_find_vma(mm, addr);
			if (vma != NULL && addr < vma->vm_start)
				vma = NULL;
			return vma;
		}

		PREFIX_STATIC_INLINE
		ulong
		vm_start_gap(vma_s *vma) {
			ulong vm_start = vma->vm_start;
			if (vma->vm_flags & VM_GROWSDOWN) {
				vm_start -= stack_guard_gap;
				if (vm_start > vma->vm_start)
					vm_start = 0;
			}
			return vm_start;
		}

		PREFIX_STATIC_INLINE
		ulong
		vm_end_gap(vma_s *vma) {
			ulong vm_end = vma->vm_end;
			if (vma->vm_flags & VM_GROWSUP) {
				vm_end += stack_guard_gap;
				if (vm_end < vma->vm_end)
					vm_end = -PAGE_SIZE;
			}
			return vm_end;
		}

		PREFIX_STATIC_INLINE
		ulong
		vma_pages(vma_s *vma) {
			return (vma->vm_end - vma->vm_start) >> PAGE_SHIFT;
		}

		/* Look up the first VMA which exactly match the interval vm_start ... vm_end */
		PREFIX_STATIC_INLINE
		vma_s
		*find_exact_vma(mm_s *mm, ulong vm_start, ulong vm_end) {
			vma_s *vma = simple_find_vma(mm, vm_start);
			if (vma && (vma->vm_start != vm_start ||
					vma->vm_end != vm_end))
				vma = NULL;
			return vma;
		}


		/*
		 * Combine the mmap "prot" argument into "vm_flags" used internally.
		 */
		PREFIX_STATIC_INLINE
		ulong
		calc_vm_prot_bits(ulong prot, ulong pkey) {
			return _calc_vm_trans(prot, PROT_READ, VM_READ) |
					_calc_vm_trans(prot, PROT_WRITE, VM_WRITE) |
					_calc_vm_trans(prot, PROT_EXEC, VM_EXEC) |
					arch_calc_vm_prot_bits(prot, pkey);
		}

		/*
		 * Combine the mmap "flags" argument into "vm_flags" used internally.
		 */
		PREFIX_STATIC_INLINE
		ulong
		calc_vm_flag_bits(ulong flags) {
			return _calc_vm_trans(flags, MAP_GROWSDOWN, VM_GROWSDOWN) |
					_calc_vm_trans(flags, MAP_LOCKED, VM_LOCKED) |
					_calc_vm_trans(flags, MAP_SYNC, VM_SYNC) |
					arch_calc_vm_flag_bits(flags);
		}

	#endif

#endif /* _LINUX_VM_MAP_H_ */