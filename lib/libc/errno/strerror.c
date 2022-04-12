/*
 * libc/errno/strerror.c
 */

#include <errno.h>

const char *const sys_errlist[] = {
	"No error: 0",				/*  0 - ENOERROR */
	"Operation not permitted",		/*  1 - EPERM */
	"No such file or directory",		/*  2 - ENOENT */
	"No such process",			/*  3 - ESRCH */
	"Interrupted system call",		/*  4 - EINTR */
	"Input/output error",			/*  5 - EIO */
	"Device not configured",		/*  6 - ENXIO */
	"Argument list too long",		/*  7 - E2BIG */
	"Exec format error",			/*  8 - ENOEXEC */
	"Bad file descriptor",			/*  9 - EBADF */
	"No child processes",			/* 10 - ECHILD */
	"Resource deadlock avoided",		/* 11 - EDEADLK */
	"Cannot allocate memory",		/* 12 - ENOMEM */
	"Permission denied",			/* 13 - EACCES */
	"Bad address",				/* 14 - EFAULT */
	"Block device required",		/* 15 - ENOTBLK */
	"Device busy",				/* 16 - EBUSY */
	"File exists",				/* 17 - EEXIST */
	"Cross-device link",			/* 18 - EXDEV */
	"Operation not supported by device",	/* 19 - ENODEV */
	"Not a directory",			/* 20 - ENOTDIR */
	"Is a directory",			/* 21 - EISDIR */
	"Invalid argument",			/* 22 - EINVAL */
	"Too many open files in system",	/* 23 - ENFILE */
	"Too many open files",			/* 24 - EMFILE */
	"Inappropriate ioctl for device",	/* 25 - ENOTTY */
	"Text file busy",			/* 26 - ETXTBSY */
	"File too large",			/* 27 - EFBIG */
	"No space left on device",		/* 28 - ENOSPC */
	"Illegal seek",				/* 29 - ESPIPE */
	"Read-only file system",		/* 30 - EROFS */
	"Too many links",			/* 31 - EMLINK */
	"Broken pipe",				/* 32 - EPIPE */

/* math software */
	"Numerical argument out of domain",	/* 33 - EDOM */
	"Result too large",			/* 34 - ERANGE */

/* non-blocking and interrupt i/o */
	"Resource temporarily unavailable",	/* 35 - EAGAIN */
						/* 35 - EWOULDBLOCK */
	"Operation now in progress",		/* 36 - EINPROGRESS */
	"Operation already in progress",	/* 37 - EALREADY */

/* ipc/network software -- argument errors */
	"Socket operation on non-socket",	/* 38 - ENOTSOCK */
	"Destination address required",		/* 39 - EDESTADDRREQ */
	"Message too long",			/* 40 - EMSGSIZE */
	"Protocol wrong type for socket",	/* 41 - EPROTOTYPE */
	"Protocol not available",		/* 42 - ENOPROTOOPT */
	"Protocol not supported",		/* 43 - EPROTONOSUPPORT */
	"Socket type not supported",		/* 44 - ESOCKTNOSUPPORT */
	"Operation not supported",		/* 45 - EOPNOTSUPP */
	"Protocol family not supported",	/* 46 - EPFNOSUPPORT */
						/* 47 - EAFNOSUPPORT */
	"Address family not supported by protocol family",
	"Address already in use",		/* 48 - EADDRINUSE */
	"Can't assign requested address",	/* 49 - EADDRNOTAVAIL */

/* ipc/network software -- operational errors */
	"Network is down",			/* 50 - ENETDOWN */
	"Network is unreachable",		/* 51 - ENETUNREACH */
	"Network dropped connection on reset",	/* 52 - ENETRESET */
	"Software caused connection abort",	/* 53 - ECONNABORTED */
	"Connection reset by peer",		/* 54 - ECONNRESET */
	"No buffer space available",		/* 55 - ENOBUFS */
	"Socket is already connected",		/* 56 - EISCONN */
	"Socket is not connected",		/* 57 - ENOTCONN */
	"Can't send after socket shutdown",	/* 58 - ESHUTDOWN */
	"Too many references: can't splice",	/* 59 - ETOOMANYREFS */
	"Operation timed out",			/* 60 - ETIMEDOUT */
	"Connection refused",			/* 61 - ECONNREFUSED */

	"Too many levels of symbolic links",	/* 62 - ELOOP */
	"File name too long",			/* 63 - ENAMETOOLONG */

/* should be rearranged */
	"Host is down",				/* 64 - EHOSTDOWN */
	"No route to host",			/* 65 - EHOSTUNREACH */
	"Directory not empty",			/* 66 - ENOTEMPTY */

/* quotas & mush */
	"Too many processes",			/* 67 - EPROCLIM */
	"Too many users",			/* 68 - EUSERS */
	"Disc quota exceeded",			/* 69 - EDQUOT */

/* Network File System */
	"Stale NFS file handle",		/* 70 - ESTALE */
	"Too many levels of remote in path",	/* 71 - EREMOTE */
	"RPC struct is bad",			/* 72 - EBADRPC */
	"RPC version wrong",			/* 73 - ERPCMISMATCH */
	"RPC prog. not avail",			/* 74 - EPROGUNAVAIL */
	"Program version wrong",		/* 75 - EPROGMISMATCH */
	"Bad procedure for program",		/* 76 - EPROCUNAVAIL */

	"No locks available",			/* 77 - ENOLCK */
	"Function not implemented",		/* 78 - ENOSYS */
	"Inappropriate file type or format",	/* 79 - EFTYPE */
	"Authentication error",			/* 80 - EAUTH */
	"Need authenticator",			/* 81 - ENEEDAUTH */
	"Identifier removed",			/* 82 - EIDRM */
	"No message of desired type",		/* 83 - ENOMSG */
	"Value too large to be stored in data type", /* 84 - EOVERFLOW */
	"Operation canceled",			/* 85 - ECANCELED */
	"Illegal byte sequence",		/* 86 - EILSEQ */
	"Attribute not found",			/* 87 - ENOATTR */

/* General */
	"Programming error",			/* 88 - EDOOFUS */

	"Bad message",				/* 89 - EBADMSG */
	"Multihop attempted",			/* 90 - EMULTIHOP */
	"Link has been severed",		/* 91 - ENOLINK */
	"Protocol error",			/* 92 - EPROTO */
	"Capabilities insufficient",		/* 93 - ENOTCAPABLE */
	"Not permitted in capability mode",	/* 94 - ECAPMODE */
	"State not recoverable",		/* 95 - ENOTRECOVERABLE */
	"Previous owner died",			/* 96 - EOWNERDEAD */
	"Integrity check failed",		/* 97 - EINTEGRITY */

/*
 * Reserved space in sys_errlist, take the next slot for a next error code.
 * Reserve prevents the array size from changing for some time.
 */
	// __uprefix,				/* 98 */
	// __uprefix,				/* 99 */
	// __uprefix,				/* 100 */
	// __uprefix,				/* 101 */
	// __uprefix,				/* 102 */
	// __uprefix,				/* 103 */
	// __uprefix,				/* 104 */
	// __uprefix,				/* 105 */
	// __uprefix,				/* 106 */
	// __uprefix,				/* 107 */
	// __uprefix,				/* 108 */
	// __uprefix,				/* 109 */
	// __uprefix,				/* 110 */
	// __uprefix,				/* 111 */
	// __uprefix,				/* 112 */
	// __uprefix,				/* 113 */
	// __uprefix,				/* 114 */
	// __uprefix,				/* 115 */
	// __uprefix,				/* 116 */
	// __uprefix,				/* 117 */
	// __uprefix,				/* 118 */
	// __uprefix,				/* 119 */
	// __uprefix,				/* 120 */
	// __uprefix,				/* 121 */
	// __uprefix,				/* 122 */
	// __uprefix,				/* 123 */
	// __uprefix,				/* 124 */
	// __uprefix,				/* 125 */
	// __uprefix,				/* 126 */
	// __uprefix,				/* 127 */
	// __uprefix,				/* 128 */
	// __uprefix,				/* 129 */
	// __uprefix,				/* 130 */
	// __uprefix,				/* 131 */
	// __uprefix,				/* 132 */
	// __uprefix,				/* 133 */
	// __uprefix,				/* 134 */
	// __uprefix,				/* 135 */
	// __uprefix,				/* 136 */
	// __uprefix,				/* 137 */
	// __uprefix,				/* 138 */
	// __uprefix,				/* 139 */
	// __uprefix,				/* 140 */
	// __uprefix,				/* 141 */
	// __uprefix,				/* 142 */
	// __uprefix,				/* 143 */
	// __uprefix,				/* 144 */
	// __uprefix,				/* 145 */
	// __uprefix,				/* 146 */
	// __uprefix,				/* 147 */
	// __uprefix,				/* 148 */
	// __uprefix,				/* 149 */
	// __uprefix,				/* 150 */
};
const int sys_nerr = ELAST + 1;

const char *const strerror(int e)
{
	unsigned int uerr;
	uerr = (e >= 0) ? e : -e;

	if (e >= sys_nerr)
		return "Invalid error number";
	else
		return sys_errlist[uerr];
}