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

#include "image.h"
#include "../kernel_c/kernel_history.h"

namespace ipl{

CImage::CImage()
{
	// default constructor
	k_InitImage(&m_Image); // always initialise m_Image in constr.
	k_EmptyImage(&m_Image); // start with empty
	m_ROIActive=false;
}

CImage::CImage(UINT32 Width, UINT32 Height, UINT16 Bits, UINT32 Color)
{
	//constr. for empty image
	k_InitImage(&m_Image); // always initialise m_Image in constr.
	k_AllocImage(Width,Height,Bits,&m_Image);
	m_ROIActive=false;
	if (Color!=0)
		Flush(Color);
}

CImage::CImage(const CPoint2D<UINT32>& Dimensions, UINT16 Bits, UINT32 Color)
{
	k_InitImage(&m_Image); // always initialise m_Image in constr.
	k_AllocImage(Dimensions.GetX(),Dimensions.GetY(),Bits,&m_Image);
	m_ROIActive=false;
	if (Color!=0)
		Flush(Color);
}

CImage::CImage(const CImage& SourceImage)
{ // copy constructor
	k_InitImage(&m_Image); // always initialise m_Image in constr.
	*this=SourceImage; // use assignment operator
}

CImage& CImage::operator=(const CStdImage& SourceImage)
{
	CStdImage::operator=(SourceImage);
	return *this;
}

CImage& CImage::operator=(const CImage& SourceImage)
{
	CStdImage::operator=(SourceImage);
	return *this;
}

CImage::~CImage()
{
	//destructor
	k_EmptyImage(&m_Image);
}

bool CImage::Alloc(UINT32 Width, UINT32 Height, UINT16 Bits, UINT32 Color)
{
	bool ReturnValue=k_AllocImage(Width,Height,Bits,&m_Image);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CImage::Alloc() Failed allocating image" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
	{
		if (Color!=0)
			Flush(Color);
	}
	m_ROIActive=false;
	return ReturnValue;
}

bool CImage::AllocFast(UINT32 Width, UINT32 Height, UINT16 Bits)
{
	bool ReturnValue=k_AllocImageFast(Width,Height,Bits,&m_Image);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CImage::AllocFast() Failed allocating image" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	m_ROIActive=false;
	return ReturnValue;
}

bool CImage::Load(const char* pPathAndFileName)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	CText TempHistory;

	TempHistory.PrintfAppendIPL(
		"%sCStdImage::Load(%s)",
		ipl_HistoryIndent, pPathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=k_Load(pPathAndFileName,&m_Image);
	if (ReturnValue==true)
	{
		m_Pal.SetPalettePtr(&m_Image.Pal);
		m_ROIActive=false;
		/* append the new created history in k_Load to the temporary history and
		   copy the temporary history back to this objects history */
#ifdef IPL_ADD_HISTORY
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
		ostringstream ost;
		ost << "CImage::Load(\"" << pPathAndFileName << "\") Failed loading image" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CImage::Load(const string& PathAndFileName)
{
	return Load(PathAndFileName.c_str());
}

bool CImage::Save(const char* pPathAndFileName)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::Save(%s)",
		ipl_HistoryIndent, pPathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=k_Save(pPathAndFileName,&m_Image);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CImage::Save(\"" << pPathAndFileName << "\") Failed saving image" << IPLAddFileAndLine;
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

bool CImage::Save(const string& PathAndFileName)
{
	return Save(PathAndFileName.c_str());
}

void CImage::FlipHorisontal()
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	m_History.PrintfAppendIPL(
		"%sCStdImage::FlipHorisontal()",ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif	
	if (IsEmpty()==true)
		return;
	if (GetBits()==1)
	{
		/* call kernel function */
		k_FlipHorisontal(&m_Image);
	}
	else if (GetBits()==8)
	{
		/* call kernel function */
		k_FlipHorisontal(&m_Image);
	}
	else if (GetBits()==24)
	{
		UINT32 Val,Val2,y;
		CImageRowIterator24bp It1=Row24bp(0);
		CImageRowIterator24bp It2=Row24bp(GetHeight()-1);
		for(y=0; y<GetHeight()/2; y++)
		{
			It1=Row24bp(y);
			It2=Row24bp(GetHeight()-y-1);
			while(It1!=It1.End())
			{
				Val=*It1;
				Val2=*It2;
				*It1=Val2;
				*It2=Val;
				++It1;
				++It2;
			}
		}
	}
	else
	{
		ostringstream ost;
		ost << "CImage::FlipHorisontal() Bits (" << GetBits() << ") not supported by this method" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

bool CImage::Split(CByteImage& R, CByteImage& G, CByteImage& B)
{
	// split 24 b/p image into three components of type CByteImage
	// returns false if object is not 24 b/p
	if (m_Image.Bits==24)
	{
		// ok to split image into three components
		UINT32 x,y,pixval;
		// create R,G and B image
		R.Alloc(m_Image.Width,m_Image.Height);
		G.Alloc(m_Image.Width,m_Image.Height);
		B.Alloc(m_Image.Width,m_Image.Height);
		for(y=0; y<m_Image.Height; y++)
		{
			for(x=0; x<m_Image.Width; x++)
			{
				pixval=GetPixelFast(x,y);
				R.m_ppPixel[y][x]=k_PalGetRVal(pixval);
				G.m_ppPixel[y][x]=k_PalGetGVal(pixval);
				B.m_ppPixel[y][x]=k_PalGetBVal(pixval);
			}
		}
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CImage::Split() CImage is not 24 b/p" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

bool CImage::Merge(const CByteImage& R, const CByteImage& G, const CByteImage& B)
{
	// merge three components of type CByteImage into one 24 b/p image
	// returns false if R,G and B are not all same size
	UINT32 RWidth=R.GetWidth();
	UINT32 RHeight=R.GetHeight();
	if ((R.GetOrigin()==EMPTY) || (G.GetOrigin()==EMPTY) ||
		(B.GetOrigin()==EMPTY))
	{
		ostringstream ost;
		ost << "CImage::Merge() One of the RGB matrices are empty - merge failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else if ((RWidth==G.GetWidth()) && (RWidth==B.GetWidth()) &&
		(RHeight==G.GetHeight()) && (RHeight==G.GetHeight()))
	{
		// ok to merge components into one 24 b/p image
		UINT32 x,y;
		Alloc(RWidth,RHeight,24);
		for(y=0; y<RHeight; y++)
		{
			for(x=0; x<RWidth; x++)
			{
				SetPixelFast(x,y,
					k_PalCreateRGB(R.m_ppPixel[y][x],G.m_ppPixel[y][x],B.m_ppPixel[y][x]));
			}
		}
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CImage::Merge() R,G or B do not have same dimensions" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

bool CImage::CopyConvert(UINT16 Bits,CStdImage& SourceImage)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history, done by adding to source and removing later (hack) */

	SourceImage.m_History.PrintfAppendIPL(
		"%sCStdImage::CopyConvert() From %d b/p to %d b/p, "
		"source image file info: %s",
		ipl_HistoryIndent,
		SourceImage.GetBits(),
		Bits,
		SourceImage.GetConstTImagePtr()->FileInfo.PathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=k_CopyConvert(Bits,&m_Image,SourceImage.GetConstTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CImage::CopyConvert() Failed copying image" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
		m_ROIActive=false; // if success ROI is inactivated
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

bool CImage::CopyConvertThreshold(UINT8 Threshold,CStdImage& SourceImage)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history, done by adding to source and removing later (hack) */
	SourceImage.m_History.PrintfAppendIPL(
		"%sCStdImage::CopyConvertThreshold() From 8 b/p to 1 b/p (always),"
		" source image file info: %s",
		ipl_HistoryIndent,
		SourceImage.GetConstTImagePtr()->FileInfo.PathAndFileName);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=k_CopyConvertThreshold(&m_Image,Threshold,SourceImage.GetConstTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CImage::CopyConvertThreshold() Failed copying image" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
		m_ROIActive=false; // if success ROI is inactivated
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

void CImage::PrintInfo() const
{
	k_PrintImageInfo(&m_Image);
	// write ROI information
	if (ROIActive()==true)
	{
		cout << " ROI: " << GetROIUpperLeft() << " -> " << GetROILowerRight() << endl;
	}
	else
		cout << " ROI inactive" << endl;
}

void CImage::PrintOrigin() const
{
	k_PrintOrigin(m_Image.Origin);
}

ostream& operator<<(ostream& s,const CImage& Img)
{
	s << StreamIndent << "**************** Image info ********************\n" << 
		StreamIndent << " Width=" << Img.GetWidth() << " Height=" << Img.GetHeight() <<
		" ByteWidth=" << Img.GetLineByteWidth() << " SizeImage=" <<
		(Img.GetConstTImagePtr())->TotalSize << " Bits=" << Img.GetBits() <<
		"\n" << StreamIndent << " PalEntries=" << Img.m_Pal.GetNumberOfEntries() << " BorderSize=" <<
		Img.GetBorderSize() << " Origo=" << Img.GetOrigo() << " Origin=";	
	char* OriginStr;
	k_GetOriginString((Img.GetConstTImagePtr())->Origin,&OriginStr);
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
	const TFileInfo* FileInfo=&(Img.GetConstTImagePtr())->FileInfo;
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

} // end ipl namespace
