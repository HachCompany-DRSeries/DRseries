
SHELL = /bin/sh

#CC = gcc
CC = sh-unknown-linux-gnu-g++
##AR = ar
AR = sh-unknown-linux-gnu-ar
INSTALL = install
INSTALL_DATA = $(INSTALL) -m 644

srcdir = ../../source/ipl98
prefix = ../..
exec_prefix = $(prefix)
libdir = $(exec_prefix)/lib

CFLAGS = -Wall -g

CC_FILE = $(srcdir)/ipl98_cplusplus.cpp
O_FILE = $(CC_FILE:.cpp=.o)
LIBRARY_BASE = libipl98.a
LIBRARY = $(srcdir)/$(LIBRARY_BASE)

.SUFFIXES:

all: $(LIBRARY)

$(O_FILE):
	$(CC) -o $@ -c $(CC_FILE) $(CFLAGS)

$(LIBRARY): $(O_FILE)
	$(AR) cqs $(LIBRARY) $(O_FILE)

install: all
	$(INSTALL_DATA) $(LIBRARY) $(libdir)/$(LIBRARY_BASE)

uninstall:
	rm -f $(libdir)/$(LIBRARY_BASE)

tags:
	find $(srcdir)/.. -name '*.c' -or -name '*.cpp' -or -name '*.h' | \
	xargs etags -o $(srcdir)/TAGS

clean:
	rm -f $(LIBRARY) $(O_FILE) $(srcdir)/TAGS
