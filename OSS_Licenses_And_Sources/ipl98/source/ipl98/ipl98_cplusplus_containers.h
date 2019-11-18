/********************************************************************
   The Image Processing Library 98 - IPL98
   Copyright (C) by René Dencker Eriksen - edr@mip.sdu.dk

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation. Only
   addition is, that if the library is used in proprietary software
   it must be stated that IPL98 was used.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

   More details can be found in the file licence.txt distributed
   with this library.
*********************************************************************/

/* ipl98_cplusplus_containers.h, only container classes are
   included, include this file to use the c++ containers in IPL98, 
   to use the algorithm classes include the file
   ipl98_cplusplus_algorithms.h too.

   Version 1.61, Last updated 10/31/2003 by edr@mip.sdu.dk */

#ifndef _IPL98_CPP_CONTAINERS_H
#define _IPL98_CPP_CONTAINERS_H

#ifndef _IPL98_USING_CPP
#define _IPL98_USING_CPP /* used in the kernel C part to test if namespace ipl should be used */
#endif

/* only container classes are included */

/* Include IPL98 ansi c headers */
#include "ipl98_containers.h"

/* Include small widely useful template routines */
#include "cpp/ipl98_general_functions.h"

/* include cpp classes */
#include "../time/time_date.h"
#include "cpp/error.h"

#include "../points/point2d.h"
#include "../points/point3d.h"
#include "cpp/vectors/vector2d.h"
#include "cpp/vectors/vector3d.h"

#include "cpp/text.h"
#include "cpp/palette.h"
#include "cpp/pixelgroup.h"
#include "cpp/pixelgroups.h"
#include "cpp/positiongroup2d.h"
#include "cpp/generic_image.h"
#include "cpp/std_image.h"
#include "cpp/image.h"
#include "cpp/byte_image.h"
#include "cpp/float_image.h"
#include "cpp/int_image.h"
#include "cpp/complex_image.h"
#include "cpp/corresponding_3d2d_points.h"
#include "cpp/arrays/array2d.h"
#include "cpp/arrays/array2d_it.h"
#include "cpp/arrays/array2d_const_it.h"
#include "cpp/arrays/arrayos2d.h"
#include "cpp/arrays/arrayos2d_it.h"
#include "cpp/arrays/arrayos2d_const_it.h"
#include "cpp/geometry/line2d.h"
#include "cpp/geometry/line_segment2d.h"
#include "cpp/geometry/line3d.h"
#include "cpp/geometry/plane.h"


#endif /* _IPL98_CPP_CONTAINERS_H */
