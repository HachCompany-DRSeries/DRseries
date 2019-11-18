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

/* ipl98.c, all kernel container functions in IPL98, 
   to use the algorithm classes include the file
   ipl98_algorithms.cpp too.
   Version 1.20 Last updated 4/18/2000 by edr@mip.sdu.dk */

#ifndef _IPL98_CONTAINERS_C
#define _IPL98_CONTAINERS_C

#include "ipl98_setup.c"

/* include help functions */
#include "../time/kernel_c/kernel_time_date.c"

/* Include IPL98 ansi c headers */
#include "ipl98_containers.h"

/* include simple functions and types */
#include "kernel_c/kernel_string.c"
#include "kernel_c/kernel_error.c"
#include "kernel_c/ipl98_types.c"
#include "kernel_c/kernel_io.c"
#include "kernel_c/kernel_history.c"

/************************************************/
/********             Text               ********/
/************************************************/

#include "kernel_c/kernel_text.c"

/************************************************/
/********            Palette             ********/
/************************************************/

#include "kernel_c/kernel_palette.c"

/************************************************/
/********         PixelGroup(s)          ********/
/************************************************/

#include "kernel_c/kernel_pixelgroup.c"
#include "kernel_c/kernel_pixelgroups.c"

/************************************************/
/********             Image              ********/
/************************************************/

/* include kernel routines */
#include "kernel_c/image/kernel_functions.c"

/************************************************/
/********           ByteImage            ********/
/************************************************/

/* include kernel routines */
#include "kernel_c/byte_image/b_kernel_functions.c"

/************************************************/
/********          FloatImage            ********/
/************************************************/

/* include kernel routines */
#include "kernel_c/float_image/f_kernel_functions.c"

/************************************************/
/********           IntImage             ********/
/************************************************/

/* include kernel routines */
#include "kernel_c/int_image/i_kernel_functions.c"

/************************************************/
/********          ComplexImage           ********/
/************************************************/

/* include kernel routines */
#include "kernel_c/complex_image/c_kernel_functions.c"

/************************************************/
/********    Corresponding3D2DPoints     ********/
/************************************************/

/* include kernel routines */
#include "kernel_c/kernel_corresponding_3d2d_points.c"

/* only include the following if the algorithm class definitions
   are not included. */
#ifndef _IPL98_C
/* include a single kernel algorithm class, used by the image
   classes to draw simple lines and circles */
#include "kernel_c/algorithms/kernel_graphics.c"
#endif /* _IPL98_C */

#endif /* _IPL98_CONTAINERS_C */
