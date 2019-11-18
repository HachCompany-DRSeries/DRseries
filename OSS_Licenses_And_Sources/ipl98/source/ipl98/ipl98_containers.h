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

/* ipl98_containers.h, all ansi c container functions
   to be used for c programs only

   Version 1.40, Last updated 8/14/2000 by edr@mip.sdu.dk */

#include "ipl98_setup.h"

#include "kernel_c/kernel_history.h"

/************************************************/
/********    Include help functions      ********/
/************************************************/
#include "kernel_c/general_functions/ipl_general_functions_c_only.h"
#include "../time/kernel_c/kernel_time_date.h"

/************************************************/
/******** Include simple functions/types ********/
/************************************************/
#include "kernel_c/kernel_string.h"
#include "kernel_c/kernel_error.h"
#include "kernel_c/ipl98_types.h"
#include "kernel_c/ipl98_types_c_only.h"
#include "kernel_c/kernel_io.h"

/************************************************/
/********             Text               ********/
/************************************************/

#include "kernel_c/kernel_text.h"

/************************************************/
/********            Palette             ********/
/************************************************/

#include "kernel_c/kernel_palette.h"

/************************************************/
/********         PixelGroup(s)          ********/
/************************************************/

#include "kernel_c/kernel_pixelgroup.h"
#include "kernel_c/kernel_pixelgroups.h"

/************************************************/
/********             Image              ********/
/************************************************/

/* include kernel routines */
#include "kernel_c/image/kernel_functions.h"

/************************************************/
/********           ByteImage            ********/
/************************************************/

/* include kernel routines */
#include "kernel_c/byte_image/b_kernel_functions.h"

/************************************************/
/********          FloatImage            ********/
/************************************************/

/* include kernel routines */
#include "kernel_c/float_image/f_kernel_functions.h"

/************************************************/
/********           IntImage             ********/
/************************************************/

/* include kernel routines */
#include "kernel_c/int_image/i_kernel_functions.h"

/************************************************/
/********          CompexImage           ********/
/************************************************/

/* include kernel routines */
#include "kernel_c/complex_image/c_kernel_functions.h"

/************************************************/
/********          ComplexImage           *******/
/************************************************/

/* include kernel routines */
#include "kernel_c/kernel_corresponding_3d2d_points.h"

