#if !defined(AFX_IPL98ERRORVIEW_H__16412524_377F_11D4_80E1_00C04F329F6C__INCLUDED_)
#define AFX_IPL98ERRORVIEW_H__16412524_377F_11D4_80E1_00C04F329F6C__INCLUDED_

#include "StdString.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IPL98ErrorView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIPL98ErrorView view

class CIPL98ErrorView : public CEditView
{
protected:
	DECLARE_DYNCREATE(CIPL98ErrorView)

// Attributes
public:
	CIPL98ErrorView();           // protected constructor used by dynamic creation
private:
	unsigned int m_ErrorHistorySize; // count the total lines drawn, used to check if new
							   // lines were added since last update
	CStdString m_TextInWindow;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPL98ErrorView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CIPL98ErrorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CIPL98ErrorView)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPL98ERRORVIEW_H__16412524_377F_11D4_80E1_00C04F329F6C__INCLUDED_)
