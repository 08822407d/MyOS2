---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-DIRECT-MAP-TABLE-INSTALL-83
record_type: fixed_time_source_definition_evidence
source_pin_short12: a039d9803ade
base_snapshot: time
read_channel: connector
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-10
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第83批｜固定time证据与源内容事实键

仅有本件包含新源码短锚。每段均来自time固定对象真实符号定义范围，连续1–5原物理行，路径与定位单独回源；宏外条件、extern接口不冒作真正实现。编号与事实键一一固定，后续正文/矩阵/模型/DOT按事实键解析真实符号、原文和限定；不能从注释或函数名推出真实硬件效果。

## A01｜mapping_macro_init_true
[VERIFIED mykernel/arch/x86_64/mm/init.c::kernel_physical_mapping_init]
```c
#define kernel_physical_mapping_init(paddr_start,			\
				paddr_end, page_size_mask, prot)			\
			__kernel_physical_mapping_init(paddr_start,		\
					paddr_end, page_size_mask, prot, true)
```
固定time物理行27–30；定义范围27–30；事实键`mapping_macro_init_true`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A02｜change_macro_init_false
[VERIFIED mykernel/arch/x86_64/mm/init.c::kernel_physical_mapping_change]
```c
#define kernel_physical_mapping_change(paddr_start,			\
				paddr_end, page_size_mask)					\
			__kernel_physical_mapping_init(paddr_start,		\
					paddr_end, page_size_mask,				\
						PAGE_KERNEL, false)
```
固定time物理行38–42；定义范围38–42；事实键`change_macro_init_false`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A03｜root_signature
[VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
static ulong __meminit
__kernel_physical_mapping_init(ulong paddr_start,
		ulong paddr_end, ulong page_size_mask,
		pgprot_t prot, bool init) {
```
固定time物理行823–826；定义范围823–864；事实键`root_signature`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A04｜root_initial_addr_conversion
[VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
	ulong vaddr, vaddr_start, vaddr_end, vaddr_next, paddr_last;

	paddr_last = paddr_end;
	vaddr = (ulong)__va(paddr_start);
	vaddr_end = (ulong)__va(paddr_end);
```
固定time物理行829–833；定义范围823–864；事实键`root_initial_addr_conversion`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A05｜root_loop_and_boundary
[VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
	for (; vaddr < vaddr_end; vaddr = vaddr_next) {
		pgd_t *pgd = pgd_offset_k(vaddr);
		p4d_t *p4d;

		vaddr_next = (vaddr & PGDIR_MASK) + PGDIR_SIZE;
```
固定time物理行836–840；定义范围823–864；事实键`root_loop_and_boundary`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A06｜root_reuse_existing_nonzero
[VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
		if (pgd_val(*pgd)) {
			p4d = (p4d_t *)pgd_page_vaddr(*pgd);
			paddr_last = phys_p4d_init(p4d,
					__pa(vaddr), __pa(vaddr_end),
					page_size_mask, prot, init);
```
固定time物理行842–846；定义范围823–864；事实键`root_reuse_existing_nonzero`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A07｜root_alloc_before_child
[VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
		p4d = alloc_low_page();
		paddr_last = phys_p4d_init(p4d, __pa(vaddr),
			__pa(vaddr_end), page_size_mask, prot, init);

```
固定time物理行850–853；定义范围823–864；事实键`root_alloc_before_child`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A08｜root_publish_after_recursion
[VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
		spin_lock(&init_mm.page_table_lock);
		p4d_populate_init(&init_mm, p4d_offset(pgd, vaddr),
				(pud_t *) p4d, init);
		spin_unlock(&init_mm.page_table_lock);
```
固定time物理行854–857；定义范围823–864；事实键`root_publish_after_recursion`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A09｜root_return_and_sync_comment
[VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
		pgd_changed = true;
	}
	// if (pgd_changed)
	// 	sync_global_pgds(vaddr_start, vaddr_end - 1);
	return paddr_last;
```
固定time物理行859–863；定义范围823–864；事实键`root_return_and_sync_comment`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A10｜folded_p4d_macro
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_p4d_init]
```c
#define phys_p4d_init(p4d_page, paddr, paddr_end,					\
				page_size_mask, prot, init)							\
			phys_pud_init((pud_t *)(p4d_page), (paddr),				\
					(paddr_end), (page_size_mask), (prot), (init))
```
固定time物理行818–821；定义范围818–821；事实键`folded_p4d_macro`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A11｜pgd_in_mm_pointer
[VERIFIED mykernel/mm/fault/pgtable.h::pgd_entp_in_mm]
```c
		PREFIX_STATIC_INLINE
		pgd_t
		*pgd_entp_in_mm(mm_s *mmp, ulong vaddr) {
			return (mmp->pgd + pgd_index(vaddr));
		};
```
固定time物理行168–172；定义范围168–172；事实键`pgd_in_mm_pointer`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A12｜pgd_offset_k_alias
[VERIFIED mykernel/mm/fault/pgtable_macro.h::pgd_offset_k]
```c
	#define pgd_offset_k(address)	pgd_offset(&init_mm, (address))
```
固定time物理行82；定义范围82–82；事实键`pgd_offset_k_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A13｜pgd_index_alias
[VERIFIED mykernel/mm/fault/pgtable_macro.h::pgd_index]
```c
	#define pgd_index				ent_index_in_p4d
```
固定time物理行94；定义范围94–94；事实键`pgd_index_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A14｜root_index_mask
[VERIFIED mykernel/mm/fault/pgtable.h::ent_index_in_p4d]
```c
		PREFIX_STATIC_INLINE
		ulong
		ent_index_in_p4d(ulong vaddr) {
			return (vaddr >> P4D_SHIFT) & (PTRS_PER_P4D - 1);
		}
```
固定time物理行191–195；定义范围191–195；事实键`root_index_mask`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A15｜folded_p4d_cast
[VERIFIED mykernel/mm/fault/pgtable.h::p4d_entp_from_vaddr_and_pgd_entp]
```c
		p4d_t
		*p4d_entp_from_vaddr_and_pgd_entp(
				pgd_t *pgd_entp, ulong vaddr) {
			return ((p4d_t *)pgd_entp);
		};
```
固定time物理行197–201；定义范围196–201；事实键`folded_p4d_cast`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A16｜folded_p4d_offset_alias
[VERIFIED mykernel/mm/fault/pgtable_macro.h::p4d_offset]
```c
	#define p4d_offset		p4d_entp_from_vaddr_and_pgd_entp
```
固定time物理行77；定义范围77–77；事实键`folded_p4d_offset_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A17｜pud_index_alias
[VERIFIED mykernel/mm/fault/pgtable_macro.h::pud_index]
```c
	#define pud_index		ent_index_in_pud
```
固定time物理行72；定义范围72–72；事实键`pud_index_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A18｜pmd_index_alias
[VERIFIED mykernel/mm/fault/pgtable_macro.h::pmd_index]
```c
	#define pmd_index		ent_index_in_pmd
```
固定time物理行73；定义范围73–73；事实键`pmd_index_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A19｜pud_index_formula
[VERIFIED mykernel/mm/fault/pgtable.h::ent_index_in_pud]
```c
		PREFIX_STATIC_INLINE
		ulong
		ent_index_in_pud(ulong vaddr) {
			return (vaddr >> PUD_SHIFT) & (PTRS_PER_PUD - 1);
		}
```
固定time物理行232–236；定义范围232–236；事实键`pud_index_formula`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A20｜pmd_index_formula
[VERIFIED mykernel/mm/fault/pgtable.h::ent_index_in_pmd]
```c
		PREFIX_STATIC_INLINE
		ulong
		ent_index_in_pmd(ulong vaddr) {
			return (vaddr >> PMD_SHIFT) & (PTRS_PER_PMD - 1);
		}
```
固定time物理行273–277；定义范围273–277；事实键`pmd_index_formula`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A21｜pmd_offset_real
[VERIFIED mykernel/mm/fault/pgtable.h::pmd_entp_from_vaddr_and_pud_entp]
```c
		pmd_t
		*pmd_entp_from_vaddr_and_pud_entp(pud_t *pud_entp, ulong vaddr) {
			return (pmd_t *)pude_pointed_page_vaddr(*pud_entp) +
						ent_index_in_pmd(vaddr);
		}
```
固定time物理行280–284；定义范围279–284；事实键`pmd_offset_real`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A22｜pmd_offset_alias
[VERIFIED mykernel/mm/fault/pgtable_macro.h::pmd_offset]
```c
	#define pmd_offset		pmd_entp_from_vaddr_and_pud_entp
```
固定time物理行79；定义范围79–79；事实键`pmd_offset_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A23｜pgd_nonzero_alias
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_macro_arch.h::pgd_val]
```c
	#define pgd_val				p4d_val
```
固定time物理行97；定义范围97–97；事实键`pgd_nonzero_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A24｜root_existing_vaddr_alias
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_macro_arch.h::pgd_page_vaddr]
```c
	#define pgd_page_vaddr		p4d_page_vaddr
```
固定time物理行95；定义范围95–95；事实键`root_existing_vaddr_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A25｜p4d_pointed_page_virtual
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::p4de_pointed_page_vaddr]
```c
		PREFIX_STATIC_INLINE
		ulong
		p4de_pointed_page_vaddr(p4d_t p4d_ent) {
			return (ulong)__va(p4de_pointed_page_paddr(p4d_ent));
		}
```
固定time物理行312–316；定义范围312–316；事实键`p4d_pointed_page_virtual`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A26｜init_mm_root_pointer
[VERIFIED mykernel/mm/misc/init-mm.c::init_mm]
```c
mm_s init_mm = {
	// .mm_mt				= MTREE_INIT_EXT(mm_mt, MM_MT_FLAGS, init_mm.mmap_lock),
	.pgd				= swapper_pg_dir,
	.mm_users			= ATOMIC_INIT(2),
```
固定time物理行29–32；定义范围29–60；事实键`init_mm_root_pointer`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A27｜swapper_root_alias
[VERIFIED mykernel/mm/fault/memfault_api.h::swapper_pg_dir]
```c
	#define swapper_pg_dir init_top_pgt
```
固定time物理行18；定义范围18–18；事实键`swapper_root_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A28｜init_root_array
[VERIFIED mykernel/mm/misc/init-mm.c::init_top_pgt]
```c
pgd_t	init_top_pgt[ENT_PER_TABLE]__aligned(PAGE_SIZE);
```
固定time物理行14；定义范围14–14；事实键`init_root_array`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A29｜directmap_va_offset
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_OFFSET]
```c
	#define PAGE_OFFSET				((ulong)__PAGE_OFFSET)
```
固定time物理行39；定义范围39–39；事实键`directmap_va_offset`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A30｜page_offset_base_constant
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::__PAGE_OFFSET_BASE_L4]
```c
	#define __PAGE_OFFSET_BASE_L4	_AC(0xffff800000000000, UL)
```
固定time物理行75；定义范围75–75；事实键`page_offset_base_constant`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A31｜physical_to_virtual_macro
[VERIFIED mykernel/arch/x86_64/mm/page/page_macro_arch.h::__va]
```c
	#  define __va(x)	((void *)(ulong)(x) + PAGE_OFFSET)
```
固定time物理行25；定义范围25–25；事实键`physical_to_virtual_macro`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A32｜virtual_to_physical_macro
[VERIFIED mykernel/arch/x86_64/mm/page/page_macro_arch.h::__pa]
```c
	#  define __pa(x)	__phys_addr((ulong)(x))
```
固定time物理行6；定义范围6–6；事实键`virtual_to_physical_macro`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A33｜physical_retranslation_arithmetic
[VERIFIED mykernel/arch/x86_64/mm/page/page_arch.h::__phys_addr_nodebug]
```c
		__phys_addr_nodebug(ulong x) {
			ulong y = x - __START_KERNEL_map;

			/* use the carry flag to determine if x was < __START_KERNEL_map */
			x = y + ((x > y) ? phys_base : (__START_KERNEL_map - PAGE_OFFSET));
```
固定time物理行40–44；定义范围38–47；事实键`physical_retranslation_arithmetic`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A34｜root_shift
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PGDIR_SHIFT]
```c
	#define PGDIR_SHIFT			P4D_SHIFT
```
固定time物理行270；定义范围270–270；事实键`root_shift`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A35｜root_size
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PGDIR_SIZE]
```c
	#define PGDIR_SIZE			(_AC(1, UL) << PGDIR_SHIFT)
```
固定time物理行280；定义范围280–280；事实键`root_size`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A36｜pud_shift
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PUD_SHIFT]
```c
	#define PUD_SHIFT			30
```
固定time物理行254；定义范围254–254；事实键`pud_shift`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A37｜pmd_shift
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PMD_SHIFT]
```c
	#define PMD_SHIFT			21
```
固定time物理行260；定义范围260–260；事实键`pmd_shift`。该引文只支持局部条件/表达式，不等同实际内核运行。

## A38｜one_page_alloc_wrapper
[VERIFIED mykernel/arch/x86_64/mm/page/page_arch.h::alloc_low_page]
```c
		PREFIX_STATIC_INLINE
		void
		*alloc_low_page(void) {
			return alloc_low_pages(1);
		}
```
固定time物理行89–93；定义范围89–93；事实键`one_page_alloc_wrapper`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B01｜pud_signature_initial_index
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pud_init]
```c

	ulong pages = 0, paddr_next;
	ulong paddr_last = paddr_end;
	ulong vaddr = (ulong)__va(paddr);
	int i = pud_index(vaddr);
```
固定time物理行747–751；定义范围744–815；事实键`pud_signature_initial_index`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B02｜pud_slot_va_and_next
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pud_init]
```c
		pgprot_t prot = _prot;

		vaddr = (ulong)__va(paddr);
		pud = pud_page + pud_index(vaddr);
		paddr_next = (paddr & PUD_MASK) + PUD_SIZE;
```
固定time物理行756–760；定义范围744–815；事实键`pud_slot_va_and_next`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B03｜pud_limit_nonleaf_decision
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pud_init]
```c
			continue;

		if (!pud_none(*pud)) {
			if (!pud_leaf(*pud)) {
				pmd = pmd_offset(pud, 0);
```
固定time物理行764–768；定义范围744–815；事实键`pud_limit_nonleaf_decision`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B04｜pud_existing_child_recursion
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pud_init]
```c
				pmd = pmd_offset(pud, 0);
				paddr_last =
					phys_pmd_init(pmd, paddr, paddr_end,
						page_size_mask, prot, init);
				continue;
```
固定time物理行768–772；定义范围744–815；事实键`pud_existing_child_recursion`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B05｜pud_large_reuse_or_old_prot
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pud_init]
```c
				paddr_last = paddr_next;
				continue;
			}
			prot = pte_pgprot(pte_clrhuge(*(pte_t *)pud));
		}
```
固定time物理行789–793；定义范围744–815；事实键`pud_large_reuse_or_old_prot`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B06｜pud_new_large_leaf_choice
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pud_init]
```c
		if (page_size_mask & (1<<PG_LEVEL_1G)) {
			pages++;
			spin_lock(&init_mm.page_table_lock);
			set_pud_init(pud,
				pfn_pud(paddr >> PAGE_SHIFT, prot_sethuge(prot)),
```
固定time物理行795–799；定义范围744–815；事实键`pud_new_large_leaf_choice`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B07｜pud_large_write_return
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pud_init]
```c
				pfn_pud(paddr >> PAGE_SHIFT, prot_sethuge(prot)),
					init);
			spin_unlock(&init_mm.page_table_lock);
			paddr_last = paddr_next;
			continue;
```
固定time物理行799–803；定义范围744–815；事实键`pud_large_write_return`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B08｜pud_alloc_child_and_recursion
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pud_init]
```c
		pmd = alloc_low_page();
		paddr_last = phys_pmd_init(pmd, paddr, paddr_end,
					   page_size_mask, prot, init);
```
固定time物理行806–808；定义范围744–815；事实键`pud_alloc_child_and_recursion`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B09｜pud_publish_and_count
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pud_init]
```c
		pud_populate_init(&init_mm, pud, pmd, init);
		spin_unlock(&init_mm.page_table_lock);
	}
	update_page_count(PG_LEVEL_1G, pages);
	return paddr_last;
```
固定time物理行811–815；定义范围744–815；事实键`pud_publish_and_count`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B10｜pmd_signature_initial_slot
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c

	ulong pages = 0, paddr_next;
	ulong paddr_last = paddr_end;

	int i = pmd_index(paddr);
```
固定time物理行670–674；定义范围667–735；事实键`pmd_signature_initial_slot`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B11｜pmd_iteration_next_guard
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c
		pgprot_t new_prot = prot;

		paddr_next = (paddr & PMD_MASK) + PMD_SIZE;
		if (paddr >= paddr_end)
			continue;
```
固定time物理行679–683；定义范围667–735；事实键`pmd_iteration_next_guard`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B12｜pmd_existing_nonleaf
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c
		if (!pmd_none(*pmd)) {
			if (!pmd_leaf(*pmd)) {
				spin_lock(&init_mm.page_table_lock);
				pte = (pte_t *)pmd_page_vaddr(*pmd);
				paddr_last = phys_pte_init(pte, paddr,
```
固定time物理行685–689；定义范围667–735；事实键`pmd_existing_nonleaf`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B13｜pmd_existing_child_call
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c
				paddr_last = phys_pte_init(pte, paddr,
							   paddr_end, prot,
							   init);
				spin_unlock(&init_mm.page_table_lock);
				continue;
```
固定time物理行689–693；定义范围667–735；事实键`pmd_existing_child_call`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B14｜pmd_large_reuse_vs_downgrade
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c
					pages++;
				paddr_last = paddr_next;
				continue;
			}
			new_prot = pte_pgprot(pte_clrhuge(*(pte_t *)pmd));
```
固定time物理行709–713；定义范围667–735；事实键`pmd_large_reuse_vs_downgrade`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B15｜pmd_new_large_choice
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c
		if (page_size_mask & (1<<PG_LEVEL_2M)) {
			pages++;
			spin_lock(&init_mm.page_table_lock);
			set_pmd_init(pmd,
				pfn_pmd(paddr >> PAGE_SHIFT, prot_sethuge(prot)),
```
固定time物理行716–720；定义范围667–735；事实键`pmd_new_large_choice`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B16｜pmd_new_large_write_return
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c
				pfn_pmd(paddr >> PAGE_SHIFT, prot_sethuge(prot)),
					init);
			spin_unlock(&init_mm.page_table_lock);
			paddr_last = paddr_next;
			continue;
```
固定time物理行720–724；定义范围667–735；事实键`pmd_new_large_write_return`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B17｜pmd_alloc_child_after_oldleaf
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c
		paddr_last = phys_pte_init(pte, paddr, paddr_end, new_prot, init);

		spin_lock(&init_mm.page_table_lock);
		pmd_populate_kernel_init(&init_mm, pmd, pte, init);
		spin_unlock(&init_mm.page_table_lock);
```
固定time物理行728–732；定义范围667–735；事实键`pmd_alloc_child_after_oldleaf`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B18｜pmd_count_return
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c
	}
	update_page_count(PG_LEVEL_2M, pages);
	return paddr_last;
```
固定time物理行733–735；定义范围667–735；事实键`pmd_count_return`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B19｜prot_add_pse_warning
[VERIFIED mykernel/arch/x86_64/mm/init.c::prot_sethuge]
```c
static inline pgprot_t
prot_sethuge(pgprot_t prot) {
	WARN_ON_ONCE(pgprot_val(prot) & _PAGE_PAT);
	return __pgprot(pgprot_val(prot) | _PAGE_PSE);
```
固定time物理行552–555；定义范围552–556；事实键`prot_add_pse_warning`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B20｜pud_none_alias
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_macro_arch.h::pud_none]
```c
	#define pud_none			pud_ent_is_none
```
固定time物理行53；定义范围53–53；事实键`pud_none_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B21｜pmd_none_alias
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_macro_arch.h::pmd_none]
```c
	#define pmd_none			pmd_ent_is_none
```
固定time物理行54；定义范围54–54；事实键`pmd_none_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B22｜pud_leaf_alias
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_macro_arch.h::pud_leaf]
```c
	#define pud_leaf			pud_ent_is_leaf
```
固定time物理行57；定义范围57–57；事实键`pud_leaf_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B23｜pmd_leaf_alias
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_macro_arch.h::pmd_leaf]
```c
	#define pmd_leaf			pmd_ent_is_leaf
```
固定time物理行58；定义范围58–58；事实键`pmd_leaf_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B24｜pud_none_ignores_erratum
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pud_ent_is_none]
```c
		int
		pud_ent_is_none(pud_t pud_ent) {
			return (arch_pude_val(pud_ent) &
						~(_PAGE_KNL_ERRATUM_MASK)) == 0;
		}
```
固定time物理行320–324；定义范围319–324；事实键`pud_none_ignores_erratum`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B25｜pmd_none_ignores_erratum
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pmd_ent_is_none]
```c
			 * Only check low word on 32-bit platforms,
			 * since it might be out of sync with upper half.
			 */
			return (arch_pmde_val(pmd_ent) &
						~_PAGE_KNL_ERRATUM_MASK) == 0;
```
固定time物理行364–368；定义范围361–369；事实键`pmd_none_ignores_erratum`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B26｜pud_leaf_tests_pse
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pud_ent_is_leaf]
```c
		PREFIX_STATIC_INLINE
		bool
		pud_ent_is_leaf(pud_t pud_ent) {
			return arch_pude_val(pud_ent) & _PAGE_PSE;
		}
```
固定time物理行325–329；定义范围325–329；事实键`pud_leaf_tests_pse`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B27｜pmd_leaf_tests_pse
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pmd_ent_is_leaf]
```c
		PREFIX_STATIC_INLINE
		bool
		pmd_ent_is_leaf(pmd_t pmd_ent) {
			return arch_pmde_val(pmd_ent) & _PAGE_PSE;
		}
```
固定time物理行370–374；定义范围370–374；事实键`pmd_leaf_tests_pse`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B28｜remove_pse_from_pte_cast
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pte_clrhuge]
```c
	static inline pte_t pte_clrhuge(pte_t pte)
	{
		return pte_clear_flags(pte, _PAGE_PSE);
	}
```
固定time物理行925–928；定义范围925–928；事实键`remove_pse_from_pte_cast`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B29｜clear_pte_protection_bits
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pte_clear_flags]
```c
		pte_t
		pte_clear_flags(pte_t pte, pteval_t clear) {
			pteval_t v = arch_pte_val(pte);
			return arch_make_pte(v & ~clear);
		}
```
固定time物理行230–234；定义范围229–234；事实键`clear_pte_protection_bits`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B30｜pte_to_pgprot_macro
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pte_pgprot]
```c
	#define pte_pgprot(x) __pgprot(arch_pte_flags(x))
```
固定time物理行1168；定义范围1168–1168；事实键`pte_to_pgprot_macro`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B31｜pud_leaf_frame_mask_and_flags
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pfn_pude]
```c
		pfn_pude(ulong page_nr, pgprot_t pgprot) {
			phys_addr_t pfn = (phys_addr_t)page_nr << PAGE_SHIFT;
			pfn ^= protnone_mask(pgprot_val(pgprot));
			pfn &= PHYSICAL_PUD_PAGE_MASK;
			return __pud(pfn | check_pgprot(pgprot));
```
固定time物理行537–541；定义范围535–542；事实键`pud_leaf_frame_mask_and_flags`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B32｜pmd_leaf_frame_mask_and_flags
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pfn_pmde]
```c
		pfn_pmde(ulong page_nr, pgprot_t pgprot) {
			phys_addr_t pfn = (phys_addr_t)page_nr << PAGE_SHIFT;
			pfn ^= protnone_mask(pgprot_val(pgprot));
			pfn &= PHYSICAL_PMD_PAGE_MASK;
			return __pmd(pfn | check_pgprot(pgprot));
```
固定time物理行591–595；定义范围589–596；事实键`pmd_leaf_frame_mask_and_flags`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B33｜large_leaf_bit_macro
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::_PAGE_PSE]
```c
	#define _PAGE_PSE					(_AT(pteval_t, 1) << _PAGE_BIT_PSE)
```
固定time物理行57；定义范围57–57；事实键`large_leaf_bit_macro`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B34｜physical_2m_frame_mask
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PHYSICAL_PMD_PAGE_MASK]
```c
	#define PHYSICAL_PMD_PAGE_MASK		(((signed long)PMD_MASK) & __PHYSICAL_MASK)
```
固定time物理行27；定义范围27–27；事实键`physical_2m_frame_mask`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B35｜physical_1g_frame_mask
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PHYSICAL_PUD_PAGE_MASK]
```c
	#define PHYSICAL_PUD_PAGE_MASK		(((signed long)PUD_MASK) & __PHYSICAL_MASK)
```
固定time物理行28；定义范围28–28；事实键`physical_1g_frame_mask`。该引文只支持局部条件/表达式，不等同实际内核运行。

## B36｜page_protection_supported_bits
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::check_pgprot]
```c
		PREFIX_STATIC_INLINE
		pgprotval_t
		check_pgprot(pgprot_t pgprot) {
			pgprotval_t massaged_val = massage_pgprot(pgprot);

```
固定time物理行695–699；定义范围695–711；事实键`page_protection_supported_bits`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C01｜pte_signature_and_last_default
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pte_init]
```c

	ulong pages = 0, paddr_next;
	ulong paddr_last = paddr_end;
	pte_t *pte;
	int i;
```
固定time物理行625–629；定义范围622–660；事实键`pte_signature_and_last_default`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C02｜pte_start_slot_and_next
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pte_init]
```c
	i = pte_index(paddr);

	for (; i < PTRS_PER_PTE; i++, paddr = paddr_next, pte++) {
		paddr_next = (paddr & PAGE_MASK) + PAGE_SIZE;
		if (paddr >= paddr_end)
```
固定time物理行632–636；定义范围622–660；事实键`pte_start_slot_and_next`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C03｜pte_existing_count_or_skip
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pte_init]
```c
		if (!pte_none(*pte)) {
			if (!after_bootmem)
				pages++;
			continue;
		}
```
固定time物理行645–649；定义范围622–660；事实键`pte_existing_count_or_skip`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C04｜pte_new_leaf_write
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pte_init]
```c
		if (0)
			pr_info("   pte=%p addr=%lx pte=%016lx\n", pte, paddr,
				pfn_pte(paddr >> PAGE_SHIFT, PAGE_KERNEL).val);
		pages++;
		set_pte_init(pte, pfn_pte(paddr >> PAGE_SHIFT, prot), init);
```
固定time物理行651–655；定义范围622–660；事实键`pte_new_leaf_write`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C05｜pte_new_last_count_return
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pte_init]
```c
		set_pte_init(pte, pfn_pte(paddr >> PAGE_SHIFT, prot), init);
		paddr_last = (paddr & PAGE_MASK) + PAGE_SIZE;
	}
	update_page_count(PG_LEVEL_4K, pages);
	return paddr_last;
```
固定time物理行655–659；定义范围622–660；事实键`pte_new_last_count_return`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C06｜per_level_count_array
[VERIFIED mykernel/arch/x86_64/mm/init.c::direct_pages_count]
```c
static ulong direct_pages_count[PG_LEVEL_NUM];
```
固定time物理行540；定义范围540–540；事实键`per_level_count_array`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C07｜counter_lock_and_add
[VERIFIED mykernel/arch/x86_64/mm/init.c::update_page_count]
```c
void update_page_count(int level, ulong pages) {
	/* Protect against CPA */
	spin_lock(&pgd_lock);
	direct_pages_count[level] += pages;
	spin_unlock(&pgd_lock);
```
固定time物理行541–545；定义范围541–546；事实键`counter_lock_and_add`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C08｜pmd_existing_leaf_count_early
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c
			if (page_size_mask & (1 << PG_LEVEL_2M)) {
				if (!after_bootmem)
					pages++;
				paddr_last = paddr_next;
```
固定time物理行707–710；定义范围667–735；事实键`pmd_existing_leaf_count_early`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C09｜pmd_new_leaf_counter
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c
		if (page_size_mask & (1<<PG_LEVEL_2M)) {
			pages++;
			spin_lock(&init_mm.page_table_lock);
```
固定time物理行716–718；定义范围667–735；事实键`pmd_new_leaf_counter`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C10｜pmd_counter_call
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c
	}
	update_page_count(PG_LEVEL_2M, pages);
	return paddr_last;
```
固定time物理行733–735；定义范围667–735；事实键`pmd_counter_call`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C11｜pud_existing_leaf_count_early
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pud_init]
```c
			if (page_size_mask & (1 << PG_LEVEL_1G)) {
				if (!after_bootmem)
					pages++;
				paddr_last = paddr_next;
```
固定time物理行786–789；定义范围744–815；事实键`pud_existing_leaf_count_early`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C12｜pud_new_leaf_counter
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pud_init]
```c
		if (page_size_mask & (1<<PG_LEVEL_1G)) {
			pages++;
			spin_lock(&init_mm.page_table_lock);
```
固定time物理行795–797；定义范围744–815；事实键`pud_new_leaf_counter`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C13｜pud_counter_call
[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pud_init]
```c
	}
	update_page_count(PG_LEVEL_1G, pages);
	return paddr_last;
```
固定time物理行813–815；定义范围744–815；事实键`pud_counter_call`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C14｜pte_init_bool_wrapper
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pte_init]
```c
			if (init)
				set_pte_safe(pte_ptr, pte_ent);
			else
				set_pte(pte_ptr, pte_ent);
		}
```
固定time物理行740–744；定义范围737–744；事实键`pte_init_bool_wrapper`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C15｜pmd_init_bool_wrapper
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pmd_init]
```c
		set_pmd_init(pmd_t *pmd_ptr, pmd_t pmd_ent, bool init) {
			if (init)
				set_pmd_safe(pmd_ptr, pmd_ent);
			else
				set_pmd(pmd_ptr, pmd_ent);
```
固定time物理行731–735；定义范围729–735；事实键`pmd_init_bool_wrapper`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C16｜pud_init_bool_wrapper
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pud_init]
```c
		set_pud_init(pud_t *pude_ptr, pud_t pud_ent, bool init) {
			if (init)
				set_pud_safe(pude_ptr, pud_ent);
			else
				set_pud(pude_ptr, pud_ent);
```
固定time物理行723–727；定义范围721–728；事实键`pud_init_bool_wrapper`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C17｜p4d_populate_bool_wrapper
[VERIFIED mykernel/mm/fault/pgtable.h::p4d_populate_init]
```c
			if (init)
				p4d_populate_safe(mm, p4d_entp, pud_p);
			else
				p4d_populate(mm, p4d_entp, pud_p);
		}
```
固定time物理行494–498；定义范围491–498；事实键`p4d_populate_bool_wrapper`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C18｜pud_populate_bool_wrapper
[VERIFIED mykernel/mm/fault/pgtable.h::pud_populate_init]
```c
			if (init)
				pud_populate_safe(mm, pud_entp, pmd_p);
			else
				pud_populate(mm, pud_entp, pmd_p);
		}
```
固定time物理行502–506；定义范围499–506；事实键`pud_populate_bool_wrapper`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C19｜pmd_populate_kernel_bool_wrapper
[VERIFIED mykernel/mm/fault/pgtable.h::pmd_populate_kernel_init]
```c
			if (init)
				pmd_populate_kernel_safe(mm, pmd_entp, pte_p);
			else
				pmd_populate_kernel(mm, pmd_entp, pte_p);
		}
```
固定time物理行510–514；定义范围507–515；事实键`pmd_populate_kernel_bool_wrapper`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C20｜pte_warn_then_write
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pte_safe]
```c
		set_pte_safe(pte_t *pte_ptr, pte_t pte) {
			WARN_ON_ONCE(pte_present(*pte_ptr) &&
					!pte_same(*pte_ptr, pte));\
			set_pte(pte_ptr, pte);
		}
```
固定time物理行611–615；定义范围609–615；事实键`pte_warn_then_write`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C21｜pmd_warn_then_write
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pmd_ent_safe]
```c
		void
		set_pmd_ent_safe(pmd_t *pmde_ptr, pmd_t pmd_ent) {
			WARN_ON_ONCE(pmd_present(*pmde_ptr) &&
					!pmd_same(*pmde_ptr, pmd_ent));
			set_pmd(pmde_ptr, pmd_ent);
```
固定time物理行556–560；定义范围555–561；事实键`pmd_warn_then_write`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C22｜pud_warn_then_write
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pud_ent_safe]
```c
		void
		set_pud_ent_safe(pud_t *pude_ptr, pud_t pud_ent) {
			WARN_ON_ONCE(pud_present(*pude_ptr) &&
					!pud_same(*pude_ptr, pud_ent));
			set_pud(pude_ptr, pud_ent);
```
固定time物理行502–506；定义范围501–507；事实键`pud_warn_then_write`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C23｜p4d_warn_then_write
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_p4d_ent_safe]
```c
		void
		set_p4d_ent_safe(p4d_t *p4de_ptr, p4d_t p4d_ent) {
			WARN_ON_ONCE(p4d_present(*p4de_ptr) &&
					!p4d_same(*p4de_ptr, p4d_ent));
			set_p4d(p4de_ptr, p4d_ent);
```
固定time物理行447–451；定义范围446–452；事实键`p4d_warn_then_write`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C24｜pte_write_once
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pte]
```c
		void
		set_pte(pte_t *pte_ptr, pte_t pte) {
			WRITE_ONCE(*(pte_ptr), (pte));
		}
```
固定time物理行605–608；定义范围604–608；事实键`pte_write_once`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C25｜pmd_write_once
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pmd_ent]
```c
		PREFIX_STATIC_INLINE
		void
		set_pmd_ent(pmd_t *pmde_ptr, pmd_t pmd_ent) {
			WRITE_ONCE(*(pmde_ptr), (pmd_ent));
		}
```
固定time物理行550–554；定义范围550–555；事实键`pmd_write_once`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C26｜pud_write_once
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_pud_ent]
```c
		PREFIX_STATIC_INLINE
		void
		set_pud_ent(pud_t *pude_ptr, pud_t pud_ent) {
			WRITE_ONCE(*(pude_ptr), (pud_ent));
		}
```
固定time物理行496–500；定义范围496–501；事实键`pud_write_once`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C27｜p4d_write_once
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::set_p4d_ent]
```c
		PREFIX_STATIC_INLINE
		void
		set_p4d_ent(p4d_t *p4de_ptr, p4d_t p4d_ent) {
			WRITE_ONCE(*(p4de_ptr), (p4d_ent));
		}
```
固定time物理行441–445；定义范围441–446；事实键`p4d_write_once`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C28｜pte_pfn_frame_mask_and_flags
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pfn_pte]
```c
		pfn_pte(ulong page_nr, pgprot_t pgprot) {
			phys_addr_t pfn = (phys_addr_t)page_nr << PAGE_SHIFT;
			pfn ^= protnone_mask(pgprot_val(pgprot));
			pfn &= PTE_PFN_MASK;
			return __pte(pfn | check_pgprot(pgprot));
```
固定time物理行650–654；定义范围648–655；事实键`pte_pfn_frame_mask_and_flags`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C29｜p4d_safe_parent_writer
[VERIFIED mykernel/mm/fault/pgtable.h::fill_pud_to_p4d_ent_safe]
```c
		void
		fill_pud_to_p4d_ent_safe(mm_s *mm, p4d_t *p4d_entp, pud_t *pud_p) {
			// paravirt_alloc_pud(mm, __pa(pud_p) >> PAGE_SHIFT);
			set_p4d_ent_safe(p4d_entp, __p4d(__pa(pud_p) | _PAGE_TABLE));
		}
```
固定time物理行209–213；定义范围208–213；事实键`p4d_safe_parent_writer`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C30｜pud_safe_parent_writer
[VERIFIED mykernel/mm/fault/pgtable.h::fill_pmd_to_pud_ent_safe]
```c
		void
		fill_pmd_to_pud_ent_safe(mm_s *mm, pud_t *pud_entp, pmd_t *pmd_p) {
			// paravirt_alloc_pmd(mm, __pa(pmd_p) >> PAGE_SHIFT);
			set_pud_ent_safe(pud_entp, __pud(__pa(pmd_p) | _PAGE_TABLE));
		}
```
固定time物理行250–254；定义范围249–255；事实键`pud_safe_parent_writer`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C31｜pmd_safe_parent_writer
[VERIFIED mykernel/mm/fault/pgtable.h::fill_pgtbl_to_pmd_ent_kernel_safe]
```c
		void
		fill_pgtbl_to_pmd_ent_kernel_safe(mm_s *mm, pmd_t *pmd_entp, pte_t *pgtbl_p) {
			// paravirt_alloc_pte(mm, __pa(pgtbl_p) >> PAGE_SHIFT);
			set_pmd_safe(pmd_entp, __pmd(__pa(pgtbl_p) | _PAGE_TABLE));
		}
```
固定time物理行292–296；定义范围291–296；事实键`pmd_safe_parent_writer`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C32｜p4d_populate_safe_alias
[VERIFIED mykernel/mm/fault/pgtable_macro.h::p4d_populate_safe]
```c
	#define p4d_populate_safe	fill_pud_to_p4d_ent_safe
```
固定time物理行90；定义范围90–90；事实键`p4d_populate_safe_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C33｜pud_populate_safe_alias
[VERIFIED mykernel/mm/fault/pgtable_macro.h::pud_populate_safe]
```c
	#define pud_populate_safe	fill_pmd_to_pud_ent_safe
```
固定time物理行91；定义范围91–91；事实键`pud_populate_safe_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C34｜pmd_populate_safe_alias
[VERIFIED mykernel/mm/fault/pgtable_macro.h::pmd_populate_kernel_safe]
```c
	#define pmd_populate_kernel_safe	fill_pgtbl_to_pmd_ent_kernel_safe
```
固定time物理行92；定义范围92–92；事实键`pmd_populate_safe_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C35｜p4d_populate_normal_alias
[VERIFIED mykernel/mm/fault/pgtable_macro.h::p4d_populate]
```c
	#define p4d_populate	fill_pud_to_p4d_ent
```
固定time物理行85；定义范围85–85；事实键`p4d_populate_normal_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C36｜pud_populate_normal_alias
[VERIFIED mykernel/mm/fault/pgtable_macro.h::pud_populate]
```c
	#define pud_populate	fill_pmd_to_pud_ent
```
固定time物理行86；定义范围86–86；事实键`pud_populate_normal_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C37｜pmd_populate_normal_alias
[VERIFIED mykernel/mm/fault/pgtable_macro.h::pmd_populate_kernel]
```c
	#define pmd_populate_kernel	fill_pgtbl_to_pmd_ent_kernel
```
固定time物理行88；定义范围88–88；事实键`pmd_populate_normal_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C38｜counter_level_4k_enum
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PG_LEVEL_4K]
```c
			PG_LEVEL_4K,
```
固定time物理行9；定义范围7–14；事实键`counter_level_4k_enum`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C39｜counter_level_2m_enum
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PG_LEVEL_2M]
```c
			PG_LEVEL_2M,
```
固定time物理行10；定义范围7–14；事实键`counter_level_2m_enum`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C40｜counter_level_1g_enum
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PG_LEVEL_1G]
```c
			PG_LEVEL_1G,
```
固定time物理行11；定义范围7–14；事实键`counter_level_1g_enum`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C41｜pte_table_512_slots
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PTRS_PER_PTE]
```c
	#define PTRS_PER_PTE		512
```
固定time物理行266；定义范围266–266；事实键`pte_table_512_slots`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C42｜pte_page_4096_bytes
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```
固定time物理行10；定义范围10–10；事实键`pte_page_4096_bytes`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C43｜pte_index_alias
[VERIFIED mykernel/mm/fault/pgtable_macro.h::pte_index]
```c
	#define pte_index		ent_index_in_pgtbl
```
固定time物理行74；定义范围74–74；事实键`pte_index_alias`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C44｜pte_index_formula
[VERIFIED mykernel/mm/fault/pgtable.h::ent_index_in_pgtbl]
```c
		ulong
		ent_index_in_pgtbl(ulong vaddr) {
			return (vaddr >> PAGE_SHIFT) & (PTRS_PER_PTE - 1);
		}
```
固定time物理行316–319；定义范围315–319；事实键`pte_index_formula`。该引文只支持局部条件/表达式，不等同实际内核运行。

## C45｜upper_return_consumption
[VERIFIED mykernel/arch/x86_64/mm/init.c::init_memory_mapping]
```c
				mr[i].end, mr[i].page_size_mask, prot);

	// add_pfn_range_mapped(start >> PAGE_SHIFT, ret >> PAGE_SHIFT);

	return ret >> PAGE_SHIFT;
```
固定time物理行447–451；定义范围433–452；事实键`upper_return_consumption`。该引文只支持局部条件/表达式，不等同实际内核运行。

## 取证边界

81的直接映射buffer与页分配器、82的上层页大小候选和mapped请求接口、旧79/80内存输入，均按C06回执限定消费。当前仅本链的页表存储、写包装及静态返回/计数，不执行宏预处理、编译、链接、Graphviz、真实硬件/内核/QEMU或外部规范复核。
