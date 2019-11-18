// The correct IPL98 header files must be included before including this file
#if defined (USE_IPL98_LIB) || defined (COMPILING_IPL98_LIB)


/** This file is used to create the IPL98 lib file, where the template
	classes are explicitely instantiated. But it is also used as include
	file for Visual C++ projects in which case the prefix "IPL98_LIB_PREFIX " is put
	in front of all lines. It changes the meaning, it prevents the compiler
	to instantiate template of the types specified below which reduces
	compilation time. For the IPL98 lib project COMPILING_IPL98_LIB must be
	defined. */
#ifdef COMPILING_IPL98_LIB
#define IPL98_LIB_PREFIX  
#else
#define IPL98_LIB_PREFIX extern
#endif

#pragma warning(disable: 4660)
#pragma warning(disable: 4800)

// The macro IPL98_LIB_PREFIX is empty when creating the IPL98 library. But
// is set to 'exter' when other projects use the library in order to prevent
// those project from expanding template types already in the lib file.

// CPoint2D explicit instantiation
IPL98_LIB_PREFIX template class CPoint2D<ipl::UINT16>;
IPL98_LIB_PREFIX template class CPoint2D<ipl::INT16>;
IPL98_LIB_PREFIX template class CPoint2D<ipl::UINT32>;
IPL98_LIB_PREFIX template class CPoint2D<ipl::INT32>;
IPL98_LIB_PREFIX template class CPoint2D<ipl::FLOAT32>;
IPL98_LIB_PREFIX template class CPoint2D<double>;

// CPoint3D explicit instantiation
IPL98_LIB_PREFIX template class CPoint3D<ipl::UINT16>;
IPL98_LIB_PREFIX template class CPoint3D<ipl::INT16>;
IPL98_LIB_PREFIX template class CPoint3D<ipl::UINT32>;
IPL98_LIB_PREFIX template class CPoint3D<ipl::INT32>;
IPL98_LIB_PREFIX template class CPoint3D<ipl::FLOAT32>;
IPL98_LIB_PREFIX template class CPoint3D<double>;
IPL98_LIB_PREFIX template class CPoint3D<int>;

// CVector2D explicit instantiation
IPL98_LIB_PREFIX template class ipl::CVector2D<ipl::UINT16>;
IPL98_LIB_PREFIX template class ipl::CVector2D<ipl::INT16>;
IPL98_LIB_PREFIX template class ipl::CVector2D<ipl::UINT32>;
IPL98_LIB_PREFIX template class ipl::CVector2D<ipl::INT32>;
IPL98_LIB_PREFIX template class ipl::CVector2D<ipl::FLOAT32>;
IPL98_LIB_PREFIX template class ipl::CVector2D<double>;
IPL98_LIB_PREFIX template class ipl::CVector2D<int>;

// CVector3D explicit instantiation
IPL98_LIB_PREFIX template class ipl::CVector3D<ipl::UINT16>;
IPL98_LIB_PREFIX template class ipl::CVector3D<ipl::INT16>;
IPL98_LIB_PREFIX template class ipl::CVector3D<ipl::UINT32>;
IPL98_LIB_PREFIX template class ipl::CVector3D<ipl::INT32>;
IPL98_LIB_PREFIX template class ipl::CVector3D<ipl::FLOAT32>;
IPL98_LIB_PREFIX template class ipl::CVector3D<double>;
IPL98_LIB_PREFIX template class ipl::CVector3D<int>;

// CArray2D explicit instantiation
IPL98_LIB_PREFIX template class ipl::CArray2D<double>;
IPL98_LIB_PREFIX template class ipl::CArray2D<bool>;
IPL98_LIB_PREFIX template class ipl::CArray2DIterator<double>;
IPL98_LIB_PREFIX template class ipl::CArray2DIterator<bool>;
IPL98_LIB_PREFIX template class ipl::CArray2DConstIterator<double>;
IPL98_LIB_PREFIX template class ipl::CArray2DConstIterator<bool>;

// CArrayOS2D explicit instantiation
IPL98_LIB_PREFIX template class ipl::CArrayOS2D<double>;
IPL98_LIB_PREFIX template class ipl::CArrayOS2D<bool>;
IPL98_LIB_PREFIX template class ipl::CArrayOS2DIterator<double>;
IPL98_LIB_PREFIX template class ipl::CArrayOS2DIterator<bool>;
IPL98_LIB_PREFIX template class ipl::CArrayOS2DConstIterator<double>;
IPL98_LIB_PREFIX template class ipl::CArrayOS2DConstIterator<bool>;

// CLine2D explicit instantiation
IPL98_LIB_PREFIX template class ipl::CLine2D<double>;
IPL98_LIB_PREFIX template class ipl::CLine2D<float>;

// CLineSegment2D explicit instantiation
IPL98_LIB_PREFIX template class ipl::CLineSegment2D<double>;
IPL98_LIB_PREFIX template class ipl::CLineSegment2D<float>;

// CLine3D explicit instantiation
IPL98_LIB_PREFIX template class ipl::CLine3D<double>;
IPL98_LIB_PREFIX template class ipl::CLine3D<float>;

// CPlane explicit instantiation
IPL98_LIB_PREFIX template class ipl::CPlane<double>;
IPL98_LIB_PREFIX template class ipl::CPlane<float>;

// CPositionGroup2D explicit instantiation
IPL98_LIB_PREFIX template class ipl::CPositionGroup2D< CPoint2D<ipl::FLOAT32> >;
IPL98_LIB_PREFIX template class ipl::CPositionGroup2D< CPoint2D<double> >;

#endif // USE_IPL98_LIB
