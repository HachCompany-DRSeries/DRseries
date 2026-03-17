#if !defined(AFX_TEXTVIEW_H__F3810D81_00DC_11D4_80D9_00C04F329F6C__INCLUDED_)
#define AFX_TEXTVIEW_H__F3810D81_00DC_11D4_80D9_00C04F329F6C__INCLUDED_

#include "StdString.h"
#include <string>
using namespace std;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextView view

/** A View class for the programmer to output simple text, last updated 20/7/2000.
	@see CVectorRichEdit
	@version 0.2,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
class CTextView : public CEditView
{

// Attributes
public:
	/// used to check how many lines to append since last update
	unsigned int m_SizeHistory;
	CStdString m_TextInWindow;
	/** Window name to recognize this window, in the CDocument the method GetWindow(string)
		can be used to get an object of this type, where string is the value contained
		in this attribute. Initialize to "TextView". */
	string m_WindowName;
// Operations
public:
	void UpdateNow();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextView)
	public:
	/** Sets the scrollbars and positions the window at the bottum of the main frame window. */
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	/** Takes the contest of the documents vector attribute m_History and stores it
		in this class' m_TextInWindow string. */
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTextView();
	CTextView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTextView)
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTVIEW_H__F3810D81_00DC_11D4_80D9_00C04F329F6C__INCLUDED_)
