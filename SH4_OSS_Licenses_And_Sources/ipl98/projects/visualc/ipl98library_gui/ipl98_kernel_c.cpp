/** Use this file to inckude the C kernel when creating
    a IPL98 library. It makes sure that VC++ will treat
	all files as C++ files (then __cplusplus is defined
	and the C kernel is put in ipl namespace). It also
	avoids some problems with ambiguos functions caused
	by including both C++ library files and the old
	C library files (for instance atan2). */

#include <ipl98/ipl98.c>
