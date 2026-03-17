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

#include "complex_image.h"
#include "std_image.h"
#include "int_image.h"
#include "float_image.h"
#include "../kernel_c/kernel_history.h"

namespace ipl{

CComplexImage::CComplexImage()
: m_History(&m_Image.History)
{
	// default constructor
	kC_InitImage(&m_Image); // always initialise m_Image in constr.
	kC_EmptyImage(&m_Image); // start with empty
	m_ROIActive=false;
}

CComplexImage::CComplexImage(UINT32 Width, UINT32 Height, FLOAT32 ReAndIm)
: m_History(&m_Image.History)
{
	//constr. for empty image
	kC_InitImage(&m_Image); // always initialise m_Image in constr.
	kC_AllocImage(Width,Height,&m_Image);
	m_ROIActive=false;
	if (ReAndIm!=0)
		Flush(ReAndIm,ReAndIm);
}

CComplexImage::CComplexImage(const CComplexImage& SourceImage)
: m_History(&m_Image.History)
{	// copy constructor
	kC_InitImage(&m_Image); // always initialise m_Image in constr.
	*this=SourceImage; // use assignment operator
}

CComplexImage& CComplexImage::operator=(const CComplexImage& SourceImage)
{
	// overloading of assignment operator
	if (this != &SourceImage)
	{
		// ok to do assignment
		kC_EmptyImage(&m_Image); // empty image in all cases
		// no copying if source is empty
		if (SourceImage.GetOrigin()!=EMPTY)
		{
#ifdef IPL_ADD_HISTORY
			/* add comment to history, done by adding to source and removing later (hack) */
			k_IplStartHistoryMacro();
#endif
			/* call kernel function */
			kC_CopyImage(&m_Image,SourceImage.GetConstTComplexImagePtr());
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
				"%sCIntImage::operator=", ipl_HistoryIndent);

			int k=HistoryTemp.find_first_of("\t");
			m_History.AppendIPL(HistoryTemp.substr(k,HistoryTemp.size()-1-k));

#endif
		}
	}
	else
	{
		ostringstream ost;
		ost << "CComplexImage::operator=() Cannot assign CComplexImage to itself" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return *this;
}

CComplexImage::~CComplexImage()
{
	//destructor
	kC_EmptyImage(&m_Image);
}

bool CComplexImage::Alloc(UINT32 Width, UINT32 Height, FLOAT32 ReAndIm)
{
	bool result=kC_AllocImage(Width,Height,&m_Image);
	if (result==false)
	{
		ostringstream ost;
		ost << "CComplexImage::Alloc() Failed allocating image" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
	{
		if (ReAndIm!=0)
			Flush(ReAndIm,ReAndIm);
	}
	m_ROIActive=false;
	return result;
}

bool CComplexImage::AllocFast(UINT32 Width, UINT32 Height)
{
	bool result=kC_AllocImageFast(Width,Height,&m_Image);
	if (result==false)
	{
		ostringstream ost;
		ost << "CComplexImage::AllocFast(): Failed allocating image" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	m_ROIActive=false;
	return result;
}

bool CComplexImage::Load(const char* pPathAndFileName)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	CText TempHistory;

	TempHistory.PrintfAppendIPL(
		"%sCComplexImage::Load(%s)", ipl_HistoryIndent, pPathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=kC_Load(pPathAndFileName,&m_Image);
	if (ReturnValue==true)
	{
		m_ROIActive=false;
#ifdef IPL_ADD_HISTORY
		/* append the new created history in kC_Load to the temporary history and
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
		ost << "CComplexImage::Load() Loading failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CComplexImage::Load(const string& PathAndFileName)
{
	return Load(PathAndFileName.c_str());
}

bool CComplexImage::Save(const char* pPathAndFileName)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::Save(%s)",ipl_HistoryIndent,pPathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=kC_Save(pPathAndFileName,&m_Image);
	if (ReturnValue==false)
	{	
		// load failed
		ostringstream ost;
		ost << "CComplexImage::Save() Saving failed" << IPLAddFileAndLine;
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

bool CComplexImage::Save(const string& PathAndFileName)
{
	return Save(PathAndFileName.c_str());
}

void CComplexImage::Empty()
{
	// Deletes all allocated memory for image
	// and resets variables
	kC_EmptyImage(&m_Image);
	m_ROIActive=false;
}

bool CComplexImage::GetFileName(string& Str)
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

const char* CComplexImage::GetFileName()
{
	return m_Image.FileInfo.FileName;
}

bool CComplexImage::GetPathName(string& Str)
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

const char* CComplexImage::GetPathName()
{
	return m_Image.FileInfo.PathName;
}

bool CComplexImage::GetPathAndFileName(string& Str)
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

const char* CComplexImage::GetPathAndFileName()
{
	return m_Image.FileInfo.PathAndFileName;
}

bool CComplexImage::Flush(FLOAT32 Re, FLOAT32 Im)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::Flush() Re=%g Im=%g", ipl_HistoryIndent, Re, Im);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=kC_FlushImage(Re,Im,&m_Image);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CComplexImage::Flush() Flush failed - image is empty" << IPLAddFileAndLine;
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

bool CComplexImage::SetBorder(UINT16 Size, FLOAT32 Re,FLOAT32 Im)
{
	// Set the size of image border
	if (kC_SetBorder(Size,Re,Im,&m_Image)==false)
	{
		ostringstream ost;
		ost << "CComplexImage::SetBorder() Setting border failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

bool CComplexImage::SetBorderSize(UINT16 Size)
{
	if (kC_SetBorderSize(Size, &m_Image)==false)
	{
		ostringstream ost;
		ost << "CComplexImage::SetBorderSize() Failed setting size of border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

bool CComplexImage::SetBorderColor(FLOAT32 Re, FLOAT32 Im)
{
	// Set the color of image border
	if (kC_SetBorderColor(Re,Im,&m_Image)==false)
	{
		ostringstream ost;
		ost << "CComplexImage::SetBorderColor() Failed setting color of border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

bool CComplexImage::CopySubImage(CComplexImage& SourceImage,int xmin,int ymin,
		  int xmax,int ymax)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history, done by adding to source and removing later (hack) */
	SourceImage.m_History.PrintfAppendIPL(
		"%sCComplexImage::CopySubImage(xmin=%d,ymin=%d,xmax=%d,ymax=%d)"
		" Source image file info: %s",
		ipl_HistoryIndent, xmin, ymin, xmax, ymax,
		SourceImage.GetConstTComplexImagePtr()->FileInfo.PathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=kC_CopySubImage(xmin,ymin,xmax,ymax,&m_Image,&(SourceImage.m_Image));
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CComplexImage::CopySubImage() Failed copying subimage" << IPLAddFileAndLine;
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

bool CComplexImage::CopySubImage(CComplexImage& SourceImage,const CPoint2D<int>& UpperLeft,
		  const CPoint2D<int>& LowerRight)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history, done by adding to source and removing later (hack) */
	SourceImage.m_History.PrintfAppendIPL(
		"%sCComplexImage::CopySubImage(UpperLeft=(%d,%d),LowerLeft=(%d,%d))"
		" Source image file info: %s",
		ipl_HistoryIndent, UpperLeft.GetX(), UpperLeft.GetY(),
		LowerRight.GetX(), LowerRight.GetY(),
		SourceImage.GetConstTComplexImagePtr()->FileInfo.PathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=kC_CopySubImage(UpperLeft.GetX(),UpperLeft.GetY(),LowerRight.GetX(),
					LowerRight.GetY(),&m_Image,&(SourceImage.m_Image));
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CComplexImage::CopySubImage() Failed copying subimage" << IPLAddFileAndLine;
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

UINT16 CComplexImage::GetLineByteWidth() const
{
	// get byte width of a scanline
	return m_Image.ByteWidth;
}

FLOAT32* CComplexImage::GetImageDataPtrRe()
{
	return m_Image.pPixelStreamRe;
}

FLOAT32* CComplexImage::GetImageDataPtrIm()
{
	return m_Image.pPixelStreamIm;
}

const TComplexImage* CComplexImage::GetConstTComplexImagePtr() const
{
	// get pointer to image data
	return &m_Image;
}

TComplexImage* CComplexImage::GetTComplexImagePtr()
{
	// get pointer to image data
	return &m_Image;
}

void CComplexImage::Rotate90(int Steps)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::Rotate90(Steps=%d)",ipl_HistoryIndent,Steps);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	kC_Rotate90(Steps,&m_Image);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void CComplexImage::FlipHorisontal()
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */

	m_History.PrintfAppendIPL(
		"%sCComplexImage::FlipHorisontal()",ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	kC_FlipHorisontal(&m_Image);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void CComplexImage::FlipVertical()
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::FlipVertical()", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	kC_FlipVertical(&m_Image);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void CComplexImage::PrintInfo() const
{
	kC_PrintImageInfo(&m_Image);
	// write ROI information
	if (ROIActive()==true)
	{
		cout << " ROI: " << GetROIUpperLeft() << " -> " << GetROILowerRight() << endl;
	}
	else
		cout << " ROI inactive" << endl;
}

ostream& operator<<(ostream& s,const CComplexImage& Img)
{
	s << StreamIndent << "************* ComplexImage info ****************\n" <<
		StreamIndent << " Width=" << Img.GetWidth() << " Height=" << Img.GetHeight() <<
		" ByteWidth=" << Img.GetLineByteWidth() << " SizeImage=" <<
		(Img.GetConstTComplexImagePtr())->TotalSize << endl << StreamIndent << " BorderSize=" <<
		Img.GetBorderSize() << " Origo=" << Img.GetOrigo() << " Origin=";

	char* OriginStr;
	k_GetOriginString((Img.GetConstTComplexImagePtr())->Origin,&OriginStr);

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
	const TFileInfo* FileInfo=&(Img.GetConstTComplexImagePtr())->FileInfo;
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

bool CComplexImage::ApplyBias(float Bias)
{ 
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::ApplyBias() Bias=%g", ipl_HistoryIndent, Bias);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kC_ApplyBias(Bias,GetTComplexImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CComplexImage::ApplyBias() Failed" << IPLAddFileAndLine;
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

bool CComplexImage::ScaleIntensity(float Prefactor)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::ScaleIntensity() Prefactor=%g",
		ipl_HistoryIndent, Prefactor);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kC_ScaleIntensity(Prefactor,GetTComplexImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CComplexImage::ScaleIntensity() Failed" << IPLAddFileAndLine;
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

bool CComplexImage::Add(CComplexImage &Addend)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::Add() adding a CComplexImage", ipl_HistoryIndent);
		
	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kC_Add(GetTComplexImagePtr(),Addend.GetTComplexImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CComplexImage::Add() Failed" << IPLAddFileAndLine;
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

bool CComplexImage::Subtract(CComplexImage &Subtrahend)
{ 
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::Subtract() subtracting a CComplexImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kC_Subtract(GetTComplexImagePtr(),Subtrahend.GetTComplexImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CComplexImage::Subtract() Failed" << IPLAddFileAndLine;
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

bool CComplexImage::Multiply(CComplexImage &Factor)
{ 
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::Multiply() multiplying a CComplexImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kC_Multiply(GetTComplexImagePtr(),Factor.GetTComplexImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CComplexImage::Multiply() Failed" << IPLAddFileAndLine;
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

bool CComplexImage::CCFactorAndMultiply(CComplexImage &Factor)
{ 
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::CCFactorAndMultiply()", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kC_CCFactorAndMultiply(GetTComplexImagePtr(),Factor.GetTComplexImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CComplexImage::CCFactorAndMultiply() Failed" << IPLAddFileAndLine;
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

bool CComplexImage::ComplexConjugate()
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::ComplexConjugate()", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kC_ComplexConjugate(GetTComplexImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CComplexImage::ComplexConjugate() Failed" << IPLAddFileAndLine;
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

bool CComplexImage::Rearrange()
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::Rearrange()", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kC_Rearrange(GetTComplexImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CComplexImage::Rearrange(): Failed" << IPLAddFileAndLine;
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

bool CComplexImage::CopyDisplace(int dx, int dy)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::CopyDisplace() dx=%d dy=%d",
		ipl_HistoryIndent, dx, dy);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kC_CopyDisplace(dx,dy,GetTComplexImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CComplexImage::CopyDisplace() Failed" << IPLAddFileAndLine;
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

bool CComplexImage::CutOrZeroPad(CComplexImage &Source)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::CutOrZeroPad() Source image file info: %s",
		ipl_HistoryIndent,Source.GetPathAndFileName());

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kC_CutOrZeroPad(GetTComplexImagePtr(),Source.GetTComplexImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CComplexImage::CutOrZeroPad() Failed" << IPLAddFileAndLine;
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

bool CComplexImage::Copy(CStdImage &Re, CStdImage &Im)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::Copy() sources: CStdImage", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kC_CopyFromByte(GetTComplexImagePtr(),Re.GetTImagePtr(),Im.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CComplexImage::Copy() Failed copying CStdImage" << IPLAddFileAndLine;
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

bool CComplexImage::Copy(CIntImage &Re, CIntImage &Im)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::Copy() sources: CIntImage", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kC_CopyFromInt(GetTComplexImagePtr(),Re.GetTIntImagePtr(),Im.GetTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CComplexImage::Copy() Failed copying CIntImage" << IPLAddFileAndLine;
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

bool CComplexImage::Copy(CFloatImage &Re, CFloatImage &Im)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCComplexImage::Copy() sources: CFloatImage", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=kC_CopyFromFloat(GetTComplexImagePtr(),Re.GetTFloatImagePtr(),Im.GetTFloatImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CComplexImage::Copy() Failed copying CFloatImage" << IPLAddFileAndLine;
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

bool CComplexImage::DrawLine(const CPoint2D<int>& Start, const CPoint2D<int>& End, FLOAT32 Value)
{
	T2DInt P1,P2;
	P1.x=Start.GetX();
	P1.y=Start.GetY();
	P2.x=End.GetX();
	P2.y=End.GetY();
	return kC_DrawLine(P1,P2,Value,GetTComplexImagePtr());
}

bool CComplexImage::DrawCircle(const CPoint2D<int>& Center, unsigned int Radius, FLOAT32 Value)
{
	T2DInt P1;
	P1.x=Center.GetX();
	P1.y=Center.GetY();
	return kC_DrawCircle(P1,Radius,Value,GetTComplexImagePtr());
}

} // end ipl namespace
