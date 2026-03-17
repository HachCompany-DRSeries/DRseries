#if !defined(AFX_IPL98ERRORDOC_H__16412525_377F_11D4_80E1_00C04F329F6C__INCLUDED_)
#define AFX_IPL98ERRORDOC_H__16412525_377F_11D4_80E1_00C04F329F6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IPL98ErrorDoc.h : header file
//

#include <afxmt.h>

/////////////////////////////////////////////////////////////////////////////
// CIPL98ErrorDoc document

class CIPL98ErrorDoc : public CDocument
{
protected:
	CIPL98ErrorDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CIPL98ErrorDoc)

// Attributes
public:
	CSemaphore m_Semaphore; // controls safe thread access

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPL98ErrorDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateViewsNow();
	virtual ~CIPL98ErrorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CIPL98ErrorDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPL98ERRORDOC_H__16412525_377F_11D4_80E1_00C04F329F6C__INCLUDED_)
