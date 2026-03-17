// show7Doc.h : interface of the CShow7Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOW7DOC_H__DD5E8B8F_5D67_11D4_80E8_00C04F329F6C__INCLUDED_)
#define AFX_SHOW7DOC_H__DD5E8B8F_5D67_11D4_80E8_00C04F329F6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable : 4786)

#include <ipl98/cpp/image.h>
#include "visual_gui/visual_gui.h"
#include "StdString.h"
#include <vector>
#include "VectorRichEdit.h"

using namespace ipl;

class CShow7Doc : public CDocument
{
protected: // create from serialization only
	CShow7Doc();
	DECLARE_DYNCREATE(CShow7Doc)

// Attributes
public:
	// always points to active image
	CImage* m_pImg;
	// available image objects
	CImage m_Img1,m_Img2,m_Img3,m_Img4;
	/// Set to 1,2,3 or 4
	unsigned int m_ActiveImage;
	CVisualGUI m_VisualImg;
	CVectorRichEdit m_History;
	CRect m_RubberArea; // image positions if a rubberband selection has been made
	bool m_RubberSelection; // true if a rubberband selection has been made
    void Write(char * s); //Writes a string in the text window
	void WriteLn(); //New line in the text window
	void Write(int i); //Writes an integer in a 10 character wide field
	void Write(double x); //Write a double variable in a 10 character wide field
	void WriteInt(int i, int Width); //Writes an integer in a field of specified width
	void WriteFixed(double x, int Width, int Decimals);
	                       //Writes a double using fixed point format.      
	void WriteExp(double x, int Width);
	                       //Writes a double using exponential format 
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShow7Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShow7Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CShow7Doc)
	afx_msg void OnViewDebugWindow();
	afx_msg void OnViewCleardebugwindow();
	afx_msg void OnRunA();
	afx_msg void OnViewImage1();
	afx_msg void OnViewImage2();
	afx_msg void OnViewImage3();
	afx_msg void OnViewImage4();
	afx_msg void OnFileOpenimage2();
	afx_msg void OnFileOpenimage3();
	afx_msg void OnFileOpenimage4();
	afx_msg void OnFileSaveviewedimageas();
	afx_msg void OnRunB();
	afx_msg void OnRunC();
	afx_msg void OnRunD();
	afx_msg void OnKeyboardinputValue1();
	afx_msg void OnKeyboardinputValue2();
	afx_msg void OnKeyboardinputValue3();
	afx_msg void OnKeyboardinputValue4();
	afx_msg void OnRunE();
	afx_msg void OnRunF();
	afx_msg void OnHelpDialog();
	afx_msg void OnHelpFormattingtext();
	afx_msg void OnHelpReadingandwritingfiles();
	afx_msg void OnFileOpenimage1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	/** Forces an update of all views attached to this document. Do not
		call this method from another thread, it will not be able to
		uptain the correct views and will crash the system. */
	void UpdateViewsNow();
	/** Returns a pointer to the view with the name WindowName. If you add your
		own view classes and want this method to handle them, you must add a
		few lines of code, see implementation for details. If a new view class
		does not include an attribute m_WindowName, this method will fail!
		@param WindowName Name of window to get a reference to. In the view class
		the window is found by comparing the attribute m_WindowName
		with the string supplied by this method.
		@param pView Pointer to the found window, value undefined if method fails.
		@return False, if the supplied WindowName is not found. */
	bool GetWindow(string WindowName, CView** ppView);
	void OutputImageInfo();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOW7DOC_H__DD5E8B8F_5D67_11D4_80E8_00C04F329F6C__INCLUDED_)
