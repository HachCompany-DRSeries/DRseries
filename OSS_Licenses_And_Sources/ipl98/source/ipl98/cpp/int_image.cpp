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

#include "int_image.h"
#include "std_image.h"
#include "float_image.h"
#include "complex_image.h"
#include "../kernel_c/kernel_history.h"

namespace ipl{

CIntImage::CIntImage()
: m_History(&m_Image.History)
{
	// default constructor
	kI_InitImage(&m_Image); // always initialise m_Image in constr.
	kI_EmptyImage(&m_Image); // start with empty
	m_ROIActive=false;
}

CIntImage::CIntImage(UINT32 Width, UINT32 Height, UINT32 Color)
: m_History(&m_Image.History)
{
	//constr. for empty image
	kI_InitImage(&m_Image); // always initialise m_Image in constr.
	kI_AllocImage(Width,Height,&m_Image);
	m_ROIActive=false;
	if (Color!=0)
		Flush((INT16)Color);
}

CIntImage::CIntImage(const CIntImage& SourceImage)
: m_History(&m_Image.History)
{	// copy constructor
	kI_InitImage(&m_Image); // always initialise m_Image in constr.
	*this=SourceImage; // use assignment operator
}

CIntImage& CIntImage::operator=(const CIntImage& SourceImage)
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
			kI_CopyImage(&m_Image,SourceImage.GetConstTIntImagePtr());
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
			kI_EmptyImage(&m_Image); // empty image in all cases
		}
	}
	else
	{
		ostringstream ost;
		ost << "CIntImage::operator=() Cannot assign CIntImage to itself" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return *this;
}

CIntImage::~CIntImage()
{
	//destructor
	kI_EmptyImage(&m_Image);
}

bool CIntImage::Alloc(UINT32 Width, UINT32 Height, UINT32 Color)
{
	bool result=kI_AllocImage(Width,Height,&m_Image);
	if (result==false)
	{
		ostringstream ost;
		ost << "CIntImage::Alloc() Failed allocating image" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
	{
		if (Color!=0)
			Flush((INT16)Color);
	}
	m_ROIActive=false;
	return result;
}

bool CIntImage::AllocFast(UINT32 Width, UINT32 Height)
{
	bool result=kI_AllocImageFast(Width,Height,&m_Image);
	if (result==false)
	{
		ostringstream ost;
		ost << "CIntImage::AllocFast() Failed allocating image" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	m_ROIActive=false;
	return result;
}

bool CIntImage::Load(const char* pPathAndFileName)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	CText TempHistory;

	TempHistory.PrintfAppendIPL(
		"%sCIntImage::Load(%s)",
		ipl_HistoryIndent, pPathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=kI_Load(pPathAndFileName,&m_Image);
	if (ReturnValue==true)
	{
		m_ROIActive=false;
#ifdef IPL_ADD_HISTORY
		/* append the new created history in kI_Load to the temporary history and
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
		ost << "CIntImage::Load() Loading failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CIntImage::Load(const string& PathAndFileName)
{
	return Load(PathAndFileName.c_str());
}

bool CIntImage::Save(const char* pPathAndFileName)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCIntImage::Save(%s)",
		ipl_HistoryIndent, pPathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=kI_Save(pPathAndFileName,&m_Image);
	if (ReturnValue==false)
	{	
		// load failed
		ostringstream ost;
		ost << "CIntImage::Save() Saving failed" << IPLAddFileAndLine;
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

bool CIntImage::Save(const string& PathAndFileName)
{
	return Save(PathAndFileName.c_str());
}

void CIntImage::Empty()
{
	// Deletes all allocated memory for image
	// and resets variables
	kI_EmptyImage(&m_Image);
	m_ROIActive=false;
}

bool CIntImage::GetFileName(string& Str)
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

const char* CIntImage::GetFileName()
{
	return m_Image.FileInfo.FileName;
}

bool CIntImage::GetPathName(string& Str)
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

const char* CIntImage::GetPathName()
{
	return m_Image.FileInfo.PathName;
}

bool CIntImage::GetPathAndFileName(string& Str)
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

const char* CIntImage::GetPathAndFileName()
{
	return m_Image.FileInfo.PathAndFileName;
}

bool CIntImage::Flush(INT16 Color)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCIntImage::Flush(Color=%d)",
		ipl_HistoryIndent, Color);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=kI_FlushImage(Color,&m_Image);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CIntImage::Flush() Flush failed - image is empty" << IPLAddFileAndLine;
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

bool CIntImage::SetBorder(UINT16 Size, INT16 Color)
{
	// Set the size of image border
	if (kI_SetBorder(Size,Color,&m_Image)==false)
	{
		ostringstream ost;
		ost << "CIntImage::SetBorder() Setting border failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

bool CIntImage::SetBorderSize(UINT16 Size)
{
	if (kI_SetBorderSize(Size, &m_Image)==false)
	{
		ostringstream ost;
		ost << "CIntImage::SetBorderSize() Failed setting size and color of border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

bool CIntImage::SetBorderColor(INT16 Color)
{
	// Set the color of image border
	if (kI_SetBorderColor(Color,&m_Image)==false)
	{
		ostringstream ost;
		ost << "CIntImage::SetBorderColor() Failed setting color of border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

bool CIntImage::CopySubImage(CIntImage& SourceImage,int xmin,int ymin,
		  int xmax,int ymax)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history, done by adding to source and removing later (hack) */

	SourceImage.m_History.PrintfAppendIPL(
		"%sCIntImage::CopySubImage(xmin=%d,ymin=%d,xmax=%d,ymax=%d)"
		" Source image file info: %s",
		ipl_HistoryIndent, xmin, ymin, xmax, ymax,
		SourceImage.GetConstTIntImagePtr()->FileInfo.PathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=kI_CopySubImage(xmin,ymin,xmax,ymax,&m_Image,&(SourceImage.m_Image));
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CIntImage::CopySubImage() Failed copying subimage" << IPLAddFileAndLine;
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

bool CIntImage::CopySubImage(CIntImage& SourceImage,const CPoint2D<int>& UpperLeft,
		  const CPoint2D<int>& LowerRight)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history, done by adding to source and removing later (hack) */

	SourceImage.m_History.PrintfAppendIPL(
		"%sCIntImage::CopySubImage(UpperLeft=(%d,%d),LowerLeft=(%d,%d))"
		" Source image file info: %s",
		ipl_HistoryIndent, UpperLeft.GetX(), UpperLeft.GetY(),
		LowerRight.GetX(), LowerRight.GetY(),
		SourceImage.GetConstTIntImagePtr()->FileInfo.PathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=kI_CopySubImage(UpperLeft.GetX(),UpperLeft.GetY(),LowerRight.GetX(),
					LowerRight.GetY(),&m_Image,&(SourceImage.m_Image));
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CIntImage::CopySubImage() Failed copying subimage" << IPLAddFileAndLine;
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

UINT16 CIntImage::GetLineByteWidth() const
{
	// get byte width of a scanline
	return m_Image.ByteWidth;
}

INT16* CIntImage::GetImageDataPtr()
{
	return m_Image.pPixelStream;
}

const TIntImage* CIntImage::GetConstTIntImagePtr() const
{
	// get pointer to image data
	return &m_Image;
}

TIntImage* CIntImage::GetTIntImagePtr()
{
	// get pointer to image data
	return &m_Image;
}

void CIntImage::Rotate90(int Steps)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCIntImage::Rotate90(Steps=%d)",
		ipl_HistoryIndent, Steps);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	kI_Rotate90(Steps,&m_Image);
	/* end history appending (remove a leading '\t' character) */
#ifdef IPL_ADD_HISTORY
	k_IplStopHistoryMacro();
#endif
}

void CIntImage::FlipHorisontal()
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCIntImage::FlipHorisontal()", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	kI_FlipHorisontal(&m_Image);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void CIntImage::FlipVertical()
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCIntImage::FlipVertical()", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	kI_FlipVertical(&m_Image);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void CIntImage::PrintInfo() const
{
	kI_PrintImageInfo(&m_Image);
	// write ROI information
	if (ROIActive()==true)
	{
		cout << " ROI: " << GetROIUpperLeft() << " -> " << GetROILowerRight() << endl;
	}
	else
		cout << " ROI inactive" << endl;
}

ostream& operator<<(ostream& s,const CIntImage& Img)
{
	s << StreamIndent << "*************** IntImage info ******************\n" << 
		StreamIndent << " Width=" << Img.GetWidth() << " Height=" << Img.GetHeight() <<
		" ByteWidth=" << Img.GetLineByteWidth() << " SizeImage=" <<
		(Img.GetConstTIntImagePtr())->TotalSize << endl << StreamIndent << " BorderSize=" <<
		Img.GetBorderSize() << " Origo=" << Img.GetOrigo() << " Origin=";
	
	char* OriginStr;
	k_GetOriginString((Img.GetConstTIntImagePtr())->Origin,&OriginStr);

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
	const TFileInfo* FileInfo=&(Img.GetConstTIntImagePtr())->FileInfo;
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

bool CIntImage::ApplyBias(int Bias)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCIntImage::ApplyBias() Bias=%d",
		ipl_HistoryIndent, Bias);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kI_ApplyBias(Bias,GetTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CIntImage::ApplyBias() Failed" << IPLAddFileAndLine;
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

bool CIntImage::ScaleIntensity(float Prefactor)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCIntImage::ScaleIntensity() Prefactor=%g",
		ipl_HistoryIndent, Prefactor);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kI_ScaleIntensity(Prefactor,GetTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CIntImage::ScaleIntensity() Failed" << IPLAddFileAndLine;
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

bool CIntImage::Add(CIntImage &Addend)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCIntImage::Add() adding a CIntImage", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kI_Add(GetTIntImagePtr(),Addend.GetTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CIntImage::Add() Failed" << IPLAddFileAndLine;
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

bool CIntImage::Subtract(CIntImage &Subtrahend)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCIntImage::Subtract() subtracting a CIntImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kI_Subtract(GetTIntImagePtr(),Subtrahend.GetTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CIntImage::Subtract() Failed" << IPLAddFileAndLine;
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

bool CIntImage::Multiply(CIntImage &Factor)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCIntImage::Multiply() multiplying a CIntImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kI_Multiply(GetTIntImagePtr(),Factor.GetTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CIntImage::Multiply() Failed" << IPLAddFileAndLine;
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

bool CIntImage::CopyDisplace(int dx, int dy)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCIntImage::CopyDisplace() dx=%d dy=%d",
		ipl_HistoryIndent, dx, dy);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kI_CopyDisplace(dx,dy,GetTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CIntImage::CopyDisplace() Failed" << IPLAddFileAndLine;
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

bool CIntImage::CutOrZeroPad(CIntImage &Source)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCIntImage::CutOrZeroPad() Source image file info: %s",
		ipl_HistoryIndent, Source.GetPathAndFileName());

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kI_CutOrZeroPad(GetTIntImagePtr(),Source.GetTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CIntImage::CutOrZeroPad() Failed" << IPLAddFileAndLine;
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

bool CIntImage::Copy(CStdImage &Source)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCIntImage::Copy() source: CStdImage", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kI_CopyFromByte(GetTIntImagePtr(),Source.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CIntImage::Copy() Failed copying CStdImage" << IPLAddFileAndLine;
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

bool CIntImage::Copy(CFloatImage &Source)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCIntImage::Copy() source: CFloatImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kI_CopyFromFloat(GetTIntImagePtr(),Source.GetTFloatImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CIntImage::Copy() Failed copying CIntImage" << IPLAddFileAndLine;
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

bool CIntImage::Copy(COMPLEXCHOISE mode, CComplexImage &Source)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCIntImage::Copy() source: CComplexImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kI_CopyFromComplex(mode,GetTIntImagePtr(),Source.GetTComplexImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CIntImage::Copy() Failed copying CComplexImage" << IPLAddFileAndLine;
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

bool CIntImage::DrawLine(const CPoint2D<int>& Start, const CPoint2D<int>& End, INT16 Value)
{
	T2DInt P1,P2;
	P1.x=Start.GetX();
	P1.y=Start.GetY();
	P2.x=End.GetX();
	P2.y=End.GetY();
	return kI_DrawLine(P1,P2,Value,GetTIntImagePtr());
}

bool CIntImage::DrawCircle(const CPoint2D<int>& Center, unsigned int Radius, INT16 Value)
{
	T2DInt P1;
	P1.x=Center.GetX();
	P1.y=Center.GetY();
	return kI_DrawCircle(P1,Radius,Value,GetTIntImagePtr());
}

} // end ipl namespace
