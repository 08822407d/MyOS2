/* Copyright (C) 1991-2022 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include <stdio.h>
#include <errno.h>
// #include <libintl.h>
// #include <array_length.h>

const char *const _sys_errlist_internal[] =
{
	[0] = "Success",

#ifdef EPERM
	/*
	TRANS Only the owner of the file (or other resource)
	TRANS or processes with special privileges can perform the operation. */
	[EPERM] = "Operation not permitted",
#endif

#ifdef ENOENT
	/*
	TRANS This is a ``file doesn't exist'' error
	TRANS for ordinary files that are referenced in contexts where they are
	TRANS expected to already exist. */
	[ENOENT] = "No such file or directory",
#endif

#ifdef ESRCH
	/*
	TRANS No process matches the specified process ID. */
	[ESRCH] = "No such process",
#endif

#ifdef EINTR
	/*
	TRANS An asynchronous signal occurred and prevented
	TRANS completion of the call.  When this happens, you should try the call
	TRANS again.
	TRANS
	TRANS You can choose to have functions resume after a signal that is handled,
	TRANS rather than failing with @code{EINTR}; see @ref{Interrupted
	TRANS Primitives}. */
	[EINTR] = "Interrupted system call",
#endif

#ifdef EIO
	/*
	TRANS Usually used for physical read or write errors. */
	[EIO] = "Input/output error",
#endif

#ifdef ENXIO
	/*
	TRANS The system tried to use the device
	TRANS represented by a file you specified, and it couldn't find the device.
	TRANS This can mean that the device file was installed incorrectly, or that
	TRANS the physical device is missing or not correctly attached to the
	TRANS computer. */
	[ENXIO] = "No such device or address",
#endif

#ifdef E2BIG
	/*
	TRANS Used when the arguments passed to a new program
	TRANS being executed with one of the @code{exec} functions (@pxref{Executing a
	TRANS File}) occupy too much memory space.  This condition never arises on
	TRANS @gnuhurdsystems{}. */
	[E2BIG] = "Argument list too long",
#endif

#ifdef ENOEXEC
	/*
	TRANS Invalid executable file format.  This condition is detected by the
	TRANS @code{exec} functions; see @ref{Executing a File}. */
	[ENOEXEC] = "Exec format error",
#endif

#ifdef EBADF
	/*
	TRANS For example, I/O on a descriptor that has been
	TRANS closed or reading from a descriptor open only for writing (or vice
	TRANS versa). */
	[EBADF] = "Bad file descriptor",
#endif

#ifdef ECHILD
	/*
	TRANS This error happens on operations that are
	TRANS supposed to manipulate child processes, when there aren't any processes
	TRANS to manipulate. */
	[ECHILD] = "No child processes",
#endif

#ifdef EDEADLK
	/*
	TRANS Allocating a system resource would have resulted in a
	TRANS deadlock situation.  The system does not guarantee that it will notice
	TRANS all such situations.  This error means you got lucky and the system
	TRANS noticed; it might just hang.  @xref{File Locks}, for an example. */
	[EDEADLK] = "Resource deadlock avoided",
#endif

#ifdef ENOMEM
	/*
	TRANS The system cannot allocate more virtual memory
	TRANS because its capacity is full. */
	[ENOMEM] = "Cannot allocate memory",
#endif

#ifdef EACCES
	/*
	TRANS The file permissions do not allow the attempted operation. */
	[EACCES] = "Permission denied",
#endif

#ifdef EFAULT
	/*
	TRANS An invalid pointer was detected.
	TRANS On @gnuhurdsystems{}, this error never happens; you get a signal instead. */
	[EFAULT] = "Bad address",
#endif

#ifdef ENOTBLK
	/*
	TRANS A file that isn't a block special file was given in a situation that
	TRANS requires one.  For example, trying to mount an ordinary file as a file
	TRANS system in Unix gives this error. */
	[ENOTBLK] = "Block device required",
#endif

#ifdef EBUSY
	/*
	TRANS A system resource that can't be shared is already in use.
	TRANS For example, if you try to delete a file that is the root of a currently
	TRANS mounted filesystem, you get this error. */
	[EBUSY] = "Device or resource busy",
#endif

#ifdef EEXIST
	/*
	TRANS An existing file was specified in a context where it only
	TRANS makes sense to specify a new file. */
	[EEXIST] = "File exists",
#endif

#ifdef EXDEV
	/*
	TRANS An attempt to make an improper link across file systems was detected.
	TRANS This happens not only when you use @code{link} (@pxref{Hard Links}) but
	TRANS also when you rename a file with @code{rename} (@pxref{Renaming Files}). */
	[EXDEV] = "Invalid cross-device link",
#endif

#ifdef ENODEV
	/*
	TRANS The wrong type of device was given to a function that expects a
	TRANS particular sort of device. */
	[ENODEV] = "No such device",
#endif

#ifdef ENOTDIR
	/*
	TRANS A file that isn't a directory was specified when a directory is required. */
	[ENOTDIR] = "Not a directory",
#endif

#ifdef EISDIR
	/*
	TRANS You cannot open a directory for writing,
	TRANS or create or remove hard links to it. */
	[EISDIR] = "Is a directory",
#endif

#ifdef EINVAL
	/*
	TRANS This is used to indicate various kinds of problems
	TRANS with passing the wrong argument to a library function. */
	[EINVAL] = "Invalid argument",
#endif

#ifdef EMFILE
	/*
	TRANS The current process has too many files open and can't open any more.
	TRANS Duplicate descriptors do count toward this limit.
	TRANS
	TRANS In BSD and GNU, the number of open files is controlled by a resource
	TRANS limit that can usually be increased.  If you get this error, you might
	TRANS want to increase the @code{RLIMIT_NOFILE} limit or make it unlimited;
	TRANS @pxref{Limits on Resources}. */
	[EMFILE] = "Too many open files",
#endif

#ifdef ENFILE
	/*
	TRANS There are too many distinct file openings in the entire system.  Note
	TRANS that any number of linked channels count as just one file opening; see
	TRANS @ref{Linked Channels}.  This error never occurs on @gnuhurdsystems{}. */
	[ENFILE] = "Too many open files in system",
#endif

#ifdef ENOTTY
	/*
	TRANS Inappropriate I/O control operation, such as trying to set terminal
	TRANS modes on an ordinary file. */
	[ENOTTY] = "Inappropriate ioctl for device",
#endif

#ifdef ETXTBSY
	/*
	TRANS An attempt to execute a file that is currently open for writing, or
	TRANS write to a file that is currently being executed.  Often using a
	TRANS debugger to run a program is considered having it open for writing and
	TRANS will cause this error.  (The name stands for ``text file busy''.)  This
	TRANS is not an error on @gnuhurdsystems{}; the text is copied as necessary. */
	[ETXTBSY] = "Text file busy",
#endif

#ifdef EFBIG
	/*
	TRANS The size of a file would be larger than allowed by the system. */
	[EFBIG] = "File too large",
#endif

#ifdef ENOSPC
	/*
	TRANS Write operation on a file failed because the
	TRANS disk is full. */
	[ENOSPC] = "No space left on device",
#endif

#ifdef ESPIPE
	/*
	TRANS Invalid seek operation (such as on a pipe). */
	[ESPIPE] = "Illegal seek",
#endif

#ifdef EROFS
	/*
	TRANS An attempt was made to modify something on a read-only file system. */
	[EROFS] = "Read-only file system",
#endif

#ifdef EMLINK
	/*
	TRANS The link count of a single file would become too large.
	TRANS @code{rename} can cause this error if the file being renamed already has
	TRANS as many links as it can take (@pxref{Renaming Files}). */
	[EMLINK] = "Too many links",
#endif

#ifdef EPIPE
	/*
	TRANS There is no process reading from the other end of a pipe.
	TRANS Every library function that returns this error code also generates a
	TRANS @code{SIGPIPE} signal; this signal terminates the program if not handled
	TRANS or blocked.  Thus, your program will never actually see @code{EPIPE}
	TRANS unless it has handled or blocked @code{SIGPIPE}. */
	[EPIPE] = "Broken pipe",
#endif

#ifdef EDOM
	/*
	TRANS Used by mathematical functions when an argument value does
	TRANS not fall into the domain over which the function is defined. */
	[EDOM] = "Numerical argument out of domain",
#endif

#ifdef ERANGE
	/*
	TRANS Used by mathematical functions when the result value is
	TRANS not representable because of overflow or underflow. */
	[ERANGE] = "Numerical result out of range",
#endif

#ifdef EAGAIN
	/*
	TRANS The call might work if you try again
	TRANS later.  The macro @code{EWOULDBLOCK} is another name for @code{EAGAIN};
	TRANS they are always the same in @theglibc{}.
	TRANS
	TRANS This error can happen in a few different situations:
	TRANS
	TRANS @itemize @bullet
	TRANS @item
	TRANS An operation that would block was attempted on an object that has
	TRANS non-blocking mode selected.  Trying the same operation again will block
	TRANS until some external condition makes it possible to read, write, or
	TRANS connect (whatever the operation).  You can use @code{select} to find out
	TRANS when the operation will be possible; @pxref{Waiting for I/O}.
	TRANS
	TRANS @strong{Portability Note:} In many older Unix systems, this condition
	TRANS was indicated by @code{EWOULDBLOCK}, which was a distinct error code
	TRANS different from @code{EAGAIN}.  To make your program portable, you should
	TRANS check for both codes and treat them the same.
	TRANS
	TRANS @item
	TRANS A temporary resource shortage made an operation impossible.  @code{fork}
	TRANS can return this error.  It indicates that the shortage is expected to
	TRANS pass, so your program can try the call again later and it may succeed.
	TRANS It is probably a good idea to delay for a few seconds before trying it
	TRANS again, to allow time for other processes to release scarce resources.
	TRANS Such shortages are usually fairly serious and affect the whole system,
	TRANS so usually an interactive program should report the error to the user
	TRANS and return to its command loop.
	TRANS @end itemize */
	[EAGAIN] = "Resource temporarily unavailable",
#endif

#ifdef EINPROGRESS
	/*
	TRANS An operation that cannot complete immediately was initiated on an object
	TRANS that has non-blocking mode selected.  Some functions that must always
	TRANS block (such as @code{connect}; @pxref{Connecting}) never return
	TRANS @code{EAGAIN}.  Instead, they return @code{EINPROGRESS} to indicate that
	TRANS the operation has begun and will take some time.  Attempts to manipulate
	TRANS the object before the call completes return @code{EALREADY}.  You can
	TRANS use the @code{select} function to find out when the pending operation
	TRANS has completed; @pxref{Waiting for I/O}. */
	[EINPROGRESS] = "Operation now in progress",
#endif

#ifdef EALREADY
	/*
	TRANS An operation is already in progress on an object that has non-blocking
	TRANS mode selected. */
	[EALREADY] = "Operation already in progress",
#endif

#ifdef ENOTSOCK
	/*
	TRANS A file that isn't a socket was specified when a socket is required. */
	[ENOTSOCK] = "Socket operation on non-socket",
#endif

#ifdef EMSGSIZE
	/*
	TRANS The size of a message sent on a socket was larger than the supported
	TRANS maximum size. */
	[EMSGSIZE] = "Message too long",
#endif

#ifdef EPROTOTYPE
	/*
	TRANS The socket type does not support the requested communications protocol. */
	[EPROTOTYPE] = "Protocol wrong type for socket",
#endif

#ifdef ENOPROTOOPT
	/*
	TRANS You specified a socket option that doesn't make sense for the
	TRANS particular protocol being used by the socket.  @xref{Socket Options}. */
	[ENOPROTOOPT] = "Protocol not available",
#endif

#ifdef EPROTONOSUPPORT
	/*
	TRANS The socket domain does not support the requested communications protocol
	TRANS (perhaps because the requested protocol is completely invalid).
	TRANS @xref{Creating a Socket}. */
	[EPROTONOSUPPORT] = "Protocol not supported",
#endif

#ifdef ESOCKTNOSUPPORT
	/*
	TRANS The socket type is not supported. */
	[ESOCKTNOSUPPORT] = "Socket type not supported",
#endif

#ifdef EOPNOTSUPP
	/*
	TRANS The operation you requested is not supported.  Some socket functions
	TRANS don't make sense for all types of sockets, and others may not be
	TRANS implemented for all communications protocols.  On @gnuhurdsystems{}, this
	TRANS error can happen for many calls when the object does not support the
	TRANS particular operation; it is a generic indication that the server knows
	TRANS nothing to do for that call. */
	[EOPNOTSUPP] = "Operation not supported",
#endif

#ifdef EPFNOSUPPORT
	/*
	TRANS The socket communications protocol family you requested is not supported. */
	[EPFNOSUPPORT] = "Protocol family not supported",
#endif

#ifdef EAFNOSUPPORT
	/*
	TRANS The address family specified for a socket is not supported; it is
	TRANS inconsistent with the protocol being used on the socket.  @xref{Sockets}. */
	[EAFNOSUPPORT] = "Address family not supported by protocol",
#endif

#ifdef EADDRINUSE
	/*
	TRANS The requested socket address is already in use.  @xref{Socket Addresses}. */
	[EADDRINUSE] = "Address already in use",
#endif

#ifdef EADDRNOTAVAIL
	/*
	TRANS The requested socket address is not available; for example, you tried
	TRANS to give a socket a name that doesn't match the local host name.
	TRANS @xref{Socket Addresses}. */
	[EADDRNOTAVAIL] = "Cannot assign requested address",
#endif

#ifdef ENETDOWN
	/*
	TRANS A socket operation failed because the network was down. */
	[ENETDOWN] = "Network is down",
#endif

#ifdef ENETUNREACH
	/*
	TRANS A socket operation failed because the subnet containing the remote host
	TRANS was unreachable. */
	[ENETUNREACH] = "Network is unreachable",
#endif

#ifdef ENETRESET
	/*
	TRANS A network connection was reset because the remote host crashed. */
	[ENETRESET] = "Network dropped connection on reset",
#endif

#ifdef ECONNABORTED
	/*
	TRANS A network connection was aborted locally. */
	[ECONNABORTED] = "Software caused connection abort",
#endif

#ifdef ECONNRESET
	/*
	TRANS A network connection was closed for reasons outside the control of the
	TRANS local host, such as by the remote machine rebooting or an unrecoverable
	TRANS protocol violation. */
	[ECONNRESET] = "Connection reset by peer",
#endif

#ifdef ENOBUFS
	/*
	TRANS The kernel's buffers for I/O operations are all in use.  In GNU, this
	TRANS error is always synonymous with @code{ENOMEM}; you may get one or the
	TRANS other from network operations. */
	[ENOBUFS] = "No buffer space available",
#endif

#ifdef EISCONN
	/*
	TRANS You tried to connect a socket that is already connected.
	TRANS @xref{Connecting}. */
	[EISCONN] = "Transport endpoint is already connected",
#endif

#ifdef ENOTCONN
	/*
	TRANS The socket is not connected to anything.  You get this error when you
	TRANS try to transmit data over a socket, without first specifying a
	TRANS destination for the data.  For a connectionless socket (for datagram
	TRANS protocols, such as UDP), you get @code{EDESTADDRREQ} instead. */
	[ENOTCONN] = "Transport endpoint is not connected",
#endif

#ifdef EDESTADDRREQ
	/*
	TRANS No default destination address was set for the socket.  You get this
	TRANS error when you try to transmit data over a connectionless socket,
	TRANS without first specifying a destination for the data with @code{connect}. */
	[EDESTADDRREQ] = "Destination address required",
#endif

#ifdef ESHUTDOWN
	/*
	TRANS The socket has already been shut down. */
	[ESHUTDOWN] = "Cannot send after transport endpoint shutdown",
#endif

#ifdef ETOOMANYREFS
	[ETOOMANYREFS] = "Too many references: cannot splice",
#endif

#ifdef ETIMEDOUT
	/*
	TRANS A socket operation with a specified timeout received no response during
	TRANS the timeout period. */
	[ETIMEDOUT] = "Connection timed out",
#endif

#ifdef ECONNREFUSED
	/*
	TRANS A remote host refused to allow the network connection (typically because
	TRANS it is not running the requested service). */
	[ECONNREFUSED] = "Connection refused",
#endif

#ifdef ELOOP
	/*
	TRANS Too many levels of symbolic links were encountered in looking up a file name.
	TRANS This often indicates a cycle of symbolic links. */
	[ELOOP] = "Too many levels of symbolic links",
#endif

#ifdef ENAMETOOLONG
	/*
	TRANS Filename too long (longer than @code{PATH_MAX}; @pxref{Limits for
	TRANS Files}) or host name too long (in @code{gethostname} or
	TRANS @code{sethostname}; @pxref{Host Identification}). */
	[ENAMETOOLONG] = "File name too long",
#endif

#ifdef EHOSTDOWN
	/*
	TRANS The remote host for a requested network connection is down. */
	[EHOSTDOWN] = "Host is down",
#endif

/*
TRANS The remote host for a requested network connection is not reachable. */
#ifdef EHOSTUNREACH
	[EHOSTUNREACH] = "No route to host",
#endif

#ifdef ENOTEMPTY
	/*
	TRANS Directory not empty, where an empty directory was expected.  Typically,
	TRANS this error occurs when you are trying to delete a directory. */
	[ENOTEMPTY] = "Directory not empty",
#endif

#ifdef EUSERS
	/*
	TRANS The file quota system is confused because there are too many users.
	TRANS @c This can probably happen in a GNU system when using NFS. */
	[EUSERS] = "Too many users",
#endif

#ifdef EDQUOT
	/*
	TRANS The user's disk quota was exceeded. */
	[EDQUOT] = "Disk quota exceeded",
#endif

#ifdef ESTALE
	/*
	TRANS This indicates an internal confusion in the
	TRANS file system which is due to file system rearrangements on the server host
	TRANS for NFS file systems or corruption in other file systems.
	TRANS Repairing this condition usually requires unmounting, possibly repairing
	TRANS and remounting the file system. */
	[ESTALE] = "Stale file handle",
#endif

#ifdef EREMOTE
	/*
	TRANS An attempt was made to NFS-mount a remote file system with a file name that
	TRANS already specifies an NFS-mounted file.
	TRANS (This is an error on some operating systems, but we expect it to work
	TRANS properly on @gnuhurdsystems{}, making this error code impossible.) */
	[EREMOTE] = "Object is remote",
#endif

#ifdef ENOLCK
	/*
	TRANS This is used by the file locking facilities; see
	TRANS @ref{File Locks}.  This error is never generated by @gnuhurdsystems{}, but
	TRANS it can result from an operation to an NFS server running another
	TRANS operating system. */
	[ENOLCK] = "No locks available",
#endif

#ifdef ENOSYS
	/*
	TRANS This indicates that the function called is
	TRANS not implemented at all, either in the C library itself or in the
	TRANS operating system.  When you get this error, you can be sure that this
	TRANS particular function will always fail with @code{ENOSYS} unless you
	TRANS install a new version of the C library or the operating system. */
	[ENOSYS] = "Function not implemented",
#endif

#ifdef EILSEQ
	/*
	TRANS While decoding a multibyte character the function came along an invalid
	TRANS or an incomplete sequence of bytes or the given wide character is invalid. */
	[EILSEQ] = "Invalid or incomplete multibyte or wide character",
#endif

#ifdef EBADMSG
	[EBADMSG] = "Bad message",
#endif

#ifdef EIDRM
	[EIDRM] = "Identifier removed",
#endif

#ifdef EMULTIHOP
	[EMULTIHOP] = "Multihop attempted",
#endif

#ifdef ENODATA
	[ENODATA] = "No data available",
#endif

#ifdef ENOLINK
	[ENOLINK] = "Link has been severed",
#endif

#ifdef ENOMSG
	[ENOMSG] = "No message of desired type",
#endif

#ifdef ENOSR
	[ENOSR] = "Out of streams resources",
#endif

#ifdef ENOSTR
	[ENOSTR] = "Device not a stream",
#endif

#ifdef EOVERFLOW
	[EOVERFLOW] = "Value too large for defined data type",
#endif

#ifdef EPROTO
	[EPROTO] = "Protocol error",
#endif

#ifdef ETIME
	[ETIME] = "Timer expired",
#endif

#ifdef ECANCELED
	/*
	TRANS An asynchronous operation was canceled before it
	TRANS completed.  @xref{Asynchronous I/O}.  When you call @code{aio_cancel},
	TRANS the normal result is for the operations affected to complete with this
	TRANS error; @pxref{Cancel AIO Operations}. */
	[ECANCELED] = "Operation canceled",
#endif

#ifdef EOWNERDEAD
	[EOWNERDEAD] = "Owner died",
#endif

#ifdef ENOTRECOVERABLE
	[ENOTRECOVERABLE] = "State not recoverable",
#endif

#ifdef ERESTART
	[ERESTART] = "Interrupted system call should be restarted",
#endif

#ifdef ECHRNG
	[ECHRNG] = "Channel number out of range",
#endif

#ifdef EL2NSYNC
	[EL2NSYNC] = "Level 2 not synchronized",
#endif

#ifdef EL3HLT
	[EL3HLT] = "Level 3 halted",
#endif

#ifdef EL3RST
	[EL3RST] = "Level 3 reset",
#endif

#ifdef ELNRNG
	[ELNRNG] = "Link number out of range",
#endif

#ifdef EUNATCH
	[EUNATCH] = "Protocol driver not attached",
#endif

#ifdef ENOCSI
	[ENOCSI] = "No CSI structure available",
#endif

#ifdef EL2HLT
	[EL2HLT] = "Level 2 halted",
#endif

#ifdef EBADE
	[EBADE] = "Invalid exchange",
#endif

#ifdef EBADR
	[EBADR] = "Invalid request descriptor",
#endif

#ifdef EXFULL
	[EXFULL] = "Exchange full",
#endif

#ifdef ENOANO
	[ENOANO] = "No anode",
#endif

#ifdef EBADRQC
	[EBADRQC] = "Invalid request code",
#endif

#ifdef EBADSLT
	[EBADSLT] = "Invalid slot",
#endif

#ifdef EBFONT
	[EBFONT] = "Bad font file format",
#endif

#ifdef ENONET
	[ENONET] = "Machine is not on the network",
#endif

#ifdef ENOPKG
	[ENOPKG] = "Package not installed",
#endif

#ifdef EADV
	[EADV] = "Advertise error",
#endif

#ifdef ESRMNT
	[ESRMNT] = "Srmount error",
#endif

#ifdef ECOMM
	[ECOMM] = "Communication error on send",
#endif

#ifdef EDOTDOT
	[EDOTDOT] = "RFS specific error",
#endif

#ifdef ENOTUNIQ
	[ENOTUNIQ] = "Name not unique on network",
#endif

#ifdef EBADFD
	[EBADFD] = "File descriptor in bad state",
#endif

#ifdef EREMCHG
	[EREMCHG] = "Remote address changed",
#endif

#ifdef ELIBACC
	[ELIBACC] = "Can not access a needed shared library",
#endif

#ifdef ELIBBAD
	[ELIBBAD] = "Accessing a corrupted shared library",
#endif

#ifdef ELIBSCN
	[ELIBSCN] = ".lib section in a.out corrupted",
#endif

#ifdef ELIBMAX
	[ELIBMAX] = "Attempting to link in too many shared libraries",
#endif

#ifdef ELIBEXEC
	[ELIBEXEC] = "Cannot exec a shared library directly",
#endif

#ifdef ESTRPIPE
	[ESTRPIPE] = "Streams pipe error",
#endif

#ifdef EUCLEAN
	[EUCLEAN] = "Structure needs cleaning",
#endif

#ifdef ENOTNAM
	[ENOTNAM] = "Not a XENIX named type file",
#endif

#ifdef ENAVAIL
	[ENAVAIL] = "No XENIX semaphores available",
#endif

#ifdef EISNAM
	[EISNAM] = "Is a named type file",
#endif

#ifdef EREMOTEIO
	[EREMOTEIO] = "Remote I/O error",
#endif

#ifdef ENOMEDIUM
	[ENOMEDIUM] = "No medium found",
#endif

#ifdef EMEDIUMTYPE
	[EMEDIUMTYPE] = "Wrong medium type",
#endif

#ifdef ENOKEY
	[ENOKEY] = "Required key not available",
#endif

#ifdef EKEYEXPIRED
	[EKEYEXPIRED] = "Key has expired",
#endif

#ifdef EKEYREVOKED
	[EKEYREVOKED] = "Key has been revoked",
#endif

#ifdef EKEYREJECTED
	[EKEYREJECTED] = "Key was rejected by service",
#endif

#ifdef ERFKILL
	[ERFKILL] = "Operation not possible due to RF-kill",
#endif

#ifdef EHWPOISON
	[EHWPOISON] = "Memory page has hardware error",
#endif

#ifdef EBADRPC
	[EBADRPC] = "RPC struct is bad",
#endif

#ifdef EFTYPE
	/*
	TRANS The file was the wrong type for the
	TRANS operation, or a data file had the wrong format.
	TRANS
	TRANS On some systems @code{chmod} returns this error if you try to set the
	TRANS sticky bit on a non-directory file; @pxref{Setting Permissions}. */
	[EFTYPE] = "Inappropriate file type or format",
#endif

#ifdef EPROCUNAVAIL
	[EPROCUNAVAIL] = "RPC bad procedure for program",
#endif

#ifdef EAUTH
	[EAUTH] = "Authentication error",
#endif

#ifdef EDIED
	/*
	TRANS On @gnuhurdsystems{}, opening a file returns this error when the file is
	TRANS translated by a program and the translator program dies while starting
	TRANS up, before it has connected to the file. */
	[EDIED] = "Translator died",
#endif

#ifdef ERPCMISMATCH
	[ERPCMISMATCH] = "RPC version wrong",
#endif

#ifdef EGREGIOUS
	/*
	TRANS You did @strong{what}? */
	[EGREGIOUS] = "You really blew it this time",
#endif

#ifdef EPROCLIM
	/*
	TRANS This means that the per-user limit on new process would be exceeded by
	TRANS an attempted @code{fork}.  @xref{Limits on Resources}, for details on
	TRANS the @code{RLIMIT_NPROC} limit. */
	[EPROCLIM] = "Too many processes",
#endif

#ifdef EGRATUITOUS
	/*
	TRANS This error code has no purpose. */
	[EGRATUITOUS] = "Gratuitous error",
#endif

#if defined(ENOTSUP) && ENOTSUP != EOPNOTSUPP
	/*
	TRANS A function returns this error when certain parameter
	TRANS values are valid, but the functionality they request is not available.
	TRANS This can mean that the function does not implement a particular command
	TRANS or option value or flag bit at all.  For functions that operate on some
	TRANS object given in a parameter, such as a file descriptor or a port, it
	TRANS might instead mean that only @emph{that specific object} (file
	TRANS descriptor, port, etc.) is unable to support the other parameters given;
	TRANS different file descriptors might support different ranges of parameter
	TRANS values.
	TRANS
	TRANS If the entire function is not available at all in the implementation,
	TRANS it returns @code{ENOSYS} instead. */
	[ENOTSUP] = "Not supported",
#endif

#ifdef EPROGMISMATCH
	[EPROGMISMATCH] = "RPC program version wrong",
#endif

#ifdef EBACKGROUND
	/*
	TRANS On @gnuhurdsystems{}, servers supporting the @code{term} protocol return
	TRANS this error for certain operations when the caller is not in the
	TRANS foreground process group of the terminal.  Users do not usually see this
	TRANS error because functions such as @code{read} and @code{write} translate
	TRANS it into a @code{SIGTTIN} or @code{SIGTTOU} signal.  @xref{Job Control},
	TRANS for information on process groups and these signals. */
	[EBACKGROUND] = "Inappropriate operation for background process",
#endif

#ifdef EIEIO
	/*
	TRANS Go home and have a glass of warm, dairy-fresh milk.
	TRANS @c Okay.  Since you are dying to know, I'll tell you.
	TRANS @c This is a joke, obviously.  There is a children's song which begins,
	TRANS @c "Old McDonald had a farm, e-i-e-i-o."  Every time I see the (real)
	TRANS @c errno macro EIO, I think about that song.  Probably most of my
	TRANS @c compatriots who program on Unix do, too.  One of them must have stayed
	TRANS @c up a little too late one night and decided to add it to Hurd or Glibc.
	TRANS @c Whoever did it should be castigated, but it made me laugh.
	TRANS @c  --jtobey@channel1.com
	TRANS @c
	TRANS @c "bought the farm" means "died".  -jtobey
	TRANS @c
	TRANS @c Translators, please do not translate this litteraly, translate it into
	TRANS @c an idiomatic funny way of saying that the computer died. */
	[EIEIO] = "Computer bought the farm",
#endif

#if defined(EWOULDBLOCK) && EWOULDBLOCK != EAGAIN
	/*
	TRANS In @theglibc{}, this is another name for @code{EAGAIN} (above).
	TRANS The values are always the same, on every operating system.
	TRANS
	TRANS C libraries in many older Unix systems have @code{EWOULDBLOCK} as a
	TRANS separate error code. */
	[EWOULDBLOCK] = "Operation would block",
#endif

#ifdef ENEEDAUTH
	[ENEEDAUTH] = "Need authenticator",
#endif

#ifdef ED
	/*
	TRANS The experienced user will know what is wrong.
	TRANS @c This error code is a joke.  Its perror text is part of the joke.
	TRANS @c Don't change it. */
	[ED] = "?",
#endif

#ifdef EPROGUNAVAIL
	[EPROGUNAVAIL] = "RPC program not available",
#endif
};