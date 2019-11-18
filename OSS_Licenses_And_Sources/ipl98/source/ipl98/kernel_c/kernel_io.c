/********************************************************************
   The Image Processing Library 98 - IPL98
   Copyright (C) by René Dencker Eriksen - edr@mip.sdu.dk

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation. Only
   addition is, that if the library is used in proprietary software
   it must be stated that IPL98 was used.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

   More details can be found in the file licence.txt distributed
   with this library.
*********************************************************************/

#include "kernel_io.h"
#include "kernel_string.h"
#include "kernel_big_endian.h"
#include "kernel_error.h"
#include <stdlib.h>
#include <string.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

void k_InitFileInfo(TFileInfo* pInfo)
{
	pInfo->FileName=NULL;
	pInfo->PathName=NULL;
	pInfo->PathAndFileName=NULL;
}

void k_EmptyFileInfo(TFileInfo* pInfo)
{
	if (pInfo->FileName!=NULL)
	{
		free(pInfo->FileName);
		pInfo->FileName=NULL;
	}
	if (pInfo->PathName!=NULL)
	{
		free(pInfo->PathName);
		pInfo->PathName=NULL;
	}
	if (pInfo->PathAndFileName!=NULL)
	{
		free(pInfo->PathAndFileName);
		pInfo->PathAndFileName=NULL;
	}
}

bool k_CopyFileInfo(TFileInfo* pDest,const TFileInfo* pSource)
{
	if (pDest==pSource)
	{
		/* cannot copy to itself! */
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_CopyFileInfo() Source and destination file info"
			" the same - doing nothing");
		return false;
	}
	k_EmptyFileInfo(pDest);
	if(pSource->PathAndFileName!=NULL)
	{
		pDest->PathAndFileName=(char *) malloc(strlen(pSource->PathAndFileName)+1);
		strcpy(pDest->PathAndFileName,pSource->PathAndFileName);
		pDest->PathName=(char *) malloc(strlen(pSource->PathName)+1);
		strcpy(pDest->PathName,pSource->PathName);
		pDest->FileName=(char *) malloc(strlen(pSource->FileName)+1);
		strcpy(pDest->FileName,pSource->FileName);
	}
	return true;
}

void k_PrintFileInfo(const TFileInfo* pFileInfo)
{
	if(pFileInfo->FileName!=NULL)
	{
		printf(" File name: %s ",pFileInfo->FileName);
	}
	else
	{
		printf(" File name: Not available ");
	}
	if(pFileInfo->PathName!=NULL)
	{
		printf(" Path name: %s\n",pFileInfo->PathName);
	}
	else
	{
		printf(" Path name: Not available\n");
	}
}

UINT8 k_ReadUINT8(FILE* fp)
{
	UINT8 c;
	/*printf("sizeof(UINT8)=%d\n",sizeof(UINT8));*/
	fread(&c,sizeof(UINT8),1,fp);
	return c;
}

UINT16 k_ReadUINT16(FILE* fp)
{
	UINT16 c;
	/*printf("sizeof(UINT16)=%d\n",sizeof(UINT16));*/
	fread(&c,sizeof(UINT16),1,fp);

#ifdef IPL_BIG_ENDIAN
	return  k_SWAP2BYTES(c);
#else
	return c;
#endif
}

UINT32 k_ReadUINT32(FILE* fp)
{
	UINT32 c;
	/*printf("sizeof(UINT32)=%d\n",sizeof(UINT32));*/
	fread(&c,sizeof(UINT32),1,fp);
#ifdef IPL_BIG_ENDIAN
	return k_SWAP4BYTES(c);
#else
	return c;
#endif /* IPL_BIG_ENDIAN */
}

void k_WriteUINT8(FILE* fp, UINT8 c)
{
	fwrite(&c,sizeof(UINT8),1,fp);
}

void k_WriteUINT16(FILE* fp, UINT16 c)
{
#ifdef IPL_BIG_ENDIAN
	c=k_SWAP2BYTES(c);
#endif /* IPL_BIG_ENDIAN */
	fwrite(&c,sizeof(UINT16),1,fp);
}

void k_WriteUINT32(FILE* fp, UINT32 c)
{
#ifdef IPL_BIG_ENDIAN
	c=k_SWAP4BYTES(c);
#endif /* IPL_BIG_ENDIAN */
	fwrite(&c,sizeof(UINT32),1,fp);
}

int k_SkipWhiteSpace(FILE *fp)
{
	/* skips all whitespaces including '\n', '\t', ' ' and '\r'
	   returns number of whitespace characters read */
	int ch;
	int count = -1;

	do{
		ch=fgetc(fp);
		count++;
	} while (ch=='\n' || ch == '\t' || ch == ' ' || ch == '\r');
	if (ch==EOF)
		return -1;
	else
	{
		ungetc(ch,fp);
		return count;
	}
}

int k_SkipLine(FILE *fp)
{
	int ch;
	int count = 0;

	do{
		ch=fgetc(fp);
		count++;
	} while ((ch!='\n') && (ch!=EOF));
	/*ungetc(ch,fp);*/
	if (ch==EOF)
		return -1;
	else
		return count;
}

bool k_ReadToken(FILE *fp,char** ppToken)
{
	if (k_SkipWhiteSpace(fp)!=-1) /* set fp to the first non whitespace character */
	{
		int count=-1;
		/*char* tempStr;*/
		int ch;

		do 
		{
			ch=fgetc(fp);
			count++;
		} while ((ch!='\n') && (ch != '\t') && (ch != ' ') && (ch != '\r') && (ch != EOF));
		*ppToken=(char*)malloc(count+1);
		if (fseek(fp,-(count+1),SEEK_CUR)!=0)
		{
			k_ShowMessageSimple(
				IPL_ERROR,
				"k_ReadToken() Internal error (fseek) when reading a token from file");
			return false;
		}
		if (fread(*ppToken,sizeof(char),count,fp)!=(unsigned int)count)
		{
			k_ShowMessageSimple(
				IPL_ERROR,
				"k_ReadToken() Internal error (fread) when reading a token from file");
			return false;
		}

		(*ppToken)[count]='\0';
		/*return tempStr;*/
		return true;
	}
	else
	{
		return false;
	}
}

bool k_SetFileAndPath(const char* FileAndPathName,TFileInfo* pInfo)
{
	char* Ext=NULL; /* dummy - not used */
	if (k_SplitFileName(FileAndPathName,&pInfo->PathName,&pInfo->FileName,&Ext)==false)
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_SetFileAndPath() Failed setting variables in TFileInfo structure");
		if (Ext!=NULL)
		{
			free(Ext);
		}
		return false;
	}
	pInfo->PathAndFileName=(char *) malloc(strlen(FileAndPathName)+1);
	strcpy(pInfo->PathAndFileName,FileAndPathName);
	if (Ext!=NULL)
	{
		free(Ext);
	}
	return true;
}

bool k_SplitFileName(const char* FilePathName, char** Path, char** FileNameExt, char** Ext)
{
	bool returnValue;
	int count=0;
	char* UFilePathName,*UFileNameCopy,*dummy;
	/*printf("In k_SplitFileName: Input file name=%s\n",FilePathName);*/
	if ((FilePathName==NULL) || (strcmp(FilePathName,"")==0))
	{
		k_ShowMessageSimple(
			IPL_WARNING,
			"k_SplitFileName() The given file name is empty - doing nothing");
		return false;
	}
	dummy=(char *) malloc(strlen(FilePathName)+1); /* strlen() excludes the \0 character! */;
	/* Make copy of FileName in UFilePathName */
	UFilePathName=(char *) malloc(strlen(FilePathName)+1); /* strlen() excludes the \0 character! */
	strcpy(UFilePathName,FilePathName);
	/* Convert all '\' to '/' */
	k_ConvertCharacters(UFilePathName,'\\','/');
	/* Make copy of FileName */
	UFileNameCopy=(char *) malloc(strlen(UFilePathName)+1); /* strlen() excludes the \0 character! */
	strcpy(UFileNameCopy,UFilePathName);
	if (UFilePathName[strlen(UFilePathName)-1]=='/')
	{
		k_ShowMessageSimple(IPL_ERROR, "k_SplitFileName() The given file name ends with a '/' character");
		returnValue=false;
	}
	else
	{
		/* must replace spaces with character - sscanf cannot handle space! */
		k_ConvertCharacters(UFileNameCopy,' ','#');
		/* skip all '/' in UFileNameCopy */
		/* remove a leading / if there is one */
		/* Changed to remove all leading '/' so that it will work with */
		/* UNC file names like '\\server\share\filename.ext */
		/* Changed 11.09.03 by Thomas Forsmark Soerensen*/
		while (UFileNameCopy[0]=='/')
		{
			memmove(UFileNameCopy,UFileNameCopy+1,strlen(UFileNameCopy));
		}
		do{
			sscanf(UFileNameCopy,"%[^/]/%s",dummy,UFileNameCopy);
		} while (strcmp(dummy,UFileNameCopy));
		/*printf("dummy=%s UFileNameCopy=%s\n",dummy,UFileNameCopy);*/
		/* set path name */
		*Path = (char *) malloc(strlen(UFilePathName)-strlen(UFileNameCopy)+1);
		memcpy(*Path,UFilePathName,strlen(UFilePathName)-strlen(UFileNameCopy));
		(*Path)[strlen(UFilePathName)-strlen(UFileNameCopy)]='\0';
		/* set file name with extension */
		*FileNameExt = (char *) malloc(strlen(UFileNameCopy)+1);
		strcpy(*FileNameExt,UFileNameCopy);
		/* Check if filename includes extension */
		count = strlen(UFileNameCopy);
		dummy[count]='\0';
		count--;
		while((UFileNameCopy[count] != '.') && (count>=0))
		{
			dummy[count]=UFileNameCopy[count];
			count--;
		}
		if (count<0)
		{	/* no extension found */
			/* Copy an empty string to extension variable */
			*Ext = (char *) malloc(sizeof(char));
			strcpy(*Ext,"");
		}
		else
		{	/* found extension */
			*Ext = (char *) malloc(sizeof(char)*(strlen(UFileNameCopy)-count));
			strncpy(*Ext,&(dummy[count+1]),strlen(UFileNameCopy)-count);
		}
		returnValue=true;
	}

	if (*Path!=NULL)
		k_ConvertCharacters(*Path,'#',' ');
	if (*FileNameExt!=NULL)
		k_ConvertCharacters(*FileNameExt,'#',' ');
	if (*Ext!=NULL)
		k_ConvertCharacters(*Ext,'#',' ');
	free(UFileNameCopy);
	free(dummy);
	free(UFilePathName);
	return returnValue;
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

