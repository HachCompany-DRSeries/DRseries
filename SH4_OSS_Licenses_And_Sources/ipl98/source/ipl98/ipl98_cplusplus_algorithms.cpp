/* ipl98_cplusplus_algorithms.cpp, only algorithm classes are
   included, include this file to use the c++ algorithms in
   IPL98, the files ipl98_cplusplus_container.h and
   ipl98_cplusplus_container.cpp must also be included.

   Version 1.70, Last updated 9/30/2003 by edr@mip.sdu.dk */

#ifndef _IPL98_CPP_ALGORITHMS_CPP
#define _IPL98_CPP_ALGORITHMS_CPP

/* Include IPL98 headers */
#include "ipl98_cplusplus_algorithms.h"

/* include all kernel ansi c routines/types/macros */
#include "ipl98_algorithms.c"

/************************************************/
/********          Algorithms            ********/
/************************************************/

#include "cpp/algorithms/graphics.cpp"
#include "cpp/algorithms/graph_plot2d.cpp"
#include "cpp/algorithms/statistic.cpp"
#include "cpp/algorithms/segmentate.cpp"
#include "cpp/algorithms/feature_extraction.cpp"
#include "cpp/algorithms/perspective.cpp"
#include "cpp/algorithms/morphology.cpp"
#include "cpp/algorithms/mask_operation.cpp"
#include "cpp/algorithms/local_operation.cpp"
#include "cpp/algorithms/coordinate_transform.cpp"
#include "cpp/algorithms/spectral.cpp"
#include "cpp/algorithms/equation_solver.cpp"
#include "cpp/algorithms/topography.cpp"

#include "cpp/algorithms/hough_transform/sparse_hough_space2d.cpp"
#include "cpp/algorithms/hough_transform/hough_base.cpp"
#include "cpp/algorithms/hough_transform/hough_space2d.cpp"
#include "cpp/algorithms/hough_transform/hough_sht_line.cpp"
#include "cpp/algorithms/hough_transform/hough_rht_line.cpp"
#include "cpp/algorithms/hough_transform/hough_rht_circle.cpp"
#include "cpp/algorithms/hough_transform/sparse_hough_space3d.cpp"
#include "cpp/algorithms/hough_transform/hough_probrht_line.cpp"

#endif /* _IPL98_CPP_ALGORITHMS_CPP */
