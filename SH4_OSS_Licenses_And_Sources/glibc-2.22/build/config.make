# config.make.  Generated from config.make.in by configure.
# Don't edit this file.  Put configuration parameters in configparms instead.

version = 2.22
release = stable

# Installation prefixes.
install_root = $(DESTDIR)
prefix = /usr
exec_prefix = ${prefix}
datadir = ${datarootdir}
libdir = ${exec_prefix}/lib
slibdir = /lib
rtlddir = /lib
localedir = ${exec_prefix}/lib/locale
sysconfdir = /etc
libexecdir = ${exec_prefix}/libexec
rootsbindir = /sbin
infodir = ${datarootdir}/info
includedir = ${prefix}/include
datarootdir = ${prefix}/share
localstatedir = /var

# Should we use and build ldconfig?
use-ldconfig = yes

# Maybe the `ldd' script must be rewritten.
ldd-rewrite-script = no

# System configuration.
config-machine = sh4a
base-machine = sh
config-vendor = buildroot
config-os = linux-gnu
config-sysdirs =  sysdeps/unix/sysv/linux/sh/sh4 sysdeps/unix/sysv/linux/sh sysdeps/sh/nptl sysdeps/unix/sysv/linux sysdeps/nptl sysdeps/pthread sysdeps/gnu sysdeps/unix/inet sysdeps/unix/sysv sysdeps/unix/sh sysdeps/unix sysdeps/posix sysdeps/sh/sh4/fpu sysdeps/sh/sh4 sysdeps/sh sysdeps/sh/soft-fp sysdeps/wordsize-32 sysdeps/ieee754/flt-32 sysdeps/ieee754/dbl-64 sysdeps/ieee754 sysdeps/generic
cflags-cpu = 
asflags-cpu = 

config-extra-cflags = 
config-extra-cppflags = 
config-cflags-nofma = -ffp-contract=off

defines = 
sysheaders = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/sh4a-buildroot-linux-gnu/sysroot/usr/include
sysincludes = -nostdinc -isystem /home/mschreiber/Project/buildroot-2016.05/output/host/usr/lib/gcc/sh4a-buildroot-linux-gnu/4.9.3/include -isystem /home/mschreiber/Project/buildroot-2016.05/output/host/usr/lib/gcc/sh4a-buildroot-linux-gnu/4.9.3/include-fixed -isystem /home/mschreiber/Project/buildroot-2016.05/output/host/usr/sh4a-buildroot-linux-gnu/sysroot/usr/include
c++-sysincludes = 
all-warnings = 
enable-werror = yes

have-z-combreloc = yes
have-z-execstack = yes
have-Bgroup = yes
have-protected-data = yes
with-fp = yes
old-glibc-headers = no
unwind-find-fde = yes
have-forced-unwind = yes
have-fpie = yes
gnu89-inline-CFLAGS = -fgnu89-inline
have-ssp = no
have-selinux = no
have-libaudit = 
have-libcap = 
have-cc-with-libunwind = no
fno-unit-at-a-time = -fno-toplevel-reorder -fno-section-anchors
bind-now = no
have-hash-style = yes
use-default-link = no
output-format = elf32-sh-linux

static-libgcc = -static-libgcc

exceptions = -fexceptions
multi-arch = no

mach-interface-list = 

sizeof-long-double = 8

nss-crypt = no

# Configuration options.
build-shared = yes
build-pic-default= no
build-pie-default= no
build-profile = no
build-static-nss = no
add-ons = libidn
add-on-subdirs =  libidn
sysdeps-add-ons = 
cross-compiling = yes
force-install = yes
link-obsolete-rpc = yes
build-nscd = yes
use-nscd = yes
build-hardcoded-path-in-tests= no
build-pt-chown = no
enable-lock-elision = no

# Build tools.
CC = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/bin/sh4a-buildroot-linux-gnu-gcc
CXX = 
BUILD_CC = gcc
CFLAGS = -O2 
CPPFLAGS-config = 
CPPUNDEFS = 
ASFLAGS-config =  -Wa,--noexecstack
AR = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/lib/gcc/sh4a-buildroot-linux-gnu/4.9.3/../../../../sh4a-buildroot-linux-gnu/bin/ar
NM = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/bin/sh4a-buildroot-linux-gnu-nm
MAKEINFO = :
AS = $(CC) -c
BISON = no
AUTOCONF = no
OBJDUMP = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/lib/gcc/sh4a-buildroot-linux-gnu/4.9.3/../../../../sh4a-buildroot-linux-gnu/bin/objdump
OBJCOPY = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/lib/gcc/sh4a-buildroot-linux-gnu/4.9.3/../../../../sh4a-buildroot-linux-gnu/bin/objcopy
READELF = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/bin/sh4a-buildroot-linux-gnu-readelf

# Installation tools.
INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_INFO = /usr/bin/install-info
LN_S = ln -s
MSGFMT = msgfmt

# Script execution tools.
BASH = /bin/bash
AWK = gawk
PERL = /usr/bin/perl

# Additional libraries.
LIBGD = no

# Package versions and bug reporting configuration.
PKGVERSION = (Buildroot) 
PKGVERSION_TEXI = (Buildroot) 
REPORT_BUGS_TO = <http://www.gnu.org/software/libc/bugs.html>
REPORT_BUGS_TEXI = @uref{http://www.gnu.org/software/libc/bugs.html}

# More variables may be inserted below by configure.

override stddef.h = # The installed <stddef.h> seems to be libc-friendly.
build-mathvec = no
