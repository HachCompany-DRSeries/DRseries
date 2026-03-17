#ifndef CVISUALGUI_H
#define CVISUALGUI_H

//#include <ipl98/ipl98_cplusplus_containers.h>
#include <ipl98/cpp/std_image.h>
#include "stdafx.h" // MFC core and standard components

/** Adding some Visual C++ hardware and operating system dependend
	methods such as showing in window and clipboard-functionality,
	last updated 17/7/2000. This class must be used with MS Visual C++
	and a CStdImage from the IPL98 library.
	When using this file, make sure it can include your "stdafx.h" file
	located in your project. This can be done by highlighting the file
	visual_gui.cpp in the "Project Settings" dialog box and insert a
	"." in the "Additional include directories" field located at the
	"C/C++" tab - Preprocessor.
	@version 0.30,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
class CVisualGUI
{
	public: // attributes
		ipl::CStdImage* m_pImage;
	private: // attributes
		LPBITMAPINFOHEADER m_lpBMIH;
		LPLOGPALETTE m_pLogPal;
		int m_ColorTableEntries; // number of palette entries - initialized in constructor
		//bool m_PalAndInfoHeaderAllocated;
	public: // methods
		/** constructor to initialize the private m_pImage as a CImage 
			pointer to be used with this class */
		CVisualGUI(ipl::CStdImage* pImage);
		/** destructor deallocates the m_lpBMIH and m_pLogPal if necessary */
		~CVisualGUI();
		/** Set image to be displayed. */
		void SetImage(ipl::CStdImage* pImage);
		/** Shows the image set by constructor to a Device Context DC.
			Note that if the image has a border different from 0 an internal
			copy must be created each time Show is called, this slows down
			the process of showing the image.
			@param pDC Device context pointer to do output to.
			@param Activated If true the current window is put to top 
				and we must realize palette.
			@param ZoomFactor zoom factor in image
			@param pntSrc Coordinate of the lower-left corner of the 
				CStdImage to output into rcDest.
			@return False, if the Win32 function ::StretchDIBits fails. */
		bool Show(CDC* pDC,/*CRect rcDest,*/BOOL Activated, 
			float ZoomFactor, CPoint pntSrc=0);
		/** copies to clipbrd - use OpenClipboard() first -
			Before calling CopyToClipboard, an application must open the clipboard
			by using the OpenClipboard function. Note that this method is slower
			if the image has a border different from 0.
			@param pROI A Region Of Interest, if not NULL, only this region of the image
				will be copied to the clipboard! */
		BOOL CopyToClipboard(CRect* pROI=NULL);
		/** pastes from clipbrd - use OpenClipboard() first -
			Before calling PasteFromClipboard, an application must open the clipboard
			by using the OpenClipboard function. */
		BOOL PasteFromClipboard();
	private: // methods
		// copy info stored in CStdImage class to at BITMAPINFOHEADER structure
		bool MakeBitmapInfo(LPBITMAPINFOHEADER lpBMIH, ipl::CStdImage* pImg);
		bool CreateLogPalette(LPLOGPALETTE *pLogPal, ipl::CStdImage* pImg);
};

#endif /* CVISUALGUI_H */