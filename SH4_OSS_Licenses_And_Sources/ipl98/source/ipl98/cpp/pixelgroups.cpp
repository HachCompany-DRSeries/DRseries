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

#include "pixelgroups.h"

namespace ipl{

CPixelGroups::CPixelGroups()
{
	// default constructor
	k_InitGroups(&m_Groups);
}

CPixelGroups::CPixelGroups(unsigned int InitialSize)
{
	k_InitGroups(&m_Groups);
	k_AllocGroups(&m_Groups,InitialSize);
}

CPixelGroups::CPixelGroups(const CPixelGroups& SourceGroups)
{ // copy constructor
	k_InitGroups(&m_Groups);
	*this=SourceGroups; // use assignment operator
}

CPixelGroups& CPixelGroups::operator=(const CPixelGroups& SourceGroups)
{
	// overloading of assignment operator
	if (this != &SourceGroups)
	{
		// ok to do assignment
		Empty();
		k_AllocGroups(&m_Groups,SourceGroups.GetTotalGroups());
		CPixelGroup* pGroup;
		for(unsigned int count=0; count<SourceGroups.GetTotalGroups(); count++)
		{
			if ((pGroup=SourceGroups.GetGroup(count))!=NULL)
				AddGroup(*pGroup);
			else
			{
				ostringstream ost;
				ost << "CPixelGroups::operator=() Index(" << count << ") out of range (internal error)" << IPLAddFileAndLine;
				CError::ShowMessage(IPL_ERROR,ost);
			}
		}
	}
	else
	{
		ostringstream ost;
		ost << "CPixelGroups::operator=() Cannot assign CPixelGroups to itself" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return *this;
}

CPixelGroups::~CPixelGroups()
{
	//destructor
	for(unsigned int it=0;it<m_pCGroups.size();it++)
		delete m_pCGroups[it];

	k_EmptyGroups(&m_Groups);
}

void CPixelGroups::Empty()
{
	for(unsigned int it=0;it<m_pCGroups.size();it++)
		delete m_pCGroups[it];

	m_pCGroups.clear();
	//m_pCGroups.size();

	k_EmptyGroups(&m_Groups);
}

unsigned int CPixelGroups::GetTotalGroups() const
{
	return m_Groups.NumberOfGroups;
}

CPixelGroup* CPixelGroups::GetGroup(unsigned int Index) const
{
	if (Index<m_Groups.NumberOfGroups)
	{
		return m_pCGroups[Index];
	}
	else
	{
		ostringstream ost;
		ost << "CPixelGroups::GetGroup() Index(" << Index << ") out of range" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return NULL;
	}
}

const TPixelGroups* CPixelGroups::GetConstTPixelGroupsPtr() const
{
	return &m_Groups;
}


TPixelGroups* CPixelGroups::GetTPixelGroupsPtr()
{
	return &m_Groups;
}

void CPixelGroups::AddGroup(const CPixelGroup& PixelGroup)
{
	TPixelGroup* pOldAddres=m_Groups.pGroups;
	k_AddGroupToGroups(PixelGroup.GetConstTPixelGroupPtr(),&m_Groups);
	// must also update the m_CGroups member
	// first check if the call to k_AddGroupToGroups() changed the addres of m_Groups.pGroups
	if ((pOldAddres!=m_Groups.pGroups) && (pOldAddres!=NULL))
	{
		for(unsigned int count=0; count < m_Groups.NumberOfGroups-1; count++)
		{
			(m_pCGroups[count])->MoveTPixelGroupPtr(m_Groups.pGroups+count);
		}

	}
	CPixelGroup* pPG=new CPixelGroup(m_Groups.pGroups[m_Groups.NumberOfGroups-1]);
	m_pCGroups.push_back(pPG);
	//CPixelGroup* test=m_pCGroups[65];
}

bool CPixelGroups::InsertGroupInGroups(const CPixelGroup& PixelGroup, unsigned int Index)
{
	bool returnValue=k_InsertGroupInGroups(PixelGroup.GetConstTPixelGroupPtr(),Index,&m_Groups);

	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroups::InsertGroupInGroups() Failed inserting group" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	///////////// NEED TO UPDATE THE m_CGroups array
	UpdateAll();
	return returnValue;
}

void CPixelGroups::UpdateAll()
{
	for(unsigned int it=0;it<m_pCGroups.size();it++)
		delete m_pCGroups[it];
	m_pCGroups.clear();
	for(unsigned int count=0; count<m_Groups.NumberOfGroups; count++)
	{
		CPixelGroup* pPG=new CPixelGroup(m_Groups.pGroups[count]);
		m_pCGroups.push_back(pPG);
	}
}

void CPixelGroups::Update()
{
	if (m_pCGroups.size()<m_Groups.NumberOfGroups)
	{
		// must also update the m_CGroups member
		for(unsigned int count=m_pCGroups.size(); count < m_Groups.NumberOfGroups; count++)
		{
			CPixelGroup* pPG=new CPixelGroup(m_Groups.pGroups[count]);
			m_pCGroups.push_back(pPG);
		}
	}
}

bool CPixelGroups::AddColors(const CStdImage& Source)
{
	bool returnValue=k_AddColorsToGroups(Source.GetConstTImagePtr(),&m_Groups);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroups::AddColors() Failed adding colors" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return returnValue;
}

bool CPixelGroups::RemoveGroup(unsigned int GroupIndex)
{
	bool returnValue=k_RemoveGroup(&m_Groups,GroupIndex);;
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroups::RemoveGroup() Failed removing group number " << GroupIndex << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
	{
		//delete m_pCGroups[GroupIndex];
		//m_pCGroups.erase(m_pCGroups.begin()+GroupIndex);
		UpdateAll();
	}
	return returnValue;
}

bool CPixelGroups::RemoveGroupSlow(unsigned int GroupIndex)
{
	bool returnValue=k_RemoveGroupSlow(&m_Groups,GroupIndex);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroups::RemoveGroupSlow() Failed removing group" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	///////////// NEED TO UPDATE THE m_CGroups array
	UpdateAll();
	return returnValue;
}

bool CPixelGroups::GetGroupWithPos(unsigned int& GroupIndex,const CPoint2D<INT16>& Pos,
								   unsigned int& PosIndex)
{
	T2DInt p;
	p.x=Pos.GetX();
	p.y=Pos.GetY();
	return k_GetGroupWithPos(&m_Groups,&GroupIndex,p,&PosIndex);
}

bool CPixelGroups::GetGroupWithPos(unsigned int& GroupIndex,const CPoint2D<INT16>& Pos)
{
	T2DInt p;
	p.x=Pos.GetX();
	p.y=Pos.GetY();
	return k_GetGroupWithPos(&m_Groups,&GroupIndex,p,NULL);
}

void CPixelGroups::PrintInfo()
{
	k_PrintGroupsInfo(&m_Groups);
}

ostream& operator<<(ostream& s,const CPixelGroups& Groups)
{
	s << StreamIndent << "**************** PixelGroups info ********************\n" <<
		StreamIndent << " Number of groups inserted: " << Groups.GetTotalGroups() <<
		"  TPixelGroup structures allocated: " << 
		Groups.GetConstTPixelGroupsPtr()->AllocatedGroups << endl;
	return s;
}

} // end ipl namespace
