#include "graph_plot2d.h"

namespace ipl{ // use namespace if C++

CGraphPlot2D::CGraphPlot2D(CStdImage& Img) : m_Img(Img)
{
	m_Initiated=false;
}

void CGraphPlot2D::Line(int x1, int x2, int y1,int y2,ipl::UINT32 color)
{
	m_Pg.Empty();
	CPoint2D<ipl::INT16> P1,P2;
	P1.Set(x1,y1);
	P2.Set(x2,y2);
	m_G.Line(P1,P2,m_Pg);
	for (unsigned int i=0;i<m_Pg.GetTotalPositions();i++)
	{   
		if (InsideFrame(m_Pg.GetPosition(i)))
			m_Img.SetPixel(m_Pg.GetPosition(i),color);
	}
}

int CGraphPlot2D::convx(float x) const
{
	float z,xa;
	int i;
	if (m_LogX)
	{
		xa=fabs(x);
		if (xa<1e-30)
			z=-m_cx1*20+m_cy0; 
		else 
			z=m_cx1*log(xa)/m_L10+m_cx0;
	} 
	else 
		z=m_cx1*x+m_cx0;
	if (fabs(z)<32000)
		i=(int)floor(z);
	else 
		i=(int)(z/fabs(z)*32000);
	return i;
}

int CGraphPlot2D::convy(float y) const
{
	float z,ya;
	int i;
	if (m_LogY)
	{
		ya=fabs(y);
		if (ya<1e-30)
			z=-m_cy1*20+m_cy0; 
		else 
			z=m_cy1*log(ya)/m_L10+m_cy0;
	} 
	else 
		z=m_cy1*y+m_cy0;
	if (fabs(z)<32000)
		i=(int)floor(z);
	else 
		i=-(int)(z/fabs(z)*32000);
	return i;
}

bool CGraphPlot2D::MoveTo(float x, float y)
{
	if (!m_Initiated) 
	{
		ostringstream ost;
		ost << "CGraphPlot2D::PlotCircle() Not initialized, call InitPlot()" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	m_PlotPos.Set(convx(x),convy(y));
	return true;
}

bool CGraphPlot2D::LineTo(float x, float y, ipl::UINT32 color)
{
	if (!m_Initiated) 
	{
		ostringstream ost;
		ost << "CGraphPlot2D::PlotCircle() Not initialized, call InitPlot()" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	CPoint2D<ipl::INT16> P;
	P.Set(convx(x),convy(y));
	Line(m_PlotPos.GetX(),convx(x),m_PlotPos.GetY(),convy(y),color);
	m_PlotPos=P;
	return true;
}

bool CGraphPlot2D::PlotPoint(float x,float y,ipl::UINT32 color)
{
	if (!m_Initiated) 
	{
		ostringstream ost;
		ost << "CGraphPlot2D::PlotCircle() Not initialized, call InitPlot()" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	CPoint2D<ipl::INT16> P(convx(x),convy(y));
	if (InsideFrame(P))
		m_Img.SetPixel(P,color);
	return true;
}

bool CGraphPlot2D::PlotCircle(float x,float y,int PixelRadius,ipl::UINT32 color)
{
	if (!m_Initiated) 
	{
		ostringstream ost;
		ost << "CGraphPlot2D::PlotCircle() Not initialized, call InitPlot()" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	m_Pg.Empty();
	CPoint2D<ipl::INT16> P(convx(x),convy(y));
	if (InsideFrame(P))
	{
		m_G.Circle(P,PixelRadius,m_Pg);
		m_Pg.AddToImage(m_Img,color);
	}
	return true;
}

bool CGraphPlot2D::InitPlot(float LeftX,float RightX,float BottomY,float TopY,
					   int PixWidth,int PixHeight, AXISPLOT AxisTypes)
{
	switch(AxisTypes)
	{
		case LINLIN:
		{
			InitPlot0(LeftX,RightX,BottomY,TopY,PixWidth,PixHeight,false,false);
			break;
		}
		case LINLOG:
		{
			InitPlot0(LeftX,RightX,BottomY,TopY,PixWidth,PixHeight,false,true);
			break;
		}
		case LOGLIN:
		{
			InitPlot0(LeftX,RightX,BottomY,TopY,PixWidth,PixHeight,true,false);
			break;
		}
		case LOGLOG:
		{
			InitPlot0(LeftX,RightX,BottomY,TopY,PixWidth,PixHeight,true,true);
			break;
		}
		default:
		{
			ostringstream ost;
			ost << "CGraphPlot2D::InitPlot() Unknown axis type" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			return false;
		}
	}
	return true;
}

bool CGraphPlot2D::InitPlot0(float LeftX,float RightX,
						float BottomY,float TopY,int PixWidth,int PixHeight, bool LogX,bool LogY)
{
	float x0,x1,y0,y1;
	if (LeftX==RightX) 
	{
		ostringstream ost;
		ost << "CGraphPlot2D::InitPlot0() Illegal parameters: LeftX=RightX" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	if (BottomY==TopY) 
	{
		ostringstream ost;
		ost << "CGraphPlot2D::InitPlot0() Illegal parameters: BottomY=TopY" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	if (LogX)
	{ 
		if (LeftX<=0||RightX<=0) 
		{
			ostringstream ost;
			ost << "CGraphPlot2D::InitPlot0() Illegal parameters: LeftX<=0 or RightX<=0" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			return false;
		}
	}
	if (LogY)
	{ 
		if (BottomY<=0||TopY<=0) 
		{
			ostringstream ost;
			ost << "CGraphPlot2D::InitPlot0() Illegal parameters: BottomY<=0 or TopY<=0" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			return false;
		}
	}
	if (PixHeight<10||PixHeight>2000||PixWidth<10||PixWidth>3000) 
	{
		ostringstream ost;
		ost << "CGraphPlot2D::InitPlot0() Illegal parameters: PixHeight<10 or PixHeight>2000 "
			   "or PixWidth<10 or PixWidth>3000" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
    std::ostringstream ost;
	m_L10=log((float)10);
	m_G.SetFontActive(CGraphics::Courier8x12);
	m_G.SetFontColor(0);
	m_G.SetFontBackgroundColor(CPalette::CreateRGB(255,255,255));
	m_Initiated=true;
	float m,d,d1,L,x,y,xlow,xhigh,ylow,yhigh;
	int P;
	m_ixw=PixWidth;
	m_iyw=PixHeight;
	m_ixb=100;
	m_iyb=10;
	//m_Img=CImage(m_ixw+120,m_iyw+30,24);
	if (m_Img.Alloc(m_ixw+120,m_iyw+30,24,CPalette::CreateRGB(255,255,255))==false)
	{
		if (m_Img.Alloc(m_ixw+120,m_iyw+30,8,255)==false)
		ostringstream ost;
		ost << "CGraphPlot2D::InitPlot0() Failed allocating image with both 24 b/p and 8 b/p" 
			<< IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	m_Img.Flush(CPalette::CreateRGB(255,255,255));
	Line(m_ixb,m_ixb,m_iyb,m_iyb+m_ixw,0);
	Line(m_ixb,m_ixb+m_ixw,m_iyb+m_iyw,m_iyb+m_iyw,0);
	Line(m_ixb+m_ixw,m_ixb+m_ixw,m_iyb+m_iyw,m_iyb,0);
	Line(m_ixb,m_ixb+m_ixw,m_iyb,m_iyb,0);
	m_LogX=LogX;
	m_LogY=LogY;
	x0=LeftX;
	y0=BottomY;
	if (RightX!=x0)
		x1=RightX;
	else 
		x1=(float)(x0+1e-5);
	if (TopY!=y0)
		y1=TopY;
	else y1=(float)(y0+1e-5);
	if (m_LogX)
	{
		if (x1<0)
			x1=-x1;
		if (x0<0)
			x0=-x0;
		if (x0>x1)
		{
			x=x1;
			x1=x0;
			x0=x;
		}
		if (x1/x0<15)
			x1=x0*15;
		m_cx1=m_ixw/(log(x1)-log(x0))*m_L10;
		m_cx0=m_ixb-log(x0)*m_cx1/m_L10;
	} 
	else
	{
		m_cx1=m_ixw/(x1-x0);
		m_cx0=m_ixb-x0*m_cx1;
	}
	if (LogY)
	{
		if (y1<0)
			y1=-y1;
		if (y0<0)
			y0=-y0;
		if (y0>y1)
		{
			y=y1;
			y1=y0;
			y0=y;
		}
		if (y1/y0<15)
			y1=y0*15;
		m_cy1=-m_iyw/(log(y1)-log(y0))*m_L10;
		m_cy0=m_iyb-log(y1)*m_cy1/m_L10;
	} 
	else
	{
		m_cy1=-m_iyw/(y1-y0);
		m_cy0=m_iyb-y1*m_cy1;
	}
	m_G.SetFontAlignment(CGraphics::TOPMID);
	if (RightX<LeftX)
	{
		xhigh=LeftX;
		xlow=RightX;
	}
	else
	{
		xhigh=RightX;
		xlow=LeftX;
	}
	L=(float)(log((float)(xhigh-xlow))/log((float)10));
	P=(int)floor(L);
	m=L-P;
	if (m<0.1) 
	{
		d1=(float)pow(10,P-1);
		d=2*d1;
	}
	else if (m<0.45) 
	{
		d1=(float)(0.1*pow(10,P));
		d=5*d1;
	} 
	else if (m<0.75) 
	{
		d1=(float)(0.5*pow(10,P));
		d=2*d1;
	}
	else
	{
		d1=(float)pow(10,P);
		d=2*d1;
	}
	if (m_LogX)
		x=(float)pow(10,(int)(log(xlow/100)/m_L10)); 
	else 
		x=d*floor(xlow/d)-d;
	while (x<=xhigh)
	{
		int ix=convx(x);
		if (ix>=m_ixb)
		{
			ost.str("");
			ost<< x;
			m_G.PlotString(ost.str().c_str(),ix,m_iyb+m_iyw+7,m_Img);
			Line(ix,ix,m_iyb+m_iyw,m_iyb+m_iyw-8,0);
			Line(ix,ix,m_iyb,m_iyb+8,0);
		}
		if (m_LogX)
			x*=10; 
		else 
			x+=d;
	}
	
	if (!m_LogX)
	{
		x=d1*floor(xlow/d1)-d1;
		while (x<=xhigh)
		{
			int ix=convx(x);
			if (ix>=m_ixb)
			{
				Line(ix,ix,m_iyb+m_iyw,m_iyb+m_iyw-4,0);
				Line(ix,ix,m_iyb,m_iyb+4,0);
			}
			x+=d1;
		}
	}
	m_G.SetFontAlignment(CGraphics::TOPRIGHT);
	if (TopY<BottomY)
	{
		yhigh=BottomY;
		ylow=TopY;
	}
	else
	{
		yhigh=TopY;
		ylow=BottomY;
	}
	L=log((float)(yhigh-ylow))/log((float)10);
	P=(int)floor(L);
	m=L-P;
	if (m<0.1) 
	{
		d=(float)(0.2*pow(10,P));
		d1=(float)(0.5*d);
	} 
	else if (m<0.45) 
	{
		d=(float)(0.5*pow(10,P));
		d1=(float)(0.2*d);
	} 
	else if (m<0.75) 
	{
		d=(float)pow(10,P);
		d1=(float)(0.5*d);
	} 
	else
	{
		d=(float)(2*pow(10,P));
		d1=(float)(0.5*d);
	}
	if (!LogY)
		y=d*floor(ylow/d)-d;
	else 
		y=(float)pow(10,(int)(log(ylow/100)/m_L10));
	while (y<=yhigh)
	{
		int iy=convy(y);
		if (iy<=m_iyb+m_iyw)
		{
			Line(m_ixb,m_ixb+8,iy,iy,0);
			Line(m_ixb+m_ixw,m_ixb+m_ixw-8,iy,iy,0);
			ost.str("");
			ost<< y;
			m_G.PlotString(ost.str().c_str(),m_ixb-2,iy-6,m_Img);
		}
		if (!LogY)
			y+=d; 
		else 
			y*=10;
	}
	
	if (!LogY)
	{
		y=d1*floor(ylow/d1)-d1;
		while (y<=yhigh)
		{
			int iy=convy(y);
			if (iy<=m_iyb+m_iyw)
			{
				Line(m_ixb,m_ixb+4,convy(y),convy(y),0);
				Line(m_ixb+m_ixw,m_ixb+m_ixw-4,convy(y),convy(y),0);
			}
			y+=d1;
		}
	}
	return true;
}

CPoint2D<float> CGraphPlot2D::GetUserPos(const CPoint2D<ipl::INT16>& P) const
{
	CPoint2D<float> P0(0,0);
	if (!m_Initiated) 
	{
		ostringstream ost;
		ost << "CGraphPlot2D::GetUserPos() Not initialized, call InitPlot()" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return P0;
	}
	if (m_LogX)
		P0.SetX(pow((float)10,(float)((P.GetX()-m_cx0+0.5)/m_cx1))); 
	else 
		P0.SetX((P.GetX()-m_cx0)/m_cx1);
	if (m_LogY)
		P0.SetY(pow((float)10,(float)((P.GetY()-m_cy0+0.5)/m_cy1))); 
	else 
		P0.SetY((P.GetY()-m_cy0)/m_cy1);
	return P0;
}

CPoint2D<ipl::INT16> CGraphPlot2D::GetUserPos(const CPoint2D<float>& P) const
{
	CPoint2D<ipl::INT16> P0(0,0);
	if (!m_Initiated) 
	{
		ostringstream ost;
		ost << "CGraphPlot2D::GetUserPos() Not initialized, call InitPlot()" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return P0;
	}
	P0.Set(convx(P.GetX()),convy(P.GetY()));
	return P0;
}

} // end namespace ipl
