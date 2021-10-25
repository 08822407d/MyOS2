#ifndef __ERRNO_H__
#define __ERRNO_H__

#ifdef __cplusplus
extern "C" {
#endif

enum {
	ENOERR			=  0,
	// EDOM			= -1,
	// ERANGE			= -2,
	// ENOSYS			= -3,
	// EINVAL			= -4,
	// ESPIPE			= -5,
	// EBADF			= -6,
	// ENOMEM			= -7,
	// EACCES			= -8,
	// ENFILE			= -9,
	// EMFILE			= -10,
	// ENAMETOOLONG	= -11,
	// ELOOP			= -12,
	// ENOMSG			= -13,
	// E2BIG			= -14,
	// EINTR			= -15,
	// EILSEQ			= -16,
	// ENOEXEC			= -17,
	// ENOENT			= -18,
	// EPROTOTYPE		= -19,
	// ESRCH			= -20,
	// EPERM			= -21,
	// ENOTDIR			= -22,
	// ESTALE			= -23,
	// EISDIR			= -24,
	// EOPNOTSUPP		= -25,
	// ENOTTY			= -26,
	// EAGAIN			= -27,
	// EIO				= -28,
	// ENOSPC			= -29,
	// EEXIST			= -30,
	// EBUSY			= -31,
	// EOVERFLOW		= -32,
	EPERM			= 1,		/* Operation not permitted */
	ENOENT			= 2,		/* No such file or directory */
	ESRCH			= 3,		/* No such process */
	EINTR			= 4,		/* Interrupted system call */
	EIO				= 5,		/* Input/output error */
	ENXIO			= 6,		/* Device not configured */
	E2BIG			= 7,		/* Argument list too long */
	ENOEXEC			= 8,		/* Exec format error */
	EBADF			= 9,		/* Bad file descriptor */
	ECHILD			= 10,		/* No child processes */
	EDEADLK			= 11,		/* Resource deadlock avoided */
								/* 11 was EAGAIN */
	ENOMEM			= 12,		/* Cannot allocate memory */
	EACCES			= 13,		/* Permission denied */
	EFAULT			= 14,		/* Bad address */
	#ifndef _POSIX_SOURCE
		ENOTBLK			= 15,		/* Block device required */
	#endif
	EBUSY			= 16,		/* Device busy */
	EEXIST			= 17,		/* File exists */
	EXDEV			= 18,		/* Cross-device link */
	ENODEV			= 19,		/* Operation not supported by device */
	ENOTDIR			= 20,		/* Not a directory */
	EISDIR			= 21,		/* Is a directory */
	EINVAL			= 22,		/* Invalid argument */
	ENFILE			= 23,		/* Too many open files in system */
	EMFILE			= 24,		/* Too many open files */
	ENOTTY			= 25,		/* Inappropriate ioctl for device */
	#ifndef _POSIX_SOURCE
		#define	ETXTBSY			= 26,		/* Text file busy */
	#endif
	EFBIG			= 27,		/* File too large */
	ENOSPC			= 28,		/* No space left on device */
	ESPIPE			= 29,		/* Illegal seek */
	EROFS			= 30,		/* Read-only filesystem */
	EMLINK			= 31,		/* Too many links */
	EPIPE			= 32,		/* Broken pipe */

	/* math software */
	EDOM			= 33,		/* Numerical argument out of domain */
	ERANGE			= 34,		/* Result too large */

	/* non-blocking and interrupt i/o */
	EAGAIN			= 35,		/* Resource temporarily unavailable */
	#ifndef _POSIX_SOURCE
		#define	EWOULDBLOCK	EAGAIN		/* Operation would block */
		EINPROGRESS		= 36,		/* Operation now in progress */
		EALREADY		= 37,		/* Operation already in progress */

		/* ipc/network software -- argument errors */
		ENOTSOCK		= 38,		/* Socket operation on non-socket */
		EDESTADDRREQ	= 39,		/* Destination address required */
		EMSGSIZE		= 40,		/* Message too long */
		EPROTOTYPE		= 41,		/* Protocol wrong type for socket */
		ENOPROTOOPT		= 42,		/* Protocol not available */
		EPROTONOSUPPORT	= 43,		/* Protocol not supported */
		ESOCKTNOSUPPORT	= 44,		/* Socket type not supported */
		EOPNOTSUPP		= 45,		/* Operation not supported */
		#define	ENOTSUP		EOPNOTSUPP	/* Operation not supported */
		EPFNOSUPPORT	= 46,		/* Protocol family not supported */
		EAFNOSUPPORT	= 47,		/* Address family not supported by protocol family */
		EADDRINUSE		= 48,		/* Address already in use */
		EADDRNOTAVAIL	= 49,		/* Can't assign requested address */

		/* ipc/network software -- operational errors */
		ENETDOWN		= 50,		/* Network is down */
		ENETUNREACH		= 51,		/* Network is unreachable */
		ENETRESET		= 52,		/* Network dropped connection on reset */
		ECONNABORTED	= 53,		/* Software caused connection abort */
		ECONNRESET		= 54,		/* Connection reset by peer */
		ENOBUFS			= 55,		/* No buffer space available */
		EISCONN			= 56,		/* Socket is already connected */
		ENOTCONN		= 57,		/* Socket is not connected */
		ESHUTDOWN		= 58,		/* Can't send after socket shutdown */
		ETOOMANYREFS	= 59,		/* Too many references: can't splice */
		ETIMEDOUT		= 60,		/* Operation timed out */
		ECONNREFUSED	= 61,		/* Connection refused */

		ELOOP			= 62,		/* Too many levels of symbolic links */
	#endif /* _POSIX_SOURCE */
	ENAMETOOLONG	= 63,		/* File name too long */

	/* should be rearranged */
	#ifndef _POSIX_SOURCE
		EHOSTDOWN		= 64,		/* Host is down */
		EHOSTUNREACH	= 65,		/* No route to host */
	#endif /* _POSIX_SOURCE */
	ENOTEMPTY		= 66,		/* Directory not empty */

	/* quotas & mush */
	#ifndef _POSIX_SOURCE
		EPROCLIM		= 67,		/* Too many processes */
		EUSERS			= 68,		/* Too many users */
		EDQUOT			= 69,		/* Disc quota exceeded */

		/* Network File System */
		ESTALE			= 70,		/* Stale NFS file handle */
		EREMOTE			= 71,		/* Too many levels of remote in path */
		EBADRPC			= 72,		/* RPC struct is bad */
		ERPCMISMATCH	= 73,		/* RPC version wrong */
		EPROGUNAVAIL	= 74,		/* RPC prog. not avail */
		EPROGMISMATCH	= 75,		/* Program version wrong */
		EPROCUNAVAIL	= 76,		/* Bad procedure for program */
	#endif /* _POSIX_SOURCE */

	ENOLCK			= 77,		/* No locks available */
	ENOSYS			= 78,		/* Function not implemented */

	#ifndef _POSIX_SOURCE
		EFTYPE			= 79,		/* Inappropriate file type or format */
		EAUTH			= 80,		/* Authentication error */
		ENEEDAUTH		= 81,		/* Need authenticator */
		EIDRM			= 82,		/* Identifier removed */
		ENOMSG			= 83,		/* No message of desired type */
		EOVERFLOW		= 84,		/* Value too large to be stored in data type */
		ECANCELED		= 85,		/* Operation canceled */
		EILSEQ			= 86,		/* Illegal byte sequence */
		ENOATTR			= 87,		/* Attribute not found */

		EDOOFUS			= 88,		/* Programming error */
	#endif /* _POSIX_SOURCE */

	EBADMSG			= 89,		/* Bad message */
	EMULTIHOP		= 90,		/* Multihop attempted */
	ENOLINK			= 91,		/* Link has been severed */
	EPROTO			= 92,		/* Protocol error */

	#ifndef _POSIX_SOURCE
		ENOTCAPABLE		= 93,		/* Capabilities insufficient */
		ECAPMODE		= 94,		/* Not permitted in capability mode */
		ENOTRECOVERABLE	= 95,		/* State not recoverable */
		EOWNERDEAD		= 96,		/* Previous owner died */
		EINTEGRITY		= 97,		/* Integrity check failed */
	#endif /* _POSIX_SOURCE */

	#ifndef _POSIX_SOURCE
		ELAST			= 97,		/* Must be equal largest errno */
	#endif /* _POSIX_SOURCE */
};

/*
 * A macro expanding to the errno l-value
 */
#ifndef _KERNEL
	#define errno 	(*__task_errno_location())
#endif

/*
 * Internal function returning the address of the task-specific errno
 */
volatile int * __task_errno_location(void);

/*
 * Returns a string containing a message derived from an error code
 */
char * strerror(int e);

#ifdef __cplusplus
}
#endif

#endif /* __ERRNO_H__ */
