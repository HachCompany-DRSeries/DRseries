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

#include "equation_solver.h"

namespace ipl{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CEquationSolver::CEquationSolver()
{
}

CEquationSolver::~CEquationSolver()
{
}


vector<float> CEquationSolver::LinearSolve(CArray2D<float>& A, vector<float>& b)
{
	if (A.GetWidth()!=A.GetHeight())
	{
		ostringstream ost;
		ost << "CEquationSolver::LinearSolve() Matrix A in Ax=b is not quadratic" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		vector<float> v(A.GetWidth());
		return v;
	}
	if (A.GetWidth()!=b.size())
	{
		ostringstream ost;
		ost << "CEquationSolver::LinearSolve() Vector b in Ax=b is not same dimension as A" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		vector<float> v(A.GetWidth());
		return v;
	}
	
	vector<unsigned int> index; // changed from int to unsigned int by edr
	LUDecomp(A,index);
	return LUSolve(A,index,b);
}

vector<double> CEquationSolver::LinearOverDeterminedSolve(CArray2D<double>& A, vector<double>& b)
{
	if (A.GetWidth()<A.GetHeight())
	{
		ostringstream ost;
		ost << "CEquationSolver::LinearOverDeterminedSolve() System is under determined!" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		vector<double> v(A.GetWidth());
		return v;
	}
	if (A.GetWidth()!=b.size())
	{
		ostringstream ost;
		ost << "CEquationSolver::LinearOverDeterminedSolve() Vector b in Ax=b is not same size as "
			" number of equations in A!" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		vector<double> v(A.GetWidth());
		return v;
	}
	unsigned int TotalUnknowns=A.GetHeight();
	unsigned int TotalEqs=A.GetWidth();
	vector<double> res(TotalUnknowns);

	unsigned int j,k,m,temp;
	int i;
	CArray2D<double> a(TotalUnknowns,TotalUnknowns);
	vector<double> Right(TotalUnknowns);
	double sum,max, buf;
	for (i = 0; i < (int)TotalUnknowns; i++)
	{
		for (j=0;j<TotalUnknowns;j++)
		{
			sum=0;
			for (k=0;k<TotalEqs;k++)
			{
				sum=sum+A[k][i]*A[k][j];
			}
			a[i][j]=sum;
		}
	}
	for (i = 0; i < (int)TotalUnknowns; i++)
	{
		sum=0;
		for (k=0;k<TotalEqs;k++)
			sum=sum+A[k][i]*b[k];
		Right[i]=sum;
	}
	vector<unsigned int> r(TotalUnknowns);
	for(i = 0; i < (int)TotalUnknowns; i++)
	{
		r[i]=i;
	}
	for(k=0;k<TotalUnknowns-1;k++)
	{
		max=0;
		for (i = k; i < (int)TotalUnknowns; i++)
		{
			if (fabs(a[r[i]][k])>max)
			{
				max=a[r[i]][k];
				m=i;
			}
		}
		if (m>k)
		{
			temp=r[k];
			r[k]=r[m];
			r[m]=temp;
		}
		for (i = k + 1; i < (int)TotalUnknowns; i++)
		{
			buf=a[r[i]][k];
			if (buf!=0)
			{
				buf=buf/a[r[k]][k];
				for (j=k+1;j<TotalUnknowns;j++){
					a[r[i]][j]-=buf*a[r[k]][j];
				}
				Right[r[i]]-=buf*Right[r[k]];
			}
		}
	}
	res[TotalUnknowns-1]=Right[r[TotalUnknowns-1]]/a[r[TotalUnknowns-1]][TotalUnknowns-1];
	for (i=TotalUnknowns-2;i>=0;i--)
	{
		buf=Right[r[i]];
		for (j=i+1;j<TotalUnknowns;j++){
			buf-=a[r[i]][j]*res[j];
		}
		res[i]=buf/a[r[i]][i];
	}
	return res;
}

vector<float> CEquationSolver::LinearOverDeterminedSolve1(vector<vector <float> >& A, vector<float>& b)
{
	if (A[0].size()<A.size())
	{
		ostringstream ost;
		ost << "CEquationSolver::LinearOverDeterminedSolve() System is under determined!" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		vector<float> v(A.size());
		return v;
	}
	if (A[0].size()!=b.size())
	{
		ostringstream ost;
		ost << "CEquationSolver::LinearOverDeterminedSolve() Vector b in Ax=b is not same size as "
			" number of equations in A!" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		vector<float> v(A.size());
		return v;
	}
	std::vector<float>::iterator iti,itj,itjEnd,itk,itkEnd;
	unsigned int TotalUnknowns=A.size();
	unsigned int TotalEqs=A[0].size();
	vector<float> res(TotalUnknowns);
	unsigned int j,k,m;
	unsigned int i;
	vector<vector <float> >a(TotalUnknowns);
	for (i=0;i<TotalUnknowns;i++)
		a[i].resize(TotalUnknowns);
	vector<float> Right(TotalUnknowns);
	float sum,max, buf;
	for (i = 0; i < TotalUnknowns; i++)
	{
		for (j=0;j<TotalUnknowns;j++)
		{
			sum=0;
			iti=A[i].begin();
			itjEnd=A[j].end();
			itj=A[j].begin();
			while (itj<itjEnd)
				sum+=(*iti++)*(*itj++);
			a[i][j]=sum;
		}
	}
	for (i = 0; i < TotalUnknowns; i++)
	{
		sum=0;
		for (k=0;k<TotalEqs;k++)
			sum+=A[i][k]*b[k];
		Right[i]=sum;
	}
	vector<float> bufAr;
	for(k=0;k<TotalUnknowns-1;k++)
	{
		max=0;
		for (i = k; i < TotalUnknowns; i++)
		{
			if (fabs(a[i][k])>max)
			{
				max=a[i][k];
				m=i;
			}
		}
		if (m>k)
		{
			bufAr=a[k];
			a[k]=a[m];
			a[m]=bufAr;
			buf=Right[k];
			Right[k]=Right[m];
			Right[m]=buf;
		}
		for (i = k + 1; i < TotalUnknowns; i++)
		{
			buf=a[i][k];
			if (buf!=0)
			{
				itk=a[k].begin()+k;
				iti=a[i].begin()+k;
				itkEnd=a[k].end()-1;
				buf=buf/(*itk);a[k][k];
				while (itk<itkEnd)
					(*(++iti))-=buf*(*(++itk));
				Right[i]-=buf*Right[k];
			}
		}
	}
	res[TotalUnknowns-1]=Right[TotalUnknowns-1]/a[TotalUnknowns-1][TotalUnknowns-1];
	for (i=TotalUnknowns-2;i>=0;i--)
	{
		buf=Right[i];
		for (j=i+1;j<TotalUnknowns;j++){
			buf-=a[i][j]*res[j];
		}
		res[i]=buf/a[i][i];
	}
	return res;
}


vector<float> CEquationSolver::CholeskyLinearSolve(CArray2D<float>& A, vector<float>& b)
{
	if (A.GetWidth()!=A.GetHeight())
	{
		ostringstream ost;
		ost << "CEquationSolver::CholeskyLinearSolve() Matrix A in Ax=b is not quadratic" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		vector<float> v(A.GetWidth());
		return v;
	}
	if (A.GetWidth()!=b.size())
	{
		ostringstream ost;
		ost << "CEquationSolver::CholeskyLinearSolve() Vector b in Ax=b is not same dimension as A" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		vector<float> v(A.GetWidth());
		return v;
	}
	int n = b.size();
	vector<float> p, x;
	p.resize(n);
	x.resize(n);
	CholeskyDecomp(A, p);
	return CholeskySolve(A,p,b,x);
}


void CEquationSolver::LUDecomp(CArray2D<float>& A, vector<unsigned int>& index)
{
	if (A.GetWidth()!=A.GetHeight())
	{
		ostringstream ost;
		ost << "CEquationSolver::LUDecomp() Matrix A is not quadratic" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return;
	}
	int i, imax = 0, j, k;
	int n = A.GetHeight();
	double big, dum, sum, temp;
	vector<float> vv; vv.resize(n);
	index.resize(n);
	
	for (i=0; i<n; i++) index[i] = i;
	
	for (i=0; i<n; i++) {
		big = 0.0;
		for (j=0; j<n; j++) {
			if ((temp = fabs(A[i][j])) > big) big = temp;
		}
		if (big == 0.0)
		{
			ostringstream ost;
			ost << "CEquationSolver::LUDecomp() Singular matrix in LU decompisition" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			return;
		}
		vv[i] = (float)(1.0 / big);
	}
	for (j=0; j<n; j++) {
		for (i=0; i<j; i++) {
			sum = A[i][j];
			for (k=0; k<i; k++) sum -= A[i][k] * A[k][j];
			A[i][j] = (float)sum;
		}
		big = 0.0;
		for (i=j; i<n; i++) {
			sum = A[i][j];
			for (k=0; k<j; k++) sum -= A[i][k] * A[k][j];
			A[i][j] = (float)sum;
			if ((dum = vv[i] * fabs(sum)) >= big) {
				big = dum;
				imax = i;
			}
		}
		if (j != imax) { 
			for (k=0; k<n; k++) {
				dum = A[imax][k];
				A[imax][k] = A[j][k];
				A[j][k] = (float)dum;
			}
			vv[imax] = vv[j];
		}
		index[j] = imax;
		if (A[j][j] == 0.0) A[j][j] = (float) 1.0e-20;
		if (j != n-1) {
			dum = 1.0/(A[j][j]);
			for (i=j+1; i<n; i++) A[i][j] *= (float)dum;
		}
	}
}     


vector<float> CEquationSolver::LUSolve(const CArray2D<float>& A, const vector<unsigned int>& index, 
									   vector<float>& b)
{
	if (A.GetWidth()!=A.GetHeight())
	{
		ostringstream ost;
		ost << "CEquationSolver::LUSolve() Matrix A is not quadratic" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		vector<float> v(A.GetWidth());
		return v;
	}
	if (A.GetWidth()!=b.size())
	{
		ostringstream ost;
		ost << "CEquationSolver::LUSolve() Vector b is not same dimension as A" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		vector<float> v(A.GetWidth());
		return v;
	}
	if (A.GetWidth()!=index.size())
	{
		ostringstream ost;
		ost << "CEquationSolver::LUSolve() Vector index is not same dimension as A" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		vector<float> v(A.GetWidth());
		return v;
	}
	int i, ii=0, ip, j;
	int n = A.GetHeight();
	float sum;
	
	// Forward substitution.
	for (i=0; i<n; i++)
	{
		ip = index[i];
		sum = b[ip];
		b[ip] = b[i];
		if (ii == 0) 
			for (j=ii; j<=i-1; j++) sum -= A[i][j] * b[j];
			else if (sum == 0.0f) ii = i;
			b[i] = sum;
	}
	
	// Backward substitution.
	for (i=n-1; i>=0; i--)
	{
		sum = b[i];
		for (j = i+1; j<n; j++) 
			sum -= A[i][j] * b[j];
		b[i] = sum / A[i][i];
	}
	return b;
}


void CEquationSolver::CholeskyDecomp(CArray2D<float>& A, vector<float>& p)
{
	if (A.GetWidth()!=A.GetHeight())
	{
		ostringstream ost;
		ost << "CEquationSolver::CholeskyDecomp() Matrix A is not quadratic" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return;
	}
	if (A.GetWidth()!=p.size())
	{
		ostringstream ost;
		ost << "CEquationSolver::CholeskyDecomp() Vector p is not same dimension as A" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return;
	}
	int i, j, k;
	double sum;
	int n = p.size();
	
	for (i=0; i<n; i++) {
		for (j=i; j<n; j++) {
			for (sum = A[i][j], k=i-1; k>=0; k--) {
				sum -= A[i][k] * A[j][k];
			}
			if (i == j) 
			{
				if (sum <= 0.0) 
				{
					ostringstream ost;
					ost << "CEquationSolver::CholeskyDecomp() Decomposition failed" << IPLAddFileAndLine;
					CError::ShowMessage(IPL_ERROR,ost);
					return;
				}
				p[i] = (float) sqrt(sum);
			} 
			else 
			{
				A[j][i] = (float)(sum / p[i]);
			}
		}
	}
}


vector<float> CEquationSolver::CholeskySolve(const CArray2D<float>& A, const vector<float>& p,
                                             const vector<float>& b, vector<float>& x)
{
	if (A.GetWidth()!=A.GetHeight())
	{
		ostringstream ost;
		ost << "CEquationSolver::CholeskySolve() Matrix A is not quadratic" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		vector<float> v(A.GetWidth());
		return v;
	}
	if (A.GetWidth()!=p.size())
	{
		ostringstream ost;
		ost << "CEquationSolver::CholeskySolve() Vector p is not same dimension as A" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		vector<float> v(A.GetWidth());
		return v;
	}
	int i, k;
	double sum;
	int n=p.size();
	
	for (i=0; i<n; i++) {
		for (sum = b[i], k=i-1; k>=0; k--) {
			sum -= A[i][k] * x[k];
		}
		x[i] = (float)(sum / p[i]);
	}
	for (i=n-1; i>=0; i--) {
		for (sum=x[i], k=i+1; k<n; k++) {
			sum -= A[k][i] * x[k];
		}
		x[i] = (float)(sum / p[i]);
	}
	return x;
}

} // end namespace ipl
