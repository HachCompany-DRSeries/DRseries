# Makefile -- 
# Copyright 1996, 1997 Rickard E. Faith <faith@acm.org>
# Copyright 1999 Avery Pennarun <apenwarr@worldvisions.ca>
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2, or (at your option) any
# later version.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 675 Mass Ave, Cambridge, MA 02139, USA.
#
# $Id: Makefile,v 1.30 2001/11/25 22:46:16 apenwarr Exp $
#

VERSION=3.2.1
LIB_VERSION=1:0

ifneq (,)
This makefile requires GNU Make.
endif

.SUFFIXES:

OBJS=apmlib.lo
EXES=apm apmd xapm apmsleep
HEADERS=apm.h

PREFIX=/usr
MANDIR=$(PREFIX)/man
BINDIR=$(PREFIX)/bin
SBINDIR=$(PREFIX)/sbin
LIBDIR=$(PREFIX)/lib
INCDIR=$(PREFIX)/include
PROXY_DIR=/etc
DESTDIR=

CC=gcc
CFLAGS=-O -g
XTRACFLAGS=-Wall -pipe -I. \
		-DVERSION=\"$(VERSION)\" \
		-DDEFAULT_PROXY_NAME=\"$(PROXY_DIR)/apmd_proxy\"
LDFLAGS=
LIBAPM=libapm.la
LIBAPM_FLAGS=-rpath $(LIBDIR) -version-info $(LIB_VERSION)
XLDFLAGS=-L/usr/X11R6/lib
XLIBS=-lXaw -lXmu -lXt -lXext -lSM -lICE -lX11
RANLIB=ranlib

# Uncomment these lines for a production compile
#CFLAGS=-O3 -m486 -fomit-frame-pointer
#LDFLAGS=-s

LIBTOOL=libtool --quiet
LT_COMPILE = $(LIBTOOL) --tag=CC --mode=compile $(CC)
LT_LINK = $(LIBTOOL)  --tag=CC --mode=link $(CC)
LT_INSTALL = $(LIBTOOL) --mode=install install
LT_CLEAN = $(LIBTOOL) --mode=clean rm

all: $(EXES)

$(OBJS): $(HEADERS)

%.lo: %.c
	$(LT_COMPILE) -c $(CPPFLAGS) $(CFLAGS) $(XTRACFLAGS) $<

%: %.lo $(LIBAPM)
	$(LT_LINK) -o $@ $< $(LDFLAGS) $(LIBAPM)

xapm.lo: xapm.c
	$(LT_COMPILE) -c $(CPPFLAGS) $(CFLAGS) $(XTRACFLAGS) -DNARROWPROTO $<

apmd: apmd.lo

apmsleep: apmsleep.lo

apmexists: apmexists.lo

xapm: xapm.lo $(LIBAPM)
	$(LT_LINK) -o $@ $< $(LDFLAGS) $(LIBAPM) $(XLDFLAGS) $(XLIBS)

$(LIBAPM): apmlib.lo
	$(LT_LINK) -o $@ $< $(LIBAPM_FLAGS) $(LDFLAGS)

apmlib.lo: apmlib.c apm.h
	$(LT_COMPILE) -c $(CPPFLAGS) $(CFLAGS) $(XTRACFLAGS) $<

.PHONY:	ChangeLog
ChangeLog:
	-rm -f ChangeLog
	rcs2log > ChangeLog

install: $(EXES)
	[ -d $(DESTDIR)$(BINDIR)    ] || install -d $(DESTDIR)$(BINDIR)
	[ -d $(DESTDIR)$(SBINDIR)   ] || install -d $(DESTDIR)$(SBINDIR)
	[ -d $(DESTDIR)$(LIBDIR)    ] || install -d $(DESTDIR)$(LIBDIR)
	[ -d $(DESTDIR)$(INCDIR)    ] || install -d $(DESTDIR)$(INCDIR)
	$(LT_INSTALL) -m 644 $(LIBAPM)   $(DESTDIR)$(LIBDIR)
	$(LT_INSTALL)        apm         $(DESTDIR)$(BINDIR)
	$(LT_INSTALL)        apmd        $(DESTDIR)$(SBINDIR)
	$(LT_INSTALL)        xapm        $(DESTDIR)$(BINDIR)
	$(LT_INSTALL)        apmsleep    $(DESTDIR)$(BINDIR)
	install              on_ac_power $(DESTDIR)$(BINDIR)
	install       -m 644 apm.h       $(DESTDIR)$(INCDIR)

.PHONY:	clean distclean
distclean clean:
	-$(LT_CLEAN) -f $(LIBAPM) $(EXES) *.lo
	-rm -f *.o *.s *~ core a.out build-stamp
