MS Visual C++ 6.0 projects compiling the IPL98 and creating lib-files.

Two projects are provided:

**********************************************
ipl98library

Compiles the IPL98 library including the function:

AddIPL98ErrorHistory(const char* str)

This functions outputs by default all messages created
by the IPL98 library to std out. If the user needs to
redirect this information to another place, for instance
a window, one should use the second project named
ipl98library_gui.

Output files are placed in the folder /lib and are named:

ipl98.lib  (release version)
ipl98d.lib (debug version)

**********************************************
ipl98library_gui

Compiles the IPL98 library excluding the function:

AddIPL98ErrorHistory(const char* str)

This is done by defining:

IPL_USER_SUPPLY_ERROR_FUNCTION

for the whole project in the Project Settings. It can
also be done by removing the comments in file ipl98/ipl98_setup.h
in the line:

/* #define IPL_USER_SUPPLY_ERROR_FUNCTION */

The user must supply this function. This is convenient
if one needs to redirect the output from the IPL98
library to for instance a window.

Output files are placed in the folder /lib and are named:

ipl98.lib  (release version)
ipl98d.lib (debug version)
