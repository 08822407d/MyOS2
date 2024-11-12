#include <linux/kernel/fcntl.h>
#include <linux/kernel/syscalls.h>
#include <linux/fs/file.h>
#include <linux/kernel/mm_api.h>
#include <linux/kernel/sched_api.h>
#include <linux/kernel/sched_types.h>
#include <linux/fs/namei.h>
#include <linux/lib/errno.h>
#include <linux/lib/string.h>
#include <uapi/linux/myos_sysreboot.h>
#include <asm/syscall.h>
#include <asm/setup.h>
#include <asm/insns.h>
#include <asm/signal.h>
#include <asm/proto.h>
#include <asm/delay.h>

#include <obsolete/printk.h>
#include <obsolete/arch_proto.h>




MYOS_SYSCALL_DEFINE0(no_syscall)
{
	color_printk(RED, BLACK, "no_system_call is calling\n");
	return -ENOSYS;
}

MYOS_SYSCALL_DEFINE1(myos_putstring, char *, string)
{
	color_printk(WHITE, BLACK, string);
	return 0;
}

/*======================================================================================*
 *									user memory manage									*
 *======================================================================================*/
MYOS_SYSCALL_DEFINE1(brk, unsigned long, brk)
{
	ulong newbrk, oldbrk, origbrk;
	mm_s *mm = current->mm;
	vma_s *brkvma, *next = NULL;
	ulong min_brk;
	bool downgraded = false;

	origbrk = mm->brk;
	min_brk = mm->start_brk;
	if (brk < min_brk)
		goto out;

	// /*
	//  * Check against rlimit here. If this check is done later after the test
	//  * of oldbrk with newbrk then it can escape the test and let the data
	//  * segment grow beyond its set limit the in case where the limit is
	//  * not page aligned -Ram Gupta
	//  */
	// if (check_data_rlimit(rlimit(RLIMIT_DATA), brk,
	// 		mm->start_brk, mm->end_data, mm->start_data))
	// 	goto out;

	newbrk = PAGE_ALIGN(brk);
	oldbrk = PAGE_ALIGN(mm->brk);
	if (oldbrk == newbrk) {
		mm->brk = brk;
		goto success;
	}

	/* Always allow shrinking brk. */
	if (brk <= mm->brk) {
		// /* Search one past newbrk */
		// vma_iter_init(&vmi, mm, newbrk);
		// brkvma = vma_find(&vmi, oldbrk);
		// if (!brkvma || brkvma->vm_start >= oldbrk)
		// 	goto out; /* mapping intersects with an existing non-brk vma. */

		/*
		 * mm->brk must be protected by write mmap_lock.
		 * do_vma_munmap() will drop the lock on success,  so update it
		 * before calling do_vma_munmap().
		 */
		mm->brk = brk;
		if (simple_do_vma_munmap(mm, newbrk, oldbrk))
			goto out;

		goto success_unlocked;
	}

	// if (check_brk_limits(oldbrk, newbrk - oldbrk))
	// 	goto out;

	// /*
	//  * Only check if the next VMA is within the stack_guard_gap of the
	//  * expansion area
	//  */
	// vma_iter_init(&vmi, mm, oldbrk);
	// next = vma_find(&vmi, newbrk + PAGE_SIZE + stack_guard_gap);

	// if (next && newbrk + PAGE_SIZE > vm_start_gap(next))
	// 	goto out;

	/* Ok, looks good - let it rip. */
	if (do_brk_flags(oldbrk, newbrk-oldbrk, 0) < 0)
		goto out;

	mm->brk = brk;
	// if (mm->def_flags & VM_LOCKED)
	// 	populate = true;

	// next = simple_find_vma(mm, oldbrk);
	// while (next == NULL);
	
success:
	// mmap_write_unlock(mm);
success_unlocked:
	// userfaultfd_unmap_complete(mm, &uf);
	// if (populate)
	// 	mm_populate(oldbrk, newbrk - oldbrk);
	return brk;

out:
	mm->brk = origbrk;
	// mmap_write_unlock(mm);
	return origbrk;
}


/*======================================================================================*
 *									get task infomation									*
 *======================================================================================*/
MYOS_SYSCALL_DEFINE3(ioctl, unsigned int, fd, unsigned int, cmd, unsigned long, arg)
{
	ALERT_DUMMY_SYSCALL(ioctl, IF_ALERT_DUMMY_SYSCALL);

// 	struct fd f = fdget(fd);
// 	int error;

// 	if (!f.file)
// 		return -EBADF;

// 	error = security_file_ioctl(f.file, cmd, arg);
// 	if (error)
// 		goto out;

// 	error = do_vfs_ioctl(f.file, fd, cmd, arg);
// 	if (error == -ENOIOCTLCMD)
// 		error = vfs_ioctl(f.file, cmd, arg);

// out:
// 	fdput(f);
// 	return error;

	return 0;
}


/*======================================================================================*
 *									special functions									*
 *======================================================================================*/
MYOS_SYSCALL_DEFINE2(reboot, unsigned int, cmd, void *, arg)
{
	color_printk(GREEN,BLACK,"sys_reboot\n");
	switch(cmd)
	{
		case SYSTEM_REBOOT:
			outb(0xFE, 0x64);
			break;

		case SYSTEM_POWEROFF:
			color_printk(RED,BLACK, "sys_reboot cmd SYSTEM_POWEROFF\n");
			break;

		default:
			color_printk(RED,BLACK, "sys_reboot cmd ERROR!\n");
			break;
	}
	return 0;
}



/*======================================================================================*
 *									temp dummy syscall									*
 *======================================================================================*/
MYOS_SYSCALL_DEFINE3(mprotect, ulong, start, size_t, len, ulong, prot)
{
	ALERT_DUMMY_SYSCALL(mprotect, IF_ALERT_DUMMY_SYSCALL);

	return 0;
}

MYOS_SYSCALL_DEFINE2(arch_prctl, int, option, unsigned long, arg2)
{
	long ret;

	ret = do_arch_prctl_64(current, option, arg2);
	// if (ret == -EINVAL)
	// 	ret = do_arch_prctl_common(option, arg2);

	return ret;
}

MYOS_SYSCALL_DEFINE1(set_tid_address, int *, tidptr)
{
	ALERT_DUMMY_SYSCALL(set_tid_address, IF_ALERT_DUMMY_SYSCALL);

	// current->clear_child_tid = tidptr;

	// return task_pid_vnr(current);
	return 0;
}

// MYOS_SYSCALL_DEFINE2(set_robust_list, void *, head, size_t, len)
// {
// 	pr_alert("\t!!! Dummy Syscall --- set_robust_list ---\n");

// 	return 0;
// }

// MYOS_SYSCALL_DEFINE4(rseq, void *, rseq, u32, rseq_len, int, flags, u32, sig)
// {
// 	pr_alert("\t!!! Dummy Syscall --- set_robust_list ---\n");

// 	return 0;
// }

