// DialogExamples.cpp : implementation file
//

#include "stdafx.h"
#include "show7.h"
#include "DialogExamples.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogExamples dialog


CDialogExamples::CDialogExamples(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogExamples::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogExamples)
	m_VarString = _T("");
	m_VarDouble = 0.0;
	m_VarCheck = FALSE;
	m_VarPathFileName = _T("");
	m_Radio = -1;
	m_VarSpin = 0;
	//}}AFX_DATA_INIT
	m_DisableSlider=false;
}


void CDialogExamples::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogExamples)
	DDX_Text(pDX, IDC_VAR_STRING, m_VarString);
	DDX_Text(pDX, IDC_VAR_DOUBLE, m_VarDouble);
	DDX_Check(pDX, IDC_CHECK1, m_VarCheck);
	DDX_Text(pDX, IDC_EDIT_PATHFILENAME, m_VarPathFileName);
	DDX_Radio(pDX, IDC_RADIO1, m_Radio);
	DDX_Text(pDX, IDC_VAR_SPIN, m_VarSpin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogExamples, CDialog)
	//{{AFX_MSG_MAP(CDialogExamples)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_EDIT_PATHFILENAME, OnUpdatePathFileName)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogExamples message handlers

BOOL CDialogExamples::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Initialize combo box
	CComboBox* pCombo = (CComboBox*) GetDlgItem(IDC_COMBO1);
	pCombo->ResetContent();
	for(int f=0; f<m_ComboBoxStrings.size(); f++)
	{
		pCombo->AddString(m_ComboBoxStrings[f].c_str());
	}
	pCombo->SetCurSel(pCombo->FindStringExact(0,"Sony"));
	// Initialize list box
	CListBox* pItemsListBox;
	pItemsListBox=(CListBox*)GetDlgItem(IDC_LIST1);
	pItemsListBox->ResetContent();
	for(int i=0; i<m_ListBoxStrings.size(); i++)
	{
		pItemsListBox->AddString(m_ListBoxStrings[i].c_str());
	}
	pItemsListBox->SetCurSel(pItemsListBox->FindStringExact(0,"Hough"));
	// initialize check box
	m_CheckBox==true ? m_VarCheck=TRUE : m_VarCheck=FALSE;
	CButton* pCheckBox = (CButton*) GetDlgItem(IDC_CHECK1);
	pCheckBox->SetCheck(m_CheckBox);
	// initialize slider
	// Note: The Class Wizard has been used to overwrite the method 
	// OnHScroll(...) (WM_HSCROLL) in order to update the slider value 
	// realtime in the dialog box.
	CSliderCtrl* pSlider= (CSliderCtrl*) GetDlgItem(IDC_SLIDER1);
	if (m_DisableSlider==true)
	{
		pSlider->EnableWindow(FALSE);
	}
	else
	{
		const unsigned int TICS=10; // number of tic marks on slider
		pSlider->SetRange(-100,100);
		pSlider->SetPos(20);
		pSlider->SetTicFreq(200/TICS);
		CString Str;
		Str.Format("%d", 20);
		SetDlgItemText(IDC_SLIDER_VAL,Str);
	}
	// Initialize radio button - not sure this is the nice way to do it
	CButton* pRadioButton;
	switch(m_Radio)
	{
		case 0:
		{
			pRadioButton = (CButton*) GetDlgItem(IDC_RADIO1);
			break;
		}
		case 1:
		{
			pRadioButton = (CButton*) GetDlgItem(IDC_RADIO2);
			break;
		}
		default:
		{
			pRadioButton=NULL;
		}
	}
	if (pRadioButton!=NULL)
		pRadioButton->SetCheck(1);

	// Set range for the spin control button
	/*	The spin control is associated with the edit control which takes care of 
		updating the edit value. The association is a BODY option in the spin control 
		properties dialog, it automatically takes the previous z-order control. Note:
		To see the z-order control open this dialog resource and choose "Layout | Tab 
		order" in the menu. */
	CSpinButtonCtrl* pSpinCtrl=(CSpinButtonCtrl*) GetDlgItem(IDC_SPIN1);
	pSpinCtrl->SetRange(0,100);
	pSpinCtrl->SetPos(0); // initialize pos to zero

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogExamples::OnButtonBrowse() 
{
	CFileDialog dlg(TRUE,"bmp","*.bmp");
	dlg.m_ofn.lpstrTitle="Choose file";
	if (dlg.DoModal()==IDOK)
	{
		m_VarPathFileName=dlg.GetPathName();
		// also update the string variable that user can access as a public attribute
		m_PathFileName.assign(dlg.GetPathName().GetBuffer(300));
		// In order to update the Edit-field with the file-name, we update all
		// dialog controls. But we need to implement what should be done
		// when an update event is created. For this we have included the line
		// ON_UPDATE_COMMAND_UI(IDC_EDIT_PATHFILENAME, OnUpdatePathFileName)
		// in the BEGIN_MESSAGE_MAP area (see elsewhere in this file). We have then
		// implemented the method OnUpdatePathFileName(CCmdUI* pCmdUI) which will
		// be called upon this event.
		UpdateDialogControls( this, TRUE );
	}		
	
}

void CDialogExamples::OnUpdatePathFileName(CCmdUI* pCmdUI)
{
	pCmdUI->SetText(m_VarPathFileName);
}

void CDialogExamples::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString Str;
	// update slider
	CSliderCtrl* pSlider= (CSliderCtrl*) GetDlgItem(IDC_SLIDER1);
	if (pSlider->IsWindowEnabled()==TRUE) // only update if feature is available
	{
		Str.Format("%d", pSlider->GetPos());
		SetDlgItemText(IDC_SLIDER_VAL,Str);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDialogExamples::OnOK() 
{
	// Set the m_ComboBoxSelectedIndex attribute to selection
	CComboBox* pCombo = (CComboBox*) GetDlgItem(IDC_COMBO1);
	m_ComboBoxSelectedIndex=pCombo->GetCurSel();
	// Set the m_ListBoxSelectedIndex attribute to selection
	CListBox* pItemsListBox;
	pItemsListBox=(CListBox*)GetDlgItem(IDC_LIST1);
	m_ListBoxSelectedIndex=pItemsListBox->GetCurSel();
	// Set the m_CheckBox to checke or unchecked - we cannot depend on m_VarCheck since
	// it is not set untill after this method has been called
	CButton* pCheckBox = (CButton*) GetDlgItem(IDC_CHECK1);
	pCheckBox->GetCheck()==1 ? m_CheckBox=1 : m_CheckBox=0;

	// Set the m_PathFileName attribute - we cannot depend on m_VarPathFileName since
	// it is not set untill after this method has been called
	CEdit* pFileEdit= (CEdit*) GetDlgItem(IDC_EDIT_PATHFILENAME);
	CString TempStr(' ',200);
	pFileEdit->GetLine(0,TempStr.GetBuffer(1),200);
	m_PathFileName.assign(TempStr.GetBuffer(200));
	// Set the m_SliderValue attribute
	CSliderCtrl* pSlider= (CSliderCtrl*) GetDlgItem(IDC_SLIDER1);
	if (pSlider->IsWindowEnabled()==TRUE) // only update if feature is available
	{
		m_SliderValue=pSlider->GetPos();
	}
	
	CDialog::OnOK();
}
