/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* File with mounted filesystems */
#define ALT_MTAB "/proc/mounts"

/* Check rights to query / set quotas before calling quotactl */
#define BSD_BEHAVIOUR 1

/* Configuration options */
#define COMPILE_OPTS " USE_LDAP_MAIL_LOOKUP EXT2_DIRECT HOSTS_ACCESS RPC BSD_BEHAVIOUR"

/* Define to 1 if translation of program messages to the user's native
   language is requested. */
#define ENABLE_NLS 1

/* Scanning of ext? filesystems using e2fslib */
#define EXT2_DIRECT 1

/* Define if the GNU dcgettext() function is already present or preinstalled.
   */
#define HAVE_DCGETTEXT 1

/* Define if the GNU gettext() function is already present or preinstalled. */
#define HAVE_GETTEXT 1

/* Define if you have the iconv() function. */
/* #undef HAVE_ICONV */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Use nl_langinfo for querying locale */
#define HAVE_NL_LANGINFO 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Use hosts.allow and hosts.deny for access checking of rpc.rquotad */
#define HOSTS_ACCESS 1

/* Name of package */
#define PACKAGE "quota"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "jack@suse.cz"

/* Locale-specific data directory */
#define PACKAGE_LOCALE_DIR "/usr/local/share/locale"

/* Define to the full name of this package. */
#define PACKAGE_NAME "quota"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "quota 4.06"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "quota"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "4.06"

/* Directory for PID files */
#define PID_DIR "/var/run"

/* Support for RPC */
#define RPC 1

/* Allow setting of quotas over RPC */
/* #undef RPC_SETQUOTA */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Use gettext to translate messages */
#define USE_GETTEXT 1

/* Lookup email address using LDAP */
#define USE_LDAP_MAIL_LOOKUP 1

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif


/* Version number of package */
#define VERSION "4.06"

/* Assume quota mount options for root filesystem */
/* #undef XFS_ROOTHACK */

/* Enable large inode numbers on Mac OS X 10.5.  */
#ifndef _DARWIN_USE_64_BIT_INODE
# define _DARWIN_USE_64_BIT_INODE 1
#endif

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for `stat' and other things to work. */
/* #undef _POSIX_SOURCE */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif
