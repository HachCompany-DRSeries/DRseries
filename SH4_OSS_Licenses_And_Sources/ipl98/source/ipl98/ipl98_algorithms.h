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

/* ipl98_algorithms.h, all kernel algorithm functions in IPL98,
   include this file to use the kernel algorithms in
   IPL98, the files ipl98_container.h and
   ipl98_container.c must also be included.

   Version 1.41, Last updated 7/2/2002 by edr@mip.sdu.dk */

#include "ipl98_containers.h"

/************************************************/
/********          Algorithms            ********/
/************************************************/

#include "kernel_c/algorithms/kernel_graphics.h"
#include "kernel_c/algorithms/kernel_statistic.h"
#include "kernel_c/algorithms/kernel_segmentate.h"
#include "kernel_c/algorithms/kernel_feature_extraction.h"
#include "kernel_c/algorithms/kernel_perspective.h"
#include "kernel_c/algorithms/kernel_morphology.h"
#include "kernel_c/algorithms/kernel_mask_operation.h"
#include "kernel_c/algorithms/kernel_local_operation.h"
#include "kernel_c/algorithms/kernel_coordinate_transform.h"
#include "kernel_c/algorithms/kernel_spectral.h"

