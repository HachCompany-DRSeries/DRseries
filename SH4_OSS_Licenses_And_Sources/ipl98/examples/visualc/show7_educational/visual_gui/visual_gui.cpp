#include "visual_gui.h"
#include <ipl98/cpp/image.h>

/**************************************************************/
/**************************************************************/
/*************** platform dependent: VISUAL C++ ***************/
/**************************************************************/
/**************************************************************/

CVisualGUI::CVisualGUI(ipl::CStdImage* pImage)
{
	m_pImage=pImage;
	m_ColorTableEntries=pImage->m_Pal.GetNumberOfEntries();
	m_lpBMIH = (LPBITMAPINFOHEADER) malloc(sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*m_ColorTableEntries);
	m_pLogPal = (LPLOGPALETTE) new char[2 * sizeof(WORD) +	m_ColorTableEntries * sizeof(PALETTEENTRY)];
}

CVisualGUI::~CVisualGUI()
{
	free(m_lpBMIH);
	delete m_pLogPal;
}

void CVisualGUI::SetImage(ipl::CStdImage* pImage)
{
	m_pImage=pImage;
}

bool CVisualGUI::Show(CDC* pDC,/*CRect rcDest,*/BOOL Activated, 
	float ZoomFactor, CPoint pntSrc)
{
/*-----------------------------------------------------------------------------
pDC:	Device context pointer to do output to.
rcDest:	Rectangle on DC to do output to.
Activated: If true the current window is put to top and we must realize palette
ZoomFactor: zoom factor in image
pntSrc:	Coordinate of the lower-left corner of the DIB to output into rcDest.
-----------------------------------------------------------------------------*/
	// display image on screen (platform dependent)
	/* Check for valid DIB handle */
	if (m_pImage->GetOrigin() == ipl::EMPTY) return FALSE;
	/* If a border is present make a copy, otherwise use the actual image */
	ipl::CStdImage* pImgToShow; // pointer to image to be shown (no border!)
	ipl::CImage TempImage;
	CRect rcDest;
	if (m_pImage->GetBorderSize()!=0)
	{
		// make copy
		TempImage=*m_pImage;
		pImgToShow = &TempImage;
		pImgToShow->SetBorderSize(0); // remove border
	}
	else
	{
		pImgToShow = m_pImage;
	}
	bool bSuccess=false;	// Success/fail flag
	if ((m_ColorTableEntries!=m_pImage->m_Pal.GetNumberOfEntries()) && (m_pImage->m_Pal.GetNumberOfEntries()!=0))
	{
		m_ColorTableEntries=m_pImage->m_Pal.GetNumberOfEntries();
		free(m_lpBMIH);
		delete m_pLogPal;
		m_lpBMIH = (LPBITMAPINFOHEADER) malloc(sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*m_ColorTableEntries);
		m_pLogPal = (LPLOGPALETTE) new char[2 * sizeof(WORD) +	m_ColorTableEntries * sizeof(PALETTEENTRY)];
	}
	
	MakeBitmapInfo(m_lpBMIH,pImgToShow);
	CSize dibsize(pImgToShow->GetWidth(),pImgToShow->GetHeight()); // Get DIB's dimensions

	if ((Activated==TRUE) && (pImgToShow->m_Pal.GetNumberOfEntries()!=0))
	{
		::CPalette Palette; // Create a Visual C Palette class
		if (CreateLogPalette(&m_pLogPal,pImgToShow)==FALSE)
		{
			AfxMessageBox("CVisualGUI::Show() Failed to CreateLogPalette");
		}
		if (Palette.CreatePalette(m_pLogPal)==0)
		{
			AfxMessageBox("CVisualGUI::Show() Failed to CreatePalette");
		}
		// only select and realize palette if device supports it
		// code from www.codeguru.com - bitmap and palettes - Drawing a bitmap
		if (pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
		{
			if (pDC->SelectPalette(&Palette,0)==NULL)
			{
				AfxMessageBox("CVisualGUI::Show() Failed to SelectPalette");
			}
			pDC->RealizePalette();
		}
	}
	if (pDC->IsPrinting())   // printer DC
	{
		// get size of printer page (in pixels)
		int cxPage = pDC->GetDeviceCaps(HORZRES);
		int cyPage = pDC->GetDeviceCaps(VERTRES);
		// get printer pixels per inch
		int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
		int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);

		// Best Fit case -- create a rectangle which preserves
		// the DIB's aspect ratio, and fills the page horizontally.
		//
		// The formula in the "->bottom" field below calculates the Y
		// position of the printed bitmap, based on the size of the
		// bitmap, the width of the page, and the relative size of
		// a printed pixel (cyInch / cxInch).
		rcDest.top = rcDest.left = 0;
		rcDest.bottom = (int)(((double)dibsize.cy*cxPage*cyInch)
				/((double)dibsize.cx*cxInch));
		rcDest.right = cxPage;
	}
	else
	{   // Viewing image
		rcDest.top = rcDest.left = 0;
		rcDest.bottom = (long)(m_lpBMIH->biHeight*ZoomFactor);
		rcDest.right = (long)(m_lpBMIH->biWidth*ZoomFactor);
	}
	/* Make sure to use the stretching mode best for color pictures */
	::SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);
	//TRACE("rcDest.left=%d rcDest.top=%d rcDest.Width()=%d rcDest.Height()=%d\n",
	//	rcDest.left,rcDest.top,rcDest.Width(),rcDest.Height());
	//TRACE("pntSrc.x=%d pntSrc.y=%d (pImgToShow->m_Image.pImageData)=%d\n",
	//	pntSrc.x,pntSrc.y,(pImgToShow->m_Image.pImageData));
	bSuccess = ::StretchDIBits(
		pDC->GetSafeHdc(),		//handle of device context
		rcDest.left,			//x-coordinate of upper-left corner of dest. rect.
		rcDest.top,				//y-coordinate of upper-left corner of dest. rect.
		rcDest.Width(),			//width of destination rectangle
		rcDest.Height(),		//height of destination rectangle
		pntSrc.x,				//x-coordinate of upper-left corner of source rect.
		pntSrc.y,				//y-coordinate of upper-left corner of source rect.
		m_lpBMIH->biWidth,		//width of source rectangle
		m_lpBMIH->biHeight,		//height of source rectangle
		(pImgToShow->GetImageDataPtr()),//address of bitmap bits
		(LPBITMAPINFO)m_lpBMIH,	//address of bitmap data
		DIB_RGB_COLORS,			//usage
		SRCCOPY);				//raster operation code
    return bSuccess;
}

BOOL CVisualGUI::CopyToClipboard(CRect* ROI)
{
	// copy to clipbrd - use OpenClipboard() first
	//Before calling CopyToClipboard, an application must open the clipboard
	//  by using the OpenClipboard function.

	if (m_pImage->GetOrigin()==ipl::EMPTY)
		return FALSE;

	ipl::CStdImage* pImgNoBorder; // pointer to image without border!
	ipl::CImage TempImage;
	if (ROI!=NULL)
	{
		// copy the ROI to a new image
		if (TempImage.CopySubImage(*m_pImage,ROI->left,ROI->top,ROI->right,ROI->bottom)!=true)
		{
			AfxMessageBox("In CVisualGUI::CopyToClipboard() ROI out of range");
			return FALSE;
		}
		pImgNoBorder = &TempImage;
	}
	else if (m_pImage->GetBorderSize()!=0)
	{
		// make copy
		TempImage=*m_pImage;
		pImgNoBorder = &TempImage;
		pImgNoBorder->SetBorder(0,0); // remove border
	}
	else
	{
		pImgNoBorder = m_pImage;
	}
	// Clean clipboard of contents, and copy the DIB.
	int SizeHeader=sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*m_pImage->m_Pal.GetNumberOfEntries();
	HGLOBAL h = ::GlobalAlloc(GHND,SizeHeader+pImgNoBorder->GetTImagePtr()->TotalSize);
	if (h != NULL)
	{
		void* lp = ::GlobalLock(h);
		LPBITMAPINFOHEADER lpBMIH;
		lpBMIH = (LPBITMAPINFOHEADER) malloc(SizeHeader);
		MakeBitmapInfo(lpBMIH,pImgNoBorder);
		//copy header and palette
		memcpy(lp,lpBMIH,SizeHeader);
		//copy image bits
		memcpy((LPBYTE)lp+SizeHeader,pImgNoBorder->GetImageDataPtr(),
						pImgNoBorder->GetTImagePtr()->TotalSize);
		::GlobalUnlock(h);
		EmptyClipboard();
		SetClipboardData(CF_DIB,h);
		CloseClipboard();
		free(lpBMIH);
		return TRUE;
	}
	return FALSE;
}

BOOL CVisualGUI::PasteFromClipboard()
{
	HGLOBAL hNewDIB = (HGLOBAL)::GetClipboardData(CF_DIB);
	if (hNewDIB != NULL) {
		m_pImage->Empty();	//free the old image
		LPBITMAPINFOHEADER lpNewBMIH = (LPBITMAPINFOHEADER)::GlobalLock(hNewDIB);
		*m_pImage=ipl::CImage(lpNewBMIH->biWidth,lpNewBMIH->biHeight,lpNewBMIH->biBitCount);
		int ColorTableEntries=m_pImage->m_Pal.GetNumberOfEntries();
		int headersize = sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*ColorTableEntries;

	    //copy palette
		memcpy((m_pImage->m_Pal.GetPalettePtr())->pPalette,(BYTE*)lpNewBMIH+sizeof(BITMAPINFOHEADER),
			sizeof(RGBQUAD)*ColorTableEntries);
		//copy image bits
		memcpy(m_pImage->GetImageDataPtr(),(LPBYTE)lpNewBMIH+headersize,m_pImage->GetConstTImagePtr()->TotalSize);
		
		::GlobalUnlock(hNewDIB);
		CloseClipboard();	//enable other windows to access the clipboard
		return TRUE;
	}
	return FALSE;
}

// copy info stored in CStdImage class to at BITMAPINFOHEADER structure
bool CVisualGUI::MakeBitmapInfo(LPBITMAPINFOHEADER lpBMIH, ipl::CStdImage* pImg)
{
	// copy info stored in CImage class to at BITMAPINFOHEADER structure
	lpBMIH->biSize=sizeof(BITMAPINFOHEADER);
	lpBMIH->biWidth=pImg->GetWidth();
	lpBMIH->biHeight=pImg->GetHeight();
	lpBMIH->biPlanes=1;
	lpBMIH->biBitCount=pImg->GetBits();
	lpBMIH->biCompression=BI_RGB;
	lpBMIH->biSizeImage=0; // Visual C online help: may be set to 0 for BI_RGB bitmaps
	lpBMIH->biClrUsed=0;
	// copy palette to this structure
	if (pImg->GetOrigin() != ipl::EMPTY)
	{
		DWORD* Entry=(DWORD*)(lpBMIH)+sizeof(BITMAPINFOHEADER)/sizeof(DWORD);
		memcpy(Entry,pImg->GetTImagePtr()->Pal.pPalette,4*(pImg->m_Pal.GetNumberOfEntries()));
	}
	return true;
}

bool CVisualGUI::CreateLogPalette(LPLOGPALETTE *pLogPal, ipl::CStdImage* pImg)
{
	// makes a logical palette (hPalette) from the Image's color table
	// this palette will be selected and realized prior to drawing the DIB

	if (m_ColorTableEntries == 0) return FALSE;
	(*pLogPal)->palVersion = 0x300;
	(*pLogPal)->palNumEntries = m_ColorTableEntries;
	DWORD* pDibQuad = (DWORD*) pImg->GetConstTImagePtr()->Pal.pPalette; // pointer to palette data
	memcpy((*pLogPal)->palPalEntry,(pDibQuad),m_ColorTableEntries*sizeof(DWORD));
	return TRUE;
}
