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

#include "perspective.h"
#include "../ipl98_general_functions.h"
#include "../arrays/array2d.h"
#include "equation_solver.h"
#include <fstream>
#include <iomanip>
#include <ios>

namespace ipl{

using std::ios;
using std::ios_base;
using std::scientific;
using std::setprecision;
using std::setiosflags;
using std::setw;
using std::ofstream;
using std::cout;
using std::endl;

const double CPerspective::m_Version=1.63;

CPerspective::CPerspective()
{
	m_CamMatrixCalibrated=false;
    m_CamParametersAvailable=false;
	m_RadCalibrationPerformed=false;
	m_ImageDimInitialized=false;
}

CPerspective::CPerspective(const CStdImage& Img)
{
	m_CamMatrixCalibrated=false;
    m_CamParametersAvailable=false;
	m_RadCalibrationPerformed=false;
	m_CenterX=(FLOAT32)(Img.GetWidth()/2.0);
	m_CenterY=(FLOAT32)(Img.GetHeight()/2.0);
	m_ImageDimInitialized=true;
}

CPerspective::CPerspective(unsigned int Width, unsigned int Height)
{
	m_CamMatrixCalibrated=false;
    m_CamParametersAvailable=false;
	m_RadCalibrationPerformed=false;
	m_CenterX=(FLOAT32)(Width/2.0);
	m_CenterY=(FLOAT32)(Height/2.0);
	m_ImageDimInitialized=true;
}

CPerspective::CPerspective(CPoint2D<unsigned int> Dimensions)
{
	m_CamMatrixCalibrated=false;
    m_CamParametersAvailable=false;
	m_RadCalibrationPerformed=false;
	m_CenterX=(FLOAT32)(Dimensions.GetX()/2.0);
	m_CenterY=(FLOAT32)(Dimensions.GetY()/2.0);
	m_ImageDimInitialized=true;
}

CPerspective::CPerspective(const CPerspective& Source)
{
	*this=Source;
}

CPerspective::~CPerspective()
{
}

CPerspective& CPerspective::operator=(const CPerspective& Source)
{
	// overloading of assignment operator
	if (this != &Source)
	{
		// ok to do assignment
		m_CamParametersAvailable=Source.m_CamParametersAvailable;
		if (m_CamParametersAvailable==true)
			m_Par=Source.m_Par; // only need to copy Par structure if data available
		m_Cam=Source.m_Cam;
		m_CamMatrixCalibrated=Source.m_CamMatrixCalibrated;
		m_RadCalibrationPerformed=Source.m_RadCalibrationPerformed;
		m_k=Source.m_k;
		m_ImageDimInitialized=Source.ImageDimensionsInitialized();
		if (Source.ImageDimensionsInitialized()==true)
		{
			m_CenterX=Source.m_CenterX;
			m_CenterY=Source.m_CenterY;
		}
	}
	else
	{
		ostringstream ost;
		ost << "CPerspective::operator=() Cannot assign CPerspective object to itself" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return *this;
}

bool CPerspective::Calibrate(const CCorresponding3D2DPoints& PointSets)
{
	CCorresponding3D2DPoints PointSetsTemp(PointSets);
	PointSetsTemp.RemoveAllSets(false,true,false);
	if (!k_CalibrateCamera(PointSetsTemp.GetConstT3D2DPointsPtr(), &m_Cam))
	{
		ostringstream ost;
		ost << "CPerspective::Calibrate() Calibration failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		m_CamMatrixCalibrated=true;
		MatrixToParameters();
		return true;
	}
}

bool CPerspective::Set(const TCameraMatrix& Cam, double k, unsigned int Width, unsigned Height)
{
	m_Cam=Cam;
	m_k=k;
	m_CenterX=(FLOAT32)(Width/2.0);
	m_CenterY=(FLOAT32)(Height/2.0);
	m_ImageDimInitialized=true;
	m_CamMatrixCalibrated=true;
	m_RadCalibrationPerformed=true;
	m_CamParametersAvailable=false;
	return true;
}

void CPerspective::SetCameraMatrix(const TCameraMatrix& Cam)
{
	m_Cam=Cam;
	m_CamMatrixCalibrated=true;
	m_RadCalibrationPerformed=false;
	m_CamParametersAvailable=false;
}

void CPerspective::SetCameraMatrixAndRadDistortion(const TCameraMatrix& Cam, double k)
{
	m_Cam=Cam;
	m_k=k;
	m_CamMatrixCalibrated=true;
	m_RadCalibrationPerformed=true;
	m_CamParametersAvailable=false;
}

bool CPerspective::Calc3DTo2D(const CPoint3D<FLOAT32>& w, CPoint2D<FLOAT32>& Pnt) const
{
	if (m_CamMatrixCalibrated==false)
	{
		ostringstream ost;
		ost << "CPerspective::Calc3DTo2D() Camera matrix not yet calculated" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		T3DFloat AnsiW;
		AnsiW.x=w.GetX();
		AnsiW.y=w.GetY();
		AnsiW.z=w.GetZ();
		T2DFloat AnsiPnt;
		k_Calc3DTo2D(AnsiW,&m_Cam,&AnsiPnt);
		Pnt.Set(AnsiPnt.x,AnsiPnt.y);
		return true;
	}
}

bool CPerspective::Direction(const CPoint2D<FLOAT32>& p,CPoint3D<FLOAT32>& Dir) const
{
	if (m_CamMatrixCalibrated==false)
	{
		ostringstream ost;
		ost << "CPerspective::Direction() Camera matrix not yet calculated" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		T3DFloat AnsiDir;
		T2DFloat AnsiP;
		AnsiP.x=p.GetX();
		AnsiP.y=p.GetY();
		k_Direction(AnsiP,&m_Cam,&AnsiDir);
		Dir.Set(AnsiDir.x,AnsiDir.y,AnsiDir.z);
		return true;
	}
}

bool CPerspective::PinHole(CPoint3D<FLOAT32>& PinHole) const
{
	if (m_CamMatrixCalibrated==false)
	{
		ostringstream ost;
		ost << "CPerspective::PinHole() Camera matrix not yet calculated" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		T3DFloat AnsiPH;
		k_PinHole(&m_Cam, &AnsiPH);
		PinHole.Set(AnsiPH.x,AnsiPH.y,AnsiPH.z);
		return true;
	}
}

CPoint3D<FLOAT32> CPerspective::GetPinHole() const
{
	CPoint3D<FLOAT32> P;
	PinHole(P);
	return P;
}

CLine3D<float> CPerspective::GetRay(const CPoint2D<float>& P) const
{
	if (ParametersAvailable()==false)
	{
		ostringstream ost;
		ost << "CPerspective::GetRay() Camera parameters not available" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	CPoint3D<FLOAT32> DirP;
	Direction(P,DirP);
	CLine3D<float> L(GetPinHole(),CVector3D<float>(DirP.GetX(),DirP.GetY(),DirP.GetZ()));
	return L;
}

CLine3D<float> CPerspective::GetOpticalAxis() const
{
	return GetRay(CPoint2D<float>((float)m_Par.xh,(float)m_Par.yh));
}

bool CPerspective::CalibrateWithRadialDist(const CCorresponding3D2DPoints& PointSets)
{
	CCorresponding3D2DPoints PointSetsTemp(PointSets);
	PointSetsTemp.RemoveAllSets(false,true,false);
	bool ReturnValue;
	if (ImageDimensionsInitialized()==false)
	{
		ostringstream ost;
		ost << "CPerspective::CalibrateWithRadialDist() Image dimensions not initialized"
			<< IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	ReturnValue=k_CalibrateWithRadialDist(PointSetsTemp.GetConstT3D2DPointsPtr(),
											&m_Cam,&m_k,m_CenterX,m_CenterY);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CPerspective::CalibrateWithRadialDist() Failed calibration with "
			"radial distortion correction" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
	{
		m_RadCalibrationPerformed=true;
		m_CamMatrixCalibrated=true;
		MatrixToParameters();
	}
	return ReturnValue;
}

bool CPerspective::Calc3DTo2DRad(const CPoint3D<FLOAT32>& w, CPoint2D<FLOAT32>& Pnt) const
{
	if (m_RadCalibrationPerformed==false)
	{
		ostringstream ost;
		ost << "CCalibration::Calc3DTo2DRad() Need to calibrate system first by calling CalibrateWithRadialDist()" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		T2DFloat P;
		T3DFloat wc;
		wc.x=w.GetX();
		wc.y=w.GetY();
		wc.z=w.GetZ();
		k_Calc3DTo2DRad(wc,&m_Cam,m_k,&P,m_CenterX,m_CenterY);
		Pnt.Set(P.x,P.y);
		return true;
	}
}

bool CPerspective::CalibrationPerformed() const
{
	return m_CamMatrixCalibrated;
}

bool CPerspective::RadCalibrationPerformed() const
{
	return m_RadCalibrationPerformed;
}

CPoint2D<int> CPerspective::GetMaxRadialDisplacement() const
{
	if (m_RadCalibrationPerformed!=true)
	{
		ostringstream ost;
		ost << "CPerspective::MaxRadialDisplacement() Calibration not performet yet, call CalibrateWithRadialDist() first" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return CPoint2D<int>(-1,-1);
	}
	else
	{
		T2DInt Pos;
		Pos=k_GetMaxRadialDisplacement(m_k,m_CenterX,m_CenterY);
		return CPoint2D<int>(Pos.x,Pos.y);
	}
}

bool CPerspective::RemoveRadialDistortion(CStdImage& Dest, CStdImage& Source, bool PreserveImageSize)
{
	bool ReturnValue;
	if (m_RadCalibrationPerformed==false)
	{
		ostringstream ost;
		ost << "CPerspective::RemoveRadialDistortion() Need to calibrate system first by calling CalibrateWithRadialDist()" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		ReturnValue=false;
	}
	else
	{
		ReturnValue=k_RemoveRadialDistortion(Dest.GetTImagePtr(),Source.GetTImagePtr(),m_k,PreserveImageSize);
		if (ReturnValue==false)
		{
			ostringstream ost;
			ost << "CPerspective::RemoveRadialDistortion() Failed removing radial distortion" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
		}
	}
	return ReturnValue;
}

bool CPerspective::RemoveRadialDistortionExtK(CStdImage& Dest, CStdImage& Source, double k, bool PreserveImageSize)
{
	bool ReturnValue;
	if (m_CamMatrixCalibrated==false)
	{
		ostringstream ost;
		ost << "CPerspective::RemoveRadialDistortionExtK() Need to calibrate system first by calling CalibrateWithRadialDist() or Calibrate()" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		ReturnValue=false;
	}
	else
	{
		ReturnValue=k_RemoveRadialDistortion(Dest.GetTImagePtr(),Source.GetTImagePtr(),k,PreserveImageSize);
		if (ReturnValue==false)
		{
			ostringstream ost;
			ost << "CPerspective::RemoveRadialDistortionExtK() Failed removing radial distortion" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
		}
	}
	return ReturnValue;
}

bool CPerspective::SetImageDimensions(unsigned int Width, unsigned Height)
{
	if ((Width==0) || (Height==0))
	{
		ostringstream ost;
		ost << "CPerspective::SetImageDimensions() Width or Height parameters is 0" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	m_CamMatrixCalibrated=false;
    m_CamParametersAvailable=false;
	m_RadCalibrationPerformed=false;
	m_CenterX=(FLOAT32)(Width/2.0);
	m_CenterY=(FLOAT32)(Height/2.0);
	m_ImageDimInitialized=true;
	return true;
}

bool CPerspective::SetImageDimensions(CPoint2D<unsigned int> Dimensions)
{
	return SetImageDimensions(Dimensions.GetX(), Dimensions.GetY());
}

bool CPerspective::GetImageDimensions(unsigned int &Width, unsigned int &Height) const
{
	if (ImageDimensionsInitialized()==false)
	{
		ostringstream ost;
		ost << "CPerspective::GetImageDimensions() Image dimensions not initialized"
			<< IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	Width = (unsigned int)(2*m_CenterX);
	Height = (unsigned int)(2*m_CenterY);
	return true;
}

CPoint2D<unsigned int> CPerspective::GetImageDimensions() const
{
	return CPoint2D<unsigned int>((unsigned int)(2*m_CenterX),(unsigned int)(2*m_CenterY));
}

CPoint2D<FLOAT32> CPerspective::GetPosRadRemoved(FLOAT32 x,FLOAT32 y) const
{
	if (m_RadCalibrationPerformed!=true)
	{
		ostringstream ost;
		ost << "CPerspective::GetPosRadRemoved() Calibration not performet yet, call CalibrateWithRadialDist() first" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return CPoint2D<FLOAT32>(-1,-1);
	}
	else
	{
		T2DFloat Pos;
		Pos=k_GetPosRadRemoved(x,y,m_k,m_CenterX,m_CenterY);
		return CPoint2D<FLOAT32>(Pos.x,Pos.y);
	}
}

CPoint2D<FLOAT32> CPerspective::GetPosRadRemoved(const CPoint2D<FLOAT32>& Pos) const
{
	if (m_RadCalibrationPerformed!=true)
	{
		ostringstream ost;
		ost << "CPerspective::GetPosRadRemoved() Calibration not performet yet, call CalibrateWithRadialDist() first" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return CPoint2D<FLOAT32>(-1,-1);
	}
	else
	{
		T2DFloat Pos1;
		Pos1=k_GetPosRadRemoved(Pos.GetX(),Pos.GetY(),m_k,m_CenterX,m_CenterY);
		return CPoint2D<FLOAT32>(Pos1.x,Pos1.y);
	}
}

CPoint2D<FLOAT32> CPerspective::GetPosInverseRad(FLOAT32 xr, FLOAT32 yr) const
{
	if (m_RadCalibrationPerformed!=true)
	{
		ostringstream ost;
		ost << "CPerspective::GetPosInverseRad() Calibration not performet yet, call CalibrateWithRadialDist() first" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return CPoint2D<FLOAT32>(-1,-1);
	}
	else
	{
		T2DFloat Pos;
		Pos=k_GetPosInverseRad(xr,yr,m_k,m_CenterX,m_CenterY);
		return CPoint2D<FLOAT32>(Pos.x,Pos.y);
	}
}

CPoint2D<FLOAT32> CPerspective::GetPosInverseRad(const CPoint2D<FLOAT32>& PosRad) const
{
	if (m_RadCalibrationPerformed!=true)
	{
		ostringstream ost;
		ost << "CPerspective::GetPosInverseRad() Calibration not performet yet, call CalibrateWithRadialDist() first" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return CPoint2D<FLOAT32>(-1,-1);
	}
	else
	{
		T2DFloat Pos;
		Pos=k_GetPosInverseRad(PosRad.GetX(),PosRad.GetY(),m_k,m_CenterX,m_CenterY);
		return CPoint2D<FLOAT32>(Pos.x,Pos.y);
	}
}

FLOAT32 CPerspective::GetCamMatrix(unsigned short i, unsigned short j) const
{
	if ((i>4) || (j>4) || (i==0) || (j==0))
	{
		ostringstream ost;
		ost << "CPerspective::GetCamMatrix() Indexes out of range" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}

	if ((i==1) && (j==1))
		return m_Cam.a11;
	else if ((i==1) && (j==2))
		return m_Cam.a12;
	else if ((i==1) && (j==3))
		return m_Cam.a13;
	else if ((i==1) && (j==4))
		return m_Cam.a14;
	else if ((i==2) && (j==1))
		return m_Cam.a21;
	else if ((i==2) && (j==2))
		return m_Cam.a22;
	else if ((i==2) && (j==3))
		return m_Cam.a23;
	else if ((i==2) && (j==4))
		return m_Cam.a24;
	else if ((i==3) && (j==1))
		return m_Cam.a31;
	else if ((i==3) && (j==2))
		return m_Cam.a32;
	else if ((i==3) && (j==3))
		return m_Cam.a33;
	else if ((i==3) && (j==4))
		return m_Cam.a34;
	else if ((i==4) && (j==1))
		return m_Cam.a41;
	else if ((i==4) && (j==2))
		return m_Cam.a42;
	else if ((i==4) && (j==3))
		return m_Cam.a43;
	else if ((i==4) && (j==4))
		return m_Cam.a44;
	else
		return 0; // should not be possible!
}

void CPerspective::PrintCameraMatrix(bool WithIndexes) const
{
	k_PrintCameraMatrix(&m_Cam,WithIndexes);
}

bool CPerspective::PrintErrors(const CCorresponding3D2DPoints& PointSets) const
{
	return ErrorsToStream(PointSets,cout);
}

bool CPerspective::ErrorsToStream(const CCorresponding3D2DPoints& PointSets, ostream& Stream) const
{
	if (m_CamMatrixCalibrated!=true)
	{
		ostringstream ost;
		ost << "CPerspective::ErrorsToStream() Calibration not performet yet" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
//	else if (PointSets.GetTotalPointSetsInUse()!=PointSets.GetTotalPointSets())
//	{
//		ostringstream ost;
//		ost << "CPerspective::ErrorsToStream() Some of the point sets are not complete" << IPLAddFileAndLine;
//		CError::ShowMessage(IPL_ERROR,ost);
//		return false;
//	}
	else
	{
		float TotalErrorX=0, TotalErrorY=0;
		float MaxErrorX=0,   MaxErrorY=0, MaxErrorSignX, MaxErrorSignY;
		if (m_RadCalibrationPerformed==true)
		{
			Stream << "**************** CPerspective errors (with radial correction) ******************\n";
		}
		else
		{
			Stream << "**************** CPerspective errors (no radial correction) ******************\n";
		}
		unsigned int i,Count=0;
		for(i=0;i<PointSets.GetTotalPointSets();i++)
		{
			if ((PointSets.Point2DInUse(i)==true) && (PointSets.Point3DInUse(i)==true))
			{
				Count++;
				CPoint3D<FLOAT32> pnt3d=PointSets.GetPoint3D(i);
				CPoint2D<FLOAT32> pnt2d;
				if (m_RadCalibrationPerformed==true)
					Calc3DTo2DRad(pnt3d,pnt2d);
				else
					Calc3DTo2D(pnt3d,pnt2d);
				float xerror=pnt2d.GetX()-PointSets.GetPoint2D(i).GetX();
				float yerror=pnt2d.GetY()-PointSets.GetPoint2D(i).GetY();
				Stream.width(8); 
				Stream.setf(ios::left,ios::adjustfield);
				Stream << " Point " << i << " Id=" << PointSets.GetId(i) << " at " << PointSets.GetPoint2D(i);
				//cout.width(13); cout.setf(ios::left,ios::adjustfield);
				Stream << " Error: (" << xerror << "," << yerror << ")" << endl;
				TotalErrorX+=fabs(xerror);
				TotalErrorY+=fabs(yerror);
				if (fabs(xerror)>MaxErrorX)
				{
					MaxErrorX=fabs(xerror);
					MaxErrorSignX=xerror;
				}
				if (fabs(yerror)>MaxErrorY)
				{
					MaxErrorY=fabs(yerror);
					MaxErrorSignY=yerror;
				}
			}
		}
		Stream << "Total mean error: (" << TotalErrorX/Count << "," << TotalErrorY/Count << ")"
			<< " Max X error=" << MaxErrorSignX << " Max Y error=" << MaxErrorSignY << endl;
//		Stream << "Total mean error: (" << TotalErrorX/i << "," << TotalErrorY/i << ")"
//			<< " Max X error=" << MaxErrorSignX << " Max Y error=" << MaxErrorSignY << endl;
		return true;
	}
}

bool CPerspective::CamParametersToStream(ostream& Stream) const
{
	Stream << endl << StreamIndent << "**************** TCameraParameters info ********************" << endl;
	if (m_CamParametersAvailable==false)
	{
		Stream << StreamIndent << " not available" << endl;
		return false;
	}
	else
	{
		Stream << StreamIndent << " x coordinate of pin-hole (dx) = " << m_Par.dx << endl;
		Stream << StreamIndent << " y coordinate of pin-hole (dy) = " << m_Par.dy << endl;
		Stream << StreamIndent << " z coordinate of pin-hole (dz) = " << m_Par.dz << endl;
		Stream << StreamIndent << " rotation of camera around the x-axis (a) = " << m_Par.a << "[rad] = " << m_Par.a*180/PI << " [deg]"<< endl;
		Stream << StreamIndent << " rotation of camera around the y-axis (b) = " << m_Par.b << "[rad] = " << m_Par.b*180/PI << " [deg]"<< endl;
		Stream << StreamIndent << " rotation of camera around the z-axis (c) = " << m_Par.c << "[rad] = " << m_Par.c*180/PI << " [deg]"<< endl;
		Stream << StreamIndent << " focal length = " << m_Par.FocalLength << " [pixels] " << endl;
		Stream << StreamIndent << " x-coordinate of the focal centre on the CCD chip (xh) = " << m_Par.xh << " [pixels]" << endl;
		Stream << StreamIndent << " y-coordinate of the focal centre on the CCD chip (yh) = " << m_Par.yh << " [pixels]" << endl;
		Stream << StreamIndent << " aspect ratio, scale of V relative to U (p) = " << m_Par.p << endl;
		Stream << StreamIndent << " skewness of V relative to U (beta) = " << m_Par.beta << " [rad] = " << m_Par.beta*180/PI << " [deg]"<< endl;
		return true;
	}		
}

ostream& operator<<(ostream& s,const CPerspective& Persp)
{
	s << StreamIndent << "# Calibration data written by the << stream operator for the "
		"CPerspective class from IPL98" << endl;
	// output comments given in the parameter "Comments"
	s << StreamIndent << "#\n" << StreamIndent << "# CPerspective class version" << endl;
	s << StreamIndent << "PerspectiveVersion " <<  Persp.GetVersion() << endl << endl;
	s << StreamIndent << "# Image dimensions " << endl;
	s << StreamIndent << "ImageDimensionsInitialized: " << Persp.ImageDimensionsInitialized() << endl;
	if (Persp.ImageDimensionsInitialized()==true)
	{
		unsigned int Width,Height;
		Persp.GetImageDimensions(Width, Height);
		s << StreamIndent << "Width " << Width << endl << "Height " << Height << endl << endl;
	}
	s << StreamIndent << "# Camera matrix" << endl;
	s << StreamIndent << "CalibrationPerformed: " << Persp.CalibrationPerformed() << endl;
	if (Persp.CalibrationPerformed()==true)
	{
		const TCameraMatrix* pCam=Persp.GetConstTCameraMatrixPtr();
		s << StreamIndent << pCam->a11 << "   " << pCam->a12 << "   " << pCam->a13 << "   " << pCam->a14 << endl;
		s << StreamIndent << pCam->a21 << "   " << pCam->a22 << "   " << pCam->a23 << "   " << pCam->a24 << endl;
		s << StreamIndent << pCam->a31 << "   " << pCam->a32 << "   " << pCam->a33 << "   " << pCam->a34 << endl;
		s << StreamIndent << pCam->a41 << "   " << pCam->a42 << "   " << pCam->a43 << "   " << pCam->a44 << endl;
		s << "\n" << StreamIndent << "# k radial distortion parameter" << endl;
		s << StreamIndent << "RadCalibrationPerformed: " << Persp.RadCalibrationPerformed() << endl;
		if (Persp.RadCalibrationPerformed()==true)
		{
			s << StreamIndent << "k " << Persp.GetRadDistParameter() << endl;
		}
		s << endl;
		// output camera parameters (if available)
		ostringstream ParamsStr;
		Persp.CamParametersToStream(ParamsStr);
		string Str(ParamsStr.str());
		s << ConvertLinesToComments(Str);
	}
	return s;
}

std::istream& operator>>(istream& is, CPerspective& Persp)
{
	string Token;
	ipl::SkipSpaceAndComments(is);
	// read 'PerspectiveVersion' token
	is >> Token;
	if (Token!=string("PerspectiveVersion"))
	{
		ostringstream ost;
		ost << "operator>>(istream&, CPerspective&) Token CPerspective "
			"not found" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return is;
	}
	double FileVersion;
	is >> FileVersion;
	if (FileVersion!=Persp.GetVersion())
	{
		ostringstream ost;
		ost << "operator>>(istream&, CPerspective&) File version is " << FileVersion
			<< " but this class is version " << Persp.GetVersion() 
			<< ", trying to read anyway" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	ipl::SkipSpaceAndComments(is);
	is >> Token;
	if (Token!=string("ImageDimensionsInitialized:"))
	{
		ostringstream ost;
		ost << "operator>>(istream&, CPerspective&) Token ImageDimensionsInitialized: "
			"not found" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return is;
	}
	bool ImageDimensionsInitialized;
	is >> ImageDimensionsInitialized;
	if (ImageDimensionsInitialized==true)
	{
		ipl::SkipSpaceAndComments(is);
		unsigned int Width, Height;
		is >> Token;
		if (Token!=string("Width"))
		{
			ostringstream ost;
			ost << "operator>>(istream&, CPerspective&) Token Width "
				"not found" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			return is;
		}
		is >> Width;
		ipl::SkipSpaceAndComments(is);
		is >> Token;
		if (Token!=string("Height"))
		{
			ostringstream ost;
			ost << "operator>>(istream&, CPerspective&) Token Height "
				"not found" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			return is;
		}
		is >> Height;
		Persp.SetImageDimensions(
			CPoint2D<unsigned int>((unsigned int)Width, (unsigned int)Height));
	}
	ipl::SkipSpaceAndComments(is);
	is >> Token;
	if (Token!=string("CalibrationPerformed:"))
	{
		ostringstream ost;
		ost << "operator>>(istream&, CPerspective&) Token CalibrationPerformed: "
			"not found" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return is;
	}
	ipl::SkipSpaceAndComments(is);
	bool CalibrationPerformed;
	is >> CalibrationPerformed;
	if (CalibrationPerformed==true)
	{
		TCameraMatrix Cam;
		ipl::SkipSpaceAndComments(is);
		is >> Cam.a11 >> Cam.a12 >> Cam.a13 >> Cam.a14;
		ipl::SkipSpaceAndComments(is);
		is >> Cam.a21 >> Cam.a22 >> Cam.a23 >> Cam.a24;
		ipl::SkipSpaceAndComments(is);
		is >> Cam.a31 >> Cam.a32 >> Cam.a33 >> Cam.a34;
		ipl::SkipSpaceAndComments(is);
		is >> Cam.a41 >> Cam.a42 >> Cam.a43 >> Cam.a44;
		ipl::SkipSpaceAndComments(is);
		// read distortion parameter if available
		ipl::SkipSpaceAndComments(is);
		is >> Token;
		if (Token!=string("RadCalibrationPerformed:"))
		{
			ostringstream ost;
			ost << "operator>>(istream&, CPerspective&) Token RadCalibrationPerformed: "
				"not found" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			return is;
		}
		ipl::SkipSpaceAndComments(is);
		bool RadCalibrationPerformed;
		is >> RadCalibrationPerformed;
		if (RadCalibrationPerformed==true)
		{
			is >> Token;
			if (Token!=string("k"))
			{
				ostringstream ost;
				ost << "operator>>(istream&, CPerspective&) Token k "
					"not found" << IPLAddFileAndLine;
				CError::ShowMessage(IPL_ERROR,ost);
				return is;
			}
			double k;
			is >> k;
			Persp.SetCameraMatrixAndRadDistortion(Cam, k);
		}
		else
		{
			// only set the camera matrix
			Persp.SetCameraMatrix(Cam);
		}
		Persp.MatrixToParameters();
	}
	return is;
}

bool CPerspective::ParametersAvailable() const
{
	return m_CamParametersAvailable;
}

bool CPerspective::MatrixToParameters()
{
	if (m_CamMatrixCalibrated==false)
	{
		ostringstream ost;
		ost << "CPerspective::MatrixToParameters() Camera matrix not yet calculated" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
        if (k_MatrixToParameters(&m_Cam, &m_Par)==false)
			return false;
        m_CamParametersAvailable=true;
		return true;
	}

}


bool CPerspective::ParametersToMatrix()
{
	if (m_CamParametersAvailable==false)
	{
		ostringstream ost;
		ost << "CPerspective::ParametersToMatrix() Camera parameters not yet calculated" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		k_ParametersToMatrix(&m_Par, &m_Cam);
		m_CamMatrixCalibrated=true;
		return true;
	}
}

void CPerspective::DeriveCameraPose(const CCorresponding3D2DPoints &PointSets, const CPerspective &InCamera, 
									bool StartGuessAvailable)
{
    CCorresponding3D2DPoints P3D2D(PointSets);
	P3D2D.RemoveAllSets(false,true,false);
	if (&InCamera!=this)
		*(this)=InCamera;
    float scale;
    int TotalIterations=3;
    if (!StartGuessAvailable)
	{
		CPerspective LocalCamera(InCamera);
		LocalCamera.CalibrateWithRadialDist(P3D2D);
		LocalCamera.MatrixToParameters();
		m_Par.dx=LocalCamera.m_Par.dx;
		m_Par.dy=LocalCamera.m_Par.dy;
		m_Par.dz=LocalCamera.m_Par.dz;
		m_Par.a=LocalCamera.m_Par.a;
		m_Par.b=LocalCamera.m_Par.b;
		m_Par.c=LocalCamera.m_Par.c;
	}			
    scale=(FLOAT32)(1.0/sqrt(m_Par.dx*m_Par.dx+m_Par.dy*m_Par.dy+m_Par.dz*m_Par.dz));
    for (int i = 0; i < (int)P3D2D.GetTotalPointSets(); i++)
		P3D2D.SetPoint3D(scale*P3D2D.GetPoint3D(i),i);

	m_Par.dx*=scale;
	m_Par.dy*=scale;
	m_Par.dz*=scale;
    ParametersToMatrix();
    for (int j=0;j<TotalIterations;j++)
		UpdateExternal(P3D2D);
    m_Par.dx/=scale;
    m_Par.dy/=scale;
    m_Par.dz/=scale;
    ParametersToMatrix();
}

bool CPerspective::Load(const char* pPathAndFileName)
{
	ifstream ifst;
	ifst.open(pPathAndFileName);
	if (ifst)
	{
		ifst >> *this;
		ifst.close();
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CPerspective::Load() Failed loading file: " << pPathAndFileName << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

bool CPerspective::Save(const char* pPathAndFileName,const char* pComments) const
{
	ofstream ofst;
	ofst.open(pPathAndFileName);
	if (ofst)
	{
		if (pComments!=NULL)
		{
			// First convert the pComments to lines with a '#' in front
			string Comments(pComments);
			Comments=ConvertLinesToComments(Comments);
			ofst << Comments << endl;
		}
		ofst << *this;
		ofst.close();
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CPerspective::Save() Failed saving file: " << pPathAndFileName << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

double CPerspective::GetVersion()
{
	return m_Version;
}

bool CPerspective::GetErrors(const CCorresponding3D2DPoints& PointSets, vector<CPoint2D<FLOAT32> >& Errors)
{
	if (m_CamMatrixCalibrated!=true)
	{
		ostringstream ost;
		ost << "CPerspective::GetErrors() Calibration not performet yet" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		Errors.clear();
		unsigned int i;
		for(i=0;i<PointSets.GetTotalPointSets();i++)
		{
			if ((PointSets.Point2DInUse(i)==true) && (PointSets.Point3DInUse(i)==true))
			{
				CPoint3D<FLOAT32> pnt3d=PointSets.GetPoint3D(i);
				CPoint2D<FLOAT32> pnt2d;
				if (m_RadCalibrationPerformed==true)
					Calc3DTo2DRad(pnt3d,pnt2d);
				else
					Calc3DTo2D(pnt3d,pnt2d);
				float xerror=pnt2d.GetX()-PointSets.GetPoint2D(i).GetX();
				float yerror=pnt2d.GetY()-PointSets.GetPoint2D(i).GetY();
				Errors.push_back(CPoint2D<FLOAT32>(xerror,yerror));
			}
		}
		return true;
	}
}

float CPerspective::GetTotalError(const CCorresponding3D2DPoints& PointSets)
{
	vector<CPoint2D<FLOAT32> > Errors;
	GetErrors(PointSets, Errors);
	float Sum=0;
	for(unsigned int i=0;i<Errors.size(); i++)
	{
		Sum += Errors[i].GetX()*Errors[i].GetX() + Errors[i].GetY()*Errors[i].GetY();
	}
	return sqrt(Sum/Errors.size());
}

void CPerspective::UpdateExternal(const CCorresponding3D2DPoints& PointSets)
{
   int TotalUnknowns=6;
   int i,j;
   CPoint2D<FLOAT32> P20,Del;
   CPoint3D<FLOAT32> P3;
   int TotalMarks=PointSets.GetTotalPointSets();
   int TotalEqs=2*TotalMarks;
   CArray2D<double> anm(TotalEqs,TotalUnknowns);
   vector<double> right(TotalEqs);
   vector<double> res;
   for (i=0;i<TotalMarks;i++)
   { 
	   P3=PointSets.GetPoint3D(i);
	   Calc3DTo2DRad(P3,P20);
	   for (j=0;j<TotalUnknowns;j++)
	   {
		   Del=GetPartialDerivative(j,PointSets,i,P20);
		   anm[i*2][j]=Del.GetX();
	       anm[i*2+1][j]=Del.GetY();
		   right[2*i]=P20.GetX()-PointSets.GetPoint2D(i).GetX();
           right[2*i+1]=P20.GetY()-PointSets.GetPoint2D(i).GetY();
	   }
   }
	res=CEquationSolver::LinearOverDeterminedSolve(anm,right);
   for (j=0;j<TotalUnknowns;j++)
   {
	   SetParameter(j,(FLOAT32)(GetParameter(j)-res[j]));
   }
}

CPoint2D<FLOAT32> CPerspective::GetPartialDerivative(int ParameterIndex,
													 const CCorresponding3D2DPoints& PointSets,
													 int MarkIndex, const CPoint2D<FLOAT32>& P20)
{
	FLOAT32 x,y;
	float Step=(float)0.01; // same step for all parameters since they must be normalized prior to using this method
	CPoint2D<FLOAT32> P2,Del,Result;
	FLOAT32 Param=GetParameter(ParameterIndex);
	FLOAT32 Param1=Param+Step;
	SetParameter(ParameterIndex,Param1);
	ParametersToMatrix();
	Calc3DTo2DRad(PointSets.GetPoint3D(MarkIndex),P2);
	Del=P2-P20;
	x=Del.GetX();
	y=Del.GetY();
	if (x*x+y*y<1e-4)
	{
		SetParameter(ParameterIndex,Param+100*Step);
        ParametersToMatrix();
		Calc3DTo2DRad(PointSets.GetPoint3D(MarkIndex),P2);
		Result=(P2-P20)/(Step*100);
	}
	if (x*x+y*y>10)
	{
		SetParameter(ParameterIndex,(FLOAT32)(Param+Step/30.0));
        ParametersToMatrix();
		Calc3DTo2DRad(PointSets.GetPoint3D(MarkIndex),P2);
		Result=(P2-P20)/(Step/30.0);
	}
	else 
		Result=Del/Step;
	SetParameter(ParameterIndex,Param);
	ParametersToMatrix();
	return Result;
}

} // end ipl namespace
