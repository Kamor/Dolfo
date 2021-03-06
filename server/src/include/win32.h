/*
    Daimonin, the Massive Multiuser Online Role Playing Game
    Server Applicatiom

    Copyright (C) 2001 Michael Toennies

    A split from Crossfire, a Multiplayer game for X-windows.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    The author can be reached via e-mail to info@daimonin.org
*/
/*
 * Structures and types used to implement opendir/readdir/closedir
 * on Windows 95/NT and set the loe level defines
 */

#ifndef __WIN32_H
#define __WIN32_H

#if !defined(AFX_STDAFX_H__31666CA1_2474_11D5_AE6C_F07569C10000__INCLUDED_)
#define AFX_STDAFX_H__31666CA1_2474_11D5_AE6C_F07569C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif /* _MSC_VER > 1000 */

#endif

#define WIN32_LEAN_AND_MEAN
/* includes for VC - plz add other include settings for different compiler
 * when needed and comment it
 */
#include <winsock2.h>
#include <time.h>
#include <direct.h>
#include <math.h>
#include <sys/stat.h>   /* odd: but you don't get stat here with __STDC__ */
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <process.h>

/* FIXME: This appears to be for the benefit of MSVC. I think for various
 * 'reasons' many ANSI and unix-equivalent identifiers are prefixed with _
 * under (at least some versions of) VC. The non-_ identifier may or may not
 * exist too. But that wasn't even consistent usage in the server code. So I'm
 * not entirely sure of the reasoning but whatever, we define non-_ identifiers
 * here, when they don't exist already and then use these proper names in the
 * source.
 *
 * -- Smacky 20160803 */
#ifndef fstat
#   define fstat(__a, __b) _fstat(__a, __b)
#endif

#ifndef stat
#   define stat _stat
#endif

#ifndef isatty
#    define isatty(__a) _isatty(__a)
#endif

#ifndef fileno
#    define fileno(__a) _fileno(__a)
#endif

#if ! defined inline
#   define inline __inline
#endif

#if ! defined unlink
#   define unlink(__a) _unlink(__a)
#endif

#if ! defined mkdir
#   define mkdir(__a, __b) _mkdir(__a)
#endif

#if ! defined getpid
#   define getpid() _getpid()
#endif

//#if ! defined popen
//#   define popen(__a, __b) _popen(__a, __b)
//#endif
//
//#if ! defined pclose
//#   define pclose(__a) _pclose(__a)
//#endif

#if ! defined strdup
#   define strdup(__a) _strdup(__a)
#endif

#if ! defined stricmp
#   define stricmp(__a, __b) _stricmp(__a, __b)
#endif

#if ! defined strnicmp
#   define strnicmp(__a, __b, __c) _strnicmp(__a, __b, __c)
#endif

#if ! defined getcwd
#   define getcwd(__a, __b) _getcwd(__a, __b)
#endif

#if ! defined chdir
#   define chdir(__a) _chdir(__a)
#endif

#if ! defined access
#   define access(__a, __b) _access(__a, __b)
#endif

#if ! defined R_OK
#   define R_OK 6
#endif

#if ! defined F_OK
#   define F_OK 6
#endif

#if ! defined chmod
#   define chmod(__a, __b) _chmod(__a, __b)
#endif

#if ! defined umask
#   define umask(__a) _umask(__a)
#endif

#if ! defined snprintf
#   define snprintf _snprintf
#endif

#if ! defined strcasecmp
#   define strcasecmp(__a, __b) _stricmp(__a, __b)
#endif

#if ! defined strncasecmp
#   define strncasecmp(__a, __b, __c) _strnicmp(__a, __b, __c)
#endif

#if ! defined atoll
#   define atoll  _atoi64
#endif

#ifndef MINGW
#define __STDC__ 1      /* odd too: CF want this, but don't include it */
/* before the standard includes */
#endif

#ifndef HAVE_FCNTL_H
#define HAVE_FCNTL_H
#endif
#ifndef HAVE_STDDEF_H
#define HAVE_STDDEF_H
#endif
#define GETTIMEOFDAY_TWO_ARGS
#define MAXPATHLEN 256

#ifndef MSG_DONTWAIT
#define MSG_DONTWAIT 0
#endif

#define PREFIXDIR ""

#ifndef MINGW
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#define S_ISREG(x) (((x) & S_IFMT) == S_IFREG)
#endif

#ifndef S_ISGID
#define S_ISGID 0002000
#endif
#ifndef S_IWOTH
#define S_IWOTH 0000200
#endif
#ifndef S_IWGRP
#define S_IWGRP 0000020
#endif
#ifndef S_IWUSR
#define S_IWUSR 0000002
#endif
#ifndef S_IROTH
#define S_IROTH 0000400
#endif
#ifndef S_IRGRP
#define S_IRGRP 0000040
#endif
#ifndef S_IRUSR
#define S_IRUSR 0000004
#endif

#define YY_NEVER_INTERACTIVE 1

/* struct dirent - same as Unix */
typedef struct dirent
{
    long            d_ino;                  /* inode (always 1 in WIN32) */
    off_t           d_off;              /* offset to this dirent */
    unsigned short  d_reclen;   /* length of d_name */
    char            d_name[_MAX_FNAME + 1]; /* filename (null terminated) */
}dirent;

#define NAMLEN(dirent) strlen((dirent)->d_name)

/* typedef DIR - not the same as Unix */
typedef struct
{
    long                    handle;             /* _findfirst/_findnext handle */
    short                   offset;             /* offset into directory */
    short                   finished;               /* 1 if there are not more files */
    struct _finddata_t      fileinfo;   /* from _findfirst/_findnext */
    char                   *dir;                    /* the dir we are reading */
    struct dirent           dent;           /* the dirent to return */
} DIR;

#ifndef _TIMEZONE_DEFINED /* also in sys/time.h */
#define _TIMEZONE_DEFINED
struct timezone {
  int tz_minuteswest;
  int tz_dsttime;
};
#endif /* _TIMEZONE_DEFINED */

/* Function prototypes */
extern int              gettimeofday(struct timeval *time_Info, struct timezone *timezone_Info);
extern DIR             *opendir(const char *);
extern struct dirent   *readdir(DIR *);
extern int              closedir(DIR *);
extern void             rewinddir(DIR *);

#endif /* ifndef __WIN32_H */
