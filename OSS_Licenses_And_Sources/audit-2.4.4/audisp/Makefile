# Makefile.in generated by automake 1.15 from Makefile.am.
# audisp/Makefile.  Generated from Makefile.in by configure.

# Copyright (C) 1994-2014 Free Software Foundation, Inc.

# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



# Makefile.am-- 
# Copyright 2007,2011,2015 Red Hat Inc., Durham, North Carolina.
# All Rights Reserved.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# Authors:
#   Steve Grubb <sgrubb@redhat.com>
#



am__is_gnu_make = { \
  if test -z '$(MAKELEVEL)'; then \
    false; \
  elif test -n '$(MAKE_HOST)'; then \
    true; \
  elif test -n '$(MAKE_VERSION)' && test -n '$(CURDIR)'; then \
    true; \
  else \
    false; \
  fi; \
}
am__make_running_with_option = \
  case $${target_option-} in \
      ?) ;; \
      *) echo "am__make_running_with_option: internal error: invalid" \
              "target option '$${target_option-}' specified" >&2; \
         exit 1;; \
  esac; \
  has_opt=no; \
  sane_makeflags=$$MAKEFLAGS; \
  if $(am__is_gnu_make); then \
    sane_makeflags=$$MFLAGS; \
  else \
    case $$MAKEFLAGS in \
      *\\[\ \	]*) \
        bs=\\; \
        sane_makeflags=`printf '%s\n' "$$MAKEFLAGS" \
          | sed "s/$$bs$$bs[$$bs $$bs	]*//g"`;; \
    esac; \
  fi; \
  skip_next=no; \
  strip_trailopt () \
  { \
    flg=`printf '%s\n' "$$flg" | sed "s/$$1.*$$//"`; \
  }; \
  for flg in $$sane_makeflags; do \
    test $$skip_next = yes && { skip_next=no; continue; }; \
    case $$flg in \
      *=*|--*) continue;; \
        -*I) strip_trailopt 'I'; skip_next=yes;; \
      -*I?*) strip_trailopt 'I';; \
        -*O) strip_trailopt 'O'; skip_next=yes;; \
      -*O?*) strip_trailopt 'O';; \
        -*l) strip_trailopt 'l'; skip_next=yes;; \
      -*l?*) strip_trailopt 'l';; \
      -[dEDm]) skip_next=yes;; \
      -[JT]) skip_next=yes;; \
    esac; \
    case $$flg in \
      *$$target_option*) has_opt=yes; break;; \
    esac; \
  done; \
  test $$has_opt = yes
am__make_dryrun = (target_option=n; $(am__make_running_with_option))
am__make_keepgoing = (target_option=k; $(am__make_running_with_option))
pkgdatadir = $(datadir)/audit
pkgincludedir = $(includedir)/audit
pkglibdir = $(libdir)/audit
pkglibexecdir = $(libexecdir)/audit
am__cd = CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)" && cd
install_sh_DATA = $(install_sh) -c -m 644
install_sh_PROGRAM = $(install_sh) -c
install_sh_SCRIPT = $(install_sh) -c
INSTALL_HEADER = $(INSTALL_DATA)
transform = $(program_transform_name)
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_triplet = i686-pc-linux-gnu
host_triplet = sh4a-buildroot-linux-gnu
target_triplet = sh4a-buildroot-linux-gnu
sbin_PROGRAMS = audispd$(EXEEXT)
subdir = audisp
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
am__aclocal_m4_deps = $(top_srcdir)/m4/ax_prog_cc_for_build.m4 \
	$(top_srcdir)/m4/cap-ng.m4 $(top_srcdir)/m4/libtool.m4 \
	$(top_srcdir)/m4/ltoptions.m4 $(top_srcdir)/m4/ltsugar.m4 \
	$(top_srcdir)/m4/ltversion.m4 $(top_srcdir)/m4/lt~obsolete.m4 \
	$(top_srcdir)/src/libev/libev.m4 $(top_srcdir)/configure.ac
am__configure_deps = $(am__aclocal_m4_deps) $(CONFIGURE_DEPENDENCIES) \
	$(ACLOCAL_M4)
DIST_COMMON = $(srcdir)/Makefile.am $(noinst_HEADERS) \
	$(am__DIST_COMMON)
mkinstalldirs = $(install_sh) -d
CONFIG_HEADER = $(top_builddir)/config.h
CONFIG_CLEAN_VPATH_FILES =
am__installdirs = "$(DESTDIR)$(sbindir)"
PROGRAMS = $(sbin_PROGRAMS)
am_audispd_OBJECTS = audispd-audispd.$(OBJEXT) \
	audispd-audispd-config.$(OBJEXT) \
	audispd-audispd-pconfig.$(OBJEXT) \
	audispd-audispd-llist.$(OBJEXT) audispd-queue.$(OBJEXT) \
	audispd-audispd-builtins.$(OBJEXT)
audispd_OBJECTS = $(am_audispd_OBJECTS)
audispd_LDADD = $(LDADD)
audispd_DEPENDENCIES =
AM_V_lt = $(am__v_lt_$(V))
am__v_lt_ = $(am__v_lt_$(AM_DEFAULT_VERBOSITY))
am__v_lt_0 = --silent
am__v_lt_1 = 
audispd_LINK = $(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=link $(CCLD) $(audispd_CFLAGS) \
	$(CFLAGS) $(audispd_LDFLAGS) $(LDFLAGS) -o $@
AM_V_P = $(am__v_P_$(V))
am__v_P_ = $(am__v_P_$(AM_DEFAULT_VERBOSITY))
am__v_P_0 = false
am__v_P_1 = :
AM_V_GEN = $(am__v_GEN_$(V))
am__v_GEN_ = $(am__v_GEN_$(AM_DEFAULT_VERBOSITY))
am__v_GEN_0 = @echo "  GEN     " $@;
am__v_GEN_1 = 
AM_V_at = $(am__v_at_$(V))
am__v_at_ = $(am__v_at_$(AM_DEFAULT_VERBOSITY))
am__v_at_0 = @
am__v_at_1 = 
DEFAULT_INCLUDES = -I. -I$(top_builddir)
depcomp =
am__depfiles_maybe =
COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
LTCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=compile $(CC) $(DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
	$(AM_CFLAGS) $(CFLAGS)
AM_V_CC = $(am__v_CC_$(V))
am__v_CC_ = $(am__v_CC_$(AM_DEFAULT_VERBOSITY))
am__v_CC_0 = @echo "  CC      " $@;
am__v_CC_1 = 
CCLD = $(CC)
LINK = $(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) \
	$(AM_LDFLAGS) $(LDFLAGS) -o $@
AM_V_CCLD = $(am__v_CCLD_$(V))
am__v_CCLD_ = $(am__v_CCLD_$(AM_DEFAULT_VERBOSITY))
am__v_CCLD_0 = @echo "  CCLD    " $@;
am__v_CCLD_1 = 
SOURCES = $(audispd_SOURCES)
DIST_SOURCES = $(audispd_SOURCES)
RECURSIVE_TARGETS = all-recursive check-recursive cscopelist-recursive \
	ctags-recursive dvi-recursive html-recursive info-recursive \
	install-data-recursive install-dvi-recursive \
	install-exec-recursive install-html-recursive \
	install-info-recursive install-pdf-recursive \
	install-ps-recursive install-recursive installcheck-recursive \
	installdirs-recursive pdf-recursive ps-recursive \
	tags-recursive uninstall-recursive
am__can_run_installinfo = \
  case $$AM_UPDATE_INFO_DIR in \
    n|no|NO) false;; \
    *) (install-info --version) >/dev/null 2>&1;; \
  esac
HEADERS = $(noinst_HEADERS)
RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\
  distclean-recursive maintainer-clean-recursive
am__recursive_targets = \
  $(RECURSIVE_TARGETS) \
  $(RECURSIVE_CLEAN_TARGETS) \
  $(am__extra_recursive_targets)
AM_RECURSIVE_TARGETS = $(am__recursive_targets:-recursive=) TAGS CTAGS \
	distdir
am__tagged_files = $(HEADERS) $(SOURCES) $(TAGS_FILES) $(LISP)
# Read a list of newline-separated strings from the standard input,
# and print each of them once, without duplicates.  Input order is
# *not* preserved.
am__uniquify_input = $(AWK) '\
  BEGIN { nonempty = 0; } \
  { items[$$0] = 1; nonempty = 1; } \
  END { if (nonempty) { for (i in items) print i; }; } \
'
# Make sure the list of sources is unique.  This is necessary because,
# e.g., the same source file might be shared among _SOURCES variables
# for different programs/libraries.
am__define_uniq_tagged_files = \
  list='$(am__tagged_files)'; \
  unique=`for i in $$list; do \
    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
  done | $(am__uniquify_input)`
ETAGS = etags
CTAGS = ctags
DIST_SUBDIRS = $(SUBDIRS)
am__DIST_COMMON = $(srcdir)/Makefile.in
DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
am__relativize = \
  dir0=`pwd`; \
  sed_first='s,^\([^/]*\)/.*$$,\1,'; \
  sed_rest='s,^[^/]*/*,,'; \
  sed_last='s,^.*/\([^/]*\)$$,\1,'; \
  sed_butlast='s,/*[^/]*$$,,'; \
  while test -n "$$dir1"; do \
    first=`echo "$$dir1" | sed -e "$$sed_first"`; \
    if test "$$first" != "."; then \
      if test "$$first" = ".."; then \
        dir2=`echo "$$dir0" | sed -e "$$sed_last"`/"$$dir2"; \
        dir0=`echo "$$dir0" | sed -e "$$sed_butlast"`; \
      else \
        first2=`echo "$$dir2" | sed -e "$$sed_first"`; \
        if test "$$first2" = "$$first"; then \
          dir2=`echo "$$dir2" | sed -e "$$sed_rest"`; \
        else \
          dir2="../$$dir2"; \
        fi; \
        dir0="$$dir0"/"$$first"; \
      fi; \
    fi; \
    dir1=`echo "$$dir1" | sed -e "$$sed_rest"`; \
  done; \
  reldir="$$dir2"
ACLOCAL = ${SHELL} /home/mschreiber/Project/buildroot-2016.05/output/build/audit-2.4.4/missing aclocal-1.15
AMTAR = $${TAR-tar}
AM_DEFAULT_VERBOSITY = 1
AR = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/bin/sh4a-buildroot-linux-gnu-ar
AUTOCONF = ${SHELL} /home/mschreiber/Project/buildroot-2016.05/output/build/audit-2.4.4/missing autoconf
AUTOHEADER = ${SHELL} /home/mschreiber/Project/buildroot-2016.05/output/build/audit-2.4.4/missing autoheader
AUTOMAKE = ${SHELL} /home/mschreiber/Project/buildroot-2016.05/output/build/audit-2.4.4/missing automake-1.15
AWK = gawk
BUILD_EXEEXT = 
BUILD_OBJEXT = 
CAPNG_LDADD = 
CC = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/bin/sh4a-buildroot-linux-gnu-gcc
CCDEPMODE = depmode=none
CC_FOR_BUILD = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/bin/ccache /usr/bin/gcc
CFLAGS = -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64  -Os 
CFLAGS_FOR_BUILD = -O2 -I/home/mschreiber/Project/buildroot-2016.05/output/host/usr/include
CPP = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/bin/sh4a-buildroot-linux-gnu-cpp
CPPFLAGS = -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64
CPPFLAGS_FOR_BUILD = -I/home/mschreiber/Project/buildroot-2016.05/output/host/usr/include
CPP_FOR_BUILD = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/bin/ccache /usr/bin/gcc -E
CYGPATH_W = echo
DEBUG = -DNDEBUG
DEFS = -DHAVE_CONFIG_H
DEPDIR = .deps
DLLTOOL = false
DSYMUTIL = 
DUMPBIN = 
ECHO_C = 
ECHO_N = -n
ECHO_T = 
EGREP = /bin/grep -E
EXEEXT = 
FGREP = /bin/grep -F
GOLANG = no
GOROOT = 
GREP = /bin/grep
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT = ${INSTALL}
INSTALL_STRIP_PROGRAM = $(install_sh) -c -s
LD = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/bin/sh4a-buildroot-linux-gnu-ld
LDFLAGS = 
LDFLAGS_FOR_BUILD = -L/home/mschreiber/Project/buildroot-2016.05/output/host/lib -L/home/mschreiber/Project/buildroot-2016.05/output/host/usr/lib -Wl,-rpath,/home/mschreiber/Project/buildroot-2016.05/output/host/usr/lib
LIBOBJS = 
LIBPRELUDE_CFLAGS = 
LIBPRELUDE_LDFLAGS = 
LIBS = -L${top_builddir}/src/mt -lauditmt 
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LIBTOOL_DEPS = ./ltmain.sh
LIBWRAP_LIBS = 
LIPO = 
LN_S = ln -s
LTLIBOBJS = 
MAKEINFO = ${SHELL} /home/mschreiber/Project/buildroot-2016.05/output/build/audit-2.4.4/missing makeinfo
MANIFEST_TOOL = :
MKDIR_P = /bin/mkdir -p
NM = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/bin/sh4a-buildroot-linux-gnu-nm
NMEDIT = 
OBJDUMP = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/bin/sh4a-buildroot-linux-gnu-objdump
OBJEXT = o
OTOOL = 
OTOOL64 = 
PACKAGE = audit
PACKAGE_BUGREPORT = 
PACKAGE_NAME = audit
PACKAGE_STRING = audit 2.4.4
PACKAGE_TARNAME = audit
PACKAGE_URL = 
PACKAGE_VERSION = 2.4.4
PATH_SEPARATOR = :
PYINCLUDEDIR = 
PYTHON = 
PYTHON3 = 
PYTHON3_CFLAGS = 
PYTHON3_EXEC_PREFIX = 
PYTHON3_INCLUDES = 
PYTHON3_LIBS = 
PYTHON3_PREFIX = 
PYTHON_EXEC_PREFIX = 
PYTHON_PLATFORM = 
PYTHON_PREFIX = 
PYTHON_VERSION = 
RANLIB = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/bin/sh4a-buildroot-linux-gnu-ranlib
SED = /bin/sed
SET_MAKE = 
SHELL = /bin/sh
STRIP = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/bin/sh4a-buildroot-linux-gnu-strip
VERSION = 2.4.4
abs_builddir = /home/mschreiber/Project/buildroot-2016.05/output/build/audit-2.4.4/audisp
abs_srcdir = /home/mschreiber/Project/buildroot-2016.05/output/build/audit-2.4.4/audisp
abs_top_builddir = /home/mschreiber/Project/buildroot-2016.05/output/build/audit-2.4.4
abs_top_srcdir = /home/mschreiber/Project/buildroot-2016.05/output/build/audit-2.4.4
ac_ct_AR = 
ac_ct_CC = 
ac_ct_CC_FOR_BUILD = /home/mschreiber/Project/buildroot-2016.05/output/host/usr/bin/ccache /usr/bin/gcc
ac_ct_DUMPBIN = 
am__include = include
am__leading_dot = .
am__quote = 
am__tar = $${TAR-tar} chof - "$$tardir"
am__untar = $${TAR-tar} xf -
bindir = ${exec_prefix}/bin
build = i686-pc-linux-gnu
build_alias = i686-pc-linux-gnu
build_cpu = i686
build_os = linux-gnu
build_vendor = pc
builddir = .
datadir = ${datarootdir}
datarootdir = ${prefix}/share
docdir = ${datarootdir}/doc/${PACKAGE_TARNAME}
dvidir = ${docdir}
exec_prefix = /usr
gobind_dir = 
gss_libs = 
host = sh4a-buildroot-linux-gnu
host_alias = sh4a-buildroot-linux-gnu
host_cpu = sh4a
host_os = linux-gnu
host_vendor = buildroot
htmldir = ${docdir}
includedir = ${prefix}/include
infodir = ${datarootdir}/info
install_sh = ${SHELL} /home/mschreiber/Project/buildroot-2016.05/output/build/audit-2.4.4/install-sh
libdir = ${exec_prefix}/lib
libexecdir = ${exec_prefix}/libexec
localedir = ${datarootdir}/locale
localstatedir = /var
mandir = ${datarootdir}/man
mkdir_p = $(MKDIR_P)
oldincludedir = /usr/include
pdfdir = ${docdir}
pkgpyexecdir = 
pkgpythondir = 
prefix = /usr
program_transform_name = s&^&&
psdir = ${docdir}
py3execdir = 
pybind_dir = 
pyexecdir = 
python3dir = 
pythondir = 
sbindir = ${exec_prefix}/sbin
sharedstatedir = ${prefix}/com
srcdir = .
sysconfdir = /etc
target = sh4a-buildroot-linux-gnu
target_alias = sh4a-buildroot-linux-gnu
target_cpu = sh4a
target_os = linux-gnu
target_vendor = buildroot
top_build_prefix = ../
top_builddir = ..
top_srcdir = ..
use_python3 = no
SUBDIRS = plugins 
CONFIG_CLEAN_FILES = *.rej *.orig
AUTOMAKE_OPTIONS = no-dependencies
AM_CPPFLAGS = -I${top_srcdir} -I${top_srcdir}/lib
noinst_HEADERS = audispd-config.h audispd-pconfig.h audispd-llist.h \
	queue.h audispd-builtins.h

LDADD = -lpthread
AM_CFLAGS = -D_REENTRANT 
audispd_SOURCES = audispd.c audispd-config.c audispd-pconfig.c \
	audispd-llist.c queue.c audispd-builtins.c

audispd_CFLAGS = -fPIE -DPIE -g -D_GNU_SOURCE
audispd_LDFLAGS = -pie -Wl,-z,relro -Wl,-z,now
all: all-recursive

.SUFFIXES:
.SUFFIXES: .c .lo .o .obj
$(srcdir)/Makefile.in:  $(srcdir)/Makefile.am  $(am__configure_deps)
	@for dep in $?; do \
	  case '$(am__configure_deps)' in \
	    *$$dep*) \
	      ( cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh ) \
	        && { if test -f $@; then exit 0; else break; fi; }; \
	      exit 1;; \
	  esac; \
	done; \
	echo ' cd $(top_srcdir) && $(AUTOMAKE) --gnu audisp/Makefile'; \
	$(am__cd) $(top_srcdir) && \
	  $(AUTOMAKE) --gnu audisp/Makefile
Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
	@case '$?' in \
	  *config.status*) \
	    cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh;; \
	  *) \
	    echo ' cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe)'; \
	    cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe);; \
	esac;

$(top_builddir)/config.status: $(top_srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh

$(top_srcdir)/configure:  $(am__configure_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(ACLOCAL_M4):  $(am__aclocal_m4_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(am__aclocal_m4_deps):
install-sbinPROGRAMS: $(sbin_PROGRAMS)
	@$(NORMAL_INSTALL)
	@list='$(sbin_PROGRAMS)'; test -n "$(sbindir)" || list=; \
	if test -n "$$list"; then \
	  echo " $(MKDIR_P) '$(DESTDIR)$(sbindir)'"; \
	  $(MKDIR_P) "$(DESTDIR)$(sbindir)" || exit 1; \
	fi; \
	for p in $$list; do echo "$$p $$p"; done | \
	sed 's/$(EXEEXT)$$//' | \
	while read p p1; do if test -f $$p \
	 || test -f $$p1 \
	  ; then echo "$$p"; echo "$$p"; else :; fi; \
	done | \
	sed -e 'p;s,.*/,,;n;h' \
	    -e 's|.*|.|' \
	    -e 'p;x;s,.*/,,;s/$(EXEEXT)$$//;$(transform);s/$$/$(EXEEXT)/' | \
	sed 'N;N;N;s,\n, ,g' | \
	$(AWK) 'BEGIN { files["."] = ""; dirs["."] = 1 } \
	  { d=$$3; if (dirs[d] != 1) { print "d", d; dirs[d] = 1 } \
	    if ($$2 == $$4) files[d] = files[d] " " $$1; \
	    else { print "f", $$3 "/" $$4, $$1; } } \
	  END { for (d in files) print "f", d, files[d] }' | \
	while read type dir files; do \
	    if test "$$dir" = .; then dir=; else dir=/$$dir; fi; \
	    test -z "$$files" || { \
	    echo " $(INSTALL_PROGRAM_ENV) $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=install $(INSTALL_PROGRAM) $$files '$(DESTDIR)$(sbindir)$$dir'"; \
	    $(INSTALL_PROGRAM_ENV) $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=install $(INSTALL_PROGRAM) $$files "$(DESTDIR)$(sbindir)$$dir" || exit $$?; \
	    } \
	; done

uninstall-sbinPROGRAMS:
	@$(NORMAL_UNINSTALL)
	@list='$(sbin_PROGRAMS)'; test -n "$(sbindir)" || list=; \
	files=`for p in $$list; do echo "$$p"; done | \
	  sed -e 'h;s,^.*/,,;s/$(EXEEXT)$$//;$(transform)' \
	      -e 's/$$/$(EXEEXT)/' \
	`; \
	test -n "$$list" || exit 0; \
	echo " ( cd '$(DESTDIR)$(sbindir)' && rm -f" $$files ")"; \
	cd "$(DESTDIR)$(sbindir)" && rm -f $$files

clean-sbinPROGRAMS:
	@list='$(sbin_PROGRAMS)'; test -n "$$list" || exit 0; \
	echo " rm -f" $$list; \
	rm -f $$list || exit $$?; \
	test -n "$(EXEEXT)" || exit 0; \
	list=`for p in $$list; do echo "$$p"; done | sed 's/$(EXEEXT)$$//'`; \
	echo " rm -f" $$list; \
	rm -f $$list

audispd$(EXEEXT): $(audispd_OBJECTS) $(audispd_DEPENDENCIES) $(EXTRA_audispd_DEPENDENCIES) 
	@rm -f audispd$(EXEEXT)
	$(AM_V_CCLD)$(audispd_LINK) $(audispd_OBJECTS) $(audispd_LDADD) $(LIBS)

mostlyclean-compile:
	-rm -f *.$(OBJEXT)

distclean-compile:
	-rm -f *.tab.c

.c.o:
	$(AM_V_CC)$(COMPILE) -c -o $@ $<

.c.obj:
	$(AM_V_CC)$(COMPILE) -c -o $@ `$(CYGPATH_W) '$<'`

.c.lo:
	$(AM_V_CC)$(LTCOMPILE) -c -o $@ $<

audispd-audispd.o: audispd.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(audispd_CFLAGS) $(CFLAGS) -c -o audispd-audispd.o `test -f 'audispd.c' || echo '$(srcdir)/'`audispd.c

audispd-audispd.obj: audispd.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(audispd_CFLAGS) $(CFLAGS) -c -o audispd-audispd.obj `if test -f 'audispd.c'; then $(CYGPATH_W) 'audispd.c'; else $(CYGPATH_W) '$(srcdir)/audispd.c'; fi`

audispd-audispd-config.o: audispd-config.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(audispd_CFLAGS) $(CFLAGS) -c -o audispd-audispd-config.o `test -f 'audispd-config.c' || echo '$(srcdir)/'`audispd-config.c

audispd-audispd-config.obj: audispd-config.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(audispd_CFLAGS) $(CFLAGS) -c -o audispd-audispd-config.obj `if test -f 'audispd-config.c'; then $(CYGPATH_W) 'audispd-config.c'; else $(CYGPATH_W) '$(srcdir)/audispd-config.c'; fi`

audispd-audispd-pconfig.o: audispd-pconfig.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(audispd_CFLAGS) $(CFLAGS) -c -o audispd-audispd-pconfig.o `test -f 'audispd-pconfig.c' || echo '$(srcdir)/'`audispd-pconfig.c

audispd-audispd-pconfig.obj: audispd-pconfig.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(audispd_CFLAGS) $(CFLAGS) -c -o audispd-audispd-pconfig.obj `if test -f 'audispd-pconfig.c'; then $(CYGPATH_W) 'audispd-pconfig.c'; else $(CYGPATH_W) '$(srcdir)/audispd-pconfig.c'; fi`

audispd-audispd-llist.o: audispd-llist.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(audispd_CFLAGS) $(CFLAGS) -c -o audispd-audispd-llist.o `test -f 'audispd-llist.c' || echo '$(srcdir)/'`audispd-llist.c

audispd-audispd-llist.obj: audispd-llist.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(audispd_CFLAGS) $(CFLAGS) -c -o audispd-audispd-llist.obj `if test -f 'audispd-llist.c'; then $(CYGPATH_W) 'audispd-llist.c'; else $(CYGPATH_W) '$(srcdir)/audispd-llist.c'; fi`

audispd-queue.o: queue.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(audispd_CFLAGS) $(CFLAGS) -c -o audispd-queue.o `test -f 'queue.c' || echo '$(srcdir)/'`queue.c

audispd-queue.obj: queue.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(audispd_CFLAGS) $(CFLAGS) -c -o audispd-queue.obj `if test -f 'queue.c'; then $(CYGPATH_W) 'queue.c'; else $(CYGPATH_W) '$(srcdir)/queue.c'; fi`

audispd-audispd-builtins.o: audispd-builtins.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(audispd_CFLAGS) $(CFLAGS) -c -o audispd-audispd-builtins.o `test -f 'audispd-builtins.c' || echo '$(srcdir)/'`audispd-builtins.c

audispd-audispd-builtins.obj: audispd-builtins.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(audispd_CFLAGS) $(CFLAGS) -c -o audispd-audispd-builtins.obj `if test -f 'audispd-builtins.c'; then $(CYGPATH_W) 'audispd-builtins.c'; else $(CYGPATH_W) '$(srcdir)/audispd-builtins.c'; fi`

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

# This directory's subdirectories are mostly independent; you can cd
# into them and run 'make' without going through this Makefile.
# To change the values of 'make' variables: instead of editing Makefiles,
# (1) if the variable is set in 'config.status', edit 'config.status'
#     (which will cause the Makefiles to be regenerated when you run 'make');
# (2) otherwise, pass the desired values on the 'make' command line.
$(am__recursive_targets):
	@fail=; \
	if $(am__make_keepgoing); then \
	  failcom='fail=yes'; \
	else \
	  failcom='exit 1'; \
	fi; \
	dot_seen=no; \
	target=`echo $@ | sed s/-recursive//`; \
	case "$@" in \
	  distclean-* | maintainer-clean-*) list='$(DIST_SUBDIRS)' ;; \
	  *) list='$(SUBDIRS)' ;; \
	esac; \
	for subdir in $$list; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    dot_seen=yes; \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  ($(am__cd) $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	  || eval $$failcom; \
	done; \
	if test "$$dot_seen" = "no"; then \
	  $(MAKE) $(AM_MAKEFLAGS) "$$target-am" || exit 1; \
	fi; test -z "$$fail"

ID: $(am__tagged_files)
	$(am__define_uniq_tagged_files); mkid -fID $$unique
tags: tags-recursive
TAGS: tags

tags-am: $(TAGS_DEPENDENCIES) $(am__tagged_files)
	set x; \
	here=`pwd`; \
	if ($(ETAGS) --etags-include --version) >/dev/null 2>&1; then \
	  include_option=--etags-include; \
	  empty_fix=.; \
	else \
	  include_option=--include; \
	  empty_fix=; \
	fi; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  if test "$$subdir" = .; then :; else \
	    test ! -f $$subdir/TAGS || \
	      set "$$@" "$$include_option=$$here/$$subdir/TAGS"; \
	  fi; \
	done; \
	$(am__define_uniq_tagged_files); \
	shift; \
	if test -z "$(ETAGS_ARGS)$$*$$unique"; then :; else \
	  test -n "$$unique" || unique=$$empty_fix; \
	  if test $$# -gt 0; then \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      "$$@" $$unique; \
	  else \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      $$unique; \
	  fi; \
	fi
ctags: ctags-recursive

CTAGS: ctags
ctags-am: $(TAGS_DEPENDENCIES) $(am__tagged_files)
	$(am__define_uniq_tagged_files); \
	test -z "$(CTAGS_ARGS)$$unique" \
	  || $(CTAGS) $(CTAGSFLAGS) $(AM_CTAGSFLAGS) $(CTAGS_ARGS) \
	     $$unique

GTAGS:
	here=`$(am__cd) $(top_builddir) && pwd` \
	  && $(am__cd) $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) "$$here"
cscopelist: cscopelist-recursive

cscopelist-am: $(am__tagged_files)
	list='$(am__tagged_files)'; \
	case "$(srcdir)" in \
	  [\\/]* | ?:[\\/]*) sdir="$(srcdir)" ;; \
	  *) sdir=$(subdir)/$(srcdir) ;; \
	esac; \
	for i in $$list; do \
	  if test -f "$$i"; then \
	    echo "$(subdir)/$$i"; \
	  else \
	    echo "$$sdir/$$i"; \
	  fi; \
	done >> $(top_builddir)/cscope.files

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH tags

distdir: $(DISTFILES)
	@srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	topsrcdirstrip=`echo "$(top_srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	list='$(DISTFILES)'; \
	  dist_files=`for file in $$list; do echo $$file; done | \
	  sed -e "s|^$$srcdirstrip/||;t" \
	      -e "s|^$$topsrcdirstrip/|$(top_builddir)/|;t"`; \
	case $$dist_files in \
	  */*) $(MKDIR_P) `echo "$$dist_files" | \
			   sed '/\//!d;s|^|$(distdir)/|;s,/[^/]*$$,,' | \
			   sort -u` ;; \
	esac; \
	for file in $$dist_files; do \
	  if test -f $$file || test -d $$file; then d=.; else d=$(srcdir); fi; \
	  if test -d $$d/$$file; then \
	    dir=`echo "/$$file" | sed -e 's,/[^/]*$$,,'`; \
	    if test -d "$(distdir)/$$file"; then \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    if test -d $(srcdir)/$$file && test $$d != $(srcdir); then \
	      cp -fpR $(srcdir)/$$file "$(distdir)$$dir" || exit 1; \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    cp -fpR $$d/$$file "$(distdir)$$dir" || exit 1; \
	  else \
	    test -f "$(distdir)/$$file" \
	    || cp -p $$d/$$file "$(distdir)/$$file" \
	    || exit 1; \
	  fi; \
	done
	@list='$(DIST_SUBDIRS)'; for subdir in $$list; do \
	  if test "$$subdir" = .; then :; else \
	    $(am__make_dryrun) \
	      || test -d "$(distdir)/$$subdir" \
	      || $(MKDIR_P) "$(distdir)/$$subdir" \
	      || exit 1; \
	    dir1=$$subdir; dir2="$(distdir)/$$subdir"; \
	    $(am__relativize); \
	    new_distdir=$$reldir; \
	    dir1=$$subdir; dir2="$(top_distdir)"; \
	    $(am__relativize); \
	    new_top_distdir=$$reldir; \
	    echo " (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) top_distdir="$$new_top_distdir" distdir="$$new_distdir" \\"; \
	    echo "     am__remove_distdir=: am__skip_length_check=: am__skip_mode_fix=: distdir)"; \
	    ($(am__cd) $$subdir && \
	      $(MAKE) $(AM_MAKEFLAGS) \
	        top_distdir="$$new_top_distdir" \
	        distdir="$$new_distdir" \
		am__remove_distdir=: \
		am__skip_length_check=: \
		am__skip_mode_fix=: \
	        distdir) \
	      || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-recursive
all-am: Makefile $(PROGRAMS) $(HEADERS)
installdirs: installdirs-recursive
installdirs-am:
	for dir in "$(DESTDIR)$(sbindir)"; do \
	  test -z "$$dir" || $(MKDIR_P) "$$dir"; \
	done
install: install-recursive
install-exec: install-exec-recursive
install-data: install-data-recursive
uninstall: uninstall-recursive

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-recursive
install-strip:
	if test -z '$(STRIP)'; then \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	      install; \
	else \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	    "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'" install; \
	fi
mostlyclean-generic:

clean-generic:

distclean-generic:
	-test -z "$(CONFIG_CLEAN_FILES)" || rm -f $(CONFIG_CLEAN_FILES)
	-test . = "$(srcdir)" || test -z "$(CONFIG_CLEAN_VPATH_FILES)" || rm -f $(CONFIG_CLEAN_VPATH_FILES)

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-recursive

clean-am: clean-generic clean-libtool clean-sbinPROGRAMS \
	mostlyclean-am

distclean: distclean-recursive
	-rm -f Makefile
distclean-am: clean-am distclean-compile distclean-generic \
	distclean-tags

dvi: dvi-recursive

dvi-am:

html: html-recursive

html-am:

info: info-recursive

info-am:

install-data-am:

install-dvi: install-dvi-recursive

install-dvi-am:

install-exec-am: install-sbinPROGRAMS
	@$(NORMAL_INSTALL)
	$(MAKE) $(AM_MAKEFLAGS) install-exec-hook
install-html: install-html-recursive

install-html-am:

install-info: install-info-recursive

install-info-am:

install-man:

install-pdf: install-pdf-recursive

install-pdf-am:

install-ps: install-ps-recursive

install-ps-am:

installcheck-am:

maintainer-clean: maintainer-clean-recursive
	-rm -f Makefile
maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-recursive

mostlyclean-am: mostlyclean-compile mostlyclean-generic \
	mostlyclean-libtool

pdf: pdf-recursive

pdf-am:

ps: ps-recursive

ps-am:

uninstall-am: uninstall-sbinPROGRAMS

.MAKE: $(am__recursive_targets) install-am install-exec-am \
	install-strip

.PHONY: $(am__recursive_targets) CTAGS GTAGS TAGS all all-am check \
	check-am clean clean-generic clean-libtool clean-sbinPROGRAMS \
	cscopelist-am ctags ctags-am distclean distclean-compile \
	distclean-generic distclean-libtool distclean-tags distdir dvi \
	dvi-am html html-am info info-am install install-am \
	install-data install-data-am install-dvi install-dvi-am \
	install-exec install-exec-am install-exec-hook install-html \
	install-html-am install-info install-info-am install-man \
	install-pdf install-pdf-am install-ps install-ps-am \
	install-sbinPROGRAMS install-strip installcheck \
	installcheck-am installdirs installdirs-am maintainer-clean \
	maintainer-clean-generic mostlyclean mostlyclean-compile \
	mostlyclean-generic mostlyclean-libtool pdf pdf-am ps ps-am \
	tags tags-am uninstall uninstall-am uninstall-sbinPROGRAMS

.PRECIOUS: Makefile


install-exec-hook:
	chmod 0750 $(DESTDIR)$(sbindir)/audispd

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
