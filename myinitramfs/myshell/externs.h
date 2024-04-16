/* externs.h -- extern function declarations which do not appear in their
   own header file. */

/* Copyright (C) 1993-2020 Free Software Foundation, Inc.

   This file is part of GNU Bash, the Bourne Again SHell.

   Bash is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Bash is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Bash.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Make sure that this is included *after* config.h! */

#ifndef _EXTERNS_H_
#define _EXTERNS_H_

	extern int reader_loop(void);
   extern int read_command(void);

#endif /* _EXTERNS_H_ */