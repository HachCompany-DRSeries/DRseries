/* ipl98_cplusplus_containers.cpp, only container classes are
   included, include this file to use the c++ containers in IPL98, 
   to use the algorithm classes include the file
   ipl98_cplusplus_algorithms.h too.

   Version 1.60, Last updated 4/7/2002 by edr@mip.sdu.dk */

#ifndef _IPL98_CPP_CONTAINERS_CPP
#define _IPL98_CPP_CONTAINERS_CPP

/* Include IPL98 headers */
#include "ipl98_cplusplus_containers.h"

/* include all kernel ansi c routines/types/macros */
#include "ipl98_containers.c"

/* Include small widely useful template routines */
#include "cpp/ipl98_general_functions.cpp"

/* include help classes */
#include "../time/time_date.cpp"

/* include cpp container classes in ipl */
#include "cpp/text.cpp"
#include "cpp/palette.cpp"
#include "cpp/pixelgroup.cpp"
#include "cpp/pixelgroups.cpp"
#include "cpp/std_image.cpp"
#include "cpp/image.cpp"
#include "cpp/byte_image.cpp"
#include "cpp/float_image.cpp"
#include "cpp/int_image.cpp"
#include "cpp/complex_image.cpp"
#include "cpp/corresponding_3d2d_points.cpp"

#endif /* _IPL98_CPP_CONTAINERS_CPP */
