// IPL98ErrorView.cpp : implementation file
//

#include "stdafx.h"
#include "show7.h"
#include "IPL98ErrorDoc.h"
#include "IPL98ErrorView.h"
#include "MainFrm.h"
#include "VectorRichEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CVectorRichEdit IPL98ErrorHistory;

void AddIPL98ErrorHistory(const char* str)
{
	IPL98ErrorHistory.push_back(str);
	CShow7App* pApp = (CShow7App*) AfxGetApp();
	if(pApp->m_ThreadRunning==false)
	{
		// okay to update via the document class
		pApp->m_pErrorDoc->UpdateViewsNow();
	}
	else
	{
		// thread running - only send a PAINT message so the
		// main thread takes care of updating
		::PostMessage(pApp->m_ErrorViewHWND,WM_PAINT,0,0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CIPL98ErrorView

IMPLEMENT_DYNCREATE(CIPL98ErrorView, CEditView)

CIPL98ErrorView::CIPL98ErrorView()
{
	m_ErrorHistorySize=0;
}

CIPL98ErrorView::~CIPL98ErrorView()
{
}


BEGIN_MESSAGE_MAP(CIPL98ErrorView, CEditView)
	//{{AFX_MSG_MAP(CIPL98ErrorView)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIPL98ErrorView drawing

void CIPL98ErrorView::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CIPL98ErrorView diagnostics

#ifdef _DEBUG
void CIPL98ErrorView::AssertValid() const
{
	CEditView::AssertValid();
}

void CIPL98ErrorView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIPL98ErrorView message handlers

void CIPL98ErrorView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();

	CShow7App* pApp = (CShow7App*) AfxGetApp();
    pApp->m_ErrorViewHWND=m_hWnd;

	const double HEIGHT_TEXT_WINDOW=0.50; /* fraction of mainframes client window height */
	const double WIDTH_TEXT_WINDOW=0.25; /* fraction of mainframes client window height */
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
	GetParent()->SetWindowPos((const CWnd*)NULL, (int)(rcClient.Width()*(1-WIDTH_TEXT_WINDOW)),0,
		(int)((rcClient.Width()-ncwidth-4)*WIDTH_TEXT_WINDOW),(int)(rcClient.Height()*HEIGHT_TEXT_WINDOW),
		SWP_NOZORDER);
	
	// TODO: Add your specialized code here and/or call the base class
	GetEditCtrl().SetReadOnly();	
}

void CIPL98ErrorView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CIPL98ErrorDoc* pDoc = (CIPL98ErrorDoc*)GetDocument();
	CSingleLock Safe(&(pDoc->m_Semaphore));
	//Safe.Lock();
	if (Safe.Lock(5)!=0)
	{
		GetParentFrame()->SetWindowText("IPL98 Errors and Warnings");
		if (IPL98ErrorHistory.size()<m_ErrorHistorySize)
		{
			// the history in the ErrorHistory must have been emptied
			m_TextInWindow.erase();
			m_ErrorHistorySize=0;
		}
		if (m_TextInWindow.capacity()<(m_TextInWindow.size()+200))
		{
			m_TextInWindow.reserve(m_TextInWindow.size()+20000);
		}
		// TODO: add draw code here
		if (m_ErrorHistorySize<IPL98ErrorHistory.size())
		{
			for(unsigned int it=m_ErrorHistorySize;it<IPL98ErrorHistory.size();it++)
			{
				m_TextInWindow+=IPL98ErrorHistory[it];
			}
			SetWindowText(m_TextInWindow);
			
			// Scroll the edit control
			GetEditCtrl().LineScroll(GetEditCtrl().GetLineCount(), 0);
			m_ErrorHistorySize=IPL98ErrorHistory.size();
		}
		Invalidate();
		Safe.Unlock();
	}	
}

void CIPL98ErrorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CIPL98ErrorDoc* pDoc = (CIPL98ErrorDoc*)GetDocument();
	CSingleLock Safe(&(pDoc->m_Semaphore));
	Safe.Lock();
	CEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	CStdString str;
	GetParentFrame()->SetWindowText("IPL98 Errors and Warnings");
	Safe.Unlock();
	OnUpdate(NULL,0,NULL);
	UpdateWindow();
}

void CIPL98ErrorView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	OnUpdate(NULL,0,NULL);	
	// Do not call CEditView::OnPaint() for painting messages
	CEditView::OnPaint();
}

