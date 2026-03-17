// TextView.cpp : implementation file
//

#include "stdafx.h"
#include "show7.h"
#include "TextView.h"
#include "Show7Doc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextView

IMPLEMENT_DYNCREATE(CTextView, CEditView)

CTextView::CTextView()
{
	m_SizeHistory=0;
	m_WindowName=string("TextView");
}

CTextView::~CTextView()
{
}


BEGIN_MESSAGE_MAP(CTextView, CEditView)
	//{{AFX_MSG_MAP(CTextView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextView drawing

void CTextView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTextView diagnostics

#ifdef _DEBUG
void CTextView::AssertValid() const
{
	CEditView::AssertValid();
}

void CTextView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextView message handlers

void CTextView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CShow7Doc* pDoc = (CShow7Doc*)GetDocument();
	if (pDoc->m_History.size()<m_SizeHistory)
	{
		// the history in CDocument must have been emptied
		m_TextInWindow.erase();
		m_SizeHistory=0;
	}
	if (m_TextInWindow.capacity()<(m_TextInWindow.size()+200))
		m_TextInWindow.reserve(m_TextInWindow.size()+20000);
	// TODO: add draw code here
	for(unsigned int it=m_SizeHistory;it<pDoc->m_History.size();it++)
	{
		m_TextInWindow+=pDoc->m_History[it];
	}
	SetWindowText(m_TextInWindow);
	m_SizeHistory=pDoc->m_History.size();
	// Scroll the edit control
	GetEditCtrl().LineScroll(GetEditCtrl().GetLineCount(), 0);
	
	
}

void CTextView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
	const double HEIGHT_TEXT_WINDOW=0.33; /* fraction of mainframes client window height */
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	pFrame->RecalcLayout();
	// Get valid mainframe and client window rects
	CRect rcFrm, rcClient;
	pFrame->GetWindowRect(&rcFrm);
	pFrame->GetClientRect(&rcClient);
	// Calc non-client size of frame window
	int ncwidth = rcFrm.Width() - rcClient.Width();
	int ncheight = rcFrm.Height() - rcClient.Height();
	// resize it
	GetParent()->SetWindowPos((const CWnd*)NULL, (int)0, (int)(rcClient.Height()*(1-HEIGHT_TEXT_WINDOW)),
		rcClient.Width()-ncwidth-4 + ncwidth,
		rcClient.Height()*HEIGHT_TEXT_WINDOW-ncheight-4,
		SWP_NOZORDER);
	
	// TODO: Add your specialized code here and/or call the base class
	GetEditCtrl().SetReadOnly();
}

void CTextView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CShow7Doc* pDoc = (CShow7Doc*)GetDocument();
	CEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	CStdString str;
	str=pDoc->m_pImg->GetFileName();
	GetParentFrame()->SetWindowText(str + " - Text output");
	
}

void CTextView::UpdateNow()
{
	OnUpdate(NULL,0,NULL);
}
