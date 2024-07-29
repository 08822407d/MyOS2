#include "../mm_types_arch.h"

#include "vm_map_api_arch.h"

#include "uapi/linux/resource.h"


void
arch_pick_mmap_layout(mm_s * mm, rlimit_s *rlim_stack)
{
	mm->get_unmapped_area = simple_get_unmapped_area;

	// if (mmap_is_legacy())
	// 	mm->get_unmapped_area = arch_get_unmapped_area;
	// else
	// 	mm->get_unmapped_area = arch_get_unmapped_area_topdown;

	// arch_pick_mmap_base(&mm->mmap_base, &mm->mmap_legacy_base,
	// 		arch_rnd(mmap64_rnd_bits), task_size_64bit(0),
	// 		rlim_stack);
}