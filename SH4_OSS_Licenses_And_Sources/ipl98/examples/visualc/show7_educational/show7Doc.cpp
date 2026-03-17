// show7Doc.cpp : implementation of the CShow7Doc class
//

#include "stdafx.h"
#include "show7.h"
#include <time/time_date.h>
#include "show7Doc.h"
#include "show7view.h"
#include "textview.h"
#include "InputDlg.h"
#include "DialogExamples.h"
#include <ipl98/cpp/algorithms/graph_plot2d.h>
#include <points/point3d.h>
#include <ipl98/cpp/ipl98_general_functions.h>
#include <iomanip>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShow7Doc

IMPLEMENT_DYNCREATE(CShow7Doc, CDocument)

BEGIN_MESSAGE_MAP(CShow7Doc, CDocument)
	//{{AFX_MSG_MAP(CShow7Doc)
	ON_COMMAND(ID_VIEW_DEBUG_WINDOW, OnViewDebugWindow)
	ON_COMMAND(ID_VIEW_CLEARDEBUGWINDOW, OnViewCleardebugwindow)
	ON_COMMAND(ID_RUN_A, OnRunA)
	ON_COMMAND(ID_VIEW_IMAGE1, OnViewImage1)
	ON_COMMAND(ID_VIEW_IMAGE2, OnViewImage2)
	ON_COMMAND(ID_VIEW_IMAGE3, OnViewImage3)
	ON_COMMAND(ID_VIEW_IMAGE4, OnViewImage4)
	ON_COMMAND(ID_FILE_OPENIMAGE2, OnFileOpenimage2)
	ON_COMMAND(ID_FILE_OPENIMAGE3, OnFileOpenimage3)
	ON_COMMAND(ID_FILE_OPENIMAGE4, OnFileOpenimage4)
	ON_COMMAND(ID_FILE_SAVEVIEWEDIMAGEAS, OnFileSaveviewedimageas)
	ON_COMMAND(ID_RUN_B, OnRunB)
	ON_COMMAND(ID_RUN_C, OnRunC)
	ON_COMMAND(ID_RUN_D, OnRunD)
	ON_COMMAND(ID_KEYBOARDINPUT_VALUE1, OnKeyboardinputValue1)
	ON_COMMAND(ID_KEYBOARDINPUT_VALUE2, OnKeyboardinputValue2)
	ON_COMMAND(ID_KEYBOARDINPUT_VALUE3, OnKeyboardinputValue3)
	ON_COMMAND(ID_KEYBOARDINPUT_VALUE4, OnKeyboardinputValue4)
	ON_COMMAND(ID_RUN_E, OnRunE)
	ON_COMMAND(ID_RUN_F, OnRunF)
	ON_COMMAND(ID_HELP_DIALOG, OnHelpDialog)
	ON_COMMAND(ID_HELP_FORMATTINGTEXT, OnHelpFormattingtext)
	ON_COMMAND(ID_HELP_READINGANDWRITINGFILES, OnHelpReadingandwritingfiles)
	ON_COMMAND(ID_FILE_OPENIMAGE1, OnFileOpenimage1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShow7Doc construction/destruction

CShow7Doc::CShow7Doc() : m_VisualImg(&m_Img1)
{
	// TODO: add one-time construction code here
	m_RubberSelection=false;
	m_ActiveImage=1;
	m_pImg=&m_Img1;
}

CShow7Doc::~CShow7Doc()
{
}

BOOL CShow7Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CShow7Doc serialization

void CShow7Doc::Serialize(CArchive& ar)
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
// CShow7Doc diagnostics

#ifdef _DEBUG
void CShow7Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CShow7Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShow7Doc commands

void CShow7Doc::Write(char * s)	{
	CStdString S(s);S.Replace(" ","_");m_History.push_back0(S);UpdateViewsNow();}
void CShow7Doc::WriteLn(){Write("\n");}
void CShow7Doc::Write(int i){char s[12];m_History.Str(i,10,s);Write(s);};
void CShow7Doc::Write(double x){
	if ((fabs(x)>1e3)||(fabs(x)<1e-1))WriteExp(x,10); else WriteFixed(x,10,4);};
void CShow7Doc::WriteInt(int i, int Width){
    char s[30];m_History.Str(i,Width,s);Write(s);};
void CShow7Doc::WriteFixed(double x, int Width, int Decimals){
	char s[30];m_History.Str(x,Width,Decimals,s);Write(s);};
void CShow7Doc::WriteExp(double x, int Width){
    char s[30];m_History.Str(x,Width,s);Write(s);};

BOOL CShow7Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	if (!m_Img1.Load(lpszPathName))
	{
		AfxMessageBox("Load failed");
		return FALSE;
	}	
	else
	{
       // Create the text output window
        CDocTemplate* pTemplate=((CShow7App*) AfxGetApp())->m_pTemplateText;
        CFrameWnd* pFrame=pTemplate->CreateNewFrame(this,NULL);
        pTemplate->InitialUpdateFrame(pFrame,this);

		m_History.push_back("Image loaded into m_Img1");
		OutputImageInfo();
		UpdateViewsNow();

		return TRUE;
	}
}

BOOL CShow7Doc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	return m_Img1.Save(lpszPathName);
}

void CShow7Doc::OutputImageInfo()
{
	CStdString str("**************** Image info ********************");
    m_History.push_back(str);
    str.Format("Width=%d Height=%d ByteWidth=%d SizeImage=%d Bits=%d",
		m_pImg->GetWidth(),m_pImg->GetHeight(),m_pImg->GetLineByteWidth(),
		(m_pImg->GetConstTImagePtr())->TotalSize,m_pImg->GetBits());
    m_History.push_back(str);
}

void CShow7Doc::UpdateViewsNow()
{
    POSITION pos = GetFirstViewPosition();
    while (pos != NULL)
    {
        CView* pView = GetNextView(pos);
        pView->Invalidate();
        pView->UpdateWindow();
    }
    UpdateAllViews(NULL);
}

bool CShow7Doc::GetWindow(string WindowName, CView** ppView)
{	
	CView* pTempView;
	bool ViewFound=false;
	POSITION pos = GetFirstViewPosition();
	while ((pos != NULL) && (ViewFound==false))
	{
		pTempView = GetNextView(pos);
		// if a new window class is define add a new "else if"
		// below with the new class name
		if ((*ppView = dynamic_cast<CShow7View*>(pTempView)) && 
			(((CShow7View*)*ppView)->m_WindowName==WindowName))
		{
			ViewFound=true;
		}
		else if ((*ppView = dynamic_cast<CTextView*>(pTempView)) && 
			(((CTextView*)*ppView)->m_WindowName==WindowName))
		{
			ViewFound=true;
		}
	}
	return ViewFound;
}

void CShow7Doc::OnViewDebugWindow() 
{
	// TODO: Add your command handler code here
	CDocTemplate* pTemplate=((CShow7App*) AfxGetApp())->m_pTemplateText;
	CFrameWnd* pFrame=pTemplate->CreateNewFrame(this,NULL);
	pTemplate->InitialUpdateFrame(pFrame,this);    	
}

void CShow7Doc::OnViewCleardebugwindow() 
{
	// TODO: Add your command handler code here
	m_History.clear();
	UpdateViewsNow();	
}

void CShow7Doc::OnViewImage1() 
{
	// TODO: Add your command handler code here
	if ((m_Img1.GetWidth()>0)&&(m_Img1.GetHeight()>0))
	{
		m_ActiveImage=1;
		m_pImg=&m_Img1;
		UpdateViewsNow();
	}
}

void CShow7Doc::OnViewImage2() 
{
	// TODO: Add your command handler code here
	if ((m_Img2.GetWidth()>0)&&(m_Img2.GetHeight()>0))
	{
	  m_ActiveImage=2;
	  m_pImg=&m_Img2;
	  UpdateViewsNow();
	}
}

void CShow7Doc::OnViewImage3() 
{
	// TODO: Add your command handler code here
	if ((m_Img3.GetWidth()>0)&&(m_Img3.GetHeight()>0))
	{
		m_ActiveImage=3;
	    m_pImg=&m_Img3;
	    UpdateViewsNow();
	}
}

void CShow7Doc::OnViewImage4() 
{
	// TODO: Add your command handler code here
	if ((m_Img4.GetWidth()>0)&&(m_Img4.GetHeight()>0))
	{
		m_ActiveImage=4;
	    m_pImg=&m_Img4;	
	    UpdateViewsNow();
	}
}

void CShow7Doc::OnFileOpenimage1() 
{
	CStdString tempStr;
	CFileDialog dlg(TRUE,"bmp","*.bmp");
	dlg.m_ofn.lpstrTitle="Open Image1 file";
	if (dlg.DoModal()==IDOK)
	{
		tempStr=dlg.GetPathName();
		if (m_Img1.Load(tempStr)==false)
		{
			AfxMessageBox("Load failed");
		}
		else
		{
			m_History.push_back("Image loaded into m_Img1");
			OutputImageInfo();
		}
	}
	OnViewImage1(); // activates image and updates window
}

void CShow7Doc::OnFileOpenimage2() 
{
	CStdString tempStr;
	CFileDialog dlg(TRUE,"bmp","*.bmp");
	dlg.m_ofn.lpstrTitle="Open Image2 file";
	if (dlg.DoModal()==IDOK)
	{
		tempStr=dlg.GetPathName();
		if (m_Img2.Load(tempStr)==false)
		{
			AfxMessageBox("Load failed");
		}
		else
		{
			m_History.push_back("Image loaded into m_Img2");
			OutputImageInfo();
		}
	}
	OnViewImage2(); // activates image and updates window
}
  
void CShow7Doc::OnFileOpenimage3() 
{
	CStdString tempStr;
	CFileDialog dlg(TRUE,"bmp","*.bmp");
	dlg.m_ofn.lpstrTitle="Open Image3 file";
	if (dlg.DoModal()==IDOK)
	{
		tempStr=dlg.GetPathName();
		if (m_Img3.Load(tempStr)==false)
		{
			AfxMessageBox("Load failed");
		}
		else
		{
			m_History.push_back("Image loaded into m_Img3");
			OutputImageInfo();
		}
	}
	OnViewImage3(); // activates image and updates window
}

void CShow7Doc::OnFileOpenimage4() 
{
	CStdString tempStr;
	CFileDialog dlg(TRUE,"bmp","*.bmp");
	dlg.m_ofn.lpstrTitle="Open Image4 file";
	if (dlg.DoModal()==IDOK)
	{
		tempStr=dlg.GetPathName();
		if (m_Img4.Load(tempStr)==false)
		{
			AfxMessageBox("Load failed");
		}
		else
		{
			m_History.push_back("Image loaded into m_Img4");
			OutputImageInfo();
		}
	}
	OnViewImage4(); // activates image and updates window
}

void CShow7Doc::OnFileSaveviewedimageas() 
{
	// TODO: Add your command handler code here
	ostringstream ost;
	ost << "Saving ";
	switch(m_ActiveImage)
	{
	case 1:
		ost << "Image1 As";
		break;
	case 2:
		ost << "Image2 As";
		break;
	case 3:
		ost << "Image3 As";
		break;
	case 4:
		ost << "Image4 As";
		break;
	}
	CFileDialog dlg(TRUE,"bmp","*.bmp");
	dlg.m_ofn.lpstrTitle=ost.str().c_str();
	if (dlg.DoModal()==IDOK)
	{
		CString FilePathName;
		if (dlg.GetFileTitle()=="")
		{
			AfxMessageBox("ERROR: Need to specify a filename!");
			return;
		}
		FilePathName=dlg.GetPathName();
		if (m_pImg->Save(FilePathName.GetBuffer(400))==false)
		{
			AfxMessageBox("ERROR: Need to specify a filename!");
			return;
		}
	}
}

void CShow7Doc::OnRunA() 
{
	// TODO: Add your command handler code here
	//This procedure flips the image z1 about a
	//horizontal axis. The image m_Img2 holds the flipped
	//image
	int x,y; //declaration of counting variables
	m_Img2=m_Img1;   
	for (y=0;y<m_Img1.GetHeight();y++)
		for (x=0;x<m_Img1.GetWidth();x++)
			m_Img2.SetPixel(x,y,m_Img1.GetPixel(x,m_Img1.GetHeight()-1-y));
		OnViewImage2();
}

void CShow7Doc::OnRunB() 
{
	// TODO: Add your command handler code here
	//This procedure takes a cut from image m_Img1.
	//The cut is defined by the current rubber frame
	//The image m_Img2 holds the cut, and the 
	//cut coordinates are written in the text window
	int x1,x2,y1,y2,x,y;
	x1=m_RubberArea.left;
	y1=m_RubberArea.top;
	x2=m_RubberArea.right;
	y2=m_RubberArea.bottom;;
	m_Img2=CImage(x2-x1+1,y2-y1+1,m_Img1.GetBits());
	if ((x2-x1>2)&&(y2-y1>2))
	{
		for (y=y1;y<=y2;y++)
			for (x=x1;x<=x2;x++)
				m_Img2.SetPixel(x-x1,y-y1,m_Img1.GetPixel(x,y));
	}
	OnViewImage2();	
	Write("Upper left : (");WriteInt(x1,3);Write(",");WriteInt(y1,3);Write(")");WriteLn();
	Write("Lower right: (");WriteInt(x2,3);Write(",");WriteInt(y2,3);Write(")");WriteLn();
}

void CShow7Doc::OnRunC() 
{
	//Example of using CPlot2D
	CGraphPlot2D Pl(m_Img1);
	Pl.InitPlot(0,6,(float)1e-8,1,400,400,CGraphPlot2D::LINLOG);
	Pl.MoveTo(0,sin(10));
	for (float x=0;x<8;x+=(float)0.01)
		Pl.LineTo(x,sin(10*x),ipl::CPalette::CreateRGB(255,0,0));
	Pl.PlotPoint(2,0.5,0xFF00);
	Pl.PlotCircle(2,0.5,10,0xFF0000);
	OnViewImage1();
}

void CShow7Doc::OnRunD() 
{
	// TODO: Add your command handler code here
	
}
void CShow7Doc::OnRunE() 
{
	// TODO: Add your command handler code here
	
}

void CShow7Doc::OnRunF() 
{
	// TODO: Add your command handler code here
	
}
void CShow7Doc::OnKeyboardinputValue1() 
{
	// TODO: Add your command handler code here
    CInputDlg Dlg;
	float x;
	if (Dlg.DoModal()==IDOK)
	{
		x=Dlg.m_InputValue;
	}
	Write(x);WriteLn();
	
}

void CShow7Doc::OnKeyboardinputValue2() 
{
	// TODO: Add your command handler code here
	
}

void CShow7Doc::OnKeyboardinputValue3() 
{
	// TODO: Add your command handler code here
	
}

void CShow7Doc::OnKeyboardinputValue4() 
{
	// TODO: Add your command handler code here
	
}


void CShow7Doc::OnHelpDialog() 
{
	CDialogExamples Dlg;
	// initialize combo box values
	Dlg.m_ComboBoxStrings.push_back("Panasonic");
	Dlg.m_ComboBoxStrings.push_back("Sony");
	Dlg.m_ComboBoxStrings.push_back("Basler");
	// initialize simple variables
	Dlg.m_VarDouble=3.14;
	Dlg.m_VarString="Vision"; // here I use the MS CString type directly
	// Initialize list box strings
	Dlg.m_ListBoxStrings.push_back("Hough");
	Dlg.m_ListBoxStrings.push_back("Threshold");
	Dlg.m_ListBoxStrings.push_back("Segmentate");
	// Set check box to checked
	Dlg.m_CheckBox=true;
	// Set the second radio button to active
	Dlg.m_Radio=1; // the first is 0, and -1 is none (default)
	// Enable slider - set to false to see a disable slider
	Dlg.m_DisableSlider=false;
	// Set file field to something...
	Dlg.m_PathFileName.assign("d:/temp/test.bmp");

	int Response=Dlg.DoModal();
	if (Response==IDOK)
	{
		ostringstream ost;
		// Now we can simply get the data by accessing the public members
		// get the combobox selection
		ost << "ComboBox selected: " << Dlg.m_ComboBoxStrings[Dlg.m_ComboBoxSelectedIndex];
		m_History.push_back(ost.str());
		// get the listbox selection
		ost.str(""); // empty the ost object so it can be reused
		ost << "ListBox selected: " << Dlg.m_ListBoxStrings[Dlg.m_ListBoxSelectedIndex];
		m_History.push_back(ost.str());
		// get the checked or unchecked state
		ost.str("");
		ost.setf(ios_base::boolalpha); // ensure that a boolean is written with symbolic value
		ost << "Check1 checked: " << Dlg.m_CheckBox;
		m_History.push_back(ost.str());
		// get file name from Edit field
		ost.str("");
		ost << "File: " << Dlg.m_PathFileName;
		m_History.push_back(ost.str());
		// get slider value
		ost.str("");
		ost << "Slider value: " << Dlg.m_SliderValue;
		m_History.push_back(ost.str());
		// get spin value
		ost.str("");
		ost << "Spin value: " << Dlg.m_VarSpin;
		m_History.push_back(ost.str());
		// get double value
		ost.str("");
		ost << "Double value: " << Dlg.m_VarDouble;
		m_History.push_back(ost.str());
		// get string value - I use the MS CString type directly
		ost.str("");
		ost << "String value: " << Dlg.m_VarString.GetBuffer(300);
		m_History.push_back(ost.str());
		// get selected radio button value
		ost.str("");
		ost << "Radio button selected: " << Dlg.m_Radio;
		m_History.push_back(ost.str());

		UpdateViewsNow();	
	}
	else if (Response==IDCANCEL)
	{
		AfxMessageBox("You cancelled - loosing all data in dialog");
	}
}

void CShow7Doc::OnHelpFormattingtext() 
{
	// This is an example on using the std library ostringstream to format strings etc.
	ostringstream ost;
	ios_base::fmtflags Flags=ost.flags(); // remember original flags
	double MyDouble1=1.23, MyDouble2=4.5678;
	int MyInt1=432, MyInt2=27;
	CPoint3D<double> P(0.9,1.1,2.7); // an IPL98 CPoint3D type

	ost << "Default double output: " 
		<< endl << "    " << ipl::PI << " " << ipl::PI2
		<< endl << "    " << MyDouble1 << " " << MyDouble2 
		<< endl;

	ost << scientific << std::setiosflags(ios_base::right);
	ost << "Adjusted double output with scientific notation: " 
		<< endl << "    " << ipl::PI << " " << ipl::PI2
		<< endl << "    " << MyDouble1 << " " << MyDouble2 
		<< endl;
	ost.setf(ios_base::fixed,ios_base::floatfield);
	ost << setprecision(3); // output with 3 decimals
	ost << "Adjusted double output with fixed format: " 
		<< endl << "    " << ipl::PI << " " << ipl::PI2
		<< endl << "    " << MyDouble1 << " " << MyDouble2
		<< endl;

	ost.setf(ios::right,ios::adjustfield);
	ost << setfill('0') << std::setw(9) << std::setiosflags(ios_base::right);
	ost << "Adjusted integer output with fill: " 
		<< endl << "    " << setfill('0') << std::setw(4) << MyInt1 
		<< endl << "    " << setfill('#') << std::setw(4) << MyInt2
		<< endl;
	
	ost.flags(Flags); // set original flags
	ost << "Point=" << P << endl; // most container classes in IPL98 can be streamed directly

	m_History.push_back(ost.str());
	ost.str(""); // empty the ostringstream object
	UpdateViewsNow();	
}

void CShow7Doc::OnHelpReadingandwritingfiles() 
{
	// Writes and read a simple setup file. Lines starting with # are comment lines
	double VarDouble=7.2;
	int VarInt=3;
	string ImageFile("george.bmp");
	// filename of the setup file to be written
	string FileName("test.txt");
	// write a setup file
	ofstream os(FileName.c_str());
	if (!os)
	{
		AfxMessageBox("Failed opening file for writing");
		return;
	}
	else
	{
		os << "# Test example written by Show7 - Educational" << endl;
		os << "# This is a comment line" << endl << endl;
		os << "# First variable is a double used in an algorithm" << endl;
		os << "VarToAlgorithm1: " <<  VarDouble << endl << endl;
		os << "# Second variable is an integer for another algorithm" << endl;
		os << "VarToAlgorithm2:   " << VarInt << endl << endl;
		os << "# Third a string with a file name" << endl;
		os << "FileName: " << ImageFile.c_str() << endl;
		os.close();
	}	

	// empty the variables
	VarDouble=0;
	VarInt=0;
	ImageFile.erase();
	// now read the file
	ifstream is(FileName.c_str());
	if (!is)
	{
		AfxMessageBox("Failed opening file for reading");
		return;
	}
	else
	{
		// we don't check the order of tokens in this example - we simply skip them
		char* pStr = new char[64];
		ipl::SkipSpaceAndComments(is);
		// skip 'VarToAlgorithm1:' token
		is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':'));
		ipl::SkipSpaceAndComments(is);
		is >> VarDouble;
		ipl::SkipSpaceAndComments(is);
		// skip 'VarToAlgorithm2:' token
		is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':'));
		ipl::SkipSpaceAndComments(is);
		is >> VarInt;
		ipl::SkipSpaceAndComments(is);
		// skip 'FileName:' token
		is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':'));
		ipl::SkipSpaceAndComments(is);
		is.getline(pStr,250); // reads rest of line since item may consist of spaces
		ImageFile.assign(pStr);
		delete [] pStr;
	}
}
