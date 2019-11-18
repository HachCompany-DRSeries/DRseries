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

/* ipl98_algorithms.c, all kernel algorithm functions in IPL98,
   include this file to use the kernel algorithms in
   IPL98, the files ipl98_container.h and
   ipl98_container.c must also be included.

   Version 1.60 Last updated 11/29/2000 by edr@mip.sdu.dk */

#ifndef _IPL98_ALGORITHMS_C
#define _IPL98_ALGORITHMS_C

/* Include IPL98 ansi c headers */
#include "ipl98_algorithms.h"

/************************************************/
/********          Algorithms            ********/
/************************************************/

#include "kernel_c/algorithms/kernel_statistic.c"
#include "kernel_c/algorithms/kernel_segmentate.c"
#include "kernel_c/algorithms/kernel_feature_extraction.c"
#include "kernel_c/algorithms/kernel_perspective.c"
#include "kernel_c/algorithms/kernel_morphology.c"
#include "kernel_c/algorithms/kernel_mask_operation.c"
#include "kernel_c/algorithms/kernel_local_operation.c"
#include "kernel_c/algorithms/kernel_coordinate_transform.c"
#include "kernel_c/algorithms/kernel_spectral.c"

/** If the C++ CImage and CByteImage classes are in use, they
	already include this file. They need it to draw simple lines
	and circles. So in this case a container class uses an algo-
	rithm class. Using ANSI C only, we don't have this problem. */
#ifndef _IPL98_CPP_CONTAINERS_H
#include "kernel_c/algorithms/kernel_graphics.c"
#endif

#endif /* _IPL98_ALGORITHMS_C */
