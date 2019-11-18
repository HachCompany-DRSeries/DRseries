// show7.h : main header file for the SHOW7 application
//

#if !defined(AFX_SHOW7_H__DD5E8B87_5D67_11D4_80E8_00C04F329F6C__INCLUDED_)
#define AFX_SHOW7_H__DD5E8B87_5D67_11D4_80E8_00C04F329F6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "IPL98ErrorView.h"
#include "IPL98ErrorDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CShow7App:
// See show7.cpp for the implementation of this class
//

class CShow7App : public CWinApp
{
public:
	CMultiDocTemplate* m_pTemplateIPL98Errors;
	CMultiDocTemplate* m_pTemplateText;
	CIPL98ErrorDoc* m_pErrorDoc;
    HWND m_ErrorViewHWND; // set when the IPL98ErrorView is created
    bool m_ThreadRunning;

	CShow7App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShow7App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CShow7App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOW7_H__DD5E8B87_5D67_11D4_80E8_00C04F329F6C__INCLUDED_)
