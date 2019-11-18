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

#ifndef _IPL98_KERNEL_PERSPECTIVE_H
#define _IPL98_KERNEL_PERSPECTIVE_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../ipl98_types.h"
#include "../ipl98_types_c_only.h"
#include "../kernel_corresponding_3d2d_points.h"
#include "../image/kernel_functions.h"
#include <math.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup perspective ANSI-C Perspective functions
	Kernel ANSI C Camera calibration and 3D->2D/2D->3D perspective
	functions, Last updated 11/31/2000.
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 1.32
	@author Ivar Balslev (ivb@mip.sdu.dk), Brian Kirstein (kirstein@protana.com)
		and René Dencker Eriksen (edr@mip.sdu.dk)
    @{ */

/** This is the camera matrix explained in Gonzales and Woods.
	Camera matrix as given by Gonzalez and Woods page 67. */
typedef struct {
	/** @param a11 in the camera matrix. */
	FLOAT32 a11;
	/** @param a12 in the camera matrix. */
	FLOAT32 a12;
	/** @param a13 in the camera matrix. */
	FLOAT32 a13;
	/** @param a14 in the camera matrix. */
	FLOAT32 a14;
	/** @param a21 in the camera matrix. */
	FLOAT32 a21;
	/** @param a22 in the camera matrix. */
	FLOAT32 a22;
	/** @param a23 in the camera matrix. */
	FLOAT32 a23;
	/** @param a24 in the camera matrix. */
	FLOAT32 a24;
	/** @param a31 in the camera matrix. */
	FLOAT32 a31;
	/** @param a32 in the camera matrix. */
	FLOAT32 a32;
	/** @param a33 in the camera matrix. */
	FLOAT32 a33;
	/** @param a34 in the camera matrix. */
	FLOAT32 a34;
	/** @param a41 in the camera matrix. */
	FLOAT32 a41;
	/** @param a42 in the camera matrix. */
	FLOAT32 a42;
	/** @param a43 in the camera matrix. */
	FLOAT32 a43;
	/** @param a44 in the camera matrix. */
	FLOAT32 a44;
} TCameraMatrix;

/** Calibrates the camera matrix given in Gonzalez & Woods page 67.
	@param np Number of 3D world cordinates given in the parameter array World.
	@param pWorld Array containing 3D world coordinates (must be same size as pCcd array).
	@param pCcd Array containing the 2D image points (must be same size as pWorld array).
	@param pMat The camera parameters are returned in this variable. 
	@return False, if np<6 or if one of the given parameters is a NULL pointer.
	@see k_CalibrateCamera */
bool k_Calibrate(unsigned int np, const T3DFloat *pWorld, const T2DFloat *pCcd, TCameraMatrix* pMat);

/** Calibrates the camera matrix exactly as the function k_Calibrate() does, but this
	function takes a T3D2DPoints structure containing the 3D world coordinates
	and the 2D ccd coordinates.
	@param pPointSets 3D and 2D corresponding coordinates, all entries must be
		complete, ie. an entry where either the 2D or 3D point is not used is not accepted.
	@return False, if corresponding point sets is less than 6, if one of the given
		parameters is a NULL pointer, or if the pPointSets contains non complete entries.
	@see k_Calibrate */
bool k_CalibrateCamera(const T3D2DPoints* pPointSets, TCameraMatrix* pMat);

/** Finds the 2D position in image from a given 3D position and the camera matrix.
	@param w World coordinate.
	@param pMat Precalculated camera matrix.
	@param pPnt Position in image returned in this parameter. */
void k_Calc3DTo2D(T3DFloat w,const TCameraMatrix* pMat, T2DFloat* pPnt);

/** Calculates direction from p through Pinhole.
	@param p Position in 2D image.
	@param pMat Precalculated camera matrix.
	@param pDir 3D direction returned in this parameter. */
void k_Direction(T2DFloat p,const TCameraMatrix* pMat,T3DFloat* pDir);

/** Calculates the pinhole world coordinate.
	@param pMat Precalculated camera matrix.
	@param pPinHole Pinhole world coordinate returned in this parameter. */
void k_PinHole(const TCameraMatrix* pMat, T3DFloat* pPinHole);

/** Performs a normal calibration as the corresponding k_Calibrate() function in IPL98.
	In addition to that a radial distortion parameter k is estimated, see general 
	comments for more information. Note: If you don't want
	distortion corrections just use the normal k_Calibrate() function.
	@param pPointSets Contains the sets of corresponding 2D and 3D coordinates,
		at least 6 must be present.
	@param pMat The camera parameters are returned in this variable.
	@param k The distortion parameter is returned in this variable.
	@param CenterX Half image width used as center for radial distortion.
	@param CenterY Half image height used as center for radial distortion.
	@return False, if number of point sets is less than 6, if one of the given
			parameters is a NULL pointer, or if the pPointSets contains non complete entries. */
bool k_CalibrateWithRadialDist(const T3D2DPoints* pPointSets,TCameraMatrix* pMat, 
								double* k, FLOAT32 CenterX, FLOAT32 CenterY);

/** Finds the 2D position in image from a given 3D position, the camera matrix and
	the distortion parameter k. Use this function when working with an image not
	corrected for the radial distortion and a camera matrix and k-value calculated
	by one of the functions k_CalibrateWithRadialDist() or k_FindMarksAutoGlobalThreshold().
	For a given 3D world position you will get the corresponding point in your
	original radial distorted image.
	@param w World coordinate.
	@param pMat Precalculated camera matrix.
	@param k Distortion parameter, calculated by k_CalibrateWithRadialDist().
	@param pPnt Position in image returned in this parameter.
	@param CenterX Half image width used as center for radial distortion.
	@param CenterY Half image height used as center for radial distortion. */
void k_Calc3DTo2DRad(T3DFloat w, const TCameraMatrix* pMat, double k, T2DFloat* pPnt, FLOAT32 CenterX, FLOAT32 CenterY);

/** Returns the maximum radial distortion displacement for a postion in image with
	given distortion parameter k. Done by displacing a corner position which is the
	position furthest away from center.
	@param pImg The function needs the image to calculate the center (width/2, height/2).
	@param k Distortion parameter.
	@param CenterX Half image width used as center for radial distortion.
	@param CenterY Half image height used as center for radial distortion.
	@return Maximum displacement relative to center of image. */
T2DInt k_GetMaxRadialDisplacement(double k, FLOAT32 CenterX, FLOAT32 CenterY);

/** Removes the radial distortion from the image.
	@param pDest Destination of new image, may be the same as pSource. Note:
		In case of same source and destination the algorithm is a bit slower.
	@param pSource Source image, may be the same as pDest. Note:
		In case of same source and destination the algorithm is a bit slower.
	@param k Distortion parameter, calculated by k_CalibrateWithRadialDist().
	@param PreserveImageSize if true, the destination image has same dimensions as source.
		Otherwise the new image will have the dimension needed to represent the radial corrected
		image without loss of information.
	@return False if pImg is not an 8 bit gray tone image or if pMat is a NULL pointer */
bool k_RemoveRadialDistortion(TImage* pDest, TImage* pSource, double k, bool PreserveImageSize);

/** For a given position (x,y) a new radial corrected position is returned, that is the position
	in a corresponding image without radial distortions. Center of radial distortion is assumed
	to be center of image. The inverse correction is performed by the function k_GetPosInverseRad().
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@param k The distortion parameter calculated by k_CalibrateWithRadialDist().
	@param CenterX Half image width used as center for radial distortion.
	@param CenterY Half image height used as center for radial distortion.
	@return Radial corrected 2d position.
	@see k_GetPosInverseRad */
T2DFloat k_GetPosRadRemoved(FLOAT32 x,FLOAT32 y, double k, FLOAT32 CenterX, FLOAT32 CenterY);

/** For a given position (x,y) a new inverse radial corrected position is returned, 
	that is the position in a corresponding image with radial distortions. Center of
	radial distortion is assumed to be center of image. This functionality is the
	opposite of k_GetPosRadRemoved().
	@param xr The position in horisontal direction.
	@param yr The position in vertical direction.
	@param k The distortion parameter calculated by k_CalibrateWithRadialDist().
	@param CenterX Half image width used as center for radial distortion.
	@param CenterY Half image height used as center for radial distortion.
	@return Inverse radial corrected 2d position.
	@see k_GetPosRadRemoved */
T2DFloat k_GetPosInverseRad(FLOAT32 xr, FLOAT32 yr, double k, FLOAT32 CenterX, FLOAT32 CenterY);

/** Print the camera matrix contained in a TCameraMatrix structure to stdout.
	@param pMat camera matrix
	@param WithIndexes True to print indexes as given in Gonzalez & Woods p. 76 */
void k_PrintCameraMatrix(const TCameraMatrix* pMat, bool WithIndexes);

/** The camera parameters derived in the PhD Thesis
    "Industrial Vision" by Ole Knudsen, 1997, chapter 2.2. */
typedef struct {
	/** @param dx x coordinate of focal centre */
	double dx;
	/** @param dy y coordinate of focal centre */
	double dy;
	/** @param dz z coordinate of focal centre */
	double dz;
	/** @param a rotation of camera around the x-axis [rad] */
    double a;
	/** @param b rotation of camera around the y-axis [rad] */
	double b;
	/** @param c rotation of camera around the z-axis [rad] */
	double c;
	double f;
	/** @param FocalLength [pixels] */
	double FocalLength;
	/** @param xh x-coordinate of origin of the focal on the CCD chip [pixels] */
	double xh;
	/** @param yh y-coordinate of origin of the focal on the CCD chip [pixels] */
	double yh;
	/** @param p aspect ratio. Scale of V relative to U */
	double p;
	/** @param beta Skewness of V relative to U */
	double beta;
} TCameraParameters;

/** Derives the camera parameters using the method presented in the
	PhD Thesis "Industrial Vision" by Ole Knudsen, 1997, chapter 2.2.
	@param pMat Precalculated camera matrix.
	@param pPar The camera parameters are returned in this variable.
	@return False, if no solution is found to angle a,b,c or beta.
	@version 0.5
	@author Implementation by Brian Kirstein Ramsgaard (kirstein@protana.com) */
bool k_MatrixToParameters(const TCameraMatrix* pMat, TCameraParameters* pPar);

/** Builds the camera matrix form the camera parameters using the method
	presented in the PhD Thesis "Industrial Vision" by Ole Knudsen, 1997,
	chapter 2.2.
	@param pPar Precalculated camera parameters.
	@param pMat The camera matrix is returned in this variable.
	@return False, if the value of matrix element a44 is zero.
	@version 0.5
	@author Implementation by Brian Kirstein Ramsgaard (kirstein@protana.com) */
bool k_ParametersToMatrix(const TCameraParameters* pPar, TCameraMatrix* pMat);

/** Print the primary camera parameters contained in a TCameraParameters structure to stdout.
	@param pPar camera parameters */
void k_PrintCameraParameterInfo(const TCameraParameters* pPar);

/*@}*/ /* end group "perspective" */

/** @name Old functions kept for backward compatibility.
	The following functions are only kept to ensure backward compatibility,
	it is strongly recommended to use other methods in this class. */
/*@{*/ 
/** IMPORTANT: Only kept for backward compatibility, use other functions!
	Performs a normal calibration as the corresponding k_Calibrate() function in IPL98.
	In addition to that a radial distortion parameter k is estimated, see general 
	comments for more information. Note: If you don't want
	distortion corrections just use the normal k_Calibrate() function.
	@param pPointSets Contains the sets of corresponding 2D and 3D coordinates,
		at least 6 must be present.
	@param pMat The camera parameters are returned in this variable.
	@param k The distortion parameter is returned in this variable.
	@param pImg The function needs the image to calculate the center (width/2, height/2).
	@return False, if number of point sets is less than 6, if one of the given
			parameters is a NULL pointer, or if the pPointSets contains non complete entries. */
bool k_CalibrateWithRadialDistOld(T3D2DPoints* pPointSets,TCameraMatrix* pMat, 
							   double* k,const TImage* pImg);
/** IMPORTANT: Only kept for backward compatibility, use other functions!
	Finds the 2D position in image from a given 3D position, the camera matrix and
	the distortion parameter k. Use this function when working with an image not
	corrected for the radial distortion and a camera matrix and k-value calculated
	by one of the functions k_CalibrateWithRadialDist() or k_FindMarksAutoGlobalThreshold().
	For a given 3D world position you will get the corresponding point in your
	original radial distorted image.
	@param w World coordinate.
	@param pMat Precalculated camera matrix.
	@param k Distortion parameter, calculated by k_CalibrateWithRadialDist().
	@param pPnt Position in image returned in this parameter.
	@param pImg The function needs the image to calculate the center (width/2, height/2). */
void k_Calc3DTo2DRadOld(T3DFloat w, const TCameraMatrix* pMat, double k, T2DFloat* pPnt, const TImage* pImg);
/** IMPORTANT: Only kept for backward compatibility, use other functions!
	Returns the maximum radial distortion displacement for a postion in image with
	given distortion parameter k. Done by displacing a corner position which is the
	position furthest away from center.
	@param pImg The function needs the image to calculate the center (width/2, height/2).
	@param k Distortion parameter.
	@return Maximum displacement relative to center of image. */
T2DInt k_GetMaxRadialDisplacementOld(const TImage* pImg, double k);
/** IMPORTANT: Only kept for backward compatibility, use other functions!
	For a given position (x,y) a new radial corrected position is returned, that is the position
	in a corresponding image without radial distortions. Center of radial distortion is assumed
	to be center of image. The inverse correction is performed by the function k_GetPosInverseRad().
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@param pImg The function needs the image to calculate the center (width/2, height/2).
	@param k The distortion parameter calculated by k_CalibrateWithRadialDist().
	@return Radial corrected 2d position.
	@see k_GetPixelRadRemovedC
	@see k_GetPosInverseRad */
T2DFloat k_GetPosRadRemovedOld(FLOAT32 x,FLOAT32 y,const TImage* pImg, double k);
/** IMPORTANT: Only kept for backward compatibility, use other functions!
	Same as k_GetPosRadRemoved, but here the center of image is given as parameters
	instead of an image.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@param CenterX Horisontal center (width/2 of image)
	@param CenterY Vertical center (height/2 of image).
	@param k The distortion parameter calculated by k_CalibrateWithRadialDist().
	@return Radial corrected 2d position.
	@see k_GetPosRadRemoved */
T2DFloat k_GetPosRadRemovedCOld(FLOAT32 x,FLOAT32 y, FLOAT32 CenterX, FLOAT32 CenterY, double k);
/** IMPORTANT: Only kept for backward compatibility, use other functions!
	For a given position (x,y) a new inverse radial corrected position is returned, 
	that is the position in a corresponding image with radial distortions. Center of
	radial distortion is assumed to be center of image. This functionality is the
	opposite of k_GetPosRadRemoved().
	@param xr The position in horisontal direction.
	@param yr The position in vertical direction.
	@param k The distortion parameter calculated by k_CalibrateWithRadialDist().
	@return Inverse radial corrected 2d position.
	@see k_GetPosRadRemoved
	@see k_GetPosInverseRadC */
T2DFloat k_GetPosInverseRadOld(FLOAT32 xr, FLOAT32 yr, const TImage* pImg, double k);
/** IMPORTANT: Only kept for backward compatibility, use other functions!
	Same as k_GetPosInverseRad, but here the center of image is given as parameters
	instead of an image.
	@param xr The position in horisontal direction.
	@param yr The position in vertical direction.
	@param k The distortion parameter calculated by k_CalibrateWithRadialDist().
	@return Inverse radial corrected 2d position.
	@see k_GetPosInverseRad
	@see k_GetPosRadRemoved */
T2DFloat k_GetPosInverseRadCOld(FLOAT32 xr, FLOAT32 yr, FLOAT32 CenterX, FLOAT32 CenterY, double k);

/*@}*/ /* end group "Old functions" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_PERSPECTIVE_H */
