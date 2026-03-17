// IPL98ErrorDoc.cpp : implementation file
//

#include "stdafx.h"
#include "show7.h"
#include "IPL98ErrorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIPL98ErrorDoc

IMPLEMENT_DYNCREATE(CIPL98ErrorDoc, CDocument)

CIPL98ErrorDoc::CIPL98ErrorDoc() : m_Semaphore(1,1)
{
}

BOOL CIPL98ErrorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CIPL98ErrorDoc::~CIPL98ErrorDoc()
{
}


BEGIN_MESSAGE_MAP(CIPL98ErrorDoc, CDocument)
	//{{AFX_MSG_MAP(CIPL98ErrorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIPL98ErrorDoc diagnostics

#ifdef _DEBUG
void CIPL98ErrorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIPL98ErrorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIPL98ErrorDoc serialization

void CIPL98ErrorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CIPL98ErrorDoc commands

void CIPL98ErrorDoc::UpdateViewsNow()
{
	//CSingleLock Safe(&(m_Semaphore));
	//if (Safe.IsLocked()==0)
	{
		POSITION pos = GetFirstViewPosition();
		while (pos != NULL)
		{
			CIPL98ErrorView* pView = (CIPL98ErrorView*)GetNextView(pos);
			pView->Invalidate();
			pView->UpdateWindow();
		}
		UpdateAllViews(NULL);
	}
}
