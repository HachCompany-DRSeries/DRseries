
#
# Makefile template for IPL98 projects (and other smallish C++ programs)
#

# Name of compiler and standard compiler flags.
CC = g++
CPPFLAGS = -Wall -O0

# The name of the executable to build and a list of source files.
PROGRAM = run
CPP_FILES = main.cpp

# Libraries and directory paths for include files.
LINKER_FLAGS = -L$(IPL98_LIB_DIR) -lipl98
INCLUDE_FLAGS = -I$(IPL98_INCLUDE_DIR)

IPL98_INCLUDE_DIR = ../../../source
IPL98_LIB_DIR = ../../../lib

# Makefile targets, automatic generation of dependency information, etc.

O_FILES = $(CPP_FILES:.cpp=.o)
D_FILES = $(CPP_FILES:.cpp=.d)

all: $(PROGRAM)

$(PROGRAM) : $(O_FILES)
	$(CC) $(O_FILES) $(LINKER_FLAGS) -o $(PROGRAM)

-include $(D_FILES)

%.o : %.cpp
	$(CC) -c $< -o $@ $(INCLUDE_FLAGS) $(CPPFLAGS)

%.d: %.cpp
	$(SHELL) -ec '$(CC) -MM $(INCLUDE_FLAGS) $< \
	| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
	[ -s $@ ] || rm -f $@'

clean:
	rm -f $(O_FILES)
	rm -f $(D_FILES)
	rm -f $(PROGRAM)
