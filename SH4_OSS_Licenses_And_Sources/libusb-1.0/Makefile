CC = sh-unknown-linux-gnu-gcc
AR = sh-unknown-linux-gnu-ar

# CFLAGS += -I../include -fms-extensions
CFLAGS += -I.


LIB_USB = libusb-1.0.so
STATLIB_USB = libusb-1.0.a
VER_USB = 1.0
OBJS_USB = \
    ./core.o \
    ./io.o \
    ./sync.o \
    ./descriptor.o \
    ./linux_usbfs.o \

./core.o : core.c
	$(CC) -c $< $(CFLAGS) -o $@
./io.o : io.c
	$(CC) -c $< $(CFLAGS) -o $@
./sync.o : sync.c
	$(CC) -c $< $(CFLAGS) -o $@
./descriptor.o : descriptor.c
	$(CC) -c $< $(CFLAGS) -o $@
./linux_usbfs.o : linux_usbfs.c
	$(CC) -c $< $(CFLAGS) -o $@


all : $(STATLIB_USB) $(LIB_USB)
dyn : $(LIB_USB)
stat : $(STATLIB_USB)
clean :
	rm -fv $(LIB_USB)* $(STATLIB_USB) *.o


$(LIB_USB) : $(OBJS_USB)
	$(CC) -o $@.$(VER_USB) -shared -Wl,-soname,$@ $(OBJS_USB)
	ln -sf $@.$(VER_USB) $@

$(STATLIB_USB) : $(OBJS_USB)
	$(AR) -rcs $@ $(OBJS_USB)