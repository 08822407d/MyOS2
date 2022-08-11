/* Copyright (C) 1991, 92, 93, 94, 95, 96 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/*
 *	ISO C Standard: 4.9 INPUT/OUTPUT	<stdio.h>
 */

#ifndef _STDIO_H

	#if !defined(__need_FILE)
		#define _STDIO_H 1
		#include <features.h>

		#define __need_size_t
		#define __need_NULL
		#include <stddef.h>

		#define __need___va_list
		#include <stdarg.h>
		#ifndef __GNUC_VA_LIST
			#define __gnuc_va_list __ptr_t
		#endif

		#include <gnu/types.h>
	#endif /* Don't need FILE.  */
	#undef __need_FILE

	#ifndef __FILE_defined
		/* The opaque type of streams.  */
		typedef struct __stdio_file FILE;

		#define __FILE_defined 1
	#endif /* FILE not defined.  */

	#ifdef _STDIO_H

		/* The type of the second argument to `fgetpos' and `fsetpos'.  */
		typedef __off_t fpos_t;

		/* The mode of I/O, as given in the MODE argument to fopen, etc.  */
		typedef struct
		{
			unsigned int __read : 1;	  /* Open for reading.  */
			unsigned int __write : 1;	  /* Open for writing.  */
			unsigned int __append : 1;	  /* Open for appending.  */
			unsigned int __binary : 1;	  /* Opened binary.  */
			unsigned int __create : 1;	  /* Create the file.  */
			unsigned int __exclusive : 1; /* Error if it already exists.  */
			unsigned int __truncate : 1;  /* Truncate the file on opening.  */
		} __io_mode;

	// 	/* Functions to do I/O and file management for a stream.  */

	// 	/* Read NBYTES bytes from COOKIE into a buffer pointed to by BUF.
	// 	Return number of bytes read.  */
	// 	typedef __ssize_t __io_read_fn(__ptr_t __cookie,
	// 					char *__buf, size_t __nbytes);

	// 	/* Write N bytes pointed to by BUF to COOKIE.  Write all N bytes
	// 	unless there is an error.  Return number of bytes written, or -1 if
	// 	there is an error without writing anything.  If the file has been
	// 	opened for append (__mode.__append set), then set the file pointer
	// 	to the end of the file and then do the write; if not, just write at
	// 	the current file pointer.  */
	// 	typedef __ssize_t __io_write_fn(__ptr_t __cookie,
	// 					__const char *__buf, size_t __n);

	// 	/* Move COOKIE's file position to *POS bytes from the
	// 	beginning of the file (if W is SEEK_SET),
	// 	the current position (if W is SEEK_CUR),
	// 	or the end of the file (if W is SEEK_END).
	// 	Set *POS to the new file position.
	// 	Returns zero if successful, nonzero if not.  */
	// 	typedef int __io_seek_fn(__ptr_t __cookie, fpos_t *__pos, int __w);

	// 	/* Close COOKIE.  */
	// 	typedef int __io_close_fn(__ptr_t __cookie);

	// 	/* Return the file descriptor associated with COOKIE,
	// 	or -1 on error.  There need not be any associated file descriptor.  */
	// 	typedef int __io_fileno_fn(__ptr_t __cookie);

	// 	#ifdef __USE_GNU
	// 		/* User-visible names for the above.  */
	// 		typedef __io_read_fn cookie_read_function_t;
	// 		typedef __io_write_fn cookie_write_function_t;
	// 		typedef __io_seek_fn cookie_seek_function_t;
	// 		typedef __io_close_fn cookie_close_function_t;
	// 		typedef __io_fileno_fn cookie_fileno_function_t;
	// 	#endif

	// 	/* Low level interface, independent of FILE representation.  */
	// 	#if defined(__USE_GNU) && !defined(_LIBC)
	// 		/* Define the user-visible type, with user-friendly member names.  */
	// 		typedef struct
	// 		{
	// 			__io_read_fn *read;		/* Read bytes.  */
	// 			__io_write_fn *write;	/* Write bytes.  */
	// 			__io_seek_fn *seek;		/* Seek/tell file position.  */
	// 			__io_close_fn *close;	/* Close file.  */
	// 			__io_fileno_fn *fileno; /* Return file descriptor.  */
	// 		} cookie_io_functions_t;
	// 		/* This name is still used in the prototypes in this file.  */
	// 		typedef cookie_io_functions_t __io_functions;
	// 	#else
	// 		/* Stick to ANSI-safe names.  */
	// 		typedef struct
	// 		{
	// 			__io_read_fn *__read;	  /* Read bytes.  */
	// 			__io_write_fn *__write;	  /* Write bytes.  */
	// 			__io_seek_fn *__seek;	  /* Seek/tell file position.  */
	// 			__io_close_fn *__close;	  /* Close file.  */
	// 			__io_fileno_fn *__fileno; /* Return file descriptor.  */
	// 		} __io_functions;
	// 	#endif

	// 	/* Higher level interface, dependent on FILE representation.  */
	// 	typedef struct
	// 	{
	// 		/* Make room in the input buffer.  */
	// 		int(*__input)(FILE * __stream);
	// 		/* Make room in the output buffer.  */
	// 		void(*__output)(FILE * __stream, int __c);
	// 	} __room_functions;

	// 	extern __const __io_functions __default_io_functions;
	// 	extern __const __room_functions __default_room_functions;

	// 	/* Default close function.  */
	// 	extern __io_close_fn __stdio_close;
	// 	/* Open FILE with mode M, store cookie in *COOKIEPTR.  */
	// 	extern int __stdio_open(__const char *__file, __io_mode __m,
	// 					__ptr_t *__cookieptr);
	// 	/* Put out an error message for when stdio needs to die.  */
	// 	extern void __stdio_errmsg(__const char *__msg, size_t __len);
	// 	/* Generate a unique file name (and possibly open it with mode "w+b").  */
	// 	extern char *__stdio_gen_tempname(char *__buf, size_t __bufsize,
	// 					__const char *__dir, __const char *__pfx,
	// 					int __dir_search, size_t *__lenptr, FILE **__streamptr);

	// 	/* Print out MESSAGE on the error output and abort.  */
	// 	extern void __libc_fatal(__const char *__message) __attribute__((__noreturn__));

	// 	/* For thread safe I/O functions we need a lock in each stream.  We
	// 	keep the type opaque here.  */
	// 	struct __stdio_lock;

		/* The FILE structure.  */
		struct __stdio_file
		{
			/* Magic number for validation.  Must be negative in open streams
			for the glue to Unix stdio getc/putc to work.
			NOTE: stdio/glue.c has special knowledge of these first four members.  */
			int __magic;
		#define _IOMAGIC ((int)0xfedabeeb)	 /* Magic number to fill `__magic'.  */
		#define _GLUEMAGIC ((int)0xfeedbabe) /* Magic for glued Unix streams.  */

			char *__bufp;	   /* Pointer into the buffer.  */
			char *__get_limit; /* Reading limit.  */
			char *__put_limit; /* Writing limit.  */

			char *__buffer;					   /* Base of buffer.  */
			size_t __bufsize;				   /* Size of the buffer.  */
			__ptr_t __cookie;				   /* Magic cookie.  */
			__io_mode __mode;				   /* File access mode.  */
			// __io_functions __io_funcs;		   /* I/O functions.  */
			// __room_functions __room_funcs;	   /* I/O buffer room functions.  */
			fpos_t __offset;				   /* Current file position.  */
			fpos_t __target;				   /* Target file position.  */
			FILE *__next;					   /* Next FILE in the linked list.  */
			char *__pushback_bufp;			   /* Old bufp if char pushed back.  */
			unsigned char __pushback;		   /* Pushed-back character.  */
			unsigned int __pushed_back : 1;	   /* A char has been pushed back.  */
			unsigned int __eof : 1;			   /* End of file encountered.  */
			unsigned int __error : 1;		   /* Error encountered.  */
			unsigned int __userbuf : 1;		   /* Buffer from user (should not be freed).  */
			unsigned int __linebuf : 1;		   /* Flush on newline.  */
			unsigned int __linebuf_active : 1; /* put_limit is not really in use.  */
			unsigned int __seen : 1;		   /* This stream has been seen.  */
			unsigned int __ispipe : 1;		   /* Nonzero if opened by popen.  */
			// struct __stdio_lock *__lock;	   /* Pointer to associated lock.  */
		};

	// 	/* All macros used internally by other macros here and by stdio functions begin
	// 	with `__'.  All of these may evaluate their arguments more than once.  */

	// 	/* Nonzero if STREAM is a valid stream.
	// 	STREAM must be a modifiable lvalue (wow, I got to use that term).
	// 	See stdio/glue.c for what the confusing bit is about.  */
	// 	#define __validfp(stream) \
	// 		(stream != NULL &&    \
	// 		({ if (stream->__magic == _GLUEMAGIC)				      \
	// 		stream = *((struct { int __magic; FILE **__p; } *) stream)->__p;      \
	// 		stream->__magic == _IOMAGIC; }))

	// 	/* Clear the error and EOF indicators of STREAM.  */
	// 	#define __clearerr(stream) ((stream)->__error = (stream)->__eof = 0)

	// 	/* Nuke STREAM, making it unusable but available for reuse.  */
	// 	extern void __invalidate(FILE * __stream);

	// 	/* Make sure STREAM->__offset and STREAM->__target are initialized.
	// 	Returns 0 if successful, or EOF on
	// 	error (but doesn't set STREAM->__error).  */
	// 	extern int __stdio_check_offset(FILE * __stream);

	// 	/* The possibilities for the third argument to `setvbuf'.  */
	// 	#define _IOFBF 0x1 /* Full buffering.  */
	// 	#define _IOLBF 0x2 /* Line buffering.  */
	// 	#define _IONBF 0x4 /* No buffering.  */

	// 	/* Default buffer size.  */
	// 	#define BUFSIZ 1024

	// 	/* End of file character.
	// 	Some things throughout the library rely on this being -1.  */
	// 	#define EOF (-1)

	// 	/* The possibilities for the third argument to `fseek'.
	// 	These values should not be changed.  */
	// 	#define SEEK_SET 0 /* Seek from beginning of file.  */
	// 	#define SEEK_CUR 1 /* Seek from current position.  */
	// 	#define SEEK_END 2 /* Seek from end of file.  */

	// 	#ifdef __USE_SVID
	// 		/* Default path prefix for `tempnam' and `tmpnam'.  */
	// 		#define P_tmpdir "/usr/tmp"
	// 	#endif

	// 	/* Get the values:
	// 	L_tmpnam	How long an array of chars must be to be passed to `tmpnam'.
	// 	TMP_MAX	The minimum number of unique filenames generated by tmpnam
	// 			(and tempnam when it uses tmpnam's name space),
	// 			or tempnam (the two are separate).
	// 	L_ctermid	How long an array to pass to `ctermid'.
	// 	L_cuserid	How long an array to pass to `cuserid'.
	// 	FOPEN_MAX	Minimum number of files that can be open at once.
	// 	FILENAME_MAX	Maximum length of a filename.  */
	// 	#include <stdio_lim.h>

	// 	/* All the known streams are in a linked list
	// 	linked by the `next' field of the FILE structure.  */
	// 	extern FILE *__stdio_head; /* Head of the list.  */

	// 	/* Standard streams.  */
	// 	extern FILE *stdin, *stdout, *stderr;
	// 	#ifdef __STRICT_ANSI__
	// 		/* ANSI says these are macros; satisfy pedants.  */
	// 		#define stdin stdin
	// 		#define stdout stdout
	// 		#define stderr stderr
	// 	#endif

	// 	/* Remove file FILENAME.  */
	// 	extern int remove(__const char *__filename);
	// 	/* Rename file OLD to NEW.  */
	// 	extern int rename(__const char *__old, __const char *__new);

	// 	/* Create a temporary file and open it read/write.  */
	// 	extern FILE *tmpfile(void);
	// 	/* Generate a temporary filename.  */
	// 	extern char *tmpnam(char *__s);

	// 	#ifdef __USE_REENTRANT
	// 		/* This is the reentrant variant of `tmpnam'.  The only difference is
	// 		that it does not allow S to be NULL.  */
	// 		extern char *tmpnam_r(char *__s);
	// 	#endif

	// 	#if defined(__USE_SVID) || defined(__USE_XOPEN)
	// 		/* Generate a unique temporary filename using up to five characters of PFX
	// 		if it is not NULL.  The directory to put this file in is searched for
	// 		as follows: First the environment variable "TMPDIR" is checked.
	// 		If it contains the name of a writable directory, that directory is used.
	// 		If not and if DIR is not NULL, that value is checked.  If that fails,
	// 		P_tmpdir is tried and finally "/tmp".  The storage for the filename
	// 		is allocated by `malloc'.  */
	// 		extern char *tempnam(__const char *__dir, __const char *__pfx);
	// 	#endif

	// 	/* This performs actual output when necessary, flushing
	// 	STREAM's buffer and optionally writing another character.  */
	// 	extern int __flshfp(FILE * __stream, int __c);

	// 	/* Close STREAM.  */
	// 	extern int fclose(FILE * __stream);
	// 	/* Flush STREAM, or all streams if STREAM is NULL.  */
	// 	extern int fflush(FILE * __stream);

	// 	#ifdef __USE_GNU
	// 		/* Close all streams.  */
	// 		extern int __fcloseall(void);
	// 		extern int fcloseall(void);
	// 	#endif

	// 	/* Open a file and create a new stream for it.  */
	// 	extern FILE *fopen(__const char *__filename, __const char *__modes);
	// 	/* Open a file, replacing an existing stream with it. */
	// 	extern FILE *freopen(__const char *__filename, __const char *__modes,
	// 					FILE *__stream);

	// 	/* Return a new, zeroed, stream.
	// 	You must set its cookie and io_mode.
	// 	The first operation will give it a buffer unless you do.
	// 	It will also give it the default functions unless you set the `seen' flag.
	// 	The offset is set to -1, meaning it will be determined by doing a
	// 	stationary seek.  You can set it to avoid the initial tell call.
	// 	The target is set to -1, meaning it will be set to the offset
	// 	before the target is needed.
	// 	Returns NULL if a stream can't be created.  */
	// 	extern FILE *__newstream(void);

	// 	#ifdef __USE_POSIX
	// 		/* Create a new stream that refers to an existing system file descriptor.  */
	// 		extern FILE *fdopen(int __fd, __const char *__modes);
	// 	#endif

	// 	#ifdef __USE_GNU
	// 		/* Create a new stream that refers to the given magic cookie,
	// 		and uses the given functions for input and output.  */
	// 		extern FILE *fopencookie(__ptr_t __magic_cookie,
	// 						__const char *__modes, __io_functions __io_funcs);

	// 		/* Create a new stream that refers to a memory buffer.  */
	// 		extern FILE *fmemopen(__ptr_t __s, size_t __len,
	// 						__const char *__modes);

	// 		/* Open a stream that writes into a malloc'd buffer that is expanded as
	// 		necessary.  *BUFLOC and *SIZELOC are updated with the buffer's location
	// 		and the number of characters written on fflush or fclose.  */
	// 		extern FILE *open_memstream(char **__bufloc, size_t *__sizeloc);
	// 	#endif

	// 	/* If BUF is NULL, make STREAM unbuffered.
	// 	Else make it use buffer BUF, of size BUFSIZ.  */
	// 	extern void setbuf(FILE * __stream, char *__buf);
	// 	/* Make STREAM use buffering mode MODE.
	// 	If BUF is not NULL, use N bytes of it for buffering;
	// 	else allocate an internal buffer N bytes long.  */
	// 	extern int setvbuf(FILE * __stream, char *__buf,
	// 					int __modes, size_t __n);

	// 	#ifdef __USE_BSD
	// 		/* If BUF is NULL, make STREAM unbuffered.
	// 		Else make it use SIZE bytes of BUF for buffering.  */
	// 		extern void setbuffer(FILE * __stream, char *__buf, size_t __size);

	// 		/* Make STREAM line-buffered.  */
	// 		extern void setlinebuf(FILE * __stream);
	// 	#endif

	// 	/* Write formatted output to STREAM.  */
	// 	extern int fprintf(FILE * __stream, __const char *__format, ...);
	// 	/* Write formatted output to stdout.  */
	// 	extern int printf(__const char *__format, ...);
	// 	/* Write formatted output to S.  */
	// 	extern int sprintf(char *__s, __const char *__format, ...);

	// 	/* Write formatted output to S from argument list ARG.  */
	// 	extern int vfprintf(FILE * __s, __const char *__format,
	// 					__gnuc_va_list __arg);
	// 	/* Write formatted output to stdout from argument list ARG.  */
	// 	extern int vprintf(__const char *__format, __gnuc_va_list __arg);
	// 	/* Write formatted output to S from argument list ARG.  */
	// 	extern int vsprintf(char *__s, __const char *__format,
	// 					__gnuc_va_list __arg);

	// 	#ifdef __OPTIMIZE__
	// 		extern __inline int
	// 		vprintf(const char *__fmt, __gnuc_va_list __arg)
	// 		{
	// 			return vfprintf(stdout, __fmt, __arg);
	// 		}
	// 	#endif /* Optimizing.  */

	// 	#ifdef __USE_GNU
	// 		/* Maximum chars of output to write in MAXLEN.  */
	// 		extern int __snprintf(char *__s, size_t __maxlen,
	// 						__const char *__format, ...);
	// 		extern int snprintf(char *__s, size_t __maxlen,
	// 						__const char *__format, ...);

	// 		extern int __vsnprintf(char *__s, size_t __maxlen,
	// 						__const char *__format, __gnuc_va_list __arg);
	// 		extern int vsnprintf(char *__s, size_t __maxlen,
	// 						__const char *__format, __gnuc_va_list __arg);

	// 		/* Write formatted output to a string dynamically allocated with `malloc'.
	// 		Store the address of the string in *PTR.  */
	// 		extern int vasprintf(char **__ptr, __const char *__f,
	// 						__gnuc_va_list __arg);
	// 		extern int asprintf(char **__ptr, __const char *__fmt, ...);

	// 		/* Write formatted output to a file descriptor.  */
	// 		extern int vdprintf(int __fd, __const char *__fmt,
	// 						__gnuc_va_list __arg);
	// 		extern int dprintf(int __fd, __const char *__fmt, ...);
	// 	#endif

	// 	/* Read formatted input from STREAM.  */
	// 	extern int fscanf(FILE * __stream, __const char *__format, ...);
	// 	/* Read formatted input from stdin.  */
	// 	extern int scanf(__const char *__format, ...);
	// 	/* Read formatted input from S.  */
	// 	extern int sscanf(__const char *__s, __const char *__format, ...);

	// 	#ifdef __USE_GNU
	// 		/* Read formatted input from S into argument list ARG.  */
	// 		extern int __vfscanf(FILE * __s, __const char *__format,
	// 						__gnuc_va_list __arg);
	// 		extern int vfscanf(FILE * __s, __const char *__format,
	// 						__gnuc_va_list __arg);

	// 		/* Read formatted input from stdin into argument list ARG.  */
	// 		extern int __vscanf(__const char *__format, __gnuc_va_list __arg);
	// 		extern int vscanf(__const char *__format, __gnuc_va_list __arg);

	// 		/* Read formatted input from S into argument list ARG.  */
	// 		extern int __vsscanf(__const char *__s, __const char *__format,
	// 						__gnuc_va_list __arg);
	// 		extern int vsscanf(__const char *__s, __const char *__format,
	// 						__gnuc_va_list __arg);

	// 		#ifdef __OPTIMIZE__
	// 			extern __inline int
	// 			vfscanf(FILE *__s, const char *__fmt, __gnuc_va_list __arg)
	// 			{
	// 				return __vfscanf(__s, __fmt, __arg);
	// 			}
	// 			extern __inline int
	// 			vscanf(const char *__fmt, __gnuc_va_list __arg)
	// 			{
	// 				return __vfscanf(stdin, __fmt, __arg);
	// 			}
	// 			extern __inline int
	// 			vsscanf(const char *__s, const char *__fmt, __gnuc_va_list __arg)
	// 			{
	// 				return __vsscanf(__s, __fmt, __arg);
	// 			}
	// 		#endif /* Optimizing.  */
	// 	#endif /* Use GNU.  */

	// 	/* This does actual reading when necessary, filling STREAM's
	// 	buffer and returning the first character in it.  */
	// 	extern int __fillbf(FILE * __stream);

	// 	/* Read a character from STREAM.  */
	// 	extern int fgetc(FILE * __stream);
	// 	extern int getc(FILE * __stream);

	// 	/* Read a character from stdin.  */
	// 	extern int getchar(void);

	// 	/* The C standard explicitly says this can
	// 	re-evaluate its argument, so it does. */
	// 	#define __getc(stream) \
	// 		((stream)->__bufp < (stream)->__get_limit ? \
	// 		(int)((unsigned char)*(stream)->__bufp++) : \
	// 		__fillbf(stream))

	// 	/* The C standard explicitly says this is a macro,
	// 	so we always do the optimization for it.  */
	// 	#define getc(stream) __getc(stream)

	// 	#ifdef __OPTIMIZE__
	// 		extern __inline int
	// 		getchar(void)
	// 		{
	// 			return __getc(stdin);
	// 		}
	// 	#endif /* Optimizing.  */

	// 	/* Write a character to STREAM.  */
	// 	extern int fputc(int __c, FILE *__stream);
	// 	extern int putc(int __c, FILE *__stream);

	// 	/* Write a character to stdout.  */
	// 	extern int putchar(int __c);

	// 	/* The C standard explicitly says this can
	// 	re-evaluate its arguments, so it does.  */
	// 	#define __putc(c, stream) \
	// 		((stream)->__bufp < (stream)->__put_limit ? \
	// 		(int)(unsigned char)(*(stream)->__bufp++ = (unsigned char)(c)) : \
	// 		__flshfp((stream), (unsigned char)(c)))

	// 	/* The C standard explicitly says this can be a macro,
	// 	so we always do the optimization for it.  */
	// 	#define putc(c, stream) __putc((c), (stream))

	// 	#ifdef __OPTIMIZE__
	// 		extern __inline int
	// 		putchar(int __c)
	// 		{
	// 			return __putc(__c, stdout);
	// 		}
	// 	#endif

	// 	#if defined(__USE_SVID) || defined(__USE_MISC)
	// 		/* Get a word (int) from STREAM.  */
	// 		extern int getw(FILE * __stream);

	// 		/* Write a word (int) to STREAM.  */
	// 		extern int putw(int __w, FILE *__stream);
	// 	#endif

	// 	/* Get a newline-terminated string of finite length from STREAM.  */
	// 	extern char *fgets(char *__s, int __n, FILE *__stream);

	// 	/* Get a newline-terminated string from stdin, removing the newline.
	// 	DO NOT USE THIS FUNCTION!!  There is no limit on how much it will read.  */
	// 	extern char *gets(char *__s);

	// 	#ifdef __USE_GNU
	// 		#include <sys/types.h>

	// 		/* Read up to (and including) a DELIMITER from STREAM into *LINEPTR
	// 		(and null-terminate it). *LINEPTR is a pointer returned from malloc (or
	// 		NULL), pointing to *N characters of space.  It is realloc'd as
	// 		necessary.  Returns the number of characters read (not including the
	// 		null terminator), or -1 on error or EOF.  */
	// 		ssize_t __getdelim(char **__lineptr, size_t *__n,
	// 						int __delimiter, FILE *__stream);
	// 		ssize_t getdelim(char **__lineptr, size_t *__n,
	// 						int __delimiter, FILE *__stream);

	// 		/* Like `getdelim', but reads up to a newline.  */
	// 		ssize_t __getline(char **__lineptr, size_t *__n, FILE *__stream);
	// 		ssize_t getline(char **__lineptr, size_t *__n, FILE *__stream);

	// 		#ifdef __OPTIMIZE__
	// 			extern __inline ssize_t
	// 			__getline(char **__lineptr, size_t *__n, FILE *__stream)
	// 			{
	// 				return __getdelim(__lineptr, __n, '\n', __stream);
	// 			}

	// 			extern __inline ssize_t
	// 			getdelim(char **__lineptr, size_t *__n, int __delimiter, FILE *__stream)
	// 			{
	// 				return __getdelim(__lineptr, __n, __delimiter, __stream);
	// 			}
	// 			extern __inline ssize_t
	// 			getline(char **__lineptr, size_t *__n, FILE *__stream)
	// 			{
	// 				return __getline(__lineptr, __n, __stream);
	// 			}
	// 		#endif /* Optimizing.  */
	// 	#endif

	// 	/* Write a string to STREAM.  */
	// 	extern int fputs(__const char *__s, FILE *__stream);
	// 	/* Write a string, followed by a newline, to stdout.  */
	// 	extern int puts(__const char *__s);

	// 	/* Push a character back onto the input buffer of STREAM.  */
	// 	extern int ungetc(int __c, FILE *__stream);

	// 	/* Read chunks of generic data from STREAM.  */
	// 	extern size_t fread(__ptr_t __ptr, size_t __size,
	// 					size_t __n, FILE *__stream);
	// 	/* Write chunks of generic data to STREAM.  */
	// 	extern size_t fwrite(__const __ptr_t __ptr,
	// 					size_t __size, size_t __n, FILE *__s);

	// 	/* Seek to a certain position on STREAM.  */
	// 	extern int fseek(FILE * __stream, long int __off, int __whence);
	// 	/* Return the current position of STREAM.  */
	// 	extern long int ftell(FILE * __stream);
	// 	/* Rewind to the beginning of STREAM.  */
	// 	extern void rewind(FILE * __stream);

	// 	/* Get STREAM's position.  */
	// 	extern int fgetpos(FILE * __stream, fpos_t *__pos);
	// 	/* Set STREAM's position.  */
	// 	extern int fsetpos(FILE * __stream, __const fpos_t *__pos);

	// 	/* Clear the error and EOF indicators for STREAM.  */
	// 	extern void clearerr(FILE * __stream);
	// 	/* Return the EOF indicator for STREAM.  */
	// 	extern int feof(FILE * __stream);
	// 	/* Return the error indicator for STREAM.  */
	// 	extern int ferror(FILE * __stream);

	// 	#ifdef __OPTIMIZE__
	// 		#define feof(stream) ((stream)->__eof != 0)
	// 		#define ferror(stream) ((stream)->__error != 0)
	// 	#endif /* Optimizing.  */

	// 	/* Print a message describing the meaning of the value of errno.  */
	// 	extern void perror(__const char *__s);

	// 	#ifdef __USE_BSD
	// 		extern int sys_nerr;
	// 		extern const char *const sys_errlist[];
	// 	#endif
		#ifdef __USE_GNU
			extern int _sys_nerr;
			extern const char *const _sys_errlist[];
		#endif

	// 	#ifdef __USE_POSIX
	// 		/* Return the system file descriptor for STREAM.  */
	// 		extern int fileno(FILE * __stream);
	// 	#endif /* Use POSIX.  */

	// 	#if (defined(__USE_POSIX2) || defined(__USE_SVID) || \
	// 		defined(__USE_BSD) || defined(__USE_MISC))
	// 		/* Create a new stream connected to a pipe running the given command.  */
	// 		extern FILE *popen(__const char *__command, __const char *__modes);

	// 		/* Close a stream opened by popen and return the status of its child.  */
	// 		extern int pclose(FILE * __stream);
	// 	#endif

	// 	#ifdef __USE_POSIX
	// 		/* Return the name of the controlling terminal.  */
	// 		extern char *ctermid(char *__s);
	// 	#endif

	// 	#ifdef __USE_XOPEN
	// 		/* Return the name of the current user.  */
	// 		extern char *cuserid(char *__s);
	// 	#endif

	// 	#ifdef __USE_GNU
	// 		struct obstack; /* See <obstack.h>.  */

	// 		/* Open a stream that writes to OBSTACK.  */
	// 		extern FILE *open_obstack_stream(struct obstack * __obstack);

	// 		/* Write formatted output to an obstack.  */
	// 		extern int obstack_printf(struct obstack * __obstack,
	// 						__const char *__format, ...);
	// 		extern int obstack_vprintf(struct obstack * __obstack,
	// 						__const char *__format, __gnuc_va_list __args);
	// 	#endif

	#endif /* <stdio.h> included.  */

#endif /* stdio.h  */
