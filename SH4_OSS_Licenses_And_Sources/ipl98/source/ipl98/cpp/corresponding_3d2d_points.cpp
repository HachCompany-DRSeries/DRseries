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

#include "corresponding_3d2d_points.h"
#include "error.h"
#include <sstream>

namespace ipl{ // use namespace ipl

using std::ostringstream;

CCorresponding3D2DPoints::CCorresponding3D2DPoints()
{
	k_Init3D2DPoints(&m_PointSets);
}

CCorresponding3D2DPoints::CCorresponding3D2DPoints(unsigned int InitialSize)
{
	k_Init3D2DPoints(&m_PointSets);
	k_Alloc3D2DPoints(&m_PointSets,InitialSize);
}

CCorresponding3D2DPoints::~CCorresponding3D2DPoints()
{
	k_Empty3D2DPoints(&m_PointSets);
}

CCorresponding3D2DPoints::CCorresponding3D2DPoints(const CCorresponding3D2DPoints& PointSet)
{
	// copy constructor
	k_Init3D2DPoints(&m_PointSets);
	*this=PointSet;
}

CCorresponding3D2DPoints& CCorresponding3D2DPoints::operator=(const CCorresponding3D2DPoints& SourcePointSet)
{
	// overloading of assignment operator
	if (this != &SourcePointSet)
	{
		// ok to do assignment
		k_Copy3D2DPoints(&m_PointSets,SourcePointSet.GetConstT3D2DPointsPtr());
	}
	else
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::operator=() Cannot assign CCorresponding3D2DPoints to itself" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return *this;

}

void CCorresponding3D2DPoints::Empty()
{
	k_Empty3D2DPoints(&m_PointSets);
}

void CCorresponding3D2DPoints::ReAlloc3D2DPoints(unsigned int NewSize)
{
	k_ReAlloc3D2DPoints(&m_PointSets,NewSize);
}

void CCorresponding3D2DPoints::AdjustSizeOf3D2DPoints()
{
	k_AdjustSizeOf3D2DPoints(&m_PointSets);
}

bool CCorresponding3D2DPoints::SetAll3DPointsToNotUsed()
{
	return k_SetAll3DPointsToNotUsed(&m_PointSets);
}

bool CCorresponding3D2DPoints::SetAll2DPointsToNotUsed()
{
	return k_SetAll2DPointsToNotUsed(&m_PointSets);
}

bool CCorresponding3D2DPoints::SetAllIdsToNotUsed()
{
	return k_SetAllIdsToNotUsed(&m_PointSets);
}

unsigned int CCorresponding3D2DPoints::GetTotalPointSetsInUse() const
{
	return k_GetTotal3D2DPointSetsInUse(&m_PointSets);
}

unsigned int CCorresponding3D2DPoints::GetTotal2DPointsOnlyInUse() const
{
	return k_GetTotal2DPointsOnlyInUse(&m_PointSets);
}

unsigned int CCorresponding3D2DPoints::GetTotal3DPointsOnlyInUse() const
{
	return k_GetTotal3DPointsOnlyInUse(&m_PointSets);
}

unsigned int CCorresponding3D2DPoints::GetTotalPointSets() const
{
	return m_PointSets.NumberOfSets;
}

bool CCorresponding3D2DPoints::Point2DInUse(unsigned int Index) const
{
	if (Index>m_PointSets.NumberOfSets)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::Point2DInUse() Index (" << Index << ") out of range" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		return m_PointSets.pSet[Index].Pnt2DInUse;
	}
}

bool CCorresponding3D2DPoints::Point3DInUse(unsigned int Index) const
{
	if (Index>m_PointSets.NumberOfSets)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::Point3DInUse() Index (" << Index << ") out of range" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		return m_PointSets.pSet[Index].Pnt3DInUse;
	}
}

bool CCorresponding3D2DPoints::IdInUse(unsigned int Index) const
{
	if (Index>m_PointSets.NumberOfSets)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::IdInUse() Index (" << Index << ") out of range" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		return m_PointSets.pSet[Index].IdInUse;
	}
}

CPoint3D<float> CCorresponding3D2DPoints::GetPoint3D(unsigned int Index) const
{
	if (Index>m_PointSets.NumberOfSets)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::GetPoint3D() Index (" << Index << ") out of range" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return CPoint3D<float>(-1,-1,-1);
	}
	else
	{
		return CPoint3D<float>(m_PointSets.pSet[Index].Pnt3D.x,
							   m_PointSets.pSet[Index].Pnt3D.y,
							   m_PointSets.pSet[Index].Pnt3D.z);
	}
}

CPoint2D<float> CCorresponding3D2DPoints::GetPoint2D(unsigned int Index) const
{
	if (Index>m_PointSets.NumberOfSets)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::GetPoint2D() Index (" << Index << ") out of range" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return CPoint2D<float>(-1,-1);
	}
	else
	{
		return CPoint2D<float>(m_PointSets.pSet[Index].Pnt2D.x,
							   m_PointSets.pSet[Index].Pnt2D.y);
	}
}

int CCorresponding3D2DPoints::GetId(unsigned int Index) const
{
	if (Index>m_PointSets.NumberOfSets)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::GetId() Index (" << Index << ") out of range" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
	else
	{
		return m_PointSets.pSet[Index].Id;
	}
}

const T3D2DPoints* CCorresponding3D2DPoints::GetConstT3D2DPointsPtr() const
{
	return &m_PointSets;
}

T3D2DPoints* CCorresponding3D2DPoints::GetT3D2DPointsPtr()
{
	return &m_PointSets;
}

bool CCorresponding3D2DPoints::AddPointSetAndId(const CPoint3D<float>& Pnt3D, const CPoint2D<float>& Pnt2D, int Id)
{
	T2DFloat P2d;
	P2d.x=Pnt2D.GetX();
	P2d.y=Pnt2D.GetY();
	T3DFloat P3d;
	P3d.x=Pnt3D.GetX();
	P3d.y=Pnt3D.GetY();
	P3d.z=Pnt3D.GetZ();
	if (k_AddPointSetAndIdTo3D2DPoints(&P3d,&P2d,Id,&m_PointSets)!=true)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::AddPointSetAndId() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		return true;
	}
}

bool CCorresponding3D2DPoints::AddPoint3DAndId(const CPoint3D<float>& Pnt3D, int Id)
{
	T3DFloat P3d;
	P3d.x=Pnt3D.GetX();
	P3d.y=Pnt3D.GetY();
	P3d.z=Pnt3D.GetZ();
	if (k_AddPoint3DAndIdTo3D2DPoints(&P3d,Id,&m_PointSets)!=true)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::AddPoint3DAndId() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		return true;
	}
}

bool CCorresponding3D2DPoints::AddPoint2DAndId(const CPoint2D<float>& Pnt2D, int Id)
{
	T2DFloat P2d;
	P2d.x=Pnt2D.GetX();
	P2d.y=Pnt2D.GetY();
	if (k_AddPoint2DAndIdTo3D2DPoints(&P2d,Id,&m_PointSets)!=true)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::AddPoint2DAndId() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		return true;
	}
}

bool CCorresponding3D2DPoints::AddPointSet(const CPoint3D<float>& Pnt3D, const CPoint2D<float>& Pnt2D)
{
	T2DFloat P2d;
	P2d.x=Pnt2D.GetX();
	P2d.y=Pnt2D.GetY();
	T3DFloat P3d;
	P3d.x=Pnt3D.GetX();
	P3d.y=Pnt3D.GetY();
	P3d.z=Pnt3D.GetZ();
	if (k_AddPointSetTo3D2DPoints(&P3d,&P2d,&m_PointSets)!=true)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::AddPointSet() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		return true;
	}
}

bool CCorresponding3D2DPoints::RemovePointSet(int Id)
{
	if (k_RemovePointSetFrom3D2DPoints(Id,&m_PointSets)!=true)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::RemovePointSet() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		return true;
	}
}

bool CCorresponding3D2DPoints::RemovePointSetIndex(unsigned int Index)
{
	if (k_RemovePointSetIndexFrom3D2DPoints(Index,&m_PointSets)!=true)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::RemovePointSetIndex() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		return true;
	}
}

unsigned int CCorresponding3D2DPoints::RemoveAllSets(bool WithUnUsed3D, 
													 bool WithUnused2D, 
													 bool WithUnUsedId)
{
	return k_RemoveAllSetsFrom3D2DPoints(WithUnUsed3D,WithUnused2D, 
											WithUnUsedId,&m_PointSets);
}

bool CCorresponding3D2DPoints::AddPoint3D(const CPoint3D<float>& Pnt3D)
{
	T3DFloat P3d;
	P3d.x=Pnt3D.GetX();
	P3d.y=Pnt3D.GetY();
	P3d.z=Pnt3D.GetZ();
	if (k_AddPoint3DTo3D2DPoints(&P3d,&m_PointSets)!=true)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::AddPoint3D() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		return true;
	}
}

bool CCorresponding3D2DPoints::AddPoint2D(const CPoint2D<float>& Pnt2D)
{
	T2DFloat P2d;
	P2d.x=Pnt2D.GetX();
	P2d.y=Pnt2D.GetY();
	if (k_AddPoint2DTo3D2DPoints(&P2d,&m_PointSets)!=true)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::AddPoint2D() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		return true;
	}
}

bool CCorresponding3D2DPoints::SetPoint2D(const CPoint2D<float>& Pnt2D, unsigned int Index)
{
	T2DFloat P2d;
	P2d.x=Pnt2D.GetX();
	P2d.y=Pnt2D.GetY();
	if (k_SetPoint2DIn3D2DPoints(&P2d, Index, &m_PointSets)!=true)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::SetPoint2D() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		return true;
	}
}

bool CCorresponding3D2DPoints::SetPoint3D(const CPoint3D<float>& Pnt3D, unsigned int Index)
{
	T3DFloat P3d;
	P3d.x=Pnt3D.GetX();
	P3d.y=Pnt3D.GetY();
	P3d.z=Pnt3D.GetZ();
	if (k_SetPoint3DIn3D2DPoints(&P3d, Index, &m_PointSets)!=true)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::SetPoint3D() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		return true;
	}
}

bool CCorresponding3D2DPoints::GetIndex(unsigned int& Index, int Id) const
{
	if (k_GetIndexIn3D2DPoints(&Index,Id,&m_PointSets)!=true)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::GetIndex() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		return true;
	}
}

bool CCorresponding3D2DPoints::IdExists(int Id) const
{
	return k_IdExistsIn3D2DPoints(Id,&m_PointSets);
}

bool CCorresponding3D2DPoints::Load(const char* PathAndFileName)
{
	if (k_Load3D2DPoints(PathAndFileName, &m_PointSets)!=true)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::Load() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		return true;
	}
}

bool CCorresponding3D2DPoints::Save(const char* PathAndFileName) const
{
	if (k_Save3D2DPoints(PathAndFileName, &m_PointSets)!=true)
	{
		ostringstream ost;
		ost << "CCorresponding3D2DPoints::Save() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
	{
		return true;
	}
}

ostream& operator<<(ostream& s,const CCorresponding3D2DPoints& PointSets)
{
	s << StreamIndent << "**************** CCorresponding3D2DPoints info ********************\n" <<
		StreamIndent << " Number of point sets inserted: " << PointSets.GetTotalPointSets() <<
		" Allocated point sets: " << PointSets.GetConstT3D2DPointsPtr()->AllocatedSets << endl;
	unsigned int count;
	for(count=0; count<PointSets.GetTotalPointSets(); count++)
	{
		if (PointSets.Point3DInUse(count))
		{
			s << StreamIndent << " 3D pnt = " << PointSets.GetPoint3D(count);
		}
		else
		{
			s << StreamIndent << " 3D pnt = not in use";
		}
		if (PointSets.Point2DInUse(count))
		{
			s << StreamIndent << " 2D pnt = " << PointSets.GetPoint2D(count);
		}
		else
		{
			s << StreamIndent << " 2D pnt = not in use";
		}
		if (PointSets.IdInUse(count))
		{
			s << StreamIndent << " Id = " << PointSets.GetId(count);
		}
		else
		{
			s << StreamIndent << " Id = not in use";
		}
		s << endl;
	}

	return s;
}

} // end namespace ipl
