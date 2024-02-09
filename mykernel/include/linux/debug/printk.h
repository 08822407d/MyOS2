/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __KERNEL_PRINTK__
#define __KERNEL_PRINTK__

	#include <linux/lib/stdarg.h>
	#include <linux/init/init.h>
	#include <linux/debug/kern_levels.h>
	#include <linux/kernel/linkage.h>
	// #include <linux/ratelimit_types.h>
	// #include <linux/once_lite.h>


	/*
	 * content of <linux/dev_printk.h>
	 *===================================================================================
	 */
	#include <linux/kernel/compiler.h>
	#include <linux/kernel/types.h>
	// #include <linux/ratelimit.h>

	#ifndef dev_fmt
	#define dev_fmt(fmt) fmt
	#endif

	struct device;
	typedef struct device device_s;

	#define PRINTK_INFO_SUBSYSTEM_LEN	16
	#define PRINTK_INFO_DEVICE_LEN		48

	typedef struct dev_printk_info {
		char subsystem[PRINTK_INFO_SUBSYSTEM_LEN];
		char device[PRINTK_INFO_DEVICE_LEN];
	} dev_printk_info_s;

	// #ifdef CONFIG_PRINTK

	// __printf(3, 0) __cold
	// int dev_vprintk_emit(int level, const struct device *dev,
	// 			 const char *fmt, va_list args);
	// __printf(3, 4) __cold
	// int dev_printk_emit(int level, const struct device *dev, const char *fmt, ...);

	// __printf(3, 4) __cold
	// void _dev_printk(const char *level, const struct device *dev,
	// 		 const char *fmt, ...);
	// __printf(2, 3) __cold
	// void _dev_emerg(const struct device *dev, const char *fmt, ...);
	// __printf(2, 3) __cold
	// void _dev_alert(const struct device *dev, const char *fmt, ...);
	// __printf(2, 3) __cold
	// void _dev_crit(const struct device *dev, const char *fmt, ...);
	// __printf(2, 3) __cold
	// void _dev_err(const struct device *dev, const char *fmt, ...);
	// __printf(2, 3) __cold
	// void _dev_warn(const struct device *dev, const char *fmt, ...);
	// __printf(2, 3) __cold
	// void _dev_notice(const struct device *dev, const char *fmt, ...);
	// __printf(2, 3) __cold
	// void _dev_info(const struct device *dev, const char *fmt, ...);

	// #else

	// static inline __printf(3, 0)
	// int dev_vprintk_emit(int level, const struct device *dev,
	// 			 const char *fmt, va_list args)
	// { return 0; }
	// static inline __printf(3, 4)
	// int dev_printk_emit(int level, const struct device *dev, const char *fmt, ...)
	// { return 0; }

	// static inline void __dev_printk(const char *level, const struct device *dev,
	// 				struct va_format *vaf)
	// {}
	// static inline __printf(3, 4)
	// void _dev_printk(const char *level, const struct device *dev,
	// 		 const char *fmt, ...)
	// {}

	// static inline __printf(2, 3)
	// void _dev_emerg(const struct device *dev, const char *fmt, ...)
	// {}
	// static inline __printf(2, 3)
	// void _dev_crit(const struct device *dev, const char *fmt, ...)
	// {}
	// static inline __printf(2, 3)
	// void _dev_alert(const struct device *dev, const char *fmt, ...)
	// {}
	// static inline __printf(2, 3)
	// void _dev_err(const struct device *dev, const char *fmt, ...)
	// {}
	// static inline __printf(2, 3)
	// void _dev_warn(const struct device *dev, const char *fmt, ...)
	// {}
	// static inline __printf(2, 3)
	// void _dev_notice(const struct device *dev, const char *fmt, ...)
	// {}
	// static inline __printf(2, 3)
	// void _dev_info(const struct device *dev, const char *fmt, ...)
	// {}

	// #endif

	// /*
	//  * Need to take variadic arguments even though we don't use them, as dev_fmt()
	//  * may only just have been expanded and may result in multiple arguments.
	//  */
	// #define dev_printk_index_emit(level, fmt, ...) \
	// 	printk_index_subsys_emit("%s %s: ", level, fmt)

	// #define dev_printk_index_wrap(_p_func, level, dev, fmt, ...)		\
	// 	({								\
	// 		dev_printk_index_emit(level, fmt);			\
	// 		_p_func(dev, fmt, ##__VA_ARGS__);			\
	// 	})

	// /*
	//  * Some callsites directly call dev_printk rather than going through the
	//  * dev_<level> infrastructure, so we need to emit here as well as inside those
	//  * level-specific macros. Only one index entry will be produced, either way,
	//  * since dev_printk's `fmt` isn't known at compile time if going through the
	//  * dev_<level> macros.
	//  *
	//  * dev_fmt() isn't called for dev_printk when used directly, as it's used by
	//  * the dev_<level> macros internally which already have dev_fmt() processed.
	//  *
	//  * We also can't use dev_printk_index_wrap directly, because we have a separate
	//  * level to process.
	//  */
	// #define dev_printk(level, dev, fmt, ...)				\
	// 	({								\
	// 		dev_printk_index_emit(level, fmt);			\
	// 		_dev_printk(level, dev, fmt, ##__VA_ARGS__);		\
	// 	})

	// /*
	//  * #defines for all the dev_<level> macros to prefix with whatever
	//  * possible use of #define dev_fmt(fmt) ...
	//  */

	// #define dev_emerg(dev, fmt, ...) \
	// 	dev_printk_index_wrap(_dev_emerg, KERN_EMERG, dev, dev_fmt(fmt), ##__VA_ARGS__)
	// #define dev_crit(dev, fmt, ...) \
	// 	dev_printk_index_wrap(_dev_crit, KERN_CRIT, dev, dev_fmt(fmt), ##__VA_ARGS__)
	// #define dev_alert(dev, fmt, ...) \
	// 	dev_printk_index_wrap(_dev_alert, KERN_ALERT, dev, dev_fmt(fmt), ##__VA_ARGS__)
	// #define dev_err(dev, fmt, ...) \
	// 	dev_printk_index_wrap(_dev_err, KERN_ERR, dev, dev_fmt(fmt), ##__VA_ARGS__)
	// #define dev_warn(dev, fmt, ...) \
	// 	dev_printk_index_wrap(_dev_warn, KERN_WARNING, dev, dev_fmt(fmt), ##__VA_ARGS__)
	// #define dev_notice(dev, fmt, ...) \
	// 	dev_printk_index_wrap(_dev_notice, KERN_NOTICE, dev, dev_fmt(fmt), ##__VA_ARGS__)
	// #define dev_info(dev, fmt, ...) \
	// 	dev_printk_index_wrap(_dev_info, KERN_INFO, dev, dev_fmt(fmt), ##__VA_ARGS__)

	// #if defined(CONFIG_DYNAMIC_DEBUG) || \
	// 	(defined(CONFIG_DYNAMIC_DEBUG_CORE) && defined(DYNAMIC_DEBUG_MODULE))
	// #define dev_dbg(dev, fmt, ...)						\
	// 	dynamic_dev_dbg(dev, dev_fmt(fmt), ##__VA_ARGS__)
	// #elif defined(DEBUG)
	// #define dev_dbg(dev, fmt, ...)						\
	// 	dev_printk(KERN_DEBUG, dev, dev_fmt(fmt), ##__VA_ARGS__)
	// #else
	// #define dev_dbg(dev, fmt, ...)						\
	// ({									\
	// 	if (0)								\
	// 		dev_printk(KERN_DEBUG, dev, dev_fmt(fmt), ##__VA_ARGS__); \
	// })
	// #endif

	// #ifdef CONFIG_PRINTK
	// #define dev_level_once(dev_level, dev, fmt, ...)			\
	// do {									\
	// 	static bool __print_once __read_mostly;				\
	// 									\
	// 	if (!__print_once) {						\
	// 		__print_once = true;					\
	// 		dev_level(dev, fmt, ##__VA_ARGS__);			\
	// 	}								\
	// } while (0)
	// #else
	// #define dev_level_once(dev_level, dev, fmt, ...)			\
	// do {									\
	// 	if (0)								\
	// 		dev_level(dev, fmt, ##__VA_ARGS__);			\
	// } while (0)
	// #endif

	// #define dev_emerg_once(dev, fmt, ...)					\
	// 	dev_level_once(dev_emerg, dev, fmt, ##__VA_ARGS__)
	// #define dev_alert_once(dev, fmt, ...)					\
	// 	dev_level_once(dev_alert, dev, fmt, ##__VA_ARGS__)
	// #define dev_crit_once(dev, fmt, ...)					\
	// 	dev_level_once(dev_crit, dev, fmt, ##__VA_ARGS__)
	// #define dev_err_once(dev, fmt, ...)					\
	// 	dev_level_once(dev_err, dev, fmt, ##__VA_ARGS__)
	// #define dev_warn_once(dev, fmt, ...)					\
	// 	dev_level_once(dev_warn, dev, fmt, ##__VA_ARGS__)
	// #define dev_notice_once(dev, fmt, ...)					\
	// 	dev_level_once(dev_notice, dev, fmt, ##__VA_ARGS__)
	// #define dev_info_once(dev, fmt, ...)					\
	// 	dev_level_once(dev_info, dev, fmt, ##__VA_ARGS__)
	// #define dev_dbg_once(dev, fmt, ...)					\
	// 	dev_level_once(dev_dbg, dev, fmt, ##__VA_ARGS__)

	// #define dev_level_ratelimited(dev_level, dev, fmt, ...)			\
	// do {									\
	// 	static DEFINE_RATELIMIT_STATE(_rs,				\
	// 					  DEFAULT_RATELIMIT_INTERVAL,	\
	// 					  DEFAULT_RATELIMIT_BURST);		\
	// 	if (__ratelimit(&_rs))						\
	// 		dev_level(dev, fmt, ##__VA_ARGS__);			\
	// } while (0)

	// #define dev_emerg_ratelimited(dev, fmt, ...)				\
	// 	dev_level_ratelimited(dev_emerg, dev, fmt, ##__VA_ARGS__)
	// #define dev_alert_ratelimited(dev, fmt, ...)				\
	// 	dev_level_ratelimited(dev_alert, dev, fmt, ##__VA_ARGS__)
	// #define dev_crit_ratelimited(dev, fmt, ...)				\
	// 	dev_level_ratelimited(dev_crit, dev, fmt, ##__VA_ARGS__)
	// #define dev_err_ratelimited(dev, fmt, ...)				\
	// 	dev_level_ratelimited(dev_err, dev, fmt, ##__VA_ARGS__)
	// #define dev_warn_ratelimited(dev, fmt, ...)				\
	// 	dev_level_ratelimited(dev_warn, dev, fmt, ##__VA_ARGS__)
	// #define dev_notice_ratelimited(dev, fmt, ...)				\
	// 	dev_level_ratelimited(dev_notice, dev, fmt, ##__VA_ARGS__)
	// #define dev_info_ratelimited(dev, fmt, ...)				\
	// 	dev_level_ratelimited(dev_info, dev, fmt, ##__VA_ARGS__)
	// #if defined(CONFIG_DYNAMIC_DEBUG) || \
	// 	(defined(CONFIG_DYNAMIC_DEBUG_CORE) && defined(DYNAMIC_DEBUG_MODULE))
	// /* descriptor check is first to prevent flooding with "callbacks suppressed" */
	// #define dev_dbg_ratelimited(dev, fmt, ...)				\
	// do {									\
	// 	static DEFINE_RATELIMIT_STATE(_rs,				\
	// 					  DEFAULT_RATELIMIT_INTERVAL,	\
	// 					  DEFAULT_RATELIMIT_BURST);		\
	// 	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, fmt);			\
	// 	if (DYNAMIC_DEBUG_BRANCH(descriptor) &&				\
	// 		__ratelimit(&_rs))						\
	// 		__dynamic_dev_dbg(&descriptor, dev, dev_fmt(fmt),	\
	// 				  ##__VA_ARGS__);			\
	// } while (0)
	// #elif defined(DEBUG)
	// #define dev_dbg_ratelimited(dev, fmt, ...)				\
	// do {									\
	// 	static DEFINE_RATELIMIT_STATE(_rs,				\
	// 					  DEFAULT_RATELIMIT_INTERVAL,	\
	// 					  DEFAULT_RATELIMIT_BURST);		\
	// 	if (__ratelimit(&_rs))						\
	// 		dev_printk(KERN_DEBUG, dev, dev_fmt(fmt), ##__VA_ARGS__); \
	// } while (0)
	// #else
	// #define dev_dbg_ratelimited(dev, fmt, ...)				\
	// do {									\
	// 	if (0)								\
	// 		dev_printk(KERN_DEBUG, dev, dev_fmt(fmt), ##__VA_ARGS__); \
	// } while (0)
	// #endif

	// #ifdef VERBOSE_DEBUG
	// #define dev_vdbg	dev_dbg
	// #else
	// #define dev_vdbg(dev, fmt, ...)						\
	// ({									\
	// 	if (0)								\
	// 		dev_printk(KERN_DEBUG, dev, dev_fmt(fmt), ##__VA_ARGS__); \
	// })
	// #endif

	// /*
	//  * dev_WARN*() acts like dev_printk(), but with the key difference of
	//  * using WARN/WARN_ONCE to include file/line information and a backtrace.
	//  */
	// #define dev_WARN(dev, format, arg...) \
	// 	WARN(1, "%s %s: " format, dev_driver_string(dev), dev_name(dev), ## arg)

	// #define dev_WARN_ONCE(dev, condition, format, arg...) \
	// 	WARN_ONCE(condition, "%s %s: " format, \
	// 			dev_driver_string(dev), dev_name(dev), ## arg)

	// /*
	//  *===================================================================================
	//  */


	extern const char linux_banner[];
	extern const char linux_proc_banner[];

	/* If set, an oops, panic(), BUG() or die() is in progress */
	extern int oops_in_progress;

	#define PRINTK_MAX_SINGLE_HEADER_LEN	2

	static inline int
	printk_get_level(const char *buffer) {
		if (buffer[0] == KERN_SOH_ASCII && buffer[1]) {
			switch (buffer[1]) {
			case '0' ... '7':
			case 'c':	/* KERN_CONT */
				return buffer[1];
			}
		}
		return 0;
	}

	static inline const char
	*printk_skip_level(const char *buffer) {
		if (printk_get_level(buffer))
			return buffer + 2;

		return buffer;
	}

	static inline const char
	*printk_skip_headers(const char *buffer) {
		while (printk_get_level(buffer))
			buffer = printk_skip_level(buffer);

		return buffer;
	}

	/* printk's without a loglevel use this.. */
	#define MESSAGE_LOGLEVEL_DEFAULT	CONFIG_MESSAGE_LOGLEVEL_DEFAULT

	/* We show everything that is MORE important than this.. */
	#define CONSOLE_LOGLEVEL_SILENT			0	/* Mum's the word */
	#define CONSOLE_LOGLEVEL_MIN			1	/* Minimum loglevel we let people use */
	#define CONSOLE_LOGLEVEL_DEBUG			10	/* issue debug messages */
	#define CONSOLE_LOGLEVEL_MOTORMOUTH		15	/* You can't shut this one up */

	/*
	 * Default used to be hard-coded at 7, quiet used to be hardcoded at 4,
	 * we're now allowing both to be set from kernel config.
	 */
	#define CONSOLE_LOGLEVEL_DEFAULT	CONFIG_CONSOLE_LOGLEVEL_DEFAULT
	#define CONSOLE_LOGLEVEL_QUIET		CONFIG_CONSOLE_LOGLEVEL_QUIET

	// extern int console_printk[];

	// #define console_loglevel			(console_printk[0])
	// #define default_message_loglevel	(console_printk[1])
	// #define minimum_console_loglevel	(console_printk[2])
	// #define default_console_loglevel	(console_printk[3])

	extern void console_verbose(void);

	/* strlen("ratelimit") + 1 */
	#define DEVKMSG_STR_MAX_SIZE 10
	extern char devkmsg_log_str[];
	struct ctl_table;

	extern int suppress_printk;

	struct va_format {
		const char *fmt;
		va_list *va;
	};

	/*
	 * FW_BUG
	 * Add this to a message where you are sure the firmware is buggy or behaves
	 * really stupid or out of spec. Be aware that the responsible BIOS developer
	 * should be able to fix this issue or at least get a concrete idea of the
	 * problem by reading your message without the need of looking at the kernel
	 * code.
	 *
	 * Use it for definite and high priority BIOS bugs.
	 *
	 * FW_WARN
	 * Use it for not that clear (e.g. could the kernel messed up things already?)
	 * and medium priority BIOS bugs.
	 *
	 * FW_INFO
	 * Use this one if you want to tell the user or vendor about something
	 * suspicious, but generally harmless related to the firmware.
	 *
	 * Use it for information or very low priority BIOS bugs.
	 */
	#define FW_BUG		"[Firmware Bug]: "
	#define FW_WARN		"[Firmware Warn]: "
	#define FW_INFO		"[Firmware Info]: "

	/*
	 * HW_ERR
	 * Add this to a message for hardware errors, so that user can report
	 * it to hardware vendor instead of LKML or software vendor.
	 */
	#define HW_ERR		"[Hardware Error]: "

	/*
	 * DEPRECATED
	 * Add this to a message whenever you want to warn user space about the use
	 * of a deprecated aspect of an API so they can stop using it
	 */
	#define DEPRECATED	"[Deprecated]: "

	/*
	 * Dummy printk for disabled debugging statements to use whilst maintaining
	 * gcc's format checking.
	 */
	#define no_printk(fmt, ...) ({					\
				if (0)								\
					printk(fmt, ##__VA_ARGS__);		\
				0;									\
			})

	extern asmlinkage __printf(1, 2)
	void early_printk(const char *fmt, ...);

	asmlinkage __printf(4, 0)
	int vprintk_emit(int facility, int level,
			const dev_printk_info_s *dev_info,
			const char *fmt, va_list args);

	asmlinkage __printf(1, 0)
	int vprintk(const char *fmt, va_list args);

	asmlinkage __printf(1, 2) __cold
	int _printk(const char *fmt, ...);

	// /*
	//  * Special printk facility for scheduler/timekeeping use only, _DO_NOT_USE_ !
	//  */
	// __printf(1, 2) __cold int _printk_deferred(const char *fmt, ...);

	// extern void __printk_safe_enter(void);
	// extern void __printk_safe_exit(void);
	// /*
	//  * The printk_deferred_enter/exit macros are available only as a hack for
	//  * some code paths that need to defer all printk console printing. Interrupts
	//  * must be disabled for the deferred duration.
	//  */
	// #define printk_deferred_enter __printk_safe_enter
	// #define printk_deferred_exit __printk_safe_exit

	// /*
	//  * Please don't use printk_ratelimit(), because it shares ratelimiting state
	//  * with all other unrelated printk_ratelimit() callsites.  Instead use
	//  * printk_ratelimited() or plain old __ratelimit().
	//  */
	// extern int __printk_ratelimit(const char *func);
	// #define printk_ratelimit() __printk_ratelimit(__func__)
	// extern bool printk_timed_ratelimit(unsigned long *caller_jiffies,
	// 		unsigned int interval_msec);

	// extern int printk_delay_msec;
	// extern int dmesg_restrict;

	// extern void wake_up_klogd(void);

	// char *log_buf_addr_get(void);
	// u32 log_buf_len_get(void);
	// void log_buf_vmcoreinfo_setup(void);
	// void __init setup_log_buf(int early);
	// __printf(1, 2) void dump_stack_set_arch_desc(const char *fmt, ...);
	// void dump_stack_print_info(const char *log_lvl);
	// void show_regs_print_info(const char *log_lvl);
	// extern asmlinkage void dump_stack_lvl(const char *log_lvl) __cold;
	// extern asmlinkage void dump_stack(void) __cold;
	// void printk_trigger_flush(void);

	// extern int __printk_cpu_sync_try_get(void);
	// extern void __printk_cpu_sync_wait(void);
	// extern void __printk_cpu_sync_put(void);

	// /**
	//  * printk_cpu_sync_get_irqsave() - Disable interrupts and acquire the printk
	//  *                                 cpu-reentrant spinning lock.
	//  * @flags: Stack-allocated storage for saving local interrupt state,
	//  *         to be passed to printk_cpu_sync_put_irqrestore().
	//  *
	//  * If the lock is owned by another CPU, spin until it becomes available.
	//  * Interrupts are restored while spinning.
	//  *
	//  * CAUTION: This function must be used carefully. It does not behave like a
	//  * typical lock. Here are important things to watch out for...
	//  *
	//  *     * This function is reentrant on the same CPU. Therefore the calling
	//  *       code must not assume exclusive access to data if code accessing the
	//  *       data can run reentrant or within NMI context on the same CPU.
	//  *
	//  *     * If there exists usage of this function from NMI context, it becomes
	//  *       unsafe to perform any type of locking or spinning to wait for other
	//  *       CPUs after calling this function from any context. This includes
	//  *       using spinlocks or any other busy-waiting synchronization methods.
	//  */
	// #define printk_cpu_sync_get_irqsave(flags)			\
	// 			for (;;) {								\
	// 				local_irq_save(flags);				\
	// 				if (__printk_cpu_sync_try_get())	\
	// 					break;							\
	// 				local_irq_restore(flags);			\
	// 				__printk_cpu_sync_wait();			\
	// 			}

	// /**
	//  * printk_cpu_sync_put_irqrestore() - Release the printk cpu-reentrant spinning
	//  *                                    lock and restore interrupts.
	//  * @flags: Caller's saved interrupt state, from printk_cpu_sync_get_irqsave().
	//  */
	// #define printk_cpu_sync_put_irqrestore(flags)	\
	// 			do {								\
	// 				__printk_cpu_sync_put();		\
	// 				local_irq_restore(flags);		\
	// 			} while (0)

	// extern int kptr_restrict;

	/**
	 * pr_fmt - used by the pr_*() macros to generate the printk format string
	 * @fmt: format string passed from a pr_*() macro
	 *
	 * This macro can be used to generate a unified format string for pr_*()
	 * macros. A common use is to prefix all pr_*() messages in a file with a common
	 * string. For example, defining this at the top of a source file:
	 *
	 *        #define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
	 *
	 * would prefix all pr_info, pr_emerg... messages in the file with the module
	 * name.
	 */
	#ifndef pr_fmt
	#	define pr_fmt(fmt)	fmt
	#endif

	// struct module;

	// #ifdef CONFIG_PRINTK_INDEX
		struct pi_entry {
			const char		*fmt;
			const char		*func;
			const char		*file;
			unsigned int	line;

			/*
			 * While printk and pr_* have the level stored in the string at compile
			 * time, some subsystems dynamically add it at runtime through the
			 * format string. For these dynamic cases, we allow the subsystem to
			 * tell us the level at compile time.
			 *
			 * NULL indicates that the level, if any, is stored in fmt.
			 */
			const char		*level;

			/*
			 * The format string used by various subsystem specific printk()
			 * wrappers to prefix the message.
			 *
			 * Note that the static prefix defined by the pr_fmt() macro is stored
			 * directly in the message format (@fmt), not here.
			 */
			const char		*subsys_fmt_prefix;
		} __packed;

	#	define __printk_index_emit(_fmt, _level, _subsys_fmt_prefix)	\
				do {													\
					if (__builtin_constant_p(_fmt) &&					\
						__builtin_constant_p(_level)) { 				\
						/*
						* We check __builtin_constant_p multiple times here
						* for the same input because GCC will produce an error
						* if we try to assign a static variable to fmt if it
						* is not a constant, even with the outer if statement.
						*/												\
						static const struct pi_entry _entry				\
						__used = {										\
							.fmt = __builtin_constant_p(_fmt) ?			\
										(_fmt) : NULL,					\
							.func = __func__,							\
							.file = __FILE__,							\
							.line = __LINE__,							\
							.level = __builtin_constant_p(_level) ?		\
										(_level) : NULL,				\
							.subsys_fmt_prefix = _subsys_fmt_prefix,	\
						};												\
						static const struct pi_entry *_entry_ptr		\
						__used __section(".printk_index") = &_entry;	\
					}													\
				} while (0)

	// #else /* !CONFIG_PRINTK_INDEX */
	// #	define __printk_index_emit(...)	do {} while (0)
	// #endif /* CONFIG_PRINTK_INDEX */

	// /*
	//  * Some subsystems have their own custom printk that applies a va_format to a
	//  * generic format, for example, to include a device number or other metadata
	//  * alongside the format supplied by the caller.
	//  *
	//  * In order to store these in the way they would be emitted by the printk
	//  * infrastructure, the subsystem provides us with the start, fixed string, and
	//  * any subsequent text in the format string.
	//  *
	//  * We take a variable argument list as pr_fmt/dev_fmt/etc are sometimes passed
	//  * as multiple arguments (eg: `"%s: ", "blah"`), and we must only take the
	//  * first one.
	//  *
	//  * subsys_fmt_prefix must be known at compile time, or compilation will fail
	//  * (since this is a mistake). If fmt or level is not known at compile time, no
	//  * index entry will be made (since this can legitimately happen).
	//  */
	// #define printk_index_subsys_emit(subsys_fmt_prefix, level, fmt, ...) \
	// 			__printk_index_emit(fmt, level, subsys_fmt_prefix)

	#define printk_index_wrap(_p_func, _fmt, ...) ({			\
					__printk_index_emit(_fmt, NULL, NULL);		\
					_p_func(_fmt, ##__VA_ARGS__);				\
				})


	/**
	 * printk - print a kernel message
	 * @fmt: format string
	 *
	 * This is printk(). It can be called from any context. We want it to work.
	 *
	 * If printk indexing is enabled, _printk() is called from printk_index_wrap.
	 * Otherwise, printk is simply #defined to _printk.
	 *
	 * We try to grab the console_lock. If we succeed, it's easy - we log the
	 * output and call the console drivers.  If we fail to get the semaphore, we
	 * place the output into the log buffer and return. The current holder of
	 * the console_sem will notice the new output in console_unlock(); and will
	 * send it to the consoles before releasing the lock.
	 *
	 * One effect of this deferred printing is that code which calls printk() and
	 * then changes console_loglevel may break. This is because console_loglevel
	 * is inspected when the actual printing occurs.
	 *
	 * See also:
	 * printf(3)
	 *
	 * See the vsnprintf() documentation for format string extensions over C99.
	 */
	#define printk(fmt, ...)	\
				printk_index_wrap(_printk, fmt, ##__VA_ARGS__)
	// #define printk_deferred(fmt, ...)	\
	// 			printk_index_wrap(_printk_deferred, fmt, ##__VA_ARGS__)

	/**
	 * pr_emerg - Print an emergency-level message
	 * @fmt: format string
	 * @...: arguments for the format string
	 *
	 * This macro expands to a printk with KERN_EMERG loglevel. It uses pr_fmt() to
	 * generate the format string.
	 */
	#define pr_emerg(fmt, ...) \
				printk(KERN_EMERG pr_fmt(fmt), ##__VA_ARGS__)
	/**
	 * pr_alert - Print an alert-level message
	 * @fmt: format string
	 * @...: arguments for the format string
	 *
	 * This macro expands to a printk with KERN_ALERT loglevel. It uses pr_fmt() to
	 * generate the format string.
	 */
	#define pr_alert(fmt, ...) \
				printk(KERN_ALERT pr_fmt(fmt), ##__VA_ARGS__)
	/**
	 * pr_crit - Print a critical-level message
	 * @fmt: format string
	 * @...: arguments for the format string
	 *
	 * This macro expands to a printk with KERN_CRIT loglevel. It uses pr_fmt() to
	 * generate the format string.
	 */
	#define pr_crit(fmt, ...) \
				printk(KERN_CRIT pr_fmt(fmt), ##__VA_ARGS__)
	/**
	 * pr_err - Print an error-level message
	 * @fmt: format string
	 * @...: arguments for the format string
	 *
	 * This macro expands to a printk with KERN_ERR loglevel. It uses pr_fmt() to
	 * generate the format string.
	 */
	#define pr_err(fmt, ...) \
				printk(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)
	/**
	 * pr_warn - Print a warning-level message
	 * @fmt: format string
	 * @...: arguments for the format string
	 *
	 * This macro expands to a printk with KERN_WARNING loglevel. It uses pr_fmt()
	 * to generate the format string.
	 */
	#define pr_warn(fmt, ...) \
				printk(KERN_WARNING pr_fmt(fmt), ##__VA_ARGS__)
	/**
	 * pr_notice - Print a notice-level message
	 * @fmt: format string
	 * @...: arguments for the format string
	 *
	 * This macro expands to a printk with KERN_NOTICE loglevel. It uses pr_fmt() to
	 * generate the format string.
	 */
	#define pr_notice(fmt, ...) \
				printk(KERN_NOTICE pr_fmt(fmt), ##__VA_ARGS__)
	/**
	 * pr_info - Print an info-level message
	 * @fmt: format string
	 * @...: arguments for the format string
	 *
	 * This macro expands to a printk with KERN_INFO loglevel. It uses pr_fmt() to
	 * generate the format string.
	 */
	#define pr_info(fmt, ...) \
				printk(KERN_INFO pr_fmt(fmt), ##__VA_ARGS__)

	/**
	 * pr_cont - Continues a previous log message in the same line.
	 * @fmt: format string
	 * @...: arguments for the format string
	 *
	 * This macro expands to a printk with KERN_CONT loglevel. It should only be
	 * used when continuing a log message with no newline ('\n') enclosed. Otherwise
	 * it defaults back to KERN_DEFAULT loglevel.
	 */
	#define pr_cont(fmt, ...) \
				printk(KERN_CONT fmt, ##__VA_ARGS__)

	// /**
	//  * pr_devel - Print a debug-level message conditionally
	//  * @fmt: format string
	//  * @...: arguments for the format string
	//  *
	//  * This macro expands to a printk with KERN_DEBUG loglevel if DEBUG is
	//  * defined. Otherwise it does nothing.
	//  *
	//  * It uses pr_fmt() to generate the format string.
	//  */
	// #ifdef DEBUG
	// #	define pr_devel(fmt, ...) \
	// 			printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
	// #else
	// #	define pr_devel(fmt, ...) \
	// 			no_printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
	// #endif


	// /* If you are writing a driver, please use dev_dbg instead */
	// #if defined(CONFIG_DYNAMIC_DEBUG) || \
	// 	(defined(CONFIG_DYNAMIC_DEBUG_CORE) && defined(DYNAMIC_DEBUG_MODULE))
	// #	include <linux/dynamic_debug.h>

	// 	/**
	// 	 * pr_debug - Print a debug-level message conditionally
	// 	 * @fmt: format string
	// 	 * @...: arguments for the format string
	// 	 *
	// 	 * This macro expands to dynamic_pr_debug() if CONFIG_DYNAMIC_DEBUG is
	// 	 * set. Otherwise, if DEBUG is defined, it's equivalent to a printk with
	// 	 * KERN_DEBUG loglevel. If DEBUG is not defined it does nothing.
	// 	 *
	// 	 * It uses pr_fmt() to generate the format string (dynamic_pr_debug() uses
	// 	 * pr_fmt() internally).
	// 	 */
	// #	define pr_debug(fmt, ...)	\
	// 			dynamic_pr_debug(fmt, ##__VA_ARGS__)
	// #elif defined(DEBUG)
	// #	define pr_debug(fmt, ...) \
	// 			printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
	// #else
	// #	define pr_debug(fmt, ...) \
	// 			no_printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
	// #endif

	// /*
	//  * Print a one-time message (analogous to WARN_ONCE() et al):
	//  */

	// #ifdef CONFIG_PRINTK
	// #	define printk_once(fmt, ...)	\
	// 			DO_ONCE_LITE(printk, fmt, ##__VA_ARGS__)
	// #	define printk_deferred_once(fmt, ...)	\
	// 			DO_ONCE_LITE(printk_deferred, fmt, ##__VA_ARGS__)
	// #else
	// #	define printk_once(fmt, ...)	\
	// 			no_printk(fmt, ##__VA_ARGS__)
	// #	define printk_deferred_once(fmt, ...)	\
	// 			no_printk(fmt, ##__VA_ARGS__)
	// #endif

	// #define pr_emerg_once(fmt, ...)	\
	// 			printk_once(KERN_EMERG pr_fmt(fmt), ##__VA_ARGS__)
	// #define pr_alert_once(fmt, ...)	\
	// 			printk_once(KERN_ALERT pr_fmt(fmt), ##__VA_ARGS__)
	// #define pr_crit_once(fmt, ...)	\
	// 			printk_once(KERN_CRIT pr_fmt(fmt), ##__VA_ARGS__)
	// #define pr_err_once(fmt, ...)	\
	// 			printk_once(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)
	// #define pr_warn_once(fmt, ...)	\
	// 			printk_once(KERN_WARNING pr_fmt(fmt), ##__VA_ARGS__)
	// #define pr_notice_once(fmt, ...)	\
	// 			printk_once(KERN_NOTICE pr_fmt(fmt), ##__VA_ARGS__)
	// #define pr_info_once(fmt, ...)	\
	// 			printk_once(KERN_INFO pr_fmt(fmt), ##__VA_ARGS__)
	// /* no pr_cont_once, don't do that... */

	// #if defined(DEBUG)
	// #	define pr_devel_once(fmt, ...)	\
	// 			printk_once(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
	// #else
	// #	define pr_devel_once(fmt, ...)	\
	// 			no_printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
	// #endif

	// /* If you are writing a driver, please use dev_dbg instead */
	// #if defined(DEBUG)
	// #	define pr_debug_once(fmt, ...)					\
	// 			printk_once(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
	// #else
	// #	define pr_debug_once(fmt, ...)					\
	// 			no_printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
	// #endif

	// /*
	//  * ratelimited messages with local ratelimit_state,
	//  * no local ratelimit_state used in the !PRINTK case
	//  */
	// #ifdef CONFIG_PRINTK
	// #	define printk_ratelimited(fmt, ...)	({						\
	// 				static DEFINE_RATELIMIT_STATE(_rs,				\
	// 								DEFAULT_RATELIMIT_INTERVAL,		\
	// 								DEFAULT_RATELIMIT_BURST);		\
	// 				if (__ratelimit(&_rs))							\
	// 					printk(fmt, ##__VA_ARGS__);					\
	// 			})
	// #else
	// #	define printk_ratelimited(fmt, ...)	\
	// 			no_printk(fmt, ##__VA_ARGS__)
	// #endif

	// #define pr_emerg_ratelimited(fmt, ...)	\
	// 			printk_ratelimited(KERN_EMERG pr_fmt(fmt), ##__VA_ARGS__)
	// #define pr_alert_ratelimited(fmt, ...)	\
	// 			printk_ratelimited(KERN_ALERT pr_fmt(fmt), ##__VA_ARGS__)
	// #define pr_crit_ratelimited(fmt, ...)	\
	// 			printk_ratelimited(KERN_CRIT pr_fmt(fmt), ##__VA_ARGS__)
	// #define pr_err_ratelimited(fmt, ...)	\
	// 			printk_ratelimited(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)
	// #define pr_warn_ratelimited(fmt, ...)	\
	// 			printk_ratelimited(KERN_WARNING pr_fmt(fmt), ##__VA_ARGS__)
	// #define pr_notice_ratelimited(fmt, ...)	\
	// 			printk_ratelimited(KERN_NOTICE pr_fmt(fmt), ##__VA_ARGS__)
	// #define pr_info_ratelimited(fmt, ...)	\
	// 			printk_ratelimited(KERN_INFO pr_fmt(fmt), ##__VA_ARGS__)
	// /* no pr_cont_ratelimited, don't do that... */

	// #if defined(DEBUG)
	// #	define pr_devel_ratelimited(fmt, ...)	\
	// 			printk_ratelimited(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
	// #else
	// #	define pr_devel_ratelimited(fmt, ...)	\
	// 			no_printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
	// #endif

	// /* If you are writing a driver, please use dev_dbg instead */
	// #if defined(CONFIG_DYNAMIC_DEBUG) ||	\
	// 	(defined(CONFIG_DYNAMIC_DEBUG_CORE) && defined(DYNAMIC_DEBUG_MODULE))
	// /* descriptor check is first to prevent flooding with "callbacks suppressed" */
	// #	define pr_debug_ratelimited(fmt, ...)	\
	// 			do {															\
	// 				static DEFINE_RATELIMIT_STATE(_rs,							\
	// 						DEFAULT_RATELIMIT_INTERVAL,							\
	// 						DEFAULT_RATELIMIT_BURST);							\
	// 				DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, pr_fmt(fmt));		\
	// 				if (DYNAMIC_DEBUG_BRANCH(descriptor) &&						\
	// 					__ratelimit(&_rs))										\
	// 					__dynamic_pr_debug(&descriptor, pr_fmt(fmt),			\
	// 							##__VA_ARGS__);									\
	// 			} while (0)
	// #elif defined(DEBUG)
	// #	define pr_debug_ratelimited(fmt, ...)	\
	// 			printk_ratelimited(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
	// #else
	// #	define pr_debug_ratelimited(fmt, ...)	\
	// 			no_printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
	// #endif

	// extern const struct file_operations kmsg_fops;

	// enum {
	// 	DUMP_PREFIX_NONE,
	// 	DUMP_PREFIX_ADDRESS,
	// 	DUMP_PREFIX_OFFSET
	// };
	// extern int hex_dump_to_buffer(const void *buf, size_t len, int rowsize,
	// 				int groupsize, char *linebuf, size_t linebuflen,
	// 				bool ascii);
	// #ifdef CONFIG_PRINTK
	// 	extern void print_hex_dump(const char *level, const char *prefix_str,
	// 				int prefix_type, int rowsize, int groupsize,
	// 				const void *buf, size_t len, bool ascii);
	// #else
	// 	static inline void
	// 	print_hex_dump(const char *level, const char *prefix_str,
	// 			int prefix_type, int rowsize, int groupsize,
	// 			const void *buf, size_t len, bool ascii) {
	// 	}
	// 	static inline void
	// 	print_hex_dump_bytes(const char *prefix_str,
	// 			int prefix_type, const void *buf, size_t len) {
	// 	}
	// #endif

	// #if defined(CONFIG_DYNAMIC_DEBUG) || \
	// 	(defined(CONFIG_DYNAMIC_DEBUG_CORE) && defined(DYNAMIC_DEBUG_MODULE))
	// #	define print_hex_dump_debug(prefix_str, prefix_type,		\
	// 					rowsize, groupsize, buf, len, ascii)		\
	// 				dynamic_hex_dump(prefix_str, prefix_type,		\
	// 						rowsize, groupsize, buf, len, ascii)
	// #elif defined(DEBUG)
	// #	define print_hex_dump_debug(prefix_str, prefix_type,		\
	// 					rowsize, groupsize, buf, len, ascii)		\
	// 			print_hex_dump(KERN_DEBUG, prefix_str, prefix_type,	\
	// 					rowsize, groupsize, buf, len, ascii)
	// #else
	// 	static inline void
	// 	print_hex_dump_debug(const char *prefix_str,
	// 			int prefix_type, int rowsize, int groupsize,
	// 			const void *buf, size_t len, bool ascii) {
	// 	}
	// #endif

	// /**
	//  * print_hex_dump_bytes - shorthand form of print_hex_dump() with default params
	//  * @prefix_str: string to prefix each line with;
	//  *  caller supplies trailing spaces for alignment if desired
	//  * @prefix_type: controls whether prefix of an offset, address, or none
	//  *  is printed (%DUMP_PREFIX_OFFSET, %DUMP_PREFIX_ADDRESS, %DUMP_PREFIX_NONE)
	//  * @buf: data blob to dump
	//  * @len: number of bytes in the @buf
	//  *
	//  * Calls print_hex_dump(), with log level of KERN_DEBUG,
	//  * rowsize of 16, groupsize of 1, and ASCII output included.
	//  */
	// #define print_hex_dump_bytes(prefix_str, prefix_type, buf, len)	\
	// 			print_hex_dump_debug(prefix_str, prefix_type, 16, 1,	\
	// 					buf, len, true)

#endif
