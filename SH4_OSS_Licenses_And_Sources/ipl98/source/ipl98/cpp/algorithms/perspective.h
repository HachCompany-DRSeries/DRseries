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

#ifndef _IPL98_PERSPECTIVE_H
#define _IPL98_PERSPECTIVE_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../std_image.h"
#include "../corresponding_3d2d_points.h"
#include "../../kernel_c/algorithms/kernel_perspective.h"
#include "../../../points/point2d.h"
#include "../../../points/point3d.h"
#include "../geometry/line3d.h"
#include <vector>
#include <istream>

namespace ipl{ // use namespace if C++

using std::vector;
using std::istream;

/** @ingroup algorithms
@{ */

/** CPerspective Camera calibration and 3D->2D/2D->3D perspective methods, Last updated 12/29/2005.
	Contains a normal linear camera calibration, corresponding 3D-2D points must be
	available as input to the calibration routine. In addition to the normal linear calibration,
	a radial distortion	parameter k can be estimated by calling the method CalibrateWithRadialDist(). 
	The distortion is described as: Dx=kx(x^2+y^2) and Dy=ky(x^2+y^2), where the point (x,y) is
	according to the center of the image. It could be discussed if the optical center (xh,yh) should
	be used instead, but for various reasons we use the image center at the moment. A set of methods 
	is provided for dealing with the radial distortion after calibration, most important are:
	Calc3DTo2DRad(), RemoveRadialDistortion(), GetPosRadRemoved(), and GetPosInverseRad(). With these
	methods it is possible to find correspondance between image points and 3D points including the
	radial distortion parameter, when using the same camera. Example:

	\verbatim
	// The container holding the corresponding 3D-2D values.
	CCorresponding3D2DPoints PointSets;

	// Insert some code that fill out the PointSets variable with at least six sets
	// .... 

	// Creates a CPerspective object, setting image size
	CPerspective Persp(768,576);

	if (Persp.CalibrateWithRadialDist(PointSets)==true)
	{
		// print the calibration result
		cout << Persp << endl;
		cout << "Creating image without radial distortion..." << endl;
		CImage Source,Dest;
		Source.Load("a_source_image_to_be_interpreted.bmp");
		// Remove radial distortion from whole image
		// Note: It is a time consuming operation, if possible stay in original
		// image and use GetPosRadRemoved() to get positions without radial
		// distortion
		Persp.RemoveRadialDistortion(Dest,Img,false);
		// Dest now contains image without radial distortion
		Dest.Save("NoRadialDistortion.bmp");
		// let's say an algorithm finds a relevant point P in Dest
		CPoint2D<FLOAT32> P(100,200); // A point found in Dest
		// Can now get direction in 3D world coordinates
		CPoint3D<FLOAT32> Dir;
		Persp.Direction(P,Dir);
		// Position in original image (POriginal) can also be found
		CPoint2D<FLOAT32> POriginal;
		POriginal=Persp.GetPosInverseRad(P);
	}
	else
		cout << "Calibration failed" << endl;
	\endverbatim

	Note: Since version 1.34, this class has undergone a major rewrite. The image size is now given
	to the constructor. A bug fix means that the radial distortion parameter found is better, it is 
	highly recommended to use versions of this class later than 1.34. Since version 1.63, the old
	methods have been deprecated.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CPerspective ipl98/cpp/algorithms/perspective.h
	@version 1.73
	@author Ivar Balslev (ivb@mip.sdu.dk), Brian Kirstein (kirstein@protana.com) and
		René Dencker Eriksen (edr@mip.sdu.dk) */
class CPerspective{
	public: // attributes
		/** True if camera parameters has been calculated by MatrixToParameters(). 
			@see MatrixToParameters */
        bool m_CamParametersAvailable; // added by kirstein@protana.com
		/** A C structure containing the camera parameters calculated by the method
			MatrixToParameters(), see the C-documentation for members of the structure. */
        TCameraParameters m_Par; // added by kirstein@protana.com
	protected: // attributes
		/** camera parameter containing the linear calibrated values. */
		TCameraMatrix m_Cam;
		/// set to true if any of the calibration methods or the load method succeeds.
		bool m_CamMatrixCalibrated;
		/** true if CalibrateWithRadialDist() succeded or the load method found a radial value. */
		bool m_RadCalibrationPerformed;
		/** the radial distortion parameter calculated by CalibrateWithRadialDist() */
		double m_k;
		/** Set to true when image dimensions are available. */
		bool m_ImageDimInitialized;
		/** Half image dimension in X-direction. Set by constructor. */
		FLOAT32 m_CenterX;
		/** Half image dimension in Y-direction. Set by constructor. */
		FLOAT32 m_CenterY;
		/** Current version of this class, defined in perspective.cpp. */
		static const double m_Version;
	public: // methods
		/** Default constructor. Note, when using the default constructor,
			the method SetImageDimensions() must be called at a later point
			to initialize this class. */
		CPerspective();
		/** Constructor setting the image size to be used in this object.
			@param Img Must contain an image with dimensions to be used in this object,
				if image not available yet, call other constructor passing image dimensions
				as scalars. */
		CPerspective(const CStdImage& Img);
		/** Constructor setting the image size to be used in this object.
			@param Width Width of images to be used for this object.
			@param Height Height of images to be used for this object. */
		CPerspective(unsigned int Width, unsigned int Height);
		/** Same as constructor CPerspective(Width,Height), except
			image dimensions are given as a CPoint2D<unsigned int> type. */
		CPerspective(CPoint2D<unsigned int> Dimensions);
		/// Copy constructor
		CPerspective(const CPerspective& Source);
		/// default destructor
		virtual ~CPerspective();

		/** Overloading of assignment operator. All information is copied to the 
			destination. Previous data in this object are destroyed. An error is
			produced if both sides of the assignment operator is the same object. */
		CPerspective& operator=(const CPerspective& Source);

		/** Calibrates the camera matrix as given in Gonzalez & Woods page 67. This
			is a simple linear calibration, call CalibrateWithRadialDist() to get
			a calibration with radial distortion included. 3D world coordinates	and 
			corresponding 2D ccd coordinates are given in PointSets in order to
			calibrate. If succesfull, parameters are calculated by calling
			MatrixToParameters().
			@param PointSets 3D and 2D corresponding coordinates, all 3D and Id entries must
				be in use whereas a set with an unused 2D point automatically will be ignored.
				At least 6 complete point sets must be present.
			@return False, if corresponding point sets is less than 6, or if the
					PointSets contains non complete entries.
			@see CalibrateWithRadialDist
			@see MatrixToParameters */
		bool Calibrate(const CCorresponding3D2DPoints& PointSets);

		/** Sets all parameters in this class, previous values are destroyed. 
			The internal attributes m_CamMatrixCalibrated and
			m_RadCalibrationPerformed are set to true. Note: Camera parameters are
			not calculated, must call MatrixToParameters() to make them available. */
		bool Set(const TCameraMatrix& Cam, double k, unsigned int Width, unsigned Height);
		/** Sets the camera matrix attribute to the values suplied in Cam. Use this method
			if you want to provide an external camera matrix to this class. The internal
			attribute m_CamMatrixCalibrated is set to true and m_RadCalibrationPerformed is
			set to false.
			@param Cam Camera matrix, members are a11, a12, .... a44. */
		void SetCameraMatrix(const TCameraMatrix& Cam);

		/** Sets the camera matrix attribute to the values suplied in Cam and the radial
			distortion value m_k to the suplied k. Use this method
			if you want to provide an external camera matrix and radial distortion parameter
			to this class. The internal	attributes m_CamMatrixCalibrated and
			m_RadCalibrationPerformed are set to true.
			@param Cam Camera matrix, members are a11, a12, .... a44
			@param k Radial distortion parameter */
		void SetCameraMatrixAndRadDistortion(const TCameraMatrix& Cam, double k);

		/** Finds the 2D position in image with origo in upper left corner, from a given
			3D position and the precalculated camera matrix. The method Calibrate() must
			be called first and image dimensions must be initialized.
			@param w World coordinate.
			@param Pnt Position in image returned in this parameter.
			@return False, if the camera matrix has not been calculated.
			@see Calc3DTo2DRad */
		bool Calc3DTo2D(const CPoint3D<FLOAT32>& w, CPoint2D<FLOAT32>& Pnt) const;

		/** Calculates direction from p through Pinhole. Note: The type of Dir
			should have been a CVector3D, but for historical reasons it is only
			a CPoint3D.
			@param p Position in 2D image with origo in upper left corner. If radial
				distortion is included in the calibration, the user must ensure that
				p is a position without radial distortion.
			@param Dir 3D direction returned in this parameter. The vector
				is normalized, i.e. has length one.
			@return False, if the camera matrix has not been calculated or if
				image dimensions are not available. */
		bool Direction(const CPoint2D<FLOAT32>& p,CPoint3D<FLOAT32>& Dir) const;

		/** Same as Pinhole but here the 3D point is returned directly, the
			user must asure that the parameters are available.
			@return Pinhole world coordinate. */
		CPoint3D<FLOAT32> GetPinHole() const;

		/** Calculates the pinhole world coordinate.
			@param PinHole Pinhole world coordinate returned in this parameter.
			@return False, if camera parameters has not yet been calculated. */
		bool PinHole(CPoint3D<FLOAT32>& PinHole) const;

		/** Returns the 3D line from the given image point P through the pinhole.
			An error message is produced if calibration parameters are not
			available.
			@param P Point in image after removal of radial distortion. Origo
				in upper left corner of image.
			@return 3D line from point P through pinhole */
		CLine3D<float> GetRay(const CPoint2D<float>& P) const;

		/** Returns the 3D line from optical center (xh,yh) through pinhole.
			An error message is produced if calibration parameters are not
			available.
			@return 3D line from optical center through pinhole. */
		CLine3D<float> GetOpticalAxis() const;


		/** Returns the radial distortion parameter contained in the attribute m_k.
			Note: If calibration with radial distortion has not yet been performed
			the value is undefined. Use RadCalibrationPerformed() to check if it is available.*/
		inline double GetRadDistParameter() const;

		/** Performs a normal calibration as the corresponding Calibrate() method.
			In addition to that a radial distortion parameter k is estimated, see general 
			comments for more information. Note: If you don't want
			distortion corrections just use the normal Calibrate() method.
			If succesfull, parameters are calculated by calling MatrixToParameters().
			@param PointSets 3D and 2D corresponding coordinates, all 3D and Id entries must
				be in use whereas a set with an unused 2D point automatically will be ignored.
				At least 6 complete point sets must be present.
			@param PointSets Contains the sets of corresponding 2D and 3D coordinates,
				at least 6 must be present.
			@return False, if corresponding point sets is less than 6, or if the
					PointSets contains non complete entries.
			@see MatrixToParameters */
		bool CalibrateWithRadialDist(const CCorresponding3D2DPoints& PointSets);

		/** Finds the 2D position in image from a given 3D position, the camera matrix and
			the distortion parameter k. Use this function when working with an image not
			corrected for the radial distortion and a camera matrix and k-value calculated
			by one of the functions CalibrateWithRadialDist() or FindMarksAutoGlobalThreshold().
			For a given 3D world position you will get the corresponding point in your
			original radial distorted image.
			@param w World coordinate.
			@param Pnt Position in image with origo in upper left corner, returned in this parameter.
			@return False, if calibration with radial distortion has not been performed.
			@see Calc3DTo2D */
		bool Calc3DTo2DRad(const CPoint3D<FLOAT32>& w, CPoint2D<FLOAT32>& Pnt) const;

		/**	Returns the maximum radial distortion displacement for a postion in image with
			given distortion parameter k. Done by displacing a corner position which is the
			position furthest away from center.
			@return Maximum displacement relative to center of image. Returns a value of (-1,-1)
				if calibration has not been performed. */
		CPoint2D<int> GetMaxRadialDisplacement() const;

		/** Removes the radial distortion k from the image.
			@param Dest Destination of new image, may be the same as Source. Note:
				In case of same source and destination the algorithm is a bit slower.
			@param Source Source image, may be the same as Dest. Note:
				In case of same source and destination the algorithm is a bit slower.
			@param PreserveImageSize if true, the destination image has same dimensions as source.
				Otherwise the new image will have the dimension needed to represent the radial corrected
				image without loss of information.
			@return False, if Img is not an 8 bit gray tone image. */
		bool RemoveRadialDistortion(CStdImage& Dest, CStdImage& Source, bool PreserveImageSize);

		/** Removes the radial distortion from the image with an external calculated k value.
			When using this method, the camera need not be calibrated with the methods
			in this class.
			@param Dest Destination of new image, may be the same as Source. Note:
				In case of same source and destination the algorithm is a bit slower.
			@param Source Source image, may be the same as Dest. Note:
				In case of same source and destination the algorithm is a bit slower.
			@param k The radial distortion parameter. 
			@param PreserveImageSize if true, the destination image has same dimensions as source.
				Otherwise the new image will have the dimension needed to represent the radial corrected
				image without loss of information.
			@return False, if a radial calibration has not been performed yet. */
		bool RemoveRadialDistortionExtK(CStdImage& Dest, CStdImage& Source, double k, bool PreserveImageSize);

		/** Changes the image dimensions used in this class. When calling this, all previous
			calibration is canceled since the radial distortion parameter was found according
			to original image center, hence a new calibration is needed.
			@return False, if Width or Height is zero. Only available in C++. */
		bool SetImageDimensions(unsigned int Width, unsigned int Height);

		/** Same as SetImageDimensions(Width,Height), except width and height is given 
			as a CPoint2D type. */
		bool SetImageDimensions(CPoint2D<unsigned int> Dimensions);

		/** Returns the image dimensions in the provided parameters. Only
			available in C++.
			@return False, if image dimensions are not initialized. */
		bool GetImageDimensions(unsigned int &Width, unsigned int &Height) const;

		/** Returns the image dimensions set for this class. Only available in C++.
			If image dimensions are not initialized, an error message will be
			produced and the return value will be undefined. */
		CPoint2D<unsigned int> GetImageDimensions() const;

		/** Returns true, if image dimensions are available. */
		inline bool ImageDimensionsInitialized() const;

		/** Check if a calibration has been performed, i.e. either Calibrate()
			or CalibrateWithRadialDist().
			@return true, if a calibration has been performed. */
		bool CalibrationPerformed() const;

		/** Check if a radial calibration has been performed.
			@return true, if a radial calibration has been performed */
		bool RadCalibrationPerformed() const;

		/** For a given position (x,y) a new radial corrected position is returned,
			that is the position in a corresponding image without radial distortions.
			The inverse correction is performed by InverseRadialMove.  Image
			dimensions must be initialized.
			@param x The position in horisontal direction to be corrected. Origo in
				upper left corner.
			@param y The position in vertical direction to be corrected. Origo in
				upper left corner.
			@return Radial corrected 2d position. If radial calibration has not been
				performed, a value of (-1,-1) will be returned and an error message
				is produced.
			@see InverseRadialMove */
		CPoint2D<FLOAT32> GetPosRadRemoved(FLOAT32 x,FLOAT32 y) const;

		/** Same as GetPosRadRemoved, but here parameters are given as CPoint2D
			types.
			@param Pos The position to be corrected.
			@return Radial corrected 2d position. Origo in upper left corner. If
				radial calibration has not been performed, a value of (-1,-1) 
				will be returned and an error message is produced.
			@see GetPosRadRemoved */
		CPoint2D<FLOAT32> GetPosRadRemoved(const CPoint2D<FLOAT32>& Pos) const;

		/** For a given position (x,y) a new inverse radial corrected position is returned, 
			that is the position in a corresponding image with radial distortions. Center of
			radial distortion is assumed to be center of image. This functionality is the
			opposite of GetPosRadRemoved().
			@param xr The position in horisontal direction. Origo in upper left corner.
			@param yr The position in vertical direction. Origo in upper left corner.
			@return Inverse radial corrected 2d position. Origo in upper left corner. If
				radial calibration has not been performed, a value of (-1,-1)
				will be returned and an error message is produced.
			@see GetPosRadRemoved */
		CPoint2D<FLOAT32> GetPosInverseRad(FLOAT32 xr, FLOAT32 yr) const;
		
		/** Same as GetPosInverseRad, but here parameters are given as CPoint2D types.
			@param PosRad The position to be corrected. Origo in upper left corner.
			@return Inverse radial corrected 2d position. Origo in upper left corner. If
				radial calibration has not been performed, a value of (-1,-1)
				will be returned and an error message is produced.
			@see GetPosInverseRad */
		CPoint2D<FLOAT32> GetPosInverseRad(const CPoint2D<FLOAT32>& PosRad) const;

		/** Returns the camera matrix values contained in m_Cam at (i,j) starting in 
			upper left corner with index (1,1) as in Gonzales & Woods.
			@param i coloumn index.
			@param j row index.
			@return Value at (i,j) in camera matrix. */
		FLOAT32 GetCamMatrix(unsigned short i, unsigned short j) const;

		/** Gets the camera matrix pointer to the attribute m_Cam. Use this method to manipulate
			the camera matrix on your own. The fields starts from "a11" and goes to "a44".
			Note: Use RadCalibration() to check if it is available.
			@return The camera matrix.
			@see GetConstTCameraMatrixPtr */
		inline TCameraMatrix* GetTCameraMatrixPtr();

		/** Gets the constant camera matrix pointer to the attribute m_Cam. Use this method to access
			the camera matrix directly. The fields starts from "a11" and goes to "a44".
			Note: Use RadCalibration() to check if it is available.
			@return The camera matrix.
			@see GetTCameraMatrixPtr */
		inline const TCameraMatrix* GetConstTCameraMatrixPtr() const;

		/** Print the camera matrix contained in the m_Cam attribute to stdout.
			@param WithIndexes True to print indexes as given in Gonzalez & Woods p. 76 */
		void PrintCameraMatrix(bool WithIndexes) const;

		/** Prints the errors for each point to cout, done by calling the method ErrorsToStream().
			@return False, if a calibration has not been performed yet.
			@see ErrorsToStream
			@see GetErrors */
		bool PrintErrors(const CCorresponding3D2DPoints& PointSets) const;

		/** Prints the errors for each point to "stream" when the 3D world points are
			projected to image points using the camera matrix m_Cam (including radial
			distortion correction if it has been performed) and	compared to the found
			2D points in the parameter PointSets.
			@param PointSets Corresponding 3D-2D point sets.
			@param Stream The ostream to stream the output to.
			@return False, if a calibration has not been performed yet.
			@see GetErrors */
		bool ErrorsToStream(const CCorresponding3D2DPoints& PointSets, ostream& Stream) const;

		/** Returns a list of errors, in the Error parameter, for each point when the 3D world 
			points are projected to image points using the camera matrix and radial distortion
			parameter (if radial calibration has been performed. For visual inspection of errors
			call PrintErrors() or ErrorsToStream().
			@param PointSets Corresponding 3D-2D point sets.
			@param Errors A vector containing all errors, previous content of the vector is deleted.
			@return False, if no calibration is available.
			@see PrintErrors()
			@see ErrorsToStream()
			@version 0.8 */
		bool GetErrors(const CCorresponding3D2DPoints& PointSets, vector<CPoint2D<FLOAT32> >& Errors);

		/** Returns an overall error as sqrt((dx1^2+dy1^2 + dx2^2+dy2^2 ...)/Total), where
			dx1=P2d-PWorldProj etc. and Total is the total number of calibration points.
			Here P2d is the detected point in the image and PWorldProj is the 3D world point
			projected into the image using the calibration matrix and the radial distortion
			parameter. For individual errors, see GetErrors().
			@return Total calibration error. */
		float GetTotalError(const CCorresponding3D2DPoints& PointSets);

		/** Prints the camera parameters contined in the m_Par attribute to "stream".
			These parameters are calculated by the method MatrixToParameters(), if
			it has not yet been calculated an error is streamed instead.
			Calls StreamIndent() for each streamed line.
			@param Stream The ostream to stream the output to.
			@return False, if a calibration and MatrixToParameters() has not been performed yet. */
		bool CamParametersToStream(ostream& Stream) const;

		/** writes object to stream. If no calibration data available, the output
			after '##Camera Matrix' is not written to output (see format below).
			If radial distortion has not been calibrated, the distortion parameter
			is not written to file. Format is:
			\verbatim
			# Calibration data file written by the CPerspective class from IPL98
			#
			# CPerspective class version
			PerspectiveVersion 1.63

			# Image dimensions 
			ImageDimensionsInitialized: 1
			Width 640
			Height 480

			# Camera matrix
			CalibrationPerformed: 1
			0.340185   0.00673051   0.172915   110.711
			-0.00540749   0.345476   0.118377   120.463
			0   0   0   0
			-9.4659e-006   1.07785e-005   0.000524122   1

			# k radial distortion parameter
			RadCalibrationPerformed: 1
			k 9.39704e-007

			# 
			# **************** TCameraParameters info ********************
			#  x coordinate of pin-hole (dx) = 635.605
			#  y coordinate of pin-hole (dy) = 313.294
			#  z coordinate of pin-hole (dz) = -1902.92
			#  rotation of camera around the x-axis (a) = -0.020562[rad] = -1.17811 [deg]
			#  rotation of camera around the y-axis (b) = -0.0180547[rad] = -1.03446 [deg]
			#  rotation of camera around the z-axis (c) = -3.13235[rad] = -179.47 [deg]
			#  focal length = 654.694 [pixels] 
			#  x-coordinate of the focal centre on the CCD chip (xh) = 318.217 [pixels]
			#  y-coordinate of the focal centre on the CCD chip (yh) = 239.42 [pixels]
			#  aspect ratio, scale of V relative to U (p) = 0.999172
			#  skewness of V relative to U (beta) = 8.61891e-005 [rad] = 0.00493827 [deg]			
			\endverbatim
			Calls StreamIndent() for each streamed line.
			@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
			@version 0.1 */
		friend ostream& operator<<(ostream& s,const CPerspective& Persp);
		/** Reads input from given stream. Stream must be in correct format 
			according to how the << ostream operator formats output.
			@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
			@version 0.1 */
		friend std::istream& operator>>(istream& is, CPerspective& Persp);

		/** Returns true, if MatrixToParameters has been successfully called. */
		bool ParametersAvailable() const;

        /** Derives the camera parameters from a precalculated camera
            matrix using the method presented in the PhD Thesis
            "Industrial Vision" by Ole Knudsen, 1997, chapter 2.2.
			The results are stored in the attribute m_Par. In addition the public attribute
			m_CamParametersAvailable is set to true.
	        @return False, if the camera matrix has not been calculated.
            @version 0.6
            @author Implementation by Brian Kirstein Ramsgaard (kirstein@protana.com).
			@see m_Par */
        virtual bool MatrixToParameters();

        /** Builds the camera matrix from the precalculated camera
            parameters using the method presented in the PhD Thesis
            "Industrial Vision" by Ole Knudsen, 1997, chapter 2.2.
            @return False, if camera parameters are not available.
            @version 0.6
            @author Implementation by Brian Kirstein Ramsgaard
                (kirstein@protana.com) */
        bool ParametersToMatrix();
		

		void DeriveCameraPose(const CCorresponding3D2DPoints &PointSets, const CPerspective &InCamera, 
							  bool StartGuessAvailable);

		/** Load camera matrix and distortion parameter from file. Camera parameters are
			calculated, if loading succeeds, by calling MatrixToParameters.	If version of
			calibration file is different than this class version, a warning is given.
			Comment lines starts with a '#' character, can be placed anywhere.
			See the streaming operator operator<<(ostream&,const CPerspective&) for
			exact format of output.
			Note: This methods is only available for the C++ part.
			@param pPathAndFileName Name of file including extension and relative
				or absolute path.
			@return False, if file could not be opened or if the format is not correct.
			@version 0.9 */
		bool Load(const char* pPathAndFileName);

		/** Save camera matrix, distortion parameter and version number of this calibration
			class. Output is formated according to the format of operator<<(ostream&,const CPerspective&).
			Note: This methods is only available for the C++ part.
			See the streaming operator operator<<(ostream&,const CPerspective&) for exact format
			of output.
			@param pPathAndFileName Name of file and relative or absolute path. If no extension 
				is supplied in the filename a "cfg" extension is automatically added.
			@param pComments Extra comments to be added to the beginning of the file. No need
				to insert a leading '#', the method will do that. If you want more than one
				line use '\n' control character to split.
			@return False, if file could not be opened for writing or if no calibration
				data available.
			@version 0.9 */
		bool Save(const char* pPathAndFileName, const char* pComments=NULL) const;

		/** Returns the version of this class. */
		static double GetVersion();

	protected: // methods
		/** Assigns an integer value with each parameter in m_Par. Index 0 is m_Par.a, 1 is m_Par.b etc.
			In addition, index=11 is m_k (radial dist), index=12 is m_CenterX, and index=13 is m_CenterY. */
		inline FLOAT32 GetParameter(int Index) const;
		/** Sets given parameter, see GetParameter() for more info. */
		inline bool SetParameter(int Index, FLOAT32 Value);
		/** Used by DeriveCameraPose(). */
		void UpdateExternal(const CCorresponding3D2DPoints& PointSets);
		/** Used by DeriveCameraPose(). */
		CPoint2D<FLOAT32> GetPartialDerivative(int ParameterIndex, const CCorresponding3D2DPoints& PointSets,
												int MarkIndex, const CPoint2D<FLOAT32>& P20);

};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

inline double CPerspective::GetRadDistParameter() const
{
	return m_k;
}

inline TCameraMatrix* CPerspective::GetTCameraMatrixPtr()
{
	return &m_Cam;
}

inline const TCameraMatrix* CPerspective::GetConstTCameraMatrixPtr() const
{
	return &m_Cam;
}

FLOAT32 CPerspective::GetParameter(int Index) const
{
	switch(Index)
	{
		case  0: return (FLOAT32)m_Par.a;
		case  1: return (FLOAT32)m_Par.b;
		case  2: return (FLOAT32)m_Par.c;
		case  3: return (FLOAT32)m_Par.dx;
		case  4: return (FLOAT32)m_Par.dy;
		case  5: return (FLOAT32)m_Par.dz;
		case  6: return (FLOAT32)m_Par.FocalLength; 
		case  7: return (FLOAT32)m_Par.xh; 
		case  8: return (FLOAT32)m_Par.yh; 
		case  9: return (FLOAT32)m_Par.p; 
		case 10: return (FLOAT32)m_Par.beta; 
		case 11: return (FLOAT32)GetRadDistParameter(); 
		case 12: return m_CenterX;
		case 13: return m_CenterY;
		default:
		{
			ostringstream ost;
			ost << "CPerspective::GetParameter() Index=" << Index << 
				" unknown! Returning 0!" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			return 0;
		}
	}
}

bool CPerspective::SetParameter(int Index, FLOAT32 Value)
{
	switch(Index)
	{
		case  0: m_Par.a=Value;break;
		case  1: m_Par.b=Value;break;
		case  2: m_Par.c=Value;break;
		case  3: m_Par.dx=Value;break;
		case  4: m_Par.dy=Value;break;
		case  5: m_Par.dz=Value;break;
		case  6:
			{
				m_Par.FocalLength=Value;
				m_Par.f=1.0/Value;
				break; 
			}
		case  7: m_Par.xh=Value;break; 
		case  8: m_Par.yh=Value;break; 
		case  9: m_Par.p=Value;break; 
		case 10: m_Par.beta=Value;break; 
		case 11: m_k=Value;break; 
		case 12: m_CenterX=Value;break; 
		case 13: m_CenterY=Value;break;
		default:
		{
			ostringstream ost;
			ost << "CPerspective::SetParameter() Index=" << Index << 
				" unknown!" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			return false;
		}
	}
	return true;
} 

bool CPerspective::ImageDimensionsInitialized() const
{
	return m_ImageDimInitialized;
}

/*@}*/ /* end group "algorithms" */

} // end namespace ipl

#endif //_IPL98_PERSPECTIVE_H
