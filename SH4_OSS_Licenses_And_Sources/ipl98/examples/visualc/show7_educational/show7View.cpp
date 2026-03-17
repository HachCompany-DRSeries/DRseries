// show7View.cpp : implementation of the CShow7View class
//
#include "stdafx.h"
#include "show7.h"

#include "show7Doc.h"
#include "show7View.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShow7View

IMPLEMENT_DYNCREATE(CShow7View, CScrollView)

BEGIN_MESSAGE_MAP(CShow7View, CScrollView)
	//{{AFX_MSG_MAP(CShow7View)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_VIEW_ZOOM_IN, OnViewZoomIn)
	ON_COMMAND(ID_VIEW_ZOOM_OUT, OnViewZoomOut)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_EDIT_CLEAR_SELECTION, OnEditClearSelection)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_SELECTION, OnUpdateEditClearSelection)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShow7View construction/destruction

CShow7View::CShow7View()
{
	// TODO: add construction code here
	m_ZoomInvert=false;
	m_ZoomDegree=1;
	m_ZoomFactor=1;
	m_WindowName=string("ImageView");
}

CShow7View::~CShow7View()
{
}

BOOL CShow7View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CShow7View drawing

void CShow7View::OnDraw(CDC* pDC)
{
	CShow7Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	pDoc->m_VisualImg.SetImage(pDoc->m_pImg);
	if ((pDoc->m_pImg->GetWidth()!=m_Width) || (pDoc->m_pImg->GetHeight()!=m_Height))
		OnInitialUpdate();
	pDoc->m_VisualImg.Show(pDC,true,m_ZoomFactor,CPoint(0,0));
}

void CShow7View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CShow7Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	m_Width=pDoc->m_pImg->GetWidth();
	m_Height=pDoc->m_pImg->GetHeight();
	const double HEIGHT_IMAGE_WINDOW=0.64; /* fraction of mainframes client window height */
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	pFrame->RecalcLayout();
	CRect ImgClient;
	// Get valid mainframe and client window rects
	CRect rcFrm, rcClient;
	pFrame->GetWindowRect(&rcFrm);
	pFrame->GetClientRect(&rcClient);
	ImgClient.SetRect(0,0,m_Width,m_Height);
	// Calc non-client size of frame window
	int ncwidth = rcFrm.Width() - rcClient.Width();
	int ncheight = rcFrm.Height() - rcClient.Height();
	SetScrollSizes(MM_TEXT,CSize((int)(m_ZoomFactor*GetDocument()->m_pImg->GetWidth()),
        (int)(m_ZoomFactor*GetDocument()->m_pImg->GetHeight())));
	// resize it
	int Height=ImgClient.Height();
	int Width=ImgClient.Width();
	if (Height>(rcClient.Height()*HEIGHT_IMAGE_WINDOW))
		Height=rcClient.Height()*HEIGHT_IMAGE_WINDOW;
	if (Width>rcClient.Width())
		Width=rcClient.Width()-60;
	GetParent()->SetWindowPos((const CWnd*)NULL, (int)0, (int)0,
		Width+40,Height+40,SWP_NOZORDER);
//	GetParentFrame()->RecalcLayout();
//	ResizeParentToFit( /*FALSE*/ );
}


/////////////////////////////////////////////////////////////////////////////
// CShow7View printing

BOOL CShow7View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CShow7View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CShow7View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CShow7View diagnostics

#ifdef _DEBUG
void CShow7View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CShow7View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CShow7Doc* CShow7View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CShow7Doc)));
	return (CShow7Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShow7View message handlers

void CShow7View::OnEditCopy() 
{
	// TODO: Add your command handler code here
	if (OpenClipboard())
	{
		CShow7Doc* pDoc = GetDocument();
		if (((pDoc->m_RubberSelection==true) && (!pDoc->m_VisualImg.CopyToClipboard(&(pDoc->m_RubberArea)))) ||
			(!(pDoc->m_VisualImg.CopyToClipboard())))
		{
			MessageBox("Couldn't copy data to clipboard");
		}
	}	
}

void CShow7View::OnEditPaste() 
{
	// TODO: Add your command handler code here
	if (OpenClipboard())
	{
		// In CShow4Doc, "Show4" is the name of the project
		// change this to the name of your project
		CShow7Doc* pDoc = GetDocument();
		if (pDoc->m_VisualImg.PasteFromClipboard())
		{
			pDoc->SetModifiedFlag(TRUE);
			SetScrollSizes(MM_TEXT,CSize(pDoc->m_pImg->GetWidth(),
				pDoc->m_pImg->GetHeight()));
			OnInitialUpdate();
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			MessageBox("Couldn't get data from clipboard");
		}
	}	
}

void CShow7View::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDocument()->m_pImg->GetOrigin() != EMPTY);
}

void CShow7View::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB));	
}

void CShow7View::OnMouseMove(UINT nFlags, CPoint point) 
{
	CShow7Doc* pDoc = GetDocument();
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus = &pFrame->m_wndStatusBar;
	CString str;
	CPoint ptScrollPos=GetDeviceScrollPosition();
	//int x = (int)((point.x+ptScrollPos.x)/m_ZoomFactor);
	//int y = (int)((point.y+ptScrollPos.y)/m_ZoomFactor);
	CPoint2D<int> Pos((int)((point.x+ptScrollPos.x)/m_ZoomFactor),(int)((point.y+ptScrollPos.y)/m_ZoomFactor));
	if ((pStatus) && (pDoc->m_pImg->GetWidth()!=0))
	{
		Pos -= pDoc->m_pImg->GetOrigo();
		// above if is not a nice solution - but it works
		if ((Pos.GetX()>=pDoc->m_pImg->GetMaxX()) || (Pos.GetY()>=pDoc->m_pImg->GetMaxY()))
		{
			str.Format("No pixel info available");
		}
		else if (pDoc->m_pImg->GetBits()==4)
		{
			str.Format("Cannot read pixels for image with 4 b/p");
		}
		else
		{
			DWORD PixVal=pDoc->m_pImg->GetPixel(Pos);
			if (pDoc->m_pImg->m_Pal.GetNumberOfEntries()>0)
			{
				// Get the palette entry value
				pDoc->m_pImg->m_Pal.GetColor(PixVal,PixVal);
			}
			str.Format("(%3d,%3d): r=%3d g=%3d b=%3d",Pos.GetX(),Pos.GetY(),
				ipl::CPalette::GetRedVal(PixVal),ipl::CPalette::GetGreenVal(PixVal),
				ipl::CPalette::GetBlueVal(PixVal));
		}
		pStatus->SetPaneText(1,str);
	}
	
	CScrollView::OnMouseMove(nFlags, point);
}

CString CShow7View::CreateZoomString()
{
    CString str;
	if (m_ZoomInvert==TRUE)
	{
		str.Format(" [1:%d]",m_ZoomDegree);
	}
	else
	{
		str.Format(" [%d:1]",m_ZoomDegree);
	}
	return str;
}

void CShow7View::OnViewZoomIn() 
{
	// TODO: Add your command handler code here
	if (m_ZoomInvert==FALSE)
	{
		if (m_ZoomDegree>=16)
			return;
		m_ZoomDegree+=1;
		m_ZoomFactor=m_ZoomDegree;
	}
	else if (m_ZoomDegree==1) /* m_ZoomInvert=TRUE */
	{
		m_ZoomInvert=FALSE;
		m_ZoomDegree+=1;
		m_ZoomFactor=m_ZoomDegree;
	}
	else /* m_ZoomInvert=TRUE && m_ZoomDegree>1*/
	{
		m_ZoomDegree-=1;
		m_ZoomFactor=1/(float)m_ZoomDegree;
	}
	OnInitialUpdate();    
	CString str(GetDocument()->m_pImg->GetFileName());
	str += CreateZoomString();
	GetParent()->SetWindowText(str);
}

void CShow7View::OnViewZoomOut() 
{
    // TODO: Add your command handler code here
	if (m_ZoomInvert==TRUE)
	{
		if (m_ZoomDegree>=16)
			return;
		m_ZoomDegree+=1;
		m_ZoomFactor=(float)1/m_ZoomDegree;
	}
	else if (m_ZoomDegree==1) /* m_ZoomInvert=FALSE */
	{
		m_ZoomInvert=TRUE;
		m_ZoomDegree+=1;
		m_ZoomFactor=(float)1/m_ZoomDegree;
	}
	else /* m_ZoomInvert=FALSE && m_ZoomDegree>1 */
	{
		m_ZoomDegree-=1;
		m_ZoomFactor=m_ZoomDegree;
	}
	OnInitialUpdate();    
	CString str(GetDocument()->m_pImg->GetFileName());
	str += CreateZoomString();
	GetParent()->SetWindowText(str);
}

void CShow7View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CShow7Doc* pDoc = GetDocument();	
	CScrollView::OnLButtonDown(nFlags, point);
    CRectTracker tracker;
    if (tracker.TrackRubberBand(this,point))
    {
		CString str;
		CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
		CStatusBar* pStatus = &pFrame->m_wndStatusBar;
		CPoint ptScrollPos=GetDeviceScrollPosition();
		pDoc->m_RubberArea.left = (int)((tracker.m_rect.left+ptScrollPos.x)/m_ZoomFactor);
		pDoc->m_RubberArea.top = (int)((tracker.m_rect.top+ptScrollPos.y)/m_ZoomFactor);
		pDoc->m_RubberArea.right = (int)((tracker.m_rect.right+ptScrollPos.x)/m_ZoomFactor);
		pDoc->m_RubberArea.bottom = (int)((tracker.m_rect.bottom+ptScrollPos.y)/m_ZoomFactor);
		str.Format("Image selection: (%d,%d) -> (%d,%d)\n",pDoc->m_RubberArea.left,pDoc->m_RubberArea.top,
			pDoc->m_RubberArea.right,pDoc->m_RubberArea.bottom);
		pStatus->SetPaneText(0,str);
    }
	pDoc->m_RubberSelection=true;
}


void CShow7View::OnEditClearSelection() 
{
	// TODO: Add your command handler code here
	GetDocument()->m_RubberSelection=false;
}

void CShow7View::OnUpdateEditClearSelection(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDocument()->m_RubberSelection);
}

void CShow7View::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	CShow7Doc* pDoc = GetDocument();	
	
	// TODO: Add your message handler code here
	if ((!m_bInsideUpdate) && (m_nMapMode != 0)
		&& (!GetParent()->IsIconic()) && (!GetParent()->IsZoomed())
		&& (nType == SIZE_RESTORED)) {
		CRect winsize;
		GetClientRect(winsize);
		if ((winsize.right-winsize.left > m_ZoomFactor*pDoc->m_pImg->GetWidth()) ||
			(winsize.bottom-winsize.top > m_ZoomFactor*GetDocument()->m_pImg->GetHeight())) {
	
			CChildFrame* pAppFrame = (CChildFrame *)GetParentFrame();
			ASSERT_KINDOF(CChildFrame, pAppFrame);
			GetParentFrame()->RecalcLayout();
			ResizeParentToFit();
			CRect winsize;
			GetParentFrame()->GetClientRect(winsize);
			MoveWindow(0,0,winsize.right-winsize.left,
				winsize.bottom-winsize.top);
			UpdateBars();

			CPoint lastpos;
			int x, y;
			::GetCursorPos(&lastpos);
	
			RECT winRect;
			pAppFrame->GetWindowRect(&winRect);

			x = lastpos.x; y = lastpos.y;
			if (winRect.right < lastpos.x) x = winRect.right;
			if (winRect.bottom < lastpos.y) y = winRect.bottom;
			::SetCursorPos(x,y);
		}
		else UpdateBars();
	}
	
}

BOOL CShow7View::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
