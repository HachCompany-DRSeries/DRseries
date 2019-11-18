
* Building and using IPL98 with GCC

This directory contains a simple GNU Makefile for compiling the IPL98
library. You will need a GCC of version 3.0.1 or later.

To compile and build the library type the following commands:

    $ make
    $ make install

This will build a static library called libipl98.a and copy it to the
ipl98/lib directory. To delete intermediary files of the build process
you may want to run:

    $ make clean

When compiling programs using the IPL98 library the relevant paths of
the ipl98/source directory should be added to the include path of the
compiler. If for example the ipl98 directory is placed in the $HOME
directory the command line of the compiler would be similar to:

    $ g++ -c main.cc \
        -I$HOME/ipl98/source/ipl98 \
        -I$HOME/ipl98/source/ipl98/cpp \
        ...

To link a collection of object files against the library, add the
ipl98/lib directory to the library path and include a -lipl98 and -lm
option:

    $ g++ a.o b.o c.o ... \
        -L$HOME/ipl98/lib \
        -lipl98 \
        -lm

Questions / suggestions should be sent to:
Anders Lau Olsen <alauo@mip.sdu.dk>
