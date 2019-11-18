#ifndef _VECTORRICHEDIT_H
#define _VECTORRICHEDIT_H

#include <vector>
#include "StdString.h"

using namespace std;

/** A simple container class for insertion of text lines, it appends \r\n
	control codes at the end of the strings and replaces \n characters with
	\r\n, last updated the 17/7/2000. This has to be done to make the output
	look nice in a CEditView class. For more information see the IPL98
	Exchange page, follow the link "MS Visual C++ GUI and IPL98" and
	"Complete GUI application"
	@version 0.2,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
class CVectorRichEdit : public vector<CStdString>
{
	public:
		/** Add's the string str to the vector but first all appearances
			of \n is replaced with \r\n to make it look nice in CEditView.
			Note: CStdString is a class is written by Joe O'Leary, it is an
			intersection of the STL string class and MFC CString class and is
			very well written, I can highly recommend using this class with
			Visual C++, read more on Joe O'Leary homepage at
			http://home.earthlink.net/~jmoleary/projects.htm.*/
		void push_back(CStdString str)
		{
			str.Replace("\n","\r\n");
			str.append("\r\n");
			vector<CStdString>::push_back(str);
		}
		/** Add's the char pointer str to the vector but first all appearances
			of \n is replaced with \r\n to make it look nice in CEditView */
		void push_back(char* str)
		{
			CStdString StdStr(str);
			push_back(StdStr);
		}
		void push_back0(CStdString str)
		{
			str.Replace("\n","\r\n");
			vector<CStdString>::push_back(str);
		}
		void push_back0(char* str)
		{
			CStdString StdStr(str);
			push_back0(StdStr);
		}

		void Str(int i,int Width,char * s)
		{
			   char buf[100];
			   char *b = buf;
			   char s1[6],s2[6];
			   strcpy(s1,"%");
			   itoa(Width,s2,10);
			   strcat(s1,s2);
			   strcat(s1,"d");
			   sprintf(b,s1,i);
			   strcpy(s,b);
			   printf("");
			   
		}
		void Str(double x,int Width, char *s)
			{
			   char buf[100];
			   char *b = buf;
			   int d;
			   d=Width-7;
			   if (d < 0){d=0;};
			   char s1[9],s2[9];
			   strcpy(s1,"%");
			   itoa(Width,s2,10);
			   strcat(s1,s2);
			   strcpy(s2,".");
			   strcat(s1,s2);
			   itoa(d,s2,10);
			   strcat(s1,s2);
			   strcat(s1,"e");
			   sprintf(b,s1,x);
			   strcpy(s,b);
			   printf("");
			   
			};
		void Str(double x,int Width, int Decimals,char *s)
			{
			   char buf[100];
			   char *b = buf;
			   char s1[9],s2[9];
			   strcpy(s1,"%");
			   itoa(Width,s2,10);
			   strcat(s1,s2);
			   strcpy(s2,".");
			   strcat(s1,s2);
			   itoa(Decimals,s2,10);
			   strcat(s1,s2);
			   strcat(s1,"f");
			   sprintf(b,s1,x);
			   strcpy(s,b);
			   printf("");
			   
			};
};

#endif //_VECTORRICHEDIT_H