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

#include "float_image.h"
#include "std_image.h"
#include "int_image.h"
#include "complex_image.h"
#include "../kernel_c/kernel_history.h"

namespace ipl{

CFloatImage::CFloatImage()
: m_History(&m_Image.History)
{
	// default constructor
	kF_InitImage(&m_Image); // always initialise m_Image in constr.
	kF_EmptyImage(&m_Image); // start with empty
	m_ROIActive=false;
}

CFloatImage::CFloatImage(UINT32 Width, UINT32 Height, FLOAT32 Color)
: m_History(&m_Image.History)
{
	//constr. for empty image
	kF_InitImage(&m_Image); // always initialise m_Image in constr.
	kF_AllocImage(Width,Height,&m_Image);
	m_ROIActive=false;
	if (Color!=0)
		Flush(Color);
}

CFloatImage::CFloatImage(const CFloatImage& SourceImage)
: m_History(&m_Image.History)
{	// copy constructor
	kF_InitImage(&m_Image); // always initialise m_Image in constr.
	*this=SourceImage; // use assignment operator
}

CFloatImage& CFloatImage::operator=(const CFloatImage& SourceImage)
{
	// overloading of assignment operator
	if (this != &SourceImage)
	{
		// ok to do assignment
		// no copying if source is empty
		if (SourceImage.GetOrigin()!=EMPTY)
		{
#ifdef IPL_ADD_HISTORY
			k_IplStartHistoryMacro();
#endif
			/* call kernel function */
			kF_CopyImage(&m_Image,SourceImage.GetConstTFloatImagePtr());
			/* set pointers */
			m_History.SetTextPtr(&m_Image.History);
			/* copy ROI */
			m_ROIActive=SourceImage.m_ROIActive;
			if (m_ROIActive==true)
			{
				m_ROIUpperLeft=SourceImage.m_ROIUpperLeft;
				m_ROILowerRight=SourceImage.m_ROILowerRight;
			}
#ifdef IPL_ADD_HISTORY
			/* this is a really bad hack! Removing the last line added by the C code from
			   this objects history, inserting the C++ line and appending the just removed
			   line again. This assumes that only one line is added by the C code. */
			string HistoryTemp;

			/* end history appending (remove a leading '\t' character) */
			k_IplStopHistoryMacro();
			
			m_History.GetItem(m_History.TotalItems()-1,HistoryTemp);
			m_History.RemoveLastLine();

			m_History.PrintfAppendIPL(
				"%sCFloatImage::operator=", ipl_HistoryIndent);

			int k=HistoryTemp.find_first_of("\t");
			m_History.AppendIPL(HistoryTemp.substr(k,HistoryTemp.size()-1-k));
#endif
		}
		else
		{
			kF_EmptyImage(&m_Image); // empty image in all cases
		}
	}
	else
	{
		ostringstream ost;
		ost << "CFloatImage::operator=() Cannot assign CFloatImage to itself" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return *this;
}

CFloatImage::~CFloatImage()
{
	//destructor
	kF_EmptyImage(&m_Image);
}

bool CFloatImage::Alloc(UINT32 Width, UINT32 Height, FLOAT32 Color)
{
	bool result=kF_AllocImage(Width,Height,&m_Image);
	if (result==false)
	{
		ostringstream ost;
		ost << "CFloatImage::Alloc() Failed allocating image" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
	{
		if (Color!=0)
			Flush(Color);
	}
	m_ROIActive=false;
	return result;
}

bool CFloatImage::AllocFast(UINT32 Width, UINT32 Height)
{
	bool result=kF_AllocImageFast(Width,Height,&m_Image);
	if (result==false)
	{
		ostringstream ost;
		ost << "CFloatImage::AllocFast() Failed allocating image" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	m_ROIActive=false;
	return result;
}

bool CFloatImage::Load(const char* pPathAndFileName)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	CText TempHistory;

	TempHistory.PrintfAppendIPL(
		"%sCFloatImage::Load(%s)", ipl_HistoryIndent, pPathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=kF_Load(pPathAndFileName,&m_Image);
	if (ReturnValue==true)
	{
		m_ROIActive=false;
#ifdef IPL_ADD_HISTORY
		/* append the new created history in kF_Load to the temporary history and
		   copy the temporary history back to this objects history */
		TempHistory.AddDate(false);
		TempHistory.AddTime(false);
		bool AddDate=m_History.AddDate(false);
		bool AddTime=m_History.AddTime(false);
		TempHistory.AppendCText(m_History);
		m_History=TempHistory;
		m_History.AddDate(AddDate);
		m_History.AddTime(AddTime);
#endif
	}
	else
	{
		// load failed
		ostringstream ost;
		ost << "CFloatImage::Load() Loading failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CFloatImage::Load(const string& PathAndFileName)
{
	return Load(PathAndFileName.c_str());
}

bool CFloatImage::Save(const char* pPathAndFileName)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCFloatImage::Save(%s)", ipl_HistoryIndent, pPathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=kF_Save(pPathAndFileName,&m_Image);
	if (ReturnValue==false)
	{	
		// load failed
		ostringstream ost;
		ost << "CFloatImage::Save() Saving failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added above */
		m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CFloatImage::Save(const string& PathAndFileName)
{
	return Save(PathAndFileName.c_str());
}

void CFloatImage::Empty()
{
	// Deletes all allocated memory for image
	// and resets variables
	kF_EmptyImage(&m_Image);
	m_ROIActive=false;
}

bool CFloatImage::GetFileName(string& Str)
{
	Str.erase();
	if (m_Image.FileInfo.FileName!=NULL)
	{
		Str.append(m_Image.FileInfo.FileName);
		return true;
	}
	else
		return false;
}

const char* CFloatImage::GetFileName()
{
	return m_Image.FileInfo.FileName;
}

bool CFloatImage::GetPathName(string& Str)
{
	Str.erase();
	if (m_Image.FileInfo.PathName!=NULL)
	{
		Str.append(m_Image.FileInfo.PathName);
		return true;
	}
	else
		return false;
}

const char* CFloatImage::GetPathName()
{
	return m_Image.FileInfo.PathName;
}

bool CFloatImage::GetPathAndFileName(string& Str)
{
	Str.erase();
	if (m_Image.FileInfo.PathAndFileName!=NULL)
	{
		Str.append(m_Image.FileInfo.PathAndFileName);
		return true;
	}
	else
		return false;
}

const char* CFloatImage::GetPathAndFileName()
{
	return m_Image.FileInfo.PathAndFileName;
}

bool CFloatImage::Flush(FLOAT32 Color)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCFloatImage::Flush(Color=%d)", ipl_HistoryIndent, Color);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=kF_FlushImage(Color,&m_Image);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CFloatImage::Flush() Flush failed - image is empty" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_WARNING,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added above */
		m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CFloatImage::SetBorder(UINT16 Size, FLOAT32 Color)
{
	// Set the size of image border
	if (kF_SetBorder(Size,Color,&m_Image)==false)
	{
		ostringstream ost;
		ost << "CFloatImage::SetBorder() Setting border failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

bool CFloatImage::SetBorderSize(UINT16 Size)
{
	if (kF_SetBorderSize(Size, &m_Image)==false)
	{
		ostringstream ost;
		ost << "CFloatImage::SetBorderSize() Failed setting size and color of border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

bool CFloatImage::SetBorderColor(FLOAT32 Color)
{
	// Set the color of image border
	if (kF_SetBorderColor(Color,&m_Image)==false)
	{
		ostringstream ost;
		ost << "CFloatImage::SetBorderColor() Failed setting color of border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

bool CFloatImage::CopySubImage(CFloatImage& SourceImage,int xmin,int ymin,
		  int xmax,int ymax)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history, done by adding to source and removing later (hack) */

	SourceImage.m_History.PrintfAppendIPL(
		"%sCFloatImage::CopySubImage(xmin=%d,ymin=%d,xmax=%d,ymax=%d)"
		" Source image file info: %s",
		ipl_HistoryIndent, xmin, ymin, xmax, ymax,
		SourceImage.GetConstTFloatImagePtr()->FileInfo.PathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=kF_CopySubImage(xmin,ymin,xmax,ymax,&m_Image,&(SourceImage.m_Image));
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CFloatImage::CopySubImage() Failed copying subimage" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
		m_ROIActive=false;
#ifdef IPL_ADD_HISTORY
	if (&SourceImage!=this)
	{
		/* remove the history just added to the source image */
		SourceImage.m_History.RemoveLastLine();
	}
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CFloatImage::CopySubImage(CFloatImage& SourceImage,const CPoint2D<int>& UpperLeft,
		  const CPoint2D<int>& LowerRight)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history, done by adding to source and removing later (hack) */
	SourceImage.m_History.PrintfAppendIPL(
		"%sCFloatImage::CopySubImage(UpperLeft=(%d,%d),LowerLeft=(%d,%d))"
		" Source image file info: %s",
		ipl_HistoryIndent, UpperLeft.GetX(), UpperLeft.GetY(),
		LowerRight.GetX(), LowerRight.GetY(), 
		SourceImage.GetConstTFloatImagePtr()->FileInfo.PathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=kF_CopySubImage(UpperLeft.GetX(),UpperLeft.GetY(),LowerRight.GetX(),
					LowerRight.GetY(),&m_Image,&(SourceImage.m_Image));
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CFloatImage::CopySubImage() Failed copying subimage" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
		m_ROIActive=false;
#ifdef IPL_ADD_HISTORY
	if (&SourceImage!=this)
	{
		/* remove the history just added to the source image */
		SourceImage.m_History.RemoveLastLine();
	}
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

UINT16 CFloatImage::GetLineByteWidth() const
{
	// get byte width of a scanline
	return m_Image.ByteWidth;
}

FLOAT32* CFloatImage::GetImageDataPtr()
{
	return m_Image.pPixelStream;
}

const TFloatImage* CFloatImage::GetConstTFloatImagePtr() const
{
	// get pointer to image data
	return &m_Image;
}

TFloatImage* CFloatImage::GetTFloatImagePtr()
{
	// get pointer to image data
	return &m_Image;
}

void CFloatImage::Rotate90(int Steps)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCFloatImage::Rotate90(Steps=%d)", ipl_HistoryIndent, Steps);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	kF_Rotate90(Steps,&m_Image);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void CFloatImage::FlipHorisontal()
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCFloatImage::FlipHorisontal()", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	kF_FlipHorisontal(&m_Image);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void CFloatImage::FlipVertical()
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCFloatImage::FlipVertical()", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	kF_FlipVertical(&m_Image);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void CFloatImage::PrintInfo() const
{
	kF_PrintImageInfo(&m_Image);
	// write ROI information
	if (ROIActive()==true)
	{
		cout << " ROI: " << GetROIUpperLeft() << " -> " << GetROILowerRight() << endl;
	}
	else
		cout << " ROI inactive" << endl;
}

ostream& operator<<(ostream& s,const CFloatImage& Img)
{
	s << StreamIndent << "*************** FloatImage info ******************\n" << 
		StreamIndent << " Width=" << Img.GetWidth() << " Height=" << Img.GetHeight() <<
		" ByteWidth=" << Img.GetLineByteWidth() << " SizeImage=" <<
		(Img.GetConstTFloatImagePtr())->TotalSize << endl << StreamIndent << " BorderSize=" <<
		Img.GetBorderSize() << " Origo=" << Img.GetOrigo() << " Origin=";
	
	char* OriginStr;
	k_GetOriginString((Img.GetConstTFloatImagePtr())->Origin,&OriginStr);

	s << OriginStr << endl;
	free(OriginStr);
	// write ROI information
	if (Img.ROIActive()==true)
	{
		s << StreamIndent << " ROI: " << Img.GetROIUpperLeft() << " -> " << Img.GetROILowerRight() << endl;
	}
	else
		s << StreamIndent << " ROI inactive" << endl;

	// write file information
	const TFileInfo* FileInfo=&(Img.GetConstTFloatImagePtr())->FileInfo;
	if (FileInfo->FileName!=NULL)
	{
		s << StreamIndent << " File name: " << FileInfo->FileName;
	}
	else
	{
		s << StreamIndent << " File name: Not available";
	}
	if (FileInfo->PathName!=NULL)
	{
		s << StreamIndent << " File path: " << FileInfo->PathName;
	}
	else
	{
		s << StreamIndent << " File path: Not available";
	}

	return s;
}

bool CFloatImage::ApplyBias(float Bias)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */

	m_History.PrintfAppendIPL(
		"%sCFloatImage::ApplyBias() Bias=%g", ipl_HistoryIndent, Bias);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kF_ApplyBias(Bias,GetTFloatImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CFloatImage::ApplyBias() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added above */
		m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CFloatImage::ScaleIntensity(float Prefactor)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCFloatImage::ScaleIntensity() Prefactor=%g",
		ipl_HistoryIndent, Prefactor);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kF_ScaleIntensity(Prefactor,GetTFloatImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CFloatImage::ScaleIntensity() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added above */
		m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CFloatImage::Add(CFloatImage &Addend)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCFloatImage::Add() adding a CFloatImage",ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kF_Add(GetTFloatImagePtr(),Addend.GetTFloatImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CFloatImage::Add() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added above */
		m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CFloatImage::Subtract(CFloatImage &Subtrahend)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCFloatImage::Subtract() subtracting a CFloatImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kF_Subtract(GetTFloatImagePtr(),Subtrahend.GetTFloatImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CFloatImage::Subtract() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added above */
		m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CFloatImage::Multiply(CFloatImage &Factor)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCFloatImage::Multiply() multiplying a CFloatImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kF_Multiply(GetTFloatImagePtr(),Factor.GetTFloatImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CFloatImage::Multiply() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added above */
		m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CFloatImage::CopyDisplace(int dx, int dy)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCFloatImage::CopyDisplace() dx=%d dy=%d",
		ipl_HistoryIndent, dx, dy);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kF_CopyDisplace(dx,dy,GetTFloatImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CFloatImage::CopyDisplace() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added above */
		m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CFloatImage::CutOrZeroPad(CFloatImage &Source)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCFloatImage::CutOrZeroPad() Source image file info: %s",
		ipl_HistoryIndent, Source.GetPathAndFileName());

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kF_CutOrZeroPad(GetTFloatImagePtr(),Source.GetTFloatImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CFloatImage::CutOrZeroPad() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added above */
		m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CFloatImage::Copy(CStdImage &Source)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCFloatImage::Copy() source: CStdImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kF_CopyFromByte(GetTFloatImagePtr(),Source.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CFloatImage::Copy() Failed copying CStdImage" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
		m_ROIActive=false;
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
	/* remove the history line added above */
	m_History.RemoveLastLine();
#endif
	return ReturnValue;
}

bool CFloatImage::Copy(CIntImage &Source)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCFloatImage::Copy() source: CIntImage", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kF_CopyFromInt(GetTFloatImagePtr(),Source.GetTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CFloatImage::Copy() Failed copying CIntImage" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
		m_ROIActive=false;
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
	/* remove the history line added above */
	m_History.RemoveLastLine();
#endif
	return ReturnValue;
}

bool CFloatImage::Copy(COMPLEXCHOISE mode, CComplexImage &Source)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCFloatImage::Copy() source: CComplexImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kF_CopyFromComplex(mode,GetTFloatImagePtr(),Source.GetTComplexImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CFloatImage::Copy() Failed copying CComplexImage" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
		m_ROIActive=false;
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
	/* remove the history line added above */
	m_History.RemoveLastLine();
#endif
	return ReturnValue;
}

bool CFloatImage::DrawLine(const CPoint2D<int>& Start, const CPoint2D<int>& End, FLOAT32 Value)
{
	T2DInt P1,P2;
	P1.x=Start.GetX();
	P1.y=Start.GetY();
	P2.x=End.GetX();
	P2.y=End.GetY();
	return kF_DrawLine(P1,P2,Value,GetTFloatImagePtr());
}

bool CFloatImage::DrawCircle(const CPoint2D<int>& Center, unsigned int Radius, FLOAT32 Value)
{
	T2DInt P1;
	P1.x=Center.GetX();
	P1.y=Center.GetY();
	return kF_DrawCircle(P1,Radius,Value,GetTFloatImagePtr());
}

} // end ipl namespace
