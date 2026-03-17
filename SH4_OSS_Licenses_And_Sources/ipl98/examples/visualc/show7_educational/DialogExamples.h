#if !defined(AFX_DIALOGEXAMPLES_H__03955536_196A_4F92_9064_05C4D95D9252__INCLUDED_)
#define AFX_DIALOGEXAMPLES_H__03955536_196A_4F92_9064_05C4D95D9252__INCLUDED_

#pragma warning (disable : 4786)

#include <vector>
#include <string>

using std::vector;
using std::string;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogExamples.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogExamples dialog

class CDialogExamples : public CDialog
{
public: // attributes
	// The following attributes are the public ones added by me. Corresponding public attributes
	// are created by Visual C++, but they are not all standard container types, so I prefer to copy
	// the non standard types to these standard containers to avoid MFC specific code in our own code.
	vector<string> m_ComboBoxStrings;
	unsigned int m_ComboBoxSelectedIndex; // selected index in m_ComboBoxStrings - detected in OnOK()
	vector<string> m_ListBoxStrings;
	unsigned int m_ListBoxSelectedIndex; // selected index in m_ListBoxStrings - detected in OnOK()
	bool m_CheckBox;
	string m_PathFileName;
	bool m_DisableSlider;
	int m_SliderValue;
private: // attributes
// Construction
public:
	CDialogExamples(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogExamples)
	enum { IDD = IDD_DIALOG_EXAMPLE };
	CString	m_VarString;
	double	m_VarDouble;
	BOOL	m_VarCheck;
	CString	m_VarPathFileName;
	int		m_Radio;
	int		m_VarSpin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogExamples)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogExamples)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBrowse();
	virtual void OnOK();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void OnUpdatePathFileName(CCmdUI* pCmdUI);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGEXAMPLES_H__03955536_196A_4F92_9064_05C4D95D9252__INCLUDED_)
