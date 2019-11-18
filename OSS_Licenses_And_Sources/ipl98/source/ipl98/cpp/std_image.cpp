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

#include "std_image.h"
#include "int_image.h"
#include "float_image.h"
#include "complex_image.h"
#include "../kernel_c/kernel_history.h"

namespace ipl{

CStdImage::CStdImage()
: m_Pal(&m_Image.Pal), m_History(&m_Image.History)
{
}

CStdImage& CStdImage::operator=(const CStdImage& SourceImage)
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
			k_CopyImage(&m_Image,SourceImage.GetConstTImagePtr());
			/* set pointers */
			m_Pal.SetPalettePtr(&m_Image.Pal);
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
				"%sCStdImage::operator=",
				ipl_HistoryIndent);

			int k=HistoryTemp.find_first_of("\t");
			m_History.AppendIPL(HistoryTemp.substr(k,HistoryTemp.size()-1-k));

#endif
		}
		else
		{
			k_EmptyImage(&m_Image); // empty image in all cases
		}
	}
	else
	{
		ostringstream ost;
		ost << "CStdImage::operator=() Cannot assign CImage to itself" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return *this;
}

void CStdImage::Empty()
{
	// Deletes all allocated memory for image
	// and resets variables
	k_EmptyImage(&m_Image);
	m_ROIActive=false;
}

bool CStdImage::GetFileName(string& Str)
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

const char* CStdImage::GetFileName()
{
	return m_Image.FileInfo.FileName;
}

bool CStdImage::GetPathName(string& Str)
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

const char* CStdImage::GetPathName()
{
	return m_Image.FileInfo.PathName;
}

bool CStdImage::GetPathAndFileName(string& Str)
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

const char* CStdImage::GetPathAndFileName()
{
	return m_Image.FileInfo.PathAndFileName;
}

bool CStdImage::Flush(UINT32 Color)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::Flush(Color=%d)",
		ipl_HistoryIndent, Color);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=k_FlushImage(Color,&m_Image);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStdImage::Flush() Flush failed - image is empty" << IPLAddFileAndLine;
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

bool CStdImage::Invert()
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::Invert()", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=k_InvertImage(&m_Image);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStdImage::Invert() Failed inverting - image is empty" << IPLAddFileAndLine;
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

bool CStdImage::SetBorder(UINT16 Size, UINT32 Color)
{
	// Set the size of image border
	if (k_SetBorder(Size,Color,&m_Image)==false)
	{
		ostringstream ost;
		ost << "CStdImage::SetBorder() Setting border failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

bool CStdImage::SetBorderSize(UINT16 Size)
{
	if (k_SetBorderSize(Size, &m_Image)==false)
	{
		ostringstream ost;
		ost << "CStdImage::SetBorderSize() Failed setting size and color of border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

bool CStdImage::SetBorderColor(UINT32 Color)
{
	// Set the color of image border
	if (k_SetBorderColor(Color,&m_Image)==false)
	{
		ostringstream ost;
		ost << "CStdImage::SetBorderColor() Failed setting color of border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

bool CStdImage::CopySubImage(CStdImage& SourceImage,int xmin,int ymin,
				  int xmax, int ymax)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history, done by adding to source and removing later (hack) */
	SourceImage.m_History.PrintfAppendIPL(
		"%sCStdImage::CopySubImage(xmin=%d,ymin=%d,xmax=%d,ymax=%d)"
		" Source image file info: %s",
		ipl_HistoryIndent, xmin, ymin, xmax, ymax,
		SourceImage.GetPathAndFileName());

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=k_CopySubImage(xmin,ymin,xmax,ymax,&m_Image,&(SourceImage.m_Image));
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStdImage::CopySubImage()Failed copying subimage" << IPLAddFileAndLine;
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

bool CStdImage::CopySubImage(CStdImage& SourceImage,const CPoint2D<int>& UpperLeft,
		  const CPoint2D<int>& LowerRight)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history, done by adding to source and removing later (hack) */
	SourceImage.m_History.PrintfAppendIPL(
		"%sCStdImage::CopySubImage(UpperLeft=(%d,%d),LowerLeft=(%d,%d))"
		" Source image file info: %s",
		ipl_HistoryIndent, UpperLeft.GetX(), UpperLeft.GetY(),
		LowerRight.GetX(), LowerRight.GetY(),
		SourceImage.GetConstTImagePtr()->FileInfo.PathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=k_CopySubImage(UpperLeft.GetX(),UpperLeft.GetY(),LowerRight.GetX(),
					LowerRight.GetY(),&m_Image,&(SourceImage.m_Image));
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStdImage::CopySubImage() Failed copying subimage" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
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
	return true;
}

UINT16 CStdImage::GetLineByteWidth() const
{
	// get byte width of a scanline
	return m_Image.ByteWidth;
}

UINT8* CStdImage::GetImageDataPtr()
{
	return m_Image.pPixelStream;
}

const TImage* CStdImage::GetConstTImagePtr() const
{
	// get pointer to image data
	return &m_Image;
}

TImage* CStdImage::GetTImagePtr()
{
	// get pointer to image data
	return &m_Image;
}

void CStdImage::Rotate90(int Steps)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::Rotate90(Steps=%d)",
		ipl_HistoryIndent, Steps);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	k_Rotate90(Steps,&m_Image);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void CStdImage::FlipHorisontal()
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::FlipHorisontal()", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	k_FlipHorisontal(&m_Image);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void CStdImage::FlipVertical()
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::FlipVertical()", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	k_FlipVertical(&m_Image);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

bool CStdImage::ApplyBias(int Bias)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::ApplyBias() Bias=%d",
		ipl_HistoryIndent, Bias);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_ApplyBias(Bias,GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStdImage::ApplyBias() Failed" << IPLAddFileAndLine;
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

bool CStdImage::ScaleIntensity(float Prefactor)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::ScaleIntensity() Prefactor=%g",
		ipl_HistoryIndent, Prefactor);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_ScaleIntensity(Prefactor,GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStdImage::ScaleIntensity() Failed" << IPLAddFileAndLine;
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

bool CStdImage::Add(CStdImage &Addend)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::Add() adding a CStdImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_Add(GetTImagePtr(),Addend.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStdImage::Add() Failed" << IPLAddFileAndLine;
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

bool CStdImage::Subtract(CStdImage &Subtrahend)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::Subtract() subtracting a CStdImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_Subtract(GetTImagePtr(),Subtrahend.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStdImage::Subtract() Failed" << IPLAddFileAndLine;
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

bool CStdImage::Multiply(CStdImage &Factor)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::Multiply() multiplying a CStdImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_Multiply(GetTImagePtr(),Factor.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStdImage::Multiply() Failed" << IPLAddFileAndLine;
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

bool CStdImage::CopyDisplace(int dx, int dy)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::CopyDisplace() dx=%d dy=%d",
		ipl_HistoryIndent, dx, dy);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_CopyDisplace(dx,dy,&m_Image);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStdImage::CopyDisplace() Failed" << IPLAddFileAndLine;
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

bool CStdImage::CutOrZeroPad(CStdImage &Source)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::CutOrZeroPad() Source image file info: %s",
		ipl_HistoryIndent,
		Source.GetPathAndFileName());

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_CutOrZeroPad(GetTImagePtr(),Source.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStdImage::CutOrZeroPad() Failed" << IPLAddFileAndLine;
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

bool CStdImage::Copy(CIntImage &Source)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::Copy() source: CIntImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_CopyFromInt(GetTImagePtr(),Source.GetTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStdImage::Copy() Failed copying CIntImage" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
	/* remove the history line added above */
	m_History.RemoveLastLine();
#endif
	return ReturnValue;
}

bool CStdImage::Copy(CFloatImage &Source)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::Copy() source: CFloatImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_CopyFromFloat(GetTImagePtr(),Source.GetTFloatImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStdImage::Copy() Failed copying CFloatImage" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
	/* remove the history line added above */
	m_History.RemoveLastLine();
#endif
	return ReturnValue;
}

bool CStdImage::Copy(COMPLEXCHOISE mode, CComplexImage &Source)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::Copy() source: CFloatImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_CopyFromComplex(mode,GetTImagePtr(),Source.GetTComplexImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStdImage::Copy() Failed copying CComplexImage" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
	/* remove the history line added above */
	m_History.RemoveLastLine();
#endif
	return ReturnValue;
}

bool CStdImage::DrawLine(const CPoint2D<int>& Start, const CPoint2D<int>& End, UINT32 Color)
{
	T2DInt P1,P2;
	P1.x=Start.GetX();
	P1.y=Start.GetY();
	P2.x=End.GetX();
	P2.y=End.GetY();
	return k_DrawLine(P1,P2,Color,GetTImagePtr());
}

bool CStdImage::DrawCircle(const CPoint2D<int>& Center, unsigned int Radius, UINT32 Color)
{
	T2DInt P1;
	P1.x=Center.GetX();
	P1.y=Center.GetY();
	return k_DrawCircle(P1,Radius,Color,GetTImagePtr());
}

} // end ipl namespace
