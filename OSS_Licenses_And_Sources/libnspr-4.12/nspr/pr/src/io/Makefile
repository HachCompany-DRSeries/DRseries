# 
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

#! gmake

MOD_DEPTH	= ../../..
topsrcdir	= ../../..
srcdir		= .


include $(MOD_DEPTH)/config/autoconf.mk

include $(topsrcdir)/config/config.mk

CSRCS = \
    prfdcach.c \
    prmwait.c \
    priometh.c \
    pripv6.c \
	prmapopt.c \
    prlayer.c \
    prlog.c \
	prmmap.c \
    prpolevt.c \
	prprf.c \
	prscanf.c \
	prstdio.c  \
	$(NULL)

ifndef USE_PTHREADS
    CSRCS += \
	    prdir.c \
	    prfile.c \
	    prio.c \
	    prsocket.c \
	    $(NULL)
endif

TARGETS	= $(OBJS)

INCLUDES = -I$(dist_includedir) -I$(topsrcdir)/pr/include -I$(topsrcdir)/pr/include/private

DEFINES	+= -D_NSPR_BUILD_

include $(topsrcdir)/config/rules.mk

export:: $(TARGETS)

