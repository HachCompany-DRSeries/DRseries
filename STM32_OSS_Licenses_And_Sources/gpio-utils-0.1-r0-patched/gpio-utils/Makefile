#
# Simple development makefile
#

#
# add more flags here,
# but don't remove the "+="
#
CFLAGS += -O2 -Wall #-Wsign-compare -Wfloat-equal -Wformat-security #-Werror
CPPFLAGS +=
LDFLAGS +=

all: gpio-trigger gpio-overcurrent

gpio-trigger: gpio-trigger.c gpio-utils-lib.c
gpio-overcurrent: gpio-overcurrent.c gpio-utils-lib.c

clean:
	-rm -f gpio-trigger gpio-overcurrent *.o

install:

.PHONY: all install clean

# end of development makefile
