// show7View.h : interface of the CShow7View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOW7VIEW_H__DD5E8B91_5D67_11D4_80E8_00C04F329F6C__INCLUDED_)
#define AFX_SHOW7VIEW_H__DD5E8B91_5D67_11D4_80E8_00C04F329F6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CShow7View : public CScrollView
{
protected: // create from serialization only
	CShow7View();
	DECLARE_DYNCREATE(CShow7View)

// Attributes
public:
	CShow7Doc* GetDocument();
	BOOL m_ZoomInvert; /* if true the actual zoom is 1/m_ZoomFactor */
	int m_ZoomDegree; /* the degree af zoom, from 1-16 */
	float m_ZoomFactor; /* zoom factor for image to be viewed */// Operations
	/** Window name to recognize this window, in the CDocument the method GetWindow(string)
		can be used to get an object of this type, where string is the value contained
		in this attribute. Initialize to "ImageView". */
	string m_WindowName;
	unsigned int m_Width;
	unsigned int m_Height;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShow7View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShow7View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CShow7View)
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnViewZoomIn();
	afx_msg void OnViewZoomOut();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditClearSelection();
	afx_msg void OnUpdateEditClearSelection(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString CreateZoomString();
};

#ifndef _DEBUG  // debug version in show7View.cpp
inline CShow7Doc* CShow7View::GetDocument()
   { return (CShow7Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOW7VIEW_H__DD5E8B91_5D67_11D4_80E8_00C04F329F6C__INCLUDED_)
